

void init_list_include_verilog_netlists(t_spice* spice);

void init_include_user_defined_verilog_netlists(t_spice spice);
 
void dump_include_user_defined_verilog_netlists(FILE* fp,
                                                t_spice spice);

void dump_verilog_file_header(FILE* fp,
                              char* usage);

void dump_verilog_preproc(FILE* fp, 
                          t_syn_verilog_opts fpga_verilog_opts,
                          enum e_verilog_tb_type verilog_tb_type);

void dump_simulation_preproc(FILE* fp, 
                          t_syn_verilog_opts fpga_verilog_opts,
                          enum e_verilog_tb_type verilog_tb_type);

void dump_verilog_simulation_preproc(char* subckt_dir,
                                     t_syn_verilog_opts fpga_verilog_opts);

void dump_verilog_defines_preproc(char* subckt_dir,
                                 t_syn_verilog_opts fpga_verilog_opts);

void verilog_include_simulation_defines_file(FILE* fp, 
                                             char* formatted_verilog_dir);

void verilog_include_defines_preproc_file(FILE* fp, 
                                          char* formatted_verilog_dir);

FILE* verilog_create_one_subckt_file(char* subckt_dir,
                                     const char* subckt_name_prefix,
                                     const char* verilog_subckt_file_name_prefix,
                                     int grid_x, int grid_y,
                                     char** verilog_fname); 

void dump_verilog_subckt_header_file(t_llist* subckt_llist_head,
                                     char* subckt_dir,
                                     char* header_file_name);

char determine_verilog_generic_port_split_sign(enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_generic_port(FILE* fp, 
                               enum e_dump_verilog_port_type dump_port_type,
                               char* port_name, int port_lsb, int port_msb); 

void dump_verilog_generic_port_no_repeat(FILE* fp, 
                               enum e_dump_verilog_port_type dump_port_type,
                               char* port_name, int port_lsb, int port_msb); 


char* chomp_verilog_prefix(char* verilog_node_prefix);

char* format_verilog_node_prefix(char* verilog_node_prefix);

char* verilog_convert_port_type_to_string(enum e_spice_model_port_type port_type);

int rec_dump_verilog_spice_model_lib_global_ports(FILE* fp, 
                                                  t_spice_model* cur_spice_model,
                                                  boolean dump_port_type, 
                                                  boolean recursive,
                                                  boolean require_explicit_port_map);

int rec_dump_verilog_spice_model_global_ports(FILE* fp, 
                                              t_spice_model* cur_spice_model,
                                              boolean dump_port_type,
                                              boolean recursive,
                                              boolean require_explicit_port_map);

int dump_verilog_global_ports(FILE* fp, t_llist* head,
                              boolean dump_port_type);

void dump_verilog_mux_sram_one_outport(FILE* fp, 
                                       t_sram_orgz_info* cur_sram_orgz_info,
                                       t_spice_model* cur_mux_spice_model, int mux_size,
                                       int sram_lsb, int sram_msb,
                                       int port_type_index, 
                                       enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_one_outport(FILE* fp, 
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   int sram_lsb, int sram_msb,
                                   int port_type_index, 
                                   enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_mux_sram_one_local_outport(FILE* fp, 
                                             t_sram_orgz_info* cur_sram_orgz_info,
                                             t_spice_model* cur_mux_spice_model, int mux_size,
                                             int sram_lsb, int sram_msb,
                                             int port_type_index, 
                                             enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_one_local_outport(FILE* fp, 
                                         t_sram_orgz_info* cur_sram_orgz_info,
                                         int sram_lsb, int sram_msb,
                                         int port_type_index, 
                                         enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_outports(FILE* fp, 
                                t_sram_orgz_info* cur_sram_orgz_info,
                                int sram_lsb, int sram_msb,
                                enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_formal_verification_sram_ports(FILE* fp, 
                                                 t_sram_orgz_info* cur_sram_orgz_info,
                                                 int sram_lsb, int sram_msb,
                                                 enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_one_port(FILE* fp, 
                                t_sram_orgz_info* cur_sram_orgz_info,
                                int sram_lsb, int sram_msb,
                                int port_type_index, 
                                enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_local_ports(FILE* fp, 
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   int sram_lsb, int sram_msb,
                                   enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_ports(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info,
                             int sram_lsb, int sram_msb,
                             enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_reserved_sram_one_port(FILE* fp, 
                                         t_sram_orgz_info* cur_sram_orgz_info,
                                         int sram_lsb, int sram_msb,
                                         int port_type_index, 
                                         enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_formal_verification_sram_ports_wiring(FILE* fp, 
                                                        t_sram_orgz_info* cur_sram_orgz_info,
                                                        int sram_lsb, int sram_msb);

void dump_verilog_formal_verification_mux_sram_ports_wiring(FILE* fp, 
                                                            t_sram_orgz_info* cur_sram_orgz_info,
                                                            t_spice_model* cur_mux_spice_model, int mux_size,
                                                            int sram_lsb, int sram_msb);

void dump_verilog_reserved_sram_ports(FILE* fp, 
                                      t_sram_orgz_info* cur_sram_orgz_info,
                                      int sram_lsb, int sram_msb,
                                      enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_mux_sram_submodule(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info,
                                     t_spice_model* cur_mux_verilog_model, int mux_size,
                                     t_spice_model* cur_sram_verilog_model);

void dump_verilog_sram_submodule(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info,
                                 t_spice_model* sram_verilog_model);

void dump_verilog_scff_config_bus(FILE* fp,
                                 t_spice_model* mem_spice_model, 
                                 t_sram_orgz_info* cur_sram_orgz_info,
                                 int lsb, int msb,
                                 enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_mem_config_bus(FILE* fp, t_spice_model* mem_spice_model, 
                                 t_sram_orgz_info* cur_sram_orgz_info,
                                 int cur_num_sram,
                                 int num_mem_reserved_conf_bits,
                                 int num_mem_conf_bits); 

void dump_verilog_cmos_mux_config_bus(FILE* fp, t_spice_model* mux_spice_model, 
                                      t_sram_orgz_info* cur_sram_orgz_info,
                                      int mux_size, int cur_num_sram,
                                      int num_mux_reserved_conf_bits,
                                      int num_mux_conf_bits); 

void dump_verilog_mux_config_bus(FILE* fp, t_spice_model* mux_spice_model, 
                                 t_sram_orgz_info* cur_sram_orgz_info,
                                 int mux_size, int cur_num_sram,
                                 int num_mux_reserved_conf_bits,
                                 int num_mux_conf_bits); 

void dump_verilog_cmos_mux_config_bus_ports(FILE* fp, t_spice_model* mux_spice_model, 
                                            t_sram_orgz_info* cur_sram_orgz_info,
                                            int mux_size, int cur_num_sram,
                                            int num_mux_reserved_conf_bits,
                                            int num_mux_conf_bits); 

void dump_verilog_mux_config_bus_ports(FILE* fp, t_spice_model* mux_spice_model, 
                                       t_sram_orgz_info* cur_sram_orgz_info,
                                       int mux_size, int cur_num_sram,
                                       int num_mux_reserved_conf_bits,
                                       int num_mux_conf_bits); 

void dump_verilog_grid_common_port(FILE* fp, t_spice_model* cur_verilog_model,
                                   char* general_port_prefix, int lsb, int msb,
                                   enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_config_bus_internal_wires(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info, 
                                                 int lsb, int msb);

void dump_verilog_toplevel_one_grid_side_pin_with_given_index(FILE* fp, t_rr_type pin_type, 
                                                              int pin_index, int side,
                                                              int x, int y,
                                                              boolean dump_port_type);

char* generate_verilog_subckt_name(t_spice_model* spice_model, 
                                   char* postfix);

char* generate_verilog_mem_subckt_name(t_spice_model* spice_model, 
                                       t_spice_model* mem_model,
                                       char* postfix);

char* generate_verilog_mux_subckt_name(t_spice_model* spice_model, 
                                       int mux_size, char* postfix);

enum e_dump_verilog_port_type 
convert_spice_model_port_type_to_verilog_port_type(enum e_spice_model_port_type spice_model_port_type);

int dump_verilog_mem_module_one_port_map(FILE* fp,
                                         t_spice_model* mem_model,
                                         enum e_spice_model_port_type port_type_to_dump,
                                         boolean dump_port_type,
                                         int index, int num_mem, boolean dump_first_comma,
                                         boolean require_explicit_port_map);

void dump_verilog_mem_module_port_map(FILE* fp, 
                                      t_spice_model* mem_model,
                                      boolean dump_port_type,
                                      int lsb, int num_mem,
                                      boolean require_explicit_port_map);

void dump_verilog_mem_sram_submodule(FILE* fp,
                                     t_sram_orgz_info* cur_sram_orgz_info,
                                     t_spice_model* cur_verilog_model, int mux_size,
                                     t_spice_model* cur_sram_verilog_model,
                                     int lsb, int msb);

char* gen_verilog_grid_one_pin_name(int x, int y,
                                   int height, int side, int pin_index,
                                   boolean for_top_netlist);

char* gen_verilog_routing_channel_one_pin_name(t_rr_node* chan_rr_node,
                                               int x, int y, int track_idx,
                                               enum PORTS pin_direction);

char* gen_verilog_routing_channel_one_midout_name(t_cb* cur_cb_info,
                                                 int track_idx);

char* gen_verilog_one_cb_module_name(t_cb* cur_cb_info);

char* gen_verilog_one_cb_instance_name(t_cb* cur_cb_info);

char* gen_verilog_one_sb_module_name(t_sb* cur_sb_info);

char* gen_verilog_one_sb_module_name(size_t rr_sb_id);

char* gen_verilog_one_sb_instance_name(t_sb* cur_sb_info);

char* gen_verilog_one_routing_channel_module_name(t_rr_type chan_type,
                                                  int x, int y);

char* gen_verilog_one_routing_channel_instance_name(t_rr_type chan_type,
                                                    int x, int y);

char* gen_verilog_one_mux_module_name(t_spice_model* spice_model, 
                                      int mux_size);

char* gen_verilog_one_grid_instance_name(int grid_x, int grid_y);

char* gen_verilog_one_grid_module_name(int grid_x, int grid_y);

char* gen_verilog_one_block_instance_name(int grid_x, int grid_y, int grid_z);

char* gen_verilog_one_phy_block_instance_name(t_type_ptr cur_type_ptr, 
                                              int block_z);

char* gen_verilog_one_pb_graph_node_instance_name(t_pb_graph_node* cur_pb_graph_node);

char* gen_verilog_one_pb_type_pin_name(char* prefix, 
                                       t_port* cur_port, int pin_number);

char* gen_verilog_one_pb_graph_pin_full_name_in_hierarchy(t_pb_graph_pin* cur_pb_graph_pin);
char* gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_parent_node(t_pb_graph_pin* cur_pb_graph_pin);
char* gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_grand_parent_node(t_pb_graph_pin* cur_pb_graph_pin); 

char* gen_verilog_top_module_io_port_prefix(char* global_prefix, 
                                            char* io_port_prefix);

char* gen_verilog_one_pb_graph_node_full_name_in_hierarchy(t_pb_graph_node* cur_pb_graph_node);
