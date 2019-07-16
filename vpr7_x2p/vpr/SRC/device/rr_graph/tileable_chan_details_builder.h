#ifndef TILEABLE_CHAN_DETAILS_BUILDER_H 
#define TILEABLE_CHAN_DETAILS_BUILDER_H 

#include "vpr_types.h"
#include "chan_node_details.h"

int adapt_to_tileable_route_chan_width(int chan_width, std::vector<t_segment_inf> segment_inf);

ChanNodeDetails build_unidir_chan_node_details(const size_t chan_width, const size_t max_seg_length,
                                               const enum e_side device_side, 
                                               const std::vector<t_segment_inf> segment_inf);

#endif
