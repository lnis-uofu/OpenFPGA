#ifndef FABRIC_HIERARCHY_WRITER_H
#define FABRIC_HIERARCHY_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_fabric_hierarchy_to_text_file(const ModuleManager& module_manager,
                                        const std::string& fname,
                                        const bool& verbose);

} /* end namespace openfpga */

#endif
