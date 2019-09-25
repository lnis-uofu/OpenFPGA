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
                                                          const CircuitLibrary& circuit_lib,
                                                          const CircuitModelId& sram_model,
                                                          const e_sram_orgz sram_orgz_type,
                                                          const std::string& preproc_flag,
                                                          const size_t& port_size) {
  /* Create a port */
  std::string port_name = generate_formal_verification_sram_port_name(circuit_lib, sram_model);
  BasicPort module_port(port_name, port_size); 
  /* Add generated ports to the ModuleManager */
  ModulePortId port_id = module_manager.add_port(module_id, module_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add pre-processing flag if defined */
  module_manager.set_port_preproc_flag(module_id, port_id, preproc_flag);
}

/********************************************************************
 * Add a list of ports that are used for SRAM configuration to a module
 * in the module manager
 * The type and names of added ports strongly depend on the 
 * organization of SRAMs.
 * 1. Standalone SRAMs: 
 *    two ports will be added, which are regular output and inverted output 
 * 2. Scan-chain Flip-flops:
 *    two ports will be added, which are the head of scan-chain 
 *    and the tail of scan-chain
 * 3. Memory decoders:
 *    2-4 ports will be added, depending on the ports available in the SRAM
 *    Among these, two ports are mandatory: BL and WL 
 *    The other two ports are optional: BLB and WLB
 *    Note that the constraints are correletated to the checking rules 
 *    in check_circuit_library()
 ********************************************************************/
void add_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                      const ModuleId& module_id,
                                      const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& sram_model,
                                      const e_sram_orgz sram_orgz_type,
                                      const size_t& port_size) {
  /* Prepare a list of port types to be added, the port type will be used to create port names */
  std::vector<e_spice_model_port_type> model_port_types; 
  /* Prepare a list of module port types to be added, the port type will be used to specify the port type in Verilog/SPICE module */
  std::vector<ModuleManager::e_module_port_type> module_port_types; 
  /* Actual port size may be different from user specification. Think about SCFF */
  size_t sram_port_size = port_size;

  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE: 
    model_port_types.push_back(SPICE_MODEL_PORT_INPUT);
    module_port_types.push_back(ModuleManager::MODULE_INPUT_PORT);
    model_port_types.push_back(SPICE_MODEL_PORT_OUTPUT);
    module_port_types.push_back(ModuleManager::MODULE_INPUT_PORT);
    break;
  case SPICE_SRAM_SCAN_CHAIN: 
    model_port_types.push_back(SPICE_MODEL_PORT_INPUT);
    module_port_types.push_back(ModuleManager::MODULE_INPUT_PORT);
    model_port_types.push_back(SPICE_MODEL_PORT_OUTPUT);
    module_port_types.push_back(ModuleManager::MODULE_OUTPUT_PORT);
    /* SCFF head/tail are single-bit ports */
    sram_port_size = 1;
    break;
  case SPICE_SRAM_MEMORY_BANK: {
    std::vector<e_spice_model_port_type> ports_to_search;
    ports_to_search.push_back(SPICE_MODEL_PORT_BL);
    ports_to_search.push_back(SPICE_MODEL_PORT_WL);
    ports_to_search.push_back(SPICE_MODEL_PORT_BLB);
    ports_to_search.push_back(SPICE_MODEL_PORT_WLB);
    /* Try to find a BL/WL/BLB/WLB port and update the port types/module port types to be added */
    for (const auto& port_to_search : ports_to_search) {
      std::vector<CircuitPortId> found_port = circuit_lib.model_ports_by_type(sram_model, port_to_search);
      if (0 == found_port.size()) {
        continue;
      }
      model_port_types.push_back(port_to_search);
      module_port_types.push_back(ModuleManager::MODULE_INPUT_PORT);
    }
    break;
  }
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Add ports to the module manager */
  for (size_t iport = 0; iport < model_port_types.size(); ++iport) {
    /* Create a port */
    std::string port_name = generate_sram_port_name(circuit_lib, sram_model, sram_orgz_type, model_port_types[iport]);
    BasicPort module_port(port_name, sram_port_size); 
    /* Add generated ports to the ModuleManager */
    module_manager.add_port(module_id, module_port, module_port_types[iport]);
  }
}
