#include "mif_vpr_placement.h"

#include <string>
#include <vector>

#include "atom_netlist.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "openfpga_pb_parser.h"
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
  const AtomContext& atom_ctx, const DeviceContext& device_ctx,
  const std::string& model_name,
  const MifEblifPortConnections& port_connections) {
  const AtomPBBimap& atom_pb_bimap = atom_ctx.lookup().atom_pb_bimap();
  const auto& models = device_ctx.arch->models;
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

int annotate_physical_mif_grid_coordinates(
  MifPipeline& mif_pipeline, const BitstreamSetting& bitstream_setting,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx) {
  MifStorage& physical_storage =
    mif_pipeline.mutable_storage(MifPipeline::Stage::PHYSICAL);
  if (physical_storage.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  const AtomPBBimap& atom_pb_bimap = atom_ctx.lookup().atom_pb_bimap();

  for (const AtomBlockId atom_block : atom_ctx.netlist().blocks()) {
    const AtomBlockType block_type = atom_ctx.netlist().block_type(atom_block);
    if ((AtomBlockType::INPAD == block_type) ||
        (AtomBlockType::OUTPAD == block_type)) {
      continue;
    }

    const t_pb* leaf_pb = atom_pb_bimap.atom_pb(atom_block);
    if (leaf_pb == nullptr) {
      continue;
    }

    const std::string operating_pb = generate_mif_pb_path(leaf_pb);
    if (operating_pb.empty()) {
      continue;
    }
    if (!bitstream_setting.find_mif_source_by_pb_type(operating_pb)
           .is_valid()) {
      continue;
    }

    std::string target_pb = operating_pb;
    const MifAddressMapSettingId map_id =
      bitstream_setting.find_mif_address_map_by_src_pb_type(operating_pb);
    if (map_id.is_valid()) {
      target_pb = bitstream_setting.mif_address_map_des_pb_type(map_id);
    }
    const std::string type_target = strip_numeric_pb_index(target_pb);

    MifSegmentId segment_id = MifSegmentId::INVALID();
    for (const MifSegmentId& candidate : physical_storage.segments()) {
      if (strip_numeric_pb_index(physical_storage.physical_pb(candidate)) ==
          type_target) {
        segment_id = candidate;
        break;
      }
    }
    if (!segment_id.is_valid()) {
      VTR_LOG_ERROR(
        "annotate_physical_mif_grid_coordinates: no PHYSICAL segment for "
        "packed pb '%s' (AtomBlock '%s')\n",
        operating_pb.c_str(),
        atom_ctx.netlist().block_name(atom_block).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const ClusterBlockId cluster_blk = atom_ctx.lookup().atom_clb(atom_block);
    if (!cluster_blk.is_valid()) {
      VTR_LOG_ERROR(
        "annotate_physical_mif_grid_coordinates: AtomBlock '%s' has no "
        "cluster placement\n",
        atom_ctx.netlist().block_name(atom_block).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const t_pl_loc& pl_loc = place_ctx.block_locs()[cluster_blk].loc;
    if (mif_pipeline.physical_segment_has_grid_coord(segment_id)) {
      if (mif_pipeline.physical_segment_grid_x(segment_id) != pl_loc.x ||
          mif_pipeline.physical_segment_grid_y(segment_id) != pl_loc.y ||
          mif_pipeline.physical_segment_grid_z(segment_id) != pl_loc.sub_tile) {
        VTR_LOG_WARN(
          "annotate_physical_mif_grid_coordinates: PHYSICAL segment %zu "
          "already has grid (%d,%d,%d); skip AtomBlock '%s' at (%d,%d,%d)\n",
          static_cast<size_t>(segment_id),
          mif_pipeline.physical_segment_grid_x(segment_id),
          mif_pipeline.physical_segment_grid_y(segment_id),
          mif_pipeline.physical_segment_grid_z(segment_id),
          atom_ctx.netlist().block_name(atom_block).c_str(), pl_loc.x, pl_loc.y,
          pl_loc.sub_tile);
        continue;
      }
      continue;
    }

    mif_pipeline.set_physical_segment_grid_coord(segment_id, pl_loc.x, pl_loc.y,
                                                 pl_loc.sub_tile);
    VTR_LOG(
      "annotate_physical_mif_grid_coordinates: segment %zu grid (%d,%d,%d) "
      "pb '%s'\n",
      static_cast<size_t>(segment_id), pl_loc.x, pl_loc.y, pl_loc.sub_tile,
      operating_pb.c_str());
  }

  for (const MifSegmentId& segment_id : physical_storage.segments()) {
    if (!mif_pipeline.physical_segment_has_grid_coord(segment_id)) {
      VTR_LOG_WARN(
        "annotate_physical_mif_grid_coordinates: PHYSICAL segment %zu "
        "(pb '%s') has no packed MIF instance placement\n",
        static_cast<size_t>(segment_id),
        physical_storage.physical_pb(segment_id).c_str());
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
