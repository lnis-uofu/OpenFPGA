/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "boundary_timing.h"
#include "read_xml_boundary_timing.h"
#include "vtr_assert.h"
#include "vtr_log.h"
/* Headers from fabric key */
#include "atom_netlist_utils.h"
#include "io_pin_table.h"
#include "pcf2place.h"
#include "pcf_reader.h"
#include "read_blif.h"
#include "read_circuit.h"
#include "read_csv_io_pin_table.h"
#include "read_xml_arch_file.h"
int main(int argc, const char** argv) {
  /* Ensure we have the following arguments:
   * 1. Input - Users Design Constraints (.pcf)
   * 2. Input - boundary timing xml
   * 3. Input - IO pin table
   * 4. Output - sdc file
   */

  /*read blif and find netlist clock*/
  // read vpr arch file
  t_arch* arch = new t_arch;
  std::vector<t_physical_tile_type> physical_tile_types;
  std::vector<t_logical_block_type> logical_block_types;
  xml_read_arch(argv[5], false, arch, physical_tile_types, logical_block_types);

  // read netlist and set up atom netlist
  const LogicalModels& logical_models = arch->models;
  AtomNetlist atom_ntlist =
    read_blif(e_circuit_format::BLIF, argv[6], logical_models);

  std::vector<std::string> clock_names;  // Assume just one clock
  std::set<AtomPinId> netlist_clock_drivers =
    find_netlist_logical_clock_drivers(atom_ntlist, logical_models);
  for (auto clock_driver : netlist_clock_drivers) {
    AtomNetId net_id = atom_ntlist.pin_net(clock_driver);
    VTR_LOG("  Netlist Clock is '%s' ", atom_ntlist.net_name(net_id).c_str());
    clock_names.push_back(atom_ntlist.net_name(net_id).c_str());
  }
  if (clock_names.size() > 1) {
    VTR_LOG_ERROR("Only single clock supported. Please check your design! \n");
    return 1;
  }

  openfpga::PcfData pcf_data;
  VTR_LOG("Read the pcf file: %s.\n", argv[1]);
  openfpga::PcfCustomCommand pcf_custom_command;
  openfpga::read_pcf(argv[1], pcf_data, pcf_custom_command, true,
                     true); /*pcf reader will set custom constraints based on
                   custom commands*/

  VTR_LOG("Read the boundary timing file: %s.\n", argv[2]);
  openfpga::BoundaryTiming boundary_timing =
    openfpga::read_xml_boundary_timing(argv[2]);

  VTR_LOG("Read the I/O pin table from a csv file: %s.\n", argv[3]);
  openfpga::IoPinTable io_pin_table = openfpga::read_csv_io_pin_table(
    argv[3], openfpga::e_pin_table_direction_convention::EXPLICIT);

  /* Convert */
  std::string clock_name;
  if (clock_names.empty()) {
    clock_name = "virtual_clock";
  } else {
    clock_name = clock_names[0];
  }
  int status = pcf2sdc_file_generation(pcf_data, boundary_timing, io_pin_table,
                                       clock_name, argv[4], true);

  return status;
}
