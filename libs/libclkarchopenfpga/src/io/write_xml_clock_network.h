#ifndef WRITE_XML_CLOCK_NETWORK_H
#define WRITE_XML_CLOCK_NETWORK_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "clock_network.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
namespace openfpga {  // Begin namespace openfpga

int write_xml_clock_network(const char* fname, const ClockNetwork& clk_ntwk);

}  // End of namespace openfpga

#endif
