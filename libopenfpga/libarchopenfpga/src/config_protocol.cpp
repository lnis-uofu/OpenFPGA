#include "vtr_assert.h"
#include "vtr_log.h"

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

int ConfigProtocol::num_regions() const {
  return num_regions_;
}

e_blwl_protocol_type ConfigProtocol::bl_protocol_type() const {
  return bl_protocol_type_;
}

std::string ConfigProtocol::bl_memory_model_name() const {
  return bl_memory_model_name_;
}

CircuitModelId ConfigProtocol::bl_memory_model() const {
  return bl_memory_model_;
}

size_t ConfigProtocol::bl_num_banks() const {
  return bl_num_banks_;
}

e_blwl_protocol_type ConfigProtocol::wl_protocol_type() const {
  return wl_protocol_type_;
}

std::string ConfigProtocol::wl_memory_model_name() const {
  return wl_memory_model_name_;
}

CircuitModelId ConfigProtocol::wl_memory_model() const {
  return wl_memory_model_;
}

size_t ConfigProtocol::wl_num_banks() const {
  return wl_num_banks_;
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

void ConfigProtocol::set_num_regions(const int& num_regions) {
  num_regions_ = num_regions;
}

void ConfigProtocol::set_bl_protocol_type(const e_blwl_protocol_type& type) {
  if (CONFIG_MEM_QL_MEMORY_BANK != type_) {
    VTR_LOG_ERROR("BL protocol type is only applicable for configuration protocol '%d'", CONFIG_PROTOCOL_TYPE_STRING[type_]);
    return;
  }
  bl_protocol_type_ = type;
}

void ConfigProtocol::set_bl_memory_model_name(const std::string& memory_model_name) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != bl_protocol_type_) {
    VTR_LOG_ERROR("BL protocol memory model is only applicable when '%d' is defined", BLWL_PROTOCOL_TYPE_STRING[bl_protocol_type_]);
    return;
  }
  bl_memory_model_name_ = memory_model_name;
}

void ConfigProtocol::set_bl_memory_model(const CircuitModelId& memory_model) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != bl_protocol_type_) {
    VTR_LOG_ERROR("BL protocol memory model is only applicable when '%d' is defined", BLWL_PROTOCOL_TYPE_STRING[bl_protocol_type_]);
    return;
  }
  bl_memory_model_ = memory_model;
}

void ConfigProtocol::set_bl_num_banks(const size_t& num_banks) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != bl_protocol_type_) {
    VTR_LOG_ERROR("BL protocol memory model is only applicable when '%d' is defined", BLWL_PROTOCOL_TYPE_STRING[bl_protocol_type_]);
    return;
  }
  bl_num_banks_ = num_banks;
}


void ConfigProtocol::set_wl_protocol_type(const e_blwl_protocol_type& type) {
  if (CONFIG_MEM_QL_MEMORY_BANK != type_) {
    VTR_LOG_ERROR("WL protocol type is only applicable for configuration protocol '%d'", CONFIG_PROTOCOL_TYPE_STRING[type_]);
    return;
  }
  wl_protocol_type_ = type;
}

void ConfigProtocol::set_wl_memory_model_name(const std::string& memory_model_name) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != wl_protocol_type_) {
    VTR_LOG_ERROR("WL protocol memory model is only applicable when '%d' is defined", BLWL_PROTOCOL_TYPE_STRING[wl_protocol_type_]);
    return;
  }
  wl_memory_model_name_ = memory_model_name;
}

void ConfigProtocol::set_wl_memory_model(const CircuitModelId& memory_model) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != wl_protocol_type_) {
    VTR_LOG_ERROR("WL protocol memory model is only applicable when '%d' is defined", BLWL_PROTOCOL_TYPE_STRING[wl_protocol_type_]);
    return;
  }
  wl_memory_model_ = memory_model;
}

void ConfigProtocol::set_wl_num_banks(const size_t& num_banks) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != wl_protocol_type_) {
    VTR_LOG_ERROR("WL protocol memory model is only applicable when '%d' is defined", BLWL_PROTOCOL_TYPE_STRING[wl_protocol_type_]);
    return;
  }
  wl_num_banks_ = num_banks;
}

