#ifndef WRITE_XML_MODULE_NAME_MAP_H
#define WRITE_XML_MODULE_NAME_MAP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "module_name_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
namespace openfpga {  // Begin namespace openfpga

int write_xml_module_name_map(const char* fname,
                              const ModuleNameMap& module_name_map,
                              const bool& include_time_stamp,
                              const bool& verbose);

}  // End of namespace openfpga

#endif
