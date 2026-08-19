#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "openfpga_context.h"
#include "shell.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

#ifdef OPENFPGA_INCLUDE_YOSYS_COMMAND

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_yosys_commands(openfpga::Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
