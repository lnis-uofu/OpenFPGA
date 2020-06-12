/********************************************************************
 * This file includes functions to check the  annotation on
 * the physical pb_graph_node and pb_graph_pin
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_assert.h"
#include "vtr_log.h"

#include "pb_type_utils.h"
#include "check_pb_graph_annotation.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A function to check if a physical pb_graph_pin for a pb_graph_pin 
 * Print error to log file/screen when checking fails
 *******************************************************************/
static 
bool check_physical_pb_graph_pin(t_pb_graph_pin* pb_graph_pin,
                                 const VprDeviceAnnotation& vpr_device_annotation) {
  if (nullptr == vpr_device_annotation.physical_pb_graph_pin(pb_graph_pin)) {
    VTR_LOG_ERROR("Found a pb_graph_pin '%s' missing physical pb_graph_pin binding!\n",
                  pb_graph_pin->port->name);
    return false;
  }

  return true;
}

/********************************************************************
 * This function will recursively walk through all the pb_graph nodes
 * starting from a top node.
 * It aims to check 
 * - if each primitive pb_graph node has been binded to a physical 
 *   pb_graph_node
 * - if each pin of the primitive pb_graph node has been binded to 
 *   a physical graph_pin
 *******************************************************************/
static 
void rec_check_vpr_physical_pb_graph_node_annotation(t_pb_graph_node* pb_graph_node, 
                                                     const VprDeviceAnnotation& vpr_device_annotation,
                                                     size_t& num_err) {
  /* Go recursive first until we touch the primitive node */
  if (false == is_primitive_pb_type(pb_graph_node->pb_type)) {
    for (int imode = 0; imode < pb_graph_node->pb_type->num_modes; ++imode) {
      for (int ipb = 0; ipb < pb_graph_node->pb_type->modes[imode].num_pb_type_children; ++ipb) {
        /* Each child may exist multiple times in the hierarchy*/
        for (int jpb = 0; jpb < pb_graph_node->pb_type->modes[imode].pb_type_children[ipb].num_pb; ++jpb) {
          rec_check_vpr_physical_pb_graph_node_annotation(&(pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]), 
                                                          vpr_device_annotation, num_err);
        }
      }
    }
    return;
  }

  /* Ensure that the pb_graph_node has been mapped to a physical node */
  t_pb_graph_node* physical_pb_graph_node = vpr_device_annotation.physical_pb_graph_node(pb_graph_node);
  if (nullptr == physical_pb_graph_node) { 
    VTR_LOG_ERROR("Found a pb_graph_node '%s' missing physical pb_graph_node binding!\n",
                  physical_pb_graph_node->pb_type->name);
    num_err++;
    return; /* Invalid pointer already, further check is not applicable */
  } 

  /* Reach here, we should have a valid pointer to the physical pb_graph_node,
   * Check the physical pb_graph_pin for each pin 
   */
  for (int iport = 0; iport < physical_pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_input_pins[iport]; ++ipin) {
      if (false == check_physical_pb_graph_pin(&(physical_pb_graph_node->input_pins[iport][ipin]),
                                               vpr_device_annotation)) {
        num_err++;
      }
    } 
  }

  for (int iport = 0; iport < physical_pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_output_pins[iport]; ++ipin) {
      if (false == check_physical_pb_graph_pin(&(physical_pb_graph_node->output_pins[iport][ipin]),
                                               vpr_device_annotation)) {
        num_err++;
      }
    } 
  }

  for (int iport = 0; iport < physical_pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_clock_pins[iport]; ++ipin) {
      if (false == check_physical_pb_graph_pin(&(physical_pb_graph_node->clock_pins[iport][ipin]),
                                               vpr_device_annotation)) {
        num_err++;
      }
    } 
  }
}

/********************************************************************
 * Check each primitive pb_graph_node
 * - It has been binded to an physical pb_graph_node
 * - Each pin has been binded to a physical pb_graph_node pin
 *******************************************************************/
void check_physical_pb_graph_node_annotation(const DeviceContext& vpr_device_ctx, 
                                             const VprDeviceAnnotation& vpr_device_annotation) {
  size_t num_err = 0;
 
  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_graph head */
    if (nullptr == lb_type.pb_graph_head) {
      continue;
    }
    rec_check_vpr_physical_pb_graph_node_annotation(lb_type.pb_graph_head, vpr_device_annotation, num_err); 
  }

  if (0 == num_err) {
    VTR_LOG("Check pb_graph annotation for physical nodes and pins passed.\n");
  } else {
    VTR_LOG_ERROR("Check pb_graph annotation for physical nodes and pins failed with %ld errors!\n",
                  num_err);
  }
}

} /* end namespace openfpga */
