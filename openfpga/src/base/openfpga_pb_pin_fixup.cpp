/********************************************************************
 * This file includes functions to fix up the pb pin mapping results 
 * after routing optimization
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "pb_type_utils.h"
#include "openfpga_pb_pin_fixup.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Give a given pin index, find the side where this pin is located 
 * on the physical tile
 * Note:
 *   - Need to check if the pin_width_offset and pin_height_offset
 *     are properly set in VPR!!!
 *******************************************************************/
static 
std::vector<e_side> find_logic_tile_pin_side(t_physical_tile_type_ptr physical_tile,
                                             const int& physical_pin) {
  std::vector<e_side> pin_sides;
  for (const e_side& side_cand : {TOP, RIGHT, BOTTOM, LEFT}) {
    int pin_width_offset = physical_tile->pin_width_offset[physical_pin];
    int pin_height_offset = physical_tile->pin_height_offset[physical_pin];
    if (true == physical_tile->pinloc[pin_width_offset][pin_height_offset][side_cand][physical_pin]) {
      pin_sides.push_back(side_cand);
    } 
  }

  return pin_sides;
}

/********************************************************************
 * Fix up the pb pin mapping results for a given clustered block
 * 1. For each input/output pin of a clustered pb, 
 *    - find a corresponding node in RRGraph object
 *    - find the net id for the node in routing context
 *    - find the net id for the node in clustering context
 *    - if the net id does not match, we update the clustering context
 *******************************************************************/
static 
void update_cluster_pin_with_post_routing_results(const DeviceContext& device_ctx,
                                                  const ClusteringContext& clustering_ctx,
                                                  const VprRoutingAnnotation& vpr_routing_annotation,
                                                  VprClusteringAnnotation& vpr_clustering_annotation,
                                                  const vtr::Point<size_t>& grid_coord,
                                                  const ClusterBlockId& blk_id,
                                                  const bool& verbose) {
  /* Handle each pin */
  auto logical_block = clustering_ctx.clb_nlist.block_type(blk_id);
  auto physical_tile = pick_best_physical_type(logical_block);

  for (int j = 0; j < logical_block->pb_type->num_pins; j++) {
    /* Get the ptc num for the pin in rr_graph */
    int physical_pin = get_physical_pin(physical_tile, logical_block, j);
    auto pin_class = physical_tile->pin_class[physical_pin];
    auto class_inf = physical_tile->class_inf[pin_class];
    t_rr_type rr_node_type;
    if (class_inf.type == DRIVER) {
      rr_node_type = OPIN;
    } else {
      VTR_ASSERT(class_inf.type == RECEIVER);
      rr_node_type = IPIN;
    }
    std::vector<e_side> pin_sides = find_logic_tile_pin_side(physical_tile, physical_pin);
    /* As some grid go across columns or rows, we may not have the pin on any side */
    if (0 == pin_sides.size()) {
      continue;
    }

    for (const e_side& pin_side : pin_sides) {
      /* Find the net mapped to this pin in routing results */
      const RRNodeId& rr_node = device_ctx.rr_graph.find_node(grid_coord.x(), grid_coord.y(), rr_node_type, physical_pin, pin_side); 
      if (false == device_ctx.rr_graph.valid_node_id(rr_node)) {
        continue;
      }
      /* Get the cluster net id which has been mapped to this net */
      ClusterNetId routing_net_id = vpr_routing_annotation.rr_node_net(rr_node);

      /* Find the net mapped to this pin in clustering results*/
      ClusterNetId cluster_net_id = clustering_ctx.clb_nlist.block_net(blk_id, j);

      /* If matched, we finish here */
      if (routing_net_id == cluster_net_id) {
         continue;
      }
      /* Add to net modification */
      vpr_clustering_annotation.rename_net(blk_id, j, routing_net_id);
 
      std::string routing_net_name("unmapped");
      if (ClusterNetId::INVALID() != routing_net_id) {
        routing_net_name = clustering_ctx.clb_nlist.net_name(routing_net_id);
      }

      std::string cluster_net_name("unmapped");
      if (ClusterNetId::INVALID() != cluster_net_id) {
        cluster_net_name = clustering_ctx.clb_nlist.net_name(cluster_net_id);
      }

      VTR_LOGV(verbose,
               "Fixed up net '%s' mapping mismatch at clustered block '%s' pin '%s[%d]' (was net '%s')\n",
               routing_net_name.c_str(),
               clustering_ctx.clb_nlist.block_pb(blk_id)->name,
               get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->port->name,
               get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->pin_number,
               cluster_net_name.c_str()
               );
    }
  }
}

/********************************************************************
 * Main function to fix up the pb pin mapping results 
 * This function will walk through each grid
 *******************************************************************/
static 
void update_pb_pin_with_post_routing_results(const DeviceContext& device_ctx,
                                             const ClusteringContext& clustering_ctx,
                                             const PlacementContext& placement_ctx,
                                             const VprRoutingAnnotation& vpr_routing_annotation,
                                             VprClusteringAnnotation& vpr_clustering_annotation,
                                             const bool& verbose) {
  for (size_t x = 0; x < device_ctx.grid.width(); ++x) {
    for (size_t y = 0; y < device_ctx.grid.height(); ++y) {
      /* Bypass the EMPTY tiles */
      if (device_ctx.EMPTY_PHYSICAL_TILE_TYPE == device_ctx.grid[x][y].type) {
        continue;
      }
      /* Get the mapped blocks to this grid */
      for (const ClusterBlockId& cluster_blk_id : placement_ctx.grid_blocks[x][y].blocks) {
        /* Skip invalid ids */ 
        if (ClusterBlockId::INVALID() == cluster_blk_id) {
          continue;
        }
        /* We know the entrance to grid info and mapping results, do the fix-up for this block */
        vtr::Point<size_t> grid_coord(x, y);
        update_cluster_pin_with_post_routing_results(device_ctx, clustering_ctx, 
                                                     vpr_routing_annotation, vpr_clustering_annotation,
                                                     grid_coord, cluster_blk_id,
                                                     verbose);
      } 
    }
  }
}

/********************************************************************
 * Top-level function to fix up the pb pin mapping results 
 * The problem comes from a mismatch between the packing and routing results
 * When there are equivalent input/output for any grids, router will try
 * to swap the net mapping among these pins so as to achieve best 
 * routing optimization.
 * However, it will cause the packing results out-of-date as the net mapping 
 * of each grid remain untouched once packing is done.
 * This function aims to fix the mess after routing so that the net mapping
 * can be synchronized
 *******************************************************************/
void pb_pin_fixup(OpenfpgaContext& openfpga_context,
                  const Command& cmd, const CommandContext& cmd_context) { 

  vtr::ScopedStartFinishTimer timer("Fix up pb pin mapping results after routing optimization");

  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Apply fix-up to each grid */
  update_pb_pin_with_post_routing_results(g_vpr_ctx.device(),
                                          g_vpr_ctx.clustering(),
                                          g_vpr_ctx.placement(), 
                                          openfpga_context.vpr_routing_annotation(),
                                          openfpga_context.mutable_vpr_clustering_annotation(),
                                          cmd_context.option_enable(cmd, opt_verbose));
} 

} /* end namespace openfpga */
