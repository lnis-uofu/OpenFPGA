/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "write_xml_device_rr_gsb.h"

#include "openfpga_write_gsb.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Write internal structrure of all the General Switch Blocks (GSBs)
 * to an XML file 
 *******************************************************************/
int write_gsb(const OpenfpgaContext& openfpga_ctx,
              const Command& cmd, const CommandContext& cmd_context) { 

  /* Check the option '--file' is enabled or not 
   * Actually, it must be enabled as the shell interface will check 
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  CommandOptionId opt_unique = cmd.option("unique");
  CommandOptionId opt_verbose = cmd.option("verbose");

  std::string sb_file_name = cmd_context.option_value(cmd, opt_file);

  write_device_rr_gsb_to_xml(sb_file_name.c_str(),
                             g_vpr_ctx.device().grid,
                             openfpga_ctx.vpr_device_annotation(),
                             g_vpr_ctx.device().rr_graph,
                             openfpga_ctx.device_rr_gsb(),
                             cmd_context.option_enable(cmd, opt_unique),
                             cmd_context.option_enable(cmd, opt_verbose));

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
} 

} /* end namespace openfpga */
