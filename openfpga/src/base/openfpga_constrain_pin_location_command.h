#ifndef OPENFPGA_CONSTRAIN_PIN_LOCATION_COMMAND_H
#define OPENFPGA_CONSTRAIN_PIN_LOCATION_COMMAND_H

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

void add_openfpga_constrain_pin_location_command(openfpga::Shell<OpenfpgaContext>& shell); 

} /* end namespace openfpga */

#endif
