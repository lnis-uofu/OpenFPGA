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
  openfpga::read_pcf_conifg(
    argv[2], pcf_custom_command); /* pcf config.xml will be converted to data
                                     structure PcfCustomCommand*/
  openfpga::read_pcf(
    argv[1], pcf_data, true,
    pcf_custom_command); /*pcf reader will set custom constraints based on
                            custom commands*/

  openfpga::IoLocationMap io_location_map =
    openfpga::read_xml_io_location_map(argv[3]);
  VTR_LOG("Read the I/O location map from an XML file: %s.\n", argv[3]);

  /* Convert */
  openfpga::BitstreamSetting bitstream_setting;
  int status = 0;
  status = pcf2bitstream_setting(pcf_data, bitstream_setting);

  if (status) {
    return status;
  }
  /* Output */
  //   status = io_net_place.write_to_place_file(argv[5], true, true);

  return status;
}
