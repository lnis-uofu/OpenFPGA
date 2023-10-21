/*********************************************************************
 * This file includes functions to generate Verilog submodules for
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 ********************************************************************/
#include "build_memory_modules.h"

#include <algorithm>
#include <ctime>
#include <map>
#include <string>

#include "build_decoder_modules.h"
#include "circuit_library_utils.h"
#include "command_exit_codes.h"
#include "decoder_library_utils.h"
#include "memory_utils.h"
#include "module_manager.h"
#include "module_manager_utils.h"
#include "mux_graph.h"
#include "mux_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Add module nets to connect an input port of a memory module to
 * an input port of its child module
 * Restriction: this function is really designed for memory modules
 * 1. It assumes that input port name of child module is the same as memory
 *module
 * 2. It assumes exact pin-to-pin mapping:
 *     j-th pin of input port of the i-th child module is wired to the j + i*W
 *-th pin of input port of the memory module, where W is the size of port
 ********************************************************************/
static void add_module_input_nets_to_mem_modules(
  ModuleManager& module_manager, const ModuleId& mem_module,
  const ModulePortId& module_port, const CircuitLibrary& circuit_lib,
  const CircuitPortId& circuit_port, const ModuleId& child_module,
  const size_t& child_index, const size_t& child_instance) {
  /* Wire inputs of parent module to inputs of child modules */
  ModulePortId src_port_id = module_port;
  ModulePortId sink_port_id = module_manager.find_module_port(
    child_module, circuit_lib.port_prefix(circuit_port));

  /* Source pin is shifted by the number of memories */
  size_t src_pin_id =
    module_manager.module_port(mem_module, src_port_id).pins()[child_index];

  ModuleNetId net = module_manager.module_instance_port_net(
    mem_module, mem_module, 0, src_port_id, src_pin_id);
  if (ModuleNetId::INVALID() == net) {
    net = module_manager.create_module_net(mem_module);
    module_manager.add_module_net_source(mem_module, net, mem_module, 0,
                                         src_port_id, src_pin_id);
  }

  for (size_t pin_id = 0;
       pin_id <
       module_manager.module_port(child_module, sink_port_id).pins().size();
       ++pin_id) {
    /* Sink node of the input net is the input of sram module */
    size_t sink_pin_id =
      module_manager.module_port(child_module, sink_port_id).pins()[pin_id];
    module_manager.add_module_net_sink(
      mem_module, net, child_module, child_instance, sink_port_id, sink_pin_id);
  }
}

/*********************************************************************
 * Add module nets to connect an output port of a configuration-chain
 * memory module to an output port of its child module
 * Restriction: this function is really designed for memory modules
 * 1. It assumes that output port name of child module is the same as memory
 *module
 * 2. It assumes exact pin-to-pin mapping:
 *     j-th pin of output port of the i-th child module is wired to the j + i*W
 *-th pin of output port of the memory module, where W is the size of port
 * 3. It assumes fixed port name for output ports
 ********************************************************************/
std::vector<ModuleNetId> add_module_output_nets_to_chain_mem_modules(
  ModuleManager& module_manager, const ModuleId& mem_module,
  const std::string& mem_module_output_name, const CircuitLibrary& circuit_lib,
  const CircuitPortId& circuit_port, const ModuleId& child_module,
  const size_t& child_index, const size_t& child_instance) {
  std::vector<ModuleNetId> module_nets;

  /* Wire inputs of parent module to inputs of child modules */
  ModulePortId src_port_id = module_manager.find_module_port(
    child_module, circuit_lib.port_prefix(circuit_port));
  ModulePortId sink_port_id =
    module_manager.find_module_port(mem_module, mem_module_output_name);
  for (size_t pin_id = 0;
       pin_id <
       module_manager.module_port(child_module, src_port_id).pins().size();
       ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(mem_module);
    /* Source pin is shifted by the number of memories */
    size_t src_pin_id =
      module_manager.module_port(child_module, src_port_id).pins()[pin_id];
    /* Source node of the input net is the input of memory module */
    module_manager.add_module_net_source(
      mem_module, net, child_module, child_instance, src_port_id, src_pin_id);
    /* Sink node of the input net is the input of sram module */
    size_t sink_pin_id =
      child_index * circuit_lib.port_size(circuit_port) +
      module_manager.module_port(mem_module, sink_port_id).pins()[pin_id];
    module_manager.add_module_net_sink(mem_module, net, mem_module, 0,
                                       sink_port_id, sink_pin_id);

    /* Cache the nets */
    module_nets.push_back(net);
  }

  return module_nets;
}

/*********************************************************************
 * Add module nets to connect an output port of a memory module to
 * an output port of its child module
 * Restriction: this function is really designed for memory modules
 * 1. It assumes that output port name of child module is the same as memory
 *module
 * 2. It assumes exact pin-to-pin mapping:
 *     j-th pin of output port of the i-th child module is wired to the j + i*W
 *-th pin of output port of the memory module, where W is the size of port
 ********************************************************************/
static void add_module_output_nets_to_mem_modules(
  ModuleManager& module_manager, const ModuleId& mem_module,
  const CircuitLibrary& circuit_lib,
  const std::vector<CircuitPortId>& sram_output_ports,
  const ModuleId& child_module, const size_t& child_index,
  const size_t& child_instance) {
  for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
    std::string port_name;
    if (0 == iport) {
      port_name = generate_configurable_memory_data_out_name();
    } else {
      VTR_ASSERT(1 == iport);
      port_name = generate_configurable_memory_inverted_data_out_name();
    }
    add_module_output_nets_to_chain_mem_modules(
      module_manager, mem_module, port_name, circuit_lib,
      sram_output_ports[iport], child_module, child_index, child_instance);
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
static void add_module_nets_to_cmos_memory_config_chain_module(
  ModuleManager& module_manager, const ModuleId& parent_module,
  const CircuitLibrary& circuit_lib, const CircuitPortId& model_input_port,
  const CircuitPortId& model_output_port) {
  for (size_t mem_index = 0;
       mem_index <
       module_manager
         .configurable_children(parent_module,
                                ModuleManager::e_config_child_type::LOGICAL)
         .size();
       ++mem_index) {
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
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_prefix(model_input_port);
      net_sink_module_id = module_manager.configurable_children(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_instance_id = module_manager.configurable_child_instances(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_port_id =
        module_manager.find_module_port(net_sink_module_id, sink_port_name);
    } else {
      /* Find the port name of previous memory module */
      std::string src_port_name = circuit_lib.port_prefix(model_output_port);
      net_src_module_id = module_manager.configurable_children(
        parent_module,
        ModuleManager::e_config_child_type::LOGICAL)[mem_index - 1];
      net_src_instance_id = module_manager.configurable_child_instances(
        parent_module,
        ModuleManager::e_config_child_type::LOGICAL)[mem_index - 1];
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_prefix(model_input_port);
      net_sink_module_id = module_manager.configurable_children(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_instance_id = module_manager.configurable_child_instances(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_port_id =
        module_manager.find_module_port(net_sink_module_id, sink_port_name);
    }

    /* Get the pin id for source port */
    BasicPort net_src_port =
      module_manager.module_port(net_src_module_id, net_src_port_id);
    /* Get the pin id for sink port */
    BasicPort net_sink_port =
      module_manager.module_port(net_sink_module_id, net_sink_port_id);
    /* Port sizes of source and sink should match */
    VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());

    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(
        module_manager, parent_module, net_src_module_id, net_src_instance_id,
        net_src_port_id, net_src_port.pins()[pin_id]);
      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id,
                                         net_sink_instance_id, net_sink_port_id,
                                         net_sink_port.pins()[pin_id]);
    }
  }

  /* For the last memory module:
   *    net source is the configuration chain tail of the previous memory module
   *    net sink is the configuration chain tail of the primitive module
   */
  /* Find the port name of previous memory module */
  std::string src_port_name = circuit_lib.port_prefix(model_output_port);
  ModuleId net_src_module_id =
    module_manager
      .configurable_children(parent_module,
                             ModuleManager::e_config_child_type::LOGICAL)
      .back();
  size_t net_src_instance_id =
    module_manager
      .configurable_child_instances(parent_module,
                                    ModuleManager::e_config_child_type::LOGICAL)
      .back();
  ModulePortId net_src_port_id =
    module_manager.find_module_port(net_src_module_id, src_port_name);

  /* Find the port name of next memory module */
  std::string sink_port_name = generate_configuration_chain_tail_name();
  ModuleId net_sink_module_id = parent_module;
  size_t net_sink_instance_id = 0;
  ModulePortId net_sink_port_id =
    module_manager.find_module_port(net_sink_module_id, sink_port_name);

  /* Get the pin id for source port */
  BasicPort net_src_port =
    module_manager.module_port(net_src_module_id, net_src_port_id);
  /* Get the pin id for sink port */
  BasicPort net_sink_port =
    module_manager.module_port(net_sink_module_id, net_sink_port_id);
  /* Port sizes of source and sink should match */
  VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());

  /* Create a net for each pin */
  for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
    /* Create a net and add source and sink to it */
    ModuleNetId net = create_module_source_pin_net(
      module_manager, parent_module, net_src_module_id, net_src_instance_id,
      net_src_port_id, net_src_port.pins()[pin_id]);
    /* Add net sink */
    module_manager.add_module_net_sink(parent_module, net, net_sink_module_id,
                                       net_sink_instance_id, net_sink_port_id,
                                       net_sink_port.pins()[pin_id]);
  }
}

/********************************************************************
 * Connect the scan input of all the memory modules
 * under the parent module in a chain
 *
 *                +--------+    +--------+            +--------+
 *  ccff_head --->| Memory |--->| Memory |--->... --->| Memory |
 *                | Module |    | Module |            | Module |
 *                |   [0]  |    |   [1]  |            |  [N-1] |
 *                +--------+    +--------+            +--------+
 *  For the 1st memory module:
 *    net source is the configuration chain head of the primitive module
 *    net sink is the scan input of the next memory module
 *
 *  For the rest of memory modules:
 *    net source is the configuration chain tail of the previous memory module
 *    net sink is the scan input of the next memory module
 *
 *  Note that:
 *    This function is designed for memory modules ONLY!
 *    Do not use it to replace the
 *      add_module_nets_cmos_memory_chain_config_bus() !!!
 *********************************************************************/
static void add_module_nets_to_cmos_memory_scan_chain_module(
  ModuleManager& module_manager, const ModuleId& parent_module,
  const CircuitLibrary& circuit_lib, const CircuitPortId& model_input_port,
  const CircuitPortId& model_output_port) {
  for (size_t mem_index = 0;
       mem_index <
       module_manager
         .configurable_children(parent_module,
                                ModuleManager::e_config_child_type::LOGICAL)
         .size();
       ++mem_index) {
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
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_prefix(model_input_port);
      net_sink_module_id = module_manager.configurable_children(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_instance_id = module_manager.configurable_child_instances(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_port_id =
        module_manager.find_module_port(net_sink_module_id, sink_port_name);
    } else {
      /* Find the port name of previous memory module */
      std::string src_port_name = circuit_lib.port_prefix(model_output_port);
      net_src_module_id = module_manager.configurable_children(
        parent_module,
        ModuleManager::e_config_child_type::LOGICAL)[mem_index - 1];
      net_src_instance_id = module_manager.configurable_child_instances(
        parent_module,
        ModuleManager::e_config_child_type::LOGICAL)[mem_index - 1];
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_prefix(model_input_port);
      net_sink_module_id = module_manager.configurable_children(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_instance_id = module_manager.configurable_child_instances(
        parent_module, ModuleManager::e_config_child_type::LOGICAL)[mem_index];
      net_sink_port_id =
        module_manager.find_module_port(net_sink_module_id, sink_port_name);
    }

    /* Get the pin id for source port */
    BasicPort net_src_port =
      module_manager.module_port(net_src_module_id, net_src_port_id);
    /* Get the pin id for sink port */
    BasicPort net_sink_port =
      module_manager.module_port(net_sink_module_id, net_sink_port_id);
    /* Port sizes of source and sink should match */
    VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());

    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(
        module_manager, parent_module, net_src_module_id, net_src_instance_id,
        net_src_port_id, net_src_port.pins()[pin_id]);
      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id,
                                         net_sink_instance_id, net_sink_port_id,
                                         net_sink_port.pins()[pin_id]);
    }
  }
}

/*********************************************************************
 * Flatten memory organization
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
static void build_memory_flatten_module(ModuleManager& module_manager,
                                        const CircuitLibrary& circuit_lib,
                                        const std::string& module_name,
                                        const CircuitModelId& sram_model,
                                        const size_t& num_mems,
                                        const bool& verbose) {
  /* Get the global ports required by the SRAM */
  std::vector<enum e_circuit_model_port_type> global_port_types;
  global_port_types.push_back(CIRCUIT_MODEL_PORT_CLOCK);
  global_port_types.push_back(CIRCUIT_MODEL_PORT_INPUT);
  std::vector<CircuitPortId> sram_global_ports =
    circuit_lib.model_global_ports_by_type(sram_model, global_port_types, true,
                                           false);
  /* Get the BL/WL ports from the SRAM */
  std::vector<CircuitPortId> sram_bl_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_BL, true);
  std::vector<CircuitPortId> sram_wl_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WL, true);
  /* Optional: Get the WLR ports from the SRAM */
  std::vector<CircuitPortId> sram_wlr_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WLR, true);
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_OUTPUT,
                                    true);

  /* Ensure that we have only 1 BL, 1 WL and 2 output ports, as well as an
   * optional WLR*/
  VTR_ASSERT(1 == sram_bl_ports.size());
  VTR_ASSERT(1 == sram_wl_ports.size());
  VTR_ASSERT(2 > sram_wlr_ports.size());
  VTR_ASSERT(2 == sram_output_ports.size());

  /* Create a module and add to the module manager */
  VTR_LOGV(verbose, "Building memory module '%s'\n", module_name.c_str());
  ModuleId mem_module = module_manager.add_module(module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));

  /* Label module usage */
  module_manager.set_module_usage(mem_module, ModuleManager::MODULE_CONFIG);

  /* Add module ports */
  /* Input: BL port */
  BasicPort bl_port(std::string(MEMORY_BL_PORT_NAME), num_mems);
  ModulePortId mem_bl_port = module_manager.add_port(
    mem_module, bl_port, ModuleManager::MODULE_INPUT_PORT);

  BasicPort wl_port(std::string(MEMORY_WL_PORT_NAME), num_mems);
  ModulePortId mem_wl_port = module_manager.add_port(
    mem_module, wl_port, ModuleManager::MODULE_INPUT_PORT);

  BasicPort wlr_port(std::string(MEMORY_WLR_PORT_NAME), num_mems);
  ModulePortId mem_wlr_port = ModulePortId::INVALID();
  if (!sram_wlr_ports.empty()) {
    mem_wlr_port = module_manager.add_port(mem_module, wlr_port,
                                           ModuleManager::MODULE_INPUT_PORT);
  }

  /* Add each output port: port width should match the number of memories */
  for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
    std::string port_name;
    if (0 == iport) {
      port_name = generate_configurable_memory_data_out_name();
    } else {
      VTR_ASSERT(1 == iport);
      port_name = generate_configurable_memory_inverted_data_out_name();
    }
    BasicPort output_port(port_name, num_mems);
    module_manager.add_port(mem_module, output_port,
                            ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module =
    module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    size_t sram_mem_instance =
      module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);
    module_manager.add_configurable_child(
      mem_module, sram_mem_module, sram_mem_instance,
      ModuleManager::e_config_child_type::UNIFIED);

    /* Build module nets */
    /* Wire inputs of parent module to inputs of child modules */
    for (const CircuitPortId& port : sram_bl_ports) {
      add_module_input_nets_to_mem_modules(
        module_manager, mem_module, mem_bl_port, circuit_lib, port,
        sram_mem_module, i, sram_mem_instance);
    }
    for (const CircuitPortId& port : sram_wl_ports) {
      add_module_input_nets_to_mem_modules(
        module_manager, mem_module, mem_wl_port, circuit_lib, port,
        sram_mem_module, i, sram_mem_instance);
    }
    for (const CircuitPortId& port : sram_wlr_ports) {
      add_module_input_nets_to_mem_modules(
        module_manager, mem_module, mem_wlr_port, circuit_lib, port,
        sram_mem_module, i, sram_mem_instance);
    }
    /* Wire outputs of child module to outputs of parent module */
    add_module_output_nets_to_mem_modules(
      module_manager, mem_module, circuit_lib, sram_output_ports,
      sram_mem_module, i, sram_mem_instance);
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
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
 *                    |            |      ...           | config-memory output
 *                    v            v                    v
 *                +-----------------------------------------+
 *                |   Multiplexer Configuration port        |
 *
 ********************************************************************/
static void build_memory_chain_module(ModuleManager& module_manager,
                                      const CircuitLibrary& circuit_lib,
                                      const std::string& module_name,
                                      const CircuitModelId& sram_model,
                                      const size_t& num_mems,
                                      const bool& verbose) {
  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* Should have only 1 or 2 input port */
  VTR_ASSERT((1 == sram_input_ports.size()) || (2 == sram_input_ports.size()));
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_OUTPUT,
                                    true);
  /* Should have only 1 or 2 or 3 output port */
  VTR_ASSERT((1 == sram_output_ports.size()) ||
             (2 == sram_output_ports.size()) ||
             (3 == sram_output_ports.size()));

  /* Create a module and add to the module manager */
  VTR_LOGV(verbose, "Building memory module '%s'\n", module_name.c_str());
  ModuleId mem_module = module_manager.add_module(module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));

  /* Label module usage */
  module_manager.set_module_usage(mem_module, ModuleManager::MODULE_CONFIG);

  /* Add an input port, which is the head of configuration chain in the module
   */
  /* TODO: restriction!!!
   * consider only the first input of the CCFF model as the D port,
   * which will be connected to the head of the chain
   */
  BasicPort chain_head_port(generate_configuration_chain_head_name(),
                            circuit_lib.port_size(sram_input_ports[0]));
  module_manager.add_port(mem_module, chain_head_port,
                          ModuleManager::MODULE_INPUT_PORT);
  /* Add an output port, which is the tail of configuration chain in the module
   */
  /* TODO: restriction!!!
   * consider only the first output of the CCFF model as the Q port,
   * which will be connected to the tail of the chain
   */
  BasicPort chain_tail_port(generate_configuration_chain_tail_name(),
                            circuit_lib.port_size(sram_output_ports[0]));
  module_manager.add_port(mem_module, chain_tail_port,
                          ModuleManager::MODULE_OUTPUT_PORT);

  /* There could be 3 conditions w.r.t. the number of output ports:
   * - Only one output port is defined. In this case, the 1st port is the Q
   *   In such case, only Q will be considered as data output ports
   * - Two output port is defined. In this case, the 1st port is the Q while the
   * 2nd port is the QN In such case, both Q and QN will be considered as data
   * output ports
   * - Three output port is defined.
   *   In this case:
   *   - the 1st port is the Q (the chain output)
   *   - the 2nd port is the QN (the inverted data output)
   *   - the 3nd port is the configure-enabled Q
   *   In such case, configure-enabled Q and QN will be considered as data
   * output ports
   */
  size_t num_data_output_ports = sram_output_ports.size();
  if (3 == sram_output_ports.size()) {
    num_data_output_ports = 2;
  }
  for (size_t iport = 0; iport < num_data_output_ports; ++iport) {
    std::string port_name;
    if (0 == iport) {
      port_name = generate_configurable_memory_data_out_name();
    } else if (1 == iport) {
      port_name = generate_configurable_memory_inverted_data_out_name();
    }
    BasicPort output_port(port_name, num_mems);
    module_manager.add_port(mem_module, output_port,
                            ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module =
    module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    size_t sram_mem_instance =
      module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);
    module_manager.add_configurable_child(
      mem_module, sram_mem_module, sram_mem_instance,
      ModuleManager::e_config_child_type::UNIFIED);

    /* Build module nets to wire outputs of sram modules to outputs of memory
     * module */
    for (size_t iport = 0; iport < num_data_output_ports; ++iport) {
      std::string port_name;
      if (0 == iport) {
        port_name = generate_configurable_memory_data_out_name();
      } else {
        VTR_ASSERT(1 == iport);
        port_name = generate_configurable_memory_inverted_data_out_name();
      }
      /* Find the proper data output port
       * The exception is when there are 3 output ports defined
       * The 3rd port is the regular data output port to be used
       */
      CircuitPortId data_output_port_to_connect = sram_output_ports[iport];
      if ((3 == sram_output_ports.size()) && (0 == iport)) {
        data_output_port_to_connect = sram_output_ports.back();
      }

      std::vector<ModuleNetId> output_nets =
        add_module_output_nets_to_chain_mem_modules(
          module_manager, mem_module, port_name, circuit_lib,
          data_output_port_to_connect, sram_mem_module, i, sram_mem_instance);
    }
  }

  /* Build module nets to wire the configuration chain */
  add_module_nets_to_cmos_memory_config_chain_module(
    module_manager, mem_module, circuit_lib, sram_input_ports[0],
    sram_output_ports[0]);

  /* If there is a second input defined,
   * add nets to short wire the 2nd inputs to the first inputs
   */
  if (2 == sram_input_ports.size()) {
    add_module_nets_to_cmos_memory_scan_chain_module(
      module_manager, mem_module, circuit_lib, sram_input_ports[1],
      sram_output_ports[0]);
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);
}

/*********************************************************************
 * Frame-based Memory organization
 *
 *              EN      Address    Data
 *               |         |         |
 *               v         v         v
 *      +------------------------------------+
 *      |           Address Decoder          |
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
static void build_frame_memory_module(ModuleManager& module_manager,
                                      DecoderLibrary& frame_decoder_lib,
                                      const CircuitLibrary& circuit_lib,
                                      const std::string& module_name,
                                      const CircuitModelId& sram_model,
                                      const size_t& num_mems,
                                      const bool& verbose) {
  /* Get the global ports required by the SRAM */
  std::vector<enum e_circuit_model_port_type> global_port_types;
  global_port_types.push_back(CIRCUIT_MODEL_PORT_CLOCK);
  global_port_types.push_back(CIRCUIT_MODEL_PORT_INPUT);
  std::vector<CircuitPortId> sram_global_ports =
    circuit_lib.model_global_ports_by_type(sram_model, global_port_types, true,
                                           false);
  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* A SRAM cell with BL/WL should not have any input */
  VTR_ASSERT(0 == sram_input_ports.size());

  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_OUTPUT,
                                    true);

  /* Get the BL/WL ports from the SRAM
   * Here, we consider that the WL port will be EN signal of a SRAM
   * and the BL port will be the data_in signal of a SRAM
   */
  std::vector<CircuitPortId> sram_bl_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_BL, true);
  std::vector<CircuitPortId> sram_blb_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> sram_wl_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WL, true);
  std::vector<CircuitPortId> sram_wlb_ports =
    circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WLB, true);

  /* We do NOT expect any BLB port here!!!
   * TODO: to suppor this, we need an inverter circuit model to be specified by
   * users !!!
   */
  VTR_ASSERT(1 == sram_bl_ports.size());
  VTR_ASSERT(1 == circuit_lib.port_size(sram_bl_ports[0]));
  VTR_ASSERT(1 == sram_wl_ports.size());
  VTR_ASSERT(1 == circuit_lib.port_size(sram_wl_ports[0]));
  VTR_ASSERT(0 == sram_blb_ports.size());

  /* Create a module and add to the module manager */
  VTR_LOGV(verbose, "Building memory module '%s'\n", module_name.c_str());
  ModuleId mem_module = module_manager.add_module(module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));

  /* Label module usage */
  module_manager.set_module_usage(mem_module, ModuleManager::MODULE_CONFIG);

  /* Find the specification of the decoder:
   * Size of address port and data input
   */
  size_t addr_size = find_mux_local_decoder_addr_size(num_mems);
  /* Data input should match the WL (data_in) of a SRAM */
  size_t data_size = num_mems * circuit_lib.port_size(sram_bl_ports[0]);
  bool use_data_inv = (0 < sram_blb_ports.size());

  /* Search the decoder library
   * If we find one, we use the module.
   * Otherwise, we create one and add it to the decoder library
   */
  DecoderId decoder_id = frame_decoder_lib.find_decoder(
    addr_size, data_size, true, false, use_data_inv, false);
  if (DecoderId::INVALID() == decoder_id) {
    decoder_id = frame_decoder_lib.add_decoder(addr_size, data_size, true,
                                               false, use_data_inv, false);
  }
  VTR_ASSERT(DecoderId::INVALID() != decoder_id);

  /* Create a module if not existed yet */
  std::string decoder_module_name =
    generate_memory_decoder_subckt_name(addr_size, data_size);
  ModuleId decoder_module = module_manager.find_module(decoder_module_name);
  if (ModuleId::INVALID() == decoder_module) {
    decoder_module = build_frame_memory_decoder_module(
      module_manager, frame_decoder_lib, decoder_id);
  }
  VTR_ASSERT(ModuleId::INVALID() != decoder_module);

  /* Add module ports */
  /* Input: Enable port */
  BasicPort en_port(std::string(DECODER_ENABLE_PORT_NAME), 1);
  ModulePortId mem_en_port = module_manager.add_port(
    mem_module, en_port, ModuleManager::MODULE_INPUT_PORT);

  /* Input: Address port */
  BasicPort addr_port(std::string(DECODER_ADDRESS_PORT_NAME), addr_size);
  ModulePortId mem_addr_port = module_manager.add_port(
    mem_module, addr_port, ModuleManager::MODULE_INPUT_PORT);

  /* Input: Data port */
  BasicPort data_port(std::string(DECODER_DATA_IN_PORT_NAME), 1);
  ModulePortId mem_data_port = module_manager.add_port(
    mem_module, data_port, ModuleManager::MODULE_INPUT_PORT);

  /* Should have only 1 or 2 output port */
  VTR_ASSERT((1 == sram_output_ports.size()) ||
             (2 == sram_output_ports.size()));

  /* Add each output port: port width should match the number of memories */
  for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
    std::string port_name;
    if (0 == iport) {
      port_name = generate_configurable_memory_data_out_name();
    } else {
      VTR_ASSERT(1 == iport);
      port_name = generate_configurable_memory_inverted_data_out_name();
    }
    BasicPort output_port(port_name, num_mems);
    module_manager.add_port(mem_module, output_port,
                            ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* Instanciate the decoder module here */
  VTR_ASSERT(0 == module_manager.num_instance(mem_module, decoder_module));
  module_manager.add_child_module(mem_module, decoder_module);

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module =
    module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Build module nets */
  /* Wire enable port to decoder enable port */
  ModulePortId decoder_en_port = module_manager.find_module_port(
    decoder_module, std::string(DECODER_ENABLE_PORT_NAME));
  add_module_bus_nets(module_manager, mem_module, mem_module, 0, mem_en_port,
                      decoder_module, 0, decoder_en_port);

  /* Wire address port to decoder address port */
  ModulePortId decoder_addr_port = module_manager.find_module_port(
    decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
  add_module_bus_nets(module_manager, mem_module, mem_module, 0, mem_addr_port,
                      decoder_module, 0, decoder_addr_port);

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    /* Memory seed module instanciation */
    size_t sram_instance =
      module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);
    module_manager.add_configurable_child(
      mem_module, sram_mem_module, sram_instance,
      ModuleManager::e_config_child_type::UNIFIED);

    /* Wire data_in port to SRAM BL port */
    ModulePortId sram_bl_port = module_manager.find_module_port(
      sram_mem_module, circuit_lib.port_prefix(sram_bl_ports[0]));
    add_module_bus_nets(module_manager, mem_module, mem_module, 0,
                        mem_data_port, sram_mem_module, sram_instance,
                        sram_bl_port);

    /* Wire decoder data_out port to sram WL ports */
    ModulePortId sram_wl_port = module_manager.find_module_port(
      sram_mem_module, circuit_lib.port_prefix(sram_wl_ports[0]));
    ModulePortId decoder_data_port = module_manager.find_module_port(
      decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
    ModuleNetId wl_net = module_manager.create_module_net(mem_module);
    /* Source node of the input net is the input of memory module */
    module_manager.add_module_net_source(mem_module, wl_net, decoder_module, 0,
                                         decoder_data_port, sram_instance);
    module_manager.add_module_net_sink(mem_module, wl_net, sram_mem_module,
                                       sram_instance, sram_wl_port, 0);

    /* Optional: Wire decoder data_out inverted port to sram WLB ports */
    if (true == use_data_inv) {
      ModulePortId sram_wlb_port = module_manager.find_module_port(
        sram_mem_module, circuit_lib.port_lib_name(sram_wlb_ports[0]));
      ModulePortId decoder_data_inv_port = module_manager.find_module_port(
        decoder_module, std::string(DECODER_DATA_OUT_INV_PORT_NAME));
      ModuleNetId wlb_net = module_manager.create_module_net(mem_module);
      /* Source node of the input net is the input of memory module */
      module_manager.add_module_net_source(mem_module, wlb_net, decoder_module,
                                           0, decoder_data_inv_port,
                                           sram_instance);
      module_manager.add_module_net_sink(mem_module, wlb_net, sram_mem_module,
                                         sram_instance, sram_wlb_port, 0);
    }

    /* Wire inputs of parent module to outputs of child modules */
    for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
      std::string port_name;
      if (0 == iport) {
        port_name = generate_configurable_memory_data_out_name();
      } else {
        VTR_ASSERT(1 == iport);
        port_name = generate_configurable_memory_inverted_data_out_name();
      }

      add_module_output_nets_to_chain_mem_modules(
        module_manager, mem_module, port_name, circuit_lib,
        sram_output_ports[iport], sram_mem_module, i, sram_instance);
    }
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);

  /* Add the decoder as the last configurable children */
  module_manager.add_configurable_child(
    mem_module, decoder_module, 0, ModuleManager::e_config_child_type::UNIFIED);
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
static void build_memory_module(ModuleManager& module_manager,
                                DecoderLibrary& arch_decoder_lib,
                                const CircuitLibrary& circuit_lib,
                                const e_config_protocol_type& sram_orgz_type,
                                const std::string& module_name,
                                const CircuitModelId& sram_model,
                                const size_t& num_mems, const bool& verbose) {
  switch (sram_orgz_type) {
    case CONFIG_MEM_STANDALONE:
    case CONFIG_MEM_QL_MEMORY_BANK:
    case CONFIG_MEM_MEMORY_BANK:
      build_memory_flatten_module(module_manager, circuit_lib, module_name,
                                  sram_model, num_mems, verbose);
      break;
    case CONFIG_MEM_SCAN_CHAIN:
      build_memory_chain_module(module_manager, circuit_lib, module_name,
                                sram_model, num_mems, verbose);
      break;
    case CONFIG_MEM_FRAME_BASED:
      build_frame_memory_module(module_manager, arch_decoder_lib, circuit_lib,
                                module_name, sram_model, num_mems, verbose);
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid configurable memory organization!\n");
      exit(1);
  }
}

/*********************************************************************
 * Generate Verilog modules for the feedthrough memories that are used
 * by a circuit model
 *           mem_out           mem_outb
 *               |               |
 *               v               v
 *      +------------------------------------+
 *      |                                    |
 *      |                                    |
 *      |                                    |
 *      +------------------------------------+
 *          |                   |
 *          | mem_in            | mem_inb
 *          v                   v
 *      +------------------------------------+
 *      |   Multiplexer Configuration port   |
 *
 ********************************************************************/
static int build_feedthrough_memory_module(ModuleManager& module_manager,
                                           const std::string& module_name,
                                           const size_t& num_mems,
                                           const bool& verbose) {
  /* Create a module and add to the module manager */
  VTR_LOGV(verbose, "Building feedthrough memory module '%s'\n",
           module_name.c_str());
  ModuleId mem_module = module_manager.add_module(module_name);
  if (!module_manager.valid_module_id(mem_module)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Label module usage */
  module_manager.set_module_usage(mem_module, ModuleManager::MODULE_CONFIG);

  /* Add module ports */
  /* Input: memory inputs */
  BasicPort in_port(std::string(MEMORY_FEEDTHROUGH_DATA_IN_PORT_NAME),
                    num_mems);
  ModulePortId mem_in_port = module_manager.add_port(
    mem_module, in_port, ModuleManager::MODULE_INPUT_PORT);
  BasicPort inb_port(std::string(MEMORY_FEEDTHROUGH_DATA_IN_INV_PORT_NAME),
                     num_mems);
  ModulePortId mem_inb_port = module_manager.add_port(
    mem_module, inb_port, ModuleManager::MODULE_INPUT_PORT);

  /* Add each output port */
  BasicPort out_port(std::string(CONFIGURABLE_MEMORY_DATA_OUT_NAME), num_mems);
  ModulePortId mem_out_port = module_manager.add_port(
    mem_module, out_port, ModuleManager::MODULE_OUTPUT_PORT);
  BasicPort outb_port(std::string(CONFIGURABLE_MEMORY_INVERTED_DATA_OUT_NAME),
                      num_mems);
  ModulePortId mem_outb_port = module_manager.add_port(
    mem_module, outb_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Build feedthrough nets */
  for (size_t pin_id = 0; pin_id < in_port.pins().size(); ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(mem_module);
    if (!module_manager.valid_module_net_id(mem_module, net)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    module_manager.add_module_net_source(mem_module, net, mem_module, 0,
                                         mem_in_port, in_port.pins()[pin_id]);
    module_manager.add_module_net_sink(mem_module, net, mem_module, 0,
                                       mem_out_port, out_port.pins()[pin_id]);
  }
  for (size_t pin_id = 0; pin_id < inb_port.pins().size(); ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(mem_module);
    if (!module_manager.valid_module_net_id(mem_module, net)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    module_manager.add_module_net_source(mem_module, net, mem_module, 0,
                                         mem_inb_port, inb_port.pins()[pin_id]);
    module_manager.add_module_net_sink(mem_module, net, mem_module, 0,
                                       mem_outb_port, outb_port.pins()[pin_id]);
  }

  return CMD_EXEC_SUCCESS;
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
static void build_mux_memory_module(
  ModuleManager& module_manager, DecoderLibrary& arch_decoder_lib,
  const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type, const CircuitModelId& mux_model,
  const MuxGraph& mux_graph, const bool& verbose) {
  /* Find the actual number of configuration bits, based on the mux graph
   * Due to the use of local decoders inside mux, this may be
   */
  size_t num_config_bits =
    find_mux_num_config_bits(circuit_lib, mux_model, mux_graph, sram_orgz_type);
  /* Multiplexers built with different technology is in different organization
   */
  switch (circuit_lib.design_tech_type(mux_model)) {
    case CIRCUIT_MODEL_DESIGN_CMOS: {
      /* Generate module name */
      std::string module_name = generate_mux_subckt_name(
        circuit_lib, mux_model,
        find_mux_num_datapath_inputs(circuit_lib, mux_model,
                                     mux_graph.num_inputs()),
        std::string(MEMORY_MODULE_POSTFIX));

      /* Get the sram ports from the mux */
      std::vector<CircuitModelId> sram_models =
        find_circuit_sram_models(circuit_lib, mux_model);
      VTR_ASSERT(1 == sram_models.size());

      build_memory_module(module_manager, arch_decoder_lib, circuit_lib,
                          sram_orgz_type, module_name, sram_models[0],
                          num_config_bits, verbose);
      break;
    }
    case CIRCUIT_MODEL_DESIGN_RRAM:
      /* We do not need a memory submodule for RRAM MUX,
       * RRAM are embedded in the datapath
       * TODO: generate local encoders for RRAM-based multiplexers here!!!
       */
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid design technology of multiplexer '%s'\n",
                     circuit_lib.model_name(mux_model).c_str());
      exit(1);
  }
}

/*********************************************************************
 * Generate Verilog modules for the feedthrough memories that are used
 * by multiplexers
 *             SRAM ports as feedthrough (driven by physical memory blocks)
 *              |  |      |  |
 *              v  v ...  v  v
 *            +----------------+
 *            |  Memory Module |
 *            +----------------+
 *              |  |  ... |  |
 *              v  v      v  v SRAM ports of multiplexer
 *          +---------------------+
 *    in--->|  Multiplexer Module |---> out
 *          +---------------------+
 ********************************************************************/
static int build_mux_feedthrough_memory_module(
  ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type, const CircuitModelId& mux_model,
  const MuxGraph& mux_graph, const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  /* Find the actual number of configuration bits, based on the mux graph
   * Due to the use of local decoders inside mux, this may be
   */
  size_t num_config_bits =
    find_mux_num_config_bits(circuit_lib, mux_model, mux_graph, sram_orgz_type);
  /* Multiplexers built with different technology is in different organization
   */
  switch (circuit_lib.design_tech_type(mux_model)) {
    case CIRCUIT_MODEL_DESIGN_CMOS: {
      /* Generate module name */
      std::string module_name = generate_mux_subckt_name(
        circuit_lib, mux_model,
        find_mux_num_datapath_inputs(circuit_lib, mux_model,
                                     mux_graph.num_inputs()),
        std::string(MEMORY_FEEDTHROUGH_MODULE_POSTFIX));

      status = build_feedthrough_memory_module(module_manager, module_name,
                                               num_config_bits, verbose);
      break;
    }
    case CIRCUIT_MODEL_DESIGN_RRAM:
      /* We do not need a memory submodule for RRAM MUX,
       * RRAM are embedded in the datapath
       * TODO: generate local encoders for RRAM-based multiplexers here!!!
       */
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid design technology of multiplexer '%s'\n",
                     circuit_lib.model_name(mux_model).c_str());
      exit(1);
  }
  return status;
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
 * If we need feedthrough memory blocks, build the memory modules which contain
 *only feedthrough wires
 ********************************************************************/
int build_memory_modules(ModuleManager& module_manager,
                         DecoderLibrary& arch_decoder_lib,
                         const MuxLibrary& mux_lib,
                         const CircuitLibrary& circuit_lib,
                         const e_config_protocol_type& sram_orgz_type,
                         const bool& require_feedthrough_memory,
                         const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  vtr::ScopedStartFinishTimer timer("Build memory modules");

  /* Create the memory circuits for the multiplexer */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_model = mux_lib.mux_circuit_model(mux);
    /* Bypass the non-MUX circuit models (i.e., LUTs).
     * They should be handled in a different way
     * Memory circuits of LUT includes both regular and mode-select ports
     */
    if (CIRCUIT_MODEL_MUX != circuit_lib.model_type(mux_model)) {
      continue;
    }
    /* Create a Verilog module for the memories used by the multiplexer */
    build_mux_memory_module(module_manager, arch_decoder_lib, circuit_lib,
                            sram_orgz_type, mux_model, mux_graph, verbose);
    /* Create feedthrough memory module */
    if (require_feedthrough_memory) {
      status = build_mux_feedthrough_memory_module(module_manager, circuit_lib,
                                                   sram_orgz_type, mux_model,
                                                   mux_graph, verbose);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  /* Create the memory circuits for non-MUX circuit models.
   * In this case, the memory modules are designed to interface
   * the mode-select ports
   */
  for (const auto& model : circuit_lib.models()) {
    /* Bypass MUXes, they have already been considered */
    if (CIRCUIT_MODEL_MUX == circuit_lib.model_type(model)) {
      continue;
    }
    /* Bypass those modules without any SRAM ports */
    std::vector<CircuitPortId> sram_ports =
      circuit_lib.model_ports_by_type(model, CIRCUIT_MODEL_PORT_SRAM, true);
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
    std::vector<CircuitModelId> sram_models =
      find_circuit_sram_models(circuit_lib, model);
    /* Should have only 1 SRAM model */
    VTR_ASSERT(1 == sram_models.size());

    /* Create the module name for the memory block */
    std::string module_name = generate_memory_module_name(
      circuit_lib, model, sram_models[0], std::string(MEMORY_MODULE_POSTFIX));

    /* Create a Verilog module for the memories used by the circuit model */
    build_memory_module(module_manager, arch_decoder_lib, circuit_lib,
                        sram_orgz_type, module_name, sram_models[0], num_mems,
                        verbose);
    /* Create feedthrough memory module */
    if (require_feedthrough_memory) {
      module_name =
        generate_memory_module_name(circuit_lib, model, sram_models[0],
                                    std::string(MEMORY_MODULE_POSTFIX), true);
      status = build_feedthrough_memory_module(module_manager, module_name,
                                               num_mems, verbose);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }
  return status;
}

/*********************************************************************
 * Add module nets to connect an output port of a configuration-chain
 * memory module to an output port of its child module
 * Restriction: this function is really designed for memory modules
 * 1. It assumes that output port name of child module is the same as memory
 *module
 * 2. It assumes exact pin-to-pin mapping:
 *     j-th pin of output port of the i-th child module is wired to the j + i*W
 *-th pin of output port of the memory module, where W is the size of port
 * 3. It assumes fixed port name for output ports
 ********************************************************************/
static void add_module_output_nets_to_memory_group_module(
  ModuleManager& module_manager, const ModuleId& mem_module,
  const std::string& mem_module_output_name, const ModuleId& child_module,
  const size_t& output_pin_start_index, const size_t& child_instance) {
  /* Wire inputs of parent module to inputs of child modules */
  ModulePortId src_port_id =
    module_manager.find_module_port(child_module, mem_module_output_name);
  ModulePortId sink_port_id =
    module_manager.find_module_port(mem_module, mem_module_output_name);
  for (size_t pin_id = 0;
       pin_id <
       module_manager.module_port(child_module, src_port_id).pins().size();
       ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(mem_module);
    /* Source pin is shifted by the number of memories */
    size_t src_pin_id =
      module_manager.module_port(child_module, src_port_id).pins()[pin_id];
    /* Source node of the input net is the input of memory module */
    module_manager.add_module_net_source(
      mem_module, net, child_module, child_instance, src_port_id, src_pin_id);
    /* Sink node of the input net is the input of sram module */
    size_t sink_pin_id =
      output_pin_start_index +
      module_manager.module_port(mem_module, sink_port_id).pins()[pin_id];
    module_manager.add_module_net_sink(mem_module, net, mem_module, 0,
                                       sink_port_id, sink_pin_id);
  }
}

/*********************************************************************
 * Build a grouped memory module based on existing memory modules
 * - Create the module
 * - Add dedicated instance
 * - Add ports
 * - Add nets
 ********************************************************************/
int build_memory_group_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type, const std::string& module_name,
  const CircuitModelId& sram_model, const std::vector<ModuleId>& child_modules,
  const std::vector<std::string>& child_instance_names, const size_t& num_mems,
  const bool& verbose) {
  VTR_LOGV(verbose, "Building memory group module '%s'...\n",
           module_name.c_str());
  ModuleId mem_module = module_manager.add_module(module_name);
  if (!module_manager.valid_module_id(mem_module)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Label module usage */
  module_manager.set_module_usage(mem_module,
                                  ModuleManager::MODULE_CONFIG_GROUP);

  /* Add output ports */
  std::string out_port_name = generate_configurable_memory_data_out_name();
  BasicPort out_port(out_port_name, num_mems);
  module_manager.add_port(mem_module, out_port,
                          ModuleManager::MODULE_OUTPUT_PORT);

  std::string outb_port_name =
    generate_configurable_memory_inverted_data_out_name();
  BasicPort outb_port(outb_port_name, num_mems);
  module_manager.add_port(mem_module, outb_port,
                          ModuleManager::MODULE_OUTPUT_PORT);

  /* Identify the duplicated instance name: This mainly comes from the grid
   * modules, which contains multi-instanced blocks. Therefore, we just count
   * the duplicated instance names and name each of them with a unique index,
   * e.g., mem_lut -> mem_lut_0, mem_lut_1 etc. The only exception is for the
   * uinque instance name, we keep the original instance name */
  std::vector<std::string> unique_child_instance_names;
  unique_child_instance_names.reserve(child_instance_names.size());
  std::map<std::string, size_t> unique_child_instance_name_count;
  for (std::string curr_inst_name : child_instance_names) {
    auto result = unique_child_instance_name_count.find(curr_inst_name);
    if (result == unique_child_instance_name_count.end()) {
      unique_child_instance_name_count[curr_inst_name] = 1;
    } else {
      unique_child_instance_name_count[curr_inst_name]++;
    }
  }
  std::map<std::string, size_t> unique_child_instance_name_scoreboard;
  for (std::string curr_inst_name : child_instance_names) {
    if (1 == unique_child_instance_name_count[curr_inst_name]) {
      unique_child_instance_names.push_back(curr_inst_name);
      unique_child_instance_name_scoreboard[curr_inst_name] = 1;
      continue;
    }
    auto result = unique_child_instance_name_scoreboard.find(curr_inst_name);
    if (result == unique_child_instance_name_scoreboard.end()) {
      unique_child_instance_name_scoreboard[curr_inst_name] = 0;
      unique_child_instance_names.push_back(curr_inst_name);
    } else {
      unique_child_instance_name_scoreboard[curr_inst_name]++;
      unique_child_instance_names.push_back(generate_instance_name(
        curr_inst_name, unique_child_instance_name_scoreboard[curr_inst_name]));
    }
  }
  VTR_ASSERT(unique_child_instance_names.size() == child_instance_names.size());

  /* Add nets between child module outputs and memory modules */
  size_t mem_out_pin_start_index = 0;
  size_t mem_outb_pin_start_index = 0;
  for (size_t ichild = 0; ichild < child_modules.size(); ++ichild) {
    ModuleId child_module = child_modules[ichild];
    size_t child_instance =
      module_manager.num_instance(mem_module, child_module);
    module_manager.add_child_module(mem_module, child_module, false);
    module_manager.set_child_instance_name(mem_module, child_module,
                                           child_instance,
                                           unique_child_instance_names[ichild]);
    module_manager.add_configurable_child(
      mem_module, child_module, child_instance,
      ModuleManager::e_config_child_type::UNIFIED);
    /* Wire outputs of child module to outputs of parent module */
    ModulePortId child_out_port_id =
      module_manager.find_module_port(child_module, out_port_name);
    if (module_manager.valid_module_port_id(child_module, child_out_port_id)) {
      add_module_output_nets_to_memory_group_module(
        module_manager, mem_module, out_port_name, child_module,
        mem_out_pin_start_index, child_instance);
      /* Update pin counter */
      mem_out_pin_start_index +=
        module_manager.module_port(child_module, child_out_port_id).get_width();
    }
    ModulePortId child_outb_port_id =
      module_manager.find_module_port(child_module, outb_port_name);
    if (module_manager.valid_module_port_id(child_module, child_outb_port_id)) {
      add_module_output_nets_to_memory_group_module(
        module_manager, mem_module, outb_port_name, child_module,
        mem_outb_pin_start_index, child_instance);
      /* Update pin counter */
      mem_outb_pin_start_index +=
        module_manager.module_port(child_module, child_outb_port_id)
          .get_width();
    }
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);

  /* Count GPIO ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  add_module_gpio_ports_from_child_modules(module_manager, mem_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_shared_config_bits =
    find_module_num_shared_config_bits_from_child_modules(module_manager,
                                                          mem_module);
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, mem_module,
                                              module_num_shared_config_bits);
  }

  /* Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  ModuleManager::e_config_child_type config_child_type =
    ModuleManager::e_config_child_type::PHYSICAL;
  size_t module_num_config_bits =
    find_module_num_config_bits_from_child_modules(
      module_manager, mem_module, circuit_lib, sram_model, sram_orgz_type,
      config_child_type);
  if (0 < module_num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, mem_module, circuit_lib,
                                     sram_model, sram_orgz_type,
                                     module_num_config_bits);
  }

  /* Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb
   * module!
   */
  if (0 <
      module_manager.num_configurable_children(mem_module, config_child_type)) {
    add_module_nets_memory_config_bus(
      module_manager, decoder_lib, mem_module, sram_orgz_type,
      circuit_lib.design_tech_type(sram_model), config_child_type);
  }

  return CMD_EXEC_SUCCESS;
}

/*****************************************************************************
 * This function creates a physical memory module and add it the current module
 * The following tasks will be accomplished:
 * - Traverse all the logical configurable children in the module tree, starting
 *from the current module
 * - Build a list of the leaf logical configurable children and count the total
 *memory sizes, the memory size for each physical memory submodule. Note that
 *the physical memory submodule should be cached already in each leaf logical
 *configurable children
 * - Get the physical memory module required by each leaf logical configurable
 *child
 * - Create a dedicated module name for the physical memory (check if already
 *exists, if yes, skip creating a new module)
 * - Instanciate the module
 * - Built nets. Note that only the output ports of the physical memory block is
 *required, since they should drive the dedicated memory ports of logical
 *configurable children
 *****************************************************************************/
int add_physical_memory_module(ModuleManager& module_manager,
                               DecoderLibrary& decoder_lib,
                               const ModuleId& curr_module,
                               const std::string& suggested_module_name_prefix,
                               const CircuitLibrary& circuit_lib,
                               const e_config_protocol_type& sram_orgz_type,
                               const CircuitModelId& sram_model,
                               const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;

  std::vector<ModuleId> required_phy_mem_modules;
  std::vector<std::string> required_phy_mem_instance_names;
  status = rec_find_physical_memory_children(
    static_cast<const ModuleManager&>(module_manager), curr_module,
    required_phy_mem_modules, required_phy_mem_instance_names, verbose);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  size_t module_num_config_bits =
    find_module_num_config_bits_from_child_modules(
      module_manager, curr_module, circuit_lib, sram_model,
      CONFIG_MEM_FEEDTHROUGH, ModuleManager::e_config_child_type::LOGICAL);
  /* No need to build a memory when there are no configuration bits required */
  if (module_num_config_bits == 0) {
    return CMD_EXEC_SUCCESS;
  }
  std::string module_name_prefix = module_manager.module_name(curr_module);
  if (!suggested_module_name_prefix.empty()) {
    module_name_prefix = suggested_module_name_prefix;
  }
  std::string phy_mem_module_name = generate_physical_memory_module_name(
    module_name_prefix, module_num_config_bits);
  VTR_LOGV(verbose, "Adding memory group module '%s' as a child to '%s'...\n",
           phy_mem_module_name.c_str(),
           module_manager.module_name(curr_module).c_str());
  ModuleId phy_mem_module = module_manager.find_module(phy_mem_module_name);
  if (!module_manager.valid_module_id(phy_mem_module)) {
    status = build_memory_group_module(
      module_manager, decoder_lib, circuit_lib, sram_orgz_type,
      phy_mem_module_name, sram_model, required_phy_mem_modules,
      required_phy_mem_instance_names, module_num_config_bits, verbose);
  }
  if (status != CMD_EXEC_SUCCESS) {
    VTR_LOG_ERROR("Failed to create the physical memory module '%s'!\n",
                  phy_mem_module_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  phy_mem_module = module_manager.find_module(phy_mem_module_name);
  if (!module_manager.valid_module_id(phy_mem_module)) {
    VTR_LOG_ERROR("Failed to create the physical memory module '%s'!\n",
                  phy_mem_module_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  /* Add the physical memory module to the current module */
  size_t phy_mem_instance =
    module_manager.num_instance(curr_module, phy_mem_module);
  module_manager.add_child_module(curr_module, phy_mem_module, false);
  /* TODO: Give a more meaningful instance name? */
  module_manager.set_child_instance_name(curr_module, phy_mem_module,
                                         phy_mem_instance, phy_mem_module_name);

  /* Register in the physical configurable children list */
  module_manager.add_configurable_child(
    curr_module, phy_mem_module, phy_mem_instance,
    ModuleManager::e_config_child_type::PHYSICAL);

  /* Build nets between the data output of the physical memory module and the
   * outputs of the logical configurable children */
  std::map<e_circuit_model_port_type, size_t> curr_mem_pin_index;
  curr_mem_pin_index[CIRCUIT_MODEL_PORT_BL] = 0;
  curr_mem_pin_index[CIRCUIT_MODEL_PORT_BLB] = 0;
  std::map<e_circuit_model_port_type, std::string> mem2mem_port_map;
  mem2mem_port_map[CIRCUIT_MODEL_PORT_BL] =
    std::string(CONFIGURABLE_MEMORY_DATA_OUT_NAME);
  mem2mem_port_map[CIRCUIT_MODEL_PORT_BLB] =
    std::string(CONFIGURABLE_MEMORY_INVERTED_DATA_OUT_NAME);
  for (size_t ichild = 0;
       ichild < module_manager
                  .configurable_children(
                    curr_module, ModuleManager::e_config_child_type::LOGICAL)
                  .size();
       ++ichild) {
    ModuleId des_module = module_manager.configurable_children(
      curr_module, ModuleManager::e_config_child_type::LOGICAL)[ichild];
    size_t des_instance = module_manager.configurable_child_instances(
      curr_module, ModuleManager::e_config_child_type::LOGICAL)[ichild];

    for (e_circuit_model_port_type port_type :
         {CIRCUIT_MODEL_PORT_BL, CIRCUIT_MODEL_PORT_BLB}) {
      std::string src_port_name = mem2mem_port_map[port_type];
      std::string des_port_name =
        generate_sram_port_name(CONFIG_MEM_FEEDTHROUGH, port_type);
      /* Try to find these ports in the module manager */
      ModulePortId src_port_id =
        module_manager.find_module_port(phy_mem_module, src_port_name);
      if (!module_manager.valid_module_port_id(phy_mem_module, src_port_id)) {
        return CMD_EXEC_FATAL_ERROR;
      }
      BasicPort src_port =
        module_manager.module_port(phy_mem_module, src_port_id);

      ModulePortId des_port_id =
        module_manager.find_module_port(des_module, des_port_name);
      if (!module_manager.valid_module_port_id(des_module, des_port_id)) {
        return CMD_EXEC_FATAL_ERROR;
      }
      BasicPort des_port = module_manager.module_port(des_module, des_port_id);
      /* Build nets */
      for (size_t ipin = 0; ipin < des_port.pins().size(); ++ipin) {
        VTR_LOGV(
          verbose, "Building net '%s[%lu].%s[%lu]' -> '%s[%lu].%s[%lu]\n",
          module_manager.module_name(phy_mem_module).c_str(), phy_mem_instance,
          src_port.get_name().c_str(), curr_mem_pin_index[port_type],
          module_manager.module_name(des_module).c_str(), des_instance,
          des_port.get_name().c_str(), des_port.pins()[ipin]);
        /* Create a net and add source and sink to it */
        ModuleNetId net = create_module_source_pin_net(
          module_manager, curr_module, phy_mem_module, phy_mem_instance,
          src_port_id, src_port.pins()[curr_mem_pin_index[port_type]]);
        if (!module_manager.valid_module_net_id(curr_module, net)) {
          return CMD_EXEC_FATAL_ERROR;
        }
        /* Add net sink */
        module_manager.add_module_net_sink(curr_module, net, des_module,
                                           des_instance, des_port_id,
                                           des_port.pins()[ipin]);
        curr_mem_pin_index[port_type]++;
      }
    }
  }
  VTR_ASSERT(curr_mem_pin_index[CIRCUIT_MODEL_PORT_BL] ==
             module_num_config_bits);
  VTR_ASSERT(curr_mem_pin_index[CIRCUIT_MODEL_PORT_BLB] ==
             module_num_config_bits);

  return status;
}

} /* end namespace openfpga */
