#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include "command.h"
#include "command_context.h"

/* Begin namespace minishell */
namespace minishell {

bool parse_command(const std::vector<std::string>& argv,
                   const Command& cmd,
                   CommandContext& cmd_context);

} /* End namespace minshell */

#endif
