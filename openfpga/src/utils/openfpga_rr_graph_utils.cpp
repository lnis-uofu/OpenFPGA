/********************************************************************
 * This file includes most utilized functions for the rr_graph
 * data structure in the OpenFPGA context
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "openfpga_rr_graph_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Get the coordinator of a starting point of a routing track 
 * For routing tracks in INC_DIRECTION
 * (xlow, ylow) should be the starting point 
 *
 * For routing tracks in DEC_DIRECTION
 * (xhigh, yhigh) should be the starting point 
 ***********************************************************************/
vtr::Point<size_t> get_track_rr_node_start_coordinate(const RRGraph& rr_graph,
                                                      const RRNodeId& track_rr_node) {
  /* Make sure we have CHANX or CHANY */
  VTR_ASSERT( (CHANX == rr_graph.node_type(track_rr_node))
           || (CHANY == rr_graph.node_type(track_rr_node)) );
 
  vtr::Point<size_t> start_coordinator;

  if (INC_DIRECTION == rr_graph.node_direction(track_rr_node)) {
    start_coordinator.set(rr_graph.node_xlow(track_rr_node), rr_graph.node_ylow(track_rr_node));
  } else {
    VTR_ASSERT(DEC_DIRECTION == rr_graph.node_direction(track_rr_node));
    start_coordinator.set(rr_graph.node_xhigh(track_rr_node), rr_graph.node_yhigh(track_rr_node));
  }

  return start_coordinator;
}

/************************************************************************
 * Get the coordinator of a end point of a routing track 
 * For routing tracks in INC_DIRECTION
 * (xhigh, yhigh) should be the starting point 
 *
 * For routing tracks in DEC_DIRECTION
 * (xlow, ylow) should be the starting point 
 ***********************************************************************/
vtr::Point<size_t> get_track_rr_node_end_coordinate(const RRGraph& rr_graph,
                                                    const RRNodeId& track_rr_node) {
  /* Make sure we have CHANX or CHANY */
  VTR_ASSERT( (CHANX == rr_graph.node_type(track_rr_node))
           || (CHANY == rr_graph.node_type(track_rr_node)) );
 
  vtr::Point<size_t> end_coordinator;

  if (INC_DIRECTION == rr_graph.node_direction(track_rr_node)) {
    end_coordinator.set(rr_graph.node_xhigh(track_rr_node), rr_graph.node_yhigh(track_rr_node));
  } else {
    VTR_ASSERT(DEC_DIRECTION == rr_graph.node_direction(track_rr_node));
    end_coordinator.set(rr_graph.node_xlow(track_rr_node), rr_graph.node_ylow(track_rr_node));
  }

  return end_coordinator;
}

} /* end namespace openfpga */
