/********************************************************************
 * Member function implementations for RRGSBEdges.
 *
 * These were previously implemented as RRGSB member functions in
 * rr_gsb_utils.cpp (using RRGSB:: scope). They have been converted
 * to RRGSBEdges methods, replacing every direct RRGSB private member
 * access with the equivalent public RRGSB accessor:
 *
 *   chan_node_[s].get_chan_width()      → gsb.get_chan_width(s)
 *   chan_node_[s].get_node(t)           → gsb.get_chan_node(s, t)
 *   chan_node_direction_[s][t]          → gsb.get_chan_node_direction(s, t)
 *   opin_node_[s].size()               → gsb.get_num_opin_nodes(s)
 *   ipin_node_[s][i]                   → gsb.get_ipin_node(s, i)
 *   ipin_node_[s].size()               → gsb.get_num_ipin_nodes(s)
 *******************************************************************/

#include <algorithm>
#include <map>
#include <unordered_map>

#include "openfpga_side_manager.h"
#include "rr_graph_in_edges.h"
#include "rr_gsb_edges.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/********************************************************************
 * Accessors
 *******************************************************************/

std::vector<RREdgeId> RRGSBEdges::get_chan_node_in_edges(
  const RRGSB& gsb,
  const RRGraphInEdges& in_edges,
  const e_side& side,
  const size_t& track_id) const {
  SideManager side_manager(side);
  VTR_ASSERT(side_manager.validate());
  VTR_ASSERT(OUT_PORT == gsb.get_chan_node_direction(side, track_id));

  /* If sorted edges are available, return them; otherwise fall back to
   * unsorted edges from the rr_graph */
  if (chan_node_in_edges_.empty()) {
    std::vector<RREdgeId> unsorted_edges;
    for (const RREdgeId& edge :
         in_edges.node_in_edges(gsb.get_chan_node(side, track_id))) {
      unsorted_edges.push_back(edge);
    }
    return unsorted_edges;
  }

  return chan_node_in_edges_[side_manager.to_size_t()][track_id];
}

std::vector<RREdgeId> RRGSBEdges::get_ipin_node_in_edges(
  const RRGSB& gsb,
  const RRGraphInEdges& in_edges,
  const e_side& side,
  const size_t& ipin_id) const {
  SideManager side_manager(side);
  VTR_ASSERT(side_manager.validate());

  /* If sorted edges are available, return them; otherwise fall back */
  if (ipin_node_in_edges_.empty()) {
    std::vector<RREdgeId> unsorted_edges;
    for (const RREdgeId& edge :
         in_edges.node_in_edges(gsb.get_ipin_node(side, ipin_id))) {
      unsorted_edges.push_back(edge);
    }
    return unsorted_edges;
  }

  return ipin_node_in_edges_[side_manager.to_size_t()][ipin_id];
}

const std::vector<RREdgeId>& RRGSBEdges::get_chan_node_in_edges(
  const e_side& side,
  const size_t& track_id) const {
  SideManager side_manager(side);
  VTR_ASSERT(side_manager.validate());
  VTR_ASSERT(!chan_node_in_edges_.empty());
  return chan_node_in_edges_[side_manager.to_size_t()][track_id];
}

const std::vector<RREdgeId>& RRGSBEdges::get_ipin_node_in_edges(
  const e_side& side,
  const size_t& ipin_id) const {
  SideManager side_manager(side);
  VTR_ASSERT(side_manager.validate());
  VTR_ASSERT(!ipin_node_in_edges_.empty());
  return ipin_node_in_edges_[side_manager.to_size_t()][ipin_id];
}

bool RRGSBEdges::is_sb_exist(const RRGSB& gsb,
                              const RRGraphInEdges& in_edges) const {
  size_t num_sides_routing_wires = 0;
  size_t num_sides_opin_nodes = 0;
  for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    if (0 != gsb.get_chan_width(side_manager.get_side())) {
      num_sides_routing_wires++;
    }
    if (0 != gsb.get_num_opin_nodes(side_manager.get_side())) {
      num_sides_opin_nodes++;
    }
  }

  /* When there are zero nodes, the SB does not exist */
  if (num_sides_routing_wires == 0 && num_sides_opin_nodes == 0) {
    return false;
  }

  /* If there is only 1 side of routing wires and zero OPIN nodes, the SB
   * exists only if at least one outgoing channel node has an incoming edge */
  if (num_sides_routing_wires == 1 && num_sides_opin_nodes == 0) {
    size_t num_incoming_edges = 0;
    for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
      SideManager side_manager(side);
      for (size_t itrack = 0;
           itrack < gsb.get_chan_width(side_manager.get_side()); ++itrack) {
        if (OUT_PORT !=
            gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
          continue;
        }
        num_incoming_edges +=
          get_chan_node_in_edges(gsb, in_edges, side_manager.get_side(), itrack)
            .size();
      }
    }
    return num_incoming_edges != 0;
  }

  return true;
}

bool RRGSBEdges::is_sb_exist(const RRGSB& gsb) const {
  size_t num_sides_routing_wires = 0;
  size_t num_sides_opin_nodes = 0;
  for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    if (0 != gsb.get_chan_width(side_manager.get_side())) {
      num_sides_routing_wires++;
    }
    if (0 != gsb.get_num_opin_nodes(side_manager.get_side())) {
      num_sides_opin_nodes++;
    }
  }

  if (num_sides_routing_wires == 0 && num_sides_opin_nodes == 0) {
    return false;
  }

  if (num_sides_routing_wires == 1 && num_sides_opin_nodes == 0) {
    /* Must have sorted edges to use this overload */
    VTR_ASSERT(!chan_node_in_edges_.empty());
    size_t num_incoming_edges = 0;
    for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
      SideManager side_manager(side);
      for (size_t itrack = 0;
           itrack < gsb.get_chan_width(side_manager.get_side()); ++itrack) {
        if (OUT_PORT !=
            gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
          continue;
        }
        num_incoming_edges += chan_node_in_edges_[side][itrack].size();
      }
    }
    return num_incoming_edges != 0;
  }

  return true;
}

size_t RRGSBEdges::get_num_cb_opin_nodes(const e_rr_type& cb_type,
                                          const e_side& side) const {
  size_t icb_type = (cb_type == e_rr_type::CHANX) ? 0 : 1;
  SideManager side_manager(side);
  VTR_ASSERT(side_manager.validate());
  return cb_opin_node_[icb_type][side_manager.to_size_t()].size();
}

RRNodeId RRGSBEdges::get_cb_opin_node(const e_rr_type& cb_type,
                                       const e_side& side,
                                       const size_t& node_id) const {
  size_t icb_type = (cb_type == e_rr_type::CHANX) ? 0 : 1;
  SideManager side_manager(side);
  VTR_ASSERT(side_manager.validate());
  VTR_ASSERT(node_id < cb_opin_node_[icb_type][side_manager.to_size_t()].size());
  return cb_opin_node_[icb_type][side_manager.to_size_t()][node_id];
}

/********************************************************************
 * Public mutators
 *******************************************************************/

void RRGSBEdges::sort_chan_node_in_edges(const RRGSB& gsb,
                                         const RRGraphView& rr_graph,
                                         const RRGraphInEdges& in_edges,
                                         const bool reorder_incoming_edges) {
  chan_node_in_edges_.resize(gsb.get_num_sides());

  for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    chan_node_in_edges_[side].resize(gsb.get_chan_width(side_manager.get_side()));
    for (size_t track_id = 0;
         track_id < gsb.get_chan_width(side_manager.get_side()); ++track_id) {
      /* Only sort output nodes; bypass passing wires */
      if ((OUT_PORT == gsb.get_chan_node_direction(side_manager.get_side(), track_id)) &&
          (false == gsb.is_sb_node_passing_wire(rr_graph, side_manager.get_side(), track_id))) {
        sort_chan_node_in_edges(gsb, rr_graph, in_edges,
                                side_manager.get_side(), track_id,
                                reorder_incoming_edges);
      }
    }
  }
}

void RRGSBEdges::sort_ipin_node_in_edges(const RRGSB& gsb,
                                          const RRGraphView& rr_graph,
                                          const RRGraphInEdges& in_edges) {
  ipin_node_in_edges_.resize(gsb.get_num_sides());

  for (e_rr_type cb_type : {e_rr_type::CHANX, e_rr_type::CHANY}) {
    for (e_side ipin_side : gsb.get_cb_ipin_sides(cb_type)) {
      SideManager side_manager(ipin_side);
      ipin_node_in_edges_[size_t(ipin_side)].resize(
        gsb.get_num_ipin_nodes(ipin_side));
      for (size_t ipin_id = 0; ipin_id < gsb.get_num_ipin_nodes(ipin_side);
           ++ipin_id) {
        sort_ipin_node_in_edges(gsb, rr_graph, in_edges,
                                side_manager.get_side(), ipin_id);
      }
    }
  }
}

void RRGSBEdges::build_cb_opin_nodes(const RRGSB& gsb,
                                      const RRGraphView& rr_graph,
                                      const RRGraphInEdges& in_edges) {
  for (e_rr_type cb_type : {e_rr_type::CHANX, e_rr_type::CHANY}) {
    size_t icb_type = (cb_type == e_rr_type::CHANX) ? 0 : 1;
    for (e_side cb_ipin_side : gsb.get_cb_ipin_sides(cb_type)) {
      for (size_t inode = 0; inode < gsb.get_num_ipin_nodes(cb_ipin_side);
           ++inode) {
        for (const RREdgeId curr_edge :
             get_ipin_node_in_edges(gsb, in_edges, cb_ipin_side, inode)) {
          RRNodeId cand_node = rr_graph.edge_src_node(curr_edge);
          if (e_rr_type::OPIN != rr_graph.node_type(cand_node)) {
            continue;
          }
          enum e_side cb_opin_side = NUM_2D_SIDES;
          int cb_opin_index = -1;
          gsb.get_node_side_and_index(rr_graph, cand_node, IN_PORT,
                                      cb_opin_side, cb_opin_index);
          if ((-1 == cb_opin_index) || (NUM_2D_SIDES == cb_opin_side)) {
            VTR_LOG_DEBUG("GSB[%lu][%lu]:\n", gsb.get_x(), gsb.get_y());
            VTR_LOG_DEBUG("----------------------------------\n");
            VTR_LOG_DEBUG("SRC node:\n");
            VTR_LOG_DEBUG("Node info: %s\n",
                          rr_graph.node_coordinate_to_string(cand_node).c_str());
            VTR_LOG_DEBUG("Node ptc: %d\n", rr_graph.node_ptc_num(cand_node));
            VTR_LOG_DEBUG("Fan-out nodes:\n");
            for (const auto& temp_edge : rr_graph.edge_range(cand_node)) {
              VTR_LOG_DEBUG(
                "\t%s\n",
                rr_graph.node_coordinate_to_string(
                  rr_graph.edge_sink_node(temp_edge)).c_str());
            }
          }
          VTR_ASSERT((-1 != cb_opin_index) && (NUM_2D_SIDES != cb_opin_side));

          auto& opin_list =
            cb_opin_node_[icb_type][size_t(cb_opin_side)];
          if (opin_list.end() ==
              std::find(opin_list.begin(), opin_list.end(), cand_node)) {
            opin_list.push_back(cand_node);
          }
        }
      }
    }
  }
}

/********************************************************************
 * Private sort helpers (per-node)
 *******************************************************************/

void RRGSBEdges::sort_chan_node_in_edges(const RRGSB& gsb,
                                          const RRGraphView& rr_graph,
                                          const RRGraphInEdges& in_edges,
                                          const e_side& chan_side,
                                          const size_t track_id,
                                          const bool reorder_incoming_edges) {
  std::unordered_map<size_t, std::unordered_map<size_t, RREdgeId>>
    from_grid_edge_map;
  std::unordered_map<size_t, std::unordered_map<size_t, RREdgeId>>
    from_track_edge_map;

  const RRNodeId& chan_node = gsb.get_chan_node(chan_side, track_id);
  size_t edge_counter = 0;

  /* Classify each incoming edge by whether its source is an OPIN (grid)
   * or a routing track, then record its side/index within this GSB */
  for (const RREdgeId& edge : in_edges.node_in_edges(chan_node)) {
    const RRNodeId& src_node = rr_graph.edge_src_node(edge);
    e_side side = NUM_2D_SIDES;
    int index = 0;
    gsb.get_node_side_and_index(rr_graph, src_node, IN_PORT, side, index);

    if (NUM_2D_SIDES == side) {
      VTR_LOG_DEBUG("GSB[%lu][%lu]:\n", gsb.get_x(), gsb.get_y());
      VTR_LOG_DEBUG("----------------------------------\n");
      VTR_LOG_DEBUG("SRC node:\n");
      VTR_LOG_DEBUG("Node info: %s\n",
                    rr_graph.node_coordinate_to_string(src_node).c_str());
      VTR_LOG_DEBUG("Node ptc: %d\n", rr_graph.node_ptc_num(src_node));
      VTR_LOG_DEBUG("Fan-out nodes:\n");
      for (const auto& temp_edge : rr_graph.edge_range(src_node)) {
        VTR_LOG_DEBUG(
          "\t%s\n",
          rr_graph.node_coordinate_to_string(
            rr_graph.edge_sink_node(temp_edge)).c_str());
      }
      VTR_LOG_DEBUG("\n----------------------------------\n");
      VTR_LOG_DEBUG("Channel node:\n");
      VTR_LOG_DEBUG("Node info: %s\n",
                    rr_graph.node_coordinate_to_string(chan_node).c_str());
      VTR_LOG_DEBUG("Node ptc: %d\n", rr_graph.node_ptc_num(chan_node));
      VTR_LOG_DEBUG("Fan-in nodes:\n");
      for (const auto& temp_edge : in_edges.node_in_edges(chan_node)) {
        VTR_LOG_DEBUG("\t%s\n",
                      rr_graph.node_coordinate_to_string(
                        rr_graph.edge_src_node(temp_edge)).c_str());
      }
    }

    VTR_ASSERT(NUM_2D_SIDES != side);
    VTR_ASSERT(UNDEFINED != index);

    if (e_rr_type::OPIN == rr_graph.node_type(src_node)) {
      from_grid_edge_map[side][index] = edge;
    } else {
      VTR_ASSERT((e_rr_type::CHANX == rr_graph.node_type(src_node)) ||
                 (e_rr_type::CHANY == rr_graph.node_type(src_node)));
      from_track_edge_map[side][index] = edge;
    }
    edge_counter++;
  }

  auto& sorted = chan_node_in_edges_[size_t(chan_side)][track_id];

  if (reorder_incoming_edges) {
    /* OPINs first (all sides), then routing tracks (all sides) */
    for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
      for (size_t opin_id = 0; opin_id < gsb.get_num_opin_nodes(SideManager(side).get_side()); ++opin_id) {
        if (from_grid_edge_map.contains(side) &&
            from_grid_edge_map.at(side).contains(opin_id)) {
          sorted.push_back(from_grid_edge_map[side][opin_id]);
        }
      }
    }
    for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
      for (size_t itrack = 0;
           itrack < gsb.get_chan_width(SideManager(side).get_side()); ++itrack) {
        if (from_track_edge_map.contains(side) &&
            from_track_edge_map.at(side).contains(itrack)) {
          sorted.push_back(from_track_edge_map[side][itrack]);
        }
      }
    }
  } else {
    /* Per side: OPINs first, then routing tracks */
    for (size_t side = 0; side < gsb.get_num_sides(); ++side) {
      for (size_t opin_id = 0; opin_id < gsb.get_num_opin_nodes(SideManager(side).get_side()); ++opin_id) {
        if (from_grid_edge_map.contains(side) &&
            from_grid_edge_map.at(side).contains(opin_id)) {
          sorted.push_back(from_grid_edge_map[side][opin_id]);
        }
      }
      for (size_t itrack = 0;
           itrack < gsb.get_chan_width(SideManager(side).get_side()); ++itrack) {
        if (from_track_edge_map.contains(side) &&
            from_track_edge_map.at(side).contains(itrack)) {
          sorted.push_back(from_track_edge_map[side][itrack]);
        }
      }
    }
  }

  VTR_ASSERT(edge_counter == sorted.size());
}

void RRGSBEdges::sort_ipin_node_in_edges(const RRGSB& gsb,
                                          const RRGraphView& rr_graph,
                                          const RRGraphInEdges& in_edges,
                                          const e_side& ipin_side,
                                          const size_t& ipin_id) {
  std::map<size_t, std::map<size_t, RREdgeId>> from_track_edge_map;
  std::array<std::map<size_t, RREdgeId>, NUM_2D_SIDES> from_opin_edge_map;
  size_t edge_counter = 0;

  const RRNodeId& ipin_node = gsb.get_ipin_node(ipin_side, ipin_id);

  /* First pass: classify routing-track source edges */
  for (const RREdgeId& edge : in_edges.node_in_edges(ipin_node)) {
    const RRNodeId& src_node = rr_graph.edge_src_node(edge);
    if (e_rr_type::CHANX != rr_graph.node_type(src_node) &&
        e_rr_type::CHANY != rr_graph.node_type(src_node)) {
      continue;
    }

    int index = UNDEFINED;
    e_side chan_side = gsb.get_cb_chan_side(ipin_side);

    index = gsb.get_node_index(rr_graph, src_node, chan_side, IN_PORT);
    if (UNDEFINED == index) {
      index = gsb.get_node_index(rr_graph, src_node, chan_side, OUT_PORT);
    }
    if (UNDEFINED == index) {
      for (size_t side = 0; side < NUM_2D_SIDES; ++side) {
        SideManager side_manager(side);
        chan_side = side_manager.get_side();
        index = gsb.get_node_index(rr_graph, src_node, chan_side, IN_PORT);
        if (UNDEFINED != index) break;
        index = gsb.get_node_index(rr_graph, src_node, chan_side, OUT_PORT);
        if (UNDEFINED != index) break;
      }
    }

    if (UNDEFINED == index) {
      VTR_LOG_DEBUG("GSB[%lu][%lu]:\n", gsb.get_x(), gsb.get_y());
      VTR_LOG_DEBUG("----------------------------------\n");
      VTR_LOG_DEBUG("SRC node:\n");
      VTR_LOG_DEBUG("Node info: %s\n",
                    rr_graph.node_coordinate_to_string(src_node).c_str());
      VTR_LOG_DEBUG("Node ptc: %d\n", rr_graph.node_ptc_num(src_node));
      VTR_LOG_DEBUG("Fan-out nodes:\n");
      for (const auto& temp_edge : rr_graph.edge_range(src_node)) {
        VTR_LOG_DEBUG(
          "\t%s\n",
          rr_graph.node_coordinate_to_string(
            rr_graph.edge_sink_node(temp_edge)).c_str());
      }
      VTR_LOG_DEBUG("\n----------------------------------\n");
      VTR_LOG_DEBUG("IPIN node:\n");
      VTR_LOG_DEBUG("Node info: %s\n",
                    rr_graph.node_coordinate_to_string(ipin_node).c_str());
      VTR_LOG_DEBUG("Node ptc: %d\n", rr_graph.node_ptc_num(ipin_node));
      VTR_LOG_DEBUG("Fan-in nodes:\n");
      for (const auto& temp_edge : in_edges.node_in_edges(ipin_node)) {
        VTR_LOG_DEBUG("\t%s\n",
                      rr_graph.node_coordinate_to_string(
                        rr_graph.edge_src_node(temp_edge)).c_str());
      }
    }
    VTR_ASSERT(UNDEFINED != index);

    from_track_edge_map[chan_side][index] = edge;
    edge_counter++;
  }

  /* Second pass: classify OPIN source edges */
  for (const RREdgeId& edge : in_edges.node_in_edges(ipin_node)) {
    const RRNodeId& src_node = rr_graph.edge_src_node(edge);
    if (e_rr_type::OPIN != rr_graph.node_type(src_node)) {
      continue;
    }
    enum e_side cb_opin_side = NUM_2D_SIDES;
    int cb_opin_index = UNDEFINED;
    gsb.get_node_side_and_index(rr_graph, src_node, IN_PORT, cb_opin_side,
                                cb_opin_index);
    VTR_ASSERT((UNDEFINED != cb_opin_index) && (NUM_2D_SIDES != cb_opin_side));
    if (UNDEFINED == cb_opin_index || NUM_2D_SIDES == cb_opin_side) {
      VTR_LOG_DEBUG("GSB[%lu][%lu]:\n", gsb.get_x(), gsb.get_y());
      VTR_LOG_DEBUG("----------------------------------\n");
      VTR_LOG_DEBUG("SRC node:\n");
      VTR_LOG_DEBUG("Node info: %s\n",
                    rr_graph.node_coordinate_to_string(src_node).c_str());
      VTR_LOG_DEBUG("Node ptc: %d\n", rr_graph.node_ptc_num(src_node));
      VTR_LOG_DEBUG("Fan-out nodes:\n");
      for (const auto& temp_edge : rr_graph.edge_range(src_node)) {
        VTR_LOG_DEBUG(
          "\t%s\n",
          rr_graph.node_coordinate_to_string(
            rr_graph.edge_sink_node(temp_edge)).c_str());
      }
      VTR_LOG_DEBUG("\n----------------------------------\n");
      VTR_LOG_DEBUG("IPIN node:\n");
      VTR_LOG_DEBUG("Node info: %s\n",
                    rr_graph.node_coordinate_to_string(ipin_node).c_str());
      VTR_LOG_DEBUG("Node ptc: %d\n", rr_graph.node_ptc_num(ipin_node));
      VTR_LOG_DEBUG("Fan-in nodes:\n");
      for (const auto& temp_edge : in_edges.node_in_edges(ipin_node)) {
        VTR_LOG_DEBUG("\t%s\n",
                      rr_graph.node_coordinate_to_string(
                        rr_graph.edge_src_node(temp_edge)).c_str());
      }
    }
    from_opin_edge_map[size_t(cb_opin_side)][cb_opin_index] = edge;
    edge_counter++;
  }

  auto& sorted = ipin_node_in_edges_[size_t(ipin_side)][ipin_id];

  /* Store sorted edges: routing tracks first, then OPINs */
  for (size_t iside = 0; iside < NUM_2D_SIDES; ++iside) {
    for (size_t itrack = 0;
         itrack < gsb.get_chan_width(SideManager(iside).get_side()); ++itrack) {
      if (from_track_edge_map[iside].contains(itrack)) {
        sorted.push_back(from_track_edge_map[iside][itrack]);
      }
    }
  }
  for (e_side iside : {TOP, RIGHT, BOTTOM, LEFT}) {
    for (size_t ipin = 0; ipin < gsb.get_num_opin_nodes(iside); ++ipin) {
      if (from_opin_edge_map[size_t(iside)].contains(ipin)) {
        sorted.push_back(from_opin_edge_map[size_t(iside)][ipin]);
      }
    }
  }

  VTR_ASSERT(edge_counter == sorted.size());
}
