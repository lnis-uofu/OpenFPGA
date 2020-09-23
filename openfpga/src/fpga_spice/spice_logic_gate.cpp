/************************************************
 * This file includes functions on 
 * outputting SPICE netlists for logic gates:
 * - N-input AND gate
 * - N-input OR gate
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

#include "spice_constants.h"
#include "spice_writer_utils.h"
#include "spice_transistor_wrapper.h"
#include "spice_logic_gate.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate the SPICE subckt for a N-input AND gate
 *
 * Schematic
 *
 *           VDD        VDD                 VDD
 *            |          |                   |
 *           -          -                   -
 *   in0 -o||   in1 -o||    ... in[N-1] -o||
 *           -          -                   -
 *            |          |                   |
 *            +----+-----+- ... -------------+
 *                 |
 *                -
 *         in0 -||
 *                -
 *                 |
 *                -
 *         in1 -||
 *                -
 *                 |
 *                ...
 *                 |
 *                -
 *     in[N-1] -||
 *                -
 *                 |
 *                GND
 *******************************************************************/
int print_spice_and_gate_subckt(std::fstream& fp,
                                const ModuleManager& module_manager,
                                const ModuleId& module_id,
                                const CircuitLibrary& circuit_lib,
                                const CircuitModelId& circuit_model,
                                const TechnologyLibrary& tech_lib,
                                const TechnologyModelId& tech_model) {

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
   * There are at least 2 input ports and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT(2 <= input_ports.size());
  for (const auto& input_port : input_ports) {
     VTR_ASSERT(1 == circuit_lib.port_size(input_port));
  }

  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  int status = CMD_EXEC_SUCCESS;

  /* Print the inverter subckt definition */
  print_spice_subckt_definition(fp, module_manager, module_id); 

  /* Consider use size/bin to compact layout:
   * Try to size transistors to the max width for each bin
   * The last bin may not reach the max width 
   */
  float regular_pmos_bin_width = tech_lib.transistor_model_max_width(tech_model, TECH_LIB_TRANSISTOR_PMOS);
  float total_pmos_width = 1. /* TODO: allow users to define gate strength */
                           * tech_lib.model_pn_ratio(tech_model)
                           * tech_lib.transistor_model_min_width(tech_model, TECH_LIB_TRANSISTOR_PMOS);
  int num_pmos_bins = std::ceil(total_pmos_width / regular_pmos_bin_width);
  float last_pmos_bin_width = std::fmod(total_pmos_width, regular_pmos_bin_width);


  /* Output the PMOS network */
  for (const auto& input_port : input_ports) {
    for (int ibin = 0; ibin < num_pmos_bins; ++ibin) { 
      float curr_bin_width = regular_pmos_bin_width;
      /* For last bin, we need an irregular width */
      if ((ibin == num_pmos_bins - 1) 
         && (0. != last_pmos_bin_width)) {
        curr_bin_width = last_pmos_bin_width;
      }

      status = print_spice_generic_pmos_modeling(fp,
                                                 std::to_string(ibin),
                                                 std::string(SPICE_SUBCKT_VDD_PORT_NAME), 
                                                 circuit_lib.port_prefix(input_port), 
                                                 circuit_lib.port_prefix(output_ports[0]), 
                                                 tech_lib,
                                                 tech_model,
                                                 curr_bin_width);
      if (CMD_EXEC_FATAL_ERROR == status) {
        return status;
      }
    }
  }

  /* Consider use size/bin to compact layout:
   * Try to size transistors to the max width for each bin
   * The last bin may not reach the max width 
   */
  float regular_nmos_bin_width = tech_lib.transistor_model_max_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  float total_nmos_width = 1. /* TODO: allow users to define gate strength */
                           * tech_lib.transistor_model_min_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  int num_nmos_bins = std::ceil(total_nmos_width / regular_nmos_bin_width);
  float last_nmos_bin_width = std::fmod(total_nmos_width, regular_nmos_bin_width);

  /* Output the NMOS network */
  for (size_t input_id = 0; input_id < input_ports.size(); ++input_id) {
    for (int ibin = 0; ibin < num_nmos_bins; ++ibin) { 
      float curr_bin_width = regular_nmos_bin_width;
      /* For last bin, we need an irregular width */
      if ((ibin == num_nmos_bins - 1) 
         && (0. != last_nmos_bin_width)) {
        curr_bin_width = last_nmos_bin_width;
      }

      /* Depending on the input id, we assign different port names to source/drain */
      std::string source_port_name;
      std::string drain_port_name;

      if (0 == input_id) {
        /* First transistor should connect to the output port and an internal node */
        source_port_name = circuit_lib.port_prefix(output_ports[0]);
        drain_port_name = std::string("internal_node") + std::to_string(input_id);
      } else if (input_id == input_ports.size() - 1) {
        /* Last transistor should connect to an internal node and GND */
        source_port_name = std::string("internal_node") + std::to_string(input_id - 1);
        drain_port_name = std::string(SPICE_SUBCKT_GND_PORT_NAME);
      } else {
        /* Other transistors should connect to two internal nodes */ 
        source_port_name = std::string("internal_node") + std::to_string(input_id - 1);
        drain_port_name = std::string("internal_node") + std::to_string(input_id);
      }

      status = print_spice_generic_nmos_modeling(fp,
                                                 std::to_string(ibin),
                                                 source_port_name, 
                                                 circuit_lib.port_prefix(input_ports[input_id]), 
                                                 drain_port_name, 
                                                 tech_lib,
                                                 tech_model,
                                                 curr_bin_width);
      if (CMD_EXEC_FATAL_ERROR == status) {
        return status;
      }
    }
  }

  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  return status;
}

/********************************************************************
 * Generate the SPICE subckt for a N-input OR gate
 *
 * Schematic
 *
 *
 *                VDD
 *                 |
 *                -
 *        in0 -o||
 *                -
 *                 |
 *                -
 *        in1 -o||
 *                -
 *                 |
 *                ...
 *                 |
 *                -
 *    in[N-1] -o||
 *                -
 *                 |
 *            +----+-----+- ... -------------+
 *            |          |                   |
 *           -          -                   -
 *    in0 -||    in1 -||    ...  in[N-1] -||
 *           -          -                   -
 *            |          |                   |
 *           GND        GND                 GND
 *******************************************************************/
int print_spice_or_gate_subckt(std::fstream& fp,
                               const ModuleManager& module_manager,
                               const ModuleId& module_id,
                               const CircuitLibrary& circuit_lib,
                               const CircuitModelId& circuit_model,
                               const TechnologyLibrary& tech_lib,
                               const TechnologyModelId& tech_model) {

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
   * There are at least 2 input ports and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT(2 <= input_ports.size());
  for (const auto& input_port : input_ports) {
     VTR_ASSERT(1 == circuit_lib.port_size(input_port));
  }

  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  int status = CMD_EXEC_SUCCESS;

  /* Print the inverter subckt definition */
  print_spice_subckt_definition(fp, module_manager, module_id); 

  /* Consider use size/bin to compact layout:
   * Try to size transistors to the max width for each bin
   * The last bin may not reach the max width 
   */
  float regular_pmos_bin_width = tech_lib.transistor_model_max_width(tech_model, TECH_LIB_TRANSISTOR_PMOS);
  float total_pmos_width = 1. /* TODO: allow users to define gate strength */
                           * tech_lib.model_pn_ratio(tech_model)
                           * tech_lib.transistor_model_min_width(tech_model, TECH_LIB_TRANSISTOR_PMOS);
  int num_pmos_bins = std::ceil(total_pmos_width / regular_pmos_bin_width);
  float last_pmos_bin_width = std::fmod(total_pmos_width, regular_pmos_bin_width);


  /* Output the PMOS network */
  for (size_t input_id = 0; input_id < input_ports.size(); ++input_id) {
    for (int ibin = 0; ibin < num_pmos_bins; ++ibin) { 
      float curr_bin_width = regular_pmos_bin_width;
      /* For last bin, we need an irregular width */
      if ((ibin == num_pmos_bins - 1) 
         && (0. != last_pmos_bin_width)) {
        curr_bin_width = last_pmos_bin_width;
      }

      /* Depending on the input id, we assign different port names to source/drain */
      std::string source_port_name;
      std::string drain_port_name;

      if (0 == input_id) {
        /* First transistor should connect to the output port and an internal node */
        source_port_name = circuit_lib.port_prefix(output_ports[0]);
        drain_port_name = std::string("internal_node") + std::to_string(input_id);
      } else if (input_id == input_ports.size() - 1) {
        /* Last transistor should connect to an internal node and GND */
        source_port_name = std::string("internal_node") + std::to_string(input_id - 1);
        drain_port_name = std::string(SPICE_SUBCKT_VDD_PORT_NAME);
      } else {
        /* Other transistors should connect to two internal nodes */ 
        source_port_name = std::string("internal_node") + std::to_string(input_id - 1);
        drain_port_name = std::string("internal_node") + std::to_string(input_id);
      }

      status = print_spice_generic_pmos_modeling(fp,
                                                 std::to_string(ibin),
                                                 source_port_name, 
                                                 circuit_lib.port_prefix(input_ports[input_id]), 
                                                 drain_port_name, 
                                                 tech_lib,
                                                 tech_model,
                                                 curr_bin_width);
      if (CMD_EXEC_FATAL_ERROR == status) {
        return status;
      }
    }
  }

  /* Consider use size/bin to compact layout:
   * Try to size transistors to the max width for each bin
   * The last bin may not reach the max width 
   */
  float regular_nmos_bin_width = tech_lib.transistor_model_max_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  float total_nmos_width = 1. /* TODO: allow users to define gate strength */
                           * tech_lib.transistor_model_min_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  int num_nmos_bins = std::ceil(total_nmos_width / regular_nmos_bin_width);
  float last_nmos_bin_width = std::fmod(total_nmos_width, regular_nmos_bin_width);

  /* Output the NMOS network */
  for (const auto& input_port : input_ports) {
    for (int ibin = 0; ibin < num_nmos_bins; ++ibin) { 
      float curr_bin_width = regular_nmos_bin_width;
      /* For last bin, we need an irregular width */
      if ((ibin == num_nmos_bins - 1) 
         && (0. != last_nmos_bin_width)) {
        curr_bin_width = last_nmos_bin_width;
      }

      status = print_spice_generic_nmos_modeling(fp,
                                                 std::to_string(ibin),
                                                 circuit_lib.port_prefix(output_ports[0]), 
                                                 circuit_lib.port_prefix(input_port), 
                                                 std::string(SPICE_SUBCKT_GND_PORT_NAME), 
                                                 tech_lib,
                                                 tech_model,
                                                 curr_bin_width);
      if (CMD_EXEC_FATAL_ERROR == status) {
        return status;
      }
    }
  }

  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  return status;
}

} /* end namespace openfpga */
