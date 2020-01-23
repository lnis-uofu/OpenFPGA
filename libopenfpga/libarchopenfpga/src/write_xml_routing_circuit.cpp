/********************************************************************
 * This file includes functions that outputs routing circuit definition
 * to XML format
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
#include "write_xml_routing_circuit.h"

/********************************************************************
 * Write switch circuit model definition in XML format
 *******************************************************************/
static 
void write_xml_routing_component_circuit(std::fstream& fp,
                                         const char* fname,
                                         const std::string& routing_component_name, 
                                         const CircuitLibrary& circuit_lib,
                                         const std::map<std::string, CircuitModelId>& switch2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Iterate over the mapping */
  for (std::map<std::string, CircuitModelId>::const_iterator it = switch2circuit.begin(); 
       it != switch2circuit.end(); 
       ++it) {
    fp << "\t\t" << "<" << routing_component_name;
    write_xml_attribute(fp, "name", it->first.c_str()); 
    write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(it->second).c_str()); 
    fp << "/>" << "\n";
  }
}

/********************************************************************
 * Write Connection block circuit models in XML format
 *******************************************************************/
void write_xml_cb_switch_circuit(std::fstream& fp,
                                 const char* fname,
                                 const CircuitLibrary& circuit_lib,
                                 const std::map<std::string, CircuitModelId>& switch2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "\t" << "<connection_block>" << "\n";

  /* Write each switch circuit definition */ 
  write_xml_routing_component_circuit(fp, fname, std::string("switch"), circuit_lib, switch2circuit);

  /* Finish writing the root node */
  fp << "\t" << "</connection_block>" << "\n";
}

/********************************************************************
 * Write Switch block circuit models in XML format
 *******************************************************************/
void write_xml_sb_switch_circuit(std::fstream& fp,
                                 const char* fname,
                                 const CircuitLibrary& circuit_lib,
                                 const std::map<std::string, CircuitModelId>& switch2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "\t" << "<switch_block>" << "\n";

  /* Write each switch circuit definition */ 
  write_xml_routing_component_circuit(fp, fname, std::string("switch"), circuit_lib, switch2circuit);

  /* Finish writing the root node */
  fp << "\t" << "</switch_block>" << "\n";
}

/********************************************************************
 * Write routing segment circuit models in XML format
 *******************************************************************/
void write_xml_routing_segment_circuit(std::fstream& fp,
                                       const char* fname,
                                       const CircuitLibrary& circuit_lib,
                                       const std::map<std::string, CircuitModelId>& seg2circuit) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "\t" << "<routing_segment>" << "\n";

  /* Write each routing segment circuit definition */ 
  write_xml_routing_component_circuit(fp, fname, std::string("segment"), circuit_lib, seg2circuit);

  /* Finish writing the root node */
  fp << "\t" << "</routing_segment>" << "\n";
}

/********************************************************************
 * Write direction connection circuit models in XML format
 *******************************************************************/
void write_xml_direct_circuit(std::fstream& fp,
                              const char* fname,
                              const CircuitLibrary& circuit_lib,
                              const std::map<std::string, CircuitModelId>& direct2circuit) {
  /* If the direct2circuit is empty, we do not output XML */
  if (direct2circuit.empty()) {
    return;
  }

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "\t" << "<direct_connection>" << "\n";

  /* Write each direct connection circuit definition */ 
  write_xml_routing_component_circuit(fp, fname, std::string("direct"), circuit_lib, direct2circuit);

  /* Finish writing the root node */
  fp << "\t" << "</direct_connection>" << "\n";
}
