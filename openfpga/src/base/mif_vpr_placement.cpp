#include "mif_vpr_placement.h"

#include <algorithm>
#include <string>
#include <vector>

#include "globals.h"
#include "vtr_assert.h"
#include "vtr_log.h"

namespace openfpga {

namespace {

bool is_memory_model_name(const std::string& model_name) {
  return model_name.find("ram") != std::string::npos ||
         model_name.find("memory") != std::string::npos;
}

/* Match OpenFPGA bitstream / annotation pb_type path style:
 *   <pb_type>[<mode>].<sub_pb_type>[<mode>]... .<leaf_pb_type>
 * e.g. memory[mem_8x16_dp].mem_8x16_dp
 */
std::string compose_pb_type_path(const t_pb* leaf_pb) {
  if (nullptr == leaf_pb) {
    return std::string();
  }

  std::vector<const t_pb*> pb_chain;
  for (const t_pb* cur_pb = leaf_pb; cur_pb != nullptr;
       cur_pb = cur_pb->parent_pb) {
    pb_chain.push_back(cur_pb);
  }
  std::reverse(pb_chain.begin(), pb_chain.end());

  std::string pb_type_path;
  for (const t_pb* cur_pb : pb_chain) {
    VTR_ASSERT(nullptr != cur_pb->pb_graph_node);
    VTR_ASSERT(nullptr != cur_pb->pb_graph_node->pb_type);

    if (!pb_type_path.empty()) {
      pb_type_path += ".";
    }
    pb_type_path += cur_pb->pb_graph_node->pb_type->name;

    if (!cur_pb->is_primitive()) {
      t_mode* mode = cur_pb->get_mode();
      if (nullptr != mode) {
        pb_type_path += "[";
        pb_type_path += mode->name;
        pb_type_path += "]";
      }
    }
  }

  return pb_type_path;
}

} /* namespace */

std::map<std::string, std::string> get_instance_pb_type_path_from_placement() {
  std::map<std::string, std::string> inst_pb_type_path_map;
  const auto& cluster_ctx = g_vpr_ctx.clustering();
  const auto& atom_ctx = g_vpr_ctx.atom();
  const auto& models = g_vpr_ctx.device().arch->models;

  if (cluster_ctx.clb_nlist.blocks().empty()) {
    VTR_LOG("get_instance_pb_type_path_from_placement: Found no blocks.\n");
  }

  for (const AtomBlockId& atom_blk_id : atom_ctx.netlist().blocks()) {
    const std::string model_name =
      models.model_name(atom_ctx.netlist().block_model(atom_blk_id));
    if (!is_memory_model_name(model_name)) {
      continue;
    }

    const ClusterBlockId clb_blk_id = atom_ctx.lookup().atom_clb(atom_blk_id);
    if (!clb_blk_id.is_valid()) {
      continue;
    }

    const t_pb* atom_pb =
      atom_ctx.lookup().atom_pb_bimap().atom_pb(atom_blk_id);
    if (nullptr == atom_pb) {
      continue;
    }

    const std::string pb_type_path = compose_pb_type_path(atom_pb);
    if (pb_type_path.empty()) {
      continue;
    }

    const std::string instance_name =
      atom_ctx.netlist().block_name(atom_blk_id);
    inst_pb_type_path_map[instance_name] = pb_type_path;
    VTR_LOG(
      "get_instance_pb_type_path_from_placement: instance '%s' -> "
      "pb_type='%s'\n",
      instance_name.c_str(), pb_type_path.c_str());
  }

  return inst_pb_type_path_map;
}

} /* namespace openfpga */
