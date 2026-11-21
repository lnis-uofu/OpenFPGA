#ifndef QUERY_TIMING_H
#define QUERY_TIMING_H

#include <cstddef>
#include <string>

#include "clustered_netlist_fwd.h"
#include "vpr_run.h"

/********************************************************************
 * The answers of the queries about timing, which are read from the routed
 * design. The timing is analyzed again from the routing, as VPR does in its
 * analysis, so the numbers are the ones of the analysis. The times are in ns
 * and the frequencies in MHz.
 *
 * Kind timing_summary (openfpga_flow/schemas/timing_summary.schema.json), data:
 *   setup.worst_negative_slack_ns, setup.total_negative_slack_ns
 *   setup.critical_path_delay_ns   delay of the path with the least slack
 *   setup.fmax_mhz                 1 / critical_path_delay_ns
 *   setup.critical_path            {launch_clock, capture_clock}
 *   hold.worst_negative_slack_ns, hold.total_negative_slack_ns
 *   clock_domains[]                one entry per pair of clocks that has a
 *                                  path: launch_clock, capture_clock,
 *                                  intra_domain, critical_path_delay_ns,
 *                                  worst_slack_ns, total_negative_slack_ns (of
 *                                  the paths of this pair), fmax_mhz (only for
 *                                  a path within one clock, null otherwise)
 *
 * Kind critical_paths (openfpga_flow/schemas/critical_paths.schema.json), data:
 *   requested, returned    the number of paths asked for, and found
 *   paths[]                the worst setup paths, worst first: index,
 *                          launch_clock, capture_clock, startpoint,
 *                          endpoint, slack_ns, clock_launch_path[] (the
 *                          clock to the block that launches the data),
 *                          points[] (the data path, in path order: pin, type,
 *                          block, net, arrival_ns, the time since the point
 *                          before, delay_ns: for the first point, since the
 *                          clock got to its block), clock_capture_path[] (the
 *                          clock to the block that captures it, empty if the
 *                          clock is ideal),
 *                          data_arrival_ns (the arrival of the last point,
 *                          which includes the delay of the clock to the
 *                          launching block), data_required_ns. As in the
 *                          reports of VPR, slack is required minus arrival
 *
 * Both return false, and log why, when the answer cannot be made, e.g., if
 * the timing analysis is turned off
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

constexpr int TIMING_SUMMARY_SCHEMA_VERSION = 1;
constexpr int CRITICAL_PATHS_SCHEMA_VERSION = 1;

/* The most paths a query returns */
constexpr size_t CRITICAL_PATHS_MAX = 100;

/* The criticality of the worst connection of a net of the packed design, and
 * its worst setup slack in ns, from the timing of the routed design. False,
 * without a message, if there is none: the timing analysis is off, the design
 * is not routed, or the net is not one of the circuit */
bool net_timing(const vpr::VprRun& run, const ClusterNetId net,
                double& criticality, double& slack_ns);

bool timing_summary_json(const vpr::VprRun& run, std::string& json);
bool critical_paths_json(const vpr::VprRun& run, const size_t count,
                         std::string& json);

} /* End namespace openfpga */

#endif
