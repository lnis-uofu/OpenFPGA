/********************************************************************
 * Header file for build_module_graph_utils.cpp
 ********************************************************************/
#ifndef BUILD_MODULE_GRAPH_UTILS_H
#define BUILD_MODULE_GRAPH_UTILS_H

#include <string>
#include <vector>
#include "spice_types.h" 
#include "sides.h" 
#include "vtr_geometry.h" 
#include "vpr_types.h" 
#include "module_manager.h" 
#include "circuit_library.h" 

std::string generate_grid_side_port_name(const std::vector<std::vector<t_grid_tile>>& grids,
                                         const vtr::Point<size_t>& coordinate,
                                         const e_side& side, 
                                         const size_t& pin_id);

ModulePortId find_inverter_buffer_module_port(const ModuleManager& module_manager, 
                                              const ModuleId& module_id, 
                                              const CircuitLibrary& circuit_lib, 
                                              const CircuitModelId& model_id,
                                              const e_spice_model_port_type& port_type);

ModuleNetId add_inverter_buffer_child_module_and_nets(ModuleManager& module_manager,
                                                      const ModuleId& parent_module, 
                                                      const CircuitLibrary& circuit_lib, 
                                                      const CircuitModelId& model_id, 
                                                      const ModuleNetId& input_net);

#endif
