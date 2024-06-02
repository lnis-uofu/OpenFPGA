/***************************************************************************************
 * This file includes functions that are used to redo packing for physical pbs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "build_physical_lb_rr_graph.h"
#include "lb_router.h"
#include "lb_router_utils.h"
#include "pb_graph_utils.h"
#include "pb_type_utils.h"
#include "physical_pb_utils.h"
#include "repack.h"
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Try to find sink pb graph pins through walking through the fan-out edges from
 * the source pb graph pin
 * Only the sink meeting the following requirements can be considered:
 * - All the fan-out edges between the source and sink are from direct
 *interconnection
 * - sink is an input of a primitive pb_type
 *
 * Note:
 *  - This function is applicable ONLY to single-mode pb_types!!! Because their
 *routing traces are deterministic: there is only 1 valid path from a source pin
 *to a sink pin!!!
 *  - If there is a fan-out of the current source pb graph pin is not a direct
 *interconnection the direct search should stop.
 *  - This function is designed for pb graph without local routing
 *    For example: direct connection between root pb graph node to the LUT pb
 *graph node
 *
 *              root pb_graph_node
 *              +-----------------------------------------
 *              |         Intermediate pb_graph_node
 *              |       +----------------------------------
 *              |       |         primitive pb_graph_node
 *              |       |        +-------------------------
 *    I[0] ---->+------>+------->|I[0]   LUT
 *
 *  - This function is designed for passing wires inside pb graph
 *
 *              root pb_graph_node
 *              +------------------------------+
 *              | Intermediate pb_graph_node   |
 *              |       +-------------+        |
 *              |       |             |        |
 *              |       |             |        |
 *    I[0]----->+------>+--- ... ---->+------->+------>O[0]
 *
 ***************************************************************************************/
static bool rec_direct_search_sink_pb_graph_pins(
  const t_pb_graph_pin* source_pb_pin,
  std::vector<t_pb_graph_pin*>& sink_pb_pins) {
  std::vector<t_pb_graph_pin*> sink_pb_pins_to_search;

  /* Only support single-mode pb_type!!! */
  // if (1 != source_pb_pin->parent_node->pb_type->num_modes) {
  //  return false;
  //}

  for (int iedge = 0; iedge < source_pb_pin->num_output_edges; ++iedge) {
    if (DIRECT_INTERC !=
        source_pb_pin->output_edges[iedge]->interconnect->type) {
      return false;
    }
    for (int ipin = 0;
         ipin < source_pb_pin->output_edges[iedge]->num_output_pins; ++ipin) {
      t_pb_graph_pin* cand_sink_pb_pin =
        source_pb_pin->output_edges[iedge]->output_pins[ipin];
      if ((true ==
           is_primitive_pb_type(cand_sink_pb_pin->parent_node->pb_type)) &&
          (IN_PORT == cand_sink_pb_pin->port->type)) {
        sink_pb_pins.push_back(cand_sink_pb_pin);
      } else if ((true == cand_sink_pb_pin->parent_node->is_root()) &&
                 (OUT_PORT == cand_sink_pb_pin->port->type)) {
        sink_pb_pins.push_back(cand_sink_pb_pin);
      } else {
        sink_pb_pins_to_search.push_back(cand_sink_pb_pin);
      }
    }
  }

  for (t_pb_graph_pin* sink_pb_pin : sink_pb_pins_to_search) {
    bool direct_search_status =
      rec_direct_search_sink_pb_graph_pins(sink_pb_pin, sink_pb_pins);
    if (false == direct_search_status) {
      return false;
    }
  }

  /* Reach here, we succeed. */
  return true;
}

/***************************************************************************************
 * Try find all the sink pins which is mapped to a routing trace in the context
 *of pb route This function uses a recursive walk-through over the pb_route We
 *will always start from the pb_route of the source pin For each sink,
 * - if it is the end point of a routing tree, we add it to the sink list
 *   - An output of top-level pb_graph_node
 *   - An input of a primitive pb_graph_node
 * - if it is not the end point of a routing tree, we visit the pb_route
 *   corresponds to the sink pin
 *
 * Note: when you call this function at the top-level, please provide an empty
 *vector of sink_pb_pins!!!
 ***************************************************************************************/
static void rec_find_routed_sink_pb_graph_pins(
  const t_pb* pb, const t_pb_graph_pin* source_pb_pin,
  const AtomNetId& atom_net_id, const VprDeviceAnnotation& device_annotation,
  const std::map<const t_pb_graph_pin*, AtomNetId>& pb_pin_mapped_nets,
  t_pb_graph_pin** pb_graph_pin_lookup_from_index,
  std::vector<t_pb_graph_pin*>& sink_pb_pins) {
  /* Bypass unused pins */
  if (0 == pb->pb_route.count(source_pb_pin->pin_count_in_cluster)) {
    return;
  }

  /* Get the driver pb pin id, it must be valid */
  if (atom_net_id !=
      pb->pb_route[source_pb_pin->pin_count_in_cluster].atom_net_id) {
    return;
  }

  /* Check each sink nodes, if pin belongs to an input of a primitive
   * pb_graph_node, it is what we want */
  std::vector<t_pb_graph_pin*> sink_pb_pins_to_search;
  for (const int& sink_pb_pin_id :
       pb->pb_route[source_pb_pin->pin_count_in_cluster].sink_pb_pin_ids) {
    t_pb_graph_pin* sink_pb_pin =
      pb_graph_pin_lookup_from_index[sink_pb_pin_id];
    VTR_ASSERT(nullptr != sink_pb_pin);

    /* We will update sink node list only
     * - input pins of primitive nodes
     * - output pins of top node
     */
    if ((true == is_primitive_pb_type(sink_pb_pin->parent_node->pb_type)) &&
        (IN_PORT == sink_pb_pin->port->type)) {
      sink_pb_pins.push_back(sink_pb_pin);
      continue;
    }

    if ((true == sink_pb_pin->parent_node->is_root()) &&
        (OUT_PORT == sink_pb_pin->port->type)) {
      /* Be careful!!! There is an inconsistency between pb_route and actual net
       * mapping! The sink_pb_pin in the pb_route may not be the one we want due
       * to net remapping in the routing stage If the net becomes invalid, we
       * search all the fan-out of the source pb_pin and find one that is mapped
       * to the net
       */
      AtomNetId remapped_net = AtomNetId::INVALID();
      auto remapped_result = pb_pin_mapped_nets.find(sink_pb_pin);
      if (remapped_result != pb_pin_mapped_nets.end()) {
        remapped_net = remapped_result->second;
      }
      if (atom_net_id == remapped_net) {
        sink_pb_pins.push_back(sink_pb_pin);
      } else {
        VTR_ASSERT_SAFE(atom_net_id != remapped_net);
        bool found_actual_sink_pb_pin = false;
        for (int iedge = 0; iedge < source_pb_pin->num_output_edges; ++iedge) {
          /* Bypass the interconnect that does not belong to a physical mode */
          int parent_mode_index =
            source_pb_pin->output_edges[iedge]->interconnect->parent_mode_index;
          VTR_ASSERT(parent_mode_index <
                     sink_pb_pin->parent_node->pb_type->num_modes);
          if (&(sink_pb_pin->parent_node->pb_type->modes[parent_mode_index]) !=
              device_annotation.physical_mode(
                sink_pb_pin->parent_node->pb_type)) {
            continue;
          }
          for (int ipin = 0;
               ipin < source_pb_pin->output_edges[iedge]->num_output_pins;
               ++ipin) {
            const t_pb_graph_pin* cand_sink_pb_pin =
              source_pb_pin->output_edges[iedge]->output_pins[ipin];
            auto cand_remapped_result =
              pb_pin_mapped_nets.find(cand_sink_pb_pin);
            AtomNetId cand_sink_pb_pin_net = AtomNetId::INVALID();
            if (cand_remapped_result != pb_pin_mapped_nets.end()) {
              cand_sink_pb_pin_net = cand_remapped_result->second;
            }
            if (atom_net_id == cand_sink_pb_pin_net) {
              sink_pb_pins.push_back(
                const_cast<t_pb_graph_pin*>(cand_sink_pb_pin));
              found_actual_sink_pb_pin = true;
              break;
            }
          }
          if (true == found_actual_sink_pb_pin) {
            break;
          }
        }
        VTR_ASSERT(true == found_actual_sink_pb_pin);
      }
      continue;
    }

    /* We should find the pb_route recursively */
    sink_pb_pins_to_search.push_back(sink_pb_pin);
  }

  for (t_pb_graph_pin* sink_pb_pin : sink_pb_pins_to_search) {
    rec_find_routed_sink_pb_graph_pins(
      pb, sink_pb_pin, atom_net_id, device_annotation, pb_pin_mapped_nets,
      pb_graph_pin_lookup_from_index, sink_pb_pins);
  }
}

/***************************************************************************************
 * A wrapper for the recursive function rec_find_route_sink_pb_graph_pins(),
 * we ensure that we provide a clear sink node lists
 ***************************************************************************************/
static std::vector<t_pb_graph_pin*> find_routed_pb_graph_pins_atom_net(
  const t_pb* pb, const t_pb_graph_pin* source_pb_pin,
  const t_pb_graph_pin* packing_source_pb_pin, const AtomNetId& atom_net_id,
  const VprDeviceAnnotation& device_annotation,
  const std::map<const t_pb_graph_pin*, AtomNetId>& pb_pin_mapped_nets,
  t_pb_graph_pin** pb_graph_pin_lookup_from_index) {
  std::vector<t_pb_graph_pin*> sink_pb_pins;

  /* Try to directly search for sink pb_pins from the source_pb_pin,
   * which is the actual source pin to be routed from
   * Note that the packing source_pb_pin is the source pin considered by
   * VPR packer, but may not be the actual source!!!
   */
  if (true == source_pb_pin->parent_node->is_root()) {
    bool direct_search_status =
      rec_direct_search_sink_pb_graph_pins(source_pb_pin, sink_pb_pins);
    if (true == direct_search_status) {
      VTR_ASSERT(!sink_pb_pins.empty());
      /* We have find through direct searching, return now */
      return sink_pb_pins;
    }

    /* Cannot find through direct searching, reset results */
    VTR_ASSERT_SAFE(false == direct_search_status);
    sink_pb_pins.clear();
  }

  rec_find_routed_sink_pb_graph_pins(
    pb, packing_source_pb_pin, atom_net_id, device_annotation,
    pb_pin_mapped_nets, pb_graph_pin_lookup_from_index, sink_pb_pins);

  return sink_pb_pins;
}

/***************************************************************************************
 * This function will find the actual routing traces of the demanded net
 * There is a specific search space applied when searching the routing traces:
 * - ONLY applicable to the pb_pin of top-level pb_graph_node
 * - First-tier candidates are in the same port of the source pin
 * - If nothing is found in first-tier, we find expand the range by considering
 *all the pins in the same type that are available at the top-level
 *pb_graph_node
 ***************************************************************************************/
static std::vector<int> find_pb_route_by_atom_net(
  const t_pb* pb, const t_pb_graph_pin* source_pb_pin,
  const AtomNetId& atom_net_id) {
  VTR_ASSERT(true == source_pb_pin->parent_node->is_root());

  std::vector<int> pb_route_indices;

  std::vector<int> candidate_pool;
  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) ||
        (AtomNetId::INVALID() == pb->pb_route.at(pin).atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    if (atom_net_id != pb->pb_route.at(pin).atom_net_id) {
      continue;
    }
    candidate_pool.push_back(pin);
  }

  for (int pin : candidate_pool) {
    if (source_pb_pin->port == pb->pb_route.at(pin).pb_graph_pin->port) {
      pb_route_indices.push_back(pin);
    }
  }

  if (pb_route_indices.empty()) {
    for (int pin : candidate_pool) {
      if (pb->pb_route.at(pin).pb_graph_pin->parent_node->is_root() &&
          is_pb_graph_pins_share_interc(source_pb_pin,
                                        pb->pb_route.at(pin).pb_graph_pin)) {
        pb_route_indices.push_back(pin);
      }
    }
  }

  return pb_route_indices;
}

/***************************************************************************************
 * This function will find the actual routing traces of the demanded net
 * There is a specific search space applied when searching the routing traces:
 * - ONLY applicable to the pb_pin of top-level pb_graph_node
 ***************************************************************************************/
static std::vector<int> find_pb_routes_by_atom_net_among_top_pb_pins(
  const t_pb* pb, const AtomNetId& atom_net_id) {
  std::vector<int> pb_route_indices;

  std::vector<int> candidate_pool;
  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) ||
        (AtomNetId::INVALID() == pb->pb_route.at(pin).atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    if (atom_net_id != pb->pb_route.at(pin).atom_net_id) {
      continue;
    }
    if (pb->pb_route.at(pin).pb_graph_pin->parent_node->is_root()) {
      candidate_pool.push_back(pin);
    }
  }
  return candidate_pool;
}

/***************************************************************************************
 * This function will find the actual routing traces of the demanded net
 * There is a specific search space applied when searching the routing traces:
 * - ONLY applicable to the pb_pin of top-level pb_graph_node
 * - First-tier candidates are in the same port of the source pin
 * - If nothing is found in first-tier, we find expand the range by considering
 *all the pins in the same type that are available at the top-level
 *pb_graph_node
 * - Exclude all the pb_route that is from a net on a pin which should be
 *ignored
 ***************************************************************************************/
static std::vector<int> find_pb_route_by_atom_net_exclude_blacklist(
  const t_pb* pb, const t_pb_graph_pin* source_pb_pin,
  const AtomNetId& atom_net_id,
  const std::map<AtomNetId, bool>& ignored_atom_nets,
  const RepackOption& options) {
  VTR_ASSERT(true == source_pb_pin->parent_node->is_root());

  std::vector<int> pb_route_indices;

  auto result = ignored_atom_nets.find(atom_net_id);

  std::vector<int> candidate_pool;
  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) ||
        (AtomNetId::INVALID() == pb->pb_route.at(pin).atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    if (atom_net_id != pb->pb_route.at(pin).atom_net_id) {
      continue;
    }
    BasicPort curr_pin(
      std::string(pb->pb_route.at(pin).pb_graph_pin->port->name),
      pb->pb_route.at(pin).pb_graph_pin->pin_number,
      pb->pb_route.at(pin).pb_graph_pin->pin_number);
    if (result != ignored_atom_nets.end() && result->second &&
        options.is_pin_ignore_global_nets(
          std::string(pb->pb_graph_node->pb_type->name), curr_pin)) {
      continue;
    }
    candidate_pool.push_back(pin);
  }

  for (int pin : candidate_pool) {
    if (source_pb_pin->port == pb->pb_route.at(pin).pb_graph_pin->port) {
      pb_route_indices.push_back(pin);
    }
  }

  if (pb_route_indices.empty()) {
    for (int pin : candidate_pool) {
      if (pb->pb_route.at(pin).pb_graph_pin->parent_node->is_root() &&
          is_pb_graph_pins_share_interc(source_pb_pin,
                                        pb->pb_route.at(pin).pb_graph_pin)) {
        pb_route_indices.push_back(pin);
      }
    }
  }

  return pb_route_indices;
}

/***************************************************************************************
 * This function will find the actual source_pb_pin that is mapped by packed in
 *the pb route As the inputs of clustered block may be renamed during routing,
 * our pb_route results may lose consistency.
 * It is possible that the source pb_pin may not be mapped during packing but
 * be mapped during routing
 *
 * Note: this is ONLY applicable to the pb_pin of top-level pb_graph_node
 ***************************************************************************************/
static std::vector<int> find_pb_route_remapped_source_pb_pin(
  const t_pb* pb, const t_pb_graph_pin* source_pb_pin,
  const AtomNetId& atom_net_id) {
  VTR_ASSERT(true == source_pb_pin->parent_node->is_root());

  std::vector<int> pb_route_indices;

  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) ||
        (AtomNetId::INVALID() == pb->pb_route.at(pin).atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    if (atom_net_id != pb->pb_route.at(pin).atom_net_id) {
      continue;
    }
    /* Only care the pin has the same parent port as source_pb_pin
     * Due to that the source_pb_pin may be swapped during routing
     * the pb_route is out-of-date
     *
     * For those parent port is defined as non-equivalent,
     * the source pin and the pin recorded in the routing trace must match!
     *
     * TODO: should update pb_route by post routing results
     * On the other side, the swapping can only happen between equivalent pins
     * in a port. So the port must match here!
     */
    if (PortEquivalence::FULL == source_pb_pin->port->equivalent) {
      if (source_pb_pin->port == pb->pb_route.at(pin).pb_graph_pin->port) {
        pb_route_indices.push_back(pin);
      }
    } else {
      /* NOTE: INSTANCE is NOT supported! We support only NONE equivalent */
      VTR_ASSERT(PortEquivalence::NONE == source_pb_pin->port->equivalent);
      if (source_pb_pin == pb->pb_route.at(pin).pb_graph_pin) {
        pb_route_indices.push_back(pin);
      }
    }
  }

  return pb_route_indices;
}

/***************************************************************************************
 * Find the corresponding nodes in a logical block routing resource graph
 * with a given list of sink pb_graph pins
 * Note that these sink pins belong to operating pb_graph_node,
 * we will find the associated physical pb_graph_node as well as physical pins
 * and then spot the nodes in lb_rr_graph
 ***************************************************************************************/
static std::vector<LbRRNodeId> find_lb_net_physical_sink_lb_rr_nodes(
  const LbRRGraph& lb_rr_graph, const std::vector<t_pb_graph_pin*>& sink_pins,
  const VprDeviceAnnotation& device_annotation) {
  std::vector<LbRRNodeId> sink_nodes;

  for (t_pb_graph_pin* sink_pin : sink_pins) {
    /* Find the physical pin */
    t_pb_graph_pin* physical_sink_pin = nullptr;
    if (true == sink_pin->parent_node->is_root()) {
      physical_sink_pin = sink_pin;
    } else {
      physical_sink_pin = device_annotation.physical_pb_graph_pin(sink_pin);
    }

    /* if this is the root node, the physical pin is its self */
    if (nullptr == physical_sink_pin) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Fail to find a physical pin for operating pin '%s'!\n",
                     sink_pin->to_string().c_str());
    }
    VTR_ASSERT(nullptr != physical_sink_pin);

    /* Sink nodes should NOT be any output pin of primitive pb_graph_node,
     * warn that we will not include it in the sink nodes
     */
    if ((true ==
         is_primitive_pb_type(physical_sink_pin->parent_node->pb_type)) &&
        (OUT_PORT == physical_sink_pin->port->type)) {
      VTR_LOGF_ERROR(
        __FILE__, __LINE__,
        "Sink pin '%s' should NOT be an output from a primitive pb_type!\n",
        sink_pin->to_string().c_str());
    }

    LbRRNodeId sink_lb_rr_node =
      lb_rr_graph.find_node(LB_INTERMEDIATE, physical_sink_pin);
    if (true != lb_rr_graph.valid_node_id(sink_lb_rr_node)) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Fail to find the lb_rr_node for pb_graph_pin '%s'\n",
                     physical_sink_pin->to_string().c_str());
    }
    VTR_ASSERT(true == lb_rr_graph.valid_node_id(sink_lb_rr_node));

    sink_nodes.push_back(sink_lb_rr_node);
  }

  return sink_nodes;
}

/***************************************************************************************
 * Create nets to be routed, including the source nodes and terminals
 * And add them to the logical block router
 ***************************************************************************************/
static void add_lb_router_nets(
  LbRouter& lb_router, t_logical_block_type_ptr lb_type,
  const LbRRGraph& lb_rr_graph, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const ClusteringContext& clustering_ctx,
  const VprClusteringAnnotation& clustering_annotation,
  const ClusterBlockId& block_id, const RepackOption& options) {
  size_t net_counter = 0;
  bool verbose = options.verbose_output();
  RepackDesignConstraints design_constraints = options.design_constraints();

  /* Two spots to find source nodes for each nets
   *  - nets that appear in the inputs of a clustered block
   *    Note that these nets may be moved to another input of the same cluster
   * block we will locate the final pin and consider its corresponding routing
   * resource node as source
   *  - nets that appear in the outputs of a primitive pb_graph_node
   *    Note that these primitive pb_graph node are operating pb_graph_node
   *    while we are considering physical pb_graph node
   *    Therefore, we will find the outputs of physical pb_graph_node
   * corresponding to the operating one and then consider the assoicated routing
   * resource node as source
   */
  t_pb* pb = clustering_ctx.clb_nlist.block_pb(block_id);
  VTR_ASSERT(true == pb->pb_graph_node->is_root());

  /* Build the fast look-up between pb_pin_id and pb_graph_pin pointer */
  t_pb_graph_pin** pb_graph_pin_lookup_from_index =
    alloc_and_load_pb_graph_pin_lookup_from_index(lb_type);

  /* Build a fast look-up between pb_graph_pin and atom net id which it is
   * mapped to Note that, we only care the pb_graph_pin at the root
   * pb_graph_node where pb_graph_pin may be remapped to a new net due to
   * routing optimization
   */
  std::map<const t_pb_graph_pin*, AtomNetId> pb_pin_mapped_nets;
  for (int j = 0; j < lb_type->pb_type->num_pins; j++) {
    /* Find the net mapped to this pin in clustering results*/
    ClusterNetId cluster_net_id =
      clustering_ctx.clb_nlist.block_net(block_id, j);
    /* Get the actual net id because it may be renamed during routing */
    if (true == clustering_annotation.is_net_renamed(block_id, j)) {
      cluster_net_id = clustering_annotation.net(block_id, j);
    }

    /* Bypass unmapped pins */
    if (ClusterNetId::INVALID() == cluster_net_id) {
      continue;
    }

    /* Get the source pb_graph pin and find the rr_node in logical block routing
     * resource graph */
    const t_pb_graph_pin* pb_pin =
      get_pb_graph_node_pin_from_block_pin(block_id, j);
    VTR_ASSERT(pb_pin->parent_node == pb->pb_graph_node);

    AtomNetId atom_net_id = atom_ctx.lookup.atom_net(cluster_net_id);
    VTR_ASSERT(AtomNetId::INVALID() != atom_net_id);

    pb_pin_mapped_nets[pb_pin] = atom_net_id;
  }

  /* Cache the sink nodes/routing traces for the global nets which is specifed
   * to be ignored on given pins */
  std::map<AtomNetId, std::vector<LbRRNodeId>> ignored_global_net_sinks;
  std::map<AtomNetId, bool> ignored_atom_nets;
  for (int j = 0; j < lb_type->pb_type->num_pins; j++) {
    /* Get the source pb_graph pin and find the rr_node in logical block routing
     * resource graph */
    const t_pb_graph_pin* source_pb_pin =
      get_pb_graph_node_pin_from_block_pin(block_id, j);
    VTR_ASSERT(source_pb_pin->parent_node == pb->pb_graph_node);

    /* Bypass output pins */
    if (OUT_PORT == source_pb_pin->port->type) {
      continue;
    }

    /* Find the net mapped to this pin in clustering results*/
    ClusterNetId cluster_net_id =
      clustering_ctx.clb_nlist.block_net(block_id, j);
    /* Get the actual net id because it may be renamed during routing */
    if (true == clustering_annotation.is_net_renamed(block_id, j)) {
      cluster_net_id = clustering_annotation.net(block_id, j);
    }

    /* Bypass unmapped pins */
    if (ClusterNetId::INVALID() == cluster_net_id) {
      continue;
    }

    /* Only for global net which should be ignored, cache the sink nodes */
    BasicPort curr_pin(std::string(source_pb_pin->port->name),
                       source_pb_pin->pin_number, source_pb_pin->pin_number);
    if ((clustering_ctx.clb_nlist.net_is_ignored(cluster_net_id) &&
         clustering_ctx.clb_nlist.net_is_global(cluster_net_id) &&
         options.is_pin_ignore_global_nets(std::string(lb_type->pb_type->name),
                                           curr_pin)) ||
        (options.net_is_specified_to_be_ignored(
          atom_ctx.nlist.net_name(pb_pin_mapped_nets[source_pb_pin]),
          std::string(lb_type->pb_type->name), curr_pin))) {
      /* Find the net mapped to this pin in clustering results*/
      AtomNetId atom_net_id = pb_pin_mapped_nets[source_pb_pin];
      std::vector<int> pb_route_indices =
        find_pb_route_by_atom_net(pb, source_pb_pin, atom_net_id);
      VTR_ASSERT(1 == pb_route_indices.size());
      int pb_route_index = pb_route_indices[0];
      t_pb_graph_pin* packing_source_pb_pin =
        get_pb_graph_node_pin_from_block_pin(block_id, pb_route_index);
      VTR_ASSERT(nullptr != packing_source_pb_pin);

      /* Find all the sink pins in the pb_route, we walk through the input pins
       * and find the pin  */
      std::vector<t_pb_graph_pin*> sink_pb_graph_pins =
        find_routed_pb_graph_pins_atom_net(
          pb, source_pb_pin, packing_source_pb_pin, atom_net_id,
          device_annotation, pb_pin_mapped_nets,
          pb_graph_pin_lookup_from_index);
      std::vector<LbRRNodeId> sink_lb_rr_nodes =
        find_lb_net_physical_sink_lb_rr_nodes(lb_rr_graph, sink_pb_graph_pins,
                                              device_annotation);
      VTR_ASSERT(sink_lb_rr_nodes.size() == sink_pb_graph_pins.size());
      ignored_global_net_sinks[atom_net_id].insert(
        ignored_global_net_sinks[atom_net_id].end(), sink_lb_rr_nodes.begin(),
        sink_lb_rr_nodes.end());
      ignored_atom_nets[atom_net_id] = true;
    }
  }

  /* Cache all the source nodes and sinks node for each net
   * net_terminal[net][0] is the list of source nodes
   * net_terminal[net][1] is the list of sink nodes
   */
  std::map<AtomNetId, std::array<std::vector<LbRRNodeId>, 2>> net_terminals;

  /* A list showing that some sinks should be touched by some sources in a
   * multi-source net */
  std::map<AtomNetId, std::map<LbRRNodeId, std::vector<LbRRNodeId>>>
    invisible_sinks;

  /* Find the source nodes for the nets mapped to inputs of a clustered block */
  for (int j = 0; j < lb_type->pb_type->num_pins; j++) {
    /* Get the source pb_graph pin and find the rr_node in logical block routing
     * resource graph */
    const t_pb_graph_pin* source_pb_pin =
      get_pb_graph_node_pin_from_block_pin(block_id, j);
    VTR_ASSERT(source_pb_pin->parent_node == pb->pb_graph_node);

    /* Bypass output pins */
    if (OUT_PORT == source_pb_pin->port->type) {
      continue;
    }

    /* Find the net mapped to this pin in clustering results*/
    AtomNetId atom_net_id = pb_pin_mapped_nets[source_pb_pin];

    BasicPort curr_pin(std::string(source_pb_pin->port->name),
                       source_pb_pin->pin_number, source_pb_pin->pin_number);
    /* Be very careful! There is only one routing trace for the net, it should
     * never be ignored! */
    if ((ignored_atom_nets[atom_net_id]) &&
        (find_pb_routes_by_atom_net_among_top_pb_pins(pb, atom_net_id).size() >
         1) &&
        (options.is_pin_ignore_global_nets(std::string(lb_type->pb_type->name),
                                           curr_pin))) {
      VTR_LOGV(verbose, "Skip net '%s' as it is global and set to be ignored\n",
               atom_ctx.nlist.net_name(atom_net_id).c_str());
      continue;
    }

    /* Check if the net information is constrained or not */
    std::string constrained_net_name =
      design_constraints.find_constrained_pin_net(
        std::string(lb_type->pb_type->name),
        BasicPort(std::string(source_pb_pin->port->name),
                  source_pb_pin->pin_number, source_pb_pin->pin_number));

    /* Find the constrained net mapped to this pin in clustering results */
    AtomNetId constrained_atom_net_id = AtomNetId::INVALID();

    /* If the pin is constrained, we need to
     * - if this is an open net, for invalid net then
     * - if this is valid net name, find the net id from atom_netlist
     *   and overwrite the atom net id to mapped
     */
    if ((!design_constraints.unconstrained_net(constrained_net_name)) &&
        (!design_constraints.unmapped_net(constrained_net_name))) {
      constrained_atom_net_id = atom_ctx.nlist.find_net(constrained_net_name);
      if (false == atom_ctx.nlist.valid_net_id(constrained_atom_net_id)) {
        VTR_LOG_WARN(
          "Invalid net '%s' to be constrained! Will drop the constraint in "
          "repacking\n",
          constrained_net_name.c_str());
      } else {
        VTR_ASSERT_SAFE(false ==
                        atom_ctx.nlist.valid_net_id(constrained_atom_net_id));
        VTR_LOGV(verbose,
                 "Accept net '%s' to be constrained on pin '%s[%d]' during "
                 "repacking\n",
                 constrained_net_name.c_str(), source_pb_pin->port->name,
                 source_pb_pin->pin_number);
      }
    } else if (design_constraints.unconstrained_net(constrained_net_name)) {
      constrained_atom_net_id = atom_net_id;
      /* Skip for the net which has been constrained on other pins */
      if (atom_net_id &&
          design_constraints.net_pin(atom_ctx.nlist.net_name(atom_net_id))
            .is_valid()) {
        VTR_LOGV(verbose,
                 "Skip net '%s' on pin '%s[%d]' during repacking since it has "
                 "been constrained to another pin\n",
                 atom_ctx.nlist.net_name(atom_net_id).c_str(),
                 source_pb_pin->port->name, source_pb_pin->pin_number);
        continue;
      }
      VTR_LOGV(verbose,
               "Follow the same mapping results for net '%s' on pin '%s[%d]' "
               "during repacking (constrained net name is %s)\n",
               atom_ctx.nlist.net_name(atom_net_id).c_str(),
               source_pb_pin->port->name, source_pb_pin->pin_number,
               constrained_net_name.c_str());
    }

    /* Bypass unmapped pins. There are 4 conditions to consider
     * +======+=================+=============+================================+
     * | Case | Packing results | Constraints | Decision to route              |
     * +======+=================+=============+================================+
     * |  0   |  Unmapped       | Unmapped    | No routing needed              |
     * +======+=================+=============+================================+
     * |  1   |  Unmapped       | Mapped      | Find the pb source pin that    |
     * |      |                 |             | drives the constrained net and |
     * |      |                 |             | use it to find sink nodes      |
     * +======+=================+=============+================================+
     * |  2   |  Mapped         | Unmapped    | No routing needed              |
     * +======+=================+=============+================================+
     * |  3   |  Mapped         | Mapped      | Route with the constrained net |
     * |      |                 |             | but use the packing net id to  |
     * |      |                 |             | find the sink nodes to route   |
     * +======+=================+=============+================================+
     */
    if (AtomNetId::INVALID() == constrained_atom_net_id) {
      continue;
    }

    /* If we have a net to route, it must be the constrained net */
    AtomNetId atom_net_id_to_route = constrained_atom_net_id;

    /* The outputs of pb_graph_node is INTERMEDIATE node in the routing resource
     * graph, they are all connected to a common source node
     */
    LbRRNodeId source_lb_rr_node =
      lb_rr_graph.find_node(LB_INTERMEDIATE, source_pb_pin);
    VTR_ASSERT(true == lb_rr_graph.valid_node_id(source_lb_rr_node));

    /* Output verbose messages for debugging only */
    VTR_LOGV(verbose, "Pb route for Net %s:\n",
             atom_ctx.nlist.net_name(atom_net_id_to_route).c_str());

    /* As the pin remapping is allowed during routing, we should
     * - Find the routing traces from packing results which is mapped to the net
     *   from the same port (as remapping is allowed for pins in the same port
     * only)
     * - Find the source pb_graph_pin that drives the routing traces during
     * packing
     * - Then we can find the sink nodes
     *
     * When there is a pin constraint applied. The routing trace
     * - Find the routing traces from packing results which is mapped to the net
     *   with the same port constraints
     */
    std::vector<int> pb_route_indices;
    if (design_constraints.unconstrained_net(constrained_net_name)) {
      VTR_LOGV(verbose,
               "Search remapped routing traces for the unconstrained net\n");
      pb_route_indices = find_pb_route_remapped_source_pb_pin(
        pb, source_pb_pin, atom_net_id_to_route);
    } else {
      /* If this is a constrained net but the source pin is not the pin that the
       * net is constrained to, w*/
      VTR_LOGV(verbose, "Search routing traces for the constrained net\n");
      pb_route_indices = find_pb_route_by_atom_net_exclude_blacklist(
        pb, source_pb_pin, atom_net_id_to_route, ignored_atom_nets, options);
    }
    /* It could happen that the constrained net is NOT used in this clb, we just
     * skip it for routing For example, a clkB net is never mapped to any ports
     * in the pb that is clocked by clkA net
     * */
    int pb_route_index;
    if (0 == pb_route_indices.size()) {
      if (ignored_global_net_sinks[atom_net_id_to_route].empty()) {
        VTR_LOGV(verbose, "Bypass routing due to no routing traces found\n");
        continue;
      } else {
        VTR_LOGV(verbose,
                 "No regular routing traces found but only invisible routing "
                 "traces will be considered\n");
      }
    } else if (1 == pb_route_indices.size()) {
      pb_route_index = pb_route_indices[0];
    } else {
      VTR_LOG_ERROR(
        "Found %d routing traces for net \'%s\' in clustered block \'%s\'. "
        "Expect only 1.\n",
        pb_route_indices.size(),
        atom_ctx.nlist.net_name(atom_net_id_to_route).c_str(),
        clustering_ctx.clb_nlist.block_name(block_id).c_str());
      VTR_ASSERT(1 == pb_route_indices.size());
    }
    t_pb_graph_pin* packing_source_pb_pin =
      get_pb_graph_node_pin_from_block_pin(block_id, pb_route_index);
    VTR_ASSERT(nullptr != packing_source_pb_pin);

    /* Find all the sink pins in the pb_route, we walk through the input pins
     * and find the pin  */
    std::vector<t_pb_graph_pin*> sink_pb_graph_pins =
      find_routed_pb_graph_pins_atom_net(
        pb, source_pb_pin, packing_source_pb_pin, atom_net_id_to_route,
        device_annotation, pb_pin_mapped_nets, pb_graph_pin_lookup_from_index);
    std::vector<LbRRNodeId> sink_lb_rr_nodes =
      find_lb_net_physical_sink_lb_rr_nodes(lb_rr_graph, sink_pb_graph_pins,
                                            device_annotation);
    VTR_ASSERT(sink_lb_rr_nodes.size() == sink_pb_graph_pins.size());

    /* Output verbose messages for debugging only */
    VTR_LOGV(verbose, "Source node:\n\t%s -> %s\n",
             source_pb_pin->to_string().c_str(),
             source_pb_pin->to_string().c_str());
    VTR_LOGV(verbose, "Sink nodes:\n");
    for (t_pb_graph_pin* sink_pb_pin : sink_pb_graph_pins) {
      VTR_LOGV(verbose, "\t%s\n", sink_pb_pin->to_string().c_str());
    }

    /* Append sink nodes from ignored global net cache */
    sink_lb_rr_nodes.insert(
      sink_lb_rr_nodes.end(),
      ignored_global_net_sinks[atom_net_id_to_route].begin(),
      ignored_global_net_sinks[atom_net_id_to_route].end());
    VTR_LOGV(
      verbose,
      "Append %ld sinks from the routing traces of ignored global nets\n",
      ignored_global_net_sinks[atom_net_id_to_route].size());

    /* Add the net */
    add_lb_router_net_to_route(
      lb_router, lb_rr_graph, std::vector<LbRRNodeId>(1, source_lb_rr_node),
      sink_lb_rr_nodes, atom_ctx, atom_net_id_to_route);

    net_counter++;
  }

  /* Find the source nodes for the nets mapped to outputs of primitive
   * pb_graph_node */
  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) ||
        (AtomNetId::INVALID() == pb->pb_route[pin].atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    int source_pb_pin_id = pb->pb_route[pin].driver_pb_pin_id;
    if (OPEN == source_pb_pin_id) {
      continue;
    }
    VTR_ASSERT(OPEN != source_pb_pin_id &&
               source_pb_pin_id < pb->pb_graph_node->total_pb_pins);
    /* Find the corresponding pb_graph_pin and its physical pb_graph_pin */
    t_pb_graph_pin* source_pb_pin =
      pb_graph_pin_lookup_from_index[source_pb_pin_id];
    /* Skip the pin from top-level pb_graph_node, they have been handled already
     */
    if (source_pb_pin->parent_node == pb->pb_graph_node) {
      continue;
    }

    /* The pin must be an output of a primitive pb_graph_node */
    if (OUT_PORT != source_pb_pin->port->type) {
      continue;
    }
    if (true != is_primitive_pb_type(source_pb_pin->parent_node->pb_type)) {
      continue;
    }

    /* The outputs of pb_graph_node is SOURCE node in the routing resource graph
     */
    t_pb_graph_pin* physical_source_pb_pin =
      device_annotation.physical_pb_graph_pin(source_pb_pin);
    LbRRNodeId source_lb_rr_node =
      lb_rr_graph.find_node(LB_SOURCE, physical_source_pb_pin);
    VTR_ASSERT(true == lb_rr_graph.valid_node_id(source_lb_rr_node));

    AtomNetId atom_net_id = pb->pb_route[pin].atom_net_id;
    VTR_ASSERT(AtomNetId::INVALID() != atom_net_id);

    /* Find all the sink pins in the pb_route */
    std::vector<t_pb_graph_pin*> sink_pb_graph_pins =
      find_routed_pb_graph_pins_atom_net(
        pb, physical_source_pb_pin, source_pb_pin, atom_net_id,
        device_annotation, pb_pin_mapped_nets, pb_graph_pin_lookup_from_index);

    std::vector<LbRRNodeId> sink_lb_rr_nodes =
      find_lb_net_physical_sink_lb_rr_nodes(lb_rr_graph, sink_pb_graph_pins,
                                            device_annotation);
    VTR_ASSERT(sink_lb_rr_nodes.size() == sink_pb_graph_pins.size());

    /* Printf for debugging only, may be enabled if verbose is enabled
     */
    VTR_LOGV(verbose, "Pb route for Net %s:\n",
             atom_ctx.nlist.net_name(atom_net_id).c_str());
    VTR_LOGV(verbose, "Source node:\n\t%s -> %s\n",
             source_pb_pin->to_string().c_str(),
             physical_source_pb_pin->to_string().c_str());
    VTR_LOGV(verbose, "Sink nodes:\n");
    for (t_pb_graph_pin* sink_pb_pin : sink_pb_graph_pins) {
      VTR_LOGV(verbose, "\t%s\n", sink_pb_pin->to_string().c_str());
    }

    /* Add the net */
    add_lb_router_net_to_route(lb_router, lb_rr_graph,
                               std::vector<LbRRNodeId>(1, source_lb_rr_node),
                               sink_lb_rr_nodes, atom_ctx, atom_net_id);
    net_counter++;
  }

  /* Free */
  free_pb_graph_pin_lookup_from_index(pb_graph_pin_lookup_from_index);

  VTR_LOGV(verbose, "Added %lu nets to be routed.\n", net_counter);
}

/***************************************************************************************
 * Repack a clustered block in the physical mode
 * This function will do
 * - Find the lb_rr_graph that is affiliated to the clustered block
 *   and initilize the logcial tile router
 * - Create nets to be routed, including the source nodes and terminals
 *   This should consider the net remapping in the clustering_annotation
 * - Run the router to finish the repacking
 * - Output routing results to data structure PhysicalPb and store it in
 *clustering annotation
 ***************************************************************************************/
static void repack_cluster(const AtomContext& atom_ctx,
                           const ClusteringContext& clustering_ctx,
                           const VprDeviceAnnotation& device_annotation,
                           VprClusteringAnnotation& clustering_annotation,
                           const VprBitstreamAnnotation& bitstream_annotation,
                           const ClusterBlockId& block_id,
                           const RepackOption& options) {
  /* Get the pb graph that current clustered block is mapped to */
  t_logical_block_type_ptr lb_type =
    clustering_ctx.clb_nlist.block_type(block_id);
  t_pb_graph_node* pb_graph_head = lb_type->pb_graph_head;
  VTR_ASSERT(nullptr != pb_graph_head);
  bool verbose = options.verbose_output();

  /* We should get a non-empty graph */
  const LbRRGraph& lb_rr_graph =
    device_annotation.physical_lb_rr_graph(pb_graph_head);
  VTR_ASSERT(!lb_rr_graph.empty());

  VTR_LOG("Repack clustered block '%s'...",
          clustering_ctx.clb_nlist.block_name(block_id).c_str());
  VTR_LOGV(verbose, "\n");

  /* Initialize the router */
  LbRouter lb_router(lb_rr_graph, lb_type);

  /* Add nets to be routed with source and terminals */
  add_lb_router_nets(
    lb_router, lb_type, lb_rr_graph, atom_ctx, device_annotation,
    clustering_ctx,
    const_cast<const VprClusteringAnnotation&>(clustering_annotation), block_id,
    options);

  /* Initialize the modes to expand routing trees with the physical modes in
   * device annotation This is a must-do before running the routeri in the
   * purpose of repacking!!!
   */
  lb_router.set_physical_pb_modes(lb_rr_graph, device_annotation);

  /* Run the router */
  bool route_success =
    lb_router.try_route(lb_rr_graph, atom_ctx.nlist, verbose);

  if (false == route_success) {
    VTR_LOGV(verbose, "Reroute failed\n");
    exit(1);
  }
  VTR_ASSERT(true == route_success);
  VTR_LOGV(verbose, "Reroute succeed\n");

  /* Annotate routing results to physical pb */
  PhysicalPb phy_pb;
  alloc_physical_pb_from_pb_graph(phy_pb, pb_graph_head, device_annotation);
  rec_update_physical_pb_from_operating_pb(
    phy_pb, clustering_ctx.clb_nlist.block_pb(block_id),
    clustering_ctx.clb_nlist.block_pb(block_id)->pb_route, atom_ctx,
    device_annotation, bitstream_annotation, verbose);
  /* Save routing results */
  save_lb_router_results_to_physical_pb(phy_pb, lb_router, lb_rr_graph,
                                        atom_ctx.nlist, verbose);
  VTR_LOGV(verbose, "Saved results in physical pb\n");

  /* Add the pb to clustering context */
  clustering_annotation.add_physical_pb(block_id, phy_pb);

  VTR_LOG("Done\n");
}

/***************************************************************************************
 * Repack each clustered blocks in the clustering context
 ***************************************************************************************/
static void repack_clusters(const AtomContext& atom_ctx,
                            const ClusteringContext& clustering_ctx,
                            const VprDeviceAnnotation& device_annotation,
                            VprClusteringAnnotation& clustering_annotation,
                            const VprBitstreamAnnotation& bitstream_annotation,
                            const RepackOption& options) {
  vtr::ScopedStartFinishTimer timer(
    "Repack clustered blocks to physical implementation of logical tile");

  for (auto blk_id : clustering_ctx.clb_nlist.blocks()) {
    repack_cluster(atom_ctx, clustering_ctx, device_annotation,
                   clustering_annotation, bitstream_annotation, blk_id,
                   options);
  }
}

/***************************************************************************************
 * VPR's packer may create wire LUTs for routing
 * Repacker will not remove these wire LUTs
 * But repacker may create more wire LUTs for routing
 * by exploiting the routability of the physical mode of a programmable block
 * This is why this annotation is required
 ***************************************************************************************/
static void identify_physical_pb_wire_lut_created_by_repack(
  VprClusteringAnnotation& cluster_annotation, const AtomContext& atom_ctx,
  const ClusteringContext& cluster_ctx,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Identify wire LUTs created by repacking");
  int wire_lut_counter = 0;

  for (auto blk_id : cluster_ctx.clb_nlist.blocks()) {
    PhysicalPb& physical_pb = cluster_annotation.mutable_physical_pb(blk_id);
    /* Find the LUT physical pb id */
    for (const PhysicalPbId& primitive_pb : physical_pb.primitive_pbs()) {
      CircuitModelId circuit_model = device_annotation.pb_type_circuit_model(
        physical_pb.pb_graph_node(primitive_pb)->pb_type);
      VTR_ASSERT(true == circuit_lib.valid_model_id(circuit_model));
      if (CIRCUIT_MODEL_LUT != circuit_lib.model_type(circuit_model)) {
        continue;
      }

      /* Reach here, we have a LUT to deal with. Find the wire LUT that mapped
       * to the LUT */
      wire_lut_counter += identify_one_physical_pb_wire_lut_created_by_repack(
        physical_pb, primitive_pb, device_annotation, atom_ctx, circuit_lib,
        verbose);
    }
  }

  VTR_LOG("Identified %d wire LUTs created by repacker\n", wire_lut_counter);
}

/***************************************************************************************
 * Top-level function to pack physical pb_graph
 * This function will do :
 *  - create physical lb_rr_graph for each pb_graph considering physical modes
 *only the lb_rr_graph will be added to device annotation
 *  - annotate nets to be routed for each clustered block from operating modes
 *of pb_graph to physical modes of pb_graph
 *  - rerun the routing for each clustered block
 *  - store the packing results to clustering annotation
 ***************************************************************************************/
void pack_physical_pbs(const DeviceContext& device_ctx,
                       const AtomContext& atom_ctx,
                       const ClusteringContext& clustering_ctx,
                       VprDeviceAnnotation& device_annotation,
                       VprClusteringAnnotation& clustering_annotation,
                       const VprBitstreamAnnotation& bitstream_annotation,
                       const CircuitLibrary& circuit_lib,
                       const RepackOption& options) {
  /* build the routing resource graph for each logical tile */
  build_physical_lb_rr_graphs(device_ctx, device_annotation,
                              options.verbose_output());

  /* Call the LbRouter to re-pack each clustered block to physical
   * implementation */
  repack_clusters(atom_ctx, clustering_ctx,
                  const_cast<const VprDeviceAnnotation&>(device_annotation),
                  clustering_annotation, bitstream_annotation, options);

  /* Annnotate wire LUTs that are ONLY created by repacker!!!
   * This is a MUST RUN!
   */
  identify_physical_pb_wire_lut_created_by_repack(
    clustering_annotation, atom_ctx, clustering_ctx, device_annotation,
    circuit_lib, options.verbose_output());
}

} /* end namespace openfpga */
