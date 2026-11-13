#include "query_run.h"

#include <algorithm>
#include <limits>
#include <map>
#include <optional>
#include <regex>
#include <vector>

#include "globals.h"
#include "json_response.h"
#include "json_writer.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* The number that a pattern finds in a log, the last time it does: what the
 * stage settled on. The first group is the number */
std::optional<double> log_number(const std::string* log,
                                 const std::regex& pattern,
                                 const size_t group = 1) {
  if (nullptr == log) {
    return std::nullopt;
  }
  std::optional<double> found;
  for (std::sregex_iterator it(log->begin(), log->end(), pattern), end;
       it != end; ++it) {
    if ((*it)[group].matched) {
      found = std::stod((*it)[group].str());
    }
  }
  return found;
}

void write_optional(JsonWriter& json, const char* name,
                    const std::optional<double>& value) {
  json.key(name);
  value.has_value() ? json.value(*value) : json.null();
}

void write_optional_count(JsonWriter& json, const char* name,
                          const std::optional<double>& value) {
  json.key(name);
  value.has_value() ? json.value(static_cast<int64_t>(*value)) : json.null();
}

const char* const NUMBER = "([-+0-9.eE]+|nan|inf)";

}  // End anonymous namespace

bool run_report_json(const OpenFpgaSession& session, const bool in_flow,
                     const vpr::VprRun& run, std::string& json_text) {
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const auto& block_locs = g_vpr_ctx.placement().block_locs();
  const auto& route_ctx = g_vpr_ctx.routing();
  const auto& device_ctx = g_vpr_ctx.device();
  const auto& rr_graph = device_ctx.rr_graph;

  auto has = [&](const OpenFpgaStage& stage, const bool data) {
    return in_flow ? session.reached(stage) : data;
  };
  const bool packed = has(OpenFpgaStage::PACKED, clb_nlist.blocks().size() > 0);
  const bool placed =
    has(OpenFpgaStage::PLACED, packed && block_locs.size() > 0);
  const bool routed =
    has(OpenFpgaStage::ROUTED, placed && !route_ctx.route_trees.empty());

  JsonWriter json;
  begin_response(json, "run_report", RUN_REPORT_SCHEMA_VERSION, "DEVICE");
  json.begin_object();

  json.key("stages").begin_array();
  const std::pair<OpenFpgaStage, bool> stages[] = {
    {OpenFpgaStage::PACKED, packed},
    {OpenFpgaStage::PLACED, placed},
    {OpenFpgaStage::ROUTED, routed},
    {OpenFpgaStage::ANALYZED,
     in_flow && session.reached(OpenFpgaStage::ANALYZED)}};
  for (const auto& entry : stages) {
    if (!entry.second) {
      continue;
    }
    json.begin_object();
    json.key("stage").value(to_string(entry.first));
    json.key("seconds");
    const double* seconds =
      in_flow ? session.stage_seconds(entry.first) : nullptr;
    seconds != nullptr ? json.value(*seconds) : json.null();
    json.end_object();
  }
  json.end_array();

  json.key("packing");
  if (packed) {
    std::map<std::string, int64_t> by_type;
    for (const ClusterBlockId blk : clb_nlist.blocks()) {
      ++by_type[clb_nlist.block_type(blk)->name];
    }
    json.begin_object();
    json.key("clusters").value(static_cast<int64_t>(clb_nlist.blocks().size()));
    json.key("atoms").value(
      static_cast<int64_t>(g_vpr_ctx.atom().netlist().blocks().size()));
    json.key("nets").value(static_cast<int64_t>(clb_nlist.nets().size()));
    json.key("blocks").begin_array();
    for (const auto& entry : by_type) {
      json.begin_object();
      json.key("type").value(entry.first);
      json.key("count").value(entry.second);
      json.end_object();
    }
    json.end_array();
    json.end_object();
  } else {
    json.null();
  }

  json.key("placement");
  if (placed) {
    int64_t half_perimeter = 0;
    for (const ClusterNetId net : clb_nlist.nets()) {
      if (clb_nlist.net_is_ignored(net)) {
        continue;
      }
      int xmin = std::numeric_limits<int>::max(), ymin = xmin;
      int xmax = -1, ymax = -1;
      for (const ClusterPinId pin : clb_nlist.net_pins(net)) {
        const ClusterBlockId blk = clb_nlist.pin_block(pin);
        if (size_t(blk) >= block_locs.size()) {
          continue;
        }
        const t_pl_loc& loc = block_locs[blk].loc;
        xmin = std::min(xmin, loc.x);
        ymin = std::min(ymin, loc.y);
        xmax = std::max(xmax, loc.x);
        ymax = std::max(ymax, loc.y);
      }
      if (xmax >= 0) {
        half_perimeter += (xmax - xmin) + (ymax - ymin);
      }
    }
    const std::string* log =
      in_flow ? session.stage_log(OpenFpgaStage::PLACED) : nullptr;
    const std::string number = NUMBER;
    json.begin_object();
    json.key("half_perimeter").value(half_perimeter);
    write_optional(
      json, "cost",
      log_number(log, std::regex("Placement cost: " + number + ", bb_cost: ")));
    write_optional(json, "bb_cost",
                   log_number(log, std::regex("Placement cost: .*?, bb_cost: " +
                                              number + ", td_cost: ")));
    write_optional(
      json, "td_cost",
      log_number(log, std::regex("Placement cost: .*?, td_cost: " + number)));
    write_optional(
      json, "estimated_critical_path_delay_ns",
      log_number(log,
                 std::regex("Placement estimated critical path delay \\(least "
                            "slack\\): " +
                            number + " ns")));
    write_optional(
      json, "estimated_fmax_mhz",
      log_number(log, std::regex("Placement estimated critical path delay "
                                 "\\(least slack\\): .*? ns, Fmax: " +
                                 number + " MHz")));
    write_optional_count(
      json, "num_temperatures",
      log_number(log, std::regex("Placement number of temperatures: (\\d+)")));
    write_optional_count(
      json, "swap_attempts",
      log_number(log,
                 std::regex("Placement total # of swap attempts: (\\d+)")));
    json.end_object();
  } else {
    json.null();
  }

  json.key("routing");
  if (routed) {
    int64_t wires = 0, wirelength = 0;
    for (const auto& tree : route_ctx.route_trees) {
      if (!tree) {
        continue;
      }
      for (const RouteTreeNode& node : tree->all_nodes()) {
        const e_rr_type type = rr_graph.node_type(node.inode);
        if (type == e_rr_type::CHANX || type == e_rr_type::CHANY) {
          ++wires;
          wirelength +=
            (rr_graph.node_xhigh(node.inode) - rr_graph.node_xlow(node.inode)) +
            (rr_graph.node_yhigh(node.inode) - rr_graph.node_ylow(node.inode)) +
            1;
        }
      }
    }
    const std::string* log =
      in_flow ? session.stage_log(OpenFpgaStage::ROUTED) : nullptr;
    json.begin_object();
    json.key("channel_width")
      .value(
        std::max(device_ctx.chan_width.x_max, device_ctx.chan_width.y_max));
    json.key("wirelength").value(wirelength);
    json.key("wires").value(wires);
    write_optional_count(
      json, "iterations",
      log_number(log, std::regex("Successfully routed after (\\d+) routing "
                                 "iterations")));
    write_optional(
      json, "critical_path_delay_ns",
      log_number(log,
                 std::regex("Critical path: " + std::string(NUMBER) + " ns")));
    json.end_object();
  } else {
    json.null();
  }

  json.key("failed");
  bool wrote = false;
  if (in_flow) {
    for (const OpenFpgaStage& stage :
         {OpenFpgaStage::ROUTED, OpenFpgaStage::PLACED,
          OpenFpgaStage::PACKED}) {
      if (const std::string* failure = session.failure(stage)) {
        json.begin_object();
        json.key("stage").value(to_string(stage));
        json.key("message").value(*failure);
        json.end_object();
        wrote = true;
        break;
      }
    }
  }
  if (!wrote) {
    json.null();
  }
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

} /* End namespace openfpga */
