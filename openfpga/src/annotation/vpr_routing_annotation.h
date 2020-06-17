#ifndef VPR_ROUTING_ANNOTATION_H
#define VPR_ROUTING_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map> 

/* Header from vpr library */
#include "vpr_context.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This is the critical data structure to link the pb_type in VPR
 * to openfpga annotations
 * With a given pb_type pointer, it aims to identify:
 * 1. if the pb_type is a physical pb_type or a operating pb_type
 * 2. what is the circuit model id linked to a physical pb_type
 * 3. what is the physical pb_type for an operating pb_type
 * 4. what is the mode pointer that represents the physical mode for a pb_type
 *******************************************************************/
class VprRoutingAnnotation {
  public:  /* Constructor */
    VprRoutingAnnotation();
  public:  /* Public accessors */
    ClusterNetId rr_node_net(const RRNodeId& rr_node) const;
    RRNodeId rr_node_prev_node(const RRNodeId& rr_node) const;
  public:  /* Public mutators */
    void init(const RRGraph& rr_graph);
    void set_rr_node_net(const RRNodeId& rr_node,
                         const ClusterNetId& net_id);
    void set_rr_node_prev_node(const RRNodeId& rr_node,
                               const RRNodeId& prev_node);
  private: /* Internal data */
    /* Clustered net ids mapped to each rr_node */
    vtr::vector<RRNodeId, ClusterNetId> rr_node_nets_;

    /* Previous rr_node driving each rr_node */
    vtr::vector<RRNodeId, RRNodeId> rr_node_prev_nodes_;
};

} /* End namespace openfpga*/

#endif
