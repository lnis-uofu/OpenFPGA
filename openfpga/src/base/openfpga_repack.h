#ifndef OPENFPGA_REPACK_H
#define OPENFPGA_REPACK_H

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

int repack(OpenfpgaContext& openfpga_ctx,
           const Command& cmd, const CommandContext& cmd_context); 

} /* end namespace openfpga */

#endif
