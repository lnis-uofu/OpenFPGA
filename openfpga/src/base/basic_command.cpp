/********************************************************************
 * Add basic commands to the OpenFPGA shell interface, including:
 * - exit
 * - version
 * - help
 *******************************************************************/
#include "basic_command.h"

#include "command_exit_codes.h"
#include "openfpga_basic.h"
#include "openfpga_title.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: exec_external
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
static ShellCommandId add_openfpga_ext_exec_command(
  openfpga::Shell<OpenfpgaContext>& shell,
  const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("ext_exec");

  /* Add an option '--command_stream' */
  CommandOptionId opt_cmdstream = shell_cmd.add_option(
    "command", true,
    "A string stream which contains the commands to be executed");
  shell_cmd.set_option_require_value(opt_cmdstream, openfpga::OPT_STRING);

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Source a string of commands or execute a script from a file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, call_external_command);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: source
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
static ShellCommandId add_openfpga_source_command(
  openfpga::Shell<OpenfpgaContext>& shell,
  const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("source");

  /* Add an option '--command_stream' */
  CommandOptionId opt_cmdstream = shell_cmd.add_option(
    "command_stream", true,
    "A string/file stream which contains the commands to be executed");
  shell_cmd.set_option_require_value(opt_cmdstream, openfpga::OPT_STRING);

  /* Add an option '--from_file' */
  shell_cmd.add_option("from_file", false,
                       "Specify the command stream comes from a file");

  /* Add an option '--batch_mode' */
  shell_cmd.add_option(
    "batch_mode", false,
    "Enable batch mode when executing the script from a file (not a string)");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Source a string of commands or execute a script from a file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, source_existing_command);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

void add_basic_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Add a new class of commands */
  ShellCommandClassId basic_cmd_class = shell.add_command_class("Basic");

  Command shell_cmd_exit("exit");
  ShellCommandId shell_cmd_exit_id =
    shell.add_command(shell_cmd_exit, "Exit the shell");
  shell.set_command_class(shell_cmd_exit_id, basic_cmd_class);
  shell.set_command_execute_function(shell_cmd_exit_id,
                                     [shell]() { shell.exit(); });

  /* Version */
  Command shell_cmd_version("version");
  ShellCommandId shell_cmd_version_id =
    shell.add_command(shell_cmd_version, "Show version information");
  shell.set_command_class(shell_cmd_version_id, basic_cmd_class);
  shell.set_command_execute_function(shell_cmd_version_id,
                                     print_openfpga_version_info);

  /* Add a hidden command: internal_version */
  Command shell_cmd_internal_version("hidden_version");
  ShellCommandId shell_cmd_internal_version_id = shell.add_command(
    shell_cmd_internal_version, "Show internal version information", true);
  shell.set_command_class(shell_cmd_internal_version_id, basic_cmd_class);
  shell.set_command_execute_function(shell_cmd_internal_version_id,
                                     print_openfpga_version_info);

  /* Add 'source' command which can run a set of commands */
  add_openfpga_source_command(shell, basic_cmd_class,
                              std::vector<ShellCommandId>());

  /* Add 'exec_external command which can run system call */
  add_openfpga_ext_exec_command(shell, basic_cmd_class,
                                std::vector<ShellCommandId>());

  /* Note:
   * help MUST be the last to add because the linking to execute function will
   * do a snapshot on the shell
   */
  Command shell_cmd_help("help");
  ShellCommandId shell_cmd_help_id =
    shell.add_command(shell_cmd_help, "Launch help desk");
  shell.set_command_class(shell_cmd_help_id, basic_cmd_class);
  shell.set_command_execute_function(shell_cmd_help_id,
                                     [shell]() { shell.print_commands(); });
}

} /* end namespace openfpga */
