/******************************************************************************
 * Memember functions for data structure LbRouter
 ******************************************************************************/
#include "vtr_assert.h"
#include "vtr_log.h"

#include "lb_router.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
LbRouter::LbRouter(const LbRRGraph& lb_rr_graph) {
  routing_status_.resize(lb_rr_graph.nodes().size());
  explored_node_tb_.resize(lb_rr_graph.nodes().size());
  explore_id_index_ = 1;

  /* Default routing parameters */
  params_.max_iterations = 50;
  params_.pres_fac = 1;
  params_.pres_fac_mult = 2;
  params_.hist_fac = 0.3;
 
  pres_con_fac_ = 1;
}

/**************************************************
 * Public Accessors 
 *************************************************/
std::vector<LbRRNodeId> LbRouter::find_congested_rr_nodes(const LbRRGraph& lb_rr_graph) const {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  std::vector<LbRRNodeId> congested_rr_nodes;

  for (const LbRRNodeId& inode : lb_rr_graph.nodes()) {
    if (routing_status_[inode].occ > lb_rr_graph.node_capacity(inode)) {
      congested_rr_nodes.push_back(inode);
    }
  }

  return congested_rr_nodes;
}

bool LbRouter::is_route_success(const LbRRGraph& lb_rr_graph) const {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  for (const LbRRNodeId& inode : lb_rr_graph.nodes()) {
    if (routing_status_[inode].occ > lb_rr_graph.node_capacity(inode)) {
      return false;
    }
  }

  return true;
}

/**************************************************
 * Private accessors
 *************************************************/
LbRouter::t_trace* LbRouter::find_node_in_rt(t_trace* rt, const LbRRNodeId& rt_index) {
  t_trace* cur;
  if (rt->current_node == rt_index) {
    return rt;
  } else {
    for (unsigned int i = 0; i < rt->next_nodes.size(); i++) {
      cur = find_node_in_rt(&rt->next_nodes[i], rt_index);
      if (cur != nullptr) {
        return cur;
      }
    }
  }
  return nullptr;
}

/**************************************************
 * Private mutators
 *************************************************/
void LbRouter::reset_explored_node_tb() {
  for (t_explored_node_stats& explored_node : explored_node_tb_) {
    explored_node.prev_index = LbRRNodeId::INVALID();
    explored_node.explored_id = OPEN;
    explored_node.inet = OPEN;
    explored_node.enqueue_id = OPEN;
    explored_node.enqueue_cost = 0;
  }
}

bool LbRouter::add_to_rt(t_trace* rt, const LbRRNodeId& node_index, const int& irt_net) {
  std::vector<LbRRNodeId> trace_forward;
  t_trace* link_node;
  t_trace curr_node;

  /* Store path all the way back to route tree */
  LbRRNodeId rt_index = node_index;
  while (explored_node_tb_[rt_index].inet != irt_net) {
    trace_forward.push_back(rt_index);
    rt_index = explored_node_tb_[rt_index].prev_index;
    VTR_ASSERT(rt_index != LbRRNodeId::INVALID());
  }

  /* Find rt_index on the route tree */
  link_node = find_node_in_rt(rt, rt_index);
  if (link_node == nullptr) {
    VTR_LOG("Link node is nullptr. Routing impossible");
    return true;
  }

  /* Add path to root tree */
  LbRRNodeId trace_index;
  while (!trace_forward.empty()) {
    trace_index = trace_forward.back();
    curr_node.current_node = trace_index;
    link_node->next_nodes.push_back(curr_node);
    link_node = &link_node->next_nodes.back();
    trace_forward.pop_back();
  }

  return false;
}

/**************************************************
 * Private validators
 *************************************************/
bool LbRouter::matched_lb_rr_graph(const LbRRGraph& lb_rr_graph) const {
  return ( (routing_status_.size() == lb_rr_graph.nodes().size())
        && (explored_node_tb_.size() == lb_rr_graph.nodes().size()) );
}

} /* end namespace openfpga */
