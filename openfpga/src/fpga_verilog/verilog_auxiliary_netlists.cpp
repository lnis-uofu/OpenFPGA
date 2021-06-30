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
 * Print a file that includes all the fabric netlists 
 * that have been generated  and user-defined.
 * This does NOT include any testbenches!
 * Some netlists are open to compile under specific preprocessing flags
 *******************************************************************/
void print_verilog_fabric_include_netlist(const NetlistManager& netlist_manager,
                                          const std::string& src_dir,
                                          const CircuitLibrary& circuit_lib) {
  std::string verilog_fname = src_dir + std::string(FABRIC_INCLUDE_VERILOG_NETLIST_FILE_NAME);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Print the title */
  print_verilog_file_header(fp, std::string("Fabric Netlist Summary")); 

  /* Print preprocessing flags */
  print_verilog_comment(fp, std::string("------ Include defines: preproc flags -----"));
  print_verilog_include_netlist(fp, std::string(src_dir + std::string(DEFINES_VERILOG_FILE_NAME)));
  fp << std::endl;

  /* Include all the user-defined netlists */
  print_verilog_comment(fp, std::string("------ Include user-defined netlists -----"));
  for (const std::string& user_defined_netlist : find_circuit_library_unique_verilog_netlists(circuit_lib)) {
    print_verilog_include_netlist(fp, user_defined_netlist);
  }

  /* Include all the primitive modules */
  print_verilog_comment(fp, std::string("------ Include primitive module netlists -----"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::SUBMODULE_NETLIST)) {
    print_verilog_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Include all the CLB, heterogeneous block modules */
  print_verilog_comment(fp, std::string("------ Include logic block netlists -----"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::LOGIC_BLOCK_NETLIST)) {
    print_verilog_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Include all the routing architecture modules */
  print_verilog_comment(fp, std::string("------ Include routing module netlists -----"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::ROUTING_MODULE_NETLIST)) {
    print_verilog_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Include FPGA top module */
  print_verilog_comment(fp, std::string("------ Include fabric top-level netlists -----"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::TOP_MODULE_NETLIST)) {
    print_verilog_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Close the file stream */
  fp.close();
}

/********************************************************************
 * Print a file that includes all the netlists 
 * including the fabric netlists and full testbenches
 * that have been generated and user-defined.
 * Some netlists are open to compile under specific preprocessing flags
 *******************************************************************/
void print_verilog_full_testbench_include_netlists(const std::string& src_dir,
                                                   const std::string& circuit_name,
                                                   const std::string& fabric_netlist_file,
                                                   const std::string& reference_benchmark_file,
                                                   const bool& no_self_checking) {
  std::string verilog_fname = src_dir + circuit_name + std::string(TOP_VERILOG_TESTBENCH_INCLUDE_NETLIST_FILE_NAME_POSTFIX);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Print the title */
  print_verilog_file_header(fp, std::string("Netlist Summary")); 

  /* Include FPGA top module */
  print_verilog_comment(fp, std::string("------ Include fabric top-level netlists -----"));
  if (true == fabric_netlist_file.empty()) {
    print_verilog_include_netlist(fp, src_dir + std::string(FABRIC_INCLUDE_VERILOG_NETLIST_FILE_NAME));
  } else {
    VTR_ASSERT_SAFE(false == fabric_netlist_file.empty());
    print_verilog_include_netlist(fp, fabric_netlist_file);
  }
  fp << std::endl;

  /* Include reference benchmark netlist only when auto-check flag is enabled */
  if (!no_self_checking) {
    print_verilog_include_netlist(fp, std::string(reference_benchmark_file));
    fp << std::endl;
  }

  /* Include top-level testbench only when auto-check flag is enabled */
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_FILE_POSTFIX));

  /* Close the file stream */
  fp.close();
}

/********************************************************************
 * Print a file that includes all the netlists 
 * including the fabric netlists and preconfigured testbenches
 * that have been generated and user-defined.
 * Some netlists are open to compile under specific preprocessing flags
 *******************************************************************/
void print_verilog_preconfigured_testbench_include_netlists(const std::string& src_dir,
                                                            const std::string& circuit_name,
                                                            const std::string& fabric_netlist_file,
                                                            const std::string& reference_benchmark_file,
                                                            const bool& no_self_checking) {
  std::string verilog_fname = src_dir + circuit_name + std::string(TOP_VERILOG_TESTBENCH_INCLUDE_NETLIST_FILE_NAME_POSTFIX);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Print the title */
  print_verilog_file_header(fp, std::string("Netlist Summary")); 

  /* Include FPGA top module */
  print_verilog_comment(fp, std::string("------ Include fabric top-level netlists -----"));
  if (true == fabric_netlist_file.empty()) {
    print_verilog_include_netlist(fp, src_dir + std::string(FABRIC_INCLUDE_VERILOG_NETLIST_FILE_NAME));
  } else {
    VTR_ASSERT_SAFE(false == fabric_netlist_file.empty());
    print_verilog_include_netlist(fp, fabric_netlist_file);
  }
  fp << std::endl;

  /* Include reference benchmark netlist only when auto-check flag is enabled */
  if (!no_self_checking) {
    print_verilog_include_netlist(fp, std::string(reference_benchmark_file));
    fp << std::endl;
  }

  /* Include formal verification netlists */
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(FORMAL_VERIFICATION_VERILOG_FILE_POSTFIX));
  
  /* Include formal verification testbench */
  print_verilog_include_netlist(fp, src_dir + circuit_name + std::string(RANDOM_TOP_TESTBENCH_VERILOG_FILE_POSTFIX));

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

  /* Close the file stream */
  fp.close();
}

} /* end namespace openfpga */
