#ifndef VERILOG_SUBMODULES_H
#define VERILOG_SUBMODULES_H

#include "module_manager.h"
#include "mux_library.h"

void dump_verilog_submodules(ModuleManager& module_manager, 
                             const MuxLibrary& mux_lib,
                             t_sram_orgz_info* cur_sram_orgz_info,
                             char* verilog_dir, 
                             char* submodule_dir, 
                             t_arch Arch, 
                             t_det_routing_arch* routing_arch,
                             t_syn_verilog_opts fpga_verilog_opts);
#endif
