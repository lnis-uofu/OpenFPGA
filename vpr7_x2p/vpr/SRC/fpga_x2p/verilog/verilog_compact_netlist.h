#ifndef VERILOG_COMPACT_NETLIST_H 
#define VERILOG_COMPACT_NETLIST_H 

void print_verilog_physical_block(ModuleManager& module_manager,
                                  const MuxLibrary& mux_lib,
                                  const CircuitLibrary& circuit_lib,
                                  t_sram_orgz_info* cur_sram_orgz_info, 
                                  const std::string& verilog_dir_path,
                                  const std::string& subckt_dir_path,
                                  t_type_ptr phy_block_type,
                                  const e_side& border_side,
                                  const bool& use_explicit_mapping);

void dump_compact_verilog_one_physical_block(t_sram_orgz_info* cur_sram_orgz_info, 
                                             char* verilog_dir_path,
                                             char* subckt_dir_path,
                                             t_type_ptr phy_block_type,
                                             int border_side,
                                             bool is_explicit_mapping);

void print_compact_verilog_logic_blocks(ModuleManager& module_manager,
                                        const MuxLibrary& mux_lib,
                                        t_sram_orgz_info* cur_sram_orgz_info,
                                        char* verilog_dir,
                                        char* subckt_dir,
                                        t_arch& arch,
                                        const bool& is_explicit_mapping);

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
                                      t_spice verilog,
                                      bool is_explicit_verilog);
#endif
