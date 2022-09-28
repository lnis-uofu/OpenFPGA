/********************************************************************
 * This file includes functions that are used to annotate clustering results
 * from VPR to OpenFPGA
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_geometry.h"

#include "command_exit_codes.h"

#include "annotate_clustering.h"

/* begin namespace openfpga */
namespace openfpga {

/* @brief Record the net remapping and local routing trace changes in annotation 
 * This is to ensure that the clustering annotation data structure is always up-to-date
 */
bool annotate_post_routing_cluster_sync_results(const DeviceContext& device_ctx, 
                                                const ClusteringContext& clustering_ctx, 
                                                VprClusteringAnnotation& clustering_annotation) {
  VTR_LOG("Building annotation for post-routing and clustering synchornization results...");

  for (const ClusterBlockId& cluster_blk_id : clustering_ctx.clb_nlist.blocks()) {
    /* Skip invalid ids */
    if (!cluster_blk_id) {
      continue;
    }
    auto logical_block = clustering_ctx.clb_nlist.block_type(cluster_blk_id);
    for (int ipin = 0; ipin < logical_block->pb_type->num_pins; ++ipin) {
      ClusterNetId pre_routing_net_id = clustering_ctx.clb_nlist.block_net(cluster_blk_id, ipin);
      ClusterNetId post_routing_net_id = ClusterNetId::INVALID();
      auto search_result = clustering_ctx.post_routing_clb_pin_nets.at(cluster_blk_id).find(ipin);
      if (search_result != clustering_ctx.post_routing_clb_pin_nets.at(cluster_blk_id).end()) {
        post_routing_net_id = search_result->second;
      }
      if (post_routing_net_id) {
        clustering_annotation.rename_net(cluster_blk_id, ipin, post_routing_net_id);
      }
    }
  } 
  VTR_LOG("Done\n");

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
