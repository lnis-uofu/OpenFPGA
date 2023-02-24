#ifndef APPEND_CLOCK_RR_GRAPH_H
#define APPEND_CLOCK_RR_GRAPH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "clock_network.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int append_clock_rr_graph(DeviceContext& device_ctx,
                          const ClockNetwork& clk_ntwk);

} /* end namespace openfpga */

#endif
