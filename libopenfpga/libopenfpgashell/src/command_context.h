#ifndef COMMAND_CONTEXT_H
#define COMMAND_CONTEXT_H

#include <string>
#include "vtr_vector.h"
#include "command_fwd.h"
#include "command.h"

/* Begin namespace minishell */
namespace minishell {

/*********************************************************************
 * Data structure to stores parsing results for a command
 ********************************************************************/
class CommandContext {
  public:  /* Public constructor */
  /* Build a context based on the command information
   * This will allocate the internal data structure and assign default values 
   */
    CommandContext(const Command& command);
  public:  /* Public accessors */
    bool option_enable(const Command& command,
                       const CommandOptionId& option_id) const;
    std::string option_value(const Command& command,
                             const CommandOptionId& option_id) const;
    /* Check if all the required options have been enabled
     * Any thing wrong will be reported in the return vector
     *
     * Note:
     * This function is supposed to be called after parsing is completed
     */
    std::vector<CommandOptionId> check_required_options(const Command& command) const;

    /* Check all the values that are required by the options
     * Any thing wrong will be reported in the return vector
     *
     * Note:
     * This function is supposed to be called after parsing is completed
     */
    std::vector<CommandOptionId> check_required_option_values(const Command& command) const;
  public:  /* Public mutators */
    void set_option(const Command& command,
                    const CommandOptionId& option_id, const bool& status);
    void set_option_value(const Command& command,
                          const CommandOptionId& option_id, const std::string& value);
  private: /* Internal data */  
    /* Identify if the option is enabled or not */
    vtr::vector<CommandOptionId, bool> option_enabled_;  

    /* All the follow-up value of each option is stored as string,
     * It will be converted the data types by outside function 
     * according to the value types
     */
    vtr::vector<CommandOptionId, std::string> option_values_;  
};

} /* End namespace minshell */

#endif
