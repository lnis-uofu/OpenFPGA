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
 * Check if the grid coorindate given is in the device grid range 
 *******************************************************************/
static 
bool is_grid_coordinate_exist_in_device(const vtr::Point<size_t>& device_size,
                                        const vtr::Point<size_t>& grid_coordinate) {
  return (grid_coordinate < device_size);
}

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
      vtr::Point<size_t> sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_x());
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
      std::string sink_sb_port_name = generate_grid_side_port_name(sink_sb_port_coord,
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
      std::string src_cb_port_name = generate_grid_side_port_name(cb_src_port_coord,
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
 * Add module net for one direction connection between two CLBs or
 * two grids
 * This function will 
 * 1. find the pin id and port id of the source clb port in module manager
 * 2. find the pin id and port id of the destination clb port in module manager
 * 3. add a direct connection module to the top module 
 * 4. add a first module net and configure its source and sink, 
 * in order to connect the source pin to the input of the top module
 * 4. add a second module net and configure its source and sink, 
 * in order to connect the sink pin to the output of the top module
 *******************************************************************/
static 
void add_module_nets_clb2clb_direct_connection(ModuleManager& module_manager, 
                                               const ModuleId& top_module,  
                                               const CircuitLibrary& circuit_lib, 
                                               const vtr::Point<size_t>& device_size,
                                               const std::vector<std::vector<t_grid_tile>>& grids,
                                               const std::vector<std::vector<size_t>>& grid_instance_ids,
                                               const vtr::Point<size_t>& src_clb_coord, 
                                               const vtr::Point<size_t>& des_clb_coord,  
                                               const t_clb_to_clb_directs& direct) {
  /* Find the source port and destination port on the CLBs */
  BasicPort src_clb_port;
  BasicPort des_clb_port;

  src_clb_port.set_width(direct.from_clb_pin_start_index, direct.from_clb_pin_end_index);
  des_clb_port.set_width(direct.to_clb_pin_start_index, direct.to_clb_pin_end_index);

  /* Check bandwidth match between from_clb and to_clb pins */
  if (src_clb_port.get_width() != des_clb_port.get_width()) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Unmatch pin bandwidth in direct connection (name=%s)!\n",
               __FILE__, __LINE__, direct.name);
    exit(1);
  }

  /* Find the module name of source clb */
  t_type_ptr src_grid_type = grids[src_clb_coord.x()][src_clb_coord.y()].type;
  e_side src_grid_border_side = find_grid_border_side(device_size, src_clb_coord);
  std::string src_module_name_prefix(grid_verilog_file_name_prefix);
  std::string src_module_name = generate_grid_block_module_name(src_module_name_prefix, std::string(src_grid_type->name), IO_TYPE == src_grid_type, src_grid_border_side);
  ModuleId src_grid_module = module_manager.find_module(src_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(src_grid_module));
  /* Record the instance id */
  size_t src_grid_instance = grid_instance_ids[src_clb_coord.x()][src_clb_coord.y()];

  /* Find the module name of sink clb */
  t_type_ptr sink_grid_type = grids[des_clb_coord.x()][des_clb_coord.y()].type;
  e_side sink_grid_border_side = find_grid_border_side(device_size, des_clb_coord);
  std::string sink_module_name_prefix(grid_verilog_file_name_prefix);
  std::string sink_module_name = generate_grid_block_module_name(sink_module_name_prefix, std::string(sink_grid_type->name), IO_TYPE == sink_grid_type, sink_grid_border_side);
  ModuleId sink_grid_module = module_manager.find_module(sink_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sink_grid_module));
  /* Record the instance id */
  size_t sink_grid_instance = grid_instance_ids[des_clb_coord.x()][des_clb_coord.y()];

  /* Find the module id of a direct connection module */
  std::string direct_module_name = circuit_lib.model_name(direct.circuit_model);
  ModuleId direct_module = module_manager.find_module(direct_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(direct_module));

  /* Find inputs and outputs of the direct circuit module */
  std::vector<CircuitPortId> direct_input_ports = circuit_lib.model_ports_by_type(direct.circuit_model, SPICE_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == direct_input_ports.size());
  ModulePortId direct_input_port_id = module_manager.find_module_port(direct_module, circuit_lib.port_lib_name(direct_input_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(direct_module, direct_input_port_id));
  VTR_ASSERT(1 == module_manager.module_port(direct_module, direct_input_port_id).get_width());

  std::vector<CircuitPortId> direct_output_ports = circuit_lib.model_ports_by_type(direct.circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == direct_output_ports.size());
  ModulePortId direct_output_port_id = module_manager.find_module_port(direct_module, circuit_lib.port_lib_name(direct_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(direct_module, direct_output_port_id));
  VTR_ASSERT(1 == module_manager.module_port(direct_module, direct_output_port_id).get_width());

  for (size_t pin_id : src_clb_port.pins()) {
    /* Generate the pin name of source port/pin in the grid */
    size_t src_pin_height = find_grid_pin_height(grids, src_clb_coord, src_clb_port.pins()[pin_id]);
    e_side src_pin_grid_side = find_grid_pin_side(device_size, grids, src_clb_coord, src_pin_height, src_clb_port.pins()[pin_id]);
    std::string src_port_name = generate_grid_port_name(src_clb_coord, src_pin_height, src_pin_grid_side, src_clb_port.pins()[pin_id], false);
    ModulePortId src_port_id = module_manager.find_module_port(src_grid_module, src_port_name); 
    VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_port_id));
    VTR_ASSERT(1 == module_manager.module_port(src_grid_module, src_port_id).get_width());

    /* Generate the pin name of sink port/pin in the grid */
    size_t sink_pin_height = find_grid_pin_height(grids, des_clb_coord, des_clb_port.pins()[pin_id]);
    e_side sink_pin_grid_side = find_grid_pin_side(device_size, grids, des_clb_coord, sink_pin_height, des_clb_port.pins()[pin_id]);
    std::string sink_port_name = generate_grid_port_name(des_clb_coord, sink_pin_height, sink_pin_grid_side, des_clb_port.pins()[pin_id], false);
    ModulePortId sink_port_id = module_manager.find_module_port(sink_grid_module, sink_port_name); 
    VTR_ASSERT(true == module_manager.valid_module_port_id(sink_grid_module, sink_port_id));
    VTR_ASSERT(1 == module_manager.module_port(sink_grid_module, sink_port_id).get_width());

    /* Add a submodule of direct connection module to the top-level module */
    size_t direct_instance_id = module_manager.num_instance(top_module, direct_module);
    module_manager.add_child_module(top_module, direct_module);

    /* Create the 1st module net */
    ModuleNetId net_direct_src = module_manager.create_module_net(top_module); 
    /* Connect the wire between src_pin of clb and direct_instance input*/
    module_manager.add_module_net_source(top_module, net_direct_src, src_grid_module, src_grid_instance, src_port_id, 0);
    module_manager.add_module_net_sink(top_module, net_direct_src, direct_module, direct_instance_id, direct_input_port_id, 0);

    /* Create the 2nd module net */
    ModuleNetId net_direct_sink = module_manager.create_module_net(top_module); 
    /* Connect the wire between direct_instance output and sink_pin of clb */
    module_manager.add_module_net_source(top_module, net_direct_sink, direct_module, direct_instance_id, direct_output_port_id, 0);
    module_manager.add_module_net_sink(top_module, net_direct_sink, sink_grid_module, sink_grid_instance, sink_port_id, 0);
  }
}

/********************************************************************
 * Add module net of clb-to-clb direct connections to module manager
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *
 * This function supports the following types of direct connection:
 * 1. Direct connection between grids in the same column or row
 *     +------+      +------+
 *     |      |      |      |
 *     | Grid |----->| Grid |
 *     |      |      |      |
 *     +------+      +------+
 *         | direction connection 
 *         v
 *     +------+
 *     |      |
 *     | Grid |
 *     |      |
 *     +------+
 *
 * 2. Direct connections across columns and rows 
 *               +------+
 *               |      |
 *               |      v 
 *     +------+  |   +------+
 *     |      |  |   |      |
 *     | Grid |  |   | Grid |
 *     |      |  |   |      |
 *     +------+  |   +------+
 *               |
 *     +------+  |   +------+
 *     |      |  |   |      |
 *     | Grid |  |   | Grid |
 *     |      |  |   |      |
 *     +------+  |   +------+
 *        |      |
 *        +------+
 *
 *******************************************************************/
static 
void add_top_module_nets_clb2clb_direct_connections(ModuleManager& module_manager, 
                                                    const ModuleId& top_module, 
                                                    const CircuitLibrary& circuit_lib, 
                                                    const vtr::Point<size_t>& device_size,
                                                    const std::vector<std::vector<t_grid_tile>>& grids,
                                                    const std::vector<std::vector<size_t>>& grid_instance_ids,
                                                    const std::vector<t_clb_to_clb_directs>& clb2clb_directs) {
  /* Scan the grid, visit each grid and apply direct connections */
  for (size_t ix = 0; ix < device_size.x(); ++ix) {
    for (size_t iy = 0; iy < device_size.y(); ++iy) {
      /* Bypass EMPTY_TYPE*/
      if ( (NULL == grids[ix][iy].type)
        || (EMPTY_TYPE == grids[ix][iy].type)) {
        continue;
      }
      /* Bypass any grid with a non-zero offset! They have been visited in the offset=0 case */
      if (0 != grids[ix][iy].offset) {
        continue;
      }
      /* Check each clb2clb directs by comparing the source and destination clb types
       * Direct connections are made only for those matched clbs
       */ 
      for (const t_clb_to_clb_directs& direct : clb2clb_directs) {
        /* Bypass unmatched clb type */
        if (grids[ix][iy].type != direct.from_clb_type) {
          continue;
        }
        
        /* See if the destination CLB is in the bound */
        vtr::Point<size_t> src_clb_coord(ix, iy);
        vtr::Point<size_t> des_clb_coord(ix + direct.x_offset, iy + direct.y_offset);
        if (false == is_grid_coordinate_exist_in_device(device_size, des_clb_coord)) {
          continue;
        }

        /* Check if the destination clb_type matches */
        if (grids[des_clb_coord.x()][des_clb_coord.y()].type == direct.to_clb_type) {
          /* Add a module net for a direct connection with the two grids in top_model */
          add_module_nets_clb2clb_direct_connection(module_manager, top_module, circuit_lib, 
                                                    device_size, grids, grid_instance_ids,
                                                    src_clb_coord, des_clb_coord,  
                                                    direct);
        }
      }
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
                              const std::vector<t_clb_to_clb_directs>& clb2clb_directs,
                              t_sram_orgz_info* cur_sram_orgz_info,
                              const std::string& arch_name,
                              const std::string& verilog_dir,
                              const bool& compact_routing_hierarchy,
                              const bool& use_explicit_mapping) {
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
  /* TODO: Add inter-CLB direct connections */
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
