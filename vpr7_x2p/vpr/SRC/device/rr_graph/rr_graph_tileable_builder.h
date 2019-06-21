#ifndef RR_GRAPH_TILEABLE_BUILDER_H
#define RR_GRAPH_TILEABLE_BUILDER_H

#include <vector>

#include "vpr_types.h"

#include "chan_node_details.h"

ChanNodeDetails build_unidir_chan_node_details(const size_t chan_width, const size_t max_seg_length,
                                               const enum e_side device_side, 
                                               const std::vector<t_segment_inf> segment_inf);

void build_tileable_unidir_rr_graph(INP const int L_num_types,
                                    INP t_type_ptr types, INP const int L_nx, INP const int L_ny,
                                    INP struct s_grid_tile **L_grid, INP const int chan_width,
                                    INP const enum e_switch_block_type sb_type, INP const int Fs, 
                                    INP const int num_seg_types,
                                    INP const t_segment_inf * segment_inf,
                                    INP const int num_switches, INP int const delayless_switch, const int global_route_switch,
                                    INP const t_timing_inf timing_inf, INP int const wire_to_ipin_switch,
                                    INP const enum e_base_cost_type base_cost_type, 
                                    INP const t_direct_inf *directs, 
                                    INP const int num_directs, INP const boolean ignore_Fc_0, 
                                    OUTP int *Warnings); 

#endif
