#ifndef STAGE_COMMANDS_H
#define STAGE_COMMANDS_H

#include "openfpga_context.h"
#include "shell.h"

/********************************************************************
 * Commands of the flow that is run stage by stage, see staged_flow.h
 * - read_vpr_arch
 * - read_circuit
 * - pack
 * - place
 * - route
 * The OpenFPGA architecture is read by the command read_openfpga_arch
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

void add_stage_commands(openfpga::Shell<OpenfpgaContext>& shell);

} /* End namespace openfpga */

#endif
