#ifndef STAGE_GUARD_H
#define STAGE_GUARD_H

#include <functional>

#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "openfpga_session.h"
#include "staged_flow.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Make a command follow the stages of the flow that is run stage by stage: it
 * is refused if the stage it requires is not valid, and the stage it reaches
 * is recorded when it succeeds (dropped when it fails). The commands of the
 * flow of 'vpr' are not changed, see stage_ready().
 *
 * - required: the stage that must be valid. UNINIT for none
 * - reaches: the stage that the command reaches. UNINIT for none
 *******************************************************************/
template <class T, class Exec>
std::function<int(T&, const Command&, const CommandContext&)> guard_stage(
  const char* command, const OpenFpgaStage& required,
  const OpenFpgaStage& reaches, Exec exec) {
  return [=](T& openfpga_ctx, const Command& cmd,
             const CommandContext& cmd_context) {
    OpenFpgaSession& session = openfpga_ctx.mutable_session();
    if (!stage_ready(session, required, command)) {
      return static_cast<int>(CMD_EXEC_FATAL_ERROR);
    }
    const int status = exec(openfpga_ctx, cmd, cmd_context);
    if (OpenFpgaStage::UNINIT != reaches) {
      if (CMD_EXEC_SUCCESS == status) {
        stage_done(session, reaches);
      } else {
        stage_failed(session, reaches);
      }
    }
    return status;
  };
}

/* Make link_openfpga_arch record the run of VPR that it links to */
template <class T, class Exec>
std::function<int(T&, const Command&, const CommandContext&)> record_link(
  Exec exec) {
  return [=](T& openfpga_ctx, const Command& cmd,
             const CommandContext& cmd_context) {
    const int status = exec(openfpga_ctx, cmd, cmd_context);
    if (CMD_EXEC_SUCCESS == status) {
      stage_linked(openfpga_ctx.mutable_session());
    }
    return status;
  };
}

/* Make a command that uses what link_openfpga_arch made check that it is still
 * valid */
template <class T, class Exec>
std::function<int(T&, const Command&, const CommandContext&)> require_link(
  const char* command, Exec exec) {
  return [=](T& openfpga_ctx, const Command& cmd,
             const CommandContext& cmd_context) {
    if (!linked_ready(openfpga_ctx.session(), command)) {
      return static_cast<int>(CMD_EXEC_FATAL_ERROR);
    }
    return static_cast<int>(exec(openfpga_ctx, cmd, cmd_context));
  };
}

} /* End namespace openfpga */

#endif
