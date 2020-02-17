#ifndef LB_RR_GRAPH_OBJ_FWD_H
#define LB_RR_GRAPH_OBJ_FWD_H
#include "vtr_strong_id.h"

/***************************************************************
 * This file includes a light declaration for the class LbRRGraph
 * For a detailed description and how to use the class LbRRGraph,
 * please refer to lb_rr_graph_obj.h
 ***************************************************************/

class LbRRGraph;

struct lb_rr_node_id_tag;
struct lb_rr_edge_id_tag;

typedef vtr::StrongId<lb_rr_node_id_tag> LbRRNodeId;
typedef vtr::StrongId<lb_rr_edge_id_tag> LbRREdgeId;

#endif
