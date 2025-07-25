#ifndef OPENFPGA_REPACK_TEMPLATE_H
#define OPENFPGA_REPACK_TEMPLATE_H

/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
#include "build_physical_truth_table.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "fabric_global_port_info.h"
#include "globals.h"
#include "read_xml_repack_design_constraints.h"
#include "repack.h"
#include "repack_design_constraints.h"
#include "tile_annotation.h"
#include "vtr_log.h"
#include "openfpga_port_parser.h"
#include "pcf_reader.h"
#include "write_xml_repack_design_constraints.h"

/* begin namespace openfpga */
namespace openfpga {

/** Generate repack design constraints from a base RDC file and a PCF file.
 * Pin assignments in the PCF file take priority. If none of the files are
 * present or some clock nets are left unassigned, this function assigns them to
 * the first available clock pin. */
static inline RepackDesignConstraints gen_repack_constraints(const RepackDesignConstraints* base_constraints,
                                                             const openfpga::PcfData* pcf_data,
                                                             const TileAnnotation& tile_annotation){
  auto& cluster_ctx = g_vpr_ctx.clustering();
  auto& atom_ctx = g_vpr_ctx.atom();
  auto& clb_nlist = cluster_ctx.clb_nlist;

  std::map<std::string, std::map<std::string, BasicPort>> global_clock_to_tile_ports;

  /* Find the global clock port and its connections to the tile ports */
  for (TileGlobalPortId global_port_id : tile_annotation.global_ports()) {
    if (!tile_annotation.global_port_is_clock(global_port_id))
      continue;

    auto global_port_name = tile_annotation.global_port_name(global_port_id);
    const auto& tile_names = tile_annotation.global_port_tile_names(global_port_id);
    const auto& tile_ports = tile_annotation.global_port_tile_ports(global_port_id);

    for (size_t i = 0; i < tile_names.size(); ++i) {
        global_clock_to_tile_ports[global_port_name][tile_names[i]] = tile_ports[i];
    }
  }

  /* Get clock pin - clock net connections for each "top level" pb type
   * looks like: {pb_type_name: {clock_port_name: {...clock_nets}}} */
  std::map<std::string, std::map<BasicPort, std::set<std::string>>> pb_and_port_to_clock_nets;

  for (auto block_id : clb_nlist.blocks()) {
    auto pb = clb_nlist.block_pb(block_id);
    auto pb_type = pb->pb_graph_node->pb_type;

    int port_index = 0;
    for (int i = 0; i < pb_type->num_ports; i++) {
      auto& port = pb_type->ports[i];
      if(!port.is_clock)
        continue;
      for (int j = 0; j < port.num_pins; j++) {
        auto& pin = pb->pb_graph_node->clock_pins[port_index][j];
        std::string port_name = std::string(port.name) + "[0:" + std::to_string(port.num_pins-1) + "]";
        openfpga::PortParser port_parser(port_name);
        int node_index = pin.pin_count_in_cluster;
        /* Check intrablock routing to see what atom net id is connected to this pin */
        if(pb->pb_route.count(node_index)){
          AtomNetId net_id = pb->pb_route[node_index].atom_net_id;
          std::string net_name = atom_ctx.netlist().net_name(net_id);
          pb_and_port_to_clock_nets[pb_type->name][port_parser.port()].insert(net_name);
        }
      }
      port_index++;
    }
  }

/*
  for(auto [pb_type_name, clock_ports]: pb_and_port_to_clock_nets){
    std::cout << "pb type " << pb_type_name << ":\n";
    for(auto [clock_port, clock_nets]: clock_ports){
      std::cout << "\tnets for clock port " << clock_port.to_verilog_string() << "\n";
      for(auto net: clock_nets){
        std::cout << net << " ";
      }
      std::cout << "\n";
    }
  }
*/

  /* Expand the clock port into its clock pins as strings for each pb type + port.
   * We will take out the pins which already got matched */
  std::map<std::string, std::map<BasicPort, std::set<std::string>>> pb_and_port_to_clock_pins;

  for(auto [pb_type_name, clock_ports]: pb_and_port_to_clock_nets){
    for(auto [clock_port, clock_nets]: clock_ports){
      for(size_t i=0; i<clock_port.get_width(); i++){
        std::string pin_name = clock_port.get_name() + "[" + std::to_string(i) + "]";
        pb_and_port_to_clock_pins[pb_type_name][clock_port].insert(pin_name);
      }
    }
  }

  /* Matches: pb_type + port + net -> pin */
  std::map<std::string, std::map<BasicPort, std::map<std::string, std::string>>> pb_and_port_to_matches;

  /* First match nets & pins from the pcf file. Remove the net and pin when matched */
  if(pcf_data){
    for (const PcfIoConstraintId& io_id : pcf_data->io_constraints()) {
      std::string net = pcf_data->io_net(io_id);
      BasicPort pin = pcf_data->io_pin(io_id);

      /* Check if this pin is a global pin */
      if(!global_clock_to_tile_ports.count(pin.get_name())){
        continue;
      }

      /* Match for each tile name & port (XXX: We assume tile name = top level pb type name) */
      for(auto [tile_name, tile_port]: global_clock_to_tile_ports[pin.get_name()]){
        if(!pb_and_port_to_clock_nets.count(tile_name) || !pb_and_port_to_clock_nets[tile_name].count(tile_port)){
          continue;
        }

        auto& clock_nets = pb_and_port_to_clock_nets[tile_name][tile_port];
        auto& clock_pins = pb_and_port_to_clock_pins[tile_name][tile_port];

        std::string pin_name = pin.get_name() + "[" + std::to_string(pin.get_lsb()) + "]";
        /* Finally, match the net and pin name */
        if(!clock_nets.count(net) || !clock_pins.count(pin_name)){
          continue;
        }

        auto& matches = pb_and_port_to_matches[tile_name][tile_port];
        matches[net] = pin_name;
        clock_nets.erase(net);
        clock_pins.erase(pin_name);
      }
    }
  }

  /* Then match nets & pins from the repack design constraints file */
  if(base_constraints){
    for (auto rdc_id: base_constraints->design_constraints()) {
      auto type = base_constraints->type(rdc_id);
      if(type != RepackDesignConstraints::PIN_ASSIGNMENT)
        continue;

      std::string pb_type = base_constraints->pb_type(rdc_id);
      std::string net = base_constraints->net(rdc_id);
      BasicPort pin = base_constraints->pin(rdc_id);

      if(!pb_and_port_to_clock_nets.count(pb_type)){
        continue;
      }

      auto& port_to_clock_nets = pb_and_port_to_clock_nets[pb_type];
      for(auto& [port, clock_nets]: port_to_clock_nets){
        if(port.get_name() != pin.get_name()) /* Pin not in this port */
          continue;

        auto& clock_pins = pb_and_port_to_clock_pins[pb_type][port];
        std::string pin_name = pin.get_name() + "[" + std::to_string(pin.get_lsb()) + "]";

        if(!clock_nets.count(net) || !clock_pins.count(pin_name))
          continue;

        auto& matches = pb_and_port_to_matches[pb_type][port];
        matches[net] = pin_name;
        clock_nets.erase(net);  /* Shouldn't be an issue since we aren't iterating over clock_nets */
        clock_pins.erase(pin_name);
      }
    }
  }

  /* Match the rest in an arbitrary way */
  for(auto [pb_type_name, clock_ports]: pb_and_port_to_clock_nets){
    for(auto [clock_port, clock_nets]: clock_ports){
      auto& clock_pins = pb_and_port_to_clock_pins[pb_type_name][clock_port];
      auto& matches = pb_and_port_to_matches[pb_type_name][clock_port];
      for(auto& net: clock_nets){
        VTR_ASSERT_MSG(!clock_pins.empty(), "Not enough clock pins for clocks in the design");
        auto first_available_pin = *clock_pins.begin();
        matches[net] = first_available_pin;
        clock_pins.erase(first_available_pin);
      }
    }
  }

  /* Build the constraints from the matches we made */
  RepackDesignConstraints out;
  for(auto [pb_type_name, clock_ports]: pb_and_port_to_matches){
    for(auto [clock_port, matches]: clock_ports){
      for(auto [net, pin]: matches){
        auto id = out.create_design_constraint(RepackDesignConstraints::PIN_ASSIGNMENT);
        out.set_pb_type(id, pb_type_name);
        out.set_net(id, net);
        out.set_pin(id, PortParser(pin).port());
      }
    }
  }

  /* If there is a base constraints file, add the non clock pin assignment parts from it.
   * The clock pin assignments should have been handled by this function */
  std::vector<RepackDesignConstraintId> extra_rdcs_from_file;
  if(base_constraints){
    for (auto rdc_id: base_constraints->design_constraints()) {
      auto type = base_constraints->type(rdc_id);
      if(type == RepackDesignConstraints::PIN_ASSIGNMENT){
        std::string pb_type = base_constraints->pb_type(rdc_id);
        std::string net = base_constraints->net(rdc_id);
        BasicPort pin = base_constraints->pin(rdc_id);

        /* We don't know this pb_type, so we probably didn't assign */
        if(!pb_and_port_to_matches.count(pb_type)){
          extra_rdcs_from_file.push_back(rdc_id);
          continue;
        }

        bool found_port = false;

        auto& port_to_clock_nets = pb_and_port_to_clock_nets[pb_type];
        for(auto [port, clock_nets]: port_to_clock_nets){
          if(port.get_name() == pin.get_name()){
            found_port = true;
            break;
          }
        }

        /* We don't know this port, so we probably didn't assign */
        if(!found_port)
          extra_rdcs_from_file.push_back(rdc_id);
      } else {
        /* Not a pin assignment */
          extra_rdcs_from_file.push_back(rdc_id);
      }
    }
  }

  for(auto rdc_id: extra_rdcs_from_file){
    auto id = out.create_design_constraint(base_constraints->type(rdc_id));
    out.set_pb_type(id, base_constraints->pb_type(rdc_id));
    out.set_net(id, base_constraints->net(rdc_id));
    out.set_pin(id, base_constraints->pin(rdc_id));
  }

  return out;
}

/********************************************************************
 * A wrapper function to call the fabric_verilog function of FPGA-Verilog
 *******************************************************************/
template <class T>
int repack_template(T& openfpga_ctx, const Command& cmd,
                    const CommandContext& cmd_context) {
  CommandOptionId opt_design_constraints = cmd.option("design_constraints");
  CommandOptionId opt_pcf = cmd.option("pcf");
  CommandOptionId opt_write_design_constraints = cmd.option("write_design_constraints");
  CommandOptionId opt_ignore_global_nets =
    cmd.option("ignore_global_nets_on_pins");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Load design constraints from file */
  RepackDesignConstraints base_repack_design_constraints;
  openfpga::PcfData pcf_data;
  bool found_rdc = false;
  bool found_pcf = false;

  if (true == cmd_context.option_enable(cmd, opt_design_constraints)) {
    std::string dc_fname =
      cmd_context.option_value(cmd, opt_design_constraints);
    VTR_ASSERT(false == dc_fname.empty());
    base_repack_design_constraints =
      read_xml_repack_design_constraints(dc_fname.c_str());
    found_rdc = true;
  }

  if (true == cmd_context.option_enable(cmd, opt_pcf)) {
    std::string pcf_fname =
      cmd_context.option_value(cmd, opt_pcf);
    VTR_ASSERT(false == pcf_fname.empty());
    openfpga::read_pcf(pcf_fname.c_str(), pcf_data);

    if (!pcf_data.validate()) {
      VTR_LOG_ERROR("PCF contains invalid I/O assignment!\n");
      return CMD_EXEC_FATAL_ERROR;
    }
    found_pcf = true;
  }

  RepackDesignConstraints rdc = gen_repack_constraints(
    found_rdc ? &base_repack_design_constraints : nullptr,
    found_pcf ? &pcf_data : nullptr,
    openfpga_ctx.arch().tile_annotations
  );

  if (true == cmd_context.option_enable(cmd, opt_write_design_constraints)) {
    std::string out_dc_fname =
      cmd_context.option_value(cmd, opt_write_design_constraints);
    VTR_ASSERT(false == out_dc_fname.empty());
    write_xml_repack_design_constraints(out_dc_fname.c_str(), rdc);
  }

  /* Setup repacker options */
  RepackOption options;
  options.set_design_constraints(rdc);
  options.set_ignore_global_nets_on_pins(
    cmd_context.option_value(cmd, opt_ignore_global_nets));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));

  if (!options.valid()) {
    VTR_LOG("Detected errors when parsing options!\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  pack_physical_pbs(g_vpr_ctx.device(), g_vpr_ctx.atom(),
                    g_vpr_ctx.clustering(),
                    openfpga_ctx.mutable_vpr_device_annotation(),
                    openfpga_ctx.mutable_vpr_clustering_annotation(),
                    openfpga_ctx.vpr_bitstream_annotation(),
                    openfpga_ctx.arch().circuit_lib, options);

  build_physical_lut_truth_tables(
    openfpga_ctx.mutable_vpr_clustering_annotation(), g_vpr_ctx.atom(),
    g_vpr_ctx.clustering(), openfpga_ctx.vpr_device_annotation(),
    openfpga_ctx.arch().circuit_lib, options.verbose_output());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
