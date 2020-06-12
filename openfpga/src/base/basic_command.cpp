/********************************************************************
 * Add basic commands to the OpenFPGA shell interface, including:
 * - exit
 * - help
 *******************************************************************/
#include "basic_command.h"

/* begin namespace openfpga */
namespace openfpga {

void add_basic_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Add a new class of commands */
  ShellCommandClassId basic_cmd_class = shell.add_command_class("Basic");

  Command shell_cmd_exit("exit");
  ShellCommandId shell_cmd_exit_id = shell.add_command(shell_cmd_exit, "Exit the shell");
  shell.set_command_class(shell_cmd_exit_id, basic_cmd_class);
  shell.set_command_execute_function(shell_cmd_exit_id, [shell](){shell.exit();});

  /* Note: help must be the last to add because the linking to execute function will do a snapshot on the shell */
  Command shell_cmd_help("help");
  ShellCommandId shell_cmd_help_id = shell.add_command(shell_cmd_help, "Launch help desk");
  shell.set_command_class(shell_cmd_help_id, basic_cmd_class);
  shell.set_command_execute_function(shell_cmd_help_id, [shell](){shell.print_commands();});
} 

} /* end namespace openfpga */
