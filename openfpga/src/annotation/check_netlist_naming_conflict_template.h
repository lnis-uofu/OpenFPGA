#ifndef CHECK_NETLIST_NAMING_CONFLICT_TEMPLATE_H
#define CHECK_NETLIST_NAMING_CONFLICT_TEMPLATE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "check_netlist_naming_conflict.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "vtr_time.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to detect and correct any naming
 * in the users' BLIF netlist that violates the syntax of OpenFPGA
 * fabric generator, i.e., Verilog generator and SPICE generator
 *******************************************************************/
template <class T>
int check_netlist_naming_conflict_template(T& openfpga_context,
                                           const Command& cmd,
                                           const CommandContext& cmd_context) {
  vtr::ScopedStartFinishTimer timer(
    "Check naming violations of netlist blocks and nets");

  /* By default, we replace all the illegal characters with '_' */
  std::string sensitive_chars(".,:;\'\"+-<>()[]{}!@#$%^&*~`?/");
  std::string fix_chars("____________________________");

  CommandOptionId opt_fix = cmd.option("fix");

  /* Do the main job first: detect any naming in the BLIF netlist that violates
   * the syntax */
  if (false == cmd_context.option_enable(cmd, opt_fix)) {
    size_t num_conflicts = detect_netlist_naming_conflict(
      g_vpr_ctx.atom().netlist(), sensitive_chars);
    VTR_LOGV_ERROR(
      (0 < num_conflicts && (false == cmd_context.option_enable(cmd, opt_fix))),
      "Found %ld naming conflicts in the netlist. Please correct so as to use "
      "any fabric generators.\n",
      num_conflicts);
    VTR_LOGV(0 == num_conflicts,
             "Check naming conflicts in the netlist passed.\n");

    /* If we see conflicts, report minor error */
    if (0 < num_conflicts) {
      return CMD_EXEC_MINOR_ERROR;
    }

    /* Otherwise, we should see zero conflicts */
    VTR_ASSERT(0 == num_conflicts);
    return CMD_EXEC_SUCCESS;
  }

  /* If the auto correction is enabled, we apply a fix */
  if (true == cmd_context.option_enable(cmd, opt_fix)) {
    fix_netlist_naming_conflict(
      g_vpr_ctx.atom().netlist(), sensitive_chars, fix_chars,
      openfpga_context.mutable_vpr_netlist_annotation());

    CommandOptionId opt_report = cmd.option("report");
    if (true == cmd_context.option_enable(cmd, opt_report)) {
      print_netlist_naming_fix_report(
        cmd_context.option_value(cmd, opt_report), g_vpr_ctx.atom().netlist(),
        openfpga_context.vpr_netlist_annotation());
      VTR_LOG("Naming fix-up report is generated to file '%s'\n",
              cmd_context.option_value(cmd, opt_report).c_str());
    }
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
