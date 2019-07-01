#ifndef VERILOG_COMPACT_NETLIST_H 
#define VERILOG_COMPACT_NETLIST_H 

void dump_compact_verilog_one_physical_block(t_sram_orgz_info* cur_sram_orgz_info, 
                                             char* verilog_dir_path,
                                             char* subckt_dir_path,
                                             t_type_ptr phy_block_type,
                                             int border_side,
                                             bool is_explicit_mapping);

void dump_compact_verilog_logic_blocks(t_sram_orgz_info* cur_sram_orgz_info,
                                       char* verilog_dir,
                                       char* subckt_dir,
                                       t_arch* arch,
                                       bool is_explicit_mapping);

void dump_compact_verilog_top_netlist(t_sram_orgz_info* cur_sram_orgz_info,
                                      char* circuit_name,
                                      char* top_netlist_name,
                                      char* verilog_dir_path,
                                      char* submodule_dir_path,
                                      char* lb_dir_path,
                                      char* rr_dir_path,
                                      int LL_num_rr_nodes,
                                      t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices,
                                      int num_clock,
                                      boolean compact_routing_hierarchy,
                                      t_spice verilog);
#endif
