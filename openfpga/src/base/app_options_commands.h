#ifndef APP_OPTIONS_COMMANDS_H
#define APP_OPTIONS_COMMANDS_H

#include "openfpga_context.h"
#include "shell.h"

/********************************************************************
 * Commands to set and report the application options of the session
 * - set_app_option
 * - report_app_option
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

void add_app_options_commands(openfpga::Shell<OpenfpgaContext>& shell);

} /* End namespace openfpga */

#endif
