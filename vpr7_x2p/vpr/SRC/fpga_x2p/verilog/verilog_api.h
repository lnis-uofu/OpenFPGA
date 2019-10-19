#ifndef VERILOG_API_H 
#define VERILOG_API_H 

#include "vpr_types.h"
#include "module_manager.h"

void vpr_fpga_verilog(ModuleManager& module_manager,
                      t_vpr_setup vpr_setup,
                      t_arch Arch,
                      char* circuit_name);

#endif
