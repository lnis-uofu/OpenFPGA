/********************************************************************
 * Unit test functions to validate the correctness of 
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "read_xml_arch_bitstream.h"
#include "arch_bitstream_writer.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  /* Parse the bitstream from an XML file */
  openfpga::BitstreamManager test_bitstream = openfpga::read_xml_architecture_bitstream(argv[1]);
  VTR_LOG("Read the bitstream from an XML file: %s.\n",
          argv[1]);

  /* Output the circuit library to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) { 
    openfpga::write_arch_independent_bitstream_to_xml_file(test_bitstream, std::string("fpga_top"), argv[2]);
    VTR_LOG("Echo the bitstream to an XML file: %s.\n",
            argv[2]);
  }
}


