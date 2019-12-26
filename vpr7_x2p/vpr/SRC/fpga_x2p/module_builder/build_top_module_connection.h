#ifndef BUILD_TOP_MODULE_CONNECTION_H
#define BUILD_TOP_MODULE_CONNECTION_H

#include <vector>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "rr_blocks.h"
#include "module_manager.h"

void add_top_module_nets_connect_grids_and_gsbs(ModuleManager& module_manager, 
                                                const ModuleId& top_module, 
                                                const vtr::Point<size_t>& device_size,
                                                const std::vector<std::vector<t_grid_tile>>& grids,
                                                const std::vector<std::vector<size_t>>& grid_instance_ids,
                                                const DeviceRRGSB& L_device_rr_gsb,
                                                const std::vector<std::vector<size_t>>& sb_instance_ids,
                                                const std::map<t_rr_type, std::vector<std::vector<size_t>>>& cb_instance_ids,
                                                const bool& compact_routing_hierarchy,
                                                const bool& duplicate_grid_pin);

#endif
