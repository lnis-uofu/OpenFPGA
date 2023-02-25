#include "annotate_clock_rr_graph.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "command_exit_codes.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Assign mapped blocks to grid locations
 * This is used by bitstream generator mainly as a fast look-up to
 * get mapped blocks with a given coordinate
 *******************************************************************/
int append_clock_rr_graph(DeviceContext& vpr_device_ctx,
                          const ClockNetwork& clk_ntwk,
                          const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Adding clock nodes to routing resource graph");

  /* Skip if there is no clock tree */
  if (clk_ntwk.num_trees()) {
    VTR_LOG("Skip due to 0 clock trees.\nDouble check your clock architecture definition if this is unexpected\n");
    return CMD_EXEC_SUCCESS;
  }

  /* Walk through the GSB array and add clock nodes to each GSB.
   * Note that the GSB array is smaller than the grids by 1 column and 1 row!!!
   */
  vtr::Point<size_t> gsb_range(vpr_device_ctx.grid.width() - 1,
                               vpr_device_ctx.grid.height() - 1);

  size_t gsb_cnt = 0;
  /* For each switch block, determine the size of array */
  for (size_t ix = 0; ix < gsb_range.x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.y(); ++iy) {
      /* Here we give the builder the fringe coordinates so that it can handle
       * the GSBs at the borderside correctly sort drive_rr_nodes should be
       * called if required by users
       */
      const RRGSB& rr_gsb =
        build_rr_gsb(vpr_device_ctx,
                     vtr::Point<size_t>(vpr_device_ctx.grid.width() - 2,
                                        vpr_device_ctx.grid.height() - 2),
                     vtr::Point<size_t>(ix, iy));

      /* Add clock nodes to device_rr_gsb */
      vtr::Point<size_t> gsb_coordinate = rr_gsb.get_sb_coordinate();
      gsb_cnt++; /* Update counter */
      /* Print info */
      VTR_LOGV(verbose, "[%lu%] Added clock nodes to GSB[%lu][%lu]\r",
              100 * gsb_cnt / (gsb_range.x() * gsb_range.y()), ix, iy);
    }
  }
  /* Report number of added clock nodes and edges */
  VTR_LOGV(verbose, "Appended clock nodes to %d General Switch Blocks (GSBs).\n",
           gsb_range.x() * gsb_range.y());

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
