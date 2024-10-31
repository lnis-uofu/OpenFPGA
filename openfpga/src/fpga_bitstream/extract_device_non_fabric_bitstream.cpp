/********************************************************************
 * This file includes functions to build bitstream from a mapped
 * FPGA fabric.
 * We decode the bitstream from configuration of routing multiplexers
 * and Look-Up Tables (LUTs) which locate in CLBs and global routing
 *architecture
 *******************************************************************/
#include <fstream>
#include <vector>

/* Headers from vtrutil library */
#include "extract_device_non_fabric_bitstream.h"
#include "openfpga_pb_parser.h"
#include "pb_type_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

#define PRINT_LAYOUT_NAME "__layout__"

/********************************************************************
 * Extract data from the targetted PB
 *   1. If it is primitive
 *       a. If it match the targetted PB, try to get data from
 *          param of attr depends on what being defined in XML
 *       b. If it is does not match, do nothing
 *   2. If it is not primitive, then we loop for the child
 *******************************************************************/
static bool extract_pb_data(std::fstream& fp, const AtomContext& atom_ctx,
                            const t_pb* op_pb, const t_pb_type* target_pb_type,
                            const NonFabricBitstreamPBSetting& setting) {
  t_pb_graph_node* pb_graph_node = op_pb->pb_graph_node;
  t_pb_type* pb_type = pb_graph_node->pb_type;
  bool found_pb = false;
  if (true == is_primitive_pb_type(pb_type)) {
    if (target_pb_type == pb_type) {
      AtomBlockId atom_blk = atom_ctx.nlist.find_block(op_pb->name);
      VTR_ASSERT(atom_blk);
      if (setting.type == "param") {
        for (const auto& param_search : atom_ctx.nlist.block_params(atom_blk)) {
          std::string param = param_search.first;
          std::string content = param_search.second;
          if (setting.content == param) {
            fp << ",\n          \"data\" : \"" << content.c_str() << "\"";
            break;
          }
        }
      } else {
        VTR_ASSERT(setting.type == "attr");
        for (const auto& attr_search : atom_ctx.nlist.block_attrs(atom_blk)) {
          std::string attr = attr_search.first;
          std::string content = attr_search.second;
          if (setting.content == attr) {
            fp << ",\n          \"data\" : \"" << content.c_str() << "\"";
            break;
          }
        }
      }
      found_pb = true;
    }
  } else {
    t_mode* mapped_mode = &(pb_graph_node->pb_type->modes[op_pb->mode]);
    for (int ipb = 0; ipb < mapped_mode->num_pb_type_children && !found_pb;
         ++ipb) {
      /* Each child may exist multiple times in the hierarchy*/
      for (int jpb = 0;
           jpb < mapped_mode->pb_type_children[ipb].num_pb && !found_pb;
           ++jpb) {
        if ((nullptr != op_pb->child_pbs[ipb]) &&
            (nullptr != op_pb->child_pbs[ipb][jpb].name)) {
          found_pb =
            extract_pb_data(fp, atom_ctx, &(op_pb->child_pbs[ipb][jpb]),
                            target_pb_type, setting);
        }
      }
    }
  }
  return found_pb;
}

/********************************************************************
 * Extract data from the targetted PB (from that particular grid)
 *******************************************************************/
static void extract_grid_non_fabric_bitstream(
  std::fstream& fp, const VprContext& vpr_ctx,
  const ClusterBlockId& cluster_block_id, const t_pb_type* target_pb_type,
  const NonFabricBitstreamPBSetting setting) {
  const ClusteringContext& clustering_ctx = vpr_ctx.clustering();
  const AtomContext& atom_ctx = vpr_ctx.atom();

  if (ClusterBlockId::INVALID() != cluster_block_id) {
    const t_pb* op_pb = clustering_ctx.clb_nlist.block_pb(cluster_block_id);
    extract_pb_data(fp, atom_ctx, op_pb, target_pb_type, setting);
  } else {
    // Grid is valid, but this resource is not being used
  }
}

/********************************************************************
 * Extract data from the targetted PB (from the device)
 *******************************************************************/
static void extract_device_non_fabric_pb_bitstream(
  std::fstream& fp, const NonFabricBitstreamPBSetting setting,
  const std::string& target_parent_pb_name, const t_pb_type* target_pb_type,
  const VprContext& vpr_ctx) {
  const DeviceContext& device_ctx = vpr_ctx.device();
  const PlacementContext& placement_ctx = vpr_ctx.placement();
  const DeviceGrid& grids = device_ctx.grid;
  const size_t& layer = 0;

  // Loop logic block one by one
  if (target_parent_pb_name != PRINT_LAYOUT_NAME) {
    fp << ",\n      \"grid\" : [";
  }
  size_t grid_count = 0;
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      t_physical_tile_loc phy_tile_loc(ix, iy, layer);
      t_physical_tile_type_ptr grid_type =
        grids.get_physical_type(phy_tile_loc);
      // Bypass EMPTY grid
      if (true == is_empty_type(grid_type)) {
        continue;
      }

      // Skip width > 1 or height > 1 tiles (mostly heterogeneous blocks)
      if ((0 < grids.get_width_offset(phy_tile_loc)) ||
          (0 < grids.get_height_offset(phy_tile_loc))) {
        continue;
      }

      // Skip if this grid is not what we are looking for
      if (target_parent_pb_name == PRINT_LAYOUT_NAME) {
        if (grid_count) {
          fp << ",\n";
        }
        fp << "    {\n";
        fp << "      \"x\" : " << (uint32_t)(ix) << ",\n";
        fp << "      \"y\" : " << (uint32_t)(iy) << ",\n";
        fp << "      \"name\" : \"" << grid_type->name << "\"\n";
        fp << "    }";
        grid_count++;
        continue;
      }

      // Skip if this grid is not what we are looking for
      if (target_parent_pb_name != std::string(grid_type->name)) {
        continue;
      }

      // Get the mapped blocks to this grid
      for (int isubtile = 0; isubtile < grid_type->capacity; ++isubtile) {
        ClusterBlockId cluster_blk_id =
          placement_ctx.grid_blocks().block_at_location(
            {(int)ix, (int)iy, (int)isubtile, (int)layer});
        if (grid_count) {
          fp << ",";
        }
        fp << "\n";
        fp << "        {\n";
        fp << "          \"x\" : " << (uint32_t)(ix) << ",\n";
        fp << "          \"y\" : " << (uint32_t)(iy);
        extract_grid_non_fabric_bitstream(fp, vpr_ctx, cluster_blk_id,
                                          target_pb_type, setting);
        fp << "\n        }";
        grid_count++;
      }
    }
  }
  if (target_parent_pb_name == PRINT_LAYOUT_NAME) {
    fp << "\n";
  } else {
    fp << "\n      ]";
  }
}

/********************************************************************
 * Search the PB type based on the given name defined in XML
 *******************************************************************/
static t_pb_type* find_pb_type(const DeviceContext& device_ctx,
                               const std::string& parent_pb,
                               const std::string& pb) {
  t_pb_type* pb_type = nullptr;
  openfpga::PbParser pb_parser(pb);
  std::vector<std::string> names = pb_parser.parents();
  names.push_back(pb_parser.leaf());
  for (const t_logical_block_type& lb_type : device_ctx.logical_block_types) {
    /* Bypass nullptr for pb_type head */
    if (nullptr == lb_type.pb_type) {
      continue;
    }

    /* Check the name of the top-level pb_type, if it does not match, we can
     * bypass */
    if (parent_pb != std::string(lb_type.pb_type->name)) {
      continue;
    }

    /* Match the name in the top-level, we go further to search the pb_type in
     * the graph */
    pb_type = try_find_pb_type_with_given_path(lb_type.pb_type, names,
                                               pb_parser.modes());
    if (nullptr == pb_type) {
      continue;
    }
    break;
  }
  return pb_type;
}

/********************************************************************
 * A top-level function to extract data based on non-fabric bitstream setting
 *******************************************************************/
void extract_device_non_fabric_bitstream(const VprContext& vpr_ctx,
                                         const OpenfpgaContext& openfpga_ctx,
                                         const bool& verbose) {
  std::string timer_message =
    std::string("\nBuild non-fabric bitstream for implementation '") +
    vpr_ctx.atom().nlist.netlist_name() + std::string("'\n");
  vtr::ScopedStartFinishTimer timer(timer_message);
  const openfpga::BitstreamSetting& bitstream_setting =
    openfpga_ctx.bitstream_setting();
  std::vector<NonFabricBitstreamSetting> non_fabric_setting =
    bitstream_setting.non_fabric();

  // Only proceed if it is defined in bitstream_setting.xml
  if (non_fabric_setting.size()) {
    // Go through each non_fabric settting
    for (auto setting : non_fabric_setting) {
      std::fstream fp;
      fp.open(setting.file.c_str(), std::fstream::out);
      fp << "{\n";
      fp << "  \"" << setting.name.c_str() << "\" : [\n";
      if (setting.name == PRINT_LAYOUT_NAME) {
        extract_device_non_fabric_pb_bitstream(
          fp, NonFabricBitstreamPBSetting{}, setting.name, nullptr, vpr_ctx);
      } else {
        int pb_count = 0;
        // Extract each needed PB data
        for (auto pb_setting : setting.pbs) {
          std::string pb_type = setting.name + pb_setting.pb;
          t_pb_type* target_pb_type =
            find_pb_type(vpr_ctx.device(), setting.name, pb_type);
          if (pb_count) {
            fp << ",\n";
          }
          fp << "    {\n";
          fp << "      \"pb\" : \"" << pb_type.c_str() << "\",\n";
          if (target_pb_type == nullptr) {
            fp << "      \"is_primitive_pb_type\" : \"invalid\",\n";
          } else {
            if (is_primitive_pb_type(target_pb_type)) {
              fp << "      \"is_primitive_pb_type\" : \"true\",\n";
            } else {
              fp << "      \"is_primitive_pb_type\" : \"false\",\n";
            }
          }
          fp << "      \"type\" : \"" << pb_setting.type.c_str() << "\",\n";
          fp << "      \"content\" : \"" << pb_setting.content.c_str() << "\"";
          if (target_pb_type != nullptr &&
              is_primitive_pb_type(target_pb_type)) {
            extract_device_non_fabric_pb_bitstream(fp, pb_setting, setting.name,
                                                   target_pb_type, vpr_ctx);
          }
          fp << "\n    }";
          pb_count++;
        }
        if (pb_count) {
          fp << "\n";
        }
      }
      fp << "  ]\n";
      fp << "}\n";
      fp.close();
    }
  }
  VTR_LOGV(verbose, "Done\n");
}

} /* end namespace openfpga */
