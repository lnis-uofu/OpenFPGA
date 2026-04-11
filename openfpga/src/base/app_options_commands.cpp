#include "app_options.h"
#include "command_exit_codes.h"
#include "openfpga_basic.h"
#include "openfpga_title.h"

namespace openfpga {

void print_single_app_option(const std::string& option_name,
                             const AppOptionValue& option_value) {
  VTR_LOG("%s = %s", option_name.c_str(), option_value.to_string().c_str());

  if (AppOptionValue::SELECTION == option_value.type) {
    VTR_LOG(" [");
    size_t entry_cnt = 0;
    for (const auto& selection : option_value.selection_values) {
      VTR_LOG(
        "%s%s", selection.first.c_str(),
        (entry_cnt + 1 == option_value.selection_values.size()) ? "" : ", ");
      ++entry_cnt;
    }
    VTR_LOG("]");
  }

  VTR_LOG("\n");
}

int set_app_option_command(openfpga::Shell<OpenfpgaContext>* shell,
                           OpenfpgaContext& openfpga_ctx, const Command& cmd,
                           const CommandContext& cmd_context) {
  CommandOptionId opt_name = cmd.option("name");
  CommandOptionId opt_value = cmd.option("value");

  const std::string option_name = cmd_context.option_value(cmd, opt_name);
  const std::string option_value = cmd_context.option_value(cmd, opt_value);

  if (!shell->has_app_option(option_name)) {
    VTR_LOG_ERROR("Application option '%s' does not exist\n",
                  option_name.c_str());
    return CMD_EXEC_MINOR_ERROR;
  }

  shell->set_app_option(option_name, option_value);

  return CMD_EXEC_SUCCESS;
}

int report_app_option_command(openfpga::Shell<OpenfpgaContext>* shell,
                              OpenfpgaContext& openfpga_ctx, const Command& cmd,
                              const CommandContext& cmd_context) {
  CommandOptionId opt_name = cmd.option("name");
  const bool report_single_opt = cmd_context.option_enable(cmd, opt_name);

  if (report_single_opt) {
    const std::string option_name = cmd_context.option_value(cmd, opt_name);
    if (!shell->has_app_option(option_name)) {
      VTR_LOG("Application option '%s' is not set\n", option_name.c_str());
      return CMD_EXEC_MINOR_ERROR;
    }

    print_single_app_option(option_name, shell->get_app_option(option_name));
    return CMD_EXEC_SUCCESS;
  }

  const std::map<std::string, AppOptionValue> app_options =
    shell->app_options();
  if (app_options.empty()) {
    VTR_LOG("No application option has been set\n");
    return CMD_EXEC_SUCCESS;
  }

  for (const auto& app_option : app_options) {
    print_single_app_option(app_option.first, app_option.second);
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */