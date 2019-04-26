
void match_pb_types_verilog_model_rec(t_pb_type* cur_pb_type,
                                    int num_verilog_model,
                                    t_spice_model* verilog_models);

int verilog_find_path_id_between_pb_rr_nodes(t_rr_node* local_rr_graph,
                                     int src_node,
                                     int des_node);

enum e_interconnect verilog_find_pb_graph_pin_in_edges_interc_type(t_pb_graph_pin pb_graph_pin);

t_spice_model* find_pb_graph_pin_in_edges_interc_verilog_model(t_pb_graph_pin pb_graph_pin);

void stats_mux_verilog_model_pb_type_rec(t_llist** muxes_head,
                                       t_pb_type* cur_pb_type);

void stats_mux_verilog_model_pb_node_rec(t_llist** muxes_head,
                                       t_pb_graph_node* cur_pb_node);


void dump_verilog_pb_type_bus_ports(FILE* fp,
                                    char* port_prefix,
                                    int use_global_clock,
                                    t_pb_type* cur_pb_type,
                                    boolean dump_port_type,
                                    boolean dump_last_comma);

void dump_verilog_pb_type_ports(FILE* fp,
                          char* port_prefix,
                          int use_global_clock,
                          t_pb_type* cur_pb_type,
                          boolean dump_port_type,
                          boolean dump_last_comma,
                          boolean require_explicit_port_map);

void dump_verilog_dangling_des_pb_graph_pin_interc(FILE* fp,
                                                   t_pb_graph_pin* des_pb_graph_pin,
                                                   t_mode* cur_mode,
                                                   enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                                   char* parent_pin_prefix);

void generate_verilog_src_des_pb_graph_pin_prefix(t_pb_graph_pin* src_pb_graph_pin,
                                                  t_pb_graph_pin* des_pb_graph_pin,
                                                  enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                                  t_interconnect* pin2pin_interc,
                                                  char* parent_pin_prefix,
                                                  char** src_pin_prefix,
                                                  char** des_pin_prefix);

void verilog_find_interc_fan_in_des_pb_graph_pin(t_pb_graph_pin* des_pb_graph_pin,
                                                 t_mode* cur_mode,
                                                 t_interconnect** cur_interc,
                                                 int* fan_in); 

void dump_verilog_pb_graph_pin_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                      FILE* fp,
                                      char* parent_pin_prefix,
                                      enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                      t_pb_graph_pin* des_pb_graph_pin,
                                      t_mode* cur_mode);

void dump_verilog_pb_graph_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                  FILE* fp, 
                                  char* pin_prefix,
                                  t_pb_graph_node* cur_pb_graph_node,
                                  int select_mode_index);

void dump_verilog_pb_graph_primitive_node(FILE* fp,
                                          char* subckt_prefix,
                                          t_pb_graph_node* cur_pb_graph_node,
                                          int pb_type_index);

void dump_verilog_pb_primitive_verilog_model(t_sram_orgz_info* cur_sram_orgz_info,
                                             FILE* fp,
                                             char* subckt_prefix,
                                             t_pb* prim_pb,
                                             t_pb_graph_node* prim_pb_graph_node,
                                             int pb_index,
                                             t_spice_model* verilog_model,
                                             int is_idle);

void dump_verilog_phy_pb_graph_node_rec(t_sram_orgz_info* cur_sram_orgz_info,
                                        FILE* fp,
                                        char* subckt_prefix,
                                        t_pb_graph_node* cur_pb_graph_node,
                                        int pb_type_index);

void dump_verilog_block(t_sram_orgz_info* cur_sram_orgz_info,
                        FILE* fp,
                        char* subckt_name, 
                        int x,
                        int y,
                        int z,
                        t_type_ptr type_descriptor,
                        t_block* mapped_block);

void dump_verilog_physical_block(t_sram_orgz_info* cur_sram_orgz_info,
                                 FILE* fp,
                                 char* subckt_name, 
                                 int x,
                                 int y,
                                 int z,
                                 t_type_ptr type_descriptor);

void dump_verilog_grid_pins(FILE* fp,
                            int x, int y,
                            boolean top_level,
                            boolean dump_port_type,
                            boolean dump_last_comma);

void dump_verilog_io_grid_pins(FILE* fp,
                               int x, int y,
                               boolean top_level,
                               boolean dump_port_type,
                               boolean dump_last_comma);

char* get_grid_block_subckt_name(int x,
                                 int y,
                                 int z,
                                 char* subckt_prefix,
                                 t_block* mapped_block);

void dump_verilog_grid_block_subckt_pins(FILE* fp,
                                   int z,
                                   t_type_ptr type_descriptor);

void dump_verilog_io_grid_block_subckt_pins(FILE* fp,
                                      int x,
                                      int y,
                                      int z,
                                      t_type_ptr type_descriptor);

void dump_verilog_grid_blocks(t_sram_orgz_info* cur_sram_orgz_info,
                              FILE* fp,
                              int ix, int iy);

void dump_verilog_physical_grid_blocks(t_sram_orgz_info* cur_sram_orgz_info,
                                        FILE* fp,
                                        int ix, int iy,
                                        t_arch* arch);

void dump_verilog_idle_block(t_sram_orgz_info* cur_sram_orgz_info,
                             FILE* fp,
                             char* subckt_name, 
                             int x, int y, int z,
                             t_type_ptr type_descriptor);

void dump_verilog_logic_blocks(t_sram_orgz_info* cur_sram_orgz_info,
                               char* subckt_dir, t_arch* arch);

void rec_copy_name_mux_in_node(t_pb_graph_node* master_node, 
                           t_pb_graph_node* target_node);
