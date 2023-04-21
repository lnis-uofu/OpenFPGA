#include "config_protocol.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/************************************************************************
 * Member functions for class ConfigProtocol
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
ConfigProtocol::ConfigProtocol() {
  INDICE_STRING_DELIM_ = ',';
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
e_config_protocol_type ConfigProtocol::type() const { return type_; }

std::string ConfigProtocol::memory_model_name() const {
  return memory_model_name_;
}

CircuitModelId ConfigProtocol::memory_model() const { return memory_model_; }

int ConfigProtocol::num_regions() const { return num_regions_; }

std::vector<BasicPort> ConfigProtocol::prog_clock_ports() const {
  std::vector<BasicPort> keys;
  for (const auto& [k, v] : prog_clk_ccff_head_indices_) {
    keys.push_back(k);
  }
  return keys;
}

std::string ConfigProtocol::prog_clock_port_ccff_head_indices(const BasicPort& port) const {
  std::string ret("");
  auto result = prog_clk_ccff_head_indices.find(port);
  if (result != prog_clk_ccff_head_indices.end()) {
    for (size_t idx : result->second) {
      /* TODO: We need a join function */
      ret += std::to_string(idx) + std::string(INDICE_STRING_DELIM);
    }
    /* Remove the last comma */
    ret.pop();
  }
  return ret;
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

size_t ConfigProtocol::bl_num_banks() const { return bl_num_banks_; }

e_blwl_protocol_type ConfigProtocol::wl_protocol_type() const {
  return wl_protocol_type_;
}

std::string ConfigProtocol::wl_memory_model_name() const {
  return wl_memory_model_name_;
}

CircuitModelId ConfigProtocol::wl_memory_model() const {
  return wl_memory_model_;
}

size_t ConfigProtocol::wl_num_banks() const { return wl_num_banks_; }

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void ConfigProtocol::set_type(const e_config_protocol_type& type) {
  type_ = type;
}

void ConfigProtocol::set_memory_model_name(
  const std::string& memory_model_name) {
  memory_model_name_ = memory_model_name;
}

void ConfigProtocol::set_memory_model(const CircuitModelId& memory_model) {
  memory_model_ = memory_model;
}

void ConfigProtocol::set_num_regions(const int& num_regions) {
  num_regions_ = num_regions;
}

void ConfigProtocol::set_prog_clock_port_ccff_head_indices_pair(const BasicPort& port, const std::string& indices_str) {
  openfpga::StringToken tokenizer(indices_str);
  std::vector<int> token_int;
  token_int.reserve(tokenizer.split(INDICE_STRING_DELIM_).size());
  for (std::string token : tokenizer.split(INDICE_STRING_DELIM_)) {
    token_int.push_back(std::atoi(token));
  }
  auto result = prog_clk_ccff_head_indices.find(port);
  if (result != prog_clk_ccff_head_indices.end()) {
    VTR_LOG_WARN("Overwrite the pair between programming clock port '%s[%d:%d]' and ccff head indices (previous: '%s', current: '%s')!\n", port.get_name().c_str(), port.get_lsb(), port.get_msb(), prog_clock_port_ccff_head_indices(port).c_str(), indices_str.c_str());
  }
  prog_clk_ccff_head_indices_[port] = token_int;
}

void ConfigProtocol::set_bl_protocol_type(const e_blwl_protocol_type& type) {
  if (CONFIG_MEM_QL_MEMORY_BANK != type_) {
    VTR_LOG_ERROR(
      "BL protocol type is only applicable for configuration protocol '%d'",
      CONFIG_PROTOCOL_TYPE_STRING[type_]);
    return;
  }
  bl_protocol_type_ = type;
}

void ConfigProtocol::set_bl_memory_model_name(
  const std::string& memory_model_name) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != bl_protocol_type_) {
    VTR_LOG_ERROR(
      "BL protocol memory model is only applicable when '%d' is defined",
      BLWL_PROTOCOL_TYPE_STRING[bl_protocol_type_]);
    return;
  }
  bl_memory_model_name_ = memory_model_name;
}

void ConfigProtocol::set_bl_memory_model(const CircuitModelId& memory_model) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != bl_protocol_type_) {
    VTR_LOG_ERROR(
      "BL protocol memory model is only applicable when '%d' is defined",
      BLWL_PROTOCOL_TYPE_STRING[bl_protocol_type_]);
    return;
  }
  bl_memory_model_ = memory_model;
}

void ConfigProtocol::set_bl_num_banks(const size_t& num_banks) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != bl_protocol_type_) {
    VTR_LOG_ERROR(
      "BL protocol memory model is only applicable when '%d' is defined",
      BLWL_PROTOCOL_TYPE_STRING[bl_protocol_type_]);
    return;
  }
  bl_num_banks_ = num_banks;
}

void ConfigProtocol::set_wl_protocol_type(const e_blwl_protocol_type& type) {
  if (CONFIG_MEM_QL_MEMORY_BANK != type_) {
    VTR_LOG_ERROR(
      "WL protocol type is only applicable for configuration protocol '%d'",
      CONFIG_PROTOCOL_TYPE_STRING[type_]);
    return;
  }
  wl_protocol_type_ = type;
}

void ConfigProtocol::set_wl_memory_model_name(
  const std::string& memory_model_name) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != wl_protocol_type_) {
    VTR_LOG_ERROR(
      "WL protocol memory model is only applicable when '%d' is defined",
      BLWL_PROTOCOL_TYPE_STRING[wl_protocol_type_]);
    return;
  }
  wl_memory_model_name_ = memory_model_name;
}

void ConfigProtocol::set_wl_memory_model(const CircuitModelId& memory_model) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != wl_protocol_type_) {
    VTR_LOG_ERROR(
      "WL protocol memory model is only applicable when '%d' is defined",
      BLWL_PROTOCOL_TYPE_STRING[wl_protocol_type_]);
    return;
  }
  wl_memory_model_ = memory_model;
}

void ConfigProtocol::set_wl_num_banks(const size_t& num_banks) {
  if (BLWL_PROTOCOL_SHIFT_REGISTER != wl_protocol_type_) {
    VTR_LOG_ERROR(
      "WL protocol memory model is only applicable when '%d' is defined",
      BLWL_PROTOCOL_TYPE_STRING[wl_protocol_type_]);
    return;
  }
  wl_num_banks_ = num_banks;
}
