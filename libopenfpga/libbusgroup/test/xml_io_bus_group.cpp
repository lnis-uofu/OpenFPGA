/********************************************************************
 * Unit test functions to validate the correctness of 
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga */
#include "read_xml_bus_group.h"
#include "write_xml_bus_group.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  /* Parse the circuit library from an XML file */
  const openfpga::BusGroup& bus_group = read_xml_bus_group(argv[1]); 
  VTR_LOG("Parsed %lu bus(es) from XML into bus group.\n",
          bus_group.buses().size());

  /* Output the bus group to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) { 
    write_xml_bus_group(argv[2], bus_group);
    VTR_LOG("Write the bus group to an XML file: %s.\n",
            argv[2]);
  }
}


