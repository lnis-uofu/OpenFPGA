/********************************************************************
 * Add vpr-related commands to the OpenFPGA shell interface
 *******************************************************************/
#include "vpr_command.h"

#include "vpr_main.h"

/* begin namespace openfpga */
namespace openfpga {

void add_vpr_macro_command(openfpga::Shell<OpenfpgaContext>& shell,
                           ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_vpr("vpr");
  ShellCommandId shell_cmd_vpr_id =
    shell.add_command(shell_cmd_vpr,
                      "Start VPR core engine to pack, place and route a BLIF "
                      "design on a FPGA architecture; Note that this command "
                      "will keep VPR results!");
  shell.set_command_class(shell_cmd_vpr_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_vpr_id, vpr::vpr_wrapper);
}

void add_vpr_standalone_command(openfpga::Shell<OpenfpgaContext>& shell,
                                ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_vpr_stdalone("vpr_standalone");
  ShellCommandId shell_cmd_vpr_stdalone_id = shell.add_command(
    shell_cmd_vpr_stdalone,
    "Start a standalone VPR core engine to pack, place and route a BLIF "
    "design on a FPGA architecture; Note that this command will NOT keep VPR "
    "results!");
  shell.set_command_class(shell_cmd_vpr_stdalone_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_vpr_stdalone_id,
                                     vpr::vpr_standalone_wrapper);
}

void add_read_vpr_arch_command(openfpga::Shell<OpenfpgaContext>& shell,
                               ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd("read_vpr_arch");
  CommandOptionId opt_arch_file =
    shell_cmd.add_option("file", true, "file path to the VPR architecture XML");
  shell_cmd.set_option_short_name(opt_arch_file, "f");
  shell_cmd.set_option_require_value(opt_arch_file, openfpga::OPT_STRING);
  CommandOptionId opt_layout =
    shell_cmd.add_option("layout", true, "device name");
  shell_cmd.set_option_short_name(opt_layout, "l");
  shell_cmd.set_option_require_value(opt_layout, openfpga::OPT_STRING);
  ShellCommandId shell_cmd_id =
    shell.add_command(shell_cmd, "read and validate VPR architecture file");
  shell.set_command_class(shell_cmd_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_id, vpr::read_vpr_arch_template);
}

void add_show_vpr_setup_command(openfpga::Shell<OpenfpgaContext>& shell,
                                ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_show_vpr_setup("show_vpr_setup");
  ShellCommandId shell_cmd_show_vpr_setup_id = shell.add_command(
    shell_cmd_show_vpr_setup,
    "Show the current VPR setup synchronized from OpenFPGA shell options");
  shell.set_command_class(shell_cmd_show_vpr_setup_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_show_vpr_setup_id,
                                     vpr::show_vpr_setup_template);
}

void add_read_circuit_command(openfpga::Shell<OpenfpgaContext>& shell,
                              ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_read_circuit("read_circuit");
  CommandOptionId opt_circuit_file = shell_cmd_read_circuit.add_option(
    "file", true, "file path to the circuit BLIF/EBLIF file");
  shell_cmd_read_circuit.set_option_short_name(opt_circuit_file, "f");
  shell_cmd_read_circuit.set_option_require_value(opt_circuit_file,
                                                  openfpga::OPT_STRING);
  ShellCommandId shell_cmd_read_circuit_id =
    shell.add_command(shell_cmd_read_circuit, "read circuit file");
  shell.set_command_class(shell_cmd_read_circuit_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_read_circuit_id,
                                     vpr::read_circuit_template);
}

void add_pack_command(openfpga::Shell<OpenfpgaContext>& shell,
                      ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_pack("pack");
  CommandOptionId opt_pack_device = shell_cmd_pack.add_option(
    "device", false, "optional device layout override for pack stage");
  shell_cmd_pack.set_option_short_name(opt_pack_device, "d");
  shell_cmd_pack.set_option_require_value(opt_pack_device,
                                          openfpga::OPT_STRING);
  CommandOptionId opt_pack_output_file = shell_cmd_pack.add_option(
    "output_file", false, "optional output file name for pack stage");
  shell_cmd_pack.set_option_short_name(opt_pack_output_file, "o");
  shell_cmd_pack.set_option_require_value(opt_pack_output_file,
                                          openfpga::OPT_STRING);
  CommandOptionId opt_pack_verbose = shell_cmd_pack.add_option(
    "verbose", false, "enable verbose output for pack stage");
  shell_cmd_pack.set_option_short_name(opt_pack_verbose, "v");
  ShellCommandId shell_cmd_pack_id = shell.add_command(
    shell_cmd_pack,
    "Run VPR pack flow using the currently loaded architecture and circuit");
  shell.set_command_class(shell_cmd_pack_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_pack_id, vpr::pack_template);
}

void add_place_command(openfpga::Shell<OpenfpgaContext>& shell,
                       ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_place("place");
  CommandOptionId opt_place_verbose = shell_cmd_place.add_option(
    "verbose", false, "enable verbose output for place stage");
  shell_cmd_place.set_option_short_name(opt_place_verbose, "v");
  ShellCommandId shell_cmd_place_id = shell.add_command(
    shell_cmd_place,
    "Run VPR place flow using the currently loaded architecture and circuit");
  shell.set_command_class(shell_cmd_place_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_place_id, vpr::place_template);
}

void add_route_command(openfpga::Shell<OpenfpgaContext>& shell,
                       ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_route("route");
  CommandOptionId opt_route_verbose = shell_cmd_route.add_option(
    "verbose", false, "enable verbose output for route stage");
  shell_cmd_route.set_option_short_name(opt_route_verbose, "v");
  ShellCommandId shell_cmd_route_id = shell.add_command(
    shell_cmd_route,
    "Run VPR route flow using the currently loaded architecture and circuit");
  shell.set_command_class(shell_cmd_route_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_route_id, vpr::route_template);
}

void add_analysis_command(openfpga::Shell<OpenfpgaContext>& shell,
                          ShellCommandClassId vpr_cmd_class) {
  Command shell_cmd_analysis("analysis");
  CommandOptionId opt_analysis_verbose = shell_cmd_analysis.add_option(
    "verbose", false, "enable verbose output for analysis stage");
  shell_cmd_analysis.set_option_short_name(opt_analysis_verbose, "v");
  ShellCommandId shell_cmd_analysis_id =
    shell.add_command(shell_cmd_analysis,
                      "Run VPR analysis flow using the currently loaded "
                      "architecture and circuit");
  shell.set_command_class(shell_cmd_analysis_id, vpr_cmd_class);
  shell.set_command_execute_function(shell_cmd_analysis_id,
                                     vpr::analysis_template);
}

void add_vpr_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId vpr_cmd_class = shell.add_command_class("VPR");
  add_vpr_macro_command(shell, vpr_cmd_class);
  add_vpr_standalone_command(shell, vpr_cmd_class);
  add_read_vpr_arch_command(shell, vpr_cmd_class);
  add_show_vpr_setup_command(shell, vpr_cmd_class);
  add_read_circuit_command(shell, vpr_cmd_class);
  add_pack_command(shell, vpr_cmd_class);
  add_place_command(shell, vpr_cmd_class);
  add_route_command(shell, vpr_cmd_class);
  add_analysis_command(shell, vpr_cmd_class);
}
}
