/*********************************************************************
 * Member functions for class Command
 ********************************************************************/
#include "vtr_assert.h"
#include "command.h"

/* Begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Public constructors
 ********************************************************************/
Command::Command(const char* name) {
  name_ = std::string(name);
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
std::string Command::name() const {
  return name_;
}

Command::command_option_range Command::options() const {
  return vtr::make_range(option_ids_.begin(), option_ids_.end());
}

std::vector<CommandOptionId> Command::required_options() const {
  std::vector<CommandOptionId> opts;
  for (const CommandOptionId& opt : options()) {
    if (true == option_required(opt)) {
      opts.push_back(opt);
    }
  }
  return opts;
}

std::vector<CommandOptionId> Command::require_value_options() const {
  std::vector<CommandOptionId> opts;
  for (const CommandOptionId& opt : options()) {
    if (true == option_require_value(opt)) {
      opts.push_back(opt);
    }
  }
  return opts;
}

CommandOptionId Command::option(const std::string& name) const {
  /* Ensure that the name is unique in the option list */
  std::map<std::string, CommandOptionId>::const_iterator name_it = option_name2ids_.find(name);
  if (name_it == option_name2ids_.end()) {
    return CommandOptionId::INVALID();
  }
  return option_name2ids_.at(name);
}

CommandOptionId Command::short_option(const std::string& name) const {
  /* Ensure that the name is unique in the option list */
  std::map<std::string, CommandOptionId>::const_iterator name_it = option_short_name2ids_.find(name);
  if (name_it == option_short_name2ids_.end()) {
    return CommandOptionId::INVALID();
  }
  return option_short_name2ids_.at(name);
}

std::string Command::option_name(const CommandOptionId& option_id) const {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));
  return option_names_[option_id];
}

std::string Command::option_short_name(const CommandOptionId& option_id) const {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));
  return option_short_names_[option_id];
}

bool Command::option_required(const CommandOptionId& option_id) const {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));
  return option_required_[option_id];
}

bool Command::option_require_value(const CommandOptionId& option_id) const {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));
  return NUM_OPT_VALUE_TYPES != option_require_value_types_[option_id];
}

e_option_value_type Command::option_require_value_type(const CommandOptionId& option_id) const {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));
  return option_require_value_types_[option_id];
}

std::string Command::option_description(const CommandOptionId& option_id) const {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));
  return option_description_[option_id];
}

/************************************************************************
 * Public mutators
 ***********************************************************************/

/* Add an option without required values */
CommandOptionId Command::add_option(const char* name,
                                    const bool& option_required, 
                                    const char* description) {
  /* Ensure that the name is unique in the option list */
  std::map<std::string, CommandOptionId>::const_iterator name_it = option_name2ids_.find(std::string(name));
  if (name_it != option_name2ids_.end()) {
    return CommandOptionId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  CommandOptionId option = CommandOptionId(option_ids_.size());
  option_ids_.push_back(option);
  option_names_.push_back(std::string(name));
  option_short_names_.emplace_back();
  option_required_.push_back(option_required);
  option_require_value_types_.push_back(NUM_OPT_VALUE_TYPES);
  option_description_.push_back(std::string(description));

  /* Register the name and short name in the name2id map */
  option_name2ids_[std::string(name)] = option;

  return option;
} 

/* Add a short name to an option */
bool Command::set_option_short_name(const CommandOptionId& option_id, 
                                    const char* short_name) {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));

  /* Short name is optional, so do the following only when it is not empty
   * Ensure that the short name is unique in the option list 
   */
  if (true == std::string(short_name).empty()) {
    return false;
  }

  std::map<std::string, CommandOptionId>::const_iterator short_name_it = option_short_name2ids_.find(std::string(short_name));
  if (short_name_it != option_short_name2ids_.end()) {
    return false;
  }

  option_short_names_[option_id] = std::string(short_name);

  /* Short name is optional, so register it only when it is not empty */
  option_short_name2ids_[std::string(short_name)] = option_id;

  return true;
} 

void Command::set_option_require_value(const CommandOptionId& option_id,
                                       const e_option_value_type& option_require_value_type) {
  /* Validate the option id */
  VTR_ASSERT(true == valid_option_id(option_id));

  option_require_value_types_[option_id] = option_require_value_type;
}

/************************************************************************
 * Public invalidators/validators 
 ***********************************************************************/
bool Command::valid_option_id(const CommandOptionId& option_id) const {
  return ( size_t(option_id) < option_ids_.size() ) && ( option_id == option_ids_[option_id] ); 
}

} /* End namespace openfpga */
