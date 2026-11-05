#include "app_option_stage.h"

#include <map>

/* Begin namespace openfpga */
namespace openfpga {

namespace {

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
    {"crr", OpenFpgaStage::ROUTED},
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
    /* Routing */
    {"filename.route_file", OpenFpgaStage::ROUTED},
    {"filename.read_rr_graph_file", OpenFpgaStage::ROUTED},
    {"filename.read_rr_edge_override_file", OpenFpgaStage::ROUTED},
    {"filename.write_rr_graph_file", OpenFpgaStage::ROUTED},
    {"filename.read_router_lookahead", OpenFpgaStage::ROUTED},
    {"filename.read_intra_cluster_router_lookahead", OpenFpgaStage::ROUTED},
    {"filename.write_router_lookahead", OpenFpgaStage::ROUTED},
    {"filename.write_intra_cluster_router_lookahead", OpenFpgaStage::ROUTED}};
  return stages;
}

}  // End anonymous namespace

bool app_option_stage(const std::string& name, OpenFpgaStage& stage) {
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

} /* End namespace openfpga */
