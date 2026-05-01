/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of generate SDC files
 * - write_pnr_sdc : generate SDC to constrain the back-end flow for FPGA fabric
 * - write_analysis_sdc: TODO: generate SDC based on users' implementations
 *******************************************************************/
#include "openfpga_sdc_command.h"

#include "openfpga_sdc_command_template.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_sdc_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  add_openfpga_sdc_command_templates<OpenfpgaContext>(shell);
}

} /* end namespace openfpga */
