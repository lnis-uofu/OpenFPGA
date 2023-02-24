#include "annotate_clock_rr_graph.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Assign mapped blocks to grid locations
 * This is used by bitstream generator mainly as a fast look-up to
 * get mapped blocks with a given coordinate
 *******************************************************************/
int append_clock_rr_graph(DeviceContext& device_ctx,
                          const ClockNetwork& clk_ntwk) {
  VTR_LOG("Adding clock nodes to routing resource graph...");

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
