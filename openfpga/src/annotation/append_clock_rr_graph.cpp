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
  const ClockNetwork& clk_ntwk, const t_rr_type& chan_type) {
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
                                                const bool& through_channel,
                                                const ClockNetwork& clk_ntwk) {
  size_t num_nodes = 0;
  /* Check the number of CHANX nodes required */
  for (size_t iy = 0; iy < grids.height() - 1; ++iy) {
    for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channels are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chanx_exist(grids, chanx_coord))) {
        continue;
      }
      /* Estimate the routing tracks required by clock routing only */
      num_nodes += estimate_clock_rr_graph_num_chan_nodes(clk_ntwk, CHANX);
    }
  }

  for (size_t ix = 0; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      vtr::Point<size_t> chany_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channel are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chany_exist(grids, chany_coord))) {
        continue;
      }
      /* Estimate the routing tracks required by clock routing only */
      num_nodes += estimate_clock_rr_graph_num_chan_nodes(clk_ntwk, CHANY);
    }
  }

  return num_nodes;
}

/********************************************************************
 * Add clock nodes to a routing resource graph
 * For each tree and level of the tree, add a number of clock nodes
 * with direction, ptc and coordinates etc.
 *******************************************************************/
static void add_rr_graph_block_clock_nodes(RRGraphBuilder& rr_graph_builder,
                                           RRClockSpatialLookup& clk_rr_lookup,
                                           const RRGraphView& rr_graph_view,
                                           const ClockNetwork& clk_ntwk,
                                           const vtr::Point<size_t> chan_coord,
                                           const t_rr_type& chan_type,
                                           const int& cost_index_offset) {
  size_t orig_chan_width =
    rr_graph_view.node_lookup()
      .find_channel_nodes(chan_coord.x(), chan_coord.y(), chan_type)
      .size();
  size_t curr_node_ptc = orig_chan_width;

  for (auto itree : clk_ntwk.trees()) {
    for (auto ilvl : clk_ntwk.levels(itree)) {
      /* As we want to keep uni-directional wires, clock routing tracks have to be in pairs.
       * Therefore, always add clock routing tracks in pair, even one of them is not required
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
            chan_coord.x(), chan_coord.y(), chan_type, curr_node_ptc);
          rr_graph_builder.set_node_direction(clk_node, node_dir);
          rr_graph_builder.set_node_capacity(clk_node, 1);
          /* set cost_index using segment id */
          rr_graph_builder.set_node_cost_index(
            clk_node, RRIndexedDataId(cost_index_offset +
                                      size_t(clk_ntwk.default_segment())));
          /* FIXME: need to set rc_index and cost_index when building the graph
           * in VTR */
          /* register the node to a dedicated lookup */
          clk_rr_lookup.add_node(clk_node, chan_coord.x(), chan_coord.y(),
                                 itree, ilvl, ClockTreePinId(ipin), node_dir);
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
static void add_rr_graph_clock_nodes(RRGraphBuilder& rr_graph_builder,
                                     RRClockSpatialLookup& clk_rr_lookup,
                                     const RRGraphView& rr_graph_view,
                                     const DeviceGrid& grids,
                                     const bool& through_channel,
                                     const ClockNetwork& clk_ntwk) {
  /* Add X-direction clock nodes */
  for (size_t iy = 0; iy < grids.height() - 1; ++iy) {
    for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channels are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chanx_exist(grids, chanx_coord))) {
        continue;
      }
      add_rr_graph_block_clock_nodes(rr_graph_builder, clk_rr_lookup,
                                     rr_graph_view, clk_ntwk, chanx_coord,
                                     CHANX, CHANX_COST_INDEX_START);
    }
  }

  /* Add Y-direction clock nodes */
  for (size_t ix = 0; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      vtr::Point<size_t> chany_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channel are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chany_exist(grids, chany_coord))) {
        continue;
      }
      add_rr_graph_block_clock_nodes(
        rr_graph_builder, clk_rr_lookup, rr_graph_view, clk_ntwk, chany_coord,
        CHANY, CHANX_COST_INDEX_START + rr_graph_view.num_rr_segments());
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
  const RRClockSpatialLookup& clk_rr_lookup, const t_rr_type& chan_type,
  const vtr::Point<size_t>& chan_coord, const ClockTreeId& clk_tree,
  const ClockLevelId& clk_lvl, const ClockTreePinId& clk_pin,
  const Direction& direction) {
  std::vector<RRNodeId> des_nodes;

  /* Straight connection */
  vtr::Point<size_t> straight_des_coord = chan_coord;
  if (chan_type == CHANX) {
    if (direction == Direction::INC) {
      straight_des_coord.set_x(straight_des_coord.x() + 1);
    } else {
      VTR_ASSERT(direction == Direction::DEC);
      straight_des_coord.set_x(straight_des_coord.x() - 1);
    }
  } else {
    VTR_ASSERT(chan_type == CHANY);
    if (direction == Direction::INC) {
      straight_des_coord.set_y(straight_des_coord.y() + 1);
    } else {
      VTR_ASSERT(direction == Direction::DEC);
      straight_des_coord.set_y(straight_des_coord.y() - 1);
    }
  }
  RRNodeId straight_des_node =
    clk_rr_lookup.find_node(straight_des_coord.x(), straight_des_coord.y(),
                            clk_tree, clk_lvl, clk_pin, direction);
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
  t_rr_type left_des_chan_type = chan_type;
  if (chan_type == CHANX) {
    left_des_chan_type = CHANY;
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
    VTR_ASSERT(chan_type == CHANY);
    left_des_chan_type = CHANX;
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
                            next_clk_lvl, clk_pin, left_direction);
  if (rr_graph_view.valid_node(left_des_node)) {
    VTR_ASSERT(left_des_chan_type == rr_graph_view.node_type(left_des_node));
    des_nodes.push_back(left_des_node);
  }

  /* right turn connection */
  vtr::Point<size_t> right_des_coord = chan_coord;
  Direction right_direction = direction;
  t_rr_type right_des_chan_type = chan_type;
  if (chan_type == CHANX) {
    right_des_chan_type = CHANY;
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
    VTR_ASSERT(chan_type == CHANY);
    right_des_chan_type = CHANX;
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
                            next_clk_lvl, clk_pin, right_direction);
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
  const RRGraphView& rr_graph_view, const vtr::Point<size_t>& grid_coord,
  const e_side& pin_side, const ClockNetwork& clk_ntwk,
  const ClockTreeId& clk_tree, const ClockTreePinId& clk_pin) {
  t_physical_tile_type_ptr grid_type =
    grids[grid_coord.x()][grid_coord.y()].type;
  for (std::string tap_pin_name :
       clk_ntwk.tree_flatten_taps(clk_tree, clk_pin)) {
    /* tap pin name could be 'io[5:5].a2f[0]' */
    int grid_pin_idx = find_physical_tile_pin_index(grid_type, tap_pin_name);
    if (grid_pin_idx == grid_type->num_pins) {
      continue;
    }
    RRNodeId des_node = rr_graph_view.node_lookup().find_node(
      grid_coord.x(), grid_coord.y(), IPIN, grid_pin_idx, pin_side);
    if (rr_graph_view.valid_node(des_node)) {
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
  const t_rr_type& chan_type, const vtr::Point<size_t>& chan_coord,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const ClockTreePinId& clk_pin) {
  std::vector<RRNodeId> des_nodes;

  if (chan_type == CHANX) {
    /* Get the clock IPINs at the BOTTOM side of adjacent grids [x][y+1] */
    vtr::Point<size_t> bot_grid_coord(chan_coord.x(), chan_coord.y() + 1);
    try_find_and_add_clock_track2ipin_node(des_nodes, grids, rr_graph_view,
                                           bot_grid_coord, BOTTOM, clk_ntwk,
                                           clk_tree, clk_pin);

    /* Get the clock IPINs at the TOP side of adjacent grids [x][y] */
    vtr::Point<size_t> top_grid_coord(chan_coord.x(), chan_coord.y());
    try_find_and_add_clock_track2ipin_node(des_nodes, grids, rr_graph_view,
                                           top_grid_coord, TOP, clk_ntwk,
                                           clk_tree, clk_pin);
  } else {
    VTR_ASSERT(chan_type == CHANY);
    /* Get the clock IPINs at the LEFT side of adjacent grids [x][y+1] */
    vtr::Point<size_t> left_grid_coord(chan_coord.x() + 1, chan_coord.y());
    try_find_and_add_clock_track2ipin_node(des_nodes, grids, rr_graph_view,
                                           left_grid_coord, LEFT, clk_ntwk,
                                           clk_tree, clk_pin);

    /* Get the clock IPINs at the RIGHT side of adjacent grids [x][y] */
    vtr::Point<size_t> right_grid_coord(chan_coord.x(), chan_coord.y());
    try_find_and_add_clock_track2ipin_node(des_nodes, grids, rr_graph_view,
                                           right_grid_coord, RIGHT, clk_ntwk,
                                           clk_tree, clk_pin);
  }

  return des_nodes;
}

/********************************************************************
 * Add edges for the clock nodes in a given connection block
 *******************************************************************/
static void add_rr_graph_block_clock_edges(
  RRGraphBuilder& rr_graph_builder, size_t& num_edges_to_create,
  const RRClockSpatialLookup& clk_rr_lookup, const RRGraphView& rr_graph_view,
  const DeviceGrid& grids, const ClockNetwork& clk_ntwk,
  const vtr::Point<size_t> chan_coord, const t_rr_type& chan_type) {
  size_t edge_count = 0;
  for (auto itree : clk_ntwk.trees()) {
    for (auto ilvl : clk_ntwk.levels(itree)) {
      for (auto node_dir : {Direction::INC, Direction::DEC}) {
        for (auto ipin : clk_ntwk.pins(itree, ilvl, chan_type, node_dir)) {
          /* find the driver clock node through lookup */
          RRNodeId src_node = clk_rr_lookup.find_node(
            chan_coord.x(), chan_coord.y(), itree, ilvl, ipin, node_dir);
          VTR_ASSERT(rr_graph_view.valid_node(src_node));
          if (!clk_ntwk.is_last_level(itree, ilvl)) {
            /* find the fan-out clock node through lookup */
            for (RRNodeId des_node : find_clock_track2track_node(
                   rr_graph_view, clk_ntwk, clk_rr_lookup, chan_type,
                   chan_coord, itree, ilvl, ipin, node_dir)) {
              /* Create edges */
              VTR_ASSERT(rr_graph_view.valid_node(des_node));
              rr_graph_builder.create_edge(src_node, des_node,
                                           clk_ntwk.default_switch());
              edge_count++;
            }
          }
          /* If this is the clock node at the last level of the tree,
           * should drive some grid IPINs which are clocks */
          if (clk_ntwk.is_last_level(itree, ilvl)) {
            for (RRNodeId des_node : find_clock_track2ipin_node(
                   grids, rr_graph_view, chan_type, chan_coord, clk_ntwk, itree,
                   ipin)) {
              /* Create edges */
              VTR_ASSERT(rr_graph_view.valid_node(des_node));
              rr_graph_builder.create_edge(src_node, des_node,
                                           clk_ntwk.default_switch());
              edge_count++;
            }
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
static void add_rr_graph_clock_edges(RRGraphBuilder& rr_graph_builder,
                                     size_t& num_edges_to_create,
                                     const RRClockSpatialLookup& clk_rr_lookup,
                                     const RRGraphView& rr_graph_view,
                                     const DeviceGrid& grids,
                                     const bool& through_channel,
                                     const ClockNetwork& clk_ntwk) {
  /* Add edges which is driven by X-direction clock routing tracks */
  for (size_t iy = 0; iy < grids.height() - 1; ++iy) {
    for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channels are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chanx_exist(grids, chanx_coord))) {
        continue;
      }
      add_rr_graph_block_clock_edges(rr_graph_builder, num_edges_to_create,
                                     clk_rr_lookup, rr_graph_view, grids,
                                     clk_ntwk, chanx_coord, CHANX);
    }
  }

  /* Add edges which is driven by Y-direction clock routing tracks */
  for (size_t ix = 0; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      vtr::Point<size_t> chany_coord(ix, iy);
      /* Bypass if the routing channel does not exist when through channel are
       * not allowed */
      if ((false == through_channel) &&
          (false == is_chany_exist(grids, chany_coord))) {
        continue;
      }
      add_rr_graph_block_clock_edges(rr_graph_builder, num_edges_to_create,
                                     clk_rr_lookup, rr_graph_view, grids,
                                     clk_ntwk, chany_coord, CHANY);
    }
  }
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

  /* Report any clock structure we do not support yet! */
  if (clk_ntwk.num_trees() > 1) {
    VTR_LOG(
      "Currently only support 1 clock tree in programmable clock "
      "architecture\nPlease update your clock architecture definition\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Estimate the number of nodes and pre-allocate */
  size_t orig_num_nodes = vpr_device_ctx.rr_graph.num_nodes();
  size_t num_clock_nodes = estimate_clock_rr_graph_num_nodes(
    vpr_device_ctx.grid, vpr_device_ctx.arch->through_channel, clk_ntwk);
  vpr_device_ctx.rr_graph_builder.unlock_storage();
  vpr_device_ctx.rr_graph_builder.reserve_nodes(num_clock_nodes +
                                                orig_num_nodes);
  VTR_LOGV(verbose,
          "Estimate %lu clock nodes (+%.5f%) to be added to routing "
          "resource graph.\n",
          num_clock_nodes, (float)(num_clock_nodes / orig_num_nodes));

  /* Add clock nodes */
  add_rr_graph_clock_nodes(vpr_device_ctx.rr_graph_builder, clk_rr_lookup,
                           vpr_device_ctx.rr_graph, vpr_device_ctx.grid,
                           vpr_device_ctx.arch->through_channel, clk_ntwk);
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
    vpr_device_ctx.rr_graph, vpr_device_ctx.grid,
    vpr_device_ctx.arch->through_channel, clk_ntwk);

  /* TODO: Sanity checks */
  vpr_device_ctx.rr_graph_builder.init_fan_in();
  vpr_device_ctx.rr_graph_builder.partition_edges();
  vpr_device_ctx.rr_graph_builder.build_in_edges();

  /* Report number of added clock nodes and edges */
  VTR_LOG("Appended %lu clock nodes (+%.2f%) and %lu clock edges to routing "
          "resource graph.\n",
          num_clock_nodes, (float)(num_clock_nodes / orig_num_nodes),
          num_clock_edges);

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
