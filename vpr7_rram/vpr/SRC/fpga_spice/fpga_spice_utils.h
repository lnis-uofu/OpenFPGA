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

char* my_itoa(int input);

char* chomp_spice_node_prefix(char* spice_node_prefix);

char* format_spice_node_prefix(char* spice_node_prefix);

t_port* find_pb_type_port_match_spice_model_port(t_pb_type* pb_type,
                                                 t_spice_model_port* spice_model_port);

char* format_spice_node_prefix(char* spice_node_prefix);

t_port** find_pb_type_ports_match_spice_model_port_type(t_pb_type* pb_type,
                                                        enum e_spice_model_port_type port_type,
                                                        int* port_num);


t_block* search_mapped_block(int x, int y, int z);


int determine_num_sram_bits_mux_basis_subckt(t_spice_model* mux_spice_model,
                                             int mux_size,
                                             int num_input_per_level,
                                             boolean special_basis);

int determine_tree_mux_level(int mux_size);

int determine_num_input_basis_multilevel_mux(int mux_size,
                                             int mux_level);

int tree_mux_last_level_input_num(int num_level,
                                  int mux_size);

int multilevel_mux_last_level_input_num(int num_level, int num_input_per_unit,
                                        int mux_size);

int determine_lut_path_id(int lut_size,
                          int* lut_inputs);

int* decode_onelevel_mux_sram_bits(int fan_in,
                                   int mux_level,
                                   int path_id);

int* decode_multilevel_mux_sram_bits(int fan_in,
                                     int mux_level,
                                     int path_id);

int* decode_tree_mux_sram_bits(int fan_in,
                               int mux_level,
                               int path_id);

void decode_cmos_mux_sram_bits(t_spice_model* mux_spice_model,
                               int mux_size, int path_id, 
                               int* bit_len, int** conf_bits, int* mux_level);

char** my_strtok(char* str, 
                 char* delims, 
                 int* len);

int get_opposite_side(int side);

char* convert_side_index_to_string(int side);

char* convert_chan_type_to_string(t_rr_type chan_type);

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


int find_parent_pb_type_child_index(t_pb_type* parent_pb_type,
                                    int mode_index,
                                    t_pb_type* child_pb_type);

void gen_spice_name_tag_pb_rec(t_pb* cur_pb,
                               char* prefix);

void gen_spice_name_tags_all_pbs();

void check_pb_graph_edge(t_pb_graph_edge pb_graph_edge);

void check_pb_graph_pin_edges(t_pb_graph_pin pb_graph_pin);

void backup_one_pb_rr_node_pack_prev_node_edge(t_rr_node* pb_rr_node);

void update_one_grid_pack_prev_node_edge(int x, int y);

void update_grid_pbs_post_route_rr_graph();

int find_pb_mapped_logical_block_rec(t_pb* cur_pb,
                                     t_spice_model* pb_spice_model, 
                                     char* pb_spice_name_tag);

int find_grid_mapped_logical_block(int x, int y,
                                   t_spice_model* pb_spice_model,
                                   char* pb_spice_name_tag);

void stats_pb_graph_node_port_pin_numbers(t_pb_graph_node* cur_pb_graph_node,
                                          int* num_inputs,
                                          int* num_outputs,
                                          int* num_clock_pins);

void map_clb_pins_to_pb_graph_pins();

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

int find_pb_type_idle_mode_index(t_pb_type cur_pb_type);

int find_pb_type_physical_mode_index(t_pb_type cur_pb_type);

void mark_grid_type_pb_graph_node_pins_temp_net_num(int x, int y);

void assign_pb_graph_node_pin_temp_net_num_by_mode_index(t_pb_graph_pin* cur_pb_graph_pin,
                                                         int mode_index);

void mark_pb_graph_node_input_pins_temp_net_num(t_pb_graph_node* cur_pb_graph_node,
                                                int mode_index);

void mark_pb_graph_node_clock_pins_temp_net_num(t_pb_graph_node* cur_pb_graph_node,
                                                int mode_index);

void mark_pb_graph_node_output_pins_temp_net_num(t_pb_graph_node* cur_pb_graph_node,
                                                int mode_index);

void rec_mark_pb_graph_node_temp_net_num(t_pb_graph_node* cur_pb_graph_node);

void load_one_pb_graph_pin_temp_net_num_from_pb(t_pb* cur_pb,
                                                t_pb_graph_pin* cur_pb_graph_pin);

void load_pb_graph_node_temp_net_num_from_pb(t_pb* cur_pb);

void rec_mark_one_pb_unused_pb_graph_node_temp_net_num(t_pb* cur_pb);

void update_pb_vpack_net_num_from_temp_net_num(t_pb* cur_pb, 
                                               t_pb_graph_pin* cur_pb_graph_pin);

void update_pb_graph_node_temp_net_num_to_pb(t_pb_graph_node* cur_pb_graph_node,
                                             t_pb* cur_pb);

void rec_load_unused_pb_graph_node_temp_net_num_to_pb(t_pb* cur_pb);

void mark_one_pb_parasitic_nets(t_pb* cur_pb);

void init_rr_nodes_vpack_net_num_changed(int LL_num_rr_nodes,
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

int count_num_sram_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        int mux_size);

int count_num_conf_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        enum e_sram_orgz cur_sram_orgz_type,
                                        int mux_size);

int count_num_reserved_conf_bits_one_lut_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type);

int count_num_reserved_conf_bits_one_mux_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type,
                                                     int mux_size);

int count_num_reserved_conf_bits_one_rram_sram_spice_model(t_spice_model* cur_spice_model,
                                                           enum e_sram_orgz cur_sram_orgz_type);

int count_num_reserved_conf_bits_one_spice_model(t_spice_model* cur_spice_model,
                                                 enum e_sram_orgz cur_sram_orgz_type,
                                                 int mux_size);

int count_num_conf_bit_one_interc(t_interconnect* cur_interc,
                                  enum e_sram_orgz cur_sram_orgz_type);

int count_num_reserved_conf_bit_one_interc(t_interconnect* cur_interc,
                                           enum e_sram_orgz cur_sram_orgz_type);

int count_num_conf_bits_pb_type_mode_interc(t_mode* cur_pb_type_mode,
                                            enum e_sram_orgz cur_sram_orgz_type);

int rec_count_num_conf_bits_pb_type_default_mode(t_pb_type* cur_pb_type,
                                                 t_sram_orgz_info* cur_sram_orgz_info);

int rec_count_num_conf_bits_pb_type_physical_mode(t_pb_type* cur_pb_type,
                                                  t_sram_orgz_info* cur_sram_orgz_info);

int rec_count_num_conf_bits_pb(t_pb* cur_pb,
                               t_sram_orgz_info* cur_sram_orgz_info);

void init_one_grid_num_conf_bits(int ix, int iy,
                                 t_sram_orgz_info* cur_sram_orgz_info);

void init_grids_num_conf_bits(t_sram_orgz_info* cur_sram_orgz_info);

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

void rec_count_num_iopads_pb_type_physical_mode(t_pb_type* cur_pb_type);

void rec_count_num_iopads_pb_type_default_mode(t_pb_type* cur_pb_type);

void rec_count_num_iopads_pb(t_pb* cur_pb);

void init_one_grid_num_iopads(int ix, int iy);

void init_grids_num_iopads();

void rec_count_num_mode_bits_pb_type_default_mode(t_pb_type* cur_pb_type);

void rec_count_num_mode_bits_pb(t_pb* cur_pb);

void init_one_grid_num_mode_bits(int ix, int iy);

void init_grids_num_mode_bits();

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

void  
add_mux_scff_conf_bits_to_llist(int mux_size,
                           t_sram_orgz_info* cur_sram_orgz_info, 
                           int num_mux_sram_bits, int* mux_sram_bits,
                           t_spice_model* mux_spice_model);

void 
add_mux_membank_conf_bits_to_llist(int mux_size,
                                   t_sram_orgz_info* cur_sram_orgz_info, 
                                   int num_mux_sram_bits, int* mux_sram_bits,
                                   t_spice_model* mux_spice_model);

void  
add_mux_conf_bits_to_llist(int mux_size,
                           t_sram_orgz_info* cur_sram_orgz_info, 
                           int num_mux_sram_bits, int* mux_sram_bits,
                           t_spice_model* mux_spice_model);

void add_sram_membank_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, int mem_index, 
                                         int num_bls, int num_wls, 
                                         int* bl_conf_bits, int* wl_conf_bits);

void  
add_sram_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, int mem_index, 
                            int num_sram_bits, int* sram_bits);

void find_bl_wl_ports_spice_model(t_spice_model* cur_spice_model,
                                  int* num_bl_ports, t_spice_model_port*** bl_ports,
                                  int* num_wl_ports, t_spice_model_port*** wl_ports);

void find_blb_wlb_ports_spice_model(t_spice_model* cur_spice_model,
                                    int* num_blb_ports, t_spice_model_port*** blb_ports,
                                    int* num_wlb_ports, t_spice_model_port*** wlb_ports);

int* decode_mode_bits(char* mode_bits, int* num_sram_bits);

/* Useful functions for LUT decoding */
void stats_lut_spice_mux(t_llist** muxes_head,
                         t_spice_model* spice_model);

char* complete_truth_table_line(int lut_size,
                                char* input_truth_table_line);

void configure_lut_sram_bits_per_line_rec(int** sram_bits, 
                                          int lut_size,
                                          char* truth_table_line,
                                          int start_point);

int* generate_lut_sram_bits(int truth_table_len,
                            char** truth_table,
                            int lut_size,
                            int default_sram_bit_value);

char** assign_lut_truth_table(t_logical_block* mapped_logical_block,
                              int* truth_table_length);

int get_lut_output_init_val(t_logical_block* lut_logical_block);

int get_logical_block_output_init_val(t_logical_block* cur_logical_block);

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

void free_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                         enum e_sram_orgz cur_sram_orgz_type,
                         int grid_nx, int grid_ny);

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

void init_reserved_syntax_char(t_reserved_syntax_char* cur_reserved_syntax_char,
                               char cur_syntax_char, boolean cur_verilog_reserved, boolean cur_spice_reserved);

void check_mem_model_blwl_inverted(t_spice_model* cur_mem_model, 
                                   enum e_spice_model_port_type blwl_port_type,
                                   boolean* blwl_inverted);

void init_spice_mux_arch(t_spice_model* spice_model,
                         t_spice_mux_arch* spice_mux_arch,
                         int mux_size);

int find_spice_mux_arch_special_basis_size(t_spice_mux_arch spice_mux_arch);

t_llist* search_mux_linked_list(t_llist* mux_head,
                                int mux_size,
                                t_spice_model* spice_model);

void check_and_add_mux_to_linked_list(t_llist** muxes_head,
                                      int mux_size,
                                      t_spice_model* spice_model);

void free_muxes_llist(t_llist* muxes_head);

void stats_spice_muxes_routing_arch(t_llist** muxes_head,
                                    int num_switch,
                                    t_switch_inf* switches,
                                    t_spice* spice,
                                    t_det_routing_arch* routing_arch);

void stats_mux_spice_model_pb_type_rec(t_llist** muxes_head,
                                       t_pb_type* cur_pb_type);

void stats_mux_spice_model_pb_node_rec(t_llist** muxes_head,
                                       t_pb_graph_node* cur_pb_node);

t_llist* stats_spice_muxes(int num_switch,
                           t_switch_inf* switches,
                           t_spice* spice,
                           t_det_routing_arch* routing_arch);

enum e_interconnect find_pb_graph_pin_in_edges_interc_type(t_pb_graph_pin pb_graph_pin);

t_spice_model* find_pb_graph_pin_in_edges_interc_spice_model(t_pb_graph_pin pb_graph_pin);

int find_path_id_between_pb_rr_nodes(t_rr_node* local_rr_graph,
                                     int src_node,
                                     int des_node);

t_pb* get_child_pb_for_phy_pb_graph_node(t_pb* cur_pb, int ipb, int jpb);

void config_spice_model_port_inv_spice_model(int num_spice_models, 
                                             t_spice_model* spice_model);

void config_spice_models_sram_port_spice_model(int num_spice_model,
                                               t_spice_model* spice_models,
                                               t_spice_model* default_sram_spice_model);
t_pb* get_lut_child_pb(t_pb* cur_lut_pb,
                       int mode_index);

t_pb* get_hardlogic_child_pb(t_pb* cur_hardlogic_pb,
                             int mode_index);

int get_grid_pin_height(int grid_x, int grid_y, int pin_index);

int get_grid_pin_side(int grid_x, int grid_y, int pin_index);

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
