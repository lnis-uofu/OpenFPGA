/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <fstream>
#include <map>

#include "vtr_assert.h"

#include "vpr_types.h"
#include "globals.h"

#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "module_manager_utils.h"

#include "verilog_global.h"
#include "verilog_routing.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_top_module.h"

/********************************************************************
 * Generate the name for a grid block, by considering
 * 1. if it locates on the border with given device size
 * 2. its type
 * 
 * This function is mainly used in the top-level module generation
 *******************************************************************/
static 
std::string generate_grid_block_module_name_in_top_module(const std::string& prefix,
                                                          const vtr::Point<size_t>& device_size,
                                                          const std::vector<std::vector<t_grid_tile>>& grids,
                                                          const vtr::Point<size_t>& grid_coordinate) {
  /* Determine if the grid locates at the border */
  e_side border_side = find_grid_border_side(device_size, grid_coordinate);

  return generate_grid_block_module_name(prefix, std::string(grids[grid_coordinate.x()][grid_coordinate.y()].type->name), 
                                         IO_TYPE == grids[grid_coordinate.x()][grid_coordinate.y()].type, border_side);
}


/********************************************************************
 * Add a instance of a grid module to the top module
 *******************************************************************/
static 
size_t add_top_module_grid_instance(ModuleManager& module_manager,
                                    const ModuleId& top_module,
                                    t_type_ptr grid_type,
                                    const e_side& border_side) {
  /* Find the module name for this type of grid */
  std::string grid_module_name_prefix(grid_verilog_file_name_prefix);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), IO_TYPE == grid_type, border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
  /* Record the instance id */
  size_t grid_instance = module_manager.num_instance(top_module, grid_module);
  /* Add the module to top_module */ 
  module_manager.add_child_module(top_module, grid_module);

  printf("Added grid module %s to top-level module\n", grid_module_name.c_str());

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
      grid_instance_ids[ix][iy] = add_top_module_grid_instance(module_manager, top_module,
                                                               grids[ix][iy].type,
                                                               NUM_SIDES);
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
      grid_instance_ids[io_coordinate.x()][io_coordinate.y()] = add_top_module_grid_instance(module_manager, top_module, grids[io_coordinate.x()][io_coordinate.y()].type, io_side);
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
      vtr::Point<size_t> sb_coordinate(ix, iy);
      /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
      if (true == compact_routing_hierarchy) {
        DeviceCoordinator sb_coord(sb_coordinate.x(), sb_coordinate.y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(sb_coord);
        sb_coordinate.set_x(unique_mirror.get_sb_x()); 
        sb_coordinate.set_y(unique_mirror.get_sb_y()); 
      } 
      std::string sb_module_name = generate_switch_block_module_name(sb_coordinate);
      ModuleId sb_module = module_manager.find_module(sb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sb_module));
      /* Record the instance id */
      sb_instance_ids[ix][iy] = module_manager.num_instance(top_module, sb_module);
      /* Add the module to top_module */ 
      module_manager.add_child_module(top_module, sb_module);
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
      vtr::Point<size_t> cb_coordinate(ix, iy);
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      const DeviceCoordinator cb_coordinator = rr_gsb.get_cb_coordinator(cb_type);
      if ( (TRUE != is_cb_exist(cb_type, cb_coordinator.get_x(), cb_coordinator.get_y()))
        || (true != rr_gsb.is_cb_exist(cb_type))) {
        continue;
      }
      /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
      if (true == compact_routing_hierarchy) {
        DeviceCoordinator cb_coord(cb_coordinate.x(), cb_coordinate.y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
        cb_coordinate.set_x(unique_mirror.get_cb_x(cb_type)); 
        cb_coordinate.set_y(unique_mirror.get_cb_y(cb_type)); 
      } 
      std::string cb_module_name = generate_connection_block_module_name(cb_type, cb_coordinate);
      ModuleId cb_module = module_manager.find_module(cb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(cb_module));
      /* Record the instance id */
      cb_instance_ids[ix][iy] = module_manager.num_instance(top_module, cb_module);
      /* Add the module to top_module */ 
      module_manager.add_child_module(top_module, cb_module);
    }
  }

  return cb_instance_ids;
}

/********************************************************************
 * Add module nets to connect a GSB to adjacent grid ports/pins 
 * as well as connection blocks
 * This function will create nets for the following types of connections
 * between grid output pins of Switch block and adjacent grids
 * In this case, the net source is the grid pin, while the net sink
 * is the switch block pin
 *    
 *    +------------+                +------------+
 *    |            |                |            |
 *    |    Grid    |                |    Grid    |
 *    |  [x][y+1]  |                | [x+1][y+1] |
 *    |            |                |            |
 *    +------------+                +------------+
 *           |                           |
 *           |       +------------+      |
 *           +------>|            |<-----+
 *                   |   Switch   |
 *                   |   Block    |
 *           +------>|   [x][y]   |<-----+
 *           |       +------------+      |
 *           |                           |
 *           |                           |
 *    +------------+                +------------+
 *    |            |                |            |
 *    |    Grid    |                |    Grid    |
 *    |   [x][y]   |                |  [x+1][y]  |
 *    |            |                |            |
 *    +------------+                +------------+
 *
 *******************************************************************/
static 
void add_top_module_nets_connect_grids_and_sb(ModuleManager& module_manager, 
                                              const ModuleId& top_module, 
                                              const vtr::Point<size_t>& device_size,
                                              const std::vector<std::vector<t_grid_tile>>& grids,
                                              const std::vector<std::vector<size_t>>& grid_instance_ids,
                                              const RRGSB& rr_gsb, 
                                              const std::vector<std::vector<size_t>>& sb_instance_ids) {
  vtr::Point<size_t> sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t inode = 0; inode < rr_gsb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      /* Collect source-related information */
      /* Generate the grid module name by considering if it locates on the border */
      vtr::Point<size_t> grid_coordinate(rr_gsb.get_opin_node(side_manager.get_side(), inode)->xlow, (rr_gsb.get_opin_node(side_manager.get_side(), inode)->ylow));
      std::string src_grid_module_name = generate_grid_block_module_name_in_top_module(std::string(grid_verilog_file_name_prefix), device_size, grids, grid_coordinate); 
      ModuleId src_grid_module = module_manager.find_module(src_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(src_grid_module));
      size_t src_grid_instance = grid_instance_ids[grid_coordinate.x()][grid_coordinate.y()];
      size_t src_grid_pin_index = rr_gsb.get_opin_node(side_manager.get_side(), inode)->ptc_num;
      size_t src_grid_pin_height = find_grid_pin_height(grids, grid_coordinate, src_grid_pin_index);
      std::string src_grid_port_name = generate_grid_port_name(grid_coordinate, src_grid_pin_height, rr_gsb.get_opin_node_grid_side(side_manager.get_side(), inode), src_grid_pin_index, false);
      ModulePortId src_grid_port_id = module_manager.find_module_port(src_grid_module, src_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_grid_port_id));
      BasicPort src_grid_port = module_manager.module_port(src_grid_module, src_grid_port_id); 

      /* Collect sink-related information */
      std::string sink_sb_module_name = generate_switch_block_module_name(sb_coordinate);
      ModuleId sink_sb_module = module_manager.find_module(sink_sb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sink_sb_module));
      size_t sink_sb_instance = sb_instance_ids[sb_coordinate.x()][sb_coordinate.y()];
      vtr::Point<size_t> sink_sb_port_coord(rr_gsb.get_opin_node(side_manager.get_side(), inode)->xlow,
                                            rr_gsb.get_opin_node(side_manager.get_side(), inode)->ylow);
      std::string sink_sb_port_name = generate_grid_side_port_name(sink_sb_port_coord,
                                                                   rr_gsb.get_opin_node_grid_side(side_manager.get_side(), inode),
                                                                   src_grid_pin_index); 
      ModulePortId sink_sb_port_id = module_manager.find_module_port(sink_sb_module, sink_sb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sink_sb_module, sink_sb_port_id));
      BasicPort sink_sb_port =  module_manager.module_port(sink_sb_module, sink_sb_port_id); 

      /* Source and sink port should match in size */
      VTR_ASSERT(src_grid_port.get_width() == sink_sb_port.get_width());
      
      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_grid_port.pins().size(); ++pin_id) {
        ModuleNetId net = module_manager.create_module_net(top_module);
        /* Configure the net source */
        printf("src_grid_module_name: %s[%lu][%lu], src_grid_instance_id: %lu (%lu)\n", 
               src_grid_module_name.c_str(), 
               grid_coordinate.x(),grid_coordinate.y(),
               src_grid_instance, module_manager.num_instance(top_module, src_grid_module));
        module_manager.add_module_net_source(top_module, net, src_grid_module, src_grid_instance, src_grid_port_id, src_grid_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(top_module, net, sink_sb_module, sink_sb_instance, sink_sb_port_id, sink_sb_port.pins()[pin_id]);
      }
    } 
  }
}

/********************************************************************
 * TODO: This function will create nets for the connections
 * between grid input pins and connection blocks
 * In this case, the net source is the connection block pin, 
 * while the net sink is the grid input
 *
 *    +------------+      +------------------+      +------------+
 *    |            |      |                  |      |            |
 *    |    Grid    |<-----| Connection Block |----->|    Grid    |
 *    |  [x][y+1]  |      |  Y-direction     |      | [x+1][y+1] | 
 *    |            |      |    [x][y+1]      |      |            |
 *    +------------+      +------------------+      +------------+
 *          ^                                            
 *          |                                              
 *    +------------+      +------------------+      
 *    | Connection |      |                  |    
 *    |    Block   |      |  Switch Block    |  
 *    | X-direction|      |      [x][y]      |    
 *    |   [x][y]   |      |                  |     
 *    +------------+      +------------------+ 
 *          |                           
 *          v                           
 *    +------------+    
 *    |            |    
 *    |    Grid    |    
 *    |   [x][y]   |   
 *    |            |      
 *    +------------+    
 *
 *******************************************************************/
static 
void add_top_module_nets_connect_grids_and_cb(ModuleManager& module_manager, 
                                              const ModuleId& top_module, 
                                              const vtr::Point<size_t>& device_size,
                                              const std::vector<std::vector<t_grid_tile>>& grids,
                                              const std::vector<std::vector<size_t>>& grid_instance_ids,
                                              const RRGSB& rr_gsb, 
                                              const std::vector<std::vector<size_t>>& cbx_instance_ids,
                                              const std::vector<std::vector<size_t>>& cby_instance_ids) {
}

/********************************************************************
 * TODO: This function will create nets for the connections
 * between connection block and switch block pins
 * Two cases should be considered:
 * a. The switch block pin denotes an input of a routing track
 * The net source is an output of a routing track of connection block
 * while the net sink is an input of a routing track of switch block
 * b. The switch block pin denotes an output of a routing track
 * The net source is an output of routing track of switch block 
 * while the net sink is an input of a routing track of connection block
 *
 *    +------------+      +------------------+      +------------+
 *    |            |      |                  |      |            |
 *    |    Grid    |      | Connection Block |      |    Grid    |
 *    |  [x][y+1]  |      |  Y-direction     |      | [x+1][y+1] | 
 *    |            |      |    [x][y+1]      |      |            |
 *    +------------+      +------------------+      +------------+
 *                             |       ^
 *                             v       |
 *    +------------+      +------------------+      +------------+
 *    | Connection |----->|                  |----->| Connection |
 *    |    Block   |      |  Switch Block    |      |  Block     |
 *    | X-direction|<-----|      [x][y]      |<-----| X-direction|
 *    |   [x][y]   |      |                  |      | [x+1][y]   |
 *    +------------+      +------------------+      +------------+
 *                            |        ^
 *                            v        |
 *    +------------+      +------------------+
 *    |            |      |                  |
 *    |    Grid    |      | Connection Block |
 *    |   [x][y]   |      |   Y-direction    |
 *    |            |      |    [x][y]        |
 *    +------------+      +------------------+

 *******************************************************************/
static 
void add_top_module_nets_connect_sb_and_cb(ModuleManager& module_manager, 
                                           const ModuleId& top_module, 
                                           const RRGSB& rr_gsb, 
                                           const std::vector<std::vector<size_t>>& sb_instance_ids,
                                           const std::vector<std::vector<size_t>>& cbx_instance_ids,
                                           const std::vector<std::vector<size_t>>& cby_instance_ids) {
}


/********************************************************************
 * Add module nets to connect the grid ports/pins to Connection Blocks
 * and Switch Blocks
 * To make it easy, this function will iterate over all the General
 * Switch Blocks (GSBs), through which we can obtain the coordinates
 * of all the grids, connection blocks and switch blocks that are 
 * supposed to be connected tightly.
 *
 * As such, we have completed all the connection for each grid.
 * There is no need to iterate over the grids
 *
 * +-------------------------+   +---------------------------------+
 * |                         |   |          Y-direction CB         |
 * |       Grid[x][y+1]      |   |              [x][y + 1]         |
 * |                         |   +---------------------------------+
 * +-------------------------+
 *                                          TOP SIDE
 *  +-------------+              +---------------------------------+
 *  |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *  |             |              |                                 |
 *  |             |              | OPIN_NODES           OPIN_NODES |
 *  | X-direction |              |                                 |
 *  |      CB     |  LEFT SIDE   |        Switch Block             |  RIGHT SIDE
 *  |   [x][y]    |              |              [x][y]             |
 *  |             |              |                                 |
 *  |             |              | CHAN_NODES           CHAN_NODES |
 *  |             |              |                                 |
 *  |             |              | OPIN_NODES           OPIN_NODES |
 *  |             |              |                                 |
 *  |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *  +-------------+              +---------------------------------+
 *                                             BOTTOM SIDE
 *******************************************************************/
static 
void add_top_module_nets_connect_grids_and_gsbs(ModuleManager& module_manager, 
                                                const ModuleId& top_module, 
                                                const vtr::Point<size_t>& device_size,
                                                const std::vector<std::vector<t_grid_tile>>& grids,
                                                const std::vector<std::vector<size_t>>& grid_instance_ids,
                                                const DeviceRRGSB& L_device_rr_gsb,
                                                const std::vector<std::vector<size_t>>& sb_instance_ids,
                                                const std::vector<std::vector<size_t>>& cbx_instance_ids,
                                                const std::vector<std::vector<size_t>>& cby_instance_ids) {
  DeviceCoordinator gsb_range = L_device_rr_gsb.get_gsb_range();
  for (size_t ix = 0; ix < gsb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.get_y(); ++iy) {
      vtr::Point<size_t> gsb_coordinate(ix, iy);
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      /* Connect the grid pins of the GSB to adjacent grids */
      add_top_module_nets_connect_grids_and_sb(module_manager, top_module, 
                                               device_size, grids, grid_instance_ids,
                                               rr_gsb, sb_instance_ids);

      add_top_module_nets_connect_grids_and_cb(module_manager, top_module, 
                                               device_size, grids, grid_instance_ids,
                                               rr_gsb, cbx_instance_ids, cby_instance_ids);

      add_top_module_nets_connect_sb_and_cb(module_manager, top_module, 
                                            rr_gsb, sb_instance_ids, cbx_instance_ids, cby_instance_ids);
    }
  }
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
void print_verilog_top_module(ModuleManager& module_manager,
                              const CircuitLibrary& circuit_lib,
                              const vtr::Point<size_t>& device_size,
                              const std::vector<std::vector<t_grid_tile>>& grids,
                              const DeviceRRGSB& L_device_rr_gsb,
                              t_sram_orgz_info* cur_sram_orgz_info,
                              const std::string& arch_name,
                              const std::string& verilog_dir,
                              const bool& compact_routing_hierarchy,
                              const bool& use_explicit_mapping) {
  /* Create a module as the top-level fabric, and add it to the module manager */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.add_module(top_module_name);
 
  /* Add sub modules, which are grid, SB and CBX/CBY modules as instances */
  /* Add all the grids across the fabric */
  std::vector<std::vector<size_t>> grid_instance_ids = add_top_module_grid_instances(module_manager, top_module, device_size, grids);
  /* Add all the SBs across the fabric */
  std::vector<std::vector<size_t>> sb_instance_ids = add_top_module_switch_block_instances(module_manager, top_module, L_device_rr_gsb, compact_routing_hierarchy);
  /* Add all the CBX and CBYs across the fabric */
  std::vector<std::vector<size_t>> cbx_instance_ids = add_top_module_connection_block_instances(module_manager, top_module, L_device_rr_gsb, CHANX, compact_routing_hierarchy);
  std::vector<std::vector<size_t>> cby_instance_ids = add_top_module_connection_block_instances(module_manager, top_module, L_device_rr_gsb, CHANY, compact_routing_hierarchy);

  /* TODO: Add module nets to connect the sub modules */
  add_top_module_nets_connect_grids_and_gsbs(module_manager, top_module, 
                                             device_size, grids, grid_instance_ids, 
                                             L_device_rr_gsb, sb_instance_ids, cbx_instance_ids, cby_instance_ids);
  /* TODO: Add inter-CLB direct connections */

  /* TODO: Add  ports to the top-level module */

  /* TODO: Add module nets to connect the top-level ports to sub modules */

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

  /* TODO: this should be added to the cur_sram_orgz_info !!! */
  t_spice_model* mem_model = NULL;
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, & mem_model);
  CircuitModelId sram_model = circuit_lib.model(mem_model->name);  
  VTR_ASSERT(CircuitModelId::INVALID() != sram_model);

  /* Add SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_config_bits = find_module_num_config_bits_from_child_modules(module_manager, top_module, circuit_lib, sram_model, cur_sram_orgz_info->type); 
  if (0 < module_num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, top_module, circuit_lib, sram_model, cur_sram_orgz_info->type, module_num_config_bits);
  }

  /* Vectors to record all the memory modules have been added
   * They are used to add module nets of configuration bus
   */
  std::vector<ModuleId> memory_modules;
  std::vector<size_t> memory_instances;

  /* TODO: Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb module!
   */
  if (false == memory_modules.empty()) {
    add_module_nets_memory_config_bus(module_manager, top_module, 
                                      memory_modules, memory_instances,
                                      cur_sram_orgz_info->type, circuit_lib.design_tech_type(sram_model));
  }

  /* Start printing out Verilog netlists */
  /* Create the file name for Verilog netlist */
  std::string verilog_fname(verilog_dir + generate_fpga_top_netlist_name(std::string(verilog_netlist_file_postfix)));
  /* TODO: remove the bak file when the file is ready */
  verilog_fname += ".bak";

  vpr_printf(TIO_MESSAGE_INFO,
             "Writing Verilog Netlist for top-level module of FPGA fabric (%s)...\n",
             verilog_fname.c_str());

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  print_verilog_file_header(fp, std::string("Top-level Verilog module for FPGA architecture: " + std::string(arch_name))); 

  /* Print preprocessing flags */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Write the module content in Verilog format */
  write_verilog_module_to_file(fp, module_manager, top_module, use_explicit_mapping);

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();
}
