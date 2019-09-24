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

#include "fpga_x2p_naming.h"

#include "module_manager_utils.h"

/******************************************************************************
 * Add a module to the module manager based on the circuit-level
 * description of a circuit model
 * This function add a module with a given customized name
 * as well as add the ports of circuit model to the module manager
 ******************************************************************************/
ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model,
                                             const std::string& module_name) {
  ModuleId module = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module);

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

/******************************************************************************
 * Add a module to the module manager based on the circuit-level
 * description of a circuit model
 * This function add a module in the name of the circuit model
 * as well as add the ports of circuit model to the module manager
 *
 * This function is a wrapper of a more customizable function in the same name
 ******************************************************************************/
ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model) {
 
  return add_circuit_model_to_module_manager(module_manager, circuit_lib, circuit_model, circuit_lib.model_name(circuit_model));
}

/********************************************************************
 * Add a list of ports that are used for reserved SRAM ports to a module
 * in the module manager
 * The reserved SRAM ports are mainly designed for RRAM-based FPGA,
 * which are shared across modules.
 * Note that different modules may require different size of reserved
 * SRAM ports but their LSB must all start from 0 
 *                                +---------+
 *    reserved_sram_port[0:X] --->| ModuleA |
 *                                +---------+
 *
 *                                +---------+
 *    reserved_sram_port[0:Y] --->| ModuleB |
 *                                +---------+
 *
 ********************************************************************/
void add_reserved_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                               const ModuleId& module_id,
                                               const size_t& port_size) {
  /* Add a reserved BLB port to the module */
  std::string blb_port_name = generate_reserved_sram_port_name(SPICE_MODEL_PORT_BLB);
  BasicPort blb_module_port(blb_port_name, port_size); 
  /* Add generated ports to the ModuleManager */
  module_manager.add_port(module_id, blb_module_port, ModuleManager::MODULE_INPUT_PORT);

  /* Add a reserved BLB port to the module */
  std::string wl_port_name = generate_reserved_sram_port_name(SPICE_MODEL_PORT_WL);
  BasicPort wl_module_port(wl_port_name, port_size); 
  /* Add generated ports to the ModuleManager */
  module_manager.add_port(module_id, wl_module_port, ModuleManager::MODULE_INPUT_PORT);
}

/********************************************************************
 * Add a list of ports that are used for formal verification to a module
 * in the module manager
 *
 * The formal verification port will appear only when a pre-processing flag is defined
 * This function will add the pre-processing flag along with the port
 ********************************************************************/
void add_formal_verification_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                                          const ModuleId& module_id,
                                                          t_sram_orgz_info* cur_sram_orgz_info,
                                                          const std::string& preproc_flag,
                                                          const size_t& port_size) {
  /* Create a port */
  std::string port_name = generate_formal_verification_sram_port_name(cur_sram_orgz_info);
  BasicPort module_port(port_name, port_size); 
  /* Add generated ports to the ModuleManager */
  ModulePortId port_id = module_manager.add_port(module_id, module_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add pre-processing flag if defined */
  module_manager.set_port_preproc_flag(module_id, port_id, preproc_flag);
}
