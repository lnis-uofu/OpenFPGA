#include "clock_network_utils.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_time.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Link all the segments that are defined in a routing resource graph to a given
 *clock network
 *******************************************************************/
int link_clock_network_rr_segments(ClockNetwork& clk_ntwk,
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

  return CMD_EXEC_FATAL_ERROR;
}

}  // End of namespace openfpga
