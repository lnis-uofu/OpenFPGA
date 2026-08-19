#pragma once

#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "yosys_main.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the build_device_bitstream() in FPGA bitstream
 *******************************************************************/
template <class T>
int yosys_synth_template(T& openfpga_ctx, const Command& cmd,
                         const CommandContext& cmd_context) {
  CommandOptionId opt_script_file = cmd.option("file");

  return yosys_script_mode_wrapper(
      cmd_context.option_value(cmd, opt_file));
}

} // namespace openfpga ends
