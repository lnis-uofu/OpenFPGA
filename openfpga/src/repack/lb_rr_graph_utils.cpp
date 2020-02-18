/***************************************************************************************
 * This file includes most utilized functions for LbRRGraph object
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"

#include "lb_rr_graph_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Generate a string to describe a node in a logical tile rr_graph
 * in the context of logical tile
 ***************************************************************************************/
std::string describe_lb_rr_node(const LbRRGraph& lb_rr_graph,
                                const LbRRNodeId& inode) {
  std::string description;

  const t_pb_graph_pin* pb_graph_pin = lb_rr_graph.node_pb_graph_pin(inode);

  if (pb_graph_pin) {
    description += "'" + pb_graph_pin->to_string(false) + "'";
  } else if (inode == lb_rr_graph.ext_source_node()) {
    VTR_ASSERT(LB_SOURCE == lb_rr_graph.node_type(inode));
    description = "cluster-external source (LB_SOURCE)";
  } else if (inode == lb_rr_graph.ext_sink_index()) {
    VTR_ASSERT(LB_SINK == lb_rr_graph.node_type(inode));
    description = "cluster-external sink (LB_SINK)";
  } else if (LB_SINK == lb_rr_graph.node_type(inode)) {
    description = "cluster-internal sink (LB_SINK accessible via architecture pins: ";

    //To account for equivalent pins multiple pins may route to a single sink.
    //As a result we need to fin all the nodes which connect to this sink in order
    //to give user-friendly pin names
    std::vector<std::string> pin_descriptions;
    for (const LbRREdgeId edge : lb_rr_graph.node_in_edges(inode)) {
      const LbRRNodeId pin_rr_idx = lb_rr_graph.edge_src_node(edge);
      const t_pb_graph_pin* pin_pb_gpin = lb_rr_graph.node_pb_graph_pin(pin_rr_idx);
      pin_descriptions.push_back(pin_pb_gpin->to_string());
    }

    description += vtr::join(pin_descriptions, ", ");
    description += ")";

  } else if (LB_SOURCE == lb_rr_graph.node_type(inode)) {
    description = "cluster-internal source (LB_SOURCE)";
  } else if (LB_INTERMEDIATE == lb_rr_graph.node_type(inode)) {
    description = "cluster-internal intermediate?";
  } else {
    description = "<unknown lb_type_rr_node>";
  }

  return description;
}

} /* end namespace openfpga */
