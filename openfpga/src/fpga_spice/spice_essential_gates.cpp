/************************************************
 * This file includes functions on 
 * outputting Verilog netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
 ***********************************************/
#include <fstream>
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
#include "spice_essential_gates.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************
 * Print a SPICE model wrapper for a transistor model
 ***********************************************/
static 
int print_spice_transistor_model_wrapper(std::fstream& fp,
                                         const TechnologyLibrary& tech_lib,
                                         const TechnologyModelId& model) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Transistor model followed a fixed port mapping
   * [X|M]<MODEL_CARD_NAME> <DRAIN> <GATE> <SOURCE> <BULK> 
   * which is a standard in SPICE modeling
   * We will output the pmos and nmos transistors wrappers
   * which are defined in this model
   */
  for (int itype = TECH_LIB_TRANSISTOR_PMOS;
       itype < NUM_TECH_LIB_TRANSISTOR_TYPES;
       ++itype) {
    const e_tech_lib_transistor_type& trans_type = static_cast<e_tech_lib_transistor_type>(itype); 
    fp << ".subckt ";
    fp << tech_lib.transistor_model_name(model, trans_type) << TRANSISTOR_WRAPPER_POSTFIX; 
    fp << " drain gate source bulk";
    fp << " L=" << std::setprecision(10) << tech_lib.transistor_model_chan_length(model, trans_type); 
    fp << " W=" << std::setprecision(10) << tech_lib.transistor_model_min_width(model, trans_type); 
    fp << "\n";

    fp << tech_lib.model_ref(model);
    fp << "1";
    fp << " drain gate source bulk";
    fp << " " << tech_lib.transistor_model_name(model, trans_type);
    fp << " L=L W=W";
    fp << "\n";

    fp << ".ends";
    fp << "\n";
  }

  return CMD_EXEC_SUCCESS;
}

/************************************************
 * Generate the SPICE netlist for transistors
 ***********************************************/
int print_spice_transistor_wrapper(NetlistManager& netlist_manager,
                                   const TechnologyLibrary& tech_lib,
                                   const std::string& submodule_dir) {
  std::string spice_fname = submodule_dir + std::string(TRANSISTORS_SPICE_FILE_NAME);

  std::fstream fp;

  /* Create the file stream */
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_stream(spice_fname.c_str(), fp); 

  /* Create file */
  VTR_LOG("Generating SPICE netlist '%s' for essential gates...",
          spice_fname.c_str()); 

  /* Iterate over the transistor models */
  for (const TechnologyModelId& model : tech_lib.models()) {
    /* Focus on transistor model */
    if (TECH_LIB_MODEL_TRANSISTOR != tech_lib.model_type(model)) {
      continue;
    }
    /* Write a wrapper for the transistor model */
    if (CMD_EXEC_SUCCESS == print_spice_transistor_model_wrapper(fp, tech_lib, model)) {
      return CMD_EXEC_FATAL_ERROR;
    }
  } 

  /* Close file handler*/
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");

  return CMD_EXEC_SUCCESS;
}

/************************************************
 * Generate the SPICE subckt for a power gated inverter
 * The Enable signal controlled the power gating
 * Schematic
 *          LVDD
 *            |
 *           -
 *   ENb -o||
 *           -
 *            |
 *           -
 *      +-o||
 *      |    -
 *      |     |
 * in-->+     +--> OUT
 *      |     |
 *      |    -
 *      +--||
 *           -
 *            |
 *           -
 *     EN -||
 *           -
 *            |
 *          LGND
 *
 ***********************************************/
static 
int print_spice_powergated_inverter_subckt(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& module_id,
                                           const CircuitLibrary& circuit_lib,
                                           const CircuitModelId& circuit_model,
                                           const TechnologyLibrary& tech_lib,
                                           const TechnologyModelId& tech_model) {
  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Print the inverter subckt definition */
  print_spice_subckt_definition(fp, module_manager, module_id); 

  /* Find the input and output ports:
   * we do NOT support global ports here, 
   * it should be handled in another type of inverter subckt (power-gated)
   */
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);

  /* Make sure:
   * There is only 1 input port and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == input_ports.size()) && (1 == circuit_lib.port_size(input_ports[0])) );
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  /* If the circuit model is power-gated, we need to find at least one global config_enable signals */
  VTR_ASSERT(true == circuit_lib.is_power_gated(circuit_model));
  CircuitPortId en_port = find_circuit_model_power_gate_en_port(circuit_lib, circuit_model);
  CircuitPortId enb_port = find_circuit_model_power_gate_enb_port(circuit_lib, circuit_model);
  VTR_ASSERT(true == circuit_lib.valid_circuit_port_id(en_port));
  VTR_ASSERT(true == circuit_lib.valid_circuit_port_id(enb_port));

  /* TODO: may consider use size/bin to compact layout etc. */
  for (size_t i = 0; i < circuit_lib.buffer_size(circuit_model); ++i) { 
    /* Write power-gateing transistor pairs using the technology model */
    fp << "Xpmos_powergate_" << i << " ";
    fp << circuit_lib.port_prefix(output_ports[0]) << "_pmos_pg "; 
    fp << circuit_lib.port_prefix(enb_port) << " "; 
    fp << "LVDD "; 
    fp << "LVDD "; 
    fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_PMOS) << TRANSISTOR_WRAPPER_POSTFIX; 

    fp << "Xnmos_powergate_" << i << " ";
    fp << circuit_lib.port_prefix(output_ports[0]) << " _nmos_pg "; 
    fp << circuit_lib.port_prefix(en_port) << " "; 
    fp << "LGND "; 
    fp << "LGND "; 
    fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_NMOS) << TRANSISTOR_WRAPPER_POSTFIX; 

    /* Write transistor pairs using the technology model */
    fp << "Xpmos_" << i << " ";
    fp << circuit_lib.port_prefix(output_ports[0]) << " "; 
    fp << circuit_lib.port_prefix(input_ports[0]) << " "; 
    fp << circuit_lib.port_prefix(output_ports[0]) << "_pmos_pg "; 
    fp << "LVDD "; 
    fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_PMOS) << TRANSISTOR_WRAPPER_POSTFIX; 

    fp << "Xnmos_" << i << " ";
    fp << circuit_lib.port_prefix(output_ports[0]) << " "; 
    fp << circuit_lib.port_prefix(input_ports[0]) << " "; 
    fp << circuit_lib.port_prefix(output_ports[0]) << " _nmos_pg "; 
    fp << "LGND "; 
    fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_NMOS) << TRANSISTOR_WRAPPER_POSTFIX; 
  }

  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  return CMD_EXEC_SUCCESS;
}

/************************************************
 * Generate the SPICE subckt for a regular inverter
 * Schematic
 *          LVDD
 *            |
 *           -
 *      +-o||
 *      |    -
 *      |     |
 * in-->+     +--> OUT
 *      |     |
 *      |    -
 *      +--||
 *           -
 *            |
 *          LGND
 *
 ***********************************************/
static 
int print_spice_regular_inverter_subckt(std::fstream& fp,
                                        const ModuleManager& module_manager,
                                        const ModuleId& module_id,
                                        const CircuitLibrary& circuit_lib,
                                        const CircuitModelId& circuit_model,
                                        const TechnologyLibrary& tech_lib,
                                        const TechnologyModelId& tech_model) {
  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Print the inverter subckt definition */
  print_spice_subckt_definition(fp, module_manager, module_id); 

  /* Find the input and output ports:
   * we do NOT support global ports here, 
   * it should be handled in another type of inverter subckt (power-gated)
   */
  std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);

  /* Make sure:
   * There is only 1 input port and 1 output port, 
   * each size of which is 1
   */
  VTR_ASSERT( (1 == input_ports.size()) && (1 == circuit_lib.port_size(input_ports[0])) );
  VTR_ASSERT( (1 == output_ports.size()) && (1 == circuit_lib.port_size(output_ports[0])) );

  /* TODO: may consider use size/bin to compact layout etc. */
  for (size_t i = 0; i < circuit_lib.buffer_size(circuit_model); ++i) { 
    /* Write transistor pairs using the technology model */
    fp << "Xpmos_" << i << " ";
    fp << circuit_lib.port_prefix(output_ports[0]) << " "; 
    fp << circuit_lib.port_prefix(input_ports[0]) << " "; 
    fp << "LVDD "; 
    fp << "LVDD "; 
    fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_PMOS) << TRANSISTOR_WRAPPER_POSTFIX; 

    fp << "Xnmos_" << i << " ";
    fp << circuit_lib.port_prefix(output_ports[0]) << " "; 
    fp << circuit_lib.port_prefix(input_ports[0]) << " "; 
    fp << "LGND "; 
    fp << "LGND "; 
    fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_NMOS) << TRANSISTOR_WRAPPER_POSTFIX; 
  }

  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  return CMD_EXEC_SUCCESS;
}

/************************************************
 * Generate the SPICE subckt for an inverter
 * Branch on the different circuit topologies
 ***********************************************/
static 
int print_spice_inverter_subckt(std::fstream& fp,
                                const ModuleManager& module_manager,
                                const ModuleId& module_id,
                                const CircuitLibrary& circuit_lib,
                                const CircuitModelId& circuit_model,
                                const TechnologyLibrary& tech_lib,
                                const TechnologyModelId& tech_model) {
  int status = CMD_EXEC_SUCCESS;
  if (true == circuit_lib.is_power_gated(circuit_model)) {
    status = print_spice_powergated_inverter_subckt(fp,
                                                    module_manager, module_id,
                                                    circuit_lib, circuit_model,
                                                    tech_lib, tech_model);
  } else { 
    VTR_ASSERT_SAFE(false == circuit_lib.is_power_gated(circuit_model));
    status = print_spice_regular_inverter_subckt(fp,
                                                 module_manager, module_id,
                                                 circuit_lib, circuit_model,
                                                 tech_lib, tech_model);
  }
 
  return status;
}

/************************************************
 * Generate the SPICE netlist for essential gates:
 * - inverters and their templates
 * - buffers and their templates
 * - pass-transistor or transmission gates
 * - logic gates
 ***********************************************/
int print_spice_essential_gates(NetlistManager& netlist_manager,
                                const ModuleManager& module_manager,
                                const CircuitLibrary& circuit_lib,
                                const TechnologyLibrary& tech_lib,
                                const std::map<CircuitModelId, TechnologyModelId>& circuit_tech_binding,
                                const std::string& submodule_dir) {
  std::string spice_fname = submodule_dir + std::string(ESSENTIALS_SPICE_FILE_NAME);

  std::fstream fp;

  /* Create the file stream */
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_stream(spice_fname.c_str(), fp); 

  /* Create file */
  VTR_LOG("Generating SPICE netlist '%s' for transistor wrappers...",
          spice_fname.c_str()); 

  int status = CMD_EXEC_SUCCESS;

  /* Iterate over the circuit models */
  for (const CircuitModelId& circuit_model : circuit_lib.models()) {
    /* Bypass models require extern netlists */
    if (true == circuit_lib.model_circuit_netlist(circuit_model).empty()) {
      continue;
    }

    /* Spot module id */
    const ModuleId& module_id = module_manager.find_module(circuit_lib.model_name(circuit_model));

    TechnologyModelId tech_model; 
    /* Focus on inverter/buffer/pass-gate/logic gates only */
    if ( (CIRCUIT_MODEL_INVBUF == circuit_lib.model_type(circuit_model))
      || (CIRCUIT_MODEL_PASSGATE == circuit_lib.model_type(circuit_model))
      || (CIRCUIT_MODEL_GATE == circuit_lib.model_type(circuit_model))) {
      auto result = circuit_tech_binding.find(circuit_model);
      if (result == circuit_tech_binding.end()) {
        VTR_LOGF_ERROR(__FILE__, __LINE__,
                       "Unable to find technology binding for circuit model '%s'!\n",
                       circuit_lib.model_name(circuit_model).c_str()); 
        return CMD_EXEC_FATAL_ERROR;
      }
      /* Valid technology binding. Assign techology model */
      tech_model = result->second;
      /* Ensure we have a valid technology model */
      VTR_ASSERT(true == tech_lib.valid_model_id(tech_model));
      VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == tech_lib.model_type(tech_model));
    }

    /* Now branch on netlist writing */
    if (CIRCUIT_MODEL_INVBUF == circuit_lib.model_type(circuit_model)) {
      if (CIRCUIT_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model)) {
        VTR_ASSERT(true == module_manager.valid_module_id(module_id));
        status = print_spice_inverter_subckt(fp,
                                             module_manager, module_id,
                                             circuit_lib, circuit_model,
                                             tech_lib, tech_model);
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_BUF_BUF == circuit_lib.buffer_type(circuit_model));
      }

      if (CMD_EXEC_FATAL_ERROR == status) {
        break;
      }

      /* Finish, go to the next */
      continue;
    }
  } 

  /* Close file handler*/
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");

  return status;
}

} /* end namespace openfpga */
