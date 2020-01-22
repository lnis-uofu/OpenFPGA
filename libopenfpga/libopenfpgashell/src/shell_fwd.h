#ifndef SHELL_FWD_H
#define SHELL_FWD_H

#include "vtr_strong_id.h"

/* Begin namespace minishell */
namespace minishell {

/*********************************************************************
 * A strong id for the options used by a command 
 ********************************************************************/
struct shell_command_id_tag;
struct shell_command_class_id_tag;

typedef vtr::StrongId<shell_command_id_tag> ShellCommandId;
typedef vtr::StrongId<shell_command_class_id_tag> ShellCommandClassId;

} /* End namespace minshell */

#endif
