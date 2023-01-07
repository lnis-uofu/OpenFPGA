#include "openfpga_spice_command.h"

#include "openfpga_spice_command_template.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_spice_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  add_spice_command_templates<OpenfpgaContext>(shell);
}

} /* end namespace openfpga */
