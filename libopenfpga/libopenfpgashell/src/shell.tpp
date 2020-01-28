/*********************************************************************
 * Member functions for class Shell
 ********************************************************************/
#include <fstream>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_tokenizer.h"

/* Headers from readline library */
#include <readline/readline.h>
#include <readline/history.h>

/* Headers from openfpgashell library */
#include "command_parser.h"
#include "command_echo.h"

/* Begin namespace openfpga */
namespace openfpga {

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
std::string Shell<T>::title() const {
  return title_;
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

template<class T>
std::string Shell<T>::command_description(const ShellCommandId& cmd_id) const {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  return command_description_[cmd_id];
}

template<class T>
ShellCommandClassId Shell<T>::command_class(const ShellCommandId& cmd_id) const {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  return command_classes_[cmd_id];
}

template<class T>
std::string Shell<T>::command_class_name(const ShellCommandClassId& cmd_class_id) const {
  VTR_ASSERT(true == valid_command_class_id(cmd_class_id));
  return command_class_names_[cmd_class_id];
}

template<class T>
const Command& Shell<T>::command(const ShellCommandId& cmd_id) const {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  return commands_[cmd_id];
}

template<class T>
const CommandContext& Shell<T>::command_context(const ShellCommandId& cmd_id) const {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  return command_contexts_[cmd_id];
}

template<class T>
std::vector<ShellCommandId> Shell<T>::command_dependency(const ShellCommandId& cmd_id) const {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  return command_dependencies_[cmd_id];
}

template<class T>
std::vector<ShellCommandId> Shell<T>::commands_by_class(const ShellCommandClassId& cmd_class_id) const {
  VTR_ASSERT(true == valid_command_class_id(cmd_class_id));
  return commands_by_classes_[cmd_class_id];
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
template<class T>
void Shell<T>::add_title(const char* title) {
  title_ = std::string(title);
}

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
  command_classes_.push_back(ShellCommandClassId::INVALID());
  command_execute_function_types_.emplace_back();
  command_const_execute_functions_.emplace_back();
  command_standard_execute_functions_.emplace_back();
  command_short_const_execute_functions_.emplace_back();
  command_short_execute_functions_.emplace_back();
  command_builtin_execute_functions_.emplace_back();
  command_macro_execute_functions_.emplace_back();
  command_status_.push_back(false); /* By default, the command should be marked as never executed */
  command_dependencies_.emplace_back();

  /* Register the name in the name2id map */
  command_name2ids_[cmd.name()] = shell_cmd;

  return shell_cmd;
} 

template<class T>
void Shell<T>::set_command_class(const ShellCommandId& cmd_id, const ShellCommandClassId& cmd_class_id) {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  VTR_ASSERT(true == valid_command_class_id(cmd_class_id));
  command_classes_[cmd_id] = cmd_class_id;
  /* Update the fast look-up to spot commands in a class */
  std::vector<ShellCommandId>::iterator it = std::find(commands_by_classes_[cmd_class_id].begin(), commands_by_classes_[cmd_class_id].end(), cmd_id);
  /* The command does not exist in the class, add it */
  if (it == commands_by_classes_[cmd_class_id].end()) {
    commands_by_classes_[cmd_class_id].push_back(cmd_id);
  }
}

template<class T>
void Shell<T>::set_command_const_execute_function(const ShellCommandId& cmd_id, 
                                                  std::function<void(const T&, const Command&, const CommandContext&)> exec_func) {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  command_execute_function_types_[cmd_id] = CONST_STANDARD;
  command_const_execute_functions_[cmd_id] = exec_func;
}

template<class T>
void Shell<T>::set_command_execute_function(const ShellCommandId& cmd_id, 
                                            std::function<void(T&, const Command&, const CommandContext&)> exec_func) {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  command_execute_function_types_[cmd_id] = STANDARD;
  command_standard_execute_functions_[cmd_id] = exec_func;
}

template<class T>
void Shell<T>::set_command_const_execute_function(const ShellCommandId& cmd_id, 
                                                  std::function<void(const T&)> exec_func) {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  command_execute_function_types_[cmd_id] = CONST_SHORT;
  command_short_const_execute_functions_[cmd_id] = exec_func;
}

template<class T>
void Shell<T>::set_command_execute_function(const ShellCommandId& cmd_id, 
                                            std::function<void(T&)> exec_func) {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  command_execute_function_types_[cmd_id] = SHORT;
  command_short_execute_functions_[cmd_id] = exec_func;
}

template<class T>
void Shell<T>::set_command_execute_function(const ShellCommandId& cmd_id, 
                                            std::function<void()> exec_func) {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  command_execute_function_types_[cmd_id] = BUILTIN;
  command_builtin_execute_functions_[cmd_id] = exec_func;
}

template<class T>
void Shell<T>::set_command_execute_function(const ShellCommandId& cmd_id, 
                                            std::function<int(int, char**)> exec_func) {
  VTR_ASSERT(true == valid_command_id(cmd_id));
  command_execute_function_types_[cmd_id] = MACRO;
  command_macro_execute_functions_[cmd_id] = exec_func;
}

template<class T>
void Shell<T>::set_command_dependency(const ShellCommandId& cmd_id,
                                      const std::vector<ShellCommandId>& dependent_cmds) {
  /* Validate the command id as well as each of the command dependency */
  VTR_ASSERT(true == valid_command_id(cmd_id));
  for (ShellCommandId dependent_cmd : dependent_cmds) {
    VTR_ASSERT(true == valid_command_id(dependent_cmd));
  }
  command_dependencies_[cmd_id] = dependent_cmds;
}

/* Add a command with it description */
template<class T>
ShellCommandClassId Shell<T>::add_command_class(const char* name) {
  /* Ensure that the name is unique in the command list */
  std::map<std::string, ShellCommandClassId>::const_iterator name_it = command_class2ids_.find(std::string(name));
  if (name_it != command_class2ids_.end()) {
    return ShellCommandClassId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  ShellCommandClassId cmd_class = ShellCommandClassId(command_class_ids_.size());
  command_class_ids_.push_back(cmd_class);
  command_class_names_.push_back(std::string(name));

  /* Register the name in the name2id map */
  command_class2ids_[std::string(name)] = cmd_class;

  /* Register in the fast look-up for commands by classes */
  commands_by_classes_.emplace_back();

  return cmd_class;
} 

/************************************************************************
 * Public executors
 ***********************************************************************/
template <class T>
void Shell<T>::run_interactive_mode(T& context) {
  VTR_LOG("Start interactive mode of %s...\n",
          name().c_str());

  /* Print the title of the shell */
  if (!title().empty()) {
    VTR_LOG("%s\n", title().c_str());
  }

  /* Wait for users input and execute the command */
  char* cmd_line;
  while ((cmd_line = readline(std::string(name() + std::string("> ")).c_str())) != nullptr) {
    /* If the line is not empty:
     * Try to execute the command and
     * Add to history 
     */
    if (strlen(cmd_line) > 0) {
      execute_command((const char*)cmd_line, context);
      add_history(cmd_line);
    }

    /* Free the line as readline malloc a new line each time */
    free(cmd_line);
  }
}

template <class T>
void Shell<T>::run_script_mode(const char* script_file_name, T& context) {

  VTR_LOG("Reading script file %s...\n", script_file_name);

  /* Print the title of the shell */
  if (!title().empty()) {
    VTR_LOG("%s\n", title().c_str());
  } 

  std::string line;

  /* Create an input file stream */
  std::ifstream fp(script_file_name);

  if (!fp.is_open()) {
    /* Fail to open the file, ask user to check */
    VTR_LOG("Fail to open the script file: %s! Please check its location\n",
            script_file_name);
    return; 
  }

  /* Read line by line */
  while (getline(fp, line)) {
    /* If the line that starts with '#', it is commented, we can skip */ 
    if ('#' == line.front()) {
      continue;
    }
    /* Try to split the line with '#', the string before '#' is the read command we want */
    std::string cmd_part = line;
    std::size_t cmd_end_pos = line.find_first_of('#');
    /* If the full line has '#', we need the part before it */
    if (cmd_end_pos != std::string::npos) {
      cmd_part = line.substr(0, cmd_end_pos);
    }
    /* Process the command only when the line is not empty */
    if (!cmd_part.empty()) {
      execute_command(cmd_part.c_str(), context);
    }
  }
  fp.close();
}

template <class T>
void Shell<T>::print_commands() const {
  /* Print the commands by their classes */
  for (const ShellCommandClassId& cmd_class : command_class_ids_) {
    /* Print the class name */
    VTR_LOG("%s:\n", command_class_names_[cmd_class].c_str());

    size_t cnt = 0;
    for (const ShellCommandId& cmd : commands_by_classes_[cmd_class]) {
      /* Print the command names in this class
       * but limited4 command per line for a clean layout
       */
      VTR_LOG("%s", commands_[cmd].name().c_str());
      cnt++;
      if (4 == cnt) {
        VTR_LOG("\n");
        cnt = 0;
      } else {
        VTR_LOG("\t");
      } 
    }

    /* Put a new line in the end as a splitter */
    VTR_LOG("\n");
  }

  /* Put a new line in the end as a splitter */
  VTR_LOG("\n");
}

template <class T>
void Shell<T>::exit() const {
  VTR_LOG("\nThank you for using %s!\n",
          name().c_str());
  std::exit(0);
}

/************************************************************************
 * Private executors
 ***********************************************************************/
template <class T>
void Shell<T>::execute_command(const char* cmd_line,
                               T& common_context) {
  /* Tokenize the line */
  openfpga::StringToken tokenizer(cmd_line);  
  std::vector<std::string> tokens = tokenizer.split(" ");

  /* Find if the command name is valid */
  ShellCommandId cmd_id = command(tokens[0]);
  if (ShellCommandId::INVALID() == cmd_id) {
    VTR_LOG("Try to call a command '%s' which is not defined!\n",
            tokens[0].c_str());
    return;
  }

  /* Check the dependency graph to see if all the prequistics have been met */
  for (const ShellCommandId& dep_cmd : command_dependencies_[cmd_id]) {
    if (false == command_status_[dep_cmd]) {
      VTR_LOG("Command '%s' is required to be executed before command '%s'!\n",
              commands_[dep_cmd].name().c_str(), commands_[cmd_id].name().c_str());
      return;
    } 
  }

  /* Find the command! Parse the options 
   * Note:
   * Macro command will not be parsed! It will be directly executed
   */
  if (MACRO == command_execute_function_types_[cmd_id]) {
    /* Convert the tokens from string to char */
    char** argv = (char**)malloc(tokens.size() * sizeof(char*));
    for (size_t itok = 0; itok < tokens.size(); ++itok) {
      argv[itok] = (char*)malloc((tokens[itok].length() + 1) * sizeof(char));
      strcpy(argv[itok], tokens[itok].c_str());
    }
    /* Execute the marco function */
    command_macro_execute_functions_[cmd_id](tokens.size(), argv);
    /* Free the argv */
    for (size_t itok = 0; itok < tokens.size(); ++itok) {
      free(argv[itok]);
    }
    free(argv);

    /* Change the status of the command */
    command_status_[cmd_id] = true;

    /* Finish for macro command, return */
    return;
  }
 
  if (false == parse_command(tokens, commands_[cmd_id], command_contexts_[cmd_id])) {
    /* Echo the command */
    print_command_options(commands_[cmd_id]);
    return;
  }
 
  /* Parse succeed. Let user to confirm selected options */ 
  print_command_context(commands_[cmd_id], command_contexts_[cmd_id]);

  /* Execute the command depending on the type of function ! */ 
  switch (command_execute_function_types_[cmd_id]) {
  case CONST_STANDARD:
    command_const_execute_functions_[cmd_id](common_context, commands_[cmd_id], command_contexts_[cmd_id]);
    break;
  case STANDARD:
    command_standard_execute_functions_[cmd_id](common_context, commands_[cmd_id], command_contexts_[cmd_id]);
    break;
  case CONST_SHORT:
    command_short_const_execute_functions_[cmd_id](common_context);
    break;
  case SHORT:
    command_short_execute_functions_[cmd_id](common_context);
    break;
  case BUILTIN:
    command_builtin_execute_functions_[cmd_id]();
    break;
  /* MACRO should be executed eariler in this function. It should not appear here */
  default:
    /* This is not allowed! Error out */
    VTR_LOG("Invalid type of execute function for command '%s'!\n",
            commands_[cmd_id].name().c_str());
    /* Exit the shell using the exit() function inside this class! */
    exit();
  }

  /* Change the status of the command */
  command_status_[cmd_id] = true;
}

/************************************************************************
 * Public invalidators/validators 
 ***********************************************************************/
template<class T>
bool Shell<T>::valid_command_id(const ShellCommandId& cmd_id) const {
  return ( size_t(cmd_id) < command_ids_.size() ) && ( cmd_id == command_ids_[cmd_id] ); 
}

template<class T>
bool Shell<T>::valid_command_class_id(const ShellCommandClassId& cmd_class_id) const {
  return ( size_t(cmd_class_id) < command_class_ids_.size() ) && ( cmd_class_id == command_class_ids_[cmd_class_id] ); 
}

} /* End namespace openfpga */
