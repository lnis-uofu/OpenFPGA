/***************************************************************************************
 * Header file for verilog_decoders.cpp
 ***************************************************************************************/
/* TODO: merge to verilog_decoder.h */

#ifndef VERILOG_DECODERS_H
#define VERILOG_DECODERS_H

/* Include other header files which are dependency on the function declared below */
#include <fstream>
#include <string>
#include <vector>

#include "vpr_types.h"
#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"

void print_verilog_submodule_mux_local_decoders(ModuleManager& module_manager,
                                                std::vector<std::string>& netlist_names,
                                                const MuxLibrary& mux_lib,
                                                const CircuitLibrary& circuit_lib,
                                                const std::string& verilog_dir,
                                                const std::string& submodule_dir);

void print_verilog_config_peripherals(ModuleManager& module_manager,
                                      t_sram_orgz_info* cur_sram_orgz_info,
                                      const std::string& verilog_dir,
                                      const std::string& submodule_dir);

#endif
