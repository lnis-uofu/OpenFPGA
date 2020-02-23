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
 * Try find the pin id which is mapped to a given atom net id in the context of pb route
 ***************************************************************************************/
static 
std::vector<t_pb_graph_pin*> find_routed_pb_graph_pins_atom_net(const t_pb* pb,
                                                                const AtomNetId& atom_net_id,
                                                                t_pb_graph_pin** pb_graph_pin_lookup_from_index) {
  std::vector<t_pb_graph_pin*> sink_pb_pins;  

  /* Find the sink nodes from top-level node */
  for (int pin = 0; pin < pb->pb_graph_node->total_pb_pins; ++pin) {
    /* Bypass unused pins */
    if ((0 == pb->pb_route.count(pin)) || (AtomNetId::INVALID() == pb->pb_route[pin].atom_net_id)) {
      continue;
    }
    /* Get the driver pb pin id, it must be valid */
    if (atom_net_id != pb->pb_route[pin].atom_net_id) {
      continue;
    }
    /* Check each sink nodes, if pin belongs to an input of a primitive pb_graph_node, it is what we want */
    for (const int& sink_pb_pin_id : pb->pb_route[pin].sink_pb_pin_ids) {
      t_pb_graph_pin* sink_pb_pin = pb_graph_pin_lookup_from_index[sink_pb_pin_id];
      VTR_ASSERT(nullptr != sink_pb_pin);
      /* We care only
       * - input pins of primitive nodes
       * - output pins of top node 
       */ 
      if ( (true == is_primitive_pb_type(sink_pb_pin->parent_node->pb_type))
        && (IN_PORT == sink_pb_pin->port->type)) {
        sink_pb_pins.push_back(sink_pb_pin);
      }

      if ( (true == sink_pb_pin->parent_node->is_root())
        && (OUT_PORT == sink_pb_pin->port->type)) {
        sink_pb_pins.push_back(sink_pb_pin);
      }
    }
  }

  return sink_pb_pins;
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
      VTR_LOG("Fail to find a physical pin for operating pin '%s'!\n",
              sink_pin->to_string().c_str());
    }
    VTR_ASSERT(nullptr != physical_sink_pin);
    LbRRNodeId sink_lb_rr_node = lb_rr_graph.find_node(LB_INTERMEDIATE, physical_sink_pin);
    if (true != lb_rr_graph.valid_node_id(sink_lb_rr_node)) {
      VTR_LOG("Try to find the lb_rr_node for pb_graph_pin '%s'\n",
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

  /* Find the source nodes for the nets mapped to inputs of a clustered block */
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
    const t_pb_graph_pin* source_pb_pin = get_pb_graph_node_pin_from_block_pin(block_id, j);
    VTR_ASSERT(source_pb_pin->parent_node == pb->pb_graph_node);
    /* Bypass output pins */
    if (OUT_PORT == source_pb_pin->port->type) {
      continue;
    }

    /* The outputs of pb_graph_node is INTERMEDIATE node in the routing resource graph,
     * they are all connected to a common source node
     */
    LbRRNodeId source_lb_rr_node = lb_rr_graph.find_node(LB_INTERMEDIATE, source_pb_pin);
    VTR_ASSERT(true == lb_rr_graph.valid_node_id(source_lb_rr_node));

    AtomNetId atom_net_id = atom_ctx.lookup.atom_net(cluster_net_id);
    VTR_ASSERT(AtomNetId::INVALID() != atom_net_id);

    /* Find all the sink pins in the pb_route, we walk through the input pins and find the pin  */
    std::vector<t_pb_graph_pin*> sink_pb_graph_pins = find_routed_pb_graph_pins_atom_net(pb, atom_net_id, pb_graph_pin_lookup_from_index);
    std::vector<LbRRNodeId> sink_lb_rr_nodes = find_lb_net_physical_sink_lb_rr_nodes(lb_rr_graph, sink_pb_graph_pins, device_annotation);
    VTR_ASSERT(sink_lb_rr_nodes.size() == sink_pb_graph_pins.size());

    /* Add the net */
    add_lb_router_net_to_route(lb_router, lb_rr_graph,
                               source_lb_rr_node, sink_lb_rr_nodes,
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
    std::vector<t_pb_graph_pin*> sink_pb_graph_pins = find_routed_pb_graph_pins_atom_net(pb, atom_net_id, pb_graph_pin_lookup_from_index);
    std::vector<LbRRNodeId> sink_lb_rr_nodes = find_lb_net_physical_sink_lb_rr_nodes(lb_rr_graph, sink_pb_graph_pins, device_annotation);
    VTR_ASSERT(sink_lb_rr_nodes.size() == sink_pb_graph_pins.size());

    /* Add the net */
    add_lb_router_net_to_route(lb_router, lb_rr_graph,
                               source_lb_rr_node, sink_lb_rr_nodes,
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
                                           device_annotation);
  /* TODO: save routing results */
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
                     const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Repack clustered blocks to physical implementation of logical tile");

  for (auto blk_id : clustering_ctx.clb_nlist.blocks()) {
    repack_cluster(atom_ctx, clustering_ctx, 
                   device_annotation, clustering_annotation, 
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
                       const bool& verbose) {

  /* build the routing resource graph for each logical tile */
  build_physical_lb_rr_graphs(device_ctx,
                              device_annotation,
                              verbose);

  /* Call the LbRouter to re-pack each clustered block to physical implementation */ 
  repack_clusters(atom_ctx, clustering_ctx, 
                  const_cast<const VprDeviceAnnotation&>(device_annotation), clustering_annotation, 
                  verbose);
}

} /* end namespace openfpga */
