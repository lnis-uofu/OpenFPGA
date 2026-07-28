#include "mif_vpr_placement.h"

#include <algorithm>
#include <string>
#include <vector>

#include "atom_netlist.h"
#include "command_exit_codes.h"
#include "globals.h"
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

std::string get_mif_pb_type_from_vpr(const std::string& param_name,
                                     size_t param_index) {
  const AtomContext& atom_ctx = g_vpr_ctx.atom();
  const AtomPBBimap& atom_pb_bimap = atom_ctx.lookup().atom_pb_bimap();
  size_t current_index = 0;

  /* Find the requested eblif subckt in its original AtomNetlist order. */
  for (const AtomBlockId atom_block : atom_ctx.netlist().blocks()) {
    for (const auto& param : atom_ctx.netlist().block_params(atom_block)) {
      if (param.first != param_name) {
        continue;
      }
      if (current_index++ != param_index) {
        break;
      }
      const t_pb* leaf_pb = atom_pb_bimap.atom_pb(atom_block);
      if (leaf_pb == nullptr) {
        VTR_LOG_ERROR(
          "MIF/VPR binding: AtomBlock '%s' has no packed pb; run this after "
          "VPR pack\n",
          atom_ctx.netlist().block_name(atom_block).c_str());
        return std::string();
      }

      const std::string pb_path = generate_mif_pb_path(leaf_pb);
      if (pb_path.empty()) {
        VTR_LOG_ERROR(
          "MIF/VPR binding: failed to build pb_type path for '%s'\n",
          atom_ctx.netlist().block_name(atom_block).c_str());
        return std::string();
      }
      VTR_LOG("MIF/VPR binding: AtomBlock '%s' -> pb_type '%s'\n",
              atom_ctx.netlist().block_name(atom_block).c_str(),
              pb_path.c_str());
      return pb_path;
    }
  }

  VTR_LOG_ERROR("MIF/VPR binding: no .param %s at index %zu\n",
                param_name.c_str(), param_index);
  return std::string();
}

} /* namespace openfpga */
