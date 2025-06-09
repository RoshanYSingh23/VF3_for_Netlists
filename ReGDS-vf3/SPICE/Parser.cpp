
#include "Parser.hpp"

#include <algorithm>
#include <fmt/ranges.h>

namespace spice
{
    void SubCkt::parse_mosfet(std::vector<std::string>& vline)
    {
        mosfet mfet;
        mfet.name = vline[0];

        for(size_t i = 1; i < 4; i++)
        {
            if(vline[i] == "VSS" || vline[i] == "VDD" || vline[i] == "GND" || vline[i] == "VCC")
            {
                mfet.bulk = true;
            }
        }

        mfet.ds0  = vline[1];
        mfet.gate = vline[2];
        mfet.ds1  = vline[3];

        if(vline[5] == "pmos") mfet.type = 0;
        else mfet.type = 1;

        fets.push_back(mfet);
    }
    
    // SubCkt name is the last ID in a subckt instantiation
    void SubCkt::parse_component(std::string& line)
    {
        std::vector<std::string> vline = split(line, ' ', true, std::vector<std::string>());
        
        size_t i = vline.size() - 1;
        for(; i >= 0 && vline[i].find('=') != std::string::npos; i--);

        if(isLeafSymbol(vline[i]))
        {
            parse_mosfet(vline);
        }
        else
        {
            SubCktInstance inst{ .subckt_symbol=vline[i] };

            for(size_t j = 0; j < i; j++)
            {
                auto& port = vline[j];
                // WARNING: assuming the order is same
                inst.port_map.insert({j, port});
            }

            subckts.push_back(inst);
        }
    }

    void SubCkt::link(SubCktDefinitionMap& def_array)
    {
        bool failed = false;
        std::string link_errors = "";
        for(auto& instance : subckts)
        {
            if(!def_array.contains(instance.subckt_symbol))
            {
                link_errors += fmt::format("Error: Subckt {} not defined!\n", instance.subckt_symbol);
                failed = true;
                continue;
            }

            instance.mapped_subckt = &def_array[instance.subckt_symbol];
            instance.subckt = &def_array[this->name];
        }

        if(failed)
        {
            fmt::print("{}", link_errors);
            // TODO: kill process
        }
    }

    inline void SubCkt::construct_graph()
    {
        bgl_cgraph = new bgl_graph(0);

        for(size_t i = 0; i < fets.size(); i++)
        {
            fets[i].fet_v = add_vertex(bgl_vertex_property(fets[i].type), *bgl_cgraph);
            bgl_vertex_array.push_back(fets[i].fet_v);
        }

        for(size_t i = 0; i < fets.size(); i++)
        {
            for(size_t j = i + 1; j < fets.size(); j++)
            {
                int w = get_dci_weight(i, j);
                if(w == 0) continue;
                
                add_edge(i, j, bgl_edge_property(w), *bgl_cgraph);
            }
        }
    }

    inline void SubCkt::parse(std::ifstream& file, std::string definition)
    {
        std::vector<std::string> sdef = split(definition, ' ', true, std::vector<std::string>());

        name = sdef[1];

        // add pinouts
        for(size_t i = 2; i < sdef.size(); i++)
        {
            pinout.push_back(sdef[i]);
        }

        std::string line;
        while (std::getline(file, line))
        {
            if(line.starts_with(".ENDS")) break;
            if(line.empty()) continue;

            parse_component(line);

            //fets.push_back(mosfet{*this, line});
        }

        // construct the graphs
        construct_graph();
    }

    SubCktDefinition SubCkt::get_definition() const
    {
        SubCktDefinition subcktdef;
        subcktdef.name = this->name;
        subcktdef.pinout = pinout;
        return subcktdef;
    }
    SubCktInstance SubCkt::make_instance(SubCkt& source_subckt, SubCktDefinitionMap& def_array)
    {
        return SubCktInstance { .subckt=&def_array[source_subckt.name], .mapped_subckt=&def_array[name] };
    }
    void SubCkt::map_mosfet(size_t lib_mosfet, size_t mapped_mosfet, SubCkt& lib_subckt, SubCktInstance& inst)
    {
        spice::mosfet& flib = lib_subckt.fets[lib_mosfet];
        spice::mosfet& fsrc = fets[mapped_mosfet];

        if(fsrc.ds1 == "VDD" || fsrc.ds1 == "VSS" || fsrc.ds1 == "VCC" || fsrc.ds1 == "GND")
        {
            auto it = std::find(lib_subckt.pinout.begin(), lib_subckt.pinout.end(), flib.ds0);
            if(it != lib_subckt.pinout.end())
            {
                auto pinout_pos = it - lib_subckt.pinout.begin();
                if(!inst.port_map.contains(pinout_pos)) inst.port_map[pinout_pos] = fsrc.ds0;
            }
        }
        if(fsrc.ds1 == "VDD" || fsrc.ds1 == "VSS" || fsrc.ds1 == "VCC" || fsrc.ds1 == "GND")
        {
            auto it = std::find(lib_subckt.pinout.begin(), lib_subckt.pinout.end(), flib.ds1);
            if(it != lib_subckt.pinout.end())
            {
                auto pinout_pos = it - lib_subckt.pinout.begin();
                if(!inst.port_map.contains(pinout_pos)) inst.port_map[pinout_pos] = fsrc.ds1;
            }
        }
        {
            auto it = std::find(lib_subckt.pinout.begin(), lib_subckt.pinout.end(), flib.gate);
            if(it != lib_subckt.pinout.end())
            {
                auto pinout_pos = it - lib_subckt.pinout.begin();
                if(!inst.port_map.contains(pinout_pos)) inst.port_map[pinout_pos] = fsrc.gate;
            }
        }
    }
    void SubCkt::add_subckt_instance(const SubCktInstance& value)
    {
        subckts.push_back(value);
    }

    void SubCkt::repr(std::ostream& out, SubCktDefinitionMap& def_array)
    {
        out << fmt::format("module {}(inout {})\n", this->name, fmt::join(this->pinout, ", inout "));
        
        size_t component_id = 1;
        for(auto& instance : subckts)
        {
            std::vector<std::string> verilog_args;
            for(size_t i = 0; i < instance.mapped_subckt->pinout.size(); i++)
            {
                verilog_args.push_back(fmt::format(".{}({})", instance.mapped_subckt->pinout[i], instance.port_map[i]));
            }

            auto verilog_line = fmt::format("\t{} X{} ({});", instance.mapped_subckt->name, component_id, fmt::join(verilog_args, ", "));

            out << verilog_line << "\n";
            component_id++;
        }

        out << "endmodule\n";
    }

    SubCkt::SubCkt(){;}
    SubCkt::SubCkt(std::ifstream& file, std::string definition) { parse(file, definition); }

    Netlist::Netlist(const std::string& filename, SubCktDefinitionMap& def_array) : subcktdef_array(def_array)
    {
        file.open(filename);
    }

    // Sorts the components by size(asc)
    void Netlist::standard_sort()
    {
        auto compare = [&](const SubCkt& lhs, const SubCkt& rhs)
        {
            if(num_vertices(*lhs.bgl_cgraph) < num_vertices(*rhs.bgl_cgraph)) return true;
            else if(num_vertices(*lhs.bgl_cgraph) == num_vertices(*rhs.bgl_cgraph) && num_edges(*lhs.bgl_cgraph) < num_edges(*rhs.bgl_cgraph)) return true;
            return false;
        };

        std::sort(component_list.begin(), component_list.end(), compare);
    }

    // Sorts the components by size(desc)
    void Netlist::standard_reverse_sort()
    {
        auto compare = [&](const SubCkt& lhs, const SubCkt& rhs)
        {
            if(num_vertices(*lhs.bgl_cgraph) < num_vertices(*rhs.bgl_cgraph)) return true;
            else if(num_vertices(*lhs.bgl_cgraph) == num_vertices(*rhs.bgl_cgraph) && num_edges(*lhs.bgl_cgraph) < num_edges(*rhs.bgl_cgraph)) return true;
            return false;
        };

        std::sort(component_list.begin(), component_list.end(), compare);

        std::reverse(component_list.begin(), component_list.end());
    }

    void Netlist::parse()
    {
        std::string line;
        while(std::getline(file, line))
        {
            if(line.starts_with(".SUBCKT"))
            {
                component_list.push_back(SubCkt{file, line});
                subcktdef_array[component_list.back().name] = component_list.back().get_definition();
            }
        }
    }

    void Netlist::link()
    {
        for(auto& subckt : component_list)
        {
            subckt.link(subcktdef_array);
        }
    }
}

