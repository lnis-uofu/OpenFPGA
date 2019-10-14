/********************************************************************
 * Header file for verilog_top_module.cpp
 *******************************************************************/
#ifndef VERILOG_TOP_MODULE_H
#define VERILOG_TOP_MODULE_H

#include <string>
#include "spice_types.h"
#include "circuit_library.h"
#include "module_manager.h"

void print_verilog_top_module(ModuleManager& module_manager,
                              const CircuitLibrary& circuit_lib,
                              t_sram_orgz_info* cur_sram_orgz_info,
                              const std::string& arch_name,
                              const std::string& verilog_dir,
                              const bool& use_explicit_mapping);
    

#endif
