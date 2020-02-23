/********************************************************************
 * This file includes functions to build bitstream database
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

#include "build_device_bitstream.h"
#include "openfpga_bitstream.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the fabric_verilog function of FPGA-Verilog 
 *******************************************************************/
void fpga_bitstream(OpenfpgaContext& openfpga_ctx,
                    const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_verbose = cmd.option("verbose");

  openfpga_ctx.mutable_bitstream_manager() = build_device_bitstream(g_vpr_ctx, openfpga_ctx, cmd_context.option_enable(cmd, opt_verbose));
} 

} /* end namespace openfpga */
