#include "session_report_commands.h"

#include <fstream>
#include <regex>
#include <sstream>
#include <string>

#include "command_exit_codes.h"
#include "nlohmann/json.hpp"
#include "query_answer.h"
#include "query_run.h"
#include "query_stage.h"
#include "session_json.h"
#include "staged_flow.h"
#include "vpr_run.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

int report_session_command(const OpenfpgaContext& openfpga_ctx, const Command&,
                           const CommandContext&) {
  const std::string json =
    session_json(openfpga_ctx.session(), vpr::current_vpr_run());
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

int report_stage_command(const OpenfpgaContext& openfpga_ctx,
                         const Command& cmd,
                         const CommandContext& cmd_context) {
  size_t limit = 0;
  if (!count_option(cmd, cmd_context, "limit", QUERY_LIST_DEFAULT_LIMIT,
                    limit)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  std::string json;
  if (!stage_info_json(openfpga_ctx.session(),
                       string_option(cmd, cmd_context, "stage"),
                       cmd_context.option_enable(cmd, cmd.option("changed")),
                       string_option(cmd, cmd_context, "query"), limit, json)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

int report_run_command(const OpenfpgaContext& openfpga_ctx, const Command&,
                       const CommandContext&) {
  const bool in_flow = staged_flow_current(openfpga_ctx.session());
  return answer_query(openfpga_ctx, OpenFpgaStage::DEVICE, "report_run",
                      [&](const vpr::VprRun& run, std::string& json) {
                        return run_report_json(openfpga_ctx.session(), in_flow,
                                               run, json);
                      });
}

/* Keep (or with no --value nor --file, drop) the data of a plugin in the
 * session, e.g., when a checkpoint is restored. The value, or the text of the
 * file, must be JSON */
int set_attachment_command(OpenfpgaContext& openfpga_ctx, const Command& cmd,
                           const CommandContext& cmd_context) {
  const std::string key = string_option(cmd, cmd_context, "key");
  if (!std::regex_match(key, std::regex("[a-z][a-z0-9_]*\\..+"))) {
    VTR_LOG_ERROR(
      "The key of an attachment starts with the name of its plugin and a "
      "dot, e.g., 'myplugin.data', not '%s'\n",
      key.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  OpenFpgaSession& session = openfpga_ctx.mutable_session();
  const bool has_value = cmd_context.option_enable(cmd, cmd.option("value"));
  const bool has_file = cmd_context.option_enable(cmd, cmd.option("file"));
  if (has_value && has_file) {
    VTR_LOG_ERROR("Give the value of an attachment or its file, not both\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!has_value && !has_file) {
    session.clear_attachment(key);
    return CMD_EXEC_SUCCESS;
  }
  std::string value = string_option(cmd, cmd_context, "value");
  if (has_file) {
    const std::string file = string_option(cmd, cmd_context, "file");
    std::ifstream in(file);
    if (!in) {
      VTR_LOG_ERROR("Cannot read the file '%s'\n", file.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    std::stringstream text;
    text << in.rdbuf();
    value = text.str();
  }
  if (!nlohmann::json::accept(value)) {
    VTR_LOG_ERROR("The value of the attachment '%s' is not JSON\n",
                  key.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  session.set_attachment(key, value);
  return CMD_EXEC_SUCCESS;
}

}  // End anonymous namespace

void add_session_report_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Session reports");

  add_query_command(
    shell, cmd_class, Command("report_session"),
    "Answer with the stages that are reached, the files the inputs were read "
    "from and the files of the results of the stages",
    report_session_command);

  Command report_stage("report_stage");
  add_value_option(report_stage, "stage", "s", false, OPT_STRING,
                   "Only this stage, e.g., PLACED or place. Default: all");
  report_stage.add_option("changed", false,
                          "Only the options that do not have their default "
                          "value");
  add_value_option(
    report_stage, "query", "q", false, OPT_STRING,
    "Only the options whose name matches this shell-style pattern, "
    "e.g., 'place.*'");
  add_value_option(report_stage, "limit", "l", false, OPT_INT,
                   "Most options to list for each stage. Default: 1000");
  add_query_command(shell, cmd_class, report_stage,
                    "Answer with what can be changed for each stage: its "
                    "options, their values, and the stages that a change drops",
                    report_stage_command);

  add_query_command(shell, cmd_class, Command("report_run"),
                    "Answer with what each stage of the flow did: the clusters "
                    "of the packing, the cost of the placement, the wirelength "
                    "and iterations of the routing, and the time of each stage",
                    report_run_command);

  ShellCommandClassId data_class = shell.add_command_class("Session data");
  Command set_attachment("set_attachment");
  add_value_option(set_attachment, "key", "k", true, OPT_STRING,
                   "The key of the attachment: the name of its plugin, a dot "
                   "and a name, e.g., 'myplugin.data'");
  add_value_option(set_attachment, "value", "v", false, OPT_STRING,
                   "Its value, as JSON (a value with double quotes is given "
                   "in a file). Without it or --file the attachment is "
                   "dropped");
  add_value_option(set_attachment, "file", "f", false, OPT_STRING,
                   "A file whose text is its value, as JSON");
  ShellCommandId id = shell.add_command(
    set_attachment,
    "Keep the data of a plugin with the architectures of the session (an "
    "attachment, see report_session), e.g., when a checkpoint is restored");
  shell.set_command_class(id, data_class);
  shell.set_command_execute_function(id, set_attachment_command);
}

} /* End namespace openfpga */
