/******************************************************************************
 * Memember functions for data structure LbRouter
 ******************************************************************************/
#include <queue>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "physical_types.h"
#include "pb_type_graph.h"
#include "vpr_error.h"

#include "lb_rr_graph_utils.h"
#include "lb_router.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
LbRouter::LbRouter(const LbRRGraph& lb_rr_graph, t_logical_block_type_ptr lb_type) {
  routing_status_.resize(lb_rr_graph.nodes().size());
  explored_node_tb_.resize(lb_rr_graph.nodes().size());
  explore_id_index_ = 1;

  lb_type_ = lb_type;

  /* Default routing parameters */
  params_.max_iterations = 50;
  params_.pres_fac = 1;
  params_.pres_fac_mult = 2;
  params_.hist_fac = 0.3;

  is_routed_ = false;
 
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

bool LbRouter::is_routed() const {
  return is_routed_;
}

/**************************************************
 * Private accessors
 *************************************************/
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

bool LbRouter::route_has_conflict(const LbRRGraph& lb_rr_graph, t_trace* rt) const {
  t_mode* cur_mode = nullptr;
  for (unsigned int i = 0; i < rt->next_nodes.size(); i++) {
    std::vector<LbRREdgeId> edges = lb_rr_graph.find_edge(rt->current_node, rt->next_nodes[i].current_node);
    VTR_ASSERT(0 == edges.size());
    t_mode* new_mode = lb_rr_graph.edge_mode(edges[0]);
    if (cur_mode != nullptr && cur_mode != new_mode) {
      return true;
    }
    if (route_has_conflict(lb_rr_graph, &rt->next_nodes[i]) == true) {
      return true;
    }
    cur_mode = new_mode;
  }

  return false;
}

/**************************************************
 * Public mutators
 *************************************************/
bool LbRouter::try_route(const LbRRGraph& lb_rr_graph,
                         const AtomNetlist& atom_nlist,
                         const int& verbosity) {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  is_routed_ = false;
  bool is_impossible = false;

  mode_status_.is_mode_conflict = false;
  mode_status_.try_expand_all_modes = false;

  t_expansion_node exp_node;

  reset_explored_node_tb();

  /* Reset current routing */
  reset_net_rt();
  reset_routing_status();

  std::unordered_map<const t_pb_graph_node*, const t_mode*> mode_map;

  /* Iteratively remove congestion until a successful route is found.
   * Cap the total number of iterations tried so that if a solution does not exist, then the router won't run indefinitely */
  pres_con_fac_ = params_.pres_fac;
  for (int iter = 0; iter < params_.max_iterations && !is_routed_ && !is_impossible; iter++) {
    unsigned int inet;
    /* Iterate across all nets internal to logic block */
    for (inet = 0; inet < lb_net_ids_.size() && !is_impossible; inet++) {
      NetId idx = NetId(inet);
      if (is_skip_route_net(lb_rr_graph, lb_net_rt_trees_[idx])) {
        continue;
      }
      commit_remove_rt(lb_rr_graph, lb_net_rt_trees_[idx], RT_REMOVE, mode_map);
      free_net_rt(lb_net_rt_trees_[idx]);
      lb_net_rt_trees_[idx] = nullptr;
      add_source_to_rt(idx);

      /* Route each sink of net */
      for (unsigned int itarget = 1; itarget < lb_net_terminals_[idx].size() && !is_impossible; itarget++) {
        pq_.clear();
        /* Get lowest cost next node, repeat until a path is found or if it is impossible to route */

        expand_rt(idx, idx);

        is_impossible = try_expand_nodes(atom_nlist, lb_rr_graph, idx, exp_node, itarget, mode_status_.expand_all_modes, verbosity);

        if (is_impossible && !mode_status_.expand_all_modes) {
          mode_status_.try_expand_all_modes = true;
          mode_status_.expand_all_modes = true;
          break;
        }

        if (exp_node.node_index == lb_net_terminals_[idx][itarget]) {
          /* Net terminal is routed, add this to the route tree, clear data structures, and keep going */
          is_impossible = add_to_rt(lb_net_rt_trees_[idx], exp_node.node_index, idx);
        }

        if (is_impossible) {
          VTR_LOG("Routing was impossible!\n");
        } else if (mode_status_.expand_all_modes) {
          is_impossible = route_has_conflict(lb_rr_graph, lb_net_rt_trees_[idx]);
          if (is_impossible) {
            VTR_LOG("Routing was impossible due to modes!\n");
          }
        }

        explore_id_index_++;
        if (explore_id_index_ > 2000000000) {
          /* overflow protection */
          for (const LbRRNodeId& id : lb_rr_graph.nodes()) {
            explored_node_tb_[id].explored_id = OPEN;
            explored_node_tb_[id].enqueue_id = OPEN;
            explore_id_index_ = 1;
          }
        }
      }

      if (!is_impossible) {
        commit_remove_rt(lb_rr_graph, lb_net_rt_trees_[idx], RT_COMMIT, mode_map);
        if (mode_status_.is_mode_conflict) {
          is_impossible = true;
        }
      }
    }

    if (!is_impossible) {
      is_routed_ = is_route_success(lb_rr_graph);
    } else {
      --inet;
      VTR_LOGV(verbosity < 3, "Net '%s' is impossible to route within proposed %s cluster\n",
               atom_nlist.net_name(lb_net_atom_net_ids_[NetId(inet)]).c_str(), lb_type_->name);
      is_routed_ = false;
    }
    pres_con_fac_ *= params_.pres_fac_mult;
  }

  /* TODO: 
   * Let user to decide to how proceed upon the routing results: 
   * - route success: save the results through public accessors to lb_nets_
   *                  print the route results to files
   * - route fail: report all the congestion nodes 
   */
  return is_routed_;
}

/**************************************************
 * Private mutators
 *************************************************/
void LbRouter::fix_duplicate_equivalent_pins(const AtomContext& atom_ctx,
                                             const LbRRGraph& lb_rr_graph) {
  for (const NetId& ilb_net : lb_net_ids_) {
    //Collect all the sink terminals indicies which target a particular node
    std::map<LbRRNodeId, std::vector<int>> duplicate_terminals;
    for (size_t iterm = 1; iterm < lb_net_terminals_[ilb_net].size(); ++iterm) {
      LbRRNodeId node = lb_net_terminals_[ilb_net][iterm];

      duplicate_terminals[node].push_back(iterm);
    }

    for (auto kv : duplicate_terminals) {
      if (kv.second.size() < 2) continue; //Only process duplicates

      //Remap all the duplicate terminals so they target the pin instead of the sink
      for (size_t idup_term = 0; idup_term < kv.second.size(); ++idup_term) {
        int iterm = kv.second[idup_term]; //The index in terminals which is duplicated

        VTR_ASSERT(lb_net_atom_pins_[ilb_net].size() == lb_net_terminals_[ilb_net].size());
        AtomPinId atom_pin = lb_net_atom_pins_[ilb_net][iterm];
        VTR_ASSERT(atom_pin);

        const t_pb_graph_pin* pb_graph_pin = find_pb_graph_pin(atom_ctx.nlist, atom_ctx.lookup, atom_pin);
        VTR_ASSERT(pb_graph_pin);

        if (pb_graph_pin->port->equivalent == PortEquivalence::NONE) continue; //Only need to remap equivalent ports

        //Remap this terminal to an explicit pin instead of the common sink
        LbRRNodeId pin_index = lb_rr_graph.find_node(LB_INTERMEDIATE, pb_graph_pin);
        VTR_ASSERT(true == lb_rr_graph.valid_node_id(pin_index));

        VTR_LOG_WARN(
            "Found duplicate nets connected to logically equivalent pins. "
            "Remapping intra lb net %d (atom net %zu '%s') from common sink "
            "pb_route %d to fixed pin pb_route %d\n",
            size_t(ilb_net), size_t(lb_net_atom_net_ids_[ilb_net]), atom_ctx.nlist.net_name(lb_net_atom_net_ids_[ilb_net]).c_str(),
            kv.first, size_t(pin_index));

        VTR_ASSERT(1 == lb_rr_graph.node_out_edges(pin_index, &(pb_graph_pin->parent_node->pb_type->modes[0])).size());
        LbRRNodeId sink_index = lb_rr_graph.edge_sink_node(lb_rr_graph.node_out_edges(pin_index, &(pb_graph_pin->parent_node->pb_type->modes[0]))[0]);
        VTR_ASSERT(LB_SINK == lb_rr_graph.node_type(sink_index));
        VTR_ASSERT_MSG(sink_index == lb_net_terminals_[ilb_net][iterm], "Remapped pin must be connected to original sink");

        //Change the target
        lb_net_terminals_[ilb_net][iterm] = pin_index;
      }
    }
  }
}

// Check one edge for mode conflict.
bool LbRouter::check_edge_for_route_conflicts(std::unordered_map<const t_pb_graph_node*, const t_mode*>& mode_map,
                                              const t_pb_graph_pin* driver_pin,
                                              const t_pb_graph_pin* pin) {
  if (driver_pin == nullptr) {
    return false;
  }

  // Only check pins that are OUT_PORTs.
  if (pin == nullptr || pin->port == nullptr || pin->port->type != OUT_PORT) {
    return false;
  }
  VTR_ASSERT(!pin->port->is_clock);

  auto* pb_graph_node = pin->parent_node;
  VTR_ASSERT(pb_graph_node->pb_type == pin->port->parent_pb_type);

  const t_pb_graph_edge* edge = get_edge_between_pins(driver_pin, pin);
  VTR_ASSERT(edge != nullptr);

  auto mode_of_edge = edge->interconnect->parent_mode_index;
  auto* mode = &pb_graph_node->pb_type->modes[mode_of_edge];

  auto result = mode_map.insert(std::make_pair(pb_graph_node, mode));
  if (!result.second) {
    if (result.first->second != mode) {
      VTR_LOG("Differing modes for block. Got %s mode, while previously was %s for interconnect %s.\n",
              mode->name, result.first->second->name,
              edge->interconnect->name);
      // The illegal mode is added to the pb_graph_node as it resulted in a conflict during atom-to-atom routing. This mode cannot be used in the consequent cluster
      // generation try.
      auto it = illegal_modes_.find(pb_graph_node);
      if (it == illegal_modes_.end()) {
        illegal_modes_[pb_graph_node].push_back(result.first->second);
      } else {
        if (std::find(illegal_modes_.at(pb_graph_node).begin(), illegal_modes_.at(pb_graph_node).end(), result.first->second) == illegal_modes_.at(pb_graph_node).end()) {
          it->second.push_back(result.first->second);
        }
      }

      // If the number of illegal modes equals the number of available mode for a specific pb_graph_node it means that no cluster can be generated. This resuts
      // in a fatal error.
      if ((int)illegal_modes_.at(pb_graph_node).size() >= pb_graph_node->pb_type->num_modes) {
        VPR_FATAL_ERROR(VPR_ERROR_PACK, "There are no more available modes to be used. Routing Failed!");
      }

      return true;
    }
  }

  return false;
}

void LbRouter::commit_remove_rt(const LbRRGraph& lb_rr_graph,
                                t_trace* rt,
                                const e_commit_remove& op,
                                std::unordered_map<const t_pb_graph_node*, const t_mode*>& mode_map) {
  int incr;

  if (nullptr == rt) {
    return;
  }

  LbRRNodeId inode = rt->current_node;

  /* Determine if node is being used or removed */
  if (op == RT_COMMIT) {
    incr = 1;
    if (routing_status_[inode].occ >= lb_rr_graph.node_capacity(inode)) {
      routing_status_[inode].historical_usage += (routing_status_[inode].occ - lb_rr_graph.node_capacity(inode) + 1); /* store historical overuse */
    }
  } else {
    incr = -1;
    explored_node_tb_[inode].inet = NetId::INVALID();
  }

  routing_status_[inode].occ += incr;
  VTR_ASSERT(routing_status_[inode].occ >= 0);

  t_pb_graph_pin* driver_pin = lb_rr_graph.node_pb_graph_pin(inode);

  /* Recursively update route tree */
  for (unsigned int i = 0; i < rt->next_nodes.size(); i++) {
    // Check to see if there is no mode conflict between previous nets.
    // A conflict is present if there are differing modes between a pb_graph_node
    // and its children.
    if (op == RT_COMMIT && mode_status_.try_expand_all_modes) {
      const LbRRNodeId& node = rt->next_nodes[i].current_node;
      t_pb_graph_pin* pin = lb_rr_graph.node_pb_graph_pin(node);

      if (check_edge_for_route_conflicts(mode_map, driver_pin, pin)) {
        mode_status_.is_mode_conflict = true;
      }
    }

    commit_remove_rt(lb_rr_graph, &rt->next_nodes[i], op, mode_map);
  }
}

bool LbRouter::is_skip_route_net(const LbRRGraph& lb_rr_graph,
                                 t_trace* rt) {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  if (rt == nullptr) {
    return false; /* Net is not routed, therefore must route net */
  }

  LbRRNodeId inode = rt->current_node;

  /* Determine if node is overused */
  if (routing_status_[inode].occ > lb_rr_graph.node_capacity(inode)) {
    /* Conflict between this net and another net at this node, reroute net */
    return false;
  }

  /* Recursively check that rest of route tree does not have a conflict */
  for (unsigned int i = 0; i < rt->next_nodes.size(); i++) {
    if (!is_skip_route_net(lb_rr_graph, &rt->next_nodes[i])) {
      return false;
    }
  }

  /* No conflict, this net's current route is legal, skip routing this net */
  return true;
}

bool LbRouter::add_to_rt(t_trace* rt, const LbRRNodeId& node_index, const NetId& irt_net) {
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

void LbRouter::add_source_to_rt(const NetId& inet) {
  /* TODO: Validate net id */
  VTR_ASSERT(nullptr == lb_net_rt_trees_[inet]);
  lb_net_rt_trees_[inet] = new t_trace;
  lb_net_rt_trees_[inet]->current_node = lb_net_terminals_[inet][0];
}

void LbRouter::expand_rt_rec(t_trace* rt,
                             const LbRRNodeId& prev_index, 
                             const NetId& irt_net,
                             const int& explore_id_index) {
  t_expansion_node enode;

  /* Perhaps should use a cost other than zero */
  enode.cost = 0;
  enode.node_index = rt->current_node;
  enode.prev_index = prev_index;
  pq_.push(enode);
  explored_node_tb_[enode.node_index].inet = irt_net;
  explored_node_tb_[enode.node_index].explored_id = OPEN;
  explored_node_tb_[enode.node_index].enqueue_id = explore_id_index;
  explored_node_tb_[enode.node_index].enqueue_cost = 0;
  explored_node_tb_[enode.node_index].prev_index = prev_index;

  for (unsigned int i = 0; i < rt->next_nodes.size(); i++) {
    expand_rt_rec(&rt->next_nodes[i], rt->current_node, irt_net, explore_id_index);
  }
}

void LbRouter::expand_rt(const NetId& inet,
                         const NetId& irt_net) {
  VTR_ASSERT(pq_.empty());

  expand_rt_rec(lb_net_rt_trees_[inet], LbRRNodeId::INVALID(), irt_net, explore_id_index_);
}

void LbRouter::expand_edges(const LbRRGraph& lb_rr_graph,
                            t_mode* mode,
                            const LbRRNodeId& cur_inode,
                            float cur_cost,
                            int net_fanout) {
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
        pq_.push(enode);
      }
    } else {
      explored_node_tb_[enode.node_index].enqueue_id = explore_id_index_;
      explored_node_tb_[enode.node_index].enqueue_cost = enode.cost;
      pq_.push(enode);
    }
  }
}

void LbRouter::expand_node(const LbRRGraph& lb_rr_graph,
                           const t_expansion_node& exp_node,
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

  expand_edges(lb_rr_graph, mode, cur_node, cur_cost, net_fanout);
}

void LbRouter::expand_node_all_modes(const LbRRGraph& lb_rr_graph,
                                     const t_expansion_node& exp_node,
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
      if (0 == illegal_modes_.count(pb_graph_node)) {
        continue;
      }
      for (auto illegal_mode : illegal_modes_.at(pb_graph_node)) {
        if (mode == illegal_mode) {
          is_illegal = true;
          break;
        }
      }
    }

    if (is_illegal == true) {
      continue;
    }
    expand_edges(lb_rr_graph, mode, cur_inode, cur_cost, net_fanout);
  }
}

bool LbRouter::try_expand_nodes(const AtomNetlist& atom_nlist,
                                const LbRRGraph& lb_rr_graph,
                                const NetId& lb_net,
                                t_expansion_node& exp_node,
                                const int& itarget,
                                const bool& try_other_modes,
                                const int& verbosity) {
  bool is_impossible = false;

  do {
    if (pq_.empty()) {
      /* No connection possible */
      is_impossible = true;

      if (verbosity > 3) {
        //Print detailed debug info
        AtomNetId net_id = lb_net_atom_net_ids_[lb_net];
        AtomPinId driver_pin = lb_net_atom_pins_[lb_net][0];
        AtomPinId sink_pin = lb_net_atom_pins_[lb_net][itarget];
        LbRRNodeId driver_rr_node = lb_net_terminals_[lb_net][0];
        LbRRNodeId sink_rr_node = lb_net_terminals_[lb_net][itarget];

        VTR_LOG("\t\t\tNo possible routing path from %s to %s: needed for net '%s' from net pin '%s'",
                describe_lb_rr_node(lb_rr_graph, driver_rr_node).c_str(),
                describe_lb_rr_node(lb_rr_graph, sink_rr_node).c_str(),
                atom_nlist.net_name(net_id).c_str(),
                atom_nlist.pin_name(driver_pin).c_str());
        VTR_LOGV(sink_pin, " to net pin '%s'", atom_nlist.pin_name(sink_pin).c_str());
        VTR_LOG("\n");
      }
    } else {
      exp_node = pq_.top();
      pq_.pop();
      LbRRNodeId exp_inode = exp_node.node_index;

      if (explored_node_tb_[exp_inode].explored_id != explore_id_index_) {
        /* First time node is popped implies path to this node is the lowest cost.
         * If the node is popped a second time, then the path to that node is higher 
         * than this path so ignore.
         */
        explored_node_tb_[exp_inode].explored_id = explore_id_index_;
        explored_node_tb_[exp_inode].prev_index = exp_node.prev_index;
        if (exp_inode != lb_net_terminals_[lb_net][itarget]) {
          if (!try_other_modes) {
            expand_node(lb_rr_graph, exp_node, lb_net_terminals_[lb_net].size() - 1);
          } else {
            expand_node_all_modes(lb_rr_graph, exp_node, lb_net_terminals_[lb_net].size() - 1);
          }
        }
      }
    }
  } while (exp_node.node_index != lb_net_terminals_[lb_net][itarget] && !is_impossible);

  return is_impossible;
}

/**************************************************
 * Private validators
 *************************************************/
bool LbRouter::matched_lb_rr_graph(const LbRRGraph& lb_rr_graph) const {
  return ( (routing_status_.size() == lb_rr_graph.nodes().size())
        && (explored_node_tb_.size() == lb_rr_graph.nodes().size()) );
}

/**************************************************
 * Private Initializer and cleaner
 *************************************************/
void LbRouter::reset_explored_node_tb() {
  for (t_explored_node_stats& explored_node : explored_node_tb_) {
    explored_node.prev_index = LbRRNodeId::INVALID();
    explored_node.explored_id = OPEN;
    explored_node.inet = NetId::INVALID();
    explored_node.enqueue_id = OPEN;
    explored_node.enqueue_cost = 0;
  }
}

void LbRouter::reset_net_rt() {
  for (const NetId& inet : lb_net_ids_) {
    free_net_rt(lb_net_rt_trees_[inet]);
    lb_net_rt_trees_[inet] = nullptr;
  }
}

void LbRouter::reset_routing_status() {
  for (t_routing_status& status : routing_status_) {
    status.historical_usage = 0;
    status.occ = 0;
  }
}

void LbRouter::clear_nets() {
  /* TODO: Trace should no longer use pointers */
  reset_net_rt();

  lb_net_ids_.clear();
  lb_net_atom_net_ids_.clear();
  lb_net_atom_pins_.clear();
  lb_net_terminals_.clear();
  lb_net_fixed_terminals_.clear();
  lb_net_rt_trees_.clear();
}

void LbRouter::free_net_rt(t_trace* lb_trace) {
  if (lb_trace != nullptr) {
    for (unsigned int i = 0; i < lb_trace->next_nodes.size(); i++) {
      free_lb_trace(&lb_trace->next_nodes[i]);
    }
    lb_trace->next_nodes.clear();
    delete lb_trace;
  }
}

void LbRouter::free_lb_trace(t_trace* lb_trace) {
  if (lb_trace != nullptr) {
    for (unsigned int i = 0; i < lb_trace->next_nodes.size(); i++) {
      free_lb_trace(&lb_trace->next_nodes[i]);
    }
    lb_trace->next_nodes.clear();
  }
}

void LbRouter::reset_illegal_modes() {
  illegal_modes_.clear();
}


} /* end namespace openfpga */
