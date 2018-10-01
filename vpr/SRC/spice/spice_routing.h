
void fprint_routing_chan_subckt(FILE* fp,
                                int x,
                                int y,
                                t_rr_type chan_type, 
                                int chan_width,
                                t_ivec*** LL_rr_node_indices,
                                int num_segment,
                                t_segment_inf* segments);

t_rr_node** get_grid_side_pin_rr_nodes(int* num_pin_rr_nodes,
                                       t_rr_type pin_type,
                                       int x,
                                       int y,
                                       int side,
                                       t_ivec*** LL_rr_node_indices);

void fprint_grid_side_in_with_given_index(FILE* fp,
                                          int pin_index,
                                          int x,
                                          int y);

void fprint_grid_side_pins(FILE* fp,
                           t_rr_type pin_type,
                           int x,
                           int y,
                           int side);

void determine_src_chan_coordinate_switch_box(t_rr_node* src_rr_node,
                                              t_rr_node* des_rr_node,
                                              int side,
                                              int switch_box_x,
                                              int switch_box_y,
                                              int* src_chan_x,
                                              int* src_chan_y,
                                              char** src_chan_port_name);

void fprint_switch_box_chan_port(FILE* fp,
                                 int switch_box_x, 
                                 int switch_box_y, 
                                 int chan_side,
                                 t_rr_node* cur_rr_node);

void fprint_switch_box_short_interc(FILE* fp, 
                                    int switch_box_x, 
                                    int switch_box_y, 
                                    int chan_side,
                                    t_rr_node* cur_rr_node,
                                    int actual_fan_in,
                                    t_rr_node* drive_rr_node);

void fprint_switch_box_mux(FILE* fp, 
                           int switch_box_x, 
                           int switch_box_y, 
                           int chan_side,
                           t_rr_node* cur_rr_node);

void fprint_switch_box_interc(FILE* fp, 
                              int switch_box_x, 
                              int switch_box_y, 
                              int chan_side,
                              t_rr_node* cur_rr_node);

void fprint_routing_switch_box_subckt(FILE* fp, 
                                      int x, 
                                      int y, 
                                      t_ivec*** LL_rr_node_indices);

void fprint_connection_box_short_interc(FILE* fp,
                                        t_rr_type chan_type,
                                        int cb_x,
                                        int cb_y,
                                        t_rr_node* src_rr_node);

void fprint_connection_box_mux(FILE* fp,
                               t_rr_type chan_type,
                               int cb_x,
                               int cb_y,
                               t_rr_node* src_rr_node);

void fprint_connection_box_interc(FILE* fp,
                                  t_rr_type chan_type,
                                  int cb_x,
                                  int cb_y,
                                  t_rr_node* src_rr_node);

void fprint_routing_connection_box_subckt(FILE* fp,
                                          t_rr_type chan_type,
                                          int x,
                                          int y,
                                          int chan_width,
                                          t_ivec*** LL_rr_node_indices);

void generate_spice_routing_resources(char* subckt_dir,
                                      t_arch arch,
                                      t_det_routing_arch* routing_arch,
                                      t_ivec*** LL_rr_node_indices);
