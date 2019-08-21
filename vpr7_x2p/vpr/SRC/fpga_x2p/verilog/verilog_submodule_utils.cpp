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

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_submodule_utils.h"

/* All values are printed with this precision value. The higher the
 * value, the more accurate timing assignment is. Using numeric_limits
 * max_digits10 guarentees that no values change during a sequence of
 * float -> string -> float conversions */
constexpr int FLOAT_PRECISION = std::numeric_limits<float>::max_digits10;

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
     fp << "(" << std::setprecision(FLOAT_PRECISION) << circuit_lib.timing_edge_delay(timing_edge, SPICE_MODEL_DELAY_RISE);
     fp << " , ";
     fp << std::setprecision(FLOAT_PRECISION) << circuit_lib.timing_edge_delay(timing_edge, SPICE_MODEL_DELAY_RISE) << ")";
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
    fp << "\t\t$deposit(" << circuit_lib.port_lib_name(input_port) << ", 1'b0);" << std::endl;
  }
  fp << "\t`else" << std::endl;

  /* Regular case: deposite initial signal values: a random value */
  for (const auto& input_port : circuit_lib.model_input_ports(circuit_model)) {
    fp << "\t\t$deposit(" << circuit_lib.port_lib_name(input_port) << ", $random);" << std::endl;
  }

  fp << "\t`endif\n" << std::endl;
  fp << "\tend" << std::endl;
  print_verilog_comment(fp, std::string("------ END driver initialization -----"));
  fp << "`endif" << std::endl;
}

