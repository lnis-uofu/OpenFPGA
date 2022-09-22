/********************************************************************
 * This file includes functions that outputs a configuration protocol to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_utils.h" 
#include "write_xml_config_protocol.h"

/********************************************************************
 * A writer to output a configuration memory organization to XML format
 *******************************************************************/
static 
void write_xml_config_organization(std::fstream& fp,
                                   const char* fname,
                                   const ConfigProtocol& config_protocol,
                                   const CircuitLibrary& circuit_lib) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t" << "<organization";
  write_xml_attribute(fp, "type", CONFIG_PROTOCOL_TYPE_STRING[config_protocol.type()]);
  write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(config_protocol.memory_model()).c_str());
  fp << "/>" << "\n";

  /* Output BL/WL protocols */
  fp << "\t\t\t" << "<bl";
  write_xml_attribute(fp, "protocol", BLWL_PROTOCOL_TYPE_STRING[config_protocol.bl_protocol_type()]);
  write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(config_protocol.bl_memory_model()).c_str());
  write_xml_attribute(fp, "num_banks", config_protocol.bl_num_banks());
  fp << "/>" << "\n";

  fp << "\t\t\t" << "<wl";
  write_xml_attribute(fp, "protocol", BLWL_PROTOCOL_TYPE_STRING[config_protocol.wl_protocol_type()]);
  write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(config_protocol.wl_memory_model()).c_str());
  write_xml_attribute(fp, "num_banks", config_protocol.wl_num_banks());
  fp << "/>" << "\n";

  fp << "\t" << "</organization>" << "\n";
}

/********************************************************************
 * A writer to output a configuration protocol to XML format
 * Note:
 * This function should be run AFTER the function
 * link_config_protocol_to_circuit_library()
 *******************************************************************/
void write_xml_config_protocol(std::fstream& fp,
                               const char* fname,
                               const ConfigProtocol& config_protocol,
                               const CircuitLibrary& circuit_lib) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "\t" << "<configuration_protocol>" << "\n";

  /* Write configuration memory organization */ 
  write_xml_config_organization(fp, fname, config_protocol, circuit_lib);

  /* Finish writing the root node */
  fp << "\t" << "</configuration_protocol>" << "\n";
}
