#include "session_report_commands.h"

#include <string>

#include "command_exit_codes.h"
#include "session_json.h"
#include "vpr_run.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

int report_session_command(const OpenfpgaContext& openfpga_ctx, const Command&,
                           const CommandContext&) {
  const std::string json =
    session_json(openfpga_ctx.session(), vpr::current_vpr_run());
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

/* A command that does not change the data: its function is const */
void add_query_command(openfpga::Shell<OpenfpgaContext>& shell,
                       const ShellCommandClassId& cmd_class, const Command& cmd,
                       const char* description,
                       int (*exec)(const OpenfpgaContext&, const Command&,
                                   const CommandContext&)) {
  ShellCommandId id = shell.add_command(cmd, description);
  shell.set_command_class(id, cmd_class);
  shell.set_command_const_execute_function(id, exec);
}

}  // End anonymous namespace

void add_session_report_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Session reports");

  add_query_command(
    shell, cmd_class, Command("report_session"),
    "Answer with the stages that are reached, the files the inputs were read "
    "from and the files of the results of the stages",
    report_session_command);
}

} /* End namespace openfpga */
