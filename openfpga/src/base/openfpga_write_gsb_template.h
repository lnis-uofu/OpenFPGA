#ifndef OPENFPGA_WRITE_GSB_TEMPLATE_H
#define OPENFPGA_WRITE_GSB_TEMPLATE_H
/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "vtr_log.h"
#include "vtr_time.h"
#include "write_xml_device_rr_gsb.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Write internal structrure of all the General Switch Blocks (GSBs)
 * to an XML file
 *******************************************************************/
template <class T>
int write_gsb_template(const T& openfpga_ctx, const Command& cmd,
                       const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  CommandOptionId opt_unique = cmd.option("unique");
  CommandOptionId opt_exclude_rr_info = cmd.option("exclude_rr_info");
  CommandOptionId opt_exclude = cmd.option("exclude");
  CommandOptionId opt_gsb_names = cmd.option("gsb_names");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Build the options for the writer */
  RRGSBWriterOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_file));
  options.set_unique_module_only(cmd_context.option_enable(cmd, opt_unique));
  options.set_exclude_rr_info(
    cmd_context.option_enable(cmd, opt_exclude_rr_info));
  options.set_exclude_content(cmd_context.option_value(cmd, opt_exclude));
  options.set_include_gsb_names(cmd_context.option_value(cmd, opt_gsb_names));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));

  if (!options.valid()) {
    VTR_LOG("Detected errors when parsing options!\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  write_device_rr_gsb_to_xml(
    g_vpr_ctx.device().grid, openfpga_ctx.vpr_device_annotation(),
    g_vpr_ctx.device().rr_graph, openfpga_ctx.device_rr_gsb(), options);

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
