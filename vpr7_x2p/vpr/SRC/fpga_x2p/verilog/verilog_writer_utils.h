/************************************************
 * Header file for verilog_writer_utils.cpp
 * Include function declaration for most frequently
 * used Verilog writers 
 ***********************************************/
#ifndef VERILOG_WRITER_UTILS_H 
#define VERILOG_WRITER_UTILS_H 

#include <string>
#include "device_port.h"
#include "module_manager.h"

void print_verilog_file_header(std::fstream& fp,
                               const std::string& usage);

void print_verilog_include_defines_preproc_file(std::fstream& fp, 
                                                const std::string& verilog_dir);

void print_verilog_comment(std::fstream& fp, 
                           const std::string& comment);

void print_verilog_module_definition(std::fstream& fp, 
                                     const std::string& module_name);

void print_verilog_module_ports(std::fstream& fp, 
                                const ModuleManager& module_manager, const ModuleId& module_id);

void print_verilog_module_declaration(std::fstream& fp, 
                                      const ModuleManager& module_manager, const ModuleId& module_id);

void print_verilog_module_instance(std::fstream& fp, 
                                   const ModuleManager& module_manager,
                                   const ModuleId& parent_module_id, const ModuleId& child_module_id,
                                   const std::map<std::string, BasicPort>& port2port_name_map,
                                   const bool& explicit_port_map);

void print_verilog_module_end(std::fstream& fp, 
                              const std::string& module_name);

std::string generate_verilog_port(const enum e_dump_verilog_port_type& dump_port_type,
                                  const BasicPort& port_info);

std::vector<BasicPort> combine_verilog_ports(const std::vector<BasicPort>& ports); 

std::string generate_verilog_ports(const std::vector<BasicPort>& merged_ports); 

BasicPort generate_verilog_bus_port(const std::vector<BasicPort>& input_ports, 
                                    const std::string& bus_port_name);

std::string generate_verilog_local_wire(const BasicPort& output_port,
                                        const std::vector<BasicPort>& input_ports);

void print_verilog_wire_constant_values(std::fstream& fp,
                                        const BasicPort& output_port,
                                        const std::vector<size_t>& const_values);

void print_verilog_wire_connection(std::fstream& fp,
                                   const BasicPort& output_port,
                                   const BasicPort& input_port);

void print_verilog_buffer_instance(std::fstream& fp,
                                   ModuleManager& module_manager, 
                                   const CircuitLibrary& circuit_lib, 
                                   const ModuleId& parent_module_id, 
                                   const CircuitModelId& buffer_model, 
                                   const BasicPort& instance_input_port,
                                   const BasicPort& instance_output_port);

#endif
