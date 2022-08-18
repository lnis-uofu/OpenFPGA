#ifndef READ_XML_TECHNOLOGY_LIBRARY_H
#define READ_XML_TECHNOLOGY_LIBRARY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "technology_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
TechnologyLibrary read_xml_technology_library(pugi::xml_node& Node,
                                              const pugiutil::loc_data& loc_data);

#endif
