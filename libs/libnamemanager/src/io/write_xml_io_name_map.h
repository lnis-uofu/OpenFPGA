#ifndef WRITE_XML_IO_NAME_MAP_H
#define WRITE_XML_IO_NAME_MAP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "io_name_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
namespace openfpga {  // Begin namespace openfpga

int write_xml_io_name_map(const char* fname, const IoNameMap& io_name_map);

}  // End of namespace openfpga

#endif
