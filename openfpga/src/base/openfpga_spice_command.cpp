/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of generate SPICE netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - generate_fabric_spice : generate Verilog netlists about FPGA fabric 
 * - TODO: generate_spice_top_testbench : generate SPICE testbenches for top-level module
 * - TODO: generate_spice_grid_testbench : generate SPICE testbenches for grids
 * - TODO: generate_spice_cb_testbench : generate SPICE testbenches for connection blocks
 * - TODO: generate_spice_sb_testbench : generate SPICE testbenches for switch blocks
 * - TODO: generate_spice_lut_testbench : generate SPICE testbenches for Look-Up Tables
 * - TODO: generate_spice_hard_logic_testbench : generate SPICE testbenches for all the hard logics
 * - TODO: generate_spice_local_routing_testbench : generate SPICE testbenches for local routing 
 * - TODO: generate_spice_cb_routing_testbench : generate SPICE testbenches for routing circuit inside connection blocks
 * - TODO: generate_spice_sb_routing_testbench : generate SPICE testbenches for routing circuit inside switch blocks
 *******************************************************************/
#include "openfpga_spice.h"
#include "openfpga_spice_command.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: generate fabric Verilog 
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_fabric_spice_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                       const ShellCommandClassId& cmd_class_id,
                                                       const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_fabric_spice");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "Specify the output directory for SPICE netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* Add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false, "Use explicit port mapping in Verilog netlists");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");
  
  /* Add command 'write_fabric_spice' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate SPICE netlists modeling full FPGA fabric");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_fabric_spice);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

void add_openfpga_spice_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating the dependency graph */
  const ShellCommandId& build_fabric_cmd_id = shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_spice_cmd_class = shell.add_command_class("FPGA-SPICE");

  /******************************** 
   * Command 'write_fabric_spice' 
   */
  /* The 'write_fabric_spice' command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> fabric_spice_dependent_cmds;
  fabric_spice_dependent_cmds.push_back(build_fabric_cmd_id);
  add_openfpga_write_fabric_spice_command(shell,
                                          openfpga_spice_cmd_class,
                                          fabric_spice_dependent_cmds);

  /******************************** 
   * TODO: Command 'write_spice_top_testbench' 
   */
  /* The command 'write_spice_top_testbench' should NOT be executed before 'build_fabric' */
} 

} /* end namespace openfpga */
