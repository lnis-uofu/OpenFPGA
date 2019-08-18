/**************************************************
 * This file includes a data structure to describe
 * the internal structure of a multiplexer
 * using a generic graph representation  
 * A Branch is a N:1 MUX in the part of MUX graph 
 * 
 *  branch_input --->+
 *                   |
 *  branch_input --->|
 *                   |--> branch_out
 *               ... |
 *                   |
 *  branch_input --->+
 *
 *  A short example of how a two-level MUX is organized by branches 
 *
 *               +-----------+    +--------+
 * mux_inputs--->| Branch[0] |--->|        |
 *               +-----------+    |        |
 *                 ...            | Branch |---> mux_out
 *               +-----------+    | [N+1]  |
 * mux_inputs--->| Branch[N] |--->|        |
 *               +-----------+    +--------+
 *
 *************************************************/


#ifndef MUX_ARCH_H
#define MUX_ARCH_H

#include "vtr_vector.h"
#include "vtr_range.h"
#include "mux_graph_fwd.h"
#include "circuit_library.h"

class MuxGraph {
  private: /* data types used only in this class */
    enum e_mux_graph_node_type {
      MUX_INPUT_NODE,
      MUX_INTERNAL_NODE,
      MUX_OUTPUT_NODE,
      NUM_MUX_NODE_TYPES
    };
  public: /* Types and ranges */
    typedef vtr::vector<MuxNodeId, MuxNodeId>::const_iterator node_iterator;
    typedef vtr::vector<MuxEdgeId, MuxEdgeId>::const_iterator edge_iterator;
    typedef vtr::vector<MuxMemId, MuxMemId>::const_iterator mem_iterator;

    typedef vtr::Range<node_iterator> node_range;
    typedef vtr::Range<edge_iterator> edge_range;
    typedef vtr::Range<mem_iterator> mem_range;
  public: /* Constructors */
    /* Create an object based on a Circuit Model which is MUX */
    MuxGraph(const CircuitLibrary& circuit_lib, 
             const CircuitModelId& circuit_model,
             const size_t& mux_size); 
  public: /* Public accessors: Aggregates */
    node_range nodes() const;
    edge_range edges() const;
    mem_range memories() const;
  public: /* Public accessors: Data query */
    /* Find the number of inputs in the MUX graph */
    size_t num_inputs() const;
    /* Find the number of levels in the MUX graph */
    size_t num_levels() const;
    /* Find the number of SRAMs in the MUX graph */
    size_t num_memory_bits() const;
    /* Find the sizes of each branch of a MUX */
    std::vector<size_t> branch_sizes() const;
  private: /* Private mutators : basic operations */
     /* Add a unconfigured node to the MuxGraph */
     MuxNodeId add_node(const enum e_mux_graph_node_type& node_type);
     /* Add a edge connecting two nodes  */
     MuxEdgeId add_edge(const MuxNodeId& from_node, const MuxNodeId& to_node);
     /* Add a memory bit to the MuxGraph */
     MuxMemId add_mem();
     /* Link an edge to a mem */
     void set_edge_mem_id(const MuxEdgeId& edge, const MuxMemId& mem);
  private: /* Private mutators : graph builders */
    void build_multilevel_mux_graph(const size_t& mux_size, 
                                    const size_t& num_levels, const size_t& num_inputs_per_branch,
                                    const enum e_spice_model_pass_gate_logic_type& pgl_type);
    /* Build the graph for a given one-level multiplexer implementation */
    void build_onelevel_mux_graph(const size_t& mux_size, 
                                  const enum e_spice_model_pass_gate_logic_type& pgl_type); 
    /* Build the graph for a given multiplexer model */
    void build_mux_graph(const CircuitLibrary& circuit_lib, 
                         const CircuitModelId& circuit_model,
                         const size_t& mux_size); 
    /* Build fast node lookup */
    void build_node_lookup();
  private: /* Private validators */
    /* valid ids */
    bool valid_node_id(const MuxNodeId& node) const;
    bool valid_edge_id(const MuxEdgeId& edge) const;
    bool valid_mem_id(const MuxMemId& mem) const;
    /* validate/invalidate node lookup */
    bool valid_node_lookup() const;
    void invalidate_node_lookup();
  private: /* Internal data */
    vtr::vector<MuxNodeId, MuxNodeId> node_ids_;                        /* Unique ids for each node */
    vtr::vector<MuxNodeId, enum e_mux_graph_node_type> node_types_;  /* type of each node, input/output/internal */
    vtr::vector<MuxNodeId, size_t> node_input_ids_;                        /* Unique ids for each node as an input of the MUX */
    vtr::vector<MuxNodeId, size_t> node_levels_;                     /* at which level, each node belongs to */
    vtr::vector<MuxNodeId, std::vector<MuxEdgeId>> node_in_edges_;       /* ids of incoming edges to each node */
    vtr::vector<MuxNodeId, std::vector<MuxEdgeId>> node_out_edges_;      /* ids of outgoing edges from each node */

    vtr::vector<MuxEdgeId, MuxEdgeId> edge_ids_;                        /* Unique ids for each edge */
    vtr::vector<MuxEdgeId, enum e_spice_model_pass_gate_logic_type> edge_types_; /* type of each edge: tgate/pass-gate */
    vtr::vector<MuxEdgeId, MuxMemId> edge_mem_ids_;                   /* ids of memory bit that control the edge */
    vtr::vector<MuxEdgeId, bool> edge_inv_mem_;                       /* if the edge is controlled by an inverted output of a memory bit */

    vtr::vector<MuxMemId, MuxMemId> mem_ids_;                        /* ids of configuration memories */

    /* fast look-up */
    typedef std::vector<std::vector<std::vector<MuxNodeId>>> NodeLookup;
    mutable NodeLookup node_lookup_; /* [num_levels][num_types][num_nodes_per_level] */ 
};

class MuxLibrary {
  private: /* Internal data */
    vtr::vector<MuxId, MuxGraph> mux_graphs_; /* Graphs describing MUX internal structures */
    vtr::vector<MuxId, CircuitModelId> circuit_model_ids_; /* ids in the circuit library, each MUX graph belongs to*/
};

#endif
