#include "mif_vpr_placement.h"

#include <string>
#include <vector>

#include "atom_netlist.h"
#include "globals.h"
#include "openfpga_pb_parser.h"
#include "pb_type_utils.h"
#include "vpr_types.h"
#include "vtr_log.h"

namespace openfpga {

/* Convert a packed leaf pb into the path format used by mif_source:
 * parent_pb_type[mode]....leaf_pb_type[placement_index]. */
static std::string generate_mif_pb_path(const t_pb* leaf_pb) {
  std::vector<std::string> path;

  /* Walk from the packed leaf to the root pb. */
  for (const t_pb* pb = leaf_pb; pb != nullptr; pb = pb->parent_pb) {
    if (pb->pb_graph_node == nullptr) {
      return std::string();
    }

    const t_pb_type* pb_type = pb->pb_graph_node->pb_type;
    std::string component(pb_type->name);
    if (pb->is_primitive()) {
      component +=
        "[" + std::to_string(pb->pb_graph_node->placement_index) + "]";
    } else if (!pb->is_primitive()) {
      component += "[" + std::string(pb_type->modes[pb->mode].name) + "]";
    }
    path.push_back(component);
  }

  std::reverse(path.begin(), path.end());
  std::string result;

  /* Join hierarchy components using the mif_source pb_type delimiter. */
  for (const std::string& component : path) {
    if (!result.empty()) {
      result += ".";
    }
    result += component;
  }
  return result;
}

static bool is_model_output_port(const t_model& model,
                                 const std::string& port_name) {
  const size_t bracket_pos = port_name.find('[');
  const std::string base_name = port_name.substr(0, bracket_pos);
  for (const t_model_ports* port = model.outputs; port != nullptr;
       port = port->next) {
    if (base_name == port->name) {
      return true;
    }
  }
  return false;
}

std::string get_mif_pb_type_from_vpr(
  const std::string& model_name,
  const std::vector<std::pair<std::string, std::string>>& port_connections) {
  const AtomContext& atom_ctx = g_vpr_ctx.atom();
  const AtomPBBimap& atom_pb_bimap = atom_ctx.lookup().atom_pb_bimap();
  const auto& models = g_vpr_ctx.device().arch->models;
  const LogicalModelId model_id = models.get_model_by_name(model_name);
  if (!model_id.is_valid()) {
    VTR_LOG_ERROR("MIF/VPR binding: eblif model '%s' is unknown to VPR\n",
                  model_name.c_str());
    return std::string();
  }

  const t_model& model = models.get_model(model_id);
  std::string atom_block_name;

  /* Match VPR's rule: name a subckt after the net on its first output pin. */
  for (const auto& connection : port_connections) {
    if (is_model_output_port(model, connection.first)) {
      atom_block_name = connection.second;
      break;
    }
  }
  if (atom_block_name.empty()) {
    VTR_LOG_ERROR("MIF/VPR binding: model '%s' has no connected output\n",
                  model_name.c_str());
    return std::string();
  }

  const AtomBlockId atom_block = atom_ctx.netlist().find_block(atom_block_name);
  if (!atom_block.is_valid()) {
    VTR_LOG_ERROR(
      "MIF/VPR binding: cannot find AtomBlock '%s' for model '%s'\n",
      atom_block_name.c_str(), model_name.c_str());
    return std::string();
  }

  const t_pb* leaf_pb = atom_pb_bimap.atom_pb(atom_block);
  if (leaf_pb == nullptr) {
    VTR_LOG_ERROR(
      "MIF/VPR binding: AtomBlock '%s' has no packed pb; run this after "
      "VPR pack\n",
      atom_block_name.c_str());
    return std::string();
  }

  const std::string pb_path = generate_mif_pb_path(leaf_pb);
  VTR_LOG("MIF/VPR binding: AtomBlock '%s' -> pb_type '%s'\n",
          atom_block_name.c_str(), pb_path.c_str());
  return pb_path;
}

static t_pb_type* find_pb_type_by_mif_path(const DeviceContext& vpr_device_ctx,
                                           const std::string& pb_path) {
  const std::string type_only_path = strip_numeric_pb_index(pb_path);
  const PbParser parser(type_only_path);

  std::vector<std::string> target_pb_type_names = parser.parents();
  target_pb_type_names.push_back(parser.leaf());
  const std::vector<std::string> target_pb_mode_names = parser.modes();

  for (const t_logical_block_type& lb_type :
       vpr_device_ctx.logical_block_types) {
    if (lb_type.pb_type == nullptr) {
      continue;
    }
    if (target_pb_type_names[0] != std::string(lb_type.pb_type->name)) {
      continue;
    }
    t_pb_type* found = try_find_pb_type_with_given_path(
      lb_type.pb_type, target_pb_type_names, target_pb_mode_names);
    if (found != nullptr) {
      return found;
    }
  }
  return nullptr;
}

int rewrite_aggregated_mif_physical_pb(
  MifStorage& physical_storage, const DeviceContext& vpr_device_ctx,
  const VprDeviceAnnotation& vpr_device_annotation) {
  for (const MifSegmentId& segment_id : physical_storage.segments()) {
    if (!physical_storage.has_physical_pb(segment_id)) {
      VTR_LOG_ERROR(
        "rewrite_aggregated_mif_physical_pb: segment %zu has no pb_type\n",
        static_cast<size_t>(segment_id));
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string& des_pb_path = physical_storage.physical_pb(segment_id);
    t_pb_type* des_pb_type =
      find_pb_type_by_mif_path(vpr_device_ctx, des_pb_path);
    if (des_pb_type == nullptr) {
      VTR_LOG_ERROR(
        "rewrite_aggregated_mif_physical_pb: cannot find pb_type for "
        "aggregated path '%s'\n",
        des_pb_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    t_pb_type* physical_pb_type =
      vpr_device_annotation.physical_pb_type(des_pb_type);
    if (physical_pb_type == nullptr) {
      VTR_LOG_ERROR(
        "rewrite_aggregated_mif_physical_pb: no physical mapping for "
        "pb_type '%s'\n",
        des_pb_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string physical_path =
      generate_pb_type_hierarchy_path(physical_pb_type);
    if (physical_path != des_pb_path) {
      VTR_LOG("rewrite_aggregated_mif_physical_pb: '%s' -> physical '%s'\n",
              des_pb_path.c_str(), physical_path.c_str());
      physical_storage.set_segment_physical_pb(segment_id, physical_path);
    }
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
