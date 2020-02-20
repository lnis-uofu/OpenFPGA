/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

#include "verilog_api.h"
#include "repack.h"
#include "openfpga_repack.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the fabric_verilog function of FPGA-Verilog 
 *******************************************************************/
void repack(OpenfpgaContext& openfpga_ctx,
            const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_verbose = cmd.option("verbose");

  pack_physical_pbs(g_vpr_ctx.device(),
                    openfpga_ctx.mutable_vpr_device_annotation(),
                    openfpga_ctx.mutable_vpr_clustering_annotation(),
                    openfpga_ctx.vpr_routing_annotation(),
                    cmd_context.option_enable(cmd, opt_verbose));
} 

} /* end namespace openfpga */
