/************************************************
 * Header file for verilog_submodule_utils.cpp
 * Include function declaration on 
 * most utilized functions for Verilog modules
 * such as timing matrix and signal initialization
 ***********************************************/

#ifndef VERILOG_SUBMODULE_UTILS_H
#define VERILOG_SUBMODULE_UTILS_H

#include <fstream>

void print_verilog_submodule_timing(std::fstream& fp, 
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model);

void print_verilog_submodule_signal_init(std::fstream& fp, 
                                         const CircuitLibrary& circuit_lib,
                                         const CircuitModelId& circuit_model);

#endif
