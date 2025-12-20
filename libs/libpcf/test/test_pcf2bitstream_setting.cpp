/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "blif_head_reader.h"
#include "io_net_place.h"
#include "pcf2place.h"
#include "pcf_reader.h"
#include "read_csv_io_pin_table.h"
#include "read_xml_io_location_map.h"
#include "read_xml_pin_constraints.h"

int main(int argc, const char** argv) {
  /* Ensure we have the following arguments:
   * 1. Input - Users Design Constraints (.pcf)
   * 2. Input - configuration xml
   * 5. Output - BitstreamSetting data structure
   */

  /* Parse the input files */
  openfpga::PcfData pcf_data;
  VTR_LOG("Read the pcf config file: %s.\n", argv[2]);
  openfpga::PcfCustomCommand pcf_custom_command;
  openfpga::read_pcf_config(
    argv[2], pcf_custom_command); /* pcf config.xml will be converted to data
                                     structure PcfCustomCommand*/
  openfpga::read_pcf(argv[1], pcf_data, pcf_custom_command, true,
                     true); /*pcf reader will set custom constraints based on
                   custom commands*/

  openfpga::IoPinTable io_pin_table = openfpga::read_csv_io_pin_table(
    argv[4], openfpga::e_pin_table_direction_convention::QUICKLOGIC);
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n", argv[3]);

  /* Convert */
  openfpga::BitstreamSetting bitstream_setting;
  int status =
    pcf2bitstream_setting(pcf_data, bitstream_setting, io_pin_table, true);
  return status;
}
