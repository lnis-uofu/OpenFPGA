
void try_clb_pin_remap_after_placement(t_det_routing_arch det_routing_arch,
                                       t_segment_inf* segment_inf,
                                       t_timing_inf timing_inf,
                                       int num_directs,
                                       t_direct_inf* directs);

int generate_nets_sinks_prefer_sides(int n_nets, t_net* nets,
                                     int n_blks, t_block* blk);

int set_unroute_blk_pins_prefer_sides(int n_blk, t_block* blk);

void set_blk_net_one_sink_prefer_side(int* prefer_side, /* [0..3] array, should be allocated before */
                                      enum e_side src_pin_side, 
                                      t_block src_blk,
                                      t_block des_blk);

void set_src_top_side_net_one_sink_prefer_side(int* prefer_side, 
                                               t_block src_blk, t_block des_blk);

void set_src_right_side_net_one_sink_prefer_side(int* prefer_side, 
                                                 t_block src_blk, t_block des_blk);

void set_src_bottom_side_net_one_sink_prefer_side(int* prefer_side, 
                                                  t_block src_blk, t_block des_blk);

void set_src_left_side_net_one_sink_prefer_side(int* prefer_side, 
                                                t_block src_blk, t_block des_blk);

int sat_blks_pins_prefer_side(int n_nets, t_net* nets, 
                              int n_blks, t_block* blk,
                              float** net_delay, float** expected_net_delay, t_slack* slack);

int sat_one_blk_pins_prefer_side(int n_nets, t_net* nets, 
                                 t_block* target_blk,
                                 float** net_delay, float** expected_net_delay, t_slack* slack);

int try_sat_one_blk_pin_class_prefer_side(t_block* target_blk,
                                          int n_nets, t_net* nets,
                                          int class_index, 
                                          int* is_pin_conflict, int* cur_pin_side,
                                          float** net_delay, float** expected_net_delay, t_slack* slack);

int try_remap_blk_class_one_conflict_pin(t_block* target_blk, int class_index, int pin_index,
                                         int n_nets, t_net* nets,
                                         int* cur_pin_side, int* is_pin_conflict,
                                         float* esti_delay_gain, t_slack* slack);
