/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of setting up OpenFPGA core engine, including:
 * - read_openfpga_arch : read OpenFPGA architecture file
 *******************************************************************/
#include "openfpga_setup_command.h"

#include "openfpga_setup_command_template.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_setup_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  add_setup_command_templates<OpenfpgaContext>(shell);
}

} /* end namespace openfpga */
