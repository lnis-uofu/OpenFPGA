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
#include "write_xml_boundary_timing.h"
int main(int argc, const char** argv) {
  /* Ensure we have the following arguments:
   * 1. Input - Users Design Constraints (.pcf)
   * 2. Input - boundary timing xml
   * 3. Input - IO pin table
   * 4. Output - sdc file
   */

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
  std::string clock_name = "virtual_clock";

  int status = pcf2sdc_from_boundary_timing(
    pcf_data, boundary_timing, io_pin_table, clock_name, argv[4], true);

  status = write_xml_boundary_timing("boundary_timing_output_file.xml",
                                     boundary_timing);
  return status;
}
