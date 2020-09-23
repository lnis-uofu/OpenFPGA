/********************************************************************
 * This file includes functions that are used to generate SPICE files
 * or code blocks, with a focus on 
 * `include user-defined or auto-generated netlists in SPICE format
 *******************************************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_naming.h"
#include "circuit_library_utils.h"
#include "spice_constants.h"
#include "spice_writer_utils.h"
#include "spice_auxiliary_netlists.h"

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
void print_spice_fabric_include_netlist(const NetlistManager& netlist_manager,
                                        const std::string& src_dir,
                                        const CircuitLibrary& circuit_lib) {
  std::string spice_fname = src_dir + std::string(FABRIC_INCLUDE_SPICE_NETLIST_FILE_NAME);

  /* Create the file stream */
  std::fstream fp;
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(spice_fname.c_str(), fp);

  /* Print the title */
  print_spice_file_header(fp, std::string("Fabric Netlist Summary")); 

  /* Include all the user-defined netlists */
  print_spice_comment(fp, std::string("Include user-defined netlists"));
  for (const std::string& user_defined_netlist : find_circuit_library_unique_spice_netlists(circuit_lib)) {
    print_spice_include_netlist(fp, user_defined_netlist);
  }

  /* Include all the primitive modules */
  print_spice_comment(fp, std::string("Include primitive module netlists"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::SUBMODULE_NETLIST)) {
    print_spice_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Include all the CLB, heterogeneous block modules */
  print_spice_comment(fp, std::string("Include logic block netlists"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::LOGIC_BLOCK_NETLIST)) {
    print_spice_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Include all the routing architecture modules */
  print_spice_comment(fp, std::string("Include routing module netlists"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::ROUTING_MODULE_NETLIST)) {
    print_spice_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Include FPGA top module */
  print_spice_comment(fp, std::string("Include fabric top-level netlists"));
  for (const NetlistId& nlist_id : netlist_manager.netlists_by_type(NetlistManager::TOP_MODULE_NETLIST)) {
    print_spice_include_netlist(fp, netlist_manager.netlist_name(nlist_id));
  }
  fp << std::endl;

  /* Close the file stream */
  fp.close();
}

} /* end namespace openfpga */
