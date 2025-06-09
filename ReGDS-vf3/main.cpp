#ifndef STRESS_TEST_COUNT
#define STRESS_TEST_COUNT 1000
#endif
#define VF3L

#include <chrono>
#include <fstream>
#include <thread>
#include <vector>
#include <atomic>
#include <fmt/core.h>
#include <numeric>
#include <algorithm>
#include <boost/range/iterator_range.hpp>

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

using namespace vflib;
using namespace spice;

// global
spice::SubCktDefinitionMap subckt_definition_map;

// normalize raw BGL vertex_name → matching label
int normalize_gate_type(int raw) {
    switch (raw) {
      case 0: return 0;   // input pins
      case 1: return 1;   // NAND2
      case 2: return 2;   // NOR2
      case 3: return 3;   // INV
      default: return -1;
    }
}

// adjacency + normalized labels
struct SimpleGraph {
    int n;                                   // # of FETs only
    std::vector<std::vector<int>> adj;
    std::vector<int>              label;

    SimpleGraph(const bgl_graph& B, bool logLabels = false) {
        using namespace boost;
        auto idx = get(vertex_index, B);
        auto vt  = get(vertex_name,  B);

        // 1) First pass: build a map from old‐indices → new‐indices,
        //    but *only* for raw > 0 (i.e. transistor nodes).
        int nextId = 0;
        std::vector<int> old2new(num_vertices(B), -1);
        for (auto v : make_iterator_range(vertices(B))) {
            int raw = vt[v];
            if (raw > 0) {
                old2new[idx[v]] = nextId++;
            }
        }
        n = nextId;
        adj .assign(n,{});
        label.assign(n,0);

        // 2) Second pass: fill in label[] and adjacency among those new ids
        for (auto v : make_iterator_range(vertices(B))) {
            int o = idx[v];
            int ni = old2new[o];
            if (ni < 0) continue;               // skip nets (raw == 0)

            int raw = vt[v];
            int norm = normalize_gate_type(raw);
            label[ni] = norm;
            if (logLabels) {
                fmt::println("  [FET {:>2}] raw={:<2} norm={}", ni, raw, norm);
            }

            // now walk its edges, but only connect to other FETs
            for (auto e : make_iterator_range(out_edges(v, B))) {
                int w_old = idx[target(e,B)];
                int w_new = old2new[w_old];
                if (w_new >= 0) {
                    adj[ni].push_back(w_new);
                }
            }
        }
    }
};


// backtracking state for VF-3
struct CustomVF3State {
    const SimpleGraph &P, &T;
    std::vector<int> coreP, coreT, order;
    int depth = 0;

    CustomVF3State(const SimpleGraph& P_, const SimpleGraph& T_)
      : P(P_), T(T_), coreP(P_.n, -1), coreT(T_.n, -1), order(P_.n)
    { std::iota(order.begin(), order.end(), 0); }

    bool goal() const { return depth == P.n; }

    bool feasible(int u, int v) const {
        if (T.label[v] < 0 || P.label[u] != T.label[v]) return false;
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
            if (!feasible(u, v)) continue;
            coreP[u] = v; coreT[v] = u;
            if (match()) return true;
            coreP[u] = coreT[v] = -1;
        }
        --depth;
        return false;
    }
};

void optimized_vf3_hrgen(Netlist& lib, Netlist& src) {
    // 1) Build & log source graphs
    fmt::println("=== Source graphs ===");
    std::vector<SimpleGraph> SG_src;
    for (auto &c : src.component_list) {
        fmt::println("Component “{}”:", c.name);
        SG_src.emplace_back(*c.bgl_cgraph, /*logLabels=*/true);
    }

    // 2) Build & log library graphs
    fmt::println("\n=== Library graphs & sizes ===");
    std::vector<SimpleGraph> SG_lib;
    for (auto &c : lib.component_list) {
        SG_lib.emplace_back(*c.bgl_cgraph, /*logLabels=*/true);
    }
    for (size_t j = 0; j < SG_lib.size(); ++j) {
        fmt::println("Pattern “{}” has {} vertices",
                     lib.component_list[j].name,
                     SG_lib[j].n);
    }

    // 3) Split into big (>1) and small (==1)
    struct LibEntry { SimpleGraph *G; SubCkt* C; size_t sz; };
    std::vector<LibEntry> big, small;
    for (size_t j = 0; j < SG_lib.size(); ++j) {
        size_t sz = SG_lib[j].n;
        if (sz > 1) big.push_back({&SG_lib[j], &lib.component_list[j], sz});
        else        small.push_back({&SG_lib[j], &lib.component_list[j], sz});
    }

    // 4) Sort big *descending* so largest patterns win first
    std::sort(big.begin(), big.end(),
              [&](auto &a, auto &b){ return a.sz > b.sz; });

    // 5) Multithreaded matching
    auto t0 = std::chrono::high_resolution_clock::now();
    std::atomic<size_t> idx{0};
    size_t NT = std::thread::hardware_concurrency();
    std::vector<std::thread> thr; thr.reserve(NT);

    for (size_t t = 0; t < NT; ++t) {
        thr.emplace_back([&](){
            size_t i;
            while ((i = idx++) < SG_src.size()) {
                // work on a fresh copy
                SimpleGraph Tg = SG_src[i];
                auto &compT = src.component_list[i];

                // PHASE 1: big patterns
                bool progress = true;
                while (progress) {
                    progress = false;
                    for (auto &ent : big) {
                        CustomVF3State st(*ent.G, Tg);
                        if (!st.match()) continue;
                        fmt::println("Matched BIG “{}” → {}", ent.C->name, compT.name);
                        progress = true;

                        auto inst = ent.C->make_instance(compT, subckt_definition_map);
                        for (int u = 0; u < ent.G->n; ++u) {
                            int v = st.coreP[u];
                            if (v >= 0)
                                compT.map_mosfet(u, v, *ent.C, inst);
                        }
                        compT.add_subckt_instance(inst);

                        // remove matched vertices
                        for (int u = 0; u < ent.G->n; ++u) {
                            int v = st.coreP[u];
                            if (v < 0) continue;
                            Tg.label[v] = -1;
                            for (int w : Tg.adj[v]) {
                                auto &nbr = Tg.adj[w];
                                nbr.erase(std::remove(nbr.begin(), nbr.end(), v), nbr.end());
                            }
                            Tg.adj[v].clear();
                        }
                        break;  // restart big loop
                    }
                }

                // PHASE 2: small patterns
                for (auto &ent : small) {
                    bool did;
                    do {
                        CustomVF3State st(*ent.G, Tg);
                        if (!st.match()) break;
                        fmt::println("Matched SMALL “{}” → {}", ent.C->name, compT.name);

                        auto inst = ent.C->make_instance(compT, subckt_definition_map);
                        int v = st.coreP[0];
                        compT.map_mosfet(0, v, *ent.C, inst);
                        compT.add_subckt_instance(inst);

                        Tg.label[v] = -1;
                        for (int w : Tg.adj[v]) {
                            auto &nbr = Tg.adj[w];
                            nbr.erase(std::remove(nbr.begin(), nbr.end(), v), nbr.end());
                        }
                        Tg.adj[v].clear();
                        did = true;
                    } while (did);
                }
            }
        });
    }
    for (auto &th : thr) th.join();

    auto t1 = std::chrono::high_resolution_clock::now();
    fmt::println("VF3 matching time: {:.3f} ms",
        std::chrono::duration<double,std::milli>(t1 - t0).count());

    // 6) Emit Verilog
    std::ofstream out("port_out.v");
    for (auto &c : src.component_list)
        c.repr(out, subckt_definition_map);
}

int main(){
    auto T0 = std::chrono::high_resolution_clock::now();

    Netlist src("examples/c1355_net_ser_ft.sp", subckt_definition_map);
    src.parse();
    Netlist lib("examples/c1355_lib.sp", subckt_definition_map);
    lib.parse();
    src.link();
    lib.standard_reverse_sort();  // still orders lib.component_list

    optimized_vf3_hrgen(lib, src);

    auto T1 = std::chrono::high_resolution_clock::now();
    fmt::println("Total end‐to‐end per iter: {:.3f}s",
      std::chrono::duration<double>(T1 - T0).count()/STRESS_TEST_COUNT);

    return 0;
}
