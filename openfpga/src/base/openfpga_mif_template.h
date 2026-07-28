#pragma once

#include <string>

#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "read_mif.h"
#include "shell.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_mif.h"

/* begin namespace openfpga */
namespace openfpga {

template <class T>
int read_mif_template(T& openfpga_context, const Command& cmd,
                      const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  const std::string& mif_path = cmd_context.option_value(cmd, opt_file);
  const int exec_status =
    read_mif(mif_path, openfpga_context.mutable_mif_storage());
  if (CMD_EXEC_SUCCESS != exec_status) {
    return exec_status;
  }
  VTR_LOG("read_mif: read '%s'\n", mif_path.c_str());
  return CMD_EXEC_SUCCESS;
}

template <class T>
int write_mif_template(T& openfpga_context, const Command& cmd,
                       const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  /* Aggregated preload result from build_architecture_bitstream / aggregate. */
  const MifStorage& aggregated_mif_storage =
    openfpga_context.aggregated_mif_storage();
  if (aggregated_mif_storage.empty()) {
    VTR_LOG_ERROR(
      "write_mif: no aggregated MIF data; run build_architecture_bitstream "
      "first\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const int exec_status =
    write_mif(cmd_context.option_value(cmd, opt_file), aggregated_mif_storage);
  if (CMD_EXEC_SUCCESS == exec_status) {
    VTR_LOG("write_mif: wrote '%s'\n",
            cmd_context.option_value(cmd, opt_file).c_str());
  }
  return exec_status;
}

} /* end namespace openfpga */
