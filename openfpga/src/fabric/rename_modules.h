#ifndef RENAME_MODULES_H
#define RENAME_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "device_rr_gsb.h"
#include "fabric_tile.h"
#include "module_manager.h"
#include "module_name_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int init_fabric_module_name_map(ModuleNameMap& module_name_map,
                                const ModuleManager& module_manager,
                                const bool& verbose);

int update_module_map_name_with_indexing_names(ModuleNameMap& module_name_map,
                                               const DeviceRRGSB& device_rr_gsb,
                                               const FabricTile& fabric_tile,
                                               const bool& verbose);

int rename_fabric_modules(ModuleManager& module_manager,
                          const ModuleNameMap& module_name_map,
                          const bool& verbose);

int partial_rename_fabric_modules(ModuleManager& module_manager,
                                  const ModuleNameMap& module_name_map,
                                  const bool& verbose);

int update_module_name_map_with_user_version(
  ModuleNameMap& curr_module_name_map,
  const ModuleNameMap& user_module_name_map, const bool& verbose);

} /* end namespace openfpga */

#endif
