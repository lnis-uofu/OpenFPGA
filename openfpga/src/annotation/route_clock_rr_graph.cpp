#include "route_clock_rr_graph.h"

#include "command_exit_codes.h"
#include "openfpga_annotate_routing.h"
#include "openfpga_atom_netlist_utils.h"
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
  const std::vector<std::string>& clk_names, const ClockNetwork& clk_ntwk,
  const ClockTreeId clk_tree, const bool& verbose) {
  /* Find the pin id for each clock name, error out if there is any mismatch */
  if (clk_names.size() == 1 && clk_ntwk.tree_width(clk_tree) == 1) {
    /* Find cluster net id */
    ClusterNetId clk_net = cluster_nlist.find_net(clk_names[0]);
    if (!cluster_nlist.valid_net_id(clk_net)) {
      VTR_LOG_ERROR("Invalid clock name '%s'! Cannot found from netlists!\n",
                    clk_names[0].c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    tree2clk_pin_map[ClockTreePinId(0)] = clk_net;
  } else {
    for (std::string clk_name : clk_names) {
      /* Find the pin information that the net should be mapped to */
      BasicPort tree_pin = pin_constraints.net_pin(clk_name);
      if (!tree_pin.is_valid()) {
        VTR_LOG_ERROR(
          "Invalid tree pin for clock '%s'! Clock name may not be valid "
          "(mismatched with netlists)!\n",
          clk_name.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      if (tree_pin.get_width() != 1) {
        VTR_LOG_ERROR(
          "Invalid tree pin %s[%lu:%lu] for clock '%s'! Clock pin must have "
          "only a width of 1!\n",
          tree_pin.get_name().c_str(), tree_pin.get_lsb(), tree_pin.get_msb(),
          clk_name.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      if (tree_pin.get_lsb() >= clk_ntwk.tree_width(clk_tree)) {
        VTR_LOG_ERROR(
          "Invalid tree pin %s[%lu] is out of range of clock tree size '%lu'\n",
          tree_pin.get_name().c_str(), tree_pin.get_lsb(),
          clk_ntwk.tree_width(clk_tree));
        return CMD_EXEC_FATAL_ERROR;
      }
      /* Find cluster net id */
      ClusterNetId clk_net = cluster_nlist.find_net(clk_name);
      if (!cluster_nlist.valid_net_id(clk_net)) {
        VTR_LOG_ERROR("Invalid clock name '%s'! Cannot found from netlists!\n",
                      clk_name.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      /* Register the pin mapping */
      tree2clk_pin_map[ClockTreePinId(tree_pin.get_lsb())] = clk_net;
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
  VTR_LOGV(verbose, "Routing switch points of spine '%s'...\n",
           clk_ntwk.spine_name(ispine).c_str());
  vtr::Point<int> src_coord =
    clk_ntwk.spine_switch_point(ispine, switch_point_id);
  ClockSpineId des_spine =
    clk_ntwk.spine_switch_point_tap(ispine, switch_point_id);
  vtr::Point<int> des_coord = clk_ntwk.spine_start_point(des_spine);
  Direction src_spine_direction = clk_ntwk.spine_direction(ispine);
  Direction des_spine_direction = clk_ntwk.spine_direction(des_spine);
  ClockLevelId src_spine_level = clk_ntwk.spine_level(ispine);
  ClockLevelId des_spine_level = clk_ntwk.spine_level(des_spine);
  RRNodeId src_node =
    clk_rr_lookup.find_node(src_coord.x(), src_coord.y(), clk_tree,
                            src_spine_level, ipin, src_spine_direction);
  RRNodeId des_node =
    clk_rr_lookup.find_node(des_coord.x(), des_coord.y(), clk_tree,
                            des_spine_level, ipin, des_spine_direction);
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
      if (OPIN != rr_graph.node_type(opin_node)) {
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
           "Routed switch points of spine '%s' from (x=%lu, y=%lu) to spine "
           "'%s' at (x=%lu, y=%lu)\n",
           clk_ntwk.spine_name(ispine).c_str(), src_coord.x(), src_coord.y(),
           clk_ntwk.spine_name(des_spine).c_str(), des_coord.x(),
           des_coord.y());
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
  const ClockSpineId& ispine, const ClockTreePinId& ipin, const bool& verbose) {
  std::vector<vtr::Point<int>> spine_coords =
    clk_ntwk.spine_coordinates(ispine);
  size_t spine_tap_cnt = 0;
  /* Route the spine-to-IPIN connections (only for the last level) */
  if (clk_ntwk.is_last_level(ispine)) {
    VTR_LOGV(verbose, "Routing clock taps of spine '%s' for pin '%d' of tree '%s'...\n",
             clk_ntwk.spine_name(ispine).c_str(), size_t(ipin), clk_ntwk.tree_name(clk_tree).c_str());
    /* Connect to any fan-out node which is IPIN */
    for (size_t icoord = 0; icoord < spine_coords.size(); ++icoord) {
      vtr::Point<int> src_coord = spine_coords[icoord];
      Direction src_spine_direction = clk_ntwk.spine_direction(ispine);
      ClockLevelId src_spine_level = clk_ntwk.spine_level(ispine);
      RRNodeId src_node =
        clk_rr_lookup.find_node(src_coord.x(), src_coord.y(), clk_tree,
                                src_spine_level, ipin, src_spine_direction);
      for (RREdgeId edge : rr_graph.edge_range(src_node)) {
        RRNodeId des_node = rr_graph.edge_sink_node(edge);
        if (rr_graph.node_type(des_node) == IPIN) {
          VTR_LOGV(verbose, "Trying to route to IPIN '%s'\n",
                   rr_graph.node_coordinate_to_string(des_node).c_str());
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
  const bool& disable_unused_spines, const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  bool curr_spine_usage = false;
  bool curr_tap_usage = false;
  /* For last level, we just connect tap points */
  status = route_spine_taps(vpr_routing_annotation, curr_tap_usage, rr_graph,
                            clk_rr_lookup, rr_node_gnets, tree2clk_pin_map,
                            clk_ntwk, clk_tree, curr_spine, curr_pin, verbose);
  if (CMD_EXEC_SUCCESS != status) {
    return CMD_EXEC_FATAL_ERROR;
  }
  if (curr_tap_usage) {
    curr_spine_usage = true;
  }

  std::vector<vtr::Point<int>> spine_coords =
    clk_ntwk.spine_coordinates(curr_spine);
  /* We expand from the the ending point to starting point on the straight line.
   * As such, it is easy to turn off spines by any stop.
   * The spine should go in a straight line, connect all the stops on the line
   */
  bool prev_stop_usage = false;
  std::reverse(spine_coords.begin(), spine_coords.end());
  for (size_t icoord = 0; icoord < spine_coords.size(); ++icoord) {
    vtr::Point<int> switch_point_coord = spine_coords[icoord];
    bool curr_stop_usage = false;
    if (icoord == 0) {
      prev_stop_usage = true; /* The first stop is always used */
    }
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
        curr_pin, disable_unused_spines, verbose);
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
    if (disable_unused_spines && !curr_stop_usage && !prev_stop_usage) {
      VTR_LOGV(verbose,
               "Disconnect backbone of spine '%s' at (x=%lu, y=%lu) as "
               "downstream is not used\n",
               clk_ntwk.spine_name(curr_spine).c_str(), switch_point_coord.x(),
               switch_point_coord.y());
      continue;
    }
    /* Skip the first stop */
    if (icoord == spine_coords.size() - 1) {
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
    RRNodeId src_node =
      clk_rr_lookup.find_node(src_coord.x(), src_coord.y(), clk_tree,
                              src_spine_level, curr_pin, src_spine_direction);
    RRNodeId des_node =
      clk_rr_lookup.find_node(des_coord.x(), des_coord.y(), clk_tree,
                              des_spine_level, curr_pin, des_spine_direction);
    VTR_ASSERT(rr_graph.valid_node(src_node));
    VTR_ASSERT(rr_graph.valid_node(des_node));
    VTR_LOGV(verbose,
             "Routed backbone of spine '%s' from (x=%lu, y=%lu) to (x=%lu, "
             "y=%lu)...\n",
             clk_ntwk.spine_name(curr_spine).c_str(), src_coord.x(),
             src_coord.y(), des_coord.x(), des_coord.y());
    vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node, src_node);
    /* It could happen that there is no net mapped some clock pin, skip the
      * net mapping */
     if (tree2clk_pin_map.find(curr_pin) != tree2clk_pin_map.end()) {
       vpr_routing_annotation.set_rr_node_net(src_node, tree2clk_pin_map.at(curr_pin));
       vpr_routing_annotation.set_rr_node_net(des_node, tree2clk_pin_map.at(curr_pin));
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
    /* Start with the top-level spines. Recursively walk through coordinates and
     * expand on switch points */
    bool tree_usage = false;
    for (auto top_spine : clk_ntwk.tree_top_spines(clk_tree)) {
      int status = rec_expand_and_route_clock_spine(
        vpr_routing_annotation, tree_usage, rr_graph, clk_rr_lookup,
        rr_node_gnets, tree2clk_pin_map, clk_ntwk, clk_tree, top_spine, ipin,
        disable_unused_spines, verbose);
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
  const DeviceContext& vpr_device_ctx, const AtomContext& atom_ctx,
  const ClusteredNetlist& cluster_nlist, const PlacementContext& vpr_place_ctx,
  const VprNetlistAnnotation& netlist_annotation,
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

  /* If there are multiple clock signals from the netlist, require pin
   * constraints */
  std::vector<std::string> clock_net_names =
    find_atom_netlist_clock_port_names(atom_ctx.nlist, netlist_annotation);
  if (clock_net_names.empty()) {
    VTR_LOG(
      "Skip due to 0 clocks found from netlist\nDouble check your HDL design "
      "if this is unexpected\n");
    return CMD_EXEC_SUCCESS;
  }
  if (clock_net_names.size() > 1 && pin_constraints.empty()) {
    VTR_LOG(
      "There is %lu clock nets (more than 1). Require pin constraints to be "
      "specified\n",
      clock_net_names.size());
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Build rr_node-to-net mapping for global nets */
  vtr::vector<RRNodeId, ClusterNetId> rr_node_gnets =
    annotate_rr_node_global_net(vpr_device_ctx, cluster_nlist, vpr_place_ctx,
                                verbose);

  /* Route spines one by one */
  for (auto itree : clk_ntwk.trees()) {
    VTR_LOGV(verbose, "Build clock name to clock tree '%s' pin mapping...\n",
             clk_ntwk.tree_name(itree).c_str());
    std::map<ClockTreePinId, ClusterNetId> tree2clk_pin_map;
    int status = CMD_EXEC_SUCCESS;
    status =
      build_clock_tree_net_map(tree2clk_pin_map, cluster_nlist, pin_constraints,
                               clock_net_names, clk_ntwk, itree, verbose);
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }

    VTR_LOGV(verbose, "Routing clock tree '%s'...\n",
             clk_ntwk.tree_name(itree).c_str());
    status = route_clock_tree_rr_graph(
      vpr_routing_annotation, vpr_device_ctx.rr_graph, clk_rr_lookup,
      rr_node_gnets, tree2clk_pin_map, clk_ntwk, itree, disable_unused_trees,
      disable_unused_spines, verbose);
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }
    VTR_LOGV(verbose, "Done\n");
  }

  /* TODO: Sanity checks */

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
