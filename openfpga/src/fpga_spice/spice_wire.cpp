/************************************************
 * This file includes functions on 
 * outputting SPICE netlists for routing wires:
 * - regular wires (1 input and 1 output)
 * - routing track wires (1 input and 2 outputs)
 ***********************************************/
#include <fstream>
#include <cmath>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "circuit_library_utils.h"
#include "build_module_graph_utils.h"

#include "spice_constants.h"
#include "spice_writer_utils.h"
#include "spice_wire.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print SPICE modeling for pie-type RC network
 *
 * Schematic
 *                      middle out
 *                          |
 *   in ---wwww----wwww--- ... --wwww---out 
 *       |       |      |             |
 *       =       =      =             =
 *       |       |      |             |
 *      GND     GND    GND           GND
 *******************************************************************/
static 
int print_spice_wire_pi_type_rc_modeling(std::fstream& fp,
                                         const std::string& input_port_name,
                                         const std::string& output_port_name,
                                         const std::string& middle_output_port_name,
                                         const float& res_total, 
                                         const float& cap_total, 
                                         const size_t& num_levels) {

  /* Determine the resistance and capacitance of each level*/
  float res_per_level = res_total / ((float)(2 * num_levels));
  float cap_per_level = cap_total / ((float)(2 * num_levels));

  /* All the resistance and capacitance value should be larger than or equal to zero*/
  VTR_ASSERT(0. <= res_per_level);
  VTR_ASSERT(0. <= cap_per_level);

  for (size_t ilvl = 0; ilvl < num_levels; ++ilvl) {
    /* Print the first capacitor if this is the first level */
    if ((0 == ilvl) && (0. < cap_per_level)) {
      print_spice_capacitor(fp, input_port_name, std::string(SPICE_SUBCKT_GND_PORT_NAME), cap_per_level);
    }
    /* Output a regular RC pair 
     *
     *           midnode
     *              ^
     *              |
     *   ------+-ww-+-ww-+------
     *         |         |
     *         =         =
     *         |         |
     *        GND       GND
     */

    std::string lvl_input_port_name = std::string("rc_network_node") + std::to_string(ilvl);
    if (0 == ilvl) {
      lvl_input_port_name = input_port_name;
    } 

    std::string lvl_middle_port_name = std::string("rc_network_midnode") + std::to_string(ilvl);

    std::string lvl_output_port_name = std::string("rc_network_node") + std::to_string(ilvl + 1);
    if (ilvl == num_levels - 1) {
      lvl_output_port_name = output_port_name;
    }

    print_spice_resistor(fp, lvl_input_port_name, lvl_middle_port_name, res_per_level); 
    print_spice_resistor(fp, lvl_middle_port_name, lvl_output_port_name, res_per_level); 

    /* Last level only require 1 unit of cap_per_level */
    float cap_curr_level = 2. * cap_per_level;
    if (ilvl == num_levels - 1) {
      cap_curr_level = cap_per_level;
    }

    if (0. < cap_curr_level) {
      print_spice_capacitor(fp, lvl_output_port_name, std::string(SPICE_SUBCKT_GND_PORT_NAME), cap_curr_level);
    }
  }

  /* If the middle output is required, create a short connection to
   * - when the number of levels is odd
   *
   *         middle_output
   *              ^
   *              |
   *   ---ww-+-ww-+-ww-+-ww---
   *         |         |
   *         =         =
   *         |         |
   *        GND       GND
   *
   * - when the number of levels is even:
   *    
   *         middle_output
   *             ^
   *             |
   *   -+-ww--ww-+-ww--ww-+-
   *    |        |        |
   *    =        =        =
   *    |        |        |
   *   GND      GND      GND
   *          
   */
  if (!middle_output_port_name.empty()) {
    print_spice_comment(fp, std::string("Connect to the middle output"));
    std::string rc_midnode_name = std::string("rc_network_node") + std::to_string(num_levels / 2 + 1);
    if (1 == num_levels % 2) {
      rc_midnode_name = std::string("rc_network_midnode") + std::to_string(num_levels / 2);
    }
    print_spice_short_connection(fp, rc_midnode_name, middle_output_port_name); 
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Print SPICE modeling for T-type RC network
 *
 * Schematic
 *                                middle out
 *                                    |
 *   in ---ww-+--ww--+--ww--+--ww--- ... --ww--+--ww--- out 
 *            |      |      |                  |
 *            =      =      =                  =
 *            |      |      |                  |
 *           GND    GND    GND                GND
 *******************************************************************/
static 
int print_spice_wire_t_type_rc_modeling(std::fstream& fp,
                                        const std::string& input_port_name,
                                        const std::string& output_port_name,
                                        const std::string& middle_output_port_name,
                                        const float& res_total, 
                                        const float& cap_total, 
                                        const size_t& num_levels) {

  /* Determine the resistance and capacitance of each level*/
  float res_per_level = res_total / ((float)(2 * num_levels));
  float cap_per_level = cap_total / ((float)(num_levels));

  /* All the resistance and capacitance value should be larger than or equal to zero*/
  VTR_ASSERT(0. <= res_per_level);
  VTR_ASSERT(0. <= cap_per_level);

  for (size_t ilvl = 0; ilvl < num_levels; ++ilvl) {
    /* Output a regular RC pair 
     *
     *           midnode
     *              ^
     *              |
     *   --------ww-+-ww--------
     *              |         
     *              =         
     *              |         
     *             GND      
     */

    std::string lvl_input_port_name = std::string("rc_network_node") + std::to_string(ilvl);
    if (0 == ilvl) {
      lvl_input_port_name = input_port_name;
    } 

    std::string lvl_middle_port_name = std::string("rc_network_midnode") + std::to_string(ilvl);

    std::string lvl_output_port_name = std::string("rc_network_node") + std::to_string(ilvl + 1);
    if (ilvl == num_levels - 1) {
      lvl_output_port_name = output_port_name;
    }

    print_spice_resistor(fp, lvl_input_port_name, lvl_middle_port_name, res_per_level); 
    print_spice_resistor(fp, lvl_middle_port_name, lvl_output_port_name, res_per_level); 

    if (0. < cap_per_level) {
      print_spice_capacitor(fp, lvl_middle_port_name, std::string(SPICE_SUBCKT_GND_PORT_NAME), cap_per_level);
    }
  }

  /* If the middle output is required, create a short connection to
   * - when the number of levels is even
   *
   *         middle_output
   *              ^
   *              |
   *   ---ww-+-ww-+-ww-+-ww---
   *         |         |
   *         =         =
   *         |         |
   *        GND       GND
   *
   * - when the number of levels is odd:
   *    
   *         middle_output
   *             ^
   *             |
   *   -+-ww--ww-+-ww--ww-+-
   *    |        |        |
   *    =        =        =
   *    |        |        |
   *   GND      GND      GND
   *          
   */
  if (!middle_output_port_name.empty()) {
    print_spice_comment(fp, std::string("Connect to the middle output"));
    std::string rc_midnode_name = std::string("rc_network_midnode") + std::to_string(num_levels / 2);
    if (0 == num_levels % 2) {
      rc_midnode_name = std::string("rc_network_node") + std::to_string(num_levels / 2 + 1);
    }
    print_spice_short_connection(fp, rc_midnode_name, middle_output_port_name); 
  }

  return CMD_EXEC_SUCCESS;
}


/********************************************************************
 * Generate the SPICE subckt for a regular wire
 *
 * Schematic
 *
 *                                 Middle output (only for routing track wires)
 *                                      ^
 *                                      |
 *      +--------------------+    +---------------+     +--------------------+
 * in ->| Inverter or buffer |--->|   RC Network  |---->| Inverter or buffer |---> out
 *      |  Optional          |    |               |     | Optional           |
 *      +---------------------    +---------------+     +--------------------+
 *     
 *******************************************************************/
int print_spice_wire_subckt(std::fstream& fp,
                            const ModuleManager& module_manager,
                            const ModuleId& module_id,
                            const CircuitLibrary& circuit_lib,
                            const CircuitModelId& circuit_model) {

 if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Find the input and output ports:
   * we do NOT support global ports here, 
   * it should be handled in another type of inverter subckt (power-gated)
   */
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);

  /* Make sure:
   * There are 1 input ports and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == input_ports.size()) && (1 == circuit_lib.port_size(input_ports[0])) );
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  int status = CMD_EXEC_SUCCESS;

  /* Print the inverter subckt definition */
  print_spice_subckt_definition(fp, module_manager, module_id); 

  std::string input_port_name = circuit_lib.port_prefix(input_ports[0]);
  std::string output_port_name = circuit_lib.port_prefix(output_ports[0]);
  std::string middle_output_port_name;
  if (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(circuit_model)) {
    middle_output_port_name = std::string("middle") + output_port_name;
  }

  std::string rc_ntwk_input_port_name = std::string("rc_network_node") + std::to_string(0);
  std::string rc_ntwk_output_port_name = std::string("rc_network_node") + std::to_string(circuit_lib.wire_num_level(circuit_model) - 1);
  std::string rc_ntwk_middle_output_port_name;
  if (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(circuit_model)) {
    rc_ntwk_middle_output_port_name = std::string("middle") + rc_ntwk_output_port_name;
  }
  
  ModulePortId wire_module_input_port = module_manager.find_module_port(module_id, input_port_name);
  ModulePortId wire_module_output_port = module_manager.find_module_port(module_id, output_port_name);
  ModulePortId wire_module_middle_output_port = ModulePortId::INVALID();
  if (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(circuit_model)) {
    wire_module_middle_output_port = module_manager.find_module_port(module_id, output_port_name);
  }

  /* Add input buffer:
   * - There is a valid buffer model, instanciate it
   * - There is no buffer, set a short connection
   */
  if (circuit_lib.input_buffer_model(circuit_model)) {
    std::string instance_name = std::string("input_buffer");
    std::map<std::string, BasicPort> port2port_name_map;

    ModuleId buffer_module = module_manager.find_module(circuit_lib.model_name(circuit_lib.input_buffer_model(circuit_model)));
    VTR_ASSERT(true == module_manager.valid_module_id(buffer_module));
  
    ModulePortId module_input_port_id = find_inverter_buffer_module_port(module_manager, buffer_module, circuit_lib, circuit_model, CIRCUIT_MODEL_PORT_INPUT);
    ModulePortId module_output_port_id = find_inverter_buffer_module_port(module_manager, buffer_module, circuit_lib, circuit_model, CIRCUIT_MODEL_PORT_OUTPUT);

    /* Port size should be 1 ! */
    VTR_ASSERT(1 == module_manager.module_port(buffer_module, module_input_port_id).get_width());
    VTR_ASSERT(1 == module_manager.module_port(buffer_module, module_output_port_id).get_width());

    port2port_name_map[module_manager.module_port(buffer_module, module_input_port_id).get_name()] = module_manager.module_port(module_id, wire_module_input_port);
    port2port_name_map[module_manager.module_port(buffer_module, module_output_port_id).get_name()] = BasicPort(rc_ntwk_input_port_name, 1);

    print_spice_subckt_instance(fp, 
                                module_manager, 
                                buffer_module,
                                instance_name,
                                port2port_name_map);
  } else {
    print_spice_short_connection(fp, circuit_lib.port_prefix(input_ports[0]), rc_ntwk_input_port_name); 
  }

  /* Determine which type of model to print*/
  switch (circuit_lib.wire_type(circuit_model)) {
  case WIRE_MODEL_PI:
   status = print_spice_wire_pi_type_rc_modeling(fp,
                                                 rc_ntwk_input_port_name,
                                                 rc_ntwk_output_port_name,
                                                 rc_ntwk_middle_output_port_name,
                                                 circuit_lib.wire_r(circuit_model), 
                                                 circuit_lib.wire_c(circuit_model), 
                                                 circuit_lib.wire_num_level(circuit_model)); 
    break;
  case WIRE_MODEL_T: 
    status = print_spice_wire_t_type_rc_modeling(fp,
                                                 rc_ntwk_input_port_name,
                                                 rc_ntwk_output_port_name,
                                                 rc_ntwk_middle_output_port_name,
                                                 circuit_lib.wire_r(circuit_model), 
                                                 circuit_lib.wire_c(circuit_model), 
                                                 circuit_lib.wire_num_level(circuit_model)); 
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Unsupport wire model type for circuit model '%s.\n",
                   circuit_lib.model_name(circuit_model).c_str());
    return CMD_EXEC_FATAL_ERROR;
  } 

  /* Add output buffer:
   * - There is a valid buffer model, instanciate it
   * - There is no buffer, set a short connection
   */
  if (circuit_lib.output_buffer_model(circuit_model)) {
    std::string instance_name = std::string("output_buffer");
    std::map<std::string, BasicPort> port2port_name_map;

    ModuleId buffer_module = module_manager.find_module(circuit_lib.model_name(circuit_lib.output_buffer_model(circuit_model)));
    VTR_ASSERT(true == module_manager.valid_module_id(buffer_module));
  
    ModulePortId module_input_port_id = find_inverter_buffer_module_port(module_manager, buffer_module, circuit_lib, circuit_model, CIRCUIT_MODEL_PORT_INPUT);
    ModulePortId module_output_port_id = find_inverter_buffer_module_port(module_manager, buffer_module, circuit_lib, circuit_model, CIRCUIT_MODEL_PORT_OUTPUT);

    /* Port size should be 1 ! */
    VTR_ASSERT(1 == module_manager.module_port(buffer_module, module_input_port_id).get_width());
    VTR_ASSERT(1 == module_manager.module_port(buffer_module, module_output_port_id).get_width());

    port2port_name_map[module_manager.module_port(buffer_module, module_input_port_id).get_name()] = BasicPort(rc_ntwk_output_port_name, 1);
    port2port_name_map[module_manager.module_port(buffer_module, module_output_port_id).get_name()] = module_manager.module_port(module_id, wire_module_output_port);

    print_spice_subckt_instance(fp, 
                                module_manager, 
                                buffer_module,
                                instance_name,
                                port2port_name_map);

    if (!rc_ntwk_middle_output_port_name.empty()) {
      instance_name = std::string("middle_output_buffer");
      port2port_name_map[module_manager.module_port(buffer_module, module_output_port_id).get_name()] = module_manager.module_port(module_id, wire_module_middle_output_port);

    print_spice_subckt_instance(fp, 
                                module_manager, 
                                buffer_module,
                                instance_name,
                                port2port_name_map);
    }
  } else {
    print_spice_short_connection(fp, rc_ntwk_output_port_name, circuit_lib.port_prefix(output_ports[0])); 
    if (!rc_ntwk_middle_output_port_name.empty()) {
      print_spice_short_connection(fp, rc_ntwk_middle_output_port_name, circuit_lib.port_prefix(output_ports[0])); 
    }
  }

  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  return status;
}

} /* end namespace openfpga */
