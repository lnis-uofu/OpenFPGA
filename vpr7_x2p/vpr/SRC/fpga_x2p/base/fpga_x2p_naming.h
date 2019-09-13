/************************************************
 * Header file for fpga_x2p_naming.cpp
 * Include functions to generate module/port names
 * for Verilog and SPICE netlists 
 ***********************************************/

#ifndef FPGA_X2P_NAMING_H
#define FPGA_X2P_NAMING_H

#include <string>

#include "circuit_library.h"

std::string generate_verilog_mux_node_name(const size_t& node_level, 
                                           const bool& add_buffer_postfix);

std::string generate_verilog_mux_subckt_name(const CircuitLibrary& circuit_lib, 
                                             const CircuitModelId& circuit_model, 
                                             const size_t& mux_size, 
                                             const std::string& posfix) ;

std::string generate_verilog_mux_branch_subckt_name(const CircuitLibrary& circuit_lib, 
                                                    const CircuitModelId& circuit_model, 
                                                    const size_t& mux_size, 
                                                    const size_t& branch_mux_size, 
                                                    const std::string& posfix);

std::string generate_mux_local_decoder_subckt_name(const size_t& addr_size, 
                                                   const size_t& data_size); 

std::string generate_segment_wire_subckt_name(const std::string& wire_model_name, 
                                              const size_t& segment_id); 

std::string generate_segment_wire_mid_output_name(const std::string& regular_output_name); 

#endif
