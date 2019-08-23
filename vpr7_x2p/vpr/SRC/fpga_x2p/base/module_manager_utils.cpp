/******************************************************************************
 * This files includes most utilized functions 
 * for data structures for module management.
 ******************************************************************************/

#include <map>
#include <algorithm>

#include "vtr_assert.h"

#include "spice_types.h"

#include "circuit_library.h"
#include "module_manager.h"
#include "module_manager_utils.h"

ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model) {
  ModuleId module = module_manager.add_module(circuit_lib.model_name(circuit_model)); 

  /* Add ports */
  /* Find global ports and add one by one */
  for (const auto& port : circuit_lib.model_global_ports(circuit_model, true)) {
    BasicPort port_info(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module, port_info, ModuleManager::MODULE_GLOBAL_PORT);  
  }

  /* Find other ports and add one by one */
  /* Create a type-to-type map for ports */
  std::map<enum e_spice_model_port_type, ModuleManager::e_module_port_type> port_type2type_map;
  port_type2type_map[SPICE_MODEL_PORT_INOUT] = ModuleManager::MODULE_INOUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_INPUT] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_CLOCK] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_SRAM] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_BL] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_BLB] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_WL] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_WLB] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[SPICE_MODEL_PORT_OUTPUT] = ModuleManager::MODULE_OUTPUT_PORT;

  /* Input ports (ignore all the global ports when searching the circuit_lib */
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : circuit_lib.model_ports_by_type(circuit_model, kv.first, true)) {
      BasicPort port_info(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
      module_manager.add_port(module, port_info, kv.second);  
    }
  }

  /* Return the new id */
  return module;
}
