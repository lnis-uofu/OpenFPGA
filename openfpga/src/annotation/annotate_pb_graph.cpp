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

} /* end namespace openfpga */

