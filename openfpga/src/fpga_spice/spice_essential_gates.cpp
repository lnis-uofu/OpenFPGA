/************************************************
 * This file includes functions on 
 * outputting SPICE netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
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
#include "spice_buffer.h"
#include "spice_passgate.h"
#include "spice_logic_gate.h"
#include "spice_wire.h"
#include "spice_essential_gates.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate the SPICE netlist for essential gates:
 * - inverters and their templates
 * - buffers and their templates
 * - pass-transistor or transmission gates
 * - logic gates
 *******************************************************************/
int print_spice_essential_gates(NetlistManager& netlist_manager,
                                const ModuleManager& module_manager,
                                const CircuitLibrary& circuit_lib,
                                const TechnologyLibrary& tech_lib,
                                const std::map<CircuitModelId, TechnologyModelId>& circuit_tech_binding,
                                const std::string& submodule_dir) {
  int status = CMD_EXEC_SUCCESS;

  /* Iterate over the circuit models */
  for (const CircuitModelId& circuit_model : circuit_lib.models()) {
    /* Bypass models require extern netlists */
    if (!circuit_lib.model_circuit_netlist(circuit_model).empty()) {
      continue;
    }

    /* Output only the model type is supported in auto-generation */
    if ( (CIRCUIT_MODEL_INVBUF != circuit_lib.model_type(circuit_model))
      && (CIRCUIT_MODEL_PASSGATE != circuit_lib.model_type(circuit_model))
      && (CIRCUIT_MODEL_WIRE != circuit_lib.model_type(circuit_model))
      && (CIRCUIT_MODEL_GATE != circuit_lib.model_type(circuit_model))) {
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

    /* Create file stream */
    std::string spice_fname = submodule_dir + circuit_lib.model_name(circuit_model) + std::string(SPICE_NETLIST_FILE_POSTFIX);
  
    std::fstream fp;
  
    /* Create the file stream */
    fp.open(spice_fname, std::fstream::out | std::fstream::trunc);
    /* Check if the file stream if valid or not */
    check_file_stream(spice_fname.c_str(), fp); 
  
    /* Create file */
    VTR_LOG("Generating SPICE netlist '%s' for circuit model '%s'...",
            spice_fname.c_str(),
            circuit_lib.model_name(circuit_model).c_str());
  
    print_spice_file_header(fp, circuit_lib.model_name(circuit_model));

    /* A flag to record if any logic has been filled to the netlist */
    bool netlist_filled = false;

    /* Now branch on netlist writing: for inverter/buffers */
    if (CIRCUIT_MODEL_INVBUF == circuit_lib.model_type(circuit_model)) {
      if (CIRCUIT_MODEL_BUF_INV == circuit_lib.buffer_type(circuit_model)) {
        VTR_ASSERT(true == module_manager.valid_module_id(module_id));
        status = print_spice_inverter_subckt(fp,
                                             module_manager, module_id,
                                             circuit_lib, circuit_model,
                                             tech_lib, tech_model);
        netlist_filled = true;
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_BUF_BUF == circuit_lib.buffer_type(circuit_model));
        status = print_spice_buffer_subckt(fp,
                                           module_manager, module_id,
                                           circuit_lib, circuit_model,
                                           tech_lib, tech_model);
        netlist_filled = true;
      }

      if (CMD_EXEC_FATAL_ERROR == status) {
        break;
      }
    }

    /* Now branch on netlist writing: for pass-gate logic */
    if (CIRCUIT_MODEL_PASSGATE == circuit_lib.model_type(circuit_model)) {
      status = print_spice_passgate_subckt(fp,
                                           module_manager, module_id,
                                           circuit_lib, circuit_model,
                                           tech_lib, tech_model);
      netlist_filled = true;

      if (CMD_EXEC_FATAL_ERROR == status) {
        break;
      }
    }

    /* Now branch on netlist writing: for logic gate */
    if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(circuit_model)) {
      if (CIRCUIT_MODEL_GATE_AND == circuit_lib.gate_type(circuit_model)) {
        status = print_spice_and_gate_subckt(fp,
                                             module_manager, module_id,
                                             circuit_lib, circuit_model,
                                             tech_lib, tech_model);
        netlist_filled = true;
      } else if (CIRCUIT_MODEL_GATE_OR == circuit_lib.gate_type(circuit_model)) {
        status = print_spice_or_gate_subckt(fp,
                                            module_manager, module_id,
                                            circuit_lib, circuit_model,
                                            tech_lib, tech_model);
        netlist_filled = true;
      }

      if (CMD_EXEC_FATAL_ERROR == status) {
        break;
      }
    }

    /* Now branch on netlist writing: for wires */
    if (CIRCUIT_MODEL_WIRE == circuit_lib.model_type(circuit_model)) {
      status = print_spice_wire_subckt(fp,
                                       module_manager, module_id,
                                       circuit_lib, circuit_model);
      netlist_filled = true;
      if (CMD_EXEC_FATAL_ERROR == status) {
        break;
      }
    }

    /* Check if the netlist has been filled or not.
     * If not, flag a fatal error
     */
    if (false == netlist_filled) {
      VTR_LOG_ERROR("Cannot auto-generate netlist for circuit model '%s'!\n\tThe circuit topology is not supported yet!\n",
                    circuit_lib.model_name(circuit_model).c_str());
      status = CMD_EXEC_FATAL_ERROR; 
      break;
    }

    /* Close file handler*/
    fp.close();

    /* Add fname to the netlist name list */
    NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
    VTR_ASSERT(NetlistId::INVALID() != nlist_id);
    netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

    VTR_LOG("Done\n");
  } 

  return status;
}

} /* end namespace openfpga */
