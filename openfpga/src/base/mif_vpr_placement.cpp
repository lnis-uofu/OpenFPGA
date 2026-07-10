#include "mif_vpr_placement.h"

#include <cstring>
#include <string>

#include "globals.h"
#include "vtr_assert.h"
#include "vtr_log.h"

namespace openfpga {

/* Analyze vpr placement context to get ram instances' name-location map */
std::map<std::string, t_pl_loc> get_instance_info_from_placement() {
  std::map<std::string, t_pl_loc> inst_coord_map;
  auto& cluster_ctx = g_vpr_ctx.clustering();
  auto& block_locs = g_vpr_ctx.placement().block_locs();
  auto& atom_ctx = g_vpr_ctx.atom();
  auto& models = g_vpr_ctx.device().arch->models;

  if (cluster_ctx.clb_nlist.blocks().empty()) {
    VTR_LOG("get_instance_info_from_placement: Found no blocks.\n");
  }

  for (auto blk_id : cluster_ctx.clb_nlist.blocks()) {
    /* block type contains keyword: ram (OpenFPGA: memory) */
    if (strstr(cluster_ctx.clb_nlist.block_type(blk_id)->name.c_str(),
               "ram") ||
        strstr(cluster_ctx.clb_nlist.block_type(blk_id)->name.c_str(),
               "memory")) {
      std::string block_name = cluster_ctx.clb_nlist.block_name(blk_id);
      auto atom_net_id = atom_ctx.netlist().find_net(block_name);
      VTR_ASSERT(atom_net_id.is_valid());
      auto driver_block_id = atom_ctx.netlist().net_driver_block(atom_net_id);
      const std::string model_type =
        models.model_name(atom_ctx.netlist().block_model(driver_block_id));
      /* model type should be ".subckt" which is not explicitly defined */
      VTR_ASSERT(model_type != ".input" && model_type != ".output" &&
                 model_type != ".names" && model_type != ".latch");
      auto location = block_locs[blk_id].loc;
      inst_coord_map[model_type] = location;
    }
  }

  return inst_coord_map;
}

} /* namespace openfpga */
