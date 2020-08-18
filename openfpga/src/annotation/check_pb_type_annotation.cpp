/********************************************************************
 * This file includes functions to build links between pb_types
 * in particular to annotate the physical mode and physical pb_type
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_assert.h"
#include "vtr_log.h"

#include "pb_type_utils.h"
#include "circuit_library_utils.h"
#include "check_pb_type_annotation.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function will recursively traverse pb_type graph to ensure
 * 1. there is only a physical mode under each pb_type
 * 2. physical mode appears only when its parent is a physical mode.
 *******************************************************************/
static 
void rec_check_vpr_physical_pb_mode_annotation(t_pb_type* cur_pb_type,
                                               const bool& expect_physical_mode,
                                               const VprDeviceAnnotation& vpr_device_annotation,
                                               size_t& num_err) {
  /* We do not check any primitive pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    return;
  }

  /* For non-primitive pb_type:
   * - If we expect a physical mode to exist under this pb_type
   *   we should be able to find one in the annoation 
   * - If we do NOT expect a physical mode, make sure we find 
   *   nothing in the annotation
   */
  if (true == expect_physical_mode) {
    if (nullptr == vpr_device_annotation.physical_mode(cur_pb_type)) {
      VTR_LOG_ERROR("Unable to find a physical mode for a multi-mode pb_type '%s'!\n",
                    cur_pb_type->name);
      VTR_LOG_ERROR("Please specify in the OpenFPGA architecture\n");
      num_err++;
      return;
    }
  } else {
    VTR_ASSERT_SAFE(false == expect_physical_mode);
    if (nullptr != vpr_device_annotation.physical_mode(cur_pb_type)) {
      VTR_LOG_ERROR("Find a physical mode '%s' for pb_type '%s' which is not under any physical mode!\n",
                    vpr_device_annotation.physical_mode(cur_pb_type)->name,
                    cur_pb_type->name);
      num_err++;
      return;
    }
  }

  /* Traverse all the modes
   * - for pb_type children under a physical mode, we expect an physical mode 
   * - for pb_type children under non-physical mode, we expect no physical mode 
   */
  for (int imode = 0; imode < cur_pb_type->num_modes; ++imode) {
    bool expect_child_physical_mode = false;
    if (&(cur_pb_type->modes[imode]) == vpr_device_annotation.physical_mode(cur_pb_type)) {
      expect_child_physical_mode = true && expect_physical_mode; 
    }
    for (int ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ++ichild) { 
      rec_check_vpr_physical_pb_mode_annotation(&(cur_pb_type->modes[imode].pb_type_children[ichild]),
                                                expect_child_physical_mode, vpr_device_annotation,
                                                num_err);
    }
  }
}

/********************************************************************
 * This function will check the physical mode annotation for
 * each pb_type in the device
 *******************************************************************/
void check_vpr_physical_pb_mode_annotation(const DeviceContext& vpr_device_ctx, 
                                           const VprDeviceAnnotation& vpr_device_annotation) {
  size_t num_err = 0;

  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }
    /* Top pb_type should always has a physical mode! */
    rec_check_vpr_physical_pb_mode_annotation(lb_type.pb_type, true, vpr_device_annotation, num_err);
  }
  if (0 == num_err) {
    VTR_LOG("Check physical mode annotation for pb_types passed.\n");
  } else {
    VTR_LOG_ERROR("Check physical mode annotation for pb_types failed with %ld errors!\n",
                  num_err);
  }
}

/********************************************************************
 * This function will check 
 * - if a primitive pb_type has been mapped to a physical pb_type 
 * - if every port of the pb_type have been linked a port of a physical pb_type
 *******************************************************************/
static 
void check_vpr_physical_primitive_pb_type_annotation(t_pb_type* cur_pb_type,
                                                     const VprDeviceAnnotation& vpr_device_annotation,
                                                     size_t& num_err) {
  if (nullptr == vpr_device_annotation.physical_pb_type(cur_pb_type)) {
    VTR_LOG_ERROR("Find a pb_type '%s' which has not been mapped to any physical pb_type!\n",
                  cur_pb_type->name);
    VTR_LOG_ERROR("Please specify in the OpenFPGA architecture\n");
    num_err++;  
    return;
  }

  /* Now we need to check each port of the pb_type */ 
  for (t_port* pb_port : pb_type_ports(cur_pb_type)) {
    if (0 == vpr_device_annotation.physical_pb_port(pb_port).size()) {
      VTR_LOG_ERROR("Find a port '%s' of pb_type '%s' which has not been mapped to any physical port!\n",
                    pb_port->name, cur_pb_type->name);
      VTR_LOG_ERROR("Please specify in the OpenFPGA architecture\n");
      num_err++;
    }
  }

  return;
}

/********************************************************************
 * This function will recursively traverse pb_type graph to ensure
 * 1. there is only a physical mode under each pb_type
 * 2. physical mode appears only when its parent is a physical mode.
 *******************************************************************/
static 
void rec_check_vpr_physical_pb_type_annotation(t_pb_type* cur_pb_type,
                                               const VprDeviceAnnotation& vpr_device_annotation,
                                               size_t& num_err) {
  /* Primitive pb_type should always been binded to a physical pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    check_vpr_physical_primitive_pb_type_annotation(cur_pb_type, vpr_device_annotation, num_err);
    return;
  }

  /* Traverse all the modes */
  for (int imode = 0; imode < cur_pb_type->num_modes; ++imode) {
    for (int ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ++ichild) { 
      rec_check_vpr_physical_pb_type_annotation(&(cur_pb_type->modes[imode].pb_type_children[ichild]),
                                                vpr_device_annotation,
                                                num_err);
    }
  }
}

/********************************************************************
 * This function will check the physical pb_type annotation for
 * each pb_type in the device
 * Every pb_type should have been linked to a physical pb_type
 * and every port of the pb_type have been linked a port of a physical pb_type
 *******************************************************************/
void check_vpr_physical_pb_type_annotation(const DeviceContext& vpr_device_ctx, 
                                           const VprDeviceAnnotation& vpr_device_annotation) {
  size_t num_err = 0;

  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }
    /* Top pb_type should always has a physical mode! */
    rec_check_vpr_physical_pb_type_annotation(lb_type.pb_type, vpr_device_annotation, num_err);
  }
  if (0 == num_err) {
    VTR_LOG("Check physical pb_type annotation for pb_types passed.\n");
  } else {
    VTR_LOG_ERROR("Check physical pb_type annotation for pb_types failed with %ld errors!\n",
                  num_err);
  }
}

/********************************************************************
 * This function will recursively traverse only the physical mode
 * and physical pb_types in the graph to ensure
 *  - Every physical pb_type should be linked to a valid circuit model
 *  - Every port of the pb_type have been linked to a valid port of a circuit model
 *  - Every interconnect has been linked to a valid circuit model
 *    in a correct type
 *******************************************************************/
static 
void rec_check_vpr_pb_type_circuit_model_annotation(t_pb_type* cur_pb_type,
                                                    const CircuitLibrary& circuit_lib,
                                                    const VprDeviceAnnotation& vpr_device_annotation,
                                                    size_t& num_err) {
  /* Primitive pb_type should always been binded to a physical pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    /* Every physical pb_type should be linked to a valid circuit model */
    if (CircuitModelId::INVALID() == vpr_device_annotation.pb_type_circuit_model(cur_pb_type)) {
      VTR_LOG_ERROR("Found a physical pb_type '%s' missing circuit model binding!\n",
                    cur_pb_type->name);
      num_err++;
      return; /* Invalid id already, further check is not applicable */
    }
    /* Every port of the pb_type have been linked to a valid port of a circuit model */
    for (t_port* port : pb_type_ports(cur_pb_type)) {
      if (CircuitPortId::INVALID() == vpr_device_annotation.pb_circuit_port(port)) {
        VTR_LOG_ERROR("Found a port '%s' of physical pb_type '%s' missing circuit port binding!\n",
                      port->name, cur_pb_type->name);
        num_err++;
      }
    }
    return;
  }

  /* Every interconnect in the physical mode has been linked to a valid circuit model in a correct type */
  t_mode* physical_mode = vpr_device_annotation.physical_mode(cur_pb_type);
  for (t_interconnect* interc : pb_mode_interconnects(physical_mode)) {
    CircuitModelId interc_circuit_model = vpr_device_annotation.interconnect_circuit_model(interc);
    if (CircuitModelId::INVALID() == interc_circuit_model) {
      VTR_LOG_ERROR("Found an interconnect '%s' under physical mode '%s' of pb_type '%s' missing circuit model binding!\n",
                    interc->name,
                    physical_mode->name,
                    cur_pb_type->name);
      num_err++;
      continue;
    }
    e_circuit_model_type required_circuit_model_type = pb_interconnect_require_circuit_model_type(vpr_device_annotation.interconnect_physical_type(interc));
    if (circuit_lib.model_type(interc_circuit_model) != required_circuit_model_type) {
      VTR_LOG_ERROR("Found an interconnect '%s' under physical mode '%s' of pb_type '%s' linked to a circuit model '%s' with a wrong type!\nExpect: '%s' Linked: '%s'\n",
                    interc->name,
                    physical_mode->name, 
                    cur_pb_type->name,
                    circuit_lib.model_name(interc_circuit_model).c_str(),
                    CIRCUIT_MODEL_TYPE_STRING[circuit_lib.model_type(interc_circuit_model)],
                    CIRCUIT_MODEL_TYPE_STRING[required_circuit_model_type]);
      num_err++;
    }
  } 

  /* Traverse only the physical mode */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) { 
    rec_check_vpr_pb_type_circuit_model_annotation(&(physical_mode->pb_type_children[ichild]),
                                                   circuit_lib,
                                                   vpr_device_annotation,
                                                   num_err);
  }
}

/********************************************************************
 * This function will check the circuit model annotation for
 * each physical pb_type in the device
 *  - Every physical pb_type should be linked to a valid circuit model
 *  - Every port of the pb_type have been linked a valid port of a circuit model
 *  - Every interconnect has been linked to a valid circuit model
 *    in a correct type
 *******************************************************************/
void check_vpr_pb_type_circuit_model_annotation(const DeviceContext& vpr_device_ctx, 
                                                const CircuitLibrary& circuit_lib,
                                                const VprDeviceAnnotation& vpr_device_annotation) {
  size_t num_err = 0;

  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }
    /* Top pb_type should always has a physical mode! */
    rec_check_vpr_pb_type_circuit_model_annotation(lb_type.pb_type, circuit_lib, vpr_device_annotation, num_err);
  }
  if (0 == num_err) {
    VTR_LOG("Check physical pb_type annotation for circuit model passed.\n");
  } else {
    VTR_LOG_ERROR("Check physical pb_type annotation for circuit model failed with %ld errors!\n",
                  num_err);
  }
}

/********************************************************************
 * This function will recursively traverse all the primitive pb_types 
 * in the graph to ensure
 *  - If a primitive pb_type has mode bits, it must have been linked to a physical pb_type
 *    and the circuit model must have a port for mode selection. 
 *    And the port size must match the length of mode bits
 *******************************************************************/
static 
void rec_check_vpr_pb_type_mode_bits_annotation(t_pb_type* cur_pb_type,
                                                const CircuitLibrary& circuit_lib,
                                                const VprDeviceAnnotation& vpr_device_annotation,
                                                size_t& num_err) {
  /* Primitive pb_type should always been binded to a physical pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    /* Find the physical pb_type
     * If the physical pb_type has mode selection bits, this pb_type must have as well!
     */
    t_pb_type* physical_pb_type = vpr_device_annotation.physical_pb_type(cur_pb_type);

    if (nullptr == physical_pb_type) {
      VTR_LOG_ERROR("Find a pb_type '%s' which has not been mapped to any physical pb_type!\n",
                    cur_pb_type->name);
      VTR_LOG_ERROR("Please specify in the OpenFPGA architecture\n");
      num_err++;
      return;
    }

    if (vpr_device_annotation.pb_type_mode_bits(cur_pb_type).size() != vpr_device_annotation.pb_type_mode_bits(physical_pb_type).size()) {
      VTR_LOG_ERROR("Found different sizes of mode_bits for pb_type '%s' and its physical pb_type '%s'\n",
                    cur_pb_type->name,
                    physical_pb_type->name);
      num_err++;
      return;
    }
  
    /* Try to find a mode selection port for the circuit model linked to the circuit model */
    CircuitModelId circuit_model = vpr_device_annotation.pb_type_circuit_model(physical_pb_type);
    if (CircuitModelId::INVALID() == vpr_device_annotation.pb_type_circuit_model(physical_pb_type)) {
      VTR_LOG_ERROR("Found a physical pb_type '%s' missing circuit model binding!\n",
                    physical_pb_type->name);
      num_err++;
      return; /* Invalid id already, further check is not applicable */
    }
    
    if (0 == vpr_device_annotation.pb_type_mode_bits(cur_pb_type).size()) {
      /* No mode bits to be checked! */
      return;
    }
    /* Search the ports of this circuit model and we must have a mode selection port */
    std::vector<CircuitPortId> mode_select_ports = find_circuit_mode_select_sram_ports(circuit_lib, circuit_model);
    size_t port_num_mode_bits = 0;
    for (const CircuitPortId& mode_select_port : mode_select_ports) {
      port_num_mode_bits += circuit_lib.port_size(mode_select_port);
    }
    if (port_num_mode_bits != vpr_device_annotation.pb_type_mode_bits(cur_pb_type).size()) {
      VTR_LOG_ERROR("Length of mode bits of pb_type '%s' does not match the size(%ld) of mode selection ports of circuit model '%s'!\n",
                    cur_pb_type->name,
                    port_num_mode_bits,
                    circuit_lib.model_name(circuit_model).c_str());
      num_err++;
    }

    return;
  }

  /* Traverse all the modes */
  for (int imode = 0; imode < cur_pb_type->num_modes; ++imode) {
    for (int ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ++ichild) { 
      rec_check_vpr_pb_type_mode_bits_annotation(&(cur_pb_type->modes[imode].pb_type_children[ichild]),
                                                 circuit_lib, vpr_device_annotation,
                                                 num_err);
    }
  }
}

/********************************************************************
 * This function will check the mode_bits annotation for each pb_type 
 *  - If a primitive pb_type has mode bits, it must have been linked to a physical pb_type
 *  - If a primitive pb_type has mode bits, the circuit model must have 
 *    a port for mode selection. And the port size must match the length of mode bits
 *
 * Note:
 *  - This function should be run after circuit mode and mode bits annotation 
 *    is completed 
 *******************************************************************/
void check_vpr_pb_type_mode_bits_annotation(const DeviceContext& vpr_device_ctx, 
                                            const CircuitLibrary& circuit_lib,
                                            const VprDeviceAnnotation& vpr_device_annotation) {
  size_t num_err = 0;

  for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }
    /* Top pb_type should always has a physical mode! */
    rec_check_vpr_pb_type_mode_bits_annotation(lb_type.pb_type, circuit_lib, vpr_device_annotation, num_err);
  }
  if (0 == num_err) {
    VTR_LOG("Check pb_type annotation for mode selection bits passed.\n");
  } else {
    VTR_LOG_ERROR("Check physical pb_type annotation for mode selection bits failed with %ld errors!\n",
                  num_err);
  }
}

} /* end namespace openfpga */
