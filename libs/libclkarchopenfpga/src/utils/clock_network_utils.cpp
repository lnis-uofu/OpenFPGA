#include "clock_network_utils.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_time.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Link all the segments that are defined in a routing resource graph to a given
 *clock network
 *******************************************************************/
static int link_clock_network_rr_segments(ClockNetwork& clk_ntwk,
                                          const RRGraphView& rr_graph) {
  /* default segment id */
  std::string default_segment_name = clk_ntwk.default_segment_name();
  for (size_t rr_seg_id = 0; rr_seg_id < rr_graph.num_rr_segments();
       ++rr_seg_id) {
    if (rr_graph.rr_segments(RRSegmentId(rr_seg_id)).name ==
        default_segment_name) {
      clk_ntwk.set_default_segment(RRSegmentId(rr_seg_id));
      return CMD_EXEC_SUCCESS;
    }
  }
  VTR_LOG_ERROR(
    "Unable to find the default segement '%s' in VPR architecture "
    "description!\n",
    default_segment_name.c_str());
  return CMD_EXEC_FATAL_ERROR;
}

/********************************************************************
 * Link all the switches that are defined in a routing resource graph to a given
 *clock network
 *******************************************************************/
static int link_clock_network_tap_rr_switches(ClockNetwork& clk_ntwk,
                                              const RRGraphView& rr_graph) {
  /* default tap switch id */
  std::string default_tap_switch_name = clk_ntwk.default_tap_switch_name();
  for (size_t rr_switch_id = 0; rr_switch_id < rr_graph.num_rr_switches();
       ++rr_switch_id) {
    if (std::string(rr_graph.rr_switch_inf(RRSwitchId(rr_switch_id)).name) ==
        default_tap_switch_name) {
      clk_ntwk.set_default_tap_switch(RRSwitchId(rr_switch_id));
      return CMD_EXEC_SUCCESS;
    }
  }
  VTR_LOG_ERROR(
    "Unable to find the default tap switch '%s' in VPR architecture "
    "description!\n",
    default_tap_switch_name.c_str());
  return CMD_EXEC_FATAL_ERROR;
}

/********************************************************************
 * Link all the switches that are defined in a routing resource graph to a given
 *clock network
 *******************************************************************/
static int link_clock_network_driver_rr_switches(ClockNetwork& clk_ntwk,
                                                 const RRGraphView& rr_graph) {
  /* default driver switch id */
  std::string default_driver_switch_name =
    clk_ntwk.default_driver_switch_name();
  for (size_t rr_switch_id = 0; rr_switch_id < rr_graph.num_rr_switches();
       ++rr_switch_id) {
    if (std::string(rr_graph.rr_switch_inf(RRSwitchId(rr_switch_id)).name) ==
        default_driver_switch_name) {
      clk_ntwk.set_default_driver_switch(RRSwitchId(rr_switch_id));
      return CMD_EXEC_SUCCESS;
    }
  }
  VTR_LOG_ERROR(
    "Unable to find the default driver switch '%s' in VPR architecture "
    "description!\n",
    default_driver_switch_name.c_str());
  return CMD_EXEC_FATAL_ERROR;
}

int link_clock_network_rr_graph(ClockNetwork& clk_ntwk,
                                const RRGraphView& rr_graph) {
  int status = CMD_EXEC_SUCCESS;

  status = link_clock_network_rr_segments(clk_ntwk, rr_graph);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }
  status = link_clock_network_tap_rr_switches(clk_ntwk, rr_graph);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }
  status = link_clock_network_driver_rr_switches(clk_ntwk, rr_graph);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }

  return status;
}

/** Check for each global ports in tile annotation
 *  If a clock tree is required for a global port, the global port name define
 * in the tile annotation should match the one in clock clock
 */
int check_clock_network_tile_annotation(const ClockNetwork& clk_ntwk,
                                        const TileAnnotation& tile_annotation) {
  for (const TileGlobalPortId& gport_id : tile_annotation.global_ports()) {
    if (!tile_annotation.global_port_thru_dedicated_network(gport_id)) {
      continue;
    }
    std::string gport_name = tile_annotation.global_port_name(gport_id);
    std::string clk_tree_name =
      tile_annotation.global_port_clock_arch_tree_name(gport_id);
    ClockTreeId clk_tree_id = clk_ntwk.find_tree(clk_tree_name);
    if (!clk_ntwk.valid_tree_id(clk_tree_id)) {
      VTR_LOG_ERROR(
        "Invalid clock tree name '%s' defined for global port '%s' in tile "
        "annotation! Must be a valid name defined in the clock network "
        "description!\n",
        clk_tree_name.c_str(), gport_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (clk_ntwk.tree_global_port(clk_tree_id).get_name() != gport_name) {
      VTR_LOG_ERROR(
        "Global port '%s' of clock tree name '%s' must match the name of "
        "assoicated global port '%s' in tile annotation! Must be a valid name "
        "defined in the clock network description!\n",
        clk_ntwk.tree_global_port(clk_tree_id).to_verilog_string().c_str(),
        clk_tree_name.c_str(), gport_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  return CMD_EXEC_SUCCESS;
}

}  // End of namespace openfpga
