#include "route_util.h"
#include "globals.h"

vtr::Matrix<float> calculate_routing_usage(t_rr_type rr_type) {
    VTR_ASSERT(rr_type == CHANX || rr_type == CHANY);

    auto& device_ctx = g_vpr_ctx.device();
    auto& cluster_ctx = g_vpr_ctx.clustering();
    auto& route_ctx = g_vpr_ctx.routing();

    vtr::Matrix<float> usage({{device_ctx.grid.width(), device_ctx.grid.height()}}, 0.);

    //Collect all the in-use RR nodes
    std::set<RRNodeId> rr_nodes;
    for (auto net : cluster_ctx.clb_nlist.nets()) {
        t_trace* tptr = route_ctx.trace[net].head;
        while (tptr != nullptr) {
            RRNodeId inode = tptr->index;

            if (device_ctx.rr_graph.node_type(inode) == rr_type) {
                rr_nodes.insert(inode);
            }
            tptr = tptr->next;
        }
    }

    //Record number of used resources in each x/y channel
    for (const RRNodeId& inode : rr_nodes) {
        if (rr_type == CHANX) {
            VTR_ASSERT(device_ctx.rr_graph.node_type(inode) == CHANX);
            VTR_ASSERT(device_ctx.rr_graph.node_ylow(inode) == device_ctx.rr_graph.node_yhigh(inode));

            int y = device_ctx.rr_graph.node_ylow(inode);
            for (int x = device_ctx.rr_graph.node_xlow(inode); x <= device_ctx.rr_graph.node_xhigh(inode); ++x) {
                usage[x][y] += route_ctx.rr_node_route_inf[inode].occ();
            }
        } else {
            VTR_ASSERT(rr_type == CHANY);
            VTR_ASSERT(device_ctx.rr_graph.node_type(inode) == CHANY);
            VTR_ASSERT(device_ctx.rr_graph.node_xlow(inode) == device_ctx.rr_graph.node_xhigh(inode));

            int x = device_ctx.rr_graph.node_xlow(inode);
            for (int y = device_ctx.rr_graph.node_ylow(inode); y <= device_ctx.rr_graph.node_yhigh(inode); ++y) {
                usage[x][y] += route_ctx.rr_node_route_inf[inode].occ();
            }
        }
    }
    return usage;
}

vtr::Matrix<float> calculate_routing_avail(t_rr_type rr_type) {
    //Calculate the number of available resources in each x/y channel
    VTR_ASSERT(rr_type == CHANX || rr_type == CHANY);

    auto& device_ctx = g_vpr_ctx.device();

    vtr::Matrix<float> avail({{device_ctx.grid.width(), device_ctx.grid.height()}}, 0.);
    for (const RRNodeId& inode : device_ctx.rr_graph.nodes()) {

        if (device_ctx.rr_graph.node_type(inode) == CHANX && rr_type == CHANX) {
            VTR_ASSERT(device_ctx.rr_graph.node_type(inode) == CHANX);
            VTR_ASSERT(device_ctx.rr_graph.node_ylow(inode) == device_ctx.rr_graph.node_yhigh(inode));

            int y = device_ctx.rr_graph.node_ylow(inode);
            for (int x = device_ctx.rr_graph.node_xlow(inode); x <= device_ctx.rr_graph.node_xhigh(inode); ++x) {
                avail[x][y] += device_ctx.rr_graph.node_capacity(inode);
            }
        } else if (device_ctx.rr_graph.node_type(inode) == CHANY && rr_type == CHANY) {
            VTR_ASSERT(device_ctx.rr_graph.node_type(inode) == CHANY);
            VTR_ASSERT(device_ctx.rr_graph.node_xlow(inode) == device_ctx.rr_graph.node_xhigh(inode));

            int x = device_ctx.rr_graph.node_xlow(inode);
            for (int y = device_ctx.rr_graph.node_ylow(inode); y <= device_ctx.rr_graph.node_yhigh(inode); ++y) {
                avail[x][y] += device_ctx.rr_graph.node_capacity(inode);
            }
        }
    }
    return avail;
}

float routing_util(float used, float avail) {
    if (used > 0.) {
        VTR_ASSERT(avail > 0.);
        return used / avail;
    }
    return 0.;
}
