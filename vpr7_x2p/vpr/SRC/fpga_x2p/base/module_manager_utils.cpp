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
                                      const size_t& num_config_bits) {
  std::vector<std::string> sram_port_names = generate_sram_port_names(circuit_lib, sram_model, sram_orgz_type);
  size_t sram_port_size = generate_sram_port_size(sram_orgz_type, num_config_bits); 

  /* Add ports to the module manager */
  for (const std::string& sram_port_name : sram_port_names) {
    /* Add generated ports to the ModuleManager */
    BasicPort sram_port(sram_port_name, sram_port_size);
    module_manager.add_port(module_id, sram_port, ModuleManager::MODULE_INPUT_PORT);
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
                                            const ModuleId& parent_module,
                                            const std::vector<ModuleId>& memory_modules,
                                            const std::vector<size_t>& memory_instances,
                                            const e_sram_orgz& sram_orgz_type) {
  /* Ensure that the size of memory_model vector matches the memory_module vector */
  VTR_ASSERT(memory_modules.size() == memory_instances.size());

  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* Nothing to do */
    break;
  case SPICE_SRAM_SCAN_CHAIN: {
    /* Connect all the memory modules under the parent module in a chain
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
     */
    for (size_t mem_index = 0; mem_index < memory_modules.size(); ++mem_index) {
      ModuleId net_src_module_id;
      size_t net_src_instance_id;
      ModulePortId net_src_port_id;

      ModuleId net_sink_module_id;
      size_t net_sink_instance_id;
      ModulePortId net_sink_port_id;

      if (0 == mem_index) {
        /* Find the port name of configuration chain head */
        std::string src_port_name = generate_sram_port_name(sram_orgz_type, SPICE_MODEL_PORT_INPUT);
        net_src_module_id = parent_module; 
        net_src_instance_id = 0;
        net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

        /* Find the port name of next memory module */
        std::string sink_port_name = generate_configuration_chain_head_name();
        net_sink_module_id = memory_modules[mem_index]; 
        net_sink_instance_id = memory_instances[mem_index];
        net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 
      } else {
        /* Find the port name of previous memory module */
        std::string src_port_name = generate_configuration_chain_tail_name();
        net_src_module_id = memory_modules[mem_index - 1]; 
        net_src_instance_id = memory_instances[mem_index - 1];
        net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

        /* Find the port name of next memory module */
        std::string sink_port_name = generate_configuration_chain_head_name();
        net_sink_module_id = memory_modules[mem_index]; 
        net_sink_instance_id = memory_instances[mem_index];
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
        ModuleNetId net = module_manager.create_module_net(parent_module);
        /* Add net source */
        module_manager.add_module_net_source(parent_module, net, net_src_module_id, net_src_instance_id, net_src_port_id, net_src_port.pins()[pin_id]);
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
    ModuleId net_src_module_id = memory_modules.back(); 
    size_t net_src_instance_id = memory_instances.back();
    ModulePortId net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

    /* Find the port name of next memory module */
    std::string sink_port_name = generate_sram_port_name(sram_orgz_type, SPICE_MODEL_PORT_OUTPUT);
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
      ModuleNetId net = module_manager.create_module_net(parent_module);
      /* Add net source */
      module_manager.add_module_net_source(parent_module, net, net_src_module_id, net_src_instance_id, net_src_port_id, net_src_port.pins()[pin_id]);
      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);
    }
    break;
  }
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO: */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
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
 * TODO:
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
                                       const ModuleId& parent_module,
                                       const std::vector<ModuleId>& memory_modules,
                                       const std::vector<size_t>& memory_instances,
                                       const e_sram_orgz& sram_orgz_type, 
                                       const e_spice_model_design_tech& mem_tech) {
  switch (mem_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    add_module_nets_cmos_memory_config_bus(module_manager, parent_module, 
                                           memory_modules, memory_instances, 
                                           sram_orgz_type);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* TODO: */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of memory design technology !\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/********************************************************************
 * Find the size of shared(reserved) configuration ports for module 
 *******************************************************************/
size_t find_module_num_shared_config_bits(const ModuleManager& module_manager,
                                          const ModuleId& module_id) {
  std::vector<std::string> shared_config_port_names;
  shared_config_port_names.push_back(generate_reserved_sram_port_name(SPICE_MODEL_PORT_BLB));
  shared_config_port_names.push_back(generate_reserved_sram_port_name(SPICE_MODEL_PORT_WL));
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
                                   const e_sram_orgz& sram_orgz_type) {
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
  std::vector<BasicPort> gpio_ports_to_add;

  /* Iterate over the child modules */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    /* Iterate over the child instances */
    for (size_t i = 0; i < module_manager.num_instance(module_id, child); ++i) {
      /* Find all the global ports, whose port type is special */
      for (BasicPort gpio_port : module_manager.module_ports_by_type(child, ModuleManager::MODULE_GPIO_PORT)) {
        /* If this port is not mergeable, we update the list */
        bool is_mergeable = false;
        for (BasicPort& gpio_port_to_add : gpio_ports_to_add) {
          if (false == gpio_port_to_add.mergeable(gpio_port)) {
            continue;
          }
          is_mergeable = true;
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
        }
      }
    }
  } 

  /* Record the port id for each type of GPIO port */
  std::vector<ModulePortId> gpio_port_ids;
  /* Add the gpio ports for the module */
  for (const BasicPort& gpio_port_to_add : gpio_ports_to_add) {
    ModulePortId port_id = module_manager.add_port(module_id, gpio_port_to_add, ModuleManager::MODULE_GPIO_PORT);
    gpio_port_ids.push_back(port_id);
  } 

  /* Set up a counter for each type of GPIO port */
  std::vector<size_t> gpio_port_lsb(gpio_ports_to_add.size(), 0);
  /* Add module nets to connect the GPIOs of the module to the GPIOs of the sub module */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    /* Iterate over the child instances */
    for (const size_t& child_instance : module_manager.child_module_instances(module_id, child)) {
      /* Find all the global ports, whose port type is special */
      for (ModulePortId child_gpio_port_id : module_manager.module_port_ids_by_type(child, ModuleManager::MODULE_GPIO_PORT)) {
        BasicPort child_gpio_port = module_manager.module_port(child, child_gpio_port_id);
        /* Find the port with the same name! */
        for (size_t iport = 0; iport < gpio_ports_to_add.size(); ++iport) {
          if (false == gpio_ports_to_add[iport].mergeable(child_gpio_port)) {
            continue;
          }
          /* For each pin of the child port, create a net and do wiring */
          for (const size_t& pin_id : child_gpio_port.pins()) {
            /* Reach here, it means this is the port we want, create a net and configure its source and sink */
            ModuleNetId net = module_manager.create_module_net(module_id);
            module_manager.add_module_net_source(module_id, net, module_id, 0, gpio_port_ids[iport], gpio_port_lsb[iport]); 
            module_manager.add_module_net_sink(module_id, net, child, child_instance, child_gpio_port_id, pin_id); 
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
 * Add global ports to the module:
 * In this function, the following tasks are done: 
 * 1. find all the global ports from the child modules and build a list of it,
 * 2. add the ports to the pb_module
 * 3. add the module nets to connect the pb_module global ports to those of child modules
 *
 * Note: This function should be call ONLY after all the sub modules (instances)
 * have been added to the pb_module!
 * Otherwise, some global ports of the sub modules may be missed!
 *******************************************************************/
void add_module_global_ports_from_child_modules(ModuleManager& module_manager, 
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
          ModuleNetId net = module_manager.create_module_net(module_id);
          module_manager.add_module_net_source(module_id, net, module_id, 0, module_global_port_id, module_global_port.pins()[pin_id]); 
          module_manager.add_module_net_sink(module_id, net, child, child_instance, child_global_port_id, child_global_port.pins()[pin_id]); 
          /* We finish for this child gpio port */
        }
      }
    }
  } 
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
                                                      const e_sram_orgz& sram_orgz_type) {
  size_t num_config_bits = 0;

  /* Iterate over the child modules */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    num_config_bits += find_module_num_config_bits(module_manager, child, circuit_lib, sram_model, sram_orgz_type);
  } 

  return num_config_bits;
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

