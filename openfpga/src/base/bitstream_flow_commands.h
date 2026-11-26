#ifndef BITSTREAM_FLOW_COMMANDS_H
#define BITSTREAM_FLOW_COMMANDS_H
/********************************************************************
 * The bitstream of the flow that is run stage by stage
 * (docs/spec_bitstream.md): build_bitstream runs the steps of OpenFPGA from
 * the routing to the file of the fabric bitstream, with the recipe of the
 * device package, and keeps the link and the fabric that are still valid;
 * report_bitstream says what was made
 *******************************************************************/
#include "openfpga_context.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

void add_bitstream_flow_commands(Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
