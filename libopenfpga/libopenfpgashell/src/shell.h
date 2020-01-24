#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <map>
#include <vector>
#include <functional>

#include "vtr_vector.h"
#include "vtr_range.h"
#include "command.h"
#include "command_context.h"
#include "shell_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Data structure to define a working environment of a mini shell
 *
 * An example of how to use
 * -----------------------
 * // Create a new shell with a name of 'OpenFPGA' which use data structure OpenfpgaContext for data exchange
 * Shell<OpenfpgaContext> shell("OpenFPGA");
 *
 * // Add a command 'read_arch' with a description
 * ShellCommandId cmd_read_arch = shell.add_command("read_arch", "Command to read an architecture");
 * // Create a new category and classify the command to it
 * ShellCommandCategoryId cmd_category = shell.add_command_category("io");
 * shell.set_command_category(cmd_read_arch, cmd_category);
 *
 * // Add options to the command 'read_arch'
 * const Command* cmd_read_arch_obj = shell.command(cmd_read_arch);
 * // Detailed examples can be found in Command.h 
 * ...
 *
 * // Add execute function to the command 'read_arch'
 * // This is the function to be called when the command is used
 * // Assume that you have a function read_arch() which does the job of read architecture
 * // Note that the function read_arch() should return a void with only three arguments
 * //   void read_arch(T, const Command&, const CommandContext&);
 * // The first argument is the template type you set when define the shell
 * // The second argument is a read-only object storing the options for the command
 * // The third argument is a read-only object storing the parsing results for the command
 * shell.set_command_execute_function(cmd_read_arch, &read_arch); 
 * 
 * // Run a shell 
 * shell.run();
 *
 ********************************************************************/
template<class T>
class Shell {
  public: /* Types */
    typedef vtr::vector<ShellCommandId, ShellCommandId>::const_iterator shell_command_iterator;
    /* Create range */
    typedef vtr::Range<shell_command_iterator> shell_command_range;
    /* Enumeration of command types in a shell
     * Built-in commands have their own execute functions inside the shell
     */
    enum e_exec_func_type {
      CONST_STANDARD,
      STANDARD,
      CONST_SHORT,
      SHORT,
      BUILTIN,
      MACRO,
      NUM_EXEC_FUNC_TYPES
    };
  public: /* Constructor */
    Shell<T>(const char* name);
  public: /* Public accessors */
    std::string name() const;
    std::string title() const;
    shell_command_range commands() const;
    ShellCommandId command(const std::string& name) const;
    std::string command_description(const ShellCommandId& cmd_id) const;
    ShellCommandClassId command_class(const ShellCommandId& cmd_id) const;
    std::string command_class_name(const ShellCommandClassId& cmd_class_id) const;
    /* We force a read-only return objects for command and command context
     * because users should NOT modify any of them!
     */
    const Command& command(const ShellCommandId& cmd_id) const;
    const CommandContext& command_context(const ShellCommandId& cmd_id) const;
    std::vector<ShellCommandId> command_dependency(const ShellCommandId& cmd_id) const;
    std::vector<ShellCommandId> commands_by_class(const ShellCommandClassId& cmd_class_id) const;
  public: /* Public mutators */
    void add_title(const char* title);
    ShellCommandId add_command(const Command& cmd, const char* descr);
    void set_command_class(const ShellCommandId& cmd_id, const ShellCommandClassId& cmd_class_id);
    /* Link the execute function to a command
     * We support here three types of functions to be executed in the shell
     * 1. Standard function, including the data exchange <T> and commands
     * 2. Short function, including only the data exchange <T>
     * 3. Built-in function, including only the shell envoriment variables
     * 4. Marco function, which directly call a macro function without command parsing
     * Users just need to specify the function object and its type will be automatically inferred
     */
    void set_command_const_execute_function(const ShellCommandId& cmd_id,
                                      std::function<void(const T&, const Command&, const CommandContext&)> exec_func);
    void set_command_execute_function(const ShellCommandId& cmd_id,
                                      std::function<void(T&, const Command&, const CommandContext&)> exec_func);
    void set_command_const_execute_function(const ShellCommandId& cmd_id,
                                            std::function<void(const T&)> exec_func);
    void set_command_execute_function(const ShellCommandId& cmd_id,
                                      std::function<void(T&)> exec_func);
    void set_command_execute_function(const ShellCommandId& cmd_id,
                                      std::function<void()> exec_func);
    void set_command_execute_function(const ShellCommandId& cmd_id,
                                      std::function<int(int, char**)> exec_func);
    void set_command_dependency(const ShellCommandId& cmd_id,
                                const std::vector<ShellCommandId> cmd_dependency);
    ShellCommandClassId add_command_class(const char* name);
  public: /* Public validators */
    bool valid_command_id(const ShellCommandId& cmd_id) const;
    bool valid_command_class_id(const ShellCommandClassId& cmd_class_id) const;
  public: /* Public executors */
    /* Start the interactive mode, where users will type-in command by command */
    void run_interactive_mode(T& context);
    /* Start the script mode, where users provide a file which includes all the commands to run */
    void run_script_mode(const char* script_file_name, T& context);
    /* Print all the commands by their classes. This is actually the help desk */
    void print_commands() const;
    /* Quit the shell */
    void exit() const;
  private: /* Private executors */
    /* Execute a command, the command line is the user's input to launch a command
     * The common_context is the data structure to exchange data between commands
     */
    void execute_command(const char* cmd_line, T& common_context);
  private: /* Internal data */ 
    /* Name of the shell, this will appear in the interactive mode */
    std::string name_;

    /* Title of the shell, this will appear in the interactive mode as an introduction */
    std::string title_;

    /* Unique ids for each class of command */
    vtr::vector<ShellCommandClassId, ShellCommandClassId> command_class_ids_;  

    /* Names for each class of command */
    vtr::vector<ShellCommandClassId, std::string> command_class_names_;  

    /* Unique ids for each command */
    vtr::vector<ShellCommandId, ShellCommandId> command_ids_;  

    /* Objects for each command */
    vtr::vector<ShellCommandId, Command> commands_;  

    /* Parsing results for each command */
    vtr::vector<ShellCommandId, CommandContext> command_contexts_;  

    /* Description of the command, this is going to be printed out in the help desk */
    vtr::vector<ShellCommandId, std::string> command_description_;  

    /* Class ids for each command */
    vtr::vector<ShellCommandId, ShellCommandClassId> command_classes_;  

    /* Function pointers to execute each command
     * We support here three types of functions to be executed in the shell
     * 1. Standard function, including the data exchange <T> and commands
     * 2. Short function, including only the data exchange <T>
     * 3. Built-in function, including only the shell envoriment variables
     * 4. Marco function, which directly call a macro function without command parsing
     */
    vtr::vector<ShellCommandId, std::function<void(const T&, const Command&, const CommandContext&)>> command_const_execute_functions_;  
    vtr::vector<ShellCommandId, std::function<void(T&, const Command&, const CommandContext&)>> command_standard_execute_functions_;  
    vtr::vector<ShellCommandId, std::function<void(const T&)>> command_short_const_execute_functions_; 
    vtr::vector<ShellCommandId, std::function<void(T&)>> command_short_execute_functions_; 
    vtr::vector<ShellCommandId, std::function<void()>> command_builtin_execute_functions_;  
    vtr::vector<ShellCommandId, std::function<int(int, char**)>> command_macro_execute_functions_;  

    /* Type of execute functions for each command.
     * This is supposed to be an internal data ONLY 
     */
    vtr::vector<ShellCommandId, e_exec_func_type> command_execute_function_types_;  

    /* Dependency graph for different commands,
     * This helps the shell interface to check if a command need other commands to be run before its execution  
     */
    vtr::vector<ShellCommandId, std::vector<ShellCommandId>> command_dependencies_;  

    /* Fast name look-up */
    std::map<std::string, ShellCommandId> command_name2ids_;
    std::map<std::string, ShellCommandClassId> command_class2ids_;
    vtr::vector<ShellCommandClassId, std::vector<ShellCommandId>> commands_by_classes_;  
};

} /* End namespace openfpga */

/* Include the template implementation functions in the header file */
#include "shell.tpp"

#endif
