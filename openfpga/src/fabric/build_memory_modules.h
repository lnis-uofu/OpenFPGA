#ifndef BUILD_MEMORY_MODULES_H
#define BUILD_MEMORY_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "decoder_library.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<ModuleNetId> add_module_output_nets_to_chain_mem_modules(ModuleManager& module_manager,
                                                                     const ModuleId& mem_module,
                                                                     const std::string& mem_module_output_name,
                                                                     const CircuitLibrary& circuit_lib,
                                                                     const CircuitPortId& circuit_port,
                                                                     const ModuleId& child_module,
                                                                     const size_t& child_index,
                                                                     const size_t& child_instance);

void build_memory_modules(ModuleManager& module_manager,
                          DecoderLibrary& arch_decoder_lib,
                          const MuxLibrary& mux_lib,
                          const CircuitLibrary& circuit_lib,
                          const e_config_protocol_type& sram_orgz_type);

} /* end namespace openfpga */

#endif
