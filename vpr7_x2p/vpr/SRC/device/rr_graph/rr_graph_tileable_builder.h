#ifndef RR_GRAPH_TILEABLE_BUILDER_H
#define RR_GRAPH_TILEABLE_BUILDER_H

t_rr_graph build_tileable_unidir_rr_graph(INP int L_num_types,
    INP t_type_ptr types, INP int L_nx, INP int L_ny,
    INP struct s_grid_tile **L_grid, INP int chan_width,
    INP struct s_chan_width_dist *chan_capacity_inf,
    INP enum e_switch_block_type sb_type, INP int Fs, INP int num_seg_types,
    INP int num_switches, INP t_segment_inf * segment_inf,
    INP int global_route_switch, INP int delayless_switch,
    INP t_timing_inf timing_inf, INP int wire_to_ipin_switch,
    INP enum e_base_cost_type base_cost_type, INP t_direct_inf *directs, 
    INP int num_directs, INP boolean ignore_Fc_0, OUTP int *Warnings,
        /*Xifan TANG: Switch Segment Pattern Support*/
        INP int num_swseg_pattern, INP t_swseg_pattern_inf* swseg_patterns,
        INP boolean opin_to_cb_fast_edges, INP boolean opin_logic_eq_edges); 

#endif
