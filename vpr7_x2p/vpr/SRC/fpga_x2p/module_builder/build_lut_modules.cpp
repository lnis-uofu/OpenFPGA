/******************************************************************** 
 * This file include functions that create modules for 
 * the Look-Up Tables (LUTs) 
 ********************************************************************/
#include <ctime>
#include <string>
#include <vector>

#include "vtr_assert.h"
#include "util.h"
#include "spice_types.h"

#include "fpga_x2p_naming.h"
#include "circuit_library_utils.h"
#include "module_manager.h"
#include "module_manager_utils.h"

#include "build_module_graph_utils.h"
#include "build_lut_modules.h"

/********************************************************************
 * Build a module for a LUT circuit model
 * This function supports both single-output and fracturable LUTs
 * The module will be organized in a connected graph of the following instances:
 * 1. Multiplexer used inside LUT
 * 2. Input buffers 
 * 3. Input inverters
 * 4. Output buffers.
 * 6. AND/OR gates to tri-state LUT inputs
 ********************************************************************/
static 
void build_lut_module(ModuleManager& module_manager,
                      const CircuitLibrary& circuit_lib,
                      const CircuitModelId& lut_model) {
  /* Get the global ports required by LUT
   * Note that this function will only add global ports from LUT circuit model definition itself
   * We should NOT go recursively here. 
   * The global ports of sub module will be handled by another function !!! 
   *   add_module_global_ports_from_child_modules(module_manager, lut_module);
   */
  std::vector<CircuitPortId> lut_global_ports = circuit_lib.model_global_ports_by_type(lut_model, SPICE_MODEL_PORT_INPUT, false, true);
  /* Get the input ports from the mux */
  std::vector<CircuitPortId> lut_input_ports = circuit_lib.model_ports_by_type(lut_model, SPICE_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> lut_output_ports = circuit_lib.model_ports_by_type(lut_model, SPICE_MODEL_PORT_OUTPUT, true);

  /* Classify SRAM ports into two categories: regular (not for mode select) and mode-select */
  std::vector<CircuitPortId> lut_regular_sram_ports = find_circuit_regular_sram_ports(circuit_lib, lut_model);
  std::vector<CircuitPortId> lut_mode_select_sram_ports = find_circuit_mode_select_sram_ports(circuit_lib, lut_model);

  /*********************************************** 
   * Model Port Sanity Check
   ***********************************************/  
  /* Make sure that the number of ports and sizes of ports are what we want */
  if (false == circuit_lib.is_lut_fracturable(lut_model)) {
    /* Single-output LUTs: 
     * We should have only 1 input port, 1 output port and 1 SRAM port
     */
    VTR_ASSERT (1 == lut_input_ports.size());
    VTR_ASSERT (1 == lut_output_ports.size());
    VTR_ASSERT (1 == lut_regular_sram_ports.size()); 
    VTR_ASSERT (0 == lut_mode_select_sram_ports.size()); 
  } else {
    VTR_ASSERT (true == circuit_lib.is_lut_fracturable(lut_model));
    /* Fracturable LUT:
     * We should have only 1 input port, a few output ports (fracturable outputs)
     * and two SRAM ports
     */
    VTR_ASSERT (1 == lut_input_ports.size());
    VTR_ASSERT (1 <= lut_output_ports.size());
    VTR_ASSERT (1 == lut_regular_sram_ports.size()); 
    VTR_ASSERT (1 == lut_mode_select_sram_ports.size()); 
  }

  /*********************************************** 
   * Module Port addition
   ***********************************************/  
  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId lut_module = module_manager.add_module(circuit_lib.model_name(lut_model)); 
  VTR_ASSERT(true == module_manager.valid_module_id(lut_module));
  /* Add module ports */
  /* Add each global port */
  for (const auto& port : lut_global_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(lut_module, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  for (const auto& port : lut_input_ports) {
    BasicPort input_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(lut_module, input_port, ModuleManager::MODULE_INPUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(lut_module, input_port.get_name(), true);
  }
  /* Add each output port */
  for (const auto& port : lut_output_ports) {
    BasicPort output_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(lut_module, output_port, ModuleManager::MODULE_OUTPUT_PORT);
    /* Set the port to be wire-connection */
    module_manager.set_port_is_wire(lut_module, output_port.get_name(), true);
  }
  /* Add each regular (not mode select) SRAM port */
  for (const auto& port : lut_regular_sram_ports) {
    BasicPort mem_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(lut_module, mem_port, ModuleManager::MODULE_INPUT_PORT);
    BasicPort mem_inv_port(std::string(circuit_lib.port_lib_name(port) + "_inv"), circuit_lib.port_size(port));
    module_manager.add_port(lut_module, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* Add each mode-select SRAM port */
  for (const auto& port : lut_mode_select_sram_ports) {
    BasicPort mem_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(lut_module, mem_port, ModuleManager::MODULE_INPUT_PORT);
    BasicPort mem_inv_port(std::string(circuit_lib.port_lib_name(port) + "_inv"), circuit_lib.port_size(port));
    module_manager.add_port(lut_module, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /*********************************************** 
   * Child module addition: Model-select gates
   ***********************************************/  
  /* Module nets after the mode-selection circuit, this could include LUT inputs */
  std::vector<ModuleNetId> mode_selected_nets;
  /* Instanciate mode selecting circuit: AND/OR gate 
   * By following the tri-state map of LUT input port
   * The wiring of input ports will be organized as follows
   *
   *                   LUT input
   *                       |
   *                       v
   *                  +----------+
   *                  |   mode   |
   *                  | selector |
   *                  +----------+
   *                       | mode_selected_nets
   *                       v
   *     +-----------------+------------+
   *     |                              |
   *     v                              v
   *  +----------+                 +---------+
   *  | Inverter |                 |  Buffer |
   *  +----------+                 +---------+
   *       | inverter_output_net        | buffered_output_net
   *       v                            v
   *  +--------------------------------------+
   *  |      LUT Multiplexing Structure      |
   *  +--------------------------------------+
   */
  /* Get the tri-state port map for the input ports*/
  std::string tri_state_map = circuit_lib.port_tri_state_map(lut_input_ports[0]);
  size_t mode_select_port_lsb = 0;
  for (const auto& pin : circuit_lib.pins(lut_input_ports[0])) {
    ModulePortId lut_module_input_port_id = module_manager.find_module_port(lut_module, circuit_lib.port_lib_name(lut_input_ports[0]));
    VTR_ASSERT(true == module_manager.valid_module_port_id(lut_module, lut_module_input_port_id));

    /* Create a module net for the connection */
    ModuleNetId net = module_manager.create_module_net(lut_module);
    /* Set the source of the net to an lut input port */
    module_manager.add_module_net_source(lut_module, net, lut_module, 0, lut_module_input_port_id, pin);
    
    /* For an empty tri-state map or a '-' sign in tri-state map, we can short-wire mode select_output_ports */
    if (tri_state_map.empty() || ('-' == tri_state_map[pin]) ) {
      /* Update the output nets of the mode-select layer */
      mode_selected_nets.push_back(net); 
      continue; /* Finish here */
    }

    e_spice_model_gate_type required_gate_type = NUM_SPICE_MODEL_GATE_TYPES;
    /* Reach here, it means that we need a circuit for mode selection */
    if ('0' == tri_state_map[pin]) {
      /* We need a 2-input AND gate, in order to tri-state the input 
       * Detailed circuit is as follow:
       *              +---------+
       *     SRAM --->| 2-input |----> mode_select_output_port
       * LUT input--->|   AND   |
       *              +---------+
       * When SRAM is set to logic 0, the LUT input is tri-stated
       * When SRAM is set to logic 1, the LUT input is effective to the downstream circuits
       */
       required_gate_type = SPICE_MODEL_GATE_AND;
    } else {
      VTR_ASSERT ('1' == tri_state_map[pin]);
      /* We need a 2-input OR gate, in order to tri-state the input 
       * Detailed circuit is as follow:
       *              +---------+
       *     SRAM --->| 2-input |----> mode_select_output_port
       * LUT input--->|   OR    |
       *              +---------+
       * When SRAM is set to logic 1, the LUT input is tri-stated
       * When SRAM is set to logic 0, the LUT input is effective to the downstream circuits
       */
       required_gate_type = SPICE_MODEL_GATE_OR;
    } 
    /* Get the circuit model of the gate */
    CircuitModelId gate_model = circuit_lib.port_tri_state_model(lut_input_ports[0]);
    /* Check this is the gate we want ! */
    VTR_ASSERT (required_gate_type == circuit_lib.gate_type(gate_model));

    /* Prepare for the gate instanciation */
    /* Get the input ports from the gate */
    std::vector<CircuitPortId> gate_input_ports = circuit_lib.model_ports_by_type(gate_model, SPICE_MODEL_PORT_INPUT, true);
    /* Get the output ports from the gate */
    std::vector<CircuitPortId> gate_output_ports = circuit_lib.model_ports_by_type(gate_model, SPICE_MODEL_PORT_OUTPUT, true);
    /* Check the port sizes and width: 
     * we should have only 2 input ports, each of which has a size of 1 
     * we should have only 1 output port, each of which has a size of 1 
     */
    VTR_ASSERT (2 == gate_input_ports.size());
    VTR_ASSERT (1 == gate_output_ports.size());
    /* Find the module id of gate_model in the module manager */
    ModuleId gate_module = module_manager.find_module(circuit_lib.model_name(gate_model));
    /* We must have a valid id */
    VTR_ASSERT (true == module_manager.valid_module_id(gate_module));
    size_t gate_instance = module_manager.num_instance(lut_module, gate_module);
    module_manager.add_child_module(lut_module, gate_module);

    /* Create a port-to-port net connection:
     * Input[0] of the gate is wired to a SRAM mode-select port 
     * Input[1] of the gate is wired to the input port of LUT
     * Output[0] of the gate is wired to the mode_select_output_port 
     */
    /* Create a module net for the connection */
    ModuleNetId gate_sram_net = module_manager.create_module_net(lut_module);

    /* Find the module port id of the SRAM port of LUT module */
    ModulePortId lut_module_mode_select_port_id = module_manager.find_module_port(lut_module, circuit_lib.port_lib_name(lut_mode_select_sram_ports[0]));
    VTR_ASSERT(true == module_manager.valid_module_port_id(lut_module, lut_module_mode_select_port_id));
    /* Set the source of the net to an mode-select SRAM port of the LUT module */
    module_manager.add_module_net_source(lut_module, gate_sram_net, lut_module, 0, lut_module_mode_select_port_id, mode_select_port_lsb);

    /* Find the module port id of the SRAM port of LUT module */
    ModulePortId gate_module_input0_port_id = module_manager.find_module_port(gate_module, circuit_lib.port_lib_name(gate_input_ports[0]));
    VTR_ASSERT(true == module_manager.valid_module_port_id(gate_module, gate_module_input0_port_id));
    /* Set the sink of the net to an input[0] port of the gate module */
    VTR_ASSERT(1 == module_manager.module_port(gate_module, gate_module_input0_port_id).get_width());
    for (const size_t& gate_pin : module_manager.module_port(gate_module, gate_module_input0_port_id).pins()) {
      module_manager.add_module_net_sink(lut_module, gate_sram_net, gate_module, gate_instance, gate_module_input0_port_id, gate_pin);
    }

    /* Use the existing net to connect to the input[1] port of the gate module */
    ModulePortId gate_module_input1_port_id = module_manager.find_module_port(gate_module, circuit_lib.port_lib_name(gate_input_ports[1]));
    VTR_ASSERT(true == module_manager.valid_module_port_id(gate_module, gate_module_input1_port_id));
    VTR_ASSERT(1 == module_manager.module_port(gate_module, gate_module_input1_port_id).get_width());
    for (const size_t& gate_pin : module_manager.module_port(gate_module, gate_module_input1_port_id).pins()) {
      module_manager.add_module_net_sink(lut_module, net, gate_module, gate_instance, gate_module_input1_port_id, gate_pin);
    }

    /* Create a module net for the output connection */
    ModuleNetId gate_output_net = module_manager.create_module_net(lut_module);
    ModulePortId gate_module_output_port_id = module_manager.find_module_port(gate_module, circuit_lib.port_lib_name(gate_output_ports[0]));
    VTR_ASSERT(true == module_manager.valid_module_port_id(gate_module, gate_module_output_port_id));
    BasicPort gate_module_output_port = module_manager.module_port(gate_module, gate_module_output_port_id);
    VTR_ASSERT(1 == gate_module_output_port.get_width());
    module_manager.add_module_net_source(lut_module, gate_output_net, gate_module, gate_instance, gate_module_output_port_id, gate_module_output_port.get_lsb());

    /* Update the output nets of the mode-select layer */
    mode_selected_nets.push_back(gate_output_net); 

    /* update the lsb of mode select port size */
    mode_select_port_lsb++;
  }

  /* Sanitity check */
  if ( true == circuit_lib.is_lut_fracturable(lut_model) ) {
    if (mode_select_port_lsb != circuit_lib.port_size(lut_mode_select_sram_ports[0])) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(FILE:%s,LINE[%d]) Circuit model LUT (name=%s) has a unmatched tri-state map (%s) implied by mode_port size(%d)!\n",
                 __FILE__, __LINE__, 
                 circuit_lib.model_name(lut_model).c_str(), 
                 tri_state_map.c_str(), 
                 circuit_lib.port_size(lut_mode_select_sram_ports[0])); 
      exit(1);
    }
  }

  /*********************************************** 
   * Child module addition: Input inverters
   ***********************************************/  
  /* Find the circuit model of the input inverter */
  CircuitModelId input_inverter_model = circuit_lib.lut_input_inverter_model(lut_model);
  VTR_ASSERT( CircuitModelId::INVALID() != input_inverter_model );

  std::vector<ModuleNetId> lut_mux_sram_inv_nets;
  /* Now we need to add inverters by instanciating the modules */
  for (size_t pin = 0; pin < circuit_lib.port_size(lut_input_ports[0]); ++pin) {
    ModuleNetId lut_mux_sram_inv_net = add_inverter_buffer_child_module_and_nets(module_manager, lut_module, 
                                                                                 circuit_lib, input_inverter_model,
                                                                                 mode_selected_nets[pin]); 
    /* Update the net vector */ 
    lut_mux_sram_inv_nets.push_back(lut_mux_sram_inv_net);
  }

  /*********************************************** 
   * Child module addition: Input buffers
   ***********************************************/  
  /* Add buffers to mode_select output ports  */
  /* Find the circuit model of the input inverter */
  CircuitModelId input_buffer_model = circuit_lib.lut_input_buffer_model(lut_model);
  VTR_ASSERT( CircuitModelId::INVALID() != input_buffer_model );

  std::vector<ModuleNetId> lut_mux_sram_nets;
  /* Now we need to add inverters by instanciating the modules and add module nets */
  for (size_t pin = 0; pin < circuit_lib.port_size(lut_input_ports[0]); ++pin) {
    ModuleNetId lut_mux_sram_net = add_inverter_buffer_child_module_and_nets(module_manager, lut_module, 
                                                                             circuit_lib, input_buffer_model,
                                                                             mode_selected_nets[pin]); 
    /* Update the net vector */ 
    lut_mux_sram_nets.push_back(lut_mux_sram_net);
  }

  /*********************************************** 
   * Child module addition: LUT MUX 
   ***********************************************/  
  /* Find the name of LUT MUX: no need to provide a mux size, just give an invalid number (=-1) */
  std::string lut_mux_module_name = generate_mux_subckt_name(circuit_lib, lut_model, size_t(-1), std::string(""));
  /* Find the module id of LUT MUX in the module manager */
  ModuleId lut_mux_module = module_manager.find_module(lut_mux_module_name);
  /* We must have a valid id */
  VTR_ASSERT (ModuleId::INVALID() != lut_mux_module);
  /* Instanciate a LUT MUX as child module */
  size_t lut_mux_instance = module_manager.num_instance(lut_module, lut_mux_module);
  module_manager.add_child_module(lut_module, lut_mux_module);

  /* TODO: Build module nets to connect
   * 1. SRAM ports of LUT MUX module to output ports of input buffer
   * 2. Inverted SRAM ports of LUT MUX module to output ports of input inverters
   * 3. Data input of LUT MUX module to SRAM port of LUT
   * 4. Data output of LUT MUX module to output ports of LUT
   */
  ModulePortId lut_mux_sram_port_id = module_manager.find_module_port(lut_mux_module, circuit_lib.port_lib_name(lut_regular_sram_ports[0]));
  BasicPort lut_mux_sram_port = module_manager.module_port(lut_mux_module, lut_mux_sram_port_id);
  VTR_ASSERT(lut_mux_sram_port.get_width() == lut_mux_sram_nets.size());
  /* Wire the port to lut_mux_sram_net */
  for (const size_t& pin : lut_mux_sram_port.pins()) {
    module_manager.add_module_net_sink(lut_module, lut_mux_sram_nets[pin], lut_mux_module, lut_mux_instance, lut_mux_sram_port_id, pin);
  } 

  ModulePortId lut_mux_sram_inv_port_id = module_manager.find_module_port(lut_mux_module, std::string(circuit_lib.port_lib_name(lut_regular_sram_ports[0]) + "_inv"));
  BasicPort lut_mux_sram_inv_port = module_manager.module_port(lut_mux_module, lut_mux_sram_inv_port_id);
  VTR_ASSERT(lut_mux_sram_inv_port.get_width() == lut_mux_sram_inv_nets.size());
  /* Wire the port to lut_mux_sram_net */
  for (const size_t& pin : lut_mux_sram_inv_port.pins()) {
    module_manager.add_module_net_sink(lut_module, lut_mux_sram_inv_nets[pin], lut_mux_module, lut_mux_instance, lut_mux_sram_inv_port_id, pin);
  } 
  
  /*           lut_module
   *          +------------
   *          |     +------
   *  sram -->|---->| (lut_mux_input_port)
   *          |  ^  |  LUT MUX
   *          |  |  |
   *             |
   *            net
   */  
  ModulePortId lut_sram_port_id = module_manager.find_module_port(lut_module, circuit_lib.port_lib_name(lut_regular_sram_ports[0]));
  BasicPort lut_sram_port = module_manager.module_port(lut_module, lut_sram_port_id);
  ModulePortId lut_mux_input_port_id = module_manager.find_module_port(lut_mux_module, circuit_lib.port_lib_name(lut_input_ports[0]));
  BasicPort lut_mux_input_port = module_manager.module_port(lut_mux_module, lut_mux_input_port_id);
  VTR_ASSERT(lut_mux_input_port.get_width() == lut_sram_port.get_width());
  /* Wire the port to lut_mux_sram_net */
  for (size_t pin_id = 0; pin_id < lut_mux_input_port.pins().size(); ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(lut_module);
    module_manager.add_module_net_source(lut_module, net, lut_module, 0, lut_sram_port_id, lut_sram_port.pins()[pin_id]);
    module_manager.add_module_net_sink(lut_module, net, lut_mux_module, lut_mux_instance, lut_mux_input_port_id, lut_mux_input_port.pins()[pin_id]);
  } 

  for (const auto& port : lut_output_ports) {
    ModulePortId lut_output_port_id = module_manager.find_module_port(lut_module, circuit_lib.port_lib_name(port));
    BasicPort lut_output_port = module_manager.module_port(lut_module, lut_output_port_id);
    ModulePortId lut_mux_output_port_id = module_manager.find_module_port(lut_mux_module, circuit_lib.port_lib_name(port));
    BasicPort lut_mux_output_port = module_manager.module_port(lut_mux_module, lut_mux_output_port_id);
    VTR_ASSERT(lut_mux_output_port.get_width() == lut_output_port.get_width());
    /* Wire the port to lut_mux_sram_net */
    for (size_t pin_id = 0; pin_id < lut_output_port.pins().size(); ++pin_id) {
      ModuleNetId net = module_manager.create_module_net(lut_module);
      module_manager.add_module_net_source(lut_module, net, lut_mux_module, lut_mux_instance, lut_mux_output_port_id, lut_mux_output_port.pins()[pin_id]);
      module_manager.add_module_net_sink(lut_module, net, lut_module, 0, lut_output_port_id, lut_output_port.pins()[pin_id]);
    } 
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, lut_module);
}

/******************************************************************** 
 * Print Verilog modules for the Look-Up Tables (LUTs) 
 * in the circuit library
 ********************************************************************/
void build_lut_modules(ModuleManager& module_manager,
                       const CircuitLibrary& circuit_lib) {
  /* Start time count */
  clock_t t_start = clock();

  vpr_printf(TIO_MESSAGE_INFO,
             "Building Look-Up Table (LUT) modules...");

  /* Search for each LUT circuit model */
  for (const auto& lut_model : circuit_lib.models()) {
    /* Bypas non-LUT modules */
    if (SPICE_MODEL_LUT != circuit_lib.model_type(lut_model)) {
      continue;
    }
    build_lut_module(module_manager, circuit_lib, lut_model);
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %.2g seconds\n", 
             run_time_sec);  
}

