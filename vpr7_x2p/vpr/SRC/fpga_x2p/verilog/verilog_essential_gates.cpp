/************************************************
 * This file includes functions on 
 * outputting Verilog netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
 ***********************************************/
#include <fstream>
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
#include "verilog_essential_gates.h"

/************************************************
 * Print Verilog body codes of a power-gated inverter 
 * This function does NOT generate any port map !
 ***********************************************/
static 
void print_verilog_power_gated_invbuf_body(std::fstream& fp,
                                           const CircuitLibrary& circuit_lib,
                                           const CircuitModelId& circuit_model,
                                           const CircuitPortId& input_port,
                                           const CircuitPortId& output_port,
                                           const std::vector<CircuitPortId>& power_gate_ports) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Verilog codes of a power-gated inverter -----"));

  /* Create a sensitive list */
  fp << "\treg " << circuit_lib.port_lib_name(output_port) << "_reg;" << std::endl;

  fp << "\talways @(" << std::endl;
  /* Power-gate port first*/
  for (const auto& power_gate_port : power_gate_ports) {
    /* Skip first comma to dump*/
    if (0 < &power_gate_port - &power_gate_ports[0]) {
      fp << ",";
    }
    fp << circuit_lib.port_lib_name(power_gate_port);
  }
  fp << circuit_lib.port_lib_name(input_port) << ") begin" << std::endl; 

  /* Dump the case of power-gated */
  fp << "\t\tif (";
  /* For the first pin, we skip output comma */
  size_t port_cnt = 0;
  for (const auto& power_gate_port : power_gate_ports) {
    for (const auto& power_gate_pin : circuit_lib.pins(power_gate_port)) {
      if (0 < port_cnt) { 
        fp << std::endl << "\t\t&&";
      }
      fp << "(";

      /* Power-gated signal are disable during operating, enabled during configuration,
       * Therefore, we need to reverse them here   
       */
      if (0 == circuit_lib.port_default_value(power_gate_port)) {
        fp << "~";
      }
      
      fp << circuit_lib.port_lib_name(power_gate_port) << "[" << power_gate_pin << "])";

      port_cnt++; /* Update port counter*/
    }
  }

  fp << ") begin" << std::endl;
  fp << "\t\t\tassign " << circuit_lib.port_lib_name(output_port) << "_reg = "; 

  /* Branch on the type of inverter/buffer: 
   * 1. If this is an inverter or an tapered(multi-stage) buffer with odd number of stages, 
   *    we invert the input to output
   * 2. If this is a buffer or an tapere(multi-stage) buffer with even number of stages,
   *    we wire the input to output
   */
  if ( (SPICE_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model))
    || ( (SPICE_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model))
      && (1 == circuit_lib.buffer_num_levels(circuit_model) % 2 ) ) ) {
    fp << "~";
  } 

  fp << circuit_lib.port_lib_name(input_port) << ";" << std::endl;
  fp << "\t\tend else begin" << std::endl;
  fp << "\t\t\tassign " << circuit_lib.port_lib_name(output_port) << "_reg = 1'bz;" << std::endl;
  fp << "\t\tend" << std::endl;
  fp << "\tend" << std::endl;
  fp << "\tassign " << circuit_lib.port_lib_name(output_port) << " = " << circuit_lib.port_lib_name(output_port) << "_reg;" << std::endl;
}

/************************************************
 * Print Verilog body codes of a regular inverter 
 * This function does NOT generate any port map !
 ***********************************************/
static 
void print_verilog_invbuf_body(std::fstream& fp,
                               const CircuitLibrary& circuit_lib,
                               const CircuitModelId& circuit_model,
                               const CircuitPortId& input_port,
                               const CircuitPortId& output_port) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Verilog codes of a regular inverter -----"));
  
  fp << "\tassign " << circuit_lib.port_lib_name(output_port) << " = (" << circuit_lib.port_lib_name(input_port) << " == 1'bz)? $random : ";

  /* Branch on the type of inverter/buffer: 
   * 1. If this is an inverter or an tapered(multi-stage) buffer with odd number of stages, 
   *    we invert the input to output
   * 2. If this is a buffer or an tapere(multi-stage) buffer with even number of stages,
   *    we wire the input to output
   */
  if ( (SPICE_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model))
    || ( (SPICE_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model))
      && (1 == circuit_lib.buffer_num_levels(circuit_model) % 2 ) ) ) {
    fp << "~";
  } 

  fp << circuit_lib.port_lib_name(input_port) << ";" << std::endl;
}

/************************************************
 * Print a Verilog module of inverter or buffer 
 * or tapered buffer to a file 
 ***********************************************/
static 
void print_verilog_invbuf_module(std::fstream& fp,
                                 const CircuitLibrary& circuit_lib,
                                 const CircuitModelId& circuit_model) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  /* Find the input port, output port and global inputs*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT);

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

  /* dump module body */
  print_verilog_module_definition(fp, circuit_lib.model_name(circuit_model));

  /* TODO: print global ports, this should be handled by ModuleManager */
  for (const auto& port : global_ports) {
    /* Configure each global port */
    BasicPort basic_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    /* Print port */
    fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, basic_port) << "," << std::endl;
  }

  /* Dump ports */
  BasicPort input_port;
  /* Configure each input port */
  input_port.set_name(circuit_lib.port_lib_name(input_ports[0]));
  input_port.set_width(circuit_lib.port_size(input_ports[0]));
  fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, input_port) << "," << std::endl;

  BasicPort output_port;
  /* Configure each input port */
  output_port.set_name(circuit_lib.port_lib_name(output_ports[0]));
  output_port.set_width(circuit_lib.port_size(output_ports[0]));
  fp << "\t" << generate_verilog_port(VERILOG_PORT_OUTPUT, output_port) << std::endl;
  fp << ");" << std::endl;
  /* Finish dumping ports */

  /* Assign logics : depending on topology */
  /* Error out for unsupported technology */
  if ( ( SPICE_MODEL_BUF_INV != circuit_lib.buffer_type(circuit_model))
    && ( SPICE_MODEL_BUF_BUF != circuit_lib.buffer_type(circuit_model)) ) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid topology for circuit model (name=%s)!\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model));
    exit(1);
  }

  if (TRUE == circuit_lib.is_power_gated(circuit_model)) {
    /* Output Verilog codes for a power-gated inverter */
    print_verilog_power_gated_invbuf_body(fp, circuit_lib, circuit_model, input_ports[0], output_ports[0], global_ports);
  } else {
    /* Output Verilog codes for a regular inverter */
    print_verilog_invbuf_body(fp, circuit_lib, circuit_model, input_ports[0], output_ports[0]);
  }

  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, circuit_model);

  /* Print signal initialization */
  print_verilog_submodule_signal_init(fp, circuit_lib, circuit_model);

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(circuit_model));

  return;
}

/************************************************
 * Print a Verilog module of a pass-gate,
 * either transmission-gate or pass-transistor
 ***********************************************/
static 
void print_verilog_passgate_module(std::fstream& fp,
                                   const CircuitLibrary& circuit_lib,
                                   const CircuitModelId& circuit_model) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  /* Find the input port, output port*/
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT);

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
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model));
    exit(1);
  }

  /* Make sure:
   * There is only 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  /* Print Verilog module */
  print_verilog_module_definition(fp, circuit_lib.model_name(circuit_model));

  /* TODO: print global ports, this should be handled by ModuleManager */
  for (const auto& port : global_ports) {
    /* Configure each global port */
    BasicPort basic_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    /* Print port */
    fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, basic_port) << "," << std::endl;
  }

  for (const auto& input_port : input_ports) {
    /* Configure each global port */
    BasicPort basic_port(circuit_lib.port_lib_name(input_port), circuit_lib.port_size(input_port));
    /* Print port */
    fp << "\t" << generate_verilog_port(VERILOG_PORT_INPUT, basic_port) << "," << std::endl;
  }

  /* Configure each global port */
  for (const auto& output_port : output_ports) {
    BasicPort basic_port(circuit_lib.port_lib_name(output_port), circuit_lib.port_size(output_port));
    /* Print port */
    fp << "\t" << generate_verilog_port(VERILOG_PORT_OUTPUT, basic_port);
    /* Last port does not need a comma */
    if (output_port != output_ports.back()) {
      fp << "," << std::endl;
    } else {
      fp << std::endl;
    }
  }
  fp << ");" << std::endl;

  /* Dump logics: we propagate input to the output when the gate is '1' 
   * the input is blocked from output when the gate is '0'
   */
  fp << "\tassign " << circuit_lib.port_lib_name(output_ports[0]) << " = ";
  fp << circuit_lib.port_lib_name(input_ports[1]) << " ? " << circuit_lib.port_lib_name(input_ports[0]);
  fp << " : 1'bz;" << std::endl;

  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, circuit_model);

  /* Print signal initialization */
  print_verilog_submodule_signal_init(fp, circuit_lib, circuit_model);
   
  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(circuit_model));
}

/************************************************
 * Generate the Verilog netlist for essential gates
 * include inverters, buffers, transmission-gates,
 * etc.
 ***********************************************/
void print_verilog_submodule_essentials(const std::string& verilog_dir, 
                                        const std::string& submodule_dir,
                                        const CircuitLibrary& circuit_lib) {
  /* TODO: remove .bak when this part is completed and tested */
  std::string verilog_fname = submodule_dir + essentials_verilog_file_name + ".bak";

  std::fstream fp;

  /* Create the file stream */
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_handler(fp); 

  /* Create file */
  vpr_printf(TIO_MESSAGE_INFO,
             "Generating Verilog netlist (%s) for essential gates...\n",
             __FILE__, __LINE__, essentials_verilog_file_name); 

  print_verilog_file_header(fp, "Essential gates"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  for (const auto& circuit_model : circuit_lib.models()) {
    /* By pass user-defined modules */
    if (!circuit_lib.model_verilog_netlist(circuit_model).empty()) {
      continue;
    }
    if (SPICE_MODEL_INVBUF == circuit_lib.model_type(circuit_model)) {
      print_verilog_invbuf_module(fp, circuit_lib, circuit_model);
    }
    if (SPICE_MODEL_PASSGATE == circuit_lib.model_type(circuit_model)) {
      print_verilog_passgate_module(fp, circuit_lib, circuit_model);
    }
    /*
    if (SPICE_MODEL_GATE == spice_models[imodel].type) {
      dump_verilog_gate_module(fp, &(spice_models[imodel]));
    }
    */
  }
  
  /* Close file handler*/
  fp.close();

  /* Add fname to the linked list */
  /* TODO: enable this when this function is completed
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_fname.c_str());  
   */

  return;
}
