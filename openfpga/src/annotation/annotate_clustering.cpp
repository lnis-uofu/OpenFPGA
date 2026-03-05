/********************************************************************
 * This file includes functions that are used to annotate clustering results
 * from VPR to OpenFPGA
 *******************************************************************/
/* Headers from vtrutil library */
#include "annotate_clustering.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/* @brief Record the net remapping and local routing trace changes in annotation
 * This is to ensure that the clustering annotation data structure is always
 * up-to-date
 */
int annotate_post_routing_cluster_sync_results(
  const ClusteringContext& clustering_ctx,
  VprClusteringAnnotation& clustering_annotation) {
  VTR_LOG(
    "Building annotation for post-routing and clustering synchornization "
    "results...\n");

  for (const ClusterBlockId& cluster_blk_id :
       clustering_ctx.clb_nlist.blocks()) {
    /* Skip invalid ids */
    if (!cluster_blk_id) {
      continue;
    }
    auto logical_block = clustering_ctx.clb_nlist.block_type(cluster_blk_id);
    for (int ipin = 0; ipin < logical_block->pb_type->num_pins; ++ipin) {
      /* Update pin remapping from vtr data storage */
      auto blk_search_result =
        clustering_ctx.post_routing_clb_pin_nets.find(cluster_blk_id);
      if (blk_search_result != clustering_ctx.post_routing_clb_pin_nets.end()) {
        auto pin_search_result = blk_search_result->second.find(ipin);
        if (pin_search_result != blk_search_result->second.end()) {
          clustering_annotation.rename_net(cluster_blk_id, ipin,
                                           pin_search_result->second);
        }
      }
    }
  }
  VTR_LOG("Done\n");

  return CMD_EXEC_SUCCESS;
}

int annotate_cluster_physical_equivalent_sites(
  const DeviceGrid& grids, const ClusteringContext& clustering_ctx,
  const PlacementContext& place_ctx,
  const VprDeviceAnnotation& device_annotation,
  VprClusteringAnnotation& clustering_annotation, const bool& verbose) {
  VTR_LOG(
    "Building annotation on physical equivalent sites for clustered "
    "blocks...\n");

  for (const ClusterBlockId& cluster_blk_id :
       clustering_ctx.clb_nlist.blocks()) {
    /* Skip invalid ids */
    if (!cluster_blk_id) {
      continue;
    }
    /* Find the pb_type representing the physical site */
    vtr::Point<size_t> grid_coord(place_ctx.block_locs()[cluster_blk_id].loc.x,
                                  place_ctx.block_locs()[cluster_blk_id].loc.y);
    int sub_tile_z = place_ctx.block_locs()[cluster_blk_id].loc.sub_tile;
    int blk_layer = place_ctx.block_locs()[cluster_blk_id].loc.layer;
    t_physical_tile_type_ptr grid_type = grids.get_physical_type(
      t_physical_tile_loc(grid_coord.x(), grid_coord.y(), blk_layer));
    int sub_tile_index =
      device_annotation.physical_tile_z_to_subtile_index(grid_type, sub_tile_z);
    t_logical_block_type_ptr phy_lb_type =
      device_annotation.physical_equivalent_site(
        grid_type, grid_type->sub_tiles[sub_tile_index].name);
    VTR_LOGV(
      verbose,
      "Consider physical equivalent site '%s' for clustered block '%s'\n",
      phy_lb_type->name.c_str(),
      clustering_ctx.clb_nlist.block_name(cluster_blk_id).c_str());
    clustering_annotation.set_physical_equivalent_site(cluster_blk_id,
                                                       phy_lb_type);
  }

  VTR_LOG("Done\n");

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
