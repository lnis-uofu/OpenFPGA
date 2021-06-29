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

/********************************************************************
 * - Add a command to Shell environment: generate fabric Verilog 
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_fabric_verilog_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                         const ShellCommandClassId& cmd_class_id,
                                                         const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_fabric_verilog");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "Specify the output directory for Verilog netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* Add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false, "Use explicit port mapping in Verilog netlists");

  /* Add an option '--include_timing' */
  shell_cmd.add_option("include_timing", false, "Enable timing annotation in Verilog netlists");

  /* Add an option '--print_user_defined_template' */
  shell_cmd.add_option("print_user_defined_template", false, "Generate a template Verilog files for user-defined circuit models");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option("default_net_type", false, "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");
  
  /* Add command 'write_fabric_verilog' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate Verilog netlists modeling full FPGA fabric");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_fabric_verilog);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - add a command to shell environment: write full testbench
 * - add associated options 
 * - add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_full_testbench_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                         const ShellCommandClassId& cmd_class_id,
                                                         const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_full_testbench");

  /* add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "specify the output directory for hdl netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--bitstream'*/
  CommandOptionId bitstream_opt = shell_cmd.add_option("bitstream", true, "specify the bitstream to be loaded in the testbench");
  shell_cmd.set_option_require_value(bitstream_opt, openfpga::OPT_STRING);

  /* add an option '--fabric_netlist_file_path'*/
  CommandOptionId fabric_netlist_opt = shell_cmd.add_option("fabric_netlist_file_path", false, "specify the file path to the fabric hdl netlist");
  shell_cmd.set_option_require_value(fabric_netlist_opt, openfpga::OPT_STRING);

  /* add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt = shell_cmd.add_option("pin_constraints_file", false, "specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* add an option '--reference_benchmark_file_path'*/
  CommandOptionId ref_bm_opt = shell_cmd.add_option("reference_benchmark_file_path", true, "specify the file path to the reference verilog netlist");
  shell_cmd.set_option_require_value(ref_bm_opt, openfpga::OPT_STRING);

  /* add an option '--fast_configuration' */
  shell_cmd.add_option("fast_configuration", false, "reduce the period of configuration by skip certain data points");

  /* add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false, "use explicit port mapping in verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option("default_net_type", false, "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt, openfpga::OPT_STRING);

  /* Add an option '--no_self_checking' */
  shell_cmd.add_option("no_self_checking", false, "Do not generate self-checking codes for Verilog testbenches.");

  /* add an option '--include_signal_init' */
  shell_cmd.add_option("include_signal_init", false, "initialize all the signals in verilog testbenches");

  /* add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "enable verbose output");
  
  /* add command to the shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate full testbenches for an fpga fabric");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_full_testbench);

  /* add command dependency to the shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - add a command to shell environment: write preconfigured fabric wrapper
 * - add associated options 
 * - add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_preconfigured_fabric_wrapper_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                                       const ShellCommandClassId& cmd_class_id,
                                                                       const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_preconfigured_fabric_wrapper");

  /* add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "specify the output directory for hdl netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--fabric_netlist_file_path'*/
  CommandOptionId fabric_netlist_opt = shell_cmd.add_option("fabric_netlist_file_path", false, "specify the file path to the fabric hdl netlist");
  shell_cmd.set_option_require_value(fabric_netlist_opt, openfpga::OPT_STRING);

  /* add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt = shell_cmd.add_option("pin_constraints_file", false, "specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false, "use explicit port mapping in verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option("default_net_type", false, "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt, openfpga::OPT_STRING);

  /* Add an option '--embed_bitstream' */
  CommandOptionId embed_bitstream_opt = shell_cmd.add_option("embed_bitstream", false, "Embed bitstream to the Verilog wrapper netlist; This may cause a large netlist file size");
  shell_cmd.set_option_require_value(embed_bitstream_opt, openfpga::OPT_STRING);

  /* add an option '--include_signal_init' */
  shell_cmd.add_option("include_signal_init", false, "initialize all the signals in verilog testbenches");

  /* add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "enable verbose output");
  
  /* add command to the shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate a wrapper for a pre-configured fpga fabric");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_preconfigured_fabric_wrapper);

  /* add command dependency to the shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write preconfigured testbench
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_preconfigured_testbench_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                                  const ShellCommandClassId& cmd_class_id,
                                                                  const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_preconfigured_testbench");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "Specify the output directory for HDL netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--fabric_netlist_file_path'*/
  CommandOptionId fabric_netlist_opt = shell_cmd.add_option("fabric_netlist_file_path", false, "specify the file path to the fabric hdl netlist");
  shell_cmd.set_option_require_value(fabric_netlist_opt, openfpga::OPT_STRING);

  /* Add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt = shell_cmd.add_option("pin_constraints_file", false, "Specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* Add an option '--reference_benchmark_file_path'*/
  CommandOptionId ref_bm_opt = shell_cmd.add_option("reference_benchmark_file_path", false, "Specify the file path to the reference Verilog netlist. If specified, the testbench will include self-checking codes");
  shell_cmd.set_option_require_value(ref_bm_opt, openfpga::OPT_STRING);

  /* Add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false, "Use explicit port mapping in Verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option("default_net_type", false, "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");
  
  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate testbenches for a preconfigured FPGA fabric");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_preconfigured_testbench);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write simulation task info
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_simulation_task_info_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                               const ShellCommandClassId& cmd_class_id,
                                                               const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_simulation_task_info");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "Specify the file path to output simulation-related information");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* Add an option '--hdl_dir'*/
  CommandOptionId hdl_dir_opt = shell_cmd.add_option("hdl_dir", true, "Specify the directory path where HDL netlists are created");
  shell_cmd.set_option_require_value(hdl_dir_opt, openfpga::OPT_STRING);

  /* Add an option '--reference_benchmark_file_path'*/
  CommandOptionId ref_bm_opt = shell_cmd.add_option("reference_benchmark_file_path", false, "Specify the file path to the reference Verilog netlist. If specified, the testbench will include self-checking codes");
  shell_cmd.set_option_require_value(ref_bm_opt, openfpga::OPT_STRING);

  /* Add an option '--testbench_type'*/
  CommandOptionId tb_type_opt = shell_cmd.add_option("testbench_type", false, "Specify the type of testbenches to be considered. Different testbenches have different simulation parameters.");
  shell_cmd.set_option_require_value(tb_type_opt, openfpga::OPT_STRING);

  /* Add an option '--time_unit' */
  CommandOptionId time_unit_opt = shell_cmd.add_option("time_unit", false, "Specify the time unit to be used in HDL simulation. Acceptable is [a|f|p|n|u|m|k|M]s");
  shell_cmd.set_option_require_value(time_unit_opt, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");
  
  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate an interchangable simulation task configuration file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_simulation_task_info);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

void add_openfpga_verilog_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating the dependency graph */
  const ShellCommandId& build_fabric_cmd_id = shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_verilog_cmd_class = shell.add_command_class("FPGA-Verilog");

  /******************************** 
   * Command 'write_fabric_verilog' 
   */
  /* The 'write_fabric_verilog' command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> fabric_verilog_dependent_cmds;
  fabric_verilog_dependent_cmds.push_back(build_fabric_cmd_id);
  add_openfpga_write_fabric_verilog_command(shell,
                                            openfpga_verilog_cmd_class,
                                            fabric_verilog_dependent_cmds);

  /******************************** 
   * Command 'write_full_testbench' 
   */
  /* The command 'write_full_testbench' should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> full_testbench_dependent_cmds;
  full_testbench_dependent_cmds.push_back(build_fabric_cmd_id);
  add_openfpga_write_full_testbench_command(shell,
                                            openfpga_verilog_cmd_class,
                                            full_testbench_dependent_cmds);

  /******************************** 
   * Command 'write_preconfigured_fabric_wrapper' 
   */
  /* The command 'write_preconfigured_fabric_wrapper' should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> preconfig_wrapper_dependent_cmds;
  preconfig_wrapper_dependent_cmds.push_back(build_fabric_cmd_id);
  add_openfpga_write_preconfigured_fabric_wrapper_command(shell,
                                                          openfpga_verilog_cmd_class,
                                                          preconfig_wrapper_dependent_cmds);

  /******************************** 
   * Command 'write_preconfigured_testbench' 
   */
  /* The command 'write_preconfigured_testbench' should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> preconfig_testbench_dependent_cmds;
  preconfig_testbench_dependent_cmds.push_back(build_fabric_cmd_id);
  add_openfpga_write_preconfigured_testbench_command(shell,
                                                     openfpga_verilog_cmd_class,
                                                     preconfig_testbench_dependent_cmds);

  /******************************** 
   * Command 'write_simulation_task_info' 
   */
  /* The command 'write_simulation_task_info' should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> sim_task_info_dependent_cmds;
  sim_task_info_dependent_cmds.push_back(build_fabric_cmd_id);
  add_openfpga_write_simulation_task_info_command(shell,
                                                  openfpga_verilog_cmd_class,
                                                  sim_task_info_dependent_cmds);
} 

} /* end namespace openfpga */
