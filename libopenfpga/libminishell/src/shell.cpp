/*********************************************************************
 * Member functions for class Shell
 ********************************************************************/
#include "vtr_assert.h"
#include "shell.h"

/* Begin namespace minishell */
namespace minishell {

/*********************************************************************
 * Public constructors
 ********************************************************************/
template<class T>
Shell<T>::Shell(const char* name) {
  name_ = std::string(name);
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
template<class T>
std::string Shell<T>::name() const {
  return name_;
}

template<class T>
typename Shell<T>::shell_command_range Shell<T>::commands() const {
  return vtr::make_range(command_ids_.begin(), command_ids_.end());
}

template<class T>
ShellCommandId Shell<T>::command(const std::string& name) const {
  /* Ensure that the name is unique in the command list */
  std::map<std::string, ShellCommandId>::const_iterator name_it = command_name2ids_.find(name);
  if (name_it == command_name2ids_.end()) {
    return ShellCommandId::INVALID();
  }
  return command_name2ids_.at(name);
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
/* Add a command with it description */
template<class T>
ShellCommandId Shell<T>::add_command(const Command& cmd, const char* descr) {
  /* Ensure that the name is unique in the command list */
  std::map<std::string, ShellCommandId>::const_iterator name_it = command_name2ids_.find(std::string(cmd.name()));
  if (name_it != command_name2ids_.end()) {
    return ShellCommandId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  ShellCommandId shell_cmd = ShellCommandId(command_ids_.size());
  command_ids_.push_back(shell_cmd);
  commands_.emplace_back(cmd);
  command_contexts_.push_back(CommandContext(cmd));
  command_description_.push_back(descr);
  command_execute_functions_.emplace_back();
  command_dependencies_.emplace_back();

  /* Register the name in the name2id map */
  command_name2ids_[std::string(name)] = cmd.name();

  return shell_cmd;
} 

/************************************************************************
 * Public invalidators/validators 
 ***********************************************************************/
template<class T>
bool Shell<T>::valid_command_id(const ShellCommandId& cmd_id) const {
  return ( size_t(cmd_id) < command_ids_.size() ) && ( cmd_id == command_ids_[cmd_id] ); 
}

} /* End namespace minshell */
