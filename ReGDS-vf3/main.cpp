#define VF3L
#include <chrono>
#ifndef STRESS_TEST_COUNT
#define STRESS_TEST_COUNT 1000
#endif
#include <fstream>
#include <thread>
#include <vector>
#include <atomic>
#include <fmt/core.h>
#include <numeric>          // for std::iota
#include <algorithm>        // for std::find, std::sort

// vf3lib
#include <ARGEdit.hpp>
#include <ARGraph.hpp>
#include <State.hpp>
#include <FastCheck.hpp>
#include <NodeClassifier.hpp>
#include <VF3NodeSorter.hpp>
#include <MatchingEngine.hpp>
#include "VFLib.h"

// your headers
#include "SPICE/Parser.hpp"
#include "Graph.hpp"
#include "bgl_vf2pp.hpp"
#include "opt_vf2pp.hpp"
#include <boost/range/iterator_range.hpp>

using namespace vflib;
using namespace spice;

spice::SubCktDefinitionMap subckt_definition_map;

// map vertex_name → gate‐class
int normalize_gate_type(int type) {
    switch (type) {
        case 0: return 0; // input pins
        case 1: return 1; // NAND2
        case 2: return 2; // NOR2
        case 3: return 3; // INV
        default: return -1;
    }
}

// minimal adjacency+label graph for VF3
struct SimpleGraph {
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int>              label;

    SimpleGraph(const bgl_graph& B) {
        using namespace boost;
        auto idx       = get(vertex_index, B);
        auto gate_type = get(vertex_name,  B);

        n     = num_vertices(B);
        adj   .assign(n,{});
        label .assign(n, 0);

        // labels
        for (auto v : make_iterator_range(vertices(B)))
            label[idx[v]] = normalize_gate_type(gate_type[v]);

        // edges
        for (auto e : make_iterator_range(edges(B))) {
            int u = idx[source(e,B)], v = idx[target(e,B)];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
};

struct CustomVF3State {
    const SimpleGraph &P, &T;
    std::vector<int> coreP, coreT, order;
    int depth = 0;

    CustomVF3State(const SimpleGraph &P_, const SimpleGraph &T_)
      : P(P_), T(T_), coreP(P_.n, -1), coreT(T_.n, -1), order(P_.n)
    { std::iota(order.begin(), order.end(), 0); }

    bool goal() const { return depth == P.n; }

    bool feasible(int u, int v) {
        if (T.label[v] < 0) return false;
        if (P.label[u] != T.label[v]) return false;
        for (int u2 : P.adj[u]) {
            int v2 = coreP[u2];
            if (v2 >= 0) {
                auto &nbr = T.adj[v];
                if (std::find(nbr.begin(), nbr.end(), v2) == nbr.end())
                    return false;
            }
        }
        return true;
    }

    bool match() {
        if (goal()) return true;
        int u = order[depth++];
        for (int v = 0; v < T.n; ++v) {
            if (coreT[v] >= 0) continue;
            if (!feasible(u,v)) continue;
            coreP[u] = v; coreT[v] = u;
            if (match()) return true;
            coreP[u] = coreT[v] = -1;
        }
        --depth;
        return false;
    }
};

void optimized_vf3_hrgen(Netlist& lib, Netlist& src) {
    // build src/lib graphs once
    std::vector<SimpleGraph> SG_src, SG_lib;
    for (auto &c : src.component_list) SG_src.emplace_back(*c.bgl_cgraph);
    for (auto &c : lib.component_list) SG_lib.emplace_back(*c.bgl_cgraph);

    auto t0 = std::chrono::high_resolution_clock::now();
    std::atomic<size_t> idx{0};
    size_t NT = std::thread::hardware_concurrency();
    std::vector<std::thread> thr; thr.reserve(NT);

    for (size_t t = 0; t < NT; ++t) {
        thr.emplace_back([&](){
            size_t i;
            while ((i = idx++) < SG_src.size()) {
                auto &Tg    = SG_src[i];
                auto &compT = src.component_list[i];
                bool progress = true;
                auto start = std::chrono::high_resolution_clock::now();

                while (progress) {
                    progress = false;
                    // timeout 10s per component
                    if (std::chrono::duration<double>(
                        std::chrono::high_resolution_clock::now() - start
                    ).count() > 10.0) {
                        fmt::println("Timeout on {}", compT.name);
                        break;
                    }

                    for (size_t j = 0; j < SG_lib.size(); ++j) {
                        auto &Pg    = SG_lib[j];
                        auto &compP = lib.component_list[j];

                        CustomVF3State st(Pg, Tg);
                        if (!st.match()) continue;

                        fmt::println("Matched “{}” → “{}”", compP.name, compT.name);
                        progress = true;

                        auto inst = compP.make_instance(compT, subckt_definition_map);
                        // **SWAPPED** u=lib‐index, v=src‐index
                        for (int u = 0; u < Pg.n; ++u) {
                            int v = st.coreP[u];
                            if (v >= 0)
                                compT.map_mosfet(u, v, compP, inst);
                        }
                        compT.add_subckt_instance(inst);

                        // remove matched nodes from Tg
                        for (int u = 0; u < Pg.n; ++u) {
                            int v = st.coreP[u];
                            if (v < 0) continue;
                            Tg.label[v] = -1;
                            for (int w : Tg.adj[v]) {
                                auto &nbr = Tg.adj[w];
                                nbr.erase(std::remove(nbr.begin(),nbr.end(),v),
                                          nbr.end());
                            }
                            Tg.adj[v].clear();
                        }

                        break;  // restart matching on this Tg
                    }
                }
            }
        });
    }
    for (auto &th : thr) th.join();

    auto t1 = std::chrono::high_resolution_clock::now();
    fmt::println("VF3 time: {:.3f} ms",
                 std::chrono::duration<double,std::milli>(t1 - t0).count());

    // final Verilog out
    std::ofstream out("port_out.v");
    for (auto &c : src.component_list)
        c.repr(out, subckt_definition_map);
}

int main() {
    auto T0 = std::chrono::high_resolution_clock::now();

    // pick your example:
    // test1:
    // Netlist src("examples/test1.sp", subckt_definition_map);
    // c1355:
    Netlist src("examples/c1355_net_ser_ft.sp", subckt_definition_map);
    src.parse();
    Netlist lib("examples/c1355_lib.sp", subckt_definition_map); // test1.lib or c1355.sp
    lib.parse();
    src.link();

    // **match smallest first** → puts NAND2 before NAND3
    lib.standard_sort();

    optimized_vf3_hrgen(lib, src);

    auto T1 = std::chrono::high_resolution_clock::now();
    fmt::println("Total end‐to‐end: {:.3f}s",
        std::chrono::duration<double>(T1 - T0).count() / STRESS_TEST_COUNT);

    return 0;
}