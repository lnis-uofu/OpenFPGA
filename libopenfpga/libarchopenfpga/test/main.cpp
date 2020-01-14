/********************************************************************
 * Unit test functions to validate the correctness of 
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
#include "vtr_log.h"

#include "read_openfpga_xml.h"

int main(int argc, const char** argv) {
  /* Parse the circuit library from an XML file */
  const CircuitSettings& circuit_settings = read_xml_openfpga_arch(argv[1]); 
  VTR_LOG("Parsed %lu circuit models from XML into circuit library.\n",
          circuit_settings.circuit_lib.num_models());

  /* Check the circuit library */
  
  /* Output the circuit library to an XML file*/
}


