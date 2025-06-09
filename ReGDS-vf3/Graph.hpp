#pragma once
#include <iostream>
#include <tuple>
#include <fmt/core.h>

#include <boost/graph/copy.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

namespace bgl
{
    namespace 
    {
        using namespace boost;
    }

    typedef property<edge_name_t, int> bgl_edge_property;
    typedef property<vertex_name_t, int, property<vertex_index_t, int>> bgl_vertex_property;

    typedef adjacency_list<vecS, vecS, undirectedS, bgl_vertex_property, bgl_edge_property> bgl_graph;
    typedef graph_traits<bgl_graph>::vertex_descriptor node;
    typedef graph_traits<bgl_graph>::edge_descriptor edge_id;

    typedef graph_traits<bgl_graph>::vertex_iterator bst_nodeIt;
    typedef graph_traits<bgl_graph>::edge_iterator bst_edgeIt;
    typedef graph_traits<bgl_graph>::adjacency_iterator bst_incedgeIt;
    typedef graph_traits<bgl_graph>::out_edge_iterator bst_outedgeIt;
    typedef graph_traits<bgl_graph>::out_edge_iterator bst_outedgeIt;

    typedef boost::property_map<bgl_graph,vertex_name_t>::type vertex_map;
    typedef boost::property_map<bgl_graph,edge_name_t>::type edge_map;

    struct NodeIt
    {
        bst_nodeIt id;
        bst_nodeIt end;

        inline NodeIt& operator++()
        {
            id++;
            return *this;
        }

        inline auto operator*() { return *id; }

        inline bool isValid() const { return id != end; }
    };

    struct EdgeIt
    {
        bst_edgeIt id;
        bst_edgeIt end;
        bool isInvalid;

        inline EdgeIt& operator++()
        {
            id++;
            return *this;
        }

        inline auto operator*() { return *id; }

        inline bool isValid() const { return (id != end) && !isInvalid; }
    };

    struct Graph
    {
        //  Prototype: NOT SAFE
        // TODO: Switch to smart pointers
        bgl_graph* gptr;

        Graph()
        {
            // 100% Memory Leak Guarantee
            gptr = new bgl_graph(0);
        }

        inline node push_node(const int& value) { return add_vertex(bgl_vertex_property(value), *gptr); }
        inline auto push_edge(node i, node j, const int& value) { return add_edge(i, j, bgl_edge_property(value), *gptr); }

        inline void erase_edge(edge_id edgeID) { remove_edge(edgeID, *gptr); }

        inline auto node_iterator()
        {
            NodeIt nIt;
            tie(nIt.id, nIt.end) = vertices(*gptr);
            return nIt;
        }

        inline auto node_iterator(const NodeIt& init)
        {
            NodeIt nIt;
            tie(nIt.id, nIt.end) = vertices(*gptr);
            nIt.id = init.id;
            return nIt;
        }

        inline auto node_iterator(node init)
        {
            NodeIt nIt;
            tie(nIt.id, nIt.end) = vertices(*gptr);
            for(; nIt.isValid(); ++nIt)
                if(*nIt == init)
                    break;
            return nIt;
        }

        inline auto all_edge_iterator()
        {
            EdgeIt nIt; nIt.isInvalid = false;
            tie(nIt.id, nIt.end) = edges(*gptr);
            return nIt;
        }

        inline node u(edge_id e)
        {
            return source(e, *gptr);
        }

        inline node v(edge_id e)
        {
            return target(e, *gptr);
        }

        inline node u(EdgeIt e)
        {
            return source(*e, *gptr);
        }
        
        inline node v(EdgeIt e)
        {
            return target(*e, *gptr);
        }

        inline size_t node_count() { return num_vertices(*gptr); }
        inline size_t edge_count() { return num_edges(*gptr); }

        inline node oppositeNode(node n, edge_id e)
        {
            if(n == source(e, *gptr))
                return target(e, *gptr);
            return source(e, *gptr);
        }

        inline void copy(const Graph& g)
        {
            copy_graph(*g.gptr, *gptr);
        }
        inline void copy(const bgl_graph& g)
        {
            copy_graph(g, *gptr);
        }

        inline void set(bgl_graph& g)
        {
            delete gptr;
            gptr = &g;
        }

        inline vertex_map get_node_map() { return get(vertex_name, *gptr); }
        inline edge_map get_edge_map() { return get(edge_name, *gptr); }

        struct IncEdgeIt
        {
            Graph* g;
            node start;
            bst_incedgeIt id;
            bst_incedgeIt end;
            bool isInvalid;

            inline IncEdgeIt& operator++()
            {
                id++;
                return *this;
            }

            inline auto operator*()
            {
                node v = *id;
                return edge(start, v, *g->gptr).first;
            }

            inline bool isValid() const { return (id != end) && !isInvalid; }
        };
        inline IncEdgeIt edge_iterator(node i)
        {
            IncEdgeIt nIt; 
            nIt.isInvalid = false; nIt.g = this;
            nIt.start = i;
            std::tie(nIt.id, nIt.end) = adjacent_vertices(i, *gptr);
            return nIt;
        }
        // WARNING: Assumes that e includes n
        inline node oppositeNode(node n, IncEdgeIt& e)
        {
            if(n == e.start)
                return *e.id;
            return e.start;
        }

        struct IncUEdgeIt
        {
            bst_outedgeIt id;
            bst_outedgeIt end;
            bool isInvalid;

            inline IncUEdgeIt& operator++()
            {
                id++;
                return *this;
            }

            inline auto operator*()
            {
                return *id;
            }

            inline bool isValid() const { return (id != end) && !isInvalid; }
        };
        inline IncUEdgeIt out_edge_iterator(node i)
        {
            IncUEdgeIt nIt;
            nIt.isInvalid = false;
            std::tie(nIt.id, nIt.end) = out_edges(i, *gptr);
            return nIt;
        }

        inline auto getEdge(node u, node v)
        {
            return edge(u, v, *gptr).first;
        }

        struct constIncUEdgeIt
        {
            bst_outedgeIt id;
            bst_outedgeIt end;
            bool isInvalid;

            inline constIncUEdgeIt& operator++()
            {
                id++;
                return *this;
            }

            inline auto operator*()
            {
                return *id;
            }

            inline bool isValid() const { return (id != end) && !isInvalid; }
        };
        inline constIncUEdgeIt const_out_edge_iterator(node i)
        {
            constIncUEdgeIt nIt;
            nIt.isInvalid = false;
            std::tie(nIt.id, nIt.end) = out_edges(i, *gptr);
            return nIt;
        }
    };

    typedef Graph::IncEdgeIt IncEdgeIt;
    typedef Graph::IncUEdgeIt IncUEdgeIt;

    inline IncEdgeIt invalidIncEdgeIt{ .isInvalid = true };

    template<typename T>
    struct graph_node_map : public std::vector<T>
    {
        Graph& g;

        graph_node_map(Graph& _g) : g(_g), std::vector<T>(_g.node_count()) { ; }
        graph_node_map(Graph& _g, const T& value) : g(_g), std::vector<T>(_g.node_count(), value) { ; }
    };

    template<typename T>
    struct graph_edge_map : public std::vector<T>
    {
        Graph& g;

        graph_edge_map(Graph& _g, const T& value) : g(_g), std::vector<T>(_g.edge_count(), value) { ; }
    };
}

