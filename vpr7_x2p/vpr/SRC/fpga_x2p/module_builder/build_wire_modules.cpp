/***********************************************
 * This file includes functions to generate
 * Verilog submodules for wires.
 **********************************************/
#include <ctime>
#include <string>
#include <algorithm>

#include "util.h"
#include "vtr_assert.h"

/* Device-level header files */
#include "module_manager.h"
#include "module_manager_utils.h"
#include "physical_types.h"
#include "vpr_types.h"

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "build_wire_modules.h"

/********************************************************************
 * Print a Verilog module of a regular wire segment
 * Regular wire, which is 1-input and 1-output
 * This type of wires are used in the local routing architecture
 *              +------+
 *    input --->| wire |---> output
 *              +------+
 *
 *******************************************************************/
static 
void build_wire_module(ModuleManager& module_manager, 
                       const CircuitLibrary& circuit_lib,
                       const CircuitModelId& wire_model) {
  /* Find the input port, output port*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(wire_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(wire_model, SPICE_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(wire_model, SPICE_MODEL_PORT_INPUT, true, true);

  /* Makre sure the port size is what we want */
  VTR_ASSERT (1 == input_ports.size());
  VTR_ASSERT (1 == output_ports.size());
  VTR_ASSERT (1 == circuit_lib.port_size(input_ports[0]));
  VTR_ASSERT (1 == circuit_lib.port_size(output_ports[0]));

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  add_circuit_model_to_module_manager(module_manager, circuit_lib, wire_model); 
}

/********************************************************************
 * This function will only create wire modules with a number of 
 * ports that are defined by users.
 * It will NOT insert any internal logic, which should be handled 
 * by Verilog/SPICE writers
 *******************************************************************/
void build_wire_modules(ModuleManager& module_manager,
                        const CircuitLibrary& circuit_lib) {
  /* Start time count */
  clock_t t_start = clock();

  vpr_printf(TIO_MESSAGE_INFO,
             "Building wire modules...");

  /* Print Verilog models for regular wires*/
  for (const auto& wire_model : circuit_lib.models_by_type(SPICE_MODEL_WIRE)) { 
    /* Bypass user-defined circuit models */
    if ( (!circuit_lib.model_spice_netlist(wire_model).empty()) 
      && (!circuit_lib.model_verilog_netlist(wire_model).empty()) ) {
      continue;
    }
    build_wire_module(module_manager, circuit_lib, wire_model);
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %.2g seconds\n", 
             run_time_sec);  
}
