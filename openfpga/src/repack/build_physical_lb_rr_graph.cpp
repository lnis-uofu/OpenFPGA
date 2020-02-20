/***************************************************************************************
 * This file includes functions that are used to redo packing for physical pbs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "pb_type_utils.h"

#include "build_physical_lb_rr_graph.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Create all the intermediate nodes for lb_rr_graph for each pb_graph_node.
 * Different from the lb_rr_graph builder in VPR packer, this function only consider 
 * the pb_graph_node under physical modes
 ***************************************************************************************/
static 
void rec_build_physical_lb_rr_node_for_pb_graph_node(t_pb_graph_node* pb_graph_node,
                                                     LbRRGraph& lb_rr_graph,
                                                     const VprDeviceAnnotation& device_annotation) {
  t_pb_type* pb_type = pb_graph_node->pb_type;

  /* TODO: think if we need to consider wire mode of LUT when creating the lb_rr_graph here! 
   * Should we create edges through the LUT input and output nodes?
   */

  /* The only difference between primitive node and intermediate nodes is
   * the output pins of primitive node will be SINK node
   * Otherwise it is always INTERMEDIATE node
   */
  e_lb_rr_type output_pin_rr_type = LB_INTERMEDIATE;
  if (true == is_primitive_pb_type(pb_type)) {
    output_pin_rr_type = LB_SOURCE; 
  }

  /* alloc and load input pins that connect to sinks */
  for (int iport = 0; iport < pb_graph_node->num_input_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_node->num_input_pins[iport]; ipin++) {
      /* load intermediate indices */
      t_pb_graph_pin* pb_pin = &pb_graph_node->input_pins[iport][ipin];

      /* alloc and load rr node info */
      LbRRNodeId node = lb_rr_graph.create_node(LB_INTERMEDIATE);
      lb_rr_graph.set_node_capacity(node, 1);
      lb_rr_graph.set_node_pb_graph_pin(node, pb_pin);

      /* TODO: Double check if this is the case */
      lb_rr_graph.set_node_intrinsic_cost(node, 1);
    }
  }

  /* alloc and load input pins that connect to sinks */
  for (int iport = 0; iport < pb_graph_node->num_clock_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_node->num_clock_pins[iport]; ipin++) {
      /* load intermediate indices */
      t_pb_graph_pin* pb_pin = &pb_graph_node->clock_pins[iport][ipin];

      /* alloc and load rr node info */
      LbRRNodeId node = lb_rr_graph.create_node(LB_INTERMEDIATE);
      lb_rr_graph.set_node_capacity(node, 1);
      lb_rr_graph.set_node_pb_graph_pin(node, pb_pin);

      /* TODO: Double check if this is the case */
      lb_rr_graph.set_node_intrinsic_cost(node, 1);
    }
  }

  /* alloc and load output pins that are represented as rr sources */
  for (int iport = 0; iport < pb_graph_node->num_output_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_node->num_output_pins[iport]; ipin++) {
      /* load intermediate indices */
      t_pb_graph_pin* pb_pin = &pb_graph_node->output_pins[iport][ipin];

      /* alloc and load rr node info */
      LbRRNodeId node = lb_rr_graph.create_node(output_pin_rr_type);
      lb_rr_graph.set_node_capacity(node, 1);
      lb_rr_graph.set_node_pb_graph_pin(node, pb_pin);

      /* TODO: Double check if this is the case */
      lb_rr_graph.set_node_intrinsic_cost(node, 1);
    }
  }

  if (true == is_primitive_pb_type(pb_type)) {
    return; 
  }

  /* For non-primitive node: 
   * This pb_graph_node is a logic block or subcluster 
   * Go recusrively 
   */
  t_mode* physical_mode = device_annotation.physical_mode(pb_type);
  for (int ipb_type = 0; ipb_type < physical_mode->num_pb_type_children; ipb_type++) {
    for (int ipb = 0; ipb < physical_mode->pb_type_children[ipb_type].num_pb; ipb++) {
      rec_build_physical_lb_rr_node_for_pb_graph_node(&(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb_type][ipb]), lb_rr_graph, device_annotation);
    }
  }
}

/***************************************************************************************
 * Build the edge for an input/clock pb_graph_pin for a primitive pb_graph node
 * This function will identify if the port equivalence should be considered 
 ***************************************************************************************/
static 
void build_lb_rr_edge_primitive_pb_graph_input_pin(LbRRGraph& lb_rr_graph,
                                                   t_pb_graph_pin* pb_pin,
                                                   LbRRNodeId& sink_node) {
  /* Find the node that we have already created */
  LbRRNodeId node = lb_rr_graph.find_node(LB_INTERMEDIATE, pb_pin);
  VTR_ASSERT(true == lb_rr_graph.valid_node_id(node));

  PortEquivalence port_equivalent = pb_pin->port->equivalent;

  if (port_equivalent == PortEquivalence::NONE || sink_node == LbRRNodeId::INVALID()) {
    /* Create new sink for input to primitive */
    LbRRNodeId new_sink = lb_rr_graph.create_node(LB_SINK);
    if (port_equivalent != PortEquivalence::NONE) {
      lb_rr_graph.set_node_capacity(new_sink, pb_pin->port->num_pins);
    } else {
      lb_rr_graph.set_node_capacity(new_sink, 1);
    }
    sink_node = new_sink;
  }
  
  /* Connect the nodes denoting the input pins to sink, since this is a primtive node, we do not have any mode */ 
  LbRREdgeId edge = lb_rr_graph.create_edge(node, sink_node, nullptr);

  /* TODO: Double check if this is the case */
  lb_rr_graph.set_edge_intrinsic_cost(edge, 1.); 
}

/***************************************************************************************
 * Build the edge for a pb_graph_pin for a non-primitive pb_graph node
 * Note: 
 *   - this function is NOT applicable to 
 *     - any input pin of primitive pb_graph_node 
 *     - any output pin of root pb_graph_node!
 ***************************************************************************************/
static 
void build_lb_rr_edge_pb_graph_pin(LbRRGraph& lb_rr_graph,
                                   t_pb_graph_pin* pb_pin,
                                   const e_lb_rr_type& pin_rr_type,
                                   t_mode* physical_mode) {
  /* Find the node that we have already created */
  LbRRNodeId from_node = lb_rr_graph.find_node(pin_rr_type, pb_pin);
  VTR_ASSERT(true == lb_rr_graph.valid_node_id(from_node));

  /* Load edges only for physical mode! */
  for (int iedge = 0; iedge < pb_pin->num_output_edges; iedge++) {
    VTR_ASSERT(1 == pb_pin->output_edges[iedge]->num_output_pins);
    if (physical_mode != pb_pin->output_edges[iedge]->interconnect->parent_mode) {
      continue;
    }
    /* Find the node that we have already created */
    LbRRNodeId to_node = lb_rr_graph.find_node(LB_INTERMEDIATE, pb_pin->output_edges[iedge]->output_pins[0]);
    VTR_ASSERT(true == lb_rr_graph.valid_node_id(to_node));
    LbRREdgeId edge = lb_rr_graph.create_edge(from_node, to_node, physical_mode);

    /* TODO: Double check if this is the case */
    lb_rr_graph.set_edge_intrinsic_cost(edge, 1.); 
  }
}

/***************************************************************************************
 * Build the edge for an output pb_graph_pin for a root pb_graph node
 * These node should be connected to a command external lb_rr_node
 ***************************************************************************************/
static 
void build_lb_rr_edge_root_pb_graph_pin(LbRRGraph& lb_rr_graph,
                                        t_pb_graph_pin* pb_pin,
                                        const e_lb_rr_type& pin_rr_type,
                                        const LbRRNodeId& ext_rr_index) {
  /* Find the node that we have already created */
  LbRRNodeId from_node = lb_rr_graph.find_node(pin_rr_type, pb_pin);
  VTR_ASSERT(true == lb_rr_graph.valid_node_id(from_node));

  LbRREdgeId edge = lb_rr_graph.create_edge(from_node, ext_rr_index, nullptr);

  /* TODO: Double check if this is the case */
  lb_rr_graph.set_edge_intrinsic_cost(edge, 1.); 
}

/***************************************************************************************
 * Create all the edges and special nodes (SOURCE/SINK) for lb_rr_graph for each pb_graph_node.
 * Different from the lb_rr_graph builder in VPR packer, this function only consider 
 * the pb_graph_node under physical modes
 ***************************************************************************************/
static 
void rec_build_physical_lb_rr_edge_for_pb_graph_node(t_pb_graph_node* pb_graph_node,
                                                     LbRRGraph& lb_rr_graph,
                                                     const LbRRNodeId& ext_rr_index,
                                                     const VprDeviceAnnotation& device_annotation) {
  t_pb_type* pb_type = pb_graph_node->pb_type;
  t_pb_graph_node* parent_node = pb_graph_node->parent_pb_graph_node;

  /* TODO: think if we need to consider wire mode of LUT when creating the lb_rr_graph here! 
   * Should we create edges through the LUT input and output nodes?
   */

  /* The only difference between primitive node and intermediate nodes is
   * the output pins of primitive node will be SINK node
   * Otherwise it is always INTERMEDIATE node
   */
  /* The input pins should connect to sinks */
  for (int iport = 0; iport < pb_graph_node->num_input_ports; iport++) {
    LbRRNodeId sink_node = LbRRNodeId::INVALID();
    for (int ipin = 0; ipin < pb_graph_node->num_input_pins[iport]; ipin++) {
      /* load intermediate indices */
      t_pb_graph_pin* pb_pin = &pb_graph_node->input_pins[iport][ipin];
      t_mode* physical_mode = device_annotation.physical_mode(pb_type);

      if (true == is_primitive_pb_type(pb_type)) {
        build_lb_rr_edge_primitive_pb_graph_input_pin(lb_rr_graph, pb_pin, sink_node);
      } else {
        VTR_ASSERT(false == is_primitive_pb_type(pb_type));
        build_lb_rr_edge_pb_graph_pin(lb_rr_graph, pb_pin, LB_INTERMEDIATE, physical_mode);
      }
    }
  }
  /* The input pins should connect to sinks */
  for (int iport = 0; iport < pb_graph_node->num_clock_ports; iport++) {
    LbRRNodeId sink_node = LbRRNodeId::INVALID();
    for (int ipin = 0; ipin < pb_graph_node->num_clock_pins[iport]; ipin++) {
      /* load intermediate indices */
      t_pb_graph_pin* pb_pin = &pb_graph_node->clock_pins[iport][ipin];
      t_mode* physical_mode = device_annotation.physical_mode(pb_type);

      if (true == is_primitive_pb_type(pb_type)) {
        build_lb_rr_edge_primitive_pb_graph_input_pin(lb_rr_graph, pb_pin, sink_node);
      } else {
        VTR_ASSERT(false == is_primitive_pb_type(pb_type));
        build_lb_rr_edge_pb_graph_pin(lb_rr_graph, pb_pin, LB_INTERMEDIATE, physical_mode);
      }
    }
  }

  e_lb_rr_type output_pin_rr_type = LB_INTERMEDIATE;
  if (true == is_primitive_pb_type(pb_type)) {
    output_pin_rr_type = LB_SOURCE;
  }

  /* The output pins should connect to its fan-outs */
  for (int iport = 0; iport < pb_graph_node->num_output_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_node->num_output_pins[iport]; ipin++) {
      /* load intermediate indices */
      t_pb_graph_pin* pb_pin = &pb_graph_node->output_pins[iport][ipin];

      if (true == pb_graph_node->is_root()) {
        build_lb_rr_edge_root_pb_graph_pin(lb_rr_graph, pb_pin, output_pin_rr_type, ext_rr_index);
      } else {
        VTR_ASSERT(false == pb_graph_node->is_root());
        t_mode* physical_mode = device_annotation.physical_mode(parent_node->pb_type);
        build_lb_rr_edge_pb_graph_pin(lb_rr_graph, pb_pin, output_pin_rr_type, physical_mode);
      }
    }
  }

  if (true == is_primitive_pb_type(pb_type)) {
    return; 
  }

  /* For non-primitive node: 
   * This pb_graph_node is a logic block or subcluster 
   * Go recusrively 
   */
  t_mode* physical_mode = device_annotation.physical_mode(pb_graph_node->pb_type);
  for (int ipb_type = 0; ipb_type < physical_mode->num_pb_type_children; ipb_type++) {
    for (int ipb = 0; ipb < physical_mode->pb_type_children[ipb_type].num_pb; ipb++) {
      rec_build_physical_lb_rr_edge_for_pb_graph_node(&(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb_type][ipb]), lb_rr_graph, ext_rr_index, device_annotation);
    }
  }
}

/***************************************************************************************
 * This functio will create a physical lb_rr_graph for a pb_graph considering physical modes only
 ***************************************************************************************/
static 
LbRRGraph build_lb_type_physical_lb_rr_graph(t_pb_graph_node* pb_graph_head,
                                             const VprDeviceAnnotation& device_annotation,
                                             const bool& verbose) {
  LbRRGraph lb_rr_graph;

  /* TODO: ensure we have an empty lb_rr_graph */

  /* Define the external source, sink, and external interconnect for the routing resource graph of the logic block type */
  LbRRNodeId ext_source_index = lb_rr_graph.create_node(LB_SOURCE); 
  LbRRNodeId ext_sink_index = lb_rr_graph.create_node(LB_SINK); 
  LbRRNodeId ext_rr_index = lb_rr_graph.create_node(LB_INTERMEDIATE); 

  /* Build the main body of lb rr_graph by walking through the pb_graph recursively */
  /* Build all the regular nodes first */
  rec_build_physical_lb_rr_node_for_pb_graph_node(pb_graph_head, lb_rr_graph, device_annotation);
  /* Build all the edges and special node (SOURCE/SINK) */
  rec_build_physical_lb_rr_edge_for_pb_graph_node(pb_graph_head, lb_rr_graph, ext_rr_index, device_annotation);

  /*******************************************************************************
   * Build logic block source node
   *******************************************************************************/
  t_pb_type* pb_type = pb_graph_head->pb_type;

  /* External source node drives all inputs going into logic block type */
  lb_rr_graph.set_node_capacity(ext_source_index, pb_type->num_input_pins + pb_type->num_clock_pins);

  for (int iport = 0; iport < pb_graph_head->num_input_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_head->num_input_pins[iport]; ipin++) {
      LbRRNodeId to_node = lb_rr_graph.find_node(LB_INTERMEDIATE, &(pb_graph_head->input_pins[iport][ipin]));
      VTR_ASSERT(true == lb_rr_graph.valid_node_id(to_node));
      LbRREdgeId edge = lb_rr_graph.create_edge(ext_source_index, to_node, nullptr);
      lb_rr_graph.set_edge_intrinsic_cost(edge, 1.);
    }
  }

  for (int iport = 0; iport < pb_graph_head->num_clock_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_head->num_clock_pins[iport]; ipin++) {
      LbRRNodeId to_node = lb_rr_graph.find_node(LB_INTERMEDIATE, &(pb_graph_head->clock_pins[iport][ipin]));
      VTR_ASSERT(true == lb_rr_graph.valid_node_id(to_node));
      LbRREdgeId edge = lb_rr_graph.create_edge(ext_source_index, to_node, nullptr);
      lb_rr_graph.set_edge_intrinsic_cost(edge, 1.);
    }
  }

  /*******************************************************************************
   * Build logic block sink node
   *******************************************************************************/

  /* External sink node driven by all outputs exiting logic block type */
  lb_rr_graph.set_node_capacity(ext_sink_index, pb_type->num_output_pins);

  /*******************************************************************************
   * Build node that approximates external interconnect
   *******************************************************************************/

  /* External rr node that drives all existing logic block input pins and is driven by all outputs exiting logic block type */
  lb_rr_graph.set_node_capacity(ext_rr_index, pb_type->num_output_pins);

  /* Connect opin of logic block to sink */
  {
    LbRREdgeId edge = lb_rr_graph.create_edge(ext_rr_index, ext_sink_index, nullptr);
    lb_rr_graph.set_edge_intrinsic_cost(edge, 1.);
  }

  /* Connect opin of logic block to all input and clock pins of logic block type */
  for (int iport = 0; iport < pb_graph_head->num_input_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_head->num_input_pins[iport]; ipin++) {
      LbRRNodeId to_node = lb_rr_graph.find_node(LB_INTERMEDIATE, &(pb_graph_head->input_pins[iport][ipin]));
      VTR_ASSERT(true == lb_rr_graph.valid_node_id(to_node));
      LbRREdgeId edge = lb_rr_graph.create_edge(ext_rr_index, to_node, nullptr);
      /* set cost high to avoid using external interconnect unless necessary */
      lb_rr_graph.set_edge_intrinsic_cost(edge, 1000.);
    }
  }
  for (int iport = 0; iport < pb_graph_head->num_clock_ports; iport++) {
    for (int ipin = 0; ipin < pb_graph_head->num_clock_pins[iport]; ipin++) {
      LbRRNodeId to_node = lb_rr_graph.find_node(LB_INTERMEDIATE, &(pb_graph_head->clock_pins[iport][ipin]));
      VTR_ASSERT(true == lb_rr_graph.valid_node_id(to_node));
      LbRREdgeId edge = lb_rr_graph.create_edge(ext_rr_index, to_node, nullptr);
      /* set cost high to avoid using external interconnect unless necessary */
      lb_rr_graph.set_edge_intrinsic_cost(edge, 1000.);
    }
  }

  VTR_LOGV(verbose,
           "\n\tNumber of nodes: %lu\n",
           lb_rr_graph.nodes().size());

  VTR_LOGV(verbose,
           "\tNumber of edges: %lu\n",
           lb_rr_graph.edges().size());

  return lb_rr_graph;
}

/***************************************************************************************
 * This functio will create physical lb_rr_graph for each pb_graph considering physical modes only
 * the lb_rr_graph willbe added to device annotation
 ***************************************************************************************/
void build_physical_lb_rr_graphs(const DeviceContext& device_ctx,
                                 VprDeviceAnnotation& device_annotation,
                                 const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build routing resource graph for the physical implementation of logical tile");

  for (const t_logical_block_type& lb_type : device_ctx.logical_block_types) {
    /* By pass nullptr for pb_graph head */
    if (nullptr == lb_type.pb_graph_head) {
      continue;
    }

    VTR_LOGV(verbose,
             "Building routing resource graph for logical tile '%s'...",
             lb_type.pb_graph_head->pb_type->name);

    const LbRRGraph& lb_rr_graph = build_lb_type_physical_lb_rr_graph(lb_type.pb_graph_head, const_cast<const VprDeviceAnnotation&>(device_annotation), verbose); 
    /* Check the rr_graph */
    if (false == lb_rr_graph.validate()) {
      exit(1);
    }
    VTR_LOGV(verbose, 
             "Check routing resource graph for logical tile passed\n");

    device_annotation.add_physical_lb_rr_graph(lb_type.pb_graph_head, lb_rr_graph);
  }

  VTR_LOGV(verbose, "Done\n");
}

} /* end namespace openfpga */
