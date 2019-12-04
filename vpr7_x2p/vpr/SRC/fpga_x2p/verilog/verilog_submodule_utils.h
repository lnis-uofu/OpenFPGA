/************************************************
 * Header file for verilog_submodule_utils.cpp
 * Include function declaration on 
 * most utilized functions for Verilog modules
 * such as timing matrix and signal initialization
 ***********************************************/

#ifndef VERILOG_SUBMODULE_UTILS_H
#define VERILOG_SUBMODULE_UTILS_H

#include <fstream>
#include <string>
#include "module_manager.h"
#include "circuit_library.h"

void print_verilog_submodule_timing(std::fstream& fp, 
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model);

void print_verilog_submodule_signal_init(std::fstream& fp, 
                                         const CircuitLibrary& circuit_lib,
                                         const CircuitModelId& circuit_model);

void add_user_defined_verilog_modules(ModuleManager& module_manager, 
                                      const CircuitLibrary& circuit_lib);

void print_verilog_submodule_templates(const ModuleManager& module_manager,
                                       const CircuitLibrary& circuit_lib,
                                       const std::string& verilog_dir,
                                       const std::string& submodule_dir);

#endif
