/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "read_xml_io_location_map.h"
//#include "write_xml_io_location_map.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  /* Parse the fabric key from an XML file */
  openfpga::IoLocationMap io_location_map =
    openfpga::read_xml_io_location_map(argv[1]);
  VTR_LOG("Read the I/O location map from an XML file: %s.\n", argv[1]);

  /* Output to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) {
    io_location_map.write_to_xml_file(std::string(argv[2]), true, true);
    VTR_LOG("Echo the I/O location map to an XML file: %s.\n", argv[2]);
  }
}
