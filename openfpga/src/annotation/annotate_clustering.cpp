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
bool annotate_post_routing_cluster_sync_results(
  const ClusteringContext& clustering_ctx,
  VprClusteringAnnotation& clustering_annotation) {
  VTR_LOG(
    "Building annotation for post-routing and clustering synchornization "
    "results...");

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

} /* end namespace openfpga */
