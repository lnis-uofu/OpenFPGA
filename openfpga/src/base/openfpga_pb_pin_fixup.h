#ifndef OPENFPGA_PB_PIN_FIXUP_H
#define OPENFPGA_PB_PIN_FIXUP_H

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

void pb_pin_fixup(OpenfpgaContext& openfpga_context, 
                  const Command& cmd, const CommandContext& cmd_context); 

} /* end namespace openfpga */

#endif
