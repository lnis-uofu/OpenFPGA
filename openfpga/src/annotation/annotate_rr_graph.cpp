/********************************************************************
 * This file includes functions that are used to annotate device-level
 * information, in particular the routing resource graph
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"

/* Headers from vpr library */
#include "annotate_rr_graph.h"
#include "openfpga_rr_graph_utils.h"
#include "physical_types.h"
#include "rr_graph_view_util.h"

/* begin namespace openfpga */
namespace openfpga {

/* Build a RRChan Object with the given channel type and coorindators */
static RRChan build_one_rr_chan(const DeviceContext& vpr_device_ctx,
                                const t_rr_type& chan_type, const size_t& layer,
                                vtr::Point<size_t>& chan_coord) {
  std::vector<RRNodeId> chan_rr_nodes;

  /* Create a rr_chan object and check if it is unique in the graph */
  RRChan rr_chan;
  /* Fill the information */
  rr_chan.set_type(chan_type);

  /* Collect rr_nodes for this channel */
  chan_rr_nodes = find_rr_graph_chan_nodes(
    vpr_device_ctx.rr_graph, layer, chan_coord.x(), chan_coord.y(), chan_type);
  /* Fill the rr_chan */
  for (const RRNodeId& chan_rr_node : chan_rr_nodes) {
    rr_chan.add_node(vpr_device_ctx.rr_graph, chan_rr_node,
                     vpr_device_ctx.rr_graph.node_segment(chan_rr_node));
  }

  return rr_chan;
}

/* Build a General Switch Block (GSB)
 * which includes:
 * [I] A Switch Box subckt consists of following ports:
 * 1. Channel Y [x][y] inputs
 * 2. Channel X [x+1][y] inputs
 * 3. Channel Y [x][y-1] outputs
 * 4. Channel X [x][y] outputs
 * 5. Grid[x][y+1] Right side outputs pins
 * 6. Grid[x+1][y+1] Left side output pins
 * 7. Grid[x+1][y+1] Bottom side output pins
 * 8. Grid[x+1][y] Top side output pins
 * 9. Grid[x+1][y] Left side output pins
 * 10. Grid[x][y] Right side output pins
 * 11. Grid[x][y] Top side output pins
 * 12. Grid[x][y+1] Bottom side output pins
 *
 *    --------------          --------------
 *    |            |   CBY    |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *     ChanX & CBX  | Switch |     ChanX
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 * For channels chanY with INC_DIRECTION on the top side, they should be marked
 * as outputs For channels chanY with DEC_DIRECTION on the top side, they should
 * be marked as inputs For channels chanY with INC_DIRECTION on the bottom side,
 * they should be marked as inputs For channels chanY with DEC_DIRECTION on the
 * bottom side, they should be marked as outputs For channels chanX with
 * INC_DIRECTION on the left side, they should be marked as inputs For channels
 * chanX with DEC_DIRECTION on the left side, they should be marked as outputs
 * For channels chanX with INC_DIRECTION on the right side, they should be
 * marked as outputs For channels chanX with DEC_DIRECTION on the right side,
 * they should be marked as inputs
 *
 * [II] A X-direction Connection Block [x][y]
 * The connection block shares the same routing channel[x][y] with the Switch
 * Block We just need to fill the ipin nodes at TOP and BOTTOM sides as well as
 * properly fill the ipin_grid_side information [III] A Y-direction Connection
 * Block [x][y+1] The connection block shares the same routing channel[x][y+1]
 * with the Switch Block We just need to fill the ipin nodes at LEFT and RIGHT
 * sides as well as properly fill the ipin_grid_side information
 */
static RRGSB build_rr_gsb(const DeviceContext& vpr_device_ctx,
                          const vtr::Point<size_t>& gsb_range,
                          const size_t& layer,
                          const vtr::Point<size_t>& gsb_coord,
                          const bool& include_clock) {
  /* Create an object to return */
  RRGSB rr_gsb;

  VTR_ASSERT(gsb_coord.x() <= gsb_range.x());
  VTR_ASSERT(gsb_coord.y() <= gsb_range.y());

  /* Coordinator initialization */
  rr_gsb.set_coordinate(gsb_coord.x(), gsb_coord.y());

  /* Basic information*/
  rr_gsb.init_num_sides(4); /* Fixed number of sides */

  /* Find all rr_nodes of channels */
  /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    /* Local variables inside this for loop */
    SideManager side_manager(side);
    vtr::Point<size_t> coordinate =
      rr_gsb.get_side_block_coordinate(side_manager.get_side());
    RRChan rr_chan;
    std::vector<std::vector<RRNodeId>> temp_opin_rr_nodes(2);
    enum e_side opin_grid_side[2] = {NUM_SIDES, NUM_SIDES};
    enum PORTS chan_dir_to_port_dir_mapping[2] = {
      OUT_PORT, IN_PORT}; /* 0: INC_DIRECTION => ?; 1: DEC_DIRECTION => ? */

    switch (side) {
      case TOP: /* TOP = 0 */
        /* For the border, we should take special care */
        if (gsb_coord.y() == gsb_range.y()) {
          rr_gsb.clear_one_side(side_manager.get_side());
          break;
        }
        /* Routing channels*/
        /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
        /* Create a rr_chan object and check if it is unique in the graph */
        rr_chan = build_one_rr_chan(vpr_device_ctx, CHANY, layer, coordinate);
        chan_dir_to_port_dir_mapping[0] =
          OUT_PORT; /* INC_DIRECTION => OUT_PORT */
        chan_dir_to_port_dir_mapping[1] =
          IN_PORT; /* DEC_DIRECTION => IN_PORT */

        /* Build the Switch block: opin and opin_grid_side */
        /* Assign grid side of OPIN */
        /* Grid[x][y+1] RIGHT side outputs pins */
        opin_grid_side[0] = RIGHT;
        /* Grid[x+1][y+1] left side outputs pins */
        opin_grid_side[1] = LEFT;
        /* Include Grid[x][y+1] RIGHT side outputs pins */
        temp_opin_rr_nodes[0] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer, gsb_coord.x(),
          gsb_coord.y() + 1, OPIN, opin_grid_side[0]);
        /* Include Grid[x+1][y+1] Left side output pins */
        temp_opin_rr_nodes[1] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer,
          gsb_coord.x() + 1, gsb_coord.y() + 1, OPIN, opin_grid_side[1]);

        break;
      case RIGHT: /* RIGHT = 1 */
        /* For the border, we should take special care */
        if (gsb_coord.x() == gsb_range.x()) {
          rr_gsb.clear_one_side(side_manager.get_side());
          break;
        }
        /* Routing channels*/
        /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
        /* Collect rr_nodes for Tracks for top: chany[x][y+1] */
        /* Create a rr_chan object and check if it is unique in the graph */
        rr_chan = build_one_rr_chan(vpr_device_ctx, CHANX, layer, coordinate);
        chan_dir_to_port_dir_mapping[0] =
          OUT_PORT; /* INC_DIRECTION => OUT_PORT */
        chan_dir_to_port_dir_mapping[1] =
          IN_PORT; /* DEC_DIRECTION => IN_PORT */

        /* Build the Switch block: opin and opin_grid_side */
        /* Assign grid side of OPIN */
        /* Grid[x+1][y+1] BOTTOM side outputs pins */
        opin_grid_side[0] = BOTTOM;
        /* Grid[x+1][y] TOP side outputs pins */
        opin_grid_side[1] = TOP;

        /* include Grid[x+1][y+1] Bottom side output pins */
        temp_opin_rr_nodes[0] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer,
          gsb_coord.x() + 1, gsb_coord.y() + 1, OPIN, opin_grid_side[0]);
        /* include Grid[x+1][y] Top side output pins */
        temp_opin_rr_nodes[1] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer,
          gsb_coord.x() + 1, gsb_coord.y(), OPIN, opin_grid_side[1]);
        break;
      case BOTTOM: /* BOTTOM = 2*/
        /* For the border, we should take special care */
        if (gsb_coord.y() == 0) {
          rr_gsb.clear_one_side(side_manager.get_side());
          break;
        }
        /* Routing channels*/
        /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
        /* Collect rr_nodes for Tracks for bottom: chany[x][y] */
        /* Create a rr_chan object and check if it is unique in the graph */
        rr_chan = build_one_rr_chan(vpr_device_ctx, CHANY, layer, coordinate);
        chan_dir_to_port_dir_mapping[0] =
          IN_PORT; /* INC_DIRECTION => IN_PORT */
        chan_dir_to_port_dir_mapping[1] =
          OUT_PORT; /* DEC_DIRECTION => OUT_PORT */

        /* Build the Switch block: opin and opin_grid_side */
        /* Assign grid side of OPIN */
        /* Grid[x+1][y] LEFT side outputs pins */
        opin_grid_side[0] = LEFT;
        /* Grid[x][y] RIGHT side outputs pins */
        opin_grid_side[1] = RIGHT;
        /* include Grid[x+1][y] Left side output pins */
        temp_opin_rr_nodes[0] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer,
          gsb_coord.x() + 1, gsb_coord.y(), OPIN, opin_grid_side[0]);
        /* include Grid[x][y] Right side output pins */
        temp_opin_rr_nodes[1] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer, gsb_coord.x(),
          gsb_coord.y(), OPIN, opin_grid_side[1]);
        break;
      case LEFT: /* LEFT = 3 */
        /* For the border, we should take special care */
        if (gsb_coord.x() == 0) {
          rr_gsb.clear_one_side(side_manager.get_side());
          break;
        }
        /* Routing channels*/
        /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
        /* Collect rr_nodes for Tracks for left: chanx[x][y] */
        /* Create a rr_chan object and check if it is unique in the graph */
        rr_chan = build_one_rr_chan(vpr_device_ctx, CHANX, layer, coordinate);
        chan_dir_to_port_dir_mapping[0] =
          IN_PORT; /* INC_DIRECTION => IN_PORT */
        chan_dir_to_port_dir_mapping[1] =
          OUT_PORT; /* DEC_DIRECTION => OUT_PORT */

        /* Build the Switch block: opin and opin_grid_side */
        /* Grid[x][y+1] BOTTOM side outputs pins */
        opin_grid_side[0] = BOTTOM;
        /* Grid[x][y] TOP side outputs pins */
        opin_grid_side[1] = TOP;
        /* include Grid[x][y+1] Bottom side outputs pins */
        temp_opin_rr_nodes[0] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer, gsb_coord.x(),
          gsb_coord.y() + 1, OPIN, opin_grid_side[0]);
        /* include Grid[x][y] Top side output pins */
        temp_opin_rr_nodes[1] = find_rr_graph_grid_nodes(
          vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer, gsb_coord.x(),
          gsb_coord.y(), OPIN, opin_grid_side[1]);
        break;
      default:
        VTR_LOG_ERROR("Invalid side index!\n");
        exit(1);
    }

    /* Organize a vector of port direction */
    if (0 < rr_chan.get_chan_width()) {
      std::vector<enum PORTS> rr_chan_dir;
      rr_chan_dir.resize(rr_chan.get_chan_width());
      for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
        /* Identify the directionality, record it in rr_node_direction */
        if (Direction::INC ==
            vpr_device_ctx.rr_graph.node_direction(rr_chan.get_node(itrack))) {
          rr_chan_dir[itrack] = chan_dir_to_port_dir_mapping[0];
        } else {
          VTR_ASSERT(Direction::DEC == vpr_device_ctx.rr_graph.node_direction(
                                         rr_chan.get_node(itrack)));
          rr_chan_dir[itrack] = chan_dir_to_port_dir_mapping[1];
        }
      }
      /* Fill chan_rr_nodes */
      rr_gsb.add_chan_node(side_manager.get_side(), rr_chan, rr_chan_dir);
    }

    /* Fill opin_rr_nodes */
    /* Copy from temp_opin_rr_node to opin_rr_node */
    for (size_t opin_array_id = 0; opin_array_id < temp_opin_rr_nodes.size();
         ++opin_array_id) {
      for (const RRNodeId& inode : temp_opin_rr_nodes[opin_array_id]) {
        /* Skip those has no configurable outgoing, they should NOT appear in
         * the GSB connection This is for those grid output pins used by direct
         * connections
         */
        if (0 == vpr_device_ctx.rr_graph.num_configurable_edges(inode)) {
          continue;
        }
        /* Do not consider OPINs that directly drive an IPIN
         * they are supposed to be handled by direct connection
         */
        if (true ==
            is_opin_direct_connected_ipin(vpr_device_ctx.rr_graph, inode)) {
          continue;
        }

        /* Grid[x+1][y+1] Bottom side outputs pins */
        rr_gsb.add_opin_node(inode, side_manager.get_side());
      }
    }

    /* Clean ipin_rr_nodes */
    /* We do not have any IPIN for a Switch Block */
    rr_gsb.clear_ipin_nodes(side_manager.get_side());

    /* Clear the temp data */
    temp_opin_rr_nodes[0].clear();
    temp_opin_rr_nodes[1].clear();
    opin_grid_side[0] = NUM_SIDES;
    opin_grid_side[1] = NUM_SIDES;
  }

  /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    /* Local variables inside this for loop */
    SideManager side_manager(side);
    size_t ix;
    size_t iy;
    enum e_side chan_side;
    std::vector<RRNodeId> temp_ipin_rr_nodes;
    enum e_side ipin_rr_node_grid_side;

    switch (side) {
      case TOP: /* TOP = 0 */
        /* For the bording, we should take special care */
        /* Check if left side chan width is 0 or not */
        chan_side = LEFT;
        /* Build the connection block: ipin and ipin_grid_side */
        /* BOTTOM side INPUT Pins of Grid[x][y+1] */
        ix = rr_gsb.get_sb_x();
        iy = rr_gsb.get_sb_y() + 1;
        ipin_rr_node_grid_side = BOTTOM;
        break;
      case RIGHT: /* RIGHT = 1 */
        /* For the bording, we should take special care */
        /* Check if TOP side chan width is 0 or not */
        chan_side = TOP;
        /* Build the connection block: ipin and ipin_grid_side */
        /* LEFT side INPUT Pins of Grid[x+1][y+1] */
        ix = rr_gsb.get_sb_x() + 1;
        iy = rr_gsb.get_sb_y() + 1;
        ipin_rr_node_grid_side = LEFT;
        break;
      case BOTTOM: /* BOTTOM = 2*/
        /* For the bording, we should take special care */
        /* Check if left side chan width is 0 or not */
        chan_side = LEFT;
        /* Build the connection block: ipin and ipin_grid_side */
        /* TOP side INPUT Pins of Grid[x][y] */
        ix = rr_gsb.get_sb_x();
        iy = rr_gsb.get_sb_y();
        ipin_rr_node_grid_side = TOP;
        break;
      case LEFT: /* LEFT = 3 */
        /* For the bording, we should take special care */
        /* Check if left side chan width is 0 or not */
        chan_side = TOP;
        /* Build the connection block: ipin and ipin_grid_side */
        /* RIGHT side INPUT Pins of Grid[x][y+1] */
        ix = rr_gsb.get_sb_x();
        iy = rr_gsb.get_sb_y() + 1;
        ipin_rr_node_grid_side = RIGHT;
        break;
      default:
        VTR_LOG_ERROR("Invalid side index!\n");
        exit(1);
    }

    /* If there is no channel at this side, we skip ipin_node annotation */
    if (0 == rr_gsb.get_chan_width(chan_side)) {
      continue;
    }
    /* Collect IPIN rr_nodes*/
    temp_ipin_rr_nodes = find_rr_graph_grid_nodes(
      vpr_device_ctx.rr_graph, vpr_device_ctx.grid, layer, ix, iy, IPIN,
      ipin_rr_node_grid_side, include_clock);
    /* Fill the ipin nodes of RRGSB */
    for (const RRNodeId& inode : temp_ipin_rr_nodes) {
      /* Skip those has no configurable outgoing, they should NOT appear in the
       * GSB connection This is for those grid output pins used by direct
       * connections
       */
      if (0 ==
          vpr_device_ctx.rr_graph.node_configurable_in_edges(inode).size()) {
        continue;
      }

      /* Do not consider IPINs that are directly connected by an OPIN
       * they are supposed to be handled by direct connection
       */
      if (true ==
          is_ipin_direct_connected_opin(vpr_device_ctx.rr_graph, inode)) {
        continue;
      }

      rr_gsb.add_ipin_node(inode, side_manager.get_side());
    }
    /* Clear the temp data */
    temp_ipin_rr_nodes.clear();
  }

  /* Build OPIN node lists for connection blocks */
  rr_gsb.build_cb_opin_nodes(vpr_device_ctx.rr_graph);

  return rr_gsb;
}

/********************************************************************
 * Build the annotation for the routing resource graph
 * by collecting the nodes to the General Switch Block context
 *******************************************************************/
void annotate_device_rr_gsb(const DeviceContext& vpr_device_ctx,
                            DeviceRRGSB& device_rr_gsb,
                            const bool& include_clock,
                            const bool& verbose_output) {
  vtr::ScopedStartFinishTimer timer(
    "Build General Switch Block(GSB) annotation on top of routing resource "
    "graph");

  /* Note that the GSB array is smaller than the grids by 1 column and 1 row!!!
   */
  vtr::Point<size_t> gsb_range(vpr_device_ctx.grid.width() - 1,
                               vpr_device_ctx.grid.height() - 1);
  device_rr_gsb.reserve(gsb_range);

  VTR_LOGV(verbose_output, "Start annotation GSB up to [%lu][%lu]\n",
           gsb_range.x(), gsb_range.y());

  size_t gsb_cnt = 0;
  size_t layer = 0;
  /* For each switch block, determine the size of array */
  for (size_t ix = 0; ix < gsb_range.x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.y(); ++iy) {
      /* Here we give the builder the fringe coordinates so that it can handle
       * the GSBs at the borderside correctly sort drive_rr_nodes should be
       * called if required by users
       */
      const RRGSB& rr_gsb =
        build_rr_gsb(vpr_device_ctx,
                     vtr::Point<size_t>(vpr_device_ctx.grid.width() - 2,
                                        vpr_device_ctx.grid.height() - 2),
                     layer, vtr::Point<size_t>(ix, iy), include_clock);
      /* Add to device_rr_gsb */
      vtr::Point<size_t> gsb_coordinate = rr_gsb.get_sb_coordinate();
      device_rr_gsb.add_rr_gsb(gsb_coordinate, rr_gsb);
      gsb_cnt++; /* Update counter */
      /* Print info */
      VTR_LOG("[%lu%] Backannotated GSB[%lu][%lu]\r",
              100 * gsb_cnt / (gsb_range.x() * gsb_range.y()), ix, iy);
    }
  }
  /* Report number of unique mirrors */
  VTR_LOG("Backannotated %d General Switch Blocks (GSBs).\n",
          gsb_range.x() * gsb_range.y());
}

/********************************************************************
 * Sort all the incoming edges for each channel node which are
 * output ports of the GSB
 *******************************************************************/
void sort_device_rr_gsb_chan_node_in_edges(const RRGraphView& rr_graph,
                                           DeviceRRGSB& device_rr_gsb,
                                           const bool& verbose_output) {
  vtr::ScopedStartFinishTimer timer(
    "Sort incoming edges for each routing track output node of General Switch "
    "Block(GSB)");

  /* Note that the GSB array is smaller than the grids by 1 column and 1 row!!!
   */
  vtr::Point<size_t> gsb_range = device_rr_gsb.get_gsb_range();

  VTR_LOGV(verbose_output, "Start sorting edges for GSBs up to [%lu][%lu]\n",
           gsb_range.x(), gsb_range.y());

  size_t gsb_cnt = 0;

  /* For each switch block, determine the size of array */
  for (size_t ix = 0; ix < gsb_range.x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.y(); ++iy) {
      vtr::Point<size_t> gsb_coordinate(ix, iy);
      RRGSB& rr_gsb = device_rr_gsb.get_mutable_gsb(gsb_coordinate);
      rr_gsb.sort_chan_node_in_edges(rr_graph);

      gsb_cnt++; /* Update counter */

      /* Print info */
      VTR_LOG(
        "[%lu%] Sorted incoming edges for each routing track output node of "
        "GSB[%lu][%lu]\r",
        100 * gsb_cnt / (gsb_range.x() * gsb_range.y()), ix, iy);
    }
  }

  /* Report number of unique mirrors */
  VTR_LOG(
    "Sorted incoming edges for each routing track output node of %d General "
    "Switch Blocks (GSBs).\n",
    gsb_range.x() * gsb_range.y());
}

/********************************************************************
 * Sort all the incoming edges for each input pin node which are
 * output ports of the GSB
 *******************************************************************/
void sort_device_rr_gsb_ipin_node_in_edges(const RRGraphView& rr_graph,
                                           DeviceRRGSB& device_rr_gsb,
                                           const bool& verbose_output) {
  vtr::ScopedStartFinishTimer timer(
    "Sort incoming edges for each input pin node of General Switch Block(GSB)");

  /* Note that the GSB array is smaller than the grids by 1 column and 1 row!!!
   */
  vtr::Point<size_t> gsb_range = device_rr_gsb.get_gsb_range();

  VTR_LOGV(verbose_output, "Start sorting edges for GSBs up to [%lu][%lu]\n",
           gsb_range.x(), gsb_range.y());

  size_t gsb_cnt = 0;

  /* For each switch block, determine the size of array */
  for (size_t ix = 0; ix < gsb_range.x(); ++ix) {
    for (size_t iy = 0; iy < gsb_range.y(); ++iy) {
      vtr::Point<size_t> gsb_coordinate(ix, iy);
      RRGSB& rr_gsb = device_rr_gsb.get_mutable_gsb(gsb_coordinate);
      rr_gsb.sort_ipin_node_in_edges(rr_graph);

      gsb_cnt++; /* Update counter */

      /* Print info */
      VTR_LOG(
        "[%lu%] Sorted incoming edges for each input pin node of "
        "GSB[%lu][%lu]\r",
        100 * gsb_cnt / (gsb_range.x() * gsb_range.y()), ix, iy);
    }
  }

  /* Report number of unique mirrors */
  VTR_LOG(
    "Sorted incoming edges for each input pin node of %d General Switch Blocks "
    "(GSBs).\n",
    gsb_range.x() * gsb_range.y());
}

/********************************************************************
 * Build the link between rr_graph switches to their physical circuit models
 * The binding is done based on the name of rr_switches defined in the
 * OpenFPGA arch XML
 *******************************************************************/
static void annotate_rr_switch_circuit_models(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  size_t count = 0;

  for (size_t rr_switch_id = 0;
       rr_switch_id < vpr_device_ctx.rr_graph.rr_switch().size();
       rr_switch_id++) {
    std::string switch_name(
      vpr_device_ctx.rr_graph.rr_switch()[RRSwitchId(rr_switch_id)].name);
    /* Skip the delayless switch, which is only used by the edges between
     * - SOURCE and OPIN
     * - IPIN and SINK
     */
    if (switch_name == std::string(VPR_DELAYLESS_SWITCH_NAME)) {
      continue;
    }

    CircuitModelId circuit_model = CircuitModelId::INVALID();
    /* The name-to-circuit mapping is stored in either cb_switch-to-circuit or
     * sb_switch-to-circuit, Try to find one and update the device annotation
     */
    if (0 < openfpga_arch.cb_switch2circuit.count(switch_name)) {
      circuit_model = openfpga_arch.cb_switch2circuit.at(switch_name);
    }
    if (0 < openfpga_arch.sb_switch2circuit.count(switch_name)) {
      if (CircuitModelId::INVALID() != circuit_model) {
        VTR_LOG_WARN(
          "Found a connection block and a switch block switch share the same "
          "name '%s' and binded to different circuit models '%s' and "
          "'%s'!\nWill use the switch block switch binding!\n",
          switch_name.c_str(),
          openfpga_arch.circuit_lib.model_name(circuit_model).c_str(),
          openfpga_arch.circuit_lib
            .model_name(openfpga_arch.sb_switch2circuit.at(switch_name))
            .c_str());
      }
      circuit_model = openfpga_arch.sb_switch2circuit.at(switch_name);
    }

    /* Cannot find a circuit model, error out! */
    if (CircuitModelId::INVALID() == circuit_model) {
      VTR_LOG_ERROR(
        "Fail to find a circuit model for a routing resource graph switch "
        "'%s'!\nPlease check your OpenFPGA architecture XML!\n",
        switch_name.c_str());
      exit(1);
    }

    /* Check the circuit model type */
    if (CIRCUIT_MODEL_MUX !=
        openfpga_arch.circuit_lib.model_type(circuit_model)) {
      VTR_LOG_ERROR(
        "Require circuit model type '%s' for a routing resource graph switch "
        "'%s'!\nPlease check your OpenFPGA architecture XML!\n",
        CIRCUIT_MODEL_TYPE_STRING[CIRCUIT_MODEL_MUX], switch_name.c_str());
      exit(1);
    }

    /* Now update the device annotation */
    vpr_device_annotation.add_rr_switch_circuit_model(RRSwitchId(rr_switch_id),
                                                      circuit_model);
    VTR_LOGV(
      verbose_output,
      "Binded a routing resource graph switch '%s' to circuit model '%s'\n",
      switch_name.c_str(),
      openfpga_arch.circuit_lib.model_name(circuit_model).c_str());
    count++;
  }

  VTR_LOG("Binded %lu routing resource graph switches to circuit models\n",
          count);
}

/********************************************************************
 * Build the link between rr_graph routing segments to their physical circuit
 *models The binding is done based on the name of rr_segment defined in the
 * OpenFPGA arch XML
 *******************************************************************/
static void annotate_rr_segment_circuit_models(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  size_t count = 0;

  for (size_t iseg = 0; iseg < vpr_device_ctx.arch->Segments.size(); ++iseg) {
    std::string segment_name = vpr_device_ctx.arch->Segments[iseg].name;
    CircuitModelId circuit_model = CircuitModelId::INVALID();
    /* The name-to-circuit mapping is stored in either cb_switch-to-circuit or
     * sb_switch-to-circuit, Try to find one and update the device annotation
     */
    if (0 < openfpga_arch.routing_seg2circuit.count(segment_name)) {
      circuit_model = openfpga_arch.routing_seg2circuit.at(segment_name);
    }
    /* Cannot find a circuit model, error out! */
    if (CircuitModelId::INVALID() == circuit_model) {
      VTR_LOG_ERROR(
        "Fail to find a circuit model for a routing segment '%s'!\nPlease "
        "check your OpenFPGA architecture XML!\n",
        segment_name.c_str());
      exit(1);
    }

    /* Check the circuit model type */
    if (CIRCUIT_MODEL_CHAN_WIRE !=
        openfpga_arch.circuit_lib.model_type(circuit_model)) {
      VTR_LOG_ERROR(
        "Require circuit model type '%s' for a routing segment '%s'!\nPlease "
        "check your OpenFPGA architecture XML!\n",
        CIRCUIT_MODEL_TYPE_STRING[CIRCUIT_MODEL_CHAN_WIRE],
        segment_name.c_str());
      exit(1);
    }

    /* Now update the device annotation */
    vpr_device_annotation.add_rr_segment_circuit_model(RRSegmentId(iseg),
                                                       circuit_model);
    VTR_LOGV(verbose_output,
             "Binded a routing segment '%s' to circuit model '%s'\n",
             segment_name.c_str(),
             openfpga_arch.circuit_lib.model_name(circuit_model).c_str());
    count++;
  }

  VTR_LOG("Binded %lu routing segments to circuit models\n", count);
}

/********************************************************************
 * Build the link between rr_graph direct connection to their physical circuit
 *models The binding is done based on the name of directs defined in the
 * OpenFPGA arch XML
 *******************************************************************/
static void annotate_direct_circuit_models(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  size_t count = 0;

  for (int idirect = 0; idirect < vpr_device_ctx.arch->num_directs; ++idirect) {
    std::string direct_name = vpr_device_ctx.arch->Directs[idirect].name;
    /* The name-to-circuit mapping is stored in either cb_switch-to-circuit or
     * sb_switch-to-circuit, Try to find one and update the device annotation
     */
    ArchDirectId direct_id = openfpga_arch.arch_direct.direct(direct_name);
    /* Cannot find a direct, no annotation needed for this direct */
    if (ArchDirectId::INVALID() == direct_id) {
      continue;
    }

    CircuitModelId circuit_model =
      openfpga_arch.arch_direct.circuit_model(direct_id);
    /* Cannot find a circuit model, error out! */
    if (CircuitModelId::INVALID() == circuit_model) {
      VTR_LOG_ERROR(
        "Fail to find a circuit model for a direct connection '%s'!\nPlease "
        "check your OpenFPGA architecture XML!\n",
        direct_name.c_str());
      exit(1);
    }

    /* Check the circuit model type */
    if (openfpga_arch.arch_direct.type(direct_id) !=
          e_direct_type::PART_OF_CB &&
        CIRCUIT_MODEL_WIRE !=
          openfpga_arch.circuit_lib.model_type(circuit_model)) {
      VTR_LOG_ERROR(
        "Require circuit model type '%s' for a direct connection '%s'!\nPlease "
        "check your OpenFPGA architecture XML!\n",
        CIRCUIT_MODEL_TYPE_STRING[CIRCUIT_MODEL_WIRE], direct_name.c_str());
      exit(1);
    }
    if (openfpga_arch.arch_direct.type(direct_id) ==
          e_direct_type::PART_OF_CB &&
        CIRCUIT_MODEL_MUX !=
          openfpga_arch.circuit_lib.model_type(circuit_model)) {
      VTR_LOG_ERROR(
        "Require circuit model type '%s' for a direct connection '%s'!\nPlease "
        "check your OpenFPGA architecture XML!\n",
        CIRCUIT_MODEL_TYPE_STRING[CIRCUIT_MODEL_MUX], direct_name.c_str());
      exit(1);
    }

    /* Now update the device annotation */
    vpr_device_annotation.add_direct_annotation(idirect, direct_id);
    VTR_LOGV(verbose_output,
             "Binded a direct connection '%s' to circuit model '%s'\n",
             direct_name.c_str(),
             openfpga_arch.circuit_lib.model_name(circuit_model).c_str());
    count++;
  }

  VTR_LOG("Binded %lu direct connections to circuit models\n", count);
}

/********************************************************************
 * Build the link between
 * - rr_graph switches
 * - rr_graph segments
 * - directlist
 * to their physical circuit models
 *******************************************************************/
void annotate_rr_graph_circuit_models(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* Iterate over each rr_switch in the device context and bind with names */
  annotate_rr_switch_circuit_models(vpr_device_ctx, openfpga_arch,
                                    vpr_device_annotation, verbose_output);

  /* Iterate over each rr_segment in the device context and bind with names */
  annotate_rr_segment_circuit_models(vpr_device_ctx, openfpga_arch,
                                     vpr_device_annotation, verbose_output);

  /* Iterate over each direct connection in the device context and bind with
   * names */
  annotate_direct_circuit_models(vpr_device_ctx, openfpga_arch,
                                 vpr_device_annotation, verbose_output);
}

} /* end namespace openfpga */
