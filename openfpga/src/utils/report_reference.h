#ifndef REPORT_REFERENCE_H
#define REPORT_REFERENCE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "module_manager.h"
#include "module_name_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {
int report_reference(const char* fname, const std::string& module_name,
                     const ModuleManager& module_manager,
                     const ModuleNameMap& module_name_map,
                     const bool& include_time_stamp, const bool& verbose);

} /* end namespace openfpga */

#endif
