
int get_ff_output_init_val(t_logical_block* ff_logical_block);

int get_lut_output_init_val(t_logical_block* lut_logical_block);

int get_logical_block_output_init_val(t_logical_block* cur_logical_block);

void init_one_sb_info(t_sb* cur_sb); 

void free_one_sb_info(t_sb* cur_sb);

t_sb** alloc_sb_info_array(int LL_nx, int LL_ny);

void free_sb_info_array(t_sb*** LL_sb_info, int LL_nx, int LL_ny);

void init_one_cb_info(t_cb* cur_cb); 

void free_one_cb_info(t_cb* cur_cb);

t_cb** alloc_cb_info_array(int LL_nx, int LL_ny);

void free_cb_info_array(t_cb*** LL_cb_info, int LL_nx, int LL_ny);

void free_clb_nets_spice_net_info();

int get_rr_node_index_in_sb_info(t_rr_node* cur_rr_node,
                                 t_sb cur_sb_info, 
                                 int chan_side, enum PORTS rr_node_direction);

int is_rr_node_exist_opposite_side_in_sb_info(t_sb cur_sb_info,
                                              t_rr_node* src_rr_node, 
                                              int chan_side);

boolean check_drive_rr_node_imply_short(t_sb cur_sb_info,
                                        t_rr_node* src_rr_node, 
                                        int chan_side);

void get_rr_node_side_and_index_in_sb_info(t_rr_node* cur_rr_node,
                                          t_sb cur_sb_info,
                                          enum PORTS rr_node_direction,
                                          OUTP int* cur_rr_node_side, 
                                          OUTP int* cur_rr_node_index);

void get_chan_rr_node_coordinate_in_sb_info(t_sb cur_sb_info,
                                            int chan_rr_node_side,
                                            t_rr_type* chan_type,
                                            int* chan_rr_node_x, int* chan_rr_node_y);

int get_rr_node_index_in_cb_info(t_rr_node* cur_rr_node,
                                 t_cb cur_cb_info, 
                                 int chan_side, enum PORTS rr_node_direction);

void get_rr_node_side_and_index_in_cb_info(t_rr_node* cur_rr_node,
                                           t_cb cur_cb_info,
                                           enum PORTS rr_node_direction,
                                           OUTP int* cur_rr_node_side, 
                                           OUTP int* cur_rr_node_index);

t_rr_node** get_chan_rr_nodes(int* num_chan_rr_nodes,
                              t_rr_type chan_type,
                              int x, int y,
                              int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                              t_ivec*** LL_rr_node_indices);

t_rr_node** get_grid_side_pin_rr_nodes(int* num_pin_rr_nodes,
                                       t_rr_type pin_type,
                                       int x, int y, int side,
                                       int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                       t_ivec*** LL_rr_node_indices);

void build_one_switch_block_info(t_sb* cur_sb, int sb_x, int sb_y, 
                                 t_det_routing_arch RoutingArch,
                                 int LL_num_rr_nodes,
                                 t_rr_node* LL_rr_node,
                                 t_ivec*** LL_rr_node_indices);

void build_one_connection_block_info(t_cb* cur_cb, int cb_x, int cb_y, t_rr_type cb_type,
                                     int LL_num_rr_nodes,
                                     t_rr_node* LL_rr_node,
                                     t_ivec*** LL_rr_node_indices);

void update_one_grid_pack_prev_node_edge(int x, int y);

void update_grid_pbs_post_route_rr_graph();

void free_backannotate_vpr_post_route_info();

void rec_annotate_pb_type_primitive_node_physical_mode_pin(t_pb_type* top_pb_type,
                                                           t_pb_type* cur_pb_type);

void rec_mark_pb_graph_node_primitive_placement_index_in_top_node(t_pb_graph_node* cur_pb_graph_node,
                                                                  int* start_index);

void rec_link_primitive_pb_graph_node_pin_to_phy_pb_graph_pin(t_pb_graph_node* top_pb_graph_node,
                                                              t_pb_graph_node* cur_pb_graph_node);

void annotate_physical_mode_pins_in_pb_graph_node();

void alloc_and_load_phy_pb_for_mapped_block(int num_mapped_blocks, t_block* mapped_block,
                                            int L_num_vpack_nets, t_net* L_vpack_net);

void spice_backannotate_vpr_post_route_info(t_det_routing_arch RoutingArch,
                                            boolean read_activity_file,
                                            boolean parasitic_net_estimation);

