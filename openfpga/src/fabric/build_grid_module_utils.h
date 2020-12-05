#ifndef BUILD_GRID_MODULE_UTILS_H
#define BUILD_GRID_MODULE_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
/* Headers from readarch library */
#include "physical_types.h"

#include "openfpga_interconnect_types.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<e_side> find_grid_module_pin_sides(t_physical_tile_type_ptr grid_type_descriptor,
                                               const e_side& border_side);

void add_grid_module_net_connect_pb_graph_pin(ModuleManager& module_manager,
                                              const ModuleId& grid_module,
                                              const ModuleId& child_module,
                                              const size_t& child_instance,
                                              t_physical_tile_type_ptr grid_type_descriptor,
                                              t_pb_graph_pin* pb_graph_pin,
                                              const e_side& border_side,
                                              const enum e_pin2pin_interc_type& pin2pin_interc_type);

} /* end namespace openfpga */

#endif
