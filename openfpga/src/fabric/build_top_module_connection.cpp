/********************************************************************
 * This file include most utilized functions for building connections 
 * inside the module graph for FPGA fabric
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "pb_type_utils.h"
#include "rr_gsb_utils.h"
#include "openfpga_physical_tile_utils.h"

#include "build_top_module_utils.h"
#include "build_top_module_connection.h"

/* begin namespace openfpga */
namespace openfpga {

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
 *    |            |----+      +----|            |
 *    +------------+    |      |    +------------+
 *           |          v      v         |
 *           |       +------------+      |
 *           +------>|            |<-----+
 *                   |   Switch   |
 *                   |   Block    |
 *           +------>|   [x][y]   |<-----+
 *           |       +------------+      |
 *           |          ^     ^          |
 *           |          |     |          |
 *    +------------+    |     |     +------------+
 *    |            |----+     +-----|            |
 *    |    Grid    |                |    Grid    |
 *    |   [x][y]   |                |  [x+1][y]  |
 *    |            |                |            |
 *    +------------+                +------------+

 *
 *******************************************************************/
static 
void add_top_module_nets_connect_grids_and_sb(ModuleManager& module_manager, 
                                              const ModuleId& top_module, 
                                              const DeviceGrid& grids,
                                              const vtr::Matrix<size_t>& grid_instance_ids,
                                              const RRGraph& rr_graph,
                                              const DeviceRRGSB& device_rr_gsb,
                                              const RRGSB& rr_gsb, 
                                              const vtr::Matrix<size_t>& sb_instance_ids,
                                              const bool& compact_routing_hierarchy) {

  /* We could have two different coordinators, one is the instance, the other is the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x()); 
    module_gsb_coordinate.set_y(unique_mirror.get_y()); 
  } 

  /* This is the source cb that is added to the top module */
  const RRGSB& module_sb = device_rr_gsb.get_gsb(module_gsb_coordinate);
  vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(), module_sb.get_sb_y());

  /* Collect sink-related information */
  std::string sink_sb_module_name = generate_switch_block_module_name(module_sb_coordinate);
  ModuleId sink_sb_module = module_manager.find_module(sink_sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sink_sb_module));
  size_t sink_sb_instance = sb_instance_ids[instance_sb_coordinate.x()][instance_sb_coordinate.y()];

  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < module_sb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    for (size_t inode = 0; inode < module_sb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      /* Collect source-related information */
      /* Generate the grid module name by considering if it locates on the border */
      vtr::Point<size_t> grid_coordinate(rr_graph.node_xlow(rr_gsb.get_opin_node(side_manager.get_side(), inode)), 
                                         rr_graph.node_ylow(rr_gsb.get_opin_node(side_manager.get_side(), inode)));
      std::string src_grid_module_name = generate_grid_block_module_name_in_top_module(std::string(GRID_MODULE_NAME_PREFIX), grids, grid_coordinate); 
      ModuleId src_grid_module = module_manager.find_module(src_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(src_grid_module));
      size_t src_grid_instance = grid_instance_ids[grid_coordinate.x()][grid_coordinate.y()];
      size_t src_grid_pin_index = rr_graph.node_pin_num(rr_gsb.get_opin_node(side_manager.get_side(), inode));
      size_t src_grid_pin_width = grids[grid_coordinate.x()][grid_coordinate.y()].type->pin_width_offset[src_grid_pin_index];
      size_t src_grid_pin_height = grids[grid_coordinate.x()][grid_coordinate.y()].type->pin_height_offset[src_grid_pin_index];
      std::string src_grid_port_name = generate_grid_port_name(grid_coordinate, src_grid_pin_width, src_grid_pin_height,
                                                               rr_graph.node_side(rr_gsb.get_opin_node(side_manager.get_side(), inode)),
                                                               src_grid_pin_index, false);
      ModulePortId src_grid_port_id = module_manager.find_module_port(src_grid_module, src_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_grid_port_id));
      BasicPort src_grid_port = module_manager.module_port(src_grid_module, src_grid_port_id); 

      /* Collect sink-related information */
      vtr::Point<size_t> sink_sb_port_coord(rr_graph.node_xlow(module_sb.get_opin_node(side_manager.get_side(), inode)),
                                            rr_graph.node_ylow(module_sb.get_opin_node(side_manager.get_side(), inode)));
      std::string sink_sb_port_name = generate_sb_module_grid_port_name(side_manager.get_side(),
                                                                        rr_graph.node_side(module_sb.get_opin_node(side_manager.get_side(), inode)),
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
 * Add module nets to connect a GSB to adjacent grid ports/pins 
 * as well as connection blocks
 * This function will create nets for the following types of connections
 * between grid output pins of Switch block and adjacent grids
 * In this case, the net source is the grid pin, while the net sink
 * is the switch block pin
 *
 * In particular, this function considers the duplicated output pins of grids
 * when creating the connecting nets.
 * The follow figure shows the different pin postfix to be considered when
 * connecting the grid pins to SB inputs
 *    
 *    +------------+                +------------+
 *    |            |                |            |
 *    |    Grid    |                |    Grid    |
 *    |  [x][y+1]  |lower      lower| [x+1][y+1] |
 *    |            |----+      +----|            |
 *    +------------+    |      |    +------------+
 *           |lower     v      v         |upper
 *           |       +------------+      |
 *           +------>|            |<-----+
 *                   |   Switch   |
 *                   |   Block    |
 *           +------>|   [x][y]   |<-----+
 *           |       +------------+      |
 *           |          ^     ^          |
 *           |lower     |     |          |upper
 *    +------------+    |     |     +------------+
 *    |            |----+     +-----|            |
 *    |    Grid    |upper     upper |    Grid    |
 *    |   [x][y]   |                |  [x+1][y]  |
 *    |            |                |            |
 *    +------------+                +------------+
 *
 *******************************************************************/
static 
void add_top_module_nets_connect_grids_and_sb_with_duplicated_pins(ModuleManager& module_manager, 
                                                                   const ModuleId& top_module, 
                                                                   const DeviceGrid& grids,
                                                                   const vtr::Matrix<size_t>& grid_instance_ids,
                                                                   const RRGraph& rr_graph,
                                                                   const DeviceRRGSB& device_rr_gsb,
                                                                   const RRGSB& rr_gsb, 
                                                                   const vtr::Matrix<size_t>& sb_instance_ids,
                                                                   const bool& compact_routing_hierarchy) {

  /* We could have two different coordinators, one is the instance, the other is the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x()); 
    module_gsb_coordinate.set_y(unique_mirror.get_y()); 
  } 

  /* This is the source cb that is added to the top module */
  const RRGSB& module_sb = device_rr_gsb.get_gsb(module_gsb_coordinate);
  vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(), module_sb.get_sb_y());

  /* Collect sink-related information */
  std::string sink_sb_module_name = generate_switch_block_module_name(module_sb_coordinate);
  ModuleId sink_sb_module = module_manager.find_module(sink_sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sink_sb_module));
  size_t sink_sb_instance = sb_instance_ids[instance_sb_coordinate.x()][instance_sb_coordinate.y()];

  /* Create a truth table for the postfix to be used regarding to the different side of switch blocks */
  std::map<e_side, bool> sb_side2postfix_map;
  /* Boolean variable "true" indicates the upper postfix in naming functions 
   * Boolean variable "false" indicates the lower postfix in naming functions 
   */
  sb_side2postfix_map[TOP] = false;
  sb_side2postfix_map[RIGHT] = true;
  sb_side2postfix_map[BOTTOM] = true;
  sb_side2postfix_map[LEFT] = false;

  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < module_sb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    for (size_t inode = 0; inode < module_sb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      /* Collect source-related information */
      /* Generate the grid module name by considering if it locates on the border */
      vtr::Point<size_t> grid_coordinate(rr_graph.node_xlow(rr_gsb.get_opin_node(side_manager.get_side(), inode)),
                                         rr_graph.node_ylow(rr_gsb.get_opin_node(side_manager.get_side(), inode)));
      std::string src_grid_module_name = generate_grid_block_module_name_in_top_module(std::string(GRID_MODULE_NAME_PREFIX), grids, grid_coordinate); 
      ModuleId src_grid_module = module_manager.find_module(src_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(src_grid_module));
      size_t src_grid_instance = grid_instance_ids[grid_coordinate.x()][grid_coordinate.y()];
      size_t src_grid_pin_index = rr_graph.node_pin_num(rr_gsb.get_opin_node(side_manager.get_side(), inode));
      size_t src_grid_pin_width = grids[grid_coordinate.x()][grid_coordinate.y()].type->pin_width_offset[src_grid_pin_index];
      size_t src_grid_pin_height = grids[grid_coordinate.x()][grid_coordinate.y()].type->pin_height_offset[src_grid_pin_index];

      /* Pins for direct connection are NOT duplicated.
       * Follow the traditional recipe when adding nets!  
       * Xifan: I assume that each direct connection pin must have Fc=0. 
       * For other duplicated pins, we follow the new naming
       */
      std::string src_grid_port_name;
      if (0. == find_physical_tile_pin_Fc(grids[grid_coordinate.x()][grid_coordinate.y()].type, src_grid_pin_index)) {
        src_grid_port_name = generate_grid_port_name(grid_coordinate, src_grid_pin_width, src_grid_pin_height, 
                                                     rr_graph.node_side(rr_gsb.get_opin_node(side_manager.get_side(), inode)),
                                                     src_grid_pin_index, false);
      } else {
       src_grid_port_name = generate_grid_duplicated_port_name(src_grid_pin_width, src_grid_pin_height, 
                                                               rr_graph.node_side(rr_gsb.get_opin_node(side_manager.get_side(), inode)),
                                                               src_grid_pin_index, sb_side2postfix_map[side_manager.get_side()]);
      }
      ModulePortId src_grid_port_id = module_manager.find_module_port(src_grid_module, src_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_grid_port_id));
      BasicPort src_grid_port = module_manager.module_port(src_grid_module, src_grid_port_id); 

      /* Collect sink-related information */
      vtr::Point<size_t> sink_sb_port_coord(rr_graph.node_xlow(module_sb.get_opin_node(side_manager.get_side(), inode)),
                                            rr_graph.node_ylow(module_sb.get_opin_node(side_manager.get_side(), inode)));
      std::string sink_sb_port_name = generate_sb_module_grid_port_name(side_manager.get_side(),
                                                                        rr_graph.node_side(module_sb.get_opin_node(side_manager.get_side(), inode)),
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
                                              const DeviceGrid& grids,
                                              const vtr::Matrix<size_t>& grid_instance_ids,
                                              const RRGraph& rr_graph,
                                              const DeviceRRGSB& device_rr_gsb,
                                              const RRGSB& rr_gsb, 
                                              const t_rr_type& cb_type,
                                              const vtr::Matrix<size_t>& cb_instance_ids,
                                              const bool& compact_routing_hierarchy) {
  /* We could have two different coordinators, one is the instance, the other is the module */
  vtr::Point<size_t> instance_cb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  vtr::Point<size_t> module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* Skip those Connection blocks that do not exist */
  if (false == rr_gsb.is_cb_exist(cb_type)) {
    return;
  }

  /* Skip if the cb does not contain any configuration bits! */
  if (true == connection_block_contain_only_routing_tracks(rr_gsb, cb_type)) {
    return;
  }

  /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(cb_type, gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x()); 
    module_gsb_coordinate.set_y(unique_mirror.get_y()); 
  } 

  /* This is the source cb that is added to the top module */
  const RRGSB& module_cb = device_rr_gsb.get_gsb(module_gsb_coordinate);
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
      RRNodeId module_ipin_node = module_cb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> cb_src_port_coord(rr_graph.node_xlow(module_ipin_node),
                                           rr_graph.node_ylow(module_ipin_node));
      std::string src_cb_port_name = generate_cb_module_grid_port_name(cb_ipin_side,
                                                                       rr_graph.node_pin_num(module_ipin_node)); 
      ModulePortId src_cb_port_id = module_manager.find_module_port(src_cb_module, src_cb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_cb_module, src_cb_port_id));
      BasicPort src_cb_port = module_manager.module_port(src_cb_module, src_cb_port_id); 

      /* Collect sink-related information */
      /* Note that we use the instance cb pin here!!!
       * because it has the correct coordinator for the grid!!! 
       */
      RRNodeId instance_ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> grid_coordinate(rr_graph.node_xlow(instance_ipin_node), 
                                         rr_graph.node_ylow(instance_ipin_node));
      std::string sink_grid_module_name = generate_grid_block_module_name_in_top_module(std::string(GRID_MODULE_NAME_PREFIX), grids, grid_coordinate); 
      ModuleId sink_grid_module = module_manager.find_module(sink_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sink_grid_module));
      size_t sink_grid_instance = grid_instance_ids[grid_coordinate.x()][grid_coordinate.y()];
      size_t sink_grid_pin_index = rr_graph.node_pin_num(instance_ipin_node);
      size_t sink_grid_pin_width = grids[grid_coordinate.x()][grid_coordinate.y()].type->pin_width_offset[sink_grid_pin_index];
      size_t sink_grid_pin_height = grids[grid_coordinate.x()][grid_coordinate.y()].type->pin_height_offset[sink_grid_pin_index];
      std::string sink_grid_port_name = generate_grid_port_name(grid_coordinate, sink_grid_pin_width, sink_grid_pin_height,
                                                                rr_graph.node_side(rr_gsb.get_ipin_node(cb_ipin_side, inode)),
                                                                sink_grid_pin_index, false);
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
                                           const RRGraph& rr_graph,
                                           const DeviceRRGSB& device_rr_gsb,
                                           const RRGSB& rr_gsb, 
                                           const vtr::Matrix<size_t>& sb_instance_ids,
                                           const std::map<t_rr_type, vtr::Matrix<size_t>>& cb_instance_ids,
                                           const bool& compact_routing_hierarchy) {
  /* We could have two different coordinators, one is the instance, the other is the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_sb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist()) {
    return;
  }

  /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(gsb_coord);
    module_gsb_sb_coordinate.set_x(unique_mirror.get_x()); 
    module_gsb_sb_coordinate.set_y(unique_mirror.get_y()); 
  } 

  /* This is the source cb that is added to the top module */
  const RRGSB& module_sb = device_rr_gsb.get_gsb(module_gsb_sb_coordinate);
  vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(), module_sb.get_sb_y());
  std::string sb_module_name = generate_switch_block_module_name(module_sb_coordinate);
  ModuleId sb_module_id = module_manager.find_module(sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module_id));
  size_t sb_instance = sb_instance_ids[instance_sb_coordinate.x()][instance_sb_coordinate.y()];

  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < module_sb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    /* Iterate over the routing tracks on this side */
    /* Early skip: if there is no routing tracks at this side */
    if (0 == module_sb.get_chan_width(side_manager.get_side())) {
      continue;
    }
    /* Find the Connection Block module */
    /* We find the original connection block and then spot its unique mirror! 
     * Do NOT use module_sb here!!!
     */
    t_rr_type cb_type = find_top_module_cb_type_by_sb_side(side_manager.get_side());
    vtr::Point<size_t> instance_gsb_cb_coordinate = find_top_module_gsb_coordinate_by_sb_side(rr_gsb, side_manager.get_side());
    vtr::Point<size_t> module_gsb_cb_coordinate = find_top_module_gsb_coordinate_by_sb_side(rr_gsb, side_manager.get_side());

    /* Skip those Connection blocks that do not exist:
     * 1. The CB does not exist in the device level! We should skip! 
     * 2. The CB does exist but we need to make sure if the GSB includes such CBs 
     *    For TOP and LEFT side, check the existence using RRGSB method is_cb_exist()
     *    FOr RIGHT and BOTTOM side, find the adjacent RRGSB and then use is_cb_exist()
     */
    if ( TOP == side_manager.get_side() || LEFT == side_manager.get_side() ) {
      if ( false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }
    }

    if ( RIGHT == side_manager.get_side() || BOTTOM == side_manager.get_side() ) {
      const RRGSB& adjacent_gsb = device_rr_gsb.get_gsb(module_gsb_cb_coordinate);
      if ( false == adjacent_gsb.is_cb_exist(cb_type)) {
        continue;
      }
    }

    /* If we use compact routing hierarchy, we should find the unique module of CB, which is added to the top module */
    if (true == compact_routing_hierarchy) {
      const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(cb_type, module_gsb_cb_coordinate);
      module_gsb_cb_coordinate.set_x(unique_mirror.get_x()); 
      module_gsb_cb_coordinate.set_y(unique_mirror.get_y()); 
    } 
  
    const RRGSB& module_cb = device_rr_gsb.get_gsb(module_gsb_cb_coordinate);
    vtr::Point<size_t> module_cb_coordinate(module_cb.get_cb_x(cb_type), module_cb.get_cb_y(cb_type));
    std::string cb_module_name = generate_connection_block_module_name(cb_type, module_cb_coordinate);
    ModuleId cb_module_id = module_manager.find_module(cb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_module_id));
    const RRGSB& instance_cb = device_rr_gsb.get_gsb(instance_gsb_cb_coordinate);
    vtr::Point<size_t> instance_cb_coordinate(instance_cb.get_cb_x(cb_type), instance_cb.get_cb_y(cb_type));
    size_t cb_instance = cb_instance_ids.at(cb_type)[instance_cb_coordinate.x()][instance_cb_coordinate.y()];
 
    for (size_t itrack = 0; itrack < module_sb.get_chan_width(side_manager.get_side()); ++itrack) {
      std::string sb_port_name = generate_sb_module_track_port_name(rr_graph.node_type(module_sb.get_chan_node(side_manager.get_side(), itrack)),
                                                                    side_manager.get_side(), itrack,  
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
      std::string cb_port_name = generate_cb_module_track_port_name(cb_type,
                                                                    itrack,  
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
void add_top_module_nets_connect_grids_and_gsbs(ModuleManager& module_manager, 
                                                const ModuleId& top_module, 
                                                const DeviceGrid& grids,
                                                const vtr::Matrix<size_t>& grid_instance_ids,
                                                const RRGraph& rr_graph,
                                                const DeviceRRGSB& device_rr_gsb,
                                                const vtr::Matrix<size_t>& sb_instance_ids,
                                                const std::map<t_rr_type, vtr::Matrix<size_t>>& cb_instance_ids,
                                                const bool& compact_routing_hierarchy,
                                                const bool& duplicate_grid_pin) {

  vtr::Point<size_t> gsb_range = device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < gsb_range.x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.y(); ++iy) {
      vtr::Point<size_t> gsb_coordinate(ix, iy);
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      /* Connect the grid pins of the GSB to adjacent grids */
      if (false == duplicate_grid_pin) {
        add_top_module_nets_connect_grids_and_sb(module_manager, top_module, 
                                                 grids, grid_instance_ids,
                                                 rr_graph, device_rr_gsb, rr_gsb, sb_instance_ids, 
                                                 compact_routing_hierarchy);
      } else {
        VTR_ASSERT_SAFE(true == duplicate_grid_pin);
        add_top_module_nets_connect_grids_and_sb_with_duplicated_pins(module_manager, top_module, 
                                                                      grids, grid_instance_ids,
                                                                      rr_graph, device_rr_gsb, rr_gsb, sb_instance_ids, 
                                                                      compact_routing_hierarchy);
      }

      add_top_module_nets_connect_grids_and_cb(module_manager, top_module, 
                                               grids, grid_instance_ids,
                                               rr_graph, device_rr_gsb, rr_gsb, CHANX, cb_instance_ids.at(CHANX),
                                               compact_routing_hierarchy);

      add_top_module_nets_connect_grids_and_cb(module_manager, top_module, 
                                               grids, grid_instance_ids,
                                               rr_graph, device_rr_gsb, rr_gsb, CHANY, cb_instance_ids.at(CHANY),
                                               compact_routing_hierarchy);

      add_top_module_nets_connect_sb_and_cb(module_manager, top_module, 
                                            rr_graph, device_rr_gsb, rr_gsb, sb_instance_ids, cb_instance_ids,
                                            compact_routing_hierarchy);

    }
  }
}

} /* end namespace openfpga */
