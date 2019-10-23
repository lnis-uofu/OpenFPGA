/********************************************************************
 * This file includes functions that are used to organize memories 
 * in the top module of FPGA fabric
 *******************************************************************/
#include "vtr_assert.h"

#include "fpga_x2p_utils.h"
#include "fpga_x2p_naming.h"

#include "globals.h"
#include "verilog_global.h"

#include "module_manager_utils.h"
#include "build_top_module_memory.h"

/********************************************************************
 * This function adds the CBX/CBY of a tile
 * to the memory modules and memory instances 
 * This function is designed for organizing memory modules in top-level
 * module
 *******************************************************************/
static 
void organize_top_module_tile_cb_modules(ModuleManager& module_manager, 
                                         const ModuleId& top_module,
                                         const CircuitLibrary& circuit_lib,
                                         const e_sram_orgz& sram_orgz_type,
                                         const CircuitModelId& sram_model,
                                         const std::vector<std::vector<size_t>>& cb_instance_ids,
                                         const DeviceRRGSB& L_device_rr_gsb,
                                         const RRGSB& rr_gsb,
                                         const t_rr_type& cb_type,
                                         const bool& compact_routing_hierarchy) {
  /* If the CB does not exist, we can skip addition */
  if ( (TRUE != is_cb_exist(cb_type, rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type)))
    || (true != rr_gsb.is_cb_exist(cb_type))) {
    return;
  }

  vtr::Point<size_t> cb_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
  if (true == compact_routing_hierarchy) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, DeviceCoordinator(cb_coord.x(), cb_coord.y()));
    cb_coord.set_x(unique_mirror.get_cb_x(cb_type)); 
    cb_coord.set_y(unique_mirror.get_cb_y(cb_type)); 
  } 

  std::string cb_module_name = generate_connection_block_module_name(cb_type, cb_coord);
  ModuleId cb_module = module_manager.find_module(cb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

  /* Identify if this sub module includes configuration bits, 
   * we will update the memory module and instance list
   */
  if (0 < find_module_num_config_bits(module_manager, cb_module,
                                      circuit_lib, sram_model, 
                                      sram_orgz_type)) {
    module_manager.add_configurable_child(top_module, cb_module, cb_instance_ids[cb_coord.x()][cb_coord.y()]);
  }
}

/********************************************************************
 * This function adds the SB, CBX, CBY and Grid of a tile
 * to the memory modules and memory instances 
 * This function is designed for organizing memory modules in top-level
 * module
 *******************************************************************/
static 
void organize_top_module_tile_memory_modules(ModuleManager& module_manager, 
                                             const ModuleId& top_module,
                                             const CircuitLibrary& circuit_lib,
                                             const e_sram_orgz& sram_orgz_type,
                                             const CircuitModelId& sram_model,
                                             const std::vector<std::vector<t_grid_tile>>& grids,
                                             const std::vector<std::vector<size_t>>& grid_instance_ids,
                                             const DeviceRRGSB& L_device_rr_gsb,
                                             const std::vector<std::vector<size_t>>& sb_instance_ids,
                                             const std::map<t_rr_type, std::vector<std::vector<size_t>>>& cb_instance_ids,
                                             const bool& compact_routing_hierarchy,
                                             const vtr::Point<size_t>& tile_coord,
                                             const e_side& tile_border_side) {

  vtr::Point<size_t> gsb_coord_range(L_device_rr_gsb.get_gsb_range().get_x(), L_device_rr_gsb.get_gsb_range().get_y());

  vtr::Point<size_t> gsb_coord(tile_coord.x(), tile_coord.y() - 1);

  /* We do NOT consider SB and CBs if the gsb is not in the range! */
  if ( (gsb_coord.x() < gsb_coord_range.x())
    && (gsb_coord.y() < gsb_coord_range.y()) ) { 
    const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(gsb_coord.x(), gsb_coord.y());
    /* Find Switch Block: unique module id and instance id!
     * Note that switch block does always exist in a GSB
     */
    vtr::Point<size_t> sb_coord(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
    /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
    if (true == compact_routing_hierarchy) {
      const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(DeviceCoordinator(sb_coord.x(), sb_coord.y()));
      sb_coord.set_x(unique_mirror.get_sb_x()); 
      sb_coord.set_y(unique_mirror.get_sb_y()); 
    } 
    std::string sb_module_name = generate_switch_block_module_name(sb_coord);
    ModuleId sb_module = module_manager.find_module(sb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

    /* Identify if this sub module includes configuration bits, 
     * we will update the memory module and instance list
     */
    if (0 < find_module_num_config_bits(module_manager, sb_module,
                                        circuit_lib, sram_model, 
                                        sram_orgz_type)) {
      module_manager.add_configurable_child(top_module, sb_module, sb_instance_ids[sb_coord.x()][sb_coord.y()]);
    }
    
    /* Try to find and add CBX and CBY */
    organize_top_module_tile_cb_modules(module_manager, top_module, circuit_lib,
                                        sram_orgz_type, sram_model,
                                        cb_instance_ids.at(CHANX),
                                        L_device_rr_gsb, rr_gsb, CHANX,
                                        compact_routing_hierarchy);

    organize_top_module_tile_cb_modules(module_manager, top_module, circuit_lib,
                                        sram_orgz_type, sram_model,
                                        cb_instance_ids.at(CHANY),
                                        L_device_rr_gsb, rr_gsb, CHANY,
                                        compact_routing_hierarchy);
  }

  /* Find the module name for this type of grid */
  t_type_ptr grid_type = grids[tile_coord.x()][tile_coord.y()].type;
  std::string grid_module_name_prefix(grid_verilog_file_name_prefix);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), IO_TYPE == grid_type, tile_border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

  /* Identify if this sub module includes configuration bits, 
   * we will update the memory module and instance list
   */
  if (0 < find_module_num_config_bits(module_manager, grid_module,
                                      circuit_lib, sram_model, 
                                      sram_orgz_type)) {
    module_manager.add_configurable_child(top_module, grid_module, grid_instance_ids[tile_coord.x()][tile_coord.y()]);
  }
}

/********************************************************************
 * Organize the list of memory modules and instances
 * This function will record all the sub modules of the top-level module
 * (those have memory ports) to two lists:
 * 1. memory_modules records the module ids
 * 2. memory_instances records the instance ids
 * To keep a clean memory connection between sub modules and top-level module,
 * the sequence of memory_modules and memory_instances will follow
 * a chain of tiles considering their physical location
 *
 * Inter tile connection:
 *    +--------------------------------------------------------+
 *    |              +------+------+-----+------+              |
 *    |              | I/O  | I/O  | ... | I/O  |              |
 *    |              | TOP  | TOP  |     | TOP  |              |
 *    |              +------+------+-----+------+              |
 *    |           +---------------------------------->tail     |
 *    |  +------+ |  +------+------+-----+------+   +------+   |
 *    |  |      | |  |      |      |     |      |   |      |   |
 *    |  | I/O  | |  | Tile | Tile | ... | Tile |   | I/O  |   |
 *    |  | LEFT | |  | [h+1]| [h+2]|     |  [n] |   |RIGHT |   |
 *    |  +------+ |  +------+------+-----+------+   +------+   |
 *    |           +-------------------------------+            |
 *    |    ...         ...    ...    ...   ...    |   ...      |
 *    |           +-------------------------------+            |
 *    |  +------+ |  +------+------+-----+------+   +------+   |
 *    |  |      | |  |      |      |     |      |   |      |   |
 *    |  | I/O  | |  | Tile | Tile | ... | Tile |   | I/O  |   |
 *    |  | LEFT | |  | [i+1]| [i+2]|     | [j]  |   |RIGHT |   |
 *    |  +------+ |  +------+------+-----+------+   +------+   |
 *    |           +-------------------------------+            |
 *    |  +------+    +------+------+-----+------+ | +------+   |
 *    |  |      |    |      |      |     |      | | |      |   |
 *    |  | I/O  |    | Tile | Tile | ... | Tile | | | I/O  |   |
 *    |  | LEFT |    | [0]  |  [1] |     |  [i] | | |RIGHT |   |
 *    |  +------+    +------+------+-----+------+ | +------+   |
 *    +-------------------------------------------+            |
 *                   +------+------+-----+------+              |
 *                   | I/O  | I/O  | ... | I/O  |              |
 *                   |BOTTOM|BOTTOM|     |BOTTOM|              |
 *                   +------+------+-----+------+              |
 *        head >-----------------------------------------------+
 *
 * Inner tile connection
 *
 *       Tile
 *     +---------------+----------+
 *   <-+---------------+ +        |
 *     |               | |        |
 *     |     CLB       | | CBY    |
 *     |             +-|-+        |  
 *     |             | |          |
 *     +---------------+----------+
 *     |             +-+----+-----+---<---
 *     |     CBX       |   SB     |
 *     |               |          |
 *     +---------------+----------+
 *
 *******************************************************************/
void organize_top_module_memory_modules(ModuleManager& module_manager, 
                                        const ModuleId& top_module,
                                        const CircuitLibrary& circuit_lib,
                                        const e_sram_orgz& sram_orgz_type,
                                        const CircuitModelId& sram_model,
                                        const vtr::Point<size_t>& device_size,
                                        const std::vector<std::vector<t_grid_tile>>& grids,
                                        const std::vector<std::vector<size_t>>& grid_instance_ids,
                                        const DeviceRRGSB& L_device_rr_gsb,
                                        const std::vector<std::vector<size_t>>& sb_instance_ids,
                                        const std::map<t_rr_type, std::vector<std::vector<size_t>>>& cb_instance_ids,
                                        const bool& compact_routing_hierarchy) {
  /* Ensure clean vectors to return */
  VTR_ASSERT(true == module_manager.configurable_children(top_module).empty());

  /* First, organize the I/O tiles on the border */
  /* Special for the I/O tileas on RIGHT and BOTTOM,
   * which are only I/O blocks, which do NOT contain CBs and SBs 
   */
  std::vector<e_side> io_sides{BOTTOM, RIGHT, TOP, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coords;

  /* BOTTOM side I/Os */
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    io_coords[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  }

  /* RIGHT side I/Os */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
    io_coords[RIGHT].push_back(vtr::Point<size_t>(device_size.x() - 1, iy));
  }

  /* TOP side I/Os */
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    io_coords[TOP].push_back(vtr::Point<size_t>(ix, device_size.y() - 1));
  }

  /* LEFT side I/Os */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
    io_coords[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coord : io_coords[io_side]) {
      /* Identify the GSB that surrounds the grid */
      organize_top_module_tile_memory_modules(module_manager, top_module, 
                                              circuit_lib, sram_orgz_type, sram_model,
                                              grids, grid_instance_ids,
                                              L_device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                              compact_routing_hierarchy,
                                              io_coord, io_side);
    }
  }

  /* For the core grids */
  std::vector<vtr::Point<size_t>> core_coords;
  bool positive_direction = true;
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
    /* For positive direction: -----> */
    if (true == positive_direction) {
      for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
        core_coords.push_back(vtr::Point<size_t>(ix, iy)); 
      }
    } else {
      VTR_ASSERT(false == positive_direction);
      /* For negative direction: -----> */
      for (size_t ix = device_size.x() - 2; ix >= 1; --ix) {
        core_coords.push_back(vtr::Point<size_t>(ix, iy)); 
      }
    }
    /* Flip the positive direction to be negative */
    positive_direction = !positive_direction;
  }

  for (const vtr::Point<size_t>& core_coord : core_coords) {
    organize_top_module_tile_memory_modules(module_manager, top_module,  
                                            circuit_lib, sram_orgz_type, sram_model, 
                                            grids, grid_instance_ids,
                                            L_device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                            compact_routing_hierarchy,
                                            core_coord, NUM_SIDES);
  }
}


/*********************************************************************
 * Add the port-to-port connection between all the memory modules 
 * and their parent module
 *
 * Create nets to wire the control signals of memory module to 
 *    the configuration ports of primitive module
 *
 * Configuration Chain 
 * -------------------
 *
 *        config_bus (head)   config_bus (tail) 
 *            |                   ^
 * primitive  |                   |
 *   +---------------------------------------------+
 *   |        |                   |                |
 *   |        v                   |                |
 *   |  +-------------------------------------+    |
 *   |  |        CMOS-based Memory Modules    |    |
 *   |  +-------------------------------------+    |
 *   |        |                   |                |
 *   |        v                   v                |
 *   |     sram_out             sram_outb          |
 *   |                                             |
 *   +---------------------------------------------+
 *
 * Memory bank 
 * -----------
 *
 *        config_bus (BL)   config_bus (WL) 
 *            |                   |
 * primitive  |                   |
 *   +---------------------------------------------+
 *   |        |                   |                |
 *   |        v                   v                |
 *   |  +-------------------------------------+    |
 *   |  |        CMOS-based Memory Modules    |    |
 *   |  +-------------------------------------+    |
 *   |        |                   |                |
 *   |        v                   v                |
 *   |     sram_out             sram_outb          |
 *   |                                             |
 *   +---------------------------------------------+
 *
 **********************************************************************/
static 
void add_top_module_nets_cmos_memory_config_bus(ModuleManager& module_manager,
                                                const ModuleId& parent_module,
                                                const e_sram_orgz& sram_orgz_type) {
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* Nothing to do */
    break;
  case SPICE_SRAM_SCAN_CHAIN: {
    add_module_nets_cmos_memory_chain_config_bus(module_manager, parent_module, SPICE_SRAM_SCAN_CHAIN);
    break;
  }
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO: */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}


/********************************************************************
 * TODO:
 * Add the port-to-port connection between a memory module 
 * and the configuration bus of a primitive module
 *
 * Create nets to wire the control signals of memory module to 
 *    the configuration ports of primitive module
 *
 *              Primitive module
 *             +----------------------------+
 *             |                +--------+  |
 *  config     |                |        |  |
 *   ports --->|--------------->| Memory |  |
 *             |                | Module |  |
 *             |                |        |  |
 *             |                +--------+  |
 *             +----------------------------+
 *     The detailed config ports really depend on the type
 *     of SRAM organization. 
 *
 * The config_bus in the argument is the reserved address of configuration
 * bus in the parent_module for this memory module
 *
 * The configuration bus connection will depend not only 
 * the design technology of the memory cells but also the 
 * configuration styles of FPGA fabric.
 * Here we will branch on the design technology
 *
 * Note: this function SHOULD be called after the pb_type_module is created
 * and its child module (logic_module and memory_module) is created! 
 *******************************************************************/
void add_top_module_nets_memory_config_bus(ModuleManager& module_manager,
                                           const ModuleId& parent_module,
                                           const e_sram_orgz& sram_orgz_type, 
                                           const e_spice_model_design_tech& mem_tech) {
  switch (mem_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    add_top_module_nets_cmos_memory_config_bus(module_manager, parent_module, 
                                               sram_orgz_type);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* TODO: */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of memory design technology !\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

