#pragma once

#include <string>

#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "mif_pipeline.h"
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
  CommandOptionId opt_pb_type = cmd.option("pb_type");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_pb_type));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_pb_type).empty());

  const std::string& mif_path = cmd_context.option_value(cmd, opt_file);
  const std::string& pb_type = cmd_context.option_value(cmd, opt_pb_type);
  const int exec_status = read_mif_from_init_hex(
    mif_path, openfpga_context.mutable_mif_pipeline().mutable_hex(), pb_type);
  if (CMD_EXEC_SUCCESS != exec_status) {
    return exec_status;
  }
  VTR_LOG("read_mif: read '%s' (pb_type='%s')\n", mif_path.c_str(),
          pb_type.c_str());
  return CMD_EXEC_SUCCESS;
}

template <class T>
int write_mif_template(T& openfpga_context, const Command& cmd,
                       const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  /* FPGA-top unified MIF when location-map aggregation ran; else flatten
   * every placed physical MIF from build_architecture_bitstream. */
  const openfpga::MifPipeline& mif_pipeline = openfpga_context.mif_pipeline();
  const openfpga::MifStorage* aggregated_mif_storage = nullptr;
  openfpga::MifStorage physical_fallback;
  if (!mif_pipeline.top_mif().empty()) {
    aggregated_mif_storage = &mif_pipeline.top_mif();
  } else {
    physical_fallback = mif_pipeline.copy_all_physical_mifs();
    if (!physical_fallback.empty()) {
      aggregated_mif_storage = &physical_fallback;
    }
  }
  if (nullptr == aggregated_mif_storage) {
    VTR_LOG_ERROR(
      "write_mif: no aggregated MIF data; run build_architecture_bitstream "
      "first (with mif_source / logical MIF)\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const int exec_status =
    write_mif(cmd_context.option_value(cmd, opt_file), *aggregated_mif_storage);
  if (CMD_EXEC_SUCCESS == exec_status) {
    VTR_LOG("write_mif: wrote '%s'\n",
            cmd_context.option_value(cmd, opt_file).c_str());
  }
  return exec_status;
}

} /* end namespace openfpga */
