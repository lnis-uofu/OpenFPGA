#ifndef SHELL_VPR_CMD_H
#define SHELL_VPR_CMD_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "shell.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void add_vpr_commands(openfpga::Shell<OpenfpgaContext>& shell); 

} /* end namespace openfpga */

#endif
