#ifndef SESSION_JSON_H
#define SESSION_JSON_H

#include <string>

#include "openfpga_session.h"
#include "vpr_run.h"

/********************************************************************
 * What the session of the flow that is run stage by stage knows, as the
 * answer of a query of the kind session, which is not read from a stage.
 * Its schema is openfpga_flow/schemas/session.schema.json
 *
 * data is
 *   reached   the stages that are valid, in the order of the flow. Empty if
 *             the data of VPR that the session worked with was replaced
 *   inputs    vpr_arch {file, layout}, openfpga_arch {file} and circuit
 *             {file}: where they were read from, as whole paths. null for an
 *             input that is not read, or has no file
 *   results   packing, placement and routing: the file of the results of a
 *             stage that is reached, which is the one VPR wrote, or the one
 *             that was read instead of running the stage. null for the
 *             others. The path is whole, from the current directory of the
 *             process
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* Version of the schema of the kind session */
constexpr int SESSION_SCHEMA_VERSION = 1;

/* The answer. run is the live run of VPR, which may be null */
std::string session_json(const OpenFpgaSession& session,
                         const vpr::VprRun* run);

} /* End namespace openfpga */

#endif
