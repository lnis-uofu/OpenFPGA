/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <ctime>
#include <map>
#include <algorithm>

#include "vtr_assert.h"

#include "vpr_types.h"
#include "globals.h"

#include "rr_blocks_utils.h"
#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "module_manager_utils.h"
#include "build_top_module_utils.h"
#include "build_top_module_connection.h"
#include "build_top_module_memory.h"
#include "build_top_module_directs.h"

#include "verilog_global.h"
#include "build_module_graph_utils.h"
#include "build_top_module.h"

/********************************************************************
 * Add a instance of a grid module to the top module
 *******************************************************************/
static 
size_t add_top_module_grid_instance(ModuleManager& module_manager,
                                    const ModuleId& top_module,
                                    t_type_ptr grid_type,
                                    const e_side& border_side,
                                    const vtr::Point<size_t>& grid_coord) {
  /* Find the module name for this type of grid */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), IO_TYPE == grid_type, border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
  /* Record the instance id */
  size_t grid_instance = module_manager.num_instance(top_module, grid_module);
  /* Add the module to top_module */ 
  module_manager.add_child_module(top_module, grid_module);
  /* Set an unique name to the instance
   * Note: it is your risk to gurantee the name is unique!
   */
  std::string instance_name = generate_grid_block_instance_name(grid_module_name_prefix, std::string(grid_type->name), IO_TYPE == grid_type, border_side, grid_coord);
  module_manager.set_child_instance_name(top_module, grid_module, grid_instance, instance_name);

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
static 
std::vector<std::vector<size_t>> add_top_module_grid_instances(ModuleManager& module_manager,
                                                               const ModuleId& top_module,
                                                               const vtr::Point<size_t>& device_size,
                                                               const std::vector<std::vector<t_grid_tile>>& grids) {
  /* Reserve an array for the instance ids */
  std::vector<std::vector<size_t>> grid_instance_ids; 
  grid_instance_ids.resize(grids.size());
  for (size_t x = 0; x < grids.size(); ++x) {
    /* Deposite an invalid value */
    grid_instance_ids[x].resize(grids[x].size(), size_t(-1));
  }

  /* Instanciate core grids */
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grids[ix][iy].type) {
        continue;
      } 
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[ix][iy].offset) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE != grids[ix][iy].type);
      /* Add a grid module to top_module*/
      vtr::Point<size_t> grid_coord(ix, iy);
      grid_instance_ids[ix][iy] = add_top_module_grid_instance(module_manager, top_module,
                                                               grids[ix][iy].type,
                                                               NUM_SIDES, grid_coord);
    }
  }

  /* Instanciate I/O grids */
  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> io_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* TOP side*/
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) { 
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, device_size.y() - 1));
  } 

  /* RIGHT side */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) { 
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(device_size.x() - 1, iy));
  } 

  /* BOTTOM side*/
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) { 
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  } 

  /* LEFT side */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) { 
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  /* Add instances of I/O grids to top_module */
  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grids[io_coordinate.x()][io_coordinate.y()].type) {
        continue;
      } 
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[io_coordinate.x()][io_coordinate.y()].offset) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE == grids[io_coordinate.x()][io_coordinate.y()].type);
      /* Add a grid module to top_module*/
      grid_instance_ids[io_coordinate.x()][io_coordinate.y()] = add_top_module_grid_instance(module_manager, top_module, grids[io_coordinate.x()][io_coordinate.y()].type, io_side, io_coordinate);
    }
  }

  return grid_instance_ids;
}

/********************************************************************
 * Add switch blocks across the FPGA fabric to the top-level module
 * Return an 2-D array of instance ids of the switch blocks that 
 * have been added
 *******************************************************************/
static 
std::vector<std::vector<size_t>> add_top_module_switch_block_instances(ModuleManager& module_manager, 
                                                                       const ModuleId& top_module, 
                                                                       const DeviceRRGSB& L_device_rr_gsb,
                                                                       const bool& compact_routing_hierarchy) {
  /* TODO: deprecate DeviceCoordinator, use vtr::Point<size_t> only! */
  DeviceCoordinator sb_range = L_device_rr_gsb.get_gsb_range();

  /* Reserve an array for the instance ids */
  std::vector<std::vector<size_t>> sb_instance_ids; 
  sb_instance_ids.resize(sb_range.get_x());
  for (size_t x = 0; x < sb_range.get_x(); ++x) {
    /* Deposite an invalid value */
    sb_instance_ids[x].resize(sb_range.get_y(), size_t(-1));
  }

  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      vtr::Point<size_t> sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
      if (true == compact_routing_hierarchy) {
        DeviceCoordinator sb_coord(ix, iy);
        const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(sb_coord);
        sb_coordinate.set_x(unique_mirror.get_sb_x()); 
        sb_coordinate.set_y(unique_mirror.get_sb_y()); 
      } 
      std::string sb_module_name = generate_switch_block_module_name(sb_coordinate);
      ModuleId sb_module = module_manager.find_module(sb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sb_module));
      /* Record the instance id */
      sb_instance_ids[rr_gsb.get_sb_x()][rr_gsb.get_sb_y()] = module_manager.num_instance(top_module, sb_module);
      /* Add the module to top_module */ 
      module_manager.add_child_module(top_module, sb_module);
      /* Set an unique name to the instance
       * Note: it is your risk to gurantee the name is unique!
       */
      module_manager.set_child_instance_name(top_module, sb_module, 
                                             sb_instance_ids[rr_gsb.get_sb_x()][rr_gsb.get_sb_y()],
                                             generate_switch_block_module_name(vtr::Point<size_t>(rr_gsb.get_sb_x(), rr_gsb.get_sb_y())));
    }
  }

  return sb_instance_ids;
}

/********************************************************************
 * Add switch blocks across the FPGA fabric to the top-level module
 *******************************************************************/
static 
std::vector<std::vector<size_t>> add_top_module_connection_block_instances(ModuleManager& module_manager, 
                                                                           const ModuleId& top_module, 
                                                                           const DeviceRRGSB& L_device_rr_gsb,
                                                                           const t_rr_type& cb_type,
                                                                           const bool& compact_routing_hierarchy) {
  DeviceCoordinator cb_range = L_device_rr_gsb.get_gsb_range();

  /* Reserve an array for the instance ids */
  std::vector<std::vector<size_t>> cb_instance_ids; 
  cb_instance_ids.resize(cb_range.get_x());
  for (size_t x = 0; x < cb_range.get_x(); ++x) {
    /* Deposite an invalid value */
    cb_instance_ids[x].resize(cb_range.get_y(), size_t(-1));
  }

  for (size_t ix = 0; ix < cb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.get_y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      vtr::Point<size_t> cb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
      if ( false == rr_gsb.is_cb_exist(cb_type) ) {
        continue;
      }
      /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
      if (true == compact_routing_hierarchy) {
        DeviceCoordinator cb_coord(ix, iy);
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
        cb_coordinate.set_x(unique_mirror.get_cb_x(cb_type)); 
        cb_coordinate.set_y(unique_mirror.get_cb_y(cb_type)); 
      } 
      std::string cb_module_name = generate_connection_block_module_name(cb_type, cb_coordinate);
      ModuleId cb_module = module_manager.find_module(cb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(cb_module));
      /* Record the instance id */
      cb_instance_ids[rr_gsb.get_cb_x(cb_type)][rr_gsb.get_cb_y(cb_type)] = module_manager.num_instance(top_module, cb_module);
      /* Add the module to top_module */ 
      module_manager.add_child_module(top_module, cb_module);
      /* Set an unique name to the instance
       * Note: it is your risk to gurantee the name is unique!
       */
      std::string cb_instance_name = generate_connection_block_module_name(cb_type, vtr::Point<size_t>(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type)));
      module_manager.set_child_instance_name(top_module, cb_module, 
                                             cb_instance_ids[rr_gsb.get_cb_x(cb_type)][rr_gsb.get_cb_y(cb_type)],
                                             cb_instance_name); 
    }
  }

  return cb_instance_ids;
}

/********************************************************************
 * Print the top-level module for the FPGA fabric in Verilog format
 * This function will 
 * 1. name the top-level module
 * 2. include dependent netlists 
 *    - User defined netlists
 *    - Auto-generated netlists
 * 3. Add the submodules to the top-level graph
 * 4. Add module nets to connect datapath ports
 * 5. Add module nets/submodules to connect configuration ports
 *******************************************************************/
void build_top_module(ModuleManager& module_manager,
                      const CircuitLibrary& circuit_lib,
                      const vtr::Point<size_t>& device_size,
                      const std::vector<std::vector<t_grid_tile>>& grids,
                      const DeviceRRGSB& L_device_rr_gsb,
                      const std::vector<t_clb_to_clb_directs>& clb2clb_directs,
                      const e_sram_orgz& sram_orgz_type,
                      const CircuitModelId& sram_model,
                      const bool& compact_routing_hierarchy,
                      const bool& duplicate_grid_pin) {
  /* Start time count */
  clock_t t_start = clock();

  vpr_printf(TIO_MESSAGE_INFO,
             "Building FPGA fabric module...");

  /* Create a module as the top-level fabric, and add it to the module manager */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.add_module(top_module_name);

  std::map<t_rr_type, std::vector<std::vector<size_t>>> cb_instance_ids;
 
  /* Add sub modules, which are grid, SB and CBX/CBY modules as instances */
  /* Add all the grids across the fabric */
  std::vector<std::vector<size_t>> grid_instance_ids = add_top_module_grid_instances(module_manager, top_module, device_size, grids);
  /* Add all the SBs across the fabric */
  std::vector<std::vector<size_t>> sb_instance_ids = add_top_module_switch_block_instances(module_manager, top_module, L_device_rr_gsb, compact_routing_hierarchy);
  /* Add all the CBX and CBYs across the fabric */
  cb_instance_ids[CHANX] = add_top_module_connection_block_instances(module_manager, top_module, L_device_rr_gsb, CHANX, compact_routing_hierarchy);
  cb_instance_ids[CHANY] = add_top_module_connection_block_instances(module_manager, top_module, L_device_rr_gsb, CHANY, compact_routing_hierarchy);

  /* Add module nets to connect the sub modules */
  add_top_module_nets_connect_grids_and_gsbs(module_manager, top_module, 
                                             device_size, grids, grid_instance_ids, 
                                             L_device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                             compact_routing_hierarchy, duplicate_grid_pin);
  /* Add inter-CLB direct connections */
  add_top_module_nets_clb2clb_direct_connections(module_manager, top_module, circuit_lib, 
                                                 device_size, grids, grid_instance_ids,
                                                 clb2clb_directs);

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, top_module);

  /* Add GPIO ports from the sub-modules under this Verilog module 
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  add_module_gpio_ports_from_child_modules(module_manager, top_module);

  /* Add shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_shared_config_bits = find_module_num_shared_config_bits_from_child_modules(module_manager, top_module); 
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, top_module, module_num_shared_config_bits);
  }

  /* Add SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_config_bits = find_module_num_config_bits_from_child_modules(module_manager, top_module, circuit_lib, sram_model, sram_orgz_type); 
  if (0 < module_num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, top_module, circuit_lib, sram_model, sram_orgz_type, module_num_config_bits);
  }

  /* Organize the list of memory modules and instances */
  organize_top_module_memory_modules(module_manager, top_module, 
                                     circuit_lib, sram_orgz_type, sram_model,
                                     device_size, grids, grid_instance_ids, 
                                     L_device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                     compact_routing_hierarchy);

  /* Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb module!
   */
  if (0 < module_manager.configurable_children(top_module).size()) {
    add_top_module_nets_memory_config_bus(module_manager, top_module, 
                                          sram_orgz_type, circuit_lib.design_tech_type(sram_model));
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %.2g seconds\n", 
             run_time_sec);  
}
