/********************************************************************
 * This file includes functions to generate Verilog submodules for LUTs
 ********************************************************************/
#include <string>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "mux_graph.h"
#include "module_manager.h"
#include "mux_utils.h"

#include "openfpga_naming.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_lut.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************** 
 * Print Verilog modules for the Look-Up Tables (LUTs) 
 * in the circuit library
 ********************************************************************/
void print_verilog_submodule_luts(const ModuleManager& module_manager,
                                  NetlistManager& netlist_manager,
                                  const CircuitLibrary& circuit_lib,
                                  const std::string& submodule_dir,
                                  const FabricVerilogOption& options) {
  std::string verilog_fname = submodule_dir + std::string(LUTS_VERILOG_FILE_NAME);

  std::fstream fp;

  /* Create the file stream */
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_stream(verilog_fname.c_str(), fp); 

  /* Create file */
  VTR_LOG("Writing Verilog netlist for LUTs '%s'...",
          verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Look-Up Tables", options.time_stamp()); 

  /* Search for each LUT circuit model */
  for (const auto& lut_model : circuit_lib.models()) {
    /* Bypass user-defined and non-LUT modules */
    if ( (!circuit_lib.model_verilog_netlist(lut_model).empty()) 
      || (CIRCUIT_MODEL_LUT != circuit_lib.model_type(lut_model)) ) {
      continue;
    }
    /* Find the module id */
    ModuleId lut_module = module_manager.find_module(circuit_lib.model_name(lut_model));
    VTR_ASSERT(true == module_manager.valid_module_id(lut_module));
    write_verilog_module_to_file(fp, module_manager, lut_module, 
                                 options.explicit_port_mapping() || circuit_lib.dump_explicit_port_map(lut_model),
                                 options.default_net_type());
  }

  /* Close the file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(verilog_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
