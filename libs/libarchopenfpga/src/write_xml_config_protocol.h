#ifndef WRITE_XML_CONFIG_PROTOCOL_H
#define WRITE_XML_CONFIG_PROTOCOL_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "circuit_library.h"
#include "config_protocol.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_config_protocol(std::fstream& fp, const char* fname,
                               const ConfigProtocol& config_protocol,
                               const CircuitLibrary& circuit_lib);

#endif
