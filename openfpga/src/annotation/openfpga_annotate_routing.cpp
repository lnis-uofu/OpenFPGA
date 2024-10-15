/********************************************************************
 * This file includes functions that are used to annotate routing results
 * from VPR to OpenFPGA
 *******************************************************************/
#include "openfpga_annotate_routing.h"

#include "annotate_routing.h"
#include "old_traceback.h"
#include "route_util.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Create a mapping between each rr_node and its mapped nets
 * based on VPR routing results
 * - Unmapped rr_node will use invalid ids
 *******************************************************************/
void annotate_vpr_rr_node_nets(const DeviceContext& device_ctx,
                               const ClusteringContext& clustering_ctx,
                               VprRoutingAnnotation& vpr_routing_annotation,
                               const bool& verbose) {
  vtr::vector<RRNodeId, ClusterNetId> node2net =
    annotate_rr_node_nets(clustering_ctx, device_ctx, verbose);
  for (size_t node_id = 0; node_id < device_ctx.rr_graph.num_nodes();
       ++node_id) {
    vpr_routing_annotation.set_rr_node_net(
      RRNodeId(node_id),
      node2net[RRNodeId(node_id)]);
  }
  VTR_LOG("Loaded node-to-net mapping\n");
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

  auto& netlist = clustering_ctx.clb_nlist; 

  for (auto net_id : netlist.nets()) {
    /* Ignore nets that are not routed */
    if (true == netlist.net_is_ignored(net_id)) {
      continue;
    }
    /* Ignore used in local cluster only, reserved one CLB pin */
    if (false == netlist.net_sinks(net_id).size()) {
      continue;
    }
  
    auto& tree = get_route_tree_from_cluster_net_id(net_id);
    if(!tree)
      continue;
  
    for(auto& rt_node: tree->all_nodes()){
      RRNodeId rr_node = rt_node.inode;
      auto parent = rt_node.parent();
      vpr_routing_annotation.set_rr_node_prev_node(device_ctx.rr_graph, rr_node, parent ? parent->inode : RRNodeId::INVALID());
    }
  }

  VTR_LOG("Done with %d nodes mapping\n", counter);
}

} /* end namespace openfpga */
