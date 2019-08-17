/**************************************************
 * This file include a data structure to describe
 * the internal structure of a multiplexer
 * using a generic graph representation  
 *************************************************/

#ifndef MUX_ARCH_H
#define MUX_ARCH_H

#include <vector>
#include "circuit_library.h"

/* Strong Ids for MUXes */
struct mux_id_tag;
typedef vtr::StrongId<mux_id_tag> MuxId;

class MuxGraph {
  private: /* data types used only in this class */
    enum e_mux_graph_node_type {
      MUX_INPUT_NODE,
      MUX_INTERNAL_NODE,
      MUX_OUTPUT_NODE
    };
  private: /* Internal data */
    std::vector<size_t> node_ids_;                        /* Unique ids for each node */
    std::vector<size_t> node_levels_;                     /* at which level, each node belongs to */
    std::vector<enum e_mux_graph_node_type> node_types_;  /* type of each node, input/output/internal */
    std::vector<std::vector<size_t>> node_in_edges;       /* ids of incoming edges to each node */
    std::vector<std::vector<size_t>> node_out_edges;      /* ids of outgoing edges from each node */

    std::vector<size_t> edge_ids_;                        /* Unique ids for each edge */
    std::vector<enum e_spice_model_pass_gate_logic_type> edge_types_; /* type of each edge: tgate/pass-gate */
    std::vector<size_t> edge_sram_ids_;                   /* ids of SRAMs that control the edge */

    std::vector<size_t> sram_ids_;                        /* ids of SRAMs (configuration memories) */

    /* fast look-up */
    typedef std::vector<std::vector<std::vector>>> NodeLookup;
    mutable NodeLookup node_lookup_; /* [num_levels][num_branches][num_nodes_per_branch] */ 
};

class MuxLib {
  private: /* Internal data */
    vtr::vector<MuxId, MuxGraph> mux_graphs_; /* Graphs describing MUX internal structures */
    vtr::vector<MuxId, CircuitModelId> circuit_model_ids_; /* ids in the circuit library, each MUX graph belongs to*/
}

#endif
