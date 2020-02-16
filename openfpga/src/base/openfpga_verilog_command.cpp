/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - generate_fabric_verilog : generate Verilog netlists about FPGA fabric 
 * - generate_fabric_verilog_testbench : TODO: generate Verilog testbenches 
 *******************************************************************/
#include "openfpga_verilog.h"
#include "openfpga_verilog_command.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_verilog_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating the dependency graph */
  const ShellCommandId& shell_cmd_build_fabric_id = shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_verilog_cmd_class = shell.add_command_class("FPGA-Verilog");

  /******************************** 
   * Command 'wirte_fabric_verilog' 
   */
  Command shell_cmd_write_fabric_verilog("write_fabric_verilog");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId fabric_verilog_output_opt = shell_cmd_write_fabric_verilog.add_option("file", true, "Specify the output directory for Verilog netlists");
  shell_cmd_write_fabric_verilog.set_option_short_name(fabric_verilog_output_opt, "f");
  shell_cmd_write_fabric_verilog.set_option_require_value(fabric_verilog_output_opt, openfpga::OPT_STRING);
  /* Add an option '--explicit_port_mapping' */
  shell_cmd_write_fabric_verilog.add_option("explicit_port_mapping", false, "Use explicit port mapping in Verilog netlists");
  /* Add an option '--include_timing' */
  shell_cmd_write_fabric_verilog.add_option("include_timing", false, "Enable timing annotation in Verilog netlists");
  /* Add an option '--include_signal_init' */
  shell_cmd_write_fabric_verilog.add_option("include_signal_init", false, "Initialize all the signals in Verilog netlists");
  /* Add an option '--support_icarus_simulator' */
  shell_cmd_write_fabric_verilog.add_option("support_icarus_simulator", false, "Fine-tune Verilog netlists to support icarus simulator");
  /* Add an option '--verbose' */
  shell_cmd_write_fabric_verilog.add_option("verbose", false, "Enable verbose output");
  
  /* Add command 'write_fabric_verilog' to the Shell */
  ShellCommandId shell_cmd_write_fabric_verilog_id = shell.add_command(shell_cmd_write_fabric_verilog, "generate Verilog netlists modeling full FPGA fabric");
  shell.set_command_class(shell_cmd_write_fabric_verilog_id, openfpga_verilog_cmd_class);
  shell.set_command_execute_function(shell_cmd_write_fabric_verilog_id, write_fabric_verilog);

  /* The 'build_fabric' command should NOT be executed before 'link_openfpga_arch' */
  std::vector<ShellCommandId> cmd_dependency_write_fabric_verilog;
  cmd_dependency_write_fabric_verilog.push_back(shell_cmd_build_fabric_id);
  shell.set_command_dependency(shell_cmd_write_fabric_verilog_id, cmd_dependency_write_fabric_verilog);
} 

} /* end namespace openfpga */
