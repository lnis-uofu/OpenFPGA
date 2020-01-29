/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of setting up OpenFPGA core engine, including:
 * - read_openfpga_arch : read OpenFPGA architecture file
 *******************************************************************/
#include "openfpga_read_arch.h"
#include "openfpga_link_arch.h"
#include "check_netlist_naming_conflict.h"
#include "openfpga_setup_command.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_setup_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'vpr' command which is to be used in creating the dependency graph */
  const ShellCommandId& shell_cmd_vpr_id = shell.command(std::string("vpr"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_setup_cmd_class = shell.add_command_class("OpenFPGA setup");

  /******************************** 
   * Command 'read_openfpga_arch' 
   */
  Command shell_cmd_read_arch("read_openfpga_arch");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId read_arch_opt_file = shell_cmd_read_arch.add_option("file", true, "file path to the architecture XML");
  shell_cmd_read_arch.set_option_short_name(read_arch_opt_file, "f");
  shell_cmd_read_arch.set_option_require_value(read_arch_opt_file, openfpga::OPT_STRING);
  
  /* Add command 'read_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_read_arch_id = shell.add_command(shell_cmd_read_arch, "read OpenFPGA architecture file");
  shell.set_command_class(shell_cmd_read_arch_id, openfpga_setup_cmd_class);
  shell.set_command_execute_function(shell_cmd_read_arch_id, read_arch);

  /******************************** 
   * Command 'write_openfpga_arch' 
   */
  Command shell_cmd_write_arch("write_openfpga_arch");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId write_arch_opt_file = shell_cmd_write_arch.add_option("file", true, "file path to the architecture XML");
  shell_cmd_write_arch.set_option_short_name(write_arch_opt_file, "f");
  shell_cmd_write_arch.set_option_require_value(write_arch_opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_write_arch_id = shell.add_command(shell_cmd_write_arch, "write OpenFPGA architecture file");
  shell.set_command_class(shell_cmd_write_arch_id, openfpga_setup_cmd_class);
  shell.set_command_const_execute_function(shell_cmd_write_arch_id, write_arch);
  /* The 'write_openfpga_arch' command should NOT be executed before 'read_openfpga_arch' */
  shell.set_command_dependency(shell_cmd_write_arch_id, std::vector<ShellCommandId>(1, shell_cmd_read_arch_id));

  /******************************** 
   * Command 'link_openfpga_arch' 
   */
  Command shell_cmd_link_openfpga_arch("link_openfpga_arch");
  
  /* Add command 'link_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_link_openfpga_arch_id = shell.add_command(shell_cmd_link_openfpga_arch, "Bind OpenFPGA architecture to VPR");
  shell.set_command_class(shell_cmd_link_openfpga_arch_id, openfpga_setup_cmd_class);
  shell.set_command_execute_function(shell_cmd_link_openfpga_arch_id, link_arch);
  /* The 'link_openfpga_arch' command should NOT be executed before 'read_openfpga_arch' and 'vpr' */
  std::vector<ShellCommandId> cmd_dependency_link_openfpga_arch;
  cmd_dependency_link_openfpga_arch.push_back(shell_cmd_read_arch_id);
  cmd_dependency_link_openfpga_arch.push_back(shell_cmd_vpr_id);
  shell.set_command_dependency(shell_cmd_link_openfpga_arch_id, cmd_dependency_link_openfpga_arch);

  /******************************************* 
   * Command 'check_netlist_naming_conflict'
   */ 
  Command shell_cmd_check_netlist_naming_conflict("check_netlist_naming_conflict");
  /* Add an option '--correction' */
  shell_cmd_check_netlist_naming_conflict.add_option("correction", false, "Apply correction to any conflicts found");
  /* Add an option '--report' */
  CommandOptionId check_netlist_opt_rpt = shell_cmd_check_netlist_naming_conflict.add_option("report", false, "Output a report file about what any correction applied");
  shell_cmd_check_netlist_naming_conflict.set_option_require_value(check_netlist_opt_rpt, openfpga::OPT_STRING);

  /* Add command 'check_netlist_naming_conflict' to the Shell */
  ShellCommandId shell_cmd_check_netlist_naming_conflict_id = shell.add_command(shell_cmd_check_netlist_naming_conflict, "Check any block/net naming in users' BLIF netlist violates the syntax of fabric generator");
  shell.set_command_class(shell_cmd_check_netlist_naming_conflict_id, openfpga_setup_cmd_class);
  shell.set_command_execute_function(shell_cmd_check_netlist_naming_conflict_id, check_netlist_naming_conflict);
  /* The 'link_openfpga_arch' command should NOT be executed before 'read_openfpga_arch' and 'vpr' */
  std::vector<ShellCommandId> cmd_dependency_check_netlist_naming_conflict(1, shell_cmd_vpr_id);
  shell.set_command_dependency(shell_cmd_link_openfpga_arch_id, cmd_dependency_check_netlist_naming_conflict);
} 

} /* end namespace openfpga */
