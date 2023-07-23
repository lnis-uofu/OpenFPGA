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
  const FabricTileId& curr_fabric_tile_id,
  const size_t& sb_idx_in_curr_fabric_tile,
  const bool& compact_routing_hierarchy,
  const bool& verbose) {
  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist()) {
    return CMD_EXEC_SUCCESS;
  }

  vtr::Point<size_t> sink_tile_coord = fabric_tile.tile_coordinate(curr_fabric_tile_id);
  FabricTileId sink_unique_tile = fabric_tile.unique_tile(curr_fabric_tile_id);
  vtr::Point<size_t> sink_sb_coord_in_unique_tile = fabric_tile.sb_coordinates(src_unique_tile)[sb_idx_in_curr_fabric_tile];
  std::string sink_sb_instance_name_in_unique_tile =
    generate_switch_block_module_name(sink_sb_coord_in_unique_tile);

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
      if (fabric_tile.pb_in_tile(curr_fabric_tile_id, grid_coordinate)) {
        continue;
      }

      /* Find the source tile id, coordinate etc., which is required to find source tile module and port 
       *    Relationship between source tile and its unique module
       *    Take an example:
       *
       *    grid_pin name should follow unique module [i0][j0] of src_tile[x0][y0]
       *    sb_pin name should follow unique module [i1][j1] of des_tile[x1][y1]
       *
       *    However, instance id should follow the origin tiles
       *
       *    Src tile                    Des tile
       *    +------------+             +--------------+
       *    |            |             |              |
       *    |    Grid    |------------>| Switch Block |
       *    |  [x0][y0]  |             | [x1][y1]     |
       *    |            |             |              |
       *    +------------+             +--------------+
       *          ^                             ^
       *         || unique_mirror               || unique mirror
       *    +------------+             +--------------+
       *    |            |             |              |
       *    |    Grid    |<------------| Switch Block |
       *    |  [i0][j0]  |             | [i1][j1]     |
       *    |            |             |              |
       *    +------------+             +--------------+
       *
       */
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
      size_t src_tile_instance =
        tile_instance_ids[src_tile_coord.x()][src_tile_coord.y()];

      size_t src_grid_pin_index = rr_graph.node_pin_num(
        rr_gsb.get_opin_node(side_manager.get_side(), inode));
      std::string src_grid_port_name = generate_grid_module_port_name_in_top_module(
        grid_coordinate, src_grid_pin_index, vpr_device_annotation,
          rr_graph, rr_gsb.get_opin_node(side_manager.get_side(), inode));

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

      std::string sink_tile_sb_port_name = generate_tile_module_port_name(sink_sb_instance_name_in_unique_tile, sink_sb_port_name);
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
               sink_sb_instance_name_in_unique_tile.c_str(),
               sink_tile_coord.x(), sink_tile_coord.y(),
               src_tile_coord.x(), src_tile_coord.y());

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
 *    However, instance id should follow the origin Grid and Connection block
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
  const ModuleId& tile_module,
  const vtr::Matrix<size_t>& tile_instance_ids, 
  const size_t& tile_instance_id,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& curr_fabric_tile_id, const t_rr_type& cb_type,
  const size_t& cb_idx_in_curr_fabric_tile,
  const bool& compact_routing_hierarchy,
  const bool& verbose) {
  vtr::Point<size_t> src_tile_coord = fabric_tile.tile_coordinate(curr_fabric_tile_id);
  FabricTileId src_unique_tile = fabric_tile.unique_tile(curr_fabric_tile_id);
  vtr::Point<size_t> src_cb_coord_in_unique_tile = fabric_tile.cb_coordinates(src_unique_tile, cb_type)[cb_idx_in_curr_fabric_tile];
  std::string src_cb_instance_name_in_unique_tile =
    generate_connection_block_module_name(cb_type, sink_sb_coord_in_unique_tile);

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
      std::string src_tile_cb_port_name = generate_tile_module_port_name(src_cb_instance_name_in_unique_tile, src_cb_port_name);
      ModulePortId src_cb_port_id =
        module_manager.find_module_port(tile_module, src_cb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(tile_module,
                                                             src_cb_port_id));
      BasicPort src_cb_port =
        module_manager.module_port(tile_module, src_cb_port_id);

      /* Collect sink-related information */
      /* Note that we use the instance cb pin here!!!
       * because it has the correct coordinator for the grid!!!
       */
      RRNodeId instance_ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> grid_coordinate(
        rr_graph.node_xlow(instance_ipin_node),
        rr_graph.node_ylow(instance_ipin_node));
      /* Check if the grid is inside the tile, if not, create ports */
      if (fabric_tile.pb_in_tile(curr_fabric_tile_id, grid_coordinate)) {
        continue;
      }

      FabricTileId sink_fabric_tile_id = fabric_tile.find_tile_by_pb_coordinate(grid_coordinate);
      size_t pb_idx_in_sink_fabric_tile = fabric_tile.find_pb_index_in_tile(sink_fabric_tile_id, grid_coordinate);
      vtr::Point<size_t> sink_tile_coord = fabric_tile.tile_coordinate(sink_fabric_tile_id);
      vtr::Point<size_t> sink_unique_tile_coord = fabric_tile.unique_tile_coordinate(sink_fabric_tile_id);
      FabricTileId sink_unique_tile = fabric_tile.unique_tile(sink_tile_coord);
      vtr::Point<size_t> sink_pb_coord_in_unique_tile = fabric_tile.pb_coordinates(sink_unique_tile)[pb_idx_in_sink_fabric_tile];
      std::string sink_tile_module_name =
        generate_tile_module_name(sink_unique_tile_coord);
      ModuleId sink_tile_module =
        module_manager.find_module(sink_tile_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sink_tile_module));
      size_t sink_tile_instance_id = tile_instance_ids[sink_tile_coord.x()][sink_tile_coord.y()];

      std::string sink_grid_module_name =
        generate_grid_block_module_name_in_top_module(
          std::string(GRID_MODULE_NAME_PREFIX), grids, sink_pb_coord_in_unique_tile);

      size_t sink_grid_pin_index = rr_graph.node_pin_num(instance_ipin_node);

      std::string sink_grid_port_name = generate_grid_module_port_name_in_top_module(
        grid_coordinate, sink_grid_pin_index, vpr_device_annotation,
          rr_graph, rr_gsb.get_ipin_node(cb_ipin_side, inode));

      std::string sink_tile_grid_port_name = generate_tile_module_port_name(sink_grid_module_name, sink_grid_port_name);
      ModulePortId sink_grid_port_id =
        module_manager.find_module_port(sink_tile_module, sink_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           sink_tile_module, sink_grid_port_id));
      BasicPort sink_grid_port =
        module_manager.module_port(sink_tile_module, sink_grid_port_id);

      /* Create nets */
      /* Source and sink port should match in size */
      VTR_ASSERT(src_cb_port.get_width() == sink_grid_port.get_width());

      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_cb_port.pins().size();
           ++pin_id) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, top_module, tile_module, tile_instance_id,
          src_cb_port_id, src_cb_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(
          top_module, net, sink_tile_module, sink_tile_instance_id,
          sink_grid_port_id, sink_grid_port.pins()[pin_id]);
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
  const ModuleId& tile_module, 
  const vtr::Matrix<size_t>& tile_instance_ids, 
  const size_t& tile_instance_id,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& curr_fabric_tile_id,
  const size_t& sb_idx_in_curr_fabric_tile,
  const bool& compact_routing_hierarchy,
  const bool& verbose) {
  /* We could have two different coordinators, one is the instance, the other is
   * the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(),
                                            rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_sb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  vtr::Point<size_t> sb_tile_coord = fabric_tile.tile_coordinate(curr_fabric_tile_id);
  FabricTileId sb_unique_tile = fabric_tile.unique_tile(curr_fabric_tile_id);
  vtr::Point<size_t> sb_coord_in_unique_tile = fabric_tile.sb_coordinates(sb_unique_tile)[sb_idx_in_curr_fabric_tile];
  std::string sb_instance_name_in_unique_tile =
    generate_switch_block_module_name(sb_coord_in_unique_tile);

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

    const RRGSB& instance_cb =
      device_rr_gsb.get_gsb(instance_gsb_cb_coordinate);
    vtr::Point<size_t> instance_cb_coordinate(instance_cb.get_cb_x(cb_type),
                                              instance_cb.get_cb_y(cb_type));

    /* Check if the grid is inside the tile, if not, create ports */
    if (fabric_tile.cb_in_tile(fabric_tile_id, cb_type,
                               instance_cb_coordinate)) {
      continue;
    }
    /* Collect cb tile information */
    FabricTileId cb_tile = fabric_tile.find_tile_by_cb_coordinate(cb_type, instance_gsb_cb_coordinate);
    vtr::Point<size_t> cb_tile_coord = fabric_tile.tile_coordindate(cb_tile);
    size_t cb_idx_in_cb_tile = fabric_tile.find_cb_index_in_tile(cb_tile, cb_type, instance_gsb_cb_coordinate);
    FabricTileId cb_unique_tile = fabric_tile.unique_tile(cb_tile_coord);
    vtr::Point<size_t> cb_unique_tile_coord = fabric_tile.tile_coordindate(cb_unique_tile);
    vtr::Point<size_t> cb_coord_in_cb_unique_tile = fabric_tile.cb_coordinates(cb_unique_tile, cb_type)[cb_idx_in_cb_tile];
    std::string cb_instance_name_in_unique_tile =
      generate_connection_block_module_name(cb_type, cb_coord_in_cb_unique_tile);
    std::string cb_tile_module_name =
      generate_tile_module_name(cb_unique_tile_coord);
    ModuleId cb_tile_module =
      module_manager.find_module(cb_tile_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_tile_module));
    size_t cb_tile_instance =
      tile_instance_ids[cb_tile_coord.x()][cb_tile_coord.y()];

    /* Create nets */
    for (size_t itrack = 0;
         itrack < module_sb.get_chan_width(side_manager.get_side());
         ++itrack) {
      std::string sb_port_name = generate_sb_module_track_port_name(
        rr_graph.node_type(
          module_sb.get_chan_node(side_manager.get_side(), itrack)),
        side_manager.get_side(),
        module_sb.get_chan_node_direction(side_manager.get_side(), itrack));
      /* Prepare SB-related port information */
      std::string sb_tile_sb_port_name = generate_tile_module_port_name(sb_instance_name_in_unique_tile, sb_port_name);
      ModulePortId sb_port_id =
        module_manager.find_module_port(tile_module, sb_tile_sb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(tile_module,
                                                             sb_port_id));
      BasicPort sb_port =
        module_manager.module_port(tile_module, sb_port_id);

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
      std::string cb_tile_cb_port_name = generate_tile_module_port_name(cb_instance_name_in_unique_tile, cb_port_name);
      ModulePortId cb_port_id =
        module_manager.find_module_port(cb_tile_module, cb_tile_cb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(cb_tile_module,
                                                             cb_port_id));
      BasicPort cb_port =
        module_manager.module_port(cb_tile_module, cb_port_id);

      /* Configure the net source and sink:
       * If sb port is an output (source), cb port is an input (sink)
       * If sb port is an input (sink), cb port is an output (source)
       */
      if (OUT_PORT == module_sb.get_chan_node_direction(
                        side_manager.get_side(), itrack)) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, top_module, tile_module, tile_instance_id,
          sb_port_id, itrack / 2);
        module_manager.add_module_net_sink(top_module, net, cb_tile_module,
                                           cb_tile_instance, cb_port_id,
                                           itrack / 2);
      } else {
        VTR_ASSERT(IN_PORT == module_sb.get_chan_node_direction(
                                side_manager.get_side(), itrack));
        ModuleNetId net = create_module_source_pin_net(
          module_manager, top_module, cb_tile_module, cb_tile_instance,
          cb_port_id, itrack / 2);
        module_manager.add_module_net_sink(top_module, net, tile_module,
                                           tile_instance_id, sb_port_id,
                                           itrack / 2);
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
  const DeviceRRGSB& device_rr_gsb, const FabricTile& fabric_tile, const FabricTileId& curr_fabric_tile_id,
  const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;

  /* Find the module name for this type of tile */
  vtr::Point<size_t> unique_tile_coord =
    fabric_tile.unique_tile_coordinate(curr_fabric_tile_id);
  std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
  ModuleId tile_module = module_manager.find_module(tile_module_name);
  if (!module_manager.valid_module_id(tile_module)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  
  /* Find the instance id for this tile */
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(curr_fabric_tile_id);
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
      rr_graph_view, rr_gsb, fabric_tile, curr_fabric_tile_id, isb, true, verbose);
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
        module_manager, top_module, tile_module, tile_instance_ids, tile_instance_id,
        grids, vpr_device_annotation,
        device_rr_gsb, rr_graph_view, rr_gsb,
        fabric_tile, curr_fabric_tile_id,
        cb_type, icb, true, 
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
      module_manager, top_module, tile_module, tile_instance_ids, tile_instance_id, device_rr_gsb, rr_graph_view, rr_gsb,
      fabric_tile, curr_fabric_tile_id, isb, true,
      verbose);
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
      FabricTileId curr_fabric_tile_id = fabric_tile.find_tile(curr_coord);
      if (!fabric_tile.valid_tile_id(curr_fabric_tile_id)) {
        continue;
      }
      status = add_top_module_nets_around_one_tile(
        module_manager, top_module, vpr_device_annotation, grids,
        tile_instance_ids, rr_graph, device_rr_gsb, fabric_tile, curr_fabric_tile_id, verbose);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Organize the memory modules for tiles under the top-level module
 * Follow a snake-like sequence
 *       +-----------------------> Tail
 *       +<----------------------+
 *                               |
 *            ... 
 *       +---------------------->+
 *       +<----------------------+
 * head  ----------------------->+
 *******************************************************************/
static void organize_top_module_tile_based_memory_modules(
  ModuleManager& module_manager, const ModuleId& top_module,
  const CircuitLibrary& circuit_lib, const ConfigProtocol& config_protocol,
  const CircuitModelId& sram_model, const DeviceGrid& grids,
  const vtr::Matrix<size_t>& tile_instance_ids,
  const FabricTile& fabric_tile) {
  /* Ensure clean vectors to return */
  VTR_ASSERT(true == module_manager.configurable_children(top_module).empty());

  std::vector<vtr::Point<size_t>> tile_coords;
  bool positive_direction = true;
  for (size_t iy = 0; iy < grids.height(); ++iy) {
    /* For positive direction: -----> */
    if (true == positive_direction) {
      for (size_t ix = 0; ix < grids.width(); ++ix) {
        tile_coords.push_back(vtr::Point<size_t>(ix, iy));
      }
    } else {
      VTR_ASSERT(false == positive_direction);
      /* For negative direction: -----> */
      for (size_t ix = grids.width() - 1; ix >= 0; --ix) {
        tile_coords.push_back(vtr::Point<size_t>(ix, iy));
      }
    }
    /* Flip the positive direction to be negative */
    positive_direction = !positive_direction;
  }

  for (const vtr::Point<size_t>& tile_coord : tile_coords) {
    FabricTileId curr_fabric_tile_id = fabric_tile.find_tile(tile_coord);
    if (!fabric_tile.valid_tile_id(curr_fabric_tile_id)) {
      continue;
    }
    vtr::Point<size_t> curr_tile_coord = fabric_tile.tile_coordinate(curr_fabric_tile_id);
    vtr::Point<size_t> unique_tile_coord = fabric_tile.unique_tile_coordinate(curr_fabric_tile_id);
    std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
    ModuleId tile_module = mnodule_manager.find_module(tile_module_name);
    VTR_ASSERT(module_manager.valid_module_id(tile_module));
  
    if (0 < find_module_num_config_bits(module_manager, tile_module,
                                        circuit_lib, sram_model,
                                        sram_orgz_type)) {
      module_manager.add_configurable_child(
        top_module, tile_module,
        tile_instance_ids[curr_tile_coord.x()][curr_tile_coord.y()], curr_tile_coord);
    }
  }

  /* Split memory modules into different regions */
  build_top_module_configurable_regions(module_manager, top_module,
                                        config_protocol);
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
    organize_top_module_tile_based_memory_modules(
      module_manager, top_module, circuit_lib, config_protocol, sram_model,
      grids, tile_instance_ids, fabric_tile);
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
