/************************************************
 * This file includes most utilized functions for 
 * generating Verilog sub-modules
 * such as timing matrix and signal initialization
 ***********************************************/
#include <fstream>
#include <limits>
#include <iomanip>
#include "vtr_assert.h"

/* Device-level header files */
#include "spice_types.h"
#include "device_port.h"

/* FPGA-X2P context header files */
#include "fpga_x2p_utils.h"
#include "module_manager_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_submodule_utils.h"

/* All values are printed with this precision value. The higher the
 * value, the more accurate timing assignment is. Using a number of 6
 * guarentees that a precision of femtosecond which is sufficent for 
 * electrical simulation (simulation timescale is 10-9 
 */
/* constexpr int FLOAT_PRECISION = std::numeric_limits<float>::max_digits10; */
constexpr int FLOAT_PRECISION = 6; 

/************************************************
 * Print a timing matrix defined in theecircuit model
 * into a Verilog format. 
 * This function print all the timing edges available
 * in the circuit model (any pin-to-pin delay)
 ***********************************************/
void print_verilog_submodule_timing(std::fstream& fp, 
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model) {
  /* return if there is no delay info */
  if ( 0 == circuit_lib.num_delay_info(circuit_model)) {
    return;
  }

  /* Return if there is no ports */
  if (0 == circuit_lib.num_model_ports(circuit_model)) {
    return;
  }

  /* Ensure a valid file handler*/
  check_file_handler(fp);

  fp << std::endl;
  fp << "`ifdef " << verilog_timing_preproc_flag << std::endl;
  print_verilog_comment(fp, std::string("------ BEGIN Pin-to-pin Timing constraints -----"));
  fp << "\tspecify" << std::endl;

  /* Read out pin-to-pin delays by finding out all the edges belonging to a circuit model */
  for (const auto& timing_edge : circuit_lib.timing_edges_by_model(circuit_model)) {
     CircuitPortId src_port = circuit_lib.timing_edge_src_port(timing_edge);
     size_t src_pin = circuit_lib.timing_edge_src_pin(timing_edge);
     BasicPort src_port_info(circuit_lib.port_lib_name(src_port), src_pin, src_pin); 

     CircuitPortId sink_port = circuit_lib.timing_edge_sink_port(timing_edge);
     size_t sink_pin = circuit_lib.timing_edge_sink_pin(timing_edge);
     BasicPort sink_port_info(circuit_lib.port_lib_name(sink_port), sink_pin, sink_pin); 
   
     fp << "\t\t";
     fp << "(" << generate_verilog_port(VERILOG_PORT_CONKT, src_port_info);
     fp << " => ";
     fp << generate_verilog_port(VERILOG_PORT_CONKT, sink_port_info) << ")";
     fp << " = ";
     fp << "(" << std::setprecision(FLOAT_PRECISION) << circuit_lib.timing_edge_delay(timing_edge, SPICE_MODEL_DELAY_RISE) / verilog_sim_timescale;
     fp << ", ";
     fp << std::setprecision(FLOAT_PRECISION) << circuit_lib.timing_edge_delay(timing_edge, SPICE_MODEL_DELAY_FALL) / verilog_sim_timescale << ")";
     fp << ";" << std::endl;
  }

  fp << "\tendspecify" << std::endl;
  print_verilog_comment(fp, std::string("------ END Pin-to-pin Timing constraints -----"));
  fp << "`endif" << std::endl;

}

void print_verilog_submodule_signal_init(std::fstream& fp, 
                                         const CircuitLibrary& circuit_lib,
                                         const CircuitModelId& circuit_model) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  fp << std::endl;
  fp << "`ifdef " << verilog_signal_init_preproc_flag << std::endl;
  print_verilog_comment(fp, std::string("------ BEGIN driver initialization -----"));
  fp << "\tinitial begin" << std::endl;
  fp << "\t`ifdef " << verilog_formal_verification_preproc_flag << std::endl;

  /* Only for formal verification: deposite a zero signal values */
  /* Initialize each input port */
  for (const auto& input_port : circuit_lib.model_input_ports(circuit_model)) {
    BasicPort input_port_info(circuit_lib.port_lib_name(input_port), circuit_lib.port_size(input_port));
    fp << "\t\t$deposit(";
    fp << generate_verilog_port(VERILOG_PORT_CONKT, input_port_info);
    fp << ", " <<  circuit_lib.port_size(input_port) << "'b" << std::string(circuit_lib.port_size(input_port), '0');
    fp << ");" << std::endl;
  }
  fp << "\t`else" << std::endl;

  /* Regular case: deposite initial signal values: a random value */
  for (const auto& input_port : circuit_lib.model_input_ports(circuit_model)) {
    BasicPort input_port_info(circuit_lib.port_lib_name(input_port), circuit_lib.port_size(input_port));
    fp << "\t\t$deposit(";
    fp << generate_verilog_port(VERILOG_PORT_CONKT, input_port_info);
    fp << ", $random);" << std::endl;
  }

  fp << "\t`endif\n" << std::endl;
  fp << "\tend" << std::endl;
  print_verilog_comment(fp, std::string("------ END driver initialization -----"));
  fp << "`endif" << std::endl;
}

/*********************************************************************
 * Register all the user-defined modules in the module manager
 * Walk through the circuit library and add user-defined circuit models
 * to the module_manager
 ********************************************************************/
void add_user_defined_verilog_modules(ModuleManager& module_manager, 
                                      const CircuitLibrary& circuit_lib) {
  /* Iterate over Verilog modules */
  for (const auto& model : circuit_lib.models()) {
    /* We only care about user-defined models */
    if (true == circuit_lib.model_verilog_netlist(model).empty()) {
      continue;
    }
    /* Skip Routing channel wire models because they need a different name. Do it later */
    if (SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) {
      continue;
    }
    /* Reach here, the model requires a user-defined Verilog netlist, 
     * Try to find it in the module manager 
     * If not found, register it in the module_manager  
     */
    ModuleId module_id = module_manager.find_module(circuit_lib.model_name(model));
    if (ModuleId::INVALID() == module_id) {
      add_circuit_model_to_module_manager(module_manager, circuit_lib, model);
    }
  }
}

/*********************************************************************
 * Print a template for a user-defined circuit model
 * The template will include just the port declaration of the Verilog module
 * The template aims to help user to write Verilog codes with a guaranteed
 * module definition, which can be correctly instanciated (with correct
 * port mapping) in the FPGA fabric
 ********************************************************************/
static 
void print_one_verilog_template_module(const ModuleManager& module_manager,
                                       std::fstream& fp,
                                       const std::string& module_name) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Template Verilog module for " + module_name + " -----"));

  /* Find the module in module manager, which should be already registered */
  /* TODO: routing channel wire model may have a different name! */
  ModuleId template_module = module_manager.find_module(module_name);
  VTR_ASSERT(ModuleId::INVALID() != template_module);

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, template_module);
  /* Finish dumping ports */

  print_verilog_comment(fp, std::string("----- Internal logic should start here -----"));

  /* Add some empty lines as placeholders for the internal logic*/
  fp << std::endl << std::endl;
 
  print_verilog_comment(fp, std::string("----- Internal logic should end here -----"));

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/*********************************************************************
 * Print a template of all the submodules that are user-defined
 * The template will include just the port declaration of the submodule
 * The template aims to help user to write Verilog codes with a guaranteed
 * module definition, which can be correctly instanciated (with correct
 * port mapping) in the FPGA fabric
 ********************************************************************/
void print_verilog_submodule_templates(const ModuleManager& module_manager,
                                       const CircuitLibrary& circuit_lib,
                                       const std::string& verilog_dir,
                                       const std::string& submodule_dir) {
  std::string verilog_fname(submodule_dir + user_defined_template_verilog_file_name);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Print out debugging information for if the file is not opened/created properly */
  vpr_printf(TIO_MESSAGE_INFO,
             "Creating template for user-defined Verilog modules (%s)...\n",
             verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Template for user-defined Verilog modules"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Output essential models*/
  for (const auto& model : circuit_lib.models()) {
    /* Focus on user-defined modules, which must have a Verilog netlist defined */
    if (circuit_lib.model_verilog_netlist(model).empty()) {
      continue;
    }
    /* Skip Routing channel wire models because they need a different name. Do it later */
    if (SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) {
      continue;
    }
    /* Print a Verilog template for the circuit model */
    print_one_verilog_template_module(module_manager, fp, circuit_lib.model_name(model)); 
  }

  /* close file stream */
  fp.close();
 
  /* No need to add the template to the subckt include files! */
}
