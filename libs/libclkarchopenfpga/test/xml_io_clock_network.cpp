/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga */
#include "read_xml_clock_network.h"
#include "write_xml_clock_network.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  /* Parse the circuit library from an XML file */
  const openfpga::ClockNetwork& clk_ntwk = openfpga::read_xml_clock_network(argv[1]);
  VTR_LOG("Parsed %lu clock tree(s) from XML into clock network.\n",
          clk_ntwk.trees().size());

  /* Validate before write out */
  clk_ntwk.validate();
  if (!clk_ntwk.is_valid()) {
    VTR_LOG_ERROR("Invalid clock network.\n");
    exit(1);
  }

  /* Output the bus group to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) {
    openfpga::write_xml_clock_network(argv[2], clk_ntwk);
    VTR_LOG("Write the clock network to an XML file: %s.\n", argv[2]);
  }

  return 0;
}
