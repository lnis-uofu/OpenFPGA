#include "app_options_commands.h"

#include "app_options_json.h"
#include "command_exit_codes.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* Print an option on one line: name = value, followed by the values it can
 * take if it is a selection */
void print_app_option(const std::string& name, const AppOptionValue& option) {
  std::string line = name + " = " + option.to_string();
  if (AppOptionValue::SELECTION == option.type) {
    line += " [";
    const std::vector<std::string> choices = option.choices();
    for (size_t i = 0; i < choices.size(); ++i) {
      line += (0 == i ? "" : ", ") + choices[i];
    }
    line += "]";
  }
  VTR_LOG("%s\n", line.c_str());
}

int set_app_option_command(OpenfpgaContext& openfpga_ctx, const Command& cmd,
                           const CommandContext& cmd_context) {
  const std::string name = cmd_context.option_value(cmd, cmd.option("name"));
  const std::string value = cmd_context.option_value(cmd, cmd.option("value"));

  std::string error;
  if (!openfpga_ctx.mutable_session().set_app_option(name, value, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int report_app_option_command(const OpenfpgaContext& openfpga_ctx,
                              const Command& cmd,
                              const CommandContext& cmd_context) {
  const AppOptions& options = openfpga_ctx.session().app_options();

  /* The options to report: one, or all of them */
  std::vector<std::string> names;
  const CommandOptionId opt_name = cmd.option("name");
  if (cmd_context.option_enable(cmd, opt_name)) {
    const std::string name = cmd_context.option_value(cmd, opt_name);
    if (nullptr == options.find(name)) {
      VTR_LOG_ERROR("Application option '%s' does not exist\n", name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    names.push_back(name);
  } else {
    names = options.names();
  }

  /* Only the options that are not at their default value */
  if (cmd_context.option_enable(cmd, cmd.option("changed"))) {
    std::vector<std::string> changed;
    for (const std::string& name : names) {
      if (!options.is_default(name)) {
        changed.push_back(name);
      }
    }
    names = changed;
  }

  /* As a query answer: the JSON is printed, and given to the caller */
  if (cmd_context.option_enable(cmd, cmd.option("json"))) {
    const std::string json = app_options_json(options, names);
    openfpga_ctx.set_query_result(json);
    VTR_LOG("%s\n", json.c_str());
    return CMD_EXEC_SUCCESS;
  }

  for (const std::string& name : names) {
    print_app_option(name, *options.find(name));
  }
  return CMD_EXEC_SUCCESS;
}

}  // End anonymous namespace

void add_app_options_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("App options");

  /* Command 'set_app_option': set the value of an option */
  Command shell_cmd_set("set_app_option");
  CommandOptionId set_opt_name = shell_cmd_set.add_option(
    "name", true, "Name of the option, e.g., place.seed");
  shell_cmd_set.set_option_short_name(set_opt_name, "n");
  shell_cmd_set.set_option_require_value(set_opt_name, openfpga::OPT_STRING);
  CommandOptionId set_opt_value = shell_cmd_set.add_option(
    "value", true, "Value of the option, according to its type");
  shell_cmd_set.set_option_short_name(set_opt_value, "v");
  shell_cmd_set.set_option_require_value(set_opt_value, openfpga::OPT_STRING);
  ShellCommandId shell_cmd_set_id = shell.add_command(
    shell_cmd_set, "Set an application option, which tunes the flow engines");
  shell.set_command_class(shell_cmd_set_id, cmd_class);
  shell.set_command_execute_function(shell_cmd_set_id, set_app_option_command);

  /* Command 'report_app_option': show the value of one option or of all */
  Command shell_cmd_report("report_app_option");
  CommandOptionId report_opt_name = shell_cmd_report.add_option(
    "name", false,
    "Name of the option. All the options are reported if not set");
  shell_cmd_report.set_option_short_name(report_opt_name, "n");
  shell_cmd_report.set_option_require_value(report_opt_name,
                                            openfpga::OPT_STRING);
  shell_cmd_report.add_option(
    "changed", false,
    "Report only the options that do not have their default value");
  CommandOptionId report_opt_json = shell_cmd_report.add_option(
    "json", false,
    "Report as a JSON answer of the kind app_options, which is given to the "
    "caller as the data of the result");
  shell_cmd_report.set_option_short_name(report_opt_json, "j");
  ShellCommandId shell_cmd_report_id = shell.add_command(
    shell_cmd_report, "Report the value of one or all the application options");
  shell.set_command_class(shell_cmd_report_id, cmd_class);
  shell.set_command_const_execute_function(shell_cmd_report_id,
                                           report_app_option_command);
}

} /* End namespace openfpga */
