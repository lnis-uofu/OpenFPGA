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

int main(int argc, const char** argv) {
  /* Ensure we have the following arguments:
   * 1. Input - Users Design Constraints (.pcf)
   * 2. Input - configuration xml
   * 5. Output - BitstreamSetting data structure
   */
  VTR_ASSERT(6 == argc);

  /* Parse the input files */
  openfpga::PcfData pcf_data;
  openfpga::read_pcf(argv[1], pcf_data);
  VTR_LOG("Read the design constraints from a pcf file: %s.\n", argv[1]);

  //   openfpga::PcfConfig pcf_config;
  //   openfpga::read_pcfconfig(argv[2], pcf_config);
  //   VTR_LOG("Read the design constraints conifguration from a config file:
  //   %s.\n", argv[2]);

  //   /* Convert */
  int status = 0;
  //     pcf2bitStreamSetting(pcf_data,pcf_config);
  if (status) {
    return status;
  }
  /* Output */
  //   status = io_net_place.write_to_place_file(argv[5], true, true);

  return status;
}
