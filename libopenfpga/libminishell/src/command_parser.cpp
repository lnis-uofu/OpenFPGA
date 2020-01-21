/********************************************************************
 * This file includes parser(s) to convert user's input from 
 * shell interface to a data structure CommandContext
 *
 * TODO: a strong dependency is that we use VTR logging system
 *******************************************************************/
#include <cstring>

#include "vtr_assert.h"
#include "vtr_log.h"
#include "command_parser.h"

/* Begin namespace minishell */
namespace minishell {

/********************************************************************
 * Try to find an option in the command and update the CommandContext if needed
 *******************************************************************/
static 
CommandOptionId parse_option(const std::string& argv,
                             const Command& cmd,
                             CommandContext& cmd_context) {
  CommandOptionId option_id = cmd.option(argv);
  /* Not found, error out */
  if (CommandOptionId::INVALID() == option_id) {
    VTR_LOG("Detect unknown option '%s'!\n",
            argv.c_str());
  }
  /* Found, update the CommandContext */
  cmd_context.set_option(cmd, option_id, true);

  return option_id;
}

/********************************************************************
 * Try to find a short option in the command
 * Update the CommandContext if needed
 *******************************************************************/
static 
CommandOptionId parse_short_option(const std::string& argv,
                                   const Command& cmd,
                                   CommandContext& cmd_context) {
  CommandOptionId option_id = cmd.short_option(argv);
  /* Not found, error out */
  if (CommandOptionId::INVALID() == option_id) {
    VTR_LOG("Detect unknown option '%s'!\n",
            argv.c_str());
  }
  /* Found, update the CommandContext */
  cmd_context.set_option(cmd, option_id, true);

  return option_id;
}

/********************************************************************
 * Main parser to convert user's input from 
 * shell interface to a data structure CommandContext
 *******************************************************************/
bool parse_command(const std::vector<std::string>& argv,
                   const Command& cmd,
                   CommandContext& cmd_context) {
  /* We at least expect 1 arguement, which is the command name itself */
  VTR_ASSERT(1 <= argv.size());

  /* Validate that the command name matches argv[0] */
  if (argv[0] == cmd.name()) { 
    VTR_LOG("Unexpected command name '%s'!",
            argv[0]);
    return false;
  }

  /* Start from argv[1], the 1st argv is programme name */
  for (size_t iarg = 1; iarg < argv.size(); ++iarg) {
    /* Option must start with dash */
    if (0 != strncmp("-", argv[iarg].c_str(), 1)) {
      VTR_LOG("Invalid option '%s'!",
              argv[iarg].c_str());
      return false;
    }
    /* First try to process a full option 
     * which always starts with double dash '--'
     */
    if (0 == strncmp("--", argv[iarg].c_str(), 2)) {
      /* See if there is a option defined in the command object
       * Note that the first two characters are skipped when searching the name  
       */
      CommandOptionId option_id = parse_option(argv[iarg].substr(2), cmd, cmd_context);

      if (CommandOptionId::INVALID() == option_id) {
        return false;
      }

      /* If the option requires a value, we visit the next argument */
      if (true == cmd.option_require_value(option_id)) {
        ++iarg;
        /* If this is the last arugment, we finish */
        if (iarg == argv.size()) {
          break;
        }
        cmd_context.set_option_value(cmd, option_id, argv[iarg]);
      }
      /* Finish this iteration here, we found something */
      continue;
    }

    /* Second try to process a short option 
     * which always starts with double dash '-'
     */
    if (0 == strncmp("-", argv[iarg].c_str(), 1)) {
      /* See if there is a option defined in the command object
       * Note that the first two characters are skipped when searching the name  
       */
      CommandOptionId option_id = parse_short_option(argv[iarg].substr(1), cmd, cmd_context);

      if (CommandOptionId::INVALID() == option_id) {
        return false;
      }

      /* If the option requires a value, we visit the next argument */
      if (true == cmd.option_require_value(option_id)) {
        ++iarg;
        /* If this is the last arugment, we finish */
        if (iarg == argv.size()) {
          break;
        }
        cmd_context.set_option_value(cmd, option_id, argv[iarg]);
      }
      /* Finish this iteration here, we found something */
      continue;
    }
  }

  /* Ensure that all the required options have been satisfied
   * If not, we echo the details about what are missing 
   */ 
  std::vector<CommandOptionId> missing_options = cmd_context.check_required_options(cmd);
  if (!missing_options.empty()) {
    for (const CommandOptionId& missing_opt : missing_options) {
      VTR_LOG("Required option '%s' is missing!",
              cmd.option_name(missing_opt));
    }
    return false;
  }

  std::vector<CommandOptionId> missing_value_options = cmd_context.check_required_option_values(cmd);
  if (!missing_value_options.empty()) {
    for (const CommandOptionId& missing_opt : missing_value_options) {
      VTR_LOG("Require value for option '%s' is missing!",
              cmd.option_name(missing_opt));
    }
    return false;
  }

  return true;
}

} /* End namespace minshell */
