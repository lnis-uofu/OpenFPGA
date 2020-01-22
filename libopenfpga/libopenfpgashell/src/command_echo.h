#ifndef COMMAND_ECHO_H
#define COMMAND_ECHO_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "command.h"
#include "command_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* Begin namespace minishell */
namespace minishell {

void print_command_options(const Command& cmd);

void print_command_context(const Command& cmd, 
                           const CommandContext& cmd_context);

} /* End namespace minshell */

#endif
