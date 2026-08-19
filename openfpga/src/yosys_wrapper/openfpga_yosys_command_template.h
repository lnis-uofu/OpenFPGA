#pragma once
#include "openfpga_windows_compatibility.h"

/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - repack : create physical pbs and redo packing
 *******************************************************************/
#include "openfpga_yosys_template.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: repack
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_yosys_synth_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("yosys");

  /* Add an option '--design_constraints' */
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the yosys script to run");
  shell_cmd.set_option_require_value(opt_file,
                                     openfpga::OPT_STRING);

  /* Add command 'repack' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Run yosys synthesis flow with a given script", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, yosys_synth_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * Top-level function to add all the commands related to FPGA-Bitstream
 *******************************************************************/
template <class T>
void add_yosys_command_templates(openfpga::Shell<T>& shell,
                                 const bool& hidden = false) {
  /* Add a new class of commands */
  ShellCommandClassId openfpga_ys_cmd_class =
    shell.add_command_class("Yosys");

  /********************************
   * Command 'repack'
   */
  /* The 'repack' command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_ys_synth;
  add_yosys_synth_command_template(
    shell, openfpga_ys_cmd_class, cmd_dependency_ys_synth, hidden);
}

} /* end namespace openfpga */

#endif
