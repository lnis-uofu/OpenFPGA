#ifndef OPENFPGA_WRITE_GSB_H
#define OPENFPGA_WRITE_GSB_H

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

int write_gsb(const OpenfpgaContext& openfpga_ctx,
              const Command& cmd, const CommandContext& cmd_context); 

} /* end namespace openfpga */

#endif
