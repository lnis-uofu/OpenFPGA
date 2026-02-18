#ifndef RR_GSB_UTILS_H
#define RR_GSB_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>

#include "rr_graph_view.h"
#include "rr_gsb.h"
#include "rr_gsb_edges.h"
#include "vpr_device_annotation.h"

/* Forward declaration */
class RRGraphInEdges;

/********************************************************************
 * Free utility functions that operate on RRGSB objects but are
 * specific to OpenFPGA and not part of the RRGSB class itself.
 *
 * OpenFPGA-specific edge-data accessors/mutators live in
 * RRGSBEdges (rr_gsb_edges.h).
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

bool connection_block_contain_only_routing_tracks(const RRGSB& rr_gsb,
                                                  const e_rr_type& cb_type);

std::vector<RRNodeId> get_rr_gsb_chan_node_configurable_driver_nodes(
  const RRGraphView& rr_graph, const RRGraphInEdges& in_edges,
  const RRGSB& rr_gsb, const RRGSBEdges& gsb_edges, const e_side& chan_side,
  const size_t& track_id);

/* Post-sort overload: uses cached edge data (no in_edges needed).
 * Must only be called AFTER sort_chan_node_in_edges has been run. */
std::vector<RRNodeId> get_rr_gsb_chan_node_configurable_driver_nodes(
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const RRGSBEdges& gsb_edges,
  const e_side& chan_side, const size_t& track_id);

bool is_sb_mirror(const RRGraphView& rr_graph, const RRGraphInEdges& in_edges,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSBEdges& base_edges,
                  const RRGSB& cand, const RRGSBEdges& cand_edges);

bool is_cb_mirror(const RRGraphView& rr_graph, const RRGraphInEdges& in_edges,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSBEdges& base_edges,
                  const RRGSB& cand, const RRGSBEdges& cand_edges,
                  const e_rr_type& cb_type);

} /* end namespace openfpga */

#endif
