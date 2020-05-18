/***********************************************
 * This file includes functions to generate
 * Verilog submodules for multiplexers.
 * including both fundamental submodules
 * such as a branch in a multiplexer 
 * and the full multiplexer
 **********************************************/
#include <string>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "mux_graph.h"
#include "module_manager.h"
#include "mux_utils.h"
#include "circuit_library_utils.h"
#include "decoder_library_utils.h"
#include "module_manager_utils.h"
#include "build_module_graph_utils.h"
#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "build_mux_modules.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Generate structural Verilog codes (consist of transmission-gates or
 * pass-transistor) modeling an branch circuit 
 * for a multiplexer with the given size 
 *
 *                +----------+
 *   input[0] --->| tgate[0] |-+
 *                +----------+ |
 *                             |
 *                +----------+ |
 *   input[1] --->| tgate[1] |-+--->output[0]
 *                +----------+ |
 *                             |
 *     ...            ...      |
 *                             |
 *                +----------+ |
 *   input[i] --->| tgate[i] |-+
 *                +----------+
 *********************************************************************/
static 
void build_cmos_mux_branch_body(ModuleManager& module_manager,
                                const CircuitLibrary& circuit_lib, 
                                const CircuitModelId& tgate_model, 
                                const ModuleId& mux_module, 
                                const ModulePortId& module_input_port,
                                const ModulePortId& module_output_port,
                                const ModulePortId& module_mem_port,
                                const ModulePortId& module_mem_inv_port,
                                const MuxGraph& mux_graph) {
  /* Get the module id of tgate in Module manager */
  ModuleId tgate_module_id = module_manager.find_module(circuit_lib.model_name(tgate_model));
  VTR_ASSERT(ModuleId::INVALID() != tgate_module_id);

  /* Get model ports of tgate */
  std::vector<CircuitPortId> tgate_input_ports = circuit_lib.model_ports_by_type(tgate_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> tgate_output_ports = circuit_lib.model_ports_by_type(tgate_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(3 == tgate_input_ports.size());
  VTR_ASSERT(1 == tgate_output_ports.size());

  /* Find the module ports of tgate module */
  /* Input port is the data path input of the tgate, whose size must be 1 ! */
  ModulePortId tgate_module_input = module_manager.find_module_port(tgate_module_id, circuit_lib.port_prefix(tgate_input_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(tgate_module_id, tgate_module_input));
  BasicPort tgate_module_input_port = module_manager.module_port(tgate_module_id, tgate_module_input);
  VTR_ASSERT(1 == tgate_module_input_port.get_width());

  /* Mem port is the memory of the tgate, whose size must be 1 ! */
  ModulePortId tgate_module_mem = module_manager.find_module_port(tgate_module_id, circuit_lib.port_prefix(tgate_input_ports[1]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(tgate_module_id, tgate_module_mem));
  BasicPort tgate_module_mem_port = module_manager.module_port(tgate_module_id, tgate_module_mem);
  VTR_ASSERT(1 == tgate_module_mem_port.get_width());

  /* Mem inv port is the inverted memory of the tgate, whose size must be 1 ! */
  ModulePortId tgate_module_mem_inv = module_manager.find_module_port(tgate_module_id, circuit_lib.port_prefix(tgate_input_ports[2]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(tgate_module_id, tgate_module_mem_inv));
  BasicPort tgate_module_mem_inv_port = module_manager.module_port(tgate_module_id, tgate_module_mem_inv);
  VTR_ASSERT(1 == tgate_module_mem_inv_port.get_width());

  /* Output port is the data path output of the tgate, whose size must be 1 ! */
  ModulePortId tgate_module_output = module_manager.find_module_port(tgate_module_id, circuit_lib.port_prefix(tgate_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(tgate_module_id, tgate_module_output));
  BasicPort tgate_module_output_port = module_manager.module_port(tgate_module_id, tgate_module_output);
  VTR_ASSERT(1 == tgate_module_output_port.get_width());

  /* Ensure that input port size does match mux inputs */
  BasicPort input_port = module_manager.module_port(mux_module, module_input_port); 
  VTR_ASSERT(input_port.get_width() == mux_graph.num_inputs());
  
  /* Add module nets for each mux inputs */
  std::vector<ModuleNetId> mux_input_nets;
  for (const size_t& pin : input_port.pins()) {
    ModuleNetId input_net = module_manager.create_module_net(mux_module);
    mux_input_nets.push_back(input_net);
    /* Configure the source for each net */
    module_manager.add_module_net_source(mux_module, input_net, mux_module, 0, module_input_port, pin);
  }

  /* Ensure that output port size does match mux outputs */
  BasicPort output_port = module_manager.module_port(mux_module, module_output_port); 
  VTR_ASSERT(output_port.get_width() == mux_graph.num_outputs());

  /* Add module nets for each mux outputs */
  std::vector<ModuleNetId> mux_output_nets;
  for (const size_t& pin : output_port.pins()) {
    ModuleNetId output_net = module_manager.create_module_net(mux_module);
    mux_output_nets.push_back(output_net);
    /* Configure the sink for each net */
    module_manager.add_module_net_sink(mux_module, output_net, mux_module, 0, module_output_port, pin);
  }

  /* Ensure that mem port size does match mux outputs */
  BasicPort mem_port = module_manager.module_port(mux_module, module_mem_port); 
  VTR_ASSERT(mem_port.get_width() == mux_graph.num_memory_bits());

  /* Add module nets for each mem inputs */
  std::vector<ModuleNetId> mux_mem_nets;
  for (const size_t& pin : mem_port.pins()) {
    ModuleNetId mem_net = module_manager.create_module_net(mux_module);
    mux_mem_nets.push_back(mem_net);
    /* Configure the source for each net */
    module_manager.add_module_net_source(mux_module, mem_net, mux_module, 0, module_mem_port, pin);
  }

  /* Ensure that mem_inv port size does match mux outputs */
  BasicPort mem_inv_port = module_manager.module_port(mux_module, module_mem_inv_port); 
  VTR_ASSERT(mem_inv_port.get_width() == mux_graph.num_memory_bits());

  /* Add module nets for each mem inverted inputs */
  std::vector<ModuleNetId> mux_mem_inv_nets;
  for (const size_t& pin : mem_inv_port.pins()) {
    ModuleNetId mem_net = module_manager.create_module_net(mux_module);
    mux_mem_inv_nets.push_back(mem_net);
    /* Configure the source for each net */
    module_manager.add_module_net_source(mux_module, mem_net, mux_module, 0, module_mem_inv_port, pin);
  }

  /* Build a module following the connections in mux_graph */
  /* Iterate over the inputs */
  for (const auto& mux_input : mux_graph.inputs()) {
    /* Iterate over the outputs */
    for (const auto& mux_output : mux_graph.outputs()) {
      /* Add the a tgate to bridge the mux input and output */
      size_t tgate_instance = module_manager.num_instance(mux_module, tgate_module_id);
      module_manager.add_child_module(mux_module, tgate_module_id);

      /* Add module nets to connect the mux input and tgate input */
      module_manager.add_module_net_sink(mux_module, mux_input_nets[size_t(mux_graph.input_id(mux_input))], tgate_module_id, tgate_instance, tgate_module_input, tgate_module_input_port.get_lsb());

      /* if there is a connection between the input and output, a tgate will be outputted */
      std::vector<MuxEdgeId> edges = mux_graph.find_edges(mux_input, mux_output);
      /* There should be only one edge or no edge*/
      VTR_ASSERT((1 == edges.size()) || (0 == edges.size()));
      /* No need to output tgates if there are no edges between two nodes */
      if (0 == edges.size()) {
        continue;
      }

      /* Add module nets to connect the mux output and tgate output */
      module_manager.add_module_net_source(mux_module, mux_output_nets[size_t(mux_graph.output_id(mux_output))], tgate_module_id, tgate_instance, tgate_module_output, tgate_module_output_port.get_lsb());

      MuxMemId mux_mem = mux_graph.find_edge_mem(edges[0]);
      /* Add module nets to connect the mem input and tgate mem input */
      if (false == mux_graph.is_edge_use_inv_mem(edges[0])) {
        /* wire mem to mem of module, and wire mem_inv to mem_inv of module */
        module_manager.add_module_net_sink(mux_module, mux_mem_nets[size_t(mux_mem)], tgate_module_id, tgate_instance, tgate_module_mem, tgate_module_mem_port.get_lsb());
        module_manager.add_module_net_sink(mux_module, mux_mem_inv_nets[size_t(mux_mem)], tgate_module_id, tgate_instance, tgate_module_mem_inv, tgate_module_mem_inv_port.get_lsb());
      } else {
        /* wire mem_inv to mem of module, wire mem to mem_inv of module */
        module_manager.add_module_net_sink(mux_module, mux_mem_inv_nets[size_t(mux_mem)], tgate_module_id, tgate_instance, tgate_module_mem, tgate_module_mem_port.get_lsb());
        module_manager.add_module_net_sink(mux_module, mux_mem_nets[size_t(mux_mem)], tgate_module_id, tgate_instance, tgate_module_mem_inv, tgate_module_mem_inv_port.get_lsb());
      }  
    }
  }
}

/*********************************************************************
 * Generate  Verilog codes modeling an branch circuit 
 * for a CMOS multiplexer with the given size 
 * Support structural and behavioral Verilog codes
 *********************************************************************/
static 
void build_cmos_mux_branch_module(ModuleManager& module_manager,
                                  const CircuitLibrary& circuit_lib, 
                                  const CircuitModelId& mux_model, 
                                  const std::string& module_name, 
                                  const MuxGraph& mux_graph) {
  /* Get the tgate model */
  CircuitModelId tgate_model = circuit_lib.pass_gate_logic_model(mux_model);

  /* Skip output if the tgate model is a MUX2, it is handled by essential-gate generator */
  if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(tgate_model)) {
    VTR_ASSERT(CIRCUIT_MODEL_GATE_MUX2 == circuit_lib.gate_type(tgate_model));
    return;
  }

  std::vector<CircuitPortId> tgate_global_ports = circuit_lib.model_global_ports_by_type(tgate_model, CIRCUIT_MODEL_PORT_INPUT, true, true);

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of inputs */ 
  size_t num_inputs = mux_graph.num_inputs();
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 output */
  VTR_ASSERT(1 == num_outputs);
  /* MUX graph must have only 1 level*/
  VTR_ASSERT(1 == mux_graph.num_levels());

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId mux_module = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(mux_module));
  /* Add module ports */
  /* Add each input port */
  BasicPort input_port("in", num_inputs);
  ModulePortId module_input_port = module_manager.add_port(mux_module, input_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each output port */
  BasicPort output_port("out", num_outputs);
  ModulePortId module_output_port = module_manager.add_port(mux_module, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  /* Add each memory port */
  BasicPort mem_port("mem", num_mems);
  ModulePortId module_mem_port = module_manager.add_port(mux_module, mem_port, ModuleManager::MODULE_INPUT_PORT);
  BasicPort mem_inv_port("mem_inv", num_mems);
  ModulePortId module_mem_inv_port = module_manager.add_port(mux_module, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);

  /* By default we give a structural description, 
   * Writers can freely write the module in their styles 
   * For instance, Verilog writer can ignore the internal structure and write in behavioral codes 
   */
  build_cmos_mux_branch_body(module_manager, circuit_lib, tgate_model, mux_module, module_input_port, module_output_port, module_mem_port, module_mem_inv_port, mux_graph);

  /* Add global ports to the mux module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mux_module);
}

/*********************************************************************
 * Generate  Verilog codes modeling an branch circuit 
 * for a RRAM-based multiplexer with the given size 
 * Support structural and behavioral Verilog codes
 *********************************************************************/
static 
void build_rram_mux_branch_module(ModuleManager& module_manager,
                                  const CircuitLibrary& circuit_lib, 
                                  const CircuitModelId& mux_model, 
                                  const std::string& module_name, 
                                  const MuxGraph& mux_graph) {
  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  /* Get the BL and WL ports from the mux */
  std::vector<CircuitPortId> mux_blb_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> mux_wl_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_WL, true);

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of inputs */ 
  size_t num_inputs = mux_graph.num_inputs();
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 output */
  VTR_ASSERT(1 == num_outputs);
  /* MUX graph must have only 1 level*/
  VTR_ASSERT(1 == mux_graph.num_levels());
  /* MUX graph must have only 1 input and 1 BLB and 1 WL port */
  VTR_ASSERT(1 == mux_input_ports.size());
  VTR_ASSERT(1 == mux_output_ports.size());
  VTR_ASSERT(1 == mux_blb_ports.size());
  VTR_ASSERT(1 == mux_wl_ports.size());

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId mux_module = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != mux_module);

  /* Add module ports */
  /* Add each global programming enable/disable ports */
  std::vector<CircuitPortId> prog_enable_ports = circuit_lib.model_global_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_INPUT, true, true);
  for (const auto& port : prog_enable_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_prefix(port), circuit_lib.port_size(port));
    module_manager.add_port(mux_module, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }

  /* Add each input port */
  BasicPort input_port(circuit_lib.port_prefix(mux_input_ports[0]), num_inputs);
  module_manager.add_port(mux_module, input_port, ModuleManager::MODULE_INPUT_PORT);

  /* Add each output port */
  BasicPort output_port(circuit_lib.port_prefix(mux_output_ports[0]), num_outputs);
  module_manager.add_port(mux_module, output_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Add RRAM programming ports, 
   * RRAM MUXes require one more pair of BLB and WL 
   * to configure the memories. See schematic for details
   */
  BasicPort blb_port(circuit_lib.port_prefix(mux_blb_ports[0]), num_mems + 1);
  module_manager.add_port(mux_module, blb_port, ModuleManager::MODULE_INPUT_PORT);

  BasicPort wl_port(circuit_lib.port_prefix(mux_wl_ports[0]), num_mems + 1);
  module_manager.add_port(mux_module, wl_port, ModuleManager::MODULE_INPUT_PORT);

  /* Note: we do not generate the internal structure of the ReRAM-based MUX
   * circuit as a module graph! 
   * This is mainly due to that the internal structure could be different
   * in Verilog or SPICE netlists
   * Leave the writers to customize this 
   */
}

/***********************************************
 * Generate Verilog codes modeling an branch circuit 
 * for a multiplexer with the given size 
 **********************************************/
static 
void build_mux_branch_module(ModuleManager& module_manager,
                             const CircuitLibrary& circuit_lib, 
                             const CircuitModelId& mux_model, 
                             const size_t& mux_size, 
                             const MuxGraph& mux_graph) {
  std::string module_name = generate_mux_branch_subckt_name(circuit_lib, mux_model, mux_size, mux_graph.num_inputs(), MUX_BASIS_MODULE_POSTFIX);

  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(mux_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    build_cmos_mux_branch_module(module_manager, circuit_lib, mux_model, module_name, mux_graph);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    build_rram_mux_branch_module(module_manager, circuit_lib, mux_model, module_name, mux_graph);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid design technology of multiplexer '%s'\n",
                  circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }
}

/********************************************************************
 * Generate the standard-cell-based internal logic (multiplexing structure) 
 * for a multiplexer or LUT in Verilog codes 
 * This function will : 
 * 1. build a multiplexing structure by instanciating standard cells MUX2
 * 2. add intermediate buffers between multiplexing stages if specified.
 *******************************************************************/
static 
void build_cmos_mux_module_mux2_multiplexing_structure(ModuleManager& module_manager,
                                                       const CircuitLibrary& circuit_lib, 
                                                       const ModuleId& mux_module, 
                                                       const CircuitModelId& mux_model, 
                                                       const CircuitModelId& std_cell_model, 
                                                       const vtr::vector<MuxInputId, ModuleNetId>& mux_module_input_nets, 
                                                       const vtr::vector<MuxOutputId, ModuleNetId>& mux_module_output_nets, 
                                                       const vtr::vector<MuxMemId, ModuleNetId>& mux_module_mem_nets, 
                                                       const MuxGraph& mux_graph) {
  /* Get the regular (non-mode-select) sram ports from the mux */
  std::vector<CircuitPortId> mux_regular_sram_ports = find_circuit_regular_sram_ports(circuit_lib, mux_model);
  VTR_ASSERT(1 == mux_regular_sram_ports.size());

  /* Find the input ports and output ports of the standard cell */
  std::vector<CircuitPortId> std_cell_input_ports = circuit_lib.model_ports_by_type(std_cell_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> std_cell_output_ports = circuit_lib.model_ports_by_type(std_cell_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  /* Quick check the requirements on port map */
  VTR_ASSERT(3 == std_cell_input_ports.size());
  VTR_ASSERT(1 == std_cell_output_ports.size());

  /* Find module information of the standard cell MUX2 */
  std::string std_cell_module_name = circuit_lib.model_name(std_cell_model);
  /* Get the moduleId for the submodule */
  ModuleId std_cell_module_id = module_manager.find_module(std_cell_module_name);
  /* We must have one */
  VTR_ASSERT(ModuleId::INVALID() != std_cell_module_id);

  /* Find the module ports of the standard cell MUX2 module */
  std::vector<ModulePortId> std_cell_module_inputs;
  std::vector<BasicPort> std_cell_module_input_ports;
  /* Input 0 port is the first data path input of the tgate, whose size must be 1 ! */
  for (size_t port_id = 0; port_id < 2; ++port_id) {
    std_cell_module_inputs.push_back(module_manager.find_module_port(std_cell_module_id, circuit_lib.port_prefix(std_cell_input_ports[port_id])));
    VTR_ASSERT(true == module_manager.valid_module_port_id(std_cell_module_id, std_cell_module_inputs[port_id]));
    std_cell_module_input_ports.push_back(module_manager.module_port(std_cell_module_id, std_cell_module_inputs[port_id]));
    VTR_ASSERT(1 == std_cell_module_input_ports[port_id].get_width());
  }

  /* Mem port is the memory of the standard cell MUX2, whose size must be 1 ! */
  ModulePortId std_cell_module_mem = module_manager.find_module_port(std_cell_module_id, circuit_lib.port_prefix(std_cell_input_ports[2]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(std_cell_module_id, std_cell_module_mem));
  BasicPort std_cell_module_mem_port = module_manager.module_port(std_cell_module_id, std_cell_module_mem);
  VTR_ASSERT(1 == std_cell_module_mem_port.get_width());

  /* Output port is the data path output of the standard cell MUX2, whose size must be 1 ! */
  ModulePortId std_cell_module_output = module_manager.find_module_port(std_cell_module_id, circuit_lib.port_prefix(std_cell_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(std_cell_module_id, std_cell_module_output));
  BasicPort std_cell_module_output_port = module_manager.module_port(std_cell_module_id, std_cell_module_output);
  VTR_ASSERT(1 == std_cell_module_output_port.get_width());

  /* Cache Net ids for each level of the multiplexer */
  std::vector<std::vector<ModuleNetId>> module_nets_by_level;
  module_nets_by_level.resize(mux_graph.num_node_levels());
  for (size_t level = 0; level < mux_graph.num_node_levels(); ++level) {
    /* Print the internal wires located at this level */
    module_nets_by_level[level].resize(mux_graph.num_nodes_at_level(level));
  }

  /* Build the location map of intermediate buffers */
  std::vector<bool> inter_buffer_location_map = build_mux_intermediate_buffer_location_map(circuit_lib, mux_model, mux_graph.num_node_levels());
 
  /* Add all the branch modules and intermediate buffers */
  for (const auto& node : mux_graph.non_input_nodes()) {
    /* Get the size of branch circuit 
     * Instanciate an branch circuit by the size (fan-in) of the node 
     */
    size_t branch_size = mux_graph.node_in_edges(node).size();
    /* To match the standard cell MUX2: We should have only 2 input_nodes */
    VTR_ASSERT(2 == branch_size);

    /* Find the instance id */
    size_t std_cell_instance_id = module_manager.num_instance(mux_module, std_cell_module_id);
    /* Add the module to mux_module */
    module_manager.add_child_module(mux_module, std_cell_module_id);

    /* Get the node level and index in the current level */
    size_t output_node_level = mux_graph.node_level(node);
    size_t output_node_index_at_level = mux_graph.node_index_at_level(node);
    /* Set a name for the instance */
    std::string std_cell_instance_name = generate_mux_branch_instance_name(output_node_level, output_node_index_at_level, false);
    module_manager.set_child_instance_name(mux_module, std_cell_module_id, std_cell_instance_id, std_cell_instance_name);

    /* Add module nets to wire to next stage modules */
    ModuleNetId branch_net; 
    if (true == mux_graph.is_node_output(node)) {
      /* This is an output node, we should use existing output nets */
      MuxOutputId output_id = mux_graph.output_id(node);
      branch_net = mux_module_output_nets[output_id];
    } else {
      VTR_ASSERT(false == mux_graph.is_node_output(node));
      branch_net = module_manager.create_module_net(mux_module);
    }
    module_manager.add_module_net_source(mux_module, branch_net, std_cell_module_id, std_cell_instance_id, std_cell_module_output, std_cell_module_output_port.get_lsb());

    /* Record the module net id in the cache */
    module_nets_by_level[output_node_level][output_node_index_at_level] = branch_net;

    /* Wire the branch module memory ports to the nets of MUX memory ports */
    /* Get the mems in the branch circuits */
    std::vector<MuxMemId> mems; 
    for (const auto& edge : mux_graph.node_in_edges(node)) {
      /* Get the mem control the edge */
      MuxMemId mem = mux_graph.find_edge_mem(edge);
      /* Add the mem if it is not in the list */
      if (mems.end() == std::find(mems.begin(), mems.end(), mem)) {
        mems.push_back(mem);
      }
    }
    /* Connect mem to mem net one by one 
     * Note that standard cell MUX2 only needs mem but NOT mem_inv
     */
    for (const MuxMemId& mem : mems) {
      module_manager.add_module_net_sink(mux_module, mux_module_mem_nets[mem], std_cell_module_id, std_cell_instance_id, std_cell_module_mem, std_cell_module_mem_port.get_lsb());
    }

    /* Wire the branch module inputs to the nets in previous stage */
    /* Get the nodes which drive the root_node */
    std::vector<MuxNodeId> input_nodes; 
    for (const auto& edge : mux_graph.node_in_edges(node)) {
      /* Get the nodes drive the edge */
      for (const auto& src_node : mux_graph.edge_src_nodes(edge)) {
        input_nodes.push_back(src_node);
      }
    }
    /* Number of inputs should match the branch_input_size!!! */
    VTR_ASSERT(input_nodes.size() == branch_size);
    /* To match the standard cell MUX2: We should have only 2 input_nodes */
    VTR_ASSERT(2 == input_nodes.size());
    /* build the link between input_node[0] and std_cell_input_port[0] 
     * build the link between input_node[1] and std_cell_input_port[1] 
     */
    for (size_t node_id = 0; node_id < input_nodes.size(); ++node_id) {
      /* Find the port info of each input node */
      size_t input_node_level = mux_graph.node_level(input_nodes[node_id]);
      size_t input_node_index_at_level = mux_graph.node_index_at_level(input_nodes[node_id]);
      /* For inputs of mux, the net id is reserved */
      if (true == mux_graph.is_node_input(input_nodes[node_id])) {
        /* Get node input id */
        MuxInputId input_id = mux_graph.input_id(input_nodes[node_id]);
        module_manager.add_module_net_sink(mux_module, mux_module_input_nets[input_id], std_cell_module_id, std_cell_instance_id, std_cell_module_inputs[node_id], std_cell_module_input_ports[node_id].get_lsb());
      } else {
        VTR_ASSERT (false == mux_graph.is_node_input(input_nodes[node_id]));
        /* Find the input port of standard cell */
        module_manager.add_module_net_sink(mux_module, module_nets_by_level[input_node_level][input_node_index_at_level], std_cell_module_id, std_cell_instance_id, std_cell_module_inputs[node_id], std_cell_module_input_ports[node_id].get_lsb());
      }
    }

    /* Identify if an intermediate buffer is needed */
    if (false == inter_buffer_location_map[output_node_level]) { 
      continue;
    }
    /* Add an intermediate buffer to mux_module if needed */
    if (true == mux_graph.is_node_output(node)) {
      /* Output node does not need buffer addition here, it is handled outside this function */
      continue; 
    }
    /* Now we need to add intermediate buffers by instanciating the modules */
    CircuitModelId buffer_model = circuit_lib.lut_intermediate_buffer_model(mux_model);
    /* We must have a valid model id */
    VTR_ASSERT(CircuitModelId::INVALID() != buffer_model);

    /* Create a module net which sources from buffer output */
    ModuleNetId buffer_net = add_inverter_buffer_child_module_and_nets(module_manager, mux_module, circuit_lib, buffer_model, branch_net); 

    /* Record the module net id in the cache */
    module_nets_by_level[output_node_level][output_node_index_at_level] = buffer_net;
  }
}

/********************************************************************
 * Generate the pass-transistor/transmission-gate -based internal logic 
 * (multiplexing structure) for a multiplexer or LUT in Verilog codes 
 * This function will : 
 * 1. build a multiplexing structure by instanciating the branch circuits
 *    generated before
 * 2. add intermediate buffers between multiplexing stages if specified.
 *******************************************************************/
static 
void build_cmos_mux_module_tgate_multiplexing_structure(ModuleManager& module_manager,
                                                        const CircuitLibrary& circuit_lib, 
                                                        const ModuleId& mux_module, 
                                                        const CircuitModelId& circuit_model, 
                                                        const vtr::vector<MuxInputId, ModuleNetId>& mux_module_input_nets, 
                                                        const vtr::vector<MuxOutputId, ModuleNetId>& mux_module_output_nets, 
                                                        const vtr::vector<MuxMemId, ModuleNetId>& mux_module_mem_nets, 
                                                        const vtr::vector<MuxMemId, ModuleNetId>& mux_module_mem_inv_nets, 
                                                        const MuxGraph& mux_graph) {
  /* Find the actual mux size */
  size_t mux_size = find_mux_num_datapath_inputs(circuit_lib, circuit_model, mux_graph.num_inputs());

  /* Get the regular (non-mode-select) sram ports from the mux */
  std::vector<CircuitPortId> mux_regular_sram_ports = find_circuit_regular_sram_ports(circuit_lib, circuit_model);
  VTR_ASSERT(1 == mux_regular_sram_ports.size());

  /* Cache Net ids for each level of the multiplexer */
  std::vector<std::vector<ModuleNetId>> module_nets_by_level;
  module_nets_by_level.resize(mux_graph.num_node_levels());
  for (size_t level = 0; level < mux_graph.num_node_levels(); ++level) {
    /* Print the internal wires located at this level */
    module_nets_by_level[level].resize(mux_graph.num_nodes_at_level(level));
  }

  /* Build the location map of intermediate buffers */
  std::vector<bool> inter_buffer_location_map = build_mux_intermediate_buffer_location_map(circuit_lib, circuit_model, mux_graph.num_node_levels());
 
  /* Add all the branch modules and intermediate buffers */
  for (const auto& node : mux_graph.non_input_nodes()) {
    /* Get the size of branch circuit 
     * Instanciate an branch circuit by the size (fan-in) of the node 
     */
    size_t branch_size = mux_graph.node_in_edges(node).size();

    /* Instanciate the branch module which is a tgate-based module  
     */
    std::string branch_module_name= generate_mux_branch_subckt_name(circuit_lib, circuit_model, mux_size, branch_size, MUX_BASIS_MODULE_POSTFIX);
    /* Get the moduleId for the submodule */
    ModuleId branch_module_id = module_manager.find_module(branch_module_name);
    /* We must have one */
    VTR_ASSERT(ModuleId::INVALID() != branch_module_id);

    /* Find the instance id */
    size_t branch_instance_id = module_manager.num_instance(mux_module, branch_module_id);
    /* Add the module to mux_module */
    module_manager.add_child_module(mux_module, branch_module_id);

    /* Get the node level and index in the current level */
    size_t output_node_level = mux_graph.node_level(node);
    size_t output_node_index_at_level = mux_graph.node_index_at_level(node);
    /* Set a name for the instance */
    std::string branch_instance_name = generate_mux_branch_instance_name(output_node_level, output_node_index_at_level, false);
    module_manager.set_child_instance_name(mux_module, branch_module_id, branch_instance_id, branch_instance_name);

    /* Get the output port id of branch module */
    ModulePortId branch_module_output_port_id = module_manager.find_module_port(branch_module_id, std::string("out")); 
    BasicPort branch_module_output_port = module_manager.module_port(branch_module_id, branch_module_output_port_id);

    /* Add module nets to wire to next stage modules */
    ModuleNetId branch_net; 
    if (true == mux_graph.is_node_output(node)) {
      /* This is an output node, we should use existing output nets */
      MuxOutputId output_id = mux_graph.output_id(node);
      branch_net = mux_module_output_nets[output_id];
    } else {
      VTR_ASSERT(false == mux_graph.is_node_output(node));
      branch_net = module_manager.create_module_net(mux_module);
    }
    module_manager.add_module_net_source(mux_module, branch_net, branch_module_id, branch_instance_id, branch_module_output_port_id, branch_module_output_port.get_lsb());

    /* Record the module net id in the cache */
    module_nets_by_level[output_node_level][output_node_index_at_level] = branch_net;

    /* Wire the branch module memory ports to the nets of MUX memory ports */
    /* Get the mems in the branch circuits */
    std::vector<MuxMemId> mems; 
    for (const auto& edge : mux_graph.node_in_edges(node)) {
      /* Get the mem control the edge */
      MuxMemId mem = mux_graph.find_edge_mem(edge);
      /* Add the mem if it is not in the list */
      if (mems.end() == std::find(mems.begin(), mems.end(), mem)) {
        mems.push_back(mem);
      }
    }

    /* Get mem/mem_inv ports of branch module */
    ModulePortId branch_module_mem_port_id = module_manager.find_module_port(branch_module_id, std::string("mem")); 
    BasicPort branch_module_mem_port = module_manager.module_port(branch_module_id, branch_module_mem_port_id);
    ModulePortId branch_module_mem_inv_port_id = module_manager.find_module_port(branch_module_id, std::string("mem_inv")); 
    BasicPort branch_module_mem_inv_port = module_manager.module_port(branch_module_id, branch_module_mem_inv_port_id);

    /* Note that we do NOT care inverted edge-to-mem connection. 
     * It is handled in branch module generation!!!
     */
    /* Connect mem/mem_inv to mem/mem_inv net one by one */
    for (size_t mem_id = 0; mem_id < mems.size(); ++mem_id) {
      module_manager.add_module_net_sink(mux_module, mux_module_mem_nets[mems[mem_id]], branch_module_id, branch_instance_id, branch_module_mem_port_id, branch_module_mem_port.pins()[mem_id]);
      module_manager.add_module_net_sink(mux_module, mux_module_mem_inv_nets[mems[mem_id]], branch_module_id, branch_instance_id, branch_module_mem_inv_port_id, branch_module_mem_inv_port.pins()[mem_id]);
    }

    /* Wire the branch module inputs to the nets in previous stage */
    /* Get the input port id of branch module */
    ModulePortId branch_module_input_port_id = module_manager.find_module_port(branch_module_id, std::string("in")); 
    BasicPort branch_module_input_port = module_manager.module_port(branch_module_id, branch_module_input_port_id);

    /* Get the nodes which drive the root_node */
    std::vector<MuxNodeId> input_nodes; 
    for (const auto& edge : mux_graph.node_in_edges(node)) {
      /* Get the nodes drive the edge */
      for (const auto& src_node : mux_graph.edge_src_nodes(edge)) {
        input_nodes.push_back(src_node);
      }
    }
    /* Number of inputs should match the branch_input_size!!! */
    VTR_ASSERT(input_nodes.size() == branch_size);
    /* build the link between input_node and branch circuit input_port[0] 
     */
    for (size_t node_id = 0; node_id < input_nodes.size(); ++node_id) {
      /* Find the port info of each input node */
      size_t input_node_level = mux_graph.node_level(input_nodes[node_id]);
      size_t input_node_index_at_level = mux_graph.node_index_at_level(input_nodes[node_id]);
      /* For inputs of mux, the net id is reserved */
      if (true == mux_graph.is_node_input(input_nodes[node_id])) {
        /* Get node input id */
        MuxInputId input_id = mux_graph.input_id(input_nodes[node_id]);
        module_manager.add_module_net_sink(mux_module, mux_module_input_nets[input_id], branch_module_id, branch_instance_id, branch_module_input_port_id, branch_module_input_port.pins()[node_id]);
      } else {
        VTR_ASSERT (false == mux_graph.is_node_input(input_nodes[node_id]));
        module_manager.add_module_net_sink(mux_module, module_nets_by_level[input_node_level][input_node_index_at_level], branch_module_id, branch_instance_id, branch_module_input_port_id, branch_module_input_port.pins()[node_id]);
      }
    }

    /* Identify if an intermediate buffer is needed */
    if (false == inter_buffer_location_map[output_node_level]) { 
      continue;
    }
    /* Add an intermediate buffer to mux_module if needed */
    if (true == mux_graph.is_node_output(node)) {
      /* Output node does not need buffer addition here, it is handled outside this function */
      continue; 
    }
    /* Now we need to add intermediate buffers by instanciating the modules */
    CircuitModelId buffer_model = circuit_lib.lut_intermediate_buffer_model(circuit_model);
    /* We must have a valid model id */
    VTR_ASSERT(CircuitModelId::INVALID() != buffer_model);

    ModuleNetId buffer_net = add_inverter_buffer_child_module_and_nets(module_manager, mux_module, circuit_lib, buffer_model, branch_net); 

    /* Record the module net id in the cache */
    module_nets_by_level[output_node_level][output_node_index_at_level] = buffer_net;
  }
}

/*********************************************************************
 * This function will add nets and input buffers (if needed) 
 * to a mux module 
 * Module net represents the connections when there are no input buffers
 *             mux_input_net[0]
 *                  |
 *                  v    +------------
 * mux_in[0] ----------->|
 *                       |
 *                       | 
 *                       |
 *                       |  Multiplexing
 *     mux_input_net[i]  |  Structure
 *                  |    |   
 *                  v    |
 * mux_in[0] ----------->|
 *                       |
 *
 *
 * Module net represents the connections when there are input buffers
 *                           mux_input_net[0]
 *                                      |
 *                 +-----------------+  v    +------------
 * mux_in[0] ----->| input_buffer[0] |-----> |
 *                 +-----------------+       |
 *                                           | 
 *                       ...                 |
 *                                           |  Multiplexing
 *                          mux_input_net[i] |   Structure
 *                                      |    |   
 *                 +-----------------+  v    |
 * mux_in[0] ----->| input_buffer[0] |-----> |
 *                 +-----------------+       |
 *********************************************************************/
static 
vtr::vector<MuxInputId, ModuleNetId> build_mux_module_input_buffers(ModuleManager& module_manager, 
                                                                    const CircuitLibrary& circuit_lib, 
                                                                    const ModuleId& mux_module, 
                                                                    const CircuitModelId& mux_model, 
                                                                    const MuxGraph& mux_graph) {
  vtr::vector<MuxInputId, ModuleNetId> mux_input_nets(mux_graph.num_inputs(), ModuleNetId::INVALID());

  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* We should have only 1 input port! */
  VTR_ASSERT(1 == mux_input_ports.size());

  /* Get the input port from MUX module */
  ModulePortId module_input_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_prefix(mux_input_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_input_port_id);
  /* Get the port from module */
  BasicPort module_input_port = module_manager.module_port(mux_module, module_input_port_id);

  /* Iterate over all the inputs in the MUX graph */
  for (const auto& input_node : mux_graph.inputs()) {
    /* Fetch fundamental information from MUX graph w.r.t. the input node */
    MuxInputId input_index = mux_graph.input_id(input_node);
    VTR_ASSERT(MuxInputId::INVALID() != input_index);   
 
    /* For last input:
     * Add a constant value to the last input, if this MUX needs a constant input
     */
    if (  (MuxInputId(mux_graph.num_inputs() - 1) == mux_graph.input_id(input_node)) 
       && (true == circuit_lib.mux_add_const_input(mux_model)) ) {
      /* Get the constant input value */
      size_t const_value = circuit_lib.mux_const_input_value(mux_model);
      VTR_ASSERT( (0 == const_value) || (1 == const_value) ); 
      /* Instanciate a VDD module (default module)
       * and build a net between VDD and the MUX input
       */
      /* Get the moduleId for the buffer module */
      ModuleId const_val_module_id = module_manager.find_module(generate_const_value_module_name(const_value));
      /* We must have one */
      VTR_ASSERT(ModuleId::INVALID() != const_val_module_id);
      size_t const_val_instance = module_manager.num_instance(mux_module, const_val_module_id);
      module_manager.add_child_module(mux_module, const_val_module_id);
      ModulePortId const_port_id = module_manager.find_module_port(const_val_module_id, generate_const_value_module_output_port_name(const_value));

      ModuleNetId input_net = module_manager.create_module_net(mux_module);
      module_manager.add_module_net_source(mux_module, input_net, const_val_module_id, const_val_instance, const_port_id, 0);
      mux_input_nets[input_index] = input_net;
      continue;
    }

    /* When we do not need any buffer, create a net for the input directly */
    if (false == circuit_lib.is_input_buffered(mux_model)) {
      ModuleNetId input_net = module_manager.create_module_net(mux_module);
      module_manager.add_module_net_source(mux_module, input_net, mux_module, 0, module_input_port_id, size_t(input_index));
      mux_input_nets[input_index] = input_net;
      continue;
    }
    
    /* Now we need to add intermediate buffers by instanciating the modules */
    CircuitModelId buffer_model = circuit_lib.input_buffer_model(mux_model);
    /* We must have a valid model id */
    VTR_ASSERT(CircuitModelId::INVALID() != buffer_model);

    /* Connect the module net from branch output to buffer input */
    ModuleNetId buffer_net = module_manager.create_module_net(mux_module);
    module_manager.add_module_net_source(mux_module, buffer_net, mux_module, 0, module_input_port_id, size_t(input_index));

    /* Create a module net which sources from buffer output */
    ModuleNetId input_net = add_inverter_buffer_child_module_and_nets(module_manager, mux_module, circuit_lib, buffer_model, buffer_net); 
    mux_input_nets[input_index] = input_net;
  }

  return mux_input_nets;
}

/*********************************************************************
 * This function will add nets and input buffers (if needed) 
 * to a mux module 
 * Module net represents the connections when there are no output buffers
 *
 *             mux_output_net[0]
 * ------------+   |
 *             |   v
 *             |--------> mux_output[0]
 *             |
 *             |
 * Multiplexer | ...
 * Strcuture   |
 *             |--------> mux_output[i]
 *             |   ^
 *             |   |
 * ------------+  mux_output_net[i]
 *
 * Module net represents the connections when there are output buffers
 *
 *              mux_output_net[0]
 * ------------+    |
 *             |    |
 *             |    v   +------------------+   
 *             |------->| output_buffer[0] |------> mux_output[0]
 *             |        +------------------+  
 *             |
 * Multiplexer |        ...
 * Strcuture   |
 *             |        +------------------+   
 *             |------->| output_buffer[i] |------> mux_output[i]
 *             |    ^   +------------------+    
 *             |    |
 *             |    |
 * ------------+   mux_output_net[i]

 *
 *********************************************************************/
static 
vtr::vector<MuxOutputId, ModuleNetId> build_mux_module_output_buffers(ModuleManager& module_manager, 
                                                                      const CircuitLibrary& circuit_lib, 
                                                                      const ModuleId& mux_module, 
                                                                      const CircuitModelId& mux_model, 
                                                                      const MuxGraph& mux_graph) {

  /* Create module nets for output ports */
  vtr::vector<MuxOutputId, ModuleNetId> mux_output_nets(mux_graph.num_outputs(), ModuleNetId::INVALID());

  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_OUTPUT, false);

  /* Iterate over all the outputs in the MUX module */
  for (const auto& output_port : mux_output_ports) {
    /* Get the output port from MUX module */
    ModulePortId module_output_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_prefix(output_port));
    VTR_ASSERT(ModulePortId::INVALID() != module_output_port_id);
    /* Get the port from module */
    BasicPort module_output_port = module_manager.module_port(mux_module, module_output_port_id);

    /* Iterate over each pin of the output port */
    for (const size_t& pin : circuit_lib.pins(output_port)) {
      /* Fetch fundamental information from MUX graph w.r.t. the input node */
      /* Deposite the last level of the graph, which is a default value */
      size_t output_node_level = mux_graph.num_node_levels() - 1; 
      /* If there is a fracturable level specified for the output, we find the exact level */
      if (size_t(-1) != circuit_lib.port_lut_frac_level(output_port)) {
        output_node_level = circuit_lib.port_lut_frac_level(output_port);
      }
      /* Deposite a zero, which is a default value */
      size_t output_node_index_at_level = 0; 
      /* If there are output masks, we find the node_index */
      if (!circuit_lib.port_lut_output_mask(output_port).empty()) {
        output_node_index_at_level = circuit_lib.port_lut_output_mask(output_port).at(pin);
      } 
      /* Double check the node exists in the Mux Graph */
      MuxNodeId node_id = mux_graph.node_id(output_node_level, output_node_index_at_level);
      VTR_ASSERT(MuxNodeId::INVALID() != node_id);
      MuxOutputId output_index = mux_graph.output_id(node_id);

      /* Create the port information of the module output at the given pin range, which is the output of buffer instance */
      BasicPort instance_output_port(module_output_port.get_name(), pin, pin);

      /* If the output is not supposed to be buffered, create a net for the input directly */
      if (false == circuit_lib.is_output_buffered(mux_model)) {
        ModuleNetId output_net = module_manager.create_module_net(mux_module);
        module_manager.add_module_net_sink(mux_module, output_net, mux_module, 0, module_output_port_id, pin);
        mux_output_nets[output_index] = output_net;
        continue; /* Finish here */
      }

      /* Reach here, we need a buffer, create a port-to-port map and output the buffer instance */
      /* Now we need to add intermediate buffers by instanciating the modules */
      CircuitModelId buffer_model = circuit_lib.output_buffer_model(mux_model);
      /* We must have a valid model id */
      VTR_ASSERT(CircuitModelId::INVALID() != buffer_model);

      /* Create a module net which sinks at buffer input */
      ModuleNetId input_net = module_manager.create_module_net(mux_module);
      ModuleNetId output_net = add_inverter_buffer_child_module_and_nets(module_manager, mux_module, circuit_lib, buffer_model, input_net); 
      module_manager.add_module_net_sink(mux_module, output_net, mux_module, 0, module_output_port_id, pin);
      mux_output_nets[output_index] = input_net;
    }
  }

  return mux_output_nets;
}

/*********************************************************************
 * This function will 
 * 1. Build local encoders for a MUX module (if specified)
 * 2. Build nets between memory ports of a MUX module and branch circuits
 *    This happens when local encoders are not needed
 *
 *               MUX module     
 *             +---------------------
 *             | mux_mem_nets/mux_mem_inv_nets
 *             |   |
 *             |   v     +---------
 *         mem-+-------->|
 *             |         | Branch Module 
 *             |         |
 *
 * 3. Build nets between local encoders and memory ports of a MUX module
 *    This happens when local encoders are needed
 * 4. Build nets between local encoders and branch circuits
 *    This happens when local encoders are needed
 *
 *               MUX module
 *             +---------------------
 *             |
 *             |       +-------+  mux_mem_nets/mux_mem_inv_nets
 *             |       |       |    |
 *        mem--+------>|       |    v    +---------
 *             |       | Local |-------->|
 *             |       |Encoder|         | Branch
 *             |       |       |         | Module
 *             |       |       |         |
 *             |       |       |         |
 *
 *********************************************************************/
static 
void build_mux_module_local_encoders_and_memory_nets(ModuleManager& module_manager, 
                                                     const ModuleId& mux_module, 
                                                     const CircuitLibrary& circuit_lib, 
                                                     const CircuitModelId& mux_model, 
                                                     const std::vector<CircuitPortId>& mux_sram_ports, 
                                                     const MuxGraph& mux_graph,
                                                     vtr::vector<MuxMemId, ModuleNetId>& mux_mem_nets,
                                                     vtr::vector<MuxMemId, ModuleNetId>& mux_mem_inv_nets) {

  /* Create nets here, and we will configure the net source later */
  for (size_t mem = 0; mem < mux_graph.num_memory_bits(); ++mem) {
    ModuleNetId mem_net = module_manager.create_module_net(mux_module);
    mux_mem_nets.push_back(mem_net);
    ModuleNetId mem_inv_net = module_manager.create_module_net(mux_module);
    mux_mem_inv_nets.push_back(mem_inv_net);
  }

  if (false == circuit_lib.mux_use_local_encoder(mux_model)) {
    /* Add mem and mem_inv nets here */
    size_t mem_net_cnt = 0;
    for (const auto& port : mux_sram_ports) {
      ModulePortId mem_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_prefix(port));
      BasicPort mem_port = module_manager.module_port(mux_module, mem_port_id);
      for (const size_t& pin : mem_port.pins()) {
        MuxMemId mem_id = MuxMemId(mem_net_cnt);
        /* Set the module net source */
        module_manager.add_module_net_source(mux_module, mux_mem_nets[mem_id], mux_module, 0, mem_port_id, pin);
        /* Update counter */
        mem_net_cnt++;
      }
    }
    VTR_ASSERT(mem_net_cnt == mux_graph.num_memory_bits());

    /* Add mem and mem_inv nets here */
    size_t mem_inv_net_cnt = 0;
    for (const auto& port : mux_sram_ports) {
      ModulePortId mem_inv_port_id = module_manager.find_module_port(mux_module, std::string(circuit_lib.port_prefix(port) + INV_PORT_POSTFIX));
      BasicPort mem_inv_port = module_manager.module_port(mux_module, mem_inv_port_id);
      for (const size_t& pin : mem_inv_port.pins()) {
        MuxMemId mem_id = MuxMemId(mem_inv_net_cnt);
        /* Set the module net source */
        module_manager.add_module_net_source(mux_module, mux_mem_inv_nets[mem_id], mux_module, 0, mem_inv_port_id, pin);
        /* Update counter */
        mem_inv_net_cnt++;
      }
    }
    VTR_ASSERT(mem_inv_net_cnt == mux_graph.num_memory_bits());
    return; /* Finish here if local encoders are not required */
  }

  /* Add local decoder instance here */
  VTR_ASSERT(true == circuit_lib.mux_use_local_encoder(mux_model));
  BasicPort decoder_data_port(generate_mux_local_decoder_data_port_name(), mux_graph.num_memory_bits());
  BasicPort decoder_data_inv_port(generate_mux_local_decoder_data_inv_port_name(), mux_graph.num_memory_bits());

  /* Local port to record the LSB and MSB of each level, here, we deposite (0, 0) */
  ModulePortId mux_module_sram_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_prefix(mux_sram_ports[0]));
  ModulePortId mux_module_sram_inv_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_prefix(mux_sram_ports[0]) + INV_PORT_POSTFIX);
  BasicPort lvl_addr_port(circuit_lib.port_prefix(mux_sram_ports[0]), 0);
  BasicPort lvl_data_port(decoder_data_port.get_name(), 0);
  BasicPort lvl_data_inv_port(decoder_data_inv_port.get_name(), 0);

  /* Counter for mem index */
  size_t mem_net_cnt = 0;
  size_t mem_inv_net_cnt = 0;

  for (const auto& lvl : mux_graph.levels()) {
    size_t addr_size = find_mux_local_decoder_addr_size(mux_graph.num_memory_bits_at_level(lvl));
    size_t data_size = mux_graph.num_memory_bits_at_level(lvl);
    /* Update the LSB and MSB of addr and data port for the current level */
    lvl_addr_port.rotate(addr_size);
    lvl_data_port.rotate(data_size);
    lvl_data_inv_port.rotate(data_size);

    /* Exception: if the data size is one, we just need wires! */
    if (1 == data_size) {
      for (size_t pin_id = 0; pin_id < lvl_addr_port.pins().size(); ++pin_id) {
        MuxMemId mem_id = MuxMemId(mem_net_cnt);
        /* Set the module net source */
        module_manager.add_module_net_source(mux_module, mux_mem_nets[mem_id], mux_module, 0, mux_module_sram_port_id, lvl_addr_port.pins()[pin_id]);
        /* Update counter */
        mem_net_cnt++;

        MuxMemId mem_inv_id = MuxMemId(mem_inv_net_cnt);
        /* Set the module net source */
        module_manager.add_module_net_source(mux_module, mux_mem_inv_nets[mem_inv_id], mux_module, 0, mux_module_sram_inv_port_id, lvl_addr_port.pins()[pin_id]);
        /* Update counter */
        mem_inv_net_cnt++;
      }
      continue;
    }

    std::string decoder_module_name = generate_mux_local_decoder_subckt_name(addr_size, data_size);
    ModuleId decoder_module = module_manager.find_module(decoder_module_name); 
    VTR_ASSERT(ModuleId::INVALID() != decoder_module);

    size_t decoder_instance = module_manager.num_instance(mux_module, decoder_module);
    module_manager.add_child_module(mux_module, decoder_module);
  
    /* Add module nets to connect sram ports of MUX to address port */
    ModulePortId decoder_module_addr_port_id = module_manager.find_module_port(decoder_module, generate_mux_local_decoder_addr_port_name());
    BasicPort decoder_module_addr_port = module_manager.module_port(decoder_module, decoder_module_addr_port_id);
    VTR_ASSERT(decoder_module_addr_port.get_width() == lvl_addr_port.get_width());

    /* Build pin-to-pin net connection */
    for (size_t pin_id = 0; pin_id < lvl_addr_port.pins().size(); ++pin_id) {
      ModuleNetId net = module_manager.create_module_net(mux_module);
      module_manager.add_module_net_source(mux_module, net, mux_module, 0, mux_module_sram_port_id, lvl_addr_port.pins()[pin_id]);
      module_manager.add_module_net_sink(mux_module, net, decoder_module, decoder_instance, decoder_module_addr_port_id, decoder_module_addr_port.pins()[pin_id]);
    }

    /* Add module nets to connect data port to MUX mem ports */
    ModulePortId decoder_module_data_port_id = module_manager.find_module_port(decoder_module, generate_mux_local_decoder_data_port_name());
    BasicPort decoder_module_data_port = module_manager.module_port(decoder_module, decoder_module_data_port_id);
    
    /* Build pin-to-pin net connection */
    for (const size_t& pin : decoder_module_data_port.pins()) {
      ModuleNetId net = mux_mem_nets[MuxMemId(mem_net_cnt)];
      module_manager.add_module_net_source(mux_module, net, decoder_module, decoder_instance, decoder_module_data_port_id, pin);
      /* Add the module nets to mux_mem_nets cache */
      mem_net_cnt++;
    }

    ModulePortId decoder_module_data_inv_port_id = module_manager.find_module_port(decoder_module, generate_mux_local_decoder_data_inv_port_name());
    BasicPort decoder_module_data_inv_port = module_manager.module_port(decoder_module, decoder_module_data_inv_port_id);

    /* Build pin-to-pin net connection */
    for (const size_t& pin : decoder_module_data_inv_port.pins()) {
      ModuleNetId net = mux_mem_inv_nets[MuxMemId(mem_inv_net_cnt)];
      module_manager.add_module_net_source(mux_module, net, decoder_module, decoder_instance, decoder_module_data_inv_port_id, pin);
      /* Add the module nets to mux_mem_inv_nets cache */
      mem_inv_net_cnt++;
    }
  } 
  VTR_ASSERT(mem_net_cnt == mux_graph.num_memory_bits());
  VTR_ASSERT(mem_inv_net_cnt == mux_graph.num_memory_bits());
}

/*********************************************************************
 * Generate module of a CMOS multiplexer with the given size 
 * The module will consist of three parts:
 * 1. instances of the branch circuits of multiplexers which are generated before  
 *    This builds up the multiplexing structure
 * 2. Input buffers/inverters
 * 3. Output buffers/inverters
 *********************************************************************/
static 
void build_cmos_mux_module(ModuleManager& module_manager,
                           const CircuitLibrary& circuit_lib, 
                           const CircuitModelId& mux_model, 
                           const std::string& module_name, 
                           const MuxGraph& mux_graph) {
  /* Get the global ports required by MUX (and any submodules) */
  std::vector<CircuitPortId> mux_global_ports = circuit_lib.model_global_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_INPUT, true, true);
  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_OUTPUT, false);
  /* Get the sram ports from the mux 
   * Multiplexing structure does not mode_sram_ports, they are handled in LUT modules
   * Here we just bypass it.
   */
  std::vector<CircuitPortId> mux_sram_ports = find_circuit_regular_sram_ports(circuit_lib, mux_model);

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of data-path inputs */ 
  size_t num_inputs = find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs());
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* The size of of memory ports depend on 
   * if a local encoder is used for the mux or not 
   * Multiplexer local encoders are applied to memory bits at each stage 
   */
  if (true == circuit_lib.mux_use_local_encoder(mux_model)) {
    num_mems = 0;
    for (const auto& lvl : mux_graph.levels()) {
      size_t data_size = mux_graph.num_memory_bits_at_level(lvl);
      num_mems += find_mux_local_decoder_addr_size(data_size);
    } 
  }

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 output */
  VTR_ASSERT(1 == mux_input_ports.size());
  /* A quick check on the model ports */
  if ((CIRCUIT_MODEL_MUX == circuit_lib.model_type(mux_model))
    || ((CIRCUIT_MODEL_LUT == circuit_lib.model_type(mux_model))
       && (false == circuit_lib.is_lut_fracturable(mux_model))) ) {
    VTR_ASSERT(1 == mux_output_ports.size());
    VTR_ASSERT(1 == circuit_lib.port_size(mux_output_ports[0])); 
  } else {
    VTR_ASSERT_SAFE( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(mux_model)) 
                 && (true == circuit_lib.is_lut_fracturable(mux_model)) );
    for (const auto& port : mux_output_ports) {
      VTR_ASSERT(0 < circuit_lib.port_size(port));
    }
  }

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId mux_module = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != mux_module);
  /* Add module ports */
  /* Add each input port
   * Treat MUX and LUT differently 
   * 1. MUXes: we do not have a specific input/output sizes, it is inferred by architecture 
   * 2. LUTes: we do have specific input/output sizes, 
   *           but the inputs of MUXes are the SRAM ports of LUTs
   *           and the SRAM ports of MUXes are the inputs of LUTs
   */
  size_t input_port_cnt = 0;
  for (const auto& port : mux_input_ports) {
    BasicPort input_port(circuit_lib.port_prefix(port), num_inputs);
    module_manager.add_port(mux_module, input_port, ModuleManager::MODULE_INPUT_PORT);
    /* Update counter */
    input_port_cnt++;
  }
  /* Double check: We should have only 1 input port generated here! */
  VTR_ASSERT(1 == input_port_cnt);

  /* Add input buffers and update module nets for inputs */
  vtr::vector<MuxInputId, ModuleNetId> mux_input_nets = build_mux_module_input_buffers(module_manager, circuit_lib, mux_module, mux_model, mux_graph);

  for (const auto& port : mux_output_ports) {
    BasicPort output_port(circuit_lib.port_prefix(port), num_outputs);
    if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(mux_model)) {
      output_port.set_width(circuit_lib.port_size(port));
    }
    module_manager.add_port(mux_module, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* TODO: Add output buffers and update module nets for outputs */
  vtr::vector<MuxOutputId, ModuleNetId> mux_output_nets = build_mux_module_output_buffers(module_manager, circuit_lib, mux_module, mux_model, mux_graph);

  size_t sram_port_cnt = 0;
  for (const auto& port : mux_sram_ports) {
    BasicPort mem_port(circuit_lib.port_prefix(port), num_mems);
    module_manager.add_port(mux_module, mem_port, ModuleManager::MODULE_INPUT_PORT);
    BasicPort mem_inv_port(std::string(circuit_lib.port_prefix(port) + INV_PORT_POSTFIX), num_mems);
    module_manager.add_port(mux_module, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);
    /* Update counter */
    sram_port_cnt++;
  }
  VTR_ASSERT(1 == sram_port_cnt);

  /* Create module nets for mem and mem_inv ports */
  vtr::vector<MuxMemId, ModuleNetId> mux_mem_nets;
  vtr::vector<MuxMemId, ModuleNetId> mux_mem_inv_nets;
  
  build_mux_module_local_encoders_and_memory_nets(module_manager, mux_module, 
                                                  circuit_lib, mux_model, mux_sram_ports, 
                                                  mux_graph,
                                                  mux_mem_nets, mux_mem_inv_nets);
  
  /* Print the internal logic in Verilog codes */
  /* Print the Multiplexing structure in Verilog codes 
   * Separated generation strategy on using standard cell MUX2 or TGATE,
   * 1. MUX2 has a fixed port map: input_port[0] and input_port[1] is the data_path input 
   * 2. Branch TGATE-based module has a fixed port name  
   * TODO: the naming could be more flexible? 
   */
  /* Get the tgate model */
  CircuitModelId tgate_model = circuit_lib.pass_gate_logic_model(mux_model);
  /* Instanciate the branch module: 
   * Case 1: the branch module is a standard cell MUX2
   * Case 2: the branch module is a tgate-based module  
   */
  std::string branch_module_name;
  if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(tgate_model)) {
    VTR_ASSERT(CIRCUIT_MODEL_GATE_MUX2 == circuit_lib.gate_type(tgate_model));
    build_cmos_mux_module_mux2_multiplexing_structure(module_manager, circuit_lib, mux_module, mux_model, tgate_model, mux_input_nets, mux_output_nets, mux_mem_nets, mux_graph);
  } else {
    VTR_ASSERT(CIRCUIT_MODEL_PASSGATE == circuit_lib.model_type(tgate_model));
    build_cmos_mux_module_tgate_multiplexing_structure(module_manager, circuit_lib, mux_module, mux_model, mux_input_nets, mux_output_nets, mux_mem_nets, mux_mem_inv_nets, mux_graph);
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mux_module);
}

/*********************************************************************
 * Generate a module of a RRAM-based multiplexer with the given size 
 * The module will consist of three parts:
 * 1. instances of the branch circuits of multiplexers which are generated before  
 *    This builds up the 4T1R-based multiplexing structure
 *
 *                    BLB   WL
 *                     |    |       ...
 *                     v    v
 *                   +--------+            
 *           in[0]-->|        |            BLB   WL 
 *                ...| Branch |-----+       |    |
 *             in -->|   0    |     |       v    v
 *            [N-1]  +--------+     |     +--------+
 *                      ...            -->|        |
 *                    BLBs WLs         ...| Branch |
 *                     |    |    ...   -->|   X    |
 *                     v    v             +--------+
 *                   +--------+    |
 *                -->|        |    |
 *                ...| Branch |----+
 *                -->|   i    |
 *                   +--------+
 *
 * 2. Input buffers/inverters
 * 3. Output buffers/inverters
 *********************************************************************/
static 
void build_rram_mux_module(ModuleManager& module_manager,
                           const CircuitLibrary& circuit_lib, 
                           const CircuitModelId& circuit_model, 
                           const std::string& module_name, 
                           const MuxGraph& mux_graph) {
  /* Error out for the conditions where we are not yet supported! */
  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
    /* RRAM LUT is not supported now... */
    VTR_LOGF_ERROR(__FILE__, __LINE__, "RRAM-based LUT is not supported for circuit model '%s')!\n",
                  circuit_lib.model_name(circuit_model).c_str());
    exit(1);
  }

  /* Get the global ports required by MUX (and any submodules) */
  std::vector<CircuitPortId> mux_global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);
  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  /* Get the BL and WL ports from the mux */
  std::vector<CircuitPortId> mux_blb_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> mux_wl_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_WL, true);

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of data-path inputs */ 
  size_t num_inputs = find_mux_num_datapath_inputs(circuit_lib, circuit_model, mux_graph.num_inputs());
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 input and 1 BLB and 1 WL port */
  VTR_ASSERT(1 == mux_input_ports.size());
  VTR_ASSERT(1 == mux_blb_ports.size());
  VTR_ASSERT(1 == mux_wl_ports.size());

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Add module ports */
  /* Add each global port */
  for (const auto& port : mux_global_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_prefix(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  size_t input_port_cnt = 0;
  for (const auto& port : mux_input_ports) {
    BasicPort input_port(circuit_lib.port_prefix(port), num_inputs);
    module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
    /* Update counter */
    input_port_cnt++;
  }
  /* Double check: We should have only 1 input port generated here! */
  VTR_ASSERT(1 == input_port_cnt);

  for (const auto& port : mux_output_ports) {
    BasicPort output_port(circuit_lib.port_prefix(port), num_outputs);
    if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
      output_port.set_width(circuit_lib.port_size(port));
    }
    module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* BLB port */
  for (const auto& port : mux_blb_ports) {
    /* IMPORTANT: RRAM-based MUX has an additional BLB pin per level 
     * So, the actual port width of BLB should be added by the number of levels of the MUX graph 
     */
    BasicPort blb_port(circuit_lib.port_prefix(port), num_mems + mux_graph.num_levels());
    module_manager.add_port(module_id, blb_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* WL port */
  for (const auto& port : mux_wl_ports) {
    /* IMPORTANT: RRAM-based MUX has an additional WL pin per level 
     * So, the actual port width of WL should be added by the number of levels of the MUX graph 
     */
    BasicPort wl_port(circuit_lib.port_prefix(port), num_mems + mux_graph.num_levels());
    module_manager.add_port(module_id, wl_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* TODO: Add the input and output buffers in Verilog codes */
 
  /* TODO: Print the internal logic in Verilog codes */
}

/***********************************************
 * Generate Verilog codes modeling a multiplexer 
 * with the given graph-level description
 **********************************************/
static 
void build_mux_module(ModuleManager& module_manager,
                      const CircuitLibrary& circuit_lib, 
                      const CircuitModelId& circuit_model, 
                      const MuxGraph& mux_graph) {
  std::string module_name = generate_mux_subckt_name(circuit_lib, circuit_model, 
                                                     find_mux_num_datapath_inputs(circuit_lib, circuit_model, mux_graph.num_inputs()), 
                                                     std::string(""));
 
  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(circuit_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    /* SRAM-based Multiplexer Verilog module generation */
    build_cmos_mux_module(module_manager, circuit_lib, circuit_model, module_name, mux_graph);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    /* TODO: RRAM-based Multiplexer Verilog module generation */
    build_rram_mux_module(module_manager, circuit_lib, circuit_model, module_name, mux_graph);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid design technology of multiplexer '%s'\n",
                 circuit_lib.model_name(circuit_model).c_str()); 
    exit(1);
  }
}

/***********************************************
 * Generate Verilog modules for all the unique
 * multiplexers in the FPGA device
 **********************************************/
void build_mux_modules(ModuleManager& module_manager,
                       const MuxLibrary& mux_lib,
                       const CircuitLibrary& circuit_lib) {
  vtr::ScopedStartFinishTimer timer("Building multiplexer modules");

  /* Generate basis sub-circuit for unique branches shared by the multiplexers */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_circuit_model = mux_lib.mux_circuit_model(mux); 
    /* Create a mux graph for the branch circuit */
    std::vector<MuxGraph> branch_mux_graphs = mux_graph.build_mux_branch_graphs();
    /* Create branch circuits, which are N:1 one-level or 2:1 tree-like MUXes */
    for (auto branch_mux_graph : branch_mux_graphs) {
      build_mux_branch_module(module_manager, circuit_lib, mux_circuit_model, 
                              find_mux_num_datapath_inputs(circuit_lib, mux_circuit_model, mux_graph.num_inputs()), 
                              branch_mux_graph);
    }
  }

  /* Generate unique Verilog modules for the multiplexers */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_circuit_model = mux_lib.mux_circuit_model(mux); 
    /* Create MUX circuits */
    build_mux_module(module_manager, circuit_lib, mux_circuit_model, mux_graph);
  }
}

} /* end namespace openfpga */
