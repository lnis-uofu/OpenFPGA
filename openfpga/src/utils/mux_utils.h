#ifndef MUX_UTILS_H
#define MUX_UTILS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/

#include <vector>

#include "circuit_library.h"
#include "mux_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* Begin namespace openfpga */
namespace openfpga {

bool valid_mux_implementation_num_inputs(const size_t& mux_size);

size_t find_mux_num_datapath_inputs(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model,
                                    const size_t& mux_size);

size_t find_mux_implementation_num_inputs(const CircuitLibrary& circuit_lib,
                                          const CircuitModelId& circuit_model,
                                          const size_t& mux_size);

enum e_circuit_model_structure find_mux_implementation_structure(const CircuitLibrary& circuit_lib,
					                                           const CircuitModelId& circuit_model,
						                                       const size_t& mux_size);

size_t find_treelike_mux_num_levels(const size_t& mux_size);

size_t find_multilevel_mux_branch_num_inputs(const size_t& mux_size,
                                             const size_t& mux_level);

std::vector<bool> build_mux_intermediate_buffer_location_map(const CircuitLibrary& circuit_lib, 
                                                             const CircuitModelId& circuit_model,
                                                             const size_t& num_mux_levels);

size_t find_mux_num_reserved_config_bits(const CircuitLibrary& circuit_lib,
                                         const CircuitModelId& mux_model,
                                         const MuxGraph& mux_graph);

size_t find_mux_num_config_bits(const CircuitLibrary& circuit_lib,
                                const CircuitModelId& mux_model,
                                const MuxGraph& mux_graph, 
                                const e_config_protocol_type& sram_orgz_type);

size_t find_mux_num_shared_config_bits(const CircuitLibrary& circuit_lib,
                                       const CircuitModelId& mux_model,
                                       const MuxGraph& mux_graph, 
                                       const e_config_protocol_type& sram_orgz_type);

} /* End namespace openfpga*/

#endif
