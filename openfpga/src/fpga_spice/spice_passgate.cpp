/************************************************
 * This file includes functions on 
 * outputting SPICE netlists for transmission-gates
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
#include "spice_passgate.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate the SPICE modeling for the PMOS part of a pass-gate logic
 *
 * This function is created to be shared by pass-transistor and
 * transmission-gate SPICE netlist writer
 *
 * Note: 
 * - This function does NOT create a file
 *   but requires a file stream created
 * - This function only output SPICE modeling for 
 *   a pass-gate. Any preprocessing or subckt definition should not be included!
 *******************************************************************/
static 
int print_spice_passgate_pmos_modeling(std::fstream& fp,
                                       const std::string& trans_name_postfix,
                                       const std::string& input_port_name,
                                       const std::string& gate_port_name,
                                       const std::string& output_port_name,
                                       const TechnologyLibrary& tech_lib,
                                       const TechnologyModelId& tech_model,
                                       const float& trans_width) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Write transistor pairs using the technology model */
  fp << "Xpmos_" << trans_name_postfix << " ";
  fp << input_port_name << " "; 
  fp << gate_port_name << " "; 
  fp << output_port_name << " "; 
  fp << "LVDD "; 
  fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_PMOS) << TRANSISTOR_WRAPPER_POSTFIX; 
  fp << " W=" << std::setprecision(10) << trans_width;
  fp << "\n";

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Generate the SPICE modeling for the NMOS part of a pass-gate logic
 *
 * This function is created to be shared by pass-transistor and
 * transmission-gate SPICE netlist writer
 *
 * Note: 
 * - This function does NOT create a file
 *   but requires a file stream created
 * - This function only output SPICE modeling for 
 *   a pass-gate. Any preprocessing or subckt definition should not be included!
 *******************************************************************/
static 
int print_spice_passgate_nmos_modeling(std::fstream& fp,
                                       const std::string& trans_name_postfix,
                                       const std::string& input_port_name,
                                       const std::string& gate_port_name,
                                       const std::string& output_port_name,
                                       const TechnologyLibrary& tech_lib,
                                       const TechnologyModelId& tech_model,
                                       const float& trans_width) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  fp << "Xnmos_" << trans_name_postfix << " ";
  fp << input_port_name << " "; 
  fp << gate_port_name << " "; 
  fp << output_port_name << " "; 
  fp << "LGND "; 
  fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_NMOS) << TRANSISTOR_WRAPPER_POSTFIX; 
  fp << " W=" << std::setprecision(10) << trans_width;
  fp << "\n";

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Generate the SPICE subckt for a pass-transistor
 *
 * Schematic
 *
 *        sel
 *         |
 *        ===
 *        | |
 *   in --  ---out
 *
 *******************************************************************/
static 
int print_spice_pass_transistor_subckt(std::fstream& fp,
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
   * There is only 2 input port and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT(2 == input_ports.size());
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
  float regular_nmos_bin_width = tech_lib.transistor_model_max_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  float total_nmos_width = circuit_lib.pass_gate_logic_nmos_size(circuit_model)
                           * tech_lib.transistor_model_min_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  int num_nmos_bins = std::ceil(total_nmos_width / regular_nmos_bin_width);
  float last_nmos_bin_width = std::fmod(total_nmos_width, regular_nmos_bin_width);

  for (int ibin = 0; ibin < num_nmos_bins; ++ibin) { 
    float curr_bin_width = regular_nmos_bin_width;
    /* For last bin, we need an irregular width */
    if ((ibin == num_nmos_bins - 1) 
       && (0. != last_nmos_bin_width)) {
      curr_bin_width = last_nmos_bin_width;
    }

    status = print_spice_passgate_nmos_modeling(fp,
                                                std::to_string(ibin),
                                                circuit_lib.port_prefix(input_ports[0]), 
                                                circuit_lib.port_prefix(input_ports[1]), 
                                                circuit_lib.port_prefix(output_ports[0]), 
                                                tech_lib,
                                                tech_model,
                                                curr_bin_width);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
  }

  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  return status;
}

/********************************************************************
 * Generate the SPICE subckt for a transmission gate
 *
 * Schematic
 * 
 *        selb
 *         |
 *         o
 *        ===
 *        | |
 *   in --  ---out
 *        | |
 *        ===
 *         |
 *        sel
 *
 *******************************************************************/
static 
int print_spice_transmission_gate_subckt(std::fstream& fp,
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
   * There is only 3 input port and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT(3 == input_ports.size());
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
  float total_pmos_width = circuit_lib.pass_gate_logic_pmos_size(circuit_model)
                           * tech_lib.transistor_model_min_width(tech_model, TECH_LIB_TRANSISTOR_PMOS);
  int num_pmos_bins = std::ceil(total_pmos_width / regular_pmos_bin_width);
  float last_pmos_bin_width = std::fmod(total_pmos_width, regular_pmos_bin_width);
  for (int ibin = 0; ibin < num_pmos_bins; ++ibin) { 
    float curr_bin_width = regular_pmos_bin_width;
    /* For last bin, we need an irregular width */
    if ((ibin == num_pmos_bins - 1) 
       && (0. != last_pmos_bin_width)) {
      curr_bin_width = last_pmos_bin_width;
    }

    status = print_spice_passgate_pmos_modeling(fp,
                                                std::to_string(ibin),
                                                circuit_lib.port_prefix(input_ports[0]), 
                                                circuit_lib.port_prefix(input_ports[2]), 
                                                circuit_lib.port_prefix(output_ports[0]), 
                                                tech_lib,
                                                tech_model,
                                                curr_bin_width);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
  }

  /* Consider use size/bin to compact layout:
   * Try to size transistors to the max width for each bin
   * The last bin may not reach the max width 
   */
  float regular_nmos_bin_width = tech_lib.transistor_model_max_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  float total_nmos_width = circuit_lib.pass_gate_logic_nmos_size(circuit_model)
                           * tech_lib.transistor_model_min_width(tech_model, TECH_LIB_TRANSISTOR_NMOS);
  int num_nmos_bins = std::ceil(total_nmos_width / regular_nmos_bin_width);
  float last_nmos_bin_width = std::fmod(total_nmos_width, regular_nmos_bin_width);

  for (int ibin = 0; ibin < num_nmos_bins; ++ibin) { 
    float curr_bin_width = regular_nmos_bin_width;
    /* For last bin, we need an irregular width */
    if ((ibin == num_nmos_bins - 1) 
       && (0. != last_nmos_bin_width)) {
      curr_bin_width = last_nmos_bin_width;
    }

    status = print_spice_passgate_nmos_modeling(fp,
                                                std::to_string(ibin),
                                                circuit_lib.port_prefix(input_ports[0]), 
                                                circuit_lib.port_prefix(input_ports[1]), 
                                                circuit_lib.port_prefix(output_ports[0]), 
                                                tech_lib,
                                                tech_model,
                                                curr_bin_width);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
  }

  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  return status;
}

/********************************************************************
 * Generate the SPICE subckt for a pass-gate
 *
 * Note: 
 * - This function supports both pass-transistor
 *   and transmission gates
 *******************************************************************/
int print_spice_passgate_subckt(std::fstream& fp,
                                const ModuleManager& module_manager,
                                const ModuleId& module_id,
                                const CircuitLibrary& circuit_lib,
                                const CircuitModelId& circuit_model,
                                const TechnologyLibrary& tech_lib,
                                const TechnologyModelId& tech_model) {
  int status = CMD_EXEC_SUCCESS;

  if (CIRCUIT_MODEL_PASS_GATE_TRANSISTOR == circuit_lib.pass_gate_logic_type(circuit_model)) {
    status = print_spice_pass_transistor_subckt(fp,
                                                module_manager, module_id,
                                                circuit_lib, circuit_model,
                                                tech_lib, tech_model);
  } else if (CIRCUIT_MODEL_PASS_GATE_TRANSMISSION == circuit_lib.is_power_gated(circuit_model)) {
    status = print_spice_transmission_gate_subckt(fp,
                                                  module_manager, module_id,
                                                  circuit_lib, circuit_model,
                                                  tech_lib, tech_model);
  }
 
  return status;
}

} /* end namespace openfpga */
