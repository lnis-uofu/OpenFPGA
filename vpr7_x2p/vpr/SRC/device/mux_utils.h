/**************************************************
 * This file includes only declaration for the 
 * functions in mux_utils.c
 * Please refer to the source file for more details
 *************************************************/
#ifndef MUX_UTILS_H
#define MUX_UTILS_H

#include "circuit_library.h"

bool valid_mux_implementation_num_inputs(const size_t& mux_size);

size_t find_mux_implementation_num_inputs(const CircuitLibrary& circuit_lib,
                                          const CircuitModelId& circuit_model,
                                          const size_t& mux_size);


enum e_spice_model_structure find_mux_implementation_structure(const CircuitLibrary& circuit_lib,
					                                           const CircuitModelId& circuit_model,
						                                       const size_t& mux_size);

size_t find_treelike_mux_num_levels(const size_t& mux_size);

size_t find_multilevel_mux_branch_num_inputs(const size_t& mux_size,
                                             const size_t& mux_level);

#endif
