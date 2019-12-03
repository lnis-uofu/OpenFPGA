/***********************************************
 * Header file for verilog_mux.cpp
 **********************************************/

#ifndef VERILOG_MUX_H
#define VERILOG_MUX_H

/* Include other header files which are dependency on the function declared below */
#include <fstream>

#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"

void print_verilog_submodule_muxes(ModuleManager& module_manager,
                                   const MuxLibrary& mux_lib,
                                   const CircuitLibrary& circuit_lib,
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   const std::string& verilog_dir,
                                   const std::string& submodule_dir,
                                   const bool& use_explicit_port_map);

#endif
