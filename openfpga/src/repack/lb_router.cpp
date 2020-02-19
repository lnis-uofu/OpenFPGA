/******************************************************************************
 * Memember functions for data structure LbRouter
 ******************************************************************************/
#include <queue>

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

void LbRouter::expand_rt_rec(t_trace* rt,
                             const LbRRNodeId& prev_index, 
                             reservable_pq<t_expansion_node, std::vector<t_expansion_node>, compare_expansion_node>& pq,
                             const int& irt_net,
                             const int& explore_id_index) {
  t_expansion_node enode;

  /* Perhaps should use a cost other than zero */
  enode.cost = 0;
  enode.node_index = rt->current_node;
  enode.prev_index = prev_index;
  pq.push(enode);
  explored_node_tb_[enode.node_index].inet = irt_net;
  explored_node_tb_[enode.node_index].explored_id = OPEN;
  explored_node_tb_[enode.node_index].enqueue_id = explore_id_index;
  explored_node_tb_[enode.node_index].enqueue_cost = 0;
  explored_node_tb_[enode.node_index].prev_index = prev_index;

  for (unsigned int i = 0; i < rt->next_nodes.size(); i++) {
    expand_rt_rec(&rt->next_nodes[i], rt->current_node, pq, irt_net, explore_id_index);
  }
}

void LbRouter::expand_rt(const int& inet,
                         reservable_pq<t_expansion_node, std::vector<t_expansion_node>, compare_expansion_node>& pq,
                         const int& irt_net) {
  VTR_ASSERT(pq.empty());

  expand_rt_rec(lb_nets_[inet].rt_tree, LbRRNodeId::INVALID(), pq, irt_net, explore_id_index_);
}

void LbRouter::expand_edges(const LbRRGraph& lb_rr_graph,
                            t_mode* mode,
                            const LbRRNodeId& cur_inode,
                            float cur_cost,
                            int net_fanout,
                            reservable_pq<t_expansion_node, std::vector<t_expansion_node>, compare_expansion_node>& pq) {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  t_expansion_node enode;
  int usage;
  float incr_cost;

  for (const LbRREdgeId& iedge : lb_rr_graph.node_out_edges(cur_inode, mode)) {
    /* Init new expansion node */
    enode.prev_index = cur_inode;
    enode.node_index = lb_rr_graph.edge_sink_node(iedge);
    enode.cost = cur_cost;

    /* Determine incremental cost of using expansion node */
    usage = routing_status_[enode.node_index].occ + 1 - lb_rr_graph.node_capacity(enode.node_index);
    incr_cost = lb_rr_graph.node_intrinsic_cost(enode.node_index);
    incr_cost += lb_rr_graph.edge_intrinsic_cost(iedge);
    incr_cost += params_.hist_fac * routing_status_[enode.node_index].historical_usage;
    if (usage > 0) {
      incr_cost *= (usage * pres_con_fac_);
    }

    /* Adjust cost so that higher fanout nets prefer higher fanout routing nodes while lower fanout nets prefer lower fanout routing nodes */
    float fanout_factor = 1.0;
    t_mode* next_mode = routing_status_[enode.node_index].mode;
    /* Assume first mode if a mode hasn't been forced. */
    if (nullptr == next_mode) {
      next_mode = &(lb_rr_graph.node_pb_graph_pin(enode.node_index)->parent_node->pb_type->modes[0]);
    }
    if (lb_rr_graph.node_out_edges(enode.node_index, next_mode).size() > 1) {
      fanout_factor = 0.85 + (0.25 / net_fanout);
    } else {
      fanout_factor = 1.15 - (0.25 / net_fanout);
    }

    incr_cost *= fanout_factor;
    enode.cost = cur_cost + incr_cost;

    /* Add to queue if cost is lower than lowest cost path to this enode */
    if (explored_node_tb_[enode.node_index].enqueue_id == explore_id_index_) {
      if (enode.cost < explored_node_tb_[enode.node_index].enqueue_cost) {
        pq.push(enode);
      }
    } else {
      explored_node_tb_[enode.node_index].enqueue_id = explore_id_index_;
      explored_node_tb_[enode.node_index].enqueue_cost = enode.cost;
      pq.push(enode);
    }
  }
}

void LbRouter::expand_node(const LbRRGraph& lb_rr_graph,
                           const t_expansion_node& exp_node,
                           reservable_pq<t_expansion_node, std::vector<t_expansion_node>, compare_expansion_node>& pq,
                           const int& net_fanout) {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  t_expansion_node enode;

  LbRRNodeId cur_node = exp_node.node_index;
  float cur_cost = exp_node.cost;
  t_mode* mode = routing_status_[cur_node].mode;
  if (nullptr == mode) {
    mode = &(lb_rr_graph.node_pb_graph_pin(cur_node)->parent_node->pb_type->modes[0]);
  }

  expand_edges(lb_rr_graph, mode, cur_node, cur_cost, net_fanout, pq);
}

void LbRouter::expand_node_all_modes(const LbRRGraph& lb_rr_graph,
                                     const t_expansion_node& exp_node,
                                     reservable_pq<t_expansion_node, std::vector<t_expansion_node>, compare_expansion_node>& pq, 
                                     const int& net_fanout) {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  LbRRNodeId cur_inode = exp_node.node_index;
  float cur_cost = exp_node.cost;
  t_mode* cur_mode = routing_status_[cur_inode].mode;
  auto* pin = lb_rr_graph.node_pb_graph_pin(cur_inode);

  for (const LbRREdgeId& edge : lb_rr_graph.node_out_edges(cur_inode)) {
    t_mode* mode = lb_rr_graph.edge_mode(edge);
    /* If a mode has been forced, only add edges from that mode, otherwise add edges from all modes. */
    if (cur_mode != nullptr && mode != cur_mode) {
      continue;
    }

    /* Check whether a mode is illegal. If it is then the node will not be expanded */
    bool is_illegal = false;
    if (pin != nullptr) {
      auto* pb_graph_node = pin->parent_node;
      for (auto illegal_mode : pb_graph_node->illegal_modes) {
        if (mode->index == illegal_mode) {
          is_illegal = true;
          break;
        }
      }
    }

    if (is_illegal == true) {
      continue;
    }
    expand_edges(lb_rr_graph, mode, cur_inode, cur_cost, net_fanout, pq);
  }
}

/**************************************************
 * Private validators
 *************************************************/
bool LbRouter::matched_lb_rr_graph(const LbRRGraph& lb_rr_graph) const {
  return ( (routing_status_.size() == lb_rr_graph.nodes().size())
        && (explored_node_tb_.size() == lb_rr_graph.nodes().size()) );
}

} /* end namespace openfpga */
