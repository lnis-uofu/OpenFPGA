/******************************************************************************
 * This files includes most utilized functions 
 * for data structures for module management.
 ******************************************************************************/

#include <map>
#include <cmath>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "memory_utils.h"
#include "pb_type_utils.h"

#include "build_decoder_modules.h"
#include "circuit_library_utils.h"
#include "decoder_library_utils.h"
#include "module_manager_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************************
 * Reserved a number of module nets for a given module
 * based on the number of output ports of its child modules
 * for memory efficiency
 ******************************************************************************/
void reserve_module_manager_module_nets(ModuleManager& module_manager, 
                                        const ModuleId& parent_module) {
  size_t num_nets = 0;

  /* Collect the driver port types for parent module*/
  std::vector<ModuleManager::e_module_port_type> driver_port_types;
  driver_port_types.push_back(ModuleManager::MODULE_GLOBAL_PORT);
  driver_port_types.push_back(ModuleManager::MODULE_GPIN_PORT);
  driver_port_types.push_back(ModuleManager::MODULE_GPIO_PORT);
  driver_port_types.push_back(ModuleManager::MODULE_INOUT_PORT);
  driver_port_types.push_back(ModuleManager::MODULE_INPUT_PORT);
  driver_port_types.push_back(ModuleManager::MODULE_CLOCK_PORT);

  /* The number of nets depends on the sizes of input ports of parent module */
  for (const auto& port_type : driver_port_types) {
    for (const BasicPort& port : module_manager.module_ports_by_type(parent_module, port_type)) {
      num_nets += port.get_width();
    }
  }

  /* Collect the output port types */
  std::vector<ModuleManager::e_module_port_type> output_port_types;
  output_port_types.push_back(ModuleManager::MODULE_GPOUT_PORT);
  output_port_types.push_back(ModuleManager::MODULE_OUTPUT_PORT);
  
  for (const ModuleId& child_module : module_manager.child_modules(parent_module)) {
    /* The number of nets depends on the sizes of output ports of 
     * each instanciated child module
     */
    size_t num_instances = module_manager.num_instance(parent_module, child_module);

    /* Sum up the port sizes for all the output ports */
    size_t total_output_port_sizes = 0;
    for (const auto& port_type : output_port_types) {
      for (const BasicPort& port : module_manager.module_ports_by_type(child_module, port_type)) {
        total_output_port_sizes += port.get_width();
      }
    }

    num_nets += total_output_port_sizes * num_instances;
  }
  
  module_manager.reserve_module_nets(parent_module, num_nets);
}

/******************************************************************************
 * Count the 'actual' number of configurable children for a module in module manager
 * A 'true' configurable children should have a number of configurable children as well
 ******************************************************************************/
size_t count_module_manager_module_configurable_children(const ModuleManager& module_manager, 
                                                         const ModuleId& module) {
  size_t num_config_children = 0;

  for (const ModuleId& child : module_manager.configurable_children(module)) {
    if (0 != module_manager.configurable_children(child).size()) {
      num_config_children++;
    }
  }

  return num_config_children;
}

/******************************************************************************
 * Find the module id and instance id in module manager with a given instance name
 * This function will exhaustively search all the child module under a given parent
 * module 
 ******************************************************************************/
std::pair<ModuleId, size_t> find_module_manager_instance_module_info(const ModuleManager& module_manager,
                                                                     const ModuleId& parent, 
                                                                     const std::string& instance_name) {
  /* Deposit invalid values as default */
  std::pair<ModuleId, size_t> instance_info(ModuleId::INVALID(), 0);
  
  /* Search all the child module and see we have a match */
  for (const ModuleId& child : module_manager.child_modules(parent)) {
    size_t child_instance = module_manager.instance_id(parent, child, instance_name); 
    if (true == module_manager.valid_module_instance_id(parent, child, child_instance)) {
      instance_info.first = child;
      instance_info.second = child_instance;
      return instance_info;
    }
  }
  
  return instance_info; 
}

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

  /* Identify module usage based on circuit type:
   * LUT, SRAM, CCFF, I/O have specific usages
   * Others will be classified as hard IPs 
   */
  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
    module_manager.set_module_usage(module, ModuleManager::MODULE_LUT);
  } else if (CIRCUIT_MODEL_SRAM == circuit_lib.model_type(circuit_model)) {
    module_manager.set_module_usage(module, ModuleManager::MODULE_CONFIG);
  } else if (CIRCUIT_MODEL_CCFF == circuit_lib.model_type(circuit_model)) {
    module_manager.set_module_usage(module, ModuleManager::MODULE_CONFIG);
  } else if (CIRCUIT_MODEL_IOPAD == circuit_lib.model_type(circuit_model)) {
    module_manager.set_module_usage(module, ModuleManager::MODULE_IO);
  } else if (CIRCUIT_MODEL_WIRE == circuit_lib.model_type(circuit_model)) {
    module_manager.set_module_usage(module, ModuleManager::MODULE_INTERC);
  } else if (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(circuit_model)) {
    module_manager.set_module_usage(module, ModuleManager::MODULE_INTERC);
  } else {
    module_manager.set_module_usage(module, ModuleManager::MODULE_HARD_IP);
  }

  /* Add ports */
  /* Find global ports and add one by one 
   * Non-I/O Global input ports will be considered as global port to be shorted wired in the context of module manager
   * I/O Global output ports will be considered as general purpose output port in the context of module manager
   * I/O Global inout ports will be considered as general purpose i/o port in the context of module manager
   */
  for (const auto& port : circuit_lib.model_global_ports(circuit_model, false)) {
    BasicPort port_info(circuit_lib.port_prefix(port), circuit_lib.port_size(port));
    ModulePortId module_port = ModulePortId::INVALID();

    if ( (CIRCUIT_MODEL_PORT_INPUT == circuit_lib.port_type(port))
      && (false == circuit_lib.port_is_io(port)) ) {
      module_port = module_manager.add_port(module, port_info, ModuleManager::MODULE_GLOBAL_PORT);  
    } else if (CIRCUIT_MODEL_PORT_CLOCK == circuit_lib.port_type(port)) {
      module_port = module_manager.add_port(module, port_info, ModuleManager::MODULE_GLOBAL_PORT);  
    } else if ( (CIRCUIT_MODEL_PORT_INPUT == circuit_lib.port_type(port))
             && (true == circuit_lib.port_is_io(port)) ) {
      module_port = module_manager.add_port(module, port_info, ModuleManager::MODULE_GPIN_PORT);  
    } else if (CIRCUIT_MODEL_PORT_OUTPUT == circuit_lib.port_type(port)) {
      VTR_ASSERT(true == circuit_lib.port_is_io(port));
      module_port = module_manager.add_port(module, port_info, ModuleManager::MODULE_GPOUT_PORT);  
    } else if ( (CIRCUIT_MODEL_PORT_INOUT == circuit_lib.port_type(port)) 
             && (true == circuit_lib.port_is_io(port)) ) {
      module_port = module_manager.add_port(module, port_info, ModuleManager::MODULE_GPIO_PORT);  
    }

    /* Specify if the port can be mapped to an data signal */
    if (true == module_manager.valid_module_port_id(module, module_port)) {
      if (true == circuit_lib.port_is_data_io(port)) {
        module_manager.set_port_is_mappable_io(module, module_port, true);
      }
    }
  }

  /* Find other ports and add one by one */
  /* Create a type-to-type map for ports */
  std::map<enum e_circuit_model_port_type, ModuleManager::e_module_port_type> port_type2type_map;
  port_type2type_map[CIRCUIT_MODEL_PORT_INOUT] = ModuleManager::MODULE_INOUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_INPUT] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_CLOCK] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_SRAM] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_BL] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_BLB] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_WL] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_WLB] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_WLR] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[CIRCUIT_MODEL_PORT_OUTPUT] = ModuleManager::MODULE_OUTPUT_PORT;

  /* Input ports (ignore all the global ports when searching the circuit_lib */
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : circuit_lib.model_ports_by_type(circuit_model, kv.first, true)) {
      BasicPort port_info(circuit_lib.port_prefix(port), circuit_lib.port_size(port));
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
  std::string blb_port_name = generate_reserved_sram_port_name(CIRCUIT_MODEL_PORT_BLB);
  BasicPort blb_module_port(blb_port_name, port_size); 
  /* Add generated ports to the ModuleManager */
  module_manager.add_port(module_id, blb_module_port, ModuleManager::MODULE_INPUT_PORT);

  /* Add a reserved BLB port to the module */
  std::string wl_port_name = generate_reserved_sram_port_name(CIRCUIT_MODEL_PORT_WL);
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
 *    two ports will be added, which are BL and WL 
 * 2. Scan-chain Flip-flops:
 *    two ports will be added, which are the head of scan-chain 
 *    and the tail of scan-chain
 *    IMPORTANT: the port size will be forced to 1 in this case 
 *               because the head and tail are both 1-bit ports!!!
 * 3. Memory decoders:
 *    2-4 ports will be added, depending on the ports available in the SRAM
 *    Among these, two ports are mandatory: BL and WL 
 *    The other two ports are optional: BLB and WLB
 *    Note that the constraints are correletated to the checking rules 
 *    in check_circuit_library()
 * 4. Frame-based memory:
 *    - An Enable signal
 *    - An address port, whose size depends on the number of config bits 
 *      and the maximum size of address ports of configurable children
 *    - An data_in port (single-bit)
 ********************************************************************/
void add_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                      const ModuleId& module_id,
                                      const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& sram_model,
                                      const e_config_protocol_type sram_orgz_type,
                                      const size_t& num_config_bits) {
  std::vector<std::string> sram_port_names = generate_sram_port_names(circuit_lib, sram_model, sram_orgz_type);
  size_t sram_port_size = generate_sram_port_size(sram_orgz_type, num_config_bits); 

  /* Add ports to the module manager */
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE: 
  case CONFIG_MEM_QL_MEMORY_BANK:
  case CONFIG_MEM_MEMORY_BANK: {
    for (const std::string& sram_port_name : sram_port_names) {
      /* Add generated ports to the ModuleManager */
      BasicPort sram_port(sram_port_name, sram_port_size);
      module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
    }
    break;
  }
  case CONFIG_MEM_SCAN_CHAIN: { 
    /* Note that configuration chain tail is an output while head is an input 
     * IMPORTANT: this is co-designed with function generate_sram_port_names()
     * If the return vector is changed, the following codes MUST be adapted!
     */
    VTR_ASSERT(2 == sram_port_names.size());
    size_t port_counter = 0;
    for (const std::string& sram_port_name : sram_port_names) {
      /* Add generated ports to the ModuleManager */
      BasicPort sram_port(sram_port_name, sram_port_size);
      if (0 == port_counter) { 
        module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
      } else {
        VTR_ASSERT(1 == port_counter);
        module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_OUTPUT_PORT);
      }
      port_counter++;
    }
    break;
  }
  case CONFIG_MEM_FRAME_BASED: { 
    BasicPort en_port(std::string(DECODER_ENABLE_PORT_NAME), 1);
    module_manager.add_port(module_id, en_port, ModuleManager::MODULE_INPUT_PORT);

    BasicPort addr_port(std::string(DECODER_ADDRESS_PORT_NAME), num_config_bits);
    module_manager.add_port(module_id, addr_port, ModuleManager::MODULE_INPUT_PORT);

    BasicPort din_port(std::string(DECODER_DATA_IN_PORT_NAME), 1);
    module_manager.add_port(module_id, din_port, ModuleManager::MODULE_INPUT_PORT);

    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization !\n");
    exit(1);
  }
}

/********************************************************************
 * @brief Add a list of ports that are used for SRAM configuration to module 
 * in the module manager
 * @note
 *   This function is only applicable to programmable blocks, which are 
 *   - Grid
 *   - CBX/CBY
 *   - SB
 * @note 
 *   The major difference between this function and the add_sram_ports_to_module_manager()
 *   is the size of sram ports to be added when QL memory bank is selected
 *   This function will merge/group BL/WLs by considering a memory bank organization
 *   at block-level
 ********************************************************************/
void add_pb_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                         const ModuleId& module_id,
                                         const CircuitLibrary& circuit_lib,
                                         const CircuitModelId& sram_model,
                                         const e_config_protocol_type sram_orgz_type,
                                         const size_t& num_config_bits) {
  std::vector<std::string> sram_port_names = generate_sram_port_names(circuit_lib, sram_model, sram_orgz_type);
  size_t sram_port_size = generate_pb_sram_port_size(sram_orgz_type, num_config_bits); 

  /* Add ports to the module manager */
  switch (sram_orgz_type) {
  case CONFIG_MEM_QL_MEMORY_BANK:
    for (const std::string& sram_port_name : sram_port_names) {
      /* Add generated ports to the ModuleManager */
      BasicPort sram_port(sram_port_name, sram_port_size);
      /* For WL and WLR ports, we need to fine-tune it */
      if ( (CIRCUIT_MODEL_PORT_WL == circuit_lib.port_type(circuit_lib.model_port(sram_model, sram_port_name)))
        || (CIRCUIT_MODEL_PORT_WLR == circuit_lib.port_type(circuit_lib.model_port(sram_model, sram_port_name))) ) {
        sram_port.set_width(find_memory_wl_decoder_data_size(num_config_bits, sram_port_size));
      }
      module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
    }
    break;
  case CONFIG_MEM_STANDALONE: 
  case CONFIG_MEM_MEMORY_BANK: {
    for (const std::string& sram_port_name : sram_port_names) {
      /* Add generated ports to the ModuleManager */
      BasicPort sram_port(sram_port_name, sram_port_size);
      module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
    }
    break;
  }
  case CONFIG_MEM_SCAN_CHAIN: { 
    /* Note that configuration chain tail is an output while head is an input 
     * IMPORTANT: this is co-designed with function generate_sram_port_names()
     * If the return vector is changed, the following codes MUST be adapted!
     */
    VTR_ASSERT(2 == sram_port_names.size());
    size_t port_counter = 0;
    for (const std::string& sram_port_name : sram_port_names) {
      /* Add generated ports to the ModuleManager */
      BasicPort sram_port(sram_port_name, sram_port_size);
      if (0 == port_counter) { 
        module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
      } else {
        VTR_ASSERT(1 == port_counter);
        module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_OUTPUT_PORT);
      }
      port_counter++;
    }
    break;
  }
  case CONFIG_MEM_FRAME_BASED: { 
    BasicPort en_port(std::string(DECODER_ENABLE_PORT_NAME), 1);
    module_manager.add_port(module_id, en_port, ModuleManager::MODULE_INPUT_PORT);

    BasicPort addr_port(std::string(DECODER_ADDRESS_PORT_NAME), num_config_bits);
    module_manager.add_port(module_id, addr_port, ModuleManager::MODULE_INPUT_PORT);

    BasicPort din_port(std::string(DECODER_DATA_IN_PORT_NAME), 1);
    module_manager.add_port(module_id, din_port, ModuleManager::MODULE_INPUT_PORT);

    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization !\n");
    exit(1);
  }
}

/********************************************************************
 * Add ports of a pb_type block to module manager
 * Port addition will follow the sequence: inout, input, output, clock
 * This will help use to keep a clean module definition when printing out
 * To avoid port mismatch between the pb_type and its linked circuit model
 * This function will also check that each pb_type port is actually exist
 * in the linked circuit model
 *******************************************************************/
void add_primitive_pb_type_ports_to_module_manager(ModuleManager& module_manager, 
                                                   const ModuleId& module_id,
                                                   t_pb_type* cur_pb_type,
                                                   const VprDeviceAnnotation& vpr_device_annotation) {
   
  /* Find the inout ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_inout_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, CIRCUIT_MODEL_PORT_INOUT, vpr_device_annotation);
  for (auto port : pb_type_inout_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_INOUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }

  /* Find the input ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_input_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, CIRCUIT_MODEL_PORT_INPUT, vpr_device_annotation);
  for (auto port : pb_type_input_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_INPUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }

  /* Find the output ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_output_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, CIRCUIT_MODEL_PORT_OUTPUT, vpr_device_annotation);
  for (auto port : pb_type_output_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_OUTPUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }

  /* Find the clock ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_clock_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, CIRCUIT_MODEL_PORT_CLOCK, vpr_device_annotation);
  for (auto port : pb_type_clock_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_CLOCK_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }
}

/********************************************************************
 * Add ports of a pb_type block to module manager
 * This function is designed for non-primitive pb_types, which are
 * NOT linked to any circuit model.
 * Actually, this makes things much simpler.
 * We just iterate over all the ports and add it to the module
 * with the naming convention
 *******************************************************************/
void add_pb_type_ports_to_module_manager(ModuleManager& module_manager, 
                                         const ModuleId& module_id,
                                         t_pb_type* cur_pb_type) {
  /* Create a type-to-type mapping between module ports and pb_type ports */
  std::map<PORTS, ModuleManager::e_module_port_type> port_type2type_map;
  port_type2type_map[IN_PORT] = ModuleManager::MODULE_INPUT_PORT;
  port_type2type_map[OUT_PORT] = ModuleManager::MODULE_OUTPUT_PORT;
  port_type2type_map[INOUT_PORT] = ModuleManager::MODULE_INOUT_PORT;

  for (int port = 0; port < cur_pb_type->num_ports; ++port) {
    t_port* pb_type_port = &(cur_pb_type->ports[port]);
    BasicPort module_port(generate_pb_type_port_name(pb_type_port), pb_type_port->num_pins);
    module_manager.add_port(module_id, module_port, port_type2type_map[pb_type_port->type]);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }
}

/********************************************************************
 * Identify if a net is a local wire inside a module: 
 * A net is a local wire if it connects between two instances,
 * It means that any of its source and sink modules should not include current module_id
 *******************************************************************/
bool module_net_is_local_wire(const ModuleManager& module_manager, 
                              const ModuleId& module_id, const ModuleNetId& module_net) {
  /* Check all the sink modules of the net, 
   * if we have a source module is the current module, this is not local wire 
   */
  for (ModuleId src_module : module_manager.net_source_modules(module_id, module_net)) {
    if (module_id == src_module) {
      /* Here, this is not a local wire */
      return false;
    }
  }

  /* Check all the sink modules of the net */
  for (ModuleId sink_module : module_manager.net_sink_modules(module_id, module_net)) {
    if (module_id == sink_module) {
      /* Here, this is not a local wire */
      return false;
    }
  }

  return true;
}

/********************************************************************
 * Identify if a net is an output short connection inside a module: 
 * The short connection is defined as the direct connection
 * between two outputs port of the module
 *
 *            module
 *            +-----------------------------+
 *                                          |
 *               src------>+--------------->|--->outputA
 *                         |                |
 *                         |                |
 *                         +--------------->|--->outputB
 *            +-----------------------------+

 *******************************************************************/
bool module_net_include_output_short_connection(const ModuleManager& module_manager, 
                                                const ModuleId& module_id, const ModuleNetId& module_net) {
  /* Check all the sink modules of the net */
  size_t contain_num_module_output = 0;
  for (ModuleId sink_module : module_manager.net_sink_modules(module_id, module_net)) {
    if (module_id == sink_module) {
      contain_num_module_output++;
    }
  }
 
  /* If we have found more than 1 module outputs, it indicated output short connection! */
  return (1 < contain_num_module_output);
}

/********************************************************************
 * Identify if a net is a local short connection inside a module: 
 * The short connection is defined as the direct connection
 * between an input port of the module and an output port of the module
 *
 *             module
 *            +-----------------------------+
 *            |                             |
 *  inputA--->|---------------------------->|--->outputB
 *            |                             |
 *            |                             |
 *            |                             |
 *            +-----------------------------+
 *******************************************************************/
bool module_net_include_local_short_connection(const ModuleManager& module_manager, 
                                               const ModuleId& module_id, const ModuleNetId& module_net) {
  /* Check all the sink modules of the net, 
   * if we have a source module is the current module, this is not local wire 
   */
  bool contain_module_input = false;
  for (ModuleId src_module : module_manager.net_source_modules(module_id, module_net)) {
    if (module_id == src_module) {
      contain_module_input = true;
      break;
    }
  }

  /* Check all the sink modules of the net */
  bool contain_module_output = false;
  for (ModuleId sink_module : module_manager.net_sink_modules(module_id, module_net)) {
    if (module_id == sink_module) {
      contain_module_output = true;
      break;
    }
  }

  return contain_module_input & contain_module_output;
}

/********************************************************************
 * Add the port-to-port connection between a pb_type and its linked circuit model 
 * This function is mainly used to create instance of the module for a pb_type
 *
 * Note: this function SHOULD be called after the pb_type_module is created
 * and its child module is created! 
 *******************************************************************/
void add_primitive_pb_type_module_nets(ModuleManager& module_manager,
                                       const ModuleId& pb_type_module,
                                       const ModuleId& child_module,
                                       const size_t& child_instance_id,
                                       const CircuitLibrary& circuit_lib,
                                       t_pb_type* cur_pb_type,
                                       const VprDeviceAnnotation& vpr_device_annotation) {
  for (int iport = 0; iport < cur_pb_type->num_ports; ++iport) {
    t_port* pb_type_port = &(cur_pb_type->ports[iport]);
    /* Must have a linked circuit model port */
    VTR_ASSERT( CircuitPortId::INVALID() != vpr_device_annotation.pb_circuit_port(pb_type_port));

    /* Find the source port in pb_type module */
    /* Get the src module port id */
    ModulePortId src_module_port_id = module_manager.find_module_port(pb_type_module, generate_pb_type_port_name(pb_type_port));
    VTR_ASSERT(ModulePortId::INVALID() != src_module_port_id);
    BasicPort src_port = module_manager.module_port(pb_type_module, src_module_port_id);

    /* Get the des module port id */
    std::string des_module_port_name = circuit_lib.port_prefix(vpr_device_annotation.pb_circuit_port(pb_type_port));
    ModulePortId des_module_port_id = module_manager.find_module_port(child_module, des_module_port_name);
    VTR_ASSERT(ModulePortId::INVALID() != des_module_port_id);
    BasicPort des_port = module_manager.module_port(child_module, des_module_port_id);

    /* Port size must match */
    VTR_ASSERT(src_port.get_width() == des_port.get_width());

    /* For each pin, generate the nets.
     * For non-output ports (input ports, inout ports and clock ports),
     * src_port is the source of the net   
     * For output ports
     * src_port is the sink of the net
     */
    switch (pb_type_port->type) {
    case IN_PORT:
    case INOUT_PORT:
      for (size_t pin_id = 0; pin_id < src_port.pins().size(); ++pin_id) {
        ModuleNetId net = module_manager.create_module_net(pb_type_module);
        /* Add net source */
        module_manager.add_module_net_source(pb_type_module, net, pb_type_module, 0, src_module_port_id, src_port.pins()[pin_id]);
        /* Add net sink */
        module_manager.add_module_net_sink(pb_type_module, net, child_module, child_instance_id, des_module_port_id, des_port.pins()[pin_id]);
      }
      break;
    case OUT_PORT: 
      for (size_t pin_id = 0; pin_id < src_port.pins().size(); ++pin_id) {
        ModuleNetId net = module_manager.create_module_net(pb_type_module);
        /* Add net source */
        module_manager.add_module_net_source(pb_type_module, net, child_module, child_instance_id, des_module_port_id, des_port.pins()[pin_id]);
        /* Add net sink */
        module_manager.add_module_net_sink(pb_type_module, net, pb_type_module, 0, src_module_port_id, src_port.pins()[pin_id]);
      }
      break;
    default:
      VTR_LOG_ERROR("Invalid port of pb_type!\n");
      exit(1);
    }
  } 
}

/********************************************************************
 * Add the port-to-port connection between a logic module 
 * and a memory module 
 * Create nets to wire SRAM ports between logic module and memory module  
 *
 * The information about SRAM ports of logic module are stored in the
 * mem_output_bus_ports, where element [0] denotes the SRAM port while
 * element [1] denotes the SRAMb port
 *
 *         +---------+                          +--------+
 *         |         | regular SRAM port        |        |
 *         |  Logic  |-----------------------+  | Memory |
 *         | Module  | mode-select SRAM port |->| Module |
 *         |         |-----------------------+  |        |
 *         +---------+                          +--------+
 *
 * There could be multiple SRAM ports of logic module, which are wired to
 * the SRAM ports of memory module
 *
 * Note: this function SHOULD be called after the pb_type_module is created
 * and its child module (logic_module and memory_module) is created! 
 *
 * Note: this function only handle either SRAM or SRAMb ports.
 *       So, this function may be called twice to complete the wiring 
 *******************************************************************/
static 
void add_module_nets_between_logic_and_memory_sram_ports(ModuleManager& module_manager,
                                                         const ModuleId& parent_module,
                                                         const ModuleId& logic_module,
                                                         const size_t& logic_instance_id,
                                                         const ModuleId& memory_module,
                                                         const size_t& memory_instance_id, 
                                                         const std::vector<ModulePortId>& logic_module_sram_port_ids,
                                                         const ModulePortId& mem_module_sram_port_id) {
  /* Find mem_output_bus ports in logic module */
  std::vector<BasicPort> logic_module_sram_ports;
  for (const ModulePortId& logic_module_sram_port_id : logic_module_sram_port_ids) {
    logic_module_sram_ports.push_back(module_manager.module_port(logic_module, logic_module_sram_port_id));
  }
 
  /* Create a list of virtual ports to align with the SRAM port of logic module
   * Physical ports:
   *
   *      logic_module_sram_port[0]   logic_module_sram_port[1]
   *
   *      LSB[0]------------>MSB[0]   LSB------------------>MSB 
   *
   *      memory_sram_port
   *      LSBY---------------------------------------------->MSBY  
   *
   * Virtual ports:
   *      mem_module_sram_port[0]     mem_module_sram_port[1]
   *      LSBY--------------->MSBX    MSBX+1------------------>MSBY 
   *
   */
  BasicPort mem_module_port = module_manager.module_port(memory_module, mem_module_sram_port_id);
  std::vector<BasicPort> virtual_mem_module_ports;

  /* Create a counter for the LSB of virtual ports */
  size_t port_lsb = 0;
  for (const BasicPort& logic_module_sram_port : logic_module_sram_ports) {
    BasicPort virtual_port;
    virtual_port.set_name(mem_module_port.get_name());
    virtual_port.set_width(port_lsb, port_lsb + logic_module_sram_port.get_width() - 1);
    virtual_mem_module_ports.push_back(virtual_port);
    port_lsb = virtual_port.get_msb() + 1;
  }
  /* port_lsb should be aligned with the MSB of memory_sram_port */
  VTR_ASSERT(port_lsb == mem_module_port.get_msb() + 1);

  /* Wire port to port */
  for (size_t port_index = 0; port_index < logic_module_sram_ports.size(); ++port_index) {
    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < logic_module_sram_ports[port_index].pins().size(); ++pin_id) {
      ModuleNetId net = module_manager.create_module_net(parent_module);
      /* TODO: Give a name to make it clear */
      std::string net_name = module_manager.module_name(logic_module) + std::string("_") + std::to_string(logic_instance_id) + std::string("_") + logic_module_sram_ports[port_index].get_name();
      module_manager.set_net_name(parent_module, net, net_name);
      /* Add net source */
      module_manager.add_module_net_source(parent_module, net, logic_module, logic_instance_id, logic_module_sram_port_ids[port_index], logic_module_sram_ports[port_index].pins()[pin_id]);
      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, memory_module, memory_instance_id, mem_module_sram_port_id, virtual_mem_module_ports[port_index].pins()[pin_id]);
    }
  }
}

/********************************************************************
 * Add the port-to-port connection between a logic module 
 * and a memory module 
 * Create nets to wire SRAM ports between logic module and memory module  
 *
 *
 *         +---------+                        +--------+
 *         |         |  SRAM ports            |        |
 *         |  Logic  |----------------------->| Memory |
 *         | Module  |  SRAMb ports           | Module |
 *         |         |----------------------->|        |
 *         +---------+                        +--------+
 *
 * Note: this function SHOULD be called after the pb_type_module is created
 * and its child module (logic_module and memory_module) is created! 
 *
 *******************************************************************/
void add_module_nets_between_logic_and_memory_sram_bus(ModuleManager& module_manager,
                                                       const ModuleId& parent_module,
                                                       const ModuleId& logic_module,
                                                       const size_t& logic_instance_id,
                                                       const ModuleId& memory_module,
                                                       const size_t& memory_instance_id, 
                                                       const CircuitLibrary& circuit_lib,
                                                       const CircuitModelId& logic_model) {

  /* Connect SRAM port */
  /* Find SRAM ports in the circuit model for logic module */
  std::vector<std::string> logic_model_sram_port_names;
  /* Regular sram port goes first */
  for (CircuitPortId regular_sram_port : find_circuit_regular_sram_ports(circuit_lib, logic_model)) {
    logic_model_sram_port_names.push_back(circuit_lib.port_prefix(regular_sram_port));
  }
  /* Mode-select sram port goes first */
  for (CircuitPortId mode_select_sram_port : find_circuit_mode_select_sram_ports(circuit_lib, logic_model)) {
    logic_model_sram_port_names.push_back(circuit_lib.port_prefix(mode_select_sram_port));
  }
  /* Find the port ids in the memory */
  std::vector<ModulePortId> logic_module_sram_port_ids;
  for (const std::string& logic_model_sram_port_name : logic_model_sram_port_names) {
    /* Skip non-exist ports */
    if (ModulePortId::INVALID() == module_manager.find_module_port(logic_module, logic_model_sram_port_name)) {
      continue;
    }
    logic_module_sram_port_ids.push_back(module_manager.find_module_port(logic_module, logic_model_sram_port_name));
  }

  /* Get the SRAM port name of memory model */
  /* This should be a constant expression and it should be the same for all the memory module! */
  std::string memory_model_sram_port_name = generate_configurable_memory_data_out_name();
  /* Find the corresponding ports in memory module */ 
  ModulePortId mem_module_sram_port_id = module_manager.find_module_port(memory_module, memory_model_sram_port_name);

  /* Do wiring only when we have sram ports */
  if ( (false == logic_module_sram_port_ids.empty())
    || (ModulePortId::INVALID() == mem_module_sram_port_id) ) {
    add_module_nets_between_logic_and_memory_sram_ports(module_manager, parent_module, 
                                                        logic_module, logic_instance_id,
                                                        memory_module, memory_instance_id,
                                                        logic_module_sram_port_ids, mem_module_sram_port_id);
  }

  /* Connect SRAMb port */
  /* Find SRAM ports in the circuit model for logic module */
  std::vector<std::string> logic_model_sramb_port_names;
  /* Regular sram port goes first */
  for (CircuitPortId regular_sram_port : find_circuit_regular_sram_ports(circuit_lib, logic_model)) {
    logic_model_sramb_port_names.push_back(circuit_lib.port_prefix(regular_sram_port) + std::string(INV_PORT_POSTFIX));
  }
  /* Mode-select sram port goes first */
  for (CircuitPortId mode_select_sram_port : find_circuit_mode_select_sram_ports(circuit_lib, logic_model)) {
    logic_model_sramb_port_names.push_back(circuit_lib.port_prefix(mode_select_sram_port) + std::string(INV_PORT_POSTFIX));
  }
  /* Find the port ids in the memory */
  std::vector<ModulePortId> logic_module_sramb_port_ids;
  for (const std::string& logic_model_sramb_port_name : logic_model_sramb_port_names) {
    /* Skip non-exist ports */
    if (ModulePortId::INVALID() == module_manager.find_module_port(logic_module, logic_model_sramb_port_name)) {
      continue;
    }
    logic_module_sramb_port_ids.push_back(module_manager.find_module_port(logic_module, logic_model_sramb_port_name));
  }

  /* Get the SRAM port name of memory model */
  std::string memory_model_sramb_port_name = generate_configurable_memory_inverted_data_out_name();
  /* Find the corresponding ports in memory module */ 
  ModulePortId mem_module_sramb_port_id = module_manager.find_module_port(memory_module, memory_model_sramb_port_name);

  /* Do wiring only when we have sramb ports */
  if ( (false == logic_module_sramb_port_ids.empty())
    && (ModulePortId::INVALID() != mem_module_sramb_port_id) ) {
    add_module_nets_between_logic_and_memory_sram_ports(module_manager, parent_module, 
                                                        logic_module, logic_instance_id,
                                                        memory_module, memory_instance_id,
                                                        logic_module_sramb_port_ids, mem_module_sramb_port_id);
  }
}

/********************************************************************
 * Connect all the memory modules under the parent module in a flatten way
 *
 *   BL
 *   |
 *   +---------------+--------- ... --------+
 *   |               |                      |
 *   v               v                      v
 *  +--------+    +--------+            +--------+
 *  | Memory |    | Memory |    ...     | Memory |               
 *  | Module |    | Module |            | Module |
 *  |   [0]  |    |   [1]  |            |  [N-1] |             
 *  +--------+    +--------+            +--------+
 *      ^            ^                      ^
 *      |            |                      |
 *      +------------+----------------------+
 *      |
 *     WL/WLR
 * 
 * Note: 
 *   - This function will do the connection for only one type of the port,
 *      either BL or WL. So, you should call this function twice to complete 
 *      the configuration bus connection!!!
 *
 *********************************************************************/
void add_module_nets_cmos_flatten_memory_config_bus(ModuleManager& module_manager,
                                                    const ModuleId& parent_module,
                                                    const e_config_protocol_type& sram_orgz_type,
                                                    const e_circuit_model_port_type& config_port_type) {
  /* A counter for the current pin id for the source port of parent module */
  size_t cur_src_pin_id = 0;

  /* Find the port name of parent module */
  std::string src_port_name = generate_sram_port_name(sram_orgz_type, config_port_type);
  ModuleId net_src_module_id = parent_module; 
  size_t net_src_instance_id = 0;
  ModulePortId net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

  /* We may not be able to find WLR port, return now */
  if (!net_src_port_id) {
    return;
  }

  /* Get the pin id for source port */
  BasicPort net_src_port = module_manager.module_port(net_src_module_id, net_src_port_id); 

  for (size_t mem_index = 0; mem_index < module_manager.configurable_children(parent_module).size(); ++mem_index) {
    ModuleId net_sink_module_id;
    size_t net_sink_instance_id;
    ModulePortId net_sink_port_id;

    /* Find the port name of next memory module */
    std::string sink_port_name = generate_sram_port_name(sram_orgz_type, config_port_type);
    net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
    net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
    net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 

    /* Get the pin id for sink port */
    BasicPort net_sink_port = module_manager.module_port(net_sink_module_id, net_sink_port_id); 
    
    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_sink_port.pins().size(); ++pin_id) {
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(module_manager, parent_module, 
                                                     net_src_module_id, net_src_instance_id, 
                                                     net_src_port_id, net_src_port.pins()[cur_src_pin_id]);
      VTR_ASSERT(ModuleNetId::INVALID() != net);

      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);

      /* Move to the next src pin */
      cur_src_pin_id++;
    }
  }

  /* We should used all the pins of the source port!!! */
  VTR_ASSERT(net_src_port.get_width() == cur_src_pin_id);
}

/********************************************************************
 * @brief Connect all the Bit Lines (BL) of child memory modules under the 
 * parent module in a memory bank organization
 *
 *   BL<0>          BL<1>                  BL<i>
 *   |               |                      |
 *   v               v                      v
 *  +--------+    +--------+            +--------+
 *  | Memory |    | Memory |    ...     | Memory |               
 *  | Module |    | Module |            | Module |
 *  | [0,0]  |    | [1,0]  |            | [i,0]  |             
 *  +--------+    +--------+            +--------+
 *   |               |                      |
 *   v               v                      v
 *  +--------+    +--------+            +--------+
 *  | Memory |    | Memory |    ...     | Memory |               
 *  | Module |    | Module |            | Module |
 *  | [0,1]  |    | [1,1]  |            | [i,1]  |             
 *  +--------+    +--------+            +--------+
 *
 *********************************************************************/
void add_module_nets_cmos_memory_bank_bl_config_bus(ModuleManager& module_manager,
                                                    const ModuleId& parent_module,
                                                    const e_config_protocol_type& sram_orgz_type,
                                                    const e_circuit_model_port_type& config_port_type) {
  /* A counter for the current pin id for the source port of parent module */
  size_t cur_src_pin_id = 0;

  /* Find the port name of parent module */
  std::string src_port_name = generate_sram_port_name(sram_orgz_type, config_port_type);
  ModuleId net_src_module_id = parent_module; 
  size_t net_src_instance_id = 0;
  ModulePortId net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

  /* Get the pin id for source port */
  BasicPort net_src_port = module_manager.module_port(net_src_module_id, net_src_port_id); 

  for (size_t mem_index = 0; mem_index < module_manager.configurable_children(parent_module).size(); ++mem_index) {
    /* Find the port name of next memory module */
    std::string sink_port_name = generate_sram_port_name(sram_orgz_type, config_port_type);
    ModuleId net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
    size_t net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
    ModulePortId net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 

    /* Get the pin id for sink port */
    BasicPort net_sink_port = module_manager.module_port(net_sink_module_id, net_sink_port_id); 
    
    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_sink_port.pins().size(); ++pin_id) {
      size_t cur_bl_src_pin_id = cur_src_pin_id % net_src_port.pins().size();  
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(module_manager, parent_module, 
                                                     net_src_module_id, net_src_instance_id, 
                                                     net_src_port_id, net_src_port.pins()[cur_bl_src_pin_id]);
      VTR_ASSERT(ModuleNetId::INVALID() != net);

      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);

      /* Move to the next src pin */
      cur_src_pin_id++;
    }
  }
}

/********************************************************************
 * @brief Connect all the Word Lines (WL) of child memory modules under the 
 * parent module in a memory bank organization
 *
 *  +--------+    +--------+            +--------+
 *  | Memory |    | Memory |    ...     | Memory |               
 *  | Module |    | Module |            | Module |
 *  | [0,0]  |    | [1,0]  |            | [i,0]  |             
 *  +--------+    +--------+            +--------+
 *      ^            ^                      ^
 *      |            |                      |
 *      +------------+----------------------+
 *      |
 *     WL<0>/WLR<0>
 *
 *  +--------+    +--------+            +--------+
 *  | Memory |    | Memory |    ...     | Memory |               
 *  | Module |    | Module |            | Module |
 *  | [0,1]  |    | [1,1]  |            | [i,1]  |             
 *  +--------+    +--------+            +--------+
 *      ^            ^                      ^
 *      |            |                      |
 *      +------------+----------------------+
 *      |
 *     WL<1>/WLR<1>
 *
 *********************************************************************/
void add_module_nets_cmos_memory_bank_wl_config_bus(ModuleManager& module_manager,
                                                    const ModuleId& parent_module,
                                                    const e_config_protocol_type& sram_orgz_type,
                                                    const e_circuit_model_port_type& config_port_type) {
  /* A counter for the current pin id for the source port of parent module */
  size_t cur_src_pin_id = 0;

  /* Find the port name of parent module */
  std::string src_port_name = generate_sram_port_name(sram_orgz_type, config_port_type);
  std::string bl_port_name = generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_BL);

  ModuleId net_src_module_id = parent_module; 
  size_t net_src_instance_id = 0;
  ModulePortId net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 
  ModulePortId net_bl_port_id = module_manager.find_module_port(net_src_module_id, bl_port_name); 

  /* We may not be able to find WLR port, return now */
  if (!net_src_port_id) {
    return;
  }

  /* Get the pin id for source port */
  BasicPort net_src_port = module_manager.module_port(net_src_module_id, net_src_port_id); 
  BasicPort net_bl_port = module_manager.module_port(net_src_module_id, net_bl_port_id); 

  for (size_t mem_index = 0; mem_index < module_manager.configurable_children(parent_module).size(); ++mem_index) {
    /* Find the port name of next memory module */
    std::string sink_port_name = generate_sram_port_name(sram_orgz_type, config_port_type);
    ModuleId net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
    size_t net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
    ModulePortId net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 

    /* Get the pin id for sink port */
    BasicPort net_sink_port = module_manager.module_port(net_sink_module_id, net_sink_port_id); 
    
    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_sink_port.pins().size(); ++pin_id) {
      size_t cur_wl_src_pin_id = std::floor(cur_src_pin_id / net_bl_port.pins().size());  
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(module_manager, parent_module, 
                                                     net_src_module_id, net_src_instance_id, 
                                                     net_src_port_id, net_src_port.pins()[cur_wl_src_pin_id]);
      VTR_ASSERT(ModuleNetId::INVALID() != net);

      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);

      /* Move to the next src pin */
      cur_src_pin_id++;
    }
  }
}

/********************************************************************
 * Connect all the memory modules under the parent module in a chain
 * 
 *                +--------+    +--------+            +--------+
 *  ccff_head --->| Memory |--->| Memory |--->... --->| Memory |----> ccff_tail
 *                | Module |    | Module |            | Module |
 *                |   [0]  |    |   [1]  |            |  [N-1] |             
 *                +--------+    +--------+            +--------+
 *  For the 1st memory module:
 *    net source is the configuration chain head of the primitive module
 *    net sink is the configuration chain head of the next memory module
 *
 *  For the rest of memory modules:
 *    net source is the configuration chain tail of the previous memory module
 *    net sink is the configuration chain head of the next memory module
 *********************************************************************/
void add_module_nets_cmos_memory_chain_config_bus(ModuleManager& module_manager,
                                                  const ModuleId& parent_module,
                                                  const e_config_protocol_type& sram_orgz_type) {
  for (size_t mem_index = 0; mem_index < module_manager.configurable_children(parent_module).size(); ++mem_index) {
    ModuleId net_src_module_id;
    size_t net_src_instance_id;
    ModulePortId net_src_port_id;

    ModuleId net_sink_module_id;
    size_t net_sink_instance_id;
    ModulePortId net_sink_port_id;

    if (0 == mem_index) {
      /* Find the port name of configuration chain head */
      std::string src_port_name = generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_INPUT);
      net_src_module_id = parent_module; 
      net_src_instance_id = 0;
      net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

      /* Find the port name of next memory module */
      std::string sink_port_name = generate_configuration_chain_head_name();
      net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
      net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
      net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 
    } else {
      /* Find the port name of previous memory module */
      std::string src_port_name = generate_configuration_chain_tail_name();
      net_src_module_id = module_manager.configurable_children(parent_module)[mem_index - 1]; 
      net_src_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index - 1];
      net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

      /* Find the port name of next memory module */
      std::string sink_port_name = generate_configuration_chain_head_name();
      net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
      net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
      net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 
    }

    /* Get the pin id for source port */
    BasicPort net_src_port = module_manager.module_port(net_src_module_id, net_src_port_id); 
    /* Get the pin id for sink port */
    BasicPort net_sink_port = module_manager.module_port(net_sink_module_id, net_sink_port_id); 
    /* Port sizes of source and sink should match */
    VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());
    
    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(module_manager, parent_module, net_src_module_id, net_src_instance_id, net_src_port_id, net_src_port.pins()[pin_id]);
      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);
    }
  }

  /* For the last memory module:
   *    net source is the configuration chain tail of the previous memory module
   *    net sink is the configuration chain tail of the primitive module
   */
  /* Find the port name of previous memory module */
  std::string src_port_name = generate_configuration_chain_tail_name();
  ModuleId net_src_module_id = module_manager.configurable_children(parent_module).back(); 
  size_t net_src_instance_id = module_manager.configurable_child_instances(parent_module).back();
  ModulePortId net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

  /* Find the port name of next memory module */
  std::string sink_port_name = generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_OUTPUT);
  ModuleId net_sink_module_id = parent_module; 
  size_t net_sink_instance_id = 0;
  ModulePortId net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 

  /* Get the pin id for source port */
  BasicPort net_src_port = module_manager.module_port(net_src_module_id, net_src_port_id); 
  /* Get the pin id for sink port */
  BasicPort net_sink_port = module_manager.module_port(net_sink_module_id, net_sink_port_id); 
  /* Port sizes of source and sink should match */
  VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());
  
  /* Create a net for each pin */
  for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
    /* Create a net and add source and sink to it */
    ModuleNetId net = create_module_source_pin_net(module_manager, parent_module, net_src_module_id, net_src_instance_id, net_src_port_id, net_src_port.pins()[pin_id]);
    /* Add net sink */
    module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);
  }
}

/********************************************************************
 * This function will create nets for the following types of connections:
 *  - Connect the enable signal to the EN of memory module
 *  - Connect the address port to the address port of memory module
 *  - Connect the data_in (Din) to the data_in of the memory module
 *
 *       EN      ADDR      DATA_IN
 *        |        |          |
 *        v        v          v                              
 *   +-----------------------------+
 *   |        Memory Module        |
 *   |           [0]               |
 *   |                             |
 *   +-----------------------------+
 *
 * Note:
 *  - This function is ONLY applicable to single configurable child case!!!
 *
 *********************************************************************/
static 
void add_module_nets_cmos_memory_frame_short_config_bus(ModuleManager& module_manager,
                                                        const ModuleId& parent_module) {
  std::vector<ModuleId> configurable_children = module_manager.configurable_children(parent_module);

  VTR_ASSERT(1 == configurable_children.size());
  ModuleId child_module = configurable_children[0]; 

  /* Connect the enable (EN) port of the parent module
   * to the EN port of memory module
   */
  ModulePortId parent_en_port = module_manager.find_module_port(parent_module, std::string(DECODER_ENABLE_PORT_NAME));
  ModulePortId child_en_port = module_manager.find_module_port(child_module, std::string(DECODER_ENABLE_PORT_NAME));
  add_module_bus_nets(module_manager, parent_module,
                      parent_module, 0, parent_en_port,
                      child_module, 0, child_en_port);

  /* Connect the address port of the parent module to the child module address port */
  ModulePortId parent_addr_port = module_manager.find_module_port(parent_module, std::string(DECODER_ADDRESS_PORT_NAME));
  ModulePortId child_addr_port = module_manager.find_module_port(child_module, std::string(DECODER_ADDRESS_PORT_NAME));
  add_module_bus_nets(module_manager, parent_module,
                      parent_module, 0, parent_addr_port,
                      child_module, 0, child_addr_port);

  /* Connect the data_in (Din) of parent module to the data_in of the memory module
   */
  ModulePortId parent_din_port = module_manager.find_module_port(parent_module, std::string(DECODER_DATA_IN_PORT_NAME));
  ModulePortId child_din_port = module_manager.find_module_port(child_module, std::string(DECODER_DATA_IN_PORT_NAME));
  add_module_bus_nets(module_manager, parent_module,
                      parent_module, 0, parent_din_port,
                      child_module, 0, child_din_port);
}

/********************************************************************
 * This function will 
 * - Add a frame decoder to the parent module 
 *   - If the decoder exists in the library, we use the module
 *   - If the decoder does not exist, we create a new module and use it
 * - create nets for the following types of connections:
 *   - Connect the EN signal, first few bits of address of parent module 
 *     to the frame decoder inputs
 *   - Connect the enable (EN) port of memory modules under the parent module
 *     to the frame decoder outputs
 *   - Connect the data_in (Din) of parent module to the data_in of the all
 *     the memory modules
 *
 *       EN      ADDR[X - 1: X - log(N)/log2]
 *        |        |
 *        v        v                                        
 *  +--------------------------------------------+
 *  |             Frame-based decoder            |
 *  |                                            |
 *  |                Data out                    |
 *  +--------------------------------------------+
 *                     |
 *       +-------------+--------------------+
 *       |             |                    |  
 *   Din |         Din |                Din |  
 *    |  |          |  |                 |  |
 *    v  v          v  v                 v  v
 *  +--------+    +--------+            +--------+
 *  | Memory |    | Memory |    ...     | Memory |
 *  | Module |    | Module |            | Module |
 *  |   [0]  |    |   [1]  |            |  [N-1] |             
 *  +--------+    +--------+            +--------+
 *       ^             ^                    ^
 *       |             |                    |
 *       +-------------+--------------------+
 *                     |
 *                  ADDR[X - log(N)/log2 - 1: 0]
 *
 * Note:
 *  - X is the port size of address port of the parent module
 *  - the address port of child memory modules may be smaller than 
 *    X - log(N)/log2. In such case, we will drop the MSBs until it fit
 *  - This function is only applicable to 2+ configurable children!!!
 *
 *********************************************************************/
static 
void add_module_nets_cmos_memory_frame_decoder_config_bus(ModuleManager& module_manager,
                                                          DecoderLibrary& decoder_lib,
                                                          const ModuleId& parent_module) {
  std::vector<ModuleId> configurable_children = module_manager.configurable_children(parent_module);

  /* Find the decoder specification */
  size_t addr_size = find_mux_local_decoder_addr_size(configurable_children.size());
  /* Data input should match the WL (data_in) of a SRAM */
  size_t data_size = configurable_children.size(); 

  /* Search the decoder library and try to find one 
   * If not found, create a new module and add it to the module manager 
   */
  DecoderId decoder_id = decoder_lib.find_decoder(addr_size, data_size, true, false, false, false);
  if (DecoderId::INVALID() == decoder_id) {
    decoder_id = decoder_lib.add_decoder(addr_size, data_size, true, false, false, false);
  }
  VTR_ASSERT(DecoderId::INVALID() != decoder_id);

  /* Create a module if not existed yet */
  std::string decoder_module_name = generate_memory_decoder_subckt_name(addr_size, data_size);
  ModuleId decoder_module = module_manager.find_module(decoder_module_name);
  if (ModuleId::INVALID() == decoder_module) {
    decoder_module = build_frame_memory_decoder_module(module_manager,
                                                       decoder_lib,
                                                       decoder_id);
  }
  VTR_ASSERT(ModuleId::INVALID() != decoder_module);

  /* Instanciate the decoder module here */
  VTR_ASSERT(0 == module_manager.num_instance(parent_module, decoder_module));
  module_manager.add_child_module(parent_module, decoder_module);

  /* Connect the enable (EN) port of memory modules under the parent module
   * to the frame decoder inputs
   */
  ModulePortId parent_en_port = module_manager.find_module_port(parent_module, std::string(DECODER_ENABLE_PORT_NAME));
  ModulePortId decoder_en_port = module_manager.find_module_port(decoder_module, std::string(DECODER_ENABLE_PORT_NAME));
  add_module_bus_nets(module_manager, parent_module,
                      parent_module, 0, parent_en_port,
                      decoder_module, 0, decoder_en_port);

  /* Connect the address port of the parent module to the frame decoder address port
   * Note that we only connect to the first few bits of address port
   */
  ModulePortId parent_addr_port = module_manager.find_module_port(parent_module, std::string(DECODER_ADDRESS_PORT_NAME));
  ModulePortId decoder_addr_port = module_manager.find_module_port(decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
  BasicPort parent_addr_port_info = module_manager.module_port(parent_module, parent_addr_port);
  BasicPort decoder_addr_port_info = module_manager.module_port(decoder_module, decoder_addr_port);
  for (size_t ipin = 0; ipin < decoder_addr_port_info.get_width(); ++ipin) {
    ModuleNetId net = module_manager.module_instance_port_net(parent_module,
                                                              parent_module, 0, 
                                                              parent_addr_port,
                                                              parent_addr_port_info.get_msb() - ipin);
    if (ModuleNetId::INVALID() == net) { 
      net = module_manager.create_module_net(parent_module);
      /* Configure the net source */
      module_manager.add_module_net_source(parent_module, net,
                                           parent_module, 0,
                                           parent_addr_port,
                                           parent_addr_port_info.get_msb() - ipin);
    }
    /* Configure the net sink */
    module_manager.add_module_net_sink(parent_module, net,
                                       decoder_module, 0,
                                       decoder_addr_port,
                                       decoder_addr_port_info.get_msb() - ipin);
  } 

  /* Connect the address port of the parent module to the address port of configurable children
   * Note that we only connect to the last few bits of address port
   */
  for (size_t mem_index = 0; mem_index < configurable_children.size(); ++mem_index) {
    ModuleId child_module = configurable_children[mem_index]; 
    size_t child_instance = module_manager.configurable_child_instances(parent_module)[mem_index];
    ModulePortId child_addr_port = module_manager.find_module_port(child_module, std::string(DECODER_ADDRESS_PORT_NAME));
    BasicPort child_addr_port_info = module_manager.module_port(child_module, child_addr_port);
    for (size_t ipin = 0; ipin < child_addr_port_info.get_width(); ++ipin) {
      ModuleNetId net = module_manager.module_instance_port_net(parent_module,
                                                                parent_module, 0, 
                                                                parent_addr_port,
                                                                parent_addr_port_info.get_lsb() + ipin);
      if (ModuleNetId::INVALID() == net) { 
        net = module_manager.create_module_net(parent_module);
        /* Configure the net source */
        module_manager.add_module_net_source(parent_module, net,
                                             parent_module, 0,
                                             parent_addr_port,
                                             parent_addr_port_info.get_lsb() + ipin);
      }
      /* Configure the net sink */
      module_manager.add_module_net_sink(parent_module, net,
                                         child_module, child_instance,
                                         child_addr_port,
                                         child_addr_port_info.get_lsb() + ipin);
    }
  }

  /* Connect the data_in (Din) of parent module to the data_in of the all
   * the memory modules
   */
  ModulePortId parent_din_port = module_manager.find_module_port(parent_module, std::string(DECODER_DATA_IN_PORT_NAME));
  for (size_t mem_index = 0; mem_index < configurable_children.size(); ++mem_index) {
    ModuleId child_module = configurable_children[mem_index]; 
    size_t child_instance = module_manager.configurable_child_instances(parent_module)[mem_index];
    ModulePortId child_din_port = module_manager.find_module_port(child_module, std::string(DECODER_DATA_IN_PORT_NAME));
    add_module_bus_nets(module_manager, parent_module,
                        parent_module, 0, parent_din_port,
                        child_module, child_instance, child_din_port);
  }

  /* Connect the data_out port of the decoder module 
   * to the enable port of configurable children
   */
  ModulePortId decoder_dout_port = module_manager.find_module_port(decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
  BasicPort decoder_dout_port_info = module_manager.module_port(decoder_module, decoder_dout_port);
  VTR_ASSERT(decoder_dout_port_info.get_width() == configurable_children.size());
  for (size_t mem_index = 0; mem_index < configurable_children.size(); ++mem_index) {
    ModuleId child_module = configurable_children[mem_index]; 
    size_t child_instance = module_manager.configurable_child_instances(parent_module)[mem_index];
    ModulePortId child_en_port = module_manager.find_module_port(child_module, std::string(DECODER_ENABLE_PORT_NAME));
    BasicPort child_en_port_info = module_manager.module_port(child_module, child_en_port);
    for (size_t ipin = 0; ipin < child_en_port_info.get_width(); ++ipin) {
      ModuleNetId net = module_manager.module_instance_port_net(parent_module,
                                                                decoder_module, 0, 
                                                                decoder_dout_port,
                                                                decoder_dout_port_info.pins()[mem_index]);
      if (ModuleNetId::INVALID() == net) { 
        net = module_manager.create_module_net(parent_module);
        /* Configure the net source */
        module_manager.add_module_net_source(parent_module, net,
                                             decoder_module, 0,
                                             decoder_dout_port,
                                             decoder_dout_port_info.pins()[mem_index]);
      }
      /* Configure the net sink */
      module_manager.add_module_net_sink(parent_module, net,
                                         child_module, child_instance,
                                         child_en_port,
                                         child_en_port_info.pins()[ipin]);
    }
  }

  /* Add the decoder as the last configurable children */
  module_manager.add_configurable_child(parent_module, decoder_module, 0);
}

/*********************************************************************
 * Top-level function to add nets for frame-based memories
 * Add nets depending on the need
 * - If there is no configurable child, return directly.
 * - If there is only one configurable child, short wire the EN, ADDR and DATA_IN to it
 * - If there are more than two configurable childern, add a decoder and build interconnection
 *   between it and the children 
 **********************************************************************/
void add_module_nets_cmos_memory_frame_config_bus(ModuleManager& module_manager,
                                                  DecoderLibrary& decoder_lib,
                                                  const ModuleId& parent_module) {
  if (0 == module_manager.configurable_children(parent_module).size()) {
    return; 
  }
  
  if (1 == module_manager.configurable_children(parent_module).size()) {
    add_module_nets_cmos_memory_frame_short_config_bus(module_manager, parent_module);
  } else {
    VTR_ASSERT (1 < module_manager.configurable_children(parent_module).size());
    add_module_nets_cmos_memory_frame_decoder_config_bus(module_manager, decoder_lib, parent_module);
  }
}

/*********************************************************************
 * Add the port-to-port connection between all the memory modules 
 * and their parent module
 *
 * Create nets to wire the control signals of memory module to 
 *    the configuration ports of primitive module
 *
 * Configuration Chain 
 * -------------------
 *
 *        config_bus (head)   config_bus (tail) 
 *            |                   ^
 * primitive  |                   |
 *   +---------------------------------------------+
 *   |        |                   |                |
 *   |        v                   |                |
 *   |  +-------------------------------------+    |
 *   |  |        CMOS-based Memory Modules    |    |
 *   |  +-------------------------------------+    |
 *   |        |                   |                |
 *   |        v                   v                |
 *   |     sram_out             sram_outb          |
 *   |                                             |
 *   +---------------------------------------------+
 *
 * Memory bank 
 * -----------
 *
 *        config_bus (BL)   config_bus (WL) 
 *            |                   |
 * primitive  |                   |
 *   +---------------------------------------------+
 *   |        |                   |                |
 *   |        v                   v                |
 *   |  +-------------------------------------+    |
 *   |  |        CMOS-based Memory Modules    |    |
 *   |  +-------------------------------------+    |
 *   |        |                   |                |
 *   |        v                   v                |
 *   |     sram_out             sram_outb          |
 *   |                                             |
 *   +---------------------------------------------+
 *
 **********************************************************************/
static 
void add_module_nets_cmos_memory_config_bus(ModuleManager& module_manager,
                                            DecoderLibrary& decoder_lib,
                                            const ModuleId& parent_module,
                                            const e_config_protocol_type& sram_orgz_type) {
  switch (sram_orgz_type) {
  case CONFIG_MEM_SCAN_CHAIN: {
    add_module_nets_cmos_memory_chain_config_bus(module_manager, parent_module,
                                                 sram_orgz_type);
    break;
  }
  case CONFIG_MEM_STANDALONE:
  case CONFIG_MEM_QL_MEMORY_BANK:
  case CONFIG_MEM_MEMORY_BANK:
    add_module_nets_cmos_flatten_memory_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
    add_module_nets_cmos_flatten_memory_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
    add_module_nets_cmos_flatten_memory_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_WLR);
    break;
  case CONFIG_MEM_FRAME_BASED:
    add_module_nets_cmos_memory_frame_config_bus(module_manager, decoder_lib, parent_module);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization!\n");
    exit(1);
  }
}

/*********************************************************************
 * @brief Add the port-to-port connection between all the memory modules 
 * and their parent module. This function creates nets to wire the control 
 * signals of memory module to the configuration ports of primitive module
 *
 * @note This function is only applicable to programmable blocks, which are
 * grid, CBX/CBY, SB. Different from the add_pb_module_nets_cmos_memory_config_bus(),
 * this function will merge BL/WLs of child module when connect them to the parent module
 *
 * QL Memory bank 
 * --------------
 *
 *        config_bus (BL)   config_bus (WL) 
 *            |                   |
 *  parent    |                   |
 *   +---------------------------------------------+
 *   |        |                   |                |
 *   |        +---------------+   |                |
 *   |        |               |   |                |
 *   |        |   +-----------|---+                |
 *   |        |   |           |   |                |
 *   |        v   v           v   v                |
 *   |  +-------------------------------------+    |
 *   |  | Child Mem 0 | ... | Child Mem N-1   |    |
 *   |  +-------------------------------------+    |
 *   |        |                   |                |
 *   |        v                   v                |
 *   |     sram_out             sram_outb          |
 *   |                                             |
 *   +---------------------------------------------+

 *
 **********************************************************************/
static 
void add_pb_module_nets_cmos_memory_config_bus(ModuleManager& module_manager,
                                               DecoderLibrary& decoder_lib,
                                               const ModuleId& parent_module,
                                               const e_config_protocol_type& sram_orgz_type) {
  switch (sram_orgz_type) {
  case CONFIG_MEM_SCAN_CHAIN: {
    add_module_nets_cmos_memory_chain_config_bus(module_manager, parent_module,
                                                 sram_orgz_type);
    break;
  }
  case CONFIG_MEM_STANDALONE:
  case CONFIG_MEM_QL_MEMORY_BANK:
    add_module_nets_cmos_memory_bank_bl_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
    add_module_nets_cmos_memory_bank_wl_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
    add_module_nets_cmos_memory_bank_wl_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_WLR);
    break;
  case CONFIG_MEM_MEMORY_BANK:
    add_module_nets_cmos_flatten_memory_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
    add_module_nets_cmos_flatten_memory_config_bus(module_manager, parent_module,
                                                   sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
    break;
  case CONFIG_MEM_FRAME_BASED:
    add_module_nets_cmos_memory_frame_config_bus(module_manager, decoder_lib, parent_module);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization!\n");
    exit(1);
  }
}

/*********************************************************************
 * TODO:
 * Add the port-to-port connection between a logic module 
 * and a memory module inside a primitive module
 *
 * Memory bank 
 * -----------
 *        config_bus (BL)   config_bus (WL) shared_config_bugs(shared_BL/WLs) 
 *            |                   |              |        |
 * primitive  |                   |              |        |
 *   +------------------------------------------------------------+
 *   |        |                   |              |        |       |
 *   |        v                   v              v        v       |
 *   |  +----------------------------------------------------+    |
 *   |  |              ReRAM-based Memory Module             |    |
 *   |  +----------------------------------------------------+    |
 *   |        |                   |                               |
 *   |        v                   v                               |
 *   |      mem_out              mem_outb                         |
 *   |                                                            |
 *   +------------------------------------------------------------+
 *
 **********************************************************************/

/********************************************************************
 * Add the port-to-port connection between a memory module 
 * and the configuration bus of a primitive module
 *
 * Create nets to wire the control signals of memory module to 
 *    the configuration ports of primitive module
 *
 *              Primitive module
 *             +----------------------------+
 *             |                +--------+  |
 *  config     |                |        |  |
 *   ports --->|--------------->| Memory |  |
 *             |                | Module |  |
 *             |                |        |  |
 *             |                +--------+  |
 *             +----------------------------+
 *     The detailed config ports really depend on the type
 *     of SRAM organization. 
 *
 * The config_bus in the argument is the reserved address of configuration
 * bus in the parent_module for this memory module
 *
 * The configuration bus connection will depend not only 
 * the design technology of the memory cells but also the 
 * configuration styles of FPGA fabric.
 * Here we will branch on the design technology
 *
 * Note: this function SHOULD be called after the pb_type_module is created
 * and its child module (logic_module and memory_module) is created! 
 *******************************************************************/
void add_module_nets_memory_config_bus(ModuleManager& module_manager,
                                       DecoderLibrary& decoder_lib,
                                       const ModuleId& parent_module,
                                       const e_config_protocol_type& sram_orgz_type, 
                                       const e_circuit_model_design_tech& mem_tech) {
  switch (mem_tech) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    add_module_nets_cmos_memory_config_bus(module_manager, decoder_lib,
                                           parent_module, 
                                           sram_orgz_type);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    /* TODO: */
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of memory design technology!\n");
    exit(1);
  }
}


/********************************************************************
 * Add the port-to-port connection between the configuration lines of
 * a programmable block module (grid, CBX/CBY, SB) and its child module
 *
 * The configuration bus connection will depend not only 
 * the design technology of the memory cells but also the 
 * configuration styles of FPGA fabric.
 * Here we will branch on the design technology
 *
 * Note: this function SHOULD be called after the pb_type_module is created
 * and its child module (logic_module and memory_module) is created! 
 *******************************************************************/
void add_pb_module_nets_memory_config_bus(ModuleManager& module_manager,
                                          DecoderLibrary& decoder_lib,
                                          const ModuleId& parent_module,
                                          const e_config_protocol_type& sram_orgz_type, 
                                          const e_circuit_model_design_tech& mem_tech) {
  switch (mem_tech) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    add_pb_module_nets_cmos_memory_config_bus(module_manager, decoder_lib,
                                              parent_module, 
                                              sram_orgz_type);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    /* TODO: */
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of memory design technology!\n");
    exit(1);
  }
}

/********************************************************************
 * Find the size of shared(reserved) configuration ports for module 
 *******************************************************************/
size_t find_module_num_shared_config_bits(const ModuleManager& module_manager,
                                          const ModuleId& module_id) {
  std::vector<std::string> shared_config_port_names;
  shared_config_port_names.push_back(generate_reserved_sram_port_name(CIRCUIT_MODEL_PORT_BLB));
  shared_config_port_names.push_back(generate_reserved_sram_port_name(CIRCUIT_MODEL_PORT_WL));
  size_t num_shared_config_bits = 0; /* By default it has zero configuration bits*/

  /* Try to find these ports in the module manager */
  for (const std::string& shared_config_port_name : shared_config_port_names) {
    ModulePortId module_port_id = module_manager.find_module_port(module_id, shared_config_port_name);
    /* If the port does not exist, go to the next */
    if (false == module_manager.valid_module_port_id(module_id, module_port_id)) {
      continue;
    }
    /* The port exist, find the port size and update the num_config_bits if the size is larger */
    BasicPort module_port = module_manager.module_port(module_id, module_port_id);
    num_shared_config_bits = std::max((int)num_shared_config_bits, (int)module_port.get_width());
  }

  return num_shared_config_bits;
}

/********************************************************************
 * Find the size of configuration ports for module 
 *******************************************************************/
size_t find_module_num_config_bits(const ModuleManager& module_manager,
                                   const ModuleId& module_id,
                                   const CircuitLibrary& circuit_lib,
                                   const CircuitModelId& sram_model,
                                   const e_config_protocol_type& sram_orgz_type) {
  std::vector<std::string> config_port_names = generate_sram_port_names(circuit_lib, sram_model, sram_orgz_type);
  size_t num_config_bits = 0; /* By default it has zero configuration bits*/

  /* Try to find these ports in the module manager */
  for (const std::string& config_port_name : config_port_names) {
    ModulePortId module_port_id = module_manager.find_module_port(module_id, config_port_name);
    /* If the port does not exist, go to the next */
    if (false == module_manager.valid_module_port_id(module_id, module_port_id)) {
      continue;
    }
    /* The port exist, find the port size and update the num_config_bits if the size is larger */
    BasicPort module_port = module_manager.module_port(module_id, module_port_id);
    num_config_bits = std::max((int)num_config_bits, (int)module_port.get_width());
  }

  return num_config_bits;
}

/********************************************************************
 * Add General purpose I/O ports to the module:
 * In this function, the following tasks are done: 
 * 1. find all the I/O ports from the child modules and build a list of it,
 * 2. Merge all the I/O ports with the same name
 * 3. add the ports to the pb_module
 * 4. add module nets to connect to the GPIO ports of each sub module 
 *
 *   Module
 *   ----------------------+
 *                         |
 *   child[0]              |
 *   -----------+          |
 *              |----------+----> outputA[0]
 *   -----------+          |
 *                         |
 *   child[1]              |
 *   -----------+          |
 *              |----------+----> outputA[1]
 *   -----------+          |

 *
 * Note: This function should be call ONLY after all the sub modules (instances)
 * have been added to the pb_module!
 * Otherwise, some GPIO ports of the sub modules may be missed!
 *******************************************************************/
static 
void add_module_io_ports_from_child_modules(ModuleManager& module_manager, 
                                            const ModuleId& module_id,
                                            const ModuleManager::e_module_port_type& module_port_type) {
  std::vector<BasicPort> gpio_ports_to_add;
  std::vector<bool> mappable_gpio_ports;

  /* Iterate over the child modules */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    /* Iterate over the child instances */
    for (size_t i = 0; i < module_manager.num_instance(module_id, child); ++i) {
      /* Find all the global ports, whose port type is special */
      for (const ModulePortId& gpio_port_id : module_manager.module_port_ids_by_type(child, module_port_type)) {
        const BasicPort& gpio_port = module_manager.module_port(child, gpio_port_id);
        /* If this port is not mergeable, we update the list */
        bool is_mergeable = false;
        for (size_t i_gpio_port_to_add = 0; i_gpio_port_to_add < gpio_ports_to_add.size(); ++i_gpio_port_to_add) {
          BasicPort& gpio_port_to_add = gpio_ports_to_add[i_gpio_port_to_add];
          if (false == gpio_port_to_add.mergeable(gpio_port)) {
            continue;
          }
          is_mergeable = true;
          /* Mappable I/O property must match! Mismatch rarely happened
           * but should error out avoid silent bugs!
           */
          VTR_ASSERT(module_manager.port_is_mappable_io(child, gpio_port_id) == mappable_gpio_ports[i_gpio_port_to_add]);
          /* For mergeable ports, we combine the port
           * Note: do NOT use the merge() method!
           * the GPIO ports should be accumulated by the sizes of ports
           * not by the LSB/MSB range !!! 
           */
          gpio_port_to_add.combine(gpio_port);
          break;
        }
        if (false == is_mergeable) {
          /* Reach here, this is an unique gpio port, update the list */
          gpio_ports_to_add.push_back(gpio_port);
          /* If the gpio port is a mappable I/O, we should herit from the child module */
          mappable_gpio_ports.push_back(module_manager.port_is_mappable_io(child, gpio_port_id));
        }
      }
    }
  } 

  /* Record the port id for each type of GPIO port */
  std::vector<ModulePortId> gpio_port_ids;
  /* Add the gpio ports for the module */
  for (size_t iport = 0; iport < gpio_ports_to_add.size(); ++iport) {
    const BasicPort& gpio_port_to_add = gpio_ports_to_add[iport];
    ModulePortId port_id = module_manager.add_port(module_id, gpio_port_to_add, module_port_type);
    gpio_port_ids.push_back(port_id);
    if (true == mappable_gpio_ports[iport]) { 
      module_manager.set_port_is_mappable_io(module_id, port_id, true);
    }
  } 

  /* Set up a counter for each type of GPIO port */
  std::vector<size_t> gpio_port_lsb(gpio_ports_to_add.size(), 0);
  /* Add module nets to connect the GPIOs of the module to the GPIOs of the sub module */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    /* Iterate over the child instances */
    for (const size_t& child_instance : module_manager.child_module_instances(module_id, child)) {
      /* Find all the global ports, whose port type is special */
      for (ModulePortId child_gpio_port_id : module_manager.module_port_ids_by_type(child, module_port_type)) {
        BasicPort child_gpio_port = module_manager.module_port(child, child_gpio_port_id);
        /* Find the port with the same name! */
        for (size_t iport = 0; iport < gpio_ports_to_add.size(); ++iport) {
          if (false == gpio_ports_to_add[iport].mergeable(child_gpio_port)) {
            continue;
          }
          /* For each pin of the child port, create a net and do wiring */
          for (const size_t& pin_id : child_gpio_port.pins()) {
            /* Reach here, it means this is the port we want, create a net and configure its source and sink */
            /* - For GPIO and GPIN ports
             *   the source of the net is the current module 
             *   the sink of the net is the child module
             * - For GPOUT ports
             *   the source of the net is the child module
             *   the sink of the net is the current module 
             */
            if ( (ModuleManager::MODULE_GPIO_PORT == module_port_type)
              || (ModuleManager::MODULE_GPIN_PORT == module_port_type) ) {
              ModuleNetId net = create_module_source_pin_net(module_manager, module_id, module_id, 0, gpio_port_ids[iport], gpio_port_lsb[iport]); 
              module_manager.add_module_net_sink(module_id, net, child, child_instance, child_gpio_port_id, pin_id); 
            } else {
              VTR_ASSERT(ModuleManager::MODULE_GPOUT_PORT == module_port_type);
              ModuleNetId net = create_module_source_pin_net(module_manager, module_id, child, child_instance, child_gpio_port_id, pin_id); 
              module_manager.add_module_net_sink(module_id, net, module_id, 0, gpio_port_ids[iport], gpio_port_lsb[iport]); 
            }
            /* Update the LSB counter */
            gpio_port_lsb[iport]++;
          }
          /* We finish for this child gpio port */
          break;
        }
      }
    }
  }

  /* Check: all the lsb should now match the size of each GPIO port */
  for (size_t iport = 0; iport < gpio_ports_to_add.size(); ++iport) {
    if (gpio_ports_to_add[iport].get_width() != gpio_port_lsb[iport]) 
    VTR_ASSERT(gpio_ports_to_add[iport].get_width() == gpio_port_lsb[iport]);
  }
}

/********************************************************************
 * Add GPIO ports to the module:
 * In this function, the following tasks are done: 
 * 1. find all the GPIO ports from the child modules and build a list of it,
 * 2. Merge all the GPIO ports with the same name
 * 3. add the ports to the pb_module
 * 4. add module nets to connect to the GPIO ports of each sub module 
 *
 * Note: This function should be call ONLY after all the sub modules (instances)
 * have been added to the pb_module!
 * Otherwise, some GPIO ports of the sub modules may be missed!
 *******************************************************************/
void add_module_gpio_ports_from_child_modules(ModuleManager& module_manager, 
                                              const ModuleId& module_id) {
  add_module_io_ports_from_child_modules(module_manager, module_id, ModuleManager::MODULE_GPIO_PORT);

  add_module_io_ports_from_child_modules(module_manager, module_id, ModuleManager::MODULE_GPIN_PORT);

  add_module_io_ports_from_child_modules(module_manager, module_id, ModuleManager::MODULE_GPOUT_PORT);
}

/********************************************************************
 * Add global input ports to the module:
 * In this function, the following tasks are done: 
 * 1. find all the global input ports from the child modules and build a list of it,
 * 2. add the input ports to the pb_module
 * 3. add the module nets to connect the pb_module global ports to those of child modules
 *
 *                             Module
 *                           +--------------------------
 *                           |        child[0]
 *        input_portA[0] ----+-+---->+----------
 *                           | |     |
 *                           | |     +----------
 *                           | |
 *                           | |      child[1]
 *                           | +---->+----------
 *                           |       |
 *                           |       +----------
 *
 * Note: This function should be call ONLY after all the sub modules (instances)
 * have been added to the pb_module!
 * Otherwise, some global ports of the sub modules may be missed!
 *******************************************************************/
void add_module_global_input_ports_from_child_modules(ModuleManager& module_manager, 
                                                      const ModuleId& module_id) {
  std::vector<BasicPort> global_ports_to_add;

  /* Iterate over the child modules */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    /* Iterate over the child instances */
    for (size_t i = 0; i < module_manager.num_instance(module_id, child); ++i) {
      /* Find all the global ports, whose port type is special */
      for (BasicPort global_port : module_manager.module_ports_by_type(child, ModuleManager::MODULE_GLOBAL_PORT)) {
        /* Search in the global port list to be added, if this is unique, we update the list */
        std::vector<BasicPort>::iterator it = std::find(global_ports_to_add.begin(), global_ports_to_add.end(), global_port);
        if (it != global_ports_to_add.end()) {
          continue;
        }
        /* Reach here, this is an unique global port, update the list */
        global_ports_to_add.push_back(global_port);
      }
    }
  } 

  /* Record the port id for each type of global port */
  std::vector<ModulePortId> global_port_ids;
  /* Add the global ports for the module */
  for (const BasicPort& global_port_to_add : global_ports_to_add) {
    ModulePortId port_id = module_manager.add_port(module_id, global_port_to_add, ModuleManager::MODULE_GLOBAL_PORT);
    global_port_ids.push_back(port_id);
  } 

  /* Count the number of sinks for each global port */
  std::map<ModulePortId, size_t> port_sink_count;
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    /* Find all the global ports, whose port type is special */
    for (ModulePortId child_global_port_id : module_manager.module_port_ids_by_type(child, ModuleManager::MODULE_GLOBAL_PORT)) {
      BasicPort child_global_port = module_manager.module_port(child, child_global_port_id);
      /* Search in the global port list to be added, find the port id */

      std::vector<BasicPort>::iterator it = std::find(global_ports_to_add.begin(), global_ports_to_add.end(), child_global_port);
      VTR_ASSERT(it != global_ports_to_add.end());
      ModulePortId module_global_port_id = global_port_ids[it - global_ports_to_add.begin()];

      port_sink_count[module_global_port_id] += module_manager.num_instance(module_id, child);
    }
  }

  /* Add module nets to connect the global ports of the module to the global ports of the sub module */
  /* Iterate over the child modules */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    /* Iterate over the child instances */
    for (const size_t& child_instance : module_manager.child_module_instances(module_id, child)) {
      /* Find all the global ports, whose port type is special */
      for (ModulePortId child_global_port_id : module_manager.module_port_ids_by_type(child, ModuleManager::MODULE_GLOBAL_PORT)) {
        BasicPort child_global_port = module_manager.module_port(child, child_global_port_id);
        /* Search in the global port list to be added, find the port id */
        std::vector<BasicPort>::iterator it = std::find(global_ports_to_add.begin(), global_ports_to_add.end(), child_global_port);
        VTR_ASSERT(it != global_ports_to_add.end());
        ModulePortId module_global_port_id = global_port_ids[it - global_ports_to_add.begin()];
        BasicPort module_global_port = module_manager.module_port(module_id, module_global_port_id);
        /* The global ports should match in size */
        VTR_ASSERT(module_global_port.get_width() == child_global_port.get_width());
        /* For each pin of the child port, create a net and do wiring */
        for (size_t pin_id = 0; pin_id < child_global_port.pins().size(); ++pin_id) {
          /* Reach here, it means this is the port we want, create a net and configure its source and sink */
          ModuleNetId net = create_module_source_pin_net(module_manager, module_id, module_id, 0, module_global_port_id, module_global_port.pins()[pin_id]); 
          module_manager.reserve_module_net_sinks(module_id, net, port_sink_count[module_global_port_id]);
          module_manager.add_module_net_sink(module_id, net, child, child_instance, child_global_port_id, child_global_port.pins()[pin_id]); 
          /* We finish for this child gpio port */
        }
      }
    }
  } 
}

/********************************************************************
 * Add global ports to the module:
 * In this function, we will add global input ports and global output ports 
 * which are collected from the child modules 
 *
 * - Input ports: the input ports will be uniquified by names
 *                Ports with the same name will be merged to the same pin
 *                See details inside the function
 *
 * - Output ports: the output ports will be uniquified by names
 *                 Different from the input ports, output ports 
 *                 with the same name will be merged but will have indepedent pins
 *                 See details inside the function
 *
 * Note: This function should be call ONLY after all the sub modules (instances)
 * have been added to the pb_module!
 * Otherwise, some global ports of the sub modules may be missed!
 *******************************************************************/
void add_module_global_ports_from_child_modules(ModuleManager& module_manager, 
                                                const ModuleId& module_id) {
  /* Input ports */
  add_module_global_input_ports_from_child_modules(module_manager, module_id);
}

/********************************************************************
 * Find the number of shared configuration bits for a module 
 * by selected the maximum number of shared configuration bits of child modules
 *
 * Note: This function should be call ONLY after all the sub modules (instances)
 * have been added to the pb_module!
 * Otherwise, some global ports of the sub modules may be missed!
 *******************************************************************/
size_t find_module_num_shared_config_bits_from_child_modules(ModuleManager& module_manager, 
                                                             const ModuleId& module_id) {
  size_t num_shared_config_bits = 0;

  /* Iterate over the child modules */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    num_shared_config_bits = std::max((int)num_shared_config_bits, (int)find_module_num_shared_config_bits(module_manager, child));
  } 

  return num_shared_config_bits;
}

/********************************************************************
 * Find the number of configuration bits for a module 
 * by summing up the number of configuration bits of child modules
 *
 * Note: This function should be call ONLY after all the sub modules (instances)
 * have been added to the pb_module!
 * Otherwise, some global ports of the sub modules may be missed!
 *******************************************************************/
size_t find_module_num_config_bits_from_child_modules(ModuleManager& module_manager, 
                                                      const ModuleId& module_id,
                                                      const CircuitLibrary& circuit_lib,
                                                      const CircuitModelId& sram_model,
                                                      const e_config_protocol_type& sram_orgz_type) {
  size_t num_config_bits = 0;

  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE: 
  case CONFIG_MEM_SCAN_CHAIN: 
  case CONFIG_MEM_QL_MEMORY_BANK:
  case CONFIG_MEM_MEMORY_BANK: {
    /* For scan-chain, standalone and memory bank configuration protocol
     * The number of configuration bits is the sum of configuration bits 
     * per configurable children 
     */
    for (const ModuleId& child : module_manager.configurable_children(module_id)) {
      num_config_bits += find_module_num_config_bits(module_manager, child, circuit_lib, sram_model, sram_orgz_type);
    } 
    break;
  }
  case CONFIG_MEM_FRAME_BASED: {
    /* For frame-based configuration protocol
     * The number of configuration bits is the sum of
     * - the maximum of configuration bits among configurable children
     * - and the number of configurable children
     */
    for (const ModuleId& child : module_manager.configurable_children(module_id)) {
      size_t temp_num_config_bits = find_module_num_config_bits(module_manager, child, circuit_lib, sram_model, sram_orgz_type);
      num_config_bits = std::max((int)temp_num_config_bits, (int)num_config_bits);
    } 

    /* If there are more than 2 configurable children, we need a decoder
     * Otherwise, we can just short wire the address port to the children
     */
    if (1 < module_manager.configurable_children(module_id).size()) {
      num_config_bits += find_mux_local_decoder_addr_size(module_manager.configurable_children(module_id).size());
    }

    break;
  }
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization !\n");
    exit(1);
  }

  return num_config_bits;
}

/********************************************************************
 * Try to create a net for the source pin
 * This function will try
 *   - Find if there is already a net created whose source is the pin
 *     If so, it will return the net id
 *   - If not, it will create a net and configure its source
 *******************************************************************/
ModuleNetId create_module_source_pin_net(ModuleManager& module_manager,
                                         const ModuleId& cur_module_id,
                                         const ModuleId& src_module_id,
                                         const size_t& src_instance_id,
                                         const ModulePortId& src_module_port_id,
                                         const size_t& src_pin_id) {
  ModuleNetId net = module_manager.module_instance_port_net(cur_module_id,
                                                            src_module_id, src_instance_id, 
                                                            src_module_port_id, src_pin_id);
  if (ModuleNetId::INVALID() == net) { 
    net = module_manager.create_module_net(cur_module_id);
    module_manager.add_module_net_source(cur_module_id, net,
                                         src_module_id, src_instance_id,
                                         src_module_port_id, src_pin_id);
  }

  return net;
}

/********************************************************************
 * Add a bus of nets to a module (cur_module_id)
 * Note: 
 * - both src and des module should exist in the module manager
 * - src_module should be the cur_module or a child of it
 * - des_module should be the cur_module or a child of it
 * - src_instance should be valid and des_instance should be valid as well
 * - src port size should match the des port size
 *
 *******************************************************************/
void add_module_bus_nets(ModuleManager& module_manager,
                         const ModuleId& cur_module_id,
                         const ModuleId& src_module_id,
                         const size_t& src_instance_id,
                         const ModulePortId& src_module_port_id,
                         const ModuleId& des_module_id,
                         const size_t& des_instance_id,
                         const ModulePortId& des_module_port_id) {

  VTR_ASSERT(true == module_manager.valid_module_id(cur_module_id));
  VTR_ASSERT(true == module_manager.valid_module_id(src_module_id));
  VTR_ASSERT(true == module_manager.valid_module_id(des_module_id));

  VTR_ASSERT(true == module_manager.valid_module_port_id(src_module_id, src_module_port_id));
  VTR_ASSERT(true == module_manager.valid_module_port_id(des_module_id, des_module_port_id));

  if (src_module_id == cur_module_id) {
    VTR_ASSERT(0 == src_instance_id);
  } else {
    VTR_ASSERT(src_instance_id < module_manager.num_instance(cur_module_id, src_module_id));
  }

  if (des_module_id == cur_module_id) {
    VTR_ASSERT(0 == des_instance_id);
  } else {
    VTR_ASSERT(des_instance_id < module_manager.num_instance(cur_module_id, des_module_id));
  }

  const BasicPort& src_port = module_manager.module_port(src_module_id, src_module_port_id);
  const BasicPort& des_port = module_manager.module_port(des_module_id, des_module_port_id);

  if (src_port.get_width() != des_port.get_width()) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Unmatched port size: src_port %s is %lu while des_port %s is %lu!\n",
                   src_port.get_name().c_str(),
                   src_port.get_width(),
                   des_port.get_name().c_str(),
                   des_port.get_width());
    exit(1);
  }

  /* Create a net for each pin */
  for (size_t pin_id = 0; pin_id < src_port.pins().size(); ++pin_id) {
    ModuleNetId net = create_module_source_pin_net(module_manager, cur_module_id, 
                                                   src_module_id, src_instance_id, 
                                                   src_module_port_id, src_port.pins()[pin_id]);
    VTR_ASSERT(ModuleNetId::INVALID() != net);

    /* Configure the net sink */
    module_manager.add_module_net_sink(cur_module_id, net, des_module_id, des_instance_id, des_module_port_id, des_port.pins()[pin_id]);
  }
}

/********************************************************************
 * TODO:
 * Add the port-to-port connection between a logic module 
 * and a memory module inside a primitive module
 *
 * Create nets to wire the formal verification ports of 
 * primitive module to SRAM ports of logic module 
 * 
 *    Primitive module
 *
 *                     formal_port_sram
 *    +-----------------------------------------------+
 *    |                       ^                       |
 *    |    +---------+        |    +--------+         |
 *    |    |         | SRAM   |    |        |         |
 *    |    |  Logic  |--------+--->| Memory |         |
 *    |    | Module  | SRAMb       | Module |         |
 *    |    |         |--------+--->|        |         |
 *    |    +---------+        |    +--------+         |
 *    |                       v                       |
 *    +-----------------------------------------------+
 *                     formal_port_sramb
 *
 *******************************************************************/

} /* end namespace openfpga */
