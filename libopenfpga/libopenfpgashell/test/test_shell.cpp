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
  std::string shell_title;

  shell_title += std::string("The MIT License\n");
  shell_title += std::string("\n");
  shell_title += std::string("Copyright (c) 2018 LNIS - The University of Utah\n");
  shell_title += std::string("\n");
  shell_title += std::string("Permission is hereby granted, free of charge, to any person obtaining a copy\n");
  shell_title += std::string("of this software and associated documentation files (the \"Software\"), to deal\n");
  shell_title += std::string("in the Software without restriction, including without limitation the rights\n");
  shell_title += std::string("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
  shell_title += std::string("copies of the Software, and to permit persons to whom the Software is\n");
  shell_title += std::string("furnished to do so, subject to the following conditions:\n");
  shell_title += std::string("\n");
  shell_title += std::string("The above copyright notice and this permission notice shall be included in\n");
  shell_title += std::string("all copies or substantial portions of the Software.\n");
  shell_title += std::string("\n");
  shell_title += std::string("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
  shell_title += std::string("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
  shell_title += std::string("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
  shell_title += std::string("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
  shell_title += std::string("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
  shell_title += std::string("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n");
  shell_title += std::string("THE SOFTWARE.\n");

  shell.add_title(shell_title.c_str());

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
    /* Reach here there is something wrong, show the help desk */
    print_command_options(start_cmd);
  }

  return 0;
}
