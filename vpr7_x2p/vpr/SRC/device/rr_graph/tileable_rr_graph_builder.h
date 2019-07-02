#ifndef TILEABLE_RR_GRAPH_BUILDER_H
#define TILEABLE_RR_GRAPH_BUILDER_H

#include <vector>

#include "vpr_types.h"

void build_tileable_unidir_rr_graph(INP const int L_num_types,
                                    INP t_type_ptr types, INP const int L_nx, INP const int L_ny,
                                    INP struct s_grid_tile **L_grid, INP const int chan_width,
                                    INP const enum e_switch_block_type sb_type, INP const int Fs, 
                                    INP const enum e_switch_block_type sb_subtype, INP const int subFs, 
                                    INP const int num_seg_types,
                                    INP const t_segment_inf * segment_inf,
                                    INP const int num_switches, INP const int delayless_switch, 
                                    INP const t_timing_inf timing_inf, INP const int wire_to_ipin_switch,
                                    INP const enum e_base_cost_type base_cost_type, 
                                    INP const t_direct_inf *directs, 
                                    INP const int num_directs, INP const boolean ignore_Fc_0, 
                                    OUTP int *Warnings); 

#endif
