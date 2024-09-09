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
 * For global net which was remapped during routing, no tracking can be found.
 *Packer only keeps an out-of-date record on its pin mapping. Router does not
 *assign it to a new pin. So we have to restore the pin mapping. The strategy is
 *to find the first unused pin in the same port as it was mapped by the packer.
 *******************************************************************/
static int update_cluster_pin_global_net_with_post_routing_results(
  const ClusteringContext& clustering_ctx,
  VprClusteringAnnotation& clustering_annotation, const ClusterBlockId& blk_id,
  t_logical_block_type_ptr logical_block, const bool& map_gnet2msb,
  size_t& num_fixup, const bool& verbose) {
  /* Reassign global nets to unused pins in the same port where they were mapped
   * NO optimization is done here!!! First find first fit
   */
  for (int pb_type_pin = 0; pb_type_pin < logical_block->pb_type->num_pins;
       ++pb_type_pin) {
    const t_pb_graph_pin* pb_graph_pin =
      get_pb_graph_node_pin_from_block_pin(blk_id, pb_type_pin);

    /* Limitation: bypass output pins now
     * TODO: This is due to the 'instance' equivalence port
     * where outputs may be swapped. This definitely requires re-run of packing
     * It can not be solved by swapping routing traces now
     */
    if (OUT_PORT == pb_graph_pin->port->type) {
      continue;
    }

    /* Sanity check to ensure the pb_graph_pin is the top-level */
    VTR_ASSERT(pb_graph_pin->parent_node->is_root());

    /* Focus on global net only */
    ClusterNetId global_net_id =
      clustering_ctx.clb_nlist.block_net(blk_id, pb_type_pin);
    if (!clustering_ctx.clb_nlist.valid_net_id(global_net_id)) {
      continue;
    }
    if ((clustering_ctx.clb_nlist.valid_net_id(global_net_id)) &&
        (!clustering_ctx.clb_nlist.net_is_ignored(global_net_id))) {
      continue;
    }
    /* Skip this pin: it is consistent in pre- and post- routing results */
    if (!clustering_annotation.is_net_renamed(blk_id, pb_type_pin)) {
      continue;
    }
    /* This net has been remapped, find the first unused pin in the same port
     * Get the offset of the pin index in the port, based on which we can infer
     * the pin index in the context of logical block
     */
    VTR_LOG(
      "Searching for a candidate pin to accomodate global net '%s' was lost "
      "during routing optimization\n",
      clustering_ctx.clb_nlist.net_name(global_net_id).c_str());
    size_t cand_pin_start = pb_type_pin - pb_graph_pin->pin_number;
    std::vector<size_t> cand_pins(pb_graph_pin->port->num_pins);
    std::iota(cand_pins.begin(), cand_pins.end(), cand_pin_start);
    if (map_gnet2msb) {
      std::reverse(cand_pins.begin(), cand_pins.end());
    }
    bool found_cand = false;
    for (size_t cand_pin : cand_pins) {
      ClusterNetId cand_pin_net_id =
        clustering_ctx.clb_nlist.block_net(blk_id, cand_pin);
      const t_pb_graph_pin* cand_pb_graph_pin =
        get_pb_graph_node_pin_from_block_pin(blk_id, cand_pin);
      if (clustering_annotation.is_net_renamed(blk_id, cand_pin)) {
        cand_pin_net_id = clustering_annotation.net(blk_id, cand_pin);
      }
      if (clustering_ctx.clb_nlist.valid_net_id(cand_pin_net_id)) {
        VTR_LOG("Candidate pin '%s' is already mapped to net '%s'\n",
                cand_pb_graph_pin->to_string().c_str(),
                clustering_ctx.clb_nlist.net_name(cand_pin_net_id).c_str());
        continue;
      }
      /* Add to net modification */
      clustering_annotation.rename_net(blk_id, cand_pin, global_net_id);
      VTR_LOGV(verbose,
               "Remap clustered block '%s' global net '%s' to pin '%s'\n",
               clustering_ctx.clb_nlist.block_pb(blk_id)->name,
               clustering_ctx.clb_nlist.net_name(global_net_id).c_str(),
               cand_pb_graph_pin->to_string().c_str());
      found_cand = true;
      break;
    }
    /* Error out if no candidates are found */
    if (!found_cand) {
      VTR_LOG_ERROR(
        "Failed to find any unused pin in the same port to remap clustered "
        "block '%s' global net '%s' (was mapped to pin '%s').\n",
        clustering_ctx.clb_nlist.block_pb(blk_id)->name,
        clustering_ctx.clb_nlist.net_name(global_net_id).c_str(),
        pb_graph_pin->to_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    /* Update fixup counter */
    num_fixup++;
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Fix up the pb pin mapping results for a given clustered block
 * 1. For each input/output pin of a clustered pb,
 *    - find a corresponding node in RRGraph object
 *    - find the net id for the node in routing context
 *    - find the net id for the node in clustering context
 *    - if the net id does not match, we update the clustering context
 * TODO: For global net which was remapped during routing, no tracking can be
 *found. Packer only keeps an out-of-date record on its pin mapping. Router does
 *not assign it to a new pin. So we have to restore the pin mapping. The
 *strategy is to find the first unused pin in the same port as it was mapped by
 *the packer.
 *******************************************************************/
static int update_cluster_pin_with_post_routing_results(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  const VprRoutingAnnotation& vpr_routing_annotation,
  VprClusteringAnnotation& vpr_clustering_annotation, const size_t& layer,
  const vtr::Point<size_t>& grid_coord, const ClusterBlockId& blk_id,
  const e_side& border_side, const size_t& z, const bool& perimeter_cb,
  const bool& map_gnet2msb, size_t& num_fixup, const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  /* Handle each pin */
  auto logical_block = clustering_ctx.clb_nlist.block_type(blk_id);
  auto physical_tile = device_ctx.grid.get_physical_type(
    t_physical_tile_loc(grid_coord.x(), grid_coord.y(), layer));

  for (int j = 0; j < logical_block->pb_type->num_pins; j++) {
    /* Get the ptc num for the pin in rr_graph, we need t consider the z offset
     * here z offset is the location in the multiple-logic-tile tile Get
     * physical pin does not consider THIS!!!!
     */
    int physical_pin =
      get_physical_pin_at_sub_tile_location(physical_tile, logical_block, z, j);
    auto pin_class = physical_tile->pin_class[physical_pin];
    auto class_inf = physical_tile->class_inf[pin_class];

    t_rr_type rr_node_type;
    if (class_inf.type == DRIVER) {
      rr_node_type = OPIN;
    } else {
      VTR_ASSERT(class_inf.type == RECEIVER);
      rr_node_type = IPIN;
    }
    std::vector<e_side> pin_sides = find_physical_tile_pin_side(
      physical_tile, physical_pin, border_side, device_ctx.arch->perimeter_cb);
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
      if (1 != pin_sides.size()) {
        VTR_LOG_ERROR(
          "For tile '%s', found pin '%s' on %lu sides. Expect only 1. "
          "Following info is for debugging:\n",
          physical_tile->name,
          get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
            ->to_string()
            .c_str(),
          pin_sides.size());
        for (e_side curr_side : pin_sides) {
          VTR_LOG_ERROR("\t%s\n", SideManager(curr_side).c_str());
        }
        return CMD_EXEC_FATAL_ERROR;
      }
      pin_side = pin_sides[0];
    } else if (perimeter_cb) {
      /* When perimeter connection blcoks are allowed, I/O pins may occur on any
       * side but the border side */
      if (pin_sides.end() !=
          std::find(pin_sides.begin(), pin_sides.end(), border_side)) {
        VTR_LOG_ERROR(
          "For tile '%s', found pin '%s' on the boundary side '%s', which is "
          "not physically possible.\n",
          physical_tile->name,
          get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
            ->to_string()
            .c_str(),
          SideManager(border_side).c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      if (1 != pin_sides.size()) {
        VTR_LOG_ERROR(
          "For tile '%s', found pin '%s' on %lu sides. Expect only 1. "
          "Following info is for debugging:\n",
          physical_tile->name,
          get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
            ->to_string()
            .c_str(),
          pin_sides.size());
        for (e_side curr_side : pin_sides) {
          VTR_LOG_ERROR("\t%s\n", SideManager(curr_side).c_str());
        }
        return CMD_EXEC_FATAL_ERROR;
      }
      pin_side = pin_sides[0];
    } else {
      SideManager side_manager(border_side);
      if (pin_sides.end() == std::find(pin_sides.begin(), pin_sides.end(),
                                       side_manager.get_opposite())) {
        VTR_LOG_ERROR(
          "For boundary tile '%s', expect pin '%s' only on the side '%s' but "
          "found on the following sides:\n",
          physical_tile->name,
          get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
            ->to_string()
            .c_str(),
          SideManager(side_manager.get_opposite()).c_str());
        for (e_side curr_side : pin_sides) {
          VTR_LOG_ERROR("\t%s\n", SideManager(curr_side).c_str());
        }
        return CMD_EXEC_FATAL_ERROR;
      }
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
      VTR_LOGV(verbose,
               "Bypass net at clustered block '%s' pin 'grid[%ld][%ld].%s' as "
               "it is not routed\n",
               clustering_ctx.clb_nlist.block_pb(blk_id)->name, grid_coord.x(),
               grid_coord.y(),
               get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
                 ->to_string()
                 .c_str());
      continue;
    }

    /* Ignore used in local cluster only, reserved one CLB pin */
    if ((ClusterNetId::INVALID() != cluster_net_id) &&
        (0 == clustering_ctx.clb_nlist.net_sinks(cluster_net_id).size())) {
      VTR_LOGV(verbose,
               "Bypass net at clustered block '%s' pin 'grid[%ld][%ld].%s' as "
               "it is a local net inside the cluster\n",
               clustering_ctx.clb_nlist.block_pb(blk_id)->name, grid_coord.x(),
               grid_coord.y(),
               get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
                 ->to_string()
                 .c_str());
      continue;
    }

    /* If matched, we finish here */
    if (routing_net_id == cluster_net_id) {
      VTR_LOGV(verbose,
               "Bypass net at clustered block '%s' pin 'grid[%ld][%ld].%s' as "
               "it matches cluster routing\n",
               clustering_ctx.clb_nlist.block_pb(blk_id)->name, grid_coord.x(),
               grid_coord.y(),
               get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
                 ->to_string()
                 .c_str());
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
             "Fixed up net '%s' mapping mismatch at clustered block '%s' pin "
             "'grid[%ld][%ld].%s' (was net '%s')\n",
             routing_net_name.c_str(),
             clustering_ctx.clb_nlist.block_pb(blk_id)->name, grid_coord.x(),
             grid_coord.y(),
             get_pb_graph_node_pin_from_block_pin(blk_id, physical_pin)
               ->to_string()
               .c_str(),
             cluster_net_name.c_str());
    num_fixup++;
  }
  /* 2nd round of fixup: focus on global nets */
  status = update_cluster_pin_global_net_with_post_routing_results(
    clustering_ctx, vpr_clustering_annotation, blk_id, logical_block,
    map_gnet2msb, num_fixup, verbose);
  return status;
}

/********************************************************************
 * Main function to fix up the pb pin mapping results
 * This function will walk through each grid
 *******************************************************************/
int update_pb_pin_with_post_routing_results(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  const PlacementContext& placement_ctx,
  const VprRoutingAnnotation& vpr_routing_annotation,
  VprClusteringAnnotation& vpr_clustering_annotation, const bool& perimeter_cb,
  const bool& map_gnet2msb, const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  size_t num_fixup = 0;
  /* Confirm options */
  VTR_LOGV(verbose && map_gnet2msb,
           "User choose to map global net to the best fit MSB of input port\n");
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
        status = update_cluster_pin_with_post_routing_results(
          device_ctx, clustering_ctx, vpr_routing_annotation,
          vpr_clustering_annotation, layer, grid_coord, cluster_blk_id,
          NUM_SIDES, placement_ctx.block_locs[cluster_blk_id].loc.sub_tile,
          perimeter_cb, map_gnet2msb, num_fixup, verbose);
        if (status != CMD_EXEC_SUCCESS) {
          return CMD_EXEC_FATAL_ERROR;
        }
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
        status = update_cluster_pin_with_post_routing_results(
          device_ctx, clustering_ctx, vpr_routing_annotation,
          vpr_clustering_annotation, layer, io_coord, cluster_blk_id, io_side,
          placement_ctx.block_locs[cluster_blk_id].loc.sub_tile, perimeter_cb,
          map_gnet2msb, num_fixup, verbose);
        if (status != CMD_EXEC_SUCCESS) {
          return CMD_EXEC_FATAL_ERROR;
        }
      }
    }
  }
  VTR_LOG("In total %lu fixup have been applied\n", num_fixup);
  return status;
}

} /* end namespace openfpga */
