#ifndef BUILD_TOP_MODULE_DIRECTS_H
#define BUILD_TOP_MODULE_DIRECTS_H

#include <vector>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "module_manager.h"
#include "circuit_library.h"

void add_top_module_nets_clb2clb_direct_connections(ModuleManager& module_manager, 
                                                    const ModuleId& top_module, 
                                                    const CircuitLibrary& circuit_lib, 
                                                    const vtr::Point<size_t>& device_size,
                                                    const std::vector<std::vector<t_grid_tile>>& grids,
                                                    const std::vector<std::vector<size_t>>& grid_instance_ids,
                                                    const std::vector<t_clb_to_clb_directs>& clb2clb_directs);

#endif
