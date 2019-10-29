#ifndef VERILOG_PRECONFIG_TOP_MODULE_H
#define VERILOG_PRECONFIG_TOP_MODULE_H

#include <vector>
#include <string>
#include "spice_types.h" 
#include "vpr_types.h" 
#include "module_manager.h" 
#include "bitstream_manager.h" 

void print_verilog_preconfig_top_module(const ModuleManager& module_manager,
                                        const BitstreamManager& bitstream_manager,
                                        const std::vector<ConfigBitId>& fabric_bitstream,
                                        const CircuitLibrary& circuit_lib,
                                        const std::vector<CircuitPortId>& global_ports,
                                        const std::vector<t_logical_block>& L_logical_blocks,
                                        const vtr::Point<size_t>& device_size,
                                        const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                        const std::vector<t_block>& L_blocks,
                                        const std::string& circuit_name,
                                        const std::string& verilog_fname,
                                        const std::string& verilog_dir);

#endif
