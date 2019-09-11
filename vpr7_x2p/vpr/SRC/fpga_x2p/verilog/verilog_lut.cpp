/********************************************************************
 * This file includes functions to generate Verilog submodules for LUTs
 ********************************************************************/
#include <string>
#include <algorithm>

#include "util.h"
#include "vtr_assert.h"

/* Device-level header files */
#include "mux_graph.h"
#include "module_manager.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "mux_utils.h"

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_submodule_utils.h"
#include "verilog_lut.h"

/********************************************************************
 * Print a Verilog module for a LUT circuit model
 * This function supports both single-output and fracturable LUTs
 * The Verilog module will be organized in structural Verilog codes.
 * It will instanciate:
 * 1. Multiplexer used inside LUT
 * 2. Input buffers 
 * 3. Input inverters
 * 4. Output buffers.
 * 6. AND/OR gates to tri-state LUT inputs
 ********************************************************************/
static 
void print_verilog_submodule_lut(ModuleManager& module_manager,
                                 const CircuitLibrary& circuit_lib,
                                 std::fstream& fp,
                                 const CircuitModelId& circuit_model) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  /* Get the global ports required by MUX (and any submodules) */
  std::vector<CircuitPortId> lut_global_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  /* Get the input ports from the mux */
  std::vector<CircuitPortId> lut_input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> lut_output_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_OUTPUT, true);

  /* Classify SRAM ports into two categories: regular (not for mode select) and mode-select */
  std::vector<CircuitPortId> lut_regular_sram_ports;
  std::vector<CircuitPortId> lut_mode_select_sram_ports;

  { /* Create a code block to keep some variables in local */
    /* Get the sram ports from the mux */
    std::vector<CircuitPortId> lut_sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM, true);
    for (const auto& port : lut_sram_ports) {
      /* Bypass mode_select ports */
      if (true == circuit_lib.port_is_mode_select(port)) {
        lut_mode_select_sram_ports.push_back(port);
        continue;
      }  
      VTR_ASSERT_SAFE (false == circuit_lib.port_is_mode_select(port));
      lut_regular_sram_ports.push_back(port);
    }
  }

  /* Make sure that the number of ports and sizes of ports are what we want */
  if (false == circuit_lib.is_lut_fracturable(circuit_model)) {
    /* Single-output LUTs: 
     * We should have only 1 input port, 1 output port and 1 SRAM port
     */
    VTR_ASSERT (1 == lut_input_ports.size());
    VTR_ASSERT (1 == lut_output_ports.size());
    VTR_ASSERT (1 == lut_regular_sram_ports.size()); 
    VTR_ASSERT (0 == lut_mode_select_sram_ports.size()); 
  } else {
    VTR_ASSERT (true == circuit_lib.is_lut_fracturable(circuit_model));
    /* Fracturable LUT:
     * We should have only 1 input port, a few output ports (fracturable outputs)
     * and two SRAM ports
     */
    VTR_ASSERT (1 == lut_input_ports.size());
    VTR_ASSERT (1 <= lut_output_ports.size());
    VTR_ASSERT (1 == lut_regular_sram_ports.size()); 
    VTR_ASSERT (1 == lut_mode_select_sram_ports.size()); 
  }

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(circuit_lib.model_name(circuit_model)); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Add module ports */
  /* Add each global port */
  for (const auto& port : lut_global_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  for (const auto& port : lut_input_ports) {
    BasicPort input_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
  }
  /* Add each output port */
  for (const auto& port : lut_output_ports) {
    BasicPort output_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }
  /* Add each regular (not mode select) SRAM port */
  for (const auto& port : lut_regular_sram_ports) {
    BasicPort mem_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, mem_port, ModuleManager::MODULE_INPUT_PORT);
    BasicPort mem_inv_port(std::string(circuit_lib.port_lib_name(port) + "_inv"), circuit_lib.port_size(port));
    module_manager.add_port(module_id, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* Add each mode-select SRAM port */
  for (const auto& port : lut_mode_select_sram_ports) {
    BasicPort mem_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, mem_port, ModuleManager::MODULE_INPUT_PORT);
    BasicPort mem_inv_port(std::string(circuit_lib.port_lib_name(port) + "_inv"), circuit_lib.port_size(port));
    module_manager.add_port(module_id, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);

  /* Print local wires for mode selector */
  /* Local wires for the output of mode selector */
  BasicPort mode_select_output_port(std::string(circuit_lib.port_lib_name(lut_input_ports[0]) + "_mode"), circuit_lib.port_size(lut_input_ports[0]));
  fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, mode_select_output_port) << ";" << std::endl;
  /* Local wires for the output of input inverters */
  BasicPort inverted_input_port(std::string(circuit_lib.port_lib_name(lut_input_ports[0]) + "_b"), circuit_lib.port_size(lut_input_ports[0]));
  fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, inverted_input_port) << ";" << std::endl;
  /* Local wires for the output of input buffers */
  BasicPort buffered_input_port(std::string(circuit_lib.port_lib_name(lut_input_ports[0]) + "_buf"), circuit_lib.port_size(lut_input_ports[0]));
  fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, buffered_input_port) << ";" << std::endl;

  /* Instanciate mode selecting circuit: AND/OR gate 
   * By following the tri-state map of LUT input port
   * The wiring of input ports will be organized as follows
   *
   *                   LUT input
   *                       |
   *                       v
   *                  +----------+
   *                  |   mode   |
   *                  | selector |
   *                  +----------+
   *                       | mode_select_output_port
   *     +-----------------+------------+
   *     |                              |
   *  +----------+                 +---------+
   *  | Inverter |                 |  Buffer |
   *  +----------+                 +---------+
   *       | inverter_input_port        | buffered_input_port
   *       v                            v
   *  +--------------------------------------+
   *  |      LUT Multiplexing Structure      |
   *  +--------------------------------------+
   */
  print_verilog_comment(fp, std::string("---- BEGIN Instanciation of model-select gates -----"));
  /* Get the tri-state port map for the input ports*/
  std::string tri_state_map = circuit_lib.port_tri_state_map(lut_input_ports[0]);
  size_t mode_select_port_lsb = 0;
  for (const auto& pin : circuit_lib.pins(lut_input_ports[0])) {
    BasicPort cur_mode_select_output_port(mode_select_output_port.get_name(), pin, pin);
    BasicPort cur_input_port(circuit_lib.port_lib_name(lut_input_ports[0]), pin, pin);
    /* For an empty tri-state map or a '-' sign in tri-state map, we can short-wire mode select_output_ports */
    if (tri_state_map.empty() || ('-' == tri_state_map[pin]) ) {
      print_verilog_wire_connection(fp, cur_mode_select_output_port, cur_input_port, false); 
      continue; /* Finish here */
    }
    /* Reach here, it means that we need a circuit for mode selection */
    BasicPort cur_lut_mode_select_sram_port(circuit_lib.port_lib_name(lut_mode_select_sram_ports[0]), mode_select_port_lsb, mode_select_port_lsb);
    enum e_spice_model_gate_type required_gate_type;
    if ('0' == tri_state_map[pin]) {
      /* We need a 2-input AND gate, in order to tri-state the input 
       * Detailed circuit is as follow:
       *              +---------+
       *     SRAM --->| 2-input |----> mode_select_output_port
       * LUT input--->|   AND   |
       *              +---------+
       * When SRAM is set to logic 0, the LUT input is tri-stated
       * When SRAM is set to logic 1, the LUT input is effective to the downstream circuits
       */
       required_gate_type = SPICE_MODEL_GATE_AND;
    } else {
      VTR_ASSERT ('1' == tri_state_map[pin]);
      /* We need a 2-input OR gate, in order to tri-state the input 
       * Detailed circuit is as follow:
       *              +---------+
       *     SRAM --->| 2-input |----> mode_select_output_port
       * LUT input--->|   OR    |
       *              +---------+
       * When SRAM is set to logic 1, the LUT input is tri-stated
       * When SRAM is set to logic 0, the LUT input is effective to the downstream circuits
       */
       required_gate_type = SPICE_MODEL_GATE_OR;
    } 
    /* Get the circuit model of the gate */
    CircuitModelId gate_model = circuit_lib.port_tri_state_model(lut_input_ports[0]);
    /* Check this is the gate we want ! */
    VTR_ASSERT (required_gate_type == circuit_lib.gate_type(gate_model));

    /* Prepare for the gate instanciation */
    /* Get the input ports from the gate */
    std::vector<CircuitPortId> gate_input_ports = circuit_lib.model_ports_by_type(gate_model, SPICE_MODEL_PORT_INPUT, true);
    /* Get the output ports from the gate */
    std::vector<CircuitPortId> gate_output_ports = circuit_lib.model_ports_by_type(gate_model, SPICE_MODEL_PORT_OUTPUT, true);
    /* Check the port sizes and width: 
     * we should have only 2 input ports, each of which has a size of 1 
     * we should have only 1 output port, each of which has a size of 1 
     */
    VTR_ASSERT (2 == gate_input_ports.size());
    for (const auto& port : gate_input_ports) {
      VTR_ASSERT (1 == circuit_lib.port_size(port));
    }
    VTR_ASSERT (1 == gate_output_ports.size());
    for (const auto& port : gate_output_ports) {
      VTR_ASSERT (1 == circuit_lib.port_size(port));
    }
    /* Find the module id of gate_model in the module manager */
    ModuleId gate_module_id = module_manager.find_module(circuit_lib.model_name(gate_model));
    /* We must have a valid id */
    VTR_ASSERT (ModuleId::INVALID() != gate_module_id);
    /* Create a port-to-port map:
     * Input[0] of the gate is wired to a SRAM mode-select port 
     * Input[1] of the gate is wired to the input port of LUT
     * Output[0] of the gate is wired to the mode_select_output_port 
     */
    std::map<std::string, BasicPort> port2port_name_map;
    port2port_name_map[circuit_lib.port_lib_name(gate_input_ports[0])] = cur_lut_mode_select_sram_port;
    port2port_name_map[circuit_lib.port_lib_name(gate_input_ports[1])] = cur_input_port;
    port2port_name_map[circuit_lib.port_lib_name(gate_output_ports[0])] = cur_mode_select_output_port;

    /* Instanciate the gate */
    print_verilog_module_instance(fp, module_manager, module_id, gate_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(circuit_model));
    /* IMPORTANT: this update MUST be called after the instance outputting!!!!
     * update the module manager with the relationship between the parent and child modules 
     */
    module_manager.add_child_module(module_id, gate_module_id);
    /* update the lsb of mode select port size */
    mode_select_port_lsb++;
  }
  print_verilog_comment(fp, std::string("---- END Instanciation of model-select gates -----"));
  /* Sanitity check */
  if ( true == circuit_lib.is_lut_fracturable(circuit_model) ) {
    if (mode_select_port_lsb != circuit_lib.port_size(lut_mode_select_sram_ports[0])) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(FILE:%s,LINE[%d]) Circuit model LUT (name=%s) has a unmatched tri-state map (%s) implied by mode_port size(%d)!\n",
                 __FILE__, __LINE__, 
                 circuit_lib.model_name(circuit_model).c_str(), 
                 tri_state_map.c_str(), 
                 circuit_lib.port_size(lut_mode_select_sram_ports[0])); 
      exit(1);
    }
  }

  /* Add a blank-line splitter */
  fp << std::endl; 

  /* Add inverters to mode_select output ports  */
  print_verilog_comment(fp, std::string("---- BEGIN Instanciation of an input inverters modules -----"));
  /* Find the circuit model of the input inverter */
  CircuitModelId input_inverter_model = circuit_lib.lut_input_inverter_model(circuit_model);
  VTR_ASSERT( CircuitModelId::INVALID() != input_inverter_model );
  /* Now we need to add inverters by instanciating the modules */
  for (const auto& pin : circuit_lib.pins(lut_input_ports[0])) {
    /* Input of inverter is the output of mode select circuits */
    BasicPort inverter_instance_input_port(mode_select_output_port.get_name(), pin, pin);
    /* Output of inverter is the inverted input port */
    BasicPort inverter_instance_output_port(inverted_input_port.get_name(), pin, pin);

    print_verilog_buffer_instance(fp, module_manager, circuit_lib, module_id, input_inverter_model, inverter_instance_input_port, inverter_instance_output_port);
  }
  print_verilog_comment(fp, std::string("---- END Instanciation of an input inverters modules -----"));

  /* Add buffers to mode_select output ports  */
  print_verilog_comment(fp, std::string("---- BEGIN Instanciation of an input buffer modules -----"));
  /* Find the circuit model of the input inverter */
  CircuitModelId input_buffer_model = circuit_lib.lut_input_buffer_model(circuit_model);
  VTR_ASSERT( CircuitModelId::INVALID() != input_buffer_model );
  /* Now we need to add inverters by instanciating the modules */
  for (const auto& pin : circuit_lib.pins(lut_input_ports[0])) {
    /* Input of inverter is the output of mode select circuits */
    BasicPort buffer_instance_input_port(mode_select_output_port.get_name(), pin, pin);
    /* Output of inverter is the inverted input port */
    BasicPort buffer_instance_output_port(buffered_input_port.get_name(), pin, pin);

    print_verilog_buffer_instance(fp, module_manager, circuit_lib, module_id, input_buffer_model, buffer_instance_input_port, buffer_instance_output_port);
  }
  print_verilog_comment(fp, std::string("---- END Instanciation of an input buffer modules -----"));

  /* Instanciate the multiplexing structure for the LUT */ 
  print_verilog_comment(fp, std::string("---- BEGIN Instanciation of LUT multiplexer module -----"));
  /* Find the name of LUT MUX: no need to provide a mux size, just give an invalid number (=-1) */
  std::string lut_mux_module_name = generate_verilog_mux_subckt_name(circuit_lib, circuit_model, size_t(-1), std::string(""));
  /* Find the module id of LUT MUX in the module manager */
  ModuleId lut_mux_module_id = module_manager.find_module(lut_mux_module_name);
  /* We must have a valid id */
  VTR_ASSERT (ModuleId::INVALID() != lut_mux_module_id);
  /* Create a port-to-port map:
   * Input of the LUT MUX is wired to a regular SRAM port of LUT
   * Outputs of the LUT MUX is wired to the output ports of LUT by name
   * SRAM of the LUT MUX is wired to the buffered input port of LUT
   * SRAM_inv of the LUT MUX is wired to the inverted input port of LUT
   */
  std::map<std::string, BasicPort> port2port_name_map;
  port2port_name_map[circuit_lib.port_lib_name(lut_input_ports[0])] = BasicPort(circuit_lib.port_lib_name(lut_regular_sram_ports[0]), circuit_lib.port_size(lut_regular_sram_ports[0]));
  /* Skip the output ports, if we do not need a new name for the port of instance */
  port2port_name_map[circuit_lib.port_lib_name(lut_regular_sram_ports[0])] = buffered_input_port;
  /* TODO: be more flexible in naming !!! */
  port2port_name_map[std::string(circuit_lib.port_lib_name(lut_regular_sram_ports[0]) + "_inv")] = inverted_input_port;

  /* Instanciate the gate */
  print_verilog_module_instance(fp, module_manager, module_id, lut_mux_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(circuit_model));
  /* IMPORTANT: this update MUST be called after the instance outputting!!!!
   * update the module manager with the relationship between the parent and child modules 
   */
  module_manager.add_child_module(module_id, lut_mux_module_id);

  /* Print timing info */
  print_verilog_submodule_timing(fp, circuit_lib, circuit_model);

  /* Print signal initialization */
  print_verilog_submodule_signal_init(fp, circuit_lib, circuit_model);

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, circuit_lib.model_name(circuit_model));
}

/******************************************************************** 
 * Print Verilog modules for the Look-Up Tables (LUTs) 
 * in the circuit library
 ********************************************************************/
void print_verilog_submodule_luts(ModuleManager& module_manager,
                                  const CircuitLibrary& circuit_lib,
                                  const std::string& verilog_dir,
                                  const std::string& submodule_dir) {
  /* TODO: remove .bak when this part is completed and tested */
  std::string verilog_fname = submodule_dir + luts_verilog_file_name + ".bak";

  std::fstream fp;

  /* Create the file stream */
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_handler(fp); 

  /* Create file */
  vpr_printf(TIO_MESSAGE_INFO,
             "Generating Verilog netlist for LUTs (%s)...\n",
             verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Look-Up Tables"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Search for each LUT circuit model */
  for (const auto& circuit_model : circuit_lib.models()) {
    /* Bypass user-defined and non-LUT modules */
    if ( (!circuit_lib.model_verilog_netlist(circuit_model).empty()) 
      || (SPICE_MODEL_LUT != circuit_lib.model_type(circuit_model)) ) {
      continue;
    }
    print_verilog_submodule_lut(module_manager, circuit_lib, fp, circuit_model);
  }

  /* Close the file handler */
  fp.close();

  /* Add fname to the linked list */
  /* Add it when the Verilog generation is refactored
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_fname.c_str());  
   */

  return;
}

