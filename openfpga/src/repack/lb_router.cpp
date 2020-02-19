/******************************************************************************
 * Memember functions for data structure LbRouter
 ******************************************************************************/
#include "vtr_assert.h"

#include "lb_router.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors 
 *************************************************/
std::vector<LbRRNodeId> LbRouter::find_congested_rr_nodes(const LbRRGraph& lb_rr_graph) const {
  std::vector<LbRRNodeId> congested_rr_nodes;

  for (const LbRRNodeId& inode : lb_rr_graph.nodes()) {
    if (routing_status_[inode].occ > lb_rr_graph.node_capacity(inode)) {
      congested_rr_nodes.push_back(inode);
    }
  }

  return congested_rr_nodes;
}

} /* end namespace openfpga */
