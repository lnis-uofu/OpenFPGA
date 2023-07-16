/********************************************************************
 * This file includes functions that are used to build the location
 * map information for the top-level module of the FPGA fabric
 * It helps OpenFPGA to link the I/O port index in top-level module
 * to the VPR I/O mapping results
 *******************************************************************/
#include <algorithm>
#include <map>

/* Headers from vtrutil library */
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "build_tile_modules.h"
#include "module_manager_utils.h"
#include "openfpga_device_grid_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Build all the tile modules
 *******************************************************************/
static int build_tile_module(
  ModuleManager& module_manager, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id, const DeviceGrid& grids,
  const DeviceRRGSB& device_rr_gsb, const CircuitLibrary& circuit_lib,
  const CircuitModelId& sram_model,
  const e_config_protocol_type& sram_orgz_type, const bool& verbose) {
  int status_code = CMD_EXEC_SUCCESS;

  /* Create the module */
  std::string module_name = generate_tile_module_name(size_t(fabric_tile_id));
  VTR_LOGV(verbose, "Building tile module '%s'...\n");
  ModuleId tile_module = module_manager.add_module(module_name);
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);

  /* Add instance of programmable block */
  for (vtr::Point<size_t> grid_gsb_coord :
       fabric_tile.pb_coordinates(fabric_tile_id)) {
    const RRGSB& grid_rr_gsb = device_rr_gsb.get_gsb(grid_gsb_coord);
    vtr::Point<size_t> grid_coord = grid_rr_gsb.get_grid_coordinate();
    t_physical_tile_type_ptr phy_tile =
      grids.get_physical_type(grid_coord.x(), grid_coord.y());
    /* Empty type does not require a module */
    if (!is_empty_type(phy_tile)) {
      e_side grid_side = find_grid_side_by_coordinate(grids, grid_coord);
      std::string pb_module_name = generate_grid_block_module_name(
        std::string(GRID_MODULE_NAME_PREFIX), std::string(phy_tile->name),
        is_io_type(phy_tile), grid_side);
      ModuleId pb_module = module_manager.find_module(pb_module_name);
      if (!pb_module) {
        VTR_LOG_ERROR(
          "Failed to find pb module '%s' required by tile[%lu][%lu]!\n",
          pb_module_name.c_str(), tile_coord.x(), tile_coord.y());
      }
      size_t pb_instance = module_manager.num_instance(tile_module, pb_module);
      module_manager.add_child_module(tile_module, pb_module);
      if (0 < find_module_num_config_bits(module_manager, pb_module,
                                          circuit_lib, sram_model,
                                          sram_orgz_type)) {
        module_manager.add_configurable_child(tile_module, pb_module,
                                              pb_instance);
      }
      VTR_LOGV(verbose, "Added programmable module '%s' to tile[%lu][%lu]\n",
               pb_module_name.c_str(), tile_coord.x(), tile_coord.y());
    }
  }

  /* Add instance of connection blocks */
  for (t_rr_type cb_type : {CHANX, CHANY}) {
    for (vtr::Point<size_t> cb_coord :
         fabric_tile.cb_coordinates(fabric_tile_id, cb_type)) {
      /* get the unique module coord */
      const RRGSB& unique_rr_gsb =
        device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
      vtr::Point<size_t> unique_cb_coord(unique_rr_gsb.get_cb_x(cb_type),
                                         unique_rr_gsb.get_cb_y(cb_type));
      std::string cb_module_name =
        generate_connection_block_module_name(cb_type, unique_cb_coord);
      ModuleId cb_module = module_manager.find_module(cb_module_name);
      if (!cb_module) {
        VTR_LOG_ERROR(
          "Failed to find connection block module '%s' required by "
          "tile[%lu][%lu]!\n",
          cb_module_name.c_str(), tile_coord.x(), tile_coord.y());
      }
      size_t cb_instance = module_manager.num_instance(tile_module, cb_module);
      module_manager.add_child_module(tile_module, cb_module);
      if (0 < find_module_num_config_bits(module_manager, cb_module,
                                          circuit_lib, sram_model,
                                          sram_orgz_type)) {
        module_manager.add_configurable_child(tile_module, cb_module,
                                              cb_instance);
      }
      VTR_LOGV(verbose,
               "Added connection block module '%s' to tile[%lu][%lu]\n",
               cb_module_name.c_str(), tile_coord.x(), tile_coord.y());
    }
  }

  /* Add instance of switch blocks */
  for (vtr::Point<size_t> sb_coord :
       fabric_tile.sb_coordinates(fabric_tile_id)) {
    /* get the unique module coord */
    const RRGSB& unique_rr_gsb = device_rr_gsb.get_sb_unique_module(sb_coord);
    vtr::Point<size_t> unique_sb_coord(unique_rr_gsb.get_sb_x(),
                                       unique_rr_gsb.get_sb_y());
    std::string sb_module_name =
      generate_switch_block_module_name(unique_sb_coord);
    ModuleId sb_module = module_manager.find_module(sb_module_name);
    if (!sb_module) {
      VTR_LOG_ERROR(
        "Failed to find switch block module '%s' required by tile[%lu][%lu]!\n",
        sb_module_name.c_str(), tile_coord.x(), tile_coord.y());
    }
    size_t sb_instance = module_manager.num_instance(tile_module, sb_module);
    module_manager.add_child_module(tile_module, sb_module);
    if (0 < find_module_num_config_bits(module_manager, sb_module, circuit_lib,
                                        sram_model, sram_orgz_type)) {
      module_manager.add_configurable_child(tile_module, sb_module,
                                            sb_instance);
    }
    VTR_LOGV(verbose, "Added switch block module '%s' to tile[%lu][%lu]\n",
             sb_module_name.c_str(), tile_coord.x(), tile_coord.y());
  }

  /* TODO: Add module nets and ports */

  VTR_LOGV(verbose, "Done\n");
  return status_code;
}

/********************************************************************
 * Build all the tile modules
 *******************************************************************/
int build_tile_modules(ModuleManager& module_manager,
                       const FabricTile& fabric_tile, const DeviceGrid& grids,
                       const DeviceRRGSB& device_rr_gsb,
                       const CircuitLibrary& circuit_lib,
                       const CircuitModelId& sram_model,
                       const e_config_protocol_type& sram_orgz_type,
                       const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build tile modules for the FPGA fabric");

  int status_code = CMD_EXEC_SUCCESS;

  /* Build a module for each unique tile  */
  for (FabricTileId fabric_tile_id : fabric_tile.unique_tiles()) {
    status_code = build_tile_module(module_manager, fabric_tile, fabric_tile_id,
                                    grids, device_rr_gsb, circuit_lib,
                                    sram_model, sram_orgz_type, verbose);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  return status_code;
}

} /* end namespace openfpga */
