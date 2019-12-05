/***********************************************
 * Header file for verilog_wire.cpp
 **********************************************/

#ifndef VERILOG_WIRE_H
#define VERILOG_WIRE_H

/* Include other header files which are dependency on the function declared below */
#include <fstream>
#include <vector>
#include "physical_types.h"
#include "vpr_types.h"

#include "circuit_library.h"
#include "module_manager.h"

void print_verilog_submodule_wires(ModuleManager& module_manager,
                                   std::vector<std::string>& netlist_names,
                                   const CircuitLibrary& circuit_lib,
                                   const std::string& verilog_dir,
                                   const std::string& submodule_dir);

#endif
