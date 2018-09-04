
void my_free(void* ptr);

int pin_side_count(int pin_side[]);

void find_blk_net_pin_sides(t_block target_blk,
                            int pin_height,
                            int pin_index,
                            int** pin_side);

int find_blk_net_pin_side(t_block target_blk,
                          int pin_height,
                          int pin_index);

void find_blk_net_type_pins(int n_blks, t_block* blk,
                            int net_index,
                            int blk_index,
                            int* num_pins,
                            enum e_side** pin_side,
                            int** pin_index);

int check_src_blk_pin(int n_blks, t_block* blk,
                      int net_index,
                      int src_blk_index,
                      //int src_blk_port_index,
                      int src_blk_pin_index);

int check_des_blk_pin(int n_blks, t_block* blk,
                      int net_index,
                      int des_blk_index,
                      //int des_blk_port_index,
                      int des_blk_pin_index);

void esti_pin_chan_coordinate(int* pin_x, int* pin_y,
                              t_block blk, int pin_side, int pin_index);


int count_blk_one_class_num_conflict(t_block* targert_blk, int class_index,
                                     int* is_pin_conflict);

int* sort_one_class_conflict_pins_by_low_slack(t_block* target_blk, int class_index,
                                               int* is_pin_conflict, int* num_conflict, 
                                               t_slack* slack);

int is_swap2pins_match_prefer_side(int pin0_cur_side, int* pin0_prefer_side,
                                   int pin1_cur_side, int* pin1_prefer_side);

int is_type_pin_in_class(t_type_ptr type,
                         int class_index, int pin_index);

void find_src_pb_pin_to_rr_nodes(t_pb* src_pb,
                                 int src_rr_node_index,
                                 int* num_pin_to_rr_nodes,
                                 t_rr_node*** to_node);

void connect_pb_des_pin_to_src_pin(t_pb* src_pb,
                                   int src_rr_node_index,
                                   int des_rr_node_index);

void swap_blk_same_class_2pins(t_block* target_blk, int n_nets, t_net* nets,
                               int pin0_index, int pin1_index);

void mark_blk_pins_nets_sink_index(int n_nets, t_net* nets,
                                   int n_blks, t_block* blk);

void rec_update_net_info_local_rr_node_tree(t_pb* src_pb,
                                            int src_node_index);
