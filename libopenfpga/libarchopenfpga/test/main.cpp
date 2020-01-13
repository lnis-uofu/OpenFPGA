/********************************************************************
 * Unit test functions to validate the correctness of 
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
#include "read_openfpga_xml.h"

int main(int argc, const char** argv) {
  /* Parse the circuit library from an XML file */
  const CircuitSettings& circuit_setting = read_xml_openfpga_arch(argv[1]); 

  /* Check the circuit library */
  
  /* Output the circuit library to an XML file*/
}


