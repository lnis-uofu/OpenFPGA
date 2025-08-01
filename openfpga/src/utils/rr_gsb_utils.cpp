/********************************************************************
 * This file includes most utilized functions for data structure
 * DeviceRRGSB
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"
#include "rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find if a X-direction or Y-direction Connection Block contains
 * routing tracks only (zero configuration bits and routing multiplexers)
 *******************************************************************/
bool connection_block_contain_only_routing_tracks(const RRGSB& rr_gsb,
                                                  const e_rr_type& cb_type) {
  bool routing_track_only = true;

  /* Find routing multiplexers on the sides of a Connection block where IPIN
   * nodes locate */
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    SideManager side_manager(cb_ipin_side);
    if (0 < rr_gsb.get_num_ipin_nodes(cb_ipin_side)) {
      routing_track_only = false;
      break;
    }
  }

  return routing_track_only;
}

/************************************************************************
 * Find the configurable driver nodes for a node in the rr_graph
 ***********************************************************************/
std::vector<RRNodeId> get_rr_gsb_chan_node_configurable_driver_nodes(
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_side& chan_side,
  const size_t& track_id) {
  std::vector<RRNodeId> driver_nodes;
  for (const RREdgeId& edge :
       rr_gsb.get_chan_node_in_edges(rr_graph, chan_side, track_id)) {
    /* Bypass non-configurable edges */
    if (false == rr_graph.edge_is_configurable(edge)) {
      continue;
    }
    driver_nodes.push_back(rr_graph.edge_src_node(edge));
  }

  return driver_nodes;
}

/** @brief Evaluate if two routing channels are mirror to each other */
static bool is_chan_mirror(const RRGraphView& rr_graph,
                           const VprDeviceAnnotation& device_annotation,
                           const RRChan& base, const RRChan& cand) {
  /* If any following element does not match, it is not mirror */
  /* 1. type  */
  if (base.get_type() != cand.get_type()) {
    return false;
  }
  /* 2. track_width  */
  if (base.get_chan_width() != cand.get_chan_width()) {
    return false;
  }
  /* 3. for each node */
  for (size_t inode = 0; inode < base.get_chan_width(); ++inode) {
    /* 3.1 check node type */
    if (rr_graph.node_type(base.get_node(inode)) !=
        rr_graph.node_type(cand.get_node(inode))) {
      return false;
    }
    /* 3.2 check node directionality */
    if (rr_graph.node_direction(base.get_node(inode)) !=
        rr_graph.node_direction(cand.get_node(inode))) {
      return false;
    }
    /* 3.3 check node segment */
    /* FIXME: Maybe this is too tight! Consider to remove the restrictions on
     * segments */
    if (device_annotation.rr_segment_circuit_model(
          base.get_node_segment(inode)) !=
        device_annotation.rr_segment_circuit_model(
          cand.get_node_segment(inode))) {
      return false;
    }
  }

  return true;
}

/** @brief check if two rr_nodes in two GSBs have a similar set of
 * drive_rr_nodes for each drive_rr_node:
 * 1. CHANX or CHANY: should have the same side and index
 * 2. OPIN or IPIN: should have the same side and index
 * 3. each drive_rr_switch should be the same
 */
static bool is_sb_node_mirror(const RRGraphView& rr_graph,
                              const VprDeviceAnnotation& device_annotation,
                              const RRGSB& base, const RRGSB& cand,
                              const e_side& node_side, const size_t& track_id) {
  /* Ensure rr_nodes are either the output of short-connection or multiplexer */
  bool is_short_conkt =
    base.is_sb_node_passing_wire(rr_graph, node_side, track_id);

  if (is_short_conkt !=
      cand.is_sb_node_passing_wire(rr_graph, node_side, track_id)) {
    return false;
  }

  if (true == is_short_conkt) {
    /* Since, both are pass wires,
     * The two node should be equivalent
     * we can return here
     */
    return true;
  }

  /* Use unsorted/sorted edges */
  std::vector<RREdgeId> node_in_edges =
    base.get_chan_node_in_edges(rr_graph, node_side, track_id);
  std::vector<RREdgeId> cand_node_in_edges =
    cand.get_chan_node_in_edges(rr_graph, node_side, track_id);

  /* For non-passing wires, check driving rr_nodes */
  if (node_in_edges.size() != cand_node_in_edges.size()) {
    return false;
  }

  VTR_ASSERT(node_in_edges.size() == cand_node_in_edges.size());

  for (size_t iedge = 0; iedge < node_in_edges.size(); ++iedge) {
    RREdgeId src_edge = node_in_edges[iedge];
    RREdgeId src_cand_edge = cand_node_in_edges[iedge];
    RRNodeId src_node = rr_graph.edge_src_node(src_edge);
    RRNodeId src_cand_node = rr_graph.edge_src_node(src_cand_edge);
    /* node type should be the same  */
    if (rr_graph.node_type(src_node) != rr_graph.node_type(src_cand_node)) {
      return false;
    }
    /* switch type should be the same  */
    if (device_annotation.rr_switch_circuit_model(
          RRSwitchId(rr_graph.edge_switch(src_edge))) !=
        device_annotation.rr_switch_circuit_model(
          RRSwitchId(rr_graph.edge_switch(src_cand_edge)))) {
      return false;
    }
    int src_node_id, des_node_id;
    enum e_side src_node_side, des_node_side;
    base.get_node_side_and_index(rr_graph, src_node, OUT_PORT, src_node_side,
                                 src_node_id);
    cand.get_node_side_and_index(rr_graph, src_cand_node, OUT_PORT,
                                 des_node_side, des_node_id);
    if (src_node_id != des_node_id) {
      return false;
    }
    if (src_node_side != des_node_side) {
      return false;
    }
  }

  return true;
}

/** @brief Check if all the routing segments of a side of candidate SB is a
 * mirror of the current one */
static bool is_sb_side_segment_mirror(
  const RRGraphView& rr_graph, const VprDeviceAnnotation& device_annotation,
  const RRGSB& base, const RRGSB& cand, const e_side& side,
  const RRSegmentId& seg_id) {
  /* Create a side manager */
  SideManager side_manager(side);

  /* Make sure both Switch blocks has this side!!! */
  VTR_ASSERT(side_manager.to_size_t() < base.get_num_sides());
  VTR_ASSERT(side_manager.to_size_t() < cand.get_num_sides());

  /* check the numbers/directionality of channel rr_nodes */
  /* Ensure we have the same channel width on this side */
  if (base.get_chan_width(side) != cand.get_chan_width(side)) {
    return false;
  }
  for (size_t itrack = 0; itrack < base.get_chan_width(side); ++itrack) {
    /* FIXME: Maybe this is too tight! Consider to remove the restrictions on
     * segments */
    if (device_annotation.rr_segment_circuit_model(seg_id) !=
        device_annotation.rr_segment_circuit_model(
          base.get_chan_node_segment(side, itrack))) {
      continue;
    }
    /* Check the directionality of each node */
    if (base.get_chan_node_direction(side, itrack) !=
        cand.get_chan_node_direction(side, itrack)) {
      return false;
    }
    /* Check the track_id of each node
     * ptc is not necessary, we care the connectivity!
     */
    /* For OUT_PORT rr_node, we need to check fan-in */
    if (OUT_PORT != base.get_chan_node_direction(side, itrack)) {
      continue; /* skip IN_PORT */
    }

    if (false == is_sb_node_mirror(rr_graph, device_annotation, base, cand,
                                   side, itrack)) {
      return false;
    }
  }

  /* check the numbers of opin_rr_nodes */
  if (base.get_num_opin_nodes(side) != cand.get_num_opin_nodes(side)) {
    return false;
  }

  return true;
}

/** @brief check if a side of candidate SB is a mirror of the current one
 * Check the specified side of two switch blocks:
 * 1. Number of channel/opin/ipin rr_nodes are same
 * For channel rr_nodes
 * 2. check if their track_ids (ptc_num) are same
 * 3. Check if the switches (ids) are same
 * For opin/ipin rr_nodes,
 * 4. check if their parent type_descriptors same,
 * 5. check if pin class id and pin id are same
 * If all above are satisfied, the side of the two switch blocks are mirrors!
 */
static bool is_sb_side_mirror(const RRGraphView& rr_graph,
                              const VprDeviceAnnotation& device_annotation,
                              const RRGSB& base, const RRGSB& cand,
                              const e_side& side) {
  /* get a list of segments */
  std::vector<RRSegmentId> seg_ids = base.get_chan_segment_ids(side);

  for (size_t iseg = 0; iseg < seg_ids.size(); ++iseg) {
    if (false == is_sb_side_segment_mirror(rr_graph, device_annotation, base,
                                           cand, side, seg_ids[iseg])) {
      return false;
    }
  }

  return true;
}

/** @brief Identify if the Switch Block part of two GSBs are mirror (same in
 * structure) or not. Return true if so, otherwise return false Idenify mirror
 * Switch blocks Check each two switch blocks:
 * 1. Number of channel/opin/ipin rr_nodes are same
 * For channel rr_nodes
 * 2. check if their track_ids (ptc_num) are same
 * 3. Check if the switches (ids) are same
 * For opin/ipin rr_nodes,
 * 4. check if their parent type_descriptors same,
 * 5. check if pin class id and pin id are same
 * If all above are satisfied, the two switch blocks are mirrors!
 */
bool is_sb_mirror(const RRGraphView& rr_graph,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSB& cand) {
  /* check the numbers of sides */
  if (base.get_num_sides() != cand.get_num_sides()) {
    return false;
  }

  /* check the numbers/directionality of channel rr_nodes */
  for (size_t side = 0; side < base.get_num_sides(); ++side) {
    SideManager side_manager(side);
    if (false == is_sb_side_mirror(rr_graph, device_annotation, base, cand,
                                   side_manager.get_side())) {
      return false;
    }
  }

  return true;
}

/** @brief Check if two ipin_nodes have a similar set of drive_rr_nodes for each
 * drive_rr_node:
 * 1. CHANX or CHANY: should have the same side and index
 * 2. each drive_rr_switch should be the same
 */
static bool is_cb_node_mirror(const RRGraphView& rr_graph,
                              const VprDeviceAnnotation& device_annotation,
                              const RRGSB& base, const RRGSB& cand,
                              const e_rr_type& cb_type, const e_side& node_side,
                              const size_t& node_id) {
  /* Ensure rr_nodes are either the output of short-connection or multiplexer */
  std::vector<RREdgeId> node_in_edges =
    base.get_ipin_node_in_edges(rr_graph, node_side, node_id);
  std::vector<RREdgeId> cand_node_in_edges =
    cand.get_ipin_node_in_edges(rr_graph, node_side, node_id);
  if (node_in_edges.size() != cand_node_in_edges.size()) {
    return false;
  }

  for (size_t iedge = 0; iedge < node_in_edges.size(); ++iedge) {
    RREdgeId src_edge = node_in_edges[iedge];
    RREdgeId src_cand_edge = cand_node_in_edges[iedge];
    RRNodeId src_node = rr_graph.edge_src_node(src_edge);
    RRNodeId src_cand_node = rr_graph.edge_src_node(src_cand_edge);
    /* node type should be the same  */
    if (rr_graph.node_type(src_node) != rr_graph.node_type(src_cand_node)) {
      return false;
    }
    /* switch type should be the same  */
    if (device_annotation.rr_switch_circuit_model(
          RRSwitchId(rr_graph.edge_switch(src_edge))) !=
        device_annotation.rr_switch_circuit_model(
          RRSwitchId(rr_graph.edge_switch(src_cand_edge)))) {
      return false;
    }

    int src_node_id, des_node_id;
    enum e_side src_node_side, des_node_side;
    enum e_side chan_side = base.get_cb_chan_side(cb_type);
    switch (rr_graph.node_type(src_node)) {
      case e_rr_type::CHANX:
      case e_rr_type::CHANY:
        /* if the drive rr_nodes are routing tracks, find index  */
        src_node_id = base.get_chan_node_index(chan_side, src_node);
        des_node_id = cand.get_chan_node_index(chan_side, src_cand_node);
        break;
      case e_rr_type::OPIN:
        base.get_node_side_and_index(rr_graph, src_node, OUT_PORT,
                                     src_node_side, src_node_id);
        cand.get_node_side_and_index(rr_graph, src_cand_node, OUT_PORT,
                                     des_node_side, des_node_id);
        if (src_node_side != des_node_side) {
          return false;
        }
        break;
      default:
        VTR_LOG("Invalid type of drive_rr_nodes for ipin_node!\n");
        exit(1);
    }
    if (src_node_id != des_node_id) {
      return false;
    }
  }

  return true;
}

/** @brief Check if the candidate CB is a mirror of the current baselien */
bool is_cb_mirror(const RRGraphView& rr_graph,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSB& cand,
                  const e_rr_type& cb_type) {
  /* Check if channel width is the same */
  if (base.get_cb_chan_width(cb_type) != cand.get_cb_chan_width(cb_type)) {
    return false;
  }

  enum e_side chan_side = base.get_cb_chan_side(cb_type);

  /* check the numbers/directionality of channel rr_nodes */
  if (false == is_chan_mirror(rr_graph, device_annotation, base.chan(chan_side),
                              cand.chan(chan_side))) {
    return false;
  }

  /* check the equivalence of ipins */
  std::vector<enum e_side> ipin_side = base.get_cb_ipin_sides(cb_type);
  for (size_t side = 0; side < ipin_side.size(); ++side) {
    /* Ensure we have the same number of IPINs on this side */
    if (base.get_num_ipin_nodes(ipin_side[side]) !=
        cand.get_num_ipin_nodes(ipin_side[side])) {
      return false;
    }
    for (size_t inode = 0; inode < base.get_num_ipin_nodes(ipin_side[side]);
         ++inode) {
      if (false == is_cb_node_mirror(rr_graph, device_annotation, base, cand,
                                     cb_type, ipin_side[side], inode)) {
        return false;
      }
    }
  }

  return true;
}

} /* end namespace openfpga */
