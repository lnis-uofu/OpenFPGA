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
  openfpga::ClockNetwork clk_ntwk = openfpga::read_xml_clock_network(argv[1]);
  VTR_LOG("Parsed %lu clock tree(s) from XML into clock network.\n",
          clk_ntwk.trees().size());

  /* Validate before write out */
  if (!clk_ntwk.link()) {
    VTR_LOG_ERROR("Invalid clock network when linking.\n");
    exit(1);
  }
  if (!clk_ntwk.validate()) {
    VTR_LOG_ERROR("Invalid clock network.\n");
    exit(1);
  }
  VTR_ASSERT(clk_ntwk.is_valid());
  for (auto tree_id : clk_ntwk.trees()) {
    VTR_LOG("Max. depth of the clock tree '%lu' is %d\n", size_t(tree_id),
            clk_ntwk.tree_depth(tree_id));
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
