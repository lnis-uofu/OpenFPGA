#include "session_report_commands.h"

#include <functional>
#include <string>

#include "command_exit_codes.h"
#include "query_run.h"
#include "session_json.h"
#include "staged_flow.h"
#include "vpr_run.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

using Query = std::function<bool(const vpr::VprRun&, std::string&)>;

/* Make the answer of a report that reads the data of VPR, and give it: print
 * it, and hand it to the caller of the command */
int answer(const OpenfpgaContext& openfpga_ctx, const OpenFpgaStage& stage,
           const char* command, const Query& query) {
  if (!stage_ready(openfpga_ctx.session(), stage, command)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const vpr::VprRun* run = vpr::current_vpr_run();
  if (nullptr == run) {
    VTR_LOG_ERROR(
      "Cannot run '%s': there is no data of VPR. Run the flow first, e.g., "
      "with the commands that read the inputs and run the stages\n",
      command);
    return CMD_EXEC_FATAL_ERROR;
  }
  std::string json;
  if (!query(*run, json)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

int report_session_command(const OpenfpgaContext& openfpga_ctx, const Command&,
                           const CommandContext&) {
  const std::string json =
    session_json(openfpga_ctx.session(), vpr::current_vpr_run());
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

int report_run_command(const OpenfpgaContext& openfpga_ctx, const Command&,
                       const CommandContext&) {
  const bool in_flow = staged_flow_current(openfpga_ctx.session());
  return answer(openfpga_ctx, OpenFpgaStage::DEVICE, "report_run",
                [&](const vpr::VprRun& run, std::string& json) {
                  return run_report_json(openfpga_ctx.session(), in_flow, run,
                                         json);
                });
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

  add_query_command(shell, cmd_class, Command("report_run"),
                    "Answer with what each stage of the flow did: the clusters "
                    "of the packing, the cost of the placement, the wirelength "
                    "and iterations of the routing, and the time of each stage",
                    report_run_command);
}

} /* End namespace openfpga */
