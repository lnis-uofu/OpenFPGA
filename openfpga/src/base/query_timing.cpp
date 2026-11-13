#include "query_timing.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include "AnalysisDelayCalculator.h"
#include "VprTimingGraphResolver.h"
#include "concrete_timing_info.h"
#include "globals.h"
#include "json_response.h"
#include "json_writer.h"
#include "net_delay.h"
#include "tatum/report/TimingPathCollector.hpp"
#include "timing_util.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* The timing of the routed design. The delay calculator refers to the net
 * delays, so they stay where they are */
struct RoutedTiming {
  NetPinsMatrix<float> net_delay;
  std::shared_ptr<AnalysisDelayCalculator> delay_calc;
  std::shared_ptr<SetupHoldTimingInfo> info;
};

/* Analyze the timing of the routed design, as the analysis of VPR does */
std::unique_ptr<RoutedTiming> analyze_routed_timing(const vpr::VprRun& run) {
  if (!run.setup.TimingEnabled) {
    VTR_LOG_ERROR(
      "The timing analysis is turned off: set the option "
      "general.timing_analysis "
      "and run the flow again\n");
    return nullptr;
  }
  if (run.setup.RouterOpts.flat_routing) {
    VTR_LOG_ERROR("The timing queries do not support flat routing yet\n");
    return nullptr;
  }
  const auto& atom_ctx = g_vpr_ctx.atom();
  const Netlist<>& clb_nlist =
    (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
  if (g_vpr_ctx.routing().route_trees.empty()) {
    VTR_LOG_ERROR("The design has no routing: run the stage ROUTED first\n");
    return nullptr;
  }

  auto timing = std::make_unique<RoutedTiming>();
  timing->net_delay = make_net_pins_matrix<float>(clb_nlist);
  load_net_delay_from_routing(clb_nlist, timing->net_delay);
  timing->delay_calc = std::make_shared<AnalysisDelayCalculator>(
    atom_ctx.netlist(), atom_ctx.lookup(), timing->net_delay, false);
  timing->info = make_setup_hold_timing_info(
    timing->delay_calc, run.setup.AnalysisOpts.timing_update_type);
  timing->info->update();
  return timing;
}

/* The total negative slack of the setup paths that go from one clock to
 * another: what the sTNS of VPR adds up, for the paths of this pair */
double domain_pair_tns(const tatum::SetupTimingAnalyzer& setup,
                       const tatum::DomainId launch,
                       const tatum::DomainId capture) {
  double tns = 0.;
  for (const tatum::NodeId node : g_vpr_ctx.timing().graph->logical_outputs()) {
    for (const tatum::TimingTag& tag : setup.setup_slacks(node)) {
      if (tag.launch_clock_domain() == launch &&
          tag.capture_clock_domain() == capture && tag.time().value() < 0.) {
        tns += tag.time().value();
      }
    }
  }
  return tns;
}

void write_ns(JsonWriter& json, const char* name, const double seconds) {
  json.key(name).value(sec_to_nanosec(seconds));
}

/* The points of a part of a path, with the time since the point before,
 * starting from the arrival that is given. Return the last arrival */
double write_points(JsonWriter& json, const tatum::TimingSubPath& sub_path,
                    const double start_arrival,
                    const VprTimingGraphResolver& resolver) {
  const auto& atom_ctx = g_vpr_ctx.atom();
  double last_arrival = start_arrival;
  json.begin_array();
  for (const tatum::TimingPathElem& elem : sub_path.elements()) {
    const double arrival = elem.tag().time().value();
    json.begin_object();
    json.key("pin").value(resolver.node_name(elem.node()));
    json.key("type").value(resolver.node_type_name(elem.node()));
    const AtomPinId pin = atom_ctx.lookup().tnode_atom_pin(elem.node());
    json.key("block");
    if (pin.is_valid()) {
      json.value(
        atom_ctx.netlist().block_name(atom_ctx.netlist().pin_block(pin)));
    } else {
      json.null();
    }
    json.key("net");
    if (pin.is_valid() && atom_ctx.netlist().pin_net(pin).is_valid()) {
      json.value(atom_ctx.netlist().net_name(atom_ctx.netlist().pin_net(pin)));
    } else {
      json.null();
    }
    write_ns(json, "arrival_ns", arrival);
    write_ns(json, "delay_ns", arrival - last_arrival);
    json.end_object();
    last_arrival = arrival;
  }
  json.end_array();
  return last_arrival;
}

}  // End anonymous namespace

bool net_timing(const vpr::VprRun& run, const ClusterNetId net,
                double& criticality, double& slack_ns) {
  if (!run.setup.TimingEnabled || run.setup.RouterOpts.flat_routing ||
      g_vpr_ctx.routing().route_trees.empty()) {
    return false;
  }
  const auto& atom_ctx = g_vpr_ctx.atom();
  const AtomNetId atom_net = atom_ctx.lookup().atom_net(net);
  if (!atom_net.is_valid()) {
    return false;
  }
  std::unique_ptr<RoutedTiming> timing = analyze_routed_timing(run);
  if (nullptr == timing) {
    return false;
  }
  const auto& netlist = atom_ctx.netlist();
  criticality = 0.;
  double slack = std::numeric_limits<double>::infinity();
  for (const AtomPinId pin : netlist.net_sinks(atom_net)) {
    criticality =
      std::max<double>(criticality, timing->info->setup_pin_criticality(pin));
    slack = std::min<double>(slack, timing->info->setup_pin_slack(pin));
  }
  if (!std::isfinite(slack)) {
    return false;
  }
  slack_ns = sec_to_nanosec(slack);
  return true;
}

bool timing_summary_json(const vpr::VprRun& run, std::string& json_text) {
  std::unique_ptr<RoutedTiming> timing = analyze_routed_timing(run);
  if (nullptr == timing) {
    return false;
  }
  const auto& timing_ctx = g_vpr_ctx.timing();
  const tatum::TimingConstraints& constraints = *timing_ctx.constraints;
  const tatum::SetupTimingAnalyzer& setup = *timing->info->setup_analyzer();
  const tatum::HoldTimingAnalyzer& hold = *timing->info->hold_analyzer();

  const tatum::TimingPathInfo least_slack =
    find_least_slack_critical_path_delay(constraints, setup);
  const std::vector<tatum::TimingPathInfo> paths =
    tatum::find_critical_paths(*timing_ctx.graph, constraints, setup);

  JsonWriter json;
  begin_response(json, "timing_summary", TIMING_SUMMARY_SCHEMA_VERSION,
                 "ROUTED");
  json.begin_object();

  json.key("setup").begin_object();
  write_ns(json, "worst_negative_slack_ns",
           find_setup_worst_negative_slack(setup));
  write_ns(json, "total_negative_slack_ns",
           find_setup_total_negative_slack(setup));
  write_ns(json, "critical_path_delay_ns", least_slack.delay());
  json.key("fmax_mhz").value(sec_to_mhz(least_slack.delay()));
  json.key("critical_path").begin_object();
  json.key("launch_clock")
    .value(constraints.clock_domain_name(least_slack.launch_domain()));
  json.key("capture_clock")
    .value(constraints.clock_domain_name(least_slack.capture_domain()));
  json.end_object();
  json.end_object();

  json.key("hold").begin_object();
  write_ns(json, "worst_negative_slack_ns",
           find_hold_worst_negative_slack(hold));
  write_ns(json, "total_negative_slack_ns",
           find_hold_total_negative_slack(hold));
  json.end_object();

  json.key("clock_domains").begin_array();
  for (const tatum::TimingPathInfo& path : paths) {
    const bool intra = path.launch_domain() == path.capture_domain();
    json.begin_object();
    json.key("launch_clock")
      .value(constraints.clock_domain_name(path.launch_domain()));
    json.key("capture_clock")
      .value(constraints.clock_domain_name(path.capture_domain()));
    json.key("intra_domain").value(intra);
    write_ns(json, "critical_path_delay_ns", path.delay());
    write_ns(json, "worst_slack_ns", path.slack());
    write_ns(
      json, "total_negative_slack_ns",
      domain_pair_tns(setup, path.launch_domain(), path.capture_domain()));
    json.key("fmax_mhz");
    if (intra) {
      json.value(sec_to_mhz(path.delay()));
    } else {
      json.null();
    }
    json.end_object();
  }
  json.end_array();

  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

bool critical_paths_json(const vpr::VprRun& run, const size_t count,
                         std::string& json_text) {
  if (count < 1 || count > CRITICAL_PATHS_MAX) {
    VTR_LOG_ERROR("The number of paths must be between 1 and %zu, not %zu\n",
                  CRITICAL_PATHS_MAX, count);
    return false;
  }
  std::unique_ptr<RoutedTiming> timing = analyze_routed_timing(run);
  if (nullptr == timing) {
    return false;
  }
  const auto& timing_ctx = g_vpr_ctx.timing();
  const auto& atom_ctx = g_vpr_ctx.atom();
  const tatum::TimingConstraints& constraints = *timing_ctx.constraints;
  const tatum::SetupTimingAnalyzer& setup = *timing->info->setup_analyzer();

  const std::vector<tatum::TimingPath> paths =
    tatum::TimingPathCollector().collect_worst_setup_timing_paths(
      *timing_ctx.graph, setup, count);

  VprTimingGraphResolver resolver(atom_ctx.netlist(), atom_ctx.lookup(),
                                  g_vpr_ctx.device().arch->models,
                                  *timing_ctx.graph, *timing->delay_calc, false,
                                  g_vpr_ctx.placement().blk_loc_registry());

  JsonWriter json;
  begin_response(json, "critical_paths", CRITICAL_PATHS_SCHEMA_VERSION,
                 "ROUTED");
  json.begin_object();
  json.key("requested").value(static_cast<int64_t>(count));
  json.key("returned").value(static_cast<int64_t>(paths.size()));
  json.key("paths").begin_array();
  int index = 0;
  for (const tatum::TimingPath& path : paths) {
    const tatum::TimingPathInfo& info = path.path_info();
    json.begin_object();
    json.key("index").value(index++);
    json.key("launch_clock")
      .value(constraints.clock_domain_name(info.launch_domain()));
    json.key("capture_clock")
      .value(constraints.clock_domain_name(info.capture_domain()));
    json.key("startpoint").value(resolver.node_name(info.startpoint()));
    json.key("endpoint").value(resolver.node_name(info.endpoint()));
    write_ns(json, "slack_ns", info.slack());

    /* The path of the clock to the block that launches the data: the data
     * leaves that block when the clock gets there */
    json.key("clock_launch_path");
    double last_arrival =
      write_points(json, path.clock_launch_path(), 0., resolver);
    json.key("points");
    last_arrival =
      write_points(json, path.data_arrival_path(), last_arrival, resolver);
    json.key("clock_capture_path");
    write_points(json, path.clock_capture_path(), 0., resolver);
    write_ns(json, "data_arrival_ns", last_arrival);
    write_ns(json, "data_required_ns",
             path.data_required_element().tag().time().value());
    json.end_object();
  }
  json.end_array();
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

} /* End namespace openfpga */
