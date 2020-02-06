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
void annotate_rr_node_nets(const ClusteringContext& vpr_clustering_ctx,
                           const RoutingContext& vpr_routing_ctx,
                           VprRoutingAnnotation& vpr_routing_annotation) {
  size_t counter = 0;
  VTR_LOG("Annotating rr_node with routed nets...");
   
  for (auto net_id : vpr_clustering_ctx.clb_nlist.nets()) {
    /* Ignore nets that are not routed */
    if (true == vpr_clustering_ctx.clb_nlist.net_is_ignored(net_id)) {
      continue;
    }
    /* Ignore used in local cluster only, reserved one CLB pin */
    if (false == vpr_clustering_ctx.clb_nlist.net_sinks(net_id).size()) {
      continue;
    }
    t_trace* tptr = vpr_routing_ctx.trace[net_id].head;
    while (tptr != nullptr) {
      RRNodeId rr_node = tptr->index;
      vpr_routing_annotation.set_rr_node_net(rr_node, net_id);
      counter++;
      tptr = tptr->next;
    }
  }

  VTR_LOG("Done with %d nodes mapping\n", counter);
}

} /* end namespace openfpga */

