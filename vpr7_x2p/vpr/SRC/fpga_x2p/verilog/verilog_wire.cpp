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
  ModuleId wire_module = module_manager.find_module(circuit_lib.model_name(wire_model)); 
  VTR_ASSERT(true == module_manager.valid_module_id(wire_module));

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, wire_module);
  /* Finish dumping ports */

  /* Print the internal logic of Verilog module */
  /* Find the input port of the module */
  ModulePortId module_input_port_id = module_manager.find_module_port(wire_module, circuit_lib.port_lib_name(input_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_input_port_id);
  BasicPort module_input_port = module_manager.module_port(wire_module, module_input_port_id);

  /* Find the output port of the module */
  ModulePortId module_output_port_id = module_manager.find_module_port(wire_module, circuit_lib.port_lib_name(output_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_output_port_id);
  BasicPort module_output_port = module_manager.module_port(wire_module, module_output_port_id);

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
 * Top-level function to print wire modules
 *******************************************************************/
void print_verilog_submodule_wires(ModuleManager& module_manager,
                                   const CircuitLibrary& circuit_lib,
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

  /* Close the file stream */
  fp.close();

  /* Add fname to the linked list */
  /* Uncomment this when it is ready to be plugged in
   */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_fname.c_str());  

  return;
}
