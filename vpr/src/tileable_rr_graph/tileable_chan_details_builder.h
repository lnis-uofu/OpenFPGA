#ifndef TILEABLE_CHAN_DETAILS_BUILDER_H 
#define TILEABLE_CHAN_DETAILS_BUILDER_H 

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "physical_types.h"
#include "chan_node_details.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int adapt_to_tileable_route_chan_width(const int& chan_width, const std::vector<t_segment_inf>& segment_inf);

ChanNodeDetails build_unidir_chan_node_details(const size_t& chan_width, const size_t& max_seg_length,
                                               const e_side& device_side, 
                                               const std::vector<t_segment_inf>& segment_inf);

} /* end namespace openfpga */

#endif
