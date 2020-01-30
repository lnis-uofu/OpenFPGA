/************************************************************************
 * Member functions for class VprPbTypeAnnotation
 ***********************************************************************/
#include <algorithm>

#include "vtr_log.h"
#include "vtr_assert.h"
#include "vpr_pb_type_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprPbTypeAnnotation::VprPbTypeAnnotation() {
  return;
}

/************************************************************************
 * Public accessors
 ***********************************************************************/
bool VprPbTypeAnnotation::is_physical_pb_type(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, t_pb_type*>::const_iterator it = physical_pb_types_.find(pb_type);
  if (it == physical_pb_types_.end()) {
    return false;
  }
  /* A physical pb_type should be mapped to itself! Otherwise, it is an operating pb_type */
  return pb_type == physical_pb_types_.at(pb_type);
}

t_mode* VprPbTypeAnnotation::physical_mode(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, t_mode*>::const_iterator it = physical_pb_modes_.find(pb_type);
  if (it == physical_pb_modes_.end()) {
    return nullptr;
  }
  return physical_pb_modes_.at(pb_type);
}

t_pb_type* VprPbTypeAnnotation::physical_pb_type(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, t_pb_type*>::const_iterator it = physical_pb_types_.find(pb_type);
  if (it == physical_pb_types_.end()) {
    return nullptr;
  }
  return physical_pb_types_.at(pb_type);
}

t_port* VprPbTypeAnnotation::physical_pb_port(t_port* pb_port) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_port*, t_port*>::const_iterator it = physical_pb_ports_.find(pb_port);
  if (it == physical_pb_ports_.end()) {
    return nullptr;
  }
  return physical_pb_ports_.at(pb_port);
}

BasicPort VprPbTypeAnnotation::physical_pb_port_range(t_port* pb_port) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_port*, BasicPort>::const_iterator it = physical_pb_port_ranges_.find(pb_port);
  if (it == physical_pb_port_ranges_.end()) {
    /* Return an invalid port. As such the port width will be 0, which is an invalid value */
    return BasicPort();
  }
  return physical_pb_port_ranges_.at(pb_port);
}

CircuitModelId VprPbTypeAnnotation::pb_type_circuit_model(t_pb_type* physical_pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, CircuitModelId>::const_iterator it = pb_type_circuit_models_.find(physical_pb_type);
  if (it == pb_type_circuit_models_.end()) {
    /* Return an invalid circuit model id */
    return CircuitModelId::INVALID();
  }
  return pb_type_circuit_models_.at(physical_pb_type);
}

CircuitModelId VprPbTypeAnnotation::interconnect_circuit_model(t_interconnect* pb_interconnect) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_interconnect*, CircuitModelId>::const_iterator it = interconnect_circuit_models_.find(pb_interconnect);
  if (it == interconnect_circuit_models_.end()) {
    /* Return an invalid circuit model id */
    return CircuitModelId::INVALID();
  }
  return interconnect_circuit_models_.at(pb_interconnect);
}

e_interconnect VprPbTypeAnnotation::interconnect_physical_type(t_interconnect* pb_interconnect) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_interconnect*, e_interconnect>::const_iterator it = interconnect_physical_types_.find(pb_interconnect);
  if (it == interconnect_physical_types_.end()) {
    /* Return an invalid interconnect type */
    return NUM_INTERC_TYPES;
  }
  return interconnect_physical_types_.at(pb_interconnect);
}

CircuitPortId VprPbTypeAnnotation::pb_circuit_port(t_port* pb_port) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_port*, CircuitPortId>::const_iterator it = pb_circuit_ports_.find(pb_port);
  if (it == pb_circuit_ports_.end()) {
    /* Return an invalid circuit port id */
    return CircuitPortId::INVALID();
  }
  return pb_circuit_ports_.at(pb_port);
}

std::vector<size_t> VprPbTypeAnnotation::pb_type_mode_bits(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, std::vector<size_t>>::const_iterator it = pb_type_mode_bits_.find(pb_type);
  if (it == pb_type_mode_bits_.end()) {
    /* Return an empty vector */
    return std::vector<size_t>();
  }
  return pb_type_mode_bits_.at(pb_type);
}

PbGraphNodeId VprPbTypeAnnotation::pb_graph_node_unique_index(t_pb_graph_node* pb_graph_node) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, std::vector<t_pb_graph_node*>>::const_iterator it = pb_graph_node_unique_index_.find(pb_graph_node->pb_type);
  if (it == pb_graph_node_unique_index_.end()) {
    /* Invalid pb_type, return a null pointer */
    return PbGraphNodeId::INVALID();
  }

  /* Try to find the pb_graph_node in the vector */
  std::vector<t_pb_graph_node*>::const_iterator it_node = std::find(pb_graph_node_unique_index_.at(pb_graph_node->pb_type).begin(),
                                                                    pb_graph_node_unique_index_.at(pb_graph_node->pb_type).end(),
                                                                    pb_graph_node);
  /* If it exists, return the index
   * Otherwise, return an invalid id
   */
  if (it_node == pb_graph_node_unique_index_.at(pb_graph_node->pb_type).end()) {
    return PbGraphNodeId::INVALID();
  }
  return PbGraphNodeId(it_node - pb_graph_node_unique_index_.at(pb_graph_node->pb_type).begin());
}

t_pb_graph_node* VprPbTypeAnnotation::pb_graph_node(t_pb_type* pb_type, const PbGraphNodeId& unique_index) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, std::vector<t_pb_graph_node*>>::const_iterator it = pb_graph_node_unique_index_.find(pb_type);
  if (it == pb_graph_node_unique_index_.end()) {
    /* Invalid pb_type, return a null pointer */
    return nullptr;
  }
  /* Check if the unique index is in the range:
   *  - Out of range: return a null pointer
   *  - In range: return the pointer
   */
  if ((size_t)unique_index > pb_graph_node_unique_index_.at(pb_type).size() - 1) {
    return nullptr;
  }

  return pb_graph_node_unique_index_.at(pb_type)[size_t(unique_index)];
}

t_pb_graph_node* VprPbTypeAnnotation::physical_pb_graph_node(t_pb_graph_node* pb_graph_node) const {
  /* Ensure that the pb_graph_node is in the list */
  std::map<t_pb_graph_node*, t_pb_graph_node*>::const_iterator it = physical_pb_graph_nodes_.find(pb_graph_node);
  if (it == physical_pb_graph_nodes_.end()) {
    return nullptr;
  }
  return physical_pb_graph_nodes_.at(pb_graph_node);
}

int VprPbTypeAnnotation::physical_pb_type_index_factor(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, int>::const_iterator it = physical_pb_type_index_factors_.find(pb_type);
  if (it == physical_pb_type_index_factors_.end()) {
    /* Default value is 1 */
    return 1;
  }
  return physical_pb_type_index_factors_.at(pb_type);
}

int VprPbTypeAnnotation::physical_pb_type_index_offset(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, int>::const_iterator it = physical_pb_type_index_offsets_.find(pb_type);
  if (it == physical_pb_type_index_offsets_.end()) {
    /* Default value is 0 */
    return 0;
  }
  return physical_pb_type_index_offsets_.at(pb_type);
}

int VprPbTypeAnnotation::physical_pb_pin_rotate_offset(t_port* pb_port) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_port*, int>::const_iterator it = physical_pb_pin_rotate_offsets_.find(pb_port);
  if (it == physical_pb_pin_rotate_offsets_.end()) {
    /* Default value is 0 */
    return 0;
  }
  return physical_pb_pin_rotate_offsets_.at(pb_port);
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprPbTypeAnnotation::add_pb_type_physical_mode(t_pb_type* pb_type, t_mode* physical_mode) {
  /* Warn any override attempt */
  std::map<t_pb_type*, t_mode*>::const_iterator it = physical_pb_modes_.find(pb_type);
  if (it != physical_pb_modes_.end()) {
    VTR_LOG_WARN("Override the annotation between pb_type '%s' and it physical mode '%s'!\n",
                 pb_type->name, physical_mode->name);
  }

  physical_pb_modes_[pb_type] = physical_mode;
}

void VprPbTypeAnnotation::add_physical_pb_type(t_pb_type* operating_pb_type, t_pb_type* physical_pb_type) {
  /* Warn any override attempt */
  std::map<t_pb_type*, t_pb_type*>::const_iterator it = physical_pb_types_.find(operating_pb_type);
  if (it != physical_pb_types_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_type '%s' and it physical pb_type '%s'!\n",
                 operating_pb_type->name, physical_pb_type->name);
  }

  physical_pb_types_[operating_pb_type] = physical_pb_type;
}

void VprPbTypeAnnotation::add_physical_pb_port(t_port* operating_pb_port, t_port* physical_pb_port) {
  /* Warn any override attempt */
  std::map<t_port*, t_port*>::const_iterator it = physical_pb_ports_.find(operating_pb_port);
  if (it != physical_pb_ports_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_port '%s' and it physical pb_port '%s'!\n",
                 operating_pb_port->name, physical_pb_port->name);
  }

  physical_pb_ports_[operating_pb_port] = physical_pb_port;
}

void VprPbTypeAnnotation::add_physical_pb_port_range(t_port* operating_pb_port, const BasicPort& port_range) {
  /* The port range must satify the port width*/
  VTR_ASSERT((size_t)operating_pb_port->num_pins == port_range.get_width());

  /* Warn any override attempt */
  std::map<t_port*, BasicPort>::const_iterator it = physical_pb_port_ranges_.find(operating_pb_port);
  if (it != physical_pb_port_ranges_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_port '%s' and it physical pb_port range '[%ld:%ld]'!\n",
                 operating_pb_port->name, port_range.get_lsb(), port_range.get_msb());
  }

  physical_pb_port_ranges_[operating_pb_port] = port_range;
}

void VprPbTypeAnnotation::add_pb_type_circuit_model(t_pb_type* physical_pb_type, const CircuitModelId& circuit_model) {
  /* Warn any override attempt */
  std::map<t_pb_type*, CircuitModelId>::const_iterator it = pb_type_circuit_models_.find(physical_pb_type);
  if (it != pb_type_circuit_models_.end()) {
    VTR_LOG_WARN("Override the circuit model for physical pb_type '%s'!\n",
                 physical_pb_type->name);
  }

  pb_type_circuit_models_[physical_pb_type] = circuit_model;
}

void VprPbTypeAnnotation::add_interconnect_circuit_model(t_interconnect* pb_interconnect, const CircuitModelId& circuit_model) {
  /* Warn any override attempt */
  std::map<t_interconnect*, CircuitModelId>::const_iterator it = interconnect_circuit_models_.find(pb_interconnect);
  if (it != interconnect_circuit_models_.end()) {
    VTR_LOG_WARN("Override the circuit model for interconnect '%s'!\n",
                 pb_interconnect->name);
  }

  interconnect_circuit_models_[pb_interconnect] = circuit_model;
}

void VprPbTypeAnnotation::add_interconnect_physical_type(t_interconnect* pb_interconnect,
                                                         const e_interconnect& physical_type) {
  /* Warn any override attempt */
  std::map<t_interconnect*, e_interconnect>::const_iterator it = interconnect_physical_types_.find(pb_interconnect);
  if (it != interconnect_physical_types_.end()) {
    VTR_LOG_WARN("Override the physical interconnect for interconnect '%s'!\n",
                 pb_interconnect->name);
  }

  interconnect_physical_types_[pb_interconnect] = physical_type;
}

void VprPbTypeAnnotation::add_pb_circuit_port(t_port* pb_port, const CircuitPortId& circuit_port) {
  /* Warn any override attempt */
  std::map<t_port*, CircuitPortId>::const_iterator it = pb_circuit_ports_.find(pb_port);
  if (it != pb_circuit_ports_.end()) {
    VTR_LOG_WARN("Override the circuit port mapping for pb_type port '%s'!\n",
                 pb_port->name);
  }

  pb_circuit_ports_[pb_port] = circuit_port;
}

void VprPbTypeAnnotation::add_pb_type_mode_bits(t_pb_type* pb_type, const std::vector<size_t>& mode_bits) {
  /* Warn any override attempt */
  std::map<t_pb_type*, std::vector<size_t>>::const_iterator it = pb_type_mode_bits_.find(pb_type);
  if (it != pb_type_mode_bits_.end()) {
    VTR_LOG_WARN("Override the mode bits mapping for pb_type '%s'!\n",
                 pb_type->name);
  }

  pb_type_mode_bits_[pb_type] = mode_bits;
}

void VprPbTypeAnnotation::add_pb_graph_node_unique_index(t_pb_graph_node* pb_graph_node) {
  pb_graph_node_unique_index_[pb_graph_node->pb_type].push_back(pb_graph_node);
}

void VprPbTypeAnnotation::add_physical_pb_graph_node(t_pb_graph_node* operating_pb_graph_node, 
                                                     t_pb_graph_node* physical_pb_graph_node) {
  /* Warn any override attempt */
  std::map<t_pb_graph_node*, t_pb_graph_node*>::const_iterator it = physical_pb_graph_nodes_.find(operating_pb_graph_node);
  if (it != physical_pb_graph_nodes_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_graph_node '%s[%d]' and it physical pb_graph_node '%s[%d]'!\n",
                 operating_pb_graph_node->pb_type->name, 
                 operating_pb_graph_node->placement_index,
                 physical_pb_graph_node->pb_type->name, 
                 physical_pb_graph_node->placement_index);
  }

  physical_pb_graph_nodes_[operating_pb_graph_node] = physical_pb_graph_node;
}

void VprPbTypeAnnotation::add_physical_pb_type_index_factor(t_pb_type* pb_type, const int& factor) {
  /* Warn any override attempt */
  std::map<t_pb_type*, int>::const_iterator it = physical_pb_type_index_factors_.find(pb_type);
  if (it != physical_pb_type_index_factors_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_type '%s' and it physical pb_type index factor '%d'!\n",
                 pb_type->name, factor);
  }

  physical_pb_type_index_factors_[pb_type] = factor;
}

void VprPbTypeAnnotation::add_physical_pb_type_index_offset(t_pb_type* pb_type, const int& offset) {
  /* Warn any override attempt */
  std::map<t_pb_type*, int>::const_iterator it = physical_pb_type_index_offsets_.find(pb_type);
  if (it != physical_pb_type_index_offsets_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_type '%s' and it physical pb_type index offset '%d'!\n",
                 pb_type->name, offset);
  }

  physical_pb_type_index_offsets_[pb_type] = offset;
}

void VprPbTypeAnnotation::add_physical_pb_pin_rotate_offset(t_port* pb_port, const int& offset) {
  /* Warn any override attempt */
  std::map<t_port*, int>::const_iterator it = physical_pb_pin_rotate_offsets_.find(pb_port);
  if (it != physical_pb_pin_rotate_offsets_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_port '%s' and it physical pb_port pin rotate offset '%d'!\n",
                 pb_port->name, offset);
  }

  physical_pb_pin_rotate_offsets_[pb_port] = offset;
}

} /* End namespace openfpga*/
