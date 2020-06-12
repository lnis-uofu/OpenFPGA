#ifndef WRITE_XML_ROUTING_CIRCUIT_H
#define WRITE_XML_ROUTING_CIRCUIT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <map>
#include <string>

#include "circuit_library.h"
#include "arch_direct.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_cb_switch_circuit(std::fstream& fp,
                                 const char* fname,
                                 const CircuitLibrary& circuit_lib,
                                 const std::map<std::string, CircuitModelId>& switch2circuit);

void write_xml_sb_switch_circuit(std::fstream& fp,
                                 const char* fname,
                                 const CircuitLibrary& circuit_lib,
                                 const std::map<std::string, CircuitModelId>& switch2circuit);

void write_xml_routing_segment_circuit(std::fstream& fp,
                                       const char* fname,
                                       const CircuitLibrary& circuit_lib,
                                       const std::map<std::string, CircuitModelId>& seg2circuit);

void write_xml_direct_circuit(std::fstream& fp,
                              const char* fname,
                              const CircuitLibrary& circuit_lib,
                              const ArchDirect& arch_direct);

#endif
