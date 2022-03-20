/********************************************************************
 * This file include most utilized functions for building connections 
 * inside the module graph for FPGA fabric
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "pb_type_utils.h"
#include "rr_gsb_utils.h"
#include "openfpga_physical_tile_utils.h"
#include "openfpga_device_grid_utils.h"
#include "module_manager_utils.h"

#include "build_routing_module_utils.h"
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
                                              const VprDeviceAnnotation& vpr_device_annotation,
                                              const DeviceGrid& grids,
                                              const vtr::Matrix<size_t>& grid_instance_ids,
                                              const RRGraph& rr_graph,
                                              const DeviceRRGSB& device_rr_gsb,
                                              const RRGSB& rr_gsb, 
                                              const vtr::Matrix<size_t>& sb_instance_ids,
                                              const bool& compact_routing_hierarchy) {

  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist()) {
    return;
  }

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

      t_physical_tile_type_ptr grid_type_descriptor = grids[grid_coordinate.x()][grid_coordinate.y()].type;
      size_t src_grid_pin_width = grid_type_descriptor->pin_width_offset[src_grid_pin_index];
      size_t src_grid_pin_height = grid_type_descriptor->pin_height_offset[src_grid_pin_index];
      BasicPort src_grid_pin_info = vpr_device_annotation.physical_tile_pin_port_info(grid_type_descriptor, src_grid_pin_index);
      VTR_ASSERT(true == src_grid_pin_info.is_valid());
      int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(grid_type_descriptor, src_grid_pin_index);
      VTR_ASSERT(OPEN != subtile_index && subtile_index < grid_type_descriptor->capacity);
      std::string src_grid_port_name = generate_grid_port_name(src_grid_pin_width, src_grid_pin_height, subtile_index,
                                                               rr_graph.node_side(rr_gsb.get_opin_node(side_manager.get_side(), inode)),
                                                               src_grid_pin_info);
      ModulePortId src_grid_port_id = module_manager.find_module_port(src_grid_module, src_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_grid_port_id));
      BasicPort src_grid_port = module_manager.module_port(src_grid_module, src_grid_port_id); 

      /* Collect sink-related information */
      vtr::Point<size_t> sink_sb_port_coord(rr_graph.node_xlow(module_sb.get_opin_node(side_manager.get_side(), inode)),
                                            rr_graph.node_ylow(module_sb.get_opin_node(side_manager.get_side(), inode)));
      std::string sink_sb_port_name = generate_sb_module_grid_port_name(side_manager.get_side(),
                                                                        rr_graph.node_side(module_sb.get_opin_node(side_manager.get_side(), inode)),
                                                                        grids,
                                                                        vpr_device_annotation,
                                                                        rr_graph,
                                                                        module_sb.get_opin_node(side_manager.get_side(), inode)); 
      ModulePortId sink_sb_port_id = module_manager.find_module_port(sink_sb_module, sink_sb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sink_sb_module, sink_sb_port_id));
      BasicPort sink_sb_port =  module_manager.module_port(sink_sb_module, sink_sb_port_id); 

      /* Source and sink port should match in size */
      VTR_ASSERT(src_grid_port.get_width() == sink_sb_port.get_width());
      
      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_grid_port.pins().size(); ++pin_id) {
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module, src_grid_module, src_grid_instance, src_grid_port_id, src_grid_port.pins()[pin_id]);
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
                                                                   const VprDeviceAnnotation& vpr_device_annotation,
                                                                   const DeviceGrid& grids,
                                                                   const vtr::Matrix<size_t>& grid_instance_ids,
                                                                   const RRGraph& rr_graph,
                                                                   const DeviceRRGSB& device_rr_gsb,
                                                                   const RRGSB& rr_gsb, 
                                                                   const vtr::Matrix<size_t>& sb_instance_ids,
                                                                   const bool& compact_routing_hierarchy) {

  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist()) {
    return;
  }

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

      t_physical_tile_type_ptr grid_type_descriptor = grids[grid_coordinate.x()][grid_coordinate.y()].type;
      size_t src_grid_pin_width = grid_type_descriptor->pin_width_offset[src_grid_pin_index];
      size_t src_grid_pin_height = grid_type_descriptor->pin_height_offset[src_grid_pin_index];

      BasicPort src_grid_pin_info = vpr_device_annotation.physical_tile_pin_port_info(grid_type_descriptor, src_grid_pin_index);
      VTR_ASSERT(true == src_grid_pin_info.is_valid());
      int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(grid_type_descriptor, src_grid_pin_index);
      VTR_ASSERT(OPEN != subtile_index && subtile_index < grid_type_descriptor->capacity);

      /* Pins for direct connection are NOT duplicated.
       * Follow the traditional recipe when adding nets!  
       * Xifan: I assume that each direct connection pin must have Fc=0. 
       * For other duplicated pins, we follow the new naming
       */
      std::string src_grid_port_name;
      if (0. == find_physical_tile_pin_Fc(grid_type_descriptor, src_grid_pin_index)) {
        src_grid_port_name = generate_grid_port_name(src_grid_pin_width, src_grid_pin_height, subtile_index,
                                                     rr_graph.node_side(rr_gsb.get_opin_node(side_manager.get_side(), inode)),
                                                     src_grid_pin_info);
      } else {
       src_grid_port_name = generate_grid_duplicated_port_name(src_grid_pin_width, src_grid_pin_height, subtile_index, 
                                                               rr_graph.node_side(rr_gsb.get_opin_node(side_manager.get_side(), inode)),
                                                               src_grid_pin_info, sb_side2postfix_map[side_manager.get_side()]);
      }
      ModulePortId src_grid_port_id = module_manager.find_module_port(src_grid_module, src_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_grid_port_id));
      BasicPort src_grid_port = module_manager.module_port(src_grid_module, src_grid_port_id); 

      /* Collect sink-related information */
      vtr::Point<size_t> sink_sb_port_coord(rr_graph.node_xlow(module_sb.get_opin_node(side_manager.get_side(), inode)),
                                            rr_graph.node_ylow(module_sb.get_opin_node(side_manager.get_side(), inode)));
      std::string sink_sb_port_name = generate_sb_module_grid_port_name(side_manager.get_side(),
                                                                        rr_graph.node_side(module_sb.get_opin_node(side_manager.get_side(), inode)),
                                                                        grids,
                                                                        vpr_device_annotation,
                                                                        rr_graph,
                                                                        module_sb.get_opin_node(side_manager.get_side(), inode)); 
      ModulePortId sink_sb_port_id = module_manager.find_module_port(sink_sb_module, sink_sb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sink_sb_module, sink_sb_port_id));
      BasicPort sink_sb_port =  module_manager.module_port(sink_sb_module, sink_sb_port_id); 

      /* Source and sink port should match in size */
      VTR_ASSERT(src_grid_port.get_width() == sink_sb_port.get_width());
      
      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_grid_port.pins().size(); ++pin_id) {
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module, src_grid_module, src_grid_instance, src_grid_port_id, src_grid_port.pins()[pin_id]);
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
                                              const VprDeviceAnnotation& vpr_device_annotation,
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
                                                                       grids,
                                                                       vpr_device_annotation,
                                                                       rr_graph,
                                                                       module_ipin_node); 
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

      t_physical_tile_type_ptr grid_type_descriptor = grids[grid_coordinate.x()][grid_coordinate.y()].type;
      size_t sink_grid_pin_width = grid_type_descriptor->pin_width_offset[sink_grid_pin_index];
      size_t sink_grid_pin_height = grid_type_descriptor->pin_height_offset[sink_grid_pin_index];
      BasicPort sink_grid_pin_info = vpr_device_annotation.physical_tile_pin_port_info(grid_type_descriptor, sink_grid_pin_index);
      VTR_ASSERT(true == sink_grid_pin_info.is_valid());
      int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(grid_type_descriptor, sink_grid_pin_index);
      VTR_ASSERT(OPEN != subtile_index && subtile_index < grid_type_descriptor->capacity);
      std::string sink_grid_port_name = generate_grid_port_name(sink_grid_pin_width, sink_grid_pin_height, subtile_index,
                                                                rr_graph.node_side(rr_gsb.get_ipin_node(cb_ipin_side, inode)),
                                                                sink_grid_pin_info);
      ModulePortId sink_grid_port_id = module_manager.find_module_port(sink_grid_module, sink_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sink_grid_module, sink_grid_port_id));
      BasicPort sink_grid_port = module_manager.module_port(sink_grid_module, sink_grid_port_id); 

      /* Source and sink port should match in size */
      VTR_ASSERT(src_cb_port.get_width() == sink_grid_port.get_width());
      
      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_cb_port.pins().size(); ++pin_id) {
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module, src_cb_module, src_cb_instance, src_cb_port_id, src_cb_port.pins()[pin_id]);
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
                                                                    side_manager.get_side(), 
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
     
      /* Upper CB port is required if the routing tracks are on the top or right sides of 
       * the switch block, which indicated bottom and left sides of the connection blocks
       */
      bool use_cb_upper_port = (TOP == side_manager.get_side()) || (RIGHT == side_manager.get_side());
      std::string cb_port_name = generate_cb_module_track_port_name(cb_type,
                                                                    cb_port_direction,
                                                                    use_cb_upper_port);
      ModulePortId cb_port_id = module_manager.find_module_port(cb_module_id, cb_port_name); 
      VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module_id, cb_port_id));
      BasicPort cb_port = module_manager.module_port(cb_module_id, cb_port_id);

      /* Configure the net source and sink:
       * If sb port is an output (source), cb port is an input (sink) 
       * If sb port is an input (sink), cb port is an output (source) 
       */
      if (OUT_PORT == module_sb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module, sb_module_id, sb_instance, sb_port_id, itrack / 2);
        module_manager.add_module_net_sink(top_module, net, cb_module_id, cb_instance, cb_port_id, itrack / 2);
      } else {
        VTR_ASSERT(IN_PORT == module_sb.get_chan_node_direction(side_manager.get_side(), itrack));
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module, cb_module_id, cb_instance, cb_port_id, itrack / 2);
        module_manager.add_module_net_sink(top_module, net, sb_module_id, sb_instance, sb_port_id, itrack / 2);
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
                                                const VprDeviceAnnotation& vpr_device_annotation,
                                                const DeviceGrid& grids,
                                                const vtr::Matrix<size_t>& grid_instance_ids,
                                                const RRGraph& rr_graph,
                                                const DeviceRRGSB& device_rr_gsb,
                                                const vtr::Matrix<size_t>& sb_instance_ids,
                                                const std::map<t_rr_type, vtr::Matrix<size_t>>& cb_instance_ids,
                                                const bool& compact_routing_hierarchy,
                                                const bool& duplicate_grid_pin) {

  vtr::ScopedStartFinishTimer timer("Add module nets between grids and GSBs");

  vtr::Point<size_t> gsb_range = device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < gsb_range.x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.y(); ++iy) {
      vtr::Point<size_t> gsb_coordinate(ix, iy);
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);

      /* Connect the grid pins of the GSB to adjacent grids */
      if (false == duplicate_grid_pin) {
        add_top_module_nets_connect_grids_and_sb(module_manager, top_module, 
                                                 vpr_device_annotation,
                                                 grids, grid_instance_ids,
                                                 rr_graph, device_rr_gsb, rr_gsb, sb_instance_ids, 
                                                 compact_routing_hierarchy);
      } else {
        VTR_ASSERT_SAFE(true == duplicate_grid_pin);
        add_top_module_nets_connect_grids_and_sb_with_duplicated_pins(module_manager, top_module, 
                                                                      vpr_device_annotation,
                                                                      grids, grid_instance_ids,
                                                                      rr_graph, device_rr_gsb, rr_gsb, sb_instance_ids, 
                                                                      compact_routing_hierarchy);
      }

      add_top_module_nets_connect_grids_and_cb(module_manager, top_module, 
                                               vpr_device_annotation,
                                               grids, grid_instance_ids,
                                               rr_graph, device_rr_gsb, rr_gsb, CHANX, cb_instance_ids.at(CHANX),
                                               compact_routing_hierarchy);

      add_top_module_nets_connect_grids_and_cb(module_manager, top_module, 
                                               vpr_device_annotation,
                                               grids, grid_instance_ids,
                                               rr_graph, device_rr_gsb, rr_gsb, CHANY, cb_instance_ids.at(CHANY),
                                               compact_routing_hierarchy);

      add_top_module_nets_connect_sb_and_cb(module_manager, top_module, 
                                            rr_graph, device_rr_gsb, rr_gsb, sb_instance_ids, cb_instance_ids,
                                            compact_routing_hierarchy);

    }
  }
}

/********************************************************************
 * Add global port connection for a given port of a physical tile
 * that are defined as global in tile annotation 
 *******************************************************************/
static 
int build_top_module_global_net_for_given_grid_module(ModuleManager& module_manager,
                                                      const ModuleId& top_module,
                                                      const ModulePortId& top_module_port,
                                                      const TileAnnotation& tile_annotation,
                                                      const TileGlobalPortId& tile_global_port,
                                                      const BasicPort& tile_port_to_connect,
                                                      const VprDeviceAnnotation& vpr_device_annotation,
                                                      const DeviceGrid& grids,
                                                      const vtr::Point<size_t>& grid_coordinate,
                                                      const e_side& border_side,
                                                      const vtr::Matrix<size_t>& grid_instance_ids) {

  t_physical_tile_type_ptr physical_tile = grids[grid_coordinate.x()][grid_coordinate.y()].type;

  /* Find the port of the grid module according to the tile annotation */
  int grid_pin_start_index = physical_tile->num_pins;
  t_physical_tile_port physical_tile_port;
  physical_tile_port.num_pins = 0;
  for (const t_physical_tile_port& tile_port : physical_tile->ports) { 
    if (std::string(tile_port.name) == tile_port_to_connect.get_name()) {
      BasicPort ref_tile_port(tile_port.name, tile_port.num_pins);
      /* Port size must match!!! */
      if (false == ref_tile_port.contained(tile_port_to_connect)) {
        VTR_LOG_ERROR("Tile annotation '%s' port '%s[%lu:%lu]' is out of the range of physical tile port '%s[%lu:%lu]'!",
                      tile_annotation.global_port_name(tile_global_port).c_str(),
                      tile_port_to_connect.get_name().c_str(),
                      tile_port_to_connect.get_lsb(),
                      tile_port_to_connect.get_msb(),
                      ref_tile_port.get_name().c_str(),
                      ref_tile_port.get_lsb(),
                      ref_tile_port.get_msb());
        return CMD_EXEC_FATAL_ERROR;
      }
      grid_pin_start_index = tile_port.absolute_first_pin_index;
      physical_tile_port = tile_port;
      break;
    }
  }
  /* Ensure the pin index is valid */
  VTR_ASSERT(grid_pin_start_index < physical_tile->num_pins);

  /* Find the module name for this type of grid */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(physical_tile->name), is_io_type(physical_tile), border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
  size_t grid_instance = grid_instance_ids[grid_coordinate.x()][grid_coordinate.y()];

  VTR_ASSERT(1 == physical_tile->equivalent_sites.size());

  /* Ensure port width is in range */
  BasicPort src_port = module_manager.module_port(top_module, top_module_port);
  VTR_ASSERT(src_port.get_width() == tile_port_to_connect.get_width());

  /* Create a pin id mapping between the source port (top module) and the sink port (grid module) */
  std::map<size_t, size_t> sink2src_pin_map;
  for (size_t ipin = 0; ipin < tile_port_to_connect.get_width(); ++ipin) {
    size_t sink_pin = tile_port_to_connect.pins()[ipin];
    size_t src_pin = src_port.pins()[ipin];
    sink2src_pin_map[sink_pin] = src_pin;
  }

  /* A tile may consist of multiple subtile, connect to all the pins from sub tiles */
  for (int iz = 0; iz < physical_tile->capacity; ++iz) {
    for (size_t pin_id = tile_port_to_connect.get_lsb(); pin_id < tile_port_to_connect.get_msb() + 1; ++pin_id) {
      /* TODO: This should be replaced by using a pin mapping data structure from physical tile! */
      int grid_pin_index = grid_pin_start_index + iz * physical_tile->equivalent_sites[0]->pb_type->num_pins + pin_id;
      /* Find the module pin */
      size_t grid_pin_width = physical_tile->pin_width_offset[grid_pin_index];
      size_t grid_pin_height = physical_tile->pin_height_offset[grid_pin_index];
      std::vector<e_side> pin_sides = find_physical_tile_pin_side(physical_tile, grid_pin_index, border_side);

      BasicPort grid_pin_info = vpr_device_annotation.physical_tile_pin_port_info(physical_tile, grid_pin_index);
      VTR_ASSERT(true == grid_pin_info.is_valid());
      int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(physical_tile, grid_pin_index);
      VTR_ASSERT(OPEN != subtile_index && subtile_index < physical_tile->capacity);

      /* Build nets */
      for (const e_side& pin_side : pin_sides) {
        std::string grid_port_name = generate_grid_port_name(grid_pin_width, grid_pin_height, subtile_index,
                                                             pin_side,
                                                             grid_pin_info);
        ModulePortId grid_port_id = module_manager.find_module_port(grid_module, grid_port_name);
        VTR_ASSERT(true == module_manager.valid_module_port_id(grid_module, grid_port_id));

        VTR_ASSERT(1 == module_manager.module_port(grid_module, grid_port_id).get_width());

        ModuleNetId net = create_module_source_pin_net(module_manager, top_module, 
                                                       top_module, 0, 
                                                       top_module_port, src_port.pins()[sink2src_pin_map[pin_id]]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);
        
        /* Configure the net sink */
        BasicPort sink_port = module_manager.module_port(grid_module, grid_port_id);
        module_manager.add_module_net_sink(top_module, net, grid_module, grid_instance, grid_port_id, sink_port.pins()[0]);
      }
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Add global ports from grid ports that are defined as global in tile annotation 
 *******************************************************************/
int add_top_module_global_ports_from_grid_modules(ModuleManager& module_manager,
                                                  const ModuleId& top_module,
                                                  const TileAnnotation& tile_annotation,
                                                  const VprDeviceAnnotation& vpr_device_annotation,
                                                  const DeviceGrid& grids,
                                                  const vtr::Matrix<size_t>& grid_instance_ids) {
  int status = CMD_EXEC_SUCCESS;

  /* Add the global ports which are NOT yet added to the top-level module 
   * (in different names than the global ports defined in circuit library
   */
  std::vector<BasicPort> global_ports_to_add;
  for (const TileGlobalPortId& tile_global_port : tile_annotation.global_ports()) {
    ModulePortId module_port = module_manager.find_module_port(top_module, tile_annotation.global_port_name(tile_global_port));
    /* The global port size is derived from the maximum port size among all the tile port defintion */
    if (ModulePortId::INVALID() == module_port) {
      BasicPort global_port_to_add; 
      global_port_to_add.set_name(tile_annotation.global_port_name(tile_global_port));
      size_t max_port_size = 0;
      for (const BasicPort& tile_port : tile_annotation.global_port_tile_ports(tile_global_port)) {
        max_port_size = std::max(tile_port.get_width(), max_port_size);
      }
      global_port_to_add.set_width(max_port_size);
      global_ports_to_add.push_back(global_port_to_add);
    }
  }

  for (const BasicPort& global_port_to_add : global_ports_to_add) {
    module_manager.add_port(top_module, global_port_to_add, ModuleManager::MODULE_GLOBAL_PORT);
  }

  /* Add module nets */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates = generate_perimeter_grid_coordinates( grids);

  for (const TileGlobalPortId& tile_global_port : tile_annotation.global_ports()) {
    /* Must found one valid port! */
    ModulePortId top_module_port = module_manager.find_module_port(top_module, tile_annotation.global_port_name(tile_global_port));
    VTR_ASSERT(ModulePortId::INVALID() != top_module_port);

    for (size_t tile_info_id = 0; tile_info_id < tile_annotation.global_port_tile_names(tile_global_port).size(); ++tile_info_id) {
      std::string tile_name = tile_annotation.global_port_tile_names(tile_global_port)[tile_info_id];
      BasicPort tile_port = tile_annotation.global_port_tile_ports(tile_global_port)[tile_info_id];
      /* Find the coordinates for the wanted tiles */
      vtr::Point<size_t> start_coord(1, 1);
      vtr::Point<size_t> end_coord(grids.width() - 1, grids.height() - 1);
      vtr::Point<size_t> range = tile_annotation.global_port_tile_coordinates(tile_global_port)[tile_info_id];
      bool out_of_range = false;

      /* -1 means all the x should be considered */
      if (size_t(-1) != range.x()) {
        if ((range.x() < start_coord.x()) || (range.x() > end_coord.x())) {
          out_of_range = true;
        } else {
          /* Set the range */
          start_coord.set_x(range.x());
          end_coord.set_x(range.x());
        }
      }

      /* -1 means all the y should be considered */
      if (size_t(-1) != range.y()) {
        if ((range.y() < start_coord.y()) || (range.y() > end_coord.y())) {
          out_of_range = true;
        } else {
          /* Set the range */
          start_coord.set_y(range.y());
          end_coord.set_y(range.y());
        }
      }

      /* Error out immediately if the coordinate is not valid! */
      if (true == out_of_range) {
        VTR_LOG_ERROR("Coordinate (%lu, %lu) in tile annotation for tile '%s' is out of range (%lu:%lu, %lu:%lu)!",
                      range.x(), range.y(), tile_name.c_str(),
                      start_coord.x(), end_coord.x(), start_coord.y(), end_coord.y());
        return CMD_EXEC_FATAL_ERROR;
      }

      /* Spot the port from child modules from core grids */
      for (size_t ix = start_coord.x(); ix < end_coord.x(); ++ix) {
        for (size_t iy = start_coord.y(); iy < end_coord.y(); ++iy) {
          /* Bypass EMPTY tiles */
          if (true == is_empty_type(grids[ix][iy].type)) {
            continue;
          }
          /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
          if ( (0 < grids[ix][iy].width_offset)
            || (0 < grids[ix][iy].height_offset)) {
            continue;
          }

          /* Bypass the tiles whose names do not match */
          if (std::string(grids[ix][iy].type->name) != tile_name) {
            continue;
          }

          /* Create nets and finish connection build-up */
          status = build_top_module_global_net_for_given_grid_module(module_manager,
                                                                     top_module,
                                                                     top_module_port,
                                                                     tile_annotation,
                                                                     tile_global_port,
                                                                     tile_port,
                                                                     vpr_device_annotation,
                                                                     grids,
                                                                     vtr::Point<size_t>(ix, iy),
                                                                     NUM_SIDES,
                                                                     grid_instance_ids);
          if (CMD_EXEC_FATAL_ERROR == status) {
            return status;
          }
        }
      }

      /* Walk through all the grids on the perimeter, which are I/O grids */
      for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
        for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
          /* Bypass EMPTY grid */
          if (true == is_empty_type(grids[io_coordinate.x()][io_coordinate.y()].type)) {
            continue;
          } 
  
          /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
          if ( (0 < grids[io_coordinate.x()][io_coordinate.y()].width_offset)
            || (0 < grids[io_coordinate.x()][io_coordinate.y()].height_offset)) {
            continue;
          }

          /* Bypass the tiles whose names do not match */
          if (std::string(grids[io_coordinate.x()][io_coordinate.y()].type->name) != tile_name) {
            continue;
          }

          /* Check if the coordinate satisfy the tile coordinate defintion
           * - Bypass if the x is a specific number (!= -1), and io_coordinate is different 
           * - Bypass if the y is a specific number (!= -1), and io_coordinate is different 
           */
          if ((size_t(-1) != range.x()) && (range.x() != io_coordinate.x())) {
            continue;
          }
          if ((size_t(-1) != range.y()) && (range.y() != io_coordinate.y())) {
            continue;
          }

          /* Create nets and finish connection build-up */
          status = build_top_module_global_net_for_given_grid_module(module_manager,
                                                                     top_module,
                                                                     top_module_port,
                                                                     tile_annotation,
                                                                     tile_global_port,
                                                                     tile_port,
                                                                     vpr_device_annotation,
                                                                     grids,
                                                                     io_coordinate,
                                                                     io_side,
                                                                     grid_instance_ids);
          if (CMD_EXEC_FATAL_ERROR == status) {
            return status;
          }
        }
      }
    }
  }

  return status;
}

} /* end namespace openfpga */
