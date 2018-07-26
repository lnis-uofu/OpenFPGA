
void fprint_routing_chan_subckt(FILE* fp,
                                int x, int y, t_rr_type chan_type, 
                                int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                t_ivec*** LL_rr_node_indices,
                                int num_segment, t_segment_inf* segments);

void fprint_grid_side_pin_with_given_index(FILE* fp,
                                          int pin_index, int side,
                                          int x, int y);

void fprint_grid_side_pins(FILE* fp,
                           t_rr_type pin_type,
                           int x,
                           int y,
                           int side);

void fprint_switch_box_chan_port(FILE* fp,
                                 t_sb cur_sb_info, 
                                 int chan_side,
                                 t_rr_node* cur_rr_node,
                                 enum PORTS cur_rr_node_direction);

void fprint_switch_box_short_interc(FILE* fp, 
                                    t_sb cur_sb_info,
                                    int chan_side,
                                    t_rr_node* cur_rr_node,
                                    int actual_fan_in,
                                    t_rr_node* drive_rr_node);

void fprint_switch_box_mux(FILE* fp, 
                           t_sb cur_sb_info, 
                           int chan_side,
                           t_rr_node* cur_rr_node,
                           int mux_size,
                           t_rr_node** drive_rr_nodes,
                           int switch_index);

void fprint_switch_box_interc(FILE* fp, 
                              t_sb cur_sb_info,
                              int chan_side,
                              t_rr_node* cur_rr_node);

void fprint_routing_switch_box_subckt(FILE* fp, t_sb cur_sb_info,
                                      int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices);

void fprint_connection_box_short_interc(FILE* fp,
                                        t_cb cur_cb_info,
                                        t_rr_node* src_rr_node);

void fprint_connection_box_mux(FILE* fp,
                               t_cb cur_cb_info,
                               t_rr_node* src_rr_node);

void fprint_connection_box_interc(FILE* fp,
                                  t_cb cur_cb_info,
                                  t_rr_node* src_rr_node);

void fprint_routing_connection_box_subckt(FILE* fp, t_cb cur_cb_info,
                                          int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                          t_ivec*** LL_rr_node_indices);

void generate_spice_routing_resources(char* subckt_dir,
                                      t_arch arch,
                                      t_det_routing_arch* routing_arch,
                                      int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices);

