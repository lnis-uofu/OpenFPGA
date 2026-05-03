#ifndef OPENFPGA_APP_OPTIONS_COMMANDS_H
#define OPENFPGA_APP_OPTIONS_COMMANDS_H

#include "openfpga_context.h"
#include "shell.h"

namespace openfpga {

void print_single_app_option(const std::string& option_name,
                             const AppOptionValue& option_value);

int set_app_option_command(openfpga::Shell<OpenfpgaContext>* shell,
                           OpenfpgaContext& openfpga_ctx, const Command& cmd,
                           const CommandContext& cmd_context);

int report_app_option_command(openfpga::Shell<OpenfpgaContext>* shell,
                              OpenfpgaContext& openfpga_ctx, const Command& cmd,
                              const CommandContext& cmd_context);

}  // namespace openfpga

#endif  // OPENFPGA_APP_OPTIONS_COMMANDS_H