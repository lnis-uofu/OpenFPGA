/*********************************************************************
 * This file includes functions to generate Verilog submodules for 
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 ********************************************************************/
#include <ctime>
#include <string>
#include <algorithm>

#include "util.h"
#include "vtr_assert.h"

/* Device-level header files */
#include "mux_graph.h"
#include "module_manager.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "circuit_library_utils.h"
#include "module_manager_utils.h"
#include "mux_utils.h"

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "build_memory_modules.h"

/*********************************************************************
 * Add module nets to connect an input port of a memory module to 
 * an input port of its child module
 * Restriction: this function is really designed for memory modules
 * 1. It assumes that input port name of child module is the same as memory module
 * 2. It assumes exact pin-to-pin mapping: 
 *     j-th pin of input port of the i-th child module is wired to the j + i*W -th
 *     pin of input port of the memory module, where W is the size of port 
 ********************************************************************/
static 
void add_module_input_nets_to_mem_modules(ModuleManager& module_manager,
                                          const ModuleId& mem_module,
                                          const CircuitLibrary& circuit_lib,
                                          const std::vector<CircuitPortId>& circuit_ports,
                                          const ModuleId& child_module,
                                          const size_t& child_index,
                                          const size_t& child_instance) {
  /* Wire inputs of parent module to inputs of child modules */
  for (const auto& port : circuit_ports) {
    ModulePortId src_port_id = module_manager.find_module_port(mem_module, circuit_lib.port_lib_name(port));
    ModulePortId sink_port_id = module_manager.find_module_port(child_module, circuit_lib.port_lib_name(port));
    for (size_t pin_id = 0; pin_id < module_manager.module_port(mem_module, sink_port_id).pins().size(); ++pin_id) {
      ModuleNetId net = module_manager.create_module_net(mem_module);
      /* Source pin is shifted by the number of memories */
      size_t src_pin_id = child_index * circuit_lib.port_size(port) + module_manager.module_port(mem_module, src_port_id).pins()[pin_id];
      /* Source node of the input net is the input of memory module */
      module_manager.add_module_net_source(mem_module, net, mem_module, 0, src_port_id, src_pin_id);
      /* Sink node of the input net is the input of sram module */
      size_t sink_pin_id = module_manager.module_port(child_module, sink_port_id).pins()[pin_id];
      module_manager.add_module_net_sink(mem_module, net, child_module, child_instance, sink_port_id, sink_pin_id);
    }
  }
}

/*********************************************************************
 * Add module nets to connect an output port of a memory module to 
 * an output port of its child module
 * Restriction: this function is really designed for memory modules
 * 1. It assumes that output port name of child module is the same as memory module
 * 2. It assumes exact pin-to-pin mapping: 
 *     j-th pin of output port of the i-th child module is wired to the j + i*W -th
 *     pin of output port of the memory module, where W is the size of port 
 ********************************************************************/
static 
void add_module_output_nets_to_mem_modules(ModuleManager& module_manager,
                                           const ModuleId& mem_module,
                                           const CircuitLibrary& circuit_lib,
                                           const std::vector<CircuitPortId>& circuit_ports,
                                           const ModuleId& child_module,
                                           const size_t& child_index,
                                           const size_t& child_instance) {
  /* Wire inputs of parent module to inputs of child modules */
  for (const auto& port : circuit_ports) {
    ModulePortId src_port_id = module_manager.find_module_port(child_module, circuit_lib.port_lib_name(port));
    ModulePortId sink_port_id = module_manager.find_module_port(mem_module, circuit_lib.port_lib_name(port));
    for (size_t pin_id = 0; pin_id < module_manager.module_port(child_module, src_port_id).pins().size(); ++pin_id) {
      ModuleNetId net = module_manager.create_module_net(mem_module);
      /* Source pin is shifted by the number of memories */
      size_t src_pin_id = module_manager.module_port(child_module, src_port_id).pins()[pin_id];
      /* Source node of the input net is the input of memory module */
      module_manager.add_module_net_source(mem_module, net, child_module, child_instance, src_port_id, src_pin_id);
      /* Sink node of the input net is the input of sram module */
      size_t sink_pin_id = child_index * circuit_lib.port_size(port) + module_manager.module_port(mem_module, sink_port_id).pins()[pin_id];
      module_manager.add_module_net_sink(mem_module, net, mem_module, 0, sink_port_id, sink_pin_id);
    }
  }
}

/*********************************************************************
 * Add module nets to connect an output port of a configuration-chain
 * memory module to an output port of its child module
 * Restriction: this function is really designed for memory modules
 * 1. It assumes that output port name of child module is the same as memory module
 * 2. It assumes exact pin-to-pin mapping: 
 *     j-th pin of output port of the i-th child module is wired to the j + i*W -th
 *     pin of output port of the memory module, where W is the size of port 
 * 3. It assumes fixed port name for output ports
 ********************************************************************/
static 
void add_module_output_nets_to_chain_mem_modules(ModuleManager& module_manager,
                                                 const ModuleId& mem_module,
                                                 const std::string& mem_module_output_name,
                                                 const CircuitLibrary& circuit_lib,
                                                 const CircuitPortId& circuit_port,
                                                 const ModuleId& child_module,
                                                 const size_t& child_index,
                                                 const size_t& child_instance) {
  /* Wire inputs of parent module to inputs of child modules */
  ModulePortId src_port_id = module_manager.find_module_port(child_module, circuit_lib.port_lib_name(circuit_port));
  ModulePortId sink_port_id = module_manager.find_module_port(mem_module, mem_module_output_name);
  for (size_t pin_id = 0; pin_id < module_manager.module_port(child_module, src_port_id).pins().size(); ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(mem_module);
    /* Source pin is shifted by the number of memories */
    size_t src_pin_id = module_manager.module_port(child_module, src_port_id).pins()[pin_id];
    /* Source node of the input net is the input of memory module */
    module_manager.add_module_net_source(mem_module, net, child_module, child_instance, src_port_id, src_pin_id);
    /* Sink node of the input net is the input of sram module */
    size_t sink_pin_id = child_index * circuit_lib.port_size(circuit_port) + module_manager.module_port(mem_module, sink_port_id).pins()[pin_id];
    module_manager.add_module_net_sink(mem_module, net, mem_module, 0, sink_port_id, sink_pin_id);
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
 *
 *  Note that:
 *    This function is designed for memory modules ONLY!
 *    Do not use it to replace the 
 *      add_module_nets_cmos_memory_chain_config_bus() !!!
 *********************************************************************/
static 
void add_module_nets_to_cmos_memory_chain_module(ModuleManager& module_manager,
                                                 const ModuleId& parent_module,
                                                 const CircuitLibrary& circuit_lib,
                                                 const CircuitPortId& model_input_port,
                                                 const CircuitPortId& model_output_port) {
  for (size_t mem_index = 0; mem_index < module_manager.configurable_children(parent_module).size(); ++mem_index) {
    ModuleId net_src_module_id;
    size_t net_src_instance_id;
    ModulePortId net_src_port_id;

    ModuleId net_sink_module_id;
    size_t net_sink_instance_id;
    ModulePortId net_sink_port_id;

    if (0 == mem_index) {
      /* Find the port name of configuration chain head */
      std::string src_port_name = generate_configuration_chain_head_name();
      net_src_module_id = parent_module; 
      net_src_instance_id = 0;
      net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_lib_name(model_input_port);
      net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
      net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
      net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 
    } else {
      /* Find the port name of previous memory module */
      std::string src_port_name = circuit_lib.port_lib_name(model_output_port);
      net_src_module_id = module_manager.configurable_children(parent_module)[mem_index - 1]; 
      net_src_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index - 1];
      net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_lib_name(model_input_port);
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
  std::string src_port_name = circuit_lib.port_lib_name(model_output_port);
  ModuleId net_src_module_id = module_manager.configurable_children(parent_module).back(); 
  size_t net_src_instance_id = module_manager.configurable_child_instances(parent_module).back();
  ModulePortId net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

  /* Find the port name of next memory module */
  std::string sink_port_name = generate_configuration_chain_tail_name();
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
}

/*********************************************************************
 * Flat memory modules 
 *
 *        in[0]        in[1]           in[N]
 *          |            |               |
 *          v            v               v
 *      +-------+    +-------+       +-------+
 *      | SRAM  |    | SRAM  |  ...  | SRAM  |
 *      |  [0]  |    |  [1]  |       | [N-1] |
 *      +-------+    +-------+       +-------+
 *          |            |      ...      |
 *          v            v               v
 *      +------------------------------------+
 *      |   Multiplexer Configuration port   |
 *
 ********************************************************************/
static 
void build_memory_standalone_module(ModuleManager& module_manager,
                                    const CircuitLibrary& circuit_lib,
                                    const std::string& module_name,
                                    const CircuitModelId& sram_model,
                                    const size_t& num_mems) {
  /* Get the global ports required by the SRAM */
  std::vector<enum e_spice_model_port_type> global_port_types;
  global_port_types.push_back(SPICE_MODEL_PORT_CLOCK);
  global_port_types.push_back(SPICE_MODEL_PORT_INPUT);
  std::vector<CircuitPortId> sram_global_ports = circuit_lib.model_global_ports_by_type(sram_model, global_port_types, true, false);
  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_INPUT, true);
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_OUTPUT, true);

  /* Create a module and add to the module manager */
  ModuleId mem_module = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));
  
  /* Add each input port */
  for (const auto& port : sram_input_ports) {
    BasicPort input_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(mem_module, input_port, ModuleManager::MODULE_INPUT_PORT);
  }
  /* Add each output port: port width should match the number of memories */
  for (const auto& port : sram_output_ports) {
    BasicPort output_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(mem_module, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    size_t sram_mem_instance = module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);
    module_manager.add_configurable_child(mem_module, sram_mem_module, sram_mem_instance);

    /* Build module nets */
    /* Wire inputs of parent module to inputs of child modules */
    add_module_input_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_input_ports, sram_mem_module, i, sram_mem_instance);
    /* Wire inputs of parent module to outputs of child modules */
    add_module_output_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_output_ports, sram_mem_module, i, sram_mem_instance);
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);
}

/*********************************************************************
 * Scan-chain organization
 *              
 *                +-------+    +-------+            +-------+
 *  scan-chain--->| CCFF  |--->| CCFF  |--->... --->| CCFF  |---->scan-chain
 *  input&clock   |  [0]  |    |  [1]  |            | [N-1] |       output
 *                +-------+    +-------+            +-------+
 *                    |            |      ...           |
 *                    v            v                    v
 *                +-----------------------------------------+
 *                |   Multiplexer Configuration port        |
 *
 ********************************************************************/
static 
void build_memory_chain_module(ModuleManager& module_manager,
                               const CircuitLibrary& circuit_lib,
                               const std::string& module_name,
                               const CircuitModelId& sram_model,
                               const size_t& num_mems) {

  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_INPUT, true);
  /* Should have only 1 input port */
  VTR_ASSERT( 1 == sram_input_ports.size() );
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_OUTPUT, true);
  /* Should have only 1 or 2 output port */
  VTR_ASSERT( (1 == sram_output_ports.size()) || ( 2 == sram_output_ports.size()) );

  /* Create a module and add to the module manager */
  ModuleId mem_module = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));
  
  /* Add an input port, which is the head of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first input of the CCFF model as the D port,
   * which will be connected to the head of the chain
   */
  BasicPort chain_head_port(generate_configuration_chain_head_name(), 
                            circuit_lib.port_size(sram_input_ports[0]));
  module_manager.add_port(mem_module, chain_head_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add an output port, which is the tail of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first output of the CCFF model as the Q port,
   * which will be connected to the tail of the chain
   */
  BasicPort chain_tail_port(generate_configuration_chain_tail_name(), 
                            circuit_lib.port_size(sram_output_ports[0]));
  module_manager.add_port(mem_module, chain_tail_port, ModuleManager::MODULE_INPUT_PORT);

  /* Add each output port: port width should match the number of memories */
  for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
    std::string port_name;
    if (0 == iport) {
      port_name = generate_configuration_chain_data_out_name();
    } else {
      VTR_ASSERT( 1 == iport);
      port_name = generate_configuration_chain_inverted_data_out_name();
    }
    BasicPort output_port(port_name, num_mems);
    module_manager.add_port(mem_module, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    size_t sram_mem_instance = module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);
    module_manager.add_configurable_child(mem_module, sram_mem_module, sram_mem_instance);

    /* Build module nets to wire outputs of sram modules to outputs of memory module */
    for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
      std::string port_name;
      if (0 == iport) {
        port_name = generate_configuration_chain_data_out_name();
      } else {
        VTR_ASSERT( 1 == iport);
        port_name = generate_configuration_chain_inverted_data_out_name();
      }
      add_module_output_nets_to_chain_mem_modules(module_manager, mem_module, port_name, circuit_lib, sram_output_ports[iport],
                                                  sram_mem_module, i, sram_mem_instance);
    }
  }

  /* Build module nets to wire the configuration chain */
  add_module_nets_to_cmos_memory_chain_module(module_manager, mem_module, 
                                              circuit_lib, sram_input_ports[0], sram_output_ports[0]);


  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);
}

/*********************************************************************
 * Memory bank organization
 *
 *           Bit lines(BL/BLB) Word lines (WL/WLB)
 *               |               |
 *               v               v
 *      +------------------------------------+
 *      |   Memory Module Configuration port |
 *      +------------------------------------+
 *          |            |               |
 *          v            v               v
 *      +-------+    +-------+       +-------+
 *      | SRAM  |    | SRAM  |  ...  | SRAM  |
 *      |  [0]  |    |  [1]  |       | [N-1] |
 *      +-------+    +-------+       +-------+
 *          |            |      ...      |
 *          v            v               v
 *      +------------------------------------+
 *      |   Multiplexer Configuration port   |
 *
 ********************************************************************/
static 
void build_memory_bank_module(ModuleManager& module_manager,
                              const CircuitLibrary& circuit_lib,
                              const std::string& module_name,
                              const CircuitModelId& sram_model,
                              const size_t& num_mems) {
  /* Get the global ports required by the SRAM */
  std::vector<enum e_spice_model_port_type> global_port_types;
  global_port_types.push_back(SPICE_MODEL_PORT_CLOCK);
  global_port_types.push_back(SPICE_MODEL_PORT_INPUT);
  std::vector<CircuitPortId> sram_global_ports = circuit_lib.model_global_ports_by_type(sram_model, global_port_types, true, false);
  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_INPUT, true);
  /* A SRAM cell with BL/WL should not have any input */
  VTR_ASSERT( 0 == sram_input_ports.size() );
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_OUTPUT, true);
  /* Get the BL/WL ports from the SRAM */
  std::vector<CircuitPortId> sram_bl_ports  = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_BL, true);
  std::vector<CircuitPortId> sram_blb_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> sram_wl_ports  = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_WL, true);
  std::vector<CircuitPortId> sram_wlb_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_WLB, true);

  /* Create a module and add to the module manager */
  ModuleId mem_module = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));
  
  /* Add module ports: the ports come from the SRAM modules */
  /* Add each input port */
  for (const auto& port : sram_input_ports) {
    BasicPort input_port(circuit_lib.port_lib_name(port), num_mems * circuit_lib.port_size(port));
    module_manager.add_port(mem_module, input_port, ModuleManager::MODULE_INPUT_PORT);
  }
  /* Add each output port: port width should match the number of memories */
  for (const auto& port : sram_output_ports) {
    BasicPort output_port(circuit_lib.port_lib_name(port), num_mems * circuit_lib.port_size(port));
    module_manager.add_port(mem_module, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }
  /* Add each output port: port width should match the number of memories */
  for (const auto& port : sram_bl_ports) {
    BasicPort bl_port(circuit_lib.port_lib_name(port), num_mems * circuit_lib.port_size(port));
    module_manager.add_port(mem_module, bl_port, ModuleManager::MODULE_INPUT_PORT);
  }
  for (const auto& port : sram_blb_ports) {
    BasicPort blb_port(circuit_lib.port_lib_name(port), num_mems * circuit_lib.port_size(port));
    module_manager.add_port(mem_module, blb_port, ModuleManager::MODULE_INPUT_PORT);
  }
  for (const auto& port : sram_wl_ports) {
    BasicPort wl_port(circuit_lib.port_lib_name(port), num_mems * circuit_lib.port_size(port));
    module_manager.add_port(mem_module, wl_port, ModuleManager::MODULE_INPUT_PORT);
  }
  for (const auto& port : sram_wlb_ports) {
    BasicPort wlb_port(circuit_lib.port_lib_name(port), num_mems * circuit_lib.port_size(port));
    module_manager.add_port(mem_module, wlb_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    /* Memory seed module instanciation */
    size_t sram_instance = module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);

    /* Build module nets */
    /* Wire inputs of parent module to inputs of child modules */
    add_module_input_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_input_ports, sram_mem_module, i, sram_instance);
    /* Wire inputs of parent module to outputs of child modules */
    add_module_output_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_output_ports, sram_mem_module, i, sram_instance);
    /* Wire BL/WLs of parent module to BL/WLs of child modules */
    add_module_input_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_bl_ports, sram_mem_module, i, sram_instance);
    add_module_input_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_blb_ports, sram_mem_module, i, sram_instance);
    add_module_input_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_wl_ports, sram_mem_module, i, sram_instance);
    add_module_input_nets_to_mem_modules(module_manager, mem_module, circuit_lib, sram_wlb_ports, sram_mem_module, i, sram_instance);
  }

  /* TODO: if a local memory decoder is required, instanciate it here */

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);
}


/*********************************************************************
 * Generate Verilog modules for the memories that are used
 * by a circuit model 
 * The organization of memory circuit will depend on the style of
 * configuration protocols
 * Currently, we support 
 * 1. Flat SRAM organization
 * 2. Configuration chain
 * 3. Memory bank (memory decoders)
 ********************************************************************/
static 
void build_memory_module(ModuleManager& module_manager,
                         const CircuitLibrary& circuit_lib,
                         const e_sram_orgz& sram_orgz_type,
                         const std::string& module_name,
                         const CircuitModelId& sram_model,
                         const size_t& num_mems) {
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    build_memory_standalone_module(module_manager, circuit_lib, 
                                           module_name, sram_model, num_mems);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    build_memory_chain_module(module_manager, circuit_lib,  
                              module_name, sram_model, num_mems);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    build_memory_bank_module(module_manager, circuit_lib,
                             module_name, sram_model, num_mems);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, LINE%d) Invalid SRAM organization!\n",
               __FILE__, __LINE__);
  }
}


/*********************************************************************
 * Generate Verilog modules for the memories that are used
 * by multiplexers  
 *
 *            +----------------+
 * mem_in --->|  Memory Module |---> mem_out
 *            +----------------+
 *              |  |  ... |  | 
 *              v  v      v  v SRAM ports of multiplexer
 *          +---------------------+
 *    in--->|  Multiplexer Module |---> out
 *          +---------------------+
 ********************************************************************/
static 
void build_mux_memory_module(ModuleManager& module_manager,
                             const CircuitLibrary& circuit_lib,
                             const e_sram_orgz& sram_orgz_type,
                             const CircuitModelId& mux_model,
                             const MuxGraph& mux_graph) {
  /* Find the actual number of configuration bits, based on the mux graph 
   * Due to the use of local decoders inside mux, this may be 
   */
  size_t num_config_bits = find_mux_num_config_bits(circuit_lib, mux_model, mux_graph, sram_orgz_type);
  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(mux_model)) {
  case SPICE_MODEL_DESIGN_CMOS: {
    /* Generate module name */
    std::string module_name = generate_mux_subckt_name(circuit_lib, mux_model, 
                                                       find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs()), 
                                                       std::string(MEMORY_MODULE_POSTFIX));

    /* Get the sram ports from the mux */
    std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, mux_model);
    VTR_ASSERT( 1 == sram_models.size() );

    build_memory_module(module_manager, circuit_lib, sram_orgz_type, module_name, sram_models[0], num_config_bits);
    break;
  }
  case SPICE_MODEL_DESIGN_RRAM:
    /* We do not need a memory submodule for RRAM MUX,
     * RRAM are embedded in the datapath  
     * TODO: generate local encoders for RRAM-based multiplexers here!!!
     */ 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d]) Invalid design technology of multiplexer (name: %s)\n",
               __FILE__, __LINE__, circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }
}

/*********************************************************************
 * Build modules for 
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 *
 * We keep the memory modules separated from the multiplexers and other 
 * programmable circuit models, for the sake of supporting
 * various configuration schemes.
 * By following such organiztion, the Verilog modules of the circuit models
 * implements the functionality (circuit logic) only, while the memory Verilog
 * modules implements the memory circuits as well as configuration protocols.
 * For example, the local decoders of multiplexers are implemented in the
 * memory modules. 
 * Take another example, the memory circuit can implement the scan-chain or 
 * memory-bank organization for the memories.
 ********************************************************************/
void build_memory_modules(ModuleManager& module_manager,
                          const MuxLibrary& mux_lib,
                          const CircuitLibrary& circuit_lib,
                          const e_sram_orgz& sram_orgz_type) {
  /* Start time count */
  clock_t t_start = clock();

  vpr_printf(TIO_MESSAGE_INFO,
             "Building memory modules...");
  
  /* Create the memory circuits for the multiplexer */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_model = mux_lib.mux_circuit_model(mux); 
    /* Bypass the non-MUX circuit models (i.e., LUTs). 
     * They should be handled in a different way 
     * Memory circuits of LUT includes both regular and mode-select ports
     */
    if (SPICE_MODEL_MUX != circuit_lib.model_type(mux_model)) {
      continue;
    }
    /* Create a Verilog module for the memories used by the multiplexer */
    build_mux_memory_module(module_manager, circuit_lib, sram_orgz_type, mux_model, mux_graph);
  }

  /* Create the memory circuits for non-MUX circuit models.
   * In this case, the memory modules are designed to interface
   * the mode-select ports 
   */
  for (const auto& model : circuit_lib.models()) {
    /* Bypass MUXes, they have already been considered */
    if (SPICE_MODEL_MUX == circuit_lib.model_type(model)) {
      continue;
    }
    /* Bypass those modules without any SRAM ports */
    std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(model, SPICE_MODEL_PORT_SRAM, true);
    if (0 == sram_ports.size()) {
      continue;
    }
    /* Find the name of memory module */
    /* Get the total number of SRAMs */
    size_t num_mems = 0; 
    for (const auto& port : sram_ports) {
      num_mems += circuit_lib.port_size(port);
    }
    /* Get the circuit model for the memory circuit used by the multiplexer */
    std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, model);
    /* Should have only 1 SRAM model */
    VTR_ASSERT( 1 == sram_models.size() );
  
    /* Create the module name for the memory block */
    std::string module_name = generate_memory_module_name(circuit_lib, model, sram_models[0], std::string(MEMORY_MODULE_POSTFIX));

    /* Create a Verilog module for the memories used by the circuit model */
    build_memory_module(module_manager, circuit_lib, sram_orgz_type, module_name, sram_models[0], num_mems);
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %.2g seconds\n", 
             run_time_sec);  
}

