/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "spice_api.h"
#include "openfpga_spice.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the fabric SPICE generator of FPGA-SPICE
 *******************************************************************/
int write_fabric_spice(OpenfpgaContext& openfpga_ctx,
                         const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_explicit_port_mapping = cmd.option("explicit_port_mapping");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the FPGA-SPICE
   * Keep it independent from any other outside data structures
   */
  FabricSpiceOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_explicit_port_mapping(cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_compress_routing(openfpga_ctx.flow_manager().compress_routing());
  
  int status = CMD_EXEC_SUCCESS;
  status = fpga_fabric_spice(openfpga_ctx.module_graph(),
                             openfpga_ctx.mutable_spice_netlists(),
                             openfpga_ctx.arch(),
                             options);

  return status;
} 

} /* end namespace openfpga */
