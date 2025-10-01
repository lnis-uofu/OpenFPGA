/********************************************************************
 * This file includes functions to build links between pb_types
 * in particular to annotate the physical mode and physical pb_type
 *******************************************************************/
/* Headers from vtrutil library */
#include "annotate_pb_types.h"

#include "annotate_pb_graph.h"
#include "check_pb_type_annotation.h"
#include "pb_type_utils.h"
#include "vpr_device_annotation.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function will identify the physical mode for each multi-mode
 * pb_type in VPR pb_type graph by following the explicit definition
 * in OpenFPGA architecture XML
 *******************************************************************/
static void build_vpr_physical_pb_mode_explicit_annotation(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* Walk through the pb_type annotation stored in the openfpga arch */
  for (const PbTypeAnnotation& pb_type_annotation :
       openfpga_arch.pb_type_annotations) {
    /* Since our target is to annotate the physical mode name,
     * we can skip those has not physical mode defined
     */
    if (true == pb_type_annotation.physical_mode_name().empty()) {
      continue;
    }

    /* Identify if the pb_type is operating or physical,
     * For operating pb_type, get the full name of operating pb_type
     * For physical pb_type, get the full name of physical pb_type
     */
    std::vector<std::string> target_pb_type_names;
    std::vector<std::string> target_pb_mode_names;

    if (true == pb_type_annotation.is_operating_pb_type()) {
      target_pb_type_names =
        pb_type_annotation.operating_parent_pb_type_names();
      target_pb_type_names.push_back(
        pb_type_annotation.operating_pb_type_name());
      target_pb_mode_names = pb_type_annotation.operating_parent_mode_names();
    }

    if (true == pb_type_annotation.is_physical_pb_type()) {
      target_pb_type_names = pb_type_annotation.physical_parent_pb_type_names();
      target_pb_type_names.push_back(
        pb_type_annotation.physical_pb_type_name());
      target_pb_mode_names = pb_type_annotation.physical_parent_mode_names();
    }

    /* We must have at least one pb_type in the list */
    VTR_ASSERT_SAFE(0 < target_pb_type_names.size());

    /* Pb type information are located at the logic_block_types in the device
     * context of VPR We iterate over the vectors and find the pb_type matches
     * the parent_pb_type_name
     */
    bool link_success = false;

    for (const t_logical_block_type& lb_type :
         vpr_device_ctx.logical_block_types) {
      /* By pass nullptr for pb_type head */
      if (nullptr == lb_type.pb_type) {
        continue;
      }
      /* Check the name of the top-level pb_type, if it does not match, we can
       * bypass */
      if (target_pb_type_names[0] != std::string(lb_type.pb_type->name)) {
        continue;
      }
      /* Match the name in the top-level, we go further to search the pb_type in
       * the graph */
      t_pb_type* target_pb_type = try_find_pb_type_with_given_path(
        lb_type.pb_type, target_pb_type_names, target_pb_mode_names);
      if (nullptr == target_pb_type) {
        continue;
      }

      /* Found, we update the annotation by assigning the physical mode */
      t_mode* physical_mode = find_pb_type_mode(
        target_pb_type, pb_type_annotation.physical_mode_name().c_str());
      vpr_device_annotation.add_pb_type_physical_mode(target_pb_type,
                                                      physical_mode);

      /* Give a message */
      VTR_LOGV(verbose_output,
               "Annotate pb_type '%s' with physical mode '%s'\n",
               target_pb_type->name, physical_mode->name);

      link_success = true;
      break;
    }

    if (false == link_success) {
      /* Not found, error out! */
      VTR_LOG_ERROR(
        "Unable to find the pb_type '%s' in VPR architecture definition!\n",
        target_pb_type_names.back().c_str());
      return;
    }
  }
}

/********************************************************************
 * This function will recursively visit all the pb_type from the top
 * pb_type in the graph and
 * infer the physical mode for each multi-mode
 * pb_type in VPR pb_type graph without OpenFPGA architecture XML
 *
 * The following rule is applied:
 * if there is only 1 mode under a pb_type, it will be the default
 * physical mode for this pb_type
 *******************************************************************/
static void rec_infer_vpr_physical_pb_mode_annotation(
  t_pb_type* cur_pb_type, VprDeviceAnnotation& vpr_device_annotation,
  const bool& verbose_output) {
  /* We do not check any primitive pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    return;
  }

  /* For non-primitive pb_type:
   * - if there is only one mode, it will be the physical mode
   *   we just need to make sure that we do not repeatedly annotate this
   * - if there are multiple modes, we should be able to find a physical mode
   *   and then go recursively
   */
  t_mode* physical_mode = nullptr;

  if (1 == cur_pb_type->num_modes) {
    if (nullptr == vpr_device_annotation.physical_mode(cur_pb_type)) {
      /* Not assigned by explicit annotation, we should infer here */
      vpr_device_annotation.add_pb_type_physical_mode(cur_pb_type,
                                                      &(cur_pb_type->modes[0]));
      VTR_LOGV(verbose_output,
               "Implicitly infer physical mode '%s' for pb_type '%s'\n",
               cur_pb_type->modes[0].name, cur_pb_type->name);
    }
  } else {
    VTR_ASSERT(1 < cur_pb_type->num_modes);
    if (nullptr == vpr_device_annotation.physical_mode(cur_pb_type)) {
      /* Not assigned by explicit annotation, we should infer here */
      vpr_device_annotation.add_pb_type_physical_mode(cur_pb_type,
                                                      &(cur_pb_type->modes[0]));
      VTR_LOG_ERROR(
        "Unable to find a physical mode for a multi-mode pb_type '%s'!\n",
        cur_pb_type->name);
      VTR_LOG_ERROR("Please specify in the OpenFPGA architecture\n");
      return;
    }
  }

  /* Get the physical mode from annotation */
  physical_mode = vpr_device_annotation.physical_mode(cur_pb_type);

  VTR_ASSERT(nullptr != physical_mode);

  /* Traverse the pb_type children under the physical mode */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    rec_infer_vpr_physical_pb_mode_annotation(
      &(physical_mode->pb_type_children[ichild]), vpr_device_annotation,
      verbose_output);
  }
}

/********************************************************************
 * This function will infer the physical mode for each multi-mode
 * pb_type in VPR pb_type graph without OpenFPGA architecture XML
 *
 * The following rule is applied:
 * if there is only 1 mode under a pb_type, it will be the default
 * physical mode for this pb_type
 *
 * Note:
 * This function must be executed AFTER the function
 *   build_vpr_physical_pb_mode_explicit_annotation()
 *******************************************************************/
static void build_vpr_physical_pb_mode_implicit_annotation(
  const DeviceContext& vpr_device_ctx,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  for (const t_logical_block_type& lb_type :
       vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }
    rec_infer_vpr_physical_pb_mode_annotation(
      lb_type.pb_type, vpr_device_annotation, verbose_output);
  }
}

/********************************************************************
 * This function aims to make a pair of operating and physical
 * pb_types:
 * - In addition to pairing the pb_types, it will pair the ports of the pb_types
 * - For the ports which are explicited annotated as physical pin mapping
 *   in the pb_type annotation.
 *   We will check the port range and create a pair
 * - For the ports which are not specified in the pb_type annotation
 *   we assume their physical ports share the same as the operating ports
 *   We will try to find a port in the physical pb_type and check the port range
 *   If found, we will create a pair
 * - All the pairs will be updated in vpr_device_annotation
 *******************************************************************/
static bool pair_operating_and_physical_pb_types(
  t_pb_type* operating_pb_type, t_pb_type* physical_pb_type,
  const PbTypeAnnotation& pb_type_annotation,
  VprDeviceAnnotation& vpr_device_annotation) {
  /* Reach here, we should have valid operating and physical pb_types */
  VTR_ASSERT((nullptr != operating_pb_type) && (nullptr != physical_pb_type));

  /* Iterate over the ports under the operating pb_type
   * For each pin, we will try to find its physical port in the
   * pb_type_annotation if not found, we assume that the physical port is the
   * same as the operating pb_port
   */
  for (t_port* operating_pb_port : pb_type_ports(operating_pb_type)) {
    std::map<BasicPort, std::array<int, 3>> expected_physical_pb_ports =
      pb_type_annotation.physical_pb_type_port(
        std::string(operating_pb_port->name));

    /* If not defined in the annotation, set the default pair:
     * rotate_offset is 0 by default!
     */
    if (true == expected_physical_pb_ports.empty()) {
      BasicPort expected_physical_pb_port;
      expected_physical_pb_port.set_name(std::string(operating_pb_port->name));
      expected_physical_pb_port.set_width(operating_pb_port->num_pins);
      expected_physical_pb_ports[expected_physical_pb_port] = {0, 0};
    }

    for (const auto& expected_physical_pb_port : expected_physical_pb_ports) {
      /* Try to find the expected port in the physical pb_type */
      t_port* physical_pb_port = find_pb_type_port(
        physical_pb_type, expected_physical_pb_port.first.get_name());
      /* Not found, mapping fails */
      if (nullptr == physical_pb_port) {
        return false;
      }
      /* If the port range does not match, mapping fails */
      if (false == BasicPort(physical_pb_port->name, physical_pb_port->num_pins)
                     .contained(expected_physical_pb_port.first)) {
        return false;
      }
      /* Now, port mapping should succeed, we update the vpr_device_annotation
       * - port binding
       * - port range
       * - port pin rotate offset
       */
      vpr_device_annotation.add_physical_pb_port(operating_pb_port,
                                                 physical_pb_port);
      vpr_device_annotation.add_physical_pb_port_range(
        operating_pb_port, physical_pb_port, expected_physical_pb_port.first);
      vpr_device_annotation.add_physical_pb_pin_initial_offset(
        operating_pb_port, physical_pb_port,
        expected_physical_pb_port.second[0]);
      vpr_device_annotation.add_physical_pb_pin_rotate_offset(
        operating_pb_port, physical_pb_port,
        expected_physical_pb_port.second[1]);
      vpr_device_annotation.add_physical_pb_port_rotate_offset(
        operating_pb_port, physical_pb_port,
        expected_physical_pb_port.second[2]);
    }
  }

  /* Now, pb_type mapping should succeed, we update the vpr_device_annotation
   *   - pb_type binding
   *   - physical_pb_type_index_factor
   *   - physical_pb_type_index_offset
   */
  vpr_device_annotation.add_physical_pb_type(operating_pb_type,
                                             physical_pb_type);
  vpr_device_annotation.add_physical_pb_type_index_factor(
    operating_pb_type, pb_type_annotation.physical_pb_type_index_factor());
  vpr_device_annotation.add_physical_pb_type_index_offset(
    operating_pb_type, pb_type_annotation.physical_pb_type_index_offset());

  return true;
}

/********************************************************************
 * This function will identify the physical pb_type for each operating
 * pb_type in VPR pb_type graph by following the explicit definition
 * in OpenFPGA architecture XML
 *
 * Note:
 * - This function should be executed only AFTER the physical mode
 *   annotation is completed
 *******************************************************************/
static void build_vpr_physical_pb_type_explicit_annotation(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* Walk through the pb_type annotation stored in the openfpga arch */
  for (const PbTypeAnnotation& pb_type_annotation :
       openfpga_arch.pb_type_annotations) {
    /* Since our target is to annotate the operating pb_type tp physical pb_type
     * we can skip those annotation only for physical pb_type
     */
    if (true == pb_type_annotation.is_physical_pb_type()) {
      continue;
    }

    VTR_ASSERT(true == pb_type_annotation.is_operating_pb_type());

    /* Collect the information about the full hierarchy of operating pb_type to
     * be annotated */
    std::vector<std::string> target_op_pb_type_names;
    std::vector<std::string> target_op_pb_mode_names;

    target_op_pb_type_names =
      pb_type_annotation.operating_parent_pb_type_names();
    target_op_pb_type_names.push_back(
      pb_type_annotation.operating_pb_type_name());
    target_op_pb_mode_names = pb_type_annotation.operating_parent_mode_names();

    /* Collect the information about the full hierarchy of physical pb_type to
     * be annotated */
    std::vector<std::string> target_phy_pb_type_names;
    std::vector<std::string> target_phy_pb_mode_names;

    target_phy_pb_type_names =
      pb_type_annotation.physical_parent_pb_type_names();
    target_phy_pb_type_names.push_back(
      pb_type_annotation.physical_pb_type_name());
    target_phy_pb_mode_names = pb_type_annotation.physical_parent_mode_names();

    /* We must have at least one pb_type in the list */
    VTR_ASSERT_SAFE(0 < target_op_pb_type_names.size());
    VTR_ASSERT_SAFE(0 < target_phy_pb_type_names.size());

    /* Pb type information are located at the logic_block_types in the device
     * context of VPR We iterate over the vectors and find the pb_type matches
     * the parent_pb_type_name
     */
    bool link_success = false;

    for (const t_logical_block_type& lb_type :
         vpr_device_ctx.logical_block_types) {
      /* By pass nullptr for pb_type head */
      if (nullptr == lb_type.pb_type) {
        continue;
      }
      /* Check the name of the top-level pb_type, if it does not match, we can
       * bypass */
      if (target_op_pb_type_names[0] != std::string(lb_type.pb_type->name)) {
        continue;
      }
      /* Match the name in the top-level, we go further to search the operating
       * as well as physical pb_types in the graph */
      t_pb_type* target_op_pb_type = try_find_pb_type_with_given_path(
        lb_type.pb_type, target_op_pb_type_names, target_op_pb_mode_names);
      if (nullptr == target_op_pb_type) {
        continue;
      }

      t_pb_type* target_phy_pb_type = try_find_pb_type_with_given_path(
        lb_type.pb_type, target_phy_pb_type_names, target_phy_pb_mode_names);
      if (nullptr == target_phy_pb_type) {
        continue;
      }

      /* Both operating and physical pb_type have been found,
       * we update the annotation by assigning the physical mode
       */
      if (true == pair_operating_and_physical_pb_types(
                    target_op_pb_type, target_phy_pb_type, pb_type_annotation,
                    vpr_device_annotation)) {
        /* Give a message */
        VTR_LOGV(
          verbose_output,
          "Annotate operating pb_type '%s' to its physical pb_type '%s'\n",
          target_op_pb_type->name, target_phy_pb_type->name);

        link_success = true;
        break;
      }
    }

    if (false == link_success) {
      /* Not found, error out! */
      VTR_LOG_ERROR(
        "Unable to pair the operating pb_type '%s' to its physical pb_type "
        "'%s'!\n",
        target_op_pb_type_names.back().c_str(),
        target_phy_pb_type_names.back().c_str());
    }
  }
}

/********************************************************************
 * This function aims to pair a physical pb_type to itself
 *******************************************************************/
static bool self_pair_physical_pb_types(
  t_pb_type* physical_pb_type, VprDeviceAnnotation& vpr_device_annotation) {
  /* Reach here, we should have valid physical pb_types */
  VTR_ASSERT(nullptr != physical_pb_type);

  /* Iterate over the ports under the operating pb_type
   * For each pin, we will try to find its physical port in the
   * pb_type_annotation if not found, we assume that the physical port is the
   * same as the operating pb_port
   */
  for (t_port* physical_pb_port : pb_type_ports(physical_pb_type)) {
    BasicPort physical_port_range(physical_pb_port->name,
                                  physical_pb_port->num_pins);
    vpr_device_annotation.add_physical_pb_port(physical_pb_port,
                                               physical_pb_port);
    vpr_device_annotation.add_physical_pb_port_range(
      physical_pb_port, physical_pb_port, physical_port_range);
  }

  /* Now, pb_type mapping should succeed, we update the vpr_device_annotation */
  vpr_device_annotation.add_physical_pb_type(physical_pb_type,
                                             physical_pb_type);

  return true;
}

/********************************************************************
 * This function will recursively visit all the pb_type from the top
 * pb_type in the graph (only in the physical mode) and infer the
 * physical pb_type
 * This is mainly applied to single-mode pb_type graphs, where the
 * physical pb_type should be pb_type itself
 * We can infer this and save the explicit annotation required by users
 *******************************************************************/
static void rec_infer_vpr_physical_pb_type_annotation(
  t_pb_type* cur_pb_type, VprDeviceAnnotation& vpr_device_annotation,
  const bool& verbose_output) {
  /* Physical pb_type is mainly for the primitive pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    /* If the physical pb_type has been mapped, we can skip it */
    if (nullptr != vpr_device_annotation.physical_pb_type(cur_pb_type)) {
      return;
    }
    /* Create the pair here */
    if (true ==
        self_pair_physical_pb_types(cur_pb_type, vpr_device_annotation)) {
      /* Give a message */
      VTR_LOGV(
        verbose_output,
        "Implicitly infer the physical pb_type for pb_type '%s' itself\n",
        cur_pb_type->name);
    } else {
      VTR_LOG_ERROR(
        "Unable to infer the physical pb_type for pb_type '%s' itself!\n",
        cur_pb_type->name);
    }
    return;
  }

  /* Get the physical mode from annotation */
  t_mode* physical_mode = vpr_device_annotation.physical_mode(cur_pb_type);

  VTR_ASSERT(nullptr != physical_mode);

  /* Traverse the pb_type children under the physical mode */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    rec_infer_vpr_physical_pb_type_annotation(
      &(physical_mode->pb_type_children[ichild]), vpr_device_annotation,
      verbose_output);
  }
}

/********************************************************************
 * This function will infer the physical pb_type for each operating
 * pb_type in VPR pb_type graph which have not been explicitedly defined
 * in OpenFPGA architecture XML
 *
 * Note:
 * - This function should be executed only AFTER the physical mode
 *   annotation is completed
 *******************************************************************/
static void build_vpr_physical_pb_type_implicit_annotation(
  const DeviceContext& vpr_device_ctx,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  for (const t_logical_block_type& lb_type :
       vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }
    rec_infer_vpr_physical_pb_type_annotation(
      lb_type.pb_type, vpr_device_annotation, verbose_output);
  }
}

/********************************************************************
 * This function aims to link all the ports defined under a physical
 * pb_type to the ports of circuit model in the circuit library
 * The binding assumes that pb_type port should be defined with the same name
 * as the circuit port
 *******************************************************************/
static bool link_physical_pb_port_to_circuit_port(
  t_pb_type* physical_pb_type, const CircuitLibrary& circuit_lib,
  const CircuitModelId& circuit_model,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  bool link_success = true;
  /* Iterate over the pb_ports
   * Note:
   *   - Not every port defined in the circuit model will appear under the
   * pb_type Some circuit ports are just for physical implementation
   */
  for (t_port* pb_port : pb_type_ports(physical_pb_type)) {
    CircuitPortId circuit_port =
      circuit_lib.model_port(circuit_model, std::string(pb_port->name));

    /* If we cannot find a port with the same name, error out */
    if (CircuitPortId::INVALID() == circuit_port) {
      VTR_LOG_ERROR(
        "Pb type port '%s' is not found in any port of circuit model '%s'!\n",
        pb_port->name, circuit_lib.model_name(circuit_model).c_str());
      link_success = false;
      continue;
    }

    /* If the port width does not match, error out */
    if ((size_t)(pb_port->num_pins) != circuit_lib.port_size(circuit_port)) {
      VTR_LOG_ERROR(
        "Pb type port '%s[%d:%d]' does not match the port '%s[%d:%d]' of "
        "circuit model '%s' in size!\n",
        pb_port->name, 0, pb_port->num_pins - 1,
        circuit_lib.port_prefix(circuit_port).c_str(), 0,
        circuit_lib.port_size(circuit_port) - 1,
        circuit_lib.model_name(circuit_model).c_str());
      link_success = false;
      continue;
    }

    /* If the port type does not match, error out */
    if (ERR_PORT == circuit_port_require_pb_port_type(
                      circuit_lib.port_type(circuit_port))) {
      VTR_LOG_ERROR(
        "Pb type port '%s' type does not match the port type '%s' of circuit "
        "model '%s'!\n",
        pb_port->name, circuit_lib.port_prefix(circuit_port).c_str(),
        circuit_lib.model_name(circuit_model).c_str());

      link_success = false;
      continue;
    }

    /* Reach here, it means that mapping should be ok, update the
     * vpr_device_annotation */
    vpr_device_annotation.add_pb_circuit_port(pb_port, circuit_port);
    VTR_LOGV(verbose_output,
             "Bind pb type '%s' port '%s' to circuit model '%s' port '%s'\n",
             physical_pb_type->name, pb_port->name,
             circuit_lib.model_name(circuit_model).c_str(),
             circuit_lib.port_prefix(circuit_port).c_str());
  }

  return link_success;
}

/********************************************************************
 * This function aims to link a physical pb_type to a valid circuit model
 * in the circuit library
 *******************************************************************/
static bool link_physical_pb_type_to_circuit_model(
  t_pb_type* physical_pb_type, const CircuitLibrary& circuit_lib,
  const PbTypeAnnotation& pb_type_annotation,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* Reach here, we should have valid operating and physical pb_types */
  VTR_ASSERT(nullptr != physical_pb_type);

  /* This must be a physical pb_type according to our annotation! */
  if (false == vpr_device_annotation.is_physical_pb_type(physical_pb_type)) {
    VTR_LOG_ERROR(
      "An operating pb_type '%s' is not allowed to be linked to any circuit "
      "model!\n",
      physical_pb_type->name);
    return false;
  }

  std::string pb_type_circuit_model_name =
    pb_type_annotation.circuit_model_name();
  CircuitModelId circuit_model_id =
    circuit_lib.model(pb_type_circuit_model_name);

  if (CircuitModelId::INVALID() == circuit_model_id) {
    VTR_LOG_ERROR(
      "Unable to find a circuit model '%s' for physical pb_type '%s'!\n",
      pb_type_circuit_model_name.c_str(), physical_pb_type->name);
    return false;
  }

  /* Ensure that the pb_type ports can be matched in the circuit model ports */
  if (false == link_physical_pb_port_to_circuit_port(
                 physical_pb_type, circuit_lib, circuit_model_id,
                 vpr_device_annotation, verbose_output)) {
    return false;
  }

  /* Now the circuit model is valid, update the vpr_device_annotation */
  vpr_device_annotation.add_pb_type_circuit_model(physical_pb_type,
                                                  circuit_model_id);
  return true;
}

/********************************************************************
 * This function aims to link an interconnect of a physical mode of
 * a pb_type to a valid circuit model in the circuit library
 *******************************************************************/
static bool link_physical_pb_interconnect_to_circuit_model(
  t_pb_type* physical_pb_type, const std::string& interconnect_name,
  const CircuitLibrary& circuit_lib, const PbTypeAnnotation& pb_type_annotation,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* The physical pb_type should NOT be a primitive, otherwise it should never
   * contain any interconnect */
  if (true == is_primitive_pb_type(physical_pb_type)) {
    VTR_LOG_ERROR(
      "Link interconnect to circuit model is not allowed for a primitive "
      "pb_type '%s'!\n",
      physical_pb_type->name);
    return false;
  }

  /* Get the physical mode from annotation */
  t_mode* physical_mode = vpr_device_annotation.physical_mode(physical_pb_type);

  VTR_ASSERT(nullptr != physical_mode);

  /* Find the interconnect name under the physical mode of a physical pb_type */
  t_interconnect* pb_interc =
    find_pb_mode_interconnect(physical_mode, interconnect_name.c_str());

  if (nullptr == pb_interc) {
    VTR_LOG_ERROR(
      "Unable to find interconnect '%s' under physical mode '%s' of pb_type "
      "'%s'!\n",
      interconnect_name.c_str(), physical_mode->name, physical_pb_type->name);
    return false;
  }

  /* Try to find the circuit model name */
  std::string pb_type_circuit_model_name =
    pb_type_annotation.interconnect_circuit_model_name(interconnect_name);
  CircuitModelId circuit_model_id =
    circuit_lib.model(pb_type_circuit_model_name);

  if (CircuitModelId::INVALID() == circuit_model_id) {
    VTR_LOG_ERROR(
      "Unable to find a circuit model '%s' for interconnect '%s' under "
      "physical mode '%s' of pb_type '%s'!\n",
      pb_type_circuit_model_name.c_str(), interconnect_name.c_str(),
      physical_mode->name, physical_pb_type->name);
    return false;
  }

  /* Double check the type of circuit model, it should be the same as required
   * physical type */
  e_circuit_model_type required_circuit_model_type =
    pb_interconnect_require_circuit_model_type(
      vpr_device_annotation.interconnect_physical_type(pb_interc));
  if (circuit_lib.model_type(circuit_model_id) != required_circuit_model_type) {
    VTR_LOG_ERROR(
      "Circuit model '%s' type '%s' does not match required type '%s' for "
      "interconnect '%s' under physical mode '%s' of pb_type '%s'!\n",
      circuit_lib.model_name(circuit_model_id).c_str(),
      CIRCUIT_MODEL_TYPE_STRING[circuit_lib.model_type(circuit_model_id)],
      CIRCUIT_MODEL_TYPE_STRING[required_circuit_model_type], pb_interc->name,
      physical_mode->name, physical_pb_type->name);
    return false;
  }

  /* Now the circuit model is valid, update the vpr_device_annotation */
  vpr_device_annotation.add_interconnect_circuit_model(pb_interc,
                                                       circuit_model_id);

  VTR_LOGV(verbose_output,
           "Bind pb_type '%s' physical mode '%s' interconnect '%s' to circuit "
           "model '%s'\n",
           physical_pb_type->name, physical_mode->name, pb_interc->name,
           circuit_lib.model_name(circuit_model_id).c_str());

  return true;
}

/********************************************************************
 * This function will link
 * - pb_type to circuit models in circuit library by following
 *   the explicit definition in OpenFPGA architecture XML
 *
 * Note:
 * - This function should be executed only AFTER the physical mode and
 *   physical pb_type annotation is completed
 *******************************************************************/
static void link_vpr_pb_type_to_circuit_model_explicit_annotation(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* Walk through the pb_type annotation stored in the openfpga arch */
  for (const PbTypeAnnotation& pb_type_annotation :
       openfpga_arch.pb_type_annotations) {
    /* Since our target is to annotate the circuti model for physical pb_type
     * we can skip those annotation only for operating pb_type
     */
    if (true == pb_type_annotation.is_operating_pb_type()) {
      continue;
    }

    if (true == pb_type_annotation.circuit_model_name().empty()) {
      continue;
    }

    VTR_ASSERT(true == pb_type_annotation.is_physical_pb_type());

    /* Collect the information about the full hierarchy of physical pb_type to
     * be annotated */
    std::vector<std::string> target_phy_pb_type_names;
    std::vector<std::string> target_phy_pb_mode_names;

    target_phy_pb_type_names =
      pb_type_annotation.physical_parent_pb_type_names();
    target_phy_pb_type_names.push_back(
      pb_type_annotation.physical_pb_type_name());
    target_phy_pb_mode_names = pb_type_annotation.physical_parent_mode_names();

    /* We must have at least one pb_type in the list */
    VTR_ASSERT_SAFE(0 < target_phy_pb_type_names.size());

    /* Pb type information are located at the logic_block_types in the device
     * context of VPR We iterate over the vectors and find the pb_type matches
     * the parent_pb_type_name
     */
    bool link_success = false;

    for (const t_logical_block_type& lb_type :
         vpr_device_ctx.logical_block_types) {
      /* By pass nullptr for pb_type head */
      if (nullptr == lb_type.pb_type) {
        continue;
      }
      /* Check the name of the top-level pb_type, if it does not match, we can
       * bypass */
      if (target_phy_pb_type_names[0] != std::string(lb_type.pb_type->name)) {
        continue;
      }
      /* Match the name in the top-level, we go further to search the operating
       * as well as physical pb_types in the graph */
      t_pb_type* target_phy_pb_type = try_find_pb_type_with_given_path(
        lb_type.pb_type, target_phy_pb_type_names, target_phy_pb_mode_names);
      if (nullptr == target_phy_pb_type) {
        continue;
      }

      /* Only try to bind pb_type to circuit model when it is defined by users
       */
      if (true == link_physical_pb_type_to_circuit_model(
                    target_phy_pb_type, openfpga_arch.circuit_lib,
                    pb_type_annotation, vpr_device_annotation,
                    verbose_output)) {
        /* Give a message */
        VTR_LOGV(verbose_output,
                 "Bind physical pb_type '%s' to its circuit model '%s'\n",
                 target_phy_pb_type->name,
                 openfpga_arch.circuit_lib
                   .model_name(vpr_device_annotation.pb_type_circuit_model(
                     target_phy_pb_type))
                   .c_str());

        link_success = true;
        break;
      }
    }

    if (false == link_success) {
      /* Not found, error out! */
      VTR_LOG_ERROR(
        "Unable to bind physical pb_type '%s' to circuit model '%s'!\n",
        target_phy_pb_type_names.back().c_str(),
        pb_type_annotation.circuit_model_name().c_str());
      return;
    }
  }
}

/********************************************************************
 * This function will link
 * - interconnect of pb_type to circuit models in circuit library by following
 *   the explicit definition in OpenFPGA architecture XML
 *
 * Note:
 * - This function should be executed only AFTER the physical mode and
 *   physical pb_type annotation is completed
 *******************************************************************/
static void link_vpr_pb_interconnect_to_circuit_model_explicit_annotation(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* Walk through the pb_type annotation stored in the openfpga arch */
  for (const PbTypeAnnotation& pb_type_annotation :
       openfpga_arch.pb_type_annotations) {
    /* Since our target is to annotate the circuti model for physical pb_type
     * we can skip those annotation only for operating pb_type
     */
    if (true == pb_type_annotation.is_operating_pb_type()) {
      continue;
    }

    if (0 == pb_type_annotation.interconnect_names().size()) {
      continue;
    }

    VTR_ASSERT(true == pb_type_annotation.is_physical_pb_type());

    /* Collect the information about the full hierarchy of physical pb_type to
     * be annotated */
    std::vector<std::string> target_phy_pb_type_names;
    std::vector<std::string> target_phy_pb_mode_names;

    target_phy_pb_type_names =
      pb_type_annotation.physical_parent_pb_type_names();
    target_phy_pb_type_names.push_back(
      pb_type_annotation.physical_pb_type_name());
    target_phy_pb_mode_names = pb_type_annotation.physical_parent_mode_names();

    /* We must have at least one pb_type in the list */
    VTR_ASSERT_SAFE(0 < target_phy_pb_type_names.size());

    /* Pb type information are located at the logic_block_types in the device
     * context of VPR We iterate over the vectors and find the pb_type matches
     * the parent_pb_type_name
     */
    bool link_success = true;

    for (const t_logical_block_type& lb_type :
         vpr_device_ctx.logical_block_types) {
      /* By pass nullptr for pb_type head */
      if (nullptr == lb_type.pb_type) {
        continue;
      }
      /* Check the name of the top-level pb_type, if it does not match, we can
       * bypass */
      if (target_phy_pb_type_names[0] != std::string(lb_type.pb_type->name)) {
        continue;
      }
      /* Match the name in the top-level, we go further to search the operating
       * as well as physical pb_types in the graph */
      t_pb_type* target_phy_pb_type = try_find_pb_type_with_given_path(
        lb_type.pb_type, target_phy_pb_type_names, target_phy_pb_mode_names);
      if (nullptr == target_phy_pb_type) {
        continue;
      }

      /* Only try to bind interconnect to circuit model when it is defined by
       * users */
      for (const std::string& interc_name :
           pb_type_annotation.interconnect_names()) {
        if (false == link_physical_pb_interconnect_to_circuit_model(
                       target_phy_pb_type, interc_name,
                       openfpga_arch.circuit_lib, pb_type_annotation,
                       vpr_device_annotation, verbose_output)) {
          VTR_LOG_ERROR(
            "Unable to bind pb_type '%s' interconnect '%s' to circuit model "
            "'%s'!\n",
            target_phy_pb_type_names.back().c_str(), interc_name.c_str(),
            pb_type_annotation.circuit_model_name().c_str());
          link_success = false;
        }
      }
    }

    if (false == link_success) {
      /* Not found, error out! */
      VTR_LOG_ERROR(
        "Unable to bind interconnects of physical pb_type '%s' to circuit "
        "model '%s'!\n",
        target_phy_pb_type_names.back().c_str(),
        pb_type_annotation.circuit_model_name().c_str());
      return;
    }
  }
}

/********************************************************************
 * This function will recursively visit all the pb_type from the top
 * pb_type in the graph and infer the circuit model for physical mode
 * of pb_types in VPR pb_type graph without OpenFPGA architecture XML
 *
 * Because only the interconnect in physical modes need circuit model
 * annotation, we will skip all the operating modes here
 *
 * Note:
 *   - This function will automatically infer the type of circuit model
 *     that is required and find the default circuit model in the type
 *   - Interconnect type to Circuit mode type assumption:
 *     - MUX_INTERC -> CIRCUIT_MODEL_MUX
 *     - DIRECT_INTERC -> CIRCUIT_MODEL_WIRE
 *     - COMPLETE_INTERC (single input) -> CIRCUIT_MODEL_WIRE
 *     - COMPLETE_INTERC (multiple input pins) -> CIRCUIT_MODEL_MUX
 *******************************************************************/
static void rec_infer_vpr_pb_interconnect_circuit_model_annotation(
  t_pb_type* cur_pb_type, const CircuitLibrary& circuit_lib,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* We do not check any primitive pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    return;
  }

  /* Get the physical mode from annotation */
  t_mode* physical_mode = vpr_device_annotation.physical_mode(cur_pb_type);

  VTR_ASSERT(nullptr != physical_mode);

  /* Annotate the circuit model for each interconnect under this physical mode
   */
  for (t_interconnect* pb_interc : pb_mode_interconnects(physical_mode)) {
    /* If the interconnect has been annotated, we skip it */
    if (CircuitModelId::INVALID() !=
        vpr_device_annotation.interconnect_circuit_model(pb_interc)) {
      continue;
    }
    /* Infer the circuit model type for a given interconnect */
    e_circuit_model_type circuit_model_type =
      pb_interconnect_require_circuit_model_type(
        vpr_device_annotation.interconnect_physical_type(pb_interc));
    /* Try to find a default circuit model from the circuit library */
    CircuitModelId default_circuit_model =
      circuit_lib.default_model(circuit_model_type);
    /* Update the annotation if the model id is valid */
    if (CircuitModelId::INVALID() == default_circuit_model) {
      VTR_LOG_ERROR(
        "Unable to infer a circuit model for interconnect '%s' under physical "
        "mode '%s' of pb_type '%s'!\n",
        pb_interc->name, physical_mode->name, cur_pb_type->name);
    }
    vpr_device_annotation.add_interconnect_circuit_model(pb_interc,
                                                         default_circuit_model);
    VTR_LOGV(verbose_output,
             "Implicitly infer a circuit model '%s' for interconnect '%s' "
             "under physical mode '%s' of pb_type '%s'\n",
             circuit_lib.model_name(default_circuit_model).c_str(),
             pb_interc->name, physical_mode->name, cur_pb_type->name);
  }

  /* Traverse the pb_type children under the physical mode */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    rec_infer_vpr_pb_interconnect_circuit_model_annotation(
      &(physical_mode->pb_type_children[ichild]), circuit_lib,
      vpr_device_annotation, verbose_output);
  }
}

/********************************************************************
 * This function will infer the circuit model for each interconnect
 * under a physical mode of a pb_type in VPR pb_type graph without
 * OpenFPGA architecture XML
 *
 * Note:
 * This function must be executed AFTER the function
 *   build_vpr_physical_pb_mode_explicit_annotation()
 *   build_vpr_physical_pb_mode_implicit_annotation()
 *******************************************************************/
static void link_vpr_pb_interconnect_to_circuit_model_implicit_annotation(
  const DeviceContext& vpr_device_ctx, const CircuitLibrary& circuit_lib,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  for (const t_logical_block_type& lb_type :
       vpr_device_ctx.logical_block_types) {
    /* By pass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }
    rec_infer_vpr_pb_interconnect_circuit_model_annotation(
      lb_type.pb_type, circuit_lib, vpr_device_annotation, verbose_output);
  }
}

/********************************************************************
 * This function will bind mode selection bits to a primitive pb_type
 * in the vpr_device_annotation
 *******************************************************************/
static bool link_primitive_pb_type_to_mode_bits(
  t_pb_type* primitive_pb_type, const PbTypeAnnotation& pb_type_annotation,
  VprDeviceAnnotation& vpr_device_annotation) {
  /* Error out if this is not a primitive pb_type */
  if (false == is_primitive_pb_type(primitive_pb_type)) {
    VTR_LOG_ERROR(
      "Mode selection is only applicable to primitive pb_type while pb_type "
      "'%s' is not primitve !\n",
      primitive_pb_type->name);
    return false;
  }

  /* Update the annotation */
  vpr_device_annotation.add_pb_type_mode_bits(
    primitive_pb_type, pb_type_annotation.mode_bits(), true);

  return true;
}

/********************************************************************
 * This function will link
 * - pb_type to mode bits by following
 *   the explicit definition in OpenFPGA architecture XML
 *
 * Note:
 * - This function should be executed only AFTER
 *   the physical mode and physical pb_type annotation is completed
 *   the physical pb_type circuit model annotation is completed
 *******************************************************************/
static void link_vpr_pb_type_to_mode_bits_explicit_annotation(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output) {
  /* Walk through the pb_type annotation stored in the openfpga arch */
  for (const PbTypeAnnotation& pb_type_annotation :
       openfpga_arch.pb_type_annotations) {
    if (true == pb_type_annotation.mode_bits().empty()) {
      continue;
    }

    /* Convert the vector of integer to string */
    std::string mode_bits_str;
    for (const char& bit : pb_type_annotation.mode_bits()) {
      mode_bits_str += bit;
    }

    /* Collect the information about the full hierarchy of physical pb_type to
     * be annotated */
    std::vector<std::string> target_pb_type_names;
    std::vector<std::string> target_pb_mode_names;

    if (true == pb_type_annotation.is_operating_pb_type()) {
      target_pb_type_names =
        pb_type_annotation.operating_parent_pb_type_names();
      target_pb_type_names.push_back(
        pb_type_annotation.operating_pb_type_name());
      target_pb_mode_names = pb_type_annotation.operating_parent_mode_names();
    }

    if (true == pb_type_annotation.is_physical_pb_type()) {
      target_pb_type_names = pb_type_annotation.physical_parent_pb_type_names();
      target_pb_type_names.push_back(
        pb_type_annotation.physical_pb_type_name());
      target_pb_mode_names = pb_type_annotation.physical_parent_mode_names();
    }

    /* We must have at least one pb_type in the list */
    VTR_ASSERT_SAFE(0 < target_pb_type_names.size());

    /* Pb type information are located at the logic_block_types in the device
     * context of VPR We iterate over the vectors and find the pb_type matches
     * the parent_pb_type_name
     */
    bool link_success = false;

    for (const t_logical_block_type& lb_type :
         vpr_device_ctx.logical_block_types) {
      /* By pass nullptr for pb_type head */
      if (nullptr == lb_type.pb_type) {
        continue;
      }
      /* Check the name of the top-level pb_type, if it does not match, we can
       * bypass */
      if (target_pb_type_names[0] != std::string(lb_type.pb_type->name)) {
        continue;
      }
      /* Match the name in the top-level, we go further to search the operating
       * as well as physical pb_types in the graph */
      t_pb_type* target_pb_type = try_find_pb_type_with_given_path(
        lb_type.pb_type, target_pb_type_names, target_pb_mode_names);
      if (nullptr == target_pb_type) {
        continue;
      }

      /* Only try to bind pb_type to circuit model when it is defined by users
       */
      if (true == link_primitive_pb_type_to_mode_bits(target_pb_type,
                                                      pb_type_annotation,
                                                      vpr_device_annotation)) {
        /* Give a message */
        VTR_LOGV(verbose_output,
                 "Bind physical pb_type '%s' to mode selection bits '%s'\n",
                 target_pb_type->name, mode_bits_str.c_str());

        link_success = true;
        break;
      }
    }

    if (false == link_success) {
      /* Not found, error out! */
      VTR_LOG_ERROR(
        "Unable to bind pb_type '%s' to mode_selection bits '%s'!\n",
        target_pb_type_names.back().c_str(), mode_bits_str.c_str());
      return;
    }
  }
}

/********************************************************************
 * Top-level function to link openfpga architecture to VPR, including:
 * - physical pb_type
 * - circuit models for pb_type, pb interconnect
 *******************************************************************/
void annotate_pb_types(const DeviceContext& vpr_device_ctx,
                       const Arch& openfpga_arch,
                       VprDeviceAnnotation& vpr_device_annotation,
                       const bool& verbose_output) {
  /* Annotate physical mode to pb_type in the VPR pb_type graph */
  VTR_LOG("\n");
  VTR_LOG("Building annotation for physical modes in pb_type...");
  VTR_LOGV(verbose_output, "\n");
  build_vpr_physical_pb_mode_explicit_annotation(
    vpr_device_ctx, openfpga_arch, vpr_device_annotation, verbose_output);

  build_vpr_physical_pb_mode_implicit_annotation(
    vpr_device_ctx, vpr_device_annotation, verbose_output);
  VTR_LOG("Done\n");

  check_vpr_physical_pb_mode_annotation(
    vpr_device_ctx,
    const_cast<const VprDeviceAnnotation&>(vpr_device_annotation));

  /* Annotate the physical type for each interconnect under physical modes
   * Must run AFTER physical mode annotation is done and
   * BEFORE inferring the circuit model for interconnect
   */
  VTR_LOG("\n");
  VTR_LOG(
    "Building annotation about physical types for pb_type interconnection...");
  VTR_LOGV(verbose_output, "\n");
  annotate_pb_graph_interconnect_physical_type(
    vpr_device_ctx, vpr_device_annotation, verbose_output);
  VTR_LOG("Done\n");

  /* Annotate physical pb_types to operating pb_type in the VPR pb_type graph
   * This function will also annotate
   * - pb_type_index_factor
   * - pb_type_index_offset
   * - physical_pin_rotate_offset
   */
  VTR_LOG("\n");
  VTR_LOG("Building annotation between operating and physical pb_types...");
  VTR_LOGV(verbose_output, "\n");
  build_vpr_physical_pb_type_explicit_annotation(
    vpr_device_ctx, openfpga_arch, vpr_device_annotation, verbose_output);

  build_vpr_physical_pb_type_implicit_annotation(
    vpr_device_ctx, vpr_device_annotation, verbose_output);
  VTR_LOG("Done\n");

  check_vpr_physical_pb_type_annotation(
    vpr_device_ctx,
    const_cast<const VprDeviceAnnotation&>(vpr_device_annotation));

  /* Link
   * - physical pb_type to circuit model
   * - interconnect of physical pb_type to circuit model
   */
  VTR_LOG("\n");
  VTR_LOG(
    "Building annotation between physical pb_types and circuit models...");
  VTR_LOGV(verbose_output, "\n");
  link_vpr_pb_type_to_circuit_model_explicit_annotation(
    vpr_device_ctx, openfpga_arch, vpr_device_annotation, verbose_output);

  link_vpr_pb_interconnect_to_circuit_model_explicit_annotation(
    vpr_device_ctx, openfpga_arch, vpr_device_annotation, verbose_output);

  link_vpr_pb_interconnect_to_circuit_model_implicit_annotation(
    vpr_device_ctx, openfpga_arch.circuit_lib, vpr_device_annotation,
    verbose_output);
  VTR_LOG("Done\n");

  check_vpr_pb_type_circuit_model_annotation(
    vpr_device_ctx, openfpga_arch.circuit_lib,
    const_cast<const VprDeviceAnnotation&>(vpr_device_annotation));

  /* Link physical pb_type to mode_bits */
  VTR_LOG("\n");
  VTR_LOG(
    "Building annotation between physical pb_types and mode selection bits...");
  VTR_LOGV(verbose_output, "\n");
  link_vpr_pb_type_to_mode_bits_explicit_annotation(
    vpr_device_ctx, openfpga_arch, vpr_device_annotation, verbose_output);
  VTR_LOG("Done\n");

  check_vpr_pb_type_mode_bits_annotation(
    vpr_device_ctx, openfpga_arch.circuit_lib,
    const_cast<const VprDeviceAnnotation&>(vpr_device_annotation));
}

} /* end namespace openfpga */
