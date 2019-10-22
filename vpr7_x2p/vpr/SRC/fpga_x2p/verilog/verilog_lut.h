/***********************************************
 * Header file for verilog_lut.cpp
 **********************************************/

#ifndef VERILOG_LUT_H
#define VERILOG_LUT_H

/* Include other header files which are dependency on the function declared below */
#include <fstream>
#include <string>

#include "circuit_library.h"
#include "module_manager.h"

void print_verilog_submodule_luts(ModuleManager& module_manager,
                                  const CircuitLibrary& circuit_lib,
                                  const std::string& verilog_dir,
                                  const std::string& submodule_dir,
                                  const bool& use_explicit_port_map);

#endif
