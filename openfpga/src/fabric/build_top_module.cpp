/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <map>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "rr_gsb_utils.h"
#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "module_manager_utils.h"
#include "build_top_module_utils.h"
#include "build_top_module_connection.h"
#include "build_top_module_memory.h"
#include "build_top_module_directs.h"

#include "build_module_graph_utils.h"
#include "build_top_module.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Add a instance of a grid module to the top module
 *******************************************************************/
static 
size_t add_top_module_grid_instance(ModuleManager& module_manager,
                                    const ModuleId& top_module,
                                    t_physical_tile_type_ptr grid_type,
                                    const e_side& border_side,
                                    const vtr::Point<size_t>& grid_coord) {
  /* Find the module name for this type of grid */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), is_io_type(grid_type), border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
  /* Record the instance id */
  size_t grid_instance = module_manager.num_instance(top_module, grid_module);
  /* Add the module to top_module */ 
  module_manager.add_child_module(top_module, grid_module);
  /* Set an unique name to the instance
   * Note: it is your risk to gurantee the name is unique!
   */
  std::string instance_name = generate_grid_block_instance_name(grid_module_name_prefix, std::string(grid_type->name), is_io_type(grid_type), border_side, grid_coord);
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
vtr::Matrix<size_t> add_top_module_grid_instances(ModuleManager& module_manager,
                                                  const ModuleId& top_module,
                                                  IoLocationMap& io_location_map,
                                                  const DeviceGrid& grids) {
  /* Reserve an array for the instance ids */
  vtr::Matrix<size_t> grid_instance_ids({grids.width(), grids.height()}); 
  grid_instance_ids.fill(size_t(-1));

  /* Instanciate core grids */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grids[ix][iy].type)) {
        continue;
      } 
      /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
      if ( (0 < grids[ix][iy].width_offset)
        || (0 < grids[ix][iy].height_offset)) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(false == is_io_type(grids[ix][iy].type));
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
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) { 
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, grids.height() - 1));
  } 

  /* RIGHT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) { 
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(grids.width() - 1, iy));
  } 

  /* BOTTOM side*/
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) { 
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  } 

  /* LEFT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) { 
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  /* Add instances of I/O grids to top_module */
  size_t io_counter = 0;
  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grids[io_coordinate.x()][io_coordinate.y()].type)) {
        continue;
      } 
      /* Skip width, height > 1 tiles (mostly heterogeneous blocks) */
      if ( (0 < grids[io_coordinate.x()][io_coordinate.y()].width_offset)
        || (0 < grids[io_coordinate.x()][io_coordinate.y()].height_offset)) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(true == is_io_type(grids[io_coordinate.x()][io_coordinate.y()].type));
      /* Add a grid module to top_module*/
      grid_instance_ids[io_coordinate.x()][io_coordinate.y()] = add_top_module_grid_instance(module_manager, top_module, grids[io_coordinate.x()][io_coordinate.y()].type, io_side, io_coordinate);

      /* MUST DO: register in io location mapping!
       * I/O location mapping is a critical look-up for testbench generators
       * As we add the I/O grid instances to top module by following order:
       * TOP -> RIGHT -> BOTTOM -> LEFT
       * The I/O index will increase in this way as well.
       * This organization I/O indices is also consistent to the way 
       * that GPIOs are wired in function connect_gpio_module()
       *
       * Note: if you change the GPIO function, you should update here as well!
       */
      for (int z = 0; z < grids[io_coordinate.x()][io_coordinate.y()].type->capacity; ++z) {
        io_location_map.set_io_index(io_coordinate.x(), io_coordinate.y(), z, io_counter);
        io_counter++;
      }
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
vtr::Matrix<size_t> add_top_module_switch_block_instances(ModuleManager& module_manager, 
                                                          const ModuleId& top_module, 
                                                          const DeviceRRGSB& device_rr_gsb,
                                                          const bool& compact_routing_hierarchy) {
  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();

  /* Reserve an array for the instance ids */
  vtr::Matrix<size_t> sb_instance_ids({sb_range.x(), sb_range.y()}); 
  sb_instance_ids.fill(size_t(-1));

  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);

      if ( false == rr_gsb.is_sb_exist() ) {
        continue;
      }

      vtr::Point<size_t> sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
      if (true == compact_routing_hierarchy) {
        vtr::Point<size_t> sb_coord(ix, iy);
        const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(sb_coord);
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
vtr::Matrix<size_t> add_top_module_connection_block_instances(ModuleManager& module_manager, 
                                                              const ModuleId& top_module, 
                                                              const DeviceRRGSB& device_rr_gsb,
                                                              const t_rr_type& cb_type,
                                                              const bool& compact_routing_hierarchy) {
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
      vtr::Point<size_t> cb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
      if ( false == rr_gsb.is_cb_exist(cb_type) ) {
        continue;
      }
      /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
      if (true == compact_routing_hierarchy) {
        vtr::Point<size_t> cb_coord(ix, iy);
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
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
                      IoLocationMap& io_location_map,
                      const CircuitLibrary& circuit_lib,
                      const DeviceGrid& grids,
                      const RRGraph& rr_graph,
                      const DeviceRRGSB& device_rr_gsb,
                      const TileDirect& tile_direct,
                      const ArchDirect& arch_direct,
                      const e_config_protocol_type& sram_orgz_type,
                      const CircuitModelId& sram_model,
                      const bool& compact_routing_hierarchy,
                      const bool& duplicate_grid_pin) {

  vtr::ScopedStartFinishTimer timer("Build FPGA fabric module");

  /* Create a module as the top-level fabric, and add it to the module manager */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.add_module(top_module_name);

  std::map<t_rr_type, vtr::Matrix<size_t>> cb_instance_ids;
 
  /* Add sub modules, which are grid, SB and CBX/CBY modules as instances */
  /* Add all the grids across the fabric */
  vtr::Matrix<size_t> grid_instance_ids = add_top_module_grid_instances(module_manager, top_module, io_location_map, grids);
  /* Add all the SBs across the fabric */
  vtr::Matrix<size_t> sb_instance_ids = add_top_module_switch_block_instances(module_manager, top_module, device_rr_gsb, compact_routing_hierarchy);
  /* Add all the CBX and CBYs across the fabric */
  cb_instance_ids[CHANX] = add_top_module_connection_block_instances(module_manager, top_module, device_rr_gsb, CHANX, compact_routing_hierarchy);
  cb_instance_ids[CHANY] = add_top_module_connection_block_instances(module_manager, top_module, device_rr_gsb, CHANY, compact_routing_hierarchy);

  /* Add module nets to connect the sub modules */
  add_top_module_nets_connect_grids_and_gsbs(module_manager, top_module, 
                                             grids, grid_instance_ids, 
                                             rr_graph, device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                             compact_routing_hierarchy, duplicate_grid_pin);
  /* Add inter-CLB direct connections */
  add_top_module_nets_tile_direct_connections(module_manager, top_module, circuit_lib, 
                                              grids, grid_instance_ids,
                                              tile_direct, arch_direct);

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
                                     grids, grid_instance_ids, 
                                     device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                     compact_routing_hierarchy);

  /* Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb module!
   */
  if (0 < module_manager.configurable_children(top_module).size()) {
    add_top_module_nets_memory_config_bus(module_manager, top_module, 
                                          sram_orgz_type, circuit_lib.design_tech_type(sram_model));
  }
}

} /* end namespace openfpga */
