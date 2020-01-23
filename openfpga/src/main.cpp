/********************************************************************
 * Build the OpenFPGA shell interface 
 *******************************************************************/
/* Header file from vtrutil library */
#include "vtr_log.h"

/* Header file from libopenfpgashell library */
#include "command_parser.h"
#include "command_echo.h"
#include "shell.h"

/* Header file from openfpga */
#include "vpr_main.h"

#include "openfpga_title.h"
#include "openfpga_context.h"

using namespace openfpga;

/********************************************************************
 * Main function to start OpenFPGA shell interface
 *******************************************************************/
int main(int argc, char** argv) {

  /* Create the command to launch shell in different modes */
  Command start_cmd("OpenFPGA");
  /* Add two options:
   * '--interactive', -i': launch the interactive mode 
   * '--file', -f': launch the script mode 
   */
  CommandOptionId opt_interactive = start_cmd.add_option("interactive", false, "Launch OpenFPGA in interactive mode");
  start_cmd.set_option_short_name(opt_interactive, "i");

  CommandOptionId opt_script_mode = start_cmd.add_option("file", false, "Launch OpenFPGA in script mode");
  start_cmd.set_option_require_value(opt_script_mode, OPT_STRING);
  start_cmd.set_option_short_name(opt_script_mode, "f");

  CommandOptionId opt_help = start_cmd.add_option("help", false, "Help desk"); 
  start_cmd.set_option_short_name(opt_help, "h");

  /* Create a shell object
   * Add two commands, which are
   * 1. exit
   * 2. help. This must the last to add
   */
  Shell<OpenfpgaContext> shell("OpenFPGA");

  shell.add_title(create_openfpga_title());

  /* Add a new class of commands */
  ShellCommandClassId arith_cmd_class = shell.add_command_class("VPR");

  /* Create a macro command of 'vpr' which will call the main engine of vpr 
   */
  Command shell_cmd_vpr("vpr");
  ShellCommandId shell_cmd_vpr_id = shell.add_command(shell_cmd_vpr, "Start VPR core engine to pack, place and route a BLIF design on a FPGA architecture");
  shell.set_command_class(shell_cmd_vpr_id, arith_cmd_class);
  shell.set_command_execute_function(shell_cmd_vpr_id, vpr::vpr);

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
  OpenfpgaContext openfpga_context;

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
      shell.run_interactive_mode(openfpga_context);
      return 0;
    } 

    if (true == start_cmd_context.option_enable(start_cmd, opt_script_mode)) {
      shell.run_script_mode(start_cmd_context.option_value(start_cmd, opt_script_mode).c_str(),
                            openfpga_context);
      return 0;
    }
    /* Reach here there is something wrong, show the help desk */
    print_command_options(start_cmd);
  }

  return 0;
}
