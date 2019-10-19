/********************************************************************
 * This function includes the module builders for essential logic gates
 * which are the leaf circuit model in the circuit library
 *******************************************************************/
#include <vector>
#include "util.h"
#include "vtr_assert.h"

#include "fpga_x2p_naming.h"
#include "module_manager_utils.h"
#include "build_essential_modules.h"

/************************************************
 * Build a module of inverter or buffer 
 * or tapered buffer to a file 
 ***********************************************/
static 
void build_invbuf_module(ModuleManager& module_manager, 
                         const CircuitLibrary& circuit_lib,
                         const CircuitModelId& circuit_model) {
  /* Find the input port, output port and global inputs*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true, true);

  /* Make sure:
   * There is only 1 input port and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == input_ports.size()) && (1 == circuit_lib.port_size(input_ports[0])) );
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  /* TODO: move the check codes to check_circuit_library.h */
  /* If the circuit model is power-gated, we need to find at least one global config_enable signals */
  if (true == circuit_lib.is_power_gated(circuit_model)) { 
    /* Check all the ports we have are good for a power-gated circuit model */
    size_t num_err = 0;
    /* We need at least one global port */
    if (0 == global_ports.size())  {
      num_err++;
    }
    /* All the global ports should be config_enable */
    for (const auto& port : global_ports) {
      if (false == circuit_lib.port_is_config_enable(port)) {
        num_err++;
      }
    }
    /* Report errors if there are any */
    if (0 < num_err) {
      vpr_printf(TIO_MESSAGE_ERROR,
                 "Inverter/buffer circuit model (name=%s) is power-gated. At least one config-enable global port is required!\n",
                 circuit_lib.model_name(circuit_model).c_str()); 
      exit(1);
    }
  }

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = add_circuit_model_to_module_manager(module_manager, circuit_lib, circuit_model); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));
}

/************************************************
 * Build a module of a pass-gate,
 * either transmission-gate or pass-transistor
 ***********************************************/
static 
void build_passgate_module(ModuleManager& module_manager, 
                           const CircuitLibrary& circuit_lib,
                           const CircuitModelId& circuit_model) {
  /* Find the input port, output port*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true, true);

  switch (circuit_lib.pass_gate_logic_type(circuit_model)) {
  case SPICE_MODEL_PASS_GATE_TRANSMISSION:
    /* Make sure:
     * There is only 3 input port (in, sel, selb), 
     * each size of which is 1
     */
    VTR_ASSERT( 3 == input_ports.size() );
    for (const auto& input_port : input_ports) {
      VTR_ASSERT(1 == circuit_lib.port_size(input_port));
    }
    break;
  case SPICE_MODEL_PASS_GATE_TRANSISTOR:
    /* Make sure:
     * There is only 2 input port (in, sel), 
     * each size of which is 1
     */
    VTR_ASSERT( 2 == input_ports.size() );
    for (const auto& input_port : input_ports) {
      VTR_ASSERT(1 == circuit_lib.port_size(input_port));
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid topology for circuit model (name=%s)!\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model).c_str());
    exit(1);
  }

  /* Make sure:
   * There is only 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = add_circuit_model_to_module_manager(module_manager, circuit_lib, circuit_model); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));
}

/************************************************
 * Build a module of a logic gate
 * which are standard cells
 * Supported gate types: 
 * 1. N-input AND 
 * 2. N-input OR
 * 3. 2-input MUX
 ***********************************************/
static 
void build_gate_module(ModuleManager& module_manager, 
                       const CircuitLibrary& circuit_lib,
                       const CircuitModelId& circuit_model) {
  /* Find the input port, output port*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true, true);

  /* Make sure:
   * There is only 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = add_circuit_model_to_module_manager(module_manager, circuit_lib, circuit_model); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));
}


/************************************************
 * Generate the modules for essential gates
 * include inverters, buffers, transmission-gates,
 * etc.
 ***********************************************/
void build_essential_modules(ModuleManager& module_manager, 
                             const CircuitLibrary& circuit_lib) {

  for (const auto& circuit_model : circuit_lib.models()) {
    if (SPICE_MODEL_INVBUF == circuit_lib.model_type(circuit_model)) {
      build_invbuf_module(module_manager, circuit_lib, circuit_model);
      continue;
    }
    if (SPICE_MODEL_PASSGATE == circuit_lib.model_type(circuit_model)) {
      build_passgate_module(module_manager, circuit_lib, circuit_model);
      continue;
    }
    if (SPICE_MODEL_GATE == circuit_lib.model_type(circuit_model)) {
      build_gate_module(module_manager, circuit_lib, circuit_model);
      continue;
    }
  }
}

/*********************************************************************
 * Register all the user-defined modules in the module manager
 * Walk through the circuit library and add user-defined circuit models
 * to the module_manager
 ********************************************************************/
void build_user_defined_modules(ModuleManager& module_manager, 
                                const CircuitLibrary& circuit_lib, 
                                const std::vector<t_segment_inf>& routing_segments) {
  /* Iterate over Verilog modules */
  for (const auto& model : circuit_lib.models()) {
    /* We only care about user-defined models */
    if ( (true == circuit_lib.model_verilog_netlist(model).empty())
      && (true == circuit_lib.model_verilog_netlist(model).empty()) ) {
      continue;
    }
    /* Skip Routing channel wire models because they need a different name. Do it later */
    if (SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) {
      continue;
    }
    /* Reach here, the model requires a user-defined Verilog netlist, 
     * Register it in the module_manager  
     */
    add_circuit_model_to_module_manager(module_manager, circuit_lib, model);
  }

  /* Register the routing channel wires  */
  for (const auto& seg : routing_segments) {
    VTR_ASSERT( CircuitModelId::INVALID() != seg.circuit_model);
    VTR_ASSERT( SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(seg.circuit_model));
    /* We care only user-defined circuit models */
    if ( (circuit_lib.model_verilog_netlist(seg.circuit_model).empty()) 
      && (circuit_lib.model_verilog_netlist(seg.circuit_model).empty()) ) {
      continue;
    }
    /* Give a unique name for subckt of wire_model of segment, 
     * circuit_model name is unique, and segment id is unique as well
     */
    std::string segment_wire_subckt_name = generate_segment_wire_subckt_name(circuit_lib.model_name(seg.circuit_model), &seg - &routing_segments[0]);

    /* Create a Verilog Module based on the circuit model, and add to module manager */
    ModuleId module_id = add_circuit_model_to_module_manager(module_manager, circuit_lib, seg.circuit_model, segment_wire_subckt_name); 

    /* Find the output port*/
    std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(seg.circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
    /* Make sure the port size is what we want */
    VTR_ASSERT (1 == circuit_lib.port_size(output_ports[0]));
  
    /* Add a mid-output port to the module */
    BasicPort module_mid_output_port(generate_segment_wire_mid_output_name(circuit_lib.port_lib_name(output_ports[0])), circuit_lib.port_size(output_ports[0]));
    module_manager.add_port(module_id, module_mid_output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }
}

