
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

void get_rr_node_side_and_index_in_sb_info(t_rr_node* cur_rr_node,
                                          t_sb cur_sb_info,
                                          enum PORTS rr_node_direction,
                                          OUTP int* cur_rr_node_side, 
                                          OUTP int* cur_rr_node_index);

void get_chan_rr_node_coorindate_in_sb_info(t_sb cur_sb_info,
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

void free_backannotate_vpr_post_route_info();

void spice_backannotate_vpr_post_route_info(t_det_routing_arch RoutingArch,
                                            boolean parasitic_net_estimation_off);

void backannotate_vpr_post_route_info(t_det_routing_arch RoutingArch); 

