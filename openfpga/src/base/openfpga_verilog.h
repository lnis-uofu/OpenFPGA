#ifndef OPENFPGA_VERILOG_H
#define OPENFPGA_VERILOG_H

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

int write_fabric_verilog(OpenfpgaContext& openfpga_ctx,
                         const Command& cmd, const CommandContext& cmd_context); 

int write_verilog_testbench(OpenfpgaContext& openfpga_ctx,
                            const Command& cmd, const CommandContext& cmd_context); 

int write_full_testbench(OpenfpgaContext& openfpga_ctx,
                         const Command& cmd, const CommandContext& cmd_context); 

} /* end namespace openfpga */

#endif
