/********************************************************************
 * The commands of the data of the views (see view_commands.h)
 *******************************************************************/
#include "view_commands.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "command_exit_codes.h"
#include "json_response.h"
#include "json_writer.h"
#include "staged_flow.h"
#include "view_export.h"
#include "vpr_run.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

std::string option_text(const Command& cmd, const CommandContext& context,
                        const char* name) {
  const CommandOptionId option = cmd.option(name);
  return context.option_enable(cmd, option) ? context.option_value(cmd, option)
                                            : std::string();
}

bool option_int(const Command& cmd, const CommandContext& context,
                const char* name, int& value) {
  const std::string text = option_text(cmd, context, name);
  if (text.empty()) {
    return true;
  }
  try {
    size_t used = 0;
    value = std::stoi(text, &used);
    if (used == text.size()) {
      return true;
    }
  } catch (const std::exception&) {
  }
  VTR_LOG_ERROR("The value of --%s must be a whole number, not '%s'\n", name,
                text.c_str());
  return false;
}

/* The version in the header of a file of a view, or "" */
std::string file_version(const std::string& path) {
  std::ifstream in(path, std::ios::binary);
  char head[12];
  if (!in.read(head, 12) || 0 != std::string(head, 7).compare("OFVIEW1")) {
    return std::string();
  }
  uint32_t length = 0;
  for (int i = 0; i < 4; ++i) {
    length |= static_cast<uint32_t>(static_cast<uint8_t>(head[8 + i]))
              << (8 * i);
  }
  if (length > (1u << 24)) {
    return std::string();
  }
  std::string header(length, '\0');
  if (!in.read(&header[0], length)) {
    return std::string();
  }
  const std::string key = "\"version\":\"";
  const size_t at = header.find(key);
  if (std::string::npos == at) {
    return std::string();
  }
  const size_t end = header.find('"', at + key.size());
  return std::string::npos == end
           ? std::string()
           : header.substr(at + key.size(), end - at - key.size());
}

/* The answer of the two commands */
void give(const OpenfpgaContext& ctx, const std::string& kind,
          const std::string& path, const std::string& version,
          const bool cached, const ViewFile* file) {
  std::ifstream in(path, std::ios::binary | std::ios::ate);
  const int64_t bytes = in ? static_cast<int64_t>(in.tellg()) : 0;
  JsonWriter json;
  begin_response(json, "view_export", VIEW_EXPORT_SCHEMA_VERSION);
  json.begin_object();
  json.key("kind").value(kind);
  json.key("file").value(path);
  json.key("version").value(version);
  json.key("bytes").value(bytes);
  json.key("cached").value(cached);
  json.key("tables");
  if (nullptr == file) {
    json.null();
  } else {
    json.begin_object();
    for (const ViewTable& table : file->tables()) {
      json.key(table.name()).value(static_cast<int64_t>(table.rows()));
    }
    json.end_object();
  }
  json.end_object();
  end_response(json);
  const std::string text = json.str();
  ctx.set_query_result(text);
  VTR_LOG("%s\n", text.c_str());
}

/* The stage a kind needs is reached (in the flow run stage by stage) */
bool ready(const OpenfpgaContext& ctx, const std::string& kind, bool tile,
           const char* command) {
  const std::vector<OpenFpgaStage> stages = view_stages(kind, tile);
  return stages.empty() || stage_ready(ctx.session(), stages[0], command);
}

int export_view_command(const OpenfpgaContext& ctx, const Command& cmd,
                        const CommandContext& context) {
  ViewRequest request;
  request.kind = option_text(cmd, context, "kind");
  const std::string path = option_text(cmd, context, "file");
  const CommandOptionId module = cmd.option("module");
  request.has_module = context.option_enable(cmd, module);
  if (request.has_module) {
    request.module = context.option_value(cmd, module);
    if ("." == request.module) {
      request.module.clear(); /* the top */
    }
  }
  request.cone = option_text(cmd, context, "cone");
  int depth = request.depth;
  int limit = static_cast<int>(request.limit);
  int paths = static_cast<int>(request.paths);
  if (!option_int(cmd, context, "depth", depth) ||
      !option_int(cmd, context, "limit", limit) ||
      !option_int(cmd, context, "paths", paths)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  if (depth < 0 || depth > 20 || limit < 1 || paths < 1) {
    VTR_LOG_ERROR(
      "--depth is from 0 to 20, --limit and --paths are at least 1\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  request.depth = depth;
  request.limit = static_cast<size_t>(limit);
  request.paths = static_cast<size_t>(paths);
  std::stringstream blocks(option_text(cmd, context, "blocks"));
  std::string block;
  while (std::getline(blocks, block, ',')) {
    if (!block.empty()) {
      request.blocks.push_back(block);
    }
  }
  if (request.has_module && !request.cone.empty()) {
    VTR_LOG_ERROR("Give --module or --cone, not both\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!ready(ctx, request.kind, false, "export_view")) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const vpr::VprRun* run = vpr::current_vpr_run();
  if (nullptr == run) {
    VTR_LOG_ERROR("There is no data of VPR: run the flow first\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string version = view_version(ctx.session(), request.kind, false);
  /* A scope makes a file of its own, which the caller names: only a file of
   * the same version is kept as it is */
  if (!path.empty() && file_version(path) == version) {
    give(ctx, request.kind, path, version, true, nullptr);
    return CMD_EXEC_SUCCESS;
  }
  ViewFile file(request.kind, version);
  std::string error;
  if (!export_view(*run, ctx.session(), request, file, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!file.write(path, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  give(ctx, request.kind, path, version, false, &file);
  return CMD_EXEC_SUCCESS;
}

int view_tile_command(const OpenfpgaContext& ctx, const Command& cmd,
                      const CommandContext& context) {
  TileRequest request;
  request.kind = option_text(cmd, context, "kind");
  const std::string path = option_text(cmd, context, "file");
  if (!option_int(cmd, context, "level", request.level) ||
      !option_int(cmd, context, "x", request.x) ||
      !option_int(cmd, context, "y", request.y)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!ready(ctx, request.kind, true, "view_tile")) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const vpr::VprRun* run = vpr::current_vpr_run();
  if (nullptr == run) {
    VTR_LOG_ERROR("There is no data of VPR: run the flow first\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string version = view_version(ctx.session(), request.kind, true);
  if (file_version(path) == version) {
    give(ctx, request.kind, path, version, true, nullptr);
    return CMD_EXEC_SUCCESS;
  }
  ViewFile file(request.kind, version);
  std::string error;
  if (!export_tile(*run, ctx.session(), request, file, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!file.write(path, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  give(ctx, request.kind, path, version, false, &file);
  return CMD_EXEC_SUCCESS;
}

void add_text(Command& cmd, const char* name, bool required, const char* help) {
  const CommandOptionId option = cmd.add_option(name, required, help);
  cmd.set_option_require_value(option, OPT_STRING);
}

}  // namespace

void add_view_commands(Shell<OpenfpgaContext>& shell) {
  const ShellCommandClassId cmd_class = shell.add_command_class("Views");

  Command view("export_view");
  add_text(view, "kind", true,
           "device, placement, pins, routing, timing, atoms or clusters");
  add_text(view, "file", true, "The file to write");
  add_text(view, "module", false,
           "atoms: the atoms of this module and of the modules in it ('.' "
           "for the top)");
  add_text(view, "cone", false,
           "atoms: the atoms around this atom or net (with --depth)");
  add_text(view, "depth", false, "atoms: the nets away from --cone (2)");
  add_text(view, "blocks", false,
           "clusters: these clusters, separated by commas");
  add_text(view, "limit", false,
           "atoms, clusters: the most in an export (100000)");
  add_text(view, "paths", false, "timing: the number of worst paths (20)");
  const ShellCommandId view_id = shell.add_command(
    view,
    "Write the data of a view of the design as a binary file of tables "
    "(docs/plan_design_views.md). A file of the current version is kept "
    "as it is");
  shell.set_command_class(view_id, cmd_class);
  shell.set_command_const_execute_function(view_id, export_view_command);

  Command tile("view_tile");
  add_text(tile, "kind", true, "grid, placement, routing, channels or timing");
  add_text(tile, "level", true,
           "0 for the objects, more for a value per cell of 2^level tiles");
  add_text(tile, "x", true, "The column of the tile at its level");
  add_text(tile, "y", true, "The row of the tile at its level");
  add_text(tile, "file", true, "The file to write");
  const ShellCommandId tile_id = shell.add_command(
    tile,
    "Write a tile of a view of the device (16 << level grid tiles a side) as "
    "a binary file of tables. A file of the current version is kept as it "
    "is");
  shell.set_command_class(tile_id, cmd_class);
  shell.set_command_const_execute_function(tile_id, view_tile_command);
}

} /* end namespace openfpga */
