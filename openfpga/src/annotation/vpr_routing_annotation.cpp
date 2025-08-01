/************************************************************************
 * Member functions for class VprRoutingAnnotation
 ***********************************************************************/
#include "vpr_routing_annotation.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprRoutingAnnotation::VprRoutingAnnotation() { return; }

/************************************************************************
 * Public accessors
 ***********************************************************************/
ClusterNetId VprRoutingAnnotation::rr_node_net(const RRNodeId& rr_node) const {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  return rr_node_nets_[rr_node];
}

RRNodeId VprRoutingAnnotation::rr_node_prev_node(
  const RRNodeId& rr_node) const {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  return rr_node_prev_nodes_[rr_node];
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprRoutingAnnotation::init(const RRGraphView& rr_graph) {
  rr_node_nets_.resize(rr_graph.nodes().size(), ClusterNetId::INVALID());
  rr_node_prev_nodes_.resize(rr_graph.nodes().size(), RRNodeId::INVALID());
}

void VprRoutingAnnotation::set_rr_node_net(const RRNodeId& rr_node,
                                           const ClusterNetId& net_id) {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  /* Warn any override attempt */
  if ((ClusterNetId::INVALID() != rr_node_nets_[rr_node]) &&
      (net_id != rr_node_nets_[rr_node])) {
    VTR_LOG_WARN(
      "Override the net '%ld' by net '%ld' for node '%ld' with in routing "
      "context annotation!\n",
      size_t(rr_node_nets_[rr_node]), size_t(net_id), size_t(rr_node));
  }

  rr_node_nets_[rr_node] = net_id;
}

void VprRoutingAnnotation::set_rr_node_prev_node(const RRGraphView& rr_graph,
                                                 const RRNodeId& rr_node,
                                                 const RRNodeId& prev_node) {
  /* Ensure that the node_id is in the list */
  VTR_ASSERT(size_t(rr_node) < rr_node_nets_.size());
  /* XT: It seems the net information of SINK node is polluted/out-of-date in
   * VPR after routing. It is quite weird that some SINKs are not the ending
   * point of a routing path It could happen in some highly customized
   * architecture A stop sign is enforced here to avoid any exceptions
   */
  if ((RRNodeId::INVALID() != rr_node_prev_nodes_[rr_node]) &&
      (rr_graph.node_type(prev_node) == e_rr_type::SINK)) {
    return;
  }
  /* Warn any override attempt */
  if ((RRNodeId::INVALID() != rr_node_prev_nodes_[rr_node]) &&
      (prev_node != rr_node_prev_nodes_[rr_node])) {
    VTR_LOG_WARN(
      "Override the previous node '%s' by previous node '%s' for node '%s' "
      "with in routing context annotation!\n",
      rr_graph.node_coordinate_to_string(rr_node_prev_nodes_[rr_node]).c_str(),
      rr_graph.node_coordinate_to_string(prev_node).c_str(),
      rr_graph.node_coordinate_to_string(rr_node).c_str());
  }

  rr_node_prev_nodes_[rr_node] = prev_node;
}

} /* End namespace openfpga*/
