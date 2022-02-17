#ifndef READ_XML_BUS_GROUP_H
#define READ_XML_BUS_GROUP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "bus_group.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
BusGroup read_xml_bus_group(const char* fname);

#endif
