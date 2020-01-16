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

  /* Parse the circuit library from an XML file */
  const OpenFPGAArch& openfpga_arch = read_xml_openfpga_arch(argv[1]); 
  VTR_LOG("Parsed %lu circuit models from XML into circuit library.\n",
          openfpga_arch.circuit_lib.num_models());

  /* Check the circuit library */
  
  /* Output the circuit library to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) { 
    write_xml_openfpga_arch(argv[2], openfpga_arch);
    VTR_LOG("Echo the OpenFPGA architecture to an XML file: %s.\n",
            argv[2]);
  }
}


