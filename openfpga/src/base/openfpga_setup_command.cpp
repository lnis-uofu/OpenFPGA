/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of setting up OpenFPGA core engine, including:
 * - read_openfpga_arch : read OpenFPGA architecture file
 *******************************************************************/
#include "openfpga_read_arch.h"
#include "openfpga_link_arch.h"
#include "openfpga_pb_pin_fixup.h"
#include "openfpga_lut_truth_table_fixup.h"
#include "check_netlist_naming_conflict.h"
#include "openfpga_build_fabric.h"
#include "openfpga_write_gsb.h"
#include "openfpga_setup_command.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: read_openfpga_arch
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_read_arch_command(openfpga::Shell<OpenfpgaContext>& shell,
                                              const ShellCommandClassId& cmd_class_id) {
  Command shell_cmd("read_openfpga_arch");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_arch_file = shell_cmd.add_option("file", true, "file path to the architecture XML");
  shell_cmd.set_option_short_name(opt_arch_file, "f");
  shell_cmd.set_option_require_value(opt_arch_file, openfpga::OPT_STRING);

  /* Add command 'read_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "read OpenFPGA architecture file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, read_arch);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_openfpga_arch
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_arch_command(openfpga::Shell<OpenfpgaContext>& shell,
                                               const ShellCommandClassId& cmd_class_id,
                                               const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_openfpga_arch");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option("file", true, "file path to the architecture XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "write OpenFPGA architecture file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id, write_arch);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: read_openfpga_simulation_setting
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_read_simulation_setting_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                            const ShellCommandClassId& cmd_class_id) {
  Command shell_cmd("read_openfpga_simulation_setting");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option("file", true, "file path to the simulation setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'read_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "read OpenFPGA simulation setting file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, read_simulation_setting);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_openfpga_simulation_setting
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_simulation_setting_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                             const ShellCommandClassId& cmd_class_id,
                                                             const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_openfpga_simulation_setting");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option("file", true, "file path to the simulation setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "write OpenFPGA simulation setting file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id, write_simulation_setting);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: read_openfpga_bitstream_setting
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_read_bitstream_setting_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                           const ShellCommandClassId& cmd_class_id) {
  Command shell_cmd("read_openfpga_bitstream_setting");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option("file", true, "file path to the bitstream setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'read_openfpga_bitstream_setting' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "read OpenFPGA bitstream setting file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, read_bitstream_setting);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_openfpga_bitstream_setting
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_bitstream_setting_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                            const ShellCommandClassId& cmd_class_id,
                                                            const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_openfpga_bitstream_setting");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option("file", true, "file path to the bitstream setting XML");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add command 'write_openfpga_bitstream_setting' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "write OpenFPGA bitstream setting file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id, write_bitstream_setting);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: link_openfpga_arch
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_link_arch_command(openfpga::Shell<OpenfpgaContext>& shell,
                                              const ShellCommandClassId& cmd_class_id,
                                              const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("link_openfpga_arch");

  /* Add an option '--activity_file'*/
  CommandOptionId opt_act_file = shell_cmd.add_option("activity_file", false, "file path to the signal activity");
  shell_cmd.set_option_require_value(opt_act_file, openfpga::OPT_STRING);

  /* Add an option '--sort_gsb_chan_node_in_edges'*/
  shell_cmd.add_option("sort_gsb_chan_node_in_edges", false, "Sort all the incoming edges for each routing track output node in General Switch Blocks (GSBs)");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");
  
  /* Add command 'link_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "Bind OpenFPGA architecture to VPR");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, link_arch);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_gsb_to_xml
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_gsb_command(openfpga::Shell<OpenfpgaContext>& shell,
                                              const ShellCommandClassId& cmd_class_id,
                                              const std::vector<ShellCommandId>& dependent_cmds) {
  Command shell_cmd("write_gsb_to_xml");
  /* Add an option '--file' in short '-f'*/
  CommandOptionId opt_file = shell_cmd.add_option("file", true, "path to the directory that stores the XML files");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'write_openfpga_arch' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "write internal structures of General Switch Blocks to XML file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id, write_gsb);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: check_netlist_naming_conflict
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_check_netlist_naming_conflict_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                                  const ShellCommandClassId& cmd_class_id,
                                                                  const std::vector<ShellCommandId>& dependent_cmds) {

  Command shell_cmd("check_netlist_naming_conflict");

  /* Add an option '--fix' */
  shell_cmd.add_option("fix", false, "Apply correction to any conflicts found");

  /* Add an option '--report' */
  CommandOptionId opt_rpt = shell_cmd.add_option("report", false, "Output a report file about what any correction applied");
  shell_cmd.set_option_require_value(opt_rpt, openfpga::OPT_STRING);

  /* Add command 'check_netlist_naming_conflict' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "Check any block/net naming in users' BLIF netlist violates the syntax of fabric generator");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, check_netlist_naming_conflict);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: pb_pin_fixup
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_pb_pin_fixup_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                 const ShellCommandClassId& cmd_class_id,
                                                 const std::vector<ShellCommandId>& dependent_cmds) {

  Command shell_cmd("pb_pin_fixup");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'pb_pin_fixup' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "Fix up the packing results due to pin swapping during routing stage");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, pb_pin_fixup);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: lut_truth_table_fixup
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_lut_truth_table_fixup_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                          const ShellCommandClassId& cmd_class_id,
                                                          const std::vector<ShellCommandId>& dependent_cmds) {

  Command shell_cmd("lut_truth_table_fixup");
  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'lut_truth_table_fixup' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "Fix up the truth table of Look-Up Tables due to pin swapping during packing stage");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, lut_truth_table_fixup);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: build_fabric
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_build_fabric_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                 const ShellCommandClassId& cmd_class_id,
                                                 const std::vector<ShellCommandId>& dependent_cmds) {

  Command shell_cmd("build_fabric");

  /* Add an option '--frame_view' */
  shell_cmd.add_option("frame_view", false, "Build only frame view of the fabric (nets are skipped)");

  /* Add an option '--compress_routing' */
  shell_cmd.add_option("compress_routing", false, "Compress the number of unique routing modules by identifying the unique GSBs");

  /* Add an option '--duplicate_grid_pin' */
  shell_cmd.add_option("duplicate_grid_pin", false, "Duplicate the pins on the same side of a grid");

  /* Add an option '--load_fabric_key' */
  CommandOptionId opt_load_fkey = shell_cmd.add_option("load_fabric_key", false, "load the fabric key from the given file");
  shell_cmd.set_option_require_value(opt_load_fkey, openfpga::OPT_STRING);

  /* Add an option '--write_fabric_key' */
  CommandOptionId opt_write_fkey = shell_cmd.add_option("write_fabric_key", false, "output current fabric key to a file");
  shell_cmd.set_option_require_value(opt_write_fkey, openfpga::OPT_STRING);

  /* Add an option '--generate_random_fabric_key' */
  shell_cmd.add_option("generate_random_fabric_key", false, "Create a random fabric key which will shuffle the memory address for encryption purpose");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'compact_routing_hierarchy' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "Build the FPGA fabric in a graph of modules");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, build_fabric);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

/********************************************************************
 * - Add a command to Shell environment: write_fabric_hierarchy
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
ShellCommandId add_openfpga_write_fabric_hierarchy_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                           const ShellCommandClassId& cmd_class_id,
                                                           const std::vector<ShellCommandId>& dependent_cmds) {

  Command shell_cmd("write_fabric_hierarchy");

  /* Add an option '--file' */
  CommandOptionId opt_file = shell_cmd.add_option("file", true, "Specify the file name to write the hierarchy to");
  shell_cmd.set_option_short_name(opt_file, "f");
  shell_cmd.set_option_require_value(opt_file, openfpga::OPT_STRING);

  /* Add an option '--depth' */
  CommandOptionId opt_depth = shell_cmd.add_option("depth", false, "Specify the depth of hierarchy to which the writer should stop");
  shell_cmd.set_option_require_value(opt_depth, openfpga::OPT_INT);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Show verbose outputs");

  /* Add command 'write_fabric_hierarchy' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "Write the hierarchy of FPGA fabric graph to a plain-text file");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_const_execute_function(shell_cmd_id, write_fabric_hierarchy);

  /* Add command dependency to the Shell */
  shell.set_command_dependency(shell_cmd_id, dependent_cmds);

  return shell_cmd_id;
}

void add_openfpga_setup_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'vpr' command which is to be used in creating the dependency graph */
  const ShellCommandId& vpr_cmd_id = shell.command(std::string("vpr"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_setup_cmd_class = shell.add_command_class("OpenFPGA setup");

  /******************************** 
   * Command 'read_openfpga_arch' 
   */
  ShellCommandId read_arch_cmd_id = add_openfpga_read_arch_command(shell,
                                                                   openfpga_setup_cmd_class);

  /******************************** 
   * Command 'write_openfpga_arch' 
   */
  /* The 'write_openfpga_arch' command should NOT be executed before 'read_openfpga_arch' */
  std::vector<ShellCommandId> write_arch_dependent_cmds(1, read_arch_cmd_id);
  add_openfpga_write_arch_command(shell,
                                  openfpga_setup_cmd_class,
                                  write_arch_dependent_cmds);

  /******************************** 
   * Command 'read_openfpga_simulation_setting' 
   */
  ShellCommandId read_sim_setting_cmd_id = add_openfpga_read_simulation_setting_command(shell,
                                                                                        openfpga_setup_cmd_class);

  /******************************** 
   * Command 'write_openfpga_simulation_setting' 
   */
  /* The 'write_openfpga_simulation_setting' command should NOT be executed before 'read_openfpga_simulation_setting' */
  std::vector<ShellCommandId> write_sim_setting_dependent_cmds(1, read_sim_setting_cmd_id);
  add_openfpga_write_simulation_setting_command(shell,
                                                openfpga_setup_cmd_class,
                                                write_sim_setting_dependent_cmds);

  /******************************** 
   * Command 'read_openfpga_bitstream_setting' 
   */
  ShellCommandId read_bitstream_setting_cmd_id = add_openfpga_read_bitstream_setting_command(shell,
                                                                                             openfpga_setup_cmd_class);

  /******************************** 
   * Command 'write_openfpga_bitstream_setting' 
   */
  /* The 'write_openfpga_bitstream_setting' command should NOT be executed before 'read_openfpga_bitstream_setting' */
  std::vector<ShellCommandId> write_bitstream_setting_dependent_cmds(1, read_bitstream_setting_cmd_id);
  add_openfpga_write_bitstream_setting_command(shell,
                                               openfpga_setup_cmd_class,
                                               write_bitstream_setting_dependent_cmds);


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
  ShellCommandId link_arch_cmd_id = add_openfpga_link_arch_command(shell,
                                                                   openfpga_setup_cmd_class,
                                                                   link_arch_dependent_cmds);

  /******************************** 
   * Command 'write_gsb' 
   */
  /* The 'write_gsb' command should NOT be executed before 'link_openfpga_arch' */
  std::vector<ShellCommandId> write_gsb_dependent_cmds;
  write_gsb_dependent_cmds.push_back(link_arch_cmd_id);
  add_openfpga_write_gsb_command(shell,
                                 openfpga_setup_cmd_class,
                                 write_gsb_dependent_cmds);

  /******************************************* 
   * Command 'check_netlist_naming_conflict'
   */ 
  /* The 'check_netlist_naming_conflict' command should NOT be executed before 'vpr' */
  std::vector<ShellCommandId> nlist_naming_dependent_cmds;
  nlist_naming_dependent_cmds.push_back(vpr_cmd_id);
  add_openfpga_check_netlist_naming_conflict_command(shell,
                                                     openfpga_setup_cmd_class,
                                                     nlist_naming_dependent_cmds);

  /******************************** 
   * Command 'pb_pin_fixup' 
   */
  /* The 'pb_pin_fixup' command should NOT be executed before 'read_openfpga_arch' and 'vpr' */
  std::vector<ShellCommandId> pb_pin_fixup_dependent_cmds;
  pb_pin_fixup_dependent_cmds.push_back(read_arch_cmd_id);
  pb_pin_fixup_dependent_cmds.push_back(vpr_cmd_id);
  add_openfpga_pb_pin_fixup_command(shell,
                                    openfpga_setup_cmd_class,
                                    pb_pin_fixup_dependent_cmds);

  /******************************** 
   * Command 'lut_truth_table_fixup' 
   */
  /* The 'lut_truth_table_fixup' command should NOT be executed before 'read_openfpga_arch' and 'vpr' */
  std::vector<ShellCommandId> lut_tt_fixup_dependent_cmds;
  lut_tt_fixup_dependent_cmds.push_back(read_arch_cmd_id);
  lut_tt_fixup_dependent_cmds.push_back(vpr_cmd_id);
  add_openfpga_lut_truth_table_fixup_command(shell,
                                             openfpga_setup_cmd_class,
                                             lut_tt_fixup_dependent_cmds);
  /******************************** 
   * Command 'build_fabric' 
   */
  /* The 'build_fabric' command should NOT be executed before 'link_openfpga_arch' */
  std::vector<ShellCommandId> build_fabric_dependent_cmds;
  build_fabric_dependent_cmds.push_back(link_arch_cmd_id);
  ShellCommandId build_fabric_cmd_id = add_openfpga_build_fabric_command(shell,
                                                                         openfpga_setup_cmd_class,
                                                                         build_fabric_dependent_cmds);

  /******************************** 
   * Command 'write_fabric_hierarchy' 
   */
  /* The 'write_fabric_hierarchy' command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> write_fabric_hie_dependent_cmds;
  write_fabric_hie_dependent_cmds.push_back(build_fabric_cmd_id);
  add_openfpga_write_fabric_hierarchy_command(shell,
                                              openfpga_setup_cmd_class,
                                              write_fabric_hie_dependent_cmds);
} 

} /* end namespace openfpga */
