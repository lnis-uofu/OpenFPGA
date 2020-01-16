/********************************************************************
 * This file includes most utilized function to write an XML file
 *******************************************************************/
/* Headers from system goes first */
#include <fstream> 
#include <string> 

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

