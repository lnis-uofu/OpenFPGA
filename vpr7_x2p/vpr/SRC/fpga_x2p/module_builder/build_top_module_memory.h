#ifndef BUILD_TOP_MODULE_MEMORY_H
#define BUILD_TOP_MODULE_MEMORY_H

#include <vector>
#include <map>
#include "module_manager.h"
#include "spice_types.h"
#include "circuit_library.h"
#include "rr_blocks.h"

void organize_top_module_memory_modules(ModuleManager& module_manager, 
                                        const ModuleId& top_module,
                                        const CircuitLibrary& circuit_lib,
                                        const e_sram_orgz& sram_orgz_type,
                                        const CircuitModelId& sram_model,
                                        const vtr::Point<size_t>& device_size,
                                        const std::vector<std::vector<t_grid_tile>>& grids,
                                        const std::vector<std::vector<size_t>>& grid_instance_ids,
                                        const DeviceRRGSB& L_device_rr_gsb,
                                        const std::vector<std::vector<size_t>>& sb_instance_ids,
                                        const std::map<t_rr_type, std::vector<std::vector<size_t>>>& cb_instance_ids,
                                        const bool& compact_routing_hierarchy);

void add_top_module_nets_memory_config_bus(ModuleManager& module_manager,
                                           const ModuleId& parent_module,
                                           const e_sram_orgz& sram_orgz_type, 
                                           const e_spice_model_design_tech& mem_tech);

#endif
