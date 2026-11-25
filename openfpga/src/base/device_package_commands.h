#ifndef DEVICE_PACKAGE_COMMANDS_H
#define DEVICE_PACKAGE_COMMANDS_H
/********************************************************************
 * The commands of the device packages: read_device_package and
 * report_device_package (docs/spec_device_package.md, R-46 and R-51)
 *******************************************************************/
#include "openfpga_context.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

void add_device_package_commands(Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
