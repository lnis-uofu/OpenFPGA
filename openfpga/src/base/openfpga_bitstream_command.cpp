/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - repack : create physical pbs and redo packing 
 *******************************************************************/
#include "openfpga_repack.h"
#include "openfpga_bitstream_command.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_bitstream_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating the dependency graph */
  const ShellCommandId& shell_cmd_build_fabric_id = shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_bitstream_cmd_class = shell.add_command_class("FPGA-Bitstream");

  /******************************** 
   * Command 'repack' 
   */
  Command shell_cmd_repack("repack");
  /* Add an option '--verbose' */
  shell_cmd_repack.add_option("verbose", false, "Enable verbose output");
  
  /* Add command 'repack' to the Shell */
  ShellCommandId shell_cmd_repack_id = shell.add_command(shell_cmd_repack, "Pack physical programmable logic blocks");
  shell.set_command_class(shell_cmd_repack_id, openfpga_bitstream_cmd_class);
  shell.set_command_execute_function(shell_cmd_repack_id, repack);

  /* The 'repack' command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_repack;
  cmd_dependency_repack.push_back(shell_cmd_build_fabric_id);
  shell.set_command_dependency(shell_cmd_repack_id, cmd_dependency_repack);
} 

} /* end namespace openfpga */
