#ifndef LB_ROUTER_UTILS_H
#define LB_ROUTER_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "atom_netlist.h"
#include "lb_rr_graph.h"
#include "lb_router.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

LbRouter::NetId add_lb_router_net_to_route(LbRouter& lb_router,
                                           const LbRRGraph& lb_rr_graph,
                                           const LbRRNodeId& source_node,
                                           const std::vector<LbRRNodeId>& sink_nodes,
                                           const AtomContext& atom_ctx,
                                           const AtomNetId& atom_net_id);

} /* end namespace openfpga */

#endif
