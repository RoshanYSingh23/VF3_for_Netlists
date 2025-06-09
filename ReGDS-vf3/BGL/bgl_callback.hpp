#pragma once
//#include <fmt/core.h>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "../SPICE/Parser.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

namespace bgl_cb
{
    using namespace boost;

    typedef property<edge_name_t, int> bgl_edge_property;
    typedef property<vertex_name_t, int, property<vertex_index_t, int>> bgl_vertex_property;

    typedef adjacency_list<vecS, vecS, undirectedS, bgl_vertex_property, bgl_edge_property> bgl_graph;
    typedef graph_traits<bgl_graph>::vertex_descriptor  Vertex;

    struct verilog_line
    {
        spice::SubCkt* std_cell;
        std::unordered_map<size_t, size_t> port_map;
    };

    struct subgraph_callback
    {
        const bgl_graph& g1;
        bgl_graph& g2;

        std::vector<Vertex>& bgl_vertex_array;

        size_t subgraph_count;

        subgraph_callback(const bgl_graph& g1, bgl_graph& g2, std::vector<Vertex>& vertex_array) : g1(g1), g2(g2), bgl_vertex_array(vertex_array) {;}

        template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
        bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) {
            size_t vindex;
            std::vector<size_t> nodeIDs;
            //fmt::println("Subgraph(VF2) Mapping:");
            BGL_FORALL_VERTICES_T(v, g1, bgl_graph)
            {
                vindex = boost::get(boost::vertex_index_t(), g1, boost::get(f, v));
                //fmt::println("[{}: {}]", v, bgl_vertex_array[vindex]);
                nodeIDs.push_back(vindex);
            }
            subgraph_count++;
            bool removed = false;

            for (size_t i=0; i < nodeIDs.size(); ++i)
            {
                for (size_t j=i+1; j < nodeIDs.size(); ++j)
                {
                    auto ePair = boost::edge(bgl_vertex_array[nodeIDs[i]], bgl_vertex_array[nodeIDs[j]], g2);
                    if (ePair.second)
                    {
                        boost::remove_edge(ePair.first, g2);
                        removed = true;
                    }
                }
            }

            if (!removed) return true;
            return true;
        }
    };

    struct subgraph_callback_sshot
    {
        const bgl_graph& g1;
        bgl_graph& g2;

        std::vector<Vertex>& bgl_vertex_array;
        std::vector<size_t> nodeIDs;

        size_t subgraph_count;

        subgraph_callback_sshot(const bgl_graph& g1, bgl_graph& g2, std::vector<Vertex>& vertex_array) : g1(g1), g2(g2), bgl_vertex_array(vertex_array) {;}

        template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
        bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) {
            size_t vindex = 0;
            //fmt::println("Subgraph(VF2) Mapping:");
            BGL_FORALL_VERTICES_T(v, g1, bgl_graph)
            {
                vindex = boost::get(boost::vertex_index_t(), g1, boost::get(f, v));
                //fmt::println("[{}: {}]", v, bgl_vertex_array[vindex]);
                nodeIDs.push_back(vindex);
            }
            return false;
        }
    };

    struct subgraph_callback_gen
    {
        const bgl_graph& g1;
        bgl_graph& g2;

        std::vector<Vertex>& bgl_vertex_array;
        std::vector<verilog_line>& verilog_code;

        spice::SubCkt& n1;
        spice::SubCkt& n2;

        size_t subgraph_count;

        subgraph_callback_gen(const bgl_graph& g1, bgl_graph& g2, std::vector<Vertex>& vertex_array, spice::SubCkt& _n1, spice::SubCkt& _n2,
        std::vector<verilog_line>& verilog) : g1(g1), g2(g2), bgl_vertex_array(vertex_array), verilog_code(verilog), n1(_n1), n2(_n2) {;}

        template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
        bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1)
        {
            size_t vindex;
            std::vector<size_t> nodeIDs;
            verilog_line line;

            line.std_cell = &n1;

            //fmt::println("Subgraph(VF2) Mapping:");
            BGL_FORALL_VERTICES_T(v, g1, bgl_graph)
            {
                vindex = boost::get(boost::vertex_index_t(), g1, boost::get(f, v));
                //fmt::println("\t[{}: {}]", v, bgl_vertex_array[vindex]);
                nodeIDs.push_back(vindex);
                /*
                spice::mosfet& flib = n1.fets[v];
                spice::mosfet& fsrc = n2.fets[vindex];

                if((fsrc.ds1 == n2.labelID["VDD"] || fsrc.ds1 == n2.labelID["VSS"] || fsrc.ds1 == n2.labelID["VCC"] || fsrc.ds1 == n2.labelID["GND"]) 
                    && std::find(n1.pinout.begin(), n1.pinout.end(), flib.ds0) != n1.pinout.end() && !line.port_map.contains(flib.ds0))
                {
                    line.port_map[flib.ds0] = fsrc.ds0;
                }
                if((fsrc.ds1 == n2.labelID["VDD"] || fsrc.ds1 == n2.labelID["VSS"] || fsrc.ds1 == n2.labelID["VCC"] || fsrc.ds1 == n2.labelID["GND"])
                    && std::find(n1.pinout.begin(), n1.pinout.end(), flib.ds1) != n1.pinout.end() && !line.port_map.contains(flib.ds1))
                {
                    line.port_map[flib.ds1] = fsrc.ds1;
                }
                if(std::find(n1.pinout.begin(), n1.pinout.end(), flib.gate) != n1.pinout.end() && !line.port_map.contains(flib.gate))
                {
                    line.port_map[flib.gate] = fsrc.gate;
                }
                */
            }
            subgraph_count++;
            bool removed = false;

            for (size_t i=0; i < nodeIDs.size(); ++i)
            {
                boost::clear_vertex(nodeIDs[i], g2);
                /*
                for (size_t j=i+1; j < nodeIDs.size(); ++j)
                {
                    auto ePair = boost::edge(nodeIDs[i], nodeIDs[j], g2);
                    if (ePair.second)
                    {
                        boost::remove_edge(ePair.first, g2);
                        removed = true;
                    }
                }
                */
            }

            verilog_code.push_back(line);

            if (!removed) return true;
            return true;
        }
    };
}

