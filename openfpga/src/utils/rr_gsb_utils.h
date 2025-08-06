#ifndef RR_GSB_UTILS_H
#define RR_GSB_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "rr_graph_view.h"
#include "rr_gsb.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

bool connection_block_contain_only_routing_tracks(const RRGSB& rr_gsb,
                                                  const e_rr_type& cb_type);

std::vector<RRNodeId> get_rr_gsb_chan_node_configurable_driver_nodes(
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_side& chan_side,
  const size_t& track_id);

bool is_sb_mirror(const RRGraphView& rr_graph,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSB& cand);

bool is_cb_mirror(const RRGraphView& rr_graph,
                  const VprDeviceAnnotation& device_annotation,
                  const RRGSB& base, const RRGSB& cand,
                  const e_rr_type& cb_type);

} /* end namespace openfpga */

#endif
