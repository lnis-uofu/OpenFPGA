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
#include "build_routing_module_utils.h"
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
#include "openfpga_physical_tile_utils.h"
#include "openfpga_reserved_words.h"
#include "rr_gsb_utils.h"
#include "tileable_rr_graph_utils.h"

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
 * Add module nets to connect a switch block in a given tile to the programmable
 *block in adjacent tiles
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
  const size_t& curr_tile_instance_id, const DeviceGrid& grids,
  const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& curr_fabric_tile_id,
  const size_t& sb_idx_in_curr_fabric_tile,
  const bool& compact_routing_hierarchy, const bool& name_module_using_index,
  const bool& verbose) {
  /* Skip those Switch blocks that do not exist */
  if (false == rr_gsb.is_sb_exist(rr_graph)) {
    return CMD_EXEC_SUCCESS;
  }

  vtr::Point<size_t> sink_tile_coord =
    fabric_tile.tile_coordinate(curr_fabric_tile_id);
  FabricTileId sink_unique_tile = fabric_tile.unique_tile(sink_tile_coord);
  vtr::Point<size_t> sink_sb_coord_in_unique_tile =
    fabric_tile.sb_coordinates(sink_unique_tile)[sb_idx_in_curr_fabric_tile];
  std::string sink_sb_instance_name_in_unique_tile =
    generate_switch_block_module_name(sink_sb_coord_in_unique_tile);
  if (name_module_using_index) {
    sink_sb_instance_name_in_unique_tile =
      generate_switch_block_module_name_using_index(sb_idx_in_curr_fabric_tile);
  }

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

      /* Find the source tile id, coordinate etc., which is required to find
       * source tile module and port Relationship between source tile and its
       * unique module Take an example:
       *
       *    grid_pin name should follow unique module [i0][j0] of
       * src_tile[x0][y0] sb_pin name should follow unique module [i1][j1] of
       * des_tile[x1][y1]
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
      FabricTileId src_fabric_tile_id =
        fabric_tile.find_tile_by_pb_coordinate(grid_coordinate);
      size_t pb_idx_in_src_fabric_tile =
        fabric_tile.find_pb_index_in_tile(src_fabric_tile_id, grid_coordinate);
      vtr::Point<size_t> src_tile_coord =
        fabric_tile.tile_coordinate(src_fabric_tile_id);
      vtr::Point<size_t> src_unique_tile_coord =
        fabric_tile.unique_tile_coordinate(src_fabric_tile_id);
      FabricTileId src_unique_tile = fabric_tile.unique_tile(src_tile_coord);
      vtr::Point<size_t> src_pb_coord_in_unique_tile =
        fabric_tile.pb_coordinates(src_unique_tile)[pb_idx_in_src_fabric_tile];
      std::string src_tile_module_name =
        generate_tile_module_name(src_unique_tile_coord);
      ModuleId src_tile_module =
        module_manager.find_module(src_tile_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(src_tile_module));
      size_t src_tile_instance =
        tile_instance_ids[src_tile_coord.x()][src_tile_coord.y()];

      size_t src_grid_pin_index = rr_graph.node_pin_num(
        rr_gsb.get_opin_node(side_manager.get_side(), inode));
      std::string src_grid_port_name =
        generate_grid_module_port_name_in_top_module(
          grids, grid_coordinate, src_grid_pin_index, vpr_device_annotation,
          rr_graph, rr_gsb.get_opin_node(side_manager.get_side(), inode));

      std::string src_grid_module_name =
        generate_grid_block_module_name_in_top_module(
          std::string(GRID_MODULE_NAME_PREFIX), grids,
          src_pb_coord_in_unique_tile);
      std::string src_tile_grid_port_name = generate_tile_module_port_name(
        src_grid_module_name, src_grid_port_name);
      VTR_LOGV(verbose, "Try to find port '%s' from tile[%lu][%lu]\n",
               src_tile_grid_port_name.c_str(), src_tile_coord.x(),
               src_tile_coord.y());
      ModulePortId src_tile_grid_port_id = module_manager.find_module_port(
        src_tile_module, src_tile_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           src_tile_module, src_tile_grid_port_id));
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

      std::string sink_tile_sb_port_name = generate_tile_module_port_name(
        sink_sb_instance_name_in_unique_tile, sink_sb_port_name);
      ModulePortId sink_tile_sb_port_id = module_manager.find_module_port(
        curr_tile_module, sink_tile_sb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           curr_tile_module, sink_tile_sb_port_id));
      BasicPort sink_tile_sb_port =
        module_manager.module_port(curr_tile_module, sink_tile_sb_port_id);

      /* Create nets */
      VTR_LOGV(
        verbose,
        "Build inter-tile nets between switch block '%s' in tile[%lu][%lu] and "
        "programmable block in tile[%lu][%lu]\n",
        sink_sb_instance_name_in_unique_tile.c_str(), sink_tile_coord.x(),
        sink_tile_coord.y(), src_tile_coord.x(), src_tile_coord.y());

      /* Source and sink port should match in size */
      VTR_ASSERT(src_tile_grid_port.get_width() ==
                 sink_tile_sb_port.get_width());

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
  const ModuleId& tile_module, const vtr::Matrix<size_t>& tile_instance_ids,
  const size_t& tile_instance_id, const DeviceGrid& grids,
  const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const FabricTile& fabric_tile,
  const FabricTileId& curr_fabric_tile_id, const e_rr_type& cb_type,
  const size_t& cb_idx_in_curr_fabric_tile,
  const bool& compact_routing_hierarchy, const bool& name_module_using_index,
  const bool& verbose) {
  vtr::Point<size_t> src_tile_coord =
    fabric_tile.tile_coordinate(curr_fabric_tile_id);
  FabricTileId src_unique_tile = fabric_tile.unique_tile(src_tile_coord);
  vtr::Point<size_t> src_cb_coord_in_unique_tile = fabric_tile.cb_coordinates(
    src_unique_tile, cb_type)[cb_idx_in_curr_fabric_tile];
  const RRGSB& src_cb_inst_rr_gsb =
    device_rr_gsb.get_gsb(src_cb_coord_in_unique_tile);
  std::string src_cb_instance_name_in_unique_tile =
    generate_connection_block_module_name(
      cb_type, src_cb_inst_rr_gsb.get_cb_coordinate(cb_type));
  if (name_module_using_index) {
    src_cb_instance_name_in_unique_tile =
      generate_connection_block_module_name_using_index(
        cb_type, cb_idx_in_curr_fabric_tile);
  }

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

      /* Collect source-related information */
      RRNodeId module_ipin_node = module_cb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> cb_src_port_coord(
        rr_graph.node_xlow(module_ipin_node),
        rr_graph.node_ylow(module_ipin_node));
      std::string src_cb_port_name = generate_cb_module_grid_port_name(
        cb_ipin_side, grids, vpr_device_annotation, rr_graph, module_ipin_node);
      std::string src_tile_cb_port_name = generate_tile_module_port_name(
        src_cb_instance_name_in_unique_tile, src_cb_port_name);
      VTR_LOGV(
        verbose, "Finding port '%s' from connection block in tile [%lu][%lu]\n",
        src_tile_cb_port_name.c_str(), src_tile_coord.x(), src_tile_coord.y());
      ModulePortId src_cb_port_id =
        module_manager.find_module_port(tile_module, src_tile_cb_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(tile_module,
                                                             src_cb_port_id));
      BasicPort src_cb_port =
        module_manager.module_port(tile_module, src_cb_port_id);

      FabricTileId sink_fabric_tile_id =
        fabric_tile.find_tile_by_pb_coordinate(grid_coordinate);
      size_t pb_idx_in_sink_fabric_tile =
        fabric_tile.find_pb_index_in_tile(sink_fabric_tile_id, grid_coordinate);
      vtr::Point<size_t> sink_tile_coord =
        fabric_tile.tile_coordinate(sink_fabric_tile_id);
      vtr::Point<size_t> sink_unique_tile_coord =
        fabric_tile.unique_tile_coordinate(sink_fabric_tile_id);
      FabricTileId sink_unique_tile = fabric_tile.unique_tile(sink_tile_coord);
      vtr::Point<size_t> sink_pb_coord_in_unique_tile =
        fabric_tile.pb_coordinates(
          sink_unique_tile)[pb_idx_in_sink_fabric_tile];
      std::string sink_tile_module_name =
        generate_tile_module_name(sink_unique_tile_coord);
      ModuleId sink_tile_module =
        module_manager.find_module(sink_tile_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sink_tile_module));
      size_t sink_tile_instance_id =
        tile_instance_ids[sink_tile_coord.x()][sink_tile_coord.y()];

      std::string sink_grid_module_name =
        generate_grid_block_module_name_in_top_module(
          std::string(GRID_MODULE_NAME_PREFIX), grids,
          sink_pb_coord_in_unique_tile);

      size_t sink_grid_pin_index = rr_graph.node_pin_num(instance_ipin_node);

      std::string sink_grid_port_name =
        generate_grid_module_port_name_in_top_module(
          grids, grid_coordinate, sink_grid_pin_index, vpr_device_annotation,
          rr_graph, rr_gsb.get_ipin_node(cb_ipin_side, inode));

      std::string sink_tile_grid_port_name = generate_tile_module_port_name(
        sink_grid_module_name, sink_grid_port_name);
      ModulePortId sink_grid_port_id = module_manager.find_module_port(
        sink_tile_module, sink_tile_grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           sink_tile_module, sink_grid_port_id));
      BasicPort sink_grid_port =
        module_manager.module_port(sink_tile_module, sink_grid_port_id);

      /* Create nets */
      /* Source and sink port should match in size */
      VTR_ASSERT(src_cb_port.get_width() == sink_grid_port.get_width());

      /* Create a net for each pin */
      for (size_t pin_id = 0; pin_id < src_cb_port.pins().size(); ++pin_id) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, top_module, tile_module, tile_instance_id,
          src_cb_port_id, src_cb_port.pins()[pin_id]);
        /* Configure the net sink */
        module_manager.add_module_net_sink(
          top_module, net, sink_tile_module, sink_tile_instance_id,
          sink_grid_port_id, sink_grid_port.pins()[pin_id]);
      }
      VTR_LOGV(verbose,
               "Built nets between connection block of tile[%lu][%lu] and grid "
               "block of tile[%lu][%lu]\n",
               src_tile_coord.x(), src_tile_coord.y(), sink_tile_coord.x(),
               sink_tile_coord.y());
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
  const ModuleId& tile_module, const vtr::Matrix<size_t>& tile_instance_ids,
  const size_t& tile_instance_id, const DeviceRRGSB& device_rr_gsb,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const FabricTile& fabric_tile, const FabricTileId& curr_fabric_tile_id,
  const size_t& sb_idx_in_curr_fabric_tile,
  const bool& compact_routing_hierarchy, const bool& name_module_using_index,
  const bool& verbose) {
  /* We could have two different coordinators, one is the instance, the other is
   * the module */
  vtr::Point<size_t> instance_sb_coordinate(rr_gsb.get_sb_x(),
                                            rr_gsb.get_sb_y());
  vtr::Point<size_t> module_gsb_sb_coordinate(rr_gsb.get_x(), rr_gsb.get_y());

  vtr::Point<size_t> sb_tile_coord =
    fabric_tile.tile_coordinate(curr_fabric_tile_id);
  FabricTileId sb_unique_tile = fabric_tile.unique_tile(sb_tile_coord);
  vtr::Point<size_t> sb_coord_in_unique_tile =
    fabric_tile.sb_coordinates(sb_unique_tile)[sb_idx_in_curr_fabric_tile];
  std::string sb_instance_name_in_unique_tile =
    generate_switch_block_module_name(sb_coord_in_unique_tile);
  if (name_module_using_index) {
    sb_instance_name_in_unique_tile =
      generate_switch_block_module_name_using_index(sb_idx_in_curr_fabric_tile);
  }

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
    e_rr_type cb_type =
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
    if (BOTTOM == side_manager.get_side() || LEFT == side_manager.get_side()) {
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }
    }

    if (RIGHT == side_manager.get_side() || TOP == side_manager.get_side()) {
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
    if (fabric_tile.cb_in_tile(curr_fabric_tile_id, cb_type,
                               instance_gsb_cb_coordinate)) {
      continue;
    }
    /* Collect cb tile information */
    FabricTileId cb_tile = fabric_tile.find_tile_by_cb_coordinate(
      cb_type, instance_gsb_cb_coordinate);
    vtr::Point<size_t> cb_tile_coord = fabric_tile.tile_coordinate(cb_tile);
    size_t cb_idx_in_cb_tile = fabric_tile.find_cb_index_in_tile(
      cb_tile, cb_type, instance_gsb_cb_coordinate);
    FabricTileId cb_unique_tile = fabric_tile.unique_tile(cb_tile_coord);
    vtr::Point<size_t> cb_unique_tile_coord =
      fabric_tile.tile_coordinate(cb_unique_tile);
    vtr::Point<size_t> cb_coord_in_cb_unique_tile =
      fabric_tile.cb_coordinates(cb_unique_tile, cb_type)[cb_idx_in_cb_tile];
    const RRGSB& unique_cb_rr_gsb =
      device_rr_gsb.get_gsb(cb_coord_in_cb_unique_tile);
    std::string cb_instance_name_in_unique_tile =
      generate_connection_block_module_name(
        cb_type, unique_cb_rr_gsb.get_cb_coordinate(cb_type));
    if (name_module_using_index) {
      cb_instance_name_in_unique_tile =
        generate_connection_block_module_name_using_index(cb_type,
                                                          cb_idx_in_cb_tile);
    }
    std::string cb_tile_module_name =
      generate_tile_module_name(cb_unique_tile_coord);
    ModuleId cb_tile_module = module_manager.find_module(cb_tile_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_tile_module));
    size_t cb_tile_instance =
      tile_instance_ids[cb_tile_coord.x()][cb_tile_coord.y()];

    /* Create nets */
    for (size_t itrack = 0;
         itrack < module_sb.get_chan_width(side_manager.get_side()); ++itrack) {
      std::string sb_port_name = generate_sb_module_track_port_name(
        rr_graph.node_type(
          module_sb.get_chan_node(side_manager.get_side(), itrack)),
        side_manager.get_side(),
        module_sb.get_chan_node_direction(side_manager.get_side(), itrack));
      /* Prepare SB-related port information */
      std::string sb_tile_sb_port_name = generate_tile_module_port_name(
        sb_instance_name_in_unique_tile, sb_port_name);
      ModulePortId sb_port_id =
        module_manager.find_module_port(tile_module, sb_tile_sb_port_name);
      VTR_ASSERT(true ==
                 module_manager.valid_module_port_id(tile_module, sb_port_id));
      BasicPort sb_port = module_manager.module_port(tile_module, sb_port_id);

      /* Prepare CB-related port information */
      PORTS cb_port_direction = OUT_PORT;
      /* The cb port direction should be opposite to the sb port !!! */
      if (OUT_PORT ==
          module_sb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        cb_port_direction = IN_PORT;
      } else {
        VTR_ASSERT(IN_PORT == module_sb.get_chan_node_direction(
                                side_manager.get_side(), itrack));
      }

      /* Upper CB port is required if the routing tracks are on the top or
       * right sides of the switch block, which indicated bottom and left
       * sides of the connection blocks
       */
      bool use_cb_upper_port =
        (TOP == side_manager.get_side()) || (RIGHT == side_manager.get_side());
      std::string cb_port_name = generate_cb_module_track_port_name(
        cb_type, cb_port_direction, use_cb_upper_port);
      std::string cb_tile_cb_port_name = generate_tile_module_port_name(
        cb_instance_name_in_unique_tile, cb_port_name);
      VTR_LOGV(
        verbose, "Finding port '%s' from connection block in tile [%lu][%lu]\n",
        cb_tile_cb_port_name.c_str(), cb_tile_coord.x(), cb_tile_coord.y());
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
      if (OUT_PORT ==
          module_sb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, top_module, tile_module, tile_instance_id, sb_port_id,
          itrack / 2);
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
      VTR_LOGV(verbose,
               "Built nets between switch block of tile[%lu][%lu] and "
               "connection block of tile[%lu][%lu]\n",
               sb_tile_coord.x(), sb_tile_coord.y(), cb_tile_coord.x(),
               cb_tile_coord.y());
    }
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Add module nets to connect the pins between tiles
 * To make it easy, this function will iterate over all the tiles, through which
 *we can obtain the coordinates of each programmable block (PB), connection
 *block (CB) and switch block (SB). With the coordinates, we can then trace the
 *connections between these blocks using the RRGSB data structure.
 *
 *  +--------+    +----------+
 *  | Tile   |--->| Tile     |
 *  | [x][y] |<---| [x+1][y] |
 *  +--------+    +----------+
 *
 * The inter-tile connections can be categorized into four types:
 * - PB-to-SB connections: We use the GSB to find the connections with a given
 *SB coordinate. Note that we only care the connections where the driver PB is
 *not in this tile.
 * - CB-to-PB connections: We use the GSB to find the connections with a given
 *CB coordinate. Note that we only care the connections where the sink PB is not
 *in this tile.
 * - SB-to-CB connections: We use the GSB to find the connections with a given
 *SB coordinate. Note that we only care the connections where the sink CB is not
 *in this tile.
 * - PB-to-PB connections: We use the tile direct data structure to find all the
 *connections. Note that we only care the connections where the driver PB is not
 *in this tile.
 *
 *******************************************************************/
static int add_top_module_nets_around_one_tile(
  ModuleManager& module_manager, const ModuleId& top_module,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const vtr::Matrix<size_t>& tile_instance_ids,
  const RRGraphView& rr_graph_view, const DeviceRRGSB& device_rr_gsb,
  const FabricTile& fabric_tile, const FabricTileId& curr_fabric_tile_id,
  const bool& name_module_using_index, const bool& verbose) {
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
  vtr::Point<size_t> tile_coord =
    fabric_tile.tile_coordinate(curr_fabric_tile_id);
  size_t tile_instance_id = tile_instance_ids[tile_coord.x()][tile_coord.y()];

  /* Get the submodule of Switch blocks one by one, build connections between sb
   * and pb */
  for (size_t isb = 0;
       isb < fabric_tile.sb_coordinates(curr_fabric_tile_id).size(); ++isb) {
    vtr::Point<size_t> sb_coord =
      fabric_tile.sb_coordinates(curr_fabric_tile_id)[isb];
    const RRGSB& rr_gsb = device_rr_gsb.get_gsb(sb_coord);
    status = build_top_module_tile_nets_between_sb_and_pb(
      module_manager, top_module, tile_module, tile_instance_ids,
      tile_instance_id, grids, vpr_device_annotation, device_rr_gsb,
      rr_graph_view, rr_gsb, fabric_tile, curr_fabric_tile_id, isb, true,
      name_module_using_index, verbose);
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  /* Get the submodule of connection blocks one by one, build connections
   * between cb and pb */
  for (e_rr_type cb_type : {e_rr_type::CHANX, e_rr_type::CHANY}) {
    for (size_t icb = 0;
         icb < fabric_tile.cb_coordinates(curr_fabric_tile_id, cb_type).size();
         ++icb) {
      vtr::Point<size_t> cb_coord =
        fabric_tile.cb_coordinates(curr_fabric_tile_id, cb_type)[icb];
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(cb_coord);
      status = build_top_module_tile_nets_between_cb_and_pb(
        module_manager, top_module, tile_module, tile_instance_ids,
        tile_instance_id, grids, vpr_device_annotation, device_rr_gsb,
        rr_graph_view, rr_gsb, fabric_tile, curr_fabric_tile_id, cb_type, icb,
        true, name_module_using_index, verbose);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }
  /* Get the submodule of connection blocks one by one, build connections
   * between sb and cb */
  for (size_t isb = 0;
       isb < fabric_tile.sb_coordinates(curr_fabric_tile_id).size(); ++isb) {
    vtr::Point<size_t> sb_coord =
      fabric_tile.sb_coordinates(curr_fabric_tile_id)[isb];
    const RRGSB& rr_gsb = device_rr_gsb.get_gsb(sb_coord);
    status = build_top_module_tile_nets_between_sb_and_cb(
      module_manager, top_module, tile_module, tile_instance_ids,
      tile_instance_id, device_rr_gsb, rr_graph_view, rr_gsb, fabric_tile,
      curr_fabric_tile_id, isb, true, name_module_using_index, verbose);
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Walk through each tile instance and add module nets to connect the pins
 *between tiles
 *******************************************************************/
static int add_top_module_nets_connect_tiles(
  ModuleManager& module_manager, const ModuleId& top_module,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const vtr::Matrix<size_t>& tile_instance_ids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const FabricTile& fabric_tile,
  const bool& name_module_using_index, const bool& verbose) {
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
        tile_instance_ids, rr_graph, device_rr_gsb, fabric_tile,
        curr_fabric_tile_id, name_module_using_index, verbose);
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
  const vtr::Matrix<size_t>& tile_instance_ids, const FabricTile& fabric_tile) {
  /* Ensure clean vectors to return */
  VTR_ASSERT(true ==
             module_manager
               .configurable_children(
                 top_module, ModuleManager::e_config_child_type::PHYSICAL)
               .empty());

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
      for (int ix = grids.width() - 1; ix >= 0; --ix) {
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
    vtr::Point<size_t> curr_tile_coord =
      fabric_tile.tile_coordinate(curr_fabric_tile_id);
    vtr::Point<size_t> unique_tile_coord =
      fabric_tile.unique_tile_coordinate(curr_fabric_tile_id);
    std::string tile_module_name = generate_tile_module_name(unique_tile_coord);
    ModuleId tile_module = module_manager.find_module(tile_module_name);
    VTR_ASSERT(module_manager.valid_module_id(tile_module));

    if (0 < find_module_num_config_bits(module_manager, tile_module,
                                        circuit_lib, sram_model,
                                        config_protocol.type())) {
      module_manager.add_configurable_child(
        top_module, tile_module,
        tile_instance_ids[curr_tile_coord.x()][curr_tile_coord.y()],
        ModuleManager::e_config_child_type::UNIFIED,
        vtr::Point<int>(curr_tile_coord.x(), curr_tile_coord.y()));
    }
  }

  /* Split memory modules into different regions */
  build_top_module_configurable_regions(module_manager, top_module,
                                        config_protocol);
}

/*********************************************************************
 * Generate an input port for routing multiplexer inside the tile
 * which is the middle output of a routing track
 ********************************************************************/
static ModulePinInfo find_tile_module_chan_port(
  const ModuleManager& module_manager, const ModuleId& tile_module,
  const vtr::Point<size_t>& cb_coord_in_tile, const size_t& cb_idx_in_tile,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_rr_type& cb_type,
  const RRNodeId& chan_rr_node, const bool& name_module_using_index) {
  ModulePinInfo input_port_info;
  /* Generate the input port object */
  switch (rr_graph.node_type(chan_rr_node)) {
    case e_rr_type::CHANX:
    case e_rr_type::CHANY: {
      /* Create port description for the routing track middle output */
      int chan_node_track_id =
        rr_gsb.get_cb_chan_node_index(cb_type, chan_rr_node);
      /* Create a port description for the middle output */
      std::string input_port_name = generate_cb_module_track_port_name(
        cb_type, IN_PORT, 0 == chan_node_track_id % 2);
      std::string cb_instance_name_in_tile =
        generate_connection_block_module_name(cb_type, cb_coord_in_tile);
      if (name_module_using_index) {
        cb_instance_name_in_tile =
          generate_connection_block_module_name_using_index(cb_type,
                                                            cb_idx_in_tile);
      }
      std::string tile_input_port_name = generate_tile_module_port_name(
        cb_instance_name_in_tile, input_port_name);
      /* Must find a valid port id in the Switch Block module */
      input_port_info.first =
        module_manager.find_module_port(tile_module, tile_input_port_name);
      input_port_info.second = chan_node_track_id / 2;
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           tile_module, input_port_info.first));
      break;
    }
    default: /* OPIN, SOURCE, IPIN, SINK are invalid*/
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n");
      exit(1);
  }

  return input_port_info;
}

/********************************************************************
 * Add nets between a global port and its sinks at an entry point of clock tree
 *******************************************************************/
static int build_top_module_global_net_from_tile_clock_arch_tree(
  ModuleManager& module_manager, const ModuleId& top_module,
  const ModulePortId& top_module_port, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb,
  const vtr::Matrix<size_t>& tile_instance_ids, const FabricTile& fabric_tile,
  const ClockNetwork& clk_ntwk, const std::string& clk_tree_name,
  const RRClockSpatialLookup& rr_clock_lookup,
  const bool& name_module_using_index) {
  int status = CMD_EXEC_SUCCESS;

  /* Ensure the clock arch tree name is valid */
  ClockTreeId clk_tree = clk_ntwk.find_tree(clk_tree_name);
  if (!clk_ntwk.valid_tree_id(clk_tree)) {
    VTR_LOG(
      "Fail to find a matched clock tree '%s' in the clock architecture "
      "definition",
      clk_tree_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Ensure the clock tree width matches the global port size */
  if (clk_ntwk.tree_width(clk_tree) !=
      module_manager.module_port(top_module, top_module_port).get_width()) {
    VTR_LOG(
      "Clock tree '%s' does not have the same width '%lu' as the port '%s' of "
      "FPGA top module",
      clk_tree_name.c_str(), clk_ntwk.tree_width(clk_tree),
      module_manager.module_port(top_module, top_module_port)
        .to_verilog_string()
        .c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  for (ClockTreePinId pin : clk_ntwk.pins(clk_tree)) {
    BasicPort src_port =
      module_manager.module_port(top_module, top_module_port);
    /* Add the module net */
    ModuleNetId net = create_module_source_pin_net(
      module_manager, top_module, top_module, 0, top_module_port,
      src_port.pins()[size_t(pin)]);
    VTR_ASSERT(ModuleNetId::INVALID() != net);

    for (ClockSpineId spine : clk_ntwk.tree_top_spines(clk_tree)) {
      vtr::Point<int> entry_point = clk_ntwk.spine_start_point(spine);
      Direction entry_dir = clk_ntwk.spine_direction(spine);
      e_rr_type entry_track_type = clk_ntwk.spine_track_type(spine);
      /* Find the routing resource node of the entry point */
      RRNodeId entry_rr_node = rr_clock_lookup.find_node(
        entry_point.x(), entry_point.y(), clk_tree, clk_ntwk.spine_level(spine),
        pin, entry_dir, false);

      /* Get the tile module and instance at the entry point */
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb_by_cb_coordinate(
        vtr::Point<size_t>(entry_point.x(), entry_point.y()));
      vtr::Point<size_t> cb_coord_in_tile = rr_gsb.get_sb_coordinate();
      FabricTileId curr_fabric_tile_id = fabric_tile.find_tile_by_cb_coordinate(
        entry_track_type, cb_coord_in_tile);
      vtr::Point<size_t> curr_fabric_tile_coord =
        fabric_tile.tile_coordinate(curr_fabric_tile_id);
      FabricTileId unique_fabric_tile_id =
        fabric_tile.unique_tile(curr_fabric_tile_coord);
      vtr::Point<size_t> unique_fabric_tile_coord =
        fabric_tile.tile_coordinate(unique_fabric_tile_id);

      ModuleId tile_module = module_manager.find_module(
        generate_tile_module_name(unique_fabric_tile_coord));
      size_t tile_instance = tile_instance_ids[curr_fabric_tile_coord.x()]
                                              [curr_fabric_tile_coord.y()];

      /* Find the port name */
      size_t cb_idx_in_curr_fabric_tile = fabric_tile.find_cb_index_in_tile(
        curr_fabric_tile_id, entry_track_type, cb_coord_in_tile);
      vtr::Point<size_t> cb_coord_in_unique_fabric_tile =
        fabric_tile.cb_coordinates(
          unique_fabric_tile_id, entry_track_type)[cb_idx_in_curr_fabric_tile];
      ModulePinInfo des_pin_info = find_tile_module_chan_port(
        module_manager, tile_module, cb_coord_in_unique_fabric_tile,
        cb_idx_in_curr_fabric_tile, rr_graph, rr_gsb, entry_track_type,
        entry_rr_node, name_module_using_index);

      /* Configure the net sink */
      BasicPort sink_port =
        module_manager.module_port(tile_module, des_pin_info.first);
      module_manager.add_module_net_sink(top_module, net, tile_module,
                                         tile_instance, des_pin_info.first,
                                         sink_port.pins()[des_pin_info.second]);
    }
  }

  return status;
}

/********************************************************************
 * Add global port connection for a given port of a physical tile
 * that are defined as global in tile annotation
 *******************************************************************/
static int build_top_module_global_net_for_given_tile_module(
  ModuleManager& module_manager, const ModuleId& top_module,
  const ModulePortId& top_module_port, const TileAnnotation& tile_annotation,
  const TileGlobalPortId& tile_global_port,
  const BasicPort& tile_port_to_connect,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const vtr::Point<size_t>& grid_coordinate,
  const e_side& border_side, const vtr::Matrix<size_t>& tile_instance_ids,
  const FabricTile& fabric_tile, const bool& perimeter_cb) {
  /* Get the tile module and instance */
  FabricTileId curr_fabric_tile_id =
    fabric_tile.find_tile_by_pb_coordinate(grid_coordinate);
  vtr::Point<size_t> curr_fabric_tile_coord =
    fabric_tile.tile_coordinate(curr_fabric_tile_id);
  FabricTileId unique_fabric_tile_id =
    fabric_tile.unique_tile(curr_fabric_tile_coord);
  vtr::Point<size_t> unique_fabric_tile_coord =
    fabric_tile.tile_coordinate(unique_fabric_tile_id);
  std::string tile_module_name =
    generate_tile_module_name(unique_fabric_tile_coord);
  ModuleId tile_module = module_manager.find_module(tile_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(tile_module));
  size_t tile_instance =
    tile_instance_ids[curr_fabric_tile_coord.x()][curr_fabric_tile_coord.y()];

  /* Get the grid coordinate in the context of the tile */
  size_t pb_idx_in_curr_fabric_tile =
    fabric_tile.find_pb_index_in_tile(curr_fabric_tile_id, grid_coordinate);
  vtr::Point<size_t> pb_coord_in_unique_fabric_tile =
    fabric_tile.pb_coordinates(
      unique_fabric_tile_id)[pb_idx_in_curr_fabric_tile];

  t_physical_tile_type_ptr physical_tile = grids.get_physical_type(
    t_physical_tile_loc(grid_coordinate.x(), grid_coordinate.y(), layer));
  /* Find the module name for this type of grid */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_instance_name =
    generate_grid_block_module_name_in_top_module(
      grid_module_name_prefix, grids, pb_coord_in_unique_fabric_tile);
  /* Find the source port at the top-level module */
  BasicPort src_port = module_manager.module_port(top_module, top_module_port);

  /* Walk through each instance considering the unique sub tile and capacity
   * range, each instance may have an independent pin to be driven by a global
   * net! */
  for (const t_sub_tile& sub_tile : physical_tile->sub_tiles) {
    VTR_ASSERT(1 == sub_tile.equivalent_sites.size());
    int grid_pin_start_index = physical_tile->num_pins;
    t_physical_tile_port physical_tile_port;
    physical_tile_port.num_pins = 0;

    /* Count the total number of pins for this type of sub tile */
    int sub_tile_num_pins = sub_tile.num_phy_pins / sub_tile.capacity.total();

    /* For each instance of the same sub tile type, find the port of the grid
     * module according to the tile annotation A tile may consist of multiple
     * subtile, connect to all the pins from sub tiles */
    for (int subtile_index = sub_tile.capacity.low;
         subtile_index <= sub_tile.capacity.high; subtile_index++) {
      for (const t_physical_tile_port& tile_port : sub_tile.ports) {
        if (std::string(tile_port.name) == tile_port_to_connect.get_name()) {
          BasicPort ref_tile_port(tile_port.name, tile_port.num_pins);
          /* Port size must match!!! */
          if (false == ref_tile_port.contained(tile_port_to_connect)) {
            VTR_LOG_ERROR(
              "Tile annotation '%s' port '%s[%lu:%lu]' is out of the range of "
              "physical tile port '%s[%lu:%lu]'!",
              tile_annotation.global_port_name(tile_global_port).c_str(),
              tile_port_to_connect.get_name().c_str(),
              tile_port_to_connect.get_lsb(), tile_port_to_connect.get_msb(),
              ref_tile_port.get_name().c_str(), ref_tile_port.get_lsb(),
              ref_tile_port.get_msb());
            return CMD_EXEC_FATAL_ERROR;
          }
          grid_pin_start_index =
            sub_tile.sub_tile_to_tile_pin_indices
              [(subtile_index - sub_tile.capacity.low) * sub_tile_num_pins +
               tile_port.absolute_first_pin_index];
          physical_tile_port = tile_port;
          break;
        }
      }
      /* Ensure the pin index is valid */
      VTR_ASSERT(grid_pin_start_index < physical_tile->num_pins);
      /* Ensure port width is in range */
      VTR_ASSERT(src_port.get_width() == tile_port_to_connect.get_width());

      /* Create a pin id mapping between the source port (top module) and the
       * sink port (grid module) */
      std::map<size_t, size_t> sink2src_pin_map;
      for (size_t ipin = 0; ipin < tile_port_to_connect.get_width(); ++ipin) {
        size_t sink_pin = tile_port_to_connect.pins()[ipin];
        size_t src_pin = src_port.pins()[ipin];
        sink2src_pin_map[sink_pin] = src_pin;
      }

      /* Create the connections */
      for (size_t pin_id = tile_port_to_connect.get_lsb();
           pin_id < tile_port_to_connect.get_msb() + 1; ++pin_id) {
        int grid_pin_index = grid_pin_start_index + pin_id;
        /* Find the module pin */
        size_t grid_pin_width = physical_tile->pin_width_offset[grid_pin_index];
        size_t grid_pin_height =
          physical_tile->pin_height_offset[grid_pin_index];
        std::vector<e_side> pin_sides = find_physical_tile_pin_side(
          physical_tile, grid_pin_index, border_side, perimeter_cb);

        BasicPort grid_pin_info =
          vpr_device_annotation.physical_tile_pin_port_info(physical_tile,
                                                            grid_pin_index);
        VTR_ASSERT(true == grid_pin_info.is_valid());

        /* Build nets */
        for (const e_side& pin_side : pin_sides) {
          std::string grid_port_name =
            generate_grid_port_name(grid_pin_width, grid_pin_height,
                                    subtile_index, pin_side, grid_pin_info);
          if (tile_annotation.is_tile_port_to_merge(
                std::string(physical_tile->name), grid_pin_info.get_name())) {
            if (subtile_index == 0) {
              grid_port_name =
                generate_grid_port_name(0, 0, 0, TOP, grid_pin_info);
            } else {
              continue;
            }
          }
          std::string tile_grid_port_name =
            generate_tile_module_port_name(grid_instance_name, grid_port_name);
          ModulePortId tile_grid_port_id =
            module_manager.find_module_port(tile_module, tile_grid_port_name);
          VTR_ASSERT(true == module_manager.valid_module_port_id(
                               tile_module, tile_grid_port_id));

          VTR_ASSERT(1 ==
                     module_manager.module_port(tile_module, tile_grid_port_id)
                       .get_width());

          ModuleNetId net = create_module_source_pin_net(
            module_manager, top_module, top_module, 0, top_module_port,
            src_port.pins()[sink2src_pin_map[pin_id]]);
          VTR_ASSERT(ModuleNetId::INVALID() != net);

          /* Configure the net sink */
          BasicPort sink_port =
            module_manager.module_port(tile_module, tile_grid_port_id);
          module_manager.add_module_net_sink(top_module, net, tile_module,
                                             tile_instance, tile_grid_port_id,
                                             sink_port.pins()[0]);
        }
      }
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Add nets between a global port and its sinks at each tile modules
 *******************************************************************/
static int build_top_module_global_net_from_tile_modules(
  ModuleManager& module_manager, const ModuleId& top_module,
  const ModulePortId& top_module_port, const TileAnnotation& tile_annotation,
  const TileGlobalPortId& tile_global_port,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const vtr::Matrix<size_t>& tile_instance_ids,
  const FabricTile& fabric_tile, const bool& perimeter_cb) {
  int status = CMD_EXEC_SUCCESS;

  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_grid_coordinates(grids);

  for (size_t tile_info_id = 0;
       tile_info_id <
       tile_annotation.global_port_tile_names(tile_global_port).size();
       ++tile_info_id) {
    std::string tile_name =
      tile_annotation.global_port_tile_names(tile_global_port)[tile_info_id];
    BasicPort tile_port =
      tile_annotation.global_port_tile_ports(tile_global_port)[tile_info_id];
    /* Find the coordinates for the wanted tiles */
    vtr::Point<size_t> start_coord(1, 1);
    vtr::Point<size_t> end_coord(grids.width() - 1, grids.height() - 1);
    vtr::Point<size_t> range = tile_annotation.global_port_tile_coordinates(
      tile_global_port)[tile_info_id];
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
      VTR_LOG_ERROR(
        "Coordinate (%lu, %lu) in tile annotation for tile '%s' is out of "
        "range (%lu:%lu, %lu:%lu)!",
        range.x(), range.y(), tile_name.c_str(), start_coord.x(), end_coord.x(),
        start_coord.y(), end_coord.y());
      return CMD_EXEC_FATAL_ERROR;
    }

    /* Spot the port from child modules from core grids */
    for (size_t ix = start_coord.x(); ix < end_coord.x(); ++ix) {
      for (size_t iy = start_coord.y(); iy < end_coord.y(); ++iy) {
        t_physical_tile_loc phy_tile_loc(ix, iy, layer);
        t_physical_tile_type_ptr phy_tile_type =
          grids.get_physical_type(phy_tile_loc);
        /* Bypass EMPTY tiles */
        if (true == is_empty_type(phy_tile_type)) {
          continue;
        }
        /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
        if ((0 < grids.get_width_offset(phy_tile_loc)) ||
            (0 < grids.get_height_offset(phy_tile_loc))) {
          continue;
        }

        /* Bypass the tiles whose names do not match */
        if (std::string(phy_tile_type->name) != tile_name) {
          continue;
        }

        /* Create nets and finish connection build-up */
        status = build_top_module_global_net_for_given_tile_module(
          module_manager, top_module, top_module_port, tile_annotation,
          tile_global_port, tile_port, vpr_device_annotation, grids, layer,
          vtr::Point<size_t>(ix, iy), NUM_2D_SIDES, tile_instance_ids,
          fabric_tile, perimeter_cb);
        if (CMD_EXEC_FATAL_ERROR == status) {
          return status;
        }
      }
    }

    /* Walk through all the grids on the perimeter, which are I/O grids */
    for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
      for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
        t_physical_tile_loc phy_tile_loc(io_coordinate.x(), io_coordinate.y(),
                                         layer);
        t_physical_tile_type_ptr phy_tile_type =
          grids.get_physical_type(phy_tile_loc);
        /* Bypass EMPTY grid */
        if (true == is_empty_type(phy_tile_type)) {
          continue;
        }

        /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
        if ((0 < grids.get_width_offset(phy_tile_loc)) ||
            (0 < grids.get_height_offset(phy_tile_loc))) {
          continue;
        }

        /* Bypass the tiles whose names do not match */
        if (std::string(phy_tile_type->name) != tile_name) {
          continue;
        }

        /* Check if the coordinate satisfy the tile coordinate defintion
         * - Bypass if the x is a specific number (!= -1), and io_coordinate
         * is different
         * - Bypass if the y is a specific number (!= -1), and io_coordinate
         * is different
         */
        if ((size_t(-1) != range.x()) && (range.x() != io_coordinate.x())) {
          continue;
        }
        if ((size_t(-1) != range.y()) && (range.y() != io_coordinate.y())) {
          continue;
        }

        /* Create nets and finish connection build-up */
        status = build_top_module_global_net_for_given_tile_module(
          module_manager, top_module, top_module_port, tile_annotation,
          tile_global_port, tile_port, vpr_device_annotation, grids, layer,
          io_coordinate, io_side, tile_instance_ids, fabric_tile, perimeter_cb);
        if (CMD_EXEC_FATAL_ERROR == status) {
          return status;
        }
      }
    }
  }

  return status;
}

/********************************************************************
 * Add global ports from tile ports that are defined as global in tile
 *annotation
 *******************************************************************/
static int add_top_module_global_ports_from_tile_modules(
  ModuleManager& module_manager, const ModuleId& top_module,
  const TileAnnotation& tile_annotation,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb,
  const vtr::Matrix<size_t>& tile_instance_ids, const FabricTile& fabric_tile,
  const ClockNetwork& clk_ntwk, const RRClockSpatialLookup& rr_clock_lookup,
  const bool& perimeter_cb, const bool& name_module_using_index) {
  int status = CMD_EXEC_SUCCESS;

  /* Add the global ports which are NOT yet added to the top-level module
   * (in different names than the global ports defined in circuit library
   */
  std::vector<BasicPort> global_ports_to_add;
  for (const TileGlobalPortId& tile_global_port :
       tile_annotation.global_ports()) {
    ModulePortId module_port = module_manager.find_module_port(
      top_module, tile_annotation.global_port_name(tile_global_port));
    /* The global port size is derived from the maximum port size among all the
     * tile port defintion */
    if (ModulePortId::INVALID() == module_port) {
      BasicPort global_port_to_add;
      global_port_to_add.set_name(
        tile_annotation.global_port_name(tile_global_port));
      if (tile_annotation.global_port_thru_dedicated_network(
            tile_global_port)) {
        std::string clk_tree_name =
          tile_annotation.global_port_clock_arch_tree_name(tile_global_port);
        ClockTreeId clk_tree = clk_ntwk.find_tree(clk_tree_name);
        global_port_to_add.set_width(clk_ntwk.tree_width(clk_tree));
      } else {
        size_t max_port_size = 0;
        for (const BasicPort& tile_port :
             tile_annotation.global_port_tile_ports(tile_global_port)) {
          max_port_size = std::max(tile_port.get_width(), max_port_size);
        }
        global_port_to_add.set_width(max_port_size);
      }
      global_ports_to_add.push_back(global_port_to_add);
    }
  }

  for (const BasicPort& global_port_to_add : global_ports_to_add) {
    module_manager.add_port(top_module, global_port_to_add,
                            ModuleManager::MODULE_GLOBAL_PORT);
  }

  /* Add module nets */
  for (const TileGlobalPortId& tile_global_port :
       tile_annotation.global_ports()) {
    /* Must found one valid port! */
    ModulePortId top_module_port = module_manager.find_module_port(
      top_module, tile_annotation.global_port_name(tile_global_port));
    VTR_ASSERT(ModulePortId::INVALID() != top_module_port);

    /* There are two cases when building the nets:
     * - If the net will go through a dedicated clock tree network, the net will
     * drive an input of a routing block
     * - If the net will be directly wired to tiles, the net will drive an input
     * of a tile
     */
    if (!tile_annotation.global_port_clock_arch_tree_name(tile_global_port)
           .empty()) {
      status = build_top_module_global_net_from_tile_clock_arch_tree(
        module_manager, top_module, top_module_port, rr_graph, device_rr_gsb,
        tile_instance_ids, fabric_tile, clk_ntwk,
        tile_annotation.global_port_clock_arch_tree_name(tile_global_port),
        rr_clock_lookup, name_module_using_index);
    } else {
      status = build_top_module_global_net_from_tile_modules(
        module_manager, top_module, top_module_port, tile_annotation,
        tile_global_port, vpr_device_annotation, grids, layer,
        tile_instance_ids, fabric_tile, perimeter_cb);
    }
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }
  }
  return status;
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
static void add_module_nets_connect_tile_direct_connection(
  ModuleManager& module_manager, const ModuleId& top_module,
  const CircuitLibrary& circuit_lib,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const vtr::Matrix<size_t>& tile_instance_ids,
  const FabricTile& fabric_tile, const TileDirect& tile_direct,
  const TileDirectId& tile_direct_id, const ArchDirect& arch_direct) {
  vtr::Point<size_t> device_size(grids.width(), grids.height());
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);

  /* Find the module name of source clb */
  vtr::Point<size_t> src_clb_coord =
    tile_direct.from_tile_coordinate(tile_direct_id);
  FabricTileId src_tile_id =
    fabric_tile.find_tile_by_pb_coordinate(src_clb_coord);
  vtr::Point<size_t> src_tile_coord = fabric_tile.tile_coordinate(src_tile_id);
  FabricTileId src_unique_tile_id = fabric_tile.unique_tile(src_tile_coord);
  vtr::Point<size_t> src_unique_tile_coord =
    fabric_tile.tile_coordinate(src_unique_tile_id);
  std::string src_module_name =
    generate_tile_module_name(src_unique_tile_coord);
  ModuleId src_tile_module = module_manager.find_module(src_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(src_tile_module));
  /* Record the instance id */
  size_t src_tile_instance =
    tile_instance_ids[src_tile_coord.x()][src_tile_coord.y()];
  /* Grid instance name in the unique tile */
  size_t pb_idx_in_src_tile =
    fabric_tile.find_pb_index_in_tile(src_tile_id, src_clb_coord);
  vtr::Point<size_t> pb_coord_in_unique_src_tile =
    fabric_tile.pb_coordinates(src_unique_tile_id)[pb_idx_in_src_tile];
  std::string src_grid_instance_name =
    generate_grid_block_module_name_in_top_module(
      grid_module_name_prefix, grids, pb_coord_in_unique_src_tile);

  /* Find the module name of sink clb */
  vtr::Point<size_t> des_clb_coord =
    tile_direct.to_tile_coordinate(tile_direct_id);
  FabricTileId des_tile_id =
    fabric_tile.find_tile_by_pb_coordinate(des_clb_coord);
  vtr::Point<size_t> des_tile_coord = fabric_tile.tile_coordinate(des_tile_id);
  FabricTileId des_unique_tile_id = fabric_tile.unique_tile(des_tile_coord);
  vtr::Point<size_t> des_unique_tile_coord =
    fabric_tile.tile_coordinate(des_unique_tile_id);
  std::string des_module_name =
    generate_tile_module_name(des_unique_tile_coord);
  ModuleId des_tile_module = module_manager.find_module(des_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(des_tile_module));
  /* Record the instance id */
  size_t des_tile_instance =
    tile_instance_ids[des_tile_coord.x()][des_tile_coord.y()];
  /* Grid instance name in the unique tile */
  size_t pb_idx_in_des_tile =
    fabric_tile.find_pb_index_in_tile(des_tile_id, des_clb_coord);
  vtr::Point<size_t> pb_coord_in_unique_des_tile =
    fabric_tile.pb_coordinates(des_unique_tile_id)[pb_idx_in_des_tile];
  std::string des_grid_instance_name =
    generate_grid_block_module_name_in_top_module(
      grid_module_name_prefix, grids, pb_coord_in_unique_des_tile);

  /* Find the module id of a direct connection module */
  CircuitModelId direct_circuit_model =
    arch_direct.circuit_model(tile_direct.arch_direct(tile_direct_id));
  std::string direct_module_name = circuit_lib.model_name(direct_circuit_model);
  ModuleId direct_module = module_manager.find_module(direct_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(direct_module));

  /* Find inputs and outputs of the direct circuit module */
  std::vector<CircuitPortId> direct_input_ports =
    circuit_lib.model_ports_by_type(direct_circuit_model,
                                    CIRCUIT_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == direct_input_ports.size());
  ModulePortId direct_input_port_id = module_manager.find_module_port(
    direct_module, circuit_lib.port_prefix(direct_input_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(direct_module,
                                                         direct_input_port_id));
  VTR_ASSERT(1 ==
             module_manager.module_port(direct_module, direct_input_port_id)
               .get_width());

  std::vector<CircuitPortId> direct_output_ports =
    circuit_lib.model_ports_by_type(direct_circuit_model,
                                    CIRCUIT_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == direct_output_ports.size());
  ModulePortId direct_output_port_id = module_manager.find_module_port(
    direct_module, circuit_lib.port_prefix(direct_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(
                       direct_module, direct_output_port_id));
  VTR_ASSERT(1 ==
             module_manager.module_port(direct_module, direct_output_port_id)
               .get_width());

  /* Generate the pin name of source port/pin in the grid */
  e_side src_pin_grid_side = tile_direct.from_tile_side(tile_direct_id);
  size_t src_tile_pin = tile_direct.from_tile_pin(tile_direct_id);

  t_physical_tile_loc src_tile_loc(src_clb_coord.x(), src_clb_coord.y(), layer);
  t_physical_tile_type_ptr src_grid_type_descriptor =
    grids.get_physical_type(src_tile_loc);
  size_t src_pin_width =
    src_grid_type_descriptor->pin_width_offset[src_tile_pin];
  size_t src_pin_height =
    src_grid_type_descriptor->pin_height_offset[src_tile_pin];

  BasicPort src_pin_info = vpr_device_annotation.physical_tile_pin_port_info(
    src_grid_type_descriptor, src_tile_pin);
  VTR_ASSERT(true == src_pin_info.is_valid());
  int src_subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
    src_grid_type_descriptor, src_tile_pin);
  VTR_ASSERT(UNDEFINED != src_subtile_index &&
             src_subtile_index < src_grid_type_descriptor->capacity);
  std::string src_port_name =
    generate_grid_port_name(src_pin_width, src_pin_height, src_subtile_index,
                            src_pin_grid_side, src_pin_info);
  src_port_name =
    generate_tile_module_port_name(src_grid_instance_name, src_port_name);
  ModulePortId src_port_id =
    module_manager.find_module_port(src_tile_module, src_port_name);
  if (true !=
      module_manager.valid_module_port_id(src_tile_module, src_port_id)) {
    VTR_LOG_ERROR("Fail to find port '%s[%lu][%lu].%s'\n",
                  src_module_name.c_str(), src_tile_coord.x(),
                  src_tile_coord.y(), src_port_name.c_str());
  }
  VTR_ASSERT(true ==
             module_manager.valid_module_port_id(src_tile_module, src_port_id));
  VTR_ASSERT(
    1 == module_manager.module_port(src_tile_module, src_port_id).get_width());

  /* Generate the pin name of sink port/pin in the grid */
  e_side sink_pin_grid_side = tile_direct.to_tile_side(tile_direct_id);
  size_t sink_tile_pin = tile_direct.to_tile_pin(tile_direct_id);

  t_physical_tile_loc des_tile_loc(des_clb_coord.x(), des_clb_coord.y(), layer);
  t_physical_tile_type_ptr sink_grid_type_descriptor =
    grids.get_physical_type(des_tile_loc);
  size_t sink_pin_width =
    sink_grid_type_descriptor->pin_width_offset[src_tile_pin];
  size_t sink_pin_height =
    sink_grid_type_descriptor->pin_height_offset[src_tile_pin];

  BasicPort sink_pin_info = vpr_device_annotation.physical_tile_pin_port_info(
    sink_grid_type_descriptor, sink_tile_pin);
  VTR_ASSERT(true == sink_pin_info.is_valid());
  int sink_subtile_index =
    vpr_device_annotation.physical_tile_pin_subtile_index(
      sink_grid_type_descriptor, sink_tile_pin);
  VTR_ASSERT(UNDEFINED != src_subtile_index &&
             src_subtile_index < sink_grid_type_descriptor->capacity);
  std::string sink_port_name =
    generate_grid_port_name(sink_pin_width, sink_pin_height, sink_subtile_index,
                            sink_pin_grid_side, sink_pin_info);
  sink_port_name =
    generate_tile_module_port_name(des_grid_instance_name, sink_port_name);
  ModulePortId sink_port_id =
    module_manager.find_module_port(des_tile_module, sink_port_name);
  VTR_ASSERT(
    true == module_manager.valid_module_port_id(des_tile_module, sink_port_id));
  VTR_ASSERT(
    1 == module_manager.module_port(des_tile_module, sink_port_id).get_width());

  /* Add a submodule of direct connection module to the top-level module */
  size_t direct_instance_id =
    module_manager.num_instance(top_module, direct_module);
  module_manager.add_child_module(top_module, direct_module, false);

  /* Create the 1st module net */
  ModuleNetId net_direct_src = module_manager.create_module_net(top_module);
  /* Connect the wire between src_pin of clb and direct_instance input*/
  module_manager.add_module_net_source(top_module, net_direct_src,
                                       src_tile_module, src_tile_instance,
                                       src_port_id, 0);
  module_manager.add_module_net_sink(top_module, net_direct_src, direct_module,
                                     direct_instance_id, direct_input_port_id,
                                     0);

  /* Create the 2nd module net
   * Connect the wire between direct_instance output and sink_pin of clb
   */
  ModuleNetId net_direct_sink =
    create_module_source_pin_net(module_manager, top_module, direct_module,
                                 direct_instance_id, direct_output_port_id, 0);
  module_manager.add_module_net_sink(top_module, net_direct_sink,
                                     des_tile_module, des_tile_instance,
                                     sink_port_id, 0);
}

/********************************************************************
 * Add module net of clb-to-clb direct connections to module manager
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *******************************************************************/
static void add_top_module_nets_connect_tile_direct_connections(
  ModuleManager& module_manager, const ModuleId& top_module,
  const CircuitLibrary& circuit_lib,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const vtr::Matrix<size_t>& tile_instance_ids,
  const FabricTile& fabric_tile, const TileDirect& tile_direct,
  const ArchDirect& arch_direct) {
  vtr::ScopedStartFinishTimer timer(
    "Add module nets for inter-tile connections");

  for (const TileDirectId& tile_direct_id : tile_direct.directs()) {
    add_module_nets_connect_tile_direct_connection(
      module_manager, top_module, circuit_lib, vpr_device_annotation, grids,
      layer, tile_instance_ids, fabric_tile, tile_direct, tile_direct_id,
      arch_direct);
  }
}

/********************************************************************
 * Add the tile-level instances to the top module of FPGA fabric
 * and build connects between them
 *******************************************************************/
int build_top_module_tile_child_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const ClockNetwork& clk_ntwk,
  const RRClockSpatialLookup& rr_clock_lookup,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const TileAnnotation& tile_annotation,
  const RRGraphView& rr_graph, const DeviceRRGSB& device_rr_gsb,
  const TileDirect& tile_direct, const ArchDirect& arch_direct,
  const FabricTile& fabric_tile, const ConfigProtocol& config_protocol,
  const CircuitModelId& sram_model, const FabricKey& fabric_key,
  const bool& group_config_block, const bool& name_module_using_index,
  const bool& perimeter_cb, const bool& frame_view, const bool& verbose) {
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

  /* Build the nets between tiles */
  if (false == frame_view) {
    /* Reserve nets to be memory efficient */
    reserve_module_manager_module_nets(module_manager, top_module);
    /* Regular nets between tiles */
    status = add_top_module_nets_connect_tiles(
      module_manager, top_module, vpr_device_annotation, grids,
      tile_instance_ids, rr_graph, device_rr_gsb, fabric_tile,
      name_module_using_index, verbose);
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
    /* TODO: Inter-tile direct connections */
    add_top_module_nets_connect_tile_direct_connections(
      module_manager, top_module, circuit_lib, vpr_device_annotation, grids,
      layer, tile_instance_ids, fabric_tile, tile_direct, arch_direct);
  }

  /* Add global ports from tile modules: how to connect to clock architecture
   * and the global port from tile annotation
   */
  status = add_top_module_global_ports_from_tile_modules(
    module_manager, top_module, tile_annotation, vpr_device_annotation, grids,
    layer, rr_graph, device_rr_gsb, tile_instance_ids, fabric_tile, clk_ntwk,
    rr_clock_lookup, perimeter_cb, name_module_using_index);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }

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
      module_manager, circuit_lib, config_protocol, fabric_key,
      group_config_block);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
