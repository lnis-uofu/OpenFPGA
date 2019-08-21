/************************************************
 * Header file for verilog_submodule_essential.cpp
 * Include function declaration on 
 * outputting Verilog netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
 ***********************************************/
#include <fstream>
#include "vtr_assert.h"

/* Device-level header files */
#include "spice_types.h"

/* FPGA-X2P context header files */
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_essential_gates.h"

/************************************************
 * Print Verilog codes of a power-gated inverter 
 ***********************************************/
static 
void print_verilog_power_gated_inv_module(std::fstream& fp,
                                          const CircuitLibrary& circuit_lib,
                                          const CircuitPortId& input_port,
                                          const CircuitPortId& output_port,
                                          const std::vector<CircuitPortId>& power_gate_ports) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  fp << "//----- Verilog codes of a power-gated inverter -----" << std::endl;

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
  fp << "\t\t\tassign " << circuit_lib.port_lib_name(output_port) << "_reg = ~" << circuit_lib.port_lib_name(input_port) << ";" << std::endl;
  fp << "\t\tend else begin" << std::endl;
  fp << "\t\t\tassign " << circuit_lib.port_lib_name(output_port) << "_reg = 1'bz;" << std::endl;
  fp << "\t\tend" << std::endl;
  fp << "\tend" << std::endl;
  fp << "\tassign " << circuit_lib.port_lib_name(output_port) << " = " << circuit_lib.port_lib_name(output_port) << "_reg;" << std::endl;
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

  fp << "//----- Verilog module for " << circuit_lib.model_name(circuit_model) << " -----" << std::endl;

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
  fp << "module " << circuit_lib.model_name(circuit_model) << " (" << std::endl;

  /* TODO: print global ports */
  for (const auto& port : global_ports) {
    BasicPort basic_port;
    /* Configure each input port */
    basic_port.set_name(circuit_lib.port_prefix(port));
    basic_port.set_width(circuit_lib.port_size(port));
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
  fp << "\t" << generate_verilog_port(VERILOG_PORT_OUTPUT, output_port) << "," << std::endl;
  fp << ");" << std::endl;
  /* Finish dumping ports */

  /* Assign logics : depending on topology */
  switch (circuit_lib.buffer_type(circuit_model)) {
  case SPICE_MODEL_BUF_INV:
    if (TRUE == circuit_lib.is_power_gated(circuit_model)) {
      print_verilog_power_gated_inv_module(fp, circuit_lib, input_ports[0], output_ports[0], global_ports);
    }
//    } else {
//      fprintf(fp, "assign %s = (%s === 1'bz)? $random : ~%s;\n",
//                  output_port[0]->lib_name,
//                  input_port[0]->lib_name,
//                  input_port[0]->lib_name);
//    }
    break;
  case SPICE_MODEL_BUF_BUF:
//    if (TRUE == invbuf_spice_model->design_tech_info.power_gated) {
//      /* Create a sensitive list */
//      fprintf(fp, "reg %s_reg;\n", output_port[0]->lib_name);
//      fprintf(fp, "always @(");
//      /* Power-gate port first*/
//      for (iport = 0; iport < num_powergate_port; iport++) {
//        fprintf(fp, "%s,", powergate_port[iport]->lib_name);
//      }
//      fprintf(fp, "%s) begin\n", 
//                  input_port[0]->lib_name);
//      /* Dump the case of power-gated */
//      fprintf(fp, "  if (");
//      port_cnt = 0; /* Initialize the counter: decide if we need to put down '&&' */
//      for (iport = 0; iport < num_powergate_port; iport++) {
//        if (0 == powergate_port[iport]->default_val) {
//          for (ipin = 0; ipin < powergate_port[iport]->size; ipin++) {
//            if ( 0 < port_cnt ) {
//              fprintf(fp, "\n\t&&");
//            }
//            /* Power-gated signal are disable during operating, enabled during configuration,
//             * Therefore, we need to reverse them here   
//             */
//            fprintf(fp, "(~%s[%d])", 
//                         powergate_port[iport]->lib_name,
//                         ipin);
//            port_cnt++; /* Update port counter*/
//          }
//        } else {
//          assert (1 == powergate_port[iport]->default_val);
//          for (ipin = 0; ipin < powergate_port[iport]->size; ipin++) {
//            if ( 0 < port_cnt ) {
//              fprintf(fp, "\n\t&&");
//            }
//            /* Power-gated signal are disable during operating, enabled during configuration,
//             * Therefore, we need to reverse them here   
//             */
//            fprintf(fp, "(%s[%d])", 
//                        powergate_port[iport]->lib_name,
//                        ipin);
//            port_cnt++; /* Update port counter*/
//          }
//        }
//      }
//      fprintf(fp, ") begin\n");
//      fprintf(fp, "\t\tassign %s_reg = %s;\n",
//                  output_port[0]->lib_name,
//                  input_port[0]->lib_name);
//      fprintf(fp, "\tend else begin\n");
//      fprintf(fp, "\t\tassign %s_reg = 1'bz;\n",
//                  output_port[0]->lib_name);
//      fprintf(fp, "\tend\n");
//      fprintf(fp, "end\n");
//      fprintf(fp, "assign %s = %s_reg;\n",
//                  output_port[0]->lib_name,
//                  output_port[0]->lib_name);
//
//    } else if (FALSE == invbuf_spice_model->design_tech_info.buffer_info->tapered_buf) {
//      fprintf(fp, "assign %s = (%s === 1'bz)? $random : %s;\n",
//                  output_port[0]->lib_name,
//                  input_port[0]->lib_name,
//                  input_port[0]->lib_name);
//    } else {
//      assert (TRUE == invbuf_spice_model->design_tech_info.buffer_info->tapered_buf);
//      fprintf(fp, "assign %s = (%s === 1'bz)? $random : ",
//                  output_port[0]->lib_name,
//                  input_port[0]->lib_name);
//      /* depend on the stage, we may invert the output */
//      if (1 == invbuf_spice_model->design_tech_info.buffer_info->tap_buf_level % 2) {
//        fprintf(fp, "~");
//      }
//      fprintf(fp, "%s;\n",
//                  input_port[0]->lib_name);
//    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid topology for circuit model (name=%s)!\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model));
    exit(1);
  }
//
//  /* Print timing info */
//  dump_verilog_submodule_timing(fp, invbuf_spice_model);
//
//  dump_verilog_submodule_signal_init(fp, invbuf_spice_model);

  fp << "endmodule" << std::endl << std::endl;

  return;
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
    /*
    if (SPICE_MODEL_PASSGATE == spice_models[imodel].type) {
      dump_verilog_passgate_module(fp, &(spice_models[imodel]));
    }
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
