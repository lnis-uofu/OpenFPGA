/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of setting up OpenFPGA core engine, including:
 * - read_openfpga_arch : read OpenFPGA architecture file
 *******************************************************************/
#include "openfpga_constrain_pin_location_command.h"
#include "openfpga_constrain_pin_location.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: constrain_pin_location
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
void add_openfpga_constrain_pin_location_command(openfpga::Shell<OpenfpgaContext>& shell) {
  Command shell_cmd("constrain_pin_location");

  /* Options are created by following the python script crafted by QL, since that has been used for a while:
       python3 create_ioplace.py --pcf “pcf_file” --blif “blif_file” --net “net_file” --pinmap_xml “pinmap_xml_file” --csv_file “csv_file” --output “outputplace_file” 
  */
  /* Add an option '--pcf'*/
  CommandOptionId opt_pcf_file = shell_cmd.add_option("pcf", true, "file path to the user pin constraint");
  shell_cmd.set_option_require_value(opt_pcf_file, openfpga::OPT_STRING);

  /* Add an option '--blif'*/
  CommandOptionId opt_blif_file = shell_cmd.add_option("blif", true, "file path to the synthesized blif");
  shell_cmd.set_option_require_value(opt_blif_file, openfpga::OPT_STRING);

  /* Add an option '--pinmap_xml'*/
  CommandOptionId opt_pinmap_file = shell_cmd.add_option("pinmap_xml", true, "file path to the pin location XML");
  shell_cmd.set_option_require_value(opt_pinmap_file, openfpga::OPT_STRING);

  /* Add an option '--csv_file'*/
  CommandOptionId opt_csv_file = shell_cmd.add_option("csv_file", true, "file path to the pin map csv");
  shell_cmd.set_option_require_value(opt_csv_file, openfpga::OPT_STRING);

  /* Add an option '--output'*/
  CommandOptionId opt_output_file = shell_cmd.add_option("output", true, "file path to the output");
  shell_cmd.set_option_require_value(opt_output_file, openfpga::OPT_STRING);

  /* Add command 'constrain_pin_location' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "constrain pin location by generating a place file");
  shell.set_command_execute_function(shell_cmd_id, constrain_pin_location);

}

} /* end namespace openfpga */
