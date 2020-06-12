/********************************************************************
 * Add vpr-related commands to the OpenFPGA shell interface 
 *******************************************************************/
#include "vpr_main.h"

#include "vpr_command.h"

/* begin namespace openfpga */
namespace openfpga {

void add_vpr_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Add a new class of commands */
  ShellCommandClassId vpr_cmd_class = shell.add_command_class("VPR");

  /* Create a macro command of 'vpr' which will call the main engine of vpr 
   */
  Command shell_cmd_vpr("vpr");
  ShellCommandId shell_cmd_vpr_id = shell.add_command(shell_cmd_vpr, "Start VPR core engine to pack, place and route a BLIF design on a FPGA architecture");
  shell.set_command_class(shell_cmd_vpr_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_vpr_id, vpr::vpr);
}

} /* end namespace openfpga */
