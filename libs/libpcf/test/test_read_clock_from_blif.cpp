/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include <iostream>

#include "vtr_assert.h"
#include "vtr_log.h"
/* Headers from fabric key */
#include "atom_netlist_utils.h"
#include "command_exit_codes.h"
#include "read_blif.h"
#include "read_circuit.h"
#include "read_interchange_netlist.h"
#include "read_xml_arch_file.h"
#include "vtr_path.h"

int main(int argc, const char** argv) {
  /* Ensure we have the following arguments:
   * 1. Input - Users Design Constraints (.pcf)
   * 2. Input - boundary timing xml
   * 3. Input - IO pin table
   * 4. Output - sdc file
   */

  t_arch* arch = new t_arch;
  std::vector<t_physical_tile_type> physical_tile_types;
  std::vector<t_logical_block_type> logical_block_types;
  xml_read_arch(argv[1], false, arch, physical_tile_types, logical_block_types);

  // read netlist and set up atom netlist
  const LogicalModels& logical_models = arch->models;

  auto name_ext = vtr::split_ext(argv[2]);
  e_circuit_format circuit_format;
  VTR_LOG("Circuit file: %s\n", argv[2]);
  if (name_ext[1] == ".blif") {
    circuit_format = e_circuit_format::BLIF;
  } else if (name_ext[1] == ".eblif") {
    circuit_format = e_circuit_format::EBLIF;
  } else {
    VPR_FATAL_ERROR(VPR_ERROR_ATOM_NETLIST,
                    "Failed to determine file format for '%s' expected .blif "
                    "or .eblif extension \n",
                    argv[2]);
  }

  AtomNetlist atom_ntlist;
  switch (circuit_format) {
    case e_circuit_format::BLIF:
    case e_circuit_format::EBLIF:
      atom_ntlist = read_blif(circuit_format, argv[2], logical_models);
      break;
    case e_circuit_format::FPGA_INTERCHANGE:
      atom_ntlist = read_interchange_netlist(argv[2], *arch);
      break;
    default:
      VPR_FATAL_ERROR(VPR_ERROR_ATOM_NETLIST,
                      "Unable to identify circuit file format for '%s'. "
                      "Expect [blif|eblif|fpga-interchange]!\n",
                      argv[2]);
      break;
  }

  std::vector<std::string> clock_names;  // Assume just one clock
  std::set<AtomPinId> netlist_clock_drivers =
    find_netlist_logical_clock_drivers(atom_ntlist, logical_models);
  for (auto clock_driver : netlist_clock_drivers) {
    AtomNetId net_id = atom_ntlist.pin_net(clock_driver);
    VTR_LOG("  Netlist Clock is: %s \n", atom_ntlist.net_name(net_id).c_str());
    clock_names.push_back(atom_ntlist.net_name(net_id).c_str());
  }
  VTR_LOG("clock size is: %d \n", clock_names.size());
  for (auto i : clock_names) {
    VTR_LOG("clock name: %s \n", i.c_str());
  }
  return 0;
}
