#ifndef WRITE_XML_CIRCUIT_LIBRARY_H
#define WRITE_XML_CIRCUIT_LIBRARY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_circuit_library(std::fstream& fp,
                               const char* fname,
                               const CircuitLibrary& circuit_lib);

#endif
