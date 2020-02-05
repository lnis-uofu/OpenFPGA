#ifndef RR_GRAPH_INDEXED_DATA_H
#define RR_GRAPH_INDEXED_DATA_H
#include "physical_types.h"
#include "rr_graph_obj.h"

void alloc_and_load_rr_indexed_data(const std::vector<t_segment_inf>& segment_inf,
                                    const RRGraph& rr_graph,
                                    int nodes_per_chan,
                                    int wire_to_ipin_switch,
                                    enum e_base_cost_type base_cost_type);

void load_rr_index_segments(const int num_segment);

#endif
