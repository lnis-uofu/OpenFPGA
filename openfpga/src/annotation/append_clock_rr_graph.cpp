#include "append_clock_rr_graph.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

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

  size_t num_clock_nodes = 0;
  size_t num_clock_edges = 0;

  /* Report number of added clock nodes and edges */
  VTR_LOGV(
    verbose,
    "Appended %lu clock nodes and %lu clock edges to routing resource graph.\n",
    num_clock_nodes, num_clock_edges);

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
