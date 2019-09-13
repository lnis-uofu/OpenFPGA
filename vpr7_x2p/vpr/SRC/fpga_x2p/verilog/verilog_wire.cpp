/***********************************************
 * This file includes functions to generate
 * Verilog submodules for wires.
 **********************************************/
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
#include "verilog_submodule_utils.h"
#include "verilog_writer_utils.h"
#include "verilog_wire.h"

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
void print_verilog_wire_module(ModuleManager& module_manager, 
                               const CircuitLibrary& circuit_lib,
                               std::fstream& fp,
                               const CircuitModelId& wire_model) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

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
  ModuleId module_id = add_circuit_model_to_module_manager(module_manager, circuit_lib, wire_model); 

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish dumping ports */

  /* Print the internal logic of Verilog module */
  /* Find the input port of the module */
  ModulePortId module_input_port_id = module_manager.find_module_port(module_id, circuit_lib.port_lib_name(input_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_input_port_id);
  BasicPort module_input_port = module_manager.module_port(module_id, module_input_port_id);

  /* Find the output port of the module */
  ModulePortId module_output_port_id = module_manager.find_module_port(module_id, circuit_lib.port_lib_name(output_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_output_port_id);
  BasicPort module_output_port = module_manager.module_port(module_id, module_output_port_id);

  /* Print wire declaration for the inputs and outputs */
  fp << generate_verilog_port(VERILOG_PORT_WIRE, module_input_port) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_WIRE, module_output_port) << ";" << std::endl;

  /* Direct shortcut */
  print_verilog_wire_connection(fp, module_output_port, module_input_port, false);
  
  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, wire_model);
   
  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(wire_model));

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Print a Verilog module of a routing track wire segment
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
void print_verilog_routing_wire_module(ModuleManager& module_manager, 
                                       const CircuitLibrary& circuit_lib,
                                       std::fstream& fp,
                                       const CircuitModelId& wire_model,
                                       const std::string& wire_subckt_name) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  /* Find the input port, output port*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(wire_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(wire_model, SPICE_MODEL_PORT_OUTPUT, true);

  /* Makre sure the port size is what we want */
  VTR_ASSERT (1 == input_ports.size());
  VTR_ASSERT (1 == output_ports.size());
  VTR_ASSERT (1 == circuit_lib.port_size(input_ports[0]));
  VTR_ASSERT (1 == circuit_lib.port_size(output_ports[0]));

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = add_circuit_model_to_module_manager(module_manager, circuit_lib, wire_model, wire_subckt_name); 

  /* Add a mid-output port to the module */
  BasicPort module_mid_output_port(generate_segment_wire_mid_output_name(circuit_lib.port_lib_name(output_ports[0])), circuit_lib.port_size(output_ports[0]));
  module_manager.add_port(module_id, module_mid_output_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish dumping ports */

  /* Print the internal logic of Verilog module */
  /* Find the input port of the module */
  ModulePortId module_input_port_id = module_manager.find_module_port(module_id, circuit_lib.port_lib_name(input_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_input_port_id);
  BasicPort module_input_port = module_manager.module_port(module_id, module_input_port_id);

  /* Find the output port of the module */
  ModulePortId module_output_port_id = module_manager.find_module_port(module_id, circuit_lib.port_lib_name(output_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_output_port_id);
  BasicPort module_output_port = module_manager.module_port(module_id, module_output_port_id);

  /* Print wire declaration for the inputs and outputs */
  fp << generate_verilog_port(VERILOG_PORT_WIRE, module_input_port) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_WIRE, module_output_port) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_WIRE, module_mid_output_port) << ";" << std::endl;

  /* Direct shortcut */
  print_verilog_wire_connection(fp, module_output_port, module_input_port, false);
  print_verilog_wire_connection(fp, module_mid_output_port, module_input_port, false);

  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, wire_model);
   
  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(wire_model));

  /* Add an empty line as a splitter */
  fp << std::endl;
}

void print_verilog_submodule_wires(ModuleManager& module_manager,
                                   const CircuitLibrary& circuit_lib,
                                   std::vector<t_segment_inf> routing_segments,
                                   const std::string& verilog_dir,
                                   const std::string& submodule_dir) {
  std::string verilog_fname(submodule_dir + wires_verilog_file_name);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Print out debugging information for if the file is not opened/created properly */
  vpr_printf(TIO_MESSAGE_INFO,
             "Creating Verilog netlist for wires (%s)...\n",
             verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Wires"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Print Verilog models for regular wires*/
  print_verilog_comment(fp, std::string("----- BEGIN Verilog modules for regular wires -----"));
  for (const auto& model : circuit_lib.models_by_type(SPICE_MODEL_WIRE)) { 
    /* Bypass user-defined circuit models */
    if (!circuit_lib.model_verilog_netlist(model).empty()) {
      continue;
    }
    print_verilog_wire_module(module_manager, circuit_lib, fp, model);
  }
  print_verilog_comment(fp, std::string("----- END Verilog modules for regular wires -----"));
 
  /* Create wire models for routing segments*/
  print_verilog_comment(fp, std::string("----- BEGIN Verilog modules for routing track wires -----"));

  for (const auto& seg : routing_segments) {
    VTR_ASSERT( CircuitModelId::INVALID() != seg.circuit_model);
    VTR_ASSERT( SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(seg.circuit_model));
    /* Bypass user-defined circuit models */
    if (!circuit_lib.model_verilog_netlist(seg.circuit_model).empty()) {
      continue;
    }
    /* Give a unique name for subckt of wire_model of segment, 
     * circuit_model name is unique, and segment id is unique as well
     */
    std::string segment_wire_subckt_name = generate_segment_wire_subckt_name(circuit_lib.model_name(seg.circuit_model), &seg - &routing_segments[0]);

    /* Print a Verilog module */
    print_verilog_routing_wire_module(module_manager, circuit_lib, fp, seg.circuit_model, segment_wire_subckt_name);
  }
  print_verilog_comment(fp, std::string("----- END Verilog modules for routing track wires -----"));

  /* Close the file stream */
  fp.close();

  /* Add fname to the linked list */
  /* Uncomment this when it is ready to be plugged in
   */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_fname.c_str());  

  return;
}
