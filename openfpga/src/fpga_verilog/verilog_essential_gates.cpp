/************************************************
 * This file includes functions on 
 * outputting Verilog netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
 ***********************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "openfpga_naming.h"
#include "module_manager.h"
#include "module_manager_utils.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_submodule_utils.h"
#include "verilog_essential_gates.h"

/* begin namespace openfpga */
namespace openfpga {

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
  VTR_ASSERT(true == valid_file_stream(fp));

  print_verilog_comment(fp, std::string("----- Verilog codes of a power-gated inverter -----"));

  /* Create a sensitive list */
  fp << "\treg " << circuit_lib.port_lib_name(output_port) << "_reg;" << std::endl;

  fp << "\talways @(";
  /* Power-gate port first*/
  for (const auto& power_gate_port : power_gate_ports) {
    /* Only config_enable signal will be considered */
    if (false == circuit_lib.port_is_config_enable(power_gate_port)) {
      continue;
    }
    fp << circuit_lib.port_lib_name(power_gate_port);
    fp << ", ";
  }
  fp << circuit_lib.port_lib_name(input_port) << ") begin" << std::endl; 

  /* Dump the case of power-gated */
  fp << "\t\tif (";
  /* For the first pin, we skip output comma */
  size_t port_cnt = 0;
  for (const auto& power_gate_port : power_gate_ports) {
    /* Only config_enable signal will be considered */
    if (false == circuit_lib.port_is_config_enable(power_gate_port)) {
      continue;
    }
    for (const auto& power_gate_pin : circuit_lib.pins(power_gate_port)) {
      if (0 < port_cnt) { 
        fp << std::endl << "\t\t&&";
      }
      fp << "(";

      /* Power-gated signal are disable during operating, enabled during configuration,
       * Therefore, we need to reverse them here   
       */
      if (1 == circuit_lib.port_default_value(power_gate_port)) {
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
  if ( (CIRCUIT_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model))
    || ( (CIRCUIT_MODEL_BUF_BUF == circuit_lib.buffer_type(circuit_model))
      && (size_t(-1) != circuit_lib.buffer_num_levels(circuit_model)) 
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
  VTR_ASSERT(true == valid_file_stream(fp));

  print_verilog_comment(fp, std::string("----- Verilog codes of a regular inverter -----"));
  
  fp << "\tassign " << circuit_lib.port_lib_name(output_port) << " = (" << circuit_lib.port_lib_name(input_port) << " === 1'bz)? $random : ";

  /* Branch on the type of inverter/buffer: 
   * 1. If this is an inverter or an tapered(multi-stage) buffer with odd number of stages, 
   *    we invert the input to output
   * 2. If this is a buffer or an tapere(multi-stage) buffer with even number of stages,
   *    we wire the input to output
   */
  if ( (CIRCUIT_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model))
    || ( (CIRCUIT_MODEL_BUF_BUF == circuit_lib.buffer_type(circuit_model))
      && (size_t(-1) != circuit_lib.buffer_num_levels(circuit_model)) 
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
void print_verilog_invbuf_module(const ModuleManager& module_manager, 
                                 std::fstream& fp,
                                 const CircuitLibrary& circuit_lib,
                                 const CircuitModelId& circuit_model,
                                 const e_verilog_default_net_type& default_net_type) {
  /* Ensure a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

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

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.find_module(circuit_lib.model_name(circuit_model)); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id, default_net_type);
  /* Finish dumping ports */

  /* Assign logics : depending on topology */
  /* Error out for unsupported technology */
  if ( ( CIRCUIT_MODEL_BUF_INV != circuit_lib.buffer_type(circuit_model))
    && ( CIRCUIT_MODEL_BUF_BUF != circuit_lib.buffer_type(circuit_model)) ) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid topology for circuit model '%s'!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    exit(1);
  }

  if (true == circuit_lib.is_power_gated(circuit_model)) {
    /* Output Verilog codes for a power-gated inverter */
    print_verilog_power_gated_invbuf_body(fp, circuit_lib, circuit_model, input_ports[0], output_ports[0], global_ports);
  } else {
    /* Output Verilog codes for a regular inverter */
    print_verilog_invbuf_body(fp, circuit_lib, circuit_model, input_ports[0], output_ports[0]);
  }

  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, circuit_model);

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(circuit_model));
}

/************************************************
 * Print a Verilog module of a pass-gate,
 * either transmission-gate or pass-transistor
 ***********************************************/
static 
void print_verilog_passgate_module(const ModuleManager& module_manager, 
                                   std::fstream& fp,
                                   const CircuitLibrary& circuit_lib,
                                   const CircuitModelId& circuit_model,
                                   const e_verilog_default_net_type& default_net_type) {
  /* Ensure a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

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
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid topology for circuit model '%s'!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    exit(1);
  }

  /* Make sure:
   * There is only 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.find_module(circuit_lib.model_name(circuit_model)); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id, default_net_type);
  /* Finish dumping ports */

  /* Dump logics: we propagate input to the output when the gate is '1' 
   * the input is blocked from output when the gate is '0'
   */
  fp << "\tassign " << circuit_lib.port_lib_name(output_ports[0]) << " = ";
  fp << circuit_lib.port_lib_name(input_ports[1]) << " ? " << circuit_lib.port_lib_name(input_ports[0]);
  fp << " : 1'bz;" << std::endl;

  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, circuit_model);

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(circuit_model));
}

/************************************************
 * Print Verilog body codes of an N-input AND gate
 ***********************************************/
static 
void print_verilog_and_or_gate_body(std::fstream& fp,
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model,
                                    const std::vector<CircuitPortId>& input_ports,
                                    const std::vector<CircuitPortId>& output_ports) {
  /* Ensure a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Find the logic operator for the gate */
  std::string gate_verilog_operator;
  switch (circuit_lib.gate_type(circuit_model)) {
  case CIRCUIT_MODEL_GATE_AND:
    gate_verilog_operator = "&";
    break;
  case CIRCUIT_MODEL_GATE_OR:
    gate_verilog_operator = "|";
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid topology for circuit model '%s'!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    exit(1);
  }

  /* Output verilog codes */
  print_verilog_comment(fp, std::string("----- Verilog codes of a " + std::to_string(input_ports.size()) + "-input " + std::to_string(output_ports.size()) + "-output AND gate -----"));

  for (const auto& output_port : output_ports) {
    for (const auto& output_pin : circuit_lib.pins(output_port)) {
      BasicPort output_port_info(circuit_lib.port_lib_name(output_port), output_pin, output_pin);
      fp << "\tassign " << generate_verilog_port(VERILOG_PORT_CONKT, output_port_info);
      fp << " = ";

      size_t port_cnt = 0;
      for (const auto& input_port : input_ports) {
        for (const auto& input_pin : circuit_lib.pins(input_port)) {
          /* Do not output AND/OR operator for the first element in the loop */
          if (0 < port_cnt) {
            fp << " " << gate_verilog_operator << " ";
          }

          BasicPort input_port_info(circuit_lib.port_lib_name(input_port), input_pin, input_pin);
          fp << generate_verilog_port(VERILOG_PORT_CONKT, input_port_info);
            
          /* Increment the counter for port */ 
          port_cnt++;
        }
      }
      fp << ";" << std::endl;
    }
  }
}

/************************************************
 * Print Verilog body codes of an 2-input MUX gate
 ***********************************************/
static 
void print_verilog_mux2_gate_body(std::fstream& fp,
                                  const CircuitLibrary& circuit_lib,
                                  const CircuitModelId& circuit_model,
                                  const std::vector<CircuitPortId>& input_ports,
                                  const std::vector<CircuitPortId>& output_ports) {
  /* Ensure a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* TODO: Move the check codes to check_circuit_library.cpp */
  size_t num_err = 0;
  /* Check on the port sequence and map */
  /* MUX2 should only have 1 output port with size 1 */
  if (1 != output_ports.size()) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "MUX2 circuit model '%s' must have only 1 output!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    num_err++;
  } 
  for (const auto& output_port : output_ports) {  
    /* Bypass port size of 1 */
    if (1 == circuit_lib.port_size(output_port)) {
      continue;
    }
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "Output port size of a MUX2 circuit model '%s' must be 1!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    num_err++;
  }
  /* MUX2 should only have 3 output port, each of which has a port size of 1 */
  if (3 != input_ports.size()) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "MUX2 circuit model '%s' must have only 3 input!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    num_err++;
  }

  for (const auto& input_port : input_ports) {  
    /* Bypass port size of 1 */
    if (1 == circuit_lib.port_size(input_port)) {
      continue;
    }
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "Input size MUX2 circuit model '%s' must be 1!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    num_err++;
  }
  if (0 < num_err) {
    exit(1);
  }

  /* Now, we output the logic of MUX2
   * IMPORTANT Restriction:
   * We always assum the first two inputs are data inputs
   * the third input is the select port  
   */
  fp << "\tassign ";
  BasicPort out_port_info(circuit_lib.port_lib_name(output_ports[0]), 0, 0);
  BasicPort sel_port_info(circuit_lib.port_lib_name(input_ports[2]), 0, 0);
  BasicPort in0_port_info(circuit_lib.port_lib_name(input_ports[0]), 0, 0);
  BasicPort in1_port_info(circuit_lib.port_lib_name(input_ports[1]), 0, 0);

  fp << generate_verilog_port(VERILOG_PORT_CONKT, out_port_info);
  fp << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, sel_port_info);
  fp << " ? ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, in0_port_info);
  fp << " : ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, in1_port_info);
  fp << ";" << std::endl;
}

/************************************************
 * Print a Verilog module of a logic gate
 * which are standard cells
 * Supported gate types: 
 * 1. N-input AND 
 * 2. N-input OR
 * 3. 2-input MUX
 ***********************************************/
static 
void print_verilog_gate_module(const ModuleManager& module_manager, 
                               std::fstream& fp,
                               const CircuitLibrary& circuit_lib,
                               const CircuitModelId& circuit_model,
                               const e_verilog_default_net_type& default_net_type) {
  /* Ensure a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

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
  ModuleId module_id = module_manager.find_module(circuit_lib.model_name(circuit_model)); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id, default_net_type);
  /* Finish dumping ports */

  /* Dump logics */
  switch (circuit_lib.gate_type(circuit_model)) {
  case CIRCUIT_MODEL_GATE_AND:
  case CIRCUIT_MODEL_GATE_OR:
    print_verilog_and_or_gate_body(fp, circuit_lib, circuit_model, input_ports, output_ports);
    break;
  case CIRCUIT_MODEL_GATE_MUX2:
    print_verilog_mux2_gate_body(fp, circuit_lib, circuit_model, input_ports, output_ports);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid topology for circuit model '%s'!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    exit(1);
  }

  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, circuit_model);

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(circuit_model));
}

/************************************************
 * Generate the Verilog netlist for a constant generator,
 * i.e., either VDD or GND
 ***********************************************/
static 
void print_verilog_constant_generator_module(const ModuleManager& module_manager, 
                                             std::fstream& fp, 
                                             const size_t& const_value,
                                             const e_verilog_default_net_type& default_net_type) {
  /* Find the module in module manager */
  std::string module_name = generate_const_value_module_name(const_value);
  ModuleId const_val_module = module_manager.find_module(module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(const_val_module));

  /* Ensure a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, const_val_module, default_net_type);
  /* Finish dumping ports */

  /* Find the only output*/
  for (const ModulePortId& module_port_id : module_manager.module_ports(const_val_module)) {
    BasicPort module_port = module_manager.module_port(const_val_module, module_port_id);
    print_verilog_wire_constant_values(fp, module_port, std::vector<size_t>(1, const_value));
  }
  
  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);
}

/************************************************
 * Generate the Verilog netlist for essential gates
 * include inverters, buffers, transmission-gates,
 * etc.
 ***********************************************/
void print_verilog_submodule_essentials(const ModuleManager& module_manager, 
                                        NetlistManager& netlist_manager,
                                        const std::string& submodule_dir,
                                        const CircuitLibrary& circuit_lib,
                                        const FabricVerilogOption& options) { 
  /* TODO: remove .bak when this part is completed and tested */
  std::string verilog_fname = submodule_dir + std::string(ESSENTIALS_VERILOG_FILE_NAME);

  std::fstream fp;

  /* Create the file stream */
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_stream(verilog_fname.c_str(), fp); 

  /* Create file */
  VTR_LOG("Generating Verilog netlist '%s' for essential gates...",
          verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Essential gates", options.time_stamp());

  /* Print constant generators */
  /* VDD */
  print_verilog_constant_generator_module(module_manager, fp, 0, options.default_net_type());
  /* GND */
  print_verilog_constant_generator_module(module_manager, fp, 1, options.default_net_type());

  for (const auto& circuit_model : circuit_lib.models()) {
    /* By pass user-defined modules */
    if (!circuit_lib.model_verilog_netlist(circuit_model).empty()) {
      continue;
    }
    if (CIRCUIT_MODEL_INVBUF == circuit_lib.model_type(circuit_model)) {
      print_verilog_invbuf_module(module_manager, fp, circuit_lib, circuit_model, options.default_net_type());
      continue;
    }
    if (CIRCUIT_MODEL_PASSGATE == circuit_lib.model_type(circuit_model)) {
      print_verilog_passgate_module(module_manager, fp, circuit_lib, circuit_model, options.default_net_type());
      continue;
    }
    if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(circuit_model)) {
      print_verilog_gate_module(module_manager, fp, circuit_lib, circuit_model, options.default_net_type());
      continue;
    }
  }

  /* Close file handler*/
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(verilog_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
