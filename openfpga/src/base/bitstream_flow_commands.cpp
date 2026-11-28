/********************************************************************
 * build_bitstream and report_bitstream (see bitstream_flow_commands.h)
 *******************************************************************/
#include "bitstream_flow_commands.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include "bitstream_manager_utils.h"
#include "circuit_types.h"
#include "command_exit_codes.h"
#include "nlohmann/json.hpp"
#include "picosha2.h"
#include "staged_flow.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

using json = nlohmann::json;
namespace fs = std::filesystem;

constexpr int BITSTREAM_SCHEMA_VERSION = 1;
/* The fewest clock cycles of a simulation (annotate_simulation_setting) */
constexpr size_t MIN_SIMULATION_CYCLES = 2;
constexpr size_t BLOCKS_DEFAULT = 50;

std::string option_text(const Command& cmd, const CommandContext& context,
                        const char* name) {
  const CommandOptionId option = cmd.option(name);
  return context.option_enable(cmd, option) ? context.option_value(cmd, option)
                                            : std::string();
}

std::string in_quotes(const std::string& path) { return "\"" + path + "\""; }

std::string whole(const std::string& file) {
  std::error_code error;
  const fs::path path = fs::absolute(file, error);
  return error ? file : path.lexically_normal().string();
}

std::string join(const std::vector<std::string>& words) {
  std::string text;
  for (const std::string& word : words) {
    text += " " + word;
  }
  return text;
}

/* The simulation settings that build_bitstream reads when the session read
 * none: link_openfpga_arch needs some, and the bitstream does not depend on
 * them */
const char* const FIXED_SIMULATION_SETTING =
  R"(<!-- Written by build_bitstream: the session read no simulation settings.
     The bitstream does not depend on them; testbenches do -->
<openfpga_simulation_setting>
  <clock_setting>
    <operating frequency="500e6" num_cycles="2" slack="0.2"/>
    <programming frequency="100e6"/>
  </clock_setting>
  <simulator_option>
    <operating_condition temperature="25"/>
    <output_log verbose="false" captab="false"/>
    <accuracy type="abs" value="1e-13"/>
    <runtime fast_simulation="true"/>
  </simulator_option>
  <monte_carlo num_simulation_points="2"/>
  <measurement_setting>
    <slew>
      <rise upper_thres_pct="0.95" lower_thres_pct="0.05"/>
      <fall upper_thres_pct="0.05" lower_thres_pct="0.95"/>
    </slew>
    <delay>
      <rise input_thres_pct="0.5" output_thres_pct="0.5"/>
      <fall input_thres_pct="0.5" output_thres_pct="0.5"/>
    </delay>
  </measurement_setting>
  <stimulus>
    <clock>
      <rise slew_type="abs" slew_time="20e-12" />
      <fall slew_type="abs" slew_time="20e-12" />
    </clock>
    <input>
      <rise slew_type="abs" slew_time="25e-12" />
      <fall slew_type="abs" slew_time="25e-12" />
    </input>
  </stimulus>
</openfpga_simulation_setting>
)";

/* link_openfpga_arch needs simulation settings, and infers the number
 * of clock cycles from the activity of the signals when they say so, which
 * build_bitstream does not have. The number matters for testbenches only */
bool prepare_simulation_setting(Shell<OpenfpgaContext>* shell,
                                OpenfpgaContext& ctx, const fs::path& folder) {
  if (ctx.session().settings_files("simulation_setting").empty()) {
    const std::string file = (folder / "simulation_setting.xml").string();
    std::ofstream out(file);
    out << FIXED_SIMULATION_SETTING;
    out.close();
    if (!out) {
      VTR_LOG_ERROR("Cannot write '%s'\n", file.c_str());
      return false;
    }
    VTR_LOG(
      "No simulation settings were read: build_bitstream reads fixed ones, "
      "'%s'\n",
      file.c_str());
    const std::string read =
      "read_openfpga_simulation_setting --file " + in_quotes(file);
    return CMD_EXEC_SUCCESS == shell->execute_command(read.c_str(), ctx);
  }
  SimulationSetting& setting = ctx.mutable_simulation_setting();
  if (0 == setting.num_clock_cycles()) {
    setting.set_num_clock_cycles(MIN_SIMULATION_CYCLES);
    VTR_LOG_WARN(
      "The simulation settings infer the number of clock cycles from the "
      "activity of the signals, which build_bitstream does not have: it is "
      "set to %lu. It is used by testbenches only\n",
      MIN_SIMULATION_CYCLES);
  }
  return true;
}

/* Run one step. Log the step that failed */
bool step(Shell<OpenfpgaContext>* shell, OpenfpgaContext& ctx,
          const std::string& line, std::vector<std::string>& ran) {
  const std::string name = line.substr(0, line.find(' '));
  VTR_LOG("build_bitstream: %s\n", line.c_str());
  if (CMD_EXEC_SUCCESS != shell->execute_command(line.c_str(), ctx)) {
    VTR_LOG_ERROR("build_bitstream: the step '%s' failed\n", name.c_str());
    return false;
  }
  ran.push_back(name);
  return true;
}

std::string file_sha256(const std::string& file) {
  std::ifstream stream(file, std::ios::binary);
  if (!stream) {
    return std::string();
  }
  const std::string content((std::istreambuf_iterator<char>(stream)),
                            std::istreambuf_iterator<char>());
  return picosha2::hash256_hex_string(content.begin(), content.end());
}

json file_json(const std::string& file) {
  if (file.empty()) {
    return json();
  }
  std::error_code error;
  const auto bytes = fs::file_size(file, error);
  return {{"file", file},
          {"bytes", error ? json() : json(static_cast<long long>(bytes))},
          {"sha256", file_sha256(file)}};
}

/* The type of a block of the fabric: its name without the coordinates of
 * the instance (grid_clb_1__2_ is grid_clb) */
std::string block_type(const std::string& name) {
  static const std::regex coordinates("(_[0-9]+__[0-9]+_)+$");
  return std::regex_replace(name, coordinates, "");
}

/* The answer of report_bitstream */
json bitstream_json(const OpenfpgaContext& ctx, const size_t limit) {
  const OpenFpgaSession& session = ctx.session();
  json data;
  const BitstreamOutput* output = session.bitstream_output();
  if (nullptr != output) {
    const json file = file_json(output->file);
    data["file"] = file["file"];
    data["format"] = output->format;
    data["bytes"] = file["bytes"];
    data["sha256"] = file["sha256"];
    const json arch = file_json(output->arch_file);
    data["arch_file"] = arch.is_null() ? json() : arch["file"];
    data["arch_sha256"] = arch.is_null() ? json() : arch["sha256"];
    data["settings"] = output->settings;
    data["steps"] = {{"ran", output->ran}, {"kept", output->kept}};
  } else {
    for (const char* key :
         {"file", "format", "bytes", "sha256", "arch_file", "arch_sha256"}) {
      data[key] = json();
    }
    data["settings"] = json::array();
    data["steps"] = json();
  }
  data["bits"] = ctx.fabric_bitstream().num_bits();
  data["protocol"] =
    CONFIG_PROTOCOL_TYPE_STRING[ctx.arch().config_protocol.type()];
  data["regions"] = ctx.fabric_bitstream().num_regions();
  /* The bits of the blocks under the top, by type */
  const BitstreamManager& manager = ctx.bitstream_manager();
  std::map<std::string, std::pair<size_t, size_t>> types;
  for (const ConfigBlockId& top : find_bitstream_manager_top_blocks(manager)) {
    for (const ConfigBlockId& child : manager.block_children(top)) {
      std::pair<size_t, size_t>& type =
        types[block_type(manager.block_name(child))];
      type.first += 1;
      type.second +=
        rec_find_bitstream_manager_block_sum_of_bits(manager, child);
    }
  }
  std::vector<json> blocks;
  for (const auto& type : types) {
    blocks.push_back({{"type", type.first},
                      {"instances", type.second.first},
                      {"bits", type.second.second}});
  }
  std::stable_sort(blocks.begin(), blocks.end(),
                   [](const json& a, const json& b) {
                     return a["bits"].get<size_t>() > b["bits"].get<size_t>();
                   });
  data["truncated"] = blocks.size() > limit;
  if (blocks.size() > limit) {
    blocks.resize(limit);
  }
  data["blocks"] = blocks;
  return data;
}

int give(OpenfpgaContext& ctx, const json& data) {
  json answer;
  answer["schema_version"] = BITSTREAM_SCHEMA_VERSION;
  answer["kind"] = "bitstream";
  answer["stage"] = "BITSTREAM";
  answer["data"] = data;
  const std::string text =
    answer.dump(-1, ' ', false, json::error_handler_t::replace);
  ctx.set_query_result(text);
  VTR_LOG("%s\n", text.c_str());
  return CMD_EXEC_SUCCESS;
}

bool limit_option(const Command& cmd, const CommandContext& context,
                  size_t& limit) {
  const std::string text = option_text(cmd, context, "limit");
  limit = BLOCKS_DEFAULT;
  if (text.empty()) {
    return true;
  }
  try {
    size_t used = 0;
    const long value = std::stol(text, &used);
    if (used == text.size() && value >= 1) {
      limit = static_cast<size_t>(value);
      return true;
    }
  } catch (const std::exception&) {
  }
  VTR_LOG_ERROR(
    "The value of --limit must be a whole number from 1, not '%s'\n",
    text.c_str());
  return false;
}

int build_bitstream_command(Shell<OpenfpgaContext>* shell, OpenfpgaContext& ctx,
                            const Command& cmd, const CommandContext& context) {
  const auto started = std::chrono::steady_clock::now();
  const OpenFpgaSession& session = ctx.session();
  if (!staged_flow_current(session)) {
    VTR_LOG_ERROR(
      "Cannot run 'build_bitstream': it makes the bitstream of the flow run "
      "stage by stage (read_vpr_arch, read_circuit, pack, place, route). "
      "After 'vpr', run the commands of the bitstream by hand\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!stage_ready(session, OpenFpgaStage::ROUTED, "build_bitstream")) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const BitstreamRecipe recipe = session.bitstream_recipe();
  if (!recipe.supported) {
    VTR_LOG_ERROR(
      "Cannot run 'build_bitstream': %s says that its device cannot make a "
      "bitstream\n",
      recipe.owner.empty() ? "the recipe of the bitstream"
                           : recipe.owner.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  std::string format = option_text(cmd, context, "format");
  if (format.empty()) {
    format = "plain_text";
  }
  if ("plain_text" != format && "xml" != format) {
    VTR_LOG_ERROR("--format is plain_text or xml, not '%s'\n", format.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  std::string file = option_text(cmd, context, "file");
  if (file.empty()) {
    const std::string circuit =
      fs::path(session.input_file(OpenFpgaInput::CIRCUIT)).stem().string();
    file = (fs::path("bitstream") /
            ((circuit.empty() ? std::string("fabric") : circuit) +
             ("xml" == format ? ".xml" : ".bit")))
             .string();
  }
  file = whole(file);
  std::string arch_file = option_text(cmd, context, "arch_file");
  if (!arch_file.empty()) {
    arch_file = whole(arch_file);
  }
  std::error_code error;
  for (const std::string& path : {file, arch_file}) {
    if (!path.empty() && fs::path(path).has_parent_path()) {
      fs::create_directories(fs::path(path).parent_path(), error);
      if (error) {
        VTR_LOG_ERROR("Cannot make the folder of '%s': %s\n", path.c_str(),
                      error.message().c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  BitstreamOutput output;
  output.format = format;
  output.arch_file = arch_file;
  std::vector<std::string>& ran = output.ran;
  /* The analysis makes what 'vpr' makes after the routing, which the link
   * and repack need: the packing synchronized with the routing */
  if (!session.reached(OpenFpgaStage::ANALYZED) &&
      !step(shell, ctx, "analysis", ran)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!ctx.session().link_current()) {
    if (!prepare_simulation_setting(shell, ctx, fs::path(file).parent_path())) {
      return CMD_EXEC_FATAL_ERROR;
    }
    if (!step(shell, ctx, "link_openfpga_arch" + join(recipe.link_options),
              ran)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    for (const std::string& fixup : recipe.fixups) {
      const std::string line =
        "check_netlist_naming_conflict" == fixup ? fixup + " --fix" : fixup;
      if (!step(shell, ctx, line, ran)) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  } else {
    output.kept.push_back("link_openfpga_arch");
  }
  if (!ctx.session().fabric_current()) {
    if (!step(shell, ctx, "build_fabric" + join(recipe.fabric_options), ran)) {
      return CMD_EXEC_FATAL_ERROR;
    }
  } else {
    output.kept.push_back("build_fabric");
  }
  const std::vector<std::string> lines = {
    "repack",
    "build_architecture_bitstream --no_time_stamp" +
      (arch_file.empty() ? std::string()
                         : " --write_file " + in_quotes(arch_file)),
    "build_fabric_bitstream",
    "write_fabric_bitstream --file " + in_quotes(file) + " --format " + format +
      " --no_time_stamp"};
  for (const std::string& line : lines) {
    if (!step(shell, ctx, line, ran)) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  output.file = file;
  for (const char* kind : {"simulation_setting", "bitstream_setting"}) {
    for (const std::string& setting : ctx.session().settings_files(kind)) {
      output.settings.push_back(setting);
    }
  }
  OpenFpgaSession& mutable_session = ctx.mutable_session();
  mutable_session.set_bitstream_output(output);
  mutable_session.set_stage_seconds(
    OpenFpgaStage::BITSTREAM,
    std::chrono::duration<double>(std::chrono::steady_clock::now() - started)
      .count());
  return give(ctx, bitstream_json(ctx, BLOCKS_DEFAULT));
}

int report_bitstream_command(OpenfpgaContext& ctx, const Command& cmd,
                             const CommandContext& context) {
  size_t limit = BLOCKS_DEFAULT;
  if (!limit_option(cmd, context, limit)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!stage_ready(ctx.session(), OpenFpgaStage::BITSTREAM,
                   "report_bitstream")) {
    return CMD_EXEC_FATAL_ERROR;
  }
  return give(ctx, bitstream_json(ctx, limit));
}

}  // namespace

void add_bitstream_flow_commands(Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class =
    shell.add_command_class("Bitstream of the session");

  Command build("build_bitstream");
  CommandOptionId opt = build.add_option(
    "file", false,
    "The file of the fabric bitstream (default: bitstream/<circuit>.bit)");
  build.set_option_require_value(opt, OPT_STRING);
  opt = build.add_option("format", false,
                         "The format of the file: plain_text (default) or xml");
  build.set_option_require_value(opt, OPT_STRING);
  opt = build.add_option(
    "arch_file", false,
    "Also write the architecture bitstream (XML) in this file");
  build.set_option_require_value(opt, OPT_STRING);
  ShellCommandId build_id = shell.add_command(
    build,
    "Make the bitstream of the routed design of the session: link, fabric "
    "(kept when still valid), repack, architecture and fabric bitstreams, and "
    "the file, with the recipe of the bitstream of the device");
  shell.set_command_class(build_id, cmd_class);
  shell.set_command_execute_function(build_id, build_bitstream_command);

  Command report("report_bitstream");
  opt = report.add_option("limit", false,
                          "The most types of blocks listed (default: 50)");
  report.set_option_require_value(opt, OPT_STRING);
  report.add_option("json", false, "Answer JSON (always)");
  ShellCommandId report_id = shell.add_command(
    report,
    "Answer JSON about the bitstream of the session: the file, the bits, the "
    "protocol, the bits per type of block, the settings");
  shell.set_command_class(report_id, cmd_class);
  shell.set_command_execute_function(report_id, report_bitstream_command);
}

} /* end namespace openfpga */
