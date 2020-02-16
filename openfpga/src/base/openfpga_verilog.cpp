/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

#include "verilog_api.h"
#include "openfpga_verilog.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the fabric_verilog function of FPGA-Verilog 
 *******************************************************************/
void write_fabric_verilog(OpenfpgaContext& openfpga_ctx,
                          const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_explicit_port_mapping = cmd.option("explicit_port_mapping");
  CommandOptionId opt_verbose = cmd.option("verbose");
  
  fpga_fabric_verilog(openfpga_ctx.module_graph(),
                      openfpga_ctx.arch().circuit_lib,
                      openfpga_ctx.mux_lib(),
                      g_vpr_ctx.device().grid,
                      openfpga_ctx.device_rr_gsb(),
                      cmd_context.option_value(cmd, opt_output_dir),
                      openfpga_ctx.flow_manager().compress_routing(),
                      cmd_context.option_enable(cmd, opt_explicit_port_mapping),
                      cmd_context.option_enable(cmd, opt_verbose));
} 

} /* end namespace openfpga */
