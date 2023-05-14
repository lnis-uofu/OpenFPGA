#ifndef OPENFPGA_BITSTREAM_COMMAND_TEMPLATE_H
#define OPENFPGA_BITSTREAM_COMMAND_TEMPLATE_H
/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - repack : create physical pbs and redo packing
 *******************************************************************/
#include "openfpga_bitstream_template.h"
#include "openfpga_repack_template.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: repack
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_repack_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("repack");

  /* Add an option '--design_constraints' */
  CommandOptionId opt_design_constraints = shell_cmd.add_option(
    "design_constraints", false, "file path to the design constraints");
  shell_cmd.set_option_require_value(opt_design_constraints,
                                     openfpga::OPT_STRING);

  /* Add an option '--ignore_global_nets_on_pins' */
  CommandOptionId opt_ignore_global_nets =
    shell_cmd.add_option("ignore_global_nets_on_pins", false,
                         "Specify the pins where global nets will be ignored. "
                         "Routing traces are merged to other pins");
  shell_cmd.set_option_require_value(opt_ignore_global_nets,
                                     openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command 'repack' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Pack physical programmable logic blocks", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, repack_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: build_architecture_bitstream
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_build_arch_bitstream_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("build_architecture_bitstream");

  /* Add an option '--write_file' */
  CommandOptionId opt_write_file = shell_cmd.add_option(
    "write_file", false, "file path to output the bitstream database");
  shell_cmd.set_option_require_value(opt_write_file, openfpga::OPT_STRING);

  /* Add an option '--read_file' */
  CommandOptionId opt_read_file = shell_cmd.add_option(
    "read_file", false, "file path to read the bitstream database");
  shell_cmd.set_option_require_value(opt_read_file, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command 'build_architecture_bitstream' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Build fabric-independent bitstream database", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, fpga_bitstream_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: report_bitstream_distribution
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_report_bitstream_distribution_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("report_bitstream_distribution");

  /* Add an option '--file' */
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to output the bitstream distribution");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--depth' */
  CommandOptionId opt_depth = shell_cmd.add_option(
    "depth", false,
    "Specify the max. depth of blocks which will appear in report");
  shell_cmd.set_option_require_value(opt_depth, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command 'report_bitstream_distribution' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd, "Report bitstream distribution", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     report_bitstream_distribution_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: build_fabric_bitstream
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_build_fabric_bitstream_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("build_fabric_bitstream");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add an option '--write_file' */
  CommandOptionId opt_write_file = shell_cmd.add_option(
    "write_file", false,
    "file path to output the fabric dependent bitstream mapping database");
  shell_cmd.set_option_require_value(opt_write_file, openfpga::OPT_STRING);

  /* Add an option '--read_file' */
  CommandOptionId opt_read_file = shell_cmd.add_option(
    "read_file", false,
    "file path to read the fabric dependent bitstream mapping database");
  shell_cmd.set_option_require_value(opt_read_file, openfpga::OPT_STRING);

  /* Add command 'fabric_bitstream' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd,
                      "Reorganize the fabric-independent bitstream for the "
                      "FPGA fabric created by FPGA-Verilog",
                      hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     build_fabric_bitstream_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_fabric_bitstream
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_fabric_bitstream_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_fabric_bitstream");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true,
    "file path to output the fabric bitstream to plain text file");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--file_format'*/
  CommandOptionId opt_file_format = shell_cmd.add_option(
    "format", false,
    "file format of fabric bitstream [plain_text|xml]. Default: plain_text");
  shell_cmd.set_option_require_value(opt_file_format, openfpga::OPT_STRING);

  /* Add an option '--fast_configuration' */
  shell_cmd.add_option("fast_configuration", false,
                       "Reduce the size of bitstream to be downloaded");

  /* Add an option '--keep_dont_care_bit' */
  shell_cmd.add_option(
    "keep_dont_care_bits", false,
    "Keep don't care bits in bitstream file; If not enabled, don't care bits "
    "are converted to logic '0' or '1'");

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command 'fabric_bitstream' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Write the fabric-dependent bitstream to a file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_fabric_bitstream_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_io_mapping
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_io_mapping_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_io_mapping");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to output the io mapping information");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command 'fabric_bitstream' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Write the I/O mapping information to a file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_io_mapping_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * Top-level function to add all the commands related to FPGA-Bitstream
 *******************************************************************/
template <class T>
void add_bitstream_command_templates(openfpga::Shell<T>& shell,
                                     const bool& hidden = false) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating
   * the dependency graph */
  const ShellCommandId& shell_cmd_build_fabric_id =
    shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_bitstream_cmd_class =
    shell.add_command_class("FPGA-Bitstream");

  /********************************
   * Command 'repack'
   */
  /* The 'repack' command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_repack;
  cmd_dependency_repack.push_back(shell_cmd_build_fabric_id);
  ShellCommandId shell_cmd_repack_id = add_repack_command_template(
    shell, openfpga_bitstream_cmd_class, cmd_dependency_repack, hidden);

  /********************************
   * Command 'build_architecture_bitstream'
   */
  /* The 'build_architecture_bitstream' command should NOT be executed before
   * 'repack' */
  std::vector<ShellCommandId> cmd_dependency_build_arch_bitstream;
  cmd_dependency_build_arch_bitstream.push_back(shell_cmd_repack_id);
  ShellCommandId shell_cmd_build_arch_bitstream_id =
    add_build_arch_bitstream_command_template(
      shell, openfpga_bitstream_cmd_class, cmd_dependency_build_arch_bitstream,
      hidden);

  /********************************
   * Command 'report_bitstream_distribution'
   */
  /* The 'report_bitstream_distribution' command should NOT be executed before
   * 'build_architecture_bitstream' */
  std::vector<ShellCommandId> cmd_dependency_report_bitstream_distribution;
  cmd_dependency_build_arch_bitstream.push_back(
    shell_cmd_build_arch_bitstream_id);
  add_report_bitstream_distribution_command_template(
    shell, openfpga_bitstream_cmd_class,
    cmd_dependency_report_bitstream_distribution, hidden);

  /********************************
   * Command 'build_fabric_bitstream'
   */
  /* The 'build_fabric_bitstream' command should NOT be executed before
   * 'build_architecture_bitstream' */
  std::vector<ShellCommandId> cmd_dependency_build_fabric_bitstream;
  cmd_dependency_build_fabric_bitstream.push_back(
    shell_cmd_build_arch_bitstream_id);
  ShellCommandId shell_cmd_build_fabric_bitstream_id =
    add_build_fabric_bitstream_command_template(
      shell, openfpga_bitstream_cmd_class,
      cmd_dependency_build_fabric_bitstream, hidden);

  /********************************
   * Command 'write_fabric_bitstream'
   */
  /* The 'write_fabric_bitstream' command should NOT be executed before
   * 'build_fabric_bitstream' */
  std::vector<ShellCommandId> cmd_dependency_write_fabric_bitstream;
  cmd_dependency_write_fabric_bitstream.push_back(
    shell_cmd_build_fabric_bitstream_id);
  add_write_fabric_bitstream_command_template(
    shell, openfpga_bitstream_cmd_class, cmd_dependency_write_fabric_bitstream,
    hidden);

  /********************************
   * Command 'write_io_mapping'
   */
  /* The 'write_io_mapping' command should NOT be executed before 'build_fabric'
   */
  std::vector<ShellCommandId> cmd_dependency_write_io_mapping;
  cmd_dependency_write_io_mapping.push_back(shell_cmd_build_fabric_id);
  add_write_io_mapping_command_template(shell, openfpga_bitstream_cmd_class,
                                        cmd_dependency_write_io_mapping,
                                        hidden);
}

} /* end namespace openfpga */

#endif
