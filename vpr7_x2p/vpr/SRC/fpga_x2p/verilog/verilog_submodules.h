#ifndef VERILOG_SUBMODULES_H
#define VERILOG_SUBMODULES_H

#include "vpr_types.h"
#include "module_manager.h"
#include "mux_library.h"

void print_verilog_submodules(ModuleManager& module_manager, 
                              const MuxLibrary& mux_lib,
                              t_sram_orgz_info* cur_sram_orgz_info,
                              const char* verilog_dir, 
                              const char* submodule_dir, 
                              const t_arch& Arch, 
                              const t_syn_verilog_opts& fpga_verilog_opts);

/* TODO: Old function to be deprecated */
void dump_verilog_submodules(ModuleManager& module_manager, 
                             const MuxLibrary& mux_lib,
                             t_sram_orgz_info* cur_sram_orgz_info,
                             char* verilog_dir, 
                             char* submodule_dir, 
                             t_arch Arch, 
                             t_det_routing_arch* routing_arch,
                             t_syn_verilog_opts fpga_verilog_opts);

#endif
