#ifndef FABRIC_HIERARCHY_WRITER_H
#define FABRIC_HIERARCHY_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "openfpga_context.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_fabric_hierarchy_to_text_file(
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const std::string& fname, const std::string& root_module_names,
  const std::string& module_name_filter, const size_t& hie_depth_to_stop,
  const bool& exclude_empty_modules, const bool& verbose);

} /* end namespace openfpga */

#endif
