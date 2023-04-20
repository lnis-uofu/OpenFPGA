#ifndef READ_XML_CLOCK_NETWORK_H
#define READ_XML_CLOCK_NETWORK_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "clock_network.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

ClockNetwork read_xml_clock_network(const char* fname);

}  // End of namespace openfpga

#endif
