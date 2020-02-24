/********************************************************************
 * This file includes functions that are used to annotate pb_graph_node
 * and pb_graph_pins from VPR to OpenFPGA
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_geometry.h"

#include "annotate_placement.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Assign mapped blocks to grid locations 
 * This is used by bitstream generator mainly as a fast look-up to 
 * get mapped blocks with a given coordinate
 *******************************************************************/
void annotate_mapped_blocks(const DeviceContext& device_ctx, 
                            const ClusteringContext& cluster_ctx, 
                            const PlacementContext& place_ctx, 
                            VprPlacementAnnotation& place_annotation) {
  VTR_LOG("Building annotation for mapped blocks on grid locations...");

  place_annotation.init_mapped_blocks(device_ctx.grid);
  for (const ClusterBlockId& blk_id : cluster_ctx.clb_nlist.blocks()) {
    vtr::Point<size_t> grid_coord(place_ctx.block_locs[blk_id].loc.x, place_ctx.block_locs[blk_id].loc.y);
    place_annotation.add_mapped_block(grid_coord, place_ctx.block_locs[blk_id].loc.z, blk_id);
  } 
  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
