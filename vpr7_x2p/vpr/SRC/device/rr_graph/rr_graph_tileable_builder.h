#ifndef RR_GRAPH_TILEABLE_BUILDER_H
#define RR_GRAPH_TILEABLE_BUILDER_H

void build_tileable_unidir_rr_graph(INP int L_num_types,
                                    INP t_type_ptr types, INP int L_nx, INP int L_ny,
                                    INP struct s_grid_tile **L_grid, INP int chan_width,
                                    INP enum e_switch_block_type sb_type, INP int Fs, 
                                    INP int num_seg_types,
                                    INP t_segment_inf * segment_inf,
                                    INP int delayless_switch,
                                    INP t_timing_inf timing_inf, INP int wire_to_ipin_switch,
                                    INP enum e_base_cost_type base_cost_type, INP t_direct_inf *directs, 
                                    INP int num_directs, INP boolean ignore_Fc_0, OUTP int *Warnings); 

#endif
