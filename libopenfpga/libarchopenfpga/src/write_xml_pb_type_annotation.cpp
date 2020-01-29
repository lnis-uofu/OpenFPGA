/********************************************************************
 * This file includes functions that outputs pb_type annotations to XML format
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
#include "write_xml_pb_type_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * Generate the full hierarchy name for a operating pb_type
 *******************************************************************/
static 
std::string generate_operating_pb_type_hierarchy_name(const PbTypeAnnotation& pb_type_annotation) {
  /* Iterate over the parent_pb_type and modes names, they should well match */
  VTR_ASSERT_SAFE(pb_type_annotation.operating_parent_pb_type_names().size() == pb_type_annotation.operating_parent_mode_names().size());

  std::string hie_name;

  for (size_t i = 0 ; i < pb_type_annotation.operating_parent_pb_type_names().size(); ++i) {
    hie_name += pb_type_annotation.operating_parent_pb_type_names()[i];
    hie_name += std::string("[");
    hie_name += pb_type_annotation.operating_parent_mode_names()[i];
    hie_name += std::string("]");
    hie_name += std::string(".");
  }

  /* Add the leaf pb_type */
  hie_name += pb_type_annotation.operating_pb_type_name();

  return hie_name;
}

/********************************************************************
 * Generate the full hierarchy name for a operating pb_type
 *******************************************************************/
static 
std::string generate_physical_pb_type_hierarchy_name(const PbTypeAnnotation& pb_type_annotation) {
  /* Iterate over the parent_pb_type and modes names, they should well match */
  VTR_ASSERT_SAFE(pb_type_annotation.physical_parent_pb_type_names().size() == pb_type_annotation.physical_parent_mode_names().size());

  std::string hie_name;

  for (size_t i = 0 ; i < pb_type_annotation.physical_parent_pb_type_names().size(); ++i) {
    hie_name += pb_type_annotation.physical_parent_pb_type_names()[i];
    hie_name += std::string("[");
    hie_name += pb_type_annotation.physical_parent_mode_names()[i];
    hie_name += std::string("]");
    hie_name += std::string(".");
  }

  /* Add the leaf pb_type */
  hie_name += pb_type_annotation.physical_pb_type_name();

  return hie_name;
}

/********************************************************************
 * Generate the full hierarchy name for a operating pb_type
 *******************************************************************/
static 
std::string generate_physical_pb_port_name(const BasicPort& pb_port) {
  std::string port_name;

  /* Output format: <port_name>[<LSB>:<MSB>] */
  port_name += pb_port.get_name();
  port_name += std::string("[");
  port_name += std::to_string(pb_port.get_lsb());
  port_name += std::string(":");
  port_name += std::to_string(pb_port.get_msb());
  port_name += std::string("]");

  return port_name;
}

/********************************************************************
 * A writer to output an pb_type interconnection annotation to XML format
 *******************************************************************/
static 
void write_xml_interconnect_annotation(std::fstream& fp,
                                       const char* fname,
                                       const openfpga::PbTypeAnnotation& pb_type_annotation,
                                       const std::string& interc_name) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<interconnect";
 
  write_xml_attribute(fp, "name", interc_name.c_str());
  write_xml_attribute(fp, "circuit_model_name", pb_type_annotation.interconnect_circuit_model_name(interc_name).c_str());

  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output an pb_type port annotation to XML format
 *******************************************************************/
static 
void write_xml_pb_port_annotation(std::fstream& fp,
                                  const char* fname,
                                  const openfpga::PbTypeAnnotation& pb_type_annotation,
                                  const std::string& port_name) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<port";
 
  write_xml_attribute(fp, "name", port_name.c_str());
  write_xml_attribute(fp, "physical_mode_port", generate_physical_pb_port_name(pb_type_annotation.physical_pb_type_port(port_name)).c_str());
  write_xml_attribute(fp, "physical_mode_pin_rotate_offset", pb_type_annotation.physical_pin_rotate_offset(port_name));

  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output a device variation in a technology library to XML format
 *******************************************************************/
static 
void write_xml_pb_type_annotation(std::fstream& fp,
                                  const char* fname,
                                  const openfpga::PbTypeAnnotation& pb_type_annotation) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t" << "<pb_type";

  /* Write up name of the pb_type, which is different by the type of pb_type
   * 1. operating pb_type, we output name, physical_pb_type_name
   * 1. physical pb_type, we output name
   */
  if (true == pb_type_annotation.is_operating_pb_type()) { 
    write_xml_attribute(fp, "name", generate_operating_pb_type_hierarchy_name(pb_type_annotation).c_str());
    write_xml_attribute(fp, "physical_pb_type_name", generate_physical_pb_type_hierarchy_name(pb_type_annotation).c_str());
  }
  
  if (true == pb_type_annotation.is_physical_pb_type()) { 
    write_xml_attribute(fp, "name", generate_physical_pb_type_hierarchy_name(pb_type_annotation).c_str());
  }

  /* Output physical mode name */
  if (!pb_type_annotation.physical_mode_name().empty()) { 
    write_xml_attribute(fp, "physical_mode_name", pb_type_annotation.physical_mode_name().c_str());
  }

  /* Output idle mode name */
  if (!pb_type_annotation.idle_mode_name().empty()) { 
    write_xml_attribute(fp, "idle_mode_name", pb_type_annotation.idle_mode_name().c_str());
  }

  /* Output mode_bits */
  if (!pb_type_annotation.mode_bits().empty()) { 
    /* Convert the vector of integer to string */
    std::string mode_bits_str;
    for (const size_t& bit : pb_type_annotation.mode_bits()) {
      mode_bits_str += std::to_string(bit);
    }
    write_xml_attribute(fp, "mode_bits", mode_bits_str.c_str());
  }

  /* Output circuit model name */
  if (!pb_type_annotation.circuit_model_name().empty()) { 
    write_xml_attribute(fp, "circuit_model_name", pb_type_annotation.circuit_model_name().c_str());
  }

  /* Output physical mode index factor and offset, only applicable to operating mode */
  if (true == pb_type_annotation.is_operating_pb_type()) { 
    write_xml_attribute(fp, "physical_pb_type_index_factor", pb_type_annotation.physical_pb_type_index_factor());
    write_xml_attribute(fp, "physical_pb_type_index_offset", pb_type_annotation.physical_pb_type_index_offset());
  }

  /* If there are interconnect definition or port definition, output them
   * Otherwise we can finish here 
   */
  if ( (0 == pb_type_annotation.interconnect_names().size())
    && (0 == pb_type_annotation.port_names().size()) ) {
    fp << "/>" << "\n";
    return;
  }

  fp << ">" << "\n";

  /* Output interconnects if there are any */
  for (const std::string& interc_name : pb_type_annotation.interconnect_names()) {
    write_xml_interconnect_annotation(fp, fname, pb_type_annotation, interc_name);
  }

  /* Output pb_type ports if there are any */
  for (const std::string& port_name : pb_type_annotation.port_names()) {
    write_xml_pb_port_annotation(fp, fname, pb_type_annotation, port_name);
  }
 
  fp << "\t\t</pb_type>" << "\n";
}

/********************************************************************
 * A writer to output a number of pb_type annotations to XML format
 *******************************************************************/
void write_xml_pb_type_annotations(std::fstream& fp,
                                   const char* fname,
                                   const std::vector<PbTypeAnnotation>& pb_type_annotations) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node for pb_type annotations, 
   * we apply a tab becuase pb_type annotations is just a subnode 
   * under the root node <openfpga_arch>
   */
  fp << "\t" << "<pb_type_annotations>" << "\n";

  /* Write device model one by one */ 
  for (const PbTypeAnnotation& pb_type_annotation : pb_type_annotations) {
    write_xml_pb_type_annotation(fp, fname, pb_type_annotation);
  }

  /* Write the root node for pb_type annotations */
  fp << "\t" << "</pb_type_annotations>" << "\n";
}

} /* namespace openfpga ends */
