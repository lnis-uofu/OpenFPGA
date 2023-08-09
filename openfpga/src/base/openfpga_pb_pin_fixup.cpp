/********************************************************************
 * This file includes functions to fix up the pb pin mapping results
 * after routing optimization
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from vpr library */
#include "vpr_utils.h"

/* Headers from openfpgautil library */
#include "openfpga_device_grid_utils.h"
#include "openfpga_pb_pin_fixup.h"
#include "openfpga_physical_tile_utils.h"
#include "openfpga_side_manager.h"
#include "pb_type_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Fix up the pb pin mapping results for a given clustered block
 * 1. For each input/output pin of a clustered pb,
 *    - find a corresponding node in RRGraph object
 *    - find the net id for the node in routing context
 *    - find the net id for the node in clustering context
 *    - if the net id does not match, we update the clustering context
 *******************************************************************/
static void update_cluster_pin_with_post_routing_results(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  const VprRoutingAnnotation& vpr_routing_annotation,
  VprClusteringAnnotation& vpr_clustering_annotation, const size_t& layer,
  const vtr::Point<size_t>& grid_coord, const ClusterBlockId& blk_id,
  const e_side& border_side, const size_t& z, const bool& verbose) {
  /* Handle each pin */
  auto logical_block = clustering_ctx.clb_nlist.block_type(blk_id);
  auto physical_tile = device_ctx.grid.get_physical_type(
    t_physical_tile_loc(grid_coord.x(), grid_coord.y(), layer));

  for (int j = 0; j < logical_block->pb_type->num_pins; j++) {
    /* Get the ptc num for the pin in rr_graph, we need t consider the z offset
     * here z offset is the location in the multiple-logic-tile tile Get
     * physical pin does not consider THIS!!!!
     */
    int physical_pin = z * logical_block->pb_type->num_pins +
                       get_physical_pin(physical_tile, logical_block, j);
    auto pin_class = physical_tile->pin_class[physical_pin];
    auto class_inf = physical_tile->class_inf[pin_class];

    t_rr_type rr_node_type;
    if (class_inf.type == DRIVER) {
      rr_node_type = OPIN;
    } else {
      VTR_ASSERT(class_inf.type == RECEIVER);
      rr_node_type = IPIN;
    }
    std::vector<e_side> pin_sides =
      find_physical_tile_pin_side(physical_tile, physical_pin, border_side);
    /* As some grid has height/width offset, we may not have the pin on any side
     */
    if (0 == pin_sides.size()) {
      continue;
    }

    /* For regular grid, we should have pin only one side!
     * I/O grids: VPR creates the grid with duplicated pins on every side
     * but the expected side (only used side) will be opposite side of the
     * border side!
     */
    e_side pin_side = NUM_SIDES;
    if (NUM_SIDES == border_side) {
      VTR_ASSERT(1 == pin_sides.size());
      pin_side = pin_sides[0];
    } else {
      SideManager side_manager(border_side);
      VTR_ASSERT(pin_sides.end() != std::find(pin_sides.begin(),
                                              pin_sides.end(),
                                              side_manager.get_opposite()));
      pin_side = side_manager.get_opposite();
    }

    /* Find the net mapped to this pin in routing results */
    const RRNodeId& rr_node = device_ctx.rr_graph.node_lookup().find_node(
      layer, grid_coord.x(), grid_coord.y(), rr_node_type, physical_pin,
      pin_side);
    if (false == device_ctx.rr_graph.valid_node(rr_node)) {
      continue;
    }
    /* Get the cluster net id which has been mapped to this net */
    ClusterNetId routing_net_id = vpr_routing_annotation.rr_node_net(rr_node);

    /* Find the net mapped to this pin in clustering results. There are two
     * sources:
     * - The original clustering netlist, where the pin mapping is based on
     * pre-routing
     * - The post-routing pin mapping, where the pin mapping is based on
     * post-routing We always check the original clustering netlist first, if
     * there is any remapping, check the remapping data
     */
    ClusterNetId cluster_net_id = clustering_ctx.clb_nlist.block_net(blk_id, j);

    /* Ignore those net have never been routed: this check is valid only
     * when both packer has mapped a net to the pin and the router leaves the
     * pin to be unmapped This is important because we cannot bypass when router
     * forces a valid net to be mapped and the net remapping has to be
     * considered
     */
    if ((ClusterNetId::INVALID() != cluster_net_id) &&
        (ClusterNetId::INVALID() == routing_net_id) &&
        (true == clustering_ctx.clb_nlist.net_is_ignored(cluster_net_id))) {
      VTR_LOGV(
        verbose,
        "Bypass net at clustered block '%s' pin 'grid[%ld][%ld].%s.%s[%d]' as "
        "it is not routed\n",
        clustering_ctx.clb_nlist.block_pb(blk_id)->name, grid_coord.x(),
        grid_coord.y(),
        clustering_ctx.clb_nlist.block_pb(blk_id)->pb_graph_node->pb_type->name,
        get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->port->name,
        get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->pin_number);
      continue;
    }

    /* Ignore used in local cluster only, reserved one CLB pin */
    if ((ClusterNetId::INVALID() != cluster_net_id) &&
        (0 == clustering_ctx.clb_nlist.net_sinks(cluster_net_id).size())) {
      VTR_LOGV(
        verbose,
        "Bypass net at clustered block '%s' pin 'grid[%ld][%ld].%s.%s[%d]' as "
        "it is a local net inside the cluster\n",
        clustering_ctx.clb_nlist.block_pb(blk_id)->name, grid_coord.x(),
        grid_coord.y(),
        clustering_ctx.clb_nlist.block_pb(blk_id)->pb_graph_node->pb_type->name,
        get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->port->name,
        get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->pin_number);
      continue;
    }

    /* If matched, we finish here */
    if (routing_net_id == cluster_net_id) {
      VTR_LOGV(
        verbose,
        "Bypass net at clustered block '%s' pin 'grid[%ld][%ld].%s.%s[%d]' as "
        "it matches cluster routing\n",
        clustering_ctx.clb_nlist.block_pb(blk_id)->name, grid_coord.x(),
        grid_coord.y(),
        clustering_ctx.clb_nlist.block_pb(blk_id)->pb_graph_node->pb_type->name,
        get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->port->name,
        get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->pin_number);
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

    VTR_LOGV(
      verbose,
      "Fixed up net '%s' mapping mismatch at clustered block '%s' pin "
      "'grid[%ld][%ld].%s.%s[%d]' (was net '%s')\n",
      routing_net_name.c_str(), clustering_ctx.clb_nlist.block_pb(blk_id)->name,
      grid_coord.x(), grid_coord.y(),
      clustering_ctx.clb_nlist.block_pb(blk_id)->pb_graph_node->pb_type->name,
      get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->port->name,
      get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)->pin_number,
      cluster_net_name.c_str());
  }
}

/********************************************************************
 * Main function to fix up the pb pin mapping results
 * This function will walk through each grid
 *******************************************************************/
void update_pb_pin_with_post_routing_results(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  const PlacementContext& placement_ctx,
  const VprRoutingAnnotation& vpr_routing_annotation,
  VprClusteringAnnotation& vpr_clustering_annotation, const bool& verbose) {
  /* Ensure a clean start: remove all the remapping results from VTR's
   * post-routing clustering result sync-up */
  vpr_clustering_annotation.clear_net_remapping();

  size_t layer = 0;
  /* Update the core logic (center blocks of the FPGA) */
  for (size_t x = 1; x < device_ctx.grid.width() - 1; ++x) {
    for (size_t y = 1; y < device_ctx.grid.height() - 1; ++y) {
      t_physical_tile_type_ptr phy_tile =
        device_ctx.grid.get_physical_type(t_physical_tile_loc(x, y, layer));
      /* Bypass the EMPTY tiles */
      if (true == is_empty_type(phy_tile)) {
        continue;
      }
      /* Get the mapped blocks to this grid */
      for (int isubtile = 0; isubtile < phy_tile->capacity; ++isubtile) {
        ClusterBlockId cluster_blk_id =
          placement_ctx.grid_blocks.block_at_location(
            {(int)x, (int)y, (int)isubtile, (int)layer});
        /* Skip invalid ids */
        if (ClusterBlockId::INVALID() == cluster_blk_id) {
          continue;
        }
        /* We know the entrance to grid info and mapping results, do the fix-up
         * for this block */
        vtr::Point<size_t> grid_coord(x, y);
        update_cluster_pin_with_post_routing_results(
          device_ctx, clustering_ctx, vpr_routing_annotation,
          vpr_clustering_annotation, layer, grid_coord, cluster_blk_id,
          NUM_SIDES, placement_ctx.block_locs[cluster_blk_id].loc.sub_tile,
          verbose);
      }
    }
  }

  /* Create the coordinate range for each side of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_grid_coordinates(device_ctx.grid);

  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coord : io_coordinates[io_side]) {
      t_physical_tile_type_ptr phy_tile_type =
        device_ctx.grid.get_physical_type(
          t_physical_tile_loc(io_coord.x(), io_coord.y(), layer));
      /* Bypass EMPTY grid */
      if (true == is_empty_type(phy_tile_type)) {
        continue;
      }
      /* Get the mapped blocks to this grid */
      for (int isubtile = 0; isubtile < phy_tile_type->capacity; ++isubtile) {
        ClusterBlockId cluster_blk_id =
          placement_ctx.grid_blocks.block_at_location(
            {(int)io_coord.x(), (int)io_coord.y(), (int)isubtile, (int)layer});
        /* Skip invalid ids */
        if (ClusterBlockId::INVALID() == cluster_blk_id) {
          continue;
        }
        /* Update on I/O grid */
        update_cluster_pin_with_post_routing_results(
          device_ctx, clustering_ctx, vpr_routing_annotation,
          vpr_clustering_annotation, layer, io_coord, cluster_blk_id, io_side,
          placement_ctx.block_locs[cluster_blk_id].loc.sub_tile, verbose);
      }
    }
  }
}

} /* end namespace openfpga */
