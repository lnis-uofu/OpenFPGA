/*Declaration*/
/*The top function*/
int add_rr_graph_switch_segment_pattern(enum e_directionality directionality,
                                        INP int nodes_per_chan,
                                        INP int num_swseg_pattern,
                                        INP t_swseg_pattern_inf* swseg_patterns,
                                        INP t_ivec*** L_rr_node_indices,
                                        INP t_seg_details* seg_details_x,
                                        INP t_seg_details* seg_details_y);

void update_rr_nodes_driver_switch(enum e_directionality directionality);

