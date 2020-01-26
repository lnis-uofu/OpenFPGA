/************************************************************************
 * Member functions for class PbTypeAnnotation
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "pb_type_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
PbTypeAnnotation::PbTypeAnnotation() {
  return;
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
std::string PbTypeAnnotation::operating_pb_type_name() const {
  return operating_pb_type_name_;
}

std::vector<std::string> PbTypeAnnotation::operating_parent_pb_type_names() const {
  return operating_parent_pb_type_names_;
}

std::vector<std::string> PbTypeAnnotation::operating_parent_mode_names() const {
  return operating_parent_mode_names_;
}

bool PbTypeAnnotation::is_operating_pb_type() const {
  return true == operating_pb_type_name_.empty();
}


std::string PbTypeAnnotation::physical_pb_type_name() const {
  return physical_pb_type_name_;
}

std::vector<std::string> PbTypeAnnotation::physical_parent_pb_type_names() const {
  return physical_parent_pb_type_names_;
}

std::vector<std::string> PbTypeAnnotation::physical_parent_mode_names() const {
  return physical_parent_mode_names_;
}

bool PbTypeAnnotation::is_physical_pb_type() const {
  return true == physical_pb_type_name_.empty();
}

std::string PbTypeAnnotation::mode_bits() const {
  return mode_bits_;
}

std::string PbTypeAnnotation::circuit_model_name() const {
  return circuit_model_name_;
}

int PbTypeAnnotation::physical_pb_type_index_factor() const {
  return physical_pb_type_index_factor_;
}

int PbTypeAnnotation::physical_pb_type_index_offset() const {
  return physical_pb_type_index_offset_;
}

BasicPort PbTypeAnnotation::physical_pb_type_ports(const std::string& port_name) const {
  std::map<std::string, BasicPort>::const_iterator it = operating_pb_type_ports_.find(port_name);
  if (it == operating_pb_type_ports_.end()) {
    /* Return an empty port */
    return BasicPort();
  }
  return operating_pb_type_ports_.at(port_name);
}

int PbTypeAnnotation::physical_pin_rotate_offsets(const std::string& port_name) const {
  std::map<std::string, int>::const_iterator it = physical_pin_rotate_offsets_.find(port_name);
  if (it == physical_pin_rotate_offsets_.end()) {
    /* Return a zero offset which is default */
    return 0;
  }
  return physical_pin_rotate_offsets_.at(port_name);
}

std::string PbTypeAnnotation::interconnect_circuit_model_name(const std::string& interc_name) const {
  std::map<std::string, std::string>::const_iterator it = interconnect_circuit_model_names_.find(interc_name);
  if (it == interconnect_circuit_model_names_.end()) {
    return std::string();
  }

  return interconnect_circuit_model_names_.at(interc_name);
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void PbTypeAnnotation::set_operating_pb_type_name(const std::string& name) {
  operating_pb_type_name_ = name;
}

void PbTypeAnnotation::set_operating_parent_pb_type_names(const std::vector<std::string>& names) {
  operating_parent_pb_type_names_ = names;
}

void PbTypeAnnotation::set_operating_parent_mode_names(const std::vector<std::string>& names) {
  operating_parent_mode_names_ = names;
}

void PbTypeAnnotation::set_physical_pb_type_name(const std::string& name) {
  physical_pb_type_name_ = name;
}

void PbTypeAnnotation::set_physical_parent_pb_type_names(const std::vector<std::string>& names) {
  physical_parent_pb_type_names_ = names;
}

void PbTypeAnnotation::set_physical_parent_mode_names(const std::vector<std::string>& names) {
  physical_parent_mode_names_ = names;
}

void PbTypeAnnotation::set_mode_bits(const std::string& mode_bits) {
  mode_bits_ = mode_bits;
}

void PbTypeAnnotation::set_circuit_model_name(const std::string& name) {
  VTR_ASSERT(true == is_physical_pb_type());
  circuit_model_name_ = name;
}

void PbTypeAnnotation::physical_pb_type_index_factor(const int& value) {
  VTR_ASSERT(true == is_operating_pb_type());
  physical_pb_type_index_factor_ = value;
}

void PbTypeAnnotation::physical_pb_type_index_offset(const int& value) {
  VTR_ASSERT(true == is_operating_pb_type());
  physical_pb_type_index_offset_ = value;
}

void PbTypeAnnotation::add_pb_type_port_pair(const std::string& operating_pb_port_name,
                                             const BasicPort& physical_pb_port) {
  /* Give a warning if the operating_pb_port_name already exist */
  std::map<std::string, BasicPort>::const_iterator it = operating_pb_type_ports_.find(operating_pb_port_name);
  /* Give a warning if the interconnection name already exist */
  if (it != operating_pb_type_ports_.end()) {
    VTR_LOG_WARN("Redefine operating pb type port '%s' with physical pb type port '%s'\n",
                  operating_pb_port_name.c_str(), physical_pb_port.get_name().c_str());
  }

  operating_pb_type_ports_[operating_pb_port_name] = physical_pb_port;
}

void PbTypeAnnotation::set_physical_pin_rotate_offset(const std::string& operating_pb_port_name,
                                                      const int& physical_pin_rotate_offset) {
  std::map<std::string, BasicPort>::const_iterator it = operating_pb_type_ports_.find(operating_pb_port_name);
  /* Give a warning if the interconnection name already exist */
  if (it == operating_pb_type_ports_.end()) {
    VTR_LOG_ERROR("Operating pb type port '%s' does not exist! Ignore physical pin rotate offset '%d'\n",
                  operating_pb_port_name.c_str(), physical_pin_rotate_offset);
    return;
  }

  physical_pin_rotate_offsets_[operating_pb_port_name] = physical_pin_rotate_offset;
}

void PbTypeAnnotation::add_interconnect_circuit_model_pair(const std::string& interc_name,
                                                           const std::string& circuit_model_name) {
  std::map<std::string, std::string>::const_iterator it = interconnect_circuit_model_names_.find(interc_name);
  /* Give a warning if the interconnection name already exist */
  if (it != interconnect_circuit_model_names_.end()) {
    VTR_LOG_WARN("Redefine interconnect '%s' with circuit model '%s'\n",
                 interc_name.c_str(), circuit_model_name.c_str());
  }

  interconnect_circuit_model_names_[interc_name] = circuit_model_name;
}

} /* namespace openfpga ends */
