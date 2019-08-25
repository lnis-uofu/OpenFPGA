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
                                   std::map<std::string, std::string>& port2port_name_map,
                                   const bool& explicit_port_map);

void print_verilog_module_end(std::fstream& fp, 
                              const std::string& module_name);

std::string generate_verilog_port(const enum e_dump_verilog_port_type& dump_port_type,
                                  const BasicPort& port_info);

#endif
