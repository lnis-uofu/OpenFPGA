#ifndef CLOCK_NETWORK_UTILS_H
#define CLOCK_NETWORK_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "clock_network.h"
#include "rr_graph_view.h"
#include "tile_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

int link_clock_network_rr_graph(ClockNetwork& clk_ntwk,
                                const RRGraphView& rr_graph);

int check_clock_network_tile_annotation(const ClockNetwork& clk_ntwk,
                                        const TileAnnotation& tile_annotation);

}  // End of namespace openfpga

#endif
