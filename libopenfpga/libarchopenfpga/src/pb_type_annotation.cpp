/************************************************************************
 * Member functions for class PbTypeAnnotation
 ***********************************************************************/
#include <algorithm>
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
  return (false == operating_pb_type_name_.empty())
      && (false == physical_pb_type_name_.empty());
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
  return (true == operating_pb_type_name_.empty())
      && (false == physical_pb_type_name_.empty());
}

std::string PbTypeAnnotation::physical_mode_name() const {
  return physical_mode_name_;
}

std::string PbTypeAnnotation::idle_mode_name() const {
  return idle_mode_name_;
}

std::vector<size_t> PbTypeAnnotation::mode_bits() const {
  return mode_bits_;
}

std::string PbTypeAnnotation::circuit_model_name() const {
  return circuit_model_name_;
}

float PbTypeAnnotation::physical_pb_type_index_factor() const {
  return physical_pb_type_index_factor_;
}

int PbTypeAnnotation::physical_pb_type_index_offset() const {
  return physical_pb_type_index_offset_;
}

std::vector<std::string> PbTypeAnnotation::port_names() const {
  std::vector<std::string> keys;
  for (auto const& element : operating_pb_type_ports_) {
    keys.push_back(element.first);
  }
  return keys;
}

std::map<BasicPort, int> PbTypeAnnotation::physical_pb_type_port(const std::string& port_name) const {
  std::map<std::string, std::map<BasicPort, int>>::const_iterator it = operating_pb_type_ports_.find(port_name);
  if (it == operating_pb_type_ports_.end()) {
    /* Return an empty port */
    return std::map<BasicPort, int>();
  }
  return operating_pb_type_ports_.at(port_name);
}

std::vector<std::string> PbTypeAnnotation::interconnect_names() const {
  std::vector<std::string> keys;
  for (auto const& element : interconnect_circuit_model_names_) {
    keys.push_back(element.first);
  }
  return keys;
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

void PbTypeAnnotation::set_physical_mode_name(const std::string& name) {
  physical_mode_name_ = name;
}

void PbTypeAnnotation::set_idle_mode_name(const std::string& name) {
  idle_mode_name_ = name;
}

void PbTypeAnnotation::set_mode_bits(const std::vector<size_t>& mode_bits) {
  mode_bits_ = mode_bits;
}

void PbTypeAnnotation::set_circuit_model_name(const std::string& name) {
  VTR_ASSERT(true == is_physical_pb_type());
  circuit_model_name_ = name;
}

void PbTypeAnnotation::set_physical_pb_type_index_factor(const float& value) {
  VTR_ASSERT(true == is_operating_pb_type());
  physical_pb_type_index_factor_ = value;
}

void PbTypeAnnotation::set_physical_pb_type_index_offset(const int& value) {
  VTR_ASSERT(true == is_operating_pb_type());
  physical_pb_type_index_offset_ = value;
}

void PbTypeAnnotation::add_pb_type_port_pair(const std::string& operating_pb_port_name,
                                             const BasicPort& physical_pb_port) {
  /* Give a warning if the operating_pb_port_name already exist */
  std::map<std::string, std::map<BasicPort, int>>::const_iterator it = operating_pb_type_ports_.find(operating_pb_port_name);

  /* If not exist, initialize and set a default value */
  if (it == operating_pb_type_ports_.end()) {
    operating_pb_type_ports_[operating_pb_port_name][physical_pb_port] = 0;
    /* We can return early */
    return;
  }

  /* If the physical port is not in the list, we create one and set a default value */
  if (0 == operating_pb_type_ports_[operating_pb_port_name].count(physical_pb_port)) {
    operating_pb_type_ports_[operating_pb_port_name][physical_pb_port] = 0;
  }
}

void PbTypeAnnotation::set_physical_pin_rotate_offset(const std::string& operating_pb_port_name,
                                                      const BasicPort& physical_pb_port,
                                                      const int& physical_pin_rotate_offset) {
  std::map<std::string, std::map<BasicPort, int>>::const_iterator it = operating_pb_type_ports_.find(operating_pb_port_name);

  if (it == operating_pb_type_ports_.end()) {
    VTR_LOG_ERROR("The operating pb_type port '%s' is not valid!\n",
                  operating_pb_port_name.c_str());
    exit(1);
  }

  if (operating_pb_type_ports_[operating_pb_port_name].end() == operating_pb_type_ports_[operating_pb_port_name].find(physical_pb_port)) {
    VTR_LOG_ERROR("The physical pb_type port '%s[%lu:%lu]' definition for operating pb_type port '%s' is not valid!\n",
                  physical_pb_port.get_name().c_str(),
                  physical_pb_port.get_lsb(),
                  physical_pb_port.get_msb(),
                  operating_pb_port_name.c_str());
    exit(1);
  }

  operating_pb_type_ports_[operating_pb_port_name][physical_pb_port] = physical_pin_rotate_offset;
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
