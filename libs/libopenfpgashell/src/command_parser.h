#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "command.h"
#include "command_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* Begin namespace openfpga */
namespace openfpga {

bool parse_command(const std::vector<std::string>& argv,
                   const Command& cmd,
                   CommandContext& cmd_context);

} /* End namespace openfpga */

#endif
