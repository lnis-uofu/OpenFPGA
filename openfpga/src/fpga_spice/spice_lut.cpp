/********************************************************************
 * This file includes functions to generate SPICE subcircuits for LUTs
 ********************************************************************/
#include <string>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "mux_graph.h"
#include "module_manager.h"
#include "mux_utils.h"

#include "openfpga_naming.h"

#include "spice_constants.h"
#include "spice_writer_utils.h"
#include "spice_subckt_writer.h"
#include "spice_lut.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************** 
 * Print SPICE modules for the Look-Up Tables (LUTs) 
 * in the circuit library
 ********************************************************************/
int print_spice_submodule_luts(NetlistManager& netlist_manager,
                               const ModuleManager& module_manager,
                               const CircuitLibrary& circuit_lib,
                               const std::string& submodule_dir) {
  int status = CMD_EXEC_SUCCESS;

  std::string spice_fname = submodule_dir + std::string(LUTS_SPICE_FILE_NAME);

  std::fstream fp;

  /* Create the file stream */
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_stream(spice_fname.c_str(), fp); 

  /* Create file */
  VTR_LOG("Writing SPICE netlist for LUTs '%s'...",
          spice_fname.c_str()); 

  print_spice_file_header(fp, "Look-Up Tables"); 

  /* Search for each LUT circuit model */
  for (const auto& lut_model : circuit_lib.models()) {
    /* Bypass user-defined and non-LUT modules */
    if ( (!circuit_lib.model_circuit_netlist(lut_model).empty()) 
      || (CIRCUIT_MODEL_LUT != circuit_lib.model_type(lut_model)) ) {
      continue;
    }
    /* Find the module id */
    ModuleId lut_module = module_manager.find_module(circuit_lib.model_name(lut_model));
    VTR_ASSERT(true == module_manager.valid_module_id(lut_module));
    write_spice_subckt_to_file(fp, module_manager, lut_module);
  }

  /* Close the file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");

  return status;
}

} /* end namespace openfpga */
