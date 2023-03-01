#include "route_clock_rr_graph.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Route a clock tree on an existing routing resource graph
 * The strategy is to route spine one by one
 * - route the spine from the starting point to the ending point
 * - route the spine-to-spine switching points
 * - route the spine-to-IPIN connections (only for the last level)
 *******************************************************************/
static int route_clock_tree_rr_graph(
  VprRoutingAnnotation& vpr_routing_annotation, const RRGraphView& rr_graph,
  const RRClockSpatialLookup& clk_rr_lookup, const ClockNetwork& clk_ntwk,
  const ClockTreeId clk_tree, const bool& verbose) {
  for (auto ispine : clk_ntwk.spines(clk_tree)) {
    VTR_LOGV(verbose, "Routing spine '%s'...\n",
             clk_ntwk.spine_name(ispine).c_str());
    for (auto ipin : clk_ntwk.pins(clk_tree)) {
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
              VTR_ASSERT(rr_graph.valid_node(src_node));
              VTR_ASSERT(rr_graph.valid_node(des_node));
              vpr_routing_annotation.set_rr_node_prev_node(rr_graph, des_node,
                                                           src_node);
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
                         const RRClockSpatialLookup& clk_rr_lookup,
                         const ClockNetwork& clk_ntwk, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Route programmable clock network based on routing resource graph");

  /* Skip if there is no clock tree */
  if (clk_ntwk.num_trees()) {
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

  /* Route spines one by one */
  for (auto itree : clk_ntwk.trees()) {
    VTR_LOGV(verbose, "Routing clock tree '%s'...\n",
             clk_ntwk.tree_name(itree).c_str());
    int status =
      route_clock_tree_rr_graph(vpr_routing_annotation, vpr_device_ctx.rr_graph,
                                clk_rr_lookup, clk_ntwk, itree, verbose);
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }
    VTR_LOGV(verbose, "Done\n");
  }

  /* TODO: Sanity checks */

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
