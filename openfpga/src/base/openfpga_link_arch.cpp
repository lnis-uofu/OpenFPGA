/********************************************************************
 * This file includes functions to read an OpenFPGA architecture file
 * which are built on the libarchopenfpga library
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_assert.h"
#include "vtr_log.h"

#include "vpr_device_annotation.h"
#include "pb_type_utils.h"
#include "annotate_pb_types.h"
#include "annotate_pb_graph.h"
#include "annotate_routing.h"
#include "annotate_rr_gsb.h"
#include "openfpga_link_arch.h"

/* Include global variables of VPR */
#include "globals.h"

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
void link_arch(OpenfpgaContext& openfpga_context,
               const Command& cmd, const CommandContext& cmd_context) { 

  vtr::ScopedStartFinishTimer timer("Link OpenFPGA architecture to VPR architecture");

  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Annotate pb_type graphs
   * - physical pb_type
   * - mode selection bits for pb_type and pb interconnect
   * - circuit models for pb_type and pb interconnect
   */
  annotate_pb_types(g_vpr_ctx.device(), openfpga_context.arch(),
                    openfpga_context.mutable_vpr_device_annotation(),
                    cmd_context.option_enable(cmd, opt_verbose));

  /* Annotate pb_graph_nodes
   * - Give unique index to each node in the same type
   * - Bind operating pb_graph_node to their physical pb_graph_node
   * - Bind pins from operating pb_graph_node to their physical pb_graph_node pins
   */
  annotate_pb_graph(g_vpr_ctx.device(),
                    openfpga_context.mutable_vpr_device_annotation(),
                    cmd_context.option_enable(cmd, opt_verbose));

  /* Annotate net mapping to each rr_node 
   */
  openfpga_context.mutable_vpr_routing_annotation().init(g_vpr_ctx.device().rr_graph);

  annotate_rr_node_nets(g_vpr_ctx.device(), g_vpr_ctx.clustering(), g_vpr_ctx.routing(), 
                        openfpga_context.mutable_vpr_routing_annotation(),
                        cmd_context.option_enable(cmd, opt_verbose));

  /* Build the routing graph annotation
   * - RRGSB
   * - DeviceRRGSB 
   */
  annotate_device_rr_gsb(g_vpr_ctx.device(),
                         openfpga_context.mutable_device_rr_gsb(),
                         cmd_context.option_enable(cmd, opt_verbose));
} 

} /* end namespace openfpga */
