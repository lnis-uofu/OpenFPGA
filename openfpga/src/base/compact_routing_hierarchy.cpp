/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

#include "device_rr_gsb.h"
#include "device_rr_gsb_utils.h"
#include "compact_routing_hierarchy.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Identify the unique GSBs from the Device RR GSB arrays
 * This function should only be called after the GSB builder is done
 *******************************************************************/
void compact_routing_hierarchy(OpenfpgaContext& openfpga_context,
                               const Command& cmd, const CommandContext& cmd_context) { 
  vtr::ScopedStartFinishTimer timer("Identify unique General Switch Blocks (GSBs)");

  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Build unique module lists */
  openfpga_context.mutable_device_rr_gsb().build_unique_module(g_vpr_ctx.device().rr_graph);

  bool verbose_output = cmd_context.option_enable(cmd, opt_verbose);

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

} /* end namespace openfpga */
