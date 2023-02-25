#include "append_clock_rr_graph.h"
#include "rr_graph_builder_utils.h"
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

static 
size_t estimate_clock_rr_graph_num_nodes(const DeviceGrid& grids,
                                         const bool& through_channel) {
  size_t num_nodes = 0;
  /* Check the number of CHANX nodes required */
  for (size_t iy = 0; iy < grids.height() - 1; ++iy) {
    for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channels are not allowed */
      if ((false == through_channel)
        && (false == is_chanx_exist(grids, chanx_coord))) {
        continue;
      }
    }
  }

  for (size_t ix = 0; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      vtr::Point<size_t> chany_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channel are not allowed */
      if ((false == through_channel)
        && (false == is_chany_exist(grids, chany_coord))) {
        continue;
      }
    }
  }

  return num_nodes;
}

/********************************************************************
 * Assign mapped blocks to grid locations
 * This is used by bitstream generator mainly as a fast look-up to
 * get mapped blocks with a given coordinate
 *******************************************************************/
int append_clock_rr_graph(DeviceContext& vpr_device_ctx,
                          const ClockNetwork& clk_ntwk, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Appending programmable clock network to routing resource graph");

  /* Skip if there is no clock tree */
  if (clk_ntwk.num_trees()) {
    VTR_LOG(
      "Skip due to 0 clock trees.\nDouble check your clock architecture "
      "definition if this is unexpected\n");
    return CMD_EXEC_SUCCESS;
  }

  /* Estimate the number of nodes and pre-allocated */
  size_t orig_num_nodes = vpr_device_ctx.rr_graph.num_nodes();
  size_t num_clock_nodes = estimate_clock_rr_graph_num_nodes(vpr_device_ctx.grid, vpr_device_ctx.arch->through_channel);
  vpr_device_ctx.rr_graph_builder.reserve_nodes(num_clock_nodes + orig_num_nodes); 

  size_t num_clock_edges = 0;

  /* Report number of added clock nodes and edges */
  VTR_LOGV(
    verbose,
    "Appended %lu clock nodes (+%.2f\%) and %lu clock edges to routing resource graph.\n",
    num_clock_nodes, (float)(num_clock_nodes / orig_num_nodes), num_clock_edges);

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
