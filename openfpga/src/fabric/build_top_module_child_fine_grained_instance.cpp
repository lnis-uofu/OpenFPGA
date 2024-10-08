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
#include "build_top_module_child_fine_grained_instance.h"
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
 * Add a instance of a grid module to the top module
 *******************************************************************/
static size_t add_top_module_grid_instance(
  ModuleManager& module_manager, const ModuleId& top_module,
  t_physical_tile_type_ptr grid_type, const e_side& border_side,
  const vtr::Point<size_t>& grid_coord) {
  /* Find the module name for this type of grid */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_module_name = generate_grid_block_module_name(
    grid_module_name_prefix, std::string(grid_type->name),
    is_io_type(grid_type), border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
  /* Record the instance id */
  size_t grid_instance = module_manager.num_instance(top_module, grid_module);
  /* Add the module to top_module */
  module_manager.add_child_module(top_module, grid_module, false);
  /* Set an unique name to the instance
   * Note: it is your risk to gurantee the name is unique!
   */
  std::string instance_name = generate_grid_block_instance_name(
    grid_module_name_prefix, std::string(grid_type->name),
    is_io_type(grid_type), border_side, grid_coord);
  module_manager.set_child_instance_name(top_module, grid_module, grid_instance,
                                         instance_name);

  return grid_instance;
}

/********************************************************************
 * Add all the grids as sub-modules across the fabric
 * The grid modules are created for each unique type of grid (based
 * on the type in data structure data_structure
 * Here, we will iterate over the full fabric (coordinates)
 * and instanciate the grid modules
 *
 * Return an 2-D array of instance ids of the grid modules that
 * have been added
 *
 * This function assumes an island-style floorplanning for FPGA fabric
 *
 *
 *                +-----------------------------------+
 *                |              I/O grids            |
 *                |              TOP side             |
 *                +-----------------------------------+
 *
 * +-----------+  +-----------------------------------+ +------------+
 * |           |  |                                   | |            |
 * | I/O grids |  |          Core grids               | | I/O grids  |
 * | LEFT side |  | (CLB, Heterogeneous blocks, etc.) | | RIGHT side |
 * |           |  |                                   | |            |
 * +-----------+  +-----------------------------------+ +------------+
 *
 *                +-----------------------------------+
 *                |              I/O grids            |
 *                |             BOTTOM side           |
 *                +-----------------------------------+
 *
 *******************************************************************/
static vtr::Matrix<size_t> add_top_module_grid_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  const DeviceGrid& grids, const size_t& layer) {
  vtr::ScopedStartFinishTimer timer("Add grid instances to top module");

  /* Reserve an array for the instance ids */
  vtr::Matrix<size_t> grid_instance_ids({grids.width(), grids.height()});
  grid_instance_ids.fill(size_t(-1));

  /* Instanciate I/O grids */
  /* Create the coordinate range for each side of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_grid_coordinates(grids);

  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      t_physical_tile_loc phy_tile_loc(io_coordinate.x(), io_coordinate.y(),
                                       layer);
      t_physical_tile_type_ptr phy_tile_type =
        grids.get_physical_type(phy_tile_loc);
      /* Bypass EMPTY grid */
      if (true == is_empty_type(phy_tile_type)) {
        continue;
      }
      /* Skip width, height > 1 tiles (mostly heterogeneous blocks) */
      if ((0 < grids.get_width_offset(phy_tile_loc)) ||
          (0 < grids.get_height_offset(phy_tile_loc))) {
        /* Find the root of this grid, the instance id should be valid.
         * We just copy it here
         */
        vtr::Point<size_t> root_grid_coord(
          io_coordinate.x() - grids.get_width_offset(phy_tile_loc),
          io_coordinate.y() - grids.get_height_offset(phy_tile_loc));
        VTR_ASSERT(size_t(-1) !=
                   grid_instance_ids[root_grid_coord.x()][root_grid_coord.y()]);
        grid_instance_ids[io_coordinate.x()][io_coordinate.y()] =
          grid_instance_ids[root_grid_coord.x()][root_grid_coord.y()];
        continue;
      }

      /* Add a grid module to top_module*/
      grid_instance_ids[io_coordinate.x()][io_coordinate.y()] =
        add_top_module_grid_instance(module_manager, top_module, phy_tile_type,
                                     io_side, io_coordinate);
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
      t_physical_tile_loc phy_tile_loc(ix, iy, layer);
      t_physical_tile_type_ptr phy_tile_type =
        grids.get_physical_type(phy_tile_loc);
      /* Bypass EMPTY grid */
      if (true == is_empty_type(phy_tile_type)) {
        continue;
      }
      /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
      if ((0 < grids.get_width_offset(phy_tile_loc)) ||
          (0 < grids.get_height_offset(phy_tile_loc))) {
        /* Find the root of this grid, the instance id should be valid.
         * We just copy it here
         */
        vtr::Point<size_t> root_grid_coord(
          ix - grids.get_width_offset(phy_tile_loc),
          iy - grids.get_height_offset(phy_tile_loc));
        VTR_ASSERT(size_t(-1) !=
                   grid_instance_ids[root_grid_coord.x()][root_grid_coord.y()]);
        grid_instance_ids[ix][iy] =
          grid_instance_ids[root_grid_coord.x()][root_grid_coord.y()];
        continue;
      }
      /* Add a grid module to top_module*/
      vtr::Point<size_t> grid_coord(ix, iy);
      grid_instance_ids[ix][iy] = add_top_module_grid_instance(
        module_manager, top_module, phy_tile_type, NUM_2D_SIDES, grid_coord);
    }
  }

  return grid_instance_ids;
}

/********************************************************************
 * Add switch blocks across the FPGA fabric to the top-level module
 * Return an 2-D array of instance ids of the switch blocks that
 * have been added
 *******************************************************************/
static vtr::Matrix<size_t> add_top_module_switch_block_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  const RRGraphView& rr_graph, const DeviceRRGSB& device_rr_gsb,
  const bool& compact_routing_hierarchy) {
  vtr::ScopedStartFinishTimer timer("Add switch block instances to top module");

  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();

  /* Reserve an array for the instance ids */
  vtr::Matrix<size_t> sb_instance_ids({sb_range.x(), sb_range.y()});
  sb_instance_ids.fill(size_t(-1));

  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      /* If we use compact routing hierarchy, we should instanciate the unique
       * module of SB */
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);

      if (false == rr_gsb.is_sb_exist(rr_graph)) {
        continue;
      }

      vtr::Point<size_t> sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
      if (true == compact_routing_hierarchy) {
        vtr::Point<size_t> sb_coord(ix, iy);
        const RRGSB& unique_mirror =
          device_rr_gsb.get_sb_unique_module(sb_coord);
        sb_coordinate.set_x(unique_mirror.get_sb_x());
        sb_coordinate.set_y(unique_mirror.get_sb_y());
      }
      std::string sb_module_name =
        generate_switch_block_module_name(sb_coordinate);
      ModuleId sb_module = module_manager.find_module(sb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sb_module));
      /* Record the instance id */
      sb_instance_ids[rr_gsb.get_sb_x()][rr_gsb.get_sb_y()] =
        module_manager.num_instance(top_module, sb_module);
      /* Add the module to top_module */
      module_manager.add_child_module(top_module, sb_module, false);
      /* Set an unique name to the instance
       * Note: it is your risk to gurantee the name is unique!
       */
      module_manager.set_child_instance_name(
        top_module, sb_module,
        sb_instance_ids[rr_gsb.get_sb_x()][rr_gsb.get_sb_y()],
        generate_switch_block_module_name(
          vtr::Point<size_t>(rr_gsb.get_sb_x(), rr_gsb.get_sb_y())));
    }
  }

  return sb_instance_ids;
}

/********************************************************************
 * Add switch blocks across the FPGA fabric to the top-level module
 *******************************************************************/
static vtr::Matrix<size_t> add_top_module_connection_block_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  const DeviceRRGSB& device_rr_gsb, const t_rr_type& cb_type,
  const bool& compact_routing_hierarchy, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Add connection block instances to top module");

  vtr::Point<size_t> cb_range = device_rr_gsb.get_gsb_range();

  /* Reserve an array for the instance ids */
  vtr::Matrix<size_t> cb_instance_ids({cb_range.x(), cb_range.y()});
  cb_instance_ids.fill(size_t(-1));

  for (size_t ix = 0; ix < cb_range.x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1)
       * We will skip those modules
       */
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      VTR_LOGV(verbose, "Try to add %s connnection block at (%lu, %lu)\n",
               cb_type == CHANX ? "X-" : "Y-", ix, iy);
      vtr::Point<size_t> cb_coordinate(rr_gsb.get_cb_x(cb_type),
                                       rr_gsb.get_cb_y(cb_type));
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        VTR_LOGV(
          verbose,
          "Skip %s connnection block at (%lu, %lu) as it does not exist\n",
          cb_type == CHANX ? "X-" : "Y-", cb_coordinate.x(), cb_coordinate.y());
        continue;
      }
      /* If we use compact routing hierarchy, we should instanciate the unique
       * module of SB */
      if (true == compact_routing_hierarchy) {
        vtr::Point<size_t> cb_coord(ix, iy);
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        const RRGSB& unique_mirror =
          device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
        cb_coordinate.set_x(unique_mirror.get_cb_x(cb_type));
        cb_coordinate.set_y(unique_mirror.get_cb_y(cb_type));
      }
      std::string cb_module_name =
        generate_connection_block_module_name(cb_type, cb_coordinate);
      ModuleId cb_module = module_manager.find_module(cb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(cb_module));
      /* Record the instance id */
      cb_instance_ids[rr_gsb.get_cb_x(cb_type)][rr_gsb.get_cb_y(cb_type)] =
        module_manager.num_instance(top_module, cb_module);
      /* Add the module to top_module */
      module_manager.add_child_module(top_module, cb_module, false);
      /* Set an unique name to the instance
       * Note: it is your risk to gurantee the name is unique!
       */
      std::string cb_instance_name = generate_connection_block_module_name(
        cb_type,
        vtr::Point<size_t>(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type)));
      module_manager.set_child_instance_name(
        top_module, cb_module,
        cb_instance_ids[rr_gsb.get_cb_x(cb_type)][rr_gsb.get_cb_y(cb_type)],
        cb_instance_name);
      VTR_LOGV(verbose, "Added %s connnection block '%s' (module '%s')\n",
               cb_type == CHANX ? "X-" : "Y-", cb_instance_name.c_str(),
               cb_module_name.c_str());
    }
  }

  return cb_instance_ids;
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
static void add_top_module_io_children(
  ModuleManager& module_manager, const ModuleId& top_module,
  const DeviceGrid& grids, const size_t& layer,
  const vtr::Matrix<size_t>& grid_instance_ids) {
  /* Create the coordinate range for the perimeter I/Os of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_grid_coordinates(grids);

  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coord : io_coordinates[io_side]) {
      t_physical_tile_loc phy_tile_loc(io_coord.x(), io_coord.y(), layer);
      t_physical_tile_type_ptr grid_type =
        grids.get_physical_type(phy_tile_loc);
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grid_type)) {
        continue;
      }
      /* Skip width, height > 1 tiles (mostly heterogeneous blocks) */
      if ((0 < grids.get_width_offset(phy_tile_loc)) ||
          (0 < grids.get_height_offset(phy_tile_loc))) {
        continue;
      }
      /* Find the module name for this type of grid */
      std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
      std::string grid_module_name = generate_grid_block_module_name(
        grid_module_name_prefix, std::string(grid_type->name),
        is_io_type(grid_type), io_side);
      ModuleId grid_module = module_manager.find_module(grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
      /* Add a I/O children to top_module*/
      module_manager.add_io_child(top_module, grid_module,
                                  grid_instance_ids[io_coord.x()][io_coord.y()],
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
    /* Note: Do NOT add a coord two time! So when ymin == ymax, should skip this
     * point. Think about a fabric of 3x3, where the point (1,1) is added twice
     */
    if (xmin == xmax && ymin != ymax) {
      for (size_t iy = ymin; iy < ymax + 1; iy++) {
        coords.push_back(vtr::Point<size_t>(xmin, iy));
      }
    }
  }

  /* Now walk through the coordinates */
  for (vtr::Point<size_t> coord : coords) {
    t_physical_tile_loc phy_tile_loc(coord.x(), coord.y(), layer);
    t_physical_tile_type_ptr grid_type = grids.get_physical_type(phy_tile_loc);
    /* Bypass EMPTY grid */
    if (true == is_empty_type(grid_type)) {
      continue;
    }
    /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
    if ((0 < grids.get_width_offset(phy_tile_loc)) ||
        (0 < grids.get_height_offset(phy_tile_loc))) {
      continue;
    }
    /* Find the module name for this type of grid */
    std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
    std::string grid_module_name = generate_grid_block_module_name(
      grid_module_name_prefix, std::string(grid_type->name),
      is_io_type(grid_type), NUM_2D_SIDES);
    ModuleId grid_module = module_manager.find_module(grid_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
    /* Add a I/O children to top_module*/
    module_manager.add_io_child(top_module, grid_module,
                                grid_instance_ids[coord.x()][coord.y()],
                                vtr::Point<int>(coord.x(), coord.y()));
  }
}

/********************************************************************
 * Add the fine-grained instances to the top module of FPGA fabric
 * The fine-grained instances include programmable blocks, connection blocks and
 *switch blocks, each of which is an instance under the top module
 *******************************************************************/
int build_top_module_fine_grained_child_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const ClockNetwork& clk_ntwk,
  const RRClockSpatialLookup& rr_clock_lookup,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const TileAnnotation& tile_annotation,
  const RRGraphView& rr_graph, const DeviceRRGSB& device_rr_gsb,
  const TileDirect& tile_direct, const ArchDirect& arch_direct,
  const ConfigProtocol& config_protocol, const CircuitModelId& sram_model,
  const bool& frame_view, const bool& compact_routing_hierarchy,
  const bool& duplicate_grid_pin, const FabricKey& fabric_key,
  const bool& group_config_block, const bool& perimeter_cb,
  const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  std::map<t_rr_type, vtr::Matrix<size_t>> cb_instance_ids;

  /* Add sub modules, which are grid, SB and CBX/CBY modules as instances */
  /* Add all the grids across the fabric */
  vtr::Matrix<size_t> grid_instance_ids =
    add_top_module_grid_instances(module_manager, top_module, grids, layer);
  /* Add all the SBs across the fabric */
  vtr::Matrix<size_t> sb_instance_ids = add_top_module_switch_block_instances(
    module_manager, top_module, rr_graph, device_rr_gsb,
    compact_routing_hierarchy);
  /* Add all the CBX and CBYs across the fabric */
  cb_instance_ids[CHANX] = add_top_module_connection_block_instances(
    module_manager, top_module, device_rr_gsb, CHANX, compact_routing_hierarchy,
    verbose);
  cb_instance_ids[CHANY] = add_top_module_connection_block_instances(
    module_manager, top_module, device_rr_gsb, CHANY, compact_routing_hierarchy,
    verbose);

  /* Update I/O children list */
  add_top_module_io_children(module_manager, top_module, grids, layer,
                             grid_instance_ids);

  /* Add nets when we need a complete fabric modeling,
   * which is required by downstream functions
   */
  if (false == frame_view) {
    /* Reserve nets to be memory efficient */
    reserve_module_manager_module_nets(module_manager, top_module);

    /* Add module nets to connect the sub modules */
    add_top_module_nets_connect_grids_and_gsbs(
      module_manager, top_module, vpr_device_annotation, grids, layer,
      grid_instance_ids, rr_graph, device_rr_gsb, sb_instance_ids,
      cb_instance_ids, compact_routing_hierarchy, duplicate_grid_pin);
    /* Add inter-CLB direct connections */
    add_top_module_nets_tile_direct_connections(
      module_manager, top_module, circuit_lib, vpr_device_annotation, grids,
      layer, grid_instance_ids, tile_direct, arch_direct);
  }

  /* Add global ports from grid ports that are defined as global in tile
   * annotation */
  status = add_top_module_global_ports_from_grid_modules(
    module_manager, top_module, tile_annotation, vpr_device_annotation, grids,
    layer, rr_graph, device_rr_gsb, cb_instance_ids, grid_instance_ids,
    clk_ntwk, rr_clock_lookup, perimeter_cb);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }

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
    organize_top_module_memory_modules(
      module_manager, top_module, circuit_lib, config_protocol, sram_model,
      grids, layer, grid_instance_ids, device_rr_gsb, rr_graph, sb_instance_ids,
      cb_instance_ids, compact_routing_hierarchy);
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
      module_manager, circuit_lib, config_protocol, fabric_key,
      group_config_block);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
  }
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
