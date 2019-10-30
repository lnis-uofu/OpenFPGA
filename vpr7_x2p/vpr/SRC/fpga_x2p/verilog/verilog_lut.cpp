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
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_lut.h"

/******************************************************************** 
 * Print Verilog modules for the Look-Up Tables (LUTs) 
 * in the circuit library
 ********************************************************************/
void print_verilog_submodule_luts(ModuleManager& module_manager,
                                  const CircuitLibrary& circuit_lib,
                                  const std::string& verilog_dir,
                                  const std::string& submodule_dir,
                                  const bool& use_explicit_port_map) {
  /* TODO: remove .bak when this part is completed and tested */
  std::string verilog_fname = submodule_dir + luts_verilog_file_name;
  //verilog_fname +=".bak";

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
  for (const auto& lut_model : circuit_lib.models()) {
    /* Bypass user-defined and non-LUT modules */
    if ( (!circuit_lib.model_verilog_netlist(lut_model).empty()) 
      || (SPICE_MODEL_LUT != circuit_lib.model_type(lut_model)) ) {
      continue;
    }
    /* Find the module id */
    ModuleId lut_module = module_manager.find_module(circuit_lib.model_name(lut_model));
    VTR_ASSERT(true == module_manager.valid_module_id(lut_module));
    write_verilog_module_to_file(fp, module_manager, lut_module, 
                                 use_explicit_port_map || circuit_lib.dump_explicit_port_map(lut_model));
  }

  /* Close the file handler */
  fp.close();

  /* Add fname to the linked list */
  /* Add it when the Verilog generation is refactored
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_fname.c_str());  
   */

  return;
}

