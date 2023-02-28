#include "route_clock_rr_graph.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Append a programmable clock network to an existing routing resource graph
 * This function will do the following jobs:
 * - Estimate the number of clock nodes and pre-allocate memory
 * - Add clock nodes
 * - Build edges between clock nodes
 * - Sanity checks
 *******************************************************************/
int route_clock_rr_graph(VprRoutingAnnotation& vpr_routing_annotation,
                         const DeviceContext& vpr_device_ctx,
                         const RRClockSpatialLookup& clk_rr_lookup,
                         const ClockNetwork& clk_ntwk, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Route programmable clock network based on routing resource graph");

  /* Skip if there is no clock tree */
  if (clk_ntwk.num_trees()) {
    VTR_LOG(
      "Skip due to 0 clock trees.\nDouble check your clock architecture "
      "definition if this is unexpected\n");
    return CMD_EXEC_SUCCESS;
  }

  /* Report any clock structure we do not support yet! */
  if (clk_ntwk.num_trees() > 1) {
    VTR_LOG(
      "Currently only support 1 clock tree in programmable clock "
      "architecture\nPlease update your clock architecture definition\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  /* TODO: Route spines one by one */

  /* TODO: Sanity checks */

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
