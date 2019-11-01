#ifndef VERILOG_AUTOCHECK_TOP_TESTBENCH_H
#define VERILOG_AUTOCHECK_TOP_TESTBENCH_H

#include <string>
#include <vector>
#include "module_manager.h"
#include "bitstream_manager.h"
#include "circuit_library.h"

void dump_verilog_autocheck_top_testbench(t_sram_orgz_info* cur_sram_orgz_info,
                                          char* circuit_name,
                                          char* top_netlist_name,
                                          char* verilog_dir_path,
                                          t_syn_verilog_opts fpga_verilog_opts,
                                          t_spice verilog);

/*
void print_verilog_autocheck_top_testbench(const ModuleManager& module_manager,
                                           const BitstreamManager& bitstream_manager,
                                           const CircuitLibrary& circuit_lib,
                                           const std::vector<CircuitPortId>& global_ports,
                                           const std::vector<t_logical_block>& L_logical_blocks,
                                           const vtr::Point<size_t>& device_size,
                                           const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                           const std::vector<t_block>& L_blocks,
                                           const std::string& circuit_name,
                                           const std::string& verilog_fname,
                                           const std::string& verilog_dir,
                                           const t_syn_verilog_opts& fpga_verilog_opts,
                                           const t_spice_params& simulation_parameters);
*/

#endif
