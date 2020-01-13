/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef CHECK_CIRCUIT_LIBRARY_H
#define CHECK_CIRCUIT_LIBRARY_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <vector>
#include "circuit_types.h"
#include "circuit_library.h"

/* Check points to make sure we have a valid circuit library */
size_t check_one_circuit_model_port_required(const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& circuit_model, 
                                             const std::vector<enum e_circuit_model_port_type>& port_types_to_check);

size_t check_one_circuit_model_port_size_required(const CircuitLibrary& circuit_lib,
                                                  const CircuitModelId& circuit_model, 
                                                  const CircuitPortId& circuit_port,
                                                  const size_t& port_size_to_check);

size_t check_one_circuit_model_port_type_and_size_required(const CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& circuit_model, 
                                                           const enum e_circuit_model_port_type& port_type_to_check,
                                                           const size_t& num_ports_to_check,
                                                           const size_t& port_size_to_check,
                                                           const bool& include_global_ports);

size_t check_ff_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model);

size_t check_ccff_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& circuit_model);

size_t check_sram_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& circuit_model,
                                      const bool& check_blwl);

void check_circuit_library(const CircuitLibrary& circuit_lib);

#endif
