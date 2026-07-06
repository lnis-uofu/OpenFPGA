#ifndef APPEND_CLOCK_RR_GRAPH_H
#define APPEND_CLOCK_RR_GRAPH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "clock_network.h"
#include "rr_clock_spatial_lookup.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int append_clock_rr_graph(DeviceContext& vpr_device_ctx,
                          RRClockSpatialLookup& clk_rr_lookup,
                          const ClockNetwork& clk_ntwk,
                          const int rr_graph_x_offset,
                          const int rr_graph_y_offset, const bool& verbose);

} /* end namespace openfpga */

#endif
