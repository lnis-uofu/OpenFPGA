/******************************************************************** 
 *  Header file for fpga_x2p_mem_utils.cpp
 **********************************************************************/
#ifndef FPGA_X2P_MEM_UTILS_H
#define FPGA_X2P_MEM_UTILS_H

/* Header files are included for the data types appear in the function declaration below */
#include <vector>
#include "device_port.h"
#include "spice_types.h"
#include "module_manager.h"

std::map<std::string, BasicPort> generate_mem_module_port2port_map(const BasicPort& config_bus,
                                                                   const std::vector<BasicPort>& mem_output_bus_ports,
                                                                   const e_spice_model_design_tech& mem_design_tech,
                                                                   const e_sram_orgz& sram_orgz_type);

void update_mem_module_config_bus(const e_sram_orgz& sram_orgz_type,
                                  const e_spice_model_design_tech& mem_design_tech,
                                  const size_t& num_config_bits,
                                  BasicPort& config_bus);

bool check_mem_config_bus(const e_sram_orgz& sram_orgz_type, 
                          const BasicPort& config_bus, 
                          const size_t& local_expected_msb);

std::vector<std::string> generate_sram_port_names(const CircuitLibrary& circuit_lib,
                                                  const CircuitModelId& sram_model,
                                                  const e_sram_orgz sram_orgz_type);

size_t generate_sram_port_size(const e_sram_orgz sram_orgz_type,
                               const size_t& num_config_bits);

#endif
