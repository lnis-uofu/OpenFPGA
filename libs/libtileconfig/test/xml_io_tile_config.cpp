/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga */
#include "read_xml_tile_config.h"
#include "write_xml_tile_config.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  int status = 0;

  /* Parse the circuit library from an XML file */
  openfpga::TileConfig tile_config;
  status = openfpga::read_xml_tile_config(argv[1], tile_config);
  if (status != 0) {
    return status;
  }
  VTR_LOG("Parsed tile configuration from XML file: %s\n", argv[1]);

  /* Output the bus group to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) {
    status = openfpga::write_xml_tile_config(argv[2], tile_config);
    VTR_LOG("Write the tile configuration to an XML file: %s.\n", argv[2]);
  }

  return status;
}
