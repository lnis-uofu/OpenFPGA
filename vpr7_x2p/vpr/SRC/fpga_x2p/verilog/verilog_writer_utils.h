/************************************************
 * Header file for verilog_writer_utils.cpp
 * Include function declaration for most frequently
 * used Verilog writers 
 ***********************************************/
#ifndef VERILOG_WRITER_UTILS_H 
#define VERILOG_WRITER_UTILS_H 

#include <string>
#include "device_port.h"

void print_verilog_file_header(std::fstream& fp,
                               const std::string& usage);

void print_verilog_include_defines_preproc_file(std::fstream& fp, 
                                                const std::string& verilog_dir);

std::string generate_verilog_port(const enum e_dump_verilog_port_type& dump_port_type,
                                  const BasicPort& port_info);

#endif
