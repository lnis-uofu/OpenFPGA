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
 * Build module of a routing track wire segment
 * Routing track wire, which is 1-input and dual output
 * This type of wires are used in the global routing architecture.
 * One of the output is wired to another Switch block multiplexer, 
 * while the mid-output is wired to a Connection block multiplexer.
 *     
 *                  |    CLB     |
 *                  +------------+
 *                        ^
 *                        |
 *           +------------------------------+
 *           | Connection block multiplexer |
 *           +------------------------------+
 *                        ^
 *                        |  mid-output         +--------------
 *              +--------------------+          |
 *    input --->| Routing track wire |--------->| Switch Block
 *              +--------------------+  output  |
 *                                              +--------------
 *    
 *******************************************************************/
static 
void build_routing_wire_module(ModuleManager& module_manager, 
                               const CircuitLibrary& circuit_lib,
                               const CircuitModelId& wire_model,
                               const std::string& wire_subckt_name) {
  /* Find the input port, output port*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(wire_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(wire_model, SPICE_MODEL_PORT_OUTPUT, true);

  /* Make sure the port size is what we want */
  VTR_ASSERT (1 == input_ports.size());
  VTR_ASSERT (1 == output_ports.size());
  VTR_ASSERT (1 == circuit_lib.port_size(input_ports[0]));
  VTR_ASSERT (1 == circuit_lib.port_size(output_ports[0]));

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId wire_module = add_circuit_model_to_module_manager(module_manager, circuit_lib, wire_model, wire_subckt_name); 

  /* Add a mid-output port to the module */
  BasicPort module_mid_output_port(generate_segment_wire_mid_output_name(circuit_lib.port_prefix(output_ports[0])), circuit_lib.port_size(output_ports[0]));
  module_manager.add_port(wire_module, module_mid_output_port, ModuleManager::MODULE_OUTPUT_PORT);
}

/********************************************************************
 * This function will only create wire modules with a number of 
 * ports that are defined by users.
 * It will NOT insert any internal logic, which should be handled 
 * by Verilog/SPICE writers
 *******************************************************************/
void build_wire_modules(ModuleManager& module_manager,
                        const CircuitLibrary& circuit_lib,
                        std::vector<t_segment_inf> routing_segments) {
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

  for (const auto& seg : routing_segments) {
    VTR_ASSERT( CircuitModelId::INVALID() != seg.circuit_model);
    VTR_ASSERT( SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(seg.circuit_model));
    /* Bypass user-defined circuit models */
    if ( (!circuit_lib.model_spice_netlist(seg.circuit_model).empty()) 
      && (!circuit_lib.model_verilog_netlist(seg.circuit_model).empty()) ) {
      continue;
    }
    /* Give a unique name for subckt of wire_model of segment, 
     * circuit_model name is unique, and segment id is unique as well
     */
    std::string segment_wire_subckt_name = generate_segment_wire_subckt_name(circuit_lib.model_name(seg.circuit_model), &seg - &routing_segments[0]);

    /* Print a Verilog module */
    build_routing_wire_module(module_manager, circuit_lib, seg.circuit_model, segment_wire_subckt_name);
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %.2g seconds\n", 
             run_time_sec);  
}
