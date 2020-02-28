/********************************************************************
 * This file includes functions that are used to generate Verilog files
 * or code blocks, with a focus on 
 * `include user-defined or auto-generated netlists in Verilog format
 *******************************************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_naming.h"
#include "circuit_library_utils.h"
#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_auxiliary_netlists.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Local constant variables
 *******************************************************************/

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
  check_file_stream(verilog_fname.c_str(), fp);

  /* Print the title */
  print_verilog_file_header(fp, std::string("Netlist Summary")); 

  /* Print preprocessing flags */
  print_verilog_comment(fp, std::string("------ Include defines: preproc flags -----"));
  print_verilog_include_netlist(fp, std::string(src_dir + std::string(DEFINES_VERILOG_FILE_NAME)));
  fp << std::endl;

  print_verilog_comment(fp, std::string("------ Include simulation defines -----"));
  print_verilog_include_netlist(fp, src_dir + std::string(DEFINES_VERILOG_SIMULATION_FILE_NAME));
  fp << std::endl;

  /* Include all the user-defined netlists */
  for (const std::string& user_defined_netlist : find_circuit_library_unique_verilog_netlists(circuit_lib)) {
    print_verilog_include_netlist(fp, user_defined_netlist);
  }

  /* Include all the primitive modules */
  print_verilog_include_netlist(fp, src_dir + std::string(DEFAULT_SUBMODULE_DIR_NAME) + std::string(SUBMODULE_VERILOG_FILE_NAME));
  fp << std::endl;

  /* Include all the CLB, heterogeneous block modules */
  print_verilog_include_netlist(fp, src_dir  + std::string(DEFAULT_LB_DIR_NAME) + std::string(LOGIC_BLOCK_VERILOG_FILE_NAME));
  fp << std::endl;

  /* Include all the routing architecture modules */
  print_verilog_include_netlist(fp, src_dir + std::string(DEFAULT_RR_DIR_NAME) + std::string(ROUTING_VERILOG_FILE_NAME));
  fp << std::endl;

  /* Include FPGA top module */
  print_verilog_include_netlist(fp, src_dir + generate_fpga_top_netlist_name(std::string(VERILOG_NETLIST_FILE_POSTFIX)));
  fp << std::endl;

  /* Include reference benchmark netlist only when auto-check flag is enabled */
  print_verilog_preprocessing_flag(fp, std::string(AUTOCHECKED_SIMULATION_FLAG));
  fp << "\t";
  print_verilog_include_netlist(fp, std::string(reference_benchmark_file));
  print_verilog_endif(fp);
  fp << std::endl;

  /* Include formal verification netlists only when formal verification flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(VERILOG_FORMAL_VERIFICATION_PREPROC_FLAG));
  fp << "\t";
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(FORMAL_VERIFICATION_VERILOG_FILE_POSTFIX));
  
  /* Include formal verification testbench only when formal simulation flag is enabled */
  fp << "\t";
  print_verilog_preprocessing_flag(fp, std::string(FORMAL_SIMULATION_FLAG));
  fp << "\t\t";
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(RANDOM_TOP_TESTBENCH_VERILOG_FILE_POSTFIX));
  fp << "\t";
  print_verilog_endif(fp);
  
  print_verilog_endif(fp);
  fp << std::endl;

  /* Include top-level testbench only when auto-check flag is enabled */
  print_verilog_preprocessing_flag(fp, std::string(AUTOCHECKED_SIMULATION_FLAG));
  fp << "\t";
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_FILE_POSTFIX));
  print_verilog_endif(fp);
  fp << std::endl;

  /* Close the file stream */
  fp.close();
}

/********************************************************************
 * Print a Verilog file containing preprocessing flags
 * which are used enable/disable some features in FPGA Verilog modules
 *******************************************************************/
void print_verilog_preprocessing_flags_netlist(const std::string& src_dir,
                                               const FabricVerilogOption& fabric_verilog_opts) {

  std::string verilog_fname = src_dir + std::string(DEFINES_VERILOG_FILE_NAME);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Print the title */
  print_verilog_file_header(fp, std::string("Preprocessing flags to enable/disable features in FPGA Verilog modules")); 

  /* To enable timing */
  if (true == fabric_verilog_opts.include_timing()) {
    print_verilog_define_flag(fp, std::string(VERILOG_TIMING_PREPROC_FLAG), 1);
    fp << std::endl;
  } 

  /* To enable timing */
  if (true == fabric_verilog_opts.include_signal_init()) {
    print_verilog_define_flag(fp, std::string(VERILOG_SIGNAL_INIT_PREPROC_FLAG), 1);
    fp << std::endl;
  } 

  /* To enable functional verfication with Icarus */
  if (true == fabric_verilog_opts.support_icarus_simulator()) {
    print_verilog_define_flag(fp, std::string(ICARUS_SIMULATOR_FLAG), 1);
    fp << std::endl;
  } 

  /* Close the file stream */
  fp.close();
}

/********************************************************************
 * Print a Verilog file containing simulation-related preprocessing flags
 *******************************************************************/
void print_verilog_simulation_preprocessing_flags(const std::string& src_dir,
                                                  const VerilogTestbenchOption& verilog_testbench_opts) {

  std::string verilog_fname = src_dir + std::string(DEFINES_VERILOG_SIMULATION_FILE_NAME);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Print the title */
  print_verilog_file_header(fp, std::string("Preprocessing flags to enable/disable simulation features")); 

  /* To enable manualy checked simulation */
  if (true == verilog_testbench_opts.print_top_testbench()) {
    print_verilog_define_flag(fp, std::string(INITIAL_SIMULATION_FLAG), 1);
    fp << std::endl;
  } 

  /* To enable auto-checked simulation */
  if ( (true == verilog_testbench_opts.print_preconfig_top_testbench())
    || (true == verilog_testbench_opts.print_top_testbench()) ) {
    print_verilog_define_flag(fp, std::string(AUTOCHECKED_SIMULATION_FLAG), 1);
    fp << std::endl;
  } 

  /* To enable pre-configured FPGA simulation */
  if (true == verilog_testbench_opts.print_formal_verification_top_netlist()) {
    print_verilog_define_flag(fp, std::string(FORMAL_SIMULATION_FLAG), 1);
    fp << std::endl;
  } 

  /* Close the file stream */
  fp.close();
}

} /* end namespace openfpga */
