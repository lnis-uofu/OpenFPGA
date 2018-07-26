
void load_post_place_net_delay(float** net_delay,
                               int n_blks,
                               t_block* blk,
                               int n_nets, 
                               t_net* nets,
                               t_det_routing_arch det_routing_arch,
                               t_segment_inf* segment_inf,
                               t_timing_inf timing_inf,
                               int num_directs,
                               t_direct_inf* directs);

float estimate_post_place_one_net_sink_delay(int net_index, 
                                             int n_blks,
                                             t_block* blk,
                                             int src_blk_index,
                                             int des_blk_index,
                                             t_det_routing_arch det_routing_arch,
                                             t_segment_inf* segment_inf,
                                             t_timing_inf timing_inf,
                                             int num_directs,
                                             t_direct_inf* directs);

float esti_pin2pin_one_net_delay(t_block src_blk,
                                 int src_pin_side,
                                 int src_pin_index,
                                 t_block des_blk,
                                 int des_pin_side,
                                 int des_pin_index,
                                 t_det_routing_arch det_routing_arch,
                                 t_segment_inf* segment_inf,
                                 t_timing_inf timing_inf,
                                 int num_directs,
                                 t_direct_inf* directs);

void load_expected_remapped_net_delay(float** net_delay,                                       
                                      int n_blks, 
                                      t_block* blk,
                                      int n_nets, 
                                      t_net* nets,
                                      t_det_routing_arch det_routing_arch,
                                      t_segment_inf* segment_inf,
                                      t_timing_inf timing_inf,
                                      int num_directs,
                                      t_direct_inf* directs);

float esti_distance_num_seg_delay(int distance,
                                  int num_segment,
                                  t_segment_inf* segment_inf,
                                  int allow_long_segment);

float esti_one_segment_net_delay(int distance, t_segment_inf segment_inf);

float get_crossing_penalty(int num_sinks);
