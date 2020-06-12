#ifndef WRITE_XML_UTILS_H
#define WRITE_XML_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "circuit_library.h"

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

#endif
