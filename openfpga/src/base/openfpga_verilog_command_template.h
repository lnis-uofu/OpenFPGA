#ifndef OPENFPGA_VERILOG_COMMAND_TEMPLATE_H
#define OPENFPGA_VERILOG_COMMAND_TEMPLATE_H
/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - generate_fabric_verilog : generate Verilog netlists about FPGA fabric
 * - generate_fabric_verilog_testbench : TODO: generate Verilog testbenches
 *******************************************************************/
#include "openfpga_verilog_template.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: generate fabric Verilog
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_fabric_verilog_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_fabric_verilog");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true, "Specify the output directory for Verilog netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* Add an option '--constant_undriven_inputs' */
  CommandOptionId const_undriven_inputs_opt = shell_cmd.add_option(
    "constant_undriven_inputs", false,
    "Can be [none|bus0|bus1|bit0|bit1]. Use constant vdd/gnd for undriven "
    "wires in Verilog netlists. Recommand to "
    "enable when there are boundary routing tracks in FPGA fabric");
  shell_cmd.set_option_require_value(const_undriven_inputs_opt,
                                     openfpga::OPT_STRING);

  /* Add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false,
                       "Use explicit port mapping in Verilog netlists");

  /* Add an option '--include_timing' */
  shell_cmd.add_option("include_timing", false,
                       "Enable timing annotation in Verilog netlists");

  /* Add an option '--print_user_defined_template' */
  shell_cmd.add_option(
    "print_user_defined_template", false,
    "Generate a template Verilog files for user-defined circuit models");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option(
    "default_net_type", false,
    "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt,
                                     openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print a time stamp in the output files");

  /* Add an option '--use_relative_path' */
  shell_cmd.add_option(
    "use_relative_path", false,
    "Force to use relative path in netlists when including other netlists");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command 'write_fabric_verilog' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "generate Verilog netlists modeling full FPGA fabric", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_fabric_verilog_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - add a command to shell environment: write full testbench
 * - add associated options
 * - add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_full_testbench_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_full_testbench");

  /* add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true, "specify the output directory for hdl netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--dut_module'*/
  CommandOptionId dut_module_opt = shell_cmd.add_option(
    "dut_module", false,
    "specify the module name of DUT to be used in the testbench. Can be either "
    "fpga_top or fpga_core. By default, it is fpga_top.");
  shell_cmd.set_option_require_value(dut_module_opt, openfpga::OPT_STRING);

  /* add an option '--bitstream'*/
  CommandOptionId bitstream_opt = shell_cmd.add_option(
    "bitstream", true, "specify the bitstream to be loaded in the testbench");
  shell_cmd.set_option_require_value(bitstream_opt, openfpga::OPT_STRING);

  /* add an option '--simulator'*/
  CommandOptionId sim_opt = shell_cmd.add_option(
    "simulator", false, "specify the simulator to be used for the testbench");
  shell_cmd.set_option_require_value(sim_opt, openfpga::OPT_STRING);

  /* add an option '--fabric_netlist_file_path'*/
  CommandOptionId fabric_netlist_opt =
    shell_cmd.add_option("fabric_netlist_file_path", false,
                         "specify the file path to the fabric hdl netlist");
  shell_cmd.set_option_require_value(fabric_netlist_opt, openfpga::OPT_STRING);

  /* add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt =
    shell_cmd.add_option("pin_constraints_file", false,
                         "specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* add an option '--bus_group_file in short '-bgf' */
  CommandOptionId bgf_opt = shell_cmd.add_option(
    "bus_group_file", false, "specify the file path to the group pins to bus");
  shell_cmd.set_option_short_name(bgf_opt, "bgf");
  shell_cmd.set_option_require_value(bgf_opt, openfpga::OPT_STRING);

  /* add an option '--reference_benchmark_file_path'*/
  CommandOptionId ref_bm_opt = shell_cmd.add_option(
    "reference_benchmark_file_path", false,
    "specify the file path to the reference verilog netlist. If specified, the "
    "testbench will include self-checking codes");
  shell_cmd.set_option_require_value(ref_bm_opt, openfpga::OPT_STRING);

  /* add an option '--fast_configuration' */
  shell_cmd.add_option(
    "fast_configuration", false,
    "reduce the period of configuration by skip certain data points");

  /* add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false,
                       "use explicit port mapping in verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option(
    "default_net_type", false,
    "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt,
                                     openfpga::OPT_STRING);

  /* Add an option '--no_self_checking' */
  shell_cmd.add_option(
    "no_self_checking", false,
    "Do not generate self-checking codes for Verilog testbenches.");

  /* add an option '--include_signal_init' */
  shell_cmd.add_option("include_signal_init", false,
                       "initialize all the signals in verilog testbenches");

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print a time stamp in the output files");

  /* Add an option '--use_relative_path' */
  shell_cmd.add_option(
    "use_relative_path", false,
    "Force to use relative path in netlists when including other netlists");

  /* add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "enable verbose output");

  /* add command to the shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "generate full testbenches for an fpga fabric", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_full_testbench_template<T>);

  /* add command dependency to the shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - add a command to shell environment: write preconfigured fabric wrapper
 * - add associated options
 * - add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_preconfigured_fabric_wrapper_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_preconfigured_fabric_wrapper");

  /* add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true, "specify the output directory for hdl netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--fabric_netlist_file_path'*/
  CommandOptionId fabric_netlist_opt =
    shell_cmd.add_option("fabric_netlist_file_path", false,
                         "specify the file path to the fabric hdl netlist");
  shell_cmd.set_option_require_value(fabric_netlist_opt, openfpga::OPT_STRING);

  /* add an option '--dut_module'*/
  CommandOptionId dut_module_opt = shell_cmd.add_option(
    "dut_module", false,
    "specify the module name of DUT to be used in the testbench. Can be either "
    "fpga_top or fpga_core. By default, it is fpga_top.");
  shell_cmd.set_option_require_value(dut_module_opt, openfpga::OPT_STRING);

  /* add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt =
    shell_cmd.add_option("pin_constraints_file", false,
                         "specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* add an option '--bus_group_file in short '-bgf' */
  CommandOptionId bgf_opt = shell_cmd.add_option(
    "bus_group_file", false, "specify the file path to the group pins to bus");
  shell_cmd.set_option_short_name(bgf_opt, "bgf");
  shell_cmd.set_option_require_value(bgf_opt, openfpga::OPT_STRING);

  /* add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false,
                       "use explicit port mapping in verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option(
    "default_net_type", false,
    "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt,
                                     openfpga::OPT_STRING);

  /* Add an option '--embed_bitstream' */
  CommandOptionId embed_bitstream_opt =
    shell_cmd.add_option("embed_bitstream", false,
                         "Embed bitstream to the Verilog wrapper netlist; This "
                         "may cause a large netlist file size");
  shell_cmd.set_option_require_value(embed_bitstream_opt, openfpga::OPT_STRING);

  /* add an option '--include_signal_init' */
  shell_cmd.add_option("include_signal_init", false,
                       "initialize all the signals in verilog testbenches");

  /* add an option '--dump_waveform' */
  shell_cmd.add_option("dump_waveform", false,
                       "add waveform output commands to the output file");

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print a time stamp in the output files");

  /* add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "enable verbose output");

  /* add command to the shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "generate a wrapper for a pre-configured fpga fabric", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(
    shell_cmd_id, write_preconfigured_fabric_wrapper_template<T>);

  /* add command dependency to the shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - add a command to shell environment: write a template of testbench
 * - add associated options
 * - add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_testbench_template_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_testbench_template");

  /* add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true, "specify the file path to output the content");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--top_module'*/
  CommandOptionId top_module_opt =
    shell_cmd.add_option("top_module", false,
                         "specify the top-level module name to be used in the "
                         "testbench. Please avoid reserved words, i.e., "
                         "fpga_top or fpga_core. By default, it is top_tb.");
  shell_cmd.set_option_require_value(top_module_opt, openfpga::OPT_STRING);

  /* add an option '--dut_module'*/
  CommandOptionId dut_module_opt = shell_cmd.add_option(
    "dut_module", false,
    "specify the module name of DUT to be used in the testbench. Can be either "
    "fpga_top or fpga_core. By default, it is fpga_top.");
  shell_cmd.set_option_require_value(dut_module_opt, openfpga::OPT_STRING);

  /* add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false,
                       "use explicit port mapping in verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option(
    "default_net_type", false,
    "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt,
                                     openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print a time stamp in the output files");

  /* add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "enable verbose output");

  /* add command to the shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "generate a template of testbench for a pre-configured fpga fabric",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_testbench_template_template<T>);

  /* add command dependency to the shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - add a command to shell environment: write testbench io connection
 * - add associated options
 * - add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_testbench_io_connection_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_testbench_io_connection");

  /* add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true, "specify the file path to output the content");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--dut_module'*/
  CommandOptionId dut_module_opt = shell_cmd.add_option(
    "dut_module", false,
    "specify the module name of DUT to be used in the testbench. Can be either "
    "fpga_top or fpga_core. By default, it is fpga_top.");
  shell_cmd.set_option_require_value(dut_module_opt, openfpga::OPT_STRING);

  /* add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt =
    shell_cmd.add_option("pin_constraints_file", false,
                         "specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* add an option '--bus_group_file in short '-bgf' */
  CommandOptionId bgf_opt = shell_cmd.add_option(
    "bus_group_file", false, "specify the file path to the group pins to bus");
  shell_cmd.set_option_short_name(bgf_opt, "bgf");
  shell_cmd.set_option_require_value(bgf_opt, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print a time stamp in the output files");

  /* add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "enable verbose output");

  /* add command to the shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd,
                      "generate a file to describe the connection to I/Os of a "
                      "pre-configured fpga fabric",
                      hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_testbench_io_connection_template<T>);

  /* add command dependency to the shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - add a command to shell environment: write mock fpga wrapper
 * - add associated options
 * - add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_mock_fpga_wrapper_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_mock_fpga_wrapper");

  /* add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true, "specify the output directory for hdl netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt =
    shell_cmd.add_option("pin_constraints_file", false,
                         "specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* add an option '--top_module'*/
  CommandOptionId top_module_opt =
    shell_cmd.add_option("top_module", false,
                         "specify the top-level module name to be used in the "
                         "wrapper, which matters the I/O names. Can be either "
                         "fpga_top or fpga_core. By default, it is fpga_top.");
  shell_cmd.set_option_require_value(top_module_opt, openfpga::OPT_STRING);

  /* add an option '--bus_group_file in short '-bgf' */
  CommandOptionId bgf_opt = shell_cmd.add_option(
    "bus_group_file", false, "specify the file path to the group pins to bus");
  shell_cmd.set_option_short_name(bgf_opt, "bgf");
  shell_cmd.set_option_require_value(bgf_opt, openfpga::OPT_STRING);

  /* Add an option '--use_relative_path' */
  shell_cmd.add_option(
    "use_relative_path", false,
    "Force to use relative path in netlists when including other netlists");

  /* add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false,
                       "use explicit port mapping in verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option(
    "default_net_type", false,
    "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt,
                                     openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print a time stamp in the output files");

  /* add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "enable verbose output");

  /* add command to the shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "generate a wrapper of a mock fpga fabric mapped with applications",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_mock_fpga_wrapper_template<T>);

  /* add command dependency to the shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write preconfigured testbench
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_preconfigured_testbench_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_preconfigured_testbench");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true, "Specify the output directory for HDL netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* add an option '--fabric_netlist_file_path'*/
  CommandOptionId fabric_netlist_opt =
    shell_cmd.add_option("fabric_netlist_file_path", false,
                         "specify the file path to the fabric hdl netlist");
  shell_cmd.set_option_require_value(fabric_netlist_opt, openfpga::OPT_STRING);

  /* Add an option '--pin_constraints_file in short '-pcf' */
  CommandOptionId pcf_opt =
    shell_cmd.add_option("pin_constraints_file", false,
                         "Specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(pcf_opt, "pcf");
  shell_cmd.set_option_require_value(pcf_opt, openfpga::OPT_STRING);

  /* add an option '--bus_group_file in short '-bgf' */
  CommandOptionId bgf_opt = shell_cmd.add_option(
    "bus_group_file", false, "specify the file path to the group pins to bus");
  shell_cmd.set_option_short_name(bgf_opt, "bgf");
  shell_cmd.set_option_require_value(bgf_opt, openfpga::OPT_STRING);

  /* Add an option '--reference_benchmark_file_path'*/
  CommandOptionId ref_bm_opt = shell_cmd.add_option(
    "reference_benchmark_file_path", false,
    "Specify the file path to the reference Verilog netlist. If specified, the "
    "testbench will include self-checking codes");
  shell_cmd.set_option_require_value(ref_bm_opt, openfpga::OPT_STRING);

  /* Add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false,
                       "Use explicit port mapping in Verilog netlists");

  /* Add an option '--default_net_type' */
  CommandOptionId default_net_type_opt = shell_cmd.add_option(
    "default_net_type", false,
    "Set the default net type for Verilog netlists. Default value is 'none'");
  shell_cmd.set_option_require_value(default_net_type_opt,
                                     openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print a time stamp in the output files");

  /* Add an option '--use_relative_path' */
  shell_cmd.add_option(
    "use_relative_path", false,
    "Force to use relative path in netlists when including other netlists");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "generate testbenches for a preconfigured FPGA fabric", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_preconfigured_testbench_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write simulation task info
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_simulation_task_info_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_simulation_task_info");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option(
    "file", true,
    "Specify the file path to output simulation-related information");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* Add an option '--hdl_dir'*/
  CommandOptionId hdl_dir_opt = shell_cmd.add_option(
    "hdl_dir", true,
    "Specify the directory path where HDL netlists are created");
  shell_cmd.set_option_require_value(hdl_dir_opt, openfpga::OPT_STRING);

  /* Add an option '--reference_benchmark_file_path'*/
  CommandOptionId ref_bm_opt = shell_cmd.add_option(
    "reference_benchmark_file_path", false,
    "Specify the file path to the reference Verilog netlist. If specified, the "
    "testbench will include self-checking codes");
  shell_cmd.set_option_require_value(ref_bm_opt, openfpga::OPT_STRING);

  /* Add an option '--testbench_type'*/
  CommandOptionId tb_type_opt = shell_cmd.add_option(
    "testbench_type", false,
    "Specify the type of testbenches to be considered. Different testbenches "
    "have different simulation parameters.");
  shell_cmd.set_option_require_value(tb_type_opt, openfpga::OPT_STRING);

  /* Add an option '--time_unit' */
  CommandOptionId time_unit_opt =
    shell_cmd.add_option("time_unit", false,
                         "Specify the time unit to be used in HDL simulation. "
                         "Acceptable is [a|f|p|n|u|m|k|M]s");
  shell_cmd.set_option_require_value(time_unit_opt, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "generate an interchangable simulation task configuration file",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_simulation_task_info_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

template <class T>
void add_verilog_command_templates(openfpga::Shell<T>& shell,
                                   const bool& hidden = false) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating
   * the dependency graph */
  const ShellCommandId& build_fabric_cmd_id =
    shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_verilog_cmd_class =
    shell.add_command_class("FPGA-Verilog");

  /********************************
   * Command 'write_fabric_verilog'
   */
  /* The 'write_fabric_verilog' command should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> fabric_verilog_dependent_cmds;
  fabric_verilog_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_fabric_verilog_command_template<T>(
    shell, openfpga_verilog_cmd_class, fabric_verilog_dependent_cmds, hidden);

  /********************************
   * Command 'write_full_testbench'
   */
  /* The command 'write_full_testbench' should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> full_testbench_dependent_cmds;
  full_testbench_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_full_testbench_command_template<T>(
    shell, openfpga_verilog_cmd_class, full_testbench_dependent_cmds, hidden);

  /********************************
   * Command 'write_preconfigured_fabric_wrapper'
   */
  /* The command 'write_preconfigured_fabric_wrapper' should NOT be executed
   * before 'build_fabric' */
  std::vector<ShellCommandId> preconfig_wrapper_dependent_cmds;
  preconfig_wrapper_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_preconfigured_fabric_wrapper_command_template<T>(
    shell, openfpga_verilog_cmd_class, preconfig_wrapper_dependent_cmds,
    hidden);

  /********************************
   * Command 'write_testbench_template'
   */
  /* The command 'write_testbench_template' should NOT be executed
   * before 'build_fabric' */
  std::vector<ShellCommandId> testbench_template_dependent_cmds;
  testbench_template_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_testbench_template_command_template<T>(
    shell, openfpga_verilog_cmd_class, testbench_template_dependent_cmds,
    hidden);

  /********************************
   * Command 'write_testbench_io_connection'
   */
  /* The command 'write_testbench_io_connection' should NOT be executed
   * before 'build_fabric' */
  std::vector<ShellCommandId> testbench_io_conkt_dependent_cmds;
  testbench_io_conkt_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_testbench_io_connection_command_template<T>(
    shell, openfpga_verilog_cmd_class, testbench_io_conkt_dependent_cmds,
    hidden);

  /********************************
   * Command 'write_mock_fpga_wrapper'
   */
  /* The command 'write_mock_fpga_wrapper' should NOT be executed
   * before 'build_fabric' */
  std::vector<ShellCommandId> write_mock_fpga_wrapper_dependent_cmds;
  write_mock_fpga_wrapper_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_mock_fpga_wrapper_command_template<T>(
    shell, openfpga_verilog_cmd_class, write_mock_fpga_wrapper_dependent_cmds,
    hidden);

  /********************************
   * Command 'write_preconfigured_testbench'
   */
  /* The command 'write_preconfigured_testbench' should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> preconfig_testbench_dependent_cmds;
  preconfig_testbench_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_preconfigured_testbench_command_template<T>(
    shell, openfpga_verilog_cmd_class, preconfig_testbench_dependent_cmds,
    hidden);

  /********************************
   * Command 'write_simulation_task_info'
   */
  /* The command 'write_simulation_task_info' should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> sim_task_info_dependent_cmds;
  sim_task_info_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_simulation_task_info_command_template<T>(
    shell, openfpga_verilog_cmd_class, sim_task_info_dependent_cmds, hidden);
}

} /* end namespace openfpga */

#endif
