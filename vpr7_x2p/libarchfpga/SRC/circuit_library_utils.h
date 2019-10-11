/********************************************************************
 * Header file for circuit_library_utils.cpp
 *******************************************************************/
#ifndef CIRCUIT_LIBRARY_UTILS_H
#define CIRCUIT_LIBRARY_UTILS_H

/* Header files should be included in a sequence */
/* Standard header files required go first */

#include <vector>
#include "spice_types.h"
#include "circuit_library.h"

std::vector<CircuitModelId> find_circuit_sram_models(const CircuitLibrary& circuit_lib,
                                                     const CircuitModelId& circuit_model);

std::vector<CircuitPortId> find_circuit_regular_sram_ports(const CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& circuit_model);

std::vector<CircuitPortId> find_circuit_mode_select_sram_ports(const CircuitLibrary& circuit_lib,
                                                               const CircuitModelId& circuit_model);

size_t find_circuit_num_shared_config_bits(const CircuitLibrary& circuit_lib,
                                           const CircuitModelId& circuit_model,
                                           const e_sram_orgz& sram_orgz_type);

size_t find_circuit_num_config_bits(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model);

#endif
