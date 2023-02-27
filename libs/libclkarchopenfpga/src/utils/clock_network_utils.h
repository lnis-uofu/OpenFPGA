#ifndef CLOCK_NETWORK_UTILS_H
#define CLOCK_NETWORK_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "clock_network.h"
#include "rr_graph_view.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

int link_clock_network_rr_segments(ClockNetwork& clk_ntwk,
                                   const RRGraphView& rr_graph);

}  // End of namespace openfpga

#endif
