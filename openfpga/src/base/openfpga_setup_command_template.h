#ifndef OPENFPGA_SETUP_COMMAND_TEMPLATE_H
#define OPENFPGA_SETUP_COMMAND_TEMPLATE_H
/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of setting up OpenFPGA core engine, including:
 * - read_openfpga_arch : read OpenFPGA architecture file
 *******************************************************************/
#include "check_netlist_naming_conflict_template.h"
#include "openfpga_build_fabric_template.h"
#include "openfpga_link_arch_template.h"
#include "openfpga_lut_truth_table_fixup_template.h"
#include "openfpga_pb_pin_fixup_template.h"
#include "openfpga_pcf2place_template.h"
#include "openfpga_read_arch_template.h"
#include "openfpga_write_gsb_template.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: read_openfpga_arch
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_read_openfpga_arch_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const bool& hidden) {
  Command shell_cmd("read_openfpga_arch");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_arch_file =
    shell_cmd.add_option("file", true, "file path to the architecture XML");
  shell_cmd.set_option_short_name(opt_arch_file, "f");
  shell_cmd.set_option_require_value(opt_arch_file, openfpga::OPT_STRING);

  /* Add command 'read_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd, "read OpenFPGA architecture file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     read_openfpga_arch_template<T>);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_openfpga_arch
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_openfpga_arch_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_openfpga_arch");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file =
    shell_cmd.add_option("file", true, "file path to the architecture XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd, "write OpenFPGA architecture file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id,
                                           write_openfpga_arch_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: read_openfpga_simulation_setting
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_read_simulation_setting_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const bool& hidden) {
  Command shell_cmd("read_openfpga_simulation_setting");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the simulation setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'read_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "read OpenFPGA simulation setting file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     read_simulation_setting_template<T>);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_openfpga_simulation_setting
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_simulation_setting_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_openfpga_simulation_setting");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the simulation setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "write OpenFPGA simulation setting file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(
    shell_cmd_id, write_simulation_setting_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: read_openfpga_bitstream_setting
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_read_bitstream_setting_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const bool& hidden) {
  Command shell_cmd("read_openfpga_bitstream_setting");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the bitstream setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'read_openfpga_bitstream_setting' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "read OpenFPGA bitstream setting file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     read_bitstream_setting_template<T>);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_openfpga_bitstream_setting
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_bitstream_setting_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_openfpga_bitstream_setting");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the bitstream setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_bitstream_setting' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "write OpenFPGA bitstream setting file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id,
                                           write_bitstream_setting_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: link_openfpga_arch
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_link_arch_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("link_openfpga_arch");

  /* Add an option '--activity_file'*/
  CommandOptionId opt_act_file = shell_cmd.add_option(
    "activity_file", false, "file path to the signal activity");
  shell_cmd.set_option_require_value(opt_act_file, openfpga::OPT_STRING);

  /* Add an option '--sort_gsb_chan_node_in_edges'*/
  shell_cmd.add_option("sort_gsb_chan_node_in_edges", false,
                       "Sort all the incoming edges for each routing track "
                       "output node in General Switch Blocks (GSBs)");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'link_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd, "Bind OpenFPGA architecture to VPR", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, link_arch_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_gsb_to_xml
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_gsb_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_gsb_to_xml");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "path to the directory that stores the XML files");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--unique' */
  shell_cmd.add_option("unique", false, "Only output unique GSB blocks");

  /* Add an option '--exclude_rr_info' */
  shell_cmd.add_option("exclude_rr_info", false,
                       "Exclude routing resource graph information from output "
                       "files, e.g., node id as well as other attributes. This "
                       "is useful to check the connection inside GSBs purely.");

  /* Add an option '--exclude'*/
  CommandOptionId opt_exclude =
    shell_cmd.add_option("exclude", false,
                         "Exclude part of the GSB data to be outputted. Can be "
                         "[``sb``|``cbx``|``cby``]. Users can exclude multiple "
                         "parts by using a splitter ``,``");
  shell_cmd.set_option_require_value(opt_exclude, openfpga::OPT_STRING);

  /* Add an option '--gsb_names'*/
  CommandOptionId opt_gsb_names =
    shell_cmd.add_option("gsb_names", false,
                         "Specify the name of GSB to be outputted. Users can "
                         "specify multiple GSBs by using a splitter ``,``");
  shell_cmd.set_option_require_value(opt_gsb_names, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'write_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "write internal structures of General Switch Blocks to XML file",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id, write_gsb_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: check_netlist_naming_conflict
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_check_netlist_naming_conflict_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("check_netlist_naming_conflict");

  /* Add an option '--fix' */
  shell_cmd.add_option("fix", false, "Apply correction to any conflicts found");

  /* Add an option '--report' */
  CommandOptionId opt_rpt = shell_cmd.add_option(
    "report", false, "Output a report file about what any correction applied");
  shell_cmd.set_option_require_value(opt_rpt, openfpga::OPT_STRING);

  /* Add command 'check_netlist_naming_conflict' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd,
                      "Check any block/net naming in users' BLIF netlist "
                      "violates the syntax of fabric generator",
                      hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     check_netlist_naming_conflict_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: pb_pin_fixup
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_pb_pin_fixup_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("pb_pin_fixup");

  /* Add an option '--map_global_net_to_msb' */
  shell_cmd.add_option(
    "map_global_net_to_msb", false,
    "If specified, any global net including clock, reset etc, will be mapped "
    "to a best-fit Most Significant Bit (MSB) of input ports of programmable "
    "blocks. If not specified, a best-fit Least Significant Bit (LSB) will be "
    "the default choice");
  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'pb_pin_fixup' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "Fix up the packing results due to pin swapping during routing stage",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, pb_pin_fixup_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: lut_truth_table_fixup
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_lut_truth_table_fixup_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("lut_truth_table_fixup");
  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'lut_truth_table_fixup' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd,
                      "Fix up the truth table of Look-Up Tables due to pin "
                      "swapping during packing stage",
                      hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     lut_truth_table_fixup_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: build_fabric
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_build_fabric_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("build_fabric");

  /* Add an option '--frame_view' */
  shell_cmd.add_option(
    "frame_view", false,
    "Build only frame view of the fabric (nets are skipped)");

  /* Add an option '--compress_routing' */
  shell_cmd.add_option("compress_routing", false,
                       "Compress the number of unique routing modules by "
                       "identifying the unique GSBs");

  /* Add an option '--duplicate_grid_pin' */
  shell_cmd.add_option("duplicate_grid_pin", false,
                       "Duplicate the pins on the same side of a grid");

  /* Add an option '--name_module_using_index' */
  shell_cmd.add_option("name_module_using_index", false,
                       "Use index to name modules, such as cbx_0_, rather than "
                       "coordinates, such as cbx_1__0_");

  /* Add an option '--load_fabric_key' */
  CommandOptionId opt_load_fkey = shell_cmd.add_option(
    "load_fabric_key", false, "load the fabric key from the given file");
  shell_cmd.set_option_require_value(opt_load_fkey, openfpga::OPT_STRING);

  /* Add an option '--write_fabric_key' */
  CommandOptionId opt_write_fkey = shell_cmd.add_option(
    "write_fabric_key", false, "output current fabric key to a file");
  shell_cmd.set_option_require_value(opt_write_fkey, openfpga::OPT_STRING);

  /* Add an option '--group_tile' */
  CommandOptionId opt_group_tile = shell_cmd.add_option(
    "group_tile", false,
    "group programmable blocks and routing blocks into tiles. This helps to "
    "reduce the number of blocks at top-level");
  shell_cmd.set_option_require_value(opt_group_tile, openfpga::OPT_STRING);

  /* Add an option '--group_config_block' */
  shell_cmd.add_option("group_config_block", false,
                       "group configuration memory blocks under CLB/SB/CB "
                       "blocks etc. This helps to "
                       "reduce optimize the density of configuration memory "
                       "through physical design");

  /* Add an option '--generate_random_fabric_key' */
  shell_cmd.add_option("generate_random_fabric_key", false,
                       "Create a random fabric key which will shuffle the "
                       "memory address for encryption purpose");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'compact_routing_hierarchy' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Build the FPGA fabric in a graph of modules", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, build_fabric_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_fabric_hierarchy
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_fabric_hierarchy_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_fabric_hierarchy");

  /* Add an option '--file' */
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "Specify the file name to write the hierarchy to");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--module' */
  CommandOptionId opt_module = shell_cmd.add_option(
    "module", false,
    "Specify the root module name(s) which should be considered. By default, "
    "it is fpga_top. Regular expression is supported");
  shell_cmd.set_option_require_value(opt_module, openfpga::OPT_STRING);
  CommandOptionId opt_filter =
    shell_cmd.add_option("filter", false,
                         "Specify the filter which allows user to select "
                         "modules to appear under each root module tree. By "
                         "default, it is *. Regular expression is supported");
  shell_cmd.set_option_require_value(opt_filter, openfpga::OPT_STRING);

  /* Add an option '--depth' */
  CommandOptionId opt_depth = shell_cmd.add_option(
    "depth", false,
    "Specify the depth of hierarchy to which the writer should stop");
  shell_cmd.set_option_require_value(opt_depth, openfpga::OPT_INT);

  shell_cmd.add_option("exclude_empty_modules", false,
                       "Exclude modules with no qualified children (match the "
                       "names defined through filter) from the output file");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'write_fabric_hierarchy' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Write the hierarchy of FPGA fabric graph to a plain-text file",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id,
                                           write_fabric_hierarchy_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_fabric_io_info
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_fabric_io_info_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_fabric_io_info");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to output the I/O information");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd,
                      "Write the I/O information, e.g., locations and similar "
                      "attributes, to a file",
                      hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_fabric_io_info_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: pcf2place
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_pcf2place_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const bool& hidden) {
  Command shell_cmd("pcf2place");

  /* Add an option '--pcf'*/
  CommandOptionId opt_pcf_file =
    shell_cmd.add_option("pcf", true, "file path to the user pin constraint");
  shell_cmd.set_option_require_value(opt_pcf_file, openfpga::OPT_STRING);

  /* Add an option '--blif'*/
  CommandOptionId opt_blif_file = shell_cmd.add_option(
    "blif", true, "file path to the synthesized netlist (.blif)");
  shell_cmd.set_option_require_value(opt_blif_file, openfpga::OPT_STRING);

  /* Add an option '--fpga_io_map'*/
  CommandOptionId opt_fpga_io_map_file = shell_cmd.add_option(
    "fpga_io_map", true, "file path to FPGA I/O location map (.xml)");
  shell_cmd.set_option_require_value(opt_fpga_io_map_file,
                                     openfpga::OPT_STRING);

  /* Add an option '--pin_table'*/
  CommandOptionId opt_pin_table_file = shell_cmd.add_option(
    "pin_table", true, "file path to the pin table (.csv)");
  shell_cmd.set_option_require_value(opt_pin_table_file, openfpga::OPT_STRING);

  /* Add an option '--fpga_fix_pins'*/
  CommandOptionId opt_fpga_fix_pins_file = shell_cmd.add_option(
    "fpga_fix_pins", true,
    "file path to the output fix-pin placement file (.place)");
  shell_cmd.set_option_require_value(opt_fpga_fix_pins_file,
                                     openfpga::OPT_STRING);

  /* Add an option '--pin_table_direction_convention'*/
  CommandOptionId opt_pin_table_dir_convention =
    shell_cmd.add_option("pin_table_direction_convention", false,
                         "the convention to follow when inferring pin "
                         "direction from the name of ports in pin table file");
  shell_cmd.set_option_require_value(opt_pin_table_dir_convention,
                                     openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  /* Add an option '--reduce_error_to_warning' */
  shell_cmd.add_option(
    "reduce_error_to_warning", false,
    "reduce error to warning while reading commands in pcf file");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Convert user Pin Constraint File (.pcf) to an placement file",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     pcf2place_wrapper_template<T>);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: read_openfpga_clock_arch
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_read_openfpga_clock_arch_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("read_openfpga_clock_arch");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the clock architecture XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'read_openfpga_clock_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "read OpenFPGA clock architecture file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     read_openfpga_clock_arch_template<T>);
  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_openfpga_clock_arch
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_openfpga_clock_arch_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_openfpga_clock_arch");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the clock architecture XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_clock_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "write OpenFPGA clock architecture file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(
    shell_cmd_id, write_openfpga_clock_arch_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: append_clock_rr_graph
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_append_clock_rr_graph_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("append_clock_rr_graph");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'pb_pin_fixup' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "Append clock network to the routing resource graph built by VPR.", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     append_clock_rr_graph_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: route_clock_rr_graph
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_route_clock_rr_graph_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("route_clock_rr_graph");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file =
    shell_cmd.add_option("pin_constraints_file", false,
                         "specify the file path to the pin constraints");
  shell_cmd.set_option_short_name(opt_file, "pcf");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  shell_cmd.add_option("disable_unused_trees", false,
                       "Disable entire clock trees when they are not used by "
                       "any clock nets. Useful to reduce clock power");
  shell_cmd.add_option("disable_unused_spines", false,
                       "Disable part of the clock tree which are used by clock "
                       "nets. Useful to reduce clock power");
  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'pb_pin_fixup' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "Route clock network based on routing resource graph built by VPR.",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     route_clock_rr_graph_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: add_fpga_core_to_fabric
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_add_fpga_core_to_fabric_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("add_fpga_core_to_fabric");

  /* Add an option '--io_naming'*/
  CommandOptionId opt_io_naming = shell_cmd.add_option(
    "io_naming", false, "specify the file path to the I/O naming rules");
  shell_cmd.set_option_require_value(opt_io_naming, openfpga::OPT_STRING);

  /* Add an option '--instance_name'*/
  CommandOptionId opt_inst_name = shell_cmd.add_option(
    "instance_name", false, "specify the instance of fpga_core under fpga_top");
  shell_cmd.set_option_require_value(opt_inst_name, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option(
    "frame_view", false,
    "Build only frame view of the fabric (nets are skipped)");
  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'pb_pin_fixup' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "Add fpga_core as an intermediate layer to FPGA fabric. After this "
    "command, the fpga_top will remain the top-level module while there is a "
    "new module fpga_core under it. Under fpga_core, there will be the "
    "detailed building blocks",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     add_fpga_core_to_fabric_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_fabric_key
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_fabric_key_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_fabric_key");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file =
    shell_cmd.add_option("file", true, "file path to the fabric key XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--include_module_keys'*/
  shell_cmd.add_option("include_module_keys", false,
                       "Include module-level keys");
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "write fabric key of the FPGA fabric to file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id,
                                           write_fabric_key_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: rename_modules
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_rename_modules_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("rename_modules");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the XML file that contains renaming rules");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd, "Rename modules with a set of given rules", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, rename_modules_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_module_naming_rules
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_module_naming_rules_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_module_naming_rules");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "file path to the XML file that contains renaming rules");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "Output the naming rules for each module of an FPGA fabric to a given file",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(
    shell_cmd_id, write_module_naming_rules_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_pin_physical_location
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_fabric_pin_physical_location_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_fabric_pin_physical_location");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true,
    "file path to the XML file that contains pin physical location");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--module'*/
  CommandOptionId opt_module = shell_cmd.add_option(
    "module", false,
    "specify the module whose pin physical location should be outputted");
  shell_cmd.set_option_require_value(opt_module, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "Do not print time stamp in output files");

  shell_cmd.add_option(
    "show_invalid_side", false,
    "Include pins with invalid sides in output files. Recommended for "
    "debugging as the output file may include a lot of useless information");

  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(
    shell_cmd,
    "Output the pin physical location of an FPGA fabric to a given file",
    hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(
    shell_cmd_id, write_fabric_pin_physical_location_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: read_unique_blocks
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_read_unique_blocks_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("read_unique_blocks");

  /* Add an option '--file' */
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true, "specify the file which contains unique block information");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--type' */
  CommandOptionId opt_type =
    shell_cmd.add_option("type", true,
                         "Specify the type of the unique blocks file "
                         "[xml|bin]. If not specified, by default it is XML.");
  shell_cmd.set_option_require_value(opt_type, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'compact_routing_hierarchy' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd, "Preload unique blocks from xml file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     read_unique_blocks_template<T>);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_unique_blocks
 * - Add associated options
 * - Add command dependency
 *******************************************************************/
template <class T>
ShellCommandId add_write_unique_blocks_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("write_unique_blocks");

  /* Add an option '--file' */
  CommandOptionId opt_file = shell_cmd.add_option(
    "file", true,
    "specify the file which we will write unique block information to");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--type' */
  CommandOptionId opt_type =
    shell_cmd.add_option("type", true,
                         "Specify the type of the unique blocks file "
                         "[xml|bin]. If not specified, by default it is XML.");
  shell_cmd.set_option_require_value(opt_type, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'compact_routing_hierarchy' to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd, "Write unique blocks to a xml file", hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id,
                                     write_unique_blocks_template<T>);
  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/******************************************************************
 * - Add a command to Shell environment: report_reference
 * - Add associated options
 * - Add command dependency
 ******************************************************************/
template <class T>
ShellCommandId add_report_reference_command_template(
  openfpga::Shell<T>& shell, const ShellCommandClassId& cmd_class_id,
  const std::vector<ShellCommandId>& dependent_cmds, const bool& hidden) {
  Command shell_cmd("report_reference");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file =
    shell_cmd.add_option("file", true, "specify the file to output results");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--module'*/
  CommandOptionId opt_module =
    shell_cmd.add_option("module", false,
                         "specify the module under which the references of "
                         "child modules will be reported");
  shell_cmd.set_option_require_value(opt_module, openfpga::OPT_STRING);

  /* Add an option '--no_time_stamp' */
  shell_cmd.add_option("no_time_stamp", false,
                       "do not print time stamp in output files");

  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command to the Shell */
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd,
                      "report all instances of each unique module, "
                      "under a given module",
                      hidden);
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id,
                                           report_reference_template<T>);
  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

template <class T>
void add_setup_command_templates(openfpga::Shell<T>& shell,
                                 const bool& hidden = false) {
  /* Get the unique id of 'vpr' command which is to be used in creating the
   * dependency graph */
  const ShellCommandId& vpr_cmd_id = shell.command(std::string("vpr"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_setup_cmd_class =
    shell.add_command_class("OpenFPGA setup");

  /********************************
   * Command 'pcf2place'
   */
  add_pcf2place_command_template<T>(shell, openfpga_setup_cmd_class, hidden);

  /********************************
   * Command 'read_openfpga_arch'
   */
  ShellCommandId read_arch_cmd_id = add_read_openfpga_arch_command_template<T>(
    shell, openfpga_setup_cmd_class, hidden);

  /********************************
   * Command 'write_openfpga_arch'
   */
  /* The 'write_openfpga_arch' command should NOT be executed before
   * 'read_openfpga_arch' */
  std::vector<ShellCommandId> write_arch_dependent_cmds(1, read_arch_cmd_id);
  add_write_openfpga_arch_command_template<T>(
    shell, openfpga_setup_cmd_class, write_arch_dependent_cmds, hidden);

  /********************************
   * Command 'read_openfpga_simulation_setting'
   */
  ShellCommandId read_sim_setting_cmd_id =
    add_read_simulation_setting_command_template<T>(
      shell, openfpga_setup_cmd_class, hidden);

  /********************************
   * Command 'write_openfpga_simulation_setting'
   */
  /* The 'write_openfpga_simulation_setting' command should NOT be executed
   * before 'read_openfpga_simulation_setting' */
  std::vector<ShellCommandId> write_sim_setting_dependent_cmds(
    1, read_sim_setting_cmd_id);
  add_write_simulation_setting_command_template<T>(
    shell, openfpga_setup_cmd_class, write_sim_setting_dependent_cmds, hidden);

  /********************************
   * Command 'read_openfpga_bitstream_setting'
   */
  ShellCommandId read_bitstream_setting_cmd_id =
    add_read_bitstream_setting_command_template<T>(
      shell, openfpga_setup_cmd_class, hidden);

  /********************************
   * Command 'write_openfpga_bitstream_setting'
   */
  /* The 'write_openfpga_bitstream_setting' command should NOT be executed
   * before 'read_openfpga_bitstream_setting' */
  std::vector<ShellCommandId> write_bitstream_setting_dependent_cmds(
    1, read_bitstream_setting_cmd_id);
  add_write_bitstream_setting_command_template<T>(
    shell, openfpga_setup_cmd_class, write_bitstream_setting_dependent_cmds,
    hidden);

  /********************************
   * Command 'read_openfpga_clock_arch'
   */
  std::vector<ShellCommandId> read_openfpga_clock_arch_dependent_cmds;
  read_openfpga_clock_arch_dependent_cmds.push_back(vpr_cmd_id);
  read_openfpga_clock_arch_dependent_cmds.push_back(read_arch_cmd_id);
  ShellCommandId read_openfpga_clock_arch_cmd_id =
    add_read_openfpga_clock_arch_command_template<T>(
      shell, openfpga_setup_cmd_class, read_openfpga_clock_arch_dependent_cmds,
      hidden);

  /********************************
   * Command 'write_openfpga_clock_arch'
   */
  /* The 'write_openfpga_clock_arch' command should NOT be executed
   * before 'read_openfpga_clock_arch' */
  std::vector<ShellCommandId> write_openfpga_clock_arch_dependent_cmds(
    1, read_openfpga_clock_arch_cmd_id);
  add_write_openfpga_clock_arch_command_template<T>(
    shell, openfpga_setup_cmd_class, write_openfpga_clock_arch_dependent_cmds,
    hidden);

  /********************************
   * Command 'link_openfpga_arch'
   */
  /* The 'link_openfpga_arch' command should NOT be executed before 'vpr' */
  std::vector<ShellCommandId> link_arch_dependent_cmds;
  link_arch_dependent_cmds.push_back(read_arch_cmd_id);
  /* TODO: This will be uncommented when openfpga flow script is updated
   */
  link_arch_dependent_cmds.push_back(read_sim_setting_cmd_id);
  link_arch_dependent_cmds.push_back(vpr_cmd_id);
  ShellCommandId link_arch_cmd_id = add_link_arch_command_template<T>(
    shell, openfpga_setup_cmd_class, link_arch_dependent_cmds, hidden);

  /********************************
   * Command 'append_clock_rr_graph'
   */
  /* The 'append_clock_rr_graph' command should NOT be executed before
   * 'read_openfpga_clock_arch' */
  std::vector<ShellCommandId> append_clock_rr_graph_dependent_cmds;
  append_clock_rr_graph_dependent_cmds.push_back(
    read_openfpga_clock_arch_cmd_id);
  add_append_clock_rr_graph_command_template<T>(
    shell, openfpga_setup_cmd_class, append_clock_rr_graph_dependent_cmds,
    hidden);

  /********************************
   * Command 'route_clock_rr_graph'
   */
  /* The 'route_clock_rr_graph' command should NOT be executed before
   * 'read_openfpga_clock_arch' and 'link_arch' */
  std::vector<ShellCommandId> route_clock_rr_graph_dependent_cmds;
  route_clock_rr_graph_dependent_cmds.push_back(
    read_openfpga_clock_arch_cmd_id);
  route_clock_rr_graph_dependent_cmds.push_back(link_arch_cmd_id);
  add_route_clock_rr_graph_command_template<T>(
    shell, openfpga_setup_cmd_class, route_clock_rr_graph_dependent_cmds,
    hidden);

  /********************************
   * Command 'write_gsb'
   */
  /* The 'write_gsb' command should NOT be executed before 'link_openfpga_arch'
   */
  std::vector<ShellCommandId> write_gsb_dependent_cmds;
  write_gsb_dependent_cmds.push_back(link_arch_cmd_id);
  add_write_gsb_command_template<T>(shell, openfpga_setup_cmd_class,
                                    write_gsb_dependent_cmds, hidden);

  /*******************************************
   * Command 'check_netlist_naming_conflict'
   */
  /* The 'check_netlist_naming_conflict' command should NOT be executed before
   * 'vpr' */
  std::vector<ShellCommandId> nlist_naming_dependent_cmds;
  nlist_naming_dependent_cmds.push_back(vpr_cmd_id);
  add_check_netlist_naming_conflict_command_template<T>(
    shell, openfpga_setup_cmd_class, nlist_naming_dependent_cmds, hidden);

  /********************************
   * Command 'pb_pin_fixup'
   */
  /* The 'pb_pin_fixup' command should NOT be executed before
   * 'read_openfpga_arch' and 'vpr' */
  std::vector<ShellCommandId> pb_pin_fixup_dependent_cmds;
  pb_pin_fixup_dependent_cmds.push_back(read_arch_cmd_id);
  pb_pin_fixup_dependent_cmds.push_back(vpr_cmd_id);
  add_pb_pin_fixup_command_template<T>(shell, openfpga_setup_cmd_class,
                                       pb_pin_fixup_dependent_cmds, hidden);

  /********************************
   * Command 'lut_truth_table_fixup'
   */
  /* The 'lut_truth_table_fixup' command should NOT be executed before
   * 'read_openfpga_arch' and 'vpr' */
  std::vector<ShellCommandId> lut_tt_fixup_dependent_cmds;
  lut_tt_fixup_dependent_cmds.push_back(read_arch_cmd_id);
  lut_tt_fixup_dependent_cmds.push_back(vpr_cmd_id);
  add_lut_truth_table_fixup_command_template<T>(
    shell, openfpga_setup_cmd_class, lut_tt_fixup_dependent_cmds, hidden);

  /********************************
   * Command 'build_fabric'
   */
  /* The 'build_fabric' command should NOT be executed before
   * 'link_openfpga_arch' */
  std::vector<ShellCommandId> build_fabric_dependent_cmds;
  build_fabric_dependent_cmds.push_back(link_arch_cmd_id);
  ShellCommandId build_fabric_cmd_id = add_build_fabric_command_template<T>(
    shell, openfpga_setup_cmd_class, build_fabric_dependent_cmds, hidden);

  /********************************
   * Command 'add_fpga_core_to_fabric'
   */
  /* The command should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> add_fpga_core_to_fabric_dependent_cmds;
  add_fpga_core_to_fabric_dependent_cmds.push_back(build_fabric_cmd_id);
  add_add_fpga_core_to_fabric_command_template<T>(
    shell, openfpga_setup_cmd_class, add_fpga_core_to_fabric_dependent_cmds,
    hidden);

  /********************************
   * Command 'write_fabric_key'
   */
  /* The 'write_fabric_key' command should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> write_fabric_key_dependent_cmds;
  write_fabric_key_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_fabric_key_command_template<T>(
    shell, openfpga_setup_cmd_class, write_fabric_key_dependent_cmds, hidden);

  /********************************
   * Command 'write_fabric_hierarchy'
   */
  /* The 'write_fabric_hierarchy' command should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> write_fabric_hie_dependent_cmds;
  write_fabric_hie_dependent_cmds.push_back(build_fabric_cmd_id);
  add_write_fabric_hierarchy_command_template<T>(
    shell, openfpga_setup_cmd_class, write_fabric_hie_dependent_cmds, hidden);

  /********************************
   * Command 'write_fabric_io_info'
   */
  /* The 'write_fabric_io_info' command should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_write_fabric_io_info;
  cmd_dependency_write_fabric_io_info.push_back(build_fabric_cmd_id);
  add_write_fabric_io_info_command_template<T>(
    shell, openfpga_setup_cmd_class, cmd_dependency_write_fabric_io_info,
    hidden);

  /********************************
   * Command 'rename_modules'
   */
  /* The 'rename_modules' command should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_rename_modules;
  cmd_dependency_rename_modules.push_back(build_fabric_cmd_id);
  add_rename_modules_command_template<T>(shell, openfpga_setup_cmd_class,
                                         cmd_dependency_rename_modules, hidden);

  /********************************
   * Command 'write_module_naming_rules'
   */
  /* The 'write_module_naming_rules' command should NOT be executed before
   * 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_write_module_naming_rules;
  cmd_dependency_write_module_naming_rules.push_back(build_fabric_cmd_id);
  add_write_module_naming_rules_command_template<T>(
    shell, openfpga_setup_cmd_class, cmd_dependency_write_module_naming_rules,
    hidden);

  /********************************
   * Command 'write_fabric_pin_physical_location'
   */
  /* The command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_write_fabric_pin_physical_location;
  cmd_dependency_write_fabric_pin_physical_location.push_back(
    build_fabric_cmd_id);
  add_write_fabric_pin_physical_location_command_template<T>(
    shell, openfpga_setup_cmd_class,
    cmd_dependency_write_fabric_pin_physical_location, hidden);

  /********************************
   * Command 'report_reference'
   */
  /* The command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_report_reference;
  cmd_dependency_report_reference.push_back(build_fabric_cmd_id);
  add_report_reference_command_template<T>(
    shell, openfpga_setup_cmd_class, cmd_dependency_report_reference, hidden);

  /********************************
   * Command 'read_unique_blocks'
   */
  /* The command should NOT be executed before
   * 'link_openfpga_arch' */
  std::vector<ShellCommandId> cmd_dependency_read_unique_blocks_command;
  cmd_dependency_read_unique_blocks_command.push_back(link_arch_cmd_id);
  add_read_unique_blocks_command_template<T>(
    shell, openfpga_setup_cmd_class, cmd_dependency_read_unique_blocks_command,
    hidden);

  /********************************
   * Command 'write_unique_blocks'
   */
  add_write_unique_blocks_command_template<T>(
    shell, openfpga_setup_cmd_class, std::vector<ShellCommandId>(), hidden);
}
} /* end namespace openfpga */

#endif
