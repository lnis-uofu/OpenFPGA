/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <algorithm>
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "vpr_utils.h"

/* Headers from openfpgashell library */
#include "build_module_graph_utils.h"
#include "build_top_module_child_tile_instance.h"
#include "build_top_module_connection.h"
#include "build_top_module_directs.h"
#include "build_top_module_memory.h"
#include "build_top_module_memory_bank.h"
#include "build_top_module_utils.h"
#include "command_exit_codes.h"
#include "module_manager_memory_utils.h"
#include "module_manager_utils.h"
#include "openfpga_device_grid_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Add a instance of a tile module to the top module
 *******************************************************************/
static size_t add_top_module_tile_instance(ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const FabricTile& fabric_tile,
                                           const FabricTileId& fabric_tile_id) {
  /* Find the module name for this type of grid */
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);
  FabricTileId unique_fabric_tile_id = fabric_tile.unique_tile(tile_coord);
  vtr::Point<size_t> unique_tile_coord =
    fabric_tile.tile_coordinate(unique_fabric_tile_id);
  std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
  ModuleId tile_module = module_manager.find_module(tile_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(tile_module));
  /* Record the instance id */
  size_t tile_instance = module_manager.num_instance(top_module, tile_module);
  /* Add the module to top_module */
  module_manager.add_child_module(top_module, tile_module, false);
  /* Set an unique name to the instance
   * Note: it is your risk to gurantee the name is unique!
   */
  std::string instance_name = generate_tile_module_name(tile_coord);
  module_manager.set_child_instance_name(top_module, tile_module, tile_instance,
                                         instance_name);
  return tile_instance;
}

/********************************************************************
 * Add all the tiles as sub-modules across the fabric
 * Here, we will iterate over the full fabric (coordinates)
 * and instanciate the tile modules
 *
 * Return an 2-D array of instance ids of the grid modules that
 * have been added
 *
 * This function assumes an island-style floorplanning for FPGA fabric
 *
 *
 *                +-----------------------------------+
 *                |              I/O tiles            |
 *                |              TOP side             |
 *                +-----------------------------------+
 *
 * +-----------+  +-----------------------------------+ +------------+
 * |           |  |                                   | |            |
 * | I/O tiles |  |          Core tiles               | | I/O tiles  |
 * | LEFT side |  | (CLB, Heterogeneous blocks, etc.) | | RIGHT side |
 * |           |  |                                   | |            |
 * +-----------+  +-----------------------------------+ +------------+
 *
 *                +-----------------------------------+
 *                |              I/O tiles            |
 *                |             BOTTOM side           |
 *                +-----------------------------------+
 *
 *******************************************************************/
static int add_top_module_tile_instances(ModuleManager& module_manager,
                                         const ModuleId& top_module,
                                         vtr::Matrix<size_t>& tile_instance_ids,
                                         const DeviceGrid& grids,
                                         const FabricTile& fabric_tile) {
  vtr::ScopedStartFinishTimer timer("Add tile instances to top module");
  int status = CMD_EXEC_SUCCESS;

  /* Reserve an array for the instance ids */
  tile_instance_ids.resize({grids.width(), grids.height()});
  tile_instance_ids.fill(size_t(-1));

  /* Instanciate I/O grids */
  /* Create the coordinate range for each side of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_tile_coordinates(grids);

  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coord : io_coordinates[io_side]) {
      FabricTileId fabric_tile_id = fabric_tile.find_tile(io_coord);
      if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
        continue;
      }
      /* Add a tile module to top_module*/
      tile_instance_ids[io_coord.x()][io_coord.y()] =
        add_top_module_tile_instance(module_manager, top_module, fabric_tile,
                                     fabric_tile_id);
    }
  }

  /* Instanciate core grids
   * IMPORTANT: sequence matters here, it impacts the I/O indexing.
   * We should follow the same sequence as the build_io_location_map()!
   * If you change the sequence of walking through grids here, you should change
   * it in the build_io_location map()!
   */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      vtr::Point<size_t> curr_coord(ix, iy);
      FabricTileId fabric_tile_id = fabric_tile.find_tile(curr_coord);
      if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
        continue;
      }
      /* Add a tile module to top_module*/
      tile_instance_ids[curr_coord.x()][curr_coord.y()] =
        add_top_module_tile_instance(module_manager, top_module, fabric_tile,
                                     fabric_tile_id);
    }
  }
  return status;
}

/********************************************************************
 * Add the tile-level instances to the top module of FPGA fabric
 * and build connects between them
 *******************************************************************/
int build_top_module_tile_child_instances(ModuleManager& module_manager,
                                          const ModuleId& top_module,
                                          const DeviceGrid& grids,
                                          const FabricTile& fabric_tile) {
  int status = CMD_EXEC_SUCCESS;
  vtr::Matrix<size_t> tile_instance_ids;
  status = add_top_module_tile_instances(module_manager, top_module,
                                         tile_instance_ids, grids, fabric_tile);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* TODO: Build the nets between tiles */

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
