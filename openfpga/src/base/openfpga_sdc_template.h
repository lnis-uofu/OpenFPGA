#ifndef OPENFPGA_SDC_TEMPLATE_H
#define OPENFPGA_SDC_TEMPLATE_H
/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
#include "analysis_sdc_writer.h"
#include "circuit_library_utils.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "configuration_chain_sdc_writer.h"
#include "configure_port_sdc_writer.h"
#include "globals.h"
#include "openfpga_digest.h"
#include "openfpga_scale.h"
#include "pnr_sdc_writer.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the PnR SDC generator of FPGA-SDC
 *******************************************************************/
template <class T>
int write_pnr_sdc_template(const T& openfpga_ctx, const Command& cmd,
                           const CommandContext& cmd_context) {
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_flatten_names = cmd.option("flatten_names");
  CommandOptionId opt_hierarchical = cmd.option("hierarchical");
  CommandOptionId opt_time_unit = cmd.option("time_unit");
  CommandOptionId opt_output_hierarchy = cmd.option("output_hierarchy");
  CommandOptionId opt_constrain_global_port =
    cmd.option("constrain_global_port");
  CommandOptionId opt_constrain_non_clock_global_port =
    cmd.option("constrain_non_clock_global_port");
  CommandOptionId opt_constrain_grid = cmd.option("constrain_grid");
  CommandOptionId opt_constrain_sb = cmd.option("constrain_sb");
  CommandOptionId opt_constrain_cb = cmd.option("constrain_cb");
  CommandOptionId opt_constrain_configurable_memory_outputs =
    cmd.option("constrain_configurable_memory_outputs");
  CommandOptionId opt_constrain_routing_multiplexer_outputs =
    cmd.option("constrain_routing_multiplexer_outputs");
  CommandOptionId opt_constrain_switch_block_outputs =
    cmd.option("constrain_switch_block_outputs");
  CommandOptionId opt_constrain_zero_delay_paths =
    cmd.option("constrain_zero_delay_paths");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-SDC Keep it independent from any other outside data structures
   */
  std::string sdc_dir_path =
    format_dir_path(cmd_context.option_value(cmd, opt_output_dir));

  /* Create directories */
  create_directory(sdc_dir_path);

  PnrSdcOption options(sdc_dir_path);

  options.set_flatten_names(cmd_context.option_enable(cmd, opt_flatten_names));
  options.set_hierarchical(cmd_context.option_enable(cmd, opt_hierarchical));

  if (true == cmd_context.option_enable(cmd, opt_time_unit)) {
    options.set_time_unit(
      string_to_time_unit(cmd_context.option_value(cmd, opt_time_unit)));
  }

  options.set_output_hierarchy(
    cmd_context.option_enable(cmd, opt_output_hierarchy));

  options.set_constrain_global_port(
    cmd_context.option_enable(cmd, opt_constrain_global_port));
  options.set_constrain_non_clock_global_port(
    cmd_context.option_enable(cmd, opt_constrain_non_clock_global_port));
  options.set_constrain_grid(
    cmd_context.option_enable(cmd, opt_constrain_grid));
  options.set_constrain_sb(cmd_context.option_enable(cmd, opt_constrain_sb));
  options.set_constrain_cb(cmd_context.option_enable(cmd, opt_constrain_cb));
  options.set_constrain_configurable_memory_outputs(
    cmd_context.option_enable(cmd, opt_constrain_configurable_memory_outputs));
  options.set_constrain_routing_multiplexer_outputs(
    cmd_context.option_enable(cmd, opt_constrain_routing_multiplexer_outputs));
  options.set_constrain_switch_block_outputs(
    cmd_context.option_enable(cmd, opt_constrain_switch_block_outputs));
  options.set_constrain_zero_delay_paths(
    cmd_context.option_enable(cmd, opt_constrain_zero_delay_paths));
  options.set_time_stamp(!cmd_context.option_enable(cmd, opt_no_time_stamp));

  /* We first turn on default sdc option and then disable part of them by
   * following users' options */
  if (false == options.generate_sdc_pnr()) {
    options.set_generate_sdc_pnr(true);
  }

  /* Execute only when sdc is enabled */
  if (true == options.generate_sdc_pnr()) {
    return print_pnr_sdc(
      options, g_vpr_ctx.device(), openfpga_ctx.vpr_device_annotation(),
      openfpga_ctx.fabric_tile(), openfpga_ctx.device_rr_gsb(),
      openfpga_ctx.module_graph(), openfpga_ctx.mux_lib(),
      openfpga_ctx.arch().circuit_lib, openfpga_ctx.fabric_global_port_info(),
      openfpga_ctx.simulation_setting(),
      openfpga_ctx.flow_manager().compress_routing());
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A wrapper function to call the PnR SDC generator on configuration chain
 * of FPGA-SDC
 *******************************************************************/
template <class T>
int write_configuration_chain_sdc_template(const T& openfpga_ctx,
                                           const Command& cmd,
                                           const CommandContext& cmd_context) {
  /* If the configuration protocol is not a configuration chain, we will not
   * write anything */
  if (CONFIG_MEM_SCAN_CHAIN != openfpga_ctx.arch().config_protocol.type()) {
    VTR_LOGF_ERROR(
      __FILE__, __LINE__,
      "Configuration protocol is %s. Expected %s to write SDC!\n",
      CONFIG_PROTOCOL_TYPE_STRING[openfpga_ctx.arch().config_protocol.type()],
      CONFIG_PROTOCOL_TYPE_STRING[CONFIG_MEM_SCAN_CHAIN]);
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Get command options */
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_time_unit = cmd.option("time_unit");
  CommandOptionId opt_min_delay = cmd.option("min_delay");
  CommandOptionId opt_max_delay = cmd.option("max_delay");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");

  std::string sdc_dir_path =
    format_dir_path(cmd_context.option_value(cmd, opt_output_dir));

  float time_unit =
    string_to_time_unit(cmd_context.option_value(cmd, opt_time_unit));

  /* Write the SDC for configuration chain */
  print_pnr_sdc_constrain_configurable_chain(
    cmd_context.option_value(cmd, opt_output_dir), time_unit,
    std::stof(cmd_context.option_value(cmd, opt_max_delay)),
    std::stof(cmd_context.option_value(cmd, opt_min_delay)),
    !cmd_context.option_enable(cmd, opt_no_time_stamp),
    openfpga_ctx.module_graph());

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A wrapper function to call the PnR SDC generator on routing multiplexers
 * of FPGA-SDC
 *******************************************************************/
template <class T>
int write_sdc_disable_timing_configure_ports_template(
  const T& openfpga_ctx, const Command& cmd,
  const CommandContext& cmd_context) {
  /* Get command options */
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_flatten_names = cmd.option("flatten_names");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_verbose = cmd.option("verbose");

  std::string sdc_dir_path =
    format_dir_path(cmd_context.option_value(cmd, opt_output_dir));

  /* Write the SDC for configuration chain */
  if (CMD_EXEC_FATAL_ERROR ==
      print_sdc_disable_timing_configure_ports(
        cmd_context.option_value(cmd, opt_output_dir),
        cmd_context.option_enable(cmd, opt_flatten_names),
        openfpga_ctx.mux_lib(), openfpga_ctx.arch().circuit_lib,
        openfpga_ctx.module_graph(),
        !cmd_context.option_enable(cmd, opt_no_time_stamp),
        cmd_context.option_enable(cmd, opt_verbose))) {
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A wrapper function to call the analysis SDC generator of FPGA-SDC
 *******************************************************************/
template <class T>
int write_analysis_sdc_template(const T& openfpga_ctx, const Command& cmd,
                                const CommandContext& cmd_context) {
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_flatten_names = cmd.option("flatten_names");
  CommandOptionId opt_time_unit = cmd.option("time_unit");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-SDC Keep it independent from any other outside data structures
   */
  std::string sdc_dir_path =
    format_dir_path(cmd_context.option_value(cmd, opt_output_dir));

  /* Create directories */
  create_directory(sdc_dir_path);

  AnalysisSdcOption options(sdc_dir_path);
  options.set_generate_sdc_analysis(true);
  options.set_flatten_names(cmd_context.option_enable(cmd, opt_flatten_names));
  options.set_time_stamp(!cmd_context.option_enable(cmd, opt_no_time_stamp));

  if (true == cmd_context.option_enable(cmd, opt_time_unit)) {
    options.set_time_unit(
      string_to_time_unit(cmd_context.option_value(cmd, opt_time_unit)));
  }

  if (true == options.generate_sdc_analysis()) {
    print_analysis_sdc(
      options,
      1. /
        openfpga_ctx.simulation_setting().default_operating_clock_frequency(),
      g_vpr_ctx, openfpga_ctx, openfpga_ctx.flow_manager().compress_routing());
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
