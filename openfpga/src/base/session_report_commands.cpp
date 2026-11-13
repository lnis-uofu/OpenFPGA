#include "session_report_commands.h"

#include <cstddef>
#include <exception>
#include <functional>
#include <string>

#include "command_exit_codes.h"
#include "query_run.h"
#include "query_stage.h"
#include "session_json.h"
#include "staged_flow.h"
#include "vpr_run.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

using Query = std::function<bool(const vpr::VprRun&, std::string&)>;

/* The most items a report lists, unless a limit is given */
constexpr size_t QUERY_LIST_DEFAULT_LIMIT = 1000;

/* Make the answer of a report that reads the data of VPR, and give it: print
 * it, and hand it to the caller of the command */
int answer(const OpenfpgaContext& openfpga_ctx, const OpenFpgaStage& stage,
           const char* command, const Query& query) {
  if (!stage_ready(openfpga_ctx.session(), stage, command)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const vpr::VprRun* run = vpr::current_vpr_run();
  if (nullptr == run) {
    VTR_LOG_ERROR(
      "Cannot run '%s': there is no data of VPR. Run the flow first, e.g., "
      "with the commands that read the inputs and run the stages\n",
      command);
    return CMD_EXEC_FATAL_ERROR;
  }
  std::string json;
  if (!query(*run, json)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

/* The value of an option that is a whole number. Return false and log if it
 * is not one */
bool whole_number(const char* option, const std::string& text, int& number) {
  try {
    size_t used = 0;
    number = std::stoi(text, &used);
    if (used == text.size()) {
      return true;
    }
  } catch (const std::exception&) {
  }
  VTR_LOG_ERROR("The value of --%s must be a whole number, not '%s'\n", option,
                text.c_str());
  return false;
}

/* The value of an option that is a number of items, from 1 to the largest
 * size_t. The default is used if the option is not given */
bool count_option(const Command& cmd, const CommandContext& cmd_context,
                  const char* name, const size_t default_value, size_t& count) {
  const CommandOptionId option = cmd.option(name);
  if (!cmd_context.option_enable(cmd, option)) {
    count = default_value;
    return true;
  }
  int number = 0;
  if (!whole_number(name, cmd_context.option_value(cmd, option), number)) {
    return false;
  }
  if (number < 1) {
    VTR_LOG_ERROR("The value of --%s must be at least 1, not %d\n", name,
                  number);
    return false;
  }
  count = static_cast<size_t>(number);
  return true;
}

std::string string_option(const Command& cmd, const CommandContext& cmd_context,
                          const char* name) {
  const CommandOptionId option = cmd.option(name);
  return cmd_context.option_enable(cmd, option)
           ? cmd_context.option_value(cmd, option)
           : std::string();
}

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
  return answer(openfpga_ctx, OpenFpgaStage::DEVICE, "report_run",
                [&](const vpr::VprRun& run, std::string& json) {
                  return run_report_json(openfpga_ctx.session(), in_flow, run,
                                         json);
                });
}

/* An option that takes a value, which is optional unless required is set */
void add_option(Command& cmd, const char* name, const char* short_name,
                const bool required, const e_option_value_type type,
                const char* help) {
  CommandOptionId option = cmd.add_option(name, required, help);
  if (nullptr != short_name) {
    cmd.set_option_short_name(option, short_name);
  }
  cmd.set_option_require_value(option, type);
}

/* A command that does not change the data: its function is const */
void add_query_command(openfpga::Shell<OpenfpgaContext>& shell,
                       const ShellCommandClassId& cmd_class, const Command& cmd,
                       const char* description,
                       int (*exec)(const OpenfpgaContext&, const Command&,
                                   const CommandContext&)) {
  ShellCommandId id = shell.add_command(cmd, description);
  shell.set_command_class(id, cmd_class);
  shell.set_command_const_execute_function(id, exec);
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
  add_option(report_stage, "stage", "s", false, OPT_STRING,
             "Only this stage, e.g., PLACED or place. Default: all");
  report_stage.add_option("changed", false,
                          "Only the options that do not have their default "
                          "value");
  add_option(report_stage, "query", "q", false, OPT_STRING,
             "Only the options whose name matches this shell-style pattern, "
             "e.g., 'place.*'");
  add_option(report_stage, "limit", "l", false, OPT_INT,
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
}

} /* End namespace openfpga */
