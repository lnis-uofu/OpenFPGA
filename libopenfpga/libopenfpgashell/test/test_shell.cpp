/********************************************************************
 * Test the shell interface by pre-defining simple commands
 * like exit() and help()
 *******************************************************************/
#include "vtr_log.h"
#include "command_parser.h"
#include "command_echo.h"
#include "shell.h"

using namespace minishell;

class ShellContext {
};

static 
void shell_cmd_help_executor(ShellContext& context, 
                             const Command& cmd,
                             const CommandContext& cmd_context) {
  VTR_LOG("Help desk:\n");
  VTR_LOG("Available commands:\n");
  VTR_LOG("help\texit\n");
}

static 
void shell_cmd_exit_executor(ShellContext& context, 
                             const Command& cmd,
                             const CommandContext& cmd_context) {
  VTR_LOG("Thank you for using!\n");
  exit(1);
}

int main(int argc, char** argv) {
  /* Create the command to launch shell in different modes */
  Command start_cmd("test_shell");
  /* Add two options:
   * '--interactive', -i': launch the interactive mode 
   * '--file', -f': launch the script mode 
   */
  CommandOptionId opt_interactive = start_cmd.add_option("interactive", false, "Launch the shell in interactive mode");
  start_cmd.set_option_short_name(opt_interactive, "i");

  CommandOptionId opt_script_mode = start_cmd.add_option("file", false, "Launch the shell in script mode");
  start_cmd.set_option_require_value(opt_script_mode, OPT_STRING);
  start_cmd.set_option_short_name(opt_script_mode, "f");

  CommandOptionId opt_help = start_cmd.add_option("help", false, "Help desk"); 
  start_cmd.set_option_short_name(opt_help, "h");

  /* Create a shell object
   * Add two commands, which are
   * 1. help
   * 2. exit
   */
  Shell<ShellContext> shell("test_shell");
  shell.add_title("This is a simple test shell\nAuthor: Xifan Tang\n");

  Command shell_cmd_help("help");
  ShellCommandId shell_cmd_help_id = shell.add_command(shell_cmd_help, "Launch help desk");
  shell.set_command_execute_function(shell_cmd_help_id, shell_cmd_help_executor);

  Command shell_cmd_exit("exit");
  ShellCommandId shell_cmd_exit_id = shell.add_command(shell_cmd_exit, "Exit the shell");
  shell.set_command_execute_function(shell_cmd_exit_id, shell_cmd_exit_executor);

  /* Create the data base for the shell */
  ShellContext shell_context;

  /* Parse the option, to avoid issues, we use the command name to replace the argv[0] */
  std::vector<std::string> cmd_opts; 
  cmd_opts.push_back(start_cmd.name());
  for (int iarg = 1; iarg < argc; ++iarg) {
    cmd_opts.push_back(std::string(argv[iarg]));
  }

  CommandContext start_cmd_context(start_cmd);
  if (false == parse_command(cmd_opts, start_cmd, start_cmd_context)) {
    /* Parse fail: Echo the command */
    print_command_options(start_cmd);
  } else {
    /* Parse succeed. Start a shell */ 
    if (true == start_cmd_context.option_enable(start_cmd, opt_interactive)) {
      shell.run_interactive_mode(shell_context);
      return 0;
    } 

    if (true == start_cmd_context.option_enable(start_cmd, opt_script_mode)) {
      shell.run_script_mode(start_cmd_context.option_value(start_cmd, opt_script_mode).c_str(),
                            shell_context);
      return 0;
    }
  }

  return 0;
}
