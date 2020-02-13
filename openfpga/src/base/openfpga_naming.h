/************************************************
 * Header file for fpga_x2p_naming.cpp
 * Include functions to generate module/port names
 * for Verilog and SPICE netlists 
 ***********************************************/

#ifndef OPENFPGA_NAMING_H
#define OPENFPGA_NAMING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "vtr_geometry.h"
#include "circuit_library.h"
#include "device_grid.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::string generate_mux_node_name(const size_t& node_level, 
                                   const bool& add_buffer_postfix);

std::string generate_mux_branch_instance_name(const size_t& node_level, 
                                              const size_t& node_index_at_level,
                                              const bool& add_buffer_postfix);

std::string generate_mux_subckt_name(const CircuitLibrary& circuit_lib, 
                                     const CircuitModelId& circuit_model, 
                                     const size_t& mux_size, 
                                     const std::string& posfix) ;

std::string generate_mux_branch_subckt_name(const CircuitLibrary& circuit_lib, 
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

std::string generate_connection_block_netlist_name(const t_rr_type& cb_type, 
                                                   const vtr::Point<size_t>& coordinate,
                                                   const std::string& postfix);

std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const size_t& block_id);

std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const vtr::Point<size_t>& coordinate);

std::string generate_routing_track_port_name(const t_rr_type& chan_type, 
                                             const vtr::Point<size_t>& coordinate,
                                             const size_t& track_id,
                                             const PORTS& port_direction);

std::string generate_sb_module_track_port_name(const t_rr_type& chan_type, 
                                               const e_side& module_side,
                                               const size_t& track_id,
                                               const PORTS& port_direction);

std::string generate_cb_module_track_port_name(const t_rr_type& chan_type, 
                                               const size_t& track_id,
                                               const PORTS& port_direction);

std::string generate_routing_track_middle_output_port_name(const t_rr_type& chan_type, 
                                                           const vtr::Point<size_t>& coordinate,
                                                           const size_t& track_id);

std::string generate_switch_block_module_name(const vtr::Point<size_t>& coordinate);

std::string generate_connection_block_module_name(const t_rr_type& cb_type, 
                                                  const vtr::Point<size_t>& coordinate);

std::string generate_sb_mux_instance_name(const std::string& prefix,
                                          const e_side& sb_side, 
                                          const size_t& track_id, 
                                          const std::string& postfix);

std::string generate_sb_memory_instance_name(const std::string& prefix,
                                             const e_side& sb_side, 
                                             const size_t& track_id, 
                                             const std::string& postfix);

std::string generate_cb_mux_instance_name(const std::string& prefix,
                                          const e_side& cb_side, 
                                          const size_t& pin_id, 
                                          const std::string& postfix);

std::string generate_cb_memory_instance_name(const std::string& prefix,
                                             const e_side& cb_side, 
                                             const size_t& pin_id, 
                                             const std::string& postfix);

std::string generate_pb_mux_instance_name(const std::string& prefix,
                                          t_pb_graph_pin* pb_graph_pin, 
                                          const std::string& postfix);

std::string generate_pb_memory_instance_name(const std::string& prefix,
                                             t_pb_graph_pin* pb_graph_pin, 
                                             const std::string& postfix);

std::string generate_grid_port_name(const vtr::Point<size_t>& coordinate,
                                    const size_t& width, 
                                    const size_t& height, 
                                    const e_side& side, 
                                    const size_t& pin_id,
                                    const bool& for_top_netlist);

std::string generate_grid_duplicated_port_name(const size_t& width,
                                               const size_t& height, 
                                               const e_side& side, 
                                               const size_t& pin_id,
                                               const bool& upper_port);

std::string generate_grid_module_port_name(const size_t& pin_id);

std::string generate_grid_side_port_name(const DeviceGrid& grids,
                                         const vtr::Point<size_t>& coordinate,
                                         const e_side& side, 
                                         const size_t& pin_id);

std::string generate_sb_module_grid_port_name(const e_side& sb_side,
                                              const e_side& grid_side, 
                                              const size_t& pin_id);

std::string generate_cb_module_grid_port_name(const e_side& cb_side, 
                                              const size_t& pin_id);

std::string generate_reserved_sram_port_name(const e_circuit_model_port_type& port_type);

std::string generate_formal_verification_sram_port_name(const CircuitLibrary& circuit_lib,
                                                        const CircuitModelId& sram_model);

std::string generate_configuration_chain_head_name();

std::string generate_configuration_chain_tail_name();

std::string generate_configuration_chain_data_out_name();

std::string generate_configuration_chain_inverted_data_out_name();

std::string generate_mux_local_decoder_addr_port_name();

std::string generate_mux_local_decoder_data_port_name();

std::string generate_mux_local_decoder_data_inv_port_name();

std::string generate_local_config_bus_port_name();

std::string generate_sram_port_name(const e_config_protocol_type& sram_orgz_type,
                                    const e_circuit_model_port_type& port_type);

std::string generate_sram_local_port_name(const CircuitLibrary& circuit_lib,
                                          const CircuitModelId& sram_model,
                                          const e_config_protocol_type& sram_orgz_type,
                                          const e_circuit_model_port_type& port_type);

std::string generate_mux_input_bus_port_name(const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& mux_model,
                                             const size_t& mux_size, 
                                             const size_t& mux_instance_id);

std::string generate_mux_config_bus_port_name(const CircuitLibrary& circuit_lib,
                                              const CircuitModelId& mux_model,
                                              const size_t& mux_size, 
                                              const size_t& bus_id,
                                              const bool& inverted);

std::string generate_local_sram_port_name(const std::string& port_prefix, 
                                          const size_t& instance_id,
                                          const e_circuit_model_port_type& port_type);

std::string generate_mux_sram_port_name(const CircuitLibrary& circuit_lib,
                                        const CircuitModelId& mux_model,
                                        const size_t& mux_size, 
                                        const size_t& mux_instance_id,
                                        const e_circuit_model_port_type& port_type);

std::string generate_grid_block_prefix(const std::string& prefix,
                                       const e_side& io_side);

std::string generate_grid_block_netlist_name(const std::string& block_name,
                                             const bool& is_block_io,
                                             const e_side& io_side,
                                             const std::string& postfix);

std::string generate_grid_block_module_name(const std::string& prefix,
                                            const std::string& block_name,
                                            const bool& is_block_io,
                                            const e_side& io_side);

std::string generate_grid_block_instance_name(const std::string& prefix,
                                              const std::string& block_name,
                                              const bool& is_block_io,
                                              const e_side& io_side,
                                              const vtr::Point<size_t>& grid_coord);

std::string generate_physical_block_module_name(const std::string& prefix,
                                                t_pb_type* physical_pb_type);

std::string generate_physical_block_instance_name(const std::string& prefix,
                                                  t_pb_type* pb_type,
                                                  const size_t& index);

std::string generate_grid_physical_block_module_name(const std::string& prefix,
                                                     t_pb_type* pb_type,
                                                     const e_side& border_side);

std::string generate_grid_physical_block_instance_name(const std::string& prefix, 
                                                       t_pb_type* pb_type, 
                                                       const e_side& border_side,
                                                       const size_t& index);


e_side find_grid_border_side(const vtr::Point<size_t>& device_size,
                             const vtr::Point<size_t>& grid_coordinate);

bool is_core_grid_on_given_border_side(const vtr::Point<size_t>& device_size,
                                       const vtr::Point<size_t>& grid_coordinate,
                                       const e_side& border_side);

std::string generate_pb_type_port_name(t_port* pb_type_port);

std::string generate_fpga_global_io_port_name(const std::string& prefix, 
                                              const CircuitLibrary& circuit_lib,
                                              const CircuitModelId& circuit_model);

std::string generate_fpga_top_module_name();

std::string generate_fpga_top_netlist_name(const std::string& postfix);

std::string generate_const_value_module_name(const size_t& const_val);

std::string generate_const_value_module_output_port_name(const size_t& const_val);

} /* end namespace openfpga */

#endif
