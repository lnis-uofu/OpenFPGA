#ifndef LB_RR_GRAPH_UTILS_H
#define LB_RR_GRAPH_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "lb_rr_graph.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::string describe_lb_rr_node(const LbRRGraph& lb_rr_graph,
                                const LbRRNodeId& inode);

void print_lb_rr_node(const LbRRGraph& lb_rr_graph,
                      const LbRRNodeId& node);

} /* end namespace openfpga */

#endif
