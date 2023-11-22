#ifndef BUILD_MEMORY_MODULES_H
#define BUILD_MEMORY_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "circuit_library.h"
#include "decoder_library.h"
#include "module_manager.h"
#include "mux_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<ModuleNetId> add_module_output_nets_to_chain_mem_modules(
  ModuleManager& module_manager, const ModuleId& mem_module,
  const std::string& mem_module_output_name, const CircuitLibrary& circuit_lib,
  const CircuitPortId& circuit_port, const ModuleId& child_module,
  const size_t& child_index, const size_t& child_instance);

int build_memory_modules(ModuleManager& module_manager,
                         DecoderLibrary& arch_decoder_lib,
                         const MuxLibrary& mux_lib,
                         const CircuitLibrary& circuit_lib,
                         const e_config_protocol_type& sram_orgz_type,
                         const bool& require_feedthrough_memory,
                         const bool& verbose);

int build_memory_group_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type, const std::string& module_name,
  const CircuitModelId& sram_model, const std::vector<ModuleId>& child_modules,
  const std::vector<std::string>& child_instance_names, const size_t& num_mems,
  const bool& verbose);

int add_physical_memory_module(ModuleManager& module_manager,
                               DecoderLibrary& decoder_lib,
                               const ModuleId& curr_module,
                               const std::string& suggested_module_name_prefix,
                               const CircuitLibrary& circuit_lib,
                               const e_config_protocol_type& sram_orgz_type,
                               const CircuitModelId& sram_model,
                               const bool& verbose);

} /* end namespace openfpga */

#endif
