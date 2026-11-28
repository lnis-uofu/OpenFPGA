#ifndef QUERY_ANSWER_H
#define QUERY_ANSWER_H
/********************************************************************
 * Helpers of the commands that answer in JSON (see json_response.h), for
 * the reports of the shell and for the commands that plugins add: run a
 * query on the data of VPR, give its answer to the caller, and read the
 * options that such commands share
 *******************************************************************/
#include <cstddef>
#include <functional>
#include <string>

#include "command.h"
#include "command_context.h"
#include "openfpga_context.h"
#include "shell.h"
#include "vpr_run.h"

/* begin namespace openfpga */
namespace openfpga {

/* The most items a list of an answer has, unless a limit is given */
constexpr size_t QUERY_LIST_DEFAULT_LIMIT = 1000;

/* Write the JSON of an answer from the data of a VPR run. Return false (with
 * the reason logged) if it cannot be made */
using QueryFunction = std::function<bool(const vpr::VprRun&, std::string&)>;

/* Check the stage (unless !stage_is_needed), run the query on the live VPR
 * run, print the JSON and hand it to the caller (set_query_result). Returns
 * a CMD_EXEC_* code */
int answer_query(const OpenfpgaContext& ctx, const OpenFpgaStage& stage,
                 const char* command, const QueryFunction& query,
                 const bool stage_is_needed = true);

/* True if the data of a stage is there to read. Outside of the flow that is
 * run stage by stage, e.g., after the command vpr, whatever VPR holds is
 * read, and the query finds out from the data if there is any */
bool stage_data(const OpenfpgaContext& ctx, const OpenFpgaStage& stage);

/* The value of an option that is a number of items, from 1 up. The default
 * is used if the option is not given. Return false (and log) if the value is
 * not a whole number of at least 1 */
bool count_option(const Command& cmd, const CommandContext& cmd_context,
                  const char* name, const size_t default_value, size_t& count);

/* The value of an option, or "" if it is not given */
std::string string_option(const Command& cmd, const CommandContext& cmd_context,
                          const char* name);

/* Add an option that takes a value, which is optional unless required is
 * set. short_name may be null */
void add_value_option(Command& cmd, const char* name, const char* short_name,
                      const bool required, const e_option_value_type type,
                      const char* help);

/* Register a read-only command (const execute function) in a class */
void add_query_command(Shell<OpenfpgaContext>& shell,
                       const ShellCommandClassId& cmd_class, const Command& cmd,
                       const char* description,
                       int (*exec)(const OpenfpgaContext&, const Command&,
                                   const CommandContext&));

} /* end namespace openfpga */

#endif
