/************************************************
 * Header file for fpga_x2p_naming.cpp
 * Include functions to generate module/port names
 * for Verilog and SPICE netlists 
 ***********************************************/

#ifndef FPGA_X2P_NAMING_H
#define FPGA_X2P_NAMING_H

#include <string>

#include "vtr_geometry.h"
#include "circuit_library.h"
#include "vpr_types.h"

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

std::string generate_memory_module_name(const CircuitLibrary& circuit_lib,
                                        const CircuitModelId& circuit_model, 
                                        const CircuitModelId& sram_model, 
                                        const std::string& postfix);

std::string generate_routing_block_netlist_name(const std::string& prefix, 
                                                const size_t& block_id,
                                                const std::string& postfix);

std::string generate_routing_block_netlist_name(const std::string& prefix, 
                                                const vtr::Point<size_t>& block_id,
                                                const std::string& postfix);

std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const size_t& block_id);

std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const vtr::Point<size_t>& coordinate);

std::string generate_routing_track_port_name(const t_rr_type& chan_type, 
                                             const vtr::Point<size_t>& coordinate,
                                             const size_t& track_id,
                                             const PORTS& port_direction);

std::string generate_switch_block_module_name(const vtr::Point<size_t>& coordinate);

std::string generate_grid_port_name(const vtr::Point<size_t>& coordinate,
                                    const size_t& height, 
                                    const e_side& side, 
                                    const size_t& pin_id,
                                    const bool& for_top_netlist);

std::string generate_reserved_sram_port_name(const e_spice_model_port_type& port_type);

std::string generate_formal_verification_sram_port_name(const CircuitLibrary& circuit_lib,
                                                        const CircuitModelId& sram_model);

std::string generate_sram_port_name(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& sram_model,
                                    const e_sram_orgz& sram_orgz_type,
                                    const e_spice_model_port_type& port_type);

std::string generate_sram_local_port_name(const CircuitLibrary& circuit_lib,
                                          const CircuitModelId& sram_model,
                                          const e_sram_orgz& sram_orgz_type,
                                          const e_spice_model_port_type& port_type);

#endif
