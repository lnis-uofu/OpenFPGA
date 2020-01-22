/********************************************************************
 * Test the command parser by pre-defining a simple command 
 *******************************************************************/
#include "command_parser.h"
#include "command_echo.h"

using namespace minishell;

int main(int argc, char** argv) {
  /* Create a new command */
  Command cmd("read_openfpga_arch");
  /* Add options to the command */
  /* Option to specify architecture file */
  CommandOptionId opt_arch_file = cmd.add_option("file", true, "Specify the OpenFPGA architecture file"); 
  cmd.set_option_require_value(opt_arch_file, OPT_STRING);
  cmd.set_option_short_name(opt_arch_file, "f");

  CommandOptionId opt_echo_arch = cmd.add_option("echo", false, "Echo the parsed result to file"); 
  cmd.set_option_require_value(opt_echo_arch, OPT_STRING);
  cmd.set_option_short_name(opt_echo_arch, "e");

  CommandOptionId opt_help = cmd.add_option("help", false, "Help desk"); 
  cmd.set_option_short_name(opt_help, "h");

  /* Parse the option, to avoid issues, we use the command name to replace the argv[0] */
  std::vector<std::string> cmd_opts; 
  cmd_opts.push_back(cmd.name());
  for (int iarg = 1; iarg < argc; ++iarg) {
    cmd_opts.push_back(std::string(argv[iarg]));
  }

  CommandContext cmd_context(cmd);
  if (false == parse_command(cmd_opts, cmd, cmd_context)) {
    /* Echo the command */
    print_command_options(cmd);
  } else {
    /* Let user to confirm selected options */ 
    print_command_context(cmd, cmd_context);
  }

  return 0;
}
