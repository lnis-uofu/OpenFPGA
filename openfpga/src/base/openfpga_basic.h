#ifndef OPENFPGA_BASIC_H
#define OPENFPGA_BASIC_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "openfpga_context.h"
#include "shell.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int source_existing_command(openfpga::Shell<OpenfpgaContext>* shell,
                            OpenfpgaContext& openfpga_ctx, const Command& cmd,
                            const CommandContext& cmd_context);

int call_external_command(const Command& cmd,
                          const CommandContext& cmd_context);

} /* end namespace openfpga */

#endif
