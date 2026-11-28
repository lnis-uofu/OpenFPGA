/********************************************************************
 * Helpers of the commands that answer in JSON, see query_answer.h
 *******************************************************************/
#include "query_answer.h"

#include <exception>

#include "command_exit_codes.h"
#include "staged_flow.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

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

}  // End anonymous namespace

int answer_query(const OpenfpgaContext& ctx, const OpenFpgaStage& stage,
                 const char* command, const QueryFunction& query,
                 const bool stage_is_needed) {
  if (stage_is_needed && !stage_ready(ctx.session(), stage, command)) {
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
  ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

bool stage_data(const OpenfpgaContext& ctx, const OpenFpgaStage& stage) {
  return !staged_flow_current(ctx.session()) || ctx.session().reached(stage);
}

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

void add_value_option(Command& cmd, const char* name, const char* short_name,
                      const bool required, const e_option_value_type type,
                      const char* help) {
  CommandOptionId option = cmd.add_option(name, required, help);
  if (nullptr != short_name) {
    cmd.set_option_short_name(option, short_name);
  }
  cmd.set_option_require_value(option, type);
}

void add_query_command(Shell<OpenfpgaContext>& shell,
                       const ShellCommandClassId& cmd_class, const Command& cmd,
                       const char* description,
                       int (*exec)(const OpenfpgaContext&, const Command&,
                                   const CommandContext&)) {
  ShellCommandId id = shell.add_command(cmd, description);
  shell.set_command_class(id, cmd_class);
  shell.set_command_const_execute_function(id, exec);
}

} /* end namespace openfpga */
