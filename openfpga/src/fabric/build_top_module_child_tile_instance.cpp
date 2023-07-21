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
  vtr::Point<size_t> unique_tile_coord =
    fabric_tile.unique_tile_coordinate(fabric_tile_id);
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
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);
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
 * Add the I/O children to the top-level module, which impacts the I/O indexing
 * This is the default function to build the I/O sequence/indexing
 * The I/O children is added in a maze shape
 * The function supports I/Os in the center of grids, starting from the
 *bottom-left corner and ending at the center
 *
 *    +----------------------+
 *    |+--------------------+|
 *    ||+------------------+||
 *    |||+----------------+|||
 *    ||||+-------------->||||
 *    ||||+---------------+|||
 *    |||+-----------------+||
 *    ||+-------------------+|
 *    |+---------------------+
 *    ^
 *  io[0]
 *******************************************************************/
static void add_top_module_tile_io_children(
  ModuleManager& module_manager, const ModuleId& top_module,
  const DeviceGrid& grids, const FabricTile& fabric_tile,
  const vtr::Matrix<size_t>& tile_instance_ids) {
  /* Create the coordinate range for the perimeter I/Os of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_tile_coordinates(grids);

  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coord : io_coordinates[io_side]) {
      FabricTileId fabric_tile_id = fabric_tile.find_tile(io_coord);
      if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
        continue;
      }
      /* Find the module name for this type of tile */
      vtr::Point<size_t> unique_tile_coord =
        fabric_tile.unique_tile_coordinate(fabric_tile_id);
      std::string tile_module_name =
        generate_tile_module_name(unique_tile_coord);
      ModuleId tile_module = module_manager.find_module(tile_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(tile_module));
      /* Add a I/O children to top_module*/
      module_manager.add_io_child(top_module, tile_module,
                                  tile_instance_ids[io_coord.x()][io_coord.y()],
                                  vtr::Point<int>(io_coord.x(), io_coord.y()));
    }
  }

  /* Walk through the center grids */
  size_t xmin = 1;
  size_t xmax = grids.width() - 2;
  size_t ymin = 1;
  size_t ymax = grids.height() - 2;
  std::vector<vtr::Point<size_t>> coords;
  while (xmin < xmax && ymin < ymax) {
    for (size_t iy = ymin; iy < ymax + 1; iy++) {
      coords.push_back(vtr::Point<size_t>(xmin, iy));
    }
    for (size_t ix = xmin + 1; ix < xmax + 1; ix++) {
      coords.push_back(vtr::Point<size_t>(ix, ymax));
    }
    for (size_t iy = ymax - 1; iy > ymin; iy--) {
      coords.push_back(vtr::Point<size_t>(xmax, iy));
    }
    for (size_t ix = xmax; ix > xmin; ix--) {
      coords.push_back(vtr::Point<size_t>(ix, ymin));
    }
    xmin++;
    ymin++;
    xmax--;
    ymax--;
  }

  /* If height is odd, add the missing horizental line */
  if ((grids.height() - 2) % 2 == 1) {
    if (ymin == ymax) {
      for (size_t ix = xmin; ix < xmax + 1; ix++) {
        coords.push_back(vtr::Point<size_t>(ix, ymin));
      }
    }
  }
  /* If width is odd, add the missing vertical line */
  if ((grids.width() - 2) % 2 == 1) {
    if (xmin == xmax) {
      for (size_t iy = ymin; iy < ymax + 1; iy++) {
        coords.push_back(vtr::Point<size_t>(xmin, iy));
      }
    }
  }

  /* Now walk through the coordinates */
  for (vtr::Point<size_t> coord : coords) {
    FabricTileId fabric_tile_id = fabric_tile.find_tile(coord);
    if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
      continue;
    }
    /* Find the module name for this type of tile */
    vtr::Point<size_t> unique_tile_coord =
      fabric_tile.unique_tile_coordinate(fabric_tile_id);
    std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
    ModuleId tile_module = module_manager.find_module(tile_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(tile_module));
    /* Add a I/O children to top_module*/
    module_manager.add_io_child(top_module, tile_module,
                                tile_instance_ids[coord.x()][coord.y()],
                                vtr::Point<int>(coord.x(), coord.y()));
  }
}

/********************************************************************
 * Add the tile-level instances to the top module of FPGA fabric
 * and build connects between them
 *******************************************************************/
int build_top_module_tile_child_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const DeviceGrid& grids,
  const FabricTile& fabric_tile, const ConfigProtocol& config_protocol,
  const FabricKey& fabric_key, const bool& frame_view) {
  int status = CMD_EXEC_SUCCESS;
  vtr::Matrix<size_t> tile_instance_ids;
  status = add_top_module_tile_instances(module_manager, top_module,
                                         tile_instance_ids, grids, fabric_tile);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Update the I/O children list */
  add_top_module_tile_io_children(module_manager, top_module, grids,
                                  fabric_tile, tile_instance_ids);

  /* TODO: Build the nets between tiles */
  if (false == frame_view) {
    /* Reserve nets to be memory efficient */
    reserve_module_manager_module_nets(module_manager, top_module);
    /* TODO: Regular nets between tiles */
    /* TODO: Inter-tile direct connections */
  }

  /* TODO: Add global ports from tile modules: how to connect to clock
  architecture and the global port from tile annotation status =
  add_top_module_global_ports_from_grid_modules( module_manager, top_module,
  tile_annotation, vpr_device_annotation, grids, rr_graph, device_rr_gsb,
  cb_instance_ids, grid_instance_ids, clk_ntwk, rr_clock_lookup); if
  (CMD_EXEC_FATAL_ERROR == status) { return status;
  }
  */

  /* Add GPIO ports from the sub-modules under this Verilog module
   * For top-level module, we follow a special sequencing for I/O modules. So we
   * rebuild the I/O children list here
   */
  add_module_gpio_ports_from_child_modules(module_manager, top_module);

  /* Organize the list of memory modules and instances
   * If we have an empty fabric key, we organize the memory modules as routine
   * Otherwise, we will load the fabric key directly
   */
  if (true == fabric_key.empty()) {
    /* TODO: need a special one for tiles
    organize_top_module_memory_modules(
      module_manager, top_module, circuit_lib, config_protocol, sram_model,
      grids, grid_instance_ids, device_rr_gsb, sb_instance_ids, cb_instance_ids,
      compact_routing_hierarchy);
     */
  } else {
    VTR_ASSERT_SAFE(false == fabric_key.empty());
    /* Throw a fatal error when the fabric key has a mismatch in region
     * organization. between architecture file and fabric key
     */
    if (size_t(config_protocol.num_regions()) != fabric_key.regions().size()) {
      VTR_LOG_ERROR(
        "Fabric key has a different number of configurable regions (='%ld') "
        "than architecture definition (=%d)!\n",
        fabric_key.regions().size(), config_protocol.num_regions());
      return CMD_EXEC_FATAL_ERROR;
    }

    status = load_top_module_memory_modules_from_fabric_key(
      module_manager, top_module, circuit_lib, config_protocol, fabric_key);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }

    status = load_top_module_shift_register_banks_from_fabric_key(
      fabric_key, blwl_sr_banks);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }

    /* Update the memory organization in sub module (non-top) */
    status = load_submodules_memory_modules_from_fabric_key(
      module_manager, circuit_lib, config_protocol, fabric_key);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
