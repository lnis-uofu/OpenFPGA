/***********************************************
 * Header file for verilog_memory.cpp
 **********************************************/

#ifndef VERILOG_MEMORY_H
#define VERILOG_MEMORY_H

/* Include other header files which are dependency on the function declared below */
#include <fstream>

#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"

void print_verilog_submodule_memories(ModuleManager& module_manager,
                                      const MuxLibrary& mux_lib,
                                      const CircuitLibrary& circuit_lib,
                                      const std::string& verilog_dir,
                                      const std::string& submodule_dir,
                                      const bool& use_explicit_port_map);

#endif
