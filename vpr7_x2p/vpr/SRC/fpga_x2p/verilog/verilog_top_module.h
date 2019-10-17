/********************************************************************
 * Header file for verilog_top_module.cpp
 *******************************************************************/
#ifndef VERILOG_TOP_MODULE_H
#define VERILOG_TOP_MODULE_H

#include <string>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "spice_types.h"
#include "rr_blocks.h"
#include "circuit_library.h"
#include "module_manager.h"

void print_verilog_top_module(ModuleManager& module_manager,
                              const CircuitLibrary& circuit_lib,
                              const vtr::Point<size_t>& device_size,
                              const std::vector<std::vector<t_grid_tile>>& grids,
                              const DeviceRRGSB& L_device_rr_gsb,
                              const std::vector<t_clb_to_clb_directs>& clb2clb_directs,
                              t_sram_orgz_info* cur_sram_orgz_info,
                              const std::string& arch_name,
                              const std::string& verilog_dir,
                              const bool& compact_routing_hierarchy,
                              const bool& use_explicit_mapping);
    

#endif
