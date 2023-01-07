#ifndef OPENFPGA_REPACK_TEMPLATE_H
#define OPENFPGA_REPACK_TEMPLATE_H

/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
#include "build_physical_truth_table.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "read_xml_repack_design_constraints.h"
#include "repack.h"
#include "repack_design_constraints.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the fabric_verilog function of FPGA-Verilog
 *******************************************************************/
template <class T>
int repack_template(T& openfpga_ctx, const Command& cmd,
                    const CommandContext& cmd_context) {
  CommandOptionId opt_design_constraints = cmd.option("design_constraints");
  CommandOptionId opt_ignore_global_nets =
    cmd.option("ignore_global_nets_on_pins");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Load design constraints from file */
  RepackDesignConstraints repack_design_constraints;
  if (true == cmd_context.option_enable(cmd, opt_design_constraints)) {
    std::string dc_fname =
      cmd_context.option_value(cmd, opt_design_constraints);
    VTR_ASSERT(false == dc_fname.empty());
    repack_design_constraints =
      read_xml_repack_design_constraints(dc_fname.c_str());
  }

  /* Setup repacker options */
  RepackOption options;
  options.set_design_constraints(repack_design_constraints);
  options.set_ignore_global_nets_on_pins(
    cmd_context.option_value(cmd, opt_ignore_global_nets));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));

  if (!options.valid()) {
    VTR_LOG("Detected errors when parsing options!\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  pack_physical_pbs(g_vpr_ctx.device(), g_vpr_ctx.atom(),
                    g_vpr_ctx.clustering(),
                    openfpga_ctx.mutable_vpr_device_annotation(),
                    openfpga_ctx.mutable_vpr_clustering_annotation(),
                    openfpga_ctx.vpr_bitstream_annotation(),
                    openfpga_ctx.arch().circuit_lib, options);

  build_physical_lut_truth_tables(
    openfpga_ctx.mutable_vpr_clustering_annotation(), g_vpr_ctx.atom(),
    g_vpr_ctx.clustering(), openfpga_ctx.vpr_device_annotation(),
    openfpga_ctx.arch().circuit_lib, options.verbose_output());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
