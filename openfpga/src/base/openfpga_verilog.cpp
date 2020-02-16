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
  CommandOptionId opt_include_timing = cmd.option("include_timing");
  CommandOptionId opt_include_signal_init = cmd.option("include_signal_init");
  CommandOptionId opt_support_icarus_simulator = cmd.option("support_icarus_simulator");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the FPGA-Verilog
   * Keep it independent from any other outside data structures
   */
  FabricVerilogOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_explicit_port_mapping(cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_include_timing(cmd_context.option_enable(cmd, opt_include_timing));
  options.set_include_signal_init(cmd_context.option_enable(cmd, opt_include_signal_init));
  options.set_support_icarus_simulator(cmd_context.option_enable(cmd, opt_support_icarus_simulator));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_compress_routing(openfpga_ctx.flow_manager().compress_routing());
  
  fpga_fabric_verilog(openfpga_ctx.module_graph(),
                      openfpga_ctx.arch().circuit_lib,
                      openfpga_ctx.mux_lib(),
                      g_vpr_ctx.device().grid,
                      openfpga_ctx.device_rr_gsb(),
                      options);
} 

} /* end namespace openfpga */
