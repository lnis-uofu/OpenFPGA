/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "verilog_api.h"
#include "openfpga_verilog.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the fabric Verilog generator of FPGA-Verilog 
 *******************************************************************/
int write_fabric_verilog(OpenfpgaContext& openfpga_ctx,
                         const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_explicit_port_mapping = cmd.option("explicit_port_mapping");
  CommandOptionId opt_include_timing = cmd.option("include_timing");
  CommandOptionId opt_include_signal_init = cmd.option("include_signal_init");
  CommandOptionId opt_support_icarus_simulator = cmd.option("support_icarus_simulator");
  CommandOptionId opt_print_user_defined_template = cmd.option("print_user_defined_template");
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
  options.set_print_user_defined_template(cmd_context.option_enable(cmd, opt_print_user_defined_template));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_compress_routing(openfpga_ctx.flow_manager().compress_routing());
  
  fpga_fabric_verilog(openfpga_ctx.mutable_module_graph(),
                      openfpga_ctx.mutable_verilog_netlists(),
                      openfpga_ctx.arch().circuit_lib,
                      openfpga_ctx.mux_lib(),
                      openfpga_ctx.decoder_lib(),
                      g_vpr_ctx.device(),
                      openfpga_ctx.vpr_device_annotation(),
                      openfpga_ctx.device_rr_gsb(),
                      options);

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
} 

/********************************************************************
 * A wrapper function to call the Verilog testbench generator of FPGA-Verilog 
 *******************************************************************/
int write_verilog_testbench(OpenfpgaContext& openfpga_ctx,
                            const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_reference_benchmark = cmd.option("reference_benchmark_file_path");
  CommandOptionId opt_print_top_testbench = cmd.option("print_top_testbench");
  CommandOptionId opt_fast_configuration = cmd.option("fast_configuration");
  CommandOptionId opt_print_formal_verification_top_netlist = cmd.option("print_formal_verification_top_netlist");
  CommandOptionId opt_print_preconfig_top_testbench = cmd.option("print_preconfig_top_testbench");
  CommandOptionId opt_print_simulation_ini = cmd.option("print_simulation_ini");
  CommandOptionId opt_explicit_port_mapping = cmd.option("explicit_port_mapping");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the FPGA-Verilog
   * Keep it independent from any other outside data structures
   */
  VerilogTestbenchOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_reference_benchmark_file_path(cmd_context.option_value(cmd, opt_reference_benchmark));
  options.set_print_formal_verification_top_netlist(cmd_context.option_enable(cmd, opt_print_formal_verification_top_netlist));
  options.set_print_preconfig_top_testbench(cmd_context.option_enable(cmd, opt_print_preconfig_top_testbench));
  options.set_fast_configuration(cmd_context.option_enable(cmd, opt_fast_configuration));
  options.set_print_top_testbench(cmd_context.option_enable(cmd, opt_print_top_testbench));
  options.set_print_simulation_ini(cmd_context.option_value(cmd, opt_print_simulation_ini));
  options.set_explicit_port_mapping(cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  
  fpga_verilog_testbench(openfpga_ctx.module_graph(),
                         openfpga_ctx.bitstream_manager(),
                         openfpga_ctx.fabric_bitstream(),
                         g_vpr_ctx.atom(),
                         g_vpr_ctx.placement(),
                         openfpga_ctx.io_location_map(),
                         openfpga_ctx.vpr_netlist_annotation(),
                         openfpga_ctx.arch().circuit_lib,
                         openfpga_ctx.simulation_setting(),
                         openfpga_ctx.arch().config_protocol.type(),
                         options);

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
} 

} /* end namespace openfpga */
