/********************************************************************
 * This file includes functions that are used to annotate routing results
 * from VPR to OpenFPGA
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "annotate_routing.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Create a mapping between each rr_node and its mapped nets 
 * based on VPR routing results
 * - Unmapped rr_node will use invalid ids 
 *******************************************************************/
void annotate_rr_node_nets(const DeviceContext& device_ctx,
                           const ClusteringContext& clustering_ctx,
                           const RoutingContext& routing_ctx,
                           VprRoutingAnnotation& vpr_routing_annotation,
                           const bool& verbose) {
  size_t counter = 0;
  VTR_LOG("Annotating rr_node with routed nets...");
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
    t_trace* tptr = routing_ctx.trace[net_id].head;
    while (tptr != nullptr) {
      RRNodeId rr_node = tptr->index;
      /* Ignore source and sink nodes, they are the common node multiple starting and ending points */
      if ( (SOURCE != device_ctx.rr_graph.node_type(rr_node)) 
        && (SINK != device_ctx.rr_graph.node_type(rr_node)) ) {
        vpr_routing_annotation.set_rr_node_net(rr_node, net_id);
        counter++;
      }
      tptr = tptr->next;
    }
  }

  VTR_LOG("Done with %d nodes mapping\n", counter);
}


/********************************************************************
 * This function will find a previous node for a given rr_node
 * from the routing traces
 *
 * It requires a candidate which provided by upstream functions
 * Try to validate a candidate by searching it from driving node list
 * If not validated, try to find a right one in the routing traces
 *******************************************************************/
static 
RRNodeId find_previous_node_from_routing_traces(const RRGraph& rr_graph,
                                                t_trace* routing_trace_head,
                                                const RRNodeId& prev_node_candidate,
                                                const RRNodeId& cur_rr_node) {
  RRNodeId prev_node = prev_node_candidate;

  /* For a valid prev_node, ensure prev node is one of the driving nodes for this rr_node! */
  if (prev_node) {
    /* Try to spot the previous node in the incoming node list of this rr_node */
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
     * Our job now is to start from the head of the traces and find the prev_node 
     * that drives this rr_node
     *
     * This search will find the first-fit and finish.
     * This is reasonable because if there is a second-fit, it should be a longer path
     * which should be considered in routing optimization
     */
    if (false == valid_prev_node) {
      t_trace* tptr = routing_trace_head;
      while (tptr != nullptr) {
        RRNodeId cand_prev_node = tptr->index;
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
void annotate_rr_node_previous_nodes(const DeviceContext& device_ctx,
                                     const ClusteringContext& clustering_ctx,
                                     const RoutingContext& routing_ctx,
                                     VprRoutingAnnotation& vpr_routing_annotation,
                                     const bool& verbose) {
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

    t_trace* tptr = routing_ctx.trace[net_id].head;
    while (tptr != nullptr) {
      RRNodeId rr_node = tptr->index;

      /* Find the right previous node */
      prev_node = find_previous_node_from_routing_traces(device_ctx.rr_graph,
                                                         routing_ctx.trace[net_id].head,
                                                         prev_node,
                                                         rr_node);

      /* Only update mapped nodes */
      if (prev_node) {
        vpr_routing_annotation.set_rr_node_prev_node(rr_node, prev_node);
        counter++;
      }

      /* Update prev_node */
      prev_node = rr_node;

      /* Move on to the next */
      tptr = tptr->next;
    }
  }

  VTR_LOG("Done with %d nodes mapping\n", counter);
}

} /* end namespace openfpga */

