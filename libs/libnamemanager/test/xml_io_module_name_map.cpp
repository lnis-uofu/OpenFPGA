/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga */
#include "read_xml_module_name_map.h"
#include "write_xml_module_name_map.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  int status = 0;

  /* Parse the circuit library from an XML file */
  openfpga::ModuleNameMap module_name_map;
  status = openfpga::read_xml_module_name_map(argv[1], module_name_map);
  if (status != 0) {
    return status;
  }
  VTR_LOG("Parsed %lu default names from XML.\n",
          module_name_map.tags().size());

  /* Output the bus group to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) {
    status =
      openfpga::write_xml_module_name_map(argv[2], module_name_map, true, true);
    VTR_LOG("Write the module name mapping to an XML file: %s.\n", argv[2]);
  }

  return status;
}
