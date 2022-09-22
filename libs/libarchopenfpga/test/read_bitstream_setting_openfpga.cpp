/********************************************************************
 * Unit test functions to validate the correctness of 
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga */
#include "read_xml_openfpga_arch.h"
#include "write_xml_openfpga_arch.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  /* Parse the simulation settings from an XML file */
  const openfpga::BitstreamSetting& openfpga_bitstream_setting = read_xml_openfpga_bitstream_settings(argv[1]); 
  VTR_LOG("Parsed bitstream settings from XML %s.\n",
          argv[1]);

  /* Output the simulation settings to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) { 
    write_xml_openfpga_bitstream_settings(argv[2], openfpga_bitstream_setting);
    VTR_LOG("Echo the OpenFPGA bitstream settings to an XML file: %s.\n",
            argv[2]);
  }
}
