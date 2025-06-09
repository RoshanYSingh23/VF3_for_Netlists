#pragma once
#include <fmt/core.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "includes.hpp"
#include "../utils.hpp"

namespace spice
{
    struct mosfet
    {
        std::string ds0, ds1;
        std::string gate;
        int type;
        std::string name;
        bool bulk = false;
        Vertex fet_v;
    };

    struct SubCktDefinition
    {
        std::string name;
        std::vector<std::string> pinout;
    };
    typedef std::unordered_map<std::string, SubCktDefinition> SubCktDefinitionMap;

    struct SubCktInstance
    {
        // store the symbol name of the subckt so that we can initialize the pointer later on
        std::string subckt_symbol;
        SubCktDefinition* subckt;
        SubCktDefinition* mapped_subckt;
        std::unordered_map<size_t, std::string> port_map;
    };

    struct SubCkt
    {
        std::string name;
        std::vector<std::string> pinout;

        // graphs of the component
        bgl_graph* bgl_cgraph;

        std::vector<Vertex> bgl_vertex_array;

        size_t label_count;
        std::vector<std::string> labels;
        std::unordered_map<std::string, size_t> labelID;

        std::vector<mosfet> fets;
        std::vector<SubCktInstance> subckts;

        // SubCkt name is the last ID in a subckt instantiation
        void parse_component(std::string& line);

        inline int get_dci_weight(size_t i, size_t j)
        {
            int weight = 0;

            if (fets[i].gate == fets[j].gate)
                weight += 32;
            
            if (fets[i].ds0 == fets[j].gate)
                weight += 2;
            if (fets[i].ds1 == fets[j].gate)
                weight += 2;
            if (fets[i].gate == fets[j].ds0)
                weight += 2;
            if (fets[i].gate == fets[j].ds1)
                weight += 2;
            
            if (fets[i].ds0 == fets[j].ds0)
                weight += 8;
            if (fets[i].ds0 == fets[j].ds1)
                weight += 8;
            if (fets[i].ds1 == fets[j].ds0)
                weight += 8;
            if (fets[i].ds1 == fets[j].ds1)
                weight += 8;
            
            if (fets[i].bulk && fets[j].bulk && weight > 0)
                weight += 1;
            
            return weight;
        }

        // set up subckt instance pointers
        void link(SubCktDefinitionMap& def_array);

        void construct_graph();

        void parse(std::ifstream& file, std::string definition);
        
        void parse_mosfet(std::vector<std::string>& vline);

        void repr(std::ostream& out, SubCktDefinitionMap& def_array);

        SubCktDefinition get_definition() const;

        // make an instance of this subckt
        SubCktInstance make_instance(SubCkt& source_subckt, SubCktDefinitionMap& def_array);
        void map_mosfet(size_t lib_mosfet, size_t mapped_mosfet, SubCkt& lib_subckt, SubCktInstance& inst);
        void add_subckt_instance(const SubCktInstance& value);

        SubCkt();
        SubCkt(std::ifstream& file, std::string definition);
    };

    // Converts netlist into Boost graph
    struct Netlist
    {
        std::ifstream file;

        std::vector<SubCkt> component_list;
        SubCktDefinitionMap& subcktdef_array;

        Netlist(const std::string& filename, SubCktDefinitionMap& def_array);

        // Sorts the components by size(asc)
        void standard_sort();

        // Sorts the components by size(desc)
        void standard_reverse_sort();

        void parse();

        void link();
        
        ~Netlist(){;}
    };
}

