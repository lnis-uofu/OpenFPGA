#include "vtr_log.h"
#include "vtr_memory.h"

#include "vpr_types.h"
#include "vpr_error.h"

#include "globals.h"
#include "rr_graph.h"
#include "check_rr_graph.h"

/*********************** Subroutines local to this module *******************/

static bool rr_node_is_global_clb_ipin(const RRNodeId& inode);

static void check_unbuffered_edges(const RRNodeId& from_node);

static bool has_adjacent_channel(const RRGraph& rr_graph, const RRNodeId& node, const DeviceGrid& grid);

static void check_rr_edge(const RREdgeId& from_edge, const RRNodeId& to_node);

/************************ Subroutine definitions ****************************/

void check_rr_graph(const t_graph_type graph_type,
                    const DeviceGrid& grid,
                    const std::vector<t_physical_tile_type>& types) {
    e_route_type route_type = DETAILED;
    if (graph_type == GRAPH_GLOBAL) {
        route_type = GLOBAL;
    }

    auto& device_ctx = g_vpr_ctx.device();

    auto total_edges_to_node = vtr::vector<RRNodeId, int>(device_ctx.rr_graph.nodes().size());
    auto switch_types_from_current_to_node = vtr::vector<RRNodeId, unsigned char>(device_ctx.rr_graph.nodes().size());
    const int num_rr_switches = device_ctx.rr_switch_inf.size();

    for (const RRNodeId& inode : device_ctx.rr_graph.nodes()) {

        /* Ignore any uninitialized rr_graph nodes */
        if ((device_ctx.rr_graph.node_type(inode) == SOURCE)
            && (device_ctx.rr_graph.node_xlow(inode) == 0) && (device_ctx.rr_graph.node_ylow(inode) == 0)
            && (device_ctx.rr_graph.node_xhigh(inode) == 0) && (device_ctx.rr_graph.node_yhigh(inode) == 0)) {
            continue;
        }

        t_rr_type rr_type = device_ctx.rr_graph.node_type(inode);

        check_rr_node(inode, route_type, device_ctx);

        /* Check all the connectivity (edges, etc.) information.                    */

        std::map<RRNodeId, std::vector<RREdgeId>> edges_from_current_to_node;
        for (const RREdgeId& iedge : device_ctx.rr_graph.node_out_edges(inode)) {
            RRNodeId to_node = device_ctx.rr_graph.edge_sink_node(iedge);

            if (false == device_ctx.rr_graph.valid_node_id(to_node)) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_graph: node %d has an edge %d.\n"
                                "\tEdge is out of range.\n",
                                size_t(inode), size_t(to_node));
            }

            check_rr_edge(iedge, to_node);

            edges_from_current_to_node[to_node].push_back(iedge);
            total_edges_to_node[to_node]++;

            auto switch_type = size_t(device_ctx.rr_graph.edge_switch(iedge));

            if (switch_type >= (size_t)num_rr_switches) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_graph: node %d has a switch type %d.\n"
                                "\tSwitch type is out of range.\n",
                                size_t(inode), switch_type);
            }
        } /* End for all edges of node. */

        //Check that multiple edges between the same from/to nodes make sense
        for (const RREdgeId& iedge : device_ctx.rr_graph.node_out_edges(inode)) {
            RRNodeId to_node = device_ctx.rr_graph.edge_sink_node(iedge);

            if (edges_from_current_to_node[to_node].size() == 1) continue; //Single edges are always OK

            VTR_ASSERT_MSG(edges_from_current_to_node[to_node].size() > 1, "Expect multiple edges");

            t_rr_type to_rr_type = device_ctx.rr_graph.node_type(to_node);

            //Only expect chan <-> chan connections to have multiple edges
            if ((to_rr_type != CHANX && to_rr_type != CHANY)
                || (rr_type != CHANX && rr_type != CHANY)) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_graph: node %d (%s) connects to node %d (%s) %zu times - multi-connections only expected for CHAN->CHAN.\n",
                          size_t(inode), rr_node_typename[rr_type], size_t(to_node), rr_node_typename[to_rr_type], edges_from_current_to_node[to_node].size());
            }

            //Between two wire segments
            VTR_ASSERT_MSG(to_rr_type == CHANX || to_rr_type == CHANY, "Expect channel type");
            VTR_ASSERT_MSG(rr_type == CHANX || rr_type == CHANY, "Expect channel type");

            //While multiple connections between the same wires can be electrically legal,
            //they are redundant if they are of the same switch type.
            //
            //Identify any such edges with identical switches
            std::map<short, int> switch_counts;
            for (auto edge : edges_from_current_to_node[to_node]) {
                auto edge_switch = size_t(device_ctx.rr_graph.edge_switch(edge));

                switch_counts[edge_switch]++;
            }

            //Tell the user about any redundant edges
            for (auto kv : switch_counts) {
                if (kv.second <= 1) continue;

                auto switch_type = device_ctx.rr_switch_inf[kv.first].type();

                VPR_ERROR(VPR_ERROR_ROUTE, "in check_rr_graph: node %d has %d redundant connections to node %d of switch type %d (%s)",
                          size_t(inode), kv.second, size_t(to_node), kv.first, SWITCH_TYPE_STRINGS[size_t(switch_type)]);
            }
        }

        /* Slow test could leave commented out most of the time. */
        check_unbuffered_edges(inode);

        //Check that all config/non-config edges are appropriately organized
        for (auto edge : device_ctx.rr_graph.node_configurable_out_edges(inode)) {
            if (!device_ctx.rr_graph.edge_is_configurable(edge)) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE, "in check_rr_graph: node %d edge %d is non-configurable, but in configurable edges",
                                size_t(inode), size_t(edge));
            }
        }

        for (auto edge : device_ctx.rr_graph.node_non_configurable_out_edges(inode)) {
            if (device_ctx.rr_graph.edge_is_configurable(edge)) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE, "in check_rr_graph: node %d edge %d is configurable, but in non-configurable edges",
                                size_t(inode), size_t(edge));
            }
        }

    } /* End for all rr_nodes */

    /* I built a list of how many edges went to everything in the code above -- *
     * now I check that everything is reachable.                                */
    bool is_fringe_warning_sent = false;

    for (const RRNodeId& inode : device_ctx.rr_graph.nodes()) {
        t_rr_type rr_type = device_ctx.rr_graph.node_type(inode);

        if (rr_type != SOURCE) {
            if (total_edges_to_node[inode] < 1 && !rr_node_is_global_clb_ipin(inode)) {
                /* A global CLB input pin will not have any edges, and neither will  *
                 * a SOURCE or the start of a carry-chain.  Anything else is an error.
                 * For simplicity, carry-chain input pin are entirely ignored in this test
                 */
                bool is_chain = false;
                if (rr_type == IPIN) {
                    t_physical_tile_type_ptr type = device_ctx.grid[device_ctx.rr_graph.node_xlow(inode)][device_ctx.rr_graph.node_ylow(inode)].type;
                    for (const t_fc_specification& fc_spec : types[type->index].fc_specs) {
                        if (fc_spec.fc_value == 0 && fc_spec.seg_index == 0) {
                            is_chain = true;
                        }
                    }
                }

                bool is_fringe = ((device_ctx.rr_graph.node_xlow(inode) == 1)
                                  || (device_ctx.rr_graph.node_ylow(inode) == 1)
                                  || (device_ctx.rr_graph.node_xhigh(inode) == int(grid.width()) - 2)
                                  || (device_ctx.rr_graph.node_yhigh(inode) == int(grid.height()) - 2));
                bool is_wire = (device_ctx.rr_graph.node_type(inode) == CHANX
                                || device_ctx.rr_graph.node_type(inode) == CHANY);

                if (!is_chain && !is_fringe && !is_wire) {
                    if (device_ctx.rr_graph.node_type(inode) == IPIN || device_ctx.rr_graph.node_type(inode) == OPIN) {
                        if (has_adjacent_channel(device_ctx.rr_graph, inode, device_ctx.grid)) {
                            auto block_type = device_ctx.grid[device_ctx.rr_graph.node_xlow(inode)][device_ctx.rr_graph.node_ylow(inode)].type;
                            std::string pin_name = block_type_pin_index_to_name(block_type, device_ctx.rr_graph.node_pin_num(inode));
                            VTR_LOG_ERROR("in check_rr_graph: node %d (%s) at (%d,%d) block=%s side=%s pin=%s has no fanin.\n",
                                          size_t(inode), rr_node_typename[device_ctx.rr_graph.node_type(inode)], device_ctx.rr_graph.node_xlow(inode), device_ctx.rr_graph.node_ylow(inode), block_type->name, SIDE_STRING[device_ctx.rr_graph.node_side(inode)], pin_name.c_str());
                        }
                    } else {
                        VTR_LOG_ERROR("in check_rr_graph: node %d (%s) has no fanin.\n",
                                      size_t(inode), rr_node_typename[device_ctx.rr_graph.node_type(inode)]);
                    }
                } else if (!is_chain && !is_fringe_warning_sent) {
                    VTR_LOG_WARN(
                        "in check_rr_graph: fringe node %d %s at (%d,%d) has no fanin.\n"
                        "\t This is possible on a fringe node based on low Fc_out, N, and certain lengths.\n",
                        size_t(inode), rr_node_typename[device_ctx.rr_graph.node_type(inode)], device_ctx.rr_graph.node_xlow(inode), device_ctx.rr_graph.node_ylow(inode));
                    is_fringe_warning_sent = true;
                }
            }
        } else { /* SOURCE.  No fanin for now; change if feedthroughs allowed. */
            if (total_edges_to_node[inode] != 0) {
                VTR_LOG_ERROR("in check_rr_graph: SOURCE node %d has a fanin of %d, expected 0.\n",
                              size_t(inode), total_edges_to_node[inode]);
            }
        }
    }
}

static bool rr_node_is_global_clb_ipin(const RRNodeId& inode) {
    /* Returns true if inode refers to a global CLB input pin node.   */

    int ipin;
    t_physical_tile_type_ptr type;

    auto& device_ctx = g_vpr_ctx.device();

    type = device_ctx.grid[device_ctx.rr_graph.node_xlow(inode)][device_ctx.rr_graph.node_ylow(inode)].type;

    if (device_ctx.rr_graph.node_type(inode) != IPIN)
        return (false);

    ipin = device_ctx.rr_graph.node_ptc_num(inode);

    return type->is_ignored_pin[ipin];
}

void check_rr_node(const RRNodeId& inode, enum e_route_type route_type, const DeviceContext& device_ctx) {
    /* This routine checks that the rr_node is inside the grid and has a valid
     * pin number, etc.
     */

    int xlow, ylow, xhigh, yhigh, ptc_num, capacity;
    t_rr_type rr_type;
    t_physical_tile_type_ptr type;
    int nodes_per_chan, tracks_per_node, num_edges, cost_index;
    float C, R;

    rr_type = device_ctx.rr_graph.node_type(inode);
    xlow = device_ctx.rr_graph.node_xlow(inode);
    xhigh = device_ctx.rr_graph.node_xhigh(inode);
    ylow = device_ctx.rr_graph.node_ylow(inode);
    yhigh = device_ctx.rr_graph.node_yhigh(inode);
    ptc_num = device_ctx.rr_graph.node_ptc_num(inode);
    capacity = device_ctx.rr_graph.node_capacity(inode);
    cost_index = device_ctx.rr_graph.node_cost_index(inode);
    type = nullptr;

    const auto& grid = device_ctx.grid;
    if (xlow > xhigh || ylow > yhigh) {
        VPR_ERROR(VPR_ERROR_ROUTE,
                  "in check_rr_node: rr endpoints are (%d,%d) and (%d,%d).\n", xlow, ylow, xhigh, yhigh);
    }

    if (xlow < 0 || xhigh > int(grid.width()) - 1 || ylow < 0 || yhigh > int(grid.height()) - 1) {
        VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                        "in check_rr_node: rr endpoints (%d,%d) and (%d,%d) are out of range.\n", xlow, ylow, xhigh, yhigh);
    }

    if (ptc_num < 0) {
        VPR_ERROR(VPR_ERROR_ROUTE,
                  "in check_rr_node: inode %d (type %d) had a ptc_num of %d.\n", inode, rr_type, ptc_num);
    }

    if (cost_index < 0 || cost_index >= (int)device_ctx.rr_indexed_data.size()) {
        VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                        "in check_rr_node: node %d cost index (%d) is out of range.\n", inode, cost_index);
    }

    /* Check that the segment is within the array and such. */
    type = device_ctx.grid[xlow][ylow].type;

    switch (rr_type) {
        case SOURCE:
        case SINK:
            if (type == nullptr) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: node %d (type %d) is at an illegal clb location (%d, %d).\n", inode, rr_type, xlow, ylow);
            }
            if (xlow != (xhigh - type->width + 1) || ylow != (yhigh - type->height + 1)) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: node %d (type %d) has endpoints (%d,%d) and (%d,%d)\n", inode, rr_type, xlow, ylow, xhigh, yhigh);
            }
            break;
        case IPIN:
        case OPIN:
            if (type == nullptr) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: node %d (type %d) is at an illegal clb location (%d, %d).\n", inode, rr_type, xlow, ylow);
            }
            if (xlow != xhigh || ylow != yhigh) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: node %d (type %d) has endpoints (%d,%d) and (%d,%d)\n", inode, rr_type, xlow, ylow, xhigh, yhigh);
            }
            break;

        case CHANX:
            if (xlow < 1 || xhigh > int(grid.width()) - 2 || yhigh > int(grid.height()) - 2 || yhigh != ylow) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: CHANX out of range for endpoints (%d,%d) and (%d,%d)\n", xlow, ylow, xhigh, yhigh);
            }
            if (route_type == GLOBAL && xlow != xhigh) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: node %d spans multiple channel segments (not allowed for global routing).\n", inode);
            }
            break;

        case CHANY:
            if (xhigh > int(grid.width()) - 2 || ylow < 1 || yhigh > int(grid.height()) - 2 || xlow != xhigh) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "Error in check_rr_node: CHANY out of range for endpoints (%d,%d) and (%d,%d)\n", xlow, ylow, xhigh, yhigh);
            }
            if (route_type == GLOBAL && ylow != yhigh) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: node %d spans multiple channel segments (not allowed for global routing).\n", inode);
            }
            break;

        default:
            VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                            "in check_rr_node: Unexpected segment type: %d\n", rr_type);
    }

    /* Check that it's capacities and such make sense. */

    switch (rr_type) {
        case SOURCE:
            if (ptc_num >= type->num_class
                || type->class_inf[ptc_num].type != DRIVER) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: inode %d (type %d) had a ptc_num of %d.\n", inode, rr_type, ptc_num);
            }
            if (type->class_inf[ptc_num].num_pins != capacity) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: inode %d (type %d) had a capacity of %d.\n", inode, rr_type, capacity);
            }
            break;

        case SINK:
            if (ptc_num >= type->num_class
                || type->class_inf[ptc_num].type != RECEIVER) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: inode %d (type %d) had a ptc_num of %d.\n", inode, rr_type, ptc_num);
            }
            if (type->class_inf[ptc_num].num_pins != capacity) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: inode %d (type %d) has a capacity of %d.\n", inode, rr_type, capacity);
            }
            break;

        case OPIN:
            if (ptc_num >= type->num_pins
                || type->class_inf[type->pin_class[ptc_num]].type != DRIVER) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: inode %d (type %d) had a ptc_num of %d.\n", inode, rr_type, ptc_num);
            }
            if (capacity != 1) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: inode %d (type %d) has a capacity of %d.\n", inode, rr_type, capacity);
            }
            break;

        case IPIN:
            if (ptc_num >= type->num_pins
                || type->class_inf[type->pin_class[ptc_num]].type != RECEIVER) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: inode %d (type %d) had a ptc_num of %d.\n", inode, rr_type, ptc_num);
            }
            if (capacity != 1) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: inode %d (type %d) has a capacity of %d.\n", inode, rr_type, capacity);
            }
            break;

        case CHANX:
            if (route_type == DETAILED) {
                nodes_per_chan = device_ctx.chan_width.max;
                tracks_per_node = 1;
            } else {
                nodes_per_chan = 1;
                tracks_per_node = device_ctx.chan_width.x_list[ylow];
            }

            if (ptc_num >= nodes_per_chan) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: inode %d (type %d) has a ptc_num of %d.\n", inode, rr_type, ptc_num);
            }

            if (capacity != tracks_per_node) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: inode %d (type %d) has a capacity of %d.\n", inode, rr_type, capacity);
            }
            break;

        case CHANY:
            if (route_type == DETAILED) {
                nodes_per_chan = device_ctx.chan_width.max;
                tracks_per_node = 1;
            } else {
                nodes_per_chan = 1;
                tracks_per_node = device_ctx.chan_width.y_list[xlow];
            }

            if (ptc_num >= nodes_per_chan) {
                VPR_ERROR(VPR_ERROR_ROUTE,
                          "in check_rr_node: inode %d (type %d) has a ptc_num of %d.\n", inode, rr_type, ptc_num);
            }

            if (capacity != tracks_per_node) {
                VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                                "in check_rr_node: inode %d (type %d) has a capacity of %d.\n", inode, rr_type, capacity);
            }
            break;

        default:
            VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                            "in check_rr_node: Unexpected segment type: %d\n", rr_type);
    }

    /* Check that the number of (out) edges is reasonable. */
    num_edges = device_ctx.rr_graph.node_out_edges(inode).size();

    if (rr_type != SINK && rr_type != IPIN) {
        if (num_edges <= 0) {
            /* Just a warning, since a very poorly routable rr-graph could have nodes with no edges.  *
             * If such a node was ever used in a final routing (not just in an rr_graph), other       *
             * error checks in check_routing will catch it.                                           */

            //Don't worry about disconnect PINs which have no adjacent channels (i.e. on the device perimeter)
            bool check_for_out_edges = true;
            if (rr_type == IPIN || rr_type == OPIN) {
                if (!has_adjacent_channel(device_ctx.rr_graph, inode, device_ctx.grid)) {
                    check_for_out_edges = false;
                }
            }

            if (check_for_out_edges) {
                std::string info = describe_rr_node(inode);
                VTR_LOG_WARN("in check_rr_node: %s has no out-going edges.\n", info.c_str());
            }
        }
    } else if (rr_type == SINK) { /* SINK -- remove this check if feedthroughs allowed */
        if (num_edges != 0) {
            VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                            "in check_rr_node: node %d is a sink, but has %d edges.\n", size_t(inode), num_edges);
        }
    }

    /* Check that the capacitance and resistance are reasonable. */
    C = device_ctx.rr_graph.node_C(inode);
    R = device_ctx.rr_graph.node_R(inode);

    if (rr_type == CHANX || rr_type == CHANY) {
        if (C < 0. || R < 0.) {
            VPR_ERROR(VPR_ERROR_ROUTE,
                      "in check_rr_node: node %d of type %d has R = %g and C = %g.\n", size_t(inode), rr_type, R, C);
        }
    } else {
        if (C != 0. || R != 0.) {
            VPR_ERROR(VPR_ERROR_ROUTE,
                      "in check_rr_node: node %d of type %d has R = %g and C = %g.\n", size_t(inode), rr_type, R, C);
        }
    }
}

static void check_unbuffered_edges(const RRNodeId& from_node) {
    /* This routine checks that all pass transistors in the routing truly are  *
     * bidirectional.  It may be a slow check, so don't use it all the time.   */

    t_rr_type from_rr_type, to_rr_type;
    short from_switch_type;
    bool trans_matched;

    auto& device_ctx = g_vpr_ctx.device();

    from_rr_type = device_ctx.rr_graph.node_type(from_node);
    if (from_rr_type != CHANX && from_rr_type != CHANY)
        return;

    for (const RREdgeId& from_edge : device_ctx.rr_graph.node_out_edges(from_node)) {
        RRNodeId to_node = device_ctx.rr_graph.edge_sink_node(from_edge);
        to_rr_type = device_ctx.rr_graph.node_type(to_node);

        if (to_rr_type != CHANX && to_rr_type != CHANY)
            continue;

        from_switch_type = size_t(device_ctx.rr_graph.edge_switch(from_edge));

        if (device_ctx.rr_switch_inf[from_switch_type].buffered())
            continue;

        /* We know that we have a pass transistor from from_node to to_node. Now *
         * check that there is a corresponding edge from to_node back to         *
         * from_node.                                                            */

        trans_matched = false;

        for (const RREdgeId& to_edge : device_ctx.rr_graph.node_out_edges(to_node)) {
            if (device_ctx.rr_graph.edge_sink_node(to_edge) == from_node
                && (short)size_t(device_ctx.rr_graph.edge_switch(to_edge)) == from_switch_type) {
                trans_matched = true;
                break;
            }
        }

        if (trans_matched == false) {
            VPR_ERROR(VPR_ERROR_ROUTE,
                      "in check_unbuffered_edges:\n"
                      "connection from node %d to node %d uses an unbuffered switch (switch type %d '%s')\n"
                      "but there is no corresponding unbuffered switch edge in the other direction.\n",
                      size_t(from_node), size_t(to_node), from_switch_type, device_ctx.rr_switch_inf[from_switch_type].name);
        }

    } /* End for all from_node edges */
}

static bool has_adjacent_channel(const RRGraph& rr_graph, const RRNodeId& node, const DeviceGrid& grid) {
    VTR_ASSERT(rr_graph.node_type(node) == IPIN || rr_graph.node_type(node) == OPIN);

    if ((rr_graph.node_xlow(node) == 0 && rr_graph.node_side(node) != RIGHT)                          //left device edge connects only along block's right side
        || (rr_graph.node_ylow(node) == int(grid.height() - 1) && rr_graph.node_side(node) != BOTTOM) //top device edge connects only along block's bottom side
        || (rr_graph.node_xlow(node) == int(grid.width() - 1) && rr_graph.node_side(node) != LEFT)    //right deivce edge connects only along block's left side
        || (rr_graph.node_ylow(node) == 0 && rr_graph.node_side(node) != TOP)                         //bottom deivce edge connects only along block's top side
    ) {
        return false;
    }
    return true; //All other blocks will be surrounded on all sides by channels
}

static void check_rr_edge(const RREdgeId& iedge, const RRNodeId& to_node) {
    auto& device_ctx = g_vpr_ctx.device();

    //Check that to to_node's fan-in is correct, given the switch type
    int iswitch = (int)size_t(device_ctx.rr_graph.edge_switch(iedge));
    auto switch_type = device_ctx.rr_switch_inf[iswitch].type();

    int to_fanin = device_ctx.rr_graph.node_in_edges(to_node).size();
    switch (switch_type) {
        case SwitchType::BUFFER:
            //Buffer switches are non-configurable, and uni-directional -- they must have only one driver
            if (to_fanin != 1) {
                std::string msg = "Non-configurable BUFFER type switch must have only one driver. ";
                msg += vtr::string_fmt(" Actual fan-in was %d (expected 1).\n", to_fanin);
                msg += "  Possible cause is complex block output pins connecting to:\n";
                msg += "    " + describe_rr_node(to_node);

                VPR_FATAL_ERROR(VPR_ERROR_ROUTE, msg.c_str());
            }
        case SwitchType::TRISTATE:  //Fallthrough
        case SwitchType::MUX:       //Fallthrough
        case SwitchType::PASS_GATE: //Fallthrough
        case SwitchType::SHORT:     //Fallthrough
            break;                  //pass
        default:
            VPR_FATAL_ERROR(VPR_ERROR_ROUTE, "Invalid switch type %d", switch_type);
    }
}
