#include "append_clock_rr_graph.h"

#include "command_exit_codes.h"
#include "openfpga_physical_tile_utils.h"
#include "rr_graph_builder_utils.h"
#include "rr_graph_cost.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Estimate the number of clock nodes to be added for a given tile and clock
 *structure For each layer/level of a clock network, we need
 * - the clock nodes are paired in INC and DEC directions
 * - the number of clock nodes depend on the width of clock tree (number of
 *clock signals)
 * - Note that some layer only need CHANX or CHANY clock nodes since clock nodes
 *cannot make turns in the same layer. For instance
 *   - Layer 0: CHANX
 *   - Layer 1: CHANY
 *   - Layer 2: CHANX
 *******************************************************************/
static size_t estimate_clock_rr_graph_num_chan_nodes(
  const ClockNetwork& clk_ntwk, const e_rr_type& chan_type) {
  size_t num_nodes = 0;

  for (auto itree : clk_ntwk.trees()) {
    for (auto ilvl : clk_ntwk.levels(itree)) {
      num_nodes += clk_ntwk.num_tracks(itree, ilvl, chan_type);
    }
  }

  return num_nodes;
}

/********************************************************************
 * Estimate the number of clock nodes to be added.
 * Clock nodes are required by X-direction and Y-direction connection blocks
 * which are in the type of CHANX and CHANY
 * Note that switch blocks do not require any new nodes but new edges
 *******************************************************************/
static size_t estimate_clock_rr_graph_num_nodes(const DeviceGrid& grids,
                                                const size_t& layer,
                                                const bool& perimeter_cb,
                                                const bool& through_channel,
                                                const ClockNetwork& clk_ntwk) {
  size_t num_nodes = 0;
  vtr::Rect<size_t> chanx_bb(1, 0, grids.width() - 1, grids.height() - 1);
  if (perimeter_cb) {
    chanx_bb.set_xmin(0);
    chanx_bb.set_xmax(grids.width());
    chanx_bb.set_ymin(0);
    chanx_bb.set_ymax(grids.height() - 1);
  }
  /* Check the number of CHANX nodes required */
  for (size_t iy = chanx_bb.ymin(); iy < chanx_bb.ymax(); ++iy) {
    for (size_t ix = chanx_bb.xmin(); ix < chanx_bb.xmax(); ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channels are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chanx_exist(grids, layer, chanx_coord, perimeter_cb))) {
        continue;
      }
      /* Estimate the routing tracks required by clock routing only */
      num_nodes +=
        estimate_clock_rr_graph_num_chan_nodes(clk_ntwk, e_rr_type::CHANX);
    }
  }

  vtr::Rect<size_t> chany_bb(0, 1, grids.width() - 1, grids.height() - 1);
  if (perimeter_cb) {
    chany_bb.set_xmin(0);
    chany_bb.set_xmax(grids.width() - 1);
    chany_bb.set_ymin(0);
    chany_bb.set_ymax(grids.height());
  }

  for (size_t ix = chany_bb.xmin(); ix < chany_bb.xmax(); ++ix) {
    for (size_t iy = chany_bb.ymin(); iy < chany_bb.ymax(); ++iy) {
      vtr::Point<size_t> chany_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channel are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chany_exist(grids, layer, chany_coord, perimeter_cb))) {
        continue;
      }
      /* Estimate the routing tracks required by clock routing only */
      num_nodes +=
        estimate_clock_rr_graph_num_chan_nodes(clk_ntwk, e_rr_type::CHANY);
    }
  }

  return num_nodes;
}

/********************************************************************
 * Add clock nodes to a routing resource graph
 * For each tree and level of the tree, add a number of clock nodes
 * with direction, ptc and coordinates etc.
 *******************************************************************/
static void add_rr_graph_block_clock_nodes(
  RRGraphBuilder& rr_graph_builder, RRClockSpatialLookup& clk_rr_lookup,
  const RRGraphView& rr_graph_view, const ClockNetwork& clk_ntwk,
  const size_t& layer, const vtr::Point<size_t> chan_coord,
  const e_rr_type& chan_type, const int& cost_index_offset,
  const bool& verbose) {
  size_t orig_chan_width =
    rr_graph_view.node_lookup()
      .find_channel_nodes(layer, chan_coord.x(), chan_coord.y(), chan_type)
      .size();
  size_t curr_node_ptc = orig_chan_width;

  for (auto itree : clk_ntwk.trees()) {
    for (auto ilvl : clk_ntwk.levels(itree)) {
      /* As we want to keep uni-directional wires, clock routing tracks have to
       * be in pairs. Therefore, always add clock routing tracks in pair, even
       * one of them is not required
       */
      size_t num_pins = 0;
      bool require_complementary_pins = false;
      for (auto node_dir : {Direction::INC, Direction::DEC}) {
        if (0 == clk_ntwk.pins(itree, ilvl, chan_type, node_dir).size()) {
          require_complementary_pins = true;
        }
        num_pins += clk_ntwk.pins(itree, ilvl, chan_type, node_dir).size();
      }
      if (require_complementary_pins) {
        num_pins = 2 * num_pins;
      }
      for (size_t ipin = 0; ipin < num_pins / 2; ++ipin) {
        for (auto node_dir : {Direction::INC, Direction::DEC}) {
          RRNodeId clk_node = rr_graph_builder.create_node(
            layer, chan_coord.x(), chan_coord.y(), chan_type, curr_node_ptc);
          rr_graph_builder.set_node_direction(clk_node, node_dir);
          rr_graph_builder.set_node_capacity(clk_node, 1);
          rr_graph_builder.set_node_layer(clk_node, static_cast<char>(layer),
                                          static_cast<char>(layer));
          /* set cost_index using segment id */
          rr_graph_builder.set_node_cost_index(
            clk_node, RRIndexedDataId(cost_index_offset +
                                      size_t(clk_ntwk.default_segment())));
          /* FIXME: need to set rc_index and cost_index when building the graph
           * in VTR */
          /* register the node to a dedicated lookup */
          clk_rr_lookup.add_node(clk_node, chan_coord.x(), chan_coord.y(),
                                 itree, ilvl, ClockTreePinId(ipin), node_dir);
          VTR_LOGV(verbose,
                   "Added node '%lu' to clock node lookup (x='%lu' y='%lu' "
                   "tree='%lu' level='%lu' pin='%lu' direction='%s')\n",
                   size_t(clk_node), chan_coord.x(), chan_coord.y(),
                   size_t(itree), size_t(ilvl), ipin,
                   DIRECTION_STRING[size_t(node_dir)]);
          /* Update ptc count and go to next */
          curr_node_ptc++;
        }
      }
    }
  }
}

/********************************************************************
 * Add clock nodes one by one to the routing resource graph.
 * Assign node-level attributes properly and register in dedicated lookup
 *******************************************************************/
static void add_rr_graph_clock_nodes(
  RRGraphBuilder& rr_graph_builder, RRClockSpatialLookup& clk_rr_lookup,
  const RRGraphView& rr_graph_view, const DeviceGrid& grids,
  const size_t& layer, const bool& perimeter_cb, const bool& through_channel,
  const ClockNetwork& clk_ntwk, const bool& verbose) {
  /* Pre-allocate memory: Must do otherwise data will be messed up! */
  clk_rr_lookup.reserve_nodes(grids.width(), grids.height(),
                              clk_ntwk.num_trees(), clk_ntwk.max_tree_depth(),
                              clk_ntwk.max_tree_width());

  vtr::Rect<size_t> chanx_bb(1, 0, grids.width() - 1, grids.height() - 1);
  if (perimeter_cb) {
    chanx_bb.set_xmin(0);
    chanx_bb.set_xmax(grids.width());
    chanx_bb.set_ymin(0);
    chanx_bb.set_ymax(grids.height() - 1);
  }
  /* Add X-direction clock nodes */
  for (size_t iy = chanx_bb.ymin(); iy < chanx_bb.ymax(); ++iy) {
    for (size_t ix = chanx_bb.xmin(); ix < chanx_bb.xmax(); ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channels are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chanx_exist(grids, layer, chanx_coord, perimeter_cb))) {
        continue;
      }
      add_rr_graph_block_clock_nodes(
        rr_graph_builder, clk_rr_lookup, rr_graph_view, clk_ntwk, layer,
        chanx_coord, e_rr_type::CHANX, CHANX_COST_INDEX_START, verbose);
    }
  }

  /* Add Y-direction clock nodes */
  vtr::Rect<size_t> chany_bb(0, 1, grids.width() - 1, grids.height() - 1);
  if (perimeter_cb) {
    chany_bb.set_xmin(0);
    chany_bb.set_xmax(grids.width() - 1);
    chany_bb.set_ymin(0);
    chany_bb.set_ymax(grids.height());
  }
  for (size_t ix = chany_bb.xmin(); ix < chany_bb.xmax(); ++ix) {
    for (size_t iy = chany_bb.ymin(); iy < chany_bb.ymax(); ++iy) {
      vtr::Point<size_t> chany_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channel are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chany_exist(grids, layer, chany_coord, perimeter_cb))) {
        continue;
      }
      add_rr_graph_block_clock_nodes(
        rr_graph_builder, clk_rr_lookup, rr_graph_view, clk_ntwk, layer,
        chany_coord, e_rr_type::CHANY,
        CHANX_COST_INDEX_START + rr_graph_view.num_rr_segments(), verbose);
    }
  }
}

/********************************************************************
 * Find the destination CHANX|CHANY nodes for a driver clock node in a given
 *connection block There are two types of destination nodes:
 * - Straight connection where the driver clock node connects to another clock
 *node in the same direction and at the same level as well as clock index For
 *example
 *
 *   clk0_lvl0_chanx[1][1] -->------------->---> clk0_lvl0_chanx[2][1]
 *
 * - Turning connections where the driver clock node makes turns to connect
 *other clock nodes at 1-level up and in the same clock index
 *
 *
 *                            clk0_lvl1_chany[1][2]
 *                                     ^
 *                                     |
 *   clk0_lvl0_chanx[1][1] -->---------+
 *                                     |
 *                                     v
 *                            clk0_lvl1_chany[1][1]
 *
 * Coordinate system:
 *
 *        +----------+----------+------------+
 *        |   Grid   |    CBy   |   Grid     |
 *        | [x][y+1] | [x][y+1] | [x+1][y+1] |
 *        +----------+----------+------------+
 *        |   CBx    |   SB     |   CBx      |
 *        | [x][y]   |  [x][y]  | [x+1][y]   |
 *        +----------+----------+------------+
 *        |   Grid   |    CBy   |   Grid     |
 *        | [x][y]   | [x][y]   | [x+1][y]   |
 *        +----------+----------+------------+
 *
 *******************************************************************/
static std::vector<RRNodeId> find_clock_track2track_node(
  const RRGraphView& rr_graph_view, const ClockNetwork& clk_ntwk,
  const RRClockSpatialLookup& clk_rr_lookup, const e_rr_type& chan_type,
  const vtr::Point<size_t>& chan_coord, const ClockTreeId& clk_tree,
  const ClockLevelId& clk_lvl, const ClockTreePinId& clk_pin,
  const Direction& direction, const bool& verbose) {
  std::vector<RRNodeId> des_nodes;

  /* Straight connection */
  vtr::Point<size_t> straight_des_coord = chan_coord;
  if (chan_type == e_rr_type::CHANX) {
    if (direction == Direction::INC) {
      straight_des_coord.set_x(straight_des_coord.x() + 1);
    } else {
      VTR_ASSERT(direction == Direction::DEC);
      straight_des_coord.set_x(straight_des_coord.x() - 1);
    }
  } else {
    VTR_ASSERT(chan_type == e_rr_type::CHANY);
    if (direction == Direction::INC) {
      straight_des_coord.set_y(straight_des_coord.y() + 1);
    } else {
      VTR_ASSERT(direction == Direction::DEC);
      straight_des_coord.set_y(straight_des_coord.y() - 1);
    }
  }
  RRNodeId straight_des_node =
    clk_rr_lookup.find_node(straight_des_coord.x(), straight_des_coord.y(),
                            clk_tree, clk_lvl, clk_pin, direction, verbose);
  if (rr_graph_view.valid_node(straight_des_node)) {
    VTR_ASSERT(chan_type == rr_graph_view.node_type(straight_des_node));
    des_nodes.push_back(straight_des_node);
  }

  /* Check the next level if this is the last level, there are no turns
   * available */
  ClockLevelId next_clk_lvl = clk_ntwk.next_level(clk_lvl);
  if (!clk_ntwk.valid_level_id(clk_tree, next_clk_lvl)) {
    return des_nodes;
  }

  /* left turn connection */
  vtr::Point<size_t> left_des_coord = chan_coord;
  Direction left_direction = direction;
  e_rr_type left_des_chan_type = chan_type;
  if (chan_type == e_rr_type::CHANX) {
    left_des_chan_type = e_rr_type::CHANY;
    if (direction == Direction::INC) {
      /*
       *      ^
       *      |
       *   -->+
       */
      left_des_coord.set_y(left_des_coord.y() + 1);
    } else {
      /*
       *      +<--
       *      |
       *      v
       */
      VTR_ASSERT(direction == Direction::DEC);
      left_des_coord.set_x(left_des_coord.x() - 1);
    }
  } else {
    VTR_ASSERT(chan_type == e_rr_type::CHANY);
    left_des_chan_type = e_rr_type::CHANX;
    if (direction == Direction::INC) {
      /*
       *   <--+
       *      ^
       *      |
       */
      left_direction = Direction::DEC;
    } else {
      VTR_ASSERT(direction == Direction::DEC);
      /*
       *      |
       *      v
       *      +-->
       */
      left_direction = Direction::INC;
      left_des_coord.set_x(left_des_coord.x() + 1);
      left_des_coord.set_y(left_des_coord.y() - 1);
    }
  }
  RRNodeId left_des_node =
    clk_rr_lookup.find_node(left_des_coord.x(), left_des_coord.y(), clk_tree,
                            next_clk_lvl, clk_pin, left_direction, verbose);
  if (rr_graph_view.valid_node(left_des_node)) {
    VTR_ASSERT(left_des_chan_type == rr_graph_view.node_type(left_des_node));
    des_nodes.push_back(left_des_node);
  }

  /* right turn connection */
  vtr::Point<size_t> right_des_coord = chan_coord;
  Direction right_direction = direction;
  e_rr_type right_des_chan_type = chan_type;
  if (chan_type == e_rr_type::CHANX) {
    right_des_chan_type = e_rr_type::CHANY;
    if (direction == Direction::INC) {
      /*
       *   -->+
       *      |
       *      v
       */
      right_direction = Direction::DEC;
    } else {
      /*
       *      ^
       *      |
       *      +<--
       */
      VTR_ASSERT(direction == Direction::DEC);
      right_direction = Direction::INC;
      right_des_coord.set_x(right_des_coord.x() - 1);
      right_des_coord.set_y(right_des_coord.y() + 1);
    }
  } else {
    VTR_ASSERT(chan_type == e_rr_type::CHANY);
    right_des_chan_type = e_rr_type::CHANX;
    if (direction == Direction::INC) {
      /*
       *      +-->
       *      ^
       *      |
       */
      right_des_coord.set_x(right_des_coord.x() + 1);
    } else {
      VTR_ASSERT(direction == Direction::DEC);
      /*
       *      |
       *      v
       *   <--+
       */
      right_des_coord.set_y(right_des_coord.y() - 1);
    }
  }
  RRNodeId right_des_node =
    clk_rr_lookup.find_node(right_des_coord.x(), right_des_coord.y(), clk_tree,
                            next_clk_lvl, clk_pin, right_direction, verbose);
  if (rr_graph_view.valid_node(right_des_node)) {
    VTR_ASSERT(right_des_chan_type == rr_graph_view.node_type(right_des_node));
    des_nodes.push_back(right_des_node);
  }

  return des_nodes;
}

/********************************************************************
 * Try to find an IPIN of a grid which satisfy the requirement of clock pins
 * that has been defined in clock network. If the IPIN does exist in a
 * routing resource graph, add it to the node list
 *******************************************************************/
static void try_find_and_add_clock_track2ipin_node(
  std::vector<RRNodeId>& des_nodes, const DeviceGrid& grids,
  const RRGraphView& rr_graph_view, const size_t& layer,
  const vtr::Point<size_t>& grid_coord, const e_side& pin_side,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const ClockTreePinId& clk_pin, const bool& verbose) {
  t_physical_tile_type_ptr grid_type = grids.get_physical_type(
    t_physical_tile_loc(grid_coord.x(), grid_coord.y(), layer));
  VTR_LOGV(verbose, "Getting type of grid at (x=%d, y=%d)\n", grid_coord.x(),
           grid_coord.y());
  for (std::string tap_pin_name :
       clk_ntwk.tree_flatten_tap_to_ports(clk_tree, clk_pin, grid_coord)) {
    VTR_LOGV(verbose, "Checking tap pin name: %s\n", tap_pin_name.c_str());
    /* tap pin name could be 'io[5:5].a2f[0]' */
    int grid_pin_idx = find_physical_tile_pin_index(grid_type, tap_pin_name);
    if (grid_pin_idx == grid_type->num_pins) {
      continue;
    }
    VTR_LOGV(verbose, "Found a valid pin (index=%d) in physical tile\n",
             grid_pin_idx);
    RRNodeId des_node = rr_graph_view.node_lookup().find_node(
      layer, grid_coord.x(), grid_coord.y(), e_rr_type::IPIN, grid_pin_idx,
      pin_side);
    if (rr_graph_view.valid_node(des_node)) {
      VTR_LOGV(verbose, "Found a valid pin in rr graph\n");
      des_nodes.push_back(des_node);
    }
  }
}

/********************************************************************
 * Find the destination IPIN nodes for a driver clock node in a given connection
 *block.
 * For CHANX, the IPIN nodes are typically on the BOTTOM and TOP sides of
 *adjacent grids For CHANY, the IPIN nodes are typically on the LEFT and RIGHT
 *sides of adjacent grids For example Grid[1][2]
 *                                     ^
 *                                     |
 *   clk0_lvl2_chanx[1][1] -->---------+
 *                                     |
 *                                     v
 *                                  Grid[1][1]
 *
 * Coordinate system:
 *
 *        +----------+----------+------------+
 *        |   Grid   |    CBy   |   Grid     |
 *        | [x][y+1] | [x][y+1] | [x+1][y+1] |
 *        +----------+----------+------------+
 *        |   CBx    |   SB     |   CBx      |
 *        | [x][y]   |  [x][y]  | [x+1][y]   |
 *        +----------+----------+------------+
 *        |   Grid   |    CBy   |   Grid     |
 *        | [x][y]   | [x][y]   | [x+1][y]   |
 *        +----------+----------+------------+
 *******************************************************************/
static std::vector<RRNodeId> find_clock_track2ipin_node(
  const DeviceGrid& grids, const RRGraphView& rr_graph_view,
  const e_rr_type& chan_type, const size_t& layer,
  const vtr::Point<size_t>& chan_coord, const ClockNetwork& clk_ntwk,
  const ClockTreeId& clk_tree, const ClockTreePinId& clk_pin,
  const bool& verbose) {
  std::vector<RRNodeId> des_nodes;

  if (chan_type == e_rr_type::CHANX) {
    /* Get the clock IPINs at the BOTTOM side of adjacent grids [x][y+1] */
    vtr::Point<size_t> bot_grid_coord(chan_coord.x(), chan_coord.y() + 1);
    try_find_and_add_clock_track2ipin_node(
      des_nodes, grids, rr_graph_view, layer, bot_grid_coord, BOTTOM, clk_ntwk,
      clk_tree, clk_pin, verbose);

    /* Get the clock IPINs at the TOP side of adjacent grids [x][y] */
    vtr::Point<size_t> top_grid_coord(chan_coord.x(), chan_coord.y());
    try_find_and_add_clock_track2ipin_node(des_nodes, grids, rr_graph_view,
                                           layer, top_grid_coord, TOP, clk_ntwk,
                                           clk_tree, clk_pin, verbose);
  } else {
    VTR_ASSERT(chan_type == e_rr_type::CHANY);
    /* Get the clock IPINs at the LEFT side of adjacent grids [x][y+1] */
    vtr::Point<size_t> left_grid_coord(chan_coord.x() + 1, chan_coord.y());
    try_find_and_add_clock_track2ipin_node(
      des_nodes, grids, rr_graph_view, layer, left_grid_coord, LEFT, clk_ntwk,
      clk_tree, clk_pin, verbose);

    /* Get the clock IPINs at the RIGHT side of adjacent grids [x][y] */
    vtr::Point<size_t> right_grid_coord(chan_coord.x(), chan_coord.y());
    try_find_and_add_clock_track2ipin_node(
      des_nodes, grids, rr_graph_view, layer, right_grid_coord, RIGHT, clk_ntwk,
      clk_tree, clk_pin, verbose);
  }

  return des_nodes;
}

/********************************************************************
 * Add edges for the clock nodes in a given connection block
 *******************************************************************/
static void add_rr_graph_block_clock_edges(
  RRGraphBuilder& rr_graph_builder, size_t& num_edges_to_create,
  const RRClockSpatialLookup& clk_rr_lookup, const RRGraphView& rr_graph_view,
  const DeviceGrid& grids, const size_t& layer, const ClockNetwork& clk_ntwk,
  const vtr::Point<size_t>& chan_coord, const e_rr_type& chan_type,
  const bool& verbose) {
  size_t edge_count = 0;
  for (auto itree : clk_ntwk.trees()) {
    for (auto ilvl : clk_ntwk.levels(itree)) {
      /* As we want to keep uni-directional wires, clock routing tracks have to
       * be in pairs. Therefore, always add clock routing tracks in pair, even
       * one of them is not required
       */
      size_t num_pins = 0;
      bool require_complementary_pins = false;
      for (auto node_dir : {Direction::INC, Direction::DEC}) {
        if (0 == clk_ntwk.pins(itree, ilvl, chan_type, node_dir).size()) {
          require_complementary_pins = true;
        }
        num_pins += clk_ntwk.pins(itree, ilvl, chan_type, node_dir).size();
      }
      if (require_complementary_pins) {
        num_pins = 2 * num_pins;
      }
      for (size_t ipin = 0; ipin < num_pins / 2; ++ipin) {
        for (auto node_dir : {Direction::INC, Direction::DEC}) {
          /* find the driver clock node through lookup */
          RRNodeId src_node =
            clk_rr_lookup.find_node(chan_coord.x(), chan_coord.y(), itree, ilvl,
                                    ClockTreePinId(ipin), node_dir, verbose);
          VTR_LOGV(verbose,
                   "Try to find node '%lu' from clock node lookup (x='%lu' "
                   "y='%lu' tree='%lu' level='%lu' pin='%lu' direction='%s')\n",
                   size_t(src_node), chan_coord.x(), chan_coord.y(),
                   size_t(itree), size_t(ilvl), size_t(ipin),
                   DIRECTION_STRING[size_t(node_dir)]);
          VTR_ASSERT(rr_graph_view.valid_node(src_node));
          /* find the fan-out clock node through lookup */
          {
            size_t curr_edge_count = edge_count;
            for (RRNodeId des_node : find_clock_track2track_node(
                   rr_graph_view, clk_ntwk, clk_rr_lookup, chan_type,
                   chan_coord, itree, ilvl, ClockTreePinId(ipin), node_dir,
                   verbose)) {
              /* Create edges */
              VTR_ASSERT(rr_graph_view.valid_node(des_node));
              rr_graph_builder.create_edge(
                src_node, des_node, clk_ntwk.default_driver_switch(), false);
              edge_count++;
            }
            VTR_LOGV(verbose, "\tWill add %lu edges to other clock nodes\n",
                     edge_count - curr_edge_count);
          }
          /* If this is the clock node at the last level of the tree,
           * should drive some grid IPINs which are clocks */
          if (clk_ntwk.is_last_level(itree, ilvl)) {
            size_t curr_edge_count = edge_count;
            for (RRNodeId des_node : find_clock_track2ipin_node(
                   grids, rr_graph_view, chan_type, layer, chan_coord, clk_ntwk,
                   itree, ClockTreePinId(ipin), verbose)) {
              /* Create edges */
              VTR_ASSERT(rr_graph_view.valid_node(des_node));
              rr_graph_builder.create_edge(
                src_node, des_node, clk_ntwk.default_tap_switch(), false);
              edge_count++;
            }
            VTR_LOGV(verbose, "\tWill add %lu edges to IPINs\n",
                     edge_count - curr_edge_count);
          }
        }
      }
    }
  }
  /* Allocate edges */
  rr_graph_builder.build_edges(true);
  num_edges_to_create += edge_count;
}

/********************************************************************
 * Try to find an OPIN of a grid which satisfy the requirement of clock pins
 * that has been defined in clock network. If the OPIN does exist in a
 * routing resource graph, add it to the node list
 *******************************************************************/
static void try_find_and_add_clock_opin2track_node(
  std::vector<RRNodeId>& opin_nodes, const DeviceGrid& grids,
  const RRGraphView& rr_graph_view, const size_t& layer,
  const vtr::Point<int>& grid_coord, const e_side& pin_side,
  const ClockNetwork& clk_ntwk, const ClockTreePinId& clk_pin,
  const ClockInternalDriverId& int_driver_id, const bool& verbose) {
  t_physical_tile_type_ptr grid_type = grids.get_physical_type(
    t_physical_tile_loc(grid_coord.x(), grid_coord.y(), layer));
  for (std::string tap_pin_name :
       clk_ntwk.flatten_internal_driver_from_pin(int_driver_id, clk_pin)) {
    /* tap pin name could be 'io[5:5].a2f[0]' */
    int grid_pin_idx = find_physical_tile_pin_index(grid_type, tap_pin_name);
    if (grid_pin_idx == grid_type->num_pins) {
      continue;
    }
    RRNodeId opin_node = rr_graph_view.node_lookup().find_node(
      layer, grid_coord.x(), grid_coord.y(), e_rr_type::OPIN, grid_pin_idx,
      pin_side);
    if (rr_graph_view.valid_node(opin_node)) {
      VTR_LOGV(verbose, "Connected OPIN '%s' to clock network\n",
               tap_pin_name.c_str());
      opin_nodes.push_back(opin_node);
    }
  }
}

/********************************************************************
 * Find the source OPIN nodes as internal drivers for a clock node
 * For example
 *                           clk0_lvl1_chany[1][1]
 *                                     ^
 *                                     |
 *   internal_driver OPIN[0] -->-------+
 *                                     ^
 *                                     |
 *                          internal_driver OPIN[1]
 *
 * Coordinate system:
 *
 *        +----------+----------+------------+
 *        |   Grid   |    CBy   |   Grid     |
 *        | [x][y+1] | [x][y+1] | [x+1][y+1] |
 *        +----------+----------+------------+
 *        |   CBx    |   SB     |   CBx      |
 *        | [x][y]   |  [x][y]  | [x+1][y]   |
 *        +----------+----------+------------+
 *        |   Grid   |    CBy   |   Grid     |
 *        | [x][y]   | [x][y]   | [x+1][y]   |
 *        +----------+----------+------------+
 *******************************************************************/
static std::vector<RRNodeId> find_clock_opin2track_node(
  const DeviceGrid& grids, const RRGraphView& rr_graph_view,
  const size_t& layer, const vtr::Point<int>& sb_coord,
  const ClockNetwork& clk_ntwk, const ClockTreePinId& clk_pin,
  const std::vector<ClockInternalDriverId>& int_driver_ids,
  const bool& verbose) {
  std::vector<RRNodeId> opin_nodes;
  /* Find opins from
   * - Grid[x][y+1] on right and bottom sides
   * - Grid[x+1][y+1] on left and bottom sides
   * - Grid[x][y] on right and top sides
   * - Grid[x+1][y] on left and top sides
   */
  std::array<vtr::Point<int>, 4> grid_coords{
    vtr::Point<int>(sb_coord.x(), sb_coord.y() + 1),
    vtr::Point<int>(sb_coord.x() + 1, sb_coord.y() + 1),
    vtr::Point<int>(sb_coord.x(), sb_coord.y()),
    vtr::Point<int>(sb_coord.x() + 1, sb_coord.y())};
  std::array<std::array<e_side, 2>, 4> grid_sides{
    {{{e_side::RIGHT, e_side::BOTTOM}},
     {{e_side::LEFT, e_side::BOTTOM}},
     {{e_side::RIGHT, e_side::TOP}},
     {{e_side::LEFT, e_side::TOP}}}};
  for (size_t igrid = 0; igrid < 4; igrid++) {
    vtr::Point<int> grid_coord = grid_coords[igrid];
    for (e_side grid_side : grid_sides[igrid]) {
      for (ClockInternalDriverId int_driver_id : int_driver_ids) {
        try_find_and_add_clock_opin2track_node(
          opin_nodes, grids, rr_graph_view, layer, grid_coord, grid_side,
          clk_ntwk, clk_pin, int_driver_id, verbose);
      }
    }
  }
  return opin_nodes;
}

/********************************************************************
 * Add edges between OPIN of programmable blocks and clock routing tracks
 * Note that such edges only occur at the switching points of spines
 * Different from add_rr_graph_block_clock_edges(), we follow the clock spines
 *here By expanding on switching points, internal drivers will be added
 *******************************************************************/
static int add_rr_graph_opin2clk_edges(
  RRGraphBuilder& rr_graph_builder, size_t& num_edges_to_create,
  const RRClockSpatialLookup& clk_rr_lookup, const RRGraphView& rr_graph_view,
  const DeviceGrid& grids, const size_t& layer, const ClockNetwork& clk_ntwk,
  const bool& verbose) {
  size_t edge_count = 0;
  for (ClockTreeId clk_tree : clk_ntwk.trees()) {
    for (ClockSpineId ispine : clk_ntwk.spines(clk_tree)) {
      VTR_LOGV(verbose, "Finding internal drivers on spine '%s'...\n",
               clk_ntwk.spine_name(ispine).c_str());
      for (auto ipin : clk_ntwk.pins(clk_tree)) {
        for (ClockSwitchPointId switch_point_id :
             clk_ntwk.spine_switch_points(ispine)) {
          if (clk_ntwk
                .spine_switch_point_internal_drivers(ispine, switch_point_id)
                .empty()) {
            continue; /* We only focus on switching points containing internal
                         drivers */
          }
          size_t curr_edge_count = edge_count;
          /* Get the rr node of destination spine */
          ClockSpineId des_spine =
            clk_ntwk.spine_switch_point_tap(ispine, switch_point_id);
          vtr::Point<int> des_coord = clk_ntwk.spine_start_point(des_spine);
          Direction des_spine_direction = clk_ntwk.spine_direction(des_spine);
          ClockLevelId des_spine_level = clk_ntwk.spine_level(des_spine);
          RRNodeId des_node = clk_rr_lookup.find_node(
            des_coord.x(), des_coord.y(), clk_tree, des_spine_level, ipin,
            des_spine_direction, verbose);
          /* Walk through each qualified OPIN, build edges */
          vtr::Point<int> src_coord =
            clk_ntwk.spine_switch_point(ispine, switch_point_id);
          std::vector<ClockInternalDriverId> int_driver_ids =
            clk_ntwk.spine_switch_point_internal_drivers(ispine,
                                                         switch_point_id);
          for (RRNodeId src_node : find_clock_opin2track_node(
                 grids, rr_graph_view, layer, src_coord, clk_ntwk, ipin,
                 int_driver_ids, verbose)) {
            /* Create edges */
            VTR_ASSERT(rr_graph_view.valid_node(des_node));
            rr_graph_builder.create_edge(
              src_node, des_node, clk_ntwk.default_driver_switch(), false);
            edge_count++;
          }
          VTR_LOGV(verbose, "\tWill add %lu edges to OPINs at (x=%lu, y=%lu)\n",
                   edge_count - curr_edge_count, des_coord.x(), des_coord.y());
        }
      }
    }
  }
  /* Allocate edges */
  rr_graph_builder.build_edges(true);
  num_edges_to_create += edge_count;
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Add edges between OPIN of programmable blocks and clock routing tracks
 * Note that such edges only occur at the intermeidate points of spines
 * Different from add_rr_graph_opin2clk_edges(), we follow the clock spines
 *here By expanding on intermediate points, internal drivers will be added
 *******************************************************************/
static int add_rr_graph_opin2clk_intermediate_edges(
  RRGraphBuilder& rr_graph_builder, size_t& num_edges_to_create,
  const RRClockSpatialLookup& clk_rr_lookup, const RRGraphView& rr_graph_view,
  const DeviceGrid& grids, const size_t& layer, const ClockNetwork& clk_ntwk,
  const bool& verbose) {
  size_t edge_count = 0;
  for (ClockTreeId clk_tree : clk_ntwk.trees()) {
    for (ClockSpineId ispine : clk_ntwk.spines(clk_tree)) {
      VTR_LOGV(verbose, "Finding internal drivers on spine '%s'...\n",
               clk_ntwk.spine_name(ispine).c_str());
      for (auto ipin : clk_ntwk.pins(clk_tree)) {
        for (const vtr::Point<int>& coord :
             clk_ntwk.spine_coordinates(ispine)) {
          if (clk_ntwk.spine_intermediate_drivers(ispine, coord).empty()) {
            continue;
          }
          size_t curr_edge_count = edge_count;
          /* Get the rr node of destination spine */
          Direction des_spine_direction = clk_ntwk.spine_direction(ispine);
          ClockLevelId des_spine_level = clk_ntwk.spine_level(ispine);
          vtr::Point<int> des_coord =
            clk_ntwk.spine_intermediate_driver_routing_track_coord(ispine,
                                                                   coord);
          RRNodeId des_node = clk_rr_lookup.find_node(
            des_coord.x(), des_coord.y(), clk_tree, des_spine_level, ipin,
            des_spine_direction, verbose);
          if (!rr_graph_view.valid_node(des_node)) {
            continue;
          }
          /* Walk through each qualified OPIN, build edges */
          std::vector<ClockInternalDriverId> int_driver_ids =
            clk_ntwk.spine_intermediate_drivers(ispine, coord);
          for (RRNodeId src_node : find_clock_opin2track_node(
                 grids, rr_graph_view, layer, coord, clk_ntwk, ipin,
                 int_driver_ids, verbose)) {
            /* Create edges */
            VTR_ASSERT(rr_graph_view.valid_node(des_node));
            rr_graph_builder.create_edge(
              src_node, des_node, clk_ntwk.default_driver_switch(), false);
            edge_count++;
          }
          VTR_LOGV(verbose,
                   "\tWill add %lu edges from OPINs as intermediate drivers at "
                   "(x=%lu, y=%lu)\n",
                   edge_count - curr_edge_count, des_coord.x(), des_coord.y());
        }
      }
    }
  }
  /* Allocate edges */
  rr_graph_builder.build_edges(true);
  num_edges_to_create += edge_count;
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Add edges to interconnect clock nodes
 * Walk through the routing tracks in each connection block (driver nodes)
 * and add edge to their fan-out clock nodes
 * Note that
 * - clock nodes at the same level of a clock tree can only go straight
 * - clock nodes can only drive clock nodes belong to the same clock index (a
 *clock tree may contain multiple clocks)
 * - clock nodes can only drive clock nodes (by making a turn, straight
 *connection is not allowed) which are 1 level lower in the same clock tree with
 *the same clock index
 * For example
 *
 *                            clk0_lvl1_chany[1][2]
 *                                     ^
 *                                     |
 *   clk0_lvl0_chanx[1][1] -->---------+--->---> clk0_lvl0_chanx[2][1]
 *                                     |
 *                                     v
 *                            clk0_lvl1_chany[1][1]
 *******************************************************************/
static void add_rr_graph_clock_edges(
  RRGraphBuilder& rr_graph_builder, size_t& num_edges_to_create,
  const RRClockSpatialLookup& clk_rr_lookup, const RRGraphView& rr_graph_view,
  const DeviceGrid& grids, const size_t& layer, const bool& perimeter_cb,
  const bool& through_channel, const ClockNetwork& clk_ntwk,
  const bool& verbose) {
  vtr::Rect<size_t> chanx_bb(1, 0, grids.width() - 1, grids.height() - 1);
  if (perimeter_cb) {
    chanx_bb.set_xmin(0);
    chanx_bb.set_xmax(grids.width());
    chanx_bb.set_ymin(0);
    chanx_bb.set_ymax(grids.height() - 1);
  }
  /* Add edges which is driven by X-direction clock routing tracks */
  for (size_t iy = chanx_bb.ymin(); iy < chanx_bb.ymax(); ++iy) {
    for (size_t ix = chanx_bb.xmin(); ix < chanx_bb.xmax(); ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channels are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chanx_exist(grids, layer, chanx_coord, perimeter_cb))) {
        continue;
      }
      add_rr_graph_block_clock_edges(
        rr_graph_builder, num_edges_to_create, clk_rr_lookup, rr_graph_view,
        grids, layer, clk_ntwk, chanx_coord, e_rr_type::CHANX, verbose);
    }
  }

  /* Add edges which is driven by Y-direction clock routing tracks */
  vtr::Rect<size_t> chany_bb(0, 1, grids.width() - 1, grids.height() - 1);
  if (perimeter_cb) {
    chany_bb.set_xmin(0);
    chany_bb.set_xmax(grids.width() - 1);
    chany_bb.set_ymin(0);
    chany_bb.set_ymax(grids.height());
  }
  for (size_t ix = chany_bb.xmin(); ix < chany_bb.xmax(); ++ix) {
    for (size_t iy = chany_bb.ymin(); iy < chany_bb.ymax(); ++iy) {
      vtr::Point<size_t> chany_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channel are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chany_exist(grids, layer, chany_coord, perimeter_cb))) {
        continue;
      }
      add_rr_graph_block_clock_edges(
        rr_graph_builder, num_edges_to_create, clk_rr_lookup, rr_graph_view,
        grids, layer, clk_ntwk, chany_coord, e_rr_type::CHANY, verbose);
    }
  }
  /* Add edges between OPIN (internal driver) and clock routing tracks */
  add_rr_graph_opin2clk_edges(rr_graph_builder, num_edges_to_create,
                              clk_rr_lookup, rr_graph_view, grids, layer,
                              clk_ntwk, verbose);
  add_rr_graph_opin2clk_intermediate_edges(
    rr_graph_builder, num_edges_to_create, clk_rr_lookup, rr_graph_view, grids,
    layer, clk_ntwk, verbose);
}

/********************************************************************
 * Append a programmable clock network to an existing routing resource graph
 * This function will do the following jobs:
 * - Estimate the number of clock nodes and pre-allocate memory
 * - Add clock nodes
 * - Build edges between clock nodes
 * - Sanity checks
 *******************************************************************/
int append_clock_rr_graph(DeviceContext& vpr_device_ctx,
                          RRClockSpatialLookup& clk_rr_lookup,
                          const ClockNetwork& clk_ntwk, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Appending programmable clock network to routing resource graph");

  /* Skip if there is no clock tree */
  if (clk_ntwk.num_trees() == 0) {
    VTR_LOG(
      "Skip due to 0 clock trees.\nDouble check your clock architecture "
      "definition if this is unexpected\n");
    return CMD_EXEC_SUCCESS;
  }

  /* Estimate the number of nodes and pre-allocate */
  size_t orig_num_nodes = vpr_device_ctx.rr_graph.num_nodes();
  size_t num_clock_nodes = estimate_clock_rr_graph_num_nodes(
    vpr_device_ctx.grid, 0, vpr_device_ctx.arch->perimeter_cb,
    vpr_device_ctx.arch->through_channel, clk_ntwk);
  vpr_device_ctx.rr_graph_builder.unlock_storage();
  vpr_device_ctx.rr_graph_builder.reserve_nodes(num_clock_nodes +
                                                orig_num_nodes);
  VTR_LOGV(verbose,
           "Estimate %lu clock nodes (+%.5f%) to be added to routing "
           "resource graph.\n",
           num_clock_nodes, (float)(num_clock_nodes / orig_num_nodes));

  /* Add clock nodes */
  add_rr_graph_clock_nodes(
    vpr_device_ctx.rr_graph_builder, clk_rr_lookup, vpr_device_ctx.rr_graph,
    vpr_device_ctx.grid, 0, vpr_device_ctx.arch->perimeter_cb,
    vpr_device_ctx.arch->through_channel, clk_ntwk, verbose);
  VTR_LOGV(verbose,
           "Added %lu clock nodes to routing "
           "resource graph.\n",
           vpr_device_ctx.rr_graph.num_nodes() - orig_num_nodes);
  VTR_ASSERT(num_clock_nodes + orig_num_nodes ==
             vpr_device_ctx.rr_graph.num_nodes());

  /* Add edges between clock nodes*/
  size_t num_clock_edges = 0;
  add_rr_graph_clock_edges(
    vpr_device_ctx.rr_graph_builder, num_clock_edges,
    static_cast<const RRClockSpatialLookup&>(clk_rr_lookup),
    vpr_device_ctx.rr_graph, vpr_device_ctx.grid, 0,
    vpr_device_ctx.arch->perimeter_cb, vpr_device_ctx.arch->through_channel,
    clk_ntwk, verbose);
  VTR_LOGV(verbose,
           "Added %lu clock edges to routing "
           "resource graph.\n",
           num_clock_edges);

  /* TODO: Sanity checks */
  VTR_LOGV(verbose, "Initializing fan-in of nodes\n");
  vpr_device_ctx.rr_graph_builder.init_fan_in();
  VTR_LOGV(verbose, "Apply edge partitioning\n");
  vpr_device_ctx.rr_graph_builder.partition_edges();
  VTR_LOGV(verbose, "Building incoming edges\n");
  vpr_device_ctx.rr_graph_builder.build_in_edges();

  /* Report number of added clock nodes and edges */
  VTR_LOG(
    "Appended %lu clock nodes (+%.2f%) and %lu clock edges to routing "
    "resource graph.\n",
    num_clock_nodes, (float)(num_clock_nodes / orig_num_nodes),
    num_clock_edges);

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
