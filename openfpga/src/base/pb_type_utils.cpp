/********************************************************************
 * This file includes most utilized functions for the t_pb_type,
 * t_mode and t_port data structure in the OpenFPGA context
 *******************************************************************/
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "pb_type_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************************************ 
 * A pb_type is considered to be primitive when it has zero modes
 * However, this not always true. An exception is the LUT_CLASS
 * VPR added two modes by default to a LUT pb_type. Therefore,   
 * for LUT_CLASS, it is a primitive when it is binded to a blif model
 *
 * Note: 
 * - if VPR changes its mode organization for LUT pb_type
 *   this code should be adapted as well!
 ************************************************************************/
bool is_primitive_pb_type(t_pb_type* pb_type) {
  if (LUT_CLASS == pb_type->class_type) {
    /* The first mode of LUT is wire, the second is the regular LUT */
    VTR_ASSERT(std::string("wire") == std::string(pb_type->modes[0].name)); 
    VTR_ASSERT(std::string(pb_type->name) == std::string(pb_type->modes[1].name)); 
    return true;
  }
  return 0 == pb_type->num_modes;
}

/************************************************************************ 
 * A pb_type is the root pb_type when it has no parent mode
 ************************************************************************/
bool is_root_pb_type(t_pb_type* pb_type) { 
  return pb_type->parent_mode == nullptr; 
}

/************************************************************************ 
 * With a given mode name, find the mode pointer
 ************************************************************************/
t_mode* find_pb_type_mode(t_pb_type* pb_type, const char* mode_name) {
  for (int i = 0; i < pb_type->num_modes; ++i) {
    if (std::string(mode_name) == std::string(pb_type->modes[i].name)) {
       return &(pb_type->modes[i]);
    }
  }
  /* Note found, return a nullptr */
  return nullptr;
}

/************************************************************************ 
 * With a given pb_type name, find the pb_type pointer
 ************************************************************************/
t_pb_type* find_mode_child_pb_type(t_mode* mode, const char* child_name) {
  for (int i = 0; i < mode->num_pb_type_children; ++i) {
    if (std::string(child_name) == std::string(mode->pb_type_children[i].name)) {
       return &(mode->pb_type_children[i]);
    }
  }
  /* Note found, return a nullptr */
  return nullptr;
}

/************************************************************************ 
 * With a given pb_type, provide a list of its ports
 ************************************************************************/
std::vector<t_port*> pb_type_ports(t_pb_type* pb_type) {
  std::vector<t_port*> ports;
  for (int i = 0; i < pb_type->num_ports; ++i) {
    ports.push_back(&(pb_type->ports[i]));
  }
  return ports;
}

/************************************************************************ 
 * Find a port for a pb_type with a given name
 * If not found, return null pointer
 ************************************************************************/
t_port* find_pb_type_port(t_pb_type* pb_type, const std::string& port_name) {
  for (int i = 0; i < pb_type->num_ports; ++i) {
    if (port_name == std::string(pb_type->ports[i].name)) {
      return &(pb_type->ports[i]);
    }
  }
  return nullptr;
}

/********************************************************************
 * This function will traverse pb_type graph from its top to find
 * a pb_type with a given name as well as its hierarchy 
 *******************************************************************/
t_pb_type* try_find_pb_type_with_given_path(t_pb_type* top_pb_type, 
                                            const std::vector<std::string>& target_pb_type_names, 
                                            const std::vector<std::string>& target_pb_mode_names) {
  /* Ensure that number of parent names and modes matches */
  VTR_ASSERT_SAFE(target_pb_type_names.size() == target_pb_mode_names.size() + 1);

  t_pb_type* cur_pb_type = top_pb_type;

  /* If the top pb_type is what we want, we can return here */
  if (1 == target_pb_type_names.size()) {
    if (target_pb_type_names[0] == std::string(top_pb_type->name)) {
      return top_pb_type;
    }
    /* Not match, return null pointer */
    return nullptr;
  }

  /* We start from the first element of the parent names and parent modes.
   * If the pb_type does not match in name, we fail 
   * If we cannot find a mode match the name, we fail 
   */
  for (size_t i = 0; i < target_pb_type_names.size() - 1; ++i) {
    /* If this level does not match, search fail */
    if (target_pb_type_names[i] != std::string(cur_pb_type->name)) {
      return nullptr;
    }
    /* Find if the mode matches */
    t_mode* cur_mode = find_pb_type_mode(cur_pb_type, target_pb_mode_names[i].c_str()); 
    if (nullptr == cur_mode) {
      return nullptr;
    }
    /* Go to the next level of pb_type */
    cur_pb_type = find_mode_child_pb_type(cur_mode, target_pb_type_names[i + 1].c_str());
    if (nullptr == cur_pb_type) {
      return nullptr;
    }
    /* If this is already the last pb_type in the list, this is what we want */
    if (i + 1 == target_pb_type_names.size() - 1) {
      return cur_pb_type;
    }
  }

  /* Reach here, it means we find nothing */
  return nullptr;
}

/********************************************************************
 * This function will return all the interconnects defined under a mode
 * of pb_type
 *******************************************************************/
std::vector<t_interconnect*> pb_mode_interconnects(t_mode* pb_mode) {
  std::vector<t_interconnect*> interc;
  for (int i = 0; i < pb_mode->num_interconnect; ++i) {
    interc.push_back(&(pb_mode->interconnect[i]));
  }
  return interc;
}

/********************************************************************
 * This function will try to find an interconnect defined under a mode
 * of pb_type with a given name.
 * If not found, return null pointer
 *******************************************************************/
t_interconnect* find_pb_mode_interconnect(t_mode* pb_mode, const char* interc_name) {
  for (int i = 0; i < pb_mode->num_interconnect; ++i) {
    if (std::string(interc_name) == std::string(pb_mode->interconnect[i].name)) {
      return &(pb_mode->interconnect[i]);
    }
  } 

  /* Reach here, it means we find nothing */
  return nullptr;
}

/********************************************************************
 * This function will automatically infer the actual type of an interconnect
 * that will be used to implement the physical design:
 * - MUX_INTERC -> MUX_INTERC
 * - DIRECT_INTERC -> DIRECT_INTERC
 * - COMPLETE_INTERC (single input) -> DIRECT_INTERC
 * - COMPLETE_INTERC (multiple input pins) -> MUX_INTERC
 *******************************************************************/
e_interconnect pb_interconnect_physical_type(t_interconnect* pb_interc,
                                             const size_t& num_inputs) {
  /* Check */
  VTR_ASSERT(nullptr != pb_interc); 

  /* Initialize the interconnection type that will be implemented in SPICE netlist*/
  switch (pb_interc->type) {
  case DIRECT_INTERC:
    return DIRECT_INTERC;
    break;
  case COMPLETE_INTERC:
    if (1 == num_inputs) {
      return DIRECT_INTERC;
    } else {
      VTR_ASSERT(1 < num_inputs);
      return MUX_INTERC;
    }
    break;
  case MUX_INTERC:
    return MUX_INTERC;
    break;
  default:
    VTR_LOG_ERROR("Invalid type for interconnection '%s'!\n",
                   pb_interc->name);
    exit(1);
  }

  return NUM_INTERC_TYPES;
}

/********************************************************************
 * This function will automatically infer the actual type of an interconnect
 * that will be used to implement the physical design:
 * - MUX_INTERC -> CIRCUIT_MODEL_MUX
 * - DIRECT_INTERC -> CIRCUIT_MODEL_WIRE
 * 
 * Note: 
 *   - COMPLETE_INTERC should not appear here!
 *   - We assume the interconnect type is the physical type
 *     after interconnect physical type annotation is done! 
 *******************************************************************/
e_circuit_model_type pb_interconnect_require_circuit_model_type(const e_interconnect& pb_interc_type) {
  /* A map from interconnect type to circuit model type */
  std::map<e_interconnect, e_circuit_model_type> type_mapping;
  type_mapping[MUX_INTERC] = CIRCUIT_MODEL_MUX;
  type_mapping[DIRECT_INTERC] = CIRCUIT_MODEL_WIRE;

  VTR_ASSERT((MUX_INTERC == pb_interc_type) || (DIRECT_INTERC == pb_interc_type));
 
  return type_mapping.at(pb_interc_type); 
}

} /* end namespace openfpga */
