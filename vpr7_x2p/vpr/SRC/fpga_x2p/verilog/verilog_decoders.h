/***************************************************************************************
 * Header file for verilog_decoders.cpp
 ***************************************************************************************/
/* TODO: merge to verilog_decoder.h */

#ifndef VERILOG_DECODERS_H
#define VERILOG_DECODERS_H

/* Include other header files which are dependency on the function declared below */
#include <fstream>
#include <string>

#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"

void print_verilog_submodule_mux_local_decoders(ModuleManager& module_manager,
                                                const MuxLibrary& mux_lib,
                                                const CircuitLibrary& circuit_lib,
                                                const std::string& verilog_dir,
                                                const std::string& submodule_dir);

#endif
