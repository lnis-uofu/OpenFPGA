#ifndef READ_XML_MODULE_NAME_MAP_H
#define READ_XML_MODULE_NAME_MAP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "module_name_map.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

int read_xml_module_name_map(const char* fname, ModuleNameMap& module_name_map);

}  // End of namespace openfpga

#endif
