#pragma once

#include <iostream>
#include <functional>
#include <boost/graph/adjacency_list.hpp>

namespace spice
{
    using namespace boost;

    typedef property<edge_name_t, int> bgl_edge_property;
    typedef property<vertex_name_t, int, property<vertex_index_t, int>> bgl_vertex_property;

    typedef adjacency_list<vecS, vecS, undirectedS, bgl_vertex_property, bgl_edge_property> bgl_graph;
    typedef graph_traits<bgl_graph>::vertex_descriptor Vertex;

    namespace detail
    {
        inline bool isLeafSymbol(const std::string& symbol)
        {
            if(symbol == "pmos" || symbol == "nmos") return true;
            return false;
        }
        inline bool isBulkPort(const std::string& symbol)
        {
            if(symbol == "VCC") return true;
            if(symbol == "VDD") return true;
            if(symbol == "VSS") return true;
            if(symbol == "GND") return true;
            return false;
        }
    }

    inline std::function<bool (const std::string &)> isLeafSymbol = detail::isLeafSymbol;
}


