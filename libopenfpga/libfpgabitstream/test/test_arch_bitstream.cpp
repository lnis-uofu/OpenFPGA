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
#include "write_xml_arch_bitstream.h"
#include "report_arch_bitstream_distribution.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two or 3 argument */
  VTR_ASSERT((2 == argc) || (3 == argc) || (4 == argc) || (5 == argc));

  /* Parse the bitstream from an XML file */
  openfpga::BitstreamManager test_bitstream = openfpga::read_xml_architecture_bitstream(argv[1]);
  VTR_LOG("Read the bitstream from an XML file: %s.\n",
          argv[1]);

  /* Output the bitstream database to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) { 
    openfpga::write_xml_architecture_bitstream(test_bitstream, argv[2], true);
    VTR_LOG("Echo the bitstream (with time stamp) to an XML file: %s.\n",
            argv[2]);
    openfpga::write_xml_architecture_bitstream(test_bitstream, argv[2], false);
    VTR_LOG("Echo the bitstream (w/o time stamp) to an XML file: %s.\n",
            argv[2]);
  }
  /* Output the bitstream distribution to an XML file
   * This is optional only used when there is a third argument
   */
  if (4 <= argc) { 
    openfpga::report_architecture_bitstream_distribution(test_bitstream, argv[3], true);
    VTR_LOG("Echo the bitstream distribution (with time stamp) to an XML file: %s.\n",
            argv[3]);
    openfpga::report_architecture_bitstream_distribution(test_bitstream, argv[3], false);
    VTR_LOG("Echo the bitstream distribution (w/o time stamp) to an XML file: %s.\n",
            argv[3]);
  }

}


