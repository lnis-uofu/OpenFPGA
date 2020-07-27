/********************************************************************
 * Header file for circuit_library_utils.cpp
 *******************************************************************/
#ifndef CIRCUIT_LIBRARY_UTILS_H
#define CIRCUIT_LIBRARY_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "circuit_types.h"
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<CircuitModelId> find_circuit_sram_models(const CircuitLibrary& circuit_lib,
                                                     const CircuitModelId& circuit_model);

std::vector<CircuitPortId> find_circuit_regular_sram_ports(const CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& circuit_model);

std::vector<CircuitPortId> find_circuit_mode_select_sram_ports(const CircuitLibrary& circuit_lib,
                                                               const CircuitModelId& circuit_model);

size_t find_circuit_num_shared_config_bits(const CircuitLibrary& circuit_lib,
                                           const CircuitModelId& circuit_model,
                                           const e_config_protocol_type& sram_orgz_type);

size_t find_circuit_num_config_bits(const e_config_protocol_type& config_protocol_type,
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model);

std::vector<CircuitPortId> find_circuit_library_global_ports(const CircuitLibrary& circuit_lib);

std::vector<std::string> find_circuit_library_unique_verilog_netlists(const CircuitLibrary& circuit_lib);

bool check_configurable_memory_circuit_model(const e_config_protocol_type& config_protocol_type,
                                             const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& config_mem_circuit_model);

CircuitPortId find_circuit_model_power_gate_en_port(const CircuitLibrary& circuit_lib,
                                                    const CircuitModelId& circuit_model);

CircuitPortId find_circuit_model_power_gate_enb_port(const CircuitLibrary& circuit_lib,
                                                     const CircuitModelId& circuit_model);

} /* end namespace openfpga */

#endif
