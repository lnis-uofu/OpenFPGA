/********************************************************************
 * This file includes most utilized function to write an XML file
 *******************************************************************/
/* Headers from system goes first */
#include <fstream> 
#include <string> 

/* Headers from vtrutil library */
#include "vtr_assert.h" 

/* Headers from openfpgautil library */
#include "openfpga_digest.h" 

/* Headers from readarchopenfpga library */
#include "write_xml_utils.h" 

/********************************************************************
 * A most utilized function to write an XML attribute to file
 *******************************************************************/
void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const char* value) {
  /* Validate the file stream */
  openfpga::valid_file_stream(fp);

  fp << " " << attr << "=\"" << value << "\""; 
}

/********************************************************************
 * A most utilized function to write an XML attribute to file
 * This accepts the value as a boolean 
 *******************************************************************/
void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const bool& value) {
  /* Validate the file stream */
  openfpga::valid_file_stream(fp);

  fp << " " << attr << "=\"";
  if (true == value) {
    fp << "true"; 
  } else {
    VTR_ASSERT_SAFE(false == value);
    fp << "false"; 
  }
  fp << "\""; 
}

/********************************************************************
 * A most utilized function to write an XML attribute to file
 * This accepts the value as a float
 *******************************************************************/
void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const int& value) {
  /* Validate the file stream */
  openfpga::valid_file_stream(fp);

  fp << " " << attr << "=\"";
  fp << value;
  fp << "\""; 
}

/********************************************************************
 * A most utilized function to write an XML attribute to file
 * This accepts the value as a size_t
 *******************************************************************/
void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const size_t& value) {
  /* Validate the file stream */
  openfpga::valid_file_stream(fp);

  fp << " " << attr << "=\"";
  fp << value;
  fp << "\""; 
}

/********************************************************************
 * A most utilized function to write an XML attribute to file
 * This accepts the value as a float
 *******************************************************************/
void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const float& value) {
  /* Validate the file stream */
  openfpga::valid_file_stream(fp);

  fp << " " << attr << "=\"";
  fp << std::scientific << value;
  fp << "\""; 
}

/********************************************************************
 * FIXME: Use a common function to output ports
 * Generate the full hierarchy name for a operating pb_type
 *******************************************************************/
std::string generate_xml_port_name(const openfpga::BasicPort& pb_port) {
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
