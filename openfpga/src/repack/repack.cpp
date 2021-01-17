/***************************************************************************************
 * This file includes functions that are used to redo packing for physical pbs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "pb_type_utils.h"
#include "build_physical_lb_rr_graph.h"
#include "lb_router.h"
#include "lb_router_utils.h"
#include "physical_pb_utils.h"
#include "repack.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Try to find sink pb graph pins through walking through the fan-out edges from
 * the source pb graph pin
 * Only the sink meeting the following requirements can be considered:
 * - All the fan-out edges between the source and sink are from direct interconnection
 * - sink is an input of a primitive pb_type
 * 
 * Note: 
 *  - If there is a fan-out of the current source pb graph pin is not a direct interconnection
 *    the direct search should stop. 
 *  - This function is designed for pb graph without local routing
 *    For example: direct connection between root pb graph node to the LUT pb graph node
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
static 
bool rec_direct_search_sink_pb_graph_pins(const t_pb_graph_pin* source_pb_pin,
                                          std::vector<t_pb_graph_pin*>& sink_pb_pins) {

  std::vector<t_pb_graph_pin*> sink_pb_pins_to_search;

  for (int iedge = 0; iedge < source_pb_pin->num_output_edges; ++iedge) {
    if (DIRECT_INTERC != source_pb_pin->output_edges[iedge]->interconnect->type) {
      return false;
    }
    for (int ipin = 0; ipin < source_pb_pin->output_edges[iedge]->num_output_pins; ++ipin) {
      t_pb_graph_pin* cand_sink_pb_pin = source_pb_pin->output_edges[iedge]->output_pins[ipin];
      if ( (true == is_primitive_pb_type(cand_sink_pb_pin->parent_node->pb_type))
        && (IN_PORT == cand_sink_pb_pin->port->type)) {
        sink_pb_pins.push_back(cand_sink_pb_pin);
      } else if ( (true == cand_sink_pb_pin->parent_node->is_root())
                && (OUT_PORT == cand_sink_pb_pin->port->type)) {
        sink_pb_pins.push_back(cand_sink_pb_pin);
      } else {
        sink_pb_pins_to_search.push_back(cand_sink_pb_pin);
      }
    }
  }

  for (t_pb_graph_pin* sink_pb_pin : sink_pb_pins_to_search) {
    bool direct_search_status = rec_direct_search_sink_pb_graph_pins(sink_pb_pin, sink_pb_pins);
    if (false == direct_search_status) {
      return false;
    }
  }

  /* Reach here, we succeed. */
  return true;
}

/***************************************************************************************
 * Try find all the sink pins which is mapped to a routing trace in the context of pb route
 * This function uses a recursive walk-through over the pb_route
 * We will always start from the pb_route of the source pin 
 * For each sink, 
 * - if it is the end point of a routing tree, we add it to the sink list
 *   - An output of top-level pb_graph_node
 *   - An input of a primitive pb_graph_node
 * - if it is not the end point of a routing tree, we visit the pb_route 
 *   corresponds to the sink pin
 *
 * Note: when you call this function at the top-level, please provide an empty vector
 *       of sink_pb_pins!!!
 ***************************************************************************************/
static 
void rec_find_routed_sink_pb_graph_pins(const t_pb* pb,
                                        const t_pb_graph_pin* source_pb_pin,
                                        const AtomNetId& atom_net_id,
                                        const VprDeviceAnnotation& device_annotation,
                                        const std::map<const t_pb_graph_pin*, AtomNetId>& pb_pin_mapped_nets,
                                        t_pb_graph_pin** pb_graph_pin_lookup_from_index,
                                        std::vector<t_pb_graph_pin*>& sink_pb_pins) {

  /* Bypass unused pins */
  if (0 == pb->pb_route.count(source_pb_pin->pin_count_in_cluster)) {
    return;
  }

  /* Get the driver pb pin id, it must be valid */
  if (atom_net_id != pb->pb_route[source_pb_pin->pin_count_in_cluster].atom_net_id) {
    return;
  }

  /* Check each sink nodes, if pin belongs to an input of a primitive pb_graph_node, it is what we want */
  std::vector<t_pb_graph_pin*> sink_pb_pins_to_search;
  for (const int& sink_pb_pin_id : pb->pb_route[source_pb_pin->pin_count_in_cluster].sink_pb_pin_ids) {
    t_pb_graph_pin* sink_pb_pin = pb_graph_pin_lookup_from_index[sink_pb_pin_id];
    VTR_ASSERT(nullptr != sink_pb_pin);

    /* We will update sink node list only
     * - input pins of primitive nodes
     * - output pins of top node 
     */ 
    if ( (true == is_primitive_pb_type(sink_pb_pin->parent_node->pb_type))
      && (IN_PORT == sink_pb_pin->port->type)) {
      sink_pb_pins.push_back(sink_pb_pin);
      continue;
    }

    if ( (true == sink_pb_pin->parent_node->is_root())
      && (OUT_PORT == sink_pb_pin->port->type)) {
      /* Be careful!!! There is an inconsistency between pb_route and actual net mapping!
       * The sink_pb_pin in the pb_route may not be the one we want
       * due to net remapping in the routing stage
       * If the net becomes invalid, we search all the fan-out of the source pb_pin 
       * and find one that is mapped to the net
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
          int parent_mode_index = source_pb_pin->output_edges[iedge]->interconnect->parent_mode_index;
          VTR_ASSERT(parent_mode_index < sink_pb_pin->parent_node->pb_type->num_modes);
          if (&(sink_pb_pin->parent_node->pb_type->modes[parent_mode_index])
              != device_annotation.physical_mode(sink_pb_pin->parent_node->pb_type)) {
            continue;
          }
          for (int ipin = 0; ipin < source_pb_pin->output_edges[iedge]->num_output_pins; ++ipin) {
            const t_pb_graph_pin* cand_sink_pb_pin = source_pb_pin->output_edges[iedge]->output_pins[ipin];
            auto cand_remapped_result = pb_pin_mapped_nets.find(cand_sink_pb_pin);
            AtomNetId cand_sink_pb_pin_net = AtomNetId::INVALID(); 
            if (cand_remapped_result != pb_pin_mapped_nets.end()) {
              cand_sink_pb_pin_net = cand_remapped_result->second; 
            }
            if (atom_net_id == cand_sink_pb_pin_net) {
              sink_pb_pins.push_back(const_cast<t_pb_graph_pin*>(cand_sink_pb_pin));
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
    rec_find_routed_sink_pb_graph_pins(pb, sink_pb_pin, atom_net_id, device_annotation, pb_pin_mapped_nets, pb_graph_pin_lookup_from_index, sink_pb_pins);
  }
}

/***************************************************************************************
 * A wrapper for the recursive function rec_find_route_sink_pb_graph_pins(), 
 * we ensure that we provide a clear sink node lists 
 ***************************************************************************************/
static 
std::vector<t_pb_graph_pin*> find_routed_pb_graph_pins_atom_net(const t_pb* pb,
                                                                const t_pb_graph_pin* source_pb_pin,
                                                                const t_pb_graph_pin* packing_source_pb_pin,
                                                                const AtomNetId& atom_net_id,
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
    bool direct_search_status = rec_direct_search_sink_pb_graph_pins(source_pb_pin, sink_pb_pins);
    if (true == direct_search_status) {
      VTR_ASSERT(!sink_pb_pins.empty());
      /* We have find through direct searching, return now */
      return sink_pb_pins;
    }

    /* Cannot find through direct searching, reset results */
    VTR_ASSERT_SAFE(false == direct_search_status);
    sink_pb_pins.clear();
  }

  rec_find_routed_sink_pb_graph_pins(pb, packing_source_pb_pin, atom_net_id, device_annotation, pb_pin_mapped_nets, pb_graph_pin_lookup_from_index, sink_pb_pins);

  return sink_pb_pins; 
}

/***************************************************************************************
 * This function will find the actual source_pb_pin that is mapped by packed in the pb route
 * As the inputs of clustered block may be renamed during routing,
 * our pb_route results may lose consistency.
 * It is possible that the source pb_pin may not be mapped during packing but
 * be mapped during routing
 *
 * Note: this is ONLY applicable to the pb_pin of top-level pb_graph_node 
 ***************************************************************************************/
static 
int find_pb_route_remapped_source_pb_pin(const t_pb* pb,
                                         const t_pb_graph_pin* source_pb_pin,
                                         const AtomNetId& atom_net_id) {
  VTR_ASSERT(true == source_pb_pin->parent_node->is_root()); 

  std::vector<int> pb_route_indices;

  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) || (AtomNetId::INVALID() == pb->pb_route.at(pin).atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    if (atom_net_id != pb->pb_route.at(pin).atom_net_id) {
      continue;
    }
    /* Only care the pin has the same parent port as source_pb_pin 
     * Due to that the source_pb_pin may be swapped during routing
     * the pb_route is out-of-date
     * TODO: should update pb_route by post routing results
     * On the other side, the swapping can only happen between equivalent pins
     * in a port. So the port must match here!
     */
    if (source_pb_pin->port == pb->pb_route.at(pin).pb_graph_pin->port) {
      pb_route_indices.push_back(pin);
    } 
  }

  VTR_ASSERT(1 == pb_route_indices.size());

  return pb_route_indices[0];
}

/***************************************************************************************
 * Find the corresponding nodes in a logical block routing resource graph
 * with a given list of sink pb_graph pins
 * Note that these sink pins belong to operating pb_graph_node,
 * we will find the associated physical pb_graph_node as well as physical pins
 * and then spot the nodes in lb_rr_graph
 ***************************************************************************************/
static 
std::vector<LbRRNodeId> find_lb_net_physical_sink_lb_rr_nodes(const LbRRGraph& lb_rr_graph,
                                                              const std::vector<t_pb_graph_pin*>& sink_pins,
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
    if ( (true == is_primitive_pb_type(physical_sink_pin->parent_node->pb_type))
      && (OUT_PORT == physical_sink_pin->port->type)) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Sink pin '%s' should NOT be an output from a primitive pb_type!\n",
                     sink_pin->to_string().c_str());
    }

    LbRRNodeId sink_lb_rr_node = lb_rr_graph.find_node(LB_INTERMEDIATE, physical_sink_pin);
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
static 
void add_lb_router_nets(LbRouter& lb_router,
                        t_logical_block_type_ptr lb_type,
                        const LbRRGraph& lb_rr_graph,
                        const AtomContext& atom_ctx,
                        const VprDeviceAnnotation& device_annotation,
                        const ClusteringContext& clustering_ctx,
                        const VprClusteringAnnotation& clustering_annotation,
                        const RepackDesignConstraints& design_constraints,
                        const ClusterBlockId& block_id,
                        const bool& verbose) {
  size_t net_counter = 0;

  /* Two spots to find source nodes for each nets
   *  - nets that appear in the inputs of a clustered block
   *    Note that these nets may be moved to another input of the same cluster block
   *    we will locate the final pin and consider its corresponding routing resource node as source
   *  - nets that appear in the outputs of a primitive pb_graph_node
   *    Note that these primitive pb_graph node are operating pb_graph_node
   *    while we are considering physical pb_graph node
   *    Therefore, we will find the outputs of physical pb_graph_node corresponding to the operating one
   *    and then consider the assoicated routing resource node as source
   */
  t_pb* pb = clustering_ctx.clb_nlist.block_pb(block_id);
  VTR_ASSERT(true == pb->pb_graph_node->is_root());

  /* Build the fast look-up between pb_pin_id and pb_graph_pin pointer */
  t_pb_graph_pin** pb_graph_pin_lookup_from_index = alloc_and_load_pb_graph_pin_lookup_from_index(lb_type);

  /* Build a fast look-up between pb_graph_pin and atom net id which it is mapped to 
   * Note that, we only care the pb_graph_pin at the root pb_graph_node
   * where pb_graph_pin may be remapped to a new net due to routing optimization
   */
  std::map<const t_pb_graph_pin*, AtomNetId> pb_pin_mapped_nets;
  for (int j = 0; j < lb_type->pb_type->num_pins; j++) {
    /* Find the net mapped to this pin in clustering results*/
    ClusterNetId cluster_net_id = clustering_ctx.clb_nlist.block_net(block_id, j);
    /* Get the actual net id because it may be renamed during routing */
    if (true == clustering_annotation.is_net_renamed(block_id, j)) {
      cluster_net_id = clustering_annotation.net(block_id, j);
    }

    /* Bypass unmapped pins */
    if (ClusterNetId::INVALID() == cluster_net_id) {
      continue;
    }

    /* Get the source pb_graph pin and find the rr_node in logical block routing resource graph */
    const t_pb_graph_pin* pb_pin = get_pb_graph_node_pin_from_block_pin(block_id, j);
    VTR_ASSERT(pb_pin->parent_node == pb->pb_graph_node);

    AtomNetId atom_net_id = atom_ctx.lookup.atom_net(cluster_net_id);
    VTR_ASSERT(AtomNetId::INVALID() != atom_net_id);

    pb_pin_mapped_nets[pb_pin] = atom_net_id;
  }

  /* Cache all the source nodes and sinks node for each net
   * net_terminal[net][0] is the list of source nodes 
   * net_terminal[net][1] is the list of sink nodes 
   */
  std::map<AtomNetId, std::array<std::vector<LbRRNodeId>, 2>> net_terminals;

  /* A list showing that some sinks should be touched by some sources in a multi-source net */
  std::map<AtomNetId, std::map<LbRRNodeId, std::vector<LbRRNodeId>>> invisible_sinks;

  /* Find the source nodes for the nets mapped to inputs of a clustered block */
  for (int j = 0; j < lb_type->pb_type->num_pins; j++) {
    /* Get the source pb_graph pin and find the rr_node in logical block routing resource graph */
    const t_pb_graph_pin* source_pb_pin = get_pb_graph_node_pin_from_block_pin(block_id, j);
    VTR_ASSERT(source_pb_pin->parent_node == pb->pb_graph_node);

    /* Bypass output pins */
    if (OUT_PORT == source_pb_pin->port->type) {
      continue;
    }

    /* Find the net mapped to this pin in clustering results*/
    AtomNetId atom_net_id = pb_pin_mapped_nets[source_pb_pin];
    /* Bypass unmapped pins */
    if (AtomNetId::INVALID() == atom_net_id) {
      continue;
    }

    /* The outputs of pb_graph_node is INTERMEDIATE node in the routing resource graph,
     * they are all connected to a common source node
     */
    LbRRNodeId source_lb_rr_node = lb_rr_graph.find_node(LB_INTERMEDIATE, source_pb_pin);
    VTR_ASSERT(true == lb_rr_graph.valid_node_id(source_lb_rr_node));

    int pb_route_index = find_pb_route_remapped_source_pb_pin(pb, source_pb_pin, atom_net_id);
    t_pb_graph_pin* packing_source_pb_pin = get_pb_graph_node_pin_from_block_pin(block_id, pb_route_index);
    VTR_ASSERT(nullptr != packing_source_pb_pin);

    /* Find all the sink pins in the pb_route, we walk through the input pins and find the pin  */
    std::vector<t_pb_graph_pin*> sink_pb_graph_pins = find_routed_pb_graph_pins_atom_net(pb, source_pb_pin, packing_source_pb_pin, atom_net_id, device_annotation, pb_pin_mapped_nets, pb_graph_pin_lookup_from_index);
    std::vector<LbRRNodeId> sink_lb_rr_nodes = find_lb_net_physical_sink_lb_rr_nodes(lb_rr_graph, sink_pb_graph_pins, device_annotation);
    VTR_ASSERT(sink_lb_rr_nodes.size() == sink_pb_graph_pins.size());

    /* Printf for debugging only, may be enabled if verbose is enabled
     */
    VTR_LOGV(verbose,
             "Pb route for Net %s:\n", 
             atom_ctx.nlist.net_name(atom_net_id).c_str());
    VTR_LOGV(verbose,
             "Source node:\n\t%s -> %s\n",
             source_pb_pin->to_string().c_str(),
             source_pb_pin->to_string().c_str());
    VTR_LOGV(verbose, "Sink nodes:\n");
    for (t_pb_graph_pin* sink_pb_pin : sink_pb_graph_pins) {
      VTR_LOGV(verbose,
               "\t%s\n",
               sink_pb_pin->to_string().c_str());
    }

    /* Add the net */
    add_lb_router_net_to_route(lb_router, lb_rr_graph,
                               std::vector<LbRRNodeId>(1, source_lb_rr_node),
                               sink_lb_rr_nodes,
                               atom_ctx, atom_net_id);

    net_counter++;
  }

  /* Find the source nodes for the nets mapped to outputs of primitive pb_graph_node */
  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) || (AtomNetId::INVALID() == pb->pb_route[pin].atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    int source_pb_pin_id = pb->pb_route[pin].driver_pb_pin_id;
    if (OPEN == source_pb_pin_id) {
      continue;
    }
    VTR_ASSERT(OPEN != source_pb_pin_id && source_pb_pin_id < pb->pb_graph_node->total_pb_pins);
    /* Find the corresponding pb_graph_pin and its physical pb_graph_pin */
    t_pb_graph_pin* source_pb_pin = pb_graph_pin_lookup_from_index[source_pb_pin_id];
    /* Skip the pin from top-level pb_graph_node, they have been handled already */ 
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

    /* The outputs of pb_graph_node is SOURCE node in the routing resource graph */
    t_pb_graph_pin* physical_source_pb_pin = device_annotation.physical_pb_graph_pin(source_pb_pin);
    LbRRNodeId source_lb_rr_node = lb_rr_graph.find_node(LB_SOURCE, physical_source_pb_pin);
    VTR_ASSERT(true == lb_rr_graph.valid_node_id(source_lb_rr_node));

    AtomNetId atom_net_id = pb->pb_route[pin].atom_net_id;
    VTR_ASSERT(AtomNetId::INVALID() != atom_net_id);

    /* Find all the sink pins in the pb_route */
    std::vector<t_pb_graph_pin*> sink_pb_graph_pins = find_routed_pb_graph_pins_atom_net(pb, physical_source_pb_pin, source_pb_pin, atom_net_id, device_annotation, pb_pin_mapped_nets, pb_graph_pin_lookup_from_index);

    std::vector<LbRRNodeId> sink_lb_rr_nodes = find_lb_net_physical_sink_lb_rr_nodes(lb_rr_graph, sink_pb_graph_pins, device_annotation);
    VTR_ASSERT(sink_lb_rr_nodes.size() == sink_pb_graph_pins.size());

    /* Printf for debugging only, may be enabled if verbose is enabled
     */
    VTR_LOGV(verbose,
             "Pb route for Net %s:\n", 
             atom_ctx.nlist.net_name(atom_net_id).c_str());
    VTR_LOGV(verbose,
             "Source node:\n\t%s -> %s\n",
             source_pb_pin->to_string().c_str(),
             physical_source_pb_pin->to_string().c_str());
    VTR_LOGV(verbose, "Sink nodes:\n");
    for (t_pb_graph_pin* sink_pb_pin : sink_pb_graph_pins) {
      VTR_LOGV(verbose,
               "\t%s\n",
               sink_pb_pin->to_string().c_str());
    }

    /* Add the net */
    add_lb_router_net_to_route(lb_router, lb_rr_graph,
                               std::vector<LbRRNodeId>(1, source_lb_rr_node),
                               sink_lb_rr_nodes,
                               atom_ctx, atom_net_id);
    net_counter++;
  } 

  /* Free */
  free_pb_graph_pin_lookup_from_index(pb_graph_pin_lookup_from_index);

  VTR_LOGV(verbose,
           "Added %lu nets to be routed.\n",
           net_counter);
}

/***************************************************************************************
 * Repack a clustered block in the physical mode
 * This function will do 
 * - Find the lb_rr_graph that is affiliated to the clustered block 
 *   and initilize the logcial tile router 
 * - Create nets to be routed, including the source nodes and terminals
 *   This should consider the net remapping in the clustering_annotation 
 * - Run the router to finish the repacking
 * - Output routing results to data structure PhysicalPb and store it in clustering annotation
 ***************************************************************************************/
static 
void repack_cluster(const AtomContext& atom_ctx,
                    const ClusteringContext& clustering_ctx,
                    const VprDeviceAnnotation& device_annotation,
                    VprClusteringAnnotation& clustering_annotation,
                    const RepackDesignConstraints& design_constraints,
                    const ClusterBlockId& block_id,
                    const bool& verbose) {
  /* Get the pb graph that current clustered block is mapped to */
  t_logical_block_type_ptr lb_type = clustering_ctx.clb_nlist.block_type(block_id);
  t_pb_graph_node* pb_graph_head = lb_type->pb_graph_head;
  VTR_ASSERT(nullptr != pb_graph_head);

  /* We should get a non-empty graph */
  const LbRRGraph& lb_rr_graph = device_annotation.physical_lb_rr_graph(pb_graph_head);
  VTR_ASSERT(!lb_rr_graph.empty());

  VTR_LOG("Repack clustered block '%s'...",
          clustering_ctx.clb_nlist.block_name(block_id).c_str());
  VTR_LOGV(verbose, "\n");

  /* Initialize the router */
  LbRouter lb_router(lb_rr_graph, lb_type);

  /* Add nets to be routed with source and terminals */
  add_lb_router_nets(lb_router, lb_type, lb_rr_graph, atom_ctx, device_annotation,
                     clustering_ctx, const_cast<const VprClusteringAnnotation&>(clustering_annotation),
                     design_constraints,
                     block_id, verbose);

  /* Initialize the modes to expand routing trees with the physical modes in device annotation
   * This is a must-do before running the routeri in the purpose of repacking!!!
   */
  lb_router.set_physical_pb_modes(lb_rr_graph, device_annotation); 

  /* Run the router */
  bool route_success = lb_router.try_route(lb_rr_graph, atom_ctx.nlist, verbose);

  if (false == route_success) {
    VTR_LOGV(verbose, "Reroute failed\n");
    exit(1);
  }
  VTR_ASSERT(true == route_success);
  VTR_LOGV(verbose, "Reroute succeed\n");

  /* Annotate routing results to physical pb */
  PhysicalPb phy_pb;
  alloc_physical_pb_from_pb_graph(phy_pb, pb_graph_head, device_annotation);
  rec_update_physical_pb_from_operating_pb(phy_pb,
                                           clustering_ctx.clb_nlist.block_pb(block_id),
                                           clustering_ctx.clb_nlist.block_pb(block_id)->pb_route,
                                           atom_ctx,
                                           device_annotation,
                                           verbose);
  /* Save routing results */
  save_lb_router_results_to_physical_pb(phy_pb, lb_router, lb_rr_graph);
  VTR_LOGV(verbose, "Saved results in physical pb\n");

  /* Add the pb to clustering context */
  clustering_annotation.add_physical_pb(block_id, phy_pb);

  VTR_LOG("Done\n");
}

/***************************************************************************************
 * Repack each clustered blocks in the clustering context
 ***************************************************************************************/
static 
void repack_clusters(const AtomContext& atom_ctx,
                     const ClusteringContext& clustering_ctx,
                     const VprDeviceAnnotation& device_annotation,
                     VprClusteringAnnotation& clustering_annotation,
                     const RepackDesignConstraints& design_constraints,
                     const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Repack clustered blocks to physical implementation of logical tile");

  for (auto blk_id : clustering_ctx.clb_nlist.blocks()) {
    repack_cluster(atom_ctx, clustering_ctx, 
                   device_annotation,
                   clustering_annotation, 
                   design_constraints,
                   blk_id, verbose);
  }
}

/***************************************************************************************
 * Top-level function to pack physical pb_graph 
 * This function will do :
 *  - create physical lb_rr_graph for each pb_graph considering physical modes only
 *    the lb_rr_graph will be added to device annotation
 *  - annotate nets to be routed for each clustered block from operating modes of pb_graph 
 *    to physical modes of pb_graph
 *  - rerun the routing for each clustered block
 *  - store the packing results to clustering annotation
 ***************************************************************************************/
void pack_physical_pbs(const DeviceContext& device_ctx,
                       const AtomContext& atom_ctx,
                       const ClusteringContext& clustering_ctx,
                       VprDeviceAnnotation& device_annotation,
                       VprClusteringAnnotation& clustering_annotation,
                       const RepackDesignConstraints& design_constraints,
                       const bool& verbose) {

  /* build the routing resource graph for each logical tile */
  build_physical_lb_rr_graphs(device_ctx,
                              device_annotation,
                              verbose);

  /* Call the LbRouter to re-pack each clustered block to physical implementation */ 
  repack_clusters(atom_ctx, clustering_ctx, 
                  const_cast<const VprDeviceAnnotation&>(device_annotation), clustering_annotation, 
                  design_constraints,
                  verbose);
}

} /* end namespace openfpga */
