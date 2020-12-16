/********************************************************************
 * This function includes the module builders for essential logic gates
 * which are the leaf circuit model in the circuit library
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

#include "openfpga_naming.h"
#include "module_manager_utils.h"
#include "build_essential_modules.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************
 * Build a module of inverter or buffer 
 * or tapered buffer to a file 
 ***********************************************/
static 
void build_invbuf_module(ModuleManager& module_manager, 
                         const CircuitLibrary& circuit_lib,
                         const CircuitModelId& circuit_model) {
  /* Find the input port, output port and global inputs*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);

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
      VTR_LOGF_ERROR(__FILE__, __LINE__, "Inverter/buffer circuit model '%s' is power-gated. At least one config-enable global port is required!\n",
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
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);

  switch (circuit_lib.pass_gate_logic_type(circuit_model)) {
  case CIRCUIT_MODEL_PASS_GATE_TRANSMISSION:
    /* Make sure:
     * There is only 3 input port (in, sel, selb), 
     * each size of which is 1
     */
    VTR_ASSERT( 3 == input_ports.size() );
    for (const auto& input_port : input_ports) {
      VTR_ASSERT(1 == circuit_lib.port_size(input_port));
    }
    break;
  case CIRCUIT_MODEL_PASS_GATE_TRANSISTOR:
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
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid topology for circuit model '%s'!\n",
                  circuit_lib.model_name(circuit_model).c_str());
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
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);

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
  vtr::ScopedStartFinishTimer timer("Build essential (inverter/buffer/logic gate) modules");

  for (const auto& circuit_model : circuit_lib.models()) {
    /* Add essential modules upon on demand: only when it is not yet in the module library */
    ModuleId module = module_manager.find_module(circuit_lib.model_name(circuit_model));
    if (true == module_manager.valid_module_id(module)) {
      continue;
    }

    if (CIRCUIT_MODEL_INVBUF == circuit_lib.model_type(circuit_model)) {
      build_invbuf_module(module_manager, circuit_lib, circuit_model);
      continue;
    }
    if (CIRCUIT_MODEL_PASSGATE == circuit_lib.model_type(circuit_model)) {
      build_passgate_module(module_manager, circuit_lib, circuit_model);
      continue;
    }
    if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(circuit_model)) {
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
                                const CircuitLibrary& circuit_lib) {
  vtr::ScopedStartFinishTimer timer("Build user-defined modules");

  /* Iterate over Verilog modules */
  for (const auto& model : circuit_lib.models()) {
    /* We only care about user-defined models */
    if ( (true == circuit_lib.model_verilog_netlist(model).empty())
      && (true == circuit_lib.model_spice_netlist(model).empty()) ) {
      continue;
    }
    /* Skip Routing channel wire models because they need a different name. Do it later */
    if (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) {
      continue;
    }
    /* Reach here, the model requires a user-defined Verilog netlist, 
     * Register it in the module_manager  
     */
    add_circuit_model_to_module_manager(module_manager, circuit_lib, model);
  }
}

/*********************************************************************
 * This function will build a constant generator modules 
 * and add it to the module manager
 * It could be either
 * 1. VDD or 2. GND
 * Each module will have only one output port
 ********************************************************************/
static 
void build_constant_generator_module(ModuleManager& module_manager, 
                                     const size_t& const_value) {
  ModuleId const_module = module_manager.add_module(generate_const_value_module_name(const_value));
  /* Add one output port */
  BasicPort const_output_port(generate_const_value_module_output_port_name(const_value), 1);
  module_manager.add_port(const_module, const_output_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Specify the usage of this module 
   * 1: VDD
   * 0: GND
   */
  if (1 == const_value) {
    module_manager.set_module_usage(const_module, ModuleManager::MODULE_VDD);
  } else {
    VTR_ASSERT(0 == const_value);
    module_manager.set_module_usage(const_module, ModuleManager::MODULE_VSS);
  }
}

/*********************************************************************
 * This function will add two constant generator modules 
 * to the module manager
 * 1. VDD
 * 2. GND
 ********************************************************************/
void build_constant_generator_modules(ModuleManager& module_manager) {
  vtr::ScopedStartFinishTimer timer("Build constant generator modules");

  /* VDD */
  build_constant_generator_module(module_manager, 1);

  /* GND */
  build_constant_generator_module(module_manager, 0);
}

/*********************************************************************
 * This function will rename the ports of primitive modules 
 * using lib_name instead of prefix
 * Primitive modules are defined as those modules in the module manager 
 * which have user defined netlists
 ********************************************************************/
void rename_primitive_module_port_names(ModuleManager& module_manager, 
                                        const CircuitLibrary& circuit_lib) {
  for (const CircuitModelId& model : circuit_lib.models()) {
    /* We only care about user-defined models */
    if ( (true == circuit_lib.model_verilog_netlist(model).empty())
      && (true == circuit_lib.model_spice_netlist(model).empty()) ) {
      /* Exception circuit models as primitive cells
       * - Inverter, buffer, pass-gate logic, logic gate
       * which should be renamed even when auto-generated
       */
      if ( (CIRCUIT_MODEL_INVBUF != circuit_lib.model_type(model))
          && (CIRCUIT_MODEL_PASSGATE != circuit_lib.model_type(model))
          && (CIRCUIT_MODEL_GATE != circuit_lib.model_type(model)) ) {
        continue;
      }
    }
    /* Skip Routing channel wire models because they need a different name. Do it later */
    if (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) {
      continue;
    }
    /* Find the module in module manager */
    ModuleId module = module_manager.find_module(circuit_lib.model_name(model));
    /* We must find one! */
    VTR_ASSERT(true == module_manager.valid_module_id(module));

    /* Rename all the ports to use lib_name! */
    for (const CircuitPortId& model_port : circuit_lib.model_ports(model)) {
      /* Find the module port in module manager. We used prefix when creating the ports */
      ModulePortId module_port = module_manager.find_module_port(module, circuit_lib.port_prefix(model_port));
      /* We must find one! */
      VTR_ASSERT(true == module_manager.valid_module_port_id(module, module_port));
      /* Name it with lib_name */
      module_manager.set_module_port_name(module, module_port, circuit_lib.port_lib_name(model_port));
    }
  }
}

} /* end namespace openfpga */
