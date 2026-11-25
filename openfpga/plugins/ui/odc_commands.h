#ifndef ODC_COMMANDS_H
#define ODC_COMMANDS_H
/********************************************************************
 * The commands of ODC, the constraints file of OpenFPGA
 * (docs/spec_odc.md, R-40 to R-47): read_odc, write_odc, clear_odc and
 * report_constraints, and the hooks that keep the files of the tools up to
 * date and check the constraints before VPR makes the device.
 *******************************************************************/
#include "openfpga_context.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

void add_odc_commands(Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
