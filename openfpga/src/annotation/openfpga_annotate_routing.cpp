/********************************************************************
 * This file includes functions that are used to annotate routing results
 * from VPR to OpenFPGA
 *******************************************************************/
#include "openfpga_annotate_routing.h"

#include "annotate_routing.h"
#include "old_traceback.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Create a mapping between each rr_node and its mapped nets
 * - Only applicable to global nets for dedicated clock routing purpose
 * - Note that this function is different than annotate_vpr_rr_nodes()
 *   Please do not annotate global nets in vpr_routing_annotation!
 *******************************************************************/
vtr::vector<RRNodeId, ClusterNetId> annotate_rr_node_global_net(
  const DeviceContext& device_ctx, const ClusteredNetlist& cluster_nlist,
  const PlacementContext& placement_ctx,
  const VprClusteringAnnotation& clustering_annotation, const bool& verbose) {
  vtr::vector<RRNodeId, ClusterNetId> rr_node_nets;

  size_t counter = 0;
  vtr::ScopedStartFinishTimer timer("Annotating rr_node with global nets");

  const auto& rr_graph = device_ctx.rr_graph;

  rr_node_nets.resize(rr_graph.num_nodes(), ClusterNetId::INVALID());

  size_t layer = 0;

  for (ClusterNetId net_id : cluster_nlist.nets()) {
    if (!cluster_nlist.net_is_ignored(net_id)) {
      continue;
    }
    /* Walk through all the sinks */
    for (ClusterPinId pin_id : cluster_nlist.net_pins(net_id)) {
      ClusterBlockId block_id = cluster_nlist.pin_block(pin_id);
      t_block_loc blk_loc = get_block_loc(block_id, false);
      int phy_pin = placement_ctx.physical_pins[pin_id];
      t_physical_tile_type_ptr phy_tile = device_ctx.grid.get_physical_type(
        t_physical_tile_loc(blk_loc.loc.x, blk_loc.loc.y, 0));
      int node_pin_num = phy_tile->num_pins;
      /* Note that the phy_pin may not reflect the actual pin index at the
       * top-level physical tile type. It could be one of the random pin to the
       * same pin class. So here, we have to find an exact match of the pin
       * index from the clustering results! */
      int subtile_idx = blk_loc.loc.sub_tile;
      auto logical_block = cluster_nlist.block_type(block_id);
      for (int j = 0; j < logical_block->pb_type->num_pins; j++) {
        /* Find the net mapped to this pin in clustering results*/
        ClusterNetId cluster_net_id = cluster_nlist.block_net(block_id, j);
        /* Get the actual net id because it may be renamed during routing */
        if (true == clustering_annotation.is_net_renamed(block_id, j)) {
          cluster_net_id = clustering_annotation.net(block_id, j);
        }
        /* Bypass unmatched pins */
        if (cluster_net_id != net_id) {
          continue;
        }
        int curr_pin_num = get_physical_pin_at_sub_tile_location(
          phy_tile, logical_block, subtile_idx, j);
        if (phy_tile->pin_class[curr_pin_num] != phy_tile->pin_class[phy_pin]) {
          continue;
        }
        node_pin_num = curr_pin_num;
        break;
      }
      VTR_ASSERT(node_pin_num < phy_tile->num_pins);
      t_rr_type rr_pin_type = IPIN;
      if (phy_tile->class_inf[phy_tile->pin_class[node_pin_num]].type ==
          RECEIVER) {
        rr_pin_type = IPIN;
      } else if (phy_tile->class_inf[phy_tile->pin_class[node_pin_num]].type ==
                 DRIVER) {
        rr_pin_type = OPIN;
      } else {
        VTR_LOG_ERROR(
          "When annotating global net '%s', invalid rr node pin type for '%s' "
          "pin '%d'\n",
          cluster_nlist.net_name(net_id).c_str(), phy_tile->name, node_pin_num);
        exit(1);
      }
      std::vector<RRNodeId> curr_rr_nodes =
        rr_graph.node_lookup().find_nodes_at_all_sides(
          layer, blk_loc.loc.x, blk_loc.loc.y, rr_pin_type, node_pin_num);
      for (RRNodeId curr_rr_node : curr_rr_nodes) {
        VTR_LOGV(verbose, "Annotate global net '%s' on '%s' pin '%d'\n",
                 cluster_nlist.net_name(net_id).c_str(), phy_tile->name,
                 node_pin_num);
        rr_node_nets[curr_rr_node] = net_id;
        counter++;
      }
    }
  }

  VTR_LOGV(verbose, "Done with %d nodes mapping\n", counter);

  return rr_node_nets;
}

/********************************************************************
 * Create a mapping between each rr_node and its mapped nets
 * based on VPR routing results
 * - Unmapped rr_node will use invalid ids
 *******************************************************************/
void annotate_vpr_rr_node_nets(const DeviceContext& device_ctx,
                               const ClusteringContext& clustering_ctx,
                               const RoutingContext& routing_ctx,
                               VprRoutingAnnotation& vpr_routing_annotation,
                               const bool& verbose) {
  vtr::vector<RRNodeId, ParentNetId> node2net =
    annotate_rr_node_nets((const Netlist<>&)clustering_ctx.clb_nlist,
                          device_ctx, routing_ctx, verbose, false);
  for (size_t node_id = 0; node_id < device_ctx.rr_graph.num_nodes();
       ++node_id) {
    vpr_routing_annotation.set_rr_node_net(
      RRNodeId(node_id),
      convert_to_cluster_net_id(node2net[RRNodeId(node_id)]));
  }
  VTR_LOG("Loaded node-to-net mapping\n");
}

/********************************************************************
 * This function will find a previous node for a given rr_node
 * from the routing traces
 *
 * It requires a candidate which provided by upstream functions
 * Try to validate a candidate by searching it from driving node list
 * If not validated, try to find a right one in the routing traces
 *******************************************************************/
static RRNodeId find_previous_node_from_routing_traces(
  const RRGraphView& rr_graph, t_trace* routing_trace_head,
  const RRNodeId& prev_node_candidate, const RRNodeId& cur_rr_node) {
  RRNodeId prev_node = prev_node_candidate;

  /* For a valid prev_node, ensure prev node is one of the driving nodes for
   * this rr_node! */
  if (prev_node) {
    /* Try to spot the previous node in the incoming node list of this rr_node
     */
    bool valid_prev_node = false;
    for (const RREdgeId& in_edge : rr_graph.node_in_edges(cur_rr_node)) {
      if (prev_node == rr_graph.edge_src_node(in_edge)) {
        valid_prev_node = true;
        break;
      }
    }

    /* Early exit if we already validate the node */
    if (true == valid_prev_node) {
      return prev_node;
    }

    /* If we cannot find one, it could be possible that this rr_node branches
     * from an earlier point in the routing tree
     *
     *            +----- ... --->prev_node
     *            |
     *  src_node->+
     *            |
     *            +-----+ rr_node
     *
     * Our job now is to start from the head of the traces and find the
     * prev_node that drives this rr_node
     *
     * This search will find the first-fit and finish.
     * This is reasonable because if there is a second-fit, it should be a
     * longer path which should be considered in routing optimization
     */
    if (false == valid_prev_node) {
      t_trace* tptr = routing_trace_head;
      while (tptr != nullptr) {
        RRNodeId cand_prev_node = RRNodeId(tptr->index);
        bool is_good_cand = false;
        for (const RREdgeId& in_edge : rr_graph.node_in_edges(cur_rr_node)) {
          if (cand_prev_node == rr_graph.edge_src_node(in_edge)) {
            is_good_cand = true;
            break;
          }
        }

        if (true == is_good_cand) {
          /* Update prev_node */
          prev_node = cand_prev_node;
          break;
        }

        /* Move on to the next */
        tptr = tptr->next;
      }
    }
  }

  return prev_node;
}

/********************************************************************
 * Create a mapping between each rr_node and its previous node
 * based on VPR routing results
 * - Unmapped rr_node will have an invalid id of previous rr_node
 *******************************************************************/
void annotate_rr_node_previous_nodes(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  const RoutingContext& routing_ctx,
  VprRoutingAnnotation& vpr_routing_annotation, const bool& verbose) {
  size_t counter = 0;
  VTR_LOG("Annotating previous nodes for rr_node...");
  VTR_LOGV(verbose, "\n");

  for (auto net_id : clustering_ctx.clb_nlist.nets()) {
    /* Ignore nets that are not routed */
    if (true == clustering_ctx.clb_nlist.net_is_ignored(net_id)) {
      continue;
    }
    /* Ignore used in local cluster only, reserved one CLB pin */
    if (false == clustering_ctx.clb_nlist.net_sinks(net_id).size()) {
      continue;
    }

    /* Cache Previous nodes */
    RRNodeId prev_node = RRNodeId::INVALID();

    t_trace* tptr = TracebackCompat::traceback_from_route_tree(
      routing_ctx.route_trees[net_id].value());
    t_trace* head = tptr;
    while (tptr != nullptr) {
      RRNodeId rr_node = RRNodeId(tptr->index);

      /* Find the right previous node */
      prev_node = find_previous_node_from_routing_traces(
        device_ctx.rr_graph, head, prev_node, rr_node);

      /* Only update mapped nodes */
      if (prev_node) {
        vpr_routing_annotation.set_rr_node_prev_node(device_ctx.rr_graph,
                                                     rr_node, prev_node);
        counter++;
      }

      /* Update prev_node */
      prev_node = rr_node;

      /* Move on to the next */
      tptr = tptr->next;
    }
    free_traceback(head);
  }

  VTR_LOG("Done with %d nodes mapping\n", counter);
}

} /* end namespace openfpga */
