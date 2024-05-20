/********************************************************************
 * This file includes functions that are used to build the location
 * map information for the top-level module of the FPGA fabric
 * It helps OpenFPGA to link the I/O port index in top-level module
 * to the VPR I/O mapping results
 *******************************************************************/
#include "build_tile_modules.h"

#include <algorithm>
#include <map>

#include "build_grid_module_utils.h"
#include "build_routing_module_utils.h"
#include "build_top_module_utils.h"
#include "command_exit_codes.h"
#include "module_manager_utils.h"
#include "openfpga_device_grid_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "openfpga_rr_graph_utils.h"
#include "openfpga_side_manager.h"
#include "rr_gsb_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

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
static int build_tile_module_port_and_nets_between_sb_and_pb(
  ModuleManager& module_manager, const ModuleId& tile_module,
  const DeviceGrid& grids, const size_t& layer,
  const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id, const std::vector<size_t>& pb_instances,
  const std::vector<size_t>& sb_instances, const size_t& isb,
  const bool& compact_routing_hierarchy, const bool& name_module_using_index,
  const bool& frame_view, const bool& verbose) {
  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist(rr_graph)) {
    return CMD_EXEC_SUCCESS;
  }

  size_t sb_instance = sb_instances[isb];

  /* We could have two different coordinators, one is the instance, the other is
   * the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(),
                                            rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* If we use compact routing hierarchy, we should find the unique module of
   * CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x());
    module_gsb_coordinate.set_y(unique_mirror.get_y());
  }

  /* This is the source cb that is added to the top module */
  const RRGSB& module_sb = device_rr_gsb.get_gsb(module_gsb_coordinate);
  vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(),
                                          module_sb.get_sb_y());

  /* Collect sink-related information */
  std::string sink_sb_module_name =
    generate_switch_block_module_name(module_sb_coordinate);
  ModuleId sink_sb_module = module_manager.find_module(sink_sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sink_sb_module));
  size_t sink_sb_instance = sb_instance;

  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < module_sb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    for (size_t inode = 0;
         inode < module_sb.get_num_opin_nodes(side_manager.get_side());
         ++inode) {
      /* Collect source-related information */
      /* Generate the grid module name by considering if it locates on the
       * border */
      vtr::Point<size_t> grid_coordinate(
        rr_graph.node_xlow(
          rr_gsb.get_opin_node(side_manager.get_side(), inode)),
        rr_graph.node_ylow(
          rr_gsb.get_opin_node(side_manager.get_side(), inode)));
      std::string src_grid_module_name =
        generate_grid_block_module_name_in_top_module(
          std::string(GRID_MODULE_NAME_PREFIX), grids, grid_coordinate);
      ModuleId src_grid_module =
        module_manager.find_module(src_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(src_grid_module));
      size_t src_grid_pin_index = rr_graph.node_pin_num(
        rr_gsb.get_opin_node(side_manager.get_side(), inode));

      t_physical_tile_type_ptr grid_type_descriptor = grids.get_physical_type(
        t_physical_tile_loc(grid_coordinate.x(), grid_coordinate.y(), layer));
      size_t src_grid_pin_width =
        grid_type_descriptor->pin_width_offset[src_grid_pin_index];
      size_t src_grid_pin_height =
        grid_type_descriptor->pin_height_offset[src_grid_pin_index];
      BasicPort src_grid_pin_info =
        vpr_device_annotation.physical_tile_pin_port_info(grid_type_descriptor,
                                                          src_grid_pin_index);
      VTR_ASSERT(true == src_grid_pin_info.is_valid());
      int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
        grid_type_descriptor, src_grid_pin_index);
      VTR_ASSERT(OPEN != subtile_index &&
                 subtile_index < grid_type_descriptor->capacity);
      std::string src_grid_port_name = generate_grid_port_name(
        src_grid_pin_width, src_grid_pin_height, subtile_index,
        get_rr_graph_single_node_side(
          rr_graph, rr_gsb.get_opin_node(side_manager.get_side(), inode)),
        src_grid_pin_info);
      ModulePortId src_grid_port_id =
        module_manager.find_module_port(src_grid_module, src_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module,
                                                             src_grid_port_id));
      BasicPort src_grid_port =
        module_manager.module_port(src_grid_module, src_grid_port_id);

      /* Collect sink-related information */
      vtr::Point<size_t> sink_sb_port_coord(
        rr_graph.node_xlow(
          module_sb.get_opin_node(side_manager.get_side(), inode)),
        rr_graph.node_ylow(
          module_sb.get_opin_node(side_manager.get_side(), inode)));
      std::string sink_sb_port_name = generate_sb_module_grid_port_name(
        side_manager.get_side(),
        get_rr_graph_single_node_side(
          rr_graph, module_sb.get_opin_node(side_manager.get_side(), inode)),
        grids, vpr_device_annotation, rr_graph,
        module_sb.get_opin_node(side_manager.get_side(), inode));
      ModulePortId sink_sb_port_id =
        module_manager.find_module_port(sink_sb_module, sink_sb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sink_sb_module,
                                                             sink_sb_port_id));
      BasicPort sink_sb_port =
        module_manager.module_port(sink_sb_module, sink_sb_port_id);

      /* Check if the grid is inside the tile, if not, create ports */
      if (fabric_tile.pb_in_tile(fabric_tile_id, grid_coordinate)) {
        VTR_LOGV(verbose,
                 "Build intra-tile nets between switch block '%s' and "
                 "programmable block '%s[%lu][%lu]'\n",
                 sink_sb_module_name.c_str(), src_grid_module_name.c_str(),
                 grid_coordinate.x(), grid_coordinate.y());
        if (!frame_view) {
          size_t src_grid_instance =
            pb_instances[fabric_tile.find_pb_index_in_tile(fabric_tile_id,
                                                           grid_coordinate)];

          /* Source and sink port should match in size */
          VTR_ASSERT(src_grid_port.get_width() == sink_sb_port.get_width());

          /* Create a net for each pin */
          for (size_t pin_id = 0; pin_id < src_grid_port.pins().size();
               ++pin_id) {
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, src_grid_module, src_grid_instance,
              src_grid_port_id, src_grid_port.pins()[pin_id]);
            /* Configure the net sink */
            module_manager.add_module_net_sink(
              tile_module, net, sink_sb_module, sink_sb_instance,
              sink_sb_port_id, sink_sb_port.pins()[pin_id]);
          }
        }
      } else {
        /* Create a port on the tile module and create the net if required.
         * Create a proper name to avoid naming conflicts */
        std::string temp_sb_module_name = generate_switch_block_module_name(
          fabric_tile.sb_coordinates(fabric_tile_id)[isb]);
        if (name_module_using_index) {
          temp_sb_module_name =
            generate_switch_block_module_name_using_index(isb);
        }
        src_grid_port.set_name(generate_tile_module_port_name(
          temp_sb_module_name, sink_sb_port.get_name()));
        ModulePortId src_tile_port_id = module_manager.add_port(
          tile_module, src_grid_port,
          ModuleManager::e_module_port_type::MODULE_INPUT_PORT);
        /* Set port side, inherit from the child module */
        module_manager.set_port_side(
          tile_module, src_tile_port_id,
          module_manager.port_side(sink_sb_module, sink_sb_port_id));
        VTR_LOGV(
          verbose,
          "Adding ports '%s' to tile as required by the switch block '%s'...\n",
          src_grid_port.to_verilog_string().c_str(),
          sink_sb_module_name.c_str());
        if (!frame_view) {
          /* Create a net for each pin */
          for (size_t pin_id = 0; pin_id < src_grid_port.pins().size();
               ++pin_id) {
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, tile_module, 0, src_tile_port_id,
              src_grid_port.pins()[pin_id]);
            /* Configure the net sink */
            module_manager.add_module_net_sink(
              tile_module, net, sink_sb_module, sink_sb_instance,
              sink_sb_port_id, sink_sb_port.pins()[pin_id]);
          }
        }
      }
    }
  }
  return CMD_EXEC_SUCCESS;
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
static int build_tile_module_port_and_nets_between_cb_and_pb(
  ModuleManager& module_manager, const ModuleId& tile_module,
  const DeviceGrid& grids, const size_t& layer,
  const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id, const t_rr_type& cb_type,
  const std::vector<size_t>& pb_instances,
  const std::map<t_rr_type, std::vector<size_t>>& cb_instances,
  const size_t& icb, const bool& compact_routing_hierarchy,
  const bool& name_module_using_index, const bool& frame_view,
  const bool& verbose) {
  size_t cb_instance = cb_instances.at(cb_type)[icb];
  /* We could have two different coordinators, one is the instance, the other is
   * the module */
  vtr::Point<size_t> instance_cb_coordinate(rr_gsb.get_cb_x(cb_type),
                                            rr_gsb.get_cb_y(cb_type));
  vtr::Point<size_t> module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* Skip those Connection blocks that do not exist */
  if (false == rr_gsb.is_cb_exist(cb_type)) {
    return CMD_EXEC_SUCCESS;
  }

  /* Skip if the cb does not contain any configuration bits! */
  if (true == connection_block_contain_only_routing_tracks(rr_gsb, cb_type)) {
    return CMD_EXEC_SUCCESS;
  }

  /* If we use compact routing hierarchy, we should find the unique module of
   * CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror =
      device_rr_gsb.get_cb_unique_module(cb_type, gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x());
    module_gsb_coordinate.set_y(unique_mirror.get_y());
  }

  /* This is the source cb that is added to the top module */
  const RRGSB& module_cb = device_rr_gsb.get_gsb(module_gsb_coordinate);
  vtr::Point<size_t> module_cb_coordinate(module_cb.get_cb_x(cb_type),
                                          module_cb.get_cb_y(cb_type));

  /* Collect source-related information */
  std::string src_cb_module_name =
    generate_connection_block_module_name(cb_type, module_cb_coordinate);
  ModuleId src_cb_module = module_manager.find_module(src_cb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(src_cb_module));
  /* Instance id should follow the instance cb coordinate */
  size_t src_cb_instance = cb_instance;

  /* Iterate over the output pins of the Connection Block */
  std::vector<enum e_side> cb_ipin_sides = module_cb.get_cb_ipin_sides(cb_type);
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < module_cb.get_num_ipin_nodes(cb_ipin_side);
         ++inode) {
      /* Collect source-related information */
      RRNodeId module_ipin_node = module_cb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> cb_src_port_coord(
        rr_graph.node_xlow(module_ipin_node),
        rr_graph.node_ylow(module_ipin_node));
      std::string src_cb_port_name = generate_cb_module_grid_port_name(
        cb_ipin_side, grids, vpr_device_annotation, rr_graph, module_ipin_node);
      ModulePortId src_cb_port_id =
        module_manager.find_module_port(src_cb_module, src_cb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_cb_module,
                                                             src_cb_port_id));
      BasicPort src_cb_port =
        module_manager.module_port(src_cb_module, src_cb_port_id);

      /* Collect sink-related information */
      /* Note that we use the instance cb pin here!!!
       * because it has the correct coordinator for the grid!!!
       */
      RRNodeId instance_ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> grid_coordinate(
        rr_graph.node_xlow(instance_ipin_node),
        rr_graph.node_ylow(instance_ipin_node));
      std::string sink_grid_module_name =
        generate_grid_block_module_name_in_top_module(
          std::string(GRID_MODULE_NAME_PREFIX), grids, grid_coordinate);
      ModuleId sink_grid_module =
        module_manager.find_module(sink_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sink_grid_module));
      size_t sink_grid_pin_index = rr_graph.node_pin_num(instance_ipin_node);

      t_physical_tile_type_ptr grid_type_descriptor = grids.get_physical_type(
        t_physical_tile_loc(grid_coordinate.x(), grid_coordinate.y(), layer));
      size_t sink_grid_pin_width =
        grid_type_descriptor->pin_width_offset[sink_grid_pin_index];
      size_t sink_grid_pin_height =
        grid_type_descriptor->pin_height_offset[sink_grid_pin_index];
      BasicPort sink_grid_pin_info =
        vpr_device_annotation.physical_tile_pin_port_info(grid_type_descriptor,
                                                          sink_grid_pin_index);
      VTR_ASSERT(true == sink_grid_pin_info.is_valid());
      int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
        grid_type_descriptor, sink_grid_pin_index);
      VTR_ASSERT(OPEN != subtile_index &&
                 subtile_index < grid_type_descriptor->capacity);
      std::string sink_grid_port_name = generate_grid_port_name(
        sink_grid_pin_width, sink_grid_pin_height, subtile_index,
        get_rr_graph_single_node_side(
          rr_graph, rr_gsb.get_ipin_node(cb_ipin_side, inode)),
        sink_grid_pin_info);
      ModulePortId sink_grid_port_id =
        module_manager.find_module_port(sink_grid_module, sink_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           sink_grid_module, sink_grid_port_id));
      BasicPort sink_grid_port =
        module_manager.module_port(sink_grid_module, sink_grid_port_id);

      /* Check if the grid is inside the tile, if not, create ports */
      if (fabric_tile.pb_in_tile(fabric_tile_id, grid_coordinate)) {
        if (!frame_view) {
          size_t sink_grid_instance =
            pb_instances[fabric_tile.find_pb_index_in_tile(fabric_tile_id,
                                                           grid_coordinate)];

          /* Source and sink port should match in size */
          VTR_ASSERT(src_cb_port.get_width() == sink_grid_port.get_width());

          /* Create a net for each pin */
          for (size_t pin_id = 0; pin_id < src_cb_port.pins().size();
               ++pin_id) {
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, src_cb_module, src_cb_instance,
              src_cb_port_id, src_cb_port.pins()[pin_id]);
            /* Configure the net sink */
            module_manager.add_module_net_sink(
              tile_module, net, sink_grid_module, sink_grid_instance,
              sink_grid_port_id, sink_grid_port.pins()[pin_id]);
          }
        }
      } else {
        /* Create a port on the tile module and create the net if required. */
        const RRGSB& cb_inst_rr_gsb = device_rr_gsb.get_gsb(
          fabric_tile.cb_coordinates(fabric_tile_id, cb_type)[icb]);
        std::string cb_instance_name_in_tile =
          generate_connection_block_module_name(
            cb_type, cb_inst_rr_gsb.get_cb_coordinate(cb_type));
        if (name_module_using_index) {
          cb_instance_name_in_tile =
            generate_connection_block_module_name_using_index(cb_type, icb);
        }
        src_cb_port.set_name(generate_tile_module_port_name(
          cb_instance_name_in_tile, src_cb_port.get_name()));
        ModulePortId sink_tile_port_id = module_manager.add_port(
          tile_module, src_cb_port,
          ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT);
        /* Set port side, inherit from the child module */
        module_manager.set_port_side(
          tile_module, sink_tile_port_id,
          module_manager.port_side(src_cb_module, src_cb_port_id));
        VTR_LOGV(verbose,
                 "Adding ports '%s' to tile as required by the connection "
                 "block '%s'...\n",
                 src_cb_port.to_verilog_string().c_str(),
                 src_cb_module_name.c_str());
        if (!frame_view) {
          /* Create a net for each pin */
          for (size_t pin_id = 0; pin_id < src_cb_port.pins().size();
               ++pin_id) {
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, src_cb_module, src_cb_instance,
              src_cb_port_id, src_cb_port.pins()[pin_id]);
            /* Configure the net sink */
            module_manager.add_module_net_sink(tile_module, net, tile_module, 0,
                                               sink_tile_port_id,
                                               src_cb_port.pins()[pin_id]);
          }
        }
      }
    }
  }
  /* Iterate over the output pins of the Connection Block */
  std::vector<enum e_side> cb_opin_sides = module_cb.get_cb_opin_sides(cb_type);
  for (size_t iside = 0; iside < cb_opin_sides.size(); ++iside) {
    enum e_side cb_opin_side = cb_opin_sides[iside];
    for (size_t inode = 0;
         inode < module_cb.get_num_cb_opin_nodes(cb_type, cb_opin_side);
         ++inode) {
      /* Collect source-related information */
      RRNodeId module_opin_node =
        module_cb.get_cb_opin_node(cb_type, cb_opin_side, inode);
      vtr::Point<size_t> cb_src_port_coord(
        rr_graph.node_xlow(module_opin_node),
        rr_graph.node_ylow(module_opin_node));
      std::string src_cb_port_name = generate_cb_module_grid_port_name(
        cb_opin_side, grids, vpr_device_annotation, rr_graph, module_opin_node);
      ModulePortId src_cb_port_id =
        module_manager.find_module_port(src_cb_module, src_cb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_cb_module,
                                                             src_cb_port_id));
      BasicPort src_cb_port =
        module_manager.module_port(src_cb_module, src_cb_port_id);

      /* Collect sink-related information */
      /* Note that we use the instance cb pin here!!!
       * because it has the correct coordinator for the grid!!!
       */
      RRNodeId instance_opin_node =
        rr_gsb.get_cb_opin_node(cb_type, cb_opin_side, inode);
      vtr::Point<size_t> grid_coordinate(
        rr_graph.node_xlow(instance_opin_node),
        rr_graph.node_ylow(instance_opin_node));
      std::string sink_grid_module_name =
        generate_grid_block_module_name_in_top_module(
          std::string(GRID_MODULE_NAME_PREFIX), grids, grid_coordinate);
      ModuleId sink_grid_module =
        module_manager.find_module(sink_grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sink_grid_module));
      size_t sink_grid_pin_index = rr_graph.node_pin_num(instance_opin_node);

      t_physical_tile_type_ptr grid_type_descriptor = grids.get_physical_type(
        t_physical_tile_loc(grid_coordinate.x(), grid_coordinate.y(), layer));
      size_t sink_grid_pin_width =
        grid_type_descriptor->pin_width_offset[sink_grid_pin_index];
      size_t sink_grid_pin_height =
        grid_type_descriptor->pin_height_offset[sink_grid_pin_index];
      BasicPort sink_grid_pin_info =
        vpr_device_annotation.physical_tile_pin_port_info(grid_type_descriptor,
                                                          sink_grid_pin_index);
      VTR_ASSERT(true == sink_grid_pin_info.is_valid());
      int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
        grid_type_descriptor, sink_grid_pin_index);
      VTR_ASSERT(OPEN != subtile_index &&
                 subtile_index < grid_type_descriptor->capacity);
      std::string sink_grid_port_name = generate_grid_port_name(
        sink_grid_pin_width, sink_grid_pin_height, subtile_index,
        get_rr_graph_single_node_side(
          rr_graph, rr_gsb.get_cb_opin_node(cb_type, cb_opin_side, inode)),
        sink_grid_pin_info);
      ModulePortId sink_grid_port_id =
        module_manager.find_module_port(sink_grid_module, sink_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           sink_grid_module, sink_grid_port_id));
      BasicPort sink_grid_port =
        module_manager.module_port(sink_grid_module, sink_grid_port_id);

      /* Check if the grid is inside the tile, if not, create ports */
      if (fabric_tile.pb_in_tile(fabric_tile_id, grid_coordinate)) {
        if (!frame_view) {
          size_t sink_grid_instance =
            pb_instances[fabric_tile.find_pb_index_in_tile(fabric_tile_id,
                                                           grid_coordinate)];

          /* Source and sink port should match in size */
          VTR_ASSERT(src_cb_port.get_width() == sink_grid_port.get_width());

          /* Create a net for each pin. Note that the sink and source tags are
           * reverted in the following code!!! */
          for (size_t pin_id = 0; pin_id < src_cb_port.pins().size();
               ++pin_id) {
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, sink_grid_module, sink_grid_instance,
              sink_grid_port_id, sink_grid_port.pins()[pin_id]);
            /* Configure the net sink */
            module_manager.add_module_net_sink(tile_module, net, src_cb_module,
                                               src_cb_instance, src_cb_port_id,
                                               src_cb_port.pins()[pin_id]);
          }
        }
      } else {
        /* Special: No need to create a new port! Since we only support OPINs
         * from Switch blocks. Walk through all the switch blocks and find the
         * new port that it is created when connecting pb and sb */
        if (!frame_view) {
          /* This is the source sb that is added to the top module */
          const RRGSB& module_sb = device_rr_gsb.get_gsb(module_gsb_coordinate);
          vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(),
                                                  module_sb.get_sb_y());

          /* Collect sink-related information */
          std::string sink_sb_module_name =
            generate_switch_block_module_name(module_sb_coordinate);
          ModuleId sink_sb_module =
            module_manager.find_module(sink_sb_module_name);
          VTR_ASSERT(true == module_manager.valid_module_id(sink_sb_module));
          size_t isb = fabric_tile.find_sb_index_in_tile(fabric_tile_id,
                                                         module_sb_coordinate);
          std::string temp_sb_module_name = generate_switch_block_module_name(
            fabric_tile.sb_coordinates(fabric_tile_id)[isb]);
          if (name_module_using_index) {
            temp_sb_module_name =
              generate_switch_block_module_name_using_index(isb);
          }
          /* FIXME: may find a way to determine the side. Currently using
           * cb_opin_side is fine */
          vtr::Point<size_t> sink_sb_port_coord(
            rr_graph.node_xlow(module_sb.get_opin_node(cb_opin_side, inode)),
            rr_graph.node_ylow(module_sb.get_opin_node(cb_opin_side, inode)));
          std::string sink_sb_port_name = generate_sb_module_grid_port_name(
            cb_opin_side,
            get_rr_graph_single_node_side(
              rr_graph, module_sb.get_opin_node(cb_opin_side, inode)),
            grids, vpr_device_annotation, rr_graph,
            module_sb.get_opin_node(cb_opin_side, inode));
          ModulePortId sink_sb_port_id =
            module_manager.find_module_port(sink_sb_module, sink_sb_port_name);
          VTR_ASSERT(true == module_manager.valid_module_port_id(
                               sink_sb_module, sink_sb_port_id));
          BasicPort sink_sb_port =
            module_manager.module_port(sink_sb_module, sink_sb_port_id);

          sink_sb_port.set_name(generate_tile_module_port_name(
            temp_sb_module_name, sink_sb_port.get_name()));
          ModulePortId src_tile_port_id = module_manager.find_module_port(
            tile_module, sink_sb_port.get_name());

          /* Create a net for each pin */
          VTR_ASSERT(src_cb_port.pins().size() == sink_sb_port.pins().size());
          for (size_t pin_id = 0; pin_id < src_cb_port.pins().size();
               ++pin_id) {
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, tile_module, 0, src_tile_port_id,
              sink_sb_port.pins()[pin_id]);
            /* Configure the net sink */
            module_manager.add_module_net_sink(tile_module, net, src_cb_module,
                                               src_cb_instance, src_cb_port_id,
                                               src_cb_port.pins()[pin_id]);
          }
        }
      }
    }
  }

  return CMD_EXEC_SUCCESS;
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
static int build_tile_module_port_and_nets_between_sb_and_cb(
  ModuleManager& module_manager, const ModuleId& tile_module,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id,
  const std::map<t_rr_type, std::vector<size_t>>& cb_instances,
  const std::vector<size_t>& sb_instances, const size_t& isb,
  const bool& compact_routing_hierarchy, const bool& name_module_using_index,
  const bool& frame_view, const bool& verbose) {
  size_t sb_instance = sb_instances[isb];
  /* We could have two different coordinators, one is the instance, the other is
   * the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(),
                                            rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_sb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist(rr_graph)) {
    return CMD_EXEC_SUCCESS;
  }

  /* If we use compact routing hierarchy, we should find the unique module of
   * CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(gsb_coord);
    module_gsb_sb_coordinate.set_x(unique_mirror.get_x());
    module_gsb_sb_coordinate.set_y(unique_mirror.get_y());
  }

  /* This is the source cb that is added to the top module */
  const RRGSB& module_sb = device_rr_gsb.get_gsb(module_gsb_sb_coordinate);
  vtr::Point<size_t> module_sb_coordinate(module_sb.get_sb_x(),
                                          module_sb.get_sb_y());
  std::string sb_module_name =
    generate_switch_block_module_name(module_sb_coordinate);
  ModuleId sb_module_id = module_manager.find_module(sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module_id));

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
    t_rr_type cb_type =
      find_top_module_cb_type_by_sb_side(side_manager.get_side());
    vtr::Point<size_t> instance_gsb_cb_coordinate =
      find_top_module_gsb_coordinate_by_sb_side(rr_gsb,
                                                side_manager.get_side());
    vtr::Point<size_t> module_gsb_cb_coordinate =
      find_top_module_gsb_coordinate_by_sb_side(rr_gsb,
                                                side_manager.get_side());

    /* Skip those Connection blocks that do not exist:
     * 1. The CB does not exist in the device level! We should skip!
     * 2. The CB does exist but we need to make sure if the GSB includes such
     * CBs For TOP and LEFT side, check the existence using RRGSB method
     * is_cb_exist() FOr RIGHT and BOTTOM side, find the adjacent RRGSB and then
     * use is_cb_exist()
     */
    if (TOP == side_manager.get_side() || LEFT == side_manager.get_side()) {
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }
    }

    if (RIGHT == side_manager.get_side() || BOTTOM == side_manager.get_side()) {
      const RRGSB& adjacent_gsb =
        device_rr_gsb.get_gsb(module_gsb_cb_coordinate);
      if (false == adjacent_gsb.is_cb_exist(cb_type)) {
        continue;
      }
    }

    /* If we use compact routing hierarchy, we should find the unique module of
     * CB, which is added to the top module */
    if (true == compact_routing_hierarchy) {
      const RRGSB& unique_mirror =
        device_rr_gsb.get_cb_unique_module(cb_type, module_gsb_cb_coordinate);
      module_gsb_cb_coordinate.set_x(unique_mirror.get_x());
      module_gsb_cb_coordinate.set_y(unique_mirror.get_y());
    }

    const RRGSB& module_cb = device_rr_gsb.get_gsb(module_gsb_cb_coordinate);
    vtr::Point<size_t> module_cb_coordinate(module_cb.get_cb_x(cb_type),
                                            module_cb.get_cb_y(cb_type));
    std::string cb_module_name =
      generate_connection_block_module_name(cb_type, module_cb_coordinate);
    ModuleId cb_module_id = module_manager.find_module(cb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_module_id));
    const RRGSB& instance_cb =
      device_rr_gsb.get_gsb(instance_gsb_cb_coordinate);
    vtr::Point<size_t> instance_cb_coordinate(instance_cb.get_cb_x(cb_type),
                                              instance_cb.get_cb_y(cb_type));

    /* Check if the grid is inside the tile, if not, create ports */
    if (fabric_tile.cb_in_tile(fabric_tile_id, cb_type,
                               instance_gsb_cb_coordinate)) {
      VTR_LOGV(
        verbose,
        "Skip adding ports to tile as connection block '%s' is part of the "
        "tile along with the switch block '%s'...\n",
        generate_connection_block_module_name(cb_type, instance_cb_coordinate)
          .c_str(),
        sb_module_name.c_str());
      if (!frame_view) {
        size_t cb_instance =
          cb_instances.at(cb_type)[fabric_tile.find_cb_index_in_tile(
            fabric_tile_id, cb_type, instance_gsb_cb_coordinate)];

        for (size_t itrack = 0;
             itrack < module_sb.get_chan_width(side_manager.get_side());
             ++itrack) {
          std::string sb_port_name = generate_sb_module_track_port_name(
            rr_graph.node_type(
              module_sb.get_chan_node(side_manager.get_side(), itrack)),
            side_manager.get_side(),
            module_sb.get_chan_node_direction(side_manager.get_side(), itrack));
          /* Prepare SB-related port information */
          ModulePortId sb_port_id =
            module_manager.find_module_port(sb_module_id, sb_port_name);
          VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module_id,
                                                                 sb_port_id));
          BasicPort sb_port =
            module_manager.module_port(sb_module_id, sb_port_id);

          /* Prepare CB-related port information */
          PORTS cb_port_direction = OUT_PORT;
          /* The cb port direction should be opposite to the sb port !!! */
          if (OUT_PORT == module_sb.get_chan_node_direction(
                            side_manager.get_side(), itrack)) {
            cb_port_direction = IN_PORT;
          } else {
            VTR_ASSERT(IN_PORT == module_sb.get_chan_node_direction(
                                    side_manager.get_side(), itrack));
          }

          /* Upper CB port is required if the routing tracks are on the top or
           * right sides of the switch block, which indicated bottom and left
           * sides of the connection blocks
           */
          bool use_cb_upper_port = (TOP == side_manager.get_side()) ||
                                   (RIGHT == side_manager.get_side());
          std::string cb_port_name = generate_cb_module_track_port_name(
            cb_type, cb_port_direction, use_cb_upper_port);
          ModulePortId cb_port_id =
            module_manager.find_module_port(cb_module_id, cb_port_name);
          VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module_id,
                                                                 cb_port_id));
          BasicPort cb_port =
            module_manager.module_port(cb_module_id, cb_port_id);

          /* Configure the net source and sink:
           * If sb port is an output (source), cb port is an input (sink)
           * If sb port is an input (sink), cb port is an output (source)
           */
          if (OUT_PORT == module_sb.get_chan_node_direction(
                            side_manager.get_side(), itrack)) {
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, sb_module_id, sb_instance,
              sb_port_id, itrack / 2);
            module_manager.add_module_net_sink(tile_module, net, cb_module_id,
                                               cb_instance, cb_port_id,
                                               itrack / 2);
          } else {
            VTR_ASSERT(IN_PORT == module_sb.get_chan_node_direction(
                                    side_manager.get_side(), itrack));
            ModuleNetId net = create_module_source_pin_net(
              module_manager, tile_module, cb_module_id, cb_instance,
              cb_port_id, itrack / 2);
            module_manager.add_module_net_sink(tile_module, net, sb_module_id,
                                               sb_instance, sb_port_id,
                                               itrack / 2);
          }
        }
      }
    } else {
      /* Create input and output ports */
      std::string chan_input_port_name = generate_sb_module_track_port_name(
        cb_type, side_manager.get_side(), IN_PORT);
      /* Create a port on the tile module and create the net if required. */
      ModulePortId sb_chan_input_port_id =
        module_manager.find_module_port(sb_module_id, chan_input_port_name);
      BasicPort chan_input_port =
        module_manager.module_port(sb_module_id, sb_chan_input_port_id);
      std::string temp_sb_module_name = generate_switch_block_module_name(
        fabric_tile.sb_coordinates(fabric_tile_id)[isb]);
      if (name_module_using_index) {
        temp_sb_module_name =
          generate_switch_block_module_name_using_index(isb);
      }
      chan_input_port.set_name(generate_tile_module_port_name(
        temp_sb_module_name, chan_input_port.get_name()));
      ModulePortId tile_chan_input_port_id = module_manager.add_port(
        tile_module, chan_input_port,
        ModuleManager::e_module_port_type::MODULE_INPUT_PORT);
      VTR_LOGV(
        verbose,
        "Adding ports '%s' to tile as required by the switch block '%s'...\n",
        chan_input_port.to_verilog_string().c_str(), sb_module_name.c_str());
      /* Create a net for each pin */
      if (!frame_view) {
        for (size_t pin_id = 0; pin_id < chan_input_port.pins().size();
             ++pin_id) {
          ModuleNetId net = create_module_source_pin_net(
            module_manager, tile_module, tile_module, 0,
            tile_chan_input_port_id, chan_input_port.pins()[pin_id]);
          /* Configure the net sink */
          module_manager.add_module_net_sink(tile_module, net, sb_module_id,
                                             sb_instance, sb_chan_input_port_id,
                                             chan_input_port.pins()[pin_id]);
        }
      }

      std::string chan_output_port_name = generate_sb_module_track_port_name(
        cb_type, side_manager.get_side(), OUT_PORT);
      /* Create a port on the tile module and create the net if required. FIXME:
       * Create a proper name to avoid naming conflicts  */
      ModulePortId sb_chan_output_port_id =
        module_manager.find_module_port(sb_module_id, chan_output_port_name);
      BasicPort chan_output_port =
        module_manager.module_port(sb_module_id, sb_chan_output_port_id);
      chan_output_port.set_name(generate_tile_module_port_name(
        temp_sb_module_name, chan_output_port.get_name()));
      ModulePortId tile_chan_output_port_id = module_manager.add_port(
        tile_module, chan_output_port,
        ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT);
      /* Set port side, inherit from the child module */
      module_manager.set_port_side(
        tile_module, tile_chan_output_port_id,
        module_manager.port_side(sb_module_id, sb_chan_output_port_id));
      VTR_LOGV(
        verbose,
        "Adding ports '%s' to tile as required by the switch block '%s'...\n",
        chan_output_port.to_verilog_string().c_str(), sb_module_name.c_str());
      /* Create a net for each pin */
      if (!frame_view) {
        for (size_t pin_id = 0; pin_id < chan_output_port.pins().size();
             ++pin_id) {
          ModuleNetId net = create_module_source_pin_net(
            module_manager, tile_module, sb_module_id, sb_instance,
            sb_chan_output_port_id, chan_output_port.pins()[pin_id]);
          /* Configure the net sink */
          module_manager.add_module_net_sink(tile_module, net, tile_module, 0,
                                             tile_chan_output_port_id,
                                             chan_output_port.pins()[pin_id]);
        }
      }
    }
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * This function will a port for tile where its connection blocks
 * need to drive or to be driven by a switch block which is not in the tile
 *
 *    +------------+      +------------------+
 *    | Connection |      |                  |
 *    |    Block   |----->|  Switch Block    |
 *    | X-direction|<-----|      [x][y]      |
 *    |   [x][y]   |      |                  |
 *    +------------+      +------------------+
 *
 *******************************************************************/
static int build_tile_module_one_port_from_cb(
  ModuleManager& module_manager, const ModuleId& tile_module,
  const ModuleId& cb_module, const std::string& chan_port_name,
  const vtr::Point<size_t>& tile_coord,
  const std::string& cb_instance_name_in_tile, const size_t& cb_instance,
  const bool& frame_view, const bool& verbose) {
  std::string cb_module_name = module_manager.module_name(cb_module);
  ModulePortId chan_port_id =
    module_manager.find_module_port(cb_module, chan_port_name);
  BasicPort chan_port = module_manager.module_port(cb_module, chan_port_id);
  ModuleManager::e_module_port_type chan_port_type =
    module_manager.port_type(cb_module, chan_port_id);

  bool require_port_addition = false;
  for (size_t pin_id = 0; pin_id < chan_port.pins().size(); ++pin_id) {
    if (module_manager.valid_module_net_id(
          tile_module, module_manager.module_instance_port_net(
                         tile_module, cb_module, cb_instance, chan_port_id,
                         chan_port.pins()[pin_id]))) {
      continue;
    }
    require_port_addition = true;
    break;
  }

  /* Early exit if this port is fully connected inside the tile */
  if (!require_port_addition) {
    return CMD_EXEC_SUCCESS;
  }

  BasicPort tile_chan_port(chan_port);
  tile_chan_port.set_name(generate_tile_module_port_name(
    cb_instance_name_in_tile, chan_port.get_name()));

  /* Add new port */
  VTR_LOGV(verbose,
           "Adding ports '%s' to tile as required by the "
           "connection block '%s'...\n",
           tile_chan_port.to_verilog_string().c_str(), cb_module_name.c_str());
  /* Create a new port and a new net. FIXME: Create a proper name to
   * avoid naming conflicts  */
  ModulePortId tile_module_port_id =
    module_manager.add_port(tile_module, tile_chan_port, chan_port_type);
  /* Set port side, inherit from the child module */
  module_manager.set_port_side(
    tile_module, tile_module_port_id,
    module_manager.port_side(cb_module, chan_port_id));

  if (!frame_view) {
    for (size_t pin_id = 0; pin_id < chan_port.pins().size(); ++pin_id) {
      if (module_manager.valid_module_net_id(
            tile_module, module_manager.module_instance_port_net(
                           tile_module, cb_module, cb_instance, chan_port_id,
                           chan_port.pins()[pin_id]))) {
        continue;
      }
      if (chan_port_type ==
          ModuleManager::e_module_port_type::MODULE_INPUT_PORT) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, tile_module, tile_module, 0, tile_module_port_id,
          chan_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(tile_module, net, cb_module,
                                           cb_instance, chan_port_id,
                                           chan_port.pins()[pin_id]);
      } else if (chan_port_type ==
                 ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, tile_module, cb_module, cb_instance, chan_port_id,
          chan_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(tile_module, net, tile_module, 0,
                                           tile_module_port_id,
                                           chan_port.pins()[pin_id]);
      } else {
        VTR_LOG_ERROR(
          "Expect either input or output port '%s' for cb module '%s' "
          "required by tile[%lu][%lu]!\n",
          chan_port.to_verilog_string().c_str(), cb_module_name.c_str(),
          tile_coord.x(), tile_coord.y());
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * This function will create ports for tile where its connection blocks
 * need to drive or to be driven by a switch block which is not in the tile
 *
 *    +------------+      +------------------+
 *    | Connection |      |                  |
 *    |    Block   |----->|  Switch Block    |
 *    | X-direction|<-----|      [x][y]      |
 *    |   [x][y]   |      |                  |
 *    +------------+      +------------------+
 *
 *******************************************************************/
static int build_tile_module_ports_from_cb(
  ModuleManager& module_manager, const ModuleId& tile_module,
  const DeviceRRGSB& device_rr_gsb, const RRGSB& rr_gsb,
  const FabricTile& fabric_tile, const FabricTileId& curr_fabric_tile_id,
  const t_rr_type& cb_type,
  const std::map<t_rr_type, std::vector<size_t>>& cb_instances,
  const size_t& icb, const bool& compact_routing_hierarchy,
  const bool& name_module_using_index, const bool& frame_view,
  const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;

  size_t cb_instance = cb_instances.at(cb_type)[icb];
  /* We could have two different coordinators, one is the instance, the other is
   * the module */
  vtr::Point<size_t> instance_cb_coordinate(rr_gsb.get_cb_x(cb_type),
                                            rr_gsb.get_cb_y(cb_type));
  vtr::Point<size_t> module_gsb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* Skip those Connection blocks that do not exist */
  if (false == rr_gsb.is_cb_exist(cb_type)) {
    return CMD_EXEC_SUCCESS;
  }

  /* If we use compact routing hierarchy, we should find the unique module of
   * CB, which is added to the top module */
  if (true == compact_routing_hierarchy) {
    vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    const RRGSB& unique_mirror =
      device_rr_gsb.get_cb_unique_module(cb_type, gsb_coord);
    module_gsb_coordinate.set_x(unique_mirror.get_x());
    module_gsb_coordinate.set_y(unique_mirror.get_y());
  }

  /* This is the source cb that is added to the top module */
  const RRGSB& module_cb = device_rr_gsb.get_gsb(module_gsb_coordinate);
  vtr::Point<size_t> module_cb_coordinate(module_cb.get_cb_x(cb_type),
                                          module_cb.get_cb_y(cb_type));

  /* Collect source-related information */
  std::string cb_module_name =
    generate_connection_block_module_name(cb_type, module_cb_coordinate);
  ModuleId cb_module = module_manager.find_module(cb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

  /* Find the instance name for the connection block in the context of the tile
   */
  vtr::Point<size_t> cb_coord_in_unique_tile =
    fabric_tile.cb_coordinates(curr_fabric_tile_id, cb_type)[icb];
  const RRGSB& unique_rr_gsb = device_rr_gsb.get_gsb(cb_coord_in_unique_tile);
  std::string cb_instance_name_in_tile = generate_connection_block_module_name(
    cb_type, unique_rr_gsb.get_cb_coordinate(cb_type));
  if (name_module_using_index) {
    cb_instance_name_in_tile =
      generate_connection_block_module_name_using_index(cb_type, icb);
  }
  vtr::Point<size_t> tile_coord =
    fabric_tile.tile_coordinate(curr_fabric_tile_id);

  /* Check any track input and output are unconnected in the tile */
  /* Upper input port: W/2 == 0 tracks */
  std::string chan_upper_input_port_name =
    generate_cb_module_track_port_name(cb_type, IN_PORT, true);

  /* Check if any of the input port is driven, if not add new port */
  status = build_tile_module_one_port_from_cb(
    module_manager, tile_module, cb_module, chan_upper_input_port_name,
    tile_coord, cb_instance_name_in_tile, cb_instance, frame_view, verbose);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Lower input port: W/2 == 1 tracks */
  std::string chan_lower_input_port_name =
    generate_cb_module_track_port_name(cb_type, IN_PORT, false);

  /* Check if any of the input port is driven, if not add new port */
  status = build_tile_module_one_port_from_cb(
    module_manager, tile_module, cb_module, chan_lower_input_port_name,
    tile_coord, cb_instance_name_in_tile, cb_instance, frame_view, verbose);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Upper output port: W/2 == 0 tracks */
  std::string chan_upper_output_port_name =
    generate_cb_module_track_port_name(cb_type, OUT_PORT, true);

  /* Check if any of the input port is driven, if not add new port */
  status = build_tile_module_one_port_from_cb(
    module_manager, tile_module, cb_module, chan_upper_output_port_name,
    tile_coord, cb_instance_name_in_tile, cb_instance, frame_view, verbose);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Lower output port: W/2 == 1 tracks */
  std::string chan_lower_output_port_name =
    generate_cb_module_track_port_name(cb_type, OUT_PORT, false);

  /* Check if any of the input port is driven, if not add new port */
  status = build_tile_module_one_port_from_cb(
    module_manager, tile_module, cb_module, chan_lower_output_port_name,
    tile_coord, cb_instance_name_in_tile, cb_instance, frame_view, verbose);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * This function will create nets for the unconnected pins for a programmable
 *block in a tile This function should be called after the following functions:
 * - build_tile_module_port_and_nets_between_sb_and_pb()
 * - build_tile_module_port_and_nets_between_cb_and_pb()
 * - build_tile_module_port_and_nets_between_sb_and_cb()
 * The functions above build nets/connections between the current programmable
 *block and other routing blocks. However, it could happen that the programmable
 *block is the only submodule under the tile. Also, it could happen that the
 *programmable block drives or is driven by other blocks from another tile. As a
 *result, we should build the nets for these unconnected pins
 * Note that direct connections, e.g., carry chain is not handled at tile-level.
 *It will be handled in the top-level module, which should not case a negative
 *impact on the physical design.
 *
 *    +------------+
 *    |            |-->grid_xxx
 *    |    Grid    |
 *    |   [x][y]   |<--grid_xxx
 *    |            |
 *    +------------+
 ********************************************************************/
static int build_tile_port_and_nets_from_pb(
  ModuleManager& module_manager, const ModuleId& tile_module,
  const DeviceGrid& grids, const size_t& layer,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const TileAnnotation& tile_annotation, const vtr::Point<size_t>& pb_coord,
  const std::vector<size_t>& pb_instances, const FabricTile& fabric_tile,
  const FabricTileId& curr_fabric_tile_id, const size_t& ipb,
  const bool& frame_view, const bool& verbose) {
  size_t pb_instance = pb_instances[ipb];
  t_physical_tile_type_ptr phy_tile = grids.get_physical_type(
    t_physical_tile_loc(pb_coord.x(), pb_coord.y(), layer));
  /* Empty type does not require a module */
  if (is_empty_type(phy_tile)) {
    return CMD_EXEC_SUCCESS;
  }
  e_side grid_side = find_grid_border_side(
    vtr::Point<size_t>(grids.width(), grids.height()), pb_coord);
  std::string pb_module_name = generate_grid_block_module_name(
    std::string(GRID_MODULE_NAME_PREFIX), std::string(phy_tile->name),
    is_io_type(phy_tile), grid_side);
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  if (!pb_module) {
    VTR_LOG_ERROR("Failed to find pb module '%s' required by tile[%lu][%lu]!\n",
                  pb_module_name.c_str(), pb_coord.x(), pb_coord.y());
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Find the pin side for I/O grids*/
  std::vector<e_side> grid_pin_sides;
  /* For I/O grids, we care only one side
   * Otherwise, we will iterate all the 4 sides
   */
  if (true == is_io_type(phy_tile)) {
    grid_pin_sides = find_grid_module_pin_sides(phy_tile, grid_side);
  } else {
    grid_pin_sides = {TOP, RIGHT, BOTTOM, LEFT};
  }

  /* Create a map between pin class type and grid pin direction */
  std::map<e_pin_type, ModuleManager::e_module_port_type> pin_type2type_map;
  pin_type2type_map[RECEIVER] = ModuleManager::MODULE_INPUT_PORT;
  pin_type2type_map[DRIVER] = ModuleManager::MODULE_OUTPUT_PORT;

  /* Iterate over sides, height and pins */
  for (const e_side& side : grid_pin_sides) {
    for (int iwidth = 0; iwidth < phy_tile->width; ++iwidth) {
      for (int iheight = 0; iheight < phy_tile->height; ++iheight) {
        for (int ipin = 0; ipin < phy_tile->num_pins; ++ipin) {
          if (!phy_tile->pinloc[iwidth][iheight][side][ipin]) {
            continue;
          }
          /* Reach here, it means this pin is on this side */
          /* Generate the pin name,
           * we give a empty coordinate but it will not be used (see details in
           * the function
           */
          BasicPort pin_info =
            vpr_device_annotation.physical_tile_pin_port_info(phy_tile, ipin);
          VTR_ASSERT(true == pin_info.is_valid());
          int subtile_index =
            vpr_device_annotation.physical_tile_pin_subtile_index(phy_tile,
                                                                  ipin);
          VTR_ASSERT(OPEN != subtile_index &&
                     subtile_index < phy_tile->capacity);
          std::string port_name = generate_grid_port_name(
            iwidth, iheight, subtile_index, side, pin_info);
          if (tile_annotation.is_tile_port_to_merge(std::string(phy_tile->name),
                                                    pin_info.get_name())) {
            if (subtile_index == 0) {
              port_name = generate_grid_port_name(0, 0, 0, TOP, pin_info);
            } else {
              continue;
            }
          }
          BasicPort pb_port(port_name, 0, 0);
          ModulePortId pb_module_port_id =
            module_manager.find_module_port(pb_module, port_name);
          if (!module_manager.valid_module_port_id(pb_module,
                                                   pb_module_port_id)) {
            VTR_LOG_ERROR(
              "Failed to find port '%s' for pb module '%s' required by "
              "tile[%lu][%lu]!\n",
              pb_port.to_verilog_string().c_str(), pb_module_name.c_str(),
              pb_coord.x(), pb_coord.y());
            return CMD_EXEC_FATAL_ERROR;
          }
          /* Create a proper name to avoid naming conflicts  */
          std::string pb_instance_name_in_tile =
            generate_grid_block_module_name_in_top_module(
              std::string(GRID_MODULE_NAME_PREFIX), grids,
              fabric_tile.pb_coordinates(curr_fabric_tile_id)[ipb]);
          pb_port.set_name(generate_tile_module_port_name(
            pb_instance_name_in_tile, pb_port.get_name()));

          /* Find the port from the pb module and see if it is already been
           * driven or driving a net. if not, create a new port at the tile
           * module */
          if (module_manager.port_type(pb_module, pb_module_port_id) ==
              ModuleManager::e_module_port_type::MODULE_INPUT_PORT) {
            for (size_t pin_id = 0; pin_id < pb_port.pins().size(); ++pin_id) {
              if (module_manager.valid_module_net_id(
                    tile_module,
                    module_manager.module_instance_port_net(
                      tile_module, pb_module, pb_instance, pb_module_port_id,
                      pb_port.pins()[pin_id]))) {
                continue;
              }
              VTR_LOGV(verbose,
                       "Adding ports '%s' to tile as required by the "
                       "programmable block '%s'...\n",
                       pb_port.to_verilog_string().c_str(),
                       pb_module_name.c_str());
              /* Create a new port and a new net. */
              ModulePortId tile_module_port_id = module_manager.add_port(
                tile_module, pb_port,
                ModuleManager::e_module_port_type::MODULE_INPUT_PORT);
              if (!frame_view) {
                ModuleNetId net = create_module_source_pin_net(
                  module_manager, tile_module, tile_module, 0,
                  tile_module_port_id, pb_port.pins()[pin_id]);
                /* Configure the net sink */
                module_manager.add_module_net_sink(
                  tile_module, net, pb_module, pb_instance, pb_module_port_id,
                  pb_port.pins()[pin_id]);
              }
            }
          } else if (module_manager.port_type(pb_module, pb_module_port_id) ==
                     ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT) {
            /* Note that an output may drive multiple blocks, therefore, we
             * cannot just check if there is a net driven by this pin, need to
             * check the fannout of the net!!! */
            for (size_t pin_id = 0; pin_id < pb_port.pins().size(); ++pin_id) {
              ModuleNetId curr_net = module_manager.module_instance_port_net(
                tile_module, pb_module, pb_instance, pb_module_port_id,
                pb_port.pins()[pin_id]);
              bool require_port_addition = true;
              if (module_manager.valid_module_net_id(tile_module, curr_net)) {
                size_t num_fanout_in_tile =
                  module_manager.module_net_sinks(tile_module, curr_net).size();
                RRNodeId rr_node = rr_graph.node_lookup().find_node(
                  layer, pb_coord.x() + iwidth, pb_coord.y() + iheight, OPIN,
                  ipin, side);
                size_t num_fanout_required =
                  rr_graph.node_out_edges(rr_node).size();
                if (num_fanout_in_tile == num_fanout_required) {
                  require_port_addition = false;
                }
              }
              if (!require_port_addition) {
                continue;
              }
              VTR_LOGV(verbose,
                       "Adding ports '%s' to tile as required by the "
                       "programmable block '%s'...\n",
                       pb_port.to_verilog_string().c_str(),
                       pb_module_name.c_str());
              /* Create a new port and a new net. FIXME: Create a proper name to
               * avoid naming conflicts  */
              ModulePortId tile_module_port_id = module_manager.add_port(
                tile_module, pb_port,
                ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT);
              /* Set port side, inherit from the child module */
              module_manager.set_port_side(
                tile_module, tile_module_port_id,
                module_manager.port_side(pb_module, pb_module_port_id));
              if (!frame_view) {
                ModuleNetId net = create_module_source_pin_net(
                  module_manager, tile_module, pb_module, pb_instance,
                  pb_module_port_id, pb_port.pins()[pin_id]);
                /* Configure the net sink */
                module_manager.add_module_net_sink(
                  tile_module, net, tile_module, 0, tile_module_port_id,
                  pb_port.pins()[pin_id]);
              }
            }
          } else {
            VTR_LOG_ERROR(
              "Expect either input or output port '%s' for pb module '%s' "
              "required by tile[%lu][%lu]!\n",
              pb_port.to_verilog_string().c_str(), pb_module_name.c_str(),
              pb_coord.x(), pb_coord.y());
            return CMD_EXEC_FATAL_ERROR;
          }
        }
      }
    }
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Build the ports and associated nets for a tile module
 * For each submodule, e.g., programmable block, connection block and switch
 *blocks, we walk through their neighbours, starting from sb, cb and then
 *programmable blocks.
 * - If a neighbour is one of the submodules under this tile, we can build nets
 *between the submodule and its neighbour.
 * - If the neighbour is not under this tile, we should build ports for the tile
 *module and then build nets to connect Note that if frame_view is enabled, nets
 *are not built
 *******************************************************************/
static int build_tile_module_ports_and_nets(
  ModuleManager& module_manager, const ModuleId& tile_module,
  const DeviceGrid& grids, const size_t& layer,
  const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph_view,
  const TileAnnotation& tile_annotation, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id, const std::vector<size_t>& pb_instances,
  const std::map<t_rr_type, std::vector<size_t>>& cb_instances,
  const std::vector<size_t>& sb_instances, const bool& name_module_using_index,
  const bool& frame_view, const bool& verbose) {
  int status_code = CMD_EXEC_SUCCESS;

  /* Get the submodule of Switch blocks one by one, build connections between sb
   * and pb */
  for (size_t isb = 0; isb < fabric_tile.sb_coordinates(fabric_tile_id).size();
       ++isb) {
    vtr::Point<size_t> sb_coord =
      fabric_tile.sb_coordinates(fabric_tile_id)[isb];
    const RRGSB& rr_gsb = device_rr_gsb.get_gsb(sb_coord);
    status_code = build_tile_module_port_and_nets_between_sb_and_pb(
      module_manager, tile_module, grids, layer, vpr_device_annotation,
      device_rr_gsb, rr_graph_view, rr_gsb, fabric_tile, fabric_tile_id,
      pb_instances, sb_instances, isb, true, name_module_using_index,
      frame_view, verbose);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  /* Get the submodule of connection blocks one by one, build connections
   * between cb and pb */
  for (t_rr_type cb_type : {CHANX, CHANY}) {
    for (size_t icb = 0;
         icb < fabric_tile.cb_coordinates(fabric_tile_id, cb_type).size();
         ++icb) {
      vtr::Point<size_t> cb_coord =
        fabric_tile.cb_coordinates(fabric_tile_id, cb_type)[icb];
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(cb_coord);
      status_code = build_tile_module_port_and_nets_between_cb_and_pb(
        module_manager, tile_module, grids, layer, vpr_device_annotation,
        device_rr_gsb, rr_graph_view, rr_gsb, fabric_tile, fabric_tile_id,
        cb_type, pb_instances, cb_instances, icb, true, name_module_using_index,
        frame_view, verbose);
      if (status_code != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  /* Get the submodule of connection blocks one by one, build connections
   * between sb and cb */
  for (size_t isb = 0; isb < fabric_tile.sb_coordinates(fabric_tile_id).size();
       ++isb) {
    vtr::Point<size_t> sb_coord =
      fabric_tile.sb_coordinates(fabric_tile_id)[isb];
    const RRGSB& rr_gsb = device_rr_gsb.get_gsb(sb_coord);
    status_code = build_tile_module_port_and_nets_between_sb_and_cb(
      module_manager, tile_module, device_rr_gsb, rr_graph_view, rr_gsb,
      fabric_tile, fabric_tile_id, cb_instances, sb_instances, isb, true,
      name_module_using_index, frame_view, verbose);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  /* Create the ports from pb which only connects to adjacent sb and cbs, as
   * well as pb */
  for (size_t ipb = 0; ipb < fabric_tile.pb_coordinates(fabric_tile_id).size();
       ++ipb) {
    vtr::Point<size_t> pb_coord =
      fabric_tile.pb_coordinates(fabric_tile_id)[ipb];
    status_code = build_tile_port_and_nets_from_pb(
      module_manager, tile_module, grids, layer, vpr_device_annotation,
      rr_graph_view, tile_annotation, pb_coord, pb_instances, fabric_tile,
      fabric_tile_id, ipb, frame_view, verbose);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  /* Get the submodule of connection blocks one by one, build connections
   * between cb and pb */
  for (t_rr_type cb_type : {CHANX, CHANY}) {
    for (size_t icb = 0;
         icb < fabric_tile.cb_coordinates(fabric_tile_id, cb_type).size();
         ++icb) {
      vtr::Point<size_t> cb_coord =
        fabric_tile.cb_coordinates(fabric_tile_id, cb_type)[icb];
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(cb_coord);

      /* Build any ports missing from connection blocks */
      status_code = build_tile_module_ports_from_cb(
        module_manager, tile_module, device_rr_gsb, rr_gsb, fabric_tile,
        fabric_tile_id, cb_type, cb_instances, icb, true,
        name_module_using_index, frame_view, verbose);
      if (status_code != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  return status_code;
}

/********************************************************************
 * Build a tile module
 * - Add instances
 * - Add ports and nets
 * - Add global ports
 * - Add I/O ports
 * - Add configuration ports and nets
 *******************************************************************/
static int build_tile_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const FabricTile& fabric_tile, const FabricTileId& fabric_tile_id,
  const DeviceGrid& grids, const size_t& layer,
  const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph_view,
  const TileAnnotation& tile_annotation, const CircuitLibrary& circuit_lib,
  const CircuitModelId& sram_model,
  const e_config_protocol_type& sram_orgz_type,
  const bool& name_module_using_index, const bool& frame_view,
  const bool& verbose) {
  int status_code = CMD_EXEC_SUCCESS;

  /* Create the module */
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);
  std::string module_name = generate_tile_module_name(tile_coord);
  VTR_LOGV(verbose, "Building tile module '%s'...\n", module_name.c_str());
  ModuleId tile_module = module_manager.add_module(module_name);

  /* Add instance of programmable block */
  std::vector<size_t>
    pb_instances; /* Keep tracking the instance id of each pb */
  for (vtr::Point<size_t> grid_coord :
       fabric_tile.pb_coordinates(fabric_tile_id)) {
    t_physical_tile_type_ptr phy_tile = grids.get_physical_type(
      t_physical_tile_loc(grid_coord.x(), grid_coord.y(), layer));
    VTR_LOGV(verbose, "Try to find pb at [%lu][%lu]\n", grid_coord.x(),
             grid_coord.y());
    /* Empty type does not require a module */
    if (!is_empty_type(phy_tile)) {
      e_side grid_side = find_grid_border_side(
        vtr::Point<size_t>(grids.width(), grids.height()), grid_coord);
      std::string pb_module_name = generate_grid_block_module_name(
        std::string(GRID_MODULE_NAME_PREFIX), std::string(phy_tile->name),
        is_io_type(phy_tile), grid_side);
      ModuleId pb_module = module_manager.find_module(pb_module_name);
      if (!pb_module) {
        VTR_LOG_ERROR(
          "Failed to find pb module '%s' required by tile[%lu][%lu]!\n",
          pb_module_name.c_str(), tile_coord.x(), tile_coord.y());
        return CMD_EXEC_FATAL_ERROR;
      }
      size_t pb_instance = module_manager.num_instance(tile_module, pb_module);
      module_manager.add_child_module(tile_module, pb_module, false);
      std::string pb_instance_name = generate_grid_block_instance_name(
        std::string(GRID_MODULE_NAME_PREFIX), std::string(phy_tile->name),
        is_io_type(phy_tile), grid_side, grid_coord);
      module_manager.set_child_instance_name(tile_module, pb_module,
                                             pb_instance, pb_instance_name);
      if (0 < find_module_num_config_bits(module_manager, pb_module,
                                          circuit_lib, sram_model,
                                          sram_orgz_type)) {
        module_manager.add_configurable_child(
          tile_module, pb_module, pb_instance,
          ModuleManager::e_config_child_type::UNIFIED);
      }
      VTR_LOGV(
        verbose,
        "Added programmable module '%s' (instance: '%s') to tile[%lu][%lu]\n",
        pb_module_name.c_str(), pb_instance_name.c_str(), tile_coord.x(),
        tile_coord.y());
      pb_instances.push_back(pb_instance);
      /* Add a custom I/O child with the grid */
      module_manager.add_io_child(
        tile_module, pb_module, pb_instance,
        vtr::Point<int>(grid_coord.x(), grid_coord.y()));
    }
  }

  /* Add instance of connection blocks */
  std::map<t_rr_type, std::vector<size_t>>
    cb_instances; /* Keep tracking the instance id of each cb */
  for (t_rr_type cb_type : {CHANX, CHANY}) {
    for (vtr::Point<size_t> cb_coord :
         fabric_tile.cb_coordinates(fabric_tile_id, cb_type)) {
      /* get the unique module coord */
      const RRGSB& unique_rr_gsb =
        device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
      vtr::Point<size_t> unique_cb_coord(unique_rr_gsb.get_cb_x(cb_type),
                                         unique_rr_gsb.get_cb_y(cb_type));
      std::string cb_module_name =
        generate_connection_block_module_name(cb_type, unique_cb_coord);
      ModuleId cb_module = module_manager.find_module(cb_module_name);
      if (!cb_module) {
        VTR_LOG_ERROR(
          "Failed to find connection block module '%s' required by "
          "tile[%lu][%lu]!\n",
          cb_module_name.c_str(), tile_coord.x(), tile_coord.y());
        return CMD_EXEC_FATAL_ERROR;
      }
      size_t cb_instance = module_manager.num_instance(tile_module, cb_module);
      module_manager.add_child_module(tile_module, cb_module, false);
      const RRGSB& inst_rr_gsb = device_rr_gsb.get_gsb(cb_coord);
      std::string cb_instance_name = generate_connection_block_module_name(
        cb_type, inst_rr_gsb.get_cb_coordinate(cb_type));
      module_manager.set_child_instance_name(tile_module, cb_module,
                                             cb_instance, cb_instance_name);
      if (0 < find_module_num_config_bits(module_manager, cb_module,
                                          circuit_lib, sram_model,
                                          sram_orgz_type)) {
        module_manager.add_configurable_child(
          tile_module, cb_module, cb_instance,
          ModuleManager::e_config_child_type::UNIFIED);
      }
      VTR_LOGV(verbose,
               "Added connection block module '%s' (instance: '%s') to "
               "tile[%lu][%lu]\n",
               cb_module_name.c_str(), cb_instance_name.c_str(), tile_coord.x(),
               tile_coord.y());
      cb_instances[cb_type].push_back(cb_instance);
    }
  }

  /* Add instance of switch blocks */
  std::vector<size_t>
    sb_instances; /* Keep tracking the instance id of each sb */
  for (vtr::Point<size_t> sb_coord :
       fabric_tile.sb_coordinates(fabric_tile_id)) {
    /* get the unique module coord */
    const RRGSB& unique_rr_gsb = device_rr_gsb.get_sb_unique_module(sb_coord);
    vtr::Point<size_t> unique_sb_coord(unique_rr_gsb.get_sb_x(),
                                       unique_rr_gsb.get_sb_y());
    std::string sb_module_name =
      generate_switch_block_module_name(unique_sb_coord);
    ModuleId sb_module = module_manager.find_module(sb_module_name);
    if (!sb_module) {
      VTR_LOG_ERROR(
        "Failed to find switch block module '%s' required by tile[%lu][%lu]!\n",
        sb_module_name.c_str(), tile_coord.x(), tile_coord.y());
      return CMD_EXEC_FATAL_ERROR;
    }
    size_t sb_instance = module_manager.num_instance(tile_module, sb_module);
    module_manager.add_child_module(tile_module, sb_module, false);
    const RRGSB& inst_rr_gsb = device_rr_gsb.get_gsb(sb_coord);
    std::string sb_instance_name =
      generate_switch_block_module_name(inst_rr_gsb.get_sb_coordinate());
    module_manager.set_child_instance_name(tile_module, sb_module, sb_instance,
                                           sb_instance_name);
    if (0 < find_module_num_config_bits(module_manager, sb_module, circuit_lib,
                                        sram_model, sram_orgz_type)) {
      module_manager.add_configurable_child(
        tile_module, sb_module, sb_instance,
        ModuleManager::e_config_child_type::UNIFIED);
    }
    VTR_LOGV(
      verbose,
      "Added switch block module '%s' (instance: %s') to tile[%lu][%lu]\n",
      sb_module_name.c_str(), sb_instance_name.c_str(), tile_coord.x(),
      tile_coord.y());
    sb_instances.push_back(sb_instance);
  }

  /* Add module nets and ports */
  status_code = build_tile_module_ports_and_nets(
    module_manager, tile_module, grids, layer, vpr_device_annotation,
    device_rr_gsb, rr_graph_view, tile_annotation, fabric_tile, fabric_tile_id,
    pb_instances, cb_instances, sb_instances, name_module_using_index,
    frame_view, verbose);

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, tile_module);

  /* Count GPIO ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  add_module_gpio_ports_from_child_modules(module_manager, tile_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_shared_config_bits =
    find_module_num_shared_config_bits_from_child_modules(module_manager,
                                                          tile_module);
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, tile_module,
                                              module_num_shared_config_bits);
  }

  /* Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_config_bits =
    find_module_num_config_bits_from_child_modules(
      module_manager, tile_module, circuit_lib, sram_model, sram_orgz_type,
      ModuleManager::e_config_child_type::LOGICAL);
  if (0 < module_num_config_bits) {
    add_pb_sram_ports_to_module_manager(module_manager, tile_module,
                                        circuit_lib, sram_model, sram_orgz_type,
                                        module_num_config_bits);
  }

  /* Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb
   * module!
   */
  if (0 < module_manager.num_configurable_children(
            tile_module, ModuleManager::e_config_child_type::LOGICAL)) {
    add_pb_module_nets_memory_config_bus(
      module_manager, decoder_lib, tile_module, sram_orgz_type,
      circuit_lib.design_tech_type(sram_model),
      ModuleManager::e_config_child_type::LOGICAL);
  }

  VTR_LOGV(verbose, "Done\n");
  return status_code;
}

/********************************************************************
 * Build all the tile modules
 *******************************************************************/
int build_tile_modules(ModuleManager& module_manager,
                       DecoderLibrary& decoder_lib,
                       const FabricTile& fabric_tile, const DeviceGrid& grids,
                       const VprDeviceAnnotation& vpr_device_annotation,
                       const DeviceRRGSB& device_rr_gsb,
                       const RRGraphView& rr_graph_view,
                       const TileAnnotation& tile_annotation,
                       const CircuitLibrary& circuit_lib,
                       const CircuitModelId& sram_model,
                       const e_config_protocol_type& sram_orgz_type,
                       const bool& name_module_using_index,
                       const bool& frame_view, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build tile modules for the FPGA fabric");

  int status_code = CMD_EXEC_SUCCESS;

  size_t layer = 0;

  /* Build a module for each unique tile  */
  for (FabricTileId fabric_tile_id : fabric_tile.unique_tiles()) {
    status_code = build_tile_module(
      module_manager, decoder_lib, fabric_tile, fabric_tile_id, grids, layer,
      vpr_device_annotation, device_rr_gsb, rr_graph_view, tile_annotation,
      circuit_lib, sram_model, sram_orgz_type, name_module_using_index,
      frame_view, verbose);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  return status_code;
}

} /* end namespace openfpga */
