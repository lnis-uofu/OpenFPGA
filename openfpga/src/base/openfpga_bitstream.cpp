/********************************************************************
 * This file includes functions to build bitstream database
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from fpgabitstream library */
#include "read_xml_arch_bitstream.h"
#include "write_xml_arch_bitstream.h"

#include "build_device_bitstream.h"
#include "fabric_bitstream_writer.h"
#include "build_fabric_bitstream.h"
#include "openfpga_bitstream.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the build_device_bitstream() in FPGA bitstream
 *******************************************************************/
int fpga_bitstream(OpenfpgaContext& openfpga_ctx,
                   const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_write_file = cmd.option("write_file");
  CommandOptionId opt_read_file = cmd.option("read_file");

  if (true == cmd_context.option_enable(cmd, opt_read_file)) {
    openfpga_ctx.mutable_bitstream_manager() = read_xml_architecture_bitstream(cmd_context.option_value(cmd, opt_write_file).c_str());
  } else {
    openfpga_ctx.mutable_bitstream_manager() = build_device_bitstream(g_vpr_ctx,
                                                                      openfpga_ctx,
                                                                      cmd_context.option_enable(cmd, opt_verbose));
  }

  if (true == cmd_context.option_enable(cmd, opt_write_file)) {
    std::string src_dir_path = find_path_dir_name(cmd_context.option_value(cmd, opt_write_file));

    /* Create directories */
    create_directory(src_dir_path);

    write_xml_architecture_bitstream(openfpga_ctx.bitstream_manager(),
                                     cmd_context.option_value(cmd, opt_write_file));
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A wrapper function to call the build_fabric_bitstream() in FPGA bitstream
 *******************************************************************/
int build_fabric_bitstream(OpenfpgaContext& openfpga_ctx,
                           const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_file = cmd.option("file");

  /* Build fabric bitstream here */
  openfpga_ctx.mutable_fabric_bitstream() = build_fabric_dependent_bitstream(openfpga_ctx.bitstream_manager(),
                                                                             openfpga_ctx.module_graph(),
                                                                             openfpga_ctx.arch().config_protocol,
                                                                             cmd_context.option_enable(cmd, opt_verbose));

  /* Write fabric bitstream if required */
  if (true == cmd_context.option_enable(cmd, opt_file)) {
    std::string src_dir_path = find_path_dir_name(cmd_context.option_value(cmd, opt_file));

    /* Create directories */
    create_directory(src_dir_path);

    write_fabric_bitstream_to_text_file(openfpga_ctx.bitstream_manager(),
                                        openfpga_ctx.fabric_bitstream(),
                                        cmd_context.option_value(cmd, opt_file));
  }
  
  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
} 

} /* end namespace openfpga */
