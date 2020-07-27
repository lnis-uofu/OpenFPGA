#ifndef OPENFPGA_BITSTREAM_H
#define OPENFPGA_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int fpga_bitstream(OpenfpgaContext& openfpga_ctx,
                   const Command& cmd, const CommandContext& cmd_context); 

int build_fabric_bitstream(OpenfpgaContext& openfpga_ctx,
                           const Command& cmd, const CommandContext& cmd_context);

int write_fabric_bitstream(const OpenfpgaContext& openfpga_ctx,
                           const Command& cmd, const CommandContext& cmd_context);

} /* end namespace openfpga */

#endif
