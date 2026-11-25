#ifndef QUERY_COMMANDS_H
#define QUERY_COMMANDS_H

#include "openfpga_context.h"
#include "shell.h"

/********************************************************************
 * Commands that answer queries about the design, see query_timing.h and
 * query_design.h. Each prints its answer as JSON and gives it as the data of
 * the result, see json_response.h. They do not change anything, and need the
 * stage that holds what they read:
 * - timing_summary: ROUTED
 * - critical_paths --count <n>: ROUTED
 * - utilization: PACKED
 * - placement --type --prefix --limit: PLACED
 * - net_route --net <name>: ROUTED
 * - congestion --region_size <n>: ROUTED
 * - report_cluster --query <pattern> --stats --limit: PACKED
 * Outside the flow run stage by stage, e.g., after 'vpr', they read what the
 * data of VPR holds
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

void add_query_commands(openfpga::Shell<OpenfpgaContext>& shell);

} /* End namespace openfpga */

#endif
