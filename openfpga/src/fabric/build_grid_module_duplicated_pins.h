#ifndef BUILD_GRID_MODULE_DUPLICATED_PINS_H
#define BUILD_GRID_MODULE_DUPLICATED_PINS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "module_manager.h"
#include "openfpga_side_manager.h"
#include "physical_types.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void add_grid_module_duplicated_pb_type_ports(
  ModuleManager& module_manager, const ModuleId& grid_module,
  const VprDeviceAnnotation& vpr_device_annotation,
  t_physical_tile_type_ptr grid_type_descriptor, const e_side& border_side);

void add_grid_module_nets_connect_duplicated_pb_type_ports(
  ModuleManager& module_manager, const ModuleId& grid_module,
  const ModuleId& child_module, const size_t& child_instance,
  const t_sub_tile& sub_tile, const VprDeviceAnnotation& vpr_device_annotation,
  t_physical_tile_type_ptr grid_type_descriptor, const e_side& border_side);

} /* end namespace openfpga */

#endif
