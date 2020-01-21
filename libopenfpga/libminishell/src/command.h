#ifndef COMMAND_H
#define COMMAND_H

/*********************************************************************
 * This header file includes data structure for option reading
 * This is a cornerstone data structure of mini shell
 * which aims to store command-line options for parser
 *
 * This data structue is design to read-in the command line options 
 * which are organized as follows:
 *
 *   <command_name> --<command_option_1> <command_option_1_value>
 *   --<command_option_2> ...
 *
 * This is not only used by each command available in the shell
 * but also the interface of the shell, such as interactive mode
 ********************************************************************/
#include <string>
#include <map>
#include <vector>

#include "vtr_vector.h"
#include "vtr_range.h"
#include "command_fwd.h"

/* Begin namespace minishell */
namespace minishell {

/*********************************************************************
 * Supported date types of value which is followed by a command-line option
 ********************************************************************/
enum e_option_value_type {
  OPT_INT,
  OPT_FLOAT,
  OPT_STRING,
  NUM_OPT_VALUE_TYPES
};

/*********************************************************************
 * Data structure to stores all the information related to a command 
 * to be defined in the mini shell
 * This data structure should NOT contain any parsing results!
 * It should be a read-only once created!
 *
 * Note:
 * When adding an option name, please do NOT add any dash at the beginning!!!
 ********************************************************************/
class Command {
  public: /* Types */
    typedef vtr::vector<CommandOptionId, CommandOptionId>::const_iterator command_option_iterator;
    /* Create range */
    typedef vtr::Range<command_option_iterator> command_option_range;
  public: /* Constructor */
    Command(const char* name);
  public: /* Public accessors */
    std::string name() const;
    command_option_range options() const;
    /* Find all the options that are mandatory */
    std::vector<CommandOptionId> required_options() const;
    /* Find all the options that require a value */
    std::vector<CommandOptionId> require_value_options() const;
    CommandOptionId option(const std::string& name) const;
    CommandOptionId short_option(const std::string& name) const;
    std::string option_name(const CommandOptionId& option_id) const;
    std::string option_short_name(const CommandOptionId& option_id) const;
    bool option_required(const CommandOptionId& option_id) const;
    bool option_require_value(const CommandOptionId& option_id) const;
    e_option_value_type option_require_value_type(const CommandOptionId& option_id) const;
    std::string option_description(const CommandOptionId& option_id) const;
  public: /* Public mutators */
    CommandOptionId add_option(const char* name,
                               const bool& option_required,
                               const char* description); 
    bool set_option_short_name(const CommandOptionId& option_id, const char* short_name);
    void set_option_require_value(const CommandOptionId& option_id,
                                  const e_option_value_type& option_require_value_type);
  public: /* Public validators */
    bool valid_option_id(const CommandOptionId& option_id) const;
  private: /* Internal data */ 
    /* The name of the command */
    std::string name_;

    vtr::vector<CommandOptionId, CommandOptionId> option_ids_;  

    /* Information about the options available in this command */
    /* Name of command line option to appear in the user interface 
     * Regular names are typically with a prefix of double dash '--'  
     * Short names are typically with a prefix of single dash '-'  
     */
    vtr::vector<CommandOptionId, std::string> option_names_;  
    vtr::vector<CommandOptionId, std::string> option_short_names_;  

    /* If the option is manadatory when parsing */
    vtr::vector<CommandOptionId, bool> option_required_;  

    /* Data type of the option values to be converted
     * If the option does NOT require a value, this will be an invalid type 
     */
    vtr::vector<CommandOptionId, e_option_value_type> option_require_value_types_; 

    /* Description of the option, this is going to be printed out in the help desk */
    vtr::vector<CommandOptionId, std::string> option_description_;  

    /* Fast name look-up */
    std::map<std::string, CommandOptionId> option_name2ids_;
    std::map<std::string, CommandOptionId> option_short_name2ids_;
};

} /* End namespace minshell */

#endif
