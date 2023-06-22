#ifndef READ_XML_IO_NAME_MAP_H
#define READ_XML_IO_NAME_MAP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "io_name_map.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

int read_xml_io_name_map(const char* fname, IoNameMap& io_name_map);

}  // End of namespace openfpga

#endif
