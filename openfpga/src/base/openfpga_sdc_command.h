#ifndef OPENFPGA_SDC_COMMAND_H
#define OPENFPGA_SDC_COMMAND_H

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

void add_openfpga_sdc_commands(openfpga::Shell<OpenfpgaContext>& shell); 

} /* end namespace openfpga */

#endif
