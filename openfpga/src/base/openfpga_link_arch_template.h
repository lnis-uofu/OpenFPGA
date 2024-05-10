#ifndef OPENFPGA_LINK_ARCH_TEMPLATE_H
#define OPENFPGA_LINK_ARCH_TEMPLATE_H
/********************************************************************
 * This file includes functions to read an OpenFPGA architecture file
 * which are built on the libarchopenfpga library
 *******************************************************************/
#include "annotate_bitstream_setting.h"
#include "annotate_clustering.h"
#include "annotate_pb_graph.h"
#include "annotate_pb_types.h"
#include "annotate_physical_tiles.h"
#include "annotate_placement.h"
#include "annotate_rr_graph.h"
#include "annotate_simulation_setting.h"
#include "append_clock_rr_graph.h"
#include "build_tile_direct.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "mux_library_builder.h"
#include "openfpga_annotate_routing.h"
#include "openfpga_rr_graph_support.h"
#include "pb_type_utils.h"
#include "read_activity.h"
#include "read_xml_pin_constraints.h"
#include "route_clock_rr_graph.h"
#include "vpr_device_annotation.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to link openfpga architecture to VPR, including:
 * - physical pb_type
 * - mode selection bits for pb_type and pb interconnect
 * - circuit models for pb_type and pb interconnect
 * - physical pb_graph nodes and pb_graph pins
 * - circuit models for global routing architecture
 *******************************************************************/
template <class T>
int link_arch_template(T& openfpga_ctx, const Command& cmd,
                       const CommandContext& cmd_context) {
  vtr::ScopedStartFinishTimer timer(
    "Link OpenFPGA architecture to VPR architecture");

  CommandOptionId opt_activity_file = cmd.option("activity_file");
  CommandOptionId opt_sort_edge = cmd.option("sort_gsb_chan_node_in_edges");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Build fast look-up between physical tile pin index and port information */
  build_physical_tile_pin2port_info(
    g_vpr_ctx.device(), openfpga_ctx.mutable_vpr_device_annotation());

  /* Annotate pb_type graphs
   * - physical pb_type
   * - mode selection bits for pb_type and pb interconnect
   * - circuit models for pb_type and pb interconnect
   */
  annotate_pb_types(g_vpr_ctx.device(), openfpga_ctx.arch(),
                    openfpga_ctx.mutable_vpr_device_annotation(),
                    cmd_context.option_enable(cmd, opt_verbose));

  /* Annotate pb_graph_nodes
   * - Give unique index to each node in the same type
   * - Bind operating pb_graph_node to their physical pb_graph_node
   * - Bind pins from operating pb_graph_node to their physical pb_graph_node
   * pins
   */
  annotate_pb_graph(g_vpr_ctx.device(),
                    openfpga_ctx.mutable_vpr_device_annotation(),
                    cmd_context.option_enable(cmd, opt_verbose));

  /* Annotate routing architecture to circuit library */
  annotate_rr_graph_circuit_models(g_vpr_ctx.device(), openfpga_ctx.arch(),
                                   openfpga_ctx.mutable_vpr_device_annotation(),
                                   cmd_context.option_enable(cmd, opt_verbose));

  /* Annotate routing results:
   * - net mapping to each rr_node
   * - previous nodes driving each rr_node
   */
  openfpga_ctx.mutable_vpr_routing_annotation().init(
    g_vpr_ctx.device().rr_graph);

  // Incase the incoming edges are not built. This may happen when loading
  // rr_graph from an external file
  g_vpr_ctx.mutable_device().rr_graph_builder.build_in_edges();
  annotate_vpr_rr_node_nets(g_vpr_ctx.device(), g_vpr_ctx.clustering(),
                            g_vpr_ctx.routing(),
                            openfpga_ctx.mutable_vpr_routing_annotation(),
                            cmd_context.option_enable(cmd, opt_verbose));

  annotate_rr_node_previous_nodes(g_vpr_ctx.device(), g_vpr_ctx.clustering(),
                                  g_vpr_ctx.routing(),
                                  openfpga_ctx.mutable_vpr_routing_annotation(),
                                  cmd_context.option_enable(cmd, opt_verbose));

  /* Build the routing graph annotation
   * - RRGSB
   * - DeviceRRGSB
   */
  if (false == is_vpr_rr_graph_supported(g_vpr_ctx.device().rr_graph)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Build incoming edges as VPR only builds fan-out edges for each node */
  VTR_LOG("Built %ld incoming edges for routing resource graph\n",
          g_vpr_ctx.device().rr_graph.in_edges_count());
  VTR_ASSERT(g_vpr_ctx.device().rr_graph.validate_in_edges());
  annotate_device_rr_gsb(
    g_vpr_ctx.device(), openfpga_ctx.mutable_device_rr_gsb(),
    !openfpga_ctx.clock_arch().empty(), /* FIXME: consider to be more robust! */
    cmd_context.option_enable(cmd, opt_verbose));

  if (true == cmd_context.option_enable(cmd, opt_sort_edge)) {
    sort_device_rr_gsb_chan_node_in_edges(
      g_vpr_ctx.device().rr_graph, openfpga_ctx.mutable_device_rr_gsb(),
      cmd_context.option_enable(cmd, opt_verbose));
    sort_device_rr_gsb_ipin_node_in_edges(
      g_vpr_ctx.device().rr_graph, openfpga_ctx.mutable_device_rr_gsb(),
      cmd_context.option_enable(cmd, opt_verbose));
  }

  /* Build multiplexer library */
  openfpga_ctx.mutable_mux_lib() = build_device_mux_library(
    g_vpr_ctx.device(), const_cast<const T&>(openfpga_ctx));

  /* Build tile direct annotation */
  openfpga_ctx.mutable_tile_direct() = build_device_tile_direct(
    g_vpr_ctx.device(), openfpga_ctx.arch().arch_direct,
    cmd_context.option_enable(cmd, opt_verbose));

  /* Annotate clustering results */
  if (CMD_EXEC_FATAL_ERROR ==
      annotate_post_routing_cluster_sync_results(
        g_vpr_ctx.clustering(),
        openfpga_ctx.mutable_vpr_clustering_annotation())) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Annotate placement results */
  annotate_mapped_blocks(g_vpr_ctx.device(), g_vpr_ctx.clustering(),
                         g_vpr_ctx.placement(),
                         openfpga_ctx.mutable_vpr_placement_annotation());

  /* Read activity file is manadatory in the following flow-run settings
   * - When users specify that number of clock cycles
   *   should be inferred from FPGA implmentation
   * - When FPGA-SPICE is enabled
   */
  std::unordered_map<AtomNetId, t_net_power> net_activity;
  if (true == cmd_context.option_enable(cmd, opt_activity_file)) {
    net_activity =
      read_activity(g_vpr_ctx.atom().nlist,
                    cmd_context.option_value(cmd, opt_activity_file).c_str());
  }

  /* TODO: Annotate the number of clock cycles and clock frequency by following
   * VPR results We SHOULD create a new simulation setting for OpenFPGA use only
   * Avoid overwrite the raw data achieved when parsing!!!
   */
  /* OVERWRITE the simulation setting in openfpga context from the arch
   * TODO: This will be removed when openfpga flow is updated
   */
  // openfpga_ctx.mutable_simulation_setting() =
  // openfpga_ctx.mutable_arch().sim_setting;
  if (CMD_EXEC_FATAL_ERROR ==
      annotate_simulation_setting(g_vpr_ctx.atom(), g_vpr_ctx.clustering(),
                                  net_activity,
                                  openfpga_ctx.mutable_simulation_setting())) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Build bitstream annotation based on bitstream settings */
  if (CMD_EXEC_FATAL_ERROR ==
      annotate_bitstream_setting(
        openfpga_ctx.bitstream_setting(), g_vpr_ctx.device(),
        openfpga_ctx.vpr_device_annotation(),
        openfpga_ctx.mutable_vpr_bitstream_annotation())) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Top-level function to append a clock network to VPR's routing resource graph,
 *including:
 * - Routing tracks dedicated to clock network
 * - Programmable switches to enable reconfigurability of clock network
 * - Adding virtual sources for clock signals
 *******************************************************************/
template <class T>
int append_clock_rr_graph_template(T& openfpga_ctx, const Command& cmd,
                                   const CommandContext& cmd_context) {
  vtr::ScopedStartFinishTimer timer(
    "Append clock network to routing resource graph");

  CommandOptionId opt_verbose = cmd.option("verbose");

  return append_clock_rr_graph(
    g_vpr_ctx.mutable_device(), openfpga_ctx.mutable_clock_rr_lookup(),
    openfpga_ctx.clock_arch(), cmd_context.option_enable(cmd, opt_verbose));
}

/********************************************************************
 * Top-level function to route a clock network based on the clock spines
 * defined in clock architecture
 *******************************************************************/
template <class T>
int route_clock_rr_graph_template(T& openfpga_ctx, const Command& cmd,
                                  const CommandContext& cmd_context) {
  vtr::ScopedStartFinishTimer timer("Route clock routing resource graph");

  /* add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId opt_pcf = cmd.option("pin_constraints_file");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* If pin constraints are enabled by command options, read the file */
  PinConstraints pin_constraints;
  if (true == cmd_context.option_enable(cmd, opt_pcf)) {
    pin_constraints =
      read_xml_pin_constraints(cmd_context.option_value(cmd, opt_pcf).c_str());
  }

  return route_clock_rr_graph(
    openfpga_ctx.mutable_vpr_routing_annotation(), g_vpr_ctx.device(),
    g_vpr_ctx.atom(), g_vpr_ctx.clustering().clb_nlist,
    openfpga_ctx.vpr_netlist_annotation(), openfpga_ctx.clock_rr_lookup(),
    openfpga_ctx.clock_arch(), pin_constraints,
    cmd_context.option_enable(cmd, opt_verbose));
}

} /* end namespace openfpga */

#endif
