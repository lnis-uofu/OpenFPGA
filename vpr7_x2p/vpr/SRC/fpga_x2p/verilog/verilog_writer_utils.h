/************************************************
 * Header file for verilog_writer_utils.cpp
 * Include function declaration for most frequently
 * used Verilog writers 
 ***********************************************/
#ifndef VERILOG_WRITER_UTILS_H 
#define VERILOG_WRITER_UTILS_H 

#include <string>
#include "device_port.h"

std::string generate_verilog_port(const enum e_dump_verilog_port_type& dump_port_type,
                                  const BasicPort& port_info);

#endif
