#ifndef FPGA_X2P_UTILS_H
#define FPGA_X2P_UTILS_H

void my_free(void* ptr);

char* my_gettime();

char* format_dir_path(char* dir_path);

int try_access_file(char* file_path);

void my_remove_file(char* file_path);

int create_dir_path(char* dir_path);

char* my_strcat(char* str1,
                char* str2);

int split_path_prog_name(char* prog_path,
                         char  split_token,
                         char** ret_path,
                         char** ret_prog_name);

char* chomp_file_name_postfix(char* file_name);

void fprint_commented_sram_bits(FILE* fp,
                                int num_sram_bits, int* sram_bits);

t_spice_model* find_name_matched_spice_model(char* spice_model_name,
                                             int num_spice_model,
                                             t_spice_model* spice_models);

t_spice_model* get_default_spice_model(enum e_spice_model_type default_spice_model_type,
                                       int num_spice_model,
                                       t_spice_model* spice_models);

t_spice_model_port* find_spice_model_port_by_name(t_spice_model* cur_spice_model,
                                                  char* port_name);

void config_one_spice_model_buffer(int num_spice_models, 
                                   t_spice_model* spice_model,
                                   t_spice_model* cur_spice_model,
                                   t_spice_model_buffer* cur_spice_model_buffer);

void config_spice_model_input_output_buffers_pass_gate(int num_spice_models, 
                                                       t_spice_model* spice_model);

t_spice_model_port** find_spice_model_ports(t_spice_model* spice_model,
                                            enum e_spice_model_port_type port_type,
                                            int* port_num, boolean ignore_global_port);

t_spice_model_port** find_spice_model_config_done_ports(t_spice_model* spice_model,
                                                        enum e_spice_model_port_type port_type,
                                                        int* port_num, boolean ignore_global_port);


t_spice_transistor_type* find_mosfet_tech_lib(t_spice_tech_lib tech_lib,
                                              e_spice_trans_type trans_type);

char* my_itobin(int in_int, int bin_len);

char* my_itoa(int input);

char* fpga_spice_create_one_subckt_filename(char* file_name_prefix,
                                            int subckt_x, int subckt_y,
                                            char* file_name_postfix);

char* chomp_spice_node_prefix(char* spice_node_prefix);

char* format_spice_node_prefix(char* spice_node_prefix);

t_block* search_mapped_block(int x, int y, int z);

char** fpga_spice_strtok(char* str, 
                         char* delims, 
                         int* len);

int get_opposite_side(int side);

char* convert_side_index_to_string(int side);

char* convert_process_corner_to_string(enum e_process_corner process_corner);

char* convert_chan_type_to_string(t_rr_type chan_type);

char* convert_cb_type_to_string(t_rr_type chan_type);

char* convert_chan_rr_node_direction_to_string(enum PORTS chan_rr_node_direction);

void init_spice_net_info(t_spice_net_info* spice_net_info);

t_spice_model* find_iopad_spice_model(int num_spice_model,
                                      t_spice_model* spice_models);

boolean is_grid_coordinate_in_range(int x_min,
                                    int x_max, 
                                    int grid_x);

char* generate_string_spice_model_type(enum e_spice_model_type spice_model_type);

int determine_io_grid_side(int x,
                           int y);

void find_prev_rr_nodes_with_src(t_rr_node* src_rr_node,
                                 int* num_drive_rr_nodes,
                                 t_rr_node*** drive_rr_nodes,
                                 int** switch_indices);

int find_path_id_prev_rr_node(int num_drive_rr_nodes,
                              t_rr_node** drive_rr_nodes,
                              t_rr_node* src_rr_node);

int pb_pin_net_num(t_rr_node* pb_rr_graph, 
                   t_pb_graph_pin* pin);

float pb_pin_density(t_rr_node* pb_rr_graph, 
                     t_pb_graph_pin* pin);

float pb_pin_probability(t_rr_node* pb_rr_graph, 
                         t_pb_graph_pin* pin);

int pb_pin_init_value(t_rr_node* pb_rr_graph, 
                      t_pb_graph_pin* pin);

float get_rr_node_net_density(t_rr_node node);

float get_rr_node_net_probability(t_rr_node node);

int get_rr_node_net_init_value(t_rr_node node);

int recommend_num_sim_clock_cycle();

void auto_select_num_sim_clock_cycle(t_spice* spice,
                                     float signal_density_weight);

void alloc_spice_model_grid_index_low_high(t_spice_model* cur_spice_model);

void free_one_spice_model_grid_index_low_high(t_spice_model* cur_spice_model);

void free_spice_model_grid_index_low_high(int num_spice_models, 
                                          t_spice_model* spice_model);

void update_one_spice_model_grid_index_low(int x, int y, 
                                           t_spice_model* cur_spice_model);

void update_spice_models_grid_index_low(int x, int y, 
                                        int num_spice_models, 
                                        t_spice_model* spice_model);

void update_one_spice_model_grid_index_high(int x, int y, 
                                           t_spice_model* cur_spice_model);

void update_spice_models_grid_index_high(int x, int y, 
                                        int num_spice_models, 
                                        t_spice_model* spice_model);

void zero_one_spice_model_grid_index_low_high(t_spice_model* cur_spice_model);

void zero_spice_model_grid_index_low_high(int num_spice_models, 
                                          t_spice_model* spice_model);

char* gen_str_spice_model_structure(enum e_spice_model_structure spice_model_structure);

boolean check_spice_model_structure_match_switch_inf(t_switch_inf target_switch_inf);


void init_rr_nodes_vpack_net_num_changed(int LL_num_rr_nodes,
                                         t_rr_node* LL_rr_node);

void init_rr_nodes_is_parasitic_net(int LL_num_rr_nodes,
                                    t_rr_node* LL_rr_node);

boolean is_net_pi(t_net* cur_net);

int check_consistency_logical_block_net_num(t_logical_block* lgk_blk, 
                                            int num_inputs, int* input_net_num);

int rr_node_drive_switch_box(t_rr_node* src_rr_node,
                             t_rr_node* des_rr_node,
                             int switch_box_x,
                             int switch_box_y,
                             int chan_side);

void find_drive_rr_nodes_switch_box(int switch_box_x,
                                    int switch_box_y,
                                    t_rr_node* src_rr_node,
                                    int chan_side,
                                    int return_num_only,
                                    int* num_drive_rr_nodes,
                                    t_rr_node*** drive_rr_nodes,
                                    int* switch_index);

void zero_spice_models_cnt(int num_spice_models, t_spice_model* spice_model);

void zero_one_spice_model_routing_index_low_high(t_spice_model* cur_spice_model);

void zero_spice_models_routing_index_low_high(int num_spice_models, 
                                              t_spice_model* spice_model);

void alloc_spice_model_routing_index_low_high(t_spice_model* cur_spice_model);

void free_one_spice_model_routing_index_low_high(t_spice_model* cur_spice_model);

void free_spice_model_routing_index_low_high(int num_spice_models, 
                                             t_spice_model* spice_model);

void update_one_spice_model_routing_index_high(int x, int y, t_rr_type chan_type,
                                               t_spice_model* cur_spice_model);

void update_spice_models_routing_index_high(int x, int y, t_rr_type chan_type, 
                                            int num_spice_models, 
                                            t_spice_model* spice_model);

void update_one_spice_model_routing_index_low(int x, int y, t_rr_type chan_type,
                                               t_spice_model* cur_spice_model);

void update_spice_models_routing_index_low(int x, int y, t_rr_type chan_type,
                                           int num_spice_models, 
                                           t_spice_model* spice_model);


void check_sram_spice_model_ports(t_spice_model* cur_spice_model,
                                  boolean include_bl_wl);

void check_ff_spice_model_ports(t_spice_model* cur_spice_model,
                                boolean is_scff);

/* Functions to manipulate t_conf_bit and t_conf_bit_info */
void free_conf_bit(t_conf_bit* conf_bit);
void free_conf_bit_info(t_conf_bit_info* conf_bit_info);

t_conf_bit_info*  
alloc_one_conf_bit_info(int index,
                        t_conf_bit* sram_val,
                        t_conf_bit* bl_val, t_conf_bit* wl_val,
                        t_spice_model* parent_spice_model);

t_llist* 
add_conf_bit_info_to_llist(t_llist* head, int index, 
                           t_conf_bit* sram_val, t_conf_bit* bl_val, t_conf_bit* wl_val,
                           t_spice_model* parent_spice_model);


void find_bl_wl_ports_spice_model(t_spice_model* cur_spice_model,
                                  int* num_bl_ports, t_spice_model_port*** bl_ports,
                                  int* num_wl_ports, t_spice_model_port*** wl_ports);

void find_blb_wlb_ports_spice_model(t_spice_model* cur_spice_model,
                                    int* num_blb_ports, t_spice_model_port*** blb_ports,
                                    int* num_wlb_ports, t_spice_model_port*** wlb_ports);



/* Functions to manipulate structs of SRAM orgz */
t_sram_orgz_info* alloc_one_sram_orgz_info();

t_mem_bank_info* alloc_one_mem_bank_info();

void free_one_mem_bank_info(t_mem_bank_info* mem_bank_info);

t_scff_info* alloc_one_scff_info();

void free_one_scff_info(t_scff_info* scff_info);

t_standalone_sram_info* alloc_one_standalone_sram_info();

void free_one_standalone_sram_info(t_standalone_sram_info* standalone_sram_info);

void init_mem_bank_info(t_mem_bank_info* cur_mem_bank_info,
                        t_spice_model* cur_mem_model);

void try_update_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                             int updated_reserved_bl, int updated_reserved_wl);

void update_mem_bank_info_reserved_blwl(t_mem_bank_info* cur_mem_bank_info,
                                        int updated_reserved_bl, int updated_reserved_wl);

void get_mem_bank_info_reserved_blwl(t_mem_bank_info* cur_mem_bank_info,
                                     int* num_reserved_bl, int* num_reserved_wl);

void update_mem_bank_info_num_blwl(t_mem_bank_info* cur_mem_bank_info,
                                   int updated_bl, int updated_wl);

void get_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                      int* num_reserved_bl, int* num_reserved_wl);

void update_mem_bank_info_num_mem_bit(t_mem_bank_info* cur_mem_bank_info,
                                      int num_mem_bit);

void init_scff_info(t_scff_info* cur_scff_info,
                    t_spice_model* cur_mem_model);

void update_scff_info_num_mem_bit(t_scff_info* cur_scff_info,
                                  int num_mem_bit);

void init_standalone_sram_info(t_standalone_sram_info* cur_standalone_sram_info,
                               t_spice_model* cur_mem_model);

void update_standalone_sram_info_num_mem_bit(t_standalone_sram_info* cur_standalone_sram_info,
                                             int num_mem_bit);

void init_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                         enum e_sram_orgz cur_sram_orgz_type,
                         t_spice_model* cur_mem_model, 
                         int grid_nx, int grid_ny);

t_sram_orgz_info* snapshot_sram_orgz_info(t_sram_orgz_info* src_sram_orgz_info);

t_sram_orgz_info* diff_sram_orgz_info(t_sram_orgz_info* des_sram_orgz_info, 
                                      t_sram_orgz_info* base_sram_orgz_info);

void free_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                         enum e_sram_orgz cur_sram_orgz_type);

void update_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                         int updated_reserved_bl, int updated_reserved_wl);

int get_sram_orgz_info_num_mem_bit(t_sram_orgz_info* cur_sram_orgz_info);

void get_sram_orgz_info_num_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                int* cur_bl, int* cur_wl);

void update_sram_orgz_info_num_mem_bit(t_sram_orgz_info* cur_sram_orgz_info,
                                       int new_num_mem_bit);

void update_sram_orgz_info_num_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                    int new_bl, int new_wl);

void get_sram_orgz_info_mem_model(t_sram_orgz_info* cur_sram_orgz_info,
                                  t_spice_model** mem_model_ptr);

void update_sram_orgz_info_mem_model(t_sram_orgz_info* cur_sram_orgz_info,
                                     t_spice_model* cur_mem_model);

void copy_sram_orgz_info(t_sram_orgz_info* des_sram_orgz_info,
                         t_sram_orgz_info* src_sram_orgz_info);

void init_reserved_syntax_char(t_reserved_syntax_char* cur_reserved_syntax_char,
                               char cur_syntax_char, boolean cur_verilog_reserved, boolean cur_spice_reserved);

void check_mem_model_blwl_inverted(t_spice_model* cur_mem_model, 
                                   enum e_spice_model_port_type blwl_port_type,
                                   boolean* blwl_inverted);


void config_spice_model_port_inv_spice_model(int num_spice_models, 
                                             t_spice_model* spice_model);

void config_spice_models_sram_port_spice_model(int num_spice_model,
                                               t_spice_model* spice_models,
                                               t_spice_model* default_sram_spice_model);

void determine_sb_port_coordinator(t_sb cur_sb_info, int side, 
                                   int* port_x, int* port_y);

void init_spice_models_tb_cnt(int num_spice_models,
                              t_spice_model* spice_model);

void init_spice_models_grid_tb_cnt(int num_spice_models,
                                   t_spice_model* spice_model,
                                   int grid_x, int grid_y);

void check_spice_models_grid_tb_cnt(int num_spice_models,
                                    t_spice_model* spice_model,
                                    int grid_x, int grid_y,
                                    enum e_spice_model_type spice_model_type_to_check);

boolean check_negative_variation(float avg_val, 
                                 t_spice_mc_variation_params variation_params);

boolean is_cb_exist(t_rr_type cb_type,
                    int cb_x, int cb_y);

int count_cb_info_num_ipin_rr_nodes(t_cb cur_cb_info);

t_llist* add_one_subckt_file_name_to_llist(t_llist* cur_head, 
                                            char* subckt_file_path);

boolean check_subckt_file_exist_in_llist(t_llist* subckt_llist_head,
                                         char* subckt_file_name);

boolean is_primitive_pb_type(t_pb_type* cur_pb_type);

void rec_stats_spice_model_global_ports(t_spice_model* cur_spice_model,
                                        boolean recursive,
                                        t_llist** spice_model_head);

int* snapshot_spice_model_counter(int num_spice_models,
                                  t_spice_model* spice_model);

void set_spice_model_counter(int num_spice_models,
                             t_spice_model* spice_model,
                             int* spice_model_counter);

void get_logical_block_output_vpack_net_num(INP t_logical_block* cur_logical_block,
                                            OUTP int* num_lb_output_ports, OUTP int** num_lb_output_pins, 
                                            OUTP int*** lb_output_vpack_net_num);

int get_lut_logical_block_index_with_output_vpack_net_num(int target_vpack_net_num);

void get_fpga_x2p_global_op_clock_ports(t_llist* head,
                                        int* num_clock_ports,
                                        t_spice_model_port*** clock_port);

void get_fpga_x2p_global_all_clock_ports(t_llist* head,
                                        int* num_clock_ports,
                                        t_spice_model_port*** clock_port);

#endif
