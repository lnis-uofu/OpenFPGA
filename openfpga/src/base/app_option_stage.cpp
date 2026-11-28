#include "app_option_stage.h"

#include <map>
#include <stdexcept>

#include "vpr_option_table.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* The options of the plugins: their owner and their stage */
struct RegisteredOption {
  std::string owner;
  OpenFpgaStage stage;
};

std::map<std::string, RegisteredOption>& registered_options() {
  static std::map<std::string, RegisteredOption> options;
  return options;
}

/* The stage of the options of a group */
const std::map<std::string, OpenFpgaStage>& group_stages() {
  static const std::map<std::string, OpenFpgaStage> stages = {
    {"filename", OpenFpgaStage::DEVICE},
    {"general", OpenFpgaStage::DEVICE},
    {"atom", OpenFpgaStage::DEVICE},
    {"cluster", OpenFpgaStage::PACKED},
    {"aplace", OpenFpgaStage::PLACED},
    {"place", OpenFpgaStage::PLACED},
    {"tplace", OpenFpgaStage::PLACED},
    {"crr", OpenFpgaStage::DEVICE},
    {"router", OpenFpgaStage::ROUTED},
    {"trouter", OpenFpgaStage::ROUTED},
    {"analysis", OpenFpgaStage::ANALYZED},
    {"power", OpenFpgaStage::ANALYZED},
    {"build_fabric", OpenFpgaStage::REPACKED}};
  return stages;
}

/* The options whose stage is not the one of their group */
const std::map<std::string, OpenFpgaStage>& option_stages() {
  static const std::map<std::string, OpenFpgaStage> stages = {
    /* Packing */
    {"place.placer", OpenFpgaStage::PACKED},
    {"filename.net_file", OpenFpgaStage::PACKED},
    {"filename.write_block_usage", OpenFpgaStage::PACKED},
    {"aplace.appack_max_dist_th", OpenFpgaStage::PACKED},
    {"aplace.appack_unrelated_clustering_args", OpenFpgaStage::PACKED},
    {"aplace.appack_inter_die_gain_multiplier", OpenFpgaStage::PACKED},
    {"aplace.appack_gain_attenuation_fn", OpenFpgaStage::PACKED},
    /* Placement */
    {"filename.place_file", OpenFpgaStage::PLACED},
    {"filename.flat_place_file", OpenFpgaStage::PLACED},
    {"filename.write_initial_place_file", OpenFpgaStage::PLACED},
    {"filename.read_initial_place_file", OpenFpgaStage::PLACED},
    {"filename.read_flat_place_file", OpenFpgaStage::PLACED},
    {"filename.write_flat_place_file", OpenFpgaStage::PLACED},
    {"filename.write_legalized_flat_place_file", OpenFpgaStage::PLACED},
    {"filename.write_post_gp_flat_place_file", OpenFpgaStage::PLACED},
    {"filename.read_placement_delay_lookup", OpenFpgaStage::PLACED},
    {"filename.write_placement_delay_lookup", OpenFpgaStage::PLACED},
    /* The routing resource graph, which is made with the device when the
     * channel width is fixed */
    {"place.place_chan_width", OpenFpgaStage::DEVICE},
    {"router.route_chan_width", OpenFpgaStage::DEVICE},
    {"filename.read_rr_graph_file", OpenFpgaStage::DEVICE},
    {"filename.read_rr_edge_override_file", OpenFpgaStage::DEVICE},
    {"filename.write_rr_graph_file", OpenFpgaStage::DEVICE},
    /* Routing */
    {"filename.route_file", OpenFpgaStage::ROUTED},
    {"filename.read_router_lookahead", OpenFpgaStage::ROUTED},
    {"filename.read_intra_cluster_router_lookahead", OpenFpgaStage::ROUTED},
    {"filename.write_router_lookahead", OpenFpgaStage::ROUTED},
    {"filename.write_intra_cluster_router_lookahead", OpenFpgaStage::ROUTED}};
  return stages;
}

}  // End anonymous namespace

bool app_option_stage(const std::string& name, OpenFpgaStage& stage) {
  const auto registered = registered_options().find(name);
  if (registered != registered_options().end()) {
    stage = registered->second.stage;
    return true;
  }
  const auto option = option_stages().find(name);
  if (option != option_stages().end()) {
    stage = option->second;
    return true;
  }
  const size_t dot = name.find('.');
  if (std::string::npos == dot) {
    return false;
  }
  const auto group = group_stages().find(name.substr(0, dot));
  if (group == group_stages().end()) {
    return false;
  }
  stage = group->second;
  return true;
}

void register_app_option(const std::string& owner, const std::string& name,
                         const AppOptionValue& default_value,
                         const OpenFpgaStage& stage) {
  const std::string prefix = owner + ".";
  if (owner.empty() || name.size() <= prefix.size() ||
      0 != name.compare(0, prefix.size(), prefix)) {
    throw std::invalid_argument("the option '" + name +
                                "' does not start with '" + prefix + "'");
  }
  if (std::string::npos != name.find('.', prefix.size())) {
    throw std::invalid_argument("the option '" + name +
                                "' has more than one '.'");
  }
  const auto registered = registered_options().find(name);
  const bool known = 0 != app_option_defaults().count(name);
  if (known && (registered == registered_options().end() ||
                owner != registered->second.owner)) {
    throw std::invalid_argument("the option '" + name + "' is taken by " +
                                app_option_owner(name));
  }
  if (OpenFpgaStage::UNINIT == stage) {
    throw std::invalid_argument("the option '" + name +
                                "' does not tune a stage");
  }
  registered_options()[name] = RegisteredOption{owner, stage};
  add_app_option_default(name, default_value);
}

std::string app_option_owner(const std::string& name) {
  const auto registered = registered_options().find(name);
  if (registered != registered_options().end()) {
    return registered->second.owner;
  }
  if (0 == app_option_defaults().count(name)) {
    return "";
  }
  for (const VprOptionSpec& spec : vpr_option_specs()) {
    if (name == std::string(spec.group) + "." + spec.name) {
      return "vpr";
    }
  }
  return "openfpga";
}

} /* End namespace openfpga */
