/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <map>
#include <algorithm>

#include "vtr_assert.h"

#include "vpr_types.h"
#include "globals.h"

#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "module_manager_utils.h"
#include "build_top_module_memory.h"
#include "build_top_module_directs.h"

#include "verilog_global.h"
#include "build_module_graph_utils.h"
#include "build_top_module.h"

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
 * Find the cb_type of a GSB in the top-level module 
 * depending on the side of SB
 * TOP/BOTTOM side: CHANY
 * RIGHT/LEFT side: CHANX
 *******************************************************************/
static 
t_rr_type find_top_module_cb_type_by_sb_side(const e_side& sb_side) {
  VTR_ASSERT(NUM_SIDES != sb_side);

  if ((TOP == sb_side) || (BOTTOM == sb_side)) {
    return CHANY;
  }

  VTR_ASSERT((RIGHT == sb_side) || (LEFT == sb_side));
  return CHANX;
}

/********************************************************************
 * Find the GSB coordinate for a CB in the top-level module 
 * depending on the side of a SB
 * TODO: use vtr::Point<size_t> to replace DeviceCoordinator
 *******************************************************************/
static 
DeviceCoordinator find_top_module_gsb_coordinate_by_sb_side(const RRGSB& rr_gsb,
                                                            const e_side& sb_side) {
  VTR_ASSERT(NUM_SIDES != sb_side);

  DeviceCoordinator gsb_coordinate;

  if ((TOP == sb_side) || (LEFT == sb_side)) {
    gsb_coordinate.set_x(rr_gsb.get_x()); 
    gsb_coordinate.set_y(rr_gsb.get_y()); 
    return gsb_coordinate;
  }
 
  VTR_ASSERT((RIGHT == sb_side) || (BOTTOM == sb_side));
  DeviceCoordinator side_coord = rr_gsb.get_side_block_coordinator(sb_side);

  gsb_coordinate.set_x(side_coord.get_x()); 
  gsb_coordinate.set_y(side_coord.get_y()); 

  return gsb_coordinate;
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
      const DeviceCoordinator cb_coordinator = rr_gsb.get_cb_coordinator(cb_type);
      if ( (TRUE != is_cb_exist(cb_type, cb_coordinator.get_x(), cb_coordinator.get_y()))
        || (true != rr_gsb.is_cb_exist(cb_type))) {
        continue;
      }
      /* If we use compact routing hierarchy, we should instanciate the unique module of SB */
      if (true == compact_routing_hierarchy) {
        DeviceCoordinator cb_coord(ix, iy);
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
                                              const DeviceRRGSB& L_device_rr_gsb,
                                              const RRGSB& rr_gsb, 
                                              const std::vector<std::vector<size_t>>& sb_instance_ids,
                                              const bool& compact_routing_hierarchy) {

  /* We could have two different coordinators, one is the instance, the other is the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  DeviceCoordinator module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    DeviceCoordinator gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x()); 
    module_gsb_coordinate.set_y(unique_mirror.get_y()); 
  } 

  /* This is the source cb that is added to the top module */
  const RRGSB& module_sb = L_device_rr_gsb.get_gsb(module_gsb_coordinate);
  vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(), module_sb.get_sb_y());

  /* Collect sink-related information */
  std::string sink_sb_module_name = generate_switch_block_module_name(module_sb_coordinate);
  ModuleId sink_sb_module = module_manager.find_module(sink_sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sink_sb_module));
  size_t sink_sb_instance = sb_instance_ids[instance_sb_coordinate.x()][instance_sb_coordinate.y()];

  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < module_sb.get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t inode = 0; inode < module_sb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
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
      vtr::Point<size_t> sink_sb_port_coord(module_sb.get_opin_node(side_manager.get_side(), inode)->xlow,
                                            module_sb.get_opin_node(side_manager.get_side(), inode)->ylow);
      std::string sink_sb_port_name = generate_grid_side_port_name(grids, sink_sb_port_coord,
                                                                   module_sb.get_opin_node_grid_side(side_manager.get_side(), inode),
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
        module_manager.add_module_net_source(top_module, net, src_grid_module, src_grid_instance, src_grid_port_id, src_grid_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(top_module, net, sink_sb_module, sink_sb_instance, sink_sb_port_id, sink_sb_port.pins()[pin_id]);
      }
    } 
  }
}

/********************************************************************
 * This function will create nets for the connections
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
 *
 *    Relationship between source connection block and its unique module 
 *    Take an example of a CBY
 *
 *    grid_pin name should follow unique module of Grid[x][y+1]
 *    cb_pin name should follow unique module of CBY[x][y+1]
 *    
 *    However, instace id should follow the origin Grid and Connection block 
 *                    
 *                    
 *    +------------+             +------------------+ 
 *    |            |             |                  | 
 *    |    Grid    |<------------| Connection Block |
 *    |  [x][y+1]  |             |  Y-direction     | 
 *    |            |             |    [x][y+1]      |
 *    +------------+             +------------------+
 *                                         ^
 *                                        || unique mirror
 *    +------------+             +------------------+ 
 *    |            |             |                  | 
 *    |    Grid    |<------------| Connection Block |
 *    |  [i][j+1]  |             |  Y-direction     | 
 *    |            |             |    [i][j+1]      |
 *    +------------+             +------------------+
 *
 *******************************************************************/
static 
void add_top_module_nets_connect_grids_and_cb(ModuleManager& module_manager, 
                                              const ModuleId& top_module, 
                                              const vtr::Point<size_t>& device_size,
                                              const std::vector<std::vector<t_grid_tile>>& grids,
                                              const std::vector<std::vector<size_t>>& grid_instance_ids,
                                              const DeviceRRGSB& L_device_rr_gsb,
                                              const RRGSB& rr_gsb, 
                                              const t_rr_type& cb_type,
                                              const std::vector<std::vector<size_t>>& cb_instance_ids,
                                              const bool& compact_routing_hierarchy) {
  /* We could have two different coordinators, one is the instance, the other is the module */
  vtr::Point<size_t> instance_cb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  DeviceCoordinator module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* Skip those Connection blocks that do not exist */
  if ( (TRUE != is_cb_exist(cb_type, rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type)))
    || (true != rr_gsb.is_cb_exist(cb_type))) {
    return;
  }

  /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    DeviceCoordinator gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x()); 
    module_gsb_coordinate.set_y(unique_mirror.get_y()); 
  } 

  /* This is the source cb that is added to the top module */
  const RRGSB& module_cb = L_device_rr_gsb.get_gsb(module_gsb_coordinate);
  vtr::Point<size_t> module_cb_coordinate(module_cb.get_cb_x(cb_type), module_cb.get_cb_y(cb_type));

  /* Collect source-related information */
  std::string src_cb_module_name = generate_connection_block_module_name(cb_type, module_cb_coordinate);
  ModuleId src_cb_module = module_manager.find_module(src_cb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(src_cb_module));
  /* Instance id should follow the instance cb coordinate */
  size_t src_cb_instance = cb_instance_ids[instance_cb_coordinate.x()][instance_cb_coordinate.y()];

  /* Iterate over the output pins of the Connection Block */
  std::vector<enum e_side> cb_ipin_sides = module_cb.get_cb_ipin_sides(cb_type);
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < module_cb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      /* Collect source-related information */
      t_rr_node* module_ipin_node = module_cb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> cb_src_port_coord(module_ipin_node->xlow, module_ipin_node->ylow);
      std::string src_cb_port_name = generate_grid_side_port_name(grids, cb_src_port_coord,
                                                                  module_cb.get_ipin_node_grid_side(cb_ipin_side, inode),
                                                                  module_ipin_node->ptc_num); 
      ModulePortId src_cb_port_id = module_manager.find_module_port(src_cb_module, src_cb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_cb_module, src_cb_port_id));
      BasicPort src_cb_port = module_manager.module_port(src_cb_module, src_cb_port_id); 

      /* Collect sink-related information */
      /* Note that we use the instance cb pin here!!!
       * because it has the correct coordinator for the grid!!! 
       */
      t_rr_node* instance_ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> grid_coordinate(instance_ipin_node->xlow, instance_ipin_node->ylow);
      std::string sink_grid_module_name = generate_grid_block_module_name_in_top_module(std::string(grid_verilog_file_name_prefix), device_size, grids, grid_coordinate); 
      ModuleId sink_grid_module = module_manager.find_module(sink_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sink_grid_module));
      size_t sink_grid_instance = grid_instance_ids[grid_coordinate.x()][grid_coordinate.y()];
      size_t sink_grid_pin_index = instance_ipin_node->ptc_num;
      size_t sink_grid_pin_height = find_grid_pin_height(grids, grid_coordinate, sink_grid_pin_index);
      std::string sink_grid_port_name = generate_grid_port_name(grid_coordinate, sink_grid_pin_height, rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode), sink_grid_pin_index, false);
      ModulePortId sink_grid_port_id = module_manager.find_module_port(sink_grid_module, sink_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sink_grid_module, sink_grid_port_id));
      BasicPort sink_grid_port = module_manager.module_port(sink_grid_module, sink_grid_port_id); 

      /* Source and sink port should match in size */
      VTR_ASSERT(src_cb_port.get_width() == sink_grid_port.get_width());
      
      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_cb_port.pins().size(); ++pin_id) {
        ModuleNetId net = module_manager.create_module_net(top_module);
        /* Configure the net source */
        module_manager.add_module_net_source(top_module, net, src_cb_module, src_cb_instance, src_cb_port_id, src_cb_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(top_module, net, sink_grid_module, sink_grid_instance, sink_grid_port_id, sink_grid_port.pins()[pin_id]);
      }
    }
  }
}

/********************************************************************
 * This function will create nets for the connections
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
 *    +------------+      +------------------+      +------------+
 *    |            |      |                  |      |            |
 *    |    Grid    |      | Connection Block |      |    Grid    |
 *    |   [x][y]   |      |   Y-direction    |      |  [x][y+1]  |
 *    |            |      |    [x][y]        |      |            |
 *    +------------+      +------------------+      +------------+
 *
 * Here, to achieve the purpose, we can simply iterate over the 
 * four sides of switch block and make connections to adjancent 
 * connection blocks
 *
 *******************************************************************/
static 
void add_top_module_nets_connect_sb_and_cb(ModuleManager& module_manager, 
                                           const ModuleId& top_module, 
                                           const DeviceRRGSB& L_device_rr_gsb,
                                           const RRGSB& rr_gsb, 
                                           const std::vector<std::vector<size_t>>& sb_instance_ids,
                                           const std::map<t_rr_type, std::vector<std::vector<size_t>>>& cb_instance_ids,
                                           const bool& compact_routing_hierarchy) {
  /* We could have two different coordinators, one is the instance, the other is the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  DeviceCoordinator module_gsb_sb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    DeviceCoordinator gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(gsb_coord);
    module_gsb_sb_coordinate.set_x(unique_mirror.get_x()); 
    module_gsb_sb_coordinate.set_y(unique_mirror.get_y()); 
  } 

  /* This is the source cb that is added to the top module */
  const RRGSB& module_sb = L_device_rr_gsb.get_gsb(module_gsb_sb_coordinate);
  vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(), module_sb.get_sb_y());
  std::string sb_module_name = generate_switch_block_module_name(module_sb_coordinate);
  ModuleId sb_module_id = module_manager.find_module(sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module_id));
  size_t sb_instance = sb_instance_ids[instance_sb_coordinate.x()][instance_sb_coordinate.y()];

  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < module_sb.get_num_sides(); ++side) {
    Side side_manager(side);
    /* Iterate over the routing tracks on this side */
    DeviceCoordinator port_coordinator = module_sb.get_side_block_coordinator(side_manager.get_side()); 
    /* Early skip: if there is no routing tracks at this side */
    if (0 == module_sb.get_chan_width(side_manager.get_side())) {
      continue;
    }
    /* Find the Connection Block module */
    /* We find the original connection block and then spot its unique mirror! 
     * Do NOT use module_sb here!!!
     */
    t_rr_type cb_type = find_top_module_cb_type_by_sb_side(side_manager.get_side());
    DeviceCoordinator instance_gsb_cb_coordinate = find_top_module_gsb_coordinate_by_sb_side(rr_gsb, side_manager.get_side());
    DeviceCoordinator module_gsb_cb_coordinate = find_top_module_gsb_coordinate_by_sb_side(rr_gsb, side_manager.get_side());

    /* Skip those Connection blocks that do not exist */
    if ( (TRUE != is_cb_exist(cb_type, rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type)))
      || (true != rr_gsb.is_cb_exist(cb_type))) {
      continue;
    }

    /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
    if (true == compact_routing_hierarchy) {
      const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, module_gsb_cb_coordinate);
      module_gsb_cb_coordinate.set_x(unique_mirror.get_x()); 
      module_gsb_cb_coordinate.set_y(unique_mirror.get_y()); 
    } 
  
    const RRGSB& module_cb = L_device_rr_gsb.get_gsb(module_gsb_cb_coordinate);
    vtr::Point<size_t> module_cb_coordinate(module_cb.get_cb_x(cb_type), module_cb.get_cb_y(cb_type));
    std::string cb_module_name = generate_connection_block_module_name(cb_type, module_cb_coordinate);
    ModuleId cb_module_id = module_manager.find_module(cb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_module_id));
    const RRGSB& instance_cb = L_device_rr_gsb.get_gsb(instance_gsb_cb_coordinate);
    vtr::Point<size_t> instance_cb_coordinate(instance_cb.get_cb_x(cb_type), instance_cb.get_cb_y(cb_type));
    size_t cb_instance = cb_instance_ids.at(cb_type)[instance_cb_coordinate.x()][instance_cb_coordinate.y()];
 
    for (size_t itrack = 0; itrack < module_sb.get_chan_width(side_manager.get_side()); ++itrack) {
      vtr::Point<size_t> sb_port_coord(port_coordinator.get_x(), port_coordinator.get_y());
      std::string sb_port_name = generate_routing_track_port_name(module_sb.get_chan_node(side_manager.get_side(), itrack)->type,
                                                                  sb_port_coord, itrack,  
                                                                  module_sb.get_chan_node_direction(side_manager.get_side(), itrack));
      /* Prepare SB-related port information */
      ModulePortId sb_port_id = module_manager.find_module_port(sb_module_id, sb_port_name); 
      VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module_id, sb_port_id));
      BasicPort sb_port = module_manager.module_port(sb_module_id, sb_port_id);
     
      /* Prepare CB-related port information */ 
      PORTS cb_port_direction = OUT_PORT;
      /* The cb port direction should be opposite to the sb port !!! */
      if (OUT_PORT == module_sb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        cb_port_direction = IN_PORT;
      } else {
        VTR_ASSERT(IN_PORT == module_sb.get_chan_node_direction(side_manager.get_side(), itrack));
      }  
      vtr::Point<size_t> cb_port_coord(module_cb.get_cb_x(cb_type), module_cb.get_cb_y(cb_type));
      std::string cb_port_name = generate_routing_track_port_name(cb_type,
                                                                  cb_port_coord, itrack,  
                                                                  cb_port_direction);
      ModulePortId cb_port_id = module_manager.find_module_port(cb_module_id, cb_port_name); 
      VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module_id, cb_port_id));
      BasicPort cb_port = module_manager.module_port(cb_module_id, cb_port_id);

      /* Source and sink port should match in size */
      VTR_ASSERT(cb_port.get_width() == sb_port.get_width());
      
      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < cb_port.pins().size(); ++pin_id) {
        ModuleNetId net = module_manager.create_module_net(top_module);
        /* Configure the net source and sink:
         * If sb port is an output (source), cb port is an input (sink) 
         * If sb port is an input (sink), cb port is an output (source) 
         */
        if (OUT_PORT == module_sb.get_chan_node_direction(side_manager.get_side(), itrack)) {
          module_manager.add_module_net_sink(top_module, net, cb_module_id, cb_instance, cb_port_id, cb_port.pins()[pin_id]);
          module_manager.add_module_net_source(top_module, net, sb_module_id, sb_instance, sb_port_id, sb_port.pins()[pin_id]);
        } else {
          VTR_ASSERT(IN_PORT == module_sb.get_chan_node_direction(side_manager.get_side(), itrack));
          module_manager.add_module_net_source(top_module, net, cb_module_id, cb_instance, cb_port_id, cb_port.pins()[pin_id]);
          module_manager.add_module_net_sink(top_module, net, sb_module_id, sb_instance, sb_port_id, sb_port.pins()[pin_id]);
        }  
      }
    }
  }
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
                                                const std::map<t_rr_type, std::vector<std::vector<size_t>>>& cb_instance_ids,
                                                const bool& compact_routing_hierarchy) {
  DeviceCoordinator gsb_range = L_device_rr_gsb.get_gsb_range();
  for (size_t ix = 0; ix < gsb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.get_y(); ++iy) {
      vtr::Point<size_t> gsb_coordinate(ix, iy);
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      /* Connect the grid pins of the GSB to adjacent grids */
      add_top_module_nets_connect_grids_and_sb(module_manager, top_module, 
                                               device_size, grids, grid_instance_ids,
                                               L_device_rr_gsb, rr_gsb, sb_instance_ids, 
                                               compact_routing_hierarchy);

      add_top_module_nets_connect_grids_and_cb(module_manager, top_module, 
                                               device_size, grids, grid_instance_ids,
                                               L_device_rr_gsb, rr_gsb, CHANX, cb_instance_ids.at(CHANX),
                                               compact_routing_hierarchy);

      add_top_module_nets_connect_grids_and_cb(module_manager, top_module, 
                                               device_size, grids, grid_instance_ids,
                                               L_device_rr_gsb, rr_gsb, CHANY, cb_instance_ids.at(CHANY),
                                               compact_routing_hierarchy);

      add_top_module_nets_connect_sb_and_cb(module_manager, top_module, 
                                            L_device_rr_gsb, rr_gsb, sb_instance_ids, cb_instance_ids,
                                            compact_routing_hierarchy);

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
void build_top_module(ModuleManager& module_manager,
                      const CircuitLibrary& circuit_lib,
                      const vtr::Point<size_t>& device_size,
                      const std::vector<std::vector<t_grid_tile>>& grids,
                      const DeviceRRGSB& L_device_rr_gsb,
                      const std::vector<t_clb_to_clb_directs>& clb2clb_directs,
                      const e_sram_orgz& sram_orgz_type,
                      const CircuitModelId& sram_model,
                      const bool& compact_routing_hierarchy) {
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
                                             compact_routing_hierarchy);
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
}
