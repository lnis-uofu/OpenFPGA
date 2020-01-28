/********************************************************************
 * This file includes most utilized functions for the pb_type
 * and pb_graph_node data structure in the OpenFPGA context
 *******************************************************************/
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

} /* end namespace openfpga */
