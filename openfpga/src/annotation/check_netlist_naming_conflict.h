#ifndef CHECK_NETLIST_NAMING_CONFLICT_H
#define CHECK_NETLIST_NAMING_CONFLICT_H

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

void check_netlist_naming_conflict(OpenfpgaContext& openfpga_context,
                                   const Command& cmd, const CommandContext& cmd_context); 

} /* end namespace openfpga */

#endif
