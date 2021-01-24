#ifndef WRITE_XML_UTILS_H
#define WRITE_XML_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "circuit_library.h"
#include "openfpga_port.h" 

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const char* value);

void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const bool& value);

void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const int& value);

void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const float& value);

void write_xml_attribute(std::fstream& fp, 
                         const char* attr,
                         const size_t& value);

std::string generate_xml_port_name(const openfpga::BasicPort& pb_port);

#endif
