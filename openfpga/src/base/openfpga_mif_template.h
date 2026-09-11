#pragma once

#include <set>
#include <string>

#include "build_mif.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "mif_pipeline.h"
#include "mif_storage.h"
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
  auto& hex_map = openfpga_context.mutable_mif_pipeline().mutable_hex();
  if (hex_map.find(pb_type) != hex_map.end()) {
    VTR_LOG_ERROR(
      "read_mif: pb_type '%s' already has hex file '%s'; refusing '%s'\n",
      pb_type.c_str(), hex_map[pb_type].c_str(), mif_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  hex_map[pb_type] = mif_path;
  VTR_LOG("read_mif: registered '%s' (pb_type='%s')\n", mif_path.c_str(),
          pb_type.c_str());
  return CMD_EXEC_SUCCESS;
}

template <class T>
int write_mif_template(T& openfpga_context, const Command& cmd,
                       const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  CommandOptionId opt_circuit_model = cmd.option("circuit_model");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_verbose = cmd.option("verbose");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  /* FPGA-top unified MIF from location-map aggregation. */
  const openfpga::MifPipeline& mif_pipeline = openfpga_context.mif_pipeline();
  if (mif_pipeline.top_mif().empty()) {
    VTR_LOG_ERROR(
      "write_mif: no FPGA-top MIF; run build_architecture_bitstream "
      "first (with mif_source / mif location map)\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const std::map<std::string, std::string> port_to_model =
    collect_mif_data_port_circuit_models(
      openfpga_context.mif_location_map(),
      openfpga_context.vpr_device_annotation(),
      openfpga_context.arch().circuit_lib);
  std::set<std::string> unique_models;
  for (const auto& port_model : port_to_model) {
    unique_models.insert(port_model.second);
  }

  std::set<std::string> allowed_ports;
  if (true == cmd_context.option_enable(cmd, opt_circuit_model)) {
    const std::string& model_name =
      cmd_context.option_value(cmd, opt_circuit_model);
    const CircuitModelId model =
      openfpga_context.arch().circuit_lib.model(model_name);
    if (false == openfpga_context.arch().circuit_lib.valid_model_id(model)) {
      VTR_LOG_ERROR(
        "write_mif: unknown circuit model '%s' in the OpenFPGA "
        "architecture\n",
        model_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (unique_models.end() == unique_models.find(model_name)) {
      VTR_LOG_ERROR(
        "write_mif: circuit model '%s' has no unified MIF on this FPGA\n",
        model_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    for (const auto& port_model : port_to_model) {
      if (port_model.second == model_name) {
        allowed_ports.insert(port_model.first);
      }
    }
  } else if (1 < unique_models.size()) {
    VTR_LOG_ERROR(
      "write_mif: %zu unique circuit models have MIF data; specify "
      "--circuit_model <name>\n",
      unique_models.size());
    return CMD_EXEC_FATAL_ERROR;
  }

  return write_mif(cmd_context.option_value(cmd, opt_file),
                   mif_pipeline.top_mif(), allowed_ports,
                   !cmd_context.option_enable(cmd, opt_no_time_stamp),
                   cmd_context.option_enable(cmd, opt_verbose));
}

} /* end namespace openfpga */
