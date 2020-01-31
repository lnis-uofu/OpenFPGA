/********************************************************************
 * This file includes functions that are used to annotate pb_graph_node
 * and pb_graph_pins from VPR to OpenFPGA
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "pb_type_utils.h"
#include "pb_graph_utils.h"

#include "annotate_pb_graph.h"
#include "check_pb_graph_annotation.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function will recursively walk through all the pb_graph nodes
 * starting from a top node.
 * It aims to annotate the physical type of each interconnect.
 * This is due to that the type of interconnect 'complete' may diverge
 * in physical implmentation. 
 *  - When there is only one input driving a 'complete' interconnection, 
 *    it will be implemented with wires
 *  - When there are multiple inputs driving a 'complete' interconnection, 
 *    it will be implemented with routing multiplexers
 *******************************************************************/
static 
void rec_build_vpr_pb_graph_interconnect_physical_type_annotation(t_pb_graph_node* pb_graph_node, 
                                                                  VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* Skip the root node because we start from the inputs of child pb_graph node 
   *  
   *   pb_graph_node
   *   +----------------------------------
   *   |               child_pb_graph_node
   *   |             +-----------------
   *   |             |
   *   |-------------+-->input_pins
   *   |             |
   *   |-------------+-->clock_pins
   *   |
   *
   */
  if (false == pb_graph_node->is_root()) {
    /* We only care the physical modes! But we have to find it through the parent node */
    t_mode* child_physical_mode = vpr_pb_type_annotation.physical_mode(pb_graph_node->parent_pb_graph_node->pb_type);
    VTR_ASSERT(nullptr != child_physical_mode);

    std::map<t_interconnect*, size_t> interc_num_inputs;
    /* Initialize the counter */
    for (t_interconnect* interc : pb_mode_interconnects(child_physical_mode)) {
      interc_num_inputs[interc] = 0;
    }

    /* We only care input and clock pins */
    for (int iport = 0; iport < pb_graph_node->num_input_ports; ++iport) {
      for (int ipin = 0; ipin < pb_graph_node->num_input_pins[iport]; ++ipin) {
        /* For each interconnect, we count the total number of inputs */
        for (t_interconnect* interc : pb_mode_interconnects(child_physical_mode)) {
          interc_num_inputs[interc] += pb_graph_pin_inputs(&(pb_graph_node->input_pins[iport][ipin]), interc).size(); 
        }
      }
    }

    for (int iport = 0; iport < pb_graph_node->num_clock_ports; ++iport) {
      for (int ipin = 0; ipin < pb_graph_node->num_clock_pins[iport]; ++ipin) {
        /* For each interconnect, we count the total number of inputs */
        for (t_interconnect* interc : pb_mode_interconnects(child_physical_mode)) {
          interc_num_inputs[interc] += pb_graph_pin_inputs(&(pb_graph_node->clock_pins[iport][ipin]), interc).size(); 
        }
      }
    }

    /* For each interconnect that has more than 1 input, we can infer the physical type */
    for (t_interconnect* interc : pb_mode_interconnects(child_physical_mode)) {
      /* If the number inputs for an interconnect is zero, this is a 0-driver pin
       * we just set 1 to use direct wires
       */
      if (0 == interc_num_inputs[interc]) {
        interc_num_inputs[interc] = 1;
      }
      
      e_interconnect interc_physical_type = pb_interconnect_physical_type(interc, interc_num_inputs[interc]);
      if (interc_physical_type == vpr_pb_type_annotation.interconnect_physical_type(interc)) {
        /* Skip annotation if we have already done! */
        continue;
      }
      vpr_pb_type_annotation.add_interconnect_physical_type(interc, interc_physical_type);
    }
  }

  /* If we reach a primitive pb_graph node, we return */
  if (true == is_primitive_pb_type(pb_graph_node->pb_type)) {
    return;
  }

  /* Recursively visit all the child pb_graph_nodes */
  t_mode* physical_mode = vpr_pb_type_annotation.physical_mode(pb_graph_node->pb_type);
  VTR_ASSERT(nullptr != physical_mode);
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    /* Each child may exist multiple times in the hierarchy*/
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
      rec_build_vpr_pb_graph_interconnect_physical_type_annotation(&(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]), vpr_pb_type_annotation);
    }
  }
}

/********************************************************************
 * This function aims to annotate the physical type for each interconnect
 * inside the pb_graph 
 *
 * Note:
 *   - This function should be executed AFTER functions
 *       build_vpr_physical_pb_mode_explicit_annotation() 
 *       build_vpr_physical_pb_mode_implicit_annotation() 
 *******************************************************************/
void annotate_pb_graph_interconnect_physical_type(const DeviceContext& vpr_device_ctx, 
                                                  VprPbTypeAnnotation& vpr_pb_type_annotation) {
  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_graph head */
    if (nullptr == lb_type.pb_graph_head) {
      continue;
    }
    rec_build_vpr_pb_graph_interconnect_physical_type_annotation(lb_type.pb_graph_head, vpr_pb_type_annotation); 
  }
}

/********************************************************************
 * This function will recursively walk through all the pb_graph nodes
 * starting from a top node.
 * It aims to give an unique index to each pb_graph node 
 *
 * Therefore, the sequence in visiting the nodes is critical
 * Here, we will follow the strategy where primitive nodes are visited first 
 *******************************************************************/
static 
void rec_build_vpr_primitive_pb_graph_node_unique_index(t_pb_graph_node* pb_graph_node, 
                                                        VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* Go recursive first until we touch the primitive node */
  if (false == is_primitive_pb_type(pb_graph_node->pb_type)) {
    for (int imode = 0; imode < pb_graph_node->pb_type->num_modes; ++imode) {
      for (int ipb = 0; ipb < pb_graph_node->pb_type->modes[imode].num_pb_type_children; ++ipb) {
        /* Each child may exist multiple times in the hierarchy*/
        for (int jpb = 0; jpb < pb_graph_node->pb_type->modes[imode].pb_type_children[ipb].num_pb; ++jpb) {
          rec_build_vpr_primitive_pb_graph_node_unique_index(&(pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]), 
                                                             vpr_pb_type_annotation);
        }
      }
    }
    return;
  }

  /* Give a unique index to the pb_graph_node */
  vpr_pb_type_annotation.add_pb_graph_node_unique_index(pb_graph_node);
}

/********************************************************************
 * This function aims to assign an unique index to each 
 * primitive pb_graph_node by following a recursive way in walking
 * through the pb_graph
 *
 * Note: 
 * - The unique index is different from the placement_index in VPR's 
 *   pb_graph_node data structure. The placement index is only unique
 *   for a node under its parent node. If the parent node is duplicated 
 *   across the graph, the placement index is not unique.
 *   For example, a CLB contains 10 LEs and each of LE contains 2 LUTs
 *   Inside each LE, the placement index of the LUTs are 0 and 1 respectively.
 *   But these indices are not unique in the graph, as there are 20 LUTs in total
 *******************************************************************/
static 
void annotate_primitive_pb_graph_node_unique_index(const DeviceContext& vpr_device_ctx, 
                                                   VprPbTypeAnnotation& vpr_pb_type_annotation) {
  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_graph head */
    if (nullptr == lb_type.pb_graph_head) {
      continue;
    }
    rec_build_vpr_primitive_pb_graph_node_unique_index(lb_type.pb_graph_head, vpr_pb_type_annotation); 
  }
}

/********************************************************************
 * Evaluate if the two pb_graph pins are matched by
 *  - pb_type port annotation 
 *  - LSB/MSB and pin offset
 *******************************************************************/
static 
bool try_match_pb_graph_pin(t_pb_graph_pin* operating_pb_graph_pin, 
                            t_pb_graph_pin* physical_pb_graph_pin,
                            const VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* If the parent ports of the two pins are not paired, fail */
  if (physical_pb_graph_pin->port != vpr_pb_type_annotation.physical_pb_port(operating_pb_graph_pin->port)) {
    return false;
  }
  /* Check the pin number of physical pb_graph_pin matches the pin number of 
   * operating pb_graph_pin plus a rotation offset 
   *                                              operating port         physical port
   *                      LSB  port_range.lsb()    pin_number              pin_number      MSB
   *                                 |                  |                     |
   *    Operating port     |         |                  +------               |
   *                                 |                        |<----offset--->|
   *    Physical port      |         +                        +               +
   *
   * Note: 
   *   - accumulated offset is NOT the pin rotate offset specified by users
   *     It is an aggregation of the offset during pin pairing
   *     Each time, we manage to pair two pins, the accumulated offset will be incremented
   *     by the pin rotate offset value
   *     The accumulated offset will be reset to 0 when it exceeds the msb() of the physical port
   */
  int acc_offset = vpr_pb_type_annotation.physical_pb_pin_offset(operating_pb_graph_pin->port);
  const BasicPort& physical_port_range = vpr_pb_type_annotation.physical_pb_port_range(operating_pb_graph_pin->port);
  if (physical_pb_graph_pin->pin_number != operating_pb_graph_pin->pin_number
                                         + (int)physical_port_range.get_lsb() 
                                         + acc_offset) {
    return false;
  }

  /* Reach here, it means all the requirements have been met */
  return true;
}

/********************************************************************
 * Bind a pb_graph_pin from an operating pb_graph_node to 
 * a pb_graph_pin from a physical pb_graph_node
 * - the name matching rules are already defined in the vpr_pb_type_annotation
 *******************************************************************/
static 
void annotate_physical_pb_graph_pin(t_pb_graph_pin* operating_pb_graph_pin, 
                                    t_pb_graph_node* physical_pb_graph_node, 
                                    VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* Iterate over every port and pin of the operating pb_graph_node 
   * and find the physical pins 
   */
  for (int iport = 0; iport < physical_pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_input_pins[iport]; ++ipin) {
      if (false == try_match_pb_graph_pin(operating_pb_graph_pin, 
                                          &(physical_pb_graph_node->input_pins[iport][ipin]),
                                          vpr_pb_type_annotation)) {
        continue;
      }
      /* Reach here, it means the pins are matched by the annotation requirements 
       * We can pair the pin and return  
       */
      vpr_pb_type_annotation.add_physical_pb_graph_pin(operating_pb_graph_pin, &(physical_pb_graph_node->input_pins[iport][ipin]));
      VTR_LOG("Bind a pb_graph_node '%s[%d]' pin '%s[%d]' to a pb_graph_node '%s[%d]' pin '%s[%d]'!\n",
              operating_pb_graph_pin->parent_node->pb_type->name,
              operating_pb_graph_pin->parent_node->placement_index,
              operating_pb_graph_pin->port->name,
              operating_pb_graph_pin->pin_number,
              physical_pb_graph_node->pb_type->name,
              physical_pb_graph_node->placement_index,
              physical_pb_graph_node->input_pins[iport][ipin].port->name,
              physical_pb_graph_node->input_pins[iport][ipin].pin_number);
      return;
    }
  }

  for (int iport = 0; iport < physical_pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_output_pins[iport]; ++ipin) {
      if (false == try_match_pb_graph_pin(operating_pb_graph_pin, 
                                          &(physical_pb_graph_node->output_pins[iport][ipin]),
                                          vpr_pb_type_annotation)) {
        continue;
      }
      /* Reach here, it means the pins are matched by the annotation requirements 
       * We can pair the pin and return  
       */
      vpr_pb_type_annotation.add_physical_pb_graph_pin(operating_pb_graph_pin, &(physical_pb_graph_node->output_pins[iport][ipin]));
      VTR_LOG("Bind a pb_graph_node '%s[%d]' pin '%s[%d]' to a pb_graph_node '%s[%d]' pin '%s[%d]'!\n",
              operating_pb_graph_pin->parent_node->pb_type->name,
              operating_pb_graph_pin->parent_node->placement_index,
              operating_pb_graph_pin->port->name,
              operating_pb_graph_pin->pin_number,
              physical_pb_graph_node->pb_type->name,
              physical_pb_graph_node->placement_index,
              physical_pb_graph_node->output_pins[iport][ipin].port->name,
              physical_pb_graph_node->output_pins[iport][ipin].pin_number);
      return;
    }
  }

  for (int iport = 0; iport < physical_pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_clock_pins[iport]; ++ipin) {
      if (false == try_match_pb_graph_pin(operating_pb_graph_pin, 
                                          &(physical_pb_graph_node->clock_pins[iport][ipin]),
                                          vpr_pb_type_annotation)) {
        continue;
      }
      /* Reach here, it means the pins are matched by the annotation requirements 
       * We can pair the pin and return  
       */
      vpr_pb_type_annotation.add_physical_pb_graph_pin(operating_pb_graph_pin, &(physical_pb_graph_node->clock_pins[iport][ipin]));
      VTR_LOG("Bind a pb_graph_node '%s[%d]' pin '%s[%d]' to a pb_graph_node '%s[%d]' pin '%s[%d]'!\n",
              operating_pb_graph_pin->parent_node->pb_type->name,
              operating_pb_graph_pin->parent_node->placement_index,
              operating_pb_graph_pin->port->name,
              operating_pb_graph_pin->pin_number,
              physical_pb_graph_node->pb_type->name,
              physical_pb_graph_node->placement_index,
              physical_pb_graph_node->clock_pins[iport][ipin].port->name,
              physical_pb_graph_node->clock_pins[iport][ipin].pin_number);
      return;
    }
  }

  /* If we reach here, it means that pin pairing fails, error out! */
  VTR_LOG_ERROR("Fail to match a physical pin for '%s' from pb_graph_node '%s'!\n",
                operating_pb_graph_pin->port->name,
                physical_pb_graph_node->hierarchical_type_name().c_str());
}

/********************************************************************
 * This function will try bind each pin of the operating pb_graph_node
 * to a pin of the physical pb_graph_node by following the annotation
 * available in vpr_pb_type_annotation
 * It will add the pin bindings to the vpr_pb_type_annotation
 *******************************************************************/
static 
void annotate_physical_pb_graph_node_pins(t_pb_graph_node* operating_pb_graph_node, 
                                          t_pb_graph_node* physical_pb_graph_node, 
                                          VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* Iterate over every port and pin of the operating pb_graph_node 
   * and find the physical pins 
   */
  for (int iport = 0; iport < operating_pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < operating_pb_graph_node->num_input_pins[iport]; ++ipin) {
      annotate_physical_pb_graph_pin(&(operating_pb_graph_node->input_pins[iport][ipin]),
                                     physical_pb_graph_node, vpr_pb_type_annotation);
    }
  }

  for (int iport = 0; iport < operating_pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < operating_pb_graph_node->num_output_pins[iport]; ++ipin) {
      annotate_physical_pb_graph_pin(&(operating_pb_graph_node->output_pins[iport][ipin]),
                                     physical_pb_graph_node, vpr_pb_type_annotation);
    }
  }

  for (int iport = 0; iport < operating_pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < operating_pb_graph_node->num_clock_pins[iport]; ++ipin) {
      annotate_physical_pb_graph_pin(&(operating_pb_graph_node->clock_pins[iport][ipin]),
                                     physical_pb_graph_node, vpr_pb_type_annotation);
    }
  }
}

/********************************************************************
 * This function will recursively walk through all the pb_graph nodes
 * starting from a top node.
 * It aims to give an unique index to each pb_graph node 
 *
 * Therefore, the sequence in visiting the nodes is critical
 * Here, we will follow the strategy where primitive nodes are visited first 
 *******************************************************************/
static 
void rec_build_vpr_physical_pb_graph_node_annotation(t_pb_graph_node* pb_graph_node, 
                                                     VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* Go recursive first until we touch the primitive node */
  if (false == is_primitive_pb_type(pb_graph_node->pb_type)) {
    for (int imode = 0; imode < pb_graph_node->pb_type->num_modes; ++imode) {
      for (int ipb = 0; ipb < pb_graph_node->pb_type->modes[imode].num_pb_type_children; ++ipb) {
        /* Each child may exist multiple times in the hierarchy*/
        for (int jpb = 0; jpb < pb_graph_node->pb_type->modes[imode].pb_type_children[ipb].num_pb; ++jpb) {
          rec_build_vpr_physical_pb_graph_node_annotation(&(pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]), 
                                                          vpr_pb_type_annotation);
        }
      }
    }
    return;
  }

  /* To bind operating pb_graph_node to their physical pb_graph_node:
   *  - Get the physical pb_type that this type of pb_graph_node should be mapped to 
   *  - Calculate the unique index of physical pb_graph_node to which
   *    this pb_graph_node should be binded
   *  - Find the physical pb_graph_node with the given index 
   * To bind pins from operating pb_graph_node to their physical pb_graph_node pins
   */
  t_pb_type* physical_pb_type = vpr_pb_type_annotation.physical_pb_type(pb_graph_node->pb_type); 
  VTR_ASSERT(nullptr != physical_pb_type);

  /* Index inference:
   * physical_pb_graph_node_unique_index = operating_pb_graph_node_unique_index * factor + offset
   * where factor and offset are provided by users
   */
  PbGraphNodeId physical_pb_graph_node_id = PbGraphNodeId(
                                            vpr_pb_type_annotation.physical_pb_type_index_factor(pb_graph_node->pb_type)
                                          * (size_t)vpr_pb_type_annotation.pb_graph_node_unique_index(pb_graph_node)
                                          + vpr_pb_type_annotation.physical_pb_type_index_offset(pb_graph_node->pb_type)
                                            );
  t_pb_graph_node* physical_pb_graph_node = vpr_pb_type_annotation.pb_graph_node(physical_pb_type, physical_pb_graph_node_id);
  VTR_ASSERT(nullptr != physical_pb_graph_node);
  vpr_pb_type_annotation.add_physical_pb_graph_node(pb_graph_node, physical_pb_graph_node);

  VTR_LOG("Bind operating pb_graph_node '%s' to physical pb_graph_node '%s'\n",
          pb_graph_node->hierarchical_type_name().c_str(),
          physical_pb_graph_node->hierarchical_type_name().c_str());

  /* Try to bind each pins under this pb_graph_node to physical_pb_graph_node */
  annotate_physical_pb_graph_node_pins(pb_graph_node, physical_pb_graph_node, vpr_pb_type_annotation);
}

/********************************************************************
 * Find the physical pb_graph_node for  each primitive pb_graph_node
 * - Bind operating pb_graph_node to their physical pb_graph_node
 * - Bind pins from operating pb_graph_node to their physical pb_graph_node pins
 *******************************************************************/
static 
void annotate_physical_pb_graph_node(const DeviceContext& vpr_device_ctx, 
                                     VprPbTypeAnnotation& vpr_pb_type_annotation) {
  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_graph head */
    if (nullptr == lb_type.pb_graph_head) {
      continue;
    }
    rec_build_vpr_physical_pb_graph_node_annotation(lb_type.pb_graph_head, vpr_pb_type_annotation); 
  }
}

/********************************************************************
 * Top-level function to annotate all the pb_graph nodes and pins
 * - Give unique index to each primitive node in the same type
 * - Bind operating pb_graph_node to their physical pb_graph_node
 * - Bind pins from operating pb_graph_node to their physical pb_graph_node pins
 *******************************************************************/
void annotate_pb_graph(const DeviceContext& vpr_device_ctx, 
                       VprPbTypeAnnotation& vpr_pb_type_annotation) {

  VTR_LOG("Assigning unique indices for primitive pb_graph nodes...");
  annotate_primitive_pb_graph_node_unique_index(vpr_device_ctx, vpr_pb_type_annotation);
  VTR_LOG("Done\n");

  VTR_LOG("Binding operating pb_graph nodes/pins to physical pb_graph nodes/pins...\n");
  annotate_physical_pb_graph_node(vpr_device_ctx, vpr_pb_type_annotation);
  VTR_LOG("Done\n");

  /* Check each primitive pb_graph_node and pin has been binded to a physical node and pin */
  check_physical_pb_graph_node_annotation(vpr_device_ctx, const_cast<const VprPbTypeAnnotation&>(vpr_pb_type_annotation));
}

} /* end namespace openfpga */

