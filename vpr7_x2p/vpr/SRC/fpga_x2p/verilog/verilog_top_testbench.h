#ifndef VERILOG_TOP_TESTBENCH
#define VERILOG_TOP_TESTBENCH

#include <string>
#include <vector>
#include "module_manager.h"
#include "bitstream_manager.h"
#include "circuit_library.h"

void print_verilog_top_testbench(const ModuleManager& module_manager,
                                 const BitstreamManager& bitstream_manager,
                                 const std::vector<ConfigBitId>& fabric_bitstream,
                                 const e_sram_orgz& sram_orgz_type,
                                 const CircuitLibrary& circuit_lib,
                                 const std::vector<CircuitPortId>& global_ports,
                                 const std::vector<t_logical_block>& L_logical_blocks,
                                 const vtr::Point<size_t>& device_size,
                                 const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                 const std::vector<t_block>& L_blocks,
                                 const std::string& circuit_name,
                                 const std::string& verilog_fname,
                                 const std::string& verilog_dir,
                                 const t_spice_params& simulation_parameters);

void dump_verilog_top_testbench_global_ports(FILE* fp, t_llist* head,
                                             enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_top_testbench_global_ports_stimuli(FILE* fp, t_llist* head);

void dump_verilog_top_testbench_call_top_module(t_sram_orgz_info* cur_sram_orgz_info,
                                                FILE* fp,
                                                char* circuit_name,
                                                bool is_explicit_mapping);

void dump_verilog_top_testbench_stimuli(t_sram_orgz_info* cur_sram_orgz_info, 
                                        FILE* fp,
                                        t_spice verilog);

void dump_verilog_top_testbench(t_sram_orgz_info* cur_sram_orgz_info,
                                char* circuit_name,
                                const char* top_netlist_name,
                                char* verilog_dir_path,
                                t_spice verilog);

void dump_verilog_input_blif_testbench(char* circuit_name,
                                       char* top_netlist_name,
                                       char* verilog_dir_path,
                                       t_spice verilog);

#endif
