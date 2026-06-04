/********************************************************************
 * Add yosys-related commands to the OpenFPGA shell interface
 *******************************************************************/
#include "yosys_command.h"

#include "yosys_main.h"

/* begin namespace openfpga */
namespace openfpga {

void add_yosys_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Add a new class of commands */
  ShellCommandClassId yosys_cmd_class = shell.add_command_class("Yosys");

  /* Create a macro command of 'yosys' which calls the yosys executable */
  Command shell_cmd_yosys("yosys");
  ShellCommandId shell_cmd_yosys_id = shell.add_command(
    shell_cmd_yosys,
    "Start Yosys synthesis engine with passthrough command-line arguments.");
  shell.set_command_class(shell_cmd_yosys_id, yosys_cmd_class);
  shell.set_command_execute_function(shell_cmd_yosys_id, yosys_wrapper);
}

} /* end namespace openfpga */
