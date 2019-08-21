/***********************************************
 * This file includes functions to generate
 * Verilog submodules for multiplexers.
 * including both fundamental submodules
 * such as a branch in a multiplexer 
 * and the full multiplexer
 **********************************************/
#include <string>

#include "util.h"
#include "vtr_assert.h"

/* Device-level header files */
#include "mux_graph.h"
#include "physical_types.h"
#include "vpr_types.h"

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_submodule_mux.h"

/***********************************************
 * Generate Verilog codes modeling an branch circuit 
 * for a multiplexer with the given size 
 **********************************************/
static 
void generate_verilog_cmos_mux_branch_module_structural(std::fstream& fp,
                                                        const CircuitLibrary& circuit_lib, 
                                                        const CircuitModelId& circuit_model, 
                                                        const std::string& module_name, 
                                                        const MuxGraph& mux_graph) {
  /* Get the tgate model */
  CircuitModelId tgate_model = circuit_lib.pass_gate_logic_model(circuit_model);

  /* Skip output if the tgate model is a MUX2, it is handled by essential-gate generator */
  if (SPICE_MODEL_GATE == circuit_lib.model_type(tgate_model)) {
    VTR_ASSERT(SPICE_MODEL_GATE_MUX2 == circuit_lib.gate_type(tgate_model));
    return;
  }

  /* Get model ports of tgate */
  std::vector<CircuitPortId> tgate_input_ports = circuit_lib.model_ports_by_type(tgate_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> tgate_output_ports = circuit_lib.model_ports_by_type(tgate_model, SPICE_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> tgate_global_ports = circuit_lib.model_global_ports_by_type(tgate_model, SPICE_MODEL_PORT_INPUT);
  VTR_ASSERT(3 == tgate_input_ports.size());
  VTR_ASSERT(1 == tgate_output_ports.size());

  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

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

  /* Comment lines */
  fp << "//---- Structural Verilog for CMOS MUX basis module:" << module_name << "-----" << std::endl;

  /* Print the port list and definition */
  fp << "module " << module_name << "(" << std::endl;

  /* Create port information */
  BasicPort input_port;
  /* Configure each input port */
  input_port.set_name(std::string("in"));
  input_port.set_width(num_inputs);

  BasicPort output_port;
  /* Configure each input port */
  output_port.set_name(std::string("out"));
  output_port.set_width(num_outputs);

  BasicPort mem_port;
  /* Configure each input port */
  mem_port.set_name(std::string("mem"));
  mem_port.set_width(num_mems);

  BasicPort mem_inv_port;
  /* Configure each input port */
  mem_inv_port.set_name(std::string("mem_inv"));
  mem_inv_port.set_width(num_mems);

  /* TODO: Generate global ports */
  for (const auto& port : tgate_global_ports) {
    BasicPort basic_port;
    /* Configure each input port */
    basic_port.set_name(circuit_lib.port_prefix(port));
    basic_port.set_width(circuit_lib.port_size(port));
    /* Print port */
    fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, basic_port) << "," << std::endl;
  }

  /* TODO: add a module to the Module Manager */

  /* Port list */
  fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, input_port) << "," << std::endl;
  fp << "\t" << generate_verilog_port(VERILOG_PORT_OUTPUT, output_port) << "," << std::endl;
  fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, mem_port) << "," << std::endl;
  fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, mem_inv_port) << std::endl;
  fp << ");" << std::endl;

  /* Verilog Behavior description for a MUX */
  fp << "//---- Structure-level description -----" << std::endl;
  /* Special case: only one memory, switch case is simpler 
   * When mem = 1, propagate input 0; 
   * when mem = 0, propagate input 1;
   */
  if (1 == num_mems) {
    /* Transmission gates are connected to each input and also the output*/
    fp << "\t" << circuit_lib.model_name(tgate_model) << " " << circuit_lib.model_prefix(tgate_model) << "_0 ";
    /* Dump explicit port map if required */
    /* TODO: add global port support for tgate model */
    if (true == circuit_lib.dump_explicit_port_map(tgate_model)) {
      fp << " (";
      fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[0]) << "(" << "in[0]" << "),";
      fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[1]) << "(" << generate_verilog_port(VERILOG_PORT_CONKT, mem_port) << "),";
      fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[2]) << "(" << generate_verilog_port(VERILOG_PORT_CONKT, mem_inv_port) << "),";
      fp << "  ." << circuit_lib.port_lib_name(tgate_output_ports[0]) << "(" << generate_verilog_port(VERILOG_PORT_CONKT, output_port) << ")";
      fp << ");" << std::endl;
    } else {
      fp << " (";
      fp << generate_verilog_port(VERILOG_PORT_CONKT, input_port);
      fp << ", " << generate_verilog_port(VERILOG_PORT_CONKT, mem_port);
      fp << ", " << generate_verilog_port(VERILOG_PORT_CONKT, mem_inv_port);
      fp << ", " <<  generate_verilog_port(VERILOG_PORT_CONKT, output_port);
      fp << ");" << std::endl;
    }
    /* Transmission gates are connected to each input and also the output*/
    fp << "\t" << circuit_lib.model_name(tgate_model) << " " << circuit_lib.model_prefix(tgate_model) << "_1 ";
    /* Dump explicit port map if required */
    if (true == circuit_lib.dump_explicit_port_map(tgate_model)) {
      fp << " (";
      fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[0]) << "(" << "in[1]" << "),";
      fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[1]) << "(" << generate_verilog_port(VERILOG_PORT_CONKT, mem_inv_port) << "),";
      fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[2]) << "(" << generate_verilog_port(VERILOG_PORT_CONKT, mem_port) << "),";
      fp << "  ." << circuit_lib.port_lib_name(tgate_output_ports[0]) << "(" << generate_verilog_port(VERILOG_PORT_CONKT, output_port) << ")";
      fp << ");" << std::endl;
    } else {
      fp << " (";
      fp << generate_verilog_port(VERILOG_PORT_CONKT, input_port);
      fp << ", " << generate_verilog_port(VERILOG_PORT_CONKT, mem_inv_port);
      fp << ", " << generate_verilog_port(VERILOG_PORT_CONKT, mem_port);
      fp << ", " <<  generate_verilog_port(VERILOG_PORT_CONKT, output_port);
      fp << ");" << std::endl;
    }
  } else {
  /* Other cases, we need to follow the rules:
   * When mem[k] is enabled, switch on input[k]
   * Only one memory bit is enabled!
   */
    for (size_t i = 0; i < num_mems; i++) {
      fp << "\t" << circuit_lib.model_name(tgate_model) << " " << circuit_lib.model_prefix(tgate_model) << "_" << i << " ";
      if (true == circuit_lib.dump_explicit_port_map(tgate_model)) {
        fp << " (";
        fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[0]) << "(" << "in[" << i << "]" << "),";
        fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[1]) << "(" << "mem[" << i << "]" << "),";
        fp << "  ." << circuit_lib.port_lib_name(tgate_input_ports[2]) << "(" << "mem_inv[" << i << "]" << "),";
        fp << "  ." << circuit_lib.port_lib_name(tgate_output_ports[0]) << "(" << generate_verilog_port(VERILOG_PORT_CONKT, output_port) << ")";
        fp << ");" << std::endl;
      } else {
        fp << " (";
        fp << "in[" << i << "]";
        fp << ", " << "mem[" << i << "]";
        fp << ", " << "mem_inv[" << i << "]";
        fp << ", " <<  generate_verilog_port(VERILOG_PORT_CONKT, output_port);
        fp << ");" << std::endl;
      }
    }
  }

  /* Put an end to this module */
  fp << "endmodule" << std::endl;

  /* Comment lines */
  fp << "//---- END Structural Verilog CMOS MUX basis module: " << module_name << "-----" << std::endl << std::endl;

  return;
}

/***********************************************
 * Generate Verilog codes modeling an branch circuit 
 * for a multiplexer with the given size 
 **********************************************/
void generate_verilog_mux_branch_module(std::fstream& fp, 
                                        const CircuitLibrary& circuit_lib, 
                                        const CircuitModelId& circuit_model, 
                                        const size_t& mux_size, 
                                        const MuxGraph& mux_graph) {
  std::string module_name = generate_verilog_mux_branch_subckt_name(circuit_lib, circuit_model, mux_size, verilog_mux_basis_posfix);

  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(circuit_model)) {
  case SPICE_MODEL_DESIGN_CMOS:
    if (true == circuit_lib.dump_structural_verilog(circuit_model)) {
      generate_verilog_cmos_mux_branch_module_structural(fp, circuit_lib, circuit_model, module_name, mux_graph);
    } else {
      /*
      dump_verilog_cmos_mux_one_basis_module(fp, mux_basis_subckt_name,
                                             mux_size,
                                             num_input_basis_subckt,
                                             cur_spice_model,
                                             special_basis);
       */
    }
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* If requested, we can dump structural verilog for basis module */
    /*
    if (true == circuit_lib.dump_structural_verilog(circuit_model)) {
      dump_verilog_rram_mux_one_basis_module_structural(fp, mux_basis_subckt_name,
                                                        num_input_basis_subckt,
                                                        cur_spice_model);
    } else {
      dump_verilog_rram_mux_one_basis_module(fp, mux_basis_subckt_name,
                                             num_input_basis_subckt,
                                             cur_spice_model);
    }
    */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d]) Invalid design technology of multiplexer (name: %s)\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model)); 
    exit(1);
  }

  return;
}
