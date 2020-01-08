#ifndef BUILD_GRID_MODULE_DUPLICATED_PINS_H
#define BUILD_GRID_MODULE_DUPLICATED_PINS_H

#include "module_manager.h"
#include "sides.h"
#include "vpr_types.h"

void add_grid_module_duplicated_pb_type_ports(ModuleManager& module_manager,
                                              const ModuleId& grid_module,
                                              t_type_ptr grid_type_descriptor,
                                              const e_side& border_side);

void add_grid_module_nets_connect_duplicated_pb_type_ports(ModuleManager& module_manager,
                                                           const ModuleId& grid_module,
                                                           const ModuleId& child_module,
                                                           const size_t& child_instance,
                                                           t_type_ptr grid_type_descriptor,
                                                           const e_side& border_side);

#endif
