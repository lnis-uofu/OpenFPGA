#ifndef BUILD_GRID_MODULE_UTILS_H
#define BUILD_GRID_MODULE_UTILS_H

#include "vpr_types.h"
#include "sides.h"
#include "spice_types.h"
#include "module_manager.h"

e_side find_grid_module_pin_side(t_type_ptr grid_type_descriptor,
                                 const e_side& border_side);

void add_grid_module_net_connect_pb_graph_pin(ModuleManager& module_manager,
                                              const ModuleId& grid_module,
                                              const ModuleId& child_module,
                                              const size_t& child_instance,
                                              t_type_ptr grid_type_descriptor,
                                              t_pb_graph_pin* pb_graph_pin,
                                              const e_side& border_side,
                                              const enum e_spice_pin2pin_interc_type& pin2pin_interc_type);


#endif
