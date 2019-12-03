/************************************************
 * Header file for verilog_submodule_essential.cpp
 * Include function declaration on 
 * outputting Verilog netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
 ***********************************************/

#ifndef VERILOG_ESSENTIAL_GATES_H
#define VERILOG_ESSENTIAL_GATES_H

#include <string>
#include "circuit_library.h"

void print_verilog_submodule_essentials(ModuleManager& module_manager, 
                                        const std::string& verilog_dir, 
                                        const std::string& submodule_dir,
                                        const CircuitLibrary& circuit_lib);

#endif
