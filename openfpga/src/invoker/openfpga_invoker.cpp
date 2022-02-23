/********************************************************************
 * Build the OpenFPGA shell interface 
 *******************************************************************/
/* Header file for vector*/
#include <iterator>

/* Header file from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Header file from libopenfpgashell library */
#include "command_parser.h"
#include "command_echo.h"
#include "shell.h"

/* Header file from openfpga */
#include "vpr_command.h"
#include "openfpga_setup_command.h"
#include "openfpga_verilog_command.h"
#include "openfpga_bitstream_command.h"
#include "openfpga_spice_command.h"
#include "openfpga_sdc_command.h"
#include "basic_command.h"

#include "openfpga_title.h"
#include "openfpga_context.h"

int mode_invoker (std::vector<std::string> &cmd_opts, const bool& tcl_mode) {

  /* Create the command to launch shell in different modes */
  openfpga::Command start_cmd("OpenFPGA");
  /* Add options to openfpga shell interface */
  /* '--interactive', -i': launch the interactive mode */
  openfpga::CommandOptionId opt_interactive = start_cmd.add_option("interactive", false, "Launch OpenFPGA in interactive mode");
  start_cmd.set_option_short_name(opt_interactive, "i");

  /* '--file', -f': launch the script mode */
  openfpga::CommandOptionId opt_script_mode = start_cmd.add_option("file", false, "Launch OpenFPGA in script mode");
  start_cmd.set_option_require_value(opt_script_mode, openfpga::OPT_STRING);
  start_cmd.set_option_short_name(opt_script_mode, "f");

  /* '--batch_execution': execute the script in batch mode. 
   * Will exit immediately when fatal errors occurred 
   */
  openfpga::CommandOptionId opt_batch_exec = start_cmd.add_option("batch_execution", false, "Launch OpenFPGA in batch  mode when running scripts");
  start_cmd.set_option_short_name(opt_batch_exec, "batch");

  /* '--version', -v': print version information */
  openfpga::CommandOptionId opt_version = start_cmd.add_option("version", false, "Show OpenFPGA version");
  start_cmd.set_option_short_name(opt_version, "v");

  /* '--help', -h': print help desk */
  openfpga::CommandOptionId opt_help = start_cmd.add_option("help", false, "Help desk"); 
  start_cmd.set_option_short_name(opt_help, "h");

  /* Create a shell object
   * Add two commands, which are
   * 1. exit
   * 2. help. This must the last to add
   */
  openfpga::Shell<OpenfpgaContext> shell("OpenFPGA");

  shell.add_title(create_openfpga_title().c_str());

  /* Add vpr commands */
  openfpga::add_vpr_commands(shell);

  /* Add openfpga setup commands */
  openfpga::add_openfpga_setup_commands(shell);

  /* Add openfpga verilog commands */
  openfpga::add_openfpga_verilog_commands(shell);

  /* Add openfpga bitstream commands */
  openfpga::add_openfpga_bitstream_commands(shell);

  /* Add openfpga SPICE commands */
  openfpga::add_openfpga_spice_commands(shell);

  /* Add openfpga sdc commands */
  openfpga::add_openfpga_sdc_commands(shell);

  /* Add basic commands: exit, help, etc. 
   * Note:
   * This MUST be the last command group to be added! 
   */
  openfpga::add_basic_commands(shell);

  /* Create the data base for the shell */
  OpenfpgaContext openfpga_context;

  /* call shell mode based on tcl_mode */
  if (true == tcl_mode) {
    
    //VTR_LOG("I am in TCL mode ... \n");

    std::string to_send;
    /* convert vector into string */
    for (auto const& s : cmd_opts) { to_send += s; }
    shell.run_tcl_mode(openfpga_context,to_send);
    return shell.exit_code();
   
  }
  else {
    /* put the command name */
  cmd_opts.push_back(start_cmd.name());
  /* rotate the vector to get cmd name at first position */
  std::rotate(cmd_opts.rbegin(), cmd_opts.rbegin() + 1, cmd_opts.rend());

  openfpga::CommandContext start_cmd_context(start_cmd);
  if (false == parse_command(cmd_opts, start_cmd, start_cmd_context)) {
    /* Parse fail: Echo the command */
    openfpga::print_command_options(start_cmd);
  } else {
    /* Parse succeed. Branch on options */ 
    /* Show version */
    if (true == start_cmd_context.option_enable(start_cmd, opt_version)) {
      print_openfpga_version_info();
      return 0;
    }
    /* Start a shell */ 
    if (true == start_cmd_context.option_enable(start_cmd, opt_interactive)) {

      shell.run_interactive_mode(openfpga_context);
      return shell.exit_code();
    } 

    if (true == start_cmd_context.option_enable(start_cmd, opt_script_mode)) {
      shell.run_script_mode(start_cmd_context.option_value(start_cmd, opt_script_mode).c_str(),
                            openfpga_context,
                            start_cmd_context.option_enable(start_cmd, opt_batch_exec));
      return shell.exit_code();
    }
    /* Reach here there is something wrong, show the help desk */
    openfpga::print_command_options(start_cmd);
  }

  /* Reach here, it means shell execution has critical errors.
   * Return a code with fatal errors  
   */
  return 1;
  }
}
