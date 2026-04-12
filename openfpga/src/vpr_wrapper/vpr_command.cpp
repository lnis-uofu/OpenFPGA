/********************************************************************
 * Add vpr-related commands to the OpenFPGA shell interface
 *******************************************************************/
#include "vpr_command.h"

#include "vpr_main.h"

/* begin namespace openfpga */
namespace openfpga {

void add_vpr_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Add a new class of commands */
  ShellCommandClassId vpr_cmd_class = shell.add_command_class("VPR");

  /* Create a macro command of 'vpr' which will call the main engine of vpr
   */
  Command shell_cmd_vpr("vpr");
  ShellCommandId shell_cmd_vpr_id =
    shell.add_command(shell_cmd_vpr,
                      "Start VPR core engine to pack, place and route a BLIF "
                      "design on a FPGA architecture; Note that this command "
                      "will keep VPR results!");
  shell.set_command_class(shell_cmd_vpr_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_vpr_id, vpr::vpr_wrapper);

  /* Create a macro command of 'vpr_standalone' which will call the main engine
   * of vpr in a standalone way
   */
  Command shell_cmd_vpr_stdalone("vpr_standalone");
  ShellCommandId shell_cmd_vpr_stdalone_id = shell.add_command(
    shell_cmd_vpr_stdalone,
    "Start a standalone VPR core engine to pack, place and route a BLIF "
    "design on a FPGA architecture; Note that this command will NOT keep VPR "
    "results!");
  shell.set_command_class(shell_cmd_vpr_stdalone_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_vpr_stdalone_id,
                                     vpr::vpr_standalone_wrapper);

  // Add a command to read and validate VPR architecture file
  Command shell_cmd("read_vpr_arch");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_arch_file = shell_cmd.add_option(
    "file", true, "file path to the VPR architecture XML");
  shell_cmd.set_option_short_name(opt_arch_file, "f");
  shell_cmd.set_option_require_value(opt_arch_file, openfpga::OPT_STRING);

  /* Add command 'read_vpr_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "read and validate VPR architecture file");
  shell.set_command_class(shell_cmd_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_id,
                                     vpr::read_vpr_arch_template);

  /* Add command 'show_vpr_setup' to the Shell */
  Command shell_cmd_show_vpr_setup("show_vpr_setup");
  ShellCommandId shell_cmd_show_vpr_setup_id = shell.add_command(
    shell_cmd_show_vpr_setup,
    "Show the current VPR setup synchronized from OpenFPGA shell options");
  shell.set_command_class(shell_cmd_show_vpr_setup_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_show_vpr_setup_id,
                                     vpr::show_vpr_setup_template);
  }

} /* end namespace openfpga */
