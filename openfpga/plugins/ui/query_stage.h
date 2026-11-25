#ifndef QUERY_STAGE_H
#define QUERY_STAGE_H

#include <cstddef>
#include <string>

#include "openfpga_session.h"

/********************************************************************
 * The answer of the query report_stage: what can be changed for each stage of
 * the flow, from the table of the application options. It is bounded, so that
 * a model can ask "what can I change after place?" and get a short list. It
 * reads the session only: no data of VPR. The schema is
 * openfpga_flow/schemas/stage_info.schema.json
 *
 * stage_info (stage DEVICE), data:
 *   filter                {stage, changed, query}: what was asked (null if not)
 *   stages[]              the stages asked for, in the order of the flow:
 *     stage, requires (the stage before it, null for DEVICE), commands[] (the
 *     commands that reach it), reached, failed (the message, or null),
 *     dropped_by_change (the stages that are dropped when an option of this
 *     stage changes: this one and the ones that depend on it, in the order
 *     of the flow), options {total (of the stage), matched, returned,
 *     truncated, items[]: the entries of app_options}
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

constexpr int STAGE_INFO_SCHEMA_VERSION = 1;

/* An empty stage asks for all of them. The name is the one of a stage in any
 * case, or the command that runs it. Return false, and log why, if the stage
 * is not known or the limit is 0 */
bool stage_info_json(const OpenFpgaSession& session, const std::string& stage,
                     const bool changed_only, const std::string& query,
                     const size_t limit, std::string& json);

} /* End namespace openfpga */

#endif
