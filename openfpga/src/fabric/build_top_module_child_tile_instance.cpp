/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <algorithm>
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "vpr_utils.h"

/* Headers from openfpgashell library */
#include "build_module_graph_utils.h"
#include "build_top_module_child_tile_instance.h"
#include "build_top_module_connection.h"
#include "build_top_module_directs.h"
#include "build_top_module_memory.h"
#include "build_top_module_memory_bank.h"
#include "build_top_module_utils.h"
#include "command_exit_codes.h"
#include "module_manager_memory_utils.h"
#include "module_manager_utils.h"
#include "openfpga_device_grid_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Add a instance of a tile module to the top module
 *******************************************************************/
static size_t add_top_module_tile_instance(ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const FabricTile& fabric_tile,
                                           const FabricTileId& fabric_tile_id) {
  /* Find the module name for this type of grid */
  vtr::Point<size_t> unique_tile_coord =
    fabric_tile.unique_tile_coordinate(fabric_tile_id);
  std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
  ModuleId tile_module = module_manager.find_module(tile_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(tile_module));
  /* Record the instance id */
  size_t tile_instance = module_manager.num_instance(top_module, tile_module);
  /* Add the module to top_module */
  module_manager.add_child_module(top_module, tile_module, false);
  /* Set an unique name to the instance
   * Note: it is your risk to gurantee the name is unique!
   */
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);
  std::string instance_name = generate_tile_module_name(tile_coord);
  module_manager.set_child_instance_name(top_module, tile_module, tile_instance,
                                         instance_name);
  return tile_instance;
}

/********************************************************************
 * Add all the tiles as sub-modules across the fabric
 * Here, we will iterate over the full fabric (coordinates)
 * and instanciate the tile modules
 *
 * Return an 2-D array of instance ids of the grid modules that
 * have been added
 *
 * This function assumes an island-style floorplanning for FPGA fabric
 *
 *
 *                +-----------------------------------+
 *                |              I/O tiles            |
 *                |              TOP side             |
 *                +-----------------------------------+
 *
 * +-----------+  +-----------------------------------+ +------------+
 * |           |  |                                   | |            |
 * | I/O tiles |  |          Core tiles               | | I/O tiles  |
 * | LEFT side |  | (CLB, Heterogeneous blocks, etc.) | | RIGHT side |
 * |           |  |                                   | |            |
 * +-----------+  +-----------------------------------+ +------------+
 *
 *                +-----------------------------------+
 *                |              I/O tiles            |
 *                |             BOTTOM side           |
 *                +-----------------------------------+
 *
 *******************************************************************/
static int add_top_module_tile_instances(ModuleManager& module_manager,
                                         const ModuleId& top_module,
                                         vtr::Matrix<size_t>& tile_instance_ids,
                                         const DeviceGrid& grids,
                                         const FabricTile& fabric_tile) {
  vtr::ScopedStartFinishTimer timer("Add tile instances to top module");
  int status = CMD_EXEC_SUCCESS;

  /* Reserve an array for the instance ids */
  tile_instance_ids.resize({grids.width(), grids.height()});
  tile_instance_ids.fill(size_t(-1));

  /* Instanciate I/O grids */
  /* Create the coordinate range for each side of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_tile_coordinates(grids);

  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coord : io_coordinates[io_side]) {
      FabricTileId fabric_tile_id = fabric_tile.find_tile(io_coord);
      if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
        continue;
      }
      /* Add a tile module to top_module*/
      tile_instance_ids[io_coord.x()][io_coord.y()] =
        add_top_module_tile_instance(module_manager, top_module, fabric_tile,
                                     fabric_tile_id);
    }
  }

  /* Instanciate core grids
   * IMPORTANT: sequence matters here, it impacts the I/O indexing.
   * We should follow the same sequence as the build_io_location_map()!
   * If you change the sequence of walking through grids here, you should change
   * it in the build_io_location map()!
   */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      vtr::Point<size_t> curr_coord(ix, iy);
      FabricTileId fabric_tile_id = fabric_tile.find_tile(curr_coord);
      if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
        continue;
      }
      /* Add a tile module to top_module*/
      tile_instance_ids[curr_coord.x()][curr_coord.y()] =
        add_top_module_tile_instance(module_manager, top_module, fabric_tile,
                                     fabric_tile_id);
    }
  }
  return status;
}

/********************************************************************
 * Add the I/O children to the top-level module, which impacts the I/O indexing
 * This is the default function to build the I/O sequence/indexing
 * The I/O children is added in a maze shape
 * The function supports I/Os in the center of grids, starting from the
 *bottom-left corner and ending at the center
 *
 *    +----------------------+
 *    |+--------------------+|
 *    ||+------------------+||
 *    |||+----------------+|||
 *    ||||+-------------->||||
 *    ||||+---------------+|||
 *    |||+-----------------+||
 *    ||+-------------------+|
 *    |+---------------------+
 *    ^
 *  io[0]
 *******************************************************************/
static void add_top_module_tile_io_children(
  ModuleManager& module_manager, const ModuleId& top_module,
  const DeviceGrid& grids, const FabricTile& fabric_tile,
  const vtr::Matrix<size_t>& tile_instance_ids) {
  /* Create the coordinate range for the perimeter I/Os of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_tile_coordinates(grids);

  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coord : io_coordinates[io_side]) {
      FabricTileId fabric_tile_id = fabric_tile.find_tile(io_coord);
      if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
        continue;
      }
      /* Find the module name for this type of tile */
      vtr::Point<size_t> unique_tile_coord =
        fabric_tile.unique_tile_coordinate(fabric_tile_id);
      std::string tile_module_name =
        generate_tile_module_name(unique_tile_coord);
      ModuleId tile_module = module_manager.find_module(tile_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(tile_module));
      /* Add a I/O children to top_module*/
      module_manager.add_io_child(top_module, tile_module,
                                  tile_instance_ids[io_coord.x()][io_coord.y()],
                                  vtr::Point<int>(io_coord.x(), io_coord.y()));
    }
  }

  /* Walk through the center grids */
  size_t xmin = 1;
  size_t xmax = grids.width() - 2;
  size_t ymin = 1;
  size_t ymax = grids.height() - 2;
  std::vector<vtr::Point<size_t>> coords;
  while (xmin < xmax && ymin < ymax) {
    for (size_t iy = ymin; iy < ymax + 1; iy++) {
      coords.push_back(vtr::Point<size_t>(xmin, iy));
    }
    for (size_t ix = xmin + 1; ix < xmax + 1; ix++) {
      coords.push_back(vtr::Point<size_t>(ix, ymax));
    }
    for (size_t iy = ymax - 1; iy > ymin; iy--) {
      coords.push_back(vtr::Point<size_t>(xmax, iy));
    }
    for (size_t ix = xmax; ix > xmin; ix--) {
      coords.push_back(vtr::Point<size_t>(ix, ymin));
    }
    xmin++;
    ymin++;
    xmax--;
    ymax--;
  }

  /* If height is odd, add the missing horizental line */
  if ((grids.height() - 2) % 2 == 1) {
    if (ymin == ymax) {
      for (size_t ix = xmin; ix < xmax + 1; ix++) {
        coords.push_back(vtr::Point<size_t>(ix, ymin));
      }
    }
  }
  /* If width is odd, add the missing vertical line */
  if ((grids.width() - 2) % 2 == 1) {
    if (xmin == xmax) {
      for (size_t iy = ymin; iy < ymax + 1; iy++) {
        coords.push_back(vtr::Point<size_t>(xmin, iy));
      }
    }
  }

  /* Now walk through the coordinates */
  for (vtr::Point<size_t> coord : coords) {
    FabricTileId fabric_tile_id = fabric_tile.find_tile(coord);
    if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
      continue;
    }
    /* Find the module name for this type of tile */
    vtr::Point<size_t> unique_tile_coord =
      fabric_tile.unique_tile_coordinate(fabric_tile_id);
    std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
    ModuleId tile_module = module_manager.find_module(tile_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(tile_module));
    /* Add a I/O children to top_module*/
    module_manager.add_io_child(top_module, tile_module,
                                tile_instance_ids[coord.x()][coord.y()],
                                vtr::Point<int>(coord.x(), coord.y()));
  }
}

/********************************************************************
 * Add module nets to connect a switch block in a given tile to the programmable block in adjacent tiles
 *
 *  TileA             | TileB
 *    +------------+  |             +------------+
 *    |            |  |             |            |
 *    |    Grid    |  |             |    Grid    |
 *    |  [x][y+1]  |  |             | [x+1][y+1] |
 *    |            |--|-+      +----|            |
 *    +------------+  | |      |    +------------+
 *           |        | |      |         |
 *  ------------------+ v      v         |
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
static int build_top_module_tile_nets_between_sb_and_pb(
  ModuleManager& module_manager, const ModuleId& top_module,
  const ModuleId& curr_tile_module,
  const vtr::Matrix<size_t>& tile_instance_ids, 
  const size_t& curr_tile_instance_id,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id,
  const bool& compact_routing_hierarchy,
  const bool& verbose) {
  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist()) {
    return CMD_EXEC_SUCCESS;
  }

  vtr::Point<size_t> sink_tile_coord = fabric_tile.tile_coordinate(curr_fabric_tile_id);

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

  /* Connect grid output pins (OPIN) to switch block grid pins */
  for (size_t side = 0; side < module_sb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    for (size_t inode = 0;
         inode < module_sb.get_num_opin_nodes(side_manager.get_side());
         ++inode) {
      /* Collect source-related information */
      vtr::Point<size_t> grid_coordinate(
        rr_graph.node_xlow(
          rr_gsb.get_opin_node(side_manager.get_side(), inode)),
        rr_graph.node_ylow(
          rr_gsb.get_opin_node(side_manager.get_side(), inode)));
      /* Check if the grid is inside the tile, if not, create ports */
      if (fabric_tile.pb_in_tile(fabric_tile_id, grid_coordinate)) {
        continue;
      }

      /* Find the source tile id, coordinate etc., which is required to find source tile module and port */
      FabricTileId src_fabric_tile_id = fabric_tile.find_tile_by_pb_coordinate(grid_coordinate);
      size_t pb_idx_in_src_fabric_tile = fabric_tile.find_pb_index_in_tile(src_fabric_tile_id, grid_coordinate);
      vtr::Point<size_t> src_tile_coord = fabric_tile.tile_coordinate(src_fabric_tile_id);
      vtr::Point<size_t> src_unique_tile_coord = fabric_tile.unique_tile_coordinate(src_fabric_tile_id);
      FabricTileId src_unique_tile = fabric_tile.unique_tile(src_tile_coord);
      vtr::Point<size_t> src_pb_coord_in_unique_tile = fabric_tile.pb_coordinates(src_unique_tile)[pb_idx_in_src_fabric_tile];
      std::string src_tile_module_name =
        generate_tile_module_name(src_unique_tile_coord);
      ModuleId src_tile_module =
        module_manager.find_module(src_tile_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(src_tile_module));
      size_t src_grid_pin_index = rr_graph.node_pin_num(
        rr_gsb.get_opin_node(side_manager.get_side(), inode));

      t_physical_tile_type_ptr grid_type_descriptor =
        grids.get_physical_type(grid_coordinate.x(), grid_coordinate.y());
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
      std::string src_grid_module_name = generate_grid_block_module_name_in_top_module(std::string(GRID_MODULE_NAME_PREFIX), grids, src_pb_coord_in_unique_tile);
      std::string src_tile_grid_port_name = generate_tile_module_port_name(src_grid_module_name, src_grid_port_name);
      ModulePortId src_tile_grid_port_id =
        module_manager.find_module_port(src_tile_module, src_tile_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(src_tile_module,
                                                             src_tile_grid_port_id));
      BasicPort src_tile_grid_port =
        module_manager.module_port(src_tile_module, src_tile_grid_port_id);

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
      std::string sink_tile_sb_port_name = generate_tile_module_port_name(sink_sb_module_name, sink_sb_port_name);
      ModulePortId sink_tile_sb_port_id =
        module_manager.find_module_port(tile_module, sink_tile_sb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(tile_module,
                                                             sink_tile_sb_port_id));
      BasicPort sink_tile_sb_port =
        module_manager.module_port(tile_module, sink_tile_sb_port_id);

      /* Create nets */
      VTR_LOGV(verbose,
               "Build inter-tile nets between switch block '%s' in tile[%lu][%lu] and "
               "programmable block in tile[%lu][%lu]\n",
               sink_sb_module_name.c_str(),
               sink_tile_coord.x(), sink_tile_coord.y(),
               src_tile_coord.x(), src_tile_coord.y());
      size_t src_tile_instance =
        tile_instance_ids[src_tile_coord.x(), src_tile_coord.y()];

      /* Source and sink port should match in size */
      VTR_ASSERT(src_tile_grid_port.get_width() == sink_tile_sb_port.get_width());

      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_tile_grid_port.pins().size();
           ++pin_id) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, top_module, src_tile_module, src_tile_instance,
          src_tile_grid_port_id, src_tile_grid_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(
          top_module, net, curr_tile_module, curr_tile_instance_id,
          sink_tile_sb_port_id, sink_tile_sb_port.pins()[pin_id]);
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
 *    TileA           |   TileB
 *    +------------+  |   +------------------+      +------------+
 *    |            |  |   |                  |      |            |
 *    |    Grid    |<-----| Connection Block |----->|    Grid    |
 *    |  [x][y+1]  |  |   |  Y-direction     |      | [x+1][y+1] |
 *    |            |  |   |    [x][y+1]      |      |            |
 *    +------------+  |   +------------------+      +------------+
 *          ^         |
 *    ----------------+
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
static int build_top_module_tile_nets_between_cb_and_pb(
  ModuleManager& module_manager, const ModuleId& top_module,
  const ModuleId& tile_module, const size_t& tile_instance_id,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id, const t_rr_type& cb_type,
  const bool& compact_routing_hierarchy,
  const bool& verbose) {
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

      t_physical_tile_type_ptr grid_type_descriptor =
        grids.get_physical_type(grid_coordinate.x(), grid_coordinate.y());
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
        continue;
      }
      /* TODO: Create nets */
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
 *    TileA                                     |  TileB
 *    +------------+      +------------------+  |   +------------+
 *    |            |      |                  |  |   |            |
 *    |    Grid    |      | Connection Block |  |   |    Grid    |
 *    |  [x][y+1]  |      |  Y-direction     |  |   | [x+1][y+1] |
 *    |            |      |    [x][y+1]      |  |   |            |
 *    +------------+      +------------------+  |   +------------+
 *                             |       ^        |
 *                             v       |        |
 *    +------------+      +------------------+  |   +------------+
 *    | Connection |----->|                  |----->| Connection |
 *    |    Block   |      |  Switch Block    |  |   |  Block     |
 *    | X-direction|<-----|      [x][y]      |<-----| X-direction|
 *    |   [x][y]   |      |                  |  |   | [x+1][y]   |
 *    +------------+      +------------------+  |   +------------+
 *                            |        ^        |
 *    ------------------------------------------+
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
static int build_top_module_tile_nets_between_sb_and_cb(
  ModuleManager& module_manager, const ModuleId& top_module,
  const ModuleId& tile_module, const size_t& tile_instance_id,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& fabric_tile_id,
  const bool& compact_routing_hierarchy,
  const bool& verbose) {
  /* We could have two different coordinators, one is the instance, the other is
   * the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(),
                                            rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_sb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist()) {
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
                               instance_cb_coordinate)) {
      continue;
    }
    /* TODO: Create nets */
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * This function will create nets between two programmable blocks, which are direct connections
 ********************************************************************/
static int build_top_module_tile_nets_between_pbs(
  ModuleManager& module_manager, const ModuleId& top_module,
  const ModuleId& tile_module, const size_t& tile_instance_id,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const vtr::Point<size_t>& pb_coord, const size_t& pb_instance,
  const bool& compact_routing_hierarchy,
  const bool& verbose) {
  t_physical_tile_type_ptr phy_tile =
    grids.get_physical_type(pb_coord.x(), pb_coord.y());
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

          /* Find the port from the pb module and see if it is already been
           * driven or driving a net. if not, create a new port at the tile
           * module */
          if (module_manager.port_type(pb_module, pb_module_port_id) ==
              ModuleManager::e_module_port_type::MODULE_INPUT_PORT) {
            /* TODO: Create pb-to-pb nets */
          } else if (module_manager.port_type(pb_module, pb_module_port_id) ==
                     ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT) {
            /* TODO: Create pb-to-pb nets */
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
 * Add module nets to connect the pins between tiles
 * To make it easy, this function will iterate over all the tiles, through which we can obtain the coordinates
 * of each programmable block (PB), connection block (CB) and switch block (SB). With the coordinates, we can then trace the connections between these blocks using the RRGSB data structure.
 *
 *  +--------+    +----------+
 *  | Tile   |--->| Tile     |
 *  | [x][y] |<---| [x+1][y] |
 *  +--------+    +----------+
 *
 * The inter-tile connections can be categorized into four types:
 * - PB-to-SB connections: We use the GSB to find the connections with a given SB coordinate. Note that we only care the connections where the driver PB is not in this tile. 
 * - CB-to-PB connections: We use the GSB to find the connections with a given CB coordinate. Note that we only care the connections where the sink PB is not in this tile. 
 * - SB-to-CB connections: We use the GSB to find the connections with a given SB coordinate. Note that we only care the connections where the sink CB is not in this tile. 
 * - PB-to-PB connections: We use the tile direct data structure to find all the connections. Note that we only care the connections where the driver PB is not in this tile. 
 *
 *******************************************************************/
static int add_top_module_nets_around_one_tile(
  ModuleManager& module_manager, const ModuleId& top_module,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const vtr::Matrix<size_t>& tile_instance_ids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const FabricTile& fabric_tile, const FabricTileId& fabric_tile_id,
  const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;

  /* Find the module name for this type of tile */
  vtr::Point<size_t> unique_tile_coord =
    fabric_tile.unique_tile_coordinate(fabric_tile_id);
  std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
  ModuleId tile_module = module_manager.find_module(tile_module_name);
  if (!module_manager.valid_module_id(tile_module)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  
  /* Find the instance id for this tile */
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);
  size_t tile_instance_id = tile_instance_ids[tile_coord.x()][tile_coord.y()];

  /* Get the submodule of Switch blocks one by one, build connections between sb
   * and pb */
  for (size_t isb = 0; isb < fabric_tile.sb_coordinates(fabric_tile_id).size();
       ++isb) {
    vtr::Point<size_t> sb_coord =
      fabric_tile.sb_coordinates(fabric_tile_id)[isb];
    const RRGSB& rr_gsb = device_rr_gsb.get_gsb(sb_coord);
    status_code = build_top_module_tile_nets_between_sb_and_pb(
      module_manager, top_module, tile_module, tile_instance_ids, tile_instance_id, grids, vpr_device_annotation, device_rr_gsb,
      rr_graph_view, rr_gsb, fabric_tile, fabric_tile_id, true, verbose);
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
      status_code = build_top_module_tile_nets_between_cb_and_pb(
        module_manager, tile_module, grids, vpr_device_annotation,
        device_rr_gsb, rr_graph_view, rr_gsb, fabric_tile, fabric_tile_id,
        cb_type, pb_instances, cb_instances.at(cb_type)[icb], true, 
        verbose);
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
    status_code = build_top_module_tile_nets_between_sb_and_cb(
      module_manager, tile_module, device_rr_gsb, rr_graph_view, rr_gsb,
      fabric_tile, fabric_tile_id, cb_instances, sb_instances[isb], true,
      verbose);
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
    status_code = build_top_module_tile_nets_between_pbs(
      module_manager, tile_module, grids, vpr_device_annotation, pb_coord,
      pb_instances[ipb], verbose);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Walk through each tile instance and add module nets to connect the pins between tiles
 *******************************************************************/
static int add_top_module_nets_connect_tiles(
  ModuleManager& module_manager, const ModuleId& top_module,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const vtr::Matrix<size_t>& tile_instance_ids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const FabricTile& fabric_tile,
  const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Add module nets between tiles");
  int status = CMD_EXEC_SUCCESS;

  for (size_t ix = 0; ix < grids.width(); ++ix) {
    for (size_t iy = 0; iy < grids.height(); ++iy) {
      vtr::Point<size_t> curr_coord(ix, iy);
      FabricTileId fabric_tile_id = fabric_tile.find_tile(curr_coord);
      if (!fabric_tile.valid_tile_id(fabric_tile_id)) {
        continue;
      }
      status = add_top_module_nets_around_one_tile(
        module_manager, top_module, vpr_device_annotation, grids,
        tile_instance_ids, rr_graph, device_rr_gsb, fabric_tile, fabric_tile_id, verbose);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Add the tile-level instances to the top module of FPGA fabric
 * and build connects between them
 *******************************************************************/
int build_top_module_tile_child_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const DeviceGrid& grids,
  const FabricTile& fabric_tile, const ConfigProtocol& config_protocol,
  const FabricKey& fabric_key, const bool& frame_view,
  const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  vtr::Matrix<size_t> tile_instance_ids;
  status = add_top_module_tile_instances(module_manager, top_module,
                                         tile_instance_ids, grids, fabric_tile);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Update the I/O children list */
  add_top_module_tile_io_children(module_manager, top_module, grids,
                                  fabric_tile, tile_instance_ids);

  /* TODO: Build the nets between tiles */
  if (false == frame_view) {
    /* Reserve nets to be memory efficient */
    reserve_module_manager_module_nets(module_manager, top_module);
    /* TODO: Regular nets between tiles */
    status = add_top_module_nets_connect_tiles(
      module_manager, top_module, vpr_device_annotation, grids,
      tile_instance_ids, rr_graph, device_rr_gsb, fabric_tile, verbose);
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
    /* TODO: Inter-tile direct connections */
  }

  /* TODO: Add global ports from tile modules: how to connect to clock
  architecture and the global port from tile annotation status =
  add_top_module_global_ports_from_grid_modules( module_manager, top_module,
  tile_annotation, vpr_device_annotation, grids, rr_graph, device_rr_gsb,
  cb_instance_ids, grid_instance_ids, clk_ntwk, rr_clock_lookup); if
  (CMD_EXEC_FATAL_ERROR == status) { return status;
  }
  */

  /* Add GPIO ports from the sub-modules under this Verilog module
   * For top-level module, we follow a special sequencing for I/O modules. So we
   * rebuild the I/O children list here
   */
  add_module_gpio_ports_from_child_modules(module_manager, top_module);

  /* Organize the list of memory modules and instances
   * If we have an empty fabric key, we organize the memory modules as routine
   * Otherwise, we will load the fabric key directly
   */
  if (true == fabric_key.empty()) {
    /* TODO: need a special one for tiles
    organize_top_module_memory_modules(
      module_manager, top_module, circuit_lib, config_protocol, sram_model,
      grids, grid_instance_ids, device_rr_gsb, sb_instance_ids, cb_instance_ids,
      compact_routing_hierarchy);
     */
  } else {
    VTR_ASSERT_SAFE(false == fabric_key.empty());
    /* Throw a fatal error when the fabric key has a mismatch in region
     * organization. between architecture file and fabric key
     */
    if (size_t(config_protocol.num_regions()) != fabric_key.regions().size()) {
      VTR_LOG_ERROR(
        "Fabric key has a different number of configurable regions (='%ld') "
        "than architecture definition (=%d)!\n",
        fabric_key.regions().size(), config_protocol.num_regions());
      return CMD_EXEC_FATAL_ERROR;
    }

    status = load_top_module_memory_modules_from_fabric_key(
      module_manager, top_module, circuit_lib, config_protocol, fabric_key);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }

    status = load_top_module_shift_register_banks_from_fabric_key(
      fabric_key, blwl_sr_banks);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }

    /* Update the memory organization in sub module (non-top) */
    status = load_submodules_memory_modules_from_fabric_key(
      module_manager, circuit_lib, config_protocol, fabric_key);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
