/******************************************************************************
 * Memember functions for data structure FabricVerilogOption
 ******************************************************************************/
#include "fabric_verilog_options.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
FabricVerilogOption::FabricVerilogOption() {
  output_directory_.clear();
  include_timing_ = false;
  explicit_port_mapping_ = false;
  compress_routing_ = false;
  print_user_defined_template_ = false;
  default_net_type_ = VERILOG_DEFAULT_NET_TYPE_NONE;
  time_stamp_ = true;
  use_relative_path_ = false;
  constant_undriven_inputs_ = FabricVerilogOption::e_undriven_input_type::NONE;
  CONSTANT_UNDRIVEN_INPUT_TYPE_STRING_ = {"none", "bus0", "bus1", "bit0",
                                          "bit1"};
  little_endian_ = false;
  verbose_output_ = false;
}

/**************************************************
 * Public Accessors
 *************************************************/
std::string FabricVerilogOption::output_directory() const {
  return output_directory_;
}

bool FabricVerilogOption::include_timing() const { return include_timing_; }

bool FabricVerilogOption::time_stamp() const { return time_stamp_; }

bool FabricVerilogOption::use_relative_path() const {
  return use_relative_path_;
}

bool FabricVerilogOption::explicit_port_mapping() const {
  return explicit_port_mapping_;
}

bool FabricVerilogOption::compress_routing() const { return compress_routing_; }

bool FabricVerilogOption::print_user_defined_template() const {
  return print_user_defined_template_;
}

e_verilog_default_net_type FabricVerilogOption::default_net_type() const {
  return default_net_type_;
}

FabricVerilogOption::e_undriven_input_type
FabricVerilogOption::constant_undriven_inputs() const {
  return constant_undriven_inputs_;
}

bool FabricVerilogOption::constant_undriven_inputs_use_bus() const {
  return constant_undriven_inputs_ ==
           FabricVerilogOption::e_undriven_input_type::BUS0 ||
         constant_undriven_inputs_ ==
           FabricVerilogOption::e_undriven_input_type::BUS1;
}

size_t FabricVerilogOption::constant_undriven_inputs_value() const {
  if (constant_undriven_inputs_ ==
        FabricVerilogOption::e_undriven_input_type::BUS1 ||
      constant_undriven_inputs_ ==
        FabricVerilogOption::e_undriven_input_type::BIT1) {
    return 1;
  }
  return 0;
}

std::string FabricVerilogOption::full_constant_undriven_input_type_str() const {
  std::string full_type_str("[");
  for (size_t itype = 0;
       itype < size_t(FabricVerilogOption::e_undriven_input_type::NUM_TYPES);
       ++itype) {
    full_type_str += std::string(CONSTANT_UNDRIVEN_INPUT_TYPE_STRING_[itype]) +
                     std::string("|");
  }
  full_type_str.pop_back();
  full_type_str += std::string("]");
  return full_type_str;
}

bool FabricVerilogOption::little_endian() const { return little_endian_; }
bool FabricVerilogOption::verbose_output() const { return verbose_output_; }

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
void FabricVerilogOption::set_output_directory(const std::string& output_dir) {
  output_directory_ = output_dir;
}

void FabricVerilogOption::set_include_timing(const bool& enabled) {
  include_timing_ = enabled;
}

void FabricVerilogOption::set_use_relative_path(const bool& enabled) {
  use_relative_path_ = enabled;
}

void FabricVerilogOption::set_time_stamp(const bool& enabled) {
  time_stamp_ = enabled;
}

void FabricVerilogOption::set_explicit_port_mapping(const bool& enabled) {
  explicit_port_mapping_ = enabled;
}

void FabricVerilogOption::set_compress_routing(const bool& enabled) {
  compress_routing_ = enabled;
}

void FabricVerilogOption::set_print_user_defined_template(const bool& enabled) {
  print_user_defined_template_ = enabled;
}

void FabricVerilogOption::set_default_net_type(
  const std::string& default_net_type) {
  /* Decode from net type string */;
  if (default_net_type ==
      std::string(
        VERILOG_DEFAULT_NET_TYPE_STRING[VERILOG_DEFAULT_NET_TYPE_NONE])) {
    default_net_type_ = VERILOG_DEFAULT_NET_TYPE_NONE;
  } else if (default_net_type ==
             std::string(VERILOG_DEFAULT_NET_TYPE_STRING
                           [VERILOG_DEFAULT_NET_TYPE_WIRE])) {
    default_net_type_ = VERILOG_DEFAULT_NET_TYPE_WIRE;
  } else {
    VTR_LOG_WARN(
      "Invalid default net type: '%s'! Expect ['%s'|'%s']\n",
      default_net_type.c_str(),
      VERILOG_DEFAULT_NET_TYPE_STRING[VERILOG_DEFAULT_NET_TYPE_NONE],
      VERILOG_DEFAULT_NET_TYPE_STRING[VERILOG_DEFAULT_NET_TYPE_WIRE]);
  }
}

bool FabricVerilogOption::set_constant_undriven_inputs(
  const std::string& type_str) {
  bool valid_type = false;
  for (size_t itype = 0;
       itype < size_t(FabricVerilogOption::e_undriven_input_type::NUM_TYPES);
       ++itype) {
    if (std::string(CONSTANT_UNDRIVEN_INPUT_TYPE_STRING_[itype]) == type_str) {
      constant_undriven_inputs_ =
        static_cast<FabricVerilogOption::e_undriven_input_type>(itype);
      valid_type = true;
      break;
    }
  }
  if (!valid_type) {
    VTR_LOG_ERROR("Invalid types for undriven inputs: %s. Expect %s\n",
                  type_str.c_str(),
                  full_constant_undriven_input_type_str().c_str());
  }
  return valid_type;
}

bool FabricVerilogOption::set_constant_undriven_inputs(
  const FabricVerilogOption::e_undriven_input_type& type) {
  constant_undriven_inputs_ = type;
  return type != FabricVerilogOption::e_undriven_input_type::NUM_TYPES;
}

void FabricVerilogOption::set_little_endian(const bool& enabled) {
  little_endian_ = enabled;
}

void FabricVerilogOption::set_verbose_output(const bool& enabled) {
  verbose_output_ = enabled;
}

} /* end namespace openfpga */
