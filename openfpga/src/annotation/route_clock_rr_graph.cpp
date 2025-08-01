#include "route_clock_rr_graph.h"

#include "command_exit_codes.h"
#include "openfpga_annotate_routing.h"
#include "openfpga_clustered_netlist_utils.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Build the lookup between clock name and pins and clock tree pins
 * This is required for routing clock nets in each clock tree
 * Special: when there is only 1 clock and 1 clock tree (width = 1), the mapping
 *is straight forward
 * FIXME: This part works only for clock network which constains only 1 clock
 *tree!
 *******************************************************************/
static int build_clock_tree_net_map(
  std::map<ClockTreePinId, ClusterNetId>& tree2clk_pin_map,
  const ClusteredNetlist& cluster_nlist, const PinConstraints& pin_constraints,
  const std::vector<ClusterNetId>& gnets, const ClockNetwork& clk_ntwk,
  const ClockTreeId clk_tree, const bool& verbose) {
  BasicPort tree_gport = clk_ntwk.tree_global_port(clk_tree);
  /* Find the pin id for each clock name, error out if there is any mismatch */
  if (clk_ntwk.num_trees() == 1 && gnets.size() == 1 &&
      clk_ntwk.tree_width(clk_tree) == 1) {
    /* Find cluster net id */
    if (!cluster_nlist.valid_net_id(gnets[0])) {
      VTR_LOG_ERROR("Invalid clock name '%s'! Cannot be found from netlists!\n",
                    cluster_nlist.net_name(gnets[0]).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    tree2clk_pin_map[ClockTreePinId(0)] = gnets[0];
  } else {
    for (ClusterNetId gnet : gnets) {
      /* Find the pin information that the net should be mapped to */
      std::string gnet_name = cluster_nlist.net_name(gnet);
      /* The pin should match be global port name of the tree */
      BasicPort tree_pin = pin_constraints.net_pin(gnet_name);
      if (!tree_pin.is_valid()) {
        VTR_LOG_ERROR(
          "Global net '%s' is not mapped to a valid pin '%s' in pin "
          "constraints!\n",
          gnet_name.c_str(), tree_pin.to_verilog_string().c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      if (tree_pin.get_width() != 1) {
        VTR_LOG_ERROR(
          "Invalid tree pin %s[%lu:%lu] for clock '%s'! Clock pin must have "
          "only a width of 1!\n",
          tree_pin.get_name().c_str(), tree_pin.get_lsb(), tree_pin.get_msb(),
          gnet_name.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      if (tree_gport.get_name() != tree_pin.get_name()) {
        continue;
      }
      if (!tree_gport.contained(tree_pin)) {
        VTR_LOG_ERROR(
          "Invalid pin constraint port '%s' which is out of range of the "
          "global port '%s' of clock tree '%s'\n",
          tree_pin.to_verilog_string().c_str(),
          tree_gport.to_verilog_string().c_str(),
          clk_ntwk.tree_name(clk_tree).c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      /* TODO: Check the tree_pin.get_name(), see if matches the tree from ports
       */
      /* Register the pin mapping */
      tree2clk_pin_map[ClockTreePinId(tree_pin.get_lsb())] = gnet;
      VTR_LOGV(verbose, "Mapped net '%s' to pin '%s' of clock tree '%s'.\n",
               gnet_name.c_str(), tree_pin.to_verilog_string().c_str(),
               clk_ntwk.tree_name(clk_tree).c_str());
    }
  }

  VTR_LOGV(verbose, "Build a pin map for %lu clock nets and pins.\n",
           tree2clk_pin_map.size());

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Route a switching points between spines
 * - connect between two routing tracks (left or right turns)
 * - connect internal driver to routing track
 *******************************************************************/
static int route_clock_spine_switch_point(
  VprRoutingAnnotation& vpr_routing_annotation, const RRGraphView& rr_graph,
  const RRClockSpatialLookup& clk_rr_lookup,
  const vtr::vector<RRNodeId, ClusterNetId>& rr_node_gnets,
  const std::map<ClockTreePinId, ClusterNetId>& tree2clk_pin_map,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const ClockSpineId& ispine, const ClockTreePinId& ipin,
  const ClockSwitchPointId& switch_point_id, const bool& verbose) {
  vtr::Point<int> src_coord =
    clk_ntwk.spine_switch_point(ispine, switch_point_id);
  ClockSpineId des_spine =
    clk_ntwk.spine_switch_point_tap(ispine, switch_point_id);
  vtr::Point<int> des_coord = clk_ntwk.spine_start_point(des_spine);
  VTR_LOGV(verbose, "Routing switch points from spine '%s' to spine '%s'...\n",
           clk_ntwk.spine_name(ispine).c_str(),
           clk_ntwk.spine_name(des_spine).c_str());
  Direction src_spine_direction = clk_ntwk.spine_direction(ispine);
  Direction des_spine_direction = clk_ntwk.spine_direction(des_spine);
  ClockLevelId src_spine_level = clk_ntwk.spine_level(ispine);
  ClockLevelId des_spine_level = clk_ntwk.spine_level(des_spine);
  /* Special for DEC_DIR CHANX and CHANY, there should be a offset on the source
   * coordinate Note that in the following condition, the switching point occurs
   * in switch block (x, y) INC CHANY (x, y + 1)
   *                      ^
   *                      |
   * INC CHANX (x, y) --->+<---- DEC CHANX (x + 1, y)
   *                      |
   *                      v
   *              DEC CHANY (x, y)
   *
   * Note that in the following condition, the switching point occurs in switch
   * block (x, y) DEC CHANY (x, y + 1)
   *                      |
   *                      v
   * DEC CHANX (x, y) <---+----> INC CHANX (x + 1, y)
   *                      ^
   *                      |
   *              INC CHANY (x, y)
   * From the user point of view, the switching point should only occur in a
   * switch block So the coordinate of a switch block should be provided as the
   * coordinate of switching point However, the src node and des node may not
   * follow the switch block coordinate! In short, the src coordinate requires
   * an adjustment only when
   * - The src is an CHANX in DEC
   * - The src is an CHANY in DEC
   * No adjustment is required for des node as it always comes from the starting
   * point of the des spine
   */
  if (clk_ntwk.spine_track_type(ispine) == e_rr_type::CHANX &&
      src_spine_direction == Direction::DEC) {
    src_coord.set_x(des_coord.x() + 1);
  }
  if (clk_ntwk.spine_track_type(ispine) == e_rr_type::CHANY &&
      src_spine_direction == Direction::DEC) {
    src_coord.set_y(des_coord.y() + 1);
  }
  RRNodeId src_node = clk_rr_lookup.find_node(src_coord.x(), src_coord.y(),
                                              clk_tree, src_spine_level, ipin,
                                              src_spine_direction, verbose);
  RRNodeId des_node = clk_rr_lookup.find_node(des_coord.x(), des_coord.y(),
                                              clk_tree, des_spine_level, ipin,
                                              des_spine_direction, verbose);
  VTR_ASSERT(rr_graph.valid_node(src_node));
  VTR_ASSERT(rr_graph.valid_node(des_node));
  /* Internal drivers may appear at the switch point. Check if there are
   * any defined and related rr_node found as incoming edges. If the
   * global net is mapped to the internal driver, use it as the previous
   * node  */
  size_t use_int_driver = 0;
  if (!clk_ntwk.spine_switch_point_internal_drivers(ispine, switch_point_id)
         .empty() &&
      tree2clk_pin_map.find(ipin) != tree2clk_pin_map.end()) {
    for (RREdgeId cand_edge : rr_graph.node_in_edges(des_node)) {
      RRNodeId opin_node = rr_graph.edge_src_node(cand_edge);
      if (e_rr_type::OPIN != rr_graph.node_type(opin_node)) {
        continue;
      }
      if (rr_node_gnets[opin_node] != tree2clk_pin_map.at(ipin)) {
        continue;
      }
      /* This is the opin node we need, use it as the internal driver */
      vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                   opin_node);
      vpr_routing_annotation.set_rr_node_net(opin_node,
                                             tree2clk_pin_map.at(ipin));
      vpr_routing_annotation.set_rr_node_net(des_node,
                                             tree2clk_pin_map.at(ipin));
      use_int_driver++;
      VTR_LOGV(verbose,
               "Routed switch points of spine '%s' at the switching point "
               "(%lu, %lu) using internal driver\n",
               clk_ntwk.spine_name(ispine).c_str(), src_coord.x(),
               src_coord.y());
    }
  }
  if (use_int_driver > 1) {
    VTR_LOG_ERROR(
      "Found %lu internal drivers for the switching point (%lu, %lu) for "
      "spine '%s'!\n Expect only 1!\n",
      use_int_driver, src_coord.x(), src_coord.y(),
      clk_ntwk.spine_name(ispine).c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (use_int_driver == 1) {
    return CMD_EXEC_SUCCESS; /* Used internal driver, early pass */
  }
  VTR_LOGV(verbose,
           "Routed switch points of spine '%s' (node '%lu') from (x=%lu, "
           "y=%lu) to spine "
           "'%s' (node '%lu') at (x=%lu, y=%lu)\n",
           clk_ntwk.spine_name(ispine).c_str(), size_t(src_node), src_coord.x(),
           src_coord.y(), clk_ntwk.spine_name(des_spine).c_str(),
           size_t(des_node), des_coord.x(), des_coord.y());
  vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node, src_node);
  /* It could happen that there is no net mapped some clock pin, skip the
   * net mapping */
  if (tree2clk_pin_map.find(ipin) != tree2clk_pin_map.end()) {
    vpr_routing_annotation.set_rr_node_net(src_node, tree2clk_pin_map.at(ipin));
    vpr_routing_annotation.set_rr_node_net(des_node, tree2clk_pin_map.at(ipin));
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Route a spine to its tap points
 * - Only connect to tap points which are mapped by a global net
 *******************************************************************/
static int route_spine_taps(
  VprRoutingAnnotation& vpr_routing_annotation, bool& spine_usage,
  const RRGraphView& rr_graph, const RRClockSpatialLookup& clk_rr_lookup,
  const vtr::vector<RRNodeId, ClusterNetId>& rr_node_gnets,
  const std::map<ClockTreePinId, ClusterNetId>& tree2clk_pin_map,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const ClockSpineId& ispine, const ClockTreePinId& ipin,
  const bool& force_tap_routing, const bool& verbose) {
  size_t spine_tap_cnt = 0;
  /* Route the spine-to-IPIN connections (only for the last level) */
  if (clk_ntwk.is_last_level(ispine)) {
    VTR_LOGV(verbose,
             "Routing clock taps of spine '%s' for pin '%d' of tree '%s'...\n",
             clk_ntwk.spine_name(ispine).c_str(), size_t(ipin),
             clk_ntwk.tree_name(clk_tree).c_str());
    std::vector<vtr::Point<int>> spine_coords =
      clk_ntwk.spine_coordinates(ispine);
    /* Connect to any fan-out node which is IPIN */
    for (size_t icoord = 0; icoord < spine_coords.size(); ++icoord) {
      vtr::Point<int> src_coord = spine_coords[icoord];
      Direction src_spine_direction = clk_ntwk.spine_direction(ispine);
      ClockLevelId src_spine_level = clk_ntwk.spine_level(ispine);
      RRNodeId src_node = clk_rr_lookup.find_node(
        src_coord.x(), src_coord.y(), clk_tree, src_spine_level, ipin,
        src_spine_direction, verbose);
      for (RREdgeId edge : rr_graph.edge_range(src_node)) {
        RRNodeId des_node = rr_graph.edge_sink_node(edge);
        if (rr_graph.node_type(des_node) == e_rr_type::IPIN) {
          VTR_LOGV(verbose, "Trying to route to IPIN '%s'\n",
                   rr_graph.node_coordinate_to_string(des_node).c_str());
          /* If all the tap is force to route, do it as the highest priority */
          if (force_tap_routing) {
            VTR_ASSERT(rr_graph.valid_node(src_node));
            VTR_ASSERT(rr_graph.valid_node(des_node));
            VTR_LOGV(verbose, "Forcec to routed clock tap of spine '%s'\n",
                     clk_ntwk.spine_name(ispine).c_str());
            vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                         src_node);
            /* If a net is mapped to the pin, still mark it */
            if (tree2clk_pin_map.find(ipin) != tree2clk_pin_map.end()) {
              vpr_routing_annotation.set_rr_node_net(src_node,
                                                     tree2clk_pin_map.at(ipin));
              vpr_routing_annotation.set_rr_node_net(des_node,
                                                     tree2clk_pin_map.at(ipin));
            }
            continue;
          }
          /* Check if the IPIN is mapped, if not, do not connect */
          /* if the IPIN is mapped, only connect when net mapping is
           * expected */
          if (tree2clk_pin_map.find(ipin) == tree2clk_pin_map.end()) {
            VTR_LOGV(verbose,
                     "Skip routing clock tap of spine '%s' as the tree is "
                     "not used\n",
                     clk_ntwk.spine_name(ispine).c_str());
            continue;
          }
          if (!rr_node_gnets[des_node]) {
            VTR_LOGV(verbose,
                     "Skip routing clock tap of spine '%s' as the IPIN is "
                     "not mapped\n",
                     clk_ntwk.spine_name(ispine).c_str());
            continue;
          }
          if (rr_node_gnets[des_node] != tree2clk_pin_map.at(ipin)) {
            VTR_LOGV(verbose,
                     "Skip routing clock tap of spine '%s' as the net "
                     "mapping does not match clock net\n",
                     clk_ntwk.spine_name(ispine).c_str());
            continue;
          }
          VTR_ASSERT(rr_graph.valid_node(src_node));
          VTR_ASSERT(rr_graph.valid_node(des_node));
          VTR_LOGV(verbose, "Routed clock tap of spine '%s'\n",
                   clk_ntwk.spine_name(ispine).c_str());
          vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                       src_node);
          vpr_routing_annotation.set_rr_node_net(src_node,
                                                 tree2clk_pin_map.at(ipin));
          vpr_routing_annotation.set_rr_node_net(des_node,
                                                 tree2clk_pin_map.at(ipin));
          /* Increment upon any required tap */
          spine_tap_cnt++;
        }
      }
    }
  }
  if (spine_tap_cnt) {
    spine_usage = true;
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Recursively route a clock spine on an existing routing resource graph
 *******************************************************************/
static int route_spine_intermediate_drivers(
  VprRoutingAnnotation& vpr_routing_annotation, const RRGraphView& rr_graph,
  const RRClockSpatialLookup& clk_rr_lookup,
  const vtr::vector<RRNodeId, ClusterNetId>& rr_node_gnets,
  const std::map<ClockTreePinId, ClusterNetId>& tree2clk_pin_map,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const ClockSpineId& curr_spine, const ClockTreePinId& curr_pin,
  const vtr::Point<int>& des_coord, const bool& verbose) {
  Direction des_spine_direction = clk_ntwk.spine_direction(curr_spine);
  ClockLevelId des_spine_level = clk_ntwk.spine_level(curr_spine);
  RRNodeId des_node = clk_rr_lookup.find_node(
    des_coord.x(), des_coord.y(), clk_tree, des_spine_level, curr_pin,
    des_spine_direction, verbose);
  VTR_ASSERT(rr_graph.valid_node(des_node));

  /* Internal drivers may appear at the intermediate. Check if there are
   * any defined and related rr_node found as incoming edges. If the
   * global net is mapped to the internal driver, use it as the previous
   * node  */
  size_t use_int_driver = 0;
  if (!clk_ntwk
         .spine_intermediate_drivers_by_routing_track(curr_spine, des_coord)
         .empty() &&
      tree2clk_pin_map.find(curr_pin) != tree2clk_pin_map.end()) {
    VTR_LOGV(
      verbose, "Finding intermediate drivers at (%d, %d) for spine '%s'\n",
      des_coord.x(), des_coord.y(), clk_ntwk.spine_name(curr_spine).c_str());
    for (RREdgeId cand_edge : rr_graph.node_in_edges(des_node)) {
      RRNodeId opin_node = rr_graph.edge_src_node(cand_edge);
      if (e_rr_type::OPIN != rr_graph.node_type(opin_node)) {
        continue;
      }
      if (rr_node_gnets[opin_node] != tree2clk_pin_map.at(curr_pin)) {
        continue;
      }
      /* This is the opin node we need, use it as the internal driver */
      vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                   opin_node);
      vpr_routing_annotation.set_rr_node_net(opin_node,
                                             tree2clk_pin_map.at(curr_pin));
      vpr_routing_annotation.set_rr_node_net(des_node,
                                             tree2clk_pin_map.at(curr_pin));
      use_int_driver++;
      VTR_LOGV(verbose,
               "Routed intermediate point of spine '%s' at "
               "(%lu, %lu) using internal driver\n",
               clk_ntwk.spine_name(curr_spine).c_str(), des_coord.x(),
               des_coord.y());
    }
  }
  if (use_int_driver > 1) {
    VTR_LOG_ERROR(
      "Found %lu internal drivers for the intermediate point (%lu, %lu) "
      "for "
      "spine '%s'!\n Expect only 1!\n",
      use_int_driver, des_coord.x(), des_coord.y(),
      clk_ntwk.spine_name(curr_spine).c_str());
  }
  return use_int_driver;
}

/********************************************************************
 * Recursively route a clock spine on an existing routing resource graph
 * The strategy is to route spine one by one
 * - route the spine from the ending point to starting point (straight line)
 * - for each stops on the staight line, route the spine-to-spine switching
 points
 * - for each switching point (des_spine_top|bottom), go recursively
 * - If the downstream spine at any switching point is not used, disconnect
 * - If any stop on the spine (straght line) is not used, disconnect
 * - route the spine-to-IPIN connections (only for the last level)
 *
 *                des_spine_top[0...N]
 *                   ^     ^     ^     ^
 *                   |     |     |     |
 *  spine_start ---->+---->+---->+---->+->spine_end
 *                   |     |     |     |
 *                   v     v     v     v
 *                des_spine_bottom[0...N]
 *
 *  <-------------------------------------------- direction to walk through
 *
 *
 *  On each stop, we expand the spine to switch points and tap points
 *  - If the previous stop is used (connection to des_spines are required), then
 the current stop should be connected to the previous stop
 *  - If previous stop is not used, while the des_spines are required to
 connect, then the current stop should be connected to the previous stop
 *  - Only when previous stops and des_spines are not used, the current stop
 will be NOT connected to the previous stop
 *
 *                    des_spine_top[i]
 *                       ^
 *                       |
 *  spine_curr_stop ---->+->spine_prev_stop
 *                       |
 *                       v
 *                    des_spine_bottom[i]

 *
 *******************************************************************/
static int rec_expand_and_route_clock_spine(
  VprRoutingAnnotation& vpr_routing_annotation, bool& spine_usage,
  const RRGraphView& rr_graph, const RRClockSpatialLookup& clk_rr_lookup,
  const vtr::vector<RRNodeId, ClusterNetId>& rr_node_gnets,
  const std::map<ClockTreePinId, ClusterNetId>& tree2clk_pin_map,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const ClockSpineId& curr_spine, const ClockTreePinId& curr_pin,
  const bool& disable_unused_spines, const bool& force_tap_routing,
  const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  bool curr_spine_usage = false;
  bool curr_tap_usage = false;
  /* For last level, we just connect tap points */
  status = route_spine_taps(vpr_routing_annotation, curr_tap_usage, rr_graph,
                            clk_rr_lookup, rr_node_gnets, tree2clk_pin_map,
                            clk_ntwk, clk_tree, curr_spine, curr_pin,
                            force_tap_routing, verbose);
  if (CMD_EXEC_SUCCESS != status) {
    return CMD_EXEC_FATAL_ERROR;
  }
  /* If no taps are routed, this spine is not used. Early exit */
  if (disable_unused_spines && !curr_tap_usage &&
      clk_ntwk.is_last_level(curr_spine)) {
    spine_usage = false;
    VTR_LOGV(verbose,
             "Disable last-level spine '%s' as "
             "none of the taps are not used\n",
             clk_ntwk.spine_name(curr_spine).c_str());
    return CMD_EXEC_SUCCESS;
  }

  std::vector<vtr::Point<int>> spine_coords =
    clk_ntwk.spine_coordinates(curr_spine);
  /* We expand from the the ending point to starting point on the straight line.
   * As such, it is easy to turn off spines by any stop.
   * The spine should go in a straight line, connect all the stops on the line
   */
  bool prev_stop_usage = false;
  if (clk_ntwk.is_last_level(curr_spine)) {
    curr_spine_usage = curr_tap_usage;
    prev_stop_usage = curr_tap_usage;
  }
  std::reverse(spine_coords.begin(), spine_coords.end());
  for (size_t icoord = 0; icoord < spine_coords.size(); ++icoord) {
    vtr::Point<int> switch_point_coord = spine_coords[icoord];
    bool curr_stop_usage = false;
    /* Expand on the switching point here */
    for (ClockSwitchPointId switch_point_id :
         clk_ntwk.find_spine_switch_points_with_coord(curr_spine,
                                                      switch_point_coord)) {
      ClockSpineId des_spine =
        clk_ntwk.spine_switch_point_tap(curr_spine, switch_point_id);
      /* Go recursively for the destination spine */
      bool curr_branch_usage = false;
      status = rec_expand_and_route_clock_spine(
        vpr_routing_annotation, curr_branch_usage, rr_graph, clk_rr_lookup,
        rr_node_gnets, tree2clk_pin_map, clk_ntwk, clk_tree, des_spine,
        curr_pin, disable_unused_spines, force_tap_routing, verbose);
      if (CMD_EXEC_SUCCESS != status) {
        return CMD_EXEC_FATAL_ERROR;
      }
      /* Connect only when the destination spine is used */
      if (disable_unused_spines && !curr_branch_usage) {
        VTR_LOGV(verbose,
                 "Disconnect switching from spine '%s' to spine '%s' as "
                 "downstream is not used\n",
                 clk_ntwk.spine_name(curr_spine).c_str(),
                 clk_ntwk.spine_name(des_spine).c_str());
        continue;
      }
      curr_stop_usage = true;
      /* Now connect to next spine, internal drivers may join */
      status = route_clock_spine_switch_point(
        vpr_routing_annotation, rr_graph, clk_rr_lookup, rr_node_gnets,
        tree2clk_pin_map, clk_ntwk, clk_tree, curr_spine, curr_pin,
        switch_point_id, verbose);
      if (CMD_EXEC_SUCCESS != status) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
    if (curr_stop_usage) {
      curr_spine_usage = true;
    }
    if (disable_unused_spines && !curr_stop_usage && !prev_stop_usage) {
      VTR_LOGV(verbose,
               "Disconnect backbone of spine '%s' at (x=%lu, y=%lu) as "
               "downstream is not used\n",
               clk_ntwk.spine_name(curr_spine).c_str(), switch_point_coord.x(),
               switch_point_coord.y());
      continue;
    }
    /* If there are any stop is used, mark this spine is used. This is to avoid
     * that a spine is marked unused when only its 1st stop is actually used.
     * The skip condition may cause this. */
    /* Skip the first stop */
    if (icoord == spine_coords.size() - 1) {
      vtr::Point<int> des_coord = spine_coords[icoord];

      int use_int_driver = route_spine_intermediate_drivers(
        vpr_routing_annotation, rr_graph, clk_rr_lookup, rr_node_gnets,
        tree2clk_pin_map, clk_ntwk, clk_tree, curr_spine, curr_pin, des_coord,
        verbose);
      if (use_int_driver > 1) {
        return CMD_EXEC_FATAL_ERROR;
      }
      continue;
    }
    /* Connect only when next stop is used */
    vtr::Point<int> src_coord = spine_coords[icoord + 1];
    vtr::Point<int> des_coord = spine_coords[icoord];
    VTR_LOGV(verbose,
             "(icoord=%lu) Expanding on backbone of spine '%s' from (x=%lu, "
             "y=%lu) to (x=%lu, y=%lu)...\n",
             icoord, clk_ntwk.spine_name(curr_spine).c_str(), src_coord.x(),
             src_coord.y(), des_coord.x(), des_coord.y());
    Direction src_spine_direction = clk_ntwk.spine_direction(curr_spine);
    Direction des_spine_direction = clk_ntwk.spine_direction(curr_spine);
    ClockLevelId src_spine_level = clk_ntwk.spine_level(curr_spine);
    ClockLevelId des_spine_level = clk_ntwk.spine_level(curr_spine);
    RRNodeId src_node = clk_rr_lookup.find_node(
      src_coord.x(), src_coord.y(), clk_tree, src_spine_level, curr_pin,
      src_spine_direction, verbose);
    RRNodeId des_node = clk_rr_lookup.find_node(
      des_coord.x(), des_coord.y(), clk_tree, des_spine_level, curr_pin,
      des_spine_direction, verbose);
    VTR_ASSERT(rr_graph.valid_node(src_node));
    VTR_ASSERT(rr_graph.valid_node(des_node));

    /* Internal drivers may appear at the intermediate. Check if there are
     * any defined and related rr_node found as incoming edges. If the
     * global net is mapped to the internal driver, use it as the previous
     * node  */
    int use_int_driver = route_spine_intermediate_drivers(
      vpr_routing_annotation, rr_graph, clk_rr_lookup, rr_node_gnets,
      tree2clk_pin_map, clk_ntwk, clk_tree, curr_spine, curr_pin, des_coord,
      verbose);
    if (use_int_driver > 1) {
      return CMD_EXEC_FATAL_ERROR;
    }
    if (use_int_driver == 1) {
      continue; /* Used internal driver, early pass. */
    }

    VTR_LOGV(verbose,
             "Routed backbone of spine '%s' from (x=%lu, y=%lu) to (x=%lu, "
             "y=%lu)...\n",
             clk_ntwk.spine_name(curr_spine).c_str(), src_coord.x(),
             src_coord.y(), des_coord.x(), des_coord.y());
    vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node, src_node);
    /* It could happen that there is no net mapped some clock pin, skip the
     * net mapping */
    if (tree2clk_pin_map.find(curr_pin) != tree2clk_pin_map.end()) {
      vpr_routing_annotation.set_rr_node_net(src_node,
                                             tree2clk_pin_map.at(curr_pin));
      vpr_routing_annotation.set_rr_node_net(des_node,
                                             tree2clk_pin_map.at(curr_pin));
    }

    prev_stop_usage = true;
    curr_spine_usage = true;
  }
  /* Update status */
  spine_usage = curr_spine_usage;

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Route a clock tree on an existing routing resource graph
 * The strategy is to route spine one by one
 * - route the spine from the starting point to the ending point
 * - route the spine-to-spine switching points
 * - route the spine-to-IPIN connections (only for the last level)
 *******************************************************************/
static int route_clock_tree_rr_graph(
  VprRoutingAnnotation& vpr_routing_annotation, const RRGraphView& rr_graph,
  const VprBitstreamAnnotation& vpr_bitstream_annotation,
  const RRClockSpatialLookup& clk_rr_lookup,
  const vtr::vector<RRNodeId, ClusterNetId>& rr_node_gnets,
  const std::map<ClockTreePinId, ClusterNetId>& tree2clk_pin_map,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const bool& disable_unused_trees, const bool& disable_unused_spines,
  const bool& verbose) {
  for (auto ipin : clk_ntwk.pins(clk_tree)) {
    /* Do not route unused clock spines */
    if (disable_unused_trees &&
        tree2clk_pin_map.find(ipin) == tree2clk_pin_map.end()) {
      VTR_LOGV(verbose, "Skip routing unused tree '%s' pin '%lu'...\n",
               clk_ntwk.tree_name(clk_tree).c_str(), size_t(ipin));
      continue;
    }
    /* Mark if tap point should be all routed regardless of usage (net mapping)
     */
    bool force_tap_routing = false;
    if (ipin == vpr_bitstream_annotation.clock_tap_routing_pin(clk_tree)) {
      force_tap_routing = true;
    }
    /* Start with the top-level spines. Recursively walk through coordinates and
     * expand on switch points */
    bool tree_usage = false;
    for (auto top_spine : clk_ntwk.tree_top_spines(clk_tree)) {
      int status = rec_expand_and_route_clock_spine(
        vpr_routing_annotation, tree_usage, rr_graph, clk_rr_lookup,
        rr_node_gnets, tree2clk_pin_map, clk_ntwk, clk_tree, top_spine, ipin,
        disable_unused_spines, force_tap_routing, verbose);
      if (CMD_EXEC_SUCCESS != status) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
    if (!tree_usage) {
      VTR_LOGV(verbose, "Detect unused tree '%s' pin '%lu'...\n",
               clk_ntwk.tree_name(clk_tree).c_str(), size_t(ipin));
    }
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Route a clock network based on an existing routing resource graph
 * This function will do the following jobs:
 * - configure the routing annotation w.r.t. the clock node connections
 * - quick check to ensure routing is valid
 *******************************************************************/
int route_clock_rr_graph(
  VprRoutingAnnotation& vpr_routing_annotation,
  const VprClusteringAnnotation& vpr_clustering_annotation,
  const DeviceContext& vpr_device_ctx, const ClusteredNetlist& cluster_nlist,
  const PlacementContext& vpr_place_ctx,
  const VprBitstreamAnnotation& vpr_bitstream_annotation,
  const RRClockSpatialLookup& clk_rr_lookup, const ClockNetwork& clk_ntwk,
  const PinConstraints& pin_constraints, const bool& disable_unused_trees,
  const bool& disable_unused_spines, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Route programmable clock network based on routing resource graph");

  /* Skip if there is no clock tree */
  if (0 == clk_ntwk.num_trees()) {
    VTR_LOG(
      "Skip due to 0 clock trees.\nDouble check your clock architecture "
      "definition if this is unexpected\n");
    return CMD_EXEC_SUCCESS;
  }

  /* If there are multiple global signals from the netlist, require pin
   * constraints */
  std::vector<ClusterNetId> gnets =
    find_clustered_netlist_global_nets(cluster_nlist);
  if (gnets.empty()) {
    VTR_LOG(
      "Skip due to 0 global nets found from netlist\nDouble check your HDL "
      "design "
      "if this is unexpected\n");
    return CMD_EXEC_SUCCESS;
  }
  if (gnets.size() > 1 && pin_constraints.empty()) {
    VTR_LOG(
      "There is %lu global nets (more than 1). Require pin constraints to be "
      "specified\n",
      gnets.size());
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Build rr_node-to-net mapping for global nets */
  vtr::vector<RRNodeId, ClusterNetId> rr_node_gnets =
    annotate_rr_node_global_net(vpr_device_ctx, cluster_nlist, vpr_place_ctx,
                                vpr_clustering_annotation, verbose);

  /* Route spines one by one */
  for (auto itree : clk_ntwk.trees()) {
    VTR_LOGV(verbose,
             "Build global net name to clock tree '%s' pin mapping...\n",
             clk_ntwk.tree_name(itree).c_str());
    std::map<ClockTreePinId, ClusterNetId> tree2clk_pin_map;
    int status = CMD_EXEC_SUCCESS;
    status =
      build_clock_tree_net_map(tree2clk_pin_map, cluster_nlist, pin_constraints,
                               gnets, clk_ntwk, itree, verbose);
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }

    VTR_LOGV(verbose, "Routing clock tree '%s'...\n",
             clk_ntwk.tree_name(itree).c_str());
    status = route_clock_tree_rr_graph(
      vpr_routing_annotation, vpr_device_ctx.rr_graph, vpr_bitstream_annotation,
      clk_rr_lookup, rr_node_gnets, tree2clk_pin_map, clk_ntwk, itree,
      disable_unused_trees, disable_unused_spines, verbose);
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }
    VTR_LOGV(verbose, "Done\n");
  }

  /* TODO: Sanity checks */

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
