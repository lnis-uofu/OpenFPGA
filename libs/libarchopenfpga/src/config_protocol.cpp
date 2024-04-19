#include "config_protocol.h"

#include "openfpga_tokenizer.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/************************************************************************
 * Member functions for class ConfigProtocol
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
ConfigProtocol::ConfigProtocol() { INDICE_STRING_DELIM_ = ','; }

/************************************************************************
 * Public Accessors
 ***********************************************************************/
e_config_protocol_type ConfigProtocol::type() const { return type_; }

std::string ConfigProtocol::memory_model_name() const {
  return memory_model_name_;
}

CircuitModelId ConfigProtocol::memory_model() const { return memory_model_; }

int ConfigProtocol::num_regions() const { return num_regions_; }

size_t ConfigProtocol::num_prog_clocks() const {
  if (type_ != CONFIG_MEM_SCAN_CHAIN) {
    return 1;
  }
  if (prog_clk_port_.is_valid()) {
    return prog_clk_port_.get_width();
  }
  return 1;
}

openfpga::BasicPort ConfigProtocol::prog_clock_port_info() const {
  VTR_ASSERT(type_ == CONFIG_MEM_SCAN_CHAIN);
  return prog_clk_port_;
}

std::vector<openfpga::BasicPort> ConfigProtocol::prog_clock_pins() const {
  VTR_ASSERT(type_ == CONFIG_MEM_SCAN_CHAIN);
  std::vector<openfpga::BasicPort> keys;
  for (auto pin : prog_clk_port_.pins()) {
    keys.push_back(openfpga::BasicPort(prog_clk_port_.get_name(), pin, pin));
  }
  return keys;
}

std::string ConfigProtocol::prog_clock_pin_ccff_head_indices_str(
  const openfpga::BasicPort& port) const {
  VTR_ASSERT(type_ == CONFIG_MEM_SCAN_CHAIN);
  std::string ret("");
  std::vector<size_t> raw = prog_clock_pin_ccff_head_indices(port);
  if (!raw.empty()) {
    for (size_t idx : raw) {
      /* TODO: We need a join function */
      ret += std::to_string(idx) + std::to_string(INDICE_STRING_DELIM_);
    }
    /* Remove the last comma */
    ret.pop_back();
  }
  return ret;
}

std::vector<size_t> ConfigProtocol::prog_clock_pin_ccff_head_indices(
  const openfpga::BasicPort& port) const {
  VTR_ASSERT(type_ == CONFIG_MEM_SCAN_CHAIN);
  std::vector<size_t> ret;
  if (port.get_width() != 1) {
    VTR_LOG_ERROR(
      "The programming clock pin must have a width of 1 while the width "
      "specified is %ld!\n",
      port.get_width());
  }
  VTR_ASSERT(port.get_width() == 1);
  if (!prog_clk_port_.contained(port)) {
    VTR_LOG_ERROR(
      "The programming clock pin '%s[%ld]' is not out of the range [%ld, "
      "%ld]!\n",
      port.get_name().c_str(), port.get_lsb(), prog_clk_port_.get_lsb(),
      prog_clk_port_.get_msb());
  }
  VTR_ASSERT(prog_clk_port_.contained(port));
  return prog_clk_ccff_head_indices_[port.get_lsb()];
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

const QLMemoryBankConfigSetting* ConfigProtocol::ql_memory_bank_config_setting()
  const {
  return &ql_memory_bank_config_setting_;
}

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

void ConfigProtocol::set_prog_clock_port(const openfpga::BasicPort& port) {
  prog_clk_port_ = port;
  prog_clk_ccff_head_indices_.resize(prog_clk_port_.get_width());
}

void ConfigProtocol::set_prog_clock_pin_ccff_head_indices_pair(
  const openfpga::BasicPort& port, const std::string& indices_str) {
  openfpga::StringToken tokenizer(indices_str);
  std::vector<size_t> token_int;
  token_int.reserve(tokenizer.split(INDICE_STRING_DELIM_).size());
  for (std::string token : tokenizer.split(INDICE_STRING_DELIM_)) {
    token_int.push_back(std::stoi(token));
  }
  if (port.get_width() != 1) {
    VTR_LOG_ERROR(
      "The programming clock pin must have a width of 1 while the width "
      "specified is %ld!\n",
      port.get_width());
  }
  VTR_ASSERT(port.get_width() == 1);
  if (!prog_clk_port_.contained(port)) {
    VTR_LOG_ERROR(
      "The programming clock pin '%s[%ld]' is not out of the range [%ld, "
      "%ld]!\n",
      port.get_name().c_str(), port.get_lsb(), prog_clk_port_.get_lsb(),
      prog_clk_port_.get_msb());
  }
  VTR_ASSERT(prog_clk_port_.contained(port));
  if (!prog_clk_ccff_head_indices_[port.get_lsb()].empty()) {
    VTR_LOG_WARN(
      "Overwrite the pair between programming clock port '%s[%d:%d]' and ccff "
      "head indices (previous: '%s', current: '%s')!\n",
      port.get_name().c_str(), port.get_lsb(), port.get_msb(),
      prog_clock_pin_ccff_head_indices_str(port).c_str(), indices_str.c_str());
  }
  prog_clk_ccff_head_indices_[port.get_lsb()] = token_int;
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

QLMemoryBankConfigSetting* ConfigProtocol::get_ql_memory_bank_config_setting() {
  return &ql_memory_bank_config_setting_;
}

/************************************************************************
 * Private Validators
 ***********************************************************************/
int ConfigProtocol::validate_ccff_prog_clocks() const {
  int num_err = 0;
  if (prog_clock_pins().empty()) {
    return num_err;
  }
  /* Initialize scoreboard */
  std::vector<int> ccff_head_scoreboard(num_regions(), 0);
  for (openfpga::BasicPort port : prog_clock_pins()) {
    /* Must be valid first */
    if (!port.is_valid()) {
      VTR_LOG_ERROR("Programming clock '%s[%d:%d]' is not a valid port!\n",
                    port.get_name().c_str(), port.get_lsb(), port.get_msb());
      num_err++;
    }
    /* Each port should have a width of 1 */
    if (port.get_width() != 1) {
      VTR_LOG_ERROR(
        "Expect each programming clock has a size of 1 in the definition. "
        "'%s[%d:%d]' violates the rule!\n",
        port.get_name().c_str(), port.get_lsb(), port.get_msb());
      num_err++;
    }
    /* Fill scoreboard */
    for (size_t ccff_head_idx : prog_clock_pin_ccff_head_indices(port)) {
      if (ccff_head_idx >= ccff_head_scoreboard.size()) {
        VTR_LOG_ERROR(
          "Programming clock '%s[%d:%d]' controlls an invalid ccff head '%ld' "
          "(Expect [0, '%ld'])!\n",
          port.get_name().c_str(), port.get_lsb(), port.get_msb(),
          ccff_head_idx, ccff_head_scoreboard.size() - 1);
        num_err++;
      }
      ccff_head_scoreboard[ccff_head_idx]++;
    }
  }
  if (prog_clock_pins().size() > (size_t)num_regions()) {
    VTR_LOG_ERROR(
      "Number of programming clocks '%ld' is more than the number of "
      "configuration regions '%ld'!\n",
      prog_clock_pins().size(), num_regions());
    num_err++;
  }
  for (size_t iregion = 0; iregion < ccff_head_scoreboard.size(); iregion++) {
    if (ccff_head_scoreboard[iregion] == 0) {
      VTR_LOG_ERROR(
        "Configuration chain '%ld' is not driven by any programming clock!\n",
        iregion);
      num_err++;
    }
    if (ccff_head_scoreboard[iregion] > 1) {
      VTR_LOG_ERROR(
        "Configuration chain '%ld' is driven by %ld programming clock!\n",
        iregion, ccff_head_scoreboard[iregion]);
      num_err++;
    }
  }
  return num_err;
}

/************************************************************************
 * Public Validators
 ***********************************************************************/
int ConfigProtocol::validate() const {
  int num_err = 0;
  if (type() == CONFIG_MEM_SCAN_CHAIN) {
    num_err += validate_ccff_prog_clocks();
  }
  return num_err;
}
