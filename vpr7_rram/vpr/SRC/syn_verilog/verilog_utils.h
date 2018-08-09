

void init_list_include_verilog_netlists(t_spice* spice);

void init_include_user_defined_verilog_netlists(t_spice spice);
 
void dump_include_user_defined_verilog_netlists(FILE* fp,
                                                t_spice spice);

void dump_verilog_file_header(FILE* fp,
                              char* usage);

char determine_verilog_generic_port_split_sign(enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_generic_port(FILE* fp, 
                               enum e_dump_verilog_port_type dump_port_type,
                               char* port_name, int port_lsb, int port_msb); 

void decode_verilog_memory_bank_sram(t_spice_model* cur_sram_spice_model, int sram_bit,
                                     int bl_len, int wl_len, int bl_offset, int wl_offset,
                                     int* bl_conf_bits, int* wl_conf_bits);

void 
decode_and_add_verilog_sram_membank_conf_bit_to_llist(t_sram_orgz_info* cur_sram_orgz_info,
                                                      int mem_index,
                                                      int num_bl_per_sram, int num_wl_per_sram, 
                                                      int cur_sram_bit);


void decode_verilog_one_level_4t1r_mux(int path_id, 
                                       int bit_len, int* conf_bits); 

void decode_verilog_rram_mux(t_spice_model* mux_spice_model,
                             int mux_size, int path_id,
                             int* bit_len, int** conf_bits, int* mux_level);

int determine_decoder_size(int num_addr_out);

char* chomp_verilog_node_prefix(char* verilog_node_prefix);

char* format_verilog_node_prefix(char* verilog_node_prefix);

char* verilog_convert_port_type_to_string(enum e_spice_model_port_type port_type);

int rec_dump_verilog_spice_model_global_ports(FILE* fp, 
                                              t_spice_model* cur_spice_model,
                                              boolean dump_port_type,
                                              boolean recursive);

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

void dump_verilog_sram_outports(FILE* fp, 
                                t_sram_orgz_info* cur_sram_orgz_info,
                                int sram_lsb, int sram_msb,
                                enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_one_port(FILE* fp, 
                                t_sram_orgz_info* cur_sram_orgz_info,
                                int sram_lsb, int sram_msb,
                                int port_type_index, 
                                enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_sram_ports(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info,
                             int sram_lsb, int sram_msb,
                             enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_reserved_sram_one_port(FILE* fp, 
                                         t_sram_orgz_info* cur_sram_orgz_info,
                                         int sram_lsb, int sram_msb,
                                         int port_type_index, 
                                         enum e_dump_verilog_port_type dump_port_type);

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
