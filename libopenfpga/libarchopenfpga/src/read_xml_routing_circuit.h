#ifndef READ_XML_ROUTING_CIRCUIT_H
#define READ_XML_ROUTING_CIRCUIT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <map>

#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "circuit_library.h"
#include "arch_direct.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
std::map<std::string, CircuitModelId> read_xml_cb_switch_circuit(pugi::xml_node& Node,
                                                                 const pugiutil::loc_data& loc_data,
                                                                 const CircuitLibrary& circuit_lib);

std::map<std::string, CircuitModelId> read_xml_sb_switch_circuit(pugi::xml_node& Node,
                                                                 const pugiutil::loc_data& loc_data,
                                                                 const CircuitLibrary& circuit_lib);

std::map<std::string, CircuitModelId> read_xml_routing_segment_circuit(pugi::xml_node& Node,
                                                                       const pugiutil::loc_data& loc_data,
                                                                       const CircuitLibrary& circuit_lib);

ArchDirect read_xml_direct_circuit(pugi::xml_node& Node,
                                   const pugiutil::loc_data& loc_data,
                                   const CircuitLibrary& circuit_lib);


#endif
