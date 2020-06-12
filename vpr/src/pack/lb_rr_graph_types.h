#ifndef LB_RR_GRPAH_TYPES_H
#define LB_RR_GRPAH_TYPES_H

/* Describes different types of intra-logic cluster_ctx.blocks routing resource nodes */
enum e_lb_rr_type {
    LB_SOURCE = 0,
    LB_SINK,
    LB_INTERMEDIATE,
    NUM_LB_RR_TYPES
};
const std::vector<const char*> lb_rr_type_str{
    "LB_SOURCE", "LB_SINK", "LB_INTERMEDIATE", "INVALID"};

#endif
