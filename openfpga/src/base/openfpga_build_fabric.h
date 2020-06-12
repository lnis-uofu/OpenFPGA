#ifndef OPENFPGA_BUILD_FABRIC_H
#define OPENFPGA_BUILD_FABRIC_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_fabric(OpenfpgaContext& openfpga_ctx,
                 const Command& cmd, const CommandContext& cmd_context); 

int write_fabric_hierarchy(const OpenfpgaContext& openfpga_ctx,
                           const Command& cmd, const CommandContext& cmd_context); 

} /* end namespace openfpga */

#endif
