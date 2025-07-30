/********************************************************************
 * This file includes functions that are used to annotate routing results
 * from VPR to OpenFPGA
 *******************************************************************/
#include "openfpga_annotate_routing.h"

#include "annotate_routing.h"
#include "physical_types_util.h"
#include "route_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Create a mapping between each rr_node and its mapped nets
 * - Only applicable to global nets for dedicated clock routing purpose
 * - Note that this function is different than annotate_vpr_rr_nodes()
 *   Please do not annotate global nets in vpr_routing_annotation!
 *******************************************************************/
vtr::vector<RRNodeId, ClusterNetId> annotate_rr_node_global_net(
  const DeviceContext& device_ctx, const ClusteredNetlist& cluster_nlist,
  const PlacementContext& placement_ctx,
  const VprClusteringAnnotation& clustering_annotation, const bool& verbose) {
  vtr::vector<RRNodeId, ClusterNetId> rr_node_nets;

  size_t counter = 0;
  vtr::ScopedStartFinishTimer timer("Annotating rr_node with global nets");

  const auto& rr_graph = device_ctx.rr_graph;

  rr_node_nets.resize(rr_graph.num_nodes(), ClusterNetId::INVALID());

  size_t layer = 0;

  for (ClusterNetId net_id : cluster_nlist.nets()) {
    if (!cluster_nlist.net_is_ignored(net_id)) {
      continue;
    }
    /* Walk through all the sinks */
    for (ClusterPinId pin_id : cluster_nlist.net_pins(net_id)) {
      ClusterBlockId block_id = cluster_nlist.pin_block(pin_id);
      t_block_loc blk_loc = get_block_loc(block_id, false);
      int phy_pin = placement_ctx.physical_pins()[pin_id];
      t_physical_tile_type_ptr phy_tile = device_ctx.grid.get_physical_type(
        t_physical_tile_loc(blk_loc.loc.x, blk_loc.loc.y, 0));
      int node_pin_num = phy_tile->num_pins;
      /* Note that the phy_pin may not reflect the actual pin index at the
       * top-level physical tile type. It could be one of the random pin to the
       * same pin class. So here, we have to find an exact match of the pin
       * index from the clustering results! */
      int subtile_idx = blk_loc.loc.sub_tile;
      auto logical_block = cluster_nlist.block_type(block_id);
      for (int j = 0; j < logical_block->pb_type->num_pins; j++) {
        /* Find the net mapped to this pin in clustering results*/
        ClusterNetId cluster_net_id = cluster_nlist.block_net(block_id, j);
        /* Get the actual net id because it may be renamed during routing */
        if (true == clustering_annotation.is_net_renamed(block_id, j)) {
          cluster_net_id = clustering_annotation.net(block_id, j);
        }
        /* Bypass unmatched pins */
        if (cluster_net_id != net_id) {
          continue;
        }
        int curr_pin_num = get_physical_pin_at_sub_tile_location(
          phy_tile, logical_block, subtile_idx, j);
        if (phy_tile->pin_class[curr_pin_num] != phy_tile->pin_class[phy_pin]) {
          continue;
        }
        node_pin_num = curr_pin_num;
        break;
      }
      VTR_ASSERT(node_pin_num < phy_tile->num_pins);
      e_rr_type rr_pin_type = e_rr_type::IPIN;
      if (phy_tile->class_inf[phy_tile->pin_class[node_pin_num]].type ==
          e_pin_type::RECEIVER) {
        rr_pin_type = e_rr_type::IPIN;
      } else if (phy_tile->class_inf[phy_tile->pin_class[node_pin_num]].type ==
                 e_pin_type::DRIVER) {
        rr_pin_type = e_rr_type::OPIN;
      } else {
        VTR_LOG_ERROR(
          "When annotating global net '%s', invalid rr node pin type for '%s' "
          "pin '%d'\n",
          cluster_nlist.net_name(net_id).c_str(), phy_tile->name.c_str(),
          node_pin_num);
        exit(1);
      }
      std::vector<RRNodeId> curr_rr_nodes =
        rr_graph.node_lookup().find_nodes_at_all_sides(
          layer, blk_loc.loc.x, blk_loc.loc.y, rr_pin_type, node_pin_num);
      for (RRNodeId curr_rr_node : curr_rr_nodes) {
        VTR_LOGV(verbose, "Annotate global net '%s' on '%s' pin '%d'\n",
                 cluster_nlist.net_name(net_id).c_str(), phy_tile->name.c_str(),
                 node_pin_num);
        rr_node_nets[curr_rr_node] = net_id;
        counter++;
      }
    }
  }

  VTR_LOGV(verbose, "Done with %d nodes mapping\n", counter);

  return rr_node_nets;
}

/********************************************************************
 * Create a mapping between each rr_node and its mapped nets
 * based on VPR routing results
 * - Unmapped rr_node will use invalid ids
 *******************************************************************/
void annotate_vpr_rr_node_nets(const DeviceContext& device_ctx,
                               const ClusteringContext& clustering_ctx,
                               const AtomContext& atom_ctx,
                               VprRoutingAnnotation& vpr_routing_annotation,
                               const bool& verbose) {
  vtr::vector<RRNodeId, ClusterNetId> node2net =
    annotate_rr_node_nets(clustering_ctx, device_ctx, atom_ctx, verbose);
  for (size_t node_id = 0; node_id < device_ctx.rr_graph.num_nodes();
       ++node_id) {
    vpr_routing_annotation.set_rr_node_net(RRNodeId(node_id),
                                           node2net[RRNodeId(node_id)]);
  }
  VTR_LOG("Loaded node-to-net mapping\n");
}

/********************************************************************
 * Create a mapping between each rr_node and its previous node
 * based on VPR routing results
 * - Unmapped rr_node will have an invalid id of previous rr_node
 *******************************************************************/
void annotate_rr_node_previous_nodes(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  VprRoutingAnnotation& vpr_routing_annotation, const bool& verbose) {
  size_t counter = 0;
  VTR_LOG("Annotating previous nodes for rr_node...");
  VTR_LOGV(verbose, "\n");

  auto& netlist = clustering_ctx.clb_nlist;

  for (auto net_id : netlist.nets()) {
    /* Ignore nets that are not routed */
    if (true == netlist.net_is_ignored(net_id)) {
      continue;
    }
    /* Ignore used in local cluster only, reserved one CLB pin */
    if (false == netlist.net_sinks(net_id).size()) {
      continue;
    }

    auto& tree = get_route_tree_from_cluster_net_id(net_id);
    if (!tree) {
      continue;
    }

    for (auto& rt_node : tree->all_nodes()) {
      RRNodeId rr_node = rt_node.inode;
      auto parent = rt_node.parent();
      vpr_routing_annotation.set_rr_node_prev_node(
        device_ctx.rr_graph, rr_node,
        parent ? parent->inode : RRNodeId::INVALID());
    }
  }

  VTR_LOG("Done with %d nodes mapping\n", counter);
}

} /* end namespace openfpga */
