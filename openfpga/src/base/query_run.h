#ifndef QUERY_RUN_H
#define QUERY_RUN_H

#include <string>

#include "openfpga_session.h"
#include "vpr_run.h"

/********************************************************************
 * The answer of the query report_run: what each stage of the flow did, in
 * one place. It reads the data of VPR (the packing, the placement, the
 * routing), what the stages printed, and the time that each took. What only
 * the log of VPR says is null when the stage was not run by a command of
 * this session (its results were read from a file), or when the log does not
 * have it. The schema is openfpga_flow/schemas/run_report.schema.json
 *
 * run_report (stage DEVICE), data:
 *   stages[]     the stages that are reached, in the order of the flow:
 *                stage, seconds (null if it was not run by a command)
 *   packing      null until PACKED, then {clusters, atoms (blocks of the
 *                circuit), nets, blocks[]: type, count}
 *   placement    null until PLACED, then {half_perimeter (the sum over the
 *                nets of the half perimeter of the box around their blocks),
 *                cost, bb_cost, td_cost, estimated_critical_path_delay_ns,
 *                estimated_fmax_mhz, num_temperatures, swap_attempts}: the
 *                last six are from the log of the placement
 *   routing      null until ROUTED, then {channel_width, wirelength (the
 *                tiles that the wires span: VPR adds it up the same way),
 *wires, iterations, critical_path_delay_ns}: the last two are from the log of
 *the routing failed       {stage, message} of the stage that failed last, or
 *null
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

constexpr int RUN_REPORT_SCHEMA_VERSION = 1;

/* The stages that are reached are the ones of the session when the flow is
 * run stage by stage (in_flow), else the ones that have data in VPR */
bool run_report_json(const OpenFpgaSession& session, const bool in_flow,
                     const vpr::VprRun& run, std::string& json);

} /* End namespace openfpga */

#endif
