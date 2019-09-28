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

#ifndef MUX_GRAPH_H
#define MUX_GRAPH_H

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
  public: /* Public Constructors */
    /* Create an object based on a Circuit Model which is MUX */
    MuxGraph(const CircuitLibrary& circuit_lib, 
             const CircuitModelId& circuit_model,
             const size_t& mux_size); 
  private: /* Private Constructors*/
    /* Create an empty graph */
    MuxGraph();
  public: /* Public accessors: Aggregates */
    node_range nodes() const;
    /* Find the non-input nodes */
    std::vector<MuxNodeId> non_input_nodes() const;
    edge_range edges() const;
    mem_range memories() const;
    std::vector<size_t> levels() const;
  public: /* Public accessors: Data query */
    /* Find the number of inputs in the MUX graph */
    size_t num_inputs() const;
    std::vector<MuxNodeId> inputs() const;
    /* Find the number of outputs in the MUX graph */
    size_t num_outputs() const;
    std::vector<MuxNodeId> outputs() const;
    /* Find the edge between two MUX nodes */
    std::vector<MuxEdgeId> find_edges(const MuxNodeId& from_node, const MuxNodeId& to_node) const;
    /* Find the number of levels in the MUX graph */
    size_t num_levels() const;
    size_t num_node_levels() const;
    /* Find the number of SRAMs in the MUX graph */
    size_t num_memory_bits() const;
    /* Find the number of SRAMs at a level in the MUX graph */
    size_t num_memory_bits_at_level(const size_t& level) const;
    /* Find the number of nodes at a given level in the MUX graph */
    size_t num_nodes_at_level(const size_t& level) const;
    /* Find the level of a node */
    size_t node_level(const MuxNodeId& node) const;
    /* Find the index of a node at its level */
    size_t node_index_at_level(const MuxNodeId& node) const;
    /* Find the input edges for a node */
    std::vector<MuxEdgeId> node_in_edges(const MuxNodeId& node) const;
    /* Find the input nodes for a edge */
    std::vector<MuxNodeId> edge_src_nodes(const MuxEdgeId& edge) const;
    /* Find the mem that control the edge */
    MuxMemId find_edge_mem(const MuxEdgeId& edge) const;
    /* Identify if the edge is controlled by the inverted output of a mem */
    bool is_edge_use_inv_mem(const MuxEdgeId& edge) const;
    /* Find the sizes of each branch of a MUX */
    std::vector<size_t> branch_sizes() const;
    /* Generate MUX graphs for its branches */
    MuxGraph subgraph(const MuxNodeId& node) const;
    std::vector<MuxGraph> build_mux_branch_graphs() const; 
    /* Get the node id of a given input */
    MuxNodeId node_id(const MuxInputId& input_id) const;
    /* Get the node id w.r.t. the node level and node_index at the level */
    MuxNodeId node_id(const size_t& node_level, const size_t& node_index_at_level) const;
    /* Get the input id of a given node */
    MuxInputId input_id(const MuxNodeId& node_id) const;
    /* Get the output id of a given node */
    MuxOutputId output_id(const MuxNodeId& node_id) const;
    /* Decode memory bits based on an input id */
    std::vector<size_t> decode_memory_bits(const MuxInputId& input_id) const;
  private: /* Private mutators : basic operations */
     /* Add a unconfigured node to the MuxGraph */
     MuxNodeId add_node(const enum e_mux_graph_node_type& node_type);
     /* Add a edge connecting two nodes  */
     MuxEdgeId add_edge(const MuxNodeId& from_node, const MuxNodeId& to_node);
     /* Add a memory bit to the MuxGraph */
     MuxMemId add_mem();
     /* Configure the level of a memory */
     void set_mem_level(const MuxMemId& mem, const size_t& level);
     /* Link an edge to a mem */
     void set_edge_mem_id(const MuxEdgeId& edge, const MuxMemId& mem);
  private: /* Private mutators : graph builders */
    void build_multilevel_mux_graph(const size_t& mux_size, 
                                    const size_t& num_levels, const size_t& num_inputs_per_branch,
                                    const CircuitModelId& pgl_model) ;
    /* Build the graph for a given one-level multiplexer implementation */
    void build_onelevel_mux_graph(const size_t& mux_size, 
                                  const CircuitModelId& pgl_model) ;
    /* Build the graph for a given multiplexer model */
    void build_mux_graph(const CircuitLibrary& circuit_lib, 
                         const CircuitModelId& circuit_model,
                         const size_t& mux_size); 
    /* Convert some internal node to outputs according to fracturable LUT circuit design specifications */
    void add_fracturable_outputs(const CircuitLibrary& circuit_lib, 
                                 const CircuitModelId& circuit_model);
    /* Build fast node lookup */
    void build_node_lookup();
    /* Build fast mem lookup */
    void build_mem_lookup();
  private: /* Private validators */
    /* valid ids */
    bool valid_node_id(const MuxNodeId& node) const;
    bool valid_edge_id(const MuxEdgeId& edge) const;
    bool valid_mem_id(const MuxMemId& mem) const;
    bool valid_input_id(const MuxInputId& input_id) const;
    bool valid_output_id(const MuxOutputId& output_id) const;
    bool valid_level(const size_t& level) const;
    /* validate/invalidate node lookup */
    bool valid_node_lookup() const;
    void invalidate_node_lookup();
    void invalidate_mem_lookup();
    /* validate graph */
    bool valid_mux_graph() const;
  private: /* Internal data */
    vtr::vector<MuxNodeId, MuxNodeId> node_ids_;                        /* Unique ids for each node */
    vtr::vector<MuxNodeId, enum e_mux_graph_node_type> node_types_;     /* type of each node, input/output/internal */
    vtr::vector<MuxNodeId, MuxInputId> node_input_ids_;                 /* Unique ids for each node as an input of the MUX */
    vtr::vector<MuxNodeId, MuxOutputId> node_output_ids_;                 /* Unique ids for each node as an input of the MUX */
    vtr::vector<MuxNodeId, size_t> node_levels_;                       /* at which level, each node belongs to */
    vtr::vector<MuxNodeId, size_t> node_ids_at_level_;                       /* the index at the level that each node belongs to */
    vtr::vector<MuxNodeId, std::vector<MuxEdgeId>> node_in_edges_;       /* ids of incoming edges to each node */
    vtr::vector<MuxNodeId, std::vector<MuxEdgeId>> node_out_edges_;      /* ids of outgoing edges from each node */

    vtr::vector<MuxEdgeId, MuxEdgeId> edge_ids_;                        /* Unique ids for each edge */
    vtr::vector<MuxEdgeId, std::vector<MuxNodeId>> edge_src_nodes_;     /* source nodes drive this edge */
    vtr::vector<MuxEdgeId, std::vector<MuxNodeId>> edge_sink_nodes_;    /* sink nodes this edge drives */
    vtr::vector<MuxEdgeId, CircuitModelId> edge_models_; /* type of each edge: tgate/pass-gate */
    vtr::vector<MuxEdgeId, MuxMemId> edge_mem_ids_;                   /* ids of memory bit that control the edge */
    vtr::vector<MuxEdgeId, bool> edge_inv_mem_;                       /* if the edge is controlled by an inverted output of a memory bit */

    vtr::vector<MuxMemId, MuxMemId> mem_ids_;                        /* ids of configuration memories */
    vtr::vector<MuxMemId, size_t> mem_levels_;                        /* ids of configuration memories */

    /* fast look-up */
    typedef std::vector<std::vector<std::vector<MuxNodeId>>> NodeLookup;
    mutable NodeLookup node_lookup_; /* [num_levels][num_types][num_nodes_per_level] */ 
    typedef std::vector<std::vector<MuxMemId>> MemLookup;
    mutable MemLookup mem_lookup_; /* [num_levels][num_mems_per_level] */ 
};

#endif
