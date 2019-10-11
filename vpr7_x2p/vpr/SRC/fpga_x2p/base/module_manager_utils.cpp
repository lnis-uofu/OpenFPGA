/******************************************************************************
 * This files includes most utilized functions 
 * for data structures for module management.
 ******************************************************************************/

#include <map>
#include <algorithm>

#include "util.h"
#include "vtr_assert.h"

#include "spice_types.h"

#include "circuit_library.h"
#include "circuit_library_utils.h"
#include "module_manager.h"

#include "fpga_x2p_naming.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_mem_utils.h"

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
 *    IMPORTANT: the port size will be forced to 1 in this case 
 *               because the head and tail are both 1-bit ports!!!
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
  /* Actual port size may be different from user specification. Think about CCFF */
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
    /* CCFF head/tail are single-bit ports */
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

/********************************************************************
 * Add ports of a pb_type block to module manager
 * Port addition will follow the sequence: inout, input, output, clock
 * This will help use to keep a clean module definition when printing out
 * To avoid port mismatch between the pb_type and its linked circuit model
 * This function will also check that each pb_type port is actually exist
 * in the linked circuit model
 *******************************************************************/
void add_pb_type_ports_to_module_manager(ModuleManager& module_manager, 
                                         const ModuleId& module_id,
                                         t_pb_type* cur_pb_type) {
   
  /* Find the inout ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_inout_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, SPICE_MODEL_PORT_INOUT);
  for (auto port : pb_type_inout_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_INOUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }

  /* Find the input ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_input_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, SPICE_MODEL_PORT_INPUT);
  for (auto port : pb_type_input_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_INPUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }

  /* Find the output ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_output_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, SPICE_MODEL_PORT_OUTPUT);
  for (auto port : pb_type_output_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_OUTPUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(module_id, module_port.get_name(), true);
  }

  /* Find the clock ports required by the primitive pb_type, and add them to the module */
  std::vector<t_port*> pb_type_clock_ports = find_pb_type_ports_match_circuit_model_port_type(cur_pb_type, SPICE_MODEL_PORT_CLOCK);
  for (auto port : pb_type_clock_ports) {
    BasicPort module_port(generate_pb_type_port_name(port), port->num_pins);
    module_manager.add_port(module_id, module_port, ModuleManager::MODULE_CLOCK_PORT);
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
                                       t_pb_type* cur_pb_type) {
  for (int iport = 0; iport < cur_pb_type->num_ports; ++iport) {
    t_port* pb_type_port = &(cur_pb_type->ports[iport]);
    /* Must have a linked circuit model port */
    VTR_ASSERT( CircuitPortId::INVALID() != pb_type_port->circuit_model_port);

    /* Find the source port in pb_type module */
    /* Get the src module port id */
    ModulePortId src_module_port_id = module_manager.find_module_port(pb_type_module, generate_pb_type_port_name(pb_type_port));
    VTR_ASSERT(ModulePortId::INVALID() != src_module_port_id);
    BasicPort src_port = module_manager.module_port(pb_type_module, src_module_port_id);

    /* Get the des module port id */
    std::string des_module_port_name = circuit_lib.port_lib_name(pb_type_port->circuit_model_port);
    ModulePortId des_module_port_id = module_manager.find_module_port(child_module, des_module_port_name);
    VTR_ASSERT(ModulePortId::INVALID() != des_module_port_id);
    BasicPort des_port = module_manager.module_port(child_module, des_module_port_id);

    /* Port size must match */
    if (src_port.get_width() != des_port.get_width()) 
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
        module_manager.add_module_net_sink(pb_type_module, net, pb_type_module, 0, src_module_port_id, src_port.pins()[pin_id]);
        /* Add net sink */
        module_manager.add_module_net_source(pb_type_module, net, child_module, child_instance_id, des_module_port_id, des_port.pins()[pin_id]);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(File:%s, [LINE%d]) Invalid port of pb_type!\n",
                 __FILE__, __LINE__);
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
      std::string net_name = module_manager.module_name(logic_module) + std::string("_") + logic_module_sram_ports[port_index].get_name();
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
    logic_model_sram_port_names.push_back(circuit_lib.port_lib_name(regular_sram_port));
  }
  /* Mode-select sram port goes first */
  for (CircuitPortId mode_select_sram_port : find_circuit_mode_select_sram_ports(circuit_lib, logic_model)) {
    logic_model_sram_port_names.push_back(circuit_lib.port_lib_name(mode_select_sram_port));
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
  /* TODO: this should be a constant expression and it should be the same for all the memory module! */
  std::string memory_model_sram_port_name = generate_configuration_chain_data_out_name();
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
    logic_model_sramb_port_names.push_back(circuit_lib.port_lib_name(regular_sram_port) + std::string("_inv"));
  }
  /* Mode-select sram port goes first */
  for (CircuitPortId mode_select_sram_port : find_circuit_mode_select_sram_ports(circuit_lib, logic_model)) {
    logic_model_sramb_port_names.push_back(circuit_lib.port_lib_name(mode_select_sram_port) + std::string("_inv"));
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
  std::string memory_model_sramb_port_name = generate_configuration_chain_inverted_data_out_name();
  /* Find the corresponding ports in memory module */ 
  ModulePortId mem_module_sramb_port_id = module_manager.find_module_port(memory_module, memory_model_sramb_port_name);

  /* Do wiring only when we have sramb ports */
  if ( (false == logic_module_sramb_port_ids.empty())
    || (ModulePortId::INVALID() == mem_module_sramb_port_id) ) {
    add_module_nets_between_logic_and_memory_sram_ports(module_manager, parent_module, 
                                                        logic_module, logic_instance_id,
                                                        memory_module, memory_instance_id,
                                                        logic_module_sramb_port_ids, mem_module_sramb_port_id);
  }
}

/********************************************************************
 * TODO:
 * Add the port-to-port connection between a logic module 
 * and a memory module inside a primitive module
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
 * Note: this function SHOULD be called after the pb_type_module is created
 * and its child module (logic_module and memory_module) is created! 
 *******************************************************************/

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

