/********************************************************************
 * Add basic commands to the OpenFPGA shell interface, including:
 * - exit
 * - version
 * - help
 *******************************************************************/
#include "basic_command.h"

#include "command_exit_codes.h"
#include "openfpga_title.h"

/* begin namespace openfpga */
namespace openfpga {

static int source_existing_command(openfpga::Shell<OpenfpgaContext>* shell,
                                   OpenfpgaContext& openfpga_ctx,
                                   const Command& cmd,
                                   const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("from_file");
  CommandOptionId opt_batch_mode = cmd.option("batch_mode");
  CommandOptionId opt_ss = cmd.option("command_stream");

  bool is_cmd_file = cmd_context.option_enable(cmd, opt_file);
  std::string cmd_ss = cmd_context.option_value(cmd, opt_ss);

  int status = CMD_EXEC_SUCCESS;

  /* If a file is specified, run script mode of the shell, otherwise,  */
  if (is_cmd_file) {
    shell->run_script_mode(cmd_ss.c_str(), openfpga_ctx,
                           cmd_context.option_enable(cmd, opt_batch_mode));
  } else {
    /* Split the string with ';' and run each command */
    /* Remove the space at the end of the line
     * So that we can check easily if there is a continued line in the end
     */
    StringToken cmd_ss_tokenizer(cmd_ss);

    for (std::string cmd_part : cmd_ss_tokenizer.split(";")) {
      StringToken cmd_part_tokenizer(cmd_part);
      cmd_part_tokenizer.rtrim(std::string(" "));
      std::string single_cmd_line = cmd_part_tokenizer.data();

      if (!single_cmd_line.empty()) {
        status = shell->execute_command(single_cmd_line.c_str(), openfpga_ctx);

        /* Check the execution status of the command,
         * if fatal error happened, we should abort immediately
         */
        if (CMD_EXEC_FATAL_ERROR == status) {
          return CMD_EXEC_FATAL_ERROR;
        }
      }
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * - Add a command to Shell environment: repack
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

  /* Add command 'repack' to the Shell */
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

  /* Add 'source' command which can run a set of commands */
  add_openfpga_source_command(shell, basic_cmd_class,
                              std::vector<ShellCommandId>());
}

} /* end namespace openfpga */
