#pragma once

/********************************************************************
 * This file defines RRGSBEdges, a companion data structure to RRGSB
 * that stores OpenFPGA-specific cached edge data:
 *
 *   - Sorted incoming edge lists for channel nodes  (chan_node_in_edges_)
 *   - Sorted incoming edge lists for IPIN nodes     (ipin_node_in_edges_)
 *   - OPIN nodes that drive IPINs in CBs            (cb_opin_node_)
 *
 * These were previously private members of the RRGSB class in VTR,
 * but since they are only used by OpenFPGA, they now live here.
 * All access to RRGSB data goes through RRGSB's public API.
 *******************************************************************/

#include <array>
#include <vector>

#include "rr_graph_view.h"
#include "rr_gsb.h"

/* Forward declaration — full definition in rr_graph_in_edges.h */
class RRGraphInEdges;

/********************************************************************
 * RRGSBEdges: stores and provides access to the OpenFPGA-specific
 * cached edge data that accompanies a single RRGSB instance.
 *******************************************************************/
class RRGSBEdges {
 public: /* Accessors */
  /* Get the sorted (or unsorted fallback) incoming edges for a channel
   * output node at the given side and track index.
   * This overload falls back to in_edges when the cache is empty. */
  std::vector<RREdgeId> get_chan_node_in_edges(const RRGSB& gsb,
                                               const RRGraphInEdges& in_edges,
                                               const e_side& side,
                                               const size_t& track_id) const;

  /* Post-sort overload: return cached edge data for a channel output node.
   * Must only be called AFTER sort_chan_node_in_edges has been run. */
  const std::vector<RREdgeId>& get_chan_node_in_edges(const e_side& side,
                                                      const size_t& track_id) const;

  /* Get the sorted (or unsorted fallback) incoming edges for an IPIN
   * node at the given side and ipin index.
   * This overload falls back to in_edges when the cache is empty. */
  std::vector<RREdgeId> get_ipin_node_in_edges(const RRGSB& gsb,
                                               const RRGraphInEdges& in_edges,
                                               const e_side& side,
                                               const size_t& ipin_id) const;

  /* Post-sort overload: return cached edge data for an IPIN node.
   * Must only be called AFTER sort_ipin_node_in_edges has been run. */
  const std::vector<RREdgeId>& get_ipin_node_in_edges(const e_side& side,
                                                      const size_t& ipin_id) const;

  /* Check if the switch block physically exists (has routing wires /
   * OPIN nodes and at least one incoming edge).
   * This overload requires in_edges when chan_node_in_edges_ is not yet built
   * (i.e., before sort_chan_node_in_edges is called). */
  bool is_sb_exist(const RRGSB& gsb, const RRGraphInEdges& in_edges) const;

  /* Check if the switch block physically exists using only the cached
   * chan_node_in_edges_ data.  Must only be called AFTER sort_chan_node_in_edges
   * has been run for this GSB. */
  bool is_sb_exist(const RRGSB& gsb) const;

  /* Return the number of CB OPIN nodes on a given side */
  size_t get_num_cb_opin_nodes(const e_rr_type& cb_type,
                               const e_side& side) const;

  /* Return a specific CB OPIN node */
  RRNodeId get_cb_opin_node(const e_rr_type& cb_type,
                            const e_side& side,
                            const size_t& node_id) const;

 public: /* Mutators */
  /* Sort all incoming edges for every channel output node in the GSB */
  void sort_chan_node_in_edges(const RRGSB& gsb,
                               const RRGraphView& rr_graph,
                               const RRGraphInEdges& in_edges,
                               const bool reorder_incoming_edges = false);

  /* Sort all incoming edges for every IPIN node in the GSB */
  void sort_ipin_node_in_edges(const RRGSB& gsb,
                               const RRGraphView& rr_graph,
                               const RRGraphInEdges& in_edges);

  /* Build the list of OPIN nodes that drive IPINs in connection blocks */
  void build_cb_opin_nodes(const RRGSB& gsb,
                           const RRGraphView& rr_graph,
                           const RRGraphInEdges& in_edges);

 private: /* Private sort helpers (per-node) */
  void sort_chan_node_in_edges(const RRGSB& gsb,
                               const RRGraphView& rr_graph,
                               const RRGraphInEdges& in_edges,
                               const e_side& chan_side,
                               const size_t track_id,
                               const bool reorder_incoming_edges);

  void sort_ipin_node_in_edges(const RRGSB& gsb,
                               const RRGraphView& rr_graph,
                               const RRGraphInEdges& in_edges,
                               const e_side& ipin_side,
                               const size_t& ipin_id);

 private: /* Internal Data (moved from RRGSB) */
  /* Sorted incoming edges for each channel output node.
   * Storage: [chan_side][track_id][edge_index] */
  std::vector<std::vector<std::vector<RREdgeId>>> chan_node_in_edges_;

  /* Sorted incoming edges for each IPIN node.
   * Storage: [ipin_side][ipin_id][edge_index] */
  std::vector<std::vector<std::vector<RREdgeId>>> ipin_node_in_edges_;

  /* OPIN nodes driving IPINs in connection blocks.
   * Index [0] = CBX (CHANX), [1] = CBY (CHANY).
   * Storage: [cb_type_id][side][opin_index] */
  std::array<std::array<std::vector<RRNodeId>, NUM_2D_SIDES>, 2> cb_opin_node_;
};
