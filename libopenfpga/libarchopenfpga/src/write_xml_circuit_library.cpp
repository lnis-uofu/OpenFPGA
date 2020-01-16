/********************************************************************
 * This file includes functions that outputs a circuit library to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>

/* Headers from vtr util library */
#include "vtr_log.h"
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_utils.h" 
#include "write_xml_circuit_library.h"

/********************************************************************
 * A writer to output a circuit model to XML format
 *******************************************************************/
static 
void write_xml_circuit_model(std::fstream& fp,
                             const char* fname,
                             const CircuitLibrary& circuit_lib,
                             const CircuitModelId& model) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the definition of circuit model */
  fp << "\t\t" << "<circuit_model";
  write_xml_attribute(fp, "type", CIRCUIT_MODEL_TYPE_STRING[circuit_lib.model_type(model)]); 
  write_xml_attribute(fp, "name", circuit_lib.model_name(model).c_str()); 
  write_xml_attribute(fp, "prefix", circuit_lib.model_prefix(model).c_str()); 
  if (true == circuit_lib.model_is_default(model)) { 
    write_xml_attribute(fp, "is_default", "true"); 
  }
  if (true == circuit_lib.dump_structural_verilog(model)) { 
    write_xml_attribute(fp, "dump_structural_verilog", "true"); 
  }
  if (!circuit_lib.model_circuit_netlist(model).empty()) {
    write_xml_attribute(fp, "circuit_netlist", circuit_lib.model_circuit_netlist(model).c_str()); 
  }
  if (!circuit_lib.model_verilog_netlist(model).empty()) {
    write_xml_attribute(fp, "verilog_netlist", circuit_lib.model_verilog_netlist(model).c_str()); 
  }
  fp << ">" << "\n";

  /* Put an end to the XML definition of this circuit model */
  fp << "\t\t" << "</circuit_model>\n";
}

/********************************************************************
 * A writer to output a circuit library to XML format
 *******************************************************************/
void write_xml_circuit_library(std::fstream& fp,
                               const char* fname,
                               const CircuitLibrary& circuit_lib) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node for circuit_library, 
   * we apply a tab becuase circuit library is a subnode 
   * under the root node <openfpga_arch>
   */
  fp << "\t" << "<circuit_library>" << "\n";

  /* Write circuit model one by one */ 
  for (const CircuitModelId& model : circuit_lib.models()) {
    write_xml_circuit_model(fp, fname, circuit_lib, model);
  } 

  /* Write the root node for circuit_library */
  fp << "\t" << "</circuit_library>" << "\n";
}
