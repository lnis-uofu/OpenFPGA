/********************************************************************
 * This file includes functions that are used to organize memories 
 * in the top module of FPGA fabric
 *******************************************************************/
#include <cmath>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "rr_gsb_utils.h"
#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "memory_utils.h"
#include "decoder_library_utils.h"
#include "module_manager_utils.h"
#include "build_decoder_modules.h"
#include "build_top_module_memory.h"

/* begin namespace openfpga */
namespace openfpga {

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
                                         const e_config_protocol_type& sram_orgz_type,
                                         const CircuitModelId& sram_model,
                                         const vtr::Matrix<size_t>& cb_instance_ids,
                                         const DeviceRRGSB& device_rr_gsb,
                                         const RRGSB& rr_gsb,
                                         const t_rr_type& cb_type,
                                         const bool& compact_routing_hierarchy) {
  /* If the CB does not exist, we can skip addition */
  if ( false == rr_gsb.is_cb_exist(cb_type)) {
    return;
  }

  /* Skip if the cb does not contain any configuration bits! */
  if (true == connection_block_contain_only_routing_tracks(rr_gsb, cb_type)) {
    return;
  }

  vtr::Point<size_t> cb_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
  if (true == compact_routing_hierarchy) {
    /* Note: use GSB coordinate when inquire for unique modules!!! */
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(cb_type, vtr::Point<size_t>(rr_gsb.get_x(), rr_gsb.get_y()));
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
    /* Note that use the original CB coodinate for instance id searching ! */
    module_manager.add_configurable_child(top_module, cb_module, cb_instance_ids[rr_gsb.get_cb_x(cb_type)][rr_gsb.get_cb_y(cb_type)]);
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
                                             const e_config_protocol_type& sram_orgz_type,
                                             const CircuitModelId& sram_model,
                                             const DeviceGrid& grids,
                                             const vtr::Matrix<size_t>& grid_instance_ids,
                                             const DeviceRRGSB& device_rr_gsb,
                                             const vtr::Matrix<size_t>& sb_instance_ids,
                                             const std::map<t_rr_type, vtr::Matrix<size_t>>& cb_instance_ids,
                                             const bool& compact_routing_hierarchy,
                                             const vtr::Point<size_t>& tile_coord,
                                             const e_side& tile_border_side) {

  vtr::Point<size_t> gsb_coord_range = device_rr_gsb.get_gsb_range();

  vtr::Point<size_t> gsb_coord(tile_coord.x(), tile_coord.y() - 1);

  /* We do NOT consider SB and CBs if the gsb is not in the range! */
  if ( (gsb_coord.x() < gsb_coord_range.x())
    && (gsb_coord.y() < gsb_coord_range.y()) ) { 
    const RRGSB& rr_gsb = device_rr_gsb.get_gsb(gsb_coord.x(), gsb_coord.y());
    /* Find Switch Block: unique module id and instance id!
     * Note that switch block does always exist in a GSB
     */
    vtr::Point<size_t> sb_coord(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
    /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
    if (true == compact_routing_hierarchy) {
      const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(sb_coord);
      sb_coord.set_x(unique_mirror.get_sb_x()); 
      sb_coord.set_y(unique_mirror.get_sb_y()); 
    } 
    std::string sb_module_name = generate_switch_block_module_name(sb_coord);
    ModuleId sb_module = module_manager.find_module(sb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

    /* Identify if this sub module includes configuration bits, 
     * we will update the memory module and instance list
     */
    /* If the CB does not exist, we can skip addition */
    if ( true == rr_gsb.is_sb_exist()) {
      if (0 < find_module_num_config_bits(module_manager, sb_module,
                                          circuit_lib, sram_model, 
                                          sram_orgz_type)) {
        module_manager.add_configurable_child(top_module, sb_module, sb_instance_ids[rr_gsb.get_sb_x()][rr_gsb.get_sb_y()]);
      }
    }
    
    /* Try to find and add CBX and CBY */
    organize_top_module_tile_cb_modules(module_manager, top_module, circuit_lib,
                                        sram_orgz_type, sram_model,
                                        cb_instance_ids.at(CHANX),
                                        device_rr_gsb, rr_gsb, CHANX,
                                        compact_routing_hierarchy);

    organize_top_module_tile_cb_modules(module_manager, top_module, circuit_lib,
                                        sram_orgz_type, sram_model,
                                        cb_instance_ids.at(CHANY),
                                        device_rr_gsb, rr_gsb, CHANY,
                                        compact_routing_hierarchy);
  }

  /* Find the module name for this type of grid */
  t_physical_tile_type_ptr grid_type = grids[tile_coord.x()][tile_coord.y()].type;

  /* Skip EMPTY Grid */
  if (true == is_empty_type(grid_type)) {
    return;
  }
  /* Skip width > 1 or height > 1 Grid, which should already been processed when offset=0 */
  if ( (0 < grids[tile_coord.x()][tile_coord.y()].width_offset) 
    || (0 < grids[tile_coord.x()][tile_coord.y()].height_offset) ) {
    return;
  }

  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), is_io_type(grid_type), tile_border_side);
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
 * Inter-tile connection:
 *
 * Inter-tile connection always start from the I/O peripherals 
 * and the core tiles (CLBs and heterogeneous blocks).
 * The sequence of configuration memory will be organized as follows:
 *   - I/O peripherals
 *     - BOTTOM side (From left to right)
 *     - RIGHT side (From bottom to top)
 *     - TOP side (From left to right)
 *     - LEFT side (From top to bottom)
 *   - Core tiles
 *     - Tiles at the bottom row, i.e., Tile[0..i] (From left to right)
 *     - One row upper, i.e. Tile[i+1 .. j] (From right to left)
 *     - Repeat until we finish all the rows
 *
 *   Note: the tail may not always be on the top-right corner as shown in the figure.
 *         It may exit at the top-left corner.
 *         This really depends on the number of rows your have in the core tile array.
 *
 *   Note: the organization of inter-tile aims to reduce the wire length
 *         to connect between tiles. Therefore, it is organized as a snake
 *         where we can avoid long wires between rows and columns
 *
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
 * Inner tile connection:
 *
 *   Inside each tile, the configuration memory will be organized
 *   in the following sequence:
 *     - Switch Block (SB)
 *     - X-directional Connection Block (CBX)
 *     - Y-directional Connection Block (CBY)
 *     - Configurable Logic Block (CLB), which could also be heterogeneous blocks
 *
 *   Note:
 *     Due to multi-column and multi-width hetergeoenous blocks,
 *     each tile may not have one or more of SB, CBX, CBY, CLB
 *     In such case, the sequence will be respected.
 *     The missing block will just be skipped when organizing the configuration memories.
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
                                        const e_config_protocol_type& sram_orgz_type,
                                        const CircuitModelId& sram_model,
                                        const DeviceGrid& grids,
                                        const vtr::Matrix<size_t>& grid_instance_ids,
                                        const DeviceRRGSB& device_rr_gsb,
                                        const vtr::Matrix<size_t>& sb_instance_ids,
                                        const std::map<t_rr_type, vtr::Matrix<size_t>>& cb_instance_ids,
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
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    io_coords[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  }

  /* RIGHT side I/Os */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
    io_coords[RIGHT].push_back(vtr::Point<size_t>(grids.width() - 1, iy));
  }

  /* TOP side I/Os 
   * Special case for TOP side: We need tile at ix = 0, which has a SB!!! 
   *
   *  TOP-LEFT CORNER of FPGA fabric
   *    
   *    +--------+ +-------+
   *    | EMPTY  | | EMPTY |
   *    | Grid   | |  CBX  |
   *    | [0][x] | |       |
   *    +--------+ +-------+
   *    +--------+ +--------+
   *    | EMPTY  | |  SB    |
   *    | CBX    | | [0][x] |
   *    +--------+ +--------+
   * 
   */
  for (size_t ix = grids.width() - 2; ix >= 1; --ix) {
    io_coords[TOP].push_back(vtr::Point<size_t>(ix, grids.height() - 1));
  }
  io_coords[TOP].push_back(vtr::Point<size_t>(0, grids.height() - 1));

  /* LEFT side I/Os */
  for (size_t iy = grids.height() - 2; iy >= 1; --iy) {
    io_coords[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coord : io_coords[io_side]) {
      /* Identify the GSB that surrounds the grid */
      organize_top_module_tile_memory_modules(module_manager, top_module, 
                                              circuit_lib, sram_orgz_type, sram_model,
                                              grids, grid_instance_ids,
                                              device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                              compact_routing_hierarchy,
                                              io_coord, io_side);
    }
  }

  /* For the core grids */
  std::vector<vtr::Point<size_t>> core_coords;
  bool positive_direction = true;
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
    /* For positive direction: -----> */
    if (true == positive_direction) {
      for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
        core_coords.push_back(vtr::Point<size_t>(ix, iy)); 
      }
    } else {
      VTR_ASSERT(false == positive_direction);
      /* For negative direction: -----> */
      for (size_t ix = grids.width() - 2; ix >= 1; --ix) {
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
                                            device_rr_gsb, sb_instance_ids, cb_instance_ids,
                                            compact_routing_hierarchy,
                                            core_coord, NUM_SIDES);
  }
}

/********************************************************************
 * Add a list of ports that are used for SRAM configuration to the FPGA 
 * top-level module
 * The type and names of added ports strongly depend on the 
 * organization of SRAMs.
 * 1. Standalone SRAMs: 
 *    two ports will be added, which are BL and WL 
 * 2. Scan-chain Flip-flops:
 *    two ports will be added, which are the head of scan-chain 
 *    and the tail of scan-chain
 *    IMPORTANT: the port size will be forced to 1 in this case 
 *               because the head and tail are both 1-bit ports!!!
 * 3. Memory decoders:
 *    - An enable signal
 *    - A BL address port
 *    - A WL address port
 *    - A data-in port for the BL decoder
 * 4. Frame-based memory:
 *    - An Enable signal
 *    - An address port, whose size depends on the number of config bits 
 *      and the maximum size of address ports of configurable children
 *    - An data_in port (single-bit)
 ********************************************************************/
void add_top_module_sram_ports(ModuleManager& module_manager, 
                               const ModuleId& module_id,
                               const CircuitLibrary& circuit_lib,
                               const CircuitModelId& sram_model,
                               const e_config_protocol_type sram_orgz_type,
                               const size_t& num_config_bits) {
  std::vector<std::string> sram_port_names = generate_sram_port_names(circuit_lib, sram_model, sram_orgz_type);
  size_t sram_port_size = generate_sram_port_size(sram_orgz_type, num_config_bits); 

  /* Add ports to the module manager */
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE: { 
    for (const std::string& sram_port_name : sram_port_names) {
      /* Add generated ports to the ModuleManager */
      BasicPort sram_port(sram_port_name, sram_port_size);
      module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
    }
    break;
  }
  case CONFIG_MEM_MEMORY_BANK: {
    BasicPort en_port(std::string(DECODER_ENABLE_PORT_NAME), 1);
    module_manager.add_port(module_id, en_port, ModuleManager::MODULE_INPUT_PORT);

    size_t bl_addr_size = find_memory_decoder_addr_size(num_config_bits);
    BasicPort bl_addr_port(std::string(DECODER_BL_ADDRESS_PORT_NAME), bl_addr_size);
    module_manager.add_port(module_id, bl_addr_port, ModuleManager::MODULE_INPUT_PORT);

    size_t wl_addr_size = find_memory_decoder_addr_size(num_config_bits);
    BasicPort wl_addr_port(std::string(DECODER_WL_ADDRESS_PORT_NAME), wl_addr_size);
    module_manager.add_port(module_id, wl_addr_port, ModuleManager::MODULE_INPUT_PORT);

    BasicPort din_port(std::string(DECODER_DATA_IN_PORT_NAME), 1);
    module_manager.add_port(module_id, din_port, ModuleManager::MODULE_INPUT_PORT);

    break;
  }
  case CONFIG_MEM_SCAN_CHAIN: { 
    /* Note that configuration chain tail is an output while head is an input 
     * IMPORTANT: this is co-designed with function generate_sram_port_names()
     * If the return vector is changed, the following codes MUST be adapted!
     */
    VTR_ASSERT(2 == sram_port_names.size());
    size_t port_counter = 0;
    for (const std::string& sram_port_name : sram_port_names) {
      /* Add generated ports to the ModuleManager */
      BasicPort sram_port(sram_port_name, sram_port_size);
      if (0 == port_counter) { 
        module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
      } else {
        VTR_ASSERT(1 == port_counter);
        module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_OUTPUT_PORT);
      }
      port_counter++;
    }
    break;
  }
  case CONFIG_MEM_FRAME_BASED: { 
    BasicPort en_port(std::string(DECODER_ENABLE_PORT_NAME), 1);
    module_manager.add_port(module_id, en_port, ModuleManager::MODULE_INPUT_PORT);

    BasicPort addr_port(std::string(DECODER_ADDRESS_PORT_NAME), num_config_bits);
    module_manager.add_port(module_id, addr_port, ModuleManager::MODULE_INPUT_PORT);

    BasicPort din_port(std::string(DECODER_DATA_IN_PORT_NAME), 1);
    module_manager.add_port(module_id, din_port, ModuleManager::MODULE_INPUT_PORT);

    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization !\n");
    exit(1);
  }
}

/*********************************************************************
 * Top-level function to add nets for memory banks
 * - Find the number of BLs and WLs required
 * - Create BL and WL decoders, and add them to decoder library
 * - Create nets to connect from top-level module inputs to inputs of decoders
 * - Create nets to connect from outputs of decoders to BL/WL of configurable children
 *
 *                            WL_enable  WL address 
 *                               |           |
 *                               v           v
 *                           +-----------------------------------------------+
 *                           |     Word Line Decoder                         |
 *                           +-----------------------------------------------+
 *               +---------+    |             |                   | 
 *  BL           |         |    |             |                   |
 *  enable  ---->|         |-----------+--------------+----  ...  |------+--> BL[0]
 *               |         |    |      |      |       |           |      |
 *               |         |    |      v      |       v           |      v
 *               | Bit     |    |   +------+  |   +------+        |  +------+
 *  BL           | Line    |    +-->| SRAM |  +-->| SRAM |        +->| SRAM |
 *  address ---->| Decoder |    |   | [0]  |  |   |  [1] |   ...  |  |  [i] |
 *               |         |    |   +------+  |   +------+        |  +------+
 *               |         |    |             |                   |
 *               |         |-----------+--------------+----  ---  | -----+--> BL[1]
 *               |         |    |      |      |       |           |      |
 *               |         |    |      v      |       v           |      v
 *               |         |    |   +------+  |   +------+        |  +------+
 *               |         |    +-->| SRAM |  |   | SRAM |        +->| SRAM |
 *               |         |    |   | [x]  |  |   | [x+1]|   ...  |  | [x+i]|
 *               |         |    |   +------+  |   +------+        |  +------+
 *               |         |    |                                 |
 *               |         |    |     ...    ...    ...           |    ...
 *               |         |    |             |                   |
 *               |         |-----------+--------------+----  ---  | -----+--> BL[y]
 *               |         |    |      |      |       |           |      |
 *               |         |    |      v      |       v           |      v
 *               |         |    |   +------+  |   +------+        |  +------+
 *               |         |    +-->| SRAM |  +-->| SRAM |        +->| SRAM |
 *               |         |    |   | [y]  |  |   |[y+1] |   ...  |  |[y+i] |
 *               |         |    |   +------+  |   +------+        |  +------+
 *  BL           |         |    v             v                   v
 *  data_in ---->|         |  WL[0]          WL[1]              WL[i]
 *               +---------+
 *
 **********************************************************************/
static 
void add_top_module_nets_cmos_memory_bank_config_bus(ModuleManager& module_manager,
                                                     DecoderLibrary& decoder_lib,
                                                     const ModuleId& top_module,
                                                     const size_t& num_config_bits) {
  /* Find Enable port from the top-level module */ 
  ModulePortId en_port = module_manager.find_module_port(top_module, std::string(DECODER_ENABLE_PORT_NAME));
  BasicPort en_port_info = module_manager.module_port(top_module, en_port);

  /* Find data-in port from the top-level module */ 
  ModulePortId din_port = module_manager.find_module_port(top_module, std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port_info = module_manager.module_port(top_module, din_port);

  /* Find BL and WL address port from the top-level module */ 
  ModulePortId bl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port_info = module_manager.module_port(top_module, bl_addr_port);

  ModulePortId wl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port_info = module_manager.module_port(top_module, wl_addr_port);

  /* Find the number of BLs and WLs required to access each memory bit */
  size_t bl_addr_size = bl_addr_port_info.get_width();
  size_t wl_addr_size = wl_addr_port_info.get_width();
  size_t num_bls = find_memory_decoder_data_size(num_config_bits);
  size_t num_wls = find_memory_decoder_data_size(num_config_bits);
  
  /* Add the BL decoder module 
   * Search the decoder library
   * If we find one, we use the module.
   * Otherwise, we create one and add it to the decoder library
   */
  DecoderId bl_decoder_id = decoder_lib.find_decoder(bl_addr_size, num_bls,
                                                     true, true, false);
  if (DecoderId::INVALID() == bl_decoder_id) {
    bl_decoder_id = decoder_lib.add_decoder(bl_addr_size, num_bls, true, true, false);
  }
  VTR_ASSERT(DecoderId::INVALID() != bl_decoder_id);

  /* Create a module if not existed yet */
  std::string bl_decoder_module_name = generate_memory_decoder_with_data_in_subckt_name(bl_addr_size, num_bls);
  ModuleId bl_decoder_module = module_manager.find_module(bl_decoder_module_name);
  if (ModuleId::INVALID() == bl_decoder_module) {
    /* BL decoder has the same ports as the frame-based decoders
     * We reuse it here
     */
    bl_decoder_module = build_bl_memory_decoder_module(module_manager,
                                                       decoder_lib,
                                                       bl_decoder_id);
  }
  VTR_ASSERT(ModuleId::INVALID() != bl_decoder_module);
  VTR_ASSERT(0 == module_manager.num_instance(top_module, bl_decoder_module));
  module_manager.add_child_module(top_module, bl_decoder_module);

  /* Add the WL decoder module 
   * Search the decoder library
   * If we find one, we use the module.
   * Otherwise, we create one and add it to the decoder library
   */
  DecoderId wl_decoder_id = decoder_lib.find_decoder(wl_addr_size, num_wls,
                                                     true, false, false);
  if (DecoderId::INVALID() == wl_decoder_id) {
    wl_decoder_id = decoder_lib.add_decoder(wl_addr_size, num_wls, true, false, false);
  }
  VTR_ASSERT(DecoderId::INVALID() != wl_decoder_id);

  /* Create a module if not existed yet */
  std::string wl_decoder_module_name = generate_memory_decoder_subckt_name(wl_addr_size, num_wls);
  ModuleId wl_decoder_module = module_manager.find_module(wl_decoder_module_name);
  if (ModuleId::INVALID() == wl_decoder_module) {
    /* BL decoder has the same ports as the frame-based decoders
     * We reuse it here
     */
    wl_decoder_module = build_wl_memory_decoder_module(module_manager,
                                                       decoder_lib,
                                                       wl_decoder_id);
  }
  VTR_ASSERT(ModuleId::INVALID() != wl_decoder_module);
  VTR_ASSERT(0 == module_manager.num_instance(top_module, wl_decoder_module));
  module_manager.add_child_module(top_module, wl_decoder_module);

  /* Add module nets from the top module to BL decoder's inputs */
  ModulePortId bl_decoder_en_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_ENABLE_PORT_NAME));
  BasicPort bl_decoder_en_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_en_port);

  ModulePortId bl_decoder_addr_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
  BasicPort bl_decoder_addr_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_addr_port);

  ModulePortId bl_decoder_din_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort bl_decoder_din_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_din_port);

  /* Top module Enable port -> BL Decoder Enable port */
  add_module_bus_nets(module_manager,
                      top_module,
                      top_module, 0, en_port,
                      bl_decoder_module, 0, bl_decoder_en_port);

  /* Top module Address port -> BL Decoder Address port */
  add_module_bus_nets(module_manager,
                      top_module,
                      top_module, 0, bl_addr_port,
                      bl_decoder_module, 0, bl_decoder_addr_port);

  /* Top module data_in port -> BL Decoder data_in port */
  add_module_bus_nets(module_manager,
                      top_module,
                      top_module, 0, din_port,
                      bl_decoder_module, 0, bl_decoder_din_port);

  /* Add module nets from the top module to WL decoder's inputs */
  ModulePortId wl_decoder_en_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_ENABLE_PORT_NAME));
  BasicPort wl_decoder_en_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_en_port);

  ModulePortId wl_decoder_addr_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
  BasicPort wl_decoder_addr_port_info = module_manager.module_port(wl_decoder_module, bl_decoder_addr_port);

  /* Top module Enable port -> WL Decoder Enable port */
  add_module_bus_nets(module_manager,
                      top_module,
                      top_module, 0, en_port,
                      wl_decoder_module, 0, wl_decoder_en_port);

  /* Top module Address port -> WL Decoder Address port */
  add_module_bus_nets(module_manager,
                      top_module,
                      top_module, 0, wl_addr_port,
                      wl_decoder_module, 0, wl_decoder_addr_port);

  /* Add nets from BL data out to each configurable child */
  size_t cur_bl_index = 0;

  ModulePortId bl_decoder_dout_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
  BasicPort bl_decoder_dout_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_dout_port);

  for (size_t child_id = 0; child_id < module_manager.configurable_children(top_module).size(); ++child_id) {
    ModuleId child_module = module_manager.configurable_children(top_module)[child_id];
    size_t child_instance = module_manager.configurable_child_instances(top_module)[child_id];

    /* Find the BL port */
    ModulePortId child_bl_port = module_manager.find_module_port(child_module, std::string(MEMORY_BL_PORT_NAME));
    BasicPort child_bl_port_info = module_manager.module_port(child_module, child_bl_port);

    for (const size_t& sink_bl_pin : child_bl_port_info.pins()) {
      /* Find the BL decoder data index: 
       * It should be the residual when divided by the number of BLs
       */
      size_t bl_pin_id = std::floor(cur_bl_index / num_bls);

      /* Create net */
      ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                     bl_decoder_module, 0,
                                                     bl_decoder_dout_port,
                                                     bl_decoder_dout_port_info.pins()[bl_pin_id]);
      VTR_ASSERT(ModuleNetId::INVALID() != net);

      /* Add net sink */
      module_manager.add_module_net_sink(top_module, net,
                                         child_module, child_instance, child_bl_port, sink_bl_pin);

      /* Increment the BL index */
      cur_bl_index++;
    }
  }

  /* Add nets from WL data out to each configurable child */
  size_t cur_wl_index = 0;

  ModulePortId wl_decoder_dout_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
  BasicPort wl_decoder_dout_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_dout_port);

  for (size_t child_id = 0; child_id < module_manager.configurable_children(top_module).size(); ++child_id) {
    ModuleId child_module = module_manager.configurable_children(top_module)[child_id];
    size_t child_instance = module_manager.configurable_child_instances(top_module)[child_id];

    /* Find the WL port */
    ModulePortId child_wl_port = module_manager.find_module_port(child_module, std::string(MEMORY_WL_PORT_NAME));
    BasicPort child_wl_port_info = module_manager.module_port(child_module, child_wl_port);

    for (const size_t& sink_wl_pin : child_wl_port_info.pins()) {
      /* Find the BL decoder data index: 
       * It should be the residual when divided by the number of BLs
       */
      size_t wl_pin_id = cur_wl_index % num_wls;

      /* Create net */
      ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                     wl_decoder_module, 0,
                                                     wl_decoder_dout_port,
                                                     wl_decoder_dout_port_info.pins()[wl_pin_id]);
      VTR_ASSERT(ModuleNetId::INVALID() != net);

      /* Add net sink */
      module_manager.add_module_net_sink(top_module, net,
                                         child_module, child_instance, child_wl_port, sink_wl_pin);

      /* Increment the WL index */
      cur_wl_index++;
    }
  }

  /* Add the BL and WL decoders to the end of configurable children list
   * Note: this MUST be done after adding all the module nets to other regular configurable children
   */
  module_manager.add_configurable_child(top_module, bl_decoder_module, 0);
  module_manager.add_configurable_child(top_module, wl_decoder_module, 0);
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
                                                DecoderLibrary& decoder_lib,
                                                const ModuleId& parent_module,
                                                const e_config_protocol_type& sram_orgz_type,
                                                const size_t& num_config_bits) {
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    add_module_nets_cmos_flatten_memory_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
    add_module_nets_cmos_flatten_memory_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
    break;
  case CONFIG_MEM_SCAN_CHAIN: {
    add_module_nets_cmos_memory_chain_config_bus(module_manager, parent_module, CONFIG_MEM_SCAN_CHAIN);
    break;
  }
  case CONFIG_MEM_MEMORY_BANK:
    add_top_module_nets_cmos_memory_bank_config_bus(module_manager, decoder_lib, parent_module, num_config_bits);
    break;
  case CONFIG_MEM_FRAME_BASED:
    add_module_nets_cmos_memory_frame_config_bus(module_manager, decoder_lib, parent_module);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization!\n");
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
                                           DecoderLibrary& decoder_lib,
                                           const ModuleId& parent_module,
                                           const e_config_protocol_type& sram_orgz_type, 
                                           const e_circuit_model_design_tech& mem_tech,
                                           const size_t& num_config_bits) {
  switch (mem_tech) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    add_top_module_nets_cmos_memory_config_bus(module_manager, decoder_lib,
                                               parent_module, 
                                               sram_orgz_type,
                                               num_config_bits);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    /* TODO: */
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                  "Invalid type of memory design technology!\n");
    exit(1);
  }
}

} /* end namespace openfpga */
