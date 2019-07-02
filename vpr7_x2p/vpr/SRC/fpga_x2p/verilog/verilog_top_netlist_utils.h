
void dump_verilog_top_netlist_ports(t_sram_orgz_info* cur_sram_orgz_info,
                                    FILE* fp,
                                    int num_clocks,
                                    char* circuit_name,
                                    t_spice verilog);

void dump_verilog_top_netlist_internal_wires(t_sram_orgz_info* cur_sram_orgz_info,
                                             FILE* fp);

void dump_verilog_defined_channels(FILE* fp,
                                   int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                   t_ivec*** LL_rr_node_indices);

void dump_verilog_defined_connection_boxes(t_sram_orgz_info* cur_sram_orgz_info,
                                           FILE* fp);

void dump_verilog_defined_switch_boxes(t_sram_orgz_info* cur_sram_orgz_info,
                                       FILE* fp);

void dump_verilog_clb2clb_directs(FILE* fp, 
                                  int num_directs, t_clb_to_clb_directs* direct);

void dump_verilog_configuration_circuits(t_sram_orgz_info* cur_sram_orgz_info,
                                         FILE* fp);

void dump_verilog_top_module_ports(t_sram_orgz_info* cur_sram_orgz_info, 
                                   FILE* fp,
                                   enum e_dump_verilog_port_type dump_port_type);

void verilog_compact_generate_fake_xy_for_io_border_side(int border_side,  
                                                         int* ix, int* iy) ;

void dump_compact_verilog_grid_pins(FILE* fp,
                                    t_type_ptr grid_type_descriptor,
                                    boolean dump_port_type,
                                    boolean dump_last_comma) ;

void dump_compact_verilog_io_grid_pins(FILE* fp,
                                       t_type_ptr grid_type_descriptor,
                                       int border_side,
                                       boolean dump_port_type,
                                       boolean dump_last_comma) ;

char* compact_verilog_get_grid_phy_block_subckt_name(t_type_ptr grid_type_descriptor,
                                                     int z,
                                                     char* subckt_prefix);

void dump_compact_verilog_io_grid_block_subckt_pins(FILE* fp,
                                                    t_type_ptr grid_type_descriptor,
                                                    int border_side,
                                                    int z) ;


