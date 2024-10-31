#ifndef WRITE_XML_FABRIC_PIN_PHYSICAL_LOCATION_H
#define WRITE_XML_FABRIC_PIN_PHYSICAL_LOCATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_xml_fabric_pin_physical_location(const char* fname,
                                           const std::string& module_name,
                                           const ModuleManager& module_manager,
                                           const bool& show_invalid_side,
                                           const bool& include_time_stamp,
                                           const bool& verbose);

} /* end namespace openfpga */

#endif
