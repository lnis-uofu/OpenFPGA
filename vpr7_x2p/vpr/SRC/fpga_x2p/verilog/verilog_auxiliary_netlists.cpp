/********************************************************************
 * This file includes functions that are used to generate Verilog files
 * or code blocks, with a focus on 
 * `include user-defined or auto-generated netlists in Verilog format
 *******************************************************************/
#include <fstream>

#include "vtr_assert.h"

#include "circuit_library_utils.h"

#include "fpga_x2p_utils.h"
#include "fpga_x2p_naming.h"

#include "verilog_writer_utils.h"
#include "verilog_auxiliary_netlists.h"

/********************************************************************
 * Local constant variables
 *******************************************************************/
constexpr char* TOP_INCLUDE_NETLIST_FILE_NAME_POSTFIX = "_include_netlists.v";

/********************************************************************
 * Print a file that includes all the netlists that have been generated
 * and user-defined.
 * Some netlists are open to compile under specific preprocessing flags
 *******************************************************************/
void print_include_netlists(const std::string& src_dir,
                            const std::string& circuit_name,
                            const std::string& reference_benchmark_file,
                            const CircuitLibrary& circuit_lib) {
  std::string verilog_fname = src_dir + circuit_name + std::string(TOP_INCLUDE_NETLIST_FILE_NAME_POSTFIX);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_handler(fp);

  /* Print the title */
  print_verilog_file_header(fp, std::string("Netlist Summary")); 

  /* Print preprocessing flags */
  print_verilog_comment(fp, std::string("------ Include defines: preproc flags -----"));
  print_verilog_include_netlist(fp, std::string(src_dir + std::string(defines_verilog_file_name)));
  fp << std::endl;

  print_verilog_comment(fp, std::string("------ Include simulation defines -----"));
  print_verilog_include_netlist(fp, src_dir + std::string(defines_verilog_simulation_file_name));
  fp << std::endl;

  /* Include all the user-defined netlists */
  for (const std::string& user_defined_netlist : find_circuit_library_unique_verilog_netlists(circuit_lib)) {
    print_verilog_include_netlist(fp, user_defined_netlist);
  }

  /* Include all the primitive modules */
  print_verilog_include_netlist(fp, src_dir + std::string(default_submodule_dir_name) + std::string(submodule_verilog_file_name));
  fp << std::endl;

  /* Include all the CLB, heterogeneous block modules */
  print_verilog_include_netlist(fp, src_dir  + std::string(default_lb_dir_name) + std::string(logic_block_verilog_file_name));
  fp << std::endl;

  /* Include all the routing architecture modules */
  print_verilog_include_netlist(fp, src_dir + std::string(default_rr_dir_name) + std::string(routing_verilog_file_name));
  fp << std::endl;

  /* Include FPGA top module */
  print_verilog_include_netlist(fp, src_dir + generate_fpga_top_netlist_name(std::string(verilog_netlist_file_postfix)));
  fp << std::endl;

  /* Include reference benchmark netlist only when auto-check flag is enabled */
  print_verilog_preprocessing_flag(fp, std::string(autochecked_simulation_flag));
  fp << "\t";
  print_verilog_include_netlist(fp, std::string(reference_benchmark_file));
  print_verilog_endif(fp);
  fp << std::endl;

  /* Include formal verification netlists only when formal verification flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(verilog_formal_verification_preproc_flag));
  fp << "\t";
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(formal_verification_verilog_file_postfix));
  
  /* Include formal verification testbench only when formal simulation flag is enabled */
  fp << "\t";
  print_verilog_preprocessing_flag(fp, std::string(formal_simulation_flag));
  fp << "\t\t";
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(random_top_testbench_verilog_file_postfix));
  fp << "\t";
  print_verilog_endif(fp);
  
  print_verilog_endif(fp);
  fp << std::endl;

  /* Include top-level testbench only when auto-check flag is enabled */
  print_verilog_preprocessing_flag(fp, std::string(autochecked_simulation_flag));
  fp << "\t";
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(autocheck_top_testbench_verilog_file_postfix));
  print_verilog_endif(fp);
  fp << std::endl;

  /* Close the file stream */
  fp.close();
}
