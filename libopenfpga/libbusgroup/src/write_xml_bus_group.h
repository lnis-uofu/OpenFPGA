#ifndef WRITE_XML_BUS_GROUP_H
#define WRITE_XML_BUS_GROUP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "bus_group.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
namespace openfpga { // Begin namespace openfpga

int write_xml_bus_group(const char* fname,
                        const BusGroup& bus_group);

} // End of namespace openfpga

#endif
