#ifndef READ_XML_IO_LOCATION_MAP_H
#define READ_XML_IO_LOCATION_MAP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "io_location_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* Begin namespace openfpga */
namespace openfpga {

IoLocationMap read_xml_io_location_map(const char* fname);

} /* End namespace openfpga*/

#endif
