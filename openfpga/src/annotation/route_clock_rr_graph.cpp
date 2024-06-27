#include "route_clock_rr_graph.h"

#include "command_exit_codes.h"
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
 * Route a clock tree on an existing routing resource graph
 * The strategy is to route spine one by one
 * - route the spine from the starting point to the ending point
 * - route the spine-to-spine switching points
 * - route the spine-to-IPIN connections (only for the last level)
 *******************************************************************/
static int route_clock_tree_rr_graph(
  VprRoutingAnnotation& vpr_routing_annotation, const RRGraphView& rr_graph,
  const RRClockSpatialLookup& clk_rr_lookup,
  const std::map<ClockTreePinId, ClusterNetId>& tree2clk_pin_map,
  const ClockNetwork& clk_ntwk, const ClockTreeId& clk_tree,
  const bool& verbose) {
  for (auto ispine : clk_ntwk.spines(clk_tree)) {
    VTR_LOGV(verbose, "Routing spine '%s'...\n",
             clk_ntwk.spine_name(ispine).c_str());
    for (auto ipin : clk_ntwk.pins(clk_tree)) {
      /* Do not route unused clock spines */
      if (tree2clk_pin_map.find(ipin) == tree2clk_pin_map.end()) {
        continue;
      }
      /* Route the spine from starting point to ending point */
      std::vector<vtr::Point<int>> spine_coords =
        clk_ntwk.spine_coordinates(ispine);
      VTR_LOGV(verbose, "Routing backbone of spine '%s'...\n",
               clk_ntwk.spine_name(ispine).c_str());
      for (size_t icoord = 0; icoord < spine_coords.size() - 1; ++icoord) {
        vtr::Point<int> src_coord = spine_coords[icoord];
        vtr::Point<int> des_coord = spine_coords[icoord + 1];
        Direction src_spine_direction = clk_ntwk.spine_direction(ispine);
        Direction des_spine_direction = clk_ntwk.spine_direction(ispine);
        ClockLevelId src_spine_level = clk_ntwk.spine_level(ispine);
        ClockLevelId des_spine_level = clk_ntwk.spine_level(ispine);
        RRNodeId src_node =
          clk_rr_lookup.find_node(src_coord.x(), src_coord.y(), clk_tree,
                                  src_spine_level, ipin, src_spine_direction);
        RRNodeId des_node =
          clk_rr_lookup.find_node(des_coord.x(), des_coord.y(), clk_tree,
                                  des_spine_level, ipin, des_spine_direction);
        VTR_ASSERT(rr_graph.valid_node(src_node));
        VTR_ASSERT(rr_graph.valid_node(des_node));
        vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                     src_node);
      }
      /* Route the spine-to-spine switching points */
      VTR_LOGV(verbose, "Routing switch points of spine '%s'...\n",
               clk_ntwk.spine_name(ispine).c_str());
      for (ClockSwitchPointId switch_point_id :
           clk_ntwk.spine_switch_points(ispine)) {
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
        vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                     src_node);
        /* It could happen that there is no net mapped some clock pin, skip the
         * net mapping */
        if (tree2clk_pin_map.find(ipin) != tree2clk_pin_map.end()) {
          vpr_routing_annotation.set_rr_node_net(src_node,
                                                 tree2clk_pin_map.at(ipin));
          vpr_routing_annotation.set_rr_node_net(des_node,
                                                 tree2clk_pin_map.at(ipin));
        }
      }
      /* Route the spine-to-IPIN connections (only for the last level) */
      if (clk_ntwk.is_last_level(ispine)) {
        VTR_LOGV(verbose, "Routing clock taps of spine '%s'...\n",
                 clk_ntwk.spine_name(ispine).c_str());
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
              /* Check if the IPIN is mapped, if not, do not connect */ 
              /* if the IPIN is mapped, only connect when net mapping is expected */ 
              if (tree2clk_pin_map.find(ipin) == tree2clk_pin_map.end()) {
                continue;
              }
              if (!vpr_routing_annotation.rr_node_net(des_node)) {
                continue;
              }
              if (vpr_routing_annotation.rr_node_net(des_node) != tree2clk_pin_map.at(ipin)) {
                continue;
              }
              VTR_ASSERT(rr_graph.valid_node(src_node));
              VTR_ASSERT(rr_graph.valid_node(des_node));
              vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                           src_node);
              if (tree2clk_pin_map.find(ipin) != tree2clk_pin_map.end()) {
                vpr_routing_annotation.set_rr_node_net(
                  src_node, tree2clk_pin_map.at(ipin));
                vpr_routing_annotation.set_rr_node_net(
                  des_node, tree2clk_pin_map.at(ipin));
              }
            }
          }
        }
      }
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
int route_clock_rr_graph(VprRoutingAnnotation& vpr_routing_annotation,
                         const DeviceContext& vpr_device_ctx,
                         const AtomContext& atom_ctx,
                         const ClusteredNetlist& cluster_nlist,
                         const VprNetlistAnnotation& netlist_annotation,
                         const RRClockSpatialLookup& clk_rr_lookup,
                         const ClockNetwork& clk_ntwk,
                         const PinConstraints& pin_constraints,
                         const bool& verbose) {
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
      tree2clk_pin_map, clk_ntwk, itree, verbose);
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }
    VTR_LOGV(verbose, "Done\n");
  }

  /* TODO: Sanity checks */

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
