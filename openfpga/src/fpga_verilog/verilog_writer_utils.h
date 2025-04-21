/************************************************
 * Header file for verilog_writer_utils.cpp
 * Include function declaration for most frequently
 * used Verilog writers
 ***********************************************/
#ifndef VERILOG_WRITER_UTILS_H
#define VERILOG_WRITER_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>
#include <vector>

#include "circuit_library.h"
#include "module_manager.h"
#include "openfpga_port.h"
#include "verilog_port_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* Tips: for naming your function in this header/source file
 * If a function outputs to a file, its name should begin with "print_verilog"
 * If a function creates a string without outputting to a file, its name should
 * begin with "generate_verilog" Please show respect to this naming convention,
 * in order to keep a clean header/source file as well maintain a easy way to
 * identify the functions
 */
std::string generate_verilog_default_net_type_declaration(
  const e_verilog_default_net_type& default_net_type);

void print_verilog_default_net_type_declaration(
  std::fstream& fp, const e_verilog_default_net_type& default_net_type);

void print_verilog_file_header(std::fstream& fp, const std::string& usage,
                               const bool& include_time_stamp,
                               const bool& include_time_scale = false);

void print_verilog_include_netlist(std::fstream& fp,
                                   const std::string& netlist_name);

std::string generate_verilog_define_flag(const std::string& flag_name,
                                         const int& flag_value);
void print_verilog_define_flag(std::fstream& fp, const std::string& flag_name,
                               const int& flag_value);

void print_verilog_include_defines_preproc_file(std::fstream& fp,
                                                const std::string& verilog_dir);

void print_verilog_comment(std::fstream& fp, const std::string& comment);

void print_verilog_preprocessing_flag(std::fstream& fp,
                                      const std::string& preproc_flag);

void print_verilog_endif(std::fstream& fp);

void print_verilog_module_definition(std::fstream& fp,
                                     const ModuleManager& module_manager,
                                     const ModuleId& module_id);

void print_verilog_module_ports(
  std::fstream& fp, const ModuleManager& module_manager,
  const ModuleId& module_id, const e_verilog_default_net_type& default_net_type,
  const bool& little_endian);

void print_verilog_module_declaration(
  std::fstream& fp, const ModuleManager& module_manager,
  const ModuleId& module_id, const e_verilog_default_net_type& default_net_type,
  const bool& little_endian);

void print_verilog_module_instance(
  std::fstream& fp, const ModuleManager& module_manager,
  const ModuleId& module_id, const std::string& instance_name,
  const std::map<std::string, BasicPort>& port2port_name_map,
  const bool& use_explicit_port_map, const bool& little_endian);

void print_verilog_module_instance(
  std::fstream& fp, const ModuleManager& module_manager,
  const ModuleId& parent_module_id, const ModuleId& child_module_id,
  const std::map<std::string, BasicPort>& port2port_name_map,
  const bool& use_explicit_port_map, const bool& little_endian);

void print_verilog_module_end(
  std::fstream& fp, const std::string& module_name,
  const e_verilog_default_net_type& default_net_type);

std::string generate_verilog_port(
  const enum e_dump_verilog_port_type& dump_port_type,
  const BasicPort& port_info, const bool& must_print_port_size,
  const bool& little_endian);

bool two_verilog_ports_mergeable(const BasicPort& portA,
                                 const BasicPort& portB);

BasicPort merge_two_verilog_ports(const BasicPort& portA,
                                  const BasicPort& portB);

std::vector<BasicPort> combine_verilog_ports(
  const std::vector<BasicPort>& ports);

std::string generate_verilog_ports(const std::vector<BasicPort>& merged_ports,
                                   const bool& little_endian);

BasicPort generate_verilog_bus_port(const std::vector<BasicPort>& input_ports,
                                    const std::string& bus_port_name);

std::string generate_verilog_local_wire(
  const BasicPort& output_port, const std::vector<BasicPort>& input_ports,
  const bool& little_endian);

std::string generate_verilog_constant_values(
  const std::vector<size_t>& const_values, const bool& short_constant = true);

std::string generate_verilog_port_constant_values(
  const BasicPort& output_port, const std::vector<size_t>& const_values,
  const bool& little_endian, const bool& is_register = false);

std::string generate_verilog_ports_constant_values(
  const std::vector<BasicPort>& output_ports,
  const std::vector<size_t>& const_values, const bool& little_endian,
  const bool& is_register = false);

void print_verilog_wire_constant_values(std::fstream& fp,
                                        const BasicPort& output_port,
                                        const std::vector<size_t>& const_values,
                                        const bool& little_endian);

void print_verilog_wire_constant_values_bit_blast(
  std::fstream& fp, const BasicPort& output_port,
  const std::vector<size_t>& const_values, const bool& little_endian);

void print_verilog_deposit_wire_constant_values(
  std::fstream& fp, const BasicPort& output_port,
  const std::vector<size_t>& const_values, const bool& little_endian);

void print_verilog_force_wire_constant_values(
  std::fstream& fp, const BasicPort& output_port,
  const std::vector<size_t>& const_values, const bool& little_endian);

void print_verilog_wire_connection(std::fstream& fp,
                                   const BasicPort& output_port,
                                   const BasicPort& input_port,
                                   const bool& inverted,
                                   const bool& little_endian);

void print_verilog_register_connection(std::fstream& fp,
                                       const BasicPort& output_port,
                                       const BasicPort& input_port,
                                       const bool& little_endian,
                                       const bool& inverted);

void print_verilog_buffer_instance(
  std::fstream& fp, ModuleManager& module_manager,
  const CircuitLibrary& circuit_lib, const ModuleId& parent_module_id,
  const CircuitModelId& buffer_model, const BasicPort& instance_input_port,
  const BasicPort& instance_output_port, const bool& little_endian);

void print_verilog_local_sram_wires(std::fstream& fp,
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& sram_model,
                                    const e_config_protocol_type sram_orgz_type,
                                    const size_t& port_size,
                                    const bool& little_endian);

void print_verilog_local_config_bus(
  std::fstream& fp, const std::string& prefix,
  const e_config_protocol_type& sram_orgz_type, const size_t& instance_id,
  const size_t& num_conf_bits, const bool& little_endian);

void print_verilog_mux_config_bus(
  std::fstream& fp, const CircuitLibrary& circuit_lib,
  const CircuitModelId& mux_model, const e_config_protocol_type& sram_orgz_type,
  const size_t& mux_size, const size_t& mux_instance_id,
  const size_t& num_reserved_conf_bits, const size_t& num_conf_bits,
  const bool& little_endian);

void print_verilog_formal_verification_mux_sram_ports_wiring(
  std::fstream& fp, const CircuitLibrary& circuit_lib,
  const CircuitModelId& mux_model, const size_t& mux_size,
  const size_t& mux_instance_id, const size_t& num_conf_bits,
  const BasicPort& fm_config_bus, const bool& little_endian);

void print_verilog_shifted_clock_stimuli(std::fstream& fp,
                                         const BasicPort& port,
                                         const float& initial_delay,
                                         const float& pulse_width,
                                         const size_t& initial_value,
                                         const bool& little_endian);

void print_verilog_pulse_stimuli(std::fstream& fp, const BasicPort& port,
                                 const size_t& initial_value,
                                 const float& pulse_width,
                                 const size_t& flip_value,
                                 const bool& little_endian);

void print_verilog_pulse_stimuli(std::fstream& fp, const BasicPort& port,
                                 const size_t& initial_value,
                                 const std::vector<float>& pulse_widths,
                                 const std::vector<size_t>& flip_values,
                                 const std::string& wait_condition,
                                 const bool& little_endian);

void print_verilog_clock_stimuli(std::fstream& fp, const BasicPort& port,
                                 const size_t& initial_value,
                                 const float& pulse_width,
                                 const std::string& wait_condition,
                                 const bool& little_endian);

void print_verilog_netlist_include_header_file(
  const std::vector<std::string>& netlists_to_be_included,
  const char* subckt_dir, const char* header_file_name,
  const bool& include_time_stamp);

std::string escapeNames(const std::string& name);

} /* end namespace openfpga */

#endif
