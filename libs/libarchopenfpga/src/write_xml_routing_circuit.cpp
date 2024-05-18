/********************************************************************
 * This file includes functions that outputs routing circuit definition
 * to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <string>

/* Headers from vtr util library */
#include "openfpga_digest.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga library */
#include "write_xml_routing_circuit.h"
#include "write_xml_utils.h"

/********************************************************************
 * Write switch circuit model definition in XML format
 *******************************************************************/
static void write_xml_routing_component_circuit(
  std::fstream& fp, const char* fname,
  const std::string& routing_component_name, const CircuitLibrary& circuit_lib,
  const std::map<std::string, CircuitModelId>& switch2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Iterate over the mapping */
  for (std::map<std::string, CircuitModelId>::const_iterator it =
         switch2circuit.begin();
       it != switch2circuit.end(); ++it) {
    fp << "\t\t"
       << "<" << routing_component_name;
    write_xml_attribute(fp, "name", it->first.c_str());
    write_xml_attribute(fp, "circuit_model_name",
                        circuit_lib.model_name(it->second).c_str());
    fp << "/>"
       << "\n";
  }
}

/********************************************************************
 * Write switch circuit model definition in XML format
 *******************************************************************/
static void write_xml_direct_component_circuit(
  std::fstream& fp, const char* fname, const std::string& direct_tag_name,
  const CircuitLibrary& circuit_lib, const ArchDirect& arch_direct,
  const ArchDirectId& direct_id) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Iterate over the mapping */
  fp << "\t\t"
     << "<" << direct_tag_name;
  write_xml_attribute(fp, "name", arch_direct.name(direct_id).c_str());
  write_xml_attribute(
    fp, "circuit_model_name",
    circuit_lib.model_name(arch_direct.circuit_model(direct_id)).c_str());
  write_xml_attribute(fp, "type",
                      DIRECT_TYPE_STRING[size_t(arch_direct.type(direct_id))]);
  write_xml_attribute(fp, "x_dir",
                      DIRECT_DIRECTION_STRING[arch_direct.x_dir(direct_id)]);
  write_xml_attribute(fp, "y_dir",
                      DIRECT_DIRECTION_STRING[arch_direct.y_dir(direct_id)]);
  fp << "/>"
     << "\n";
}

/********************************************************************
 * Write Connection block circuit models in XML format
 *******************************************************************/
void write_xml_cb_switch_circuit(
  std::fstream& fp, const char* fname, const CircuitLibrary& circuit_lib,
  const std::map<std::string, CircuitModelId>& switch2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "\t"
     << "<connection_block>"
     << "\n";

  /* Write each switch circuit definition */
  write_xml_routing_component_circuit(fp, fname, std::string("switch"),
                                      circuit_lib, switch2circuit);

  /* Finish writing the root node */
  fp << "\t"
     << "</connection_block>"
     << "\n";
}

/********************************************************************
 * Write Switch block circuit models in XML format
 *******************************************************************/
void write_xml_sb_switch_circuit(
  std::fstream& fp, const char* fname, const CircuitLibrary& circuit_lib,
  const std::map<std::string, CircuitModelId>& switch2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "\t"
     << "<switch_block>"
     << "\n";

  /* Write each switch circuit definition */
  write_xml_routing_component_circuit(fp, fname, std::string("switch"),
                                      circuit_lib, switch2circuit);

  /* Finish writing the root node */
  fp << "\t"
     << "</switch_block>"
     << "\n";
}

/********************************************************************
 * Write routing segment circuit models in XML format
 *******************************************************************/
void write_xml_routing_segment_circuit(
  std::fstream& fp, const char* fname, const CircuitLibrary& circuit_lib,
  const std::map<std::string, CircuitModelId>& seg2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "\t"
     << "<routing_segment>"
     << "\n";

  /* Write each routing segment circuit definition */
  write_xml_routing_component_circuit(fp, fname, std::string("segment"),
                                      circuit_lib, seg2circuit);

  /* Finish writing the root node */
  fp << "\t"
     << "</routing_segment>"
     << "\n";
}

/********************************************************************
 * Write direction connection circuit models in XML format
 *******************************************************************/
void write_xml_direct_circuit(std::fstream& fp, const char* fname,
                              const CircuitLibrary& circuit_lib,
                              const ArchDirect& arch_direct) {
  /* If the direct2circuit is empty, we do not output XML */
  if (0 == arch_direct.directs().size()) {
    return;
  }

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "\t"
     << "<direct_connection>"
     << "\n";

  /* Write each direct connection circuit definition */
  for (const ArchDirectId& direct_id : arch_direct.directs()) {
    write_xml_direct_component_circuit(fp, fname, std::string("direct"),
                                       circuit_lib, arch_direct, direct_id);
  }

  /* Finish writing the root node */
  fp << "\t"
     << "</direct_connection>"
     << "\n";
}
