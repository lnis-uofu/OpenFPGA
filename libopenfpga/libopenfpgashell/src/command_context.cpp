/*********************************************************************
 * Member functions for class CommandContext 
 ********************************************************************/
#include "vtr_assert.h"
#include "command_context.h"

/* Begin namespace minishell */
namespace minishell {

/*********************************************************************
 * Public constructor
 ********************************************************************/
CommandContext::CommandContext(const Command& command) {
  option_enabled_.resize(command.options().size(), false);
  option_values_.resize(command.options().size());
}

/*********************************************************************
 * Public accessors
 ********************************************************************/
bool CommandContext::option_enable(const Command& command,
                                   const CommandOptionId& option_id) const {
  VTR_ASSERT(true == command.valid_option_id(option_id));
  return option_enabled_[option_id];
}

std::string CommandContext::option_value(const Command& command,
                                         const CommandOptionId& option_id) const {
  VTR_ASSERT(true == command.valid_option_id(option_id));
  return option_values_[option_id];
}

std::vector<CommandOptionId> CommandContext::check_required_options(const Command& command) const {
  std::vector<CommandOptionId> fail_options;
  for (const CommandOptionId& option_id : command.required_options()) {
    if (false == option_enabled_[option_id]) {
      fail_options.push_back(option_id);
    }
  }
  return fail_options;
}

std::vector<CommandOptionId> CommandContext::check_required_option_values(const Command& command) const {
  std::vector<CommandOptionId> fail_options;
  for (const CommandOptionId& option_id : command.require_value_options()) {
    if (true == option_values_[option_id].empty()) {
      fail_options.push_back(option_id);
    }
  }
  return fail_options;
}


/*********************************************************************
 * Public mutators
 ********************************************************************/
void CommandContext::set_option(const Command& command,
                                const CommandOptionId& option_id, const bool& status) {
  VTR_ASSERT(true == command.valid_option_id(option_id));
  option_enabled_[option_id] = status;
}

void CommandContext::set_option_value(const Command& command,
                                      const CommandOptionId& option_id, const std::string& value) {
  VTR_ASSERT(true == command.valid_option_id(option_id));
  option_values_[option_id] = value;
}

} /* End namespace minshell */
