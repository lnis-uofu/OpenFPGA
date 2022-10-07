/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "pcf_reader.h"
#include "pcf_writer.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  /* Parse the fabric key from an XML file */
  openfpga::PcfData pcf_data;
  openfpga::read_pcf(argv[1], pcf_data);
  VTR_LOG("Read the design constraints from a pcf file: %s.\n", argv[1]);

  /* Output to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) {
    write_pcf(argv[2], pcf_data);
    VTR_LOG("Echo the design constraints to a pcf file: %s.\n", argv[2]);
  }
}
