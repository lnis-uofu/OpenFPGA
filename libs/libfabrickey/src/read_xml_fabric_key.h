#ifndef READ_XML_FABRIC_KEY_H
#define READ_XML_FABRIC_KEY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "fabric_key.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
FabricKey read_xml_fabric_key(const char* key_fname);

#endif
