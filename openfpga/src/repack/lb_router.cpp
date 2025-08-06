/******************************************************************************
 * Memember functions for data structure LbRouter
 ******************************************************************************/
#include "lb_router.h"

#include "lb_rr_graph_utils.h"
#include "pb_type_graph.h"
#include "pb_type_utils.h"
#include "physical_types.h"
#include "vpr_error.h"
#include "vpr_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
LbRouter::LbRouter(const LbRRGraph& lb_rr_graph,
                   t_logical_block_type_ptr lb_type) {
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
LbRouter::net_range LbRouter::nets() const {
  return vtr::make_range(lb_net_ids_.begin(), lb_net_ids_.end());
}

AtomNetId LbRouter::net_atom_net_id(const NetId& net) const {
  VTR_ASSERT(true == valid_net_id(net));
  return lb_net_atom_net_ids_[net];
}

std::vector<LbRRNodeId> LbRouter::find_congested_rr_nodes(
  const LbRRGraph& lb_rr_graph) const {
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

bool LbRouter::is_routed() const { return is_routed_; }

std::vector<LbRRNodeId> LbRouter::net_routed_nodes(const NetId& net) const {
  VTR_ASSERT(true == is_routed());
  VTR_ASSERT(true == valid_net_id(net));

  std::vector<LbRRNodeId> routed_nodes;

  for (size_t isrc = 0; isrc < lb_net_sources_[net].size(); ++isrc) {
    t_trace* rt_tree = lb_net_rt_trees_[net][isrc];
    if (nullptr == rt_tree) {
      return routed_nodes;
    }
    /* Walk through the routing tree of the net */
    rec_collect_trace_nodes(rt_tree, routed_nodes);
  }

  return routed_nodes;
}

/**************************************************
 * Private accessors
 *************************************************/
bool LbRouter::is_route_success(const LbRRGraph& lb_rr_graph) const {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  for (const LbRRNodeId& inode : lb_rr_graph.nodes()) {
    if (routing_status_[inode].occ > lb_rr_graph.node_capacity(inode)) {
      VTR_LOGV(lb_rr_graph.node_pb_graph_pin(inode),
               "Route failed due to overuse pin '%s': occupancy '%ld' > "
               "capacity '%ld'!\n",
               lb_rr_graph.node_pb_graph_pin(inode)->to_string().c_str(),
               routing_status_[inode].occ, lb_rr_graph.node_capacity(inode));
      return false;
    }
  }

  return true;
}

LbRouter::t_trace* LbRouter::find_node_in_rt(t_trace* rt,
                                             const LbRRNodeId& rt_index) {
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

bool LbRouter::route_has_conflict(const LbRRGraph& lb_rr_graph,
                                  t_trace* rt) const {
  t_mode* cur_mode = nullptr;
  for (unsigned int i = 0; i < rt->next_nodes.size(); i++) {
    std::vector<LbRREdgeId> edges =
      lb_rr_graph.find_edge(rt->current_node, rt->next_nodes[i].current_node);
    VTR_ASSERT(1 == edges.size());
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

void LbRouter::rec_collect_trace_nodes(
  const t_trace* trace, std::vector<LbRRNodeId>& routed_nodes) const {
  if (routed_nodes.end() == std::find(routed_nodes.begin(), routed_nodes.end(),
                                      trace->current_node)) {
    routed_nodes.push_back(trace->current_node);
  }

  for (const t_trace& next : trace->next_nodes) {
    rec_collect_trace_nodes(&next, routed_nodes);
  }
}

/**************************************************
 * Public mutators
 *************************************************/
LbRouter::NetId LbRouter::create_net_to_route(
  const std::vector<LbRRNodeId>& sources,
  const std::vector<LbRRNodeId>& terminals) {
  /* Create an new id */
  NetId net = NetId(lb_net_ids_.size());
  lb_net_ids_.push_back(net);

  /* Allocate other attributes */
  lb_net_atom_net_ids_.push_back(AtomNetId::INVALID());
  lb_net_atom_source_pins_.emplace_back();
  lb_net_atom_sink_pins_.emplace_back();

  lb_net_sources_.push_back(sources);
  lb_net_sinks_.push_back(terminals);
  lb_net_rt_trees_.push_back(std::vector<t_trace*>(sources.size(), nullptr));

  return net;
}

void LbRouter::add_net_atom_net_id(const NetId& net,
                                   const AtomNetId& atom_net) {
  VTR_ASSERT(true == valid_net_id(net));
  lb_net_atom_net_ids_[net] = atom_net;
}

void LbRouter::add_net_atom_pins(const NetId& net, const AtomPinId& src_pin,
                                 const std::vector<AtomPinId>& terminal_pins) {
  VTR_ASSERT(true == valid_net_id(net));
  lb_net_atom_sink_pins_[net] = terminal_pins;
  lb_net_atom_source_pins_[net] = std::vector<AtomPinId>(1, src_pin);
}

void LbRouter::set_physical_pb_modes(
  const LbRRGraph& lb_rr_graph, const VprDeviceAnnotation& device_annotation) {
  /* Go through each node in the routing resource graph
   * Find the physical mode of each pb_graph_pin that is binded to the node
   * For input pins, the physical mode is a mode of its parent pb_type
   * For output pins, the physical mode is a mode of the parent pb_type of its
   * parent
   */
  for (const LbRRNodeId& node : lb_rr_graph.nodes()) {
    t_pb_graph_pin* pb_pin = lb_rr_graph.node_pb_graph_pin(node);
    if (nullptr == pb_pin) {
      routing_status_[node].mode = nullptr;
    } else {
      if (IN_PORT == pb_pin->port->type) {
        routing_status_[node].mode =
          device_annotation.physical_mode(pb_pin->parent_node->pb_type);
      } else {
        VTR_ASSERT(OUT_PORT == pb_pin->port->type);
        /* For top-level pb_graph node, the physical mode is nullptr */
        if (true == pb_pin->parent_node->is_root()) {
          routing_status_[node].mode = nullptr;
        } else {
          routing_status_[node].mode = device_annotation.physical_mode(
            pb_pin->parent_node->parent_pb_graph_node->pb_type);
          /* TODO: need to think about how to handle INOUT ports !!! */
        }
      }
    }
  }
}

bool LbRouter::try_route_net(
  const LbRRGraph& lb_rr_graph, const AtomNetlist& atom_nlist,
  const NetId& net_idx, t_expansion_node& exp_node,
  std::unordered_map<const t_pb_graph_node*, const t_mode*>& mode_map,
  const bool& verbosity) {
  /* Quick check: if all the net can be skipped, we return route succeed */
  bool skip_route = true;
  for (size_t isrc = 0; isrc < lb_net_sources_[net_idx].size(); ++isrc) {
    if (false ==
        is_skip_route_net(lb_rr_graph, lb_net_rt_trees_[net_idx][isrc])) {
      skip_route = false;
      break;
    }
  }
  if (true == skip_route) {
    return true;
  }

  std::vector<bool> sink_routed(lb_net_sinks_[net_idx].size(), false);

  for (size_t isrc = 0; isrc < lb_net_sources_[net_idx].size(); ++isrc) {
    if (true ==
        is_skip_route_net(lb_rr_graph, lb_net_rt_trees_[net_idx][isrc])) {
      continue;
    }

    commit_remove_rt(lb_rr_graph, lb_net_rt_trees_[net_idx][isrc], RT_REMOVE,
                     mode_map);
    free_net_rt(lb_net_rt_trees_[net_idx][isrc]);
    lb_net_rt_trees_[net_idx][isrc] = nullptr;
    add_source_to_rt(net_idx, isrc);

    /* Route each sink of net */
    for (size_t isink = 0; isink < lb_net_sinks_[net_idx].size(); ++isink) {
      /* Skip routed nets */
      if (true == sink_routed[isink]) {
        continue;
      }

      pq_.clear();

      /* Get lowest cost next node, repeat until a path is found or if it is
       * impossible to route */
      expand_rt(net_idx, net_idx, isrc);

      /* If we managed to expand the nodes to the sink, routing for this sink is
       * done. If not, we failed in routing. Therefore, the output of
       * try_expand_nodes() is inverted
       */
      sink_routed[isink] =
        !try_expand_nodes(atom_nlist, lb_rr_graph, net_idx, exp_node, isrc,
                          isink, mode_status_.expand_all_modes, verbosity);

      /* TODO: Debug codes, to be removed
      if (true == sink_routed[isink]) {
        VTR_LOGV(verbosity,
                 "Succeed to expand routing tree from source pin '%s' to sink
      pin '%s'!\n",
                 lb_rr_graph.node_pb_graph_pin(lb_net_sources_[net_idx][isrc])->to_string().c_str(),
                 lb_rr_graph.node_pb_graph_pin(lb_net_sinks_[net_idx][isink])->to_string().c_str());
      }
       */

      /* IMPORTANT: We do not need expand all the modes for physical repack
      if (false == sink_routed[isink] && false == mode_status_.expand_all_modes)
      { mode_status_.try_expand_all_modes = true; mode_status_.expand_all_modes
      = true; continue;
      }
       */

      if (exp_node.node_index == lb_net_sinks_[net_idx][isink]) {
        /* Net terminal is routed, add this to the route tree, clear data
         * structures, and keep going */
        sink_routed[isink] = !add_to_rt(lb_net_rt_trees_[net_idx][isrc],
                                        exp_node.node_index, net_idx);
      }

      if (false == sink_routed[isink]) {
        VTR_LOGV(
          verbosity,
          "Routing was impossible from source pin '%s' to sink pin '%s'!\n",
          lb_rr_graph.node_pb_graph_pin(lb_net_sources_[net_idx][isrc])
            ->to_string()
            .c_str(),
          lb_rr_graph.node_pb_graph_pin(lb_net_sinks_[net_idx][isink])
            ->to_string()
            .c_str());
      } else if (mode_status_.expand_all_modes) {
        sink_routed[isink] =
          !route_has_conflict(lb_rr_graph, lb_net_rt_trees_[net_idx][isrc]);
        if (false == sink_routed[isink]) {
          VTR_LOGV(verbosity, "Routing was impossible due to modes!\n");
        }
      }

      /*
      if (true == sink_routed[isink]) {
        VTR_LOGV(verbosity,
                 "Routing succeeded from source pin '%s' to sink pin '%s'!\n",
                 lb_rr_graph.node_pb_graph_pin(lb_net_sources_[net_idx][isrc])->to_string().c_str(),
                 lb_rr_graph.node_pb_graph_pin(lb_net_sinks_[net_idx][isink])->to_string().c_str());
      }
       */

      /* Increment explored node indices only when routing is successful */
      if (true == sink_routed[isink]) {
        explore_id_index_++;
        if (explore_id_index_ > 2000000000) {
          /* overflow protection */
          for (const LbRRNodeId& id : lb_rr_graph.nodes()) {
            explored_node_tb_[id].explored_id = OPEN;
            explored_node_tb_[id].enqueue_id = OPEN;
            explore_id_index_ = 1;
          }
        }
      } else {
        /* Route failed, reset the explore id index */
        reset_explored_node_tb();
        for (const LbRRNodeId& id : lb_rr_graph.nodes()) {
          explored_node_tb_[id].explored_id = OPEN;
          explored_node_tb_[id].enqueue_id = OPEN;
          explore_id_index_ = 1;
        }
      }
    }

    /* If any sinks are managed to be routed, we will try to save(commit)
     * results to route tree. During this process, we will check if there is any
     * nodes using different modes under the same pb_type
     * If so, we have conflicts and routing is considered to be failure
     */
    bool any_sink_routed = false;
    for (size_t isink = 0; isink < sink_routed.size(); ++isink) {
      if (true == sink_routed[isink]) {
        any_sink_routed = true;
        break;
      }
    }
    if (true == any_sink_routed) {
      commit_remove_rt(lb_rr_graph, lb_net_rt_trees_[net_idx][isrc], RT_COMMIT,
                       mode_map);
      if (true == mode_status_.is_mode_conflict) {
        VTR_LOGV(verbosity,
                 "Route fail due to mode conflicts when commiting the routing "
                 "tree!\n");
        for (size_t isink = 0; isink < sink_routed.size(); ++isink) {
          /* Change routed sinks to failure */
          if (true == sink_routed[isink]) {
            sink_routed[isink] = false;
          }
        }
      }
    }
  }

  /* Check the routing status for all the sinks */
  bool route_succeed = true;
  for (size_t isink = 0; isink < sink_routed.size(); ++isink) {
    if (false == sink_routed[isink]) {
      route_succeed = false;
      VTR_LOGV(verbosity, "Routing failed for sink pin '%s'!\n",
               lb_rr_graph.node_pb_graph_pin(lb_net_sinks_[net_idx][isink])
                 ->to_string()
                 .c_str());
      break;
    }
  }

  return route_succeed;
}

bool LbRouter::try_route(const LbRRGraph& lb_rr_graph,
                         const AtomNetlist& atom_nlist, const bool& verbosity) {
  /* Validate if the rr_graph is the one we used to initialize the router */
  VTR_ASSERT(true == matched_lb_rr_graph(lb_rr_graph));

  /* Ensure each net to be routed is valid */
  for (const NetId& net : lb_net_ids_) {
    VTR_ASSERT(true == check_net(lb_rr_graph, atom_nlist, net));
  }

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
   * Cap the total number of iterations tried so that if a solution does not
   * exist, then the router won't run indefinitely */
  pres_con_fac_ = params_.pres_fac;
  for (int iter = 0;
       iter < params_.max_iterations && !is_routed_ && !is_impossible; iter++) {
    unsigned int inet;
    /* Iterate across all nets internal to logic block */
    for (inet = 0; inet < lb_net_ids_.size() && !is_impossible; inet++) {
      NetId net_idx = NetId(inet);

      if (false == try_route_net(lb_rr_graph, atom_nlist, net_idx, exp_node,
                                 mode_map, verbosity)) {
        is_impossible = true;
      }
    }

    if (!is_impossible) {
      is_routed_ = is_route_success(lb_rr_graph);
    } else {
      --inet;
      VTR_LOG(
        "Net %lu '%s' is impossible to route within proposed %s cluster\n",
        inet, atom_nlist.net_name(lb_net_atom_net_ids_[NetId(inet)]).c_str(),
        lb_type_->name.c_str());
      VTR_LOG("\tNet source pin:\n");
      for (size_t isrc = 0; isrc < lb_net_sources_[NetId(inet)].size();
           ++isrc) {
        VTR_LOG(
          "\t\t%s\n",
          lb_rr_graph.node_pb_graph_pin(lb_net_sources_[NetId(inet)][isrc])
            ->to_string()
            .c_str());
      }
      VTR_LOG("\tNet sink pins:\n");
      for (size_t isink = 0; isink < lb_net_sinks_[NetId(inet)].size();
           ++isink) {
        VTR_LOG(
          "\t\t%s\n",
          lb_rr_graph.node_pb_graph_pin(lb_net_sinks_[NetId(inet)][isink])
            ->to_string()
            .c_str());
      }
      VTR_LOG("Please check your architecture XML to see if it is routable\n");

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
    // Collect all the sink terminals indicies which target a particular node
    std::map<LbRRNodeId, std::vector<int>> duplicate_terminals;
    for (size_t iterm = 0; iterm < lb_net_sinks_[ilb_net].size(); ++iterm) {
      LbRRNodeId node = lb_net_sinks_[ilb_net][iterm];

      duplicate_terminals[node].push_back(iterm);
    }

    for (auto kv : duplicate_terminals) {
      if (kv.second.size() < 2) continue;  // Only process duplicates

      // Remap all the duplicate terminals so they target the pin instead of the
      // sink
      for (size_t idup_term = 0; idup_term < kv.second.size(); ++idup_term) {
        int iterm =
          kv.second[idup_term];  // The index in terminals which is duplicated

        VTR_ASSERT(lb_net_atom_sink_pins_[ilb_net].size() ==
                   lb_net_sinks_[ilb_net].size());
        AtomPinId atom_pin = lb_net_atom_sink_pins_[ilb_net][iterm];
        VTR_ASSERT(atom_pin);

        const t_pb_graph_pin* pb_graph_pin = find_pb_graph_pin(
          atom_ctx.netlist(), atom_ctx.lookup().atom_pb_bimap(), atom_pin);
        VTR_ASSERT(pb_graph_pin);

        if (pb_graph_pin->port->equivalent == PortEquivalence::NONE)
          continue;  // Only need to remap equivalent ports

        // Remap this terminal to an explicit pin instead of the common sink
        LbRRNodeId pin_index =
          lb_rr_graph.find_node(LB_INTERMEDIATE, pb_graph_pin);
        VTR_ASSERT(true == lb_rr_graph.valid_node_id(pin_index));

        VTR_LOG_WARN(
          "Found duplicate nets connected to logically equivalent pins. "
          "Remapping intra lb net %d (atom net %zu '%s') from common sink "
          "pb_route %d to fixed pin pb_route %d\n",
          size_t(ilb_net), size_t(lb_net_atom_net_ids_[ilb_net]),
          atom_ctx.netlist().net_name(lb_net_atom_net_ids_[ilb_net]).c_str(),
          kv.first, size_t(pin_index));

        VTR_ASSERT(
          1 == lb_rr_graph
                 .node_out_edges(
                   pin_index, &(pb_graph_pin->parent_node->pb_type->modes[0]))
                 .size());
        LbRRNodeId sink_index =
          lb_rr_graph.edge_sink_node(lb_rr_graph.node_out_edges(
            pin_index, &(pb_graph_pin->parent_node->pb_type->modes[0]))[0]);
        VTR_ASSERT(LB_SINK == lb_rr_graph.node_type(sink_index));
        VTR_ASSERT_MSG(sink_index == lb_net_sinks_[ilb_net][iterm],
                       "Remapped pin must be connected to original sink");

        // Change the target
        lb_net_sinks_[ilb_net][iterm] = pin_index;
      }
    }
  }
}

// Check one edge for mode conflict.
bool LbRouter::check_edge_for_route_conflicts(
  std::unordered_map<const t_pb_graph_node*, const t_mode*>& mode_map,
  const t_pb_graph_pin* driver_pin, const t_pb_graph_pin* pin) {
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
      VTR_LOG(
        "Differing modes for block. Got %s mode, while previously was %s for "
        "interconnect %s.\n",
        mode->name, result.first->second->name, edge->interconnect->name);
      // The illegal mode is added to the pb_graph_node as it resulted in a
      // conflict during atom-to-atom routing. This mode cannot be used in the
      // consequent cluster generation try.
      auto it = illegal_modes_.find(pb_graph_node);
      if (it == illegal_modes_.end()) {
        illegal_modes_[pb_graph_node].push_back(result.first->second);
      } else {
        if (std::find(illegal_modes_.at(pb_graph_node).begin(),
                      illegal_modes_.at(pb_graph_node).end(),
                      result.first->second) ==
            illegal_modes_.at(pb_graph_node).end()) {
          it->second.push_back(result.first->second);
        }
      }

      // If the number of illegal modes equals the number of available mode for
      // a specific pb_graph_node it means that no cluster can be generated.
      // This resuts in a fatal error.
      if ((int)illegal_modes_.at(pb_graph_node).size() >=
          pb_graph_node->pb_type->num_modes) {
        VPR_FATAL_ERROR(
          VPR_ERROR_PACK,
          "There are no more available modes to be used. Routing Failed!");
      }

      return true;
    }
  }

  return false;
}

void LbRouter::commit_remove_rt(
  const LbRRGraph& lb_rr_graph, t_trace* rt, const e_commit_remove& op,
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
      routing_status_[inode].historical_usage +=
        (routing_status_[inode].occ - lb_rr_graph.node_capacity(inode) +
         1); /* store historical overuse */
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
    // A conflict is present if there are differing modes between a
    // pb_graph_node and its children.
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

bool LbRouter::is_skip_route_net(const LbRRGraph& lb_rr_graph, t_trace* rt) {
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

bool LbRouter::add_to_rt(t_trace* rt, const LbRRNodeId& node_index,
                         const NetId& irt_net) {
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

void LbRouter::add_source_to_rt(const NetId& inet, const size_t& isrc) {
  /* TODO: Validate net id */
  VTR_ASSERT(nullptr == lb_net_rt_trees_[inet][isrc]);
  lb_net_rt_trees_[inet][isrc] = new t_trace;
  lb_net_rt_trees_[inet][isrc]->current_node = lb_net_sources_[inet][isrc];
}

void LbRouter::expand_rt_rec(t_trace* rt, const LbRRNodeId& prev_index,
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
    expand_rt_rec(&rt->next_nodes[i], rt->current_node, irt_net,
                  explore_id_index);
  }
}

void LbRouter::expand_rt(const NetId& inet, const NetId& irt_net,
                         const size_t& isrc) {
  VTR_ASSERT(pq_.empty());

  expand_rt_rec(lb_net_rt_trees_[inet][isrc], LbRRNodeId::INVALID(), irt_net,
                explore_id_index_);
}

void LbRouter::expand_edges(const LbRRGraph& lb_rr_graph, t_mode* mode,
                            const LbRRNodeId& cur_inode, float cur_cost,
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
    usage = routing_status_[enode.node_index].occ + 1 -
            lb_rr_graph.node_capacity(enode.node_index);
    incr_cost = lb_rr_graph.node_intrinsic_cost(enode.node_index);
    incr_cost += lb_rr_graph.edge_intrinsic_cost(iedge);
    incr_cost +=
      params_.hist_fac * routing_status_[enode.node_index].historical_usage;
    if (usage > 0) {
      incr_cost *= (usage * pres_con_fac_);
    }

    /* Adjust cost so that higher fanout nets prefer higher fanout routing nodes
     * while lower fanout nets prefer lower fanout routing nodes */
    float fanout_factor = 1.0;
    t_mode* next_mode = routing_status_[enode.node_index].mode;
    /* Assume first mode if a mode hasn't been forced. */
    if (nullptr == next_mode) {
      /* If the node is mapped to a nullptr pb_graph_pin, this is a special
       * SINK. Use nullptr mode */
      if (nullptr == lb_rr_graph.node_pb_graph_pin(enode.node_index)) {
        next_mode = nullptr;
      } else if (true == is_primitive_pb_type(
                           lb_rr_graph.node_pb_graph_pin(enode.node_index)
                             ->parent_node->pb_type)) {
        /* For primitive node, we give nullptr as default */
        next_mode = nullptr;
      } else {
        next_mode = &(lb_rr_graph.node_pb_graph_pin(enode.node_index)
                        ->parent_node->pb_type->modes[0]);
      }
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
        /*
        if (nullptr != lb_rr_graph.node_pb_graph_pin(enode.node_index)) {
          VTR_LOG("Added node '%s' to priority queue\n",
                  lb_rr_graph.node_pb_graph_pin(enode.node_index)->to_string().c_str());
        }
         */
      }
    } else {
      explored_node_tb_[enode.node_index].enqueue_id = explore_id_index_;
      explored_node_tb_[enode.node_index].enqueue_cost = enode.cost;
      pq_.push(enode);
      /*
      if (nullptr != lb_rr_graph.node_pb_graph_pin(enode.node_index)) {
        VTR_LOG("Added node '%s' to priority queue\n",
                lb_rr_graph.node_pb_graph_pin(enode.node_index)->to_string().c_str());
      }
       */
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
    if (nullptr == lb_rr_graph.node_pb_graph_pin(cur_node)) {
      mode = nullptr;
    } else if (true ==
               is_primitive_pb_type(lb_rr_graph.node_pb_graph_pin(cur_node)
                                      ->parent_node->pb_type)) {
      mode = nullptr;
    } else {
      mode = &(lb_rr_graph.node_pb_graph_pin(cur_node)
                 ->parent_node->pb_type->modes[0]);
    }
  }

  /*
  if (nullptr != mode) {
    VTR_LOGV(lb_rr_graph.node_pb_graph_pin(cur_node),
             "Expand node '%s' by considering mode '%s'\n",
             lb_rr_graph.node_pb_graph_pin(cur_node)->to_string().c_str(),
             mode->name);
  }
   */
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
    /* If a mode has been forced, only add edges from that mode, otherwise add
     * edges from all modes. */
    if (cur_mode != nullptr && mode != cur_mode) {
      continue;
    }

    /* Check whether a mode is illegal. If it is then the node will not be
     * expanded */
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
                                const NetId& lb_net, t_expansion_node& exp_node,
                                const int& isrc, const int& itarget,
                                const bool& try_other_modes,
                                const bool& verbosity) {
  bool is_impossible = false;

  do {
    if (pq_.empty()) {
      /* No connection possible */
      is_impossible = true;

      if (true == verbosity) {
        // Print detailed debug info
        AtomNetId net_id = lb_net_atom_net_ids_[lb_net];
        AtomPinId driver_pin = lb_net_atom_source_pins_[lb_net][isrc];
        AtomPinId sink_pin = lb_net_atom_sink_pins_[lb_net][itarget];
        LbRRNodeId driver_rr_node = lb_net_sources_[lb_net][isrc];
        LbRRNodeId sink_rr_node = lb_net_sinks_[lb_net][itarget];

        VTR_LOG(
          "\t\t\tNo possible routing path from %s to %s: needed for net '%s' "
          "from net pin '%s'",
          describe_lb_rr_node(lb_rr_graph, driver_rr_node).c_str(),
          describe_lb_rr_node(lb_rr_graph, sink_rr_node).c_str(),
          atom_nlist.net_name(net_id).c_str(),
          atom_nlist.pin_name(driver_pin).c_str());
        VTR_LOGV(sink_pin, " to net pin '%s'",
                 atom_nlist.pin_name(sink_pin).c_str());
        VTR_LOG("\n");
      }
    } else {
      exp_node = pq_.top();
      pq_.pop();
      LbRRNodeId exp_inode = exp_node.node_index;

      if (explored_node_tb_[exp_inode].explored_id != explore_id_index_) {
        /* First time node is popped implies path to this node is the lowest
         * cost. If the node is popped a second time, then the path to that node
         * is higher than this path so ignore.
         */
        explored_node_tb_[exp_inode].explored_id = explore_id_index_;
        explored_node_tb_[exp_inode].prev_index = exp_node.prev_index;
        if (exp_inode != lb_net_sinks_[lb_net][itarget]) {
          if (!try_other_modes) {
            expand_node(lb_rr_graph, exp_node, lb_net_sinks_[lb_net].size());
          } else {
            expand_node_all_modes(lb_rr_graph, exp_node,
                                  lb_net_sinks_[lb_net].size());
          }
        }
      }
    }
  } while (exp_node.node_index != lb_net_sinks_[lb_net][itarget] &&
           !is_impossible);

  return is_impossible;
}

/**************************************************
 * Private validators
 *************************************************/
bool LbRouter::matched_lb_rr_graph(const LbRRGraph& lb_rr_graph) const {
  return ((routing_status_.size() == lb_rr_graph.nodes().size()) &&
          (explored_node_tb_.size() == lb_rr_graph.nodes().size()));
}

bool LbRouter::valid_net_id(const NetId& net_id) const {
  return (size_t(net_id) < lb_net_ids_.size()) &&
         (net_id == lb_net_ids_[net_id]);
}

bool LbRouter::check_net(const LbRRGraph& lb_rr_graph,
                         const AtomNetlist& atom_nlist,
                         const NetId& net) const {
  if (false == atom_nlist.valid_net_id(lb_net_atom_net_ids_[net])) {
    return false;
  }
  if (lb_net_atom_sink_pins_[net].size() != lb_net_sinks_[net].size()) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Net '%lu' has unmatched atom pins and terminals.\n",
                   size_t(net));
    return false;
  }
  /* We must have 1 source and >1 terminal */
  if (1 > lb_net_sources_[net].size()) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Net '%lu' has only %lu sources.\n",
                   size_t(net), lb_net_sources_[net].size());
    return false;
  }

  if (1 > lb_net_sinks_[net].size()) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Net '%lu' has only %lu sinks.\n",
                   size_t(net), lb_net_sinks_[net].size());
    return false;
  }
  /* Each node must be valid */
  for (const LbRRNodeId& node : lb_net_sources_[net]) {
    if (false == lb_rr_graph.valid_node_id(node)) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Net '%lu' has invalid sink node in lb_rr_graph.\n",
                     size_t(net));
      return false;
    }
  }
  for (const LbRRNodeId& node : lb_net_sinks_[net]) {
    if (false == lb_rr_graph.valid_node_id(node)) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Net '%lu' has invalid sink node in lb_rr_graph.\n",
                     size_t(net));
      return false;
    }
  }

  /* Each atom pin must be valid */
  for (const AtomPinId& pin : lb_net_atom_source_pins_[net]) {
    if (false == atom_nlist.valid_pin_id(pin)) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Net '%lu' has invalid source atom pin.\n", size_t(net));
      return false;
    }
  }
  for (const AtomPinId& pin : lb_net_atom_sink_pins_[net]) {
    if (false == atom_nlist.valid_pin_id(pin)) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Net '%lu' has invalid sink_ atom pin.\n", size_t(net));
      return false;
    }
  }

  return true;
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
    for (size_t isrc = 0; isrc < lb_net_sources_[inet].size(); ++isrc) {
      free_net_rt(lb_net_rt_trees_[inet][isrc]);
      lb_net_rt_trees_[inet][isrc] = nullptr;
    }
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
  lb_net_atom_source_pins_.clear();
  lb_net_atom_sink_pins_.clear();
  lb_net_sources_.clear();
  lb_net_sinks_.clear();
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

void LbRouter::reset_illegal_modes() { illegal_modes_.clear(); }

} /* end namespace openfpga */
