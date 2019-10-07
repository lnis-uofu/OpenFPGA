/********************************************************************
 * Header file for verilog_grid.cpp
 *******************************************************************/
#ifndef VERILOG_GRID_H
#define VERILOG_GRID_H

/* Only include headers related to the data structures used in the following function declaration */
#include <string>
#include "vpr_types.h"
#include "module_manager.h"
#include "mux_library.h"

void print_verilog_grids(ModuleManager& module_manager,
                         const CircuitLibrary& circuit_lib,
                         const MuxLibrary& mux_lib,
                         t_sram_orgz_info* cur_sram_orgz_info,
                         const std::string& verilog_dir,
                         const std::string& subckt_dir,
                         const bool& is_explicit_mapping);

#endif
