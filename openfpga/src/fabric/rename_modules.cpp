/* Headers from vtrutil library */
#include "rename_modules.h"

#include "command_exit_codes.h"
#include "openfpga_naming.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/** @brief Initialize a module name map with the existing module names from a
 * module manager. In this case, all the built-in names are the same as
 * customized names */
int init_fabric_module_name_map(ModuleNameMap& module_name_map,
                                const ModuleManager& module_manager,
                                const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  /* the module name map should be empty! */
  module_name_map.clear();
  size_t cnt = 0;
  for (ModuleId curr_module : module_manager.modules()) {
    status = module_name_map.set_tag_to_name_pair(
      module_manager.module_name(curr_module),
      module_manager.module_name(curr_module));
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_SUCCESS;
    }
    cnt++;
  }
  VTR_LOGV(verbose, "Initialized module name map for '%lu' modules\n", cnt);
  return CMD_EXEC_SUCCESS;
}

int update_module_map_name_with_indexing_names(ModuleNameMap& module_name_map,
                                               const DeviceRRGSB& device_rr_gsb,
                                               const FabricTile& fabric_tile,
                                               const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  /* Walk through the device rr gsb on the unique routing modules */
  for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(isb);
    vtr::Point<size_t> gsb_coordinate(unique_mirror.get_sb_x(),
                                      unique_mirror.get_sb_y());
    std::string name_using_coord =
      generate_switch_block_module_name(gsb_coordinate);
    std::string name_using_index =
      generate_switch_block_module_name_using_index(isb);
    status =
      module_name_map.set_tag_to_name_pair(name_using_coord, name_using_index);
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_SUCCESS;
    }
    VTR_LOGV(verbose, "Now use indexing name for module '%s' (was '%s')\n",
             name_using_index.c_str(), name_using_coord.c_str());
  }
  for (t_rr_type cb_type : {CHANX, CHANY}) {
    for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(cb_type);
         ++icb) {
      const RRGSB& unique_mirror =
        device_rr_gsb.get_cb_unique_module(cb_type, icb);
      vtr::Point<size_t> gsb_coordinate(unique_mirror.get_cb_x(cb_type),
                                        unique_mirror.get_cb_y(cb_type));
      std::string name_using_coord =
        generate_connection_block_module_name(cb_type, gsb_coordinate);
      std::string name_using_index =
        generate_connection_block_module_name_using_index(cb_type, icb);
      status = module_name_map.set_tag_to_name_pair(name_using_coord,
                                                    name_using_index);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_SUCCESS;
      }
      VTR_LOGV(verbose, "Now use indexing name for module '%s' (was '%s')\n",
               name_using_index.c_str(), name_using_coord.c_str());
    }
  }
  /* Walk through the fabric tile on the unique routing modules */
  for (size_t itile = 0; itile < fabric_tile.unique_tiles().size(); ++itile) {
    FabricTileId fabric_tile_id = fabric_tile.unique_tiles()[itile];
    vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);
    std::string name_using_coord = generate_tile_module_name(tile_coord);
    std::string name_using_index = generate_tile_module_name_using_index(itile);
    status =
      module_name_map.set_tag_to_name_pair(name_using_coord, name_using_index);
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_SUCCESS;
    }
    VTR_LOGV(verbose, "Now use indexing name for module '%s' (was '%s')\n",
             name_using_index.c_str(), name_using_coord.c_str());
  }
  return CMD_EXEC_SUCCESS;
}

int rename_fabric_modules(ModuleManager& module_manager,
                          const ModuleNameMap& module_name_map,
                          const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  size_t cnt = 0;
  for (ModuleId curr_module : module_manager.modules()) {
    std::string new_name =
      module_name_map.name(module_manager.module_name(curr_module));
    if (new_name != module_manager.module_name(curr_module)) {
      VTR_LOGV(verbose, "Rename module '%s' to its new name '%s'\n",
               module_manager.module_name(curr_module).c_str(),
               new_name.c_str());
      module_manager.set_module_name(curr_module, new_name);
    }
    cnt++;
  }
  VTR_LOG("Renamed %lu modules\n", cnt);
  return status;
}

} /* end namespace openfpga */
