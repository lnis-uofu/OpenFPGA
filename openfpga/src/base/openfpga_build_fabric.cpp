/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

#include "device_rr_gsb.h"
#include "device_rr_gsb_utils.h"
#include "build_device_module.h"
#include "openfpga_build_fabric.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Identify the unique GSBs from the Device RR GSB arrays
 * This function should only be called after the GSB builder is done
 *******************************************************************/
static 
void compress_routing_hierarchy(OpenfpgaContext& openfpga_context,
                                const bool& verbose_output) {
  vtr::ScopedStartFinishTimer timer("Identify unique General Switch Blocks (GSBs)");

  /* Build unique module lists */
  openfpga_context.mutable_device_rr_gsb().build_unique_module(g_vpr_ctx.device().rr_graph);

  /* Report the stats */
  VTR_LOGV(verbose_output, 
           "Detected %lu unique X-direction connection blocks from a total of %d (compression rate=%d%)\n",
           openfpga_context.device_rr_gsb().get_num_cb_unique_module(CHANX),
           find_device_rr_gsb_num_cb_modules(openfpga_context.device_rr_gsb(), CHANX),
           100 * (openfpga_context.device_rr_gsb().get_num_cb_unique_module(CHANX) / find_device_rr_gsb_num_cb_modules(openfpga_context.device_rr_gsb(), CHANX) - 1));

  VTR_LOGV(verbose_output,
           "Detected %lu unique Y-direction connection blocks from a total of %d (compression rate=%d%)\n",
           openfpga_context.device_rr_gsb().get_num_cb_unique_module(CHANY),
           find_device_rr_gsb_num_cb_modules(openfpga_context.device_rr_gsb(), CHANY),
           100 * (openfpga_context.device_rr_gsb().get_num_cb_unique_module(CHANY) / find_device_rr_gsb_num_cb_modules(openfpga_context.device_rr_gsb(), CHANY) - 1));

  VTR_LOGV(verbose_output,
           "Detected %lu unique switch blocks from a total of %d (compression rate=%d%)\n",
           openfpga_context.device_rr_gsb().get_num_sb_unique_module(),
           find_device_rr_gsb_num_sb_modules(openfpga_context.device_rr_gsb()),
           100 * (openfpga_context.device_rr_gsb().get_num_sb_unique_module() / find_device_rr_gsb_num_sb_modules(openfpga_context.device_rr_gsb()) - 1));

  VTR_LOGV(verbose_output,
           "Detected %lu unique general switch blocks from a total of %d (compression rate=%d%)\n",
           openfpga_context.device_rr_gsb().get_num_gsb_unique_module(),
           find_device_rr_gsb_num_gsb_modules(openfpga_context.device_rr_gsb()),
           100 * (openfpga_context.device_rr_gsb().get_num_gsb_unique_module() / find_device_rr_gsb_num_gsb_modules(openfpga_context.device_rr_gsb()) - 1));
}

/********************************************************************
 * Build the module graph for FPGA device
 *******************************************************************/
void build_fabric(OpenfpgaContext& openfpga_context,
                  const Command& cmd, const CommandContext& cmd_context) { 

  CommandOptionId opt_compress_routing = cmd.option("compress_routing");
  CommandOptionId opt_duplicate_grid_pin = cmd.option("duplicate_grid_pin");
  CommandOptionId opt_verbose = cmd.option("verbose");
  
  if (true == cmd_context.option_enable(cmd, opt_compress_routing)) {
    compress_routing_hierarchy(openfpga_context, cmd_context.option_enable(cmd, opt_verbose));
  }

  VTR_LOG("\n");

  openfpga_context.mutable_module_graph() = build_device_module_graph(g_vpr_ctx.device(),
                                                                      const_cast<const OpenfpgaContext&>(openfpga_context),
                                                                      cmd_context.option_enable(cmd, opt_duplicate_grid_pin));
} 

} /* end namespace openfpga */
