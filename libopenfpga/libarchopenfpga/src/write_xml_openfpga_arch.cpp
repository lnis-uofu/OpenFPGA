/********************************************************************
 * This file includes functions that outputs an OpenFPGAArch
 * data structure to XML format
 *******************************************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_circuit_library.h"
#include "write_xml_technology_library.h"
#include "write_xml_simulation_setting.h"
#include "write_xml_config_protocol.h"
#include "write_xml_routing_circuit.h"
#include "write_xml_pb_type_annotation.h"
#include "write_xml_openfpga_arch.h"

/********************************************************************
 * A writer to output an OpenFPGA arch database to XML format
 *******************************************************************/
void write_xml_openfpga_arch(const char* fname, 
                             const openfpga::Arch& openfpga_arch) {
  vtr::ScopedStartFinishTimer timer("Write OpenFPGA architecture");

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

  /* Write the connection block circuit definition */
  write_xml_cb_switch_circuit(fp, fname, openfpga_arch.circuit_lib, openfpga_arch.cb_switch2circuit);

  /* Write the switch block circuit definition */
  write_xml_sb_switch_circuit(fp, fname, openfpga_arch.circuit_lib, openfpga_arch.sb_switch2circuit);

  /* Write the routing segment circuit definition */
  write_xml_routing_segment_circuit(fp, fname, openfpga_arch.circuit_lib, openfpga_arch.routing_seg2circuit);

  /* Write the direct connection circuit definition */
  write_xml_direct_circuit(fp, fname, openfpga_arch.circuit_lib, openfpga_arch.arch_direct);

  /* Write the pb_type annotations */
  openfpga::write_xml_pb_type_annotations(fp, fname, openfpga_arch. pb_type_annotations);

  fp << "</openfpga_architecture>" << "\n";

  /* Write the simulation */
  write_xml_simulation_setting(fp, fname, openfpga_arch.sim_setting);

  /* Close the file stream */
  fp.close();
}

/********************************************************************
 * A writer to output an OpenFPGA simulation setting database to XML format
 *******************************************************************/
void write_xml_openfpga_simulation_settings(const char* fname, 
                                            const openfpga::SimulationSetting& openfpga_sim_setting) {
  vtr::ScopedStartFinishTimer timer("Write OpenFPGA simulation settings");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the simulation */
  write_xml_simulation_setting(fp, fname, openfpga_sim_setting);

  /* Close the file stream */
  fp.close();
}
