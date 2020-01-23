#include "vtr_assert.h"

#include "config_protocol.h"

/************************************************************************
 * Member functions for class ConfigProtocol
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
ConfigProtocol::ConfigProtocol() {
  return;
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
e_config_protocol_type ConfigProtocol::type() const {
  return type_;
}

std::string ConfigProtocol::memory_model_name() const {
  return memory_model_name_;
}

CircuitModelId ConfigProtocol::memory_model() const {
  return memory_model_;
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void ConfigProtocol::set_type(const e_config_protocol_type& type) {
  type_ = type;
}

void ConfigProtocol::set_memory_model_name(const std::string& memory_model_name) {
  memory_model_name_ = memory_model_name;
}

void ConfigProtocol::set_memory_model(const CircuitModelId& memory_model) {
  memory_model_ = memory_model;
}
