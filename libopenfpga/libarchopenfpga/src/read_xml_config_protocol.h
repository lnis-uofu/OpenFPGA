#ifndef READ_XML_CONFIG_PROTOCOL_H
#define READ_XML_CONFIG_PROTOCOL_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "config_protocol.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
ConfigProtocol read_xml_config_protocol(pugi::xml_node& Node,
                                        const pugiutil::loc_data& loc_data);

#endif
