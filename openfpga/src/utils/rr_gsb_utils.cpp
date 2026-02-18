/********************************************************************
 * This file includes free utility functions that operate on RRGSB
 * objects but are specific to OpenFPGA and not part of the RRGSB
 * class itself.
 *
 * OpenFPGA-specific edge-data accessors and mutators (edge sorting,
 * get_chan_node_in_edges, is_sb_exist, etc.) live in RRGSBEdges
 * (rr_gsb_edges.h / rr_gsb_edges.cpp).
 *******************************************************************/
#include "vtr_assert.h"
#include "vtr_log.h"

#include "openfpga_side_manager.h"
#include "rr_graph_in_edges.h"
#include "rr_gsb_edges.h"
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
 * Find the configurable driver nodes for a channel node in the rr_graph
 ***********************************************************************/
std::vector<RRNodeId> get_rr_gsb_chan_node_configurable_driver_nodes(
  const RRGraphView& rr_graph, const RRGraphInEdges& in_edges,
  const RRGSB& rr_gsb, const RRGSBEdges& gsb_edges,
  const e_side& chan_side, const size_t& track_id) {
  std::vector<RRNodeId> driver_nodes;
  for (const RREdgeId& edge :
       gsb_edges.get_chan_node_in_edges(rr_gsb, in_edges, chan_side, track_id)) {
    if (false == rr_graph.edge_is_configurable(edge)) {
      continue;
    }
    driver_nodes.push_back(rr_graph.edge_src_node(edge));
  }
  return driver_nodes;
}

/* Post-sort overload: uses cached chan_node_in_edges_ data. */
std::vector<RRNodeId> get_rr_gsb_chan_node_configurable_driver_nodes(
  const RRGraphView& rr_graph,
  const RRGSB& /*rr_gsb*/, const RRGSBEdges& gsb_edges,
  const e_side& chan_side, const size_t& track_id) {
  std::vector<RRNodeId> driver_nodes;
  for (const RREdgeId& edge :
       gsb_edges.get_chan_node_in_edges(chan_side, track_id)) {
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
  if (base.get_type() != cand.get_type()) {
    return false;
  }
  if (base.get_chan_width() != cand.get_chan_width()) {
    return false;
  }
  for (size_t inode = 0; inode < base.get_chan_width(); ++inode) {
    if (rr_graph.node_type(base.get_node(inode)) !=
        rr_graph.node_type(cand.get_node(inode))) {
      return false;
    }
    if (rr_graph.node_direction(base.get_node(inode)) !=
        rr_graph.node_direction(cand.get_node(inode))) {
      return false;
    }
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
 * drive_rr_nodes */
static bool is_sb_node_mirror(const RRGraphView& rr_graph,
                              const RRGraphInEdges& in_edges,
                              const VprDeviceAnnotation& device_annotation,
                              const RRGSB& base, const RRGSBEdges& base_edges,
                              const RRGSB& cand, const RRGSBEdges& cand_edges,
                              const e_side& node_side, const size_t& track_id) {
  bool is_short_conkt =
    base.is_sb_node_passing_wire(rr_graph, node_side, track_id);

  if (is_short_conkt !=
      cand.is_sb_node_passing_wire(rr_graph, node_side, track_id)) {
    return false;
  }
  if (true == is_short_conkt) {
    return true;
  }

  std::vector<RREdgeId> node_in_edges =
    base_edges.get_chan_node_in_edges(base, in_edges, node_side, track_id);
  std::vector<RREdgeId> cand_node_in_edges =
    cand_edges.get_chan_node_in_edges(cand, in_edges, node_side, track_id);

  if (node_in_edges.size() != cand_node_in_edges.size()) {
    return false;
  }
  VTR_ASSERT(node_in_edges.size() == cand_node_in_edges.size());

  for (size_t iedge = 0; iedge < node_in_edges.size(); ++iedge) {
    RREdgeId src_edge = node_in_edges[iedge];
    RREdgeId src_cand_edge = cand_node_in_edges[iedge];
    RRNodeId src_node = rr_graph.edge_src_node(src_edge);
    RRNodeId src_cand_node = rr_graph.edge_src_node(src_cand_edge);
    if (rr_graph.node_type(src_node) != rr_graph.node_type(src_cand_node)) {
      return false;
    }
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
  const RRGraphView& rr_graph, const RRGraphInEdges& in_edges,
  const VprDeviceAnnotation& device_annotation,
  const RRGSB& base, const RRGSBEdges& base_edges,
  const RRGSB& cand, const RRGSBEdges& cand_edges,
  const e_side& side, const RRSegmentId& seg_id) {
  SideManager side_manager(side);

  VTR_ASSERT(side_manager.to_size_t() < base.get_num_sides());
  VTR_ASSERT(side_manager.to_size_t() < cand.get_num_sides());

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
    if (base.get_chan_node_direction(side, itrack) !=
        cand.get_chan_node_direction(side, itrack)) {
      return false;
    }
    if (OUT_PORT != base.get_chan_node_direction(side, itrack)) {
      continue;
    }
    if (false == is_sb_node_mirror(rr_graph, in_edges, device_annotation,
                                   base, base_edges, cand, cand_edges,
                                   side, itrack)) {
      return false;
    }
  }

  if (base.get_num_opin_nodes(side) != cand.get_num_opin_nodes(side)) {
    return false;
  }
  return true;
}

/** @brief check if a side of candidate SB is a mirror of the current one */
static bool is_sb_side_mirror(const RRGraphView& rr_graph,
                              const RRGraphInEdges& in_edges,
                              const VprDeviceAnnotation& device_annotation,
                              const RRGSB& base, const RRGSBEdges& base_edges,
                              const RRGSB& cand, const RRGSBEdges& cand_edges,
                              const e_side& side) {
  std::vector<RRSegmentId> seg_ids = base.get_chan_segment_ids(side);

  for (size_t iseg = 0; iseg < seg_ids.size(); ++iseg) {
    if (false == is_sb_side_segment_mirror(rr_graph, in_edges, device_annotation,
                                           base, base_edges, cand, cand_edges,
                                           side, seg_ids[iseg])) {
      return false;
    }
  }
  return true;
}

/** @brief Identify if the Switch Block part of two GSBs are mirror */
bool is_sb_mirror(const RRGraphView& rr_graph,
                  const RRGraphInEdges& in_edges,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSBEdges& base_edges,
                  const RRGSB& cand, const RRGSBEdges& cand_edges) {
  if (base.get_num_sides() != cand.get_num_sides()) {
    return false;
  }
  for (size_t side = 0; side < base.get_num_sides(); ++side) {
    SideManager side_manager(side);
    if (false == is_sb_side_mirror(rr_graph, in_edges, device_annotation,
                                   base, base_edges, cand, cand_edges,
                                   side_manager.get_side())) {
      return false;
    }
  }
  return true;
}

/** @brief Check if two ipin_nodes have a similar set of drive_rr_nodes */
static bool is_cb_node_mirror(const RRGraphView& rr_graph,
                              const RRGraphInEdges& in_edges,
                              const VprDeviceAnnotation& device_annotation,
                              const RRGSB& base, const RRGSBEdges& base_edges,
                              const RRGSB& cand, const RRGSBEdges& cand_edges,
                              const e_rr_type& cb_type, const e_side& node_side,
                              const size_t& node_id) {
  std::vector<RREdgeId> node_in_edges =
    base_edges.get_ipin_node_in_edges(base, in_edges, node_side, node_id);
  std::vector<RREdgeId> cand_node_in_edges =
    cand_edges.get_ipin_node_in_edges(cand, in_edges, node_side, node_id);
  if (node_in_edges.size() != cand_node_in_edges.size()) {
    return false;
  }

  for (size_t iedge = 0; iedge < node_in_edges.size(); ++iedge) {
    RREdgeId src_edge = node_in_edges[iedge];
    RREdgeId src_cand_edge = cand_node_in_edges[iedge];
    RRNodeId src_node = rr_graph.edge_src_node(src_edge);
    RRNodeId src_cand_node = rr_graph.edge_src_node(src_cand_edge);
    if (rr_graph.node_type(src_node) != rr_graph.node_type(src_cand_node)) {
      return false;
    }
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

/** @brief Check if the candidate CB is a mirror of the current baseline */
bool is_cb_mirror(const RRGraphView& rr_graph,
                  const RRGraphInEdges& in_edges,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSBEdges& base_edges,
                  const RRGSB& cand, const RRGSBEdges& cand_edges,
                  const e_rr_type& cb_type) {
  if (base.get_cb_chan_width(cb_type) != cand.get_cb_chan_width(cb_type)) {
    return false;
  }

  enum e_side chan_side = base.get_cb_chan_side(cb_type);

  if (false == is_chan_mirror(rr_graph, device_annotation, base.chan(chan_side),
                              cand.chan(chan_side))) {
    return false;
  }

  std::vector<enum e_side> ipin_side = base.get_cb_ipin_sides(cb_type);
  for (size_t side = 0; side < ipin_side.size(); ++side) {
    if (base.get_num_ipin_nodes(ipin_side[side]) !=
        cand.get_num_ipin_nodes(ipin_side[side])) {
      return false;
    }
    for (size_t inode = 0; inode < base.get_num_ipin_nodes(ipin_side[side]);
         ++inode) {
      if (false == is_cb_node_mirror(rr_graph, in_edges, device_annotation,
                                     base, base_edges, cand, cand_edges,
                                     cb_type, ipin_side[side], inode)) {
        return false;
      }
    }
  }
  return true;
}

} /* end namespace openfpga */
