/********************************************************************
 * This file includes functions that outputs an OpenFPGAArch
 * data structure to XML format
 *******************************************************************/
#include <fstream>

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_circuit_library.h"
#include "write_xml_technology_library.h"
#include "write_xml_simulation_setting.h"
#include "write_xml_config_protocol.h"
#include "write_xml_openfpga_arch.h"

/********************************************************************
 * A writer to output an OpenFPGAArch to XML format
 *******************************************************************/
void write_xml_openfpga_arch(const char* fname, 
                             const openfpga::Arch& openfpga_arch) {
  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node for openfpga_arch */
  fp << "<openfpga_architecture>" << "\n";

  /* Write the technology library */
  write_xml_technology_library(fp, fname, openfpga_arch.tech_lib);

  /* Write the circuit library */
  write_xml_circuit_library(fp, fname, openfpga_arch.circuit_lib);

  /* Write the configuration protocol */
  write_xml_config_protocol(fp, fname, openfpga_arch.config_protocol, openfpga_arch.circuit_lib);

  fp << "</openfpga_architecture>" << "\n";

  /* Write the simulation */
  write_xml_simulation_setting(fp, fname, openfpga_arch.sim_setting);
}
