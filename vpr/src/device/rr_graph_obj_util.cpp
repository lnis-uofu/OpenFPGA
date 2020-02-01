/****************************************************************************
 * This file include most-utilized functions that manipulate on the
 * RRGraph object
 ***************************************************************************/
#include "rr_graph_obj.h"
#include "rr_graph_obj_util.h"

/****************************************************************************
 * Find the switches interconnecting two nodes
 * Return a vector of switch ids
 ***************************************************************************/
std::vector<RRSwitchId> find_rr_graph_switches(const RRGraph& rr_graph,
                                               const RRNodeId& from_node,
                                               const RRNodeId& to_node) {
    std::vector<RRSwitchId> switches;
    std::vector<RREdgeId> edges = rr_graph.find_edges(from_node, to_node);
    if (true == edges.empty()) {
        /* edge is open, we return an empty vector of switches */
        return switches;
    }

    /* Reach here, edge list is not empty, find switch id one by one
     * and update the switch list
     */
    for (auto edge : edges) {
        switches.push_back(rr_graph.edge_switch(edge));
    }

    return switches;
}

/*********************************************************************
 * Like the RRGraph.find_node() but returns all matching nodes,
 * rather than just the first. This is particularly useful for getting all instances
 * of a specific IPIN/OPIN at a specific gird tile (x,y) location.
 **********************************************************************/
std::vector<RRNodeId> find_rr_graph_nodes(const RRGraph& rr_graph,
                                          const int& x,
                                          const int& y,
                                          const t_rr_type& rr_type,
                                          const int& ptc) {
    std::vector<RRNodeId> indices;

    if (rr_type == IPIN || rr_type == OPIN) {
        //For pins we need to look at all the sides of the current grid tile

        for (e_side side : SIDES) {
            RRNodeId rr_node_index = rr_graph.find_node(x, y, rr_type, ptc, side);

            if (rr_node_index != RRNodeId::INVALID()) {
                indices.push_back(rr_node_index);
            }
        }
    } else {
        //Sides do not effect non-pins so there should only be one per ptc
        RRNodeId rr_node_index = rr_graph.find_node(x, y, rr_type, ptc);

        if (rr_node_index != RRNodeId::INVALID()) {
            indices.push_back(rr_node_index);
        }
    }

    return indices;
}

