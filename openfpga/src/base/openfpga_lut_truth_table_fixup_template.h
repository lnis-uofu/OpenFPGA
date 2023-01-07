#ifndef OPENFPGA_LUT_TRUTH_TABLE_FIXUP_TEMPLATE_H
#define OPENFPGA_LUT_TRUTH_TABLE_FIXUP_TEMPLATE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "openfpga_context.h"
#include "openfpga_lut_truth_table_fixup.h"
#include "vtr_time.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to fix up the lut truth table results after packing is
 *done The problem comes from a mismatch between the packing results and
 * original truth tables in users' BLIF file
 * As LUT inputs are equivalent in nature, the router of packer will try
 * to swap the net mapping among these pins so as to achieve best
 * routing optimization.
 * However, it will cause the original truth table out-of-date when packing is
 *done. This function aims to fix the mess after packing so that the truth table
 * can be synchronized
 *******************************************************************/
template <class T>
int lut_truth_table_fixup_template(T& openfpga_context, const Command& cmd,
                                   const CommandContext& cmd_context) {
  vtr::ScopedStartFinishTimer timer(
    "Fix up LUT truth tables after packing optimization");

  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Apply fix-up to each packed block */
  update_lut_tt_with_post_packing_results(
    g_vpr_ctx.atom(), g_vpr_ctx.clustering(),
    openfpga_context.mutable_vpr_clustering_annotation(),
    cmd_context.option_enable(cmd, opt_verbose));

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
