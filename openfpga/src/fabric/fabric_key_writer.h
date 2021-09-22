#ifndef FABRIC_KEY_WRITER_H
#define FABRIC_KEY_WRITER_H

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

int write_fabric_key_to_xml_file(const ModuleManager& module_manager,
                                 const std::string& fname,
                                 const e_config_protocol_type& config_protocol_type,
                                 const bool& verbose);

} /* end namespace openfpga */

#endif
