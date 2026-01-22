#pragma once

#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "openfpga_port.h"
#include "openfpga_port_parser.h"
#include "pcf_reader.h"
#include "repack_design_constraints.h"
#include "vtr_log.h"
#include "write_xml_repack_design_constraints.h"
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

static inline RepackDesignConstraints gen_repack_constraints(openfpga::PcfData* pcf_data){
  auto& cluster_ctx = g_vpr_ctx.clustering();
  auto& atom_ctx = g_vpr_ctx.atom();
  auto& clb_nlist = cluster_ctx.clb_nlist;

  /* 1. Get unique pb_types, clock pins and clock nets from clb_nlist.blocks */
  std::set<std::string> pb_type_names;
  std::set<BasicPort> clock_pins;
  std::set<std::string> clock_nets;

  for (auto block_id : clb_nlist.blocks()) {
    auto pb = clb_nlist.block_pb(block_id);
    auto pb_type = pb->pb_graph_node->pb_type;
    pb_type_names.insert(pb_type->name);

    int port_index = 0;
    for (int i = 0; i < pb_type->num_ports; i++) {
      auto& port = pb_type->ports[i];
      if(!port.is_clock)
        continue;
      for (int j = 0; j < port.num_pins; j++) {
        auto& pin = pb->pb_graph_node->clock_pins[port_index][j];
        std::string pin_name = std::string(port.name) + "[" + std::to_string(j) + "]";
        openfpga::PortParser port_parser(pin_name);
        clock_pins.insert(port_parser.port());
        int node_index = pin.pin_count_in_cluster;
        if(pb->pb_route.count(node_index)){
          AtomNetId net_id = pb->pb_route[node_index].atom_net_id;
          std::string net_name = atom_ctx.nlist.net_name(net_id);
          clock_nets.insert(net_name);
        }
      }
      port_index++;
    }
  }

  std::vector<std::pair<std::string, BasicPort>> matches;

  /* First match nets & pins from the pcf file */
  if(pcf_data){
    for (const PcfIoConstraintId& io_id : pcf_data->io_constraints()) {
      std::string net = pcf_data->io_net(io_id);
      BasicPort pin = pcf_data->io_pin(io_id);
      if(!(clock_nets.count(net) && clock_pins.count(pin)))
        continue;
      matches.push_back({net, pin});
      clock_nets.erase(net);
      clock_pins.erase(pin);
    }
  }

  /* Match the rest in an arbitrary way */
  for(auto net: clock_nets){
    VTR_ASSERT_MSG(!clock_pins.empty(), "Not enough clock pins for clocks in the design");
    auto first_available_pin = *clock_pins.begin();
    matches.push_back({net, first_available_pin});
    clock_pins.erase(first_available_pin);
  }

  /* 3. Create the design constraints */
  RepackDesignConstraints out;
  for(auto pb_type: pb_type_names){
    for(auto [net, pin]: matches){
      auto id = out.create_design_constraint(RepackDesignConstraints::PIN_ASSIGNMENT);
      out.set_pb_type(id, pb_type);
      out.set_net(id, net);
      out.set_pin(id, pin);
    }
  }

  return out;
}

/********************************************************************
 * Top-level function to generate a repack_design_constraints.xml file
 * from either a .pcf file or vpr context
 *******************************************************************/
template <class T>
int gen_repack_constraints_template(const Command& cmd,
                                    const CommandContext& cmd_context) {
  CommandOptionId opt_pcf = cmd.option("pcf");
  CommandOptionId opt_file = cmd.option("file");

  std::string out_fname = cmd_context.option_value(cmd, opt_file);

  RepackDesignConstraints constraints;

  if (cmd_context.option_enable(cmd, opt_pcf)) {
    std::string pcf_fname = cmd_context.option_value(cmd, opt_pcf);

    openfpga::PcfData pcf_data;
    openfpga::read_pcf(pcf_fname.c_str(), pcf_data);
    VTR_LOG("[gen_repack_constraints] Read the design constraints from a pcf file: %s.\n",
            pcf_fname.c_str());

    if (!pcf_data.validate()) {
      VTR_LOG_ERROR("[gen_repack_constraints] PCF contains invalid I/O assignment!\n");
      return CMD_EXEC_FATAL_ERROR;
    } else {
      VTR_LOG("[gen_repack_constraints] PCF basic check passed\n");
    }
    constraints = gen_repack_constraints(&pcf_data);
  } else {
    constraints = gen_repack_constraints(NULL);
  }

  write_xml_repack_design_constraints(out_fname.c_str(), constraints);
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
