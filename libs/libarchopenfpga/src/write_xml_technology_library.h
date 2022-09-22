#ifndef WRITE_XML_TECHNOLOGY_LIBRARY_H
#define WRITE_XML_TECHNOLOGY_LIBRARY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "technology_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_technology_library(std::fstream& fp,
                                  const char* fname,
                                  const TechnologyLibrary& tech_lib);

#endif
