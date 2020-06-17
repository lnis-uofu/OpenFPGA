/************************************************************************
 * Member functions for class VprRoutingAnnotation
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vpr_routing_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprRoutingAnnotation::VprRoutingAnnotation() {
  return;
}

/************************************************************************
 * Public accessors
 ***********************************************************************/
ClusterNetId VprRoutingAnnotation::rr_node_net(const RRNodeId& rr_node) const {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  return rr_node_nets_[rr_node];
}

RRNodeId VprRoutingAnnotation::rr_node_prev_node(const RRNodeId& rr_node) const {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  return rr_node_prev_nodes_[rr_node];
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprRoutingAnnotation::init(const RRGraph& rr_graph) {
  rr_node_nets_.resize(rr_graph.nodes().size(), ClusterNetId::INVALID());
  rr_node_prev_nodes_.resize(rr_graph.nodes().size(), RRNodeId::INVALID());
}

void VprRoutingAnnotation::set_rr_node_net(const RRNodeId& rr_node,
                                           const ClusterNetId& net_id) {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  /* Warn any override attempt */
  if ( (ClusterNetId::INVALID() != rr_node_nets_[rr_node])
    && (net_id != rr_node_nets_[rr_node])) {
    VTR_LOG_WARN("Override the net '%ld' by net '%ld' for node '%ld' with in routing context annotation!\n",
                 size_t(rr_node_nets_[rr_node]), size_t(net_id), size_t(rr_node));
  }

  rr_node_nets_[rr_node] = net_id;
}

void VprRoutingAnnotation::set_rr_node_prev_node(const RRNodeId& rr_node,
                                                 const RRNodeId& prev_node) {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  /* Warn any override attempt */
  if ( (RRNodeId::INVALID() != rr_node_prev_nodes_[rr_node])
    && (prev_node != rr_node_prev_nodes_[rr_node])) {
    VTR_LOG_WARN("Override the previous node '%ld' by previous node '%ld' for node '%ld' with in routing context annotation!\n",
                 size_t(rr_node_prev_nodes_[rr_node]), size_t(prev_node), size_t(rr_node));
  }

  rr_node_prev_nodes_[rr_node] = prev_node;
}

} /* End namespace openfpga*/
