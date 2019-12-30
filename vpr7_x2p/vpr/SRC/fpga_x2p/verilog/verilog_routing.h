/***********************************************
 * Header file for verilog_routing.cpp
 **********************************************/
#ifndef VERILOG_ROUTING_H
#define VERILOG_ROUTING_H

/* Include other header files which are dependency on the function declared below */
#include "mux_library.h"
#include "module_manager.h"
#include "rr_blocks.h"

void print_verilog_flatten_routing_modules(ModuleManager& module_manager,
                                           const DeviceRRGSB& L_device_rr_gsb,
                                           const t_det_routing_arch& routing_arch,
                                           const std::string& verilog_dir,
                                           const std::string& subckt_dir,
                                           const bool& use_explicit_port_map);

void print_verilog_unique_routing_modules(ModuleManager& module_manager,
                                          const DeviceRRGSB& L_device_rr_gsb,
                                          const t_det_routing_arch& routing_arch,
                                          const std::string& verilog_dir,
                                          const std::string& subckt_dir,
                                          const bool& use_explicit_port_map);

#endif
