#ifndef READ_XML_CIRCUIT_LIBRARY_H
#define READ_XML_CIRCUIT_LIBRARY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
CircuitLibrary read_xml_circuit_library(pugi::xml_node& Node,
                                        const pugiutil::loc_data& loc_data);

#endif
