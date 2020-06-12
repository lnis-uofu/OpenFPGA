/******************************************************************************
 * Memember functions for data structure VerilogTestbenchOption
 ******************************************************************************/
#include "vtr_assert.h"
#include "vtr_log.h"

#include "verilog_testbench_options.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
VerilogTestbenchOption::VerilogTestbenchOption() {
  output_directory_.clear();
  reference_benchmark_file_path_.clear();
  print_preconfig_top_testbench_ = false;
  print_formal_verification_top_netlist_ = false;
  print_top_testbench_ = false;
  simulation_ini_path_.clear();
  explicit_port_mapping_ = false;
  verbose_output_ = false;
}

/**************************************************
 * Public Accessors 
 *************************************************/
std::string VerilogTestbenchOption::output_directory() const {
  return output_directory_;
}

std::string VerilogTestbenchOption::reference_benchmark_file_path() const {
  return reference_benchmark_file_path_;
}

bool VerilogTestbenchOption::print_formal_verification_top_netlist() const {
  return print_formal_verification_top_netlist_;
}

bool VerilogTestbenchOption::print_preconfig_top_testbench() const {
  return print_preconfig_top_testbench_;
}

bool VerilogTestbenchOption::print_top_testbench() const {
  return print_top_testbench_;
}

bool VerilogTestbenchOption::fast_configuration() const {
  return fast_configuration_;
}

bool VerilogTestbenchOption::print_simulation_ini() const {
  return !simulation_ini_path_.empty();
}

std::string VerilogTestbenchOption::simulation_ini_path() const {
  return simulation_ini_path_;
}

bool VerilogTestbenchOption::explicit_port_mapping() const {
  return explicit_port_mapping_;
}

bool VerilogTestbenchOption::verbose_output() const {
  return verbose_output_;
}

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
void VerilogTestbenchOption::set_output_directory(const std::string& output_dir) {
  output_directory_ = output_dir;
}

void VerilogTestbenchOption::set_reference_benchmark_file_path(const std::string& reference_benchmark_file_path) {
  reference_benchmark_file_path_ = reference_benchmark_file_path;
  /* Chain effect on other options: 
   * Enable/disable the print_preconfig_top_testbench and print_top_testbench
   */
  set_print_preconfig_top_testbench(print_preconfig_top_testbench_); 
  set_print_top_testbench(print_top_testbench_); 
}
 
void VerilogTestbenchOption::set_print_formal_verification_top_netlist(const bool& enabled) {
  print_formal_verification_top_netlist_ = enabled;
}

void VerilogTestbenchOption::set_fast_configuration(const bool& enabled) {
  fast_configuration_ = enabled;
}

void VerilogTestbenchOption::set_print_preconfig_top_testbench(const bool& enabled) {
  print_preconfig_top_testbench_ = enabled
                                 && (!reference_benchmark_file_path_.empty());
  /* Enable print formal verification top_netlist if this is enabled */
  if (true == print_preconfig_top_testbench_) {
    if (false == print_formal_verification_top_netlist_) { 
      VTR_LOG_WARN("Forcely enable to print top-level Verilog netlist in formal verification purpose as print pre-configured top-level Verilog testbench is enabled\n");
      print_formal_verification_top_netlist_ = true;
    }
  }
}

void VerilogTestbenchOption::set_print_top_testbench(const bool& enabled) {
  print_top_testbench_ = enabled && (!reference_benchmark_file_path_.empty());
}

void VerilogTestbenchOption::set_print_simulation_ini(const std::string& simulation_ini_path) {
  simulation_ini_path_ = simulation_ini_path;
}

void VerilogTestbenchOption::set_explicit_port_mapping(const bool& enabled) {
  explicit_port_mapping_ = enabled;
}

void VerilogTestbenchOption::set_verbose_output(const bool& enabled) {
  verbose_output_ = enabled;
}

} /* end namespace openfpga */
