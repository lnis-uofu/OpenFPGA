#ifndef READ_XML_BUS_GROUP_H
#define READ_XML_BUS_GROUP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "bus_group.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

BusGroup read_xml_bus_group(const char* fname);

}  // End of namespace openfpga

#endif
