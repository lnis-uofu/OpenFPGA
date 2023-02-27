#include "clock_network_utils.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_time.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Link all the segments that are defined in a routing resource graph to a given
 *clock network
 *******************************************************************/
static 
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

/********************************************************************
 * Link all the switches that are defined in a routing resource graph to a given
 *clock network
 *******************************************************************/
static 
int link_clock_network_rr_switches(ClockNetwork& clk_ntwk,
                                   const RRGraphView& rr_graph) {
  /* default switch id */
  std::string default_switch_name = clk_ntwk.default_switch_name();
  for (size_t rr_switch_id = 0; rr_switch_id < rr_graph.num_rr_switches();
       ++rr_switch_id) {
    if (std::string(rr_graph.rr_switch_inf(RRSwitchId(rr_switch_id)).name) ==
        default_switch_name) {
      clk_ntwk.set_default_switch(RRSwitchId(rr_switch_id));
      return CMD_EXEC_SUCCESS;
    }
  }

  return CMD_EXEC_FATAL_ERROR;
}

int link_clock_network_rr_graph(ClockNetwork& clk_ntwk,
                                const RRGraphView& rr_graph) {
  int status = CMD_EXEC_SUCCESS;

  status = link_clock_network_rr_segments(clk_ntwk, rr_graph);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }
  status = link_clock_network_rr_switches(clk_ntwk, rr_graph);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }

  return status; 
}


}  // End of namespace openfpga
