/********************************************************************
 * Header file for build_routing_modules.cpp
 *******************************************************************/
#ifndef BUILD_ROUTING_MODULES_H
#define BUILD_ROUTING_MODULES_H

#include "spice_types.h"
#include "vpr_types.h"
#include "rr_blocks.h"
#include "mux_library.h"
#include "circuit_library.h"
#include "module_manager.h"

void build_flatten_routing_modules(ModuleManager& module_manager,
                                   const DeviceRRGSB& L_device_rr_gsb,
                                   const CircuitLibrary& circuit_lib,
                                   const e_sram_orgz& sram_orgz_type,
                                   const CircuitModelId& sram_model,
                                   const t_det_routing_arch& routing_arch,
                                   const std::vector<t_switch_inf>& rr_switches); 

void build_unique_routing_modules(ModuleManager& module_manager,
                                  const DeviceRRGSB& L_device_rr_gsb,
                                  const CircuitLibrary& circuit_lib,
                                  const e_sram_orgz& sram_orgz_type,
                                  const CircuitModelId& sram_model,
                                  const t_det_routing_arch& routing_arch,
                                  const std::vector<t_switch_inf>& rr_switches); 


#endif
