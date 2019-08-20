/************************************************
 * Header file for verilog_submodule_essential.cpp
 * Include function declaration on 
 * outputting Verilog netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
 ***********************************************/
#include <fstream>
#include "vtr_assert.h"

/* Device-level header files */
#include "spice_types.h"

/* FPGA-X2P context header files */
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_submodule_essential.h"

void dump_verilog_submodule_essentials(const std::string& verilog_dir, 
                                       const std::string& submodule_dir,
                                       const CircuitLibrary& circuit_lib) {
  std::string verilog_fname = submodule_dir + essentials_verilog_file_name;
  std::fstream fp;

  /* Create the file stream */
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_handler(fp); 

  /* Create file */
  vpr_printf(TIO_MESSAGE_INFO,
             "Generating Verilog netlist (%s) for essential gates...\n",
             __FILE__, __LINE__, essentials_verilog_file_name); 
  
  /* Close file handler*/
  fp.close();

  return;
}
