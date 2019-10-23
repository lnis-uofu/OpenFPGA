/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <fstream>
#include <map>
#include <algorithm>

#include "vtr_assert.h"

#include "vpr_types.h"

#include "fpga_x2p_utils.h"
#include "fpga_x2p_naming.h"

#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_top_module.h"

/********************************************************************
 * Print the top-level module for the FPGA fabric in Verilog format
 * This function will 
 * 1. name the top-level module
 * 2. include dependent netlists 
 *    - User defined netlists
 *    - Auto-generated netlists
 * 3. Add the submodules to the top-level graph
 * 4. Add module nets to connect datapath ports
 * 5. Add module nets/submodules to connect configuration ports
 *******************************************************************/
void print_verilog_top_module(ModuleManager& module_manager,
                              const std::string& arch_name,
                              const std::string& verilog_dir,
                              const bool& use_explicit_mapping) {
  /* Create a module as the top-level fabric, and add it to the module manager */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Start printing out Verilog netlists */
  /* Create the file name for Verilog netlist */
  std::string verilog_fname(verilog_dir + generate_fpga_top_netlist_name(std::string(verilog_netlist_file_postfix)));
  /* TODO: remove the bak file when the file is ready */
  verilog_fname += ".bak";

  vpr_printf(TIO_MESSAGE_INFO,
             "Writing Verilog Netlist for top-level module of FPGA fabric (%s)...\n",
             verilog_fname.c_str());

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  print_verilog_file_header(fp, std::string("Top-level Verilog module for FPGA architecture: " + std::string(arch_name))); 

  /* Print preprocessing flags */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Write the module content in Verilog format */
  write_verilog_module_to_file(fp, module_manager, top_module, use_explicit_mapping);

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();
}
