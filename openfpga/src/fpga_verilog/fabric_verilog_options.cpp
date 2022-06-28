/******************************************************************************
 * Memember functions for data structure FabricVerilogOption
 ******************************************************************************/
#include "vtr_assert.h"
#include "vtr_log.h"

#include "fabric_verilog_options.h"

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
  verbose_output_ = false;
}

/**************************************************
 * Public Accessors 
 *************************************************/
std::string FabricVerilogOption::output_directory() const {
  return output_directory_;
}

bool FabricVerilogOption::include_timing() const {
  return include_timing_;
}

bool FabricVerilogOption::time_stamp() const {
  return time_stamp_;
}

bool FabricVerilogOption::use_relative_path() const {
  return use_relative_path_;
}

bool FabricVerilogOption::explicit_port_mapping() const {
  return explicit_port_mapping_;
}

bool FabricVerilogOption::compress_routing() const {
  return compress_routing_;
}

bool FabricVerilogOption::print_user_defined_template() const {
  return print_user_defined_template_;
}

e_verilog_default_net_type FabricVerilogOption::default_net_type() const {
  return default_net_type_;
}

bool FabricVerilogOption::verbose_output() const {
  return verbose_output_;
}

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

void FabricVerilogOption::set_default_net_type(const std::string& default_net_type) {
  /* Decode from net type string */;
  if (default_net_type == std::string(VERILOG_DEFAULT_NET_TYPE_STRING[VERILOG_DEFAULT_NET_TYPE_NONE])) {
    default_net_type_ = VERILOG_DEFAULT_NET_TYPE_NONE;
  } else if (default_net_type == std::string(VERILOG_DEFAULT_NET_TYPE_STRING[VERILOG_DEFAULT_NET_TYPE_WIRE])) {
    default_net_type_ = VERILOG_DEFAULT_NET_TYPE_WIRE;
  } else {
    VTR_LOG_WARN("Invalid default net type: '%s'! Expect ['%s'|'%s']\n",
                 default_net_type.c_str(),
                 VERILOG_DEFAULT_NET_TYPE_STRING[VERILOG_DEFAULT_NET_TYPE_NONE],
                 VERILOG_DEFAULT_NET_TYPE_STRING[VERILOG_DEFAULT_NET_TYPE_WIRE]);
  }
}

void FabricVerilogOption::set_verbose_output(const bool& enabled) {
  verbose_output_ = enabled;
}

} /* end namespace openfpga */
