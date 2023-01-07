#ifndef OPENFPGA_PB_PIN_FIXUP_TEMPLATE_H
#define OPENFPGA_PB_PIN_FIXUP_TEMPLATE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "openfpga_pb_pin_fixup.h"
#include "vtr_time.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to fix up the pb pin mapping results
 * The problem comes from a mismatch between the packing and routing results
 * When there are equivalent input/output for any grids, router will try
 * to swap the net mapping among these pins so as to achieve best
 * routing optimization.
 * However, it will cause the packing results out-of-date as the net mapping
 * of each grid remain untouched once packing is done.
 * This function aims to fix the mess after routing so that the net mapping
 * can be synchronized
 *******************************************************************/
template <class T>
int pb_pin_fixup_template(T& openfpga_context, const Command& cmd,
                          const CommandContext& cmd_context) {
  vtr::ScopedStartFinishTimer timer(
    "Fix up pb pin mapping results after routing optimization");

  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Apply fix-up to each grid */
  update_pb_pin_with_post_routing_results(
    g_vpr_ctx.device(), g_vpr_ctx.clustering(), g_vpr_ctx.placement(),
    openfpga_context.vpr_routing_annotation(),
    openfpga_context.mutable_vpr_clustering_annotation(),
    cmd_context.option_enable(cmd, opt_verbose));

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
