#include "read_blif_clock_info.h"
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

std::vector<std::string> read_blif_clock_info(const char* arch_fname,
                                              const char* blif_fname,
                                              const std::string& blif_ffmt) {
  t_arch arch;
  std::vector<t_physical_tile_type> physical_tile_types;
  std::vector<t_logical_block_type> logical_block_types;
  xml_read_arch(arch_fname, false, &arch, physical_tile_types,
                logical_block_types);

  // read netlist and set up atom netlist
  const LogicalModels& logical_models = arch.models;

  auto name_ext = vtr::split_ext(blif_fname);
  e_circuit_format circuit_format;
  /* TODO: Should use VPR's API on convert string to circuit format */
  if (blif_ffmt == std::string("auto")) {
    VTR_LOG("Circuit file: %s\n", blif_fname);
    if (name_ext[1] == ".blif") {
      circuit_format = e_circuit_format::BLIF;
    } else if (name_ext[1] == ".eblif") {
      circuit_format = e_circuit_format::EBLIF;
    } else {
      VPR_FATAL_ERROR(VPR_ERROR_ATOM_NETLIST,
                      "Failed to auto-determine file format for '%s' expected .blif "
                      "or .eblif extension \n",
                      blif_fname);
    }
  } else if (blif_ffmt == std::string("blif")) {
    circuit_format = e_circuit_format::BLIF;
  } else if (blif_ffmt == std::string("eblif")) {
    circuit_format = e_circuit_format::EBLIF;
  } else {
    VPR_FATAL_ERROR(VPR_ERROR_ATOM_NETLIST,
                    "Unknown circuit format '%s' specified by users. Expected [ auto | blif | eblif ]\n",
                    blif_ffmt);
  } 

  AtomNetlist atom_ntlist;
  switch (circuit_format) {
    case e_circuit_format::BLIF:
    case e_circuit_format::EBLIF:
      atom_ntlist = read_blif(circuit_format, blif_fname, logical_models);
      break;
    case e_circuit_format::FPGA_INTERCHANGE:
      atom_ntlist = read_interchange_netlist(blif_fname, *arch);
      break;
    default:
      VPR_FATAL_ERROR(VPR_ERROR_ATOM_NETLIST,
                      "Unable to identify circuit file format for '%s'. "
                      "Expect [blif|eblif|fpga-interchange]!\n",
                      blif_fname);
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

  /* Clean */
  free_arch(&arch);
  return clock_names;
}
