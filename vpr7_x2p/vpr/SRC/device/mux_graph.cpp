/**************************************************
 * This file includes member functions for the 
 * data structures in mux_graph.h
 *************************************************/
#include <cmath>
#include <map>
#include <algorithm>

#include "util.h"
#include "vtr_assert.h"
#include "mux_utils.h"
#include "mux_graph.h"

/**************************************************
 * Member functions for the class MuxGraph
 *************************************************/

/**************************************************
 * Public Constructors 
 *************************************************/

/* Create an object based on a Circuit Model which is MUX */
MuxGraph::MuxGraph(const CircuitLibrary& circuit_lib, 
                   const CircuitModelId& circuit_model,
                   const size_t& mux_size) {
  /* Build the graph for a given multiplexer model */
  build_mux_graph(circuit_lib, circuit_model, mux_size);
} 

/**************************************************
 * Private Constructors
 *************************************************/
/* Create an empty graph */
MuxGraph::MuxGraph() {
  return;
}

/**************************************************
 * Public Accessors : Aggregates
 *************************************************/
//Accessors
MuxGraph::node_range MuxGraph::nodes() const {
  return vtr::make_range(node_ids_.begin(), node_ids_.end());
}

MuxGraph::edge_range MuxGraph::edges() const {
  return vtr::make_range(edge_ids_.begin(), edge_ids_.end());
}

MuxGraph::mem_range MuxGraph::memories() const {
  return vtr::make_range(mem_ids_.begin(), mem_ids_.end());
}

/**************************************************
 * Public Accessors: Data query
 *************************************************/

/* Find the number of inputs in the MUX graph */
size_t MuxGraph::num_inputs() const {
  /* need to check if the graph is valid or not */
  VTR_ASSERT_SAFE(valid_mux_graph());
  /* Sum up the number of INPUT nodes in each level */
  size_t num_inputs = 0;
  for (auto node_per_level : node_lookup_) {
    num_inputs += node_per_level[MUX_INPUT_NODE].size();
  }
  return num_inputs;
}

/* Find the number of outputs in the MUX graph */
size_t MuxGraph::num_outputs() const {
  /* need to check if the graph is valid or not */
  VTR_ASSERT_SAFE(valid_mux_graph());
  /* Sum up the number of INPUT nodes in each level */
  size_t num_outputs = 0;
  for (auto node_per_level : node_lookup_) {
    num_outputs += node_per_level[MUX_OUTPUT_NODE].size();
  }
  return num_outputs;
}


/* Find the number of levels in the MUX graph */
size_t MuxGraph::num_levels() const {
  /* need to check if the graph is valid or not */
  VTR_ASSERT_SAFE(valid_mux_graph());
  /* The num_levels by definition excludes the level for outputs, so a deduection is applied */
  return node_lookup_.size() - 1; 
}

/* Find the number of configuration memories in the MUX graph */
size_t MuxGraph::num_memory_bits() const {
  /* need to check if the graph is valid or not */
  VTR_ASSERT_SAFE(valid_mux_graph());
  return mem_ids_.size(); 
}

/* Find the sizes of each branch of a MUX */
std::vector<size_t> MuxGraph::branch_sizes() const {
  std::vector<size_t> branch;
  /* Visit each internal nodes/output nodes and find the the number of incoming edges */
  for (auto node : node_ids_ ) {
    /* Bypass input nodes */
    if ( (MUX_OUTPUT_NODE != node_types_[node]) 
      && (MUX_INTERNAL_NODE != node_types_[node]) ) {
      continue;
    }

    size_t branch_size = node_in_edges_[node].size();

    /* make sure the branch size is valid */
    VTR_ASSERT_SAFE(valid_mux_implementation_num_inputs(branch_size));

    /* Nodes with the same number of incoming edges, indicate the same size of branch circuit */
    std::vector<size_t>::iterator it; 
    it = std::find(branch.begin(), branch.end(), branch_size);
    /* if already exists a branch with the same size, skip updating the vector */
    if (it != branch.end()) {
      continue;
    }
    branch.push_back(branch_size);
  }

  /* Sort the branch by size */
  std::sort(branch.begin(), branch.end());

  return branch;
}

/* Build a subgraph from the given node
 * The strategy is very simple, we just 
 * extract a 1-level graph from here
 */
MuxGraph MuxGraph::subgraph(const MuxNodeId& root_node) const {
  /* Validate the node */
  VTR_ASSERT_SAFE(this->valid_node_id(root_node));

  /* Generate an empty graph */
  MuxGraph mux_graph;

  /* A map to record node-to-node mapping from origin graph to subgraph */
  std::map<MuxNodeId, MuxNodeId> node2node_map;

  /* A map to record edge-to-edge mapping from origin graph to subgraph */
  std::map<MuxEdgeId, MuxEdgeId> edge2edge_map;

  /* Add output nodes to subgraph */
  MuxNodeId to_node_subgraph = mux_graph.add_node(MUX_OUTPUT_NODE);
  mux_graph.node_levels_[to_node_subgraph] = 1;
  /* Update the node-to-node map */
  node2node_map[root_node] = to_node_subgraph;

  /* Add input nodes and edges to subgraph */
  size_t input_cnt = 0;
  for (auto edge_origin : this->node_in_edges_[root_node]) {
    VTR_ASSERT_SAFE(1 == edge_src_nodes_[edge_origin].size());
    /* Add nodes */
    MuxNodeId from_node_origin = this->edge_src_nodes_[edge_origin][0];
    MuxNodeId from_node_subgraph = mux_graph.add_node(MUX_INPUT_NODE);
    /* Configure the nodes */
    mux_graph.node_levels_[from_node_subgraph] = 0;
    mux_graph.node_input_ids_[from_node_subgraph] = MuxInputId(input_cnt);
    input_cnt++;
    /* Update the node-to-node map */
    node2node_map[from_node_origin] = from_node_subgraph;

    /* Add edges */
    MuxEdgeId edge_subgraph = mux_graph.add_edge(node2node_map[from_node_origin], node2node_map[root_node]);
    edge2edge_map[edge_origin] = edge_subgraph; 
    /* Configure edges */
    mux_graph.edge_models_[edge_subgraph] = this->edge_models_[edge_origin];
    mux_graph.edge_inv_mem_[edge_subgraph] = this->edge_inv_mem_[edge_origin];
  } 

  /* A map to record mem-to-mem mapping from origin graph to subgraph */
  std::map<MuxMemId, MuxMemId> mem2mem_map;

  /* Add memory bits and configure edges */
  for (auto edge_origin : this->node_in_edges_[root_node]) {
    MuxMemId mem_origin = this->edge_mem_ids_[edge_origin];
    /* Try to find if the mem is already in the list */
    std::map<MuxMemId, MuxMemId>::iterator it = mem2mem_map.find(mem_origin);
    if (it != mem2mem_map.end()) {
      /* Found, we skip mem addition. But make sure we have a valid one */
      VTR_ASSERT_SAFE(MuxMemId::INVALID() != mem2mem_map[mem_origin]);
      /* configure the edge */
      mux_graph.edge_mem_ids_[edge2edge_map[edge_origin]] = mem2mem_map[mem_origin];
      continue;
    }
    /* Not found, we add a memory bit and record in the mem-to-mem map */
    MuxMemId mem_subgraph = mux_graph.add_mem();
    mem2mem_map[mem_origin] = mem_subgraph;
  }

  /* Since the graph is finalized, it is time to build the fast look-up */
  mux_graph.build_node_lookup();

  return mux_graph; 
}

/* Generate MUX graphs for its branches
 * Similar to the branch_sizes() method,
 * we search all the internal nodes and 
 * find out what are the input sizes of 
 * the branches. 
 * Then we extract unique subgraphs and return 
 */
std::vector<MuxGraph> MuxGraph::build_mux_branch_graphs() const {
  std::map<size_t, bool> branch_done; /* A map showing the status of graph generation */

  std::vector<MuxGraph> branch_graphs;

  /* Visit each internal nodes/output nodes and find the the number of incoming edges */
  for (auto node : node_ids_ ) {
    /* Bypass input nodes */
    if ( (MUX_OUTPUT_NODE != node_types_[node]) 
      && (MUX_INTERNAL_NODE != node_types_[node]) ) {
      continue;
    }

    size_t branch_size = node_in_edges_[node].size();

    /* make sure the branch size is valid */
    VTR_ASSERT_SAFE(valid_mux_implementation_num_inputs(branch_size));

    /* check if the branch have been done in sub-graph extraction! */
    std::map<size_t, bool>::iterator it = branch_done.find(branch_size);
    /* if it is done, we can skip */
    if (it != branch_done.end()) {
      VTR_ASSERT(branch_done[branch_size]);
      continue;
    }

    /* Generate a subgraph and push back */
    branch_graphs.push_back(subgraph(node));

    /* Mark it is done for this branch size */
    branch_done[branch_size] = true;
  }

  return branch_graphs;
} 

/* Get the node id of a given input */
MuxNodeId MuxGraph::node_id(const MuxInputId& input_id) const {
  /* Use the node_lookup to accelerate the search */
  for (const auto& lvl : node_lookup_) {
    for (const auto& cand_node : lvl[MUX_INPUT_NODE]) {
      if (input_id == node_input_ids_[cand_node]) {
        return cand_node; 
      }
    }
  } 

  return MuxNodeId::INVALID();
}

/* Decode memory bits based on an input id */
std::vector<size_t> MuxGraph::decode_memory_bits(const MuxInputId& input_id) const {
  /* initialize the memory bits: TODO: support default value */ 
  std::vector<size_t> mem_bits(mem_ids_.size(), 0);

  /* valid the input */
  VTR_ASSERT_SAFE(valid_input_id(input_id));

  /* Route the input to the output and update mem */
  MuxNodeId next_node = node_id(input_id);
  while ( 0 < node_out_edges_[next_node].size() ) {
    VTR_ASSERT_SAFE (1 == node_out_edges_[next_node].size());
    MuxEdgeId edge = node_out_edges_[next_node][0];

    /* Configure the mem bits: 
     * if inv_mem is enabled, it means 0 to enable this edge 
     * otherwise, it is 1 to enable this edge
     */
    MuxMemId mem = edge_mem_ids_[edge];
    VTR_ASSERT_SAFE (valid_mem_id(mem));
    if (true == edge_inv_mem_[edge]) {
      mem_bits[size_t(mem)] = 0;
    } else {
      mem_bits[size_t(mem)] = 1;
    }

    /* each edge must have 1 fan-out */
    VTR_ASSERT_SAFE (1 == edge_sink_nodes_[edge].size());

    /* Visit the next node */
    next_node = edge_sink_nodes_[edge][0]; 
  }
  VTR_ASSERT_SAFE(MUX_OUTPUT_NODE == node_types_[next_node]);

  return mem_bits;
}

/**************************************************
 * Private mutators: basic operations 
 *************************************************/
/* Add a unconfigured node to the MuxGraph */
MuxNodeId MuxGraph::add_node(const enum e_mux_graph_node_type& node_type) {
  MuxNodeId node = MuxNodeId(node_ids_.size());
  /* Push to the node list */
  node_ids_.push_back(node);
  /* Resize the other node-related vectors */
  node_types_.push_back(node_type);
  node_input_ids_.push_back(MuxInputId::INVALID());
  node_levels_.push_back(-1);
  node_in_edges_.emplace_back();
  node_out_edges_.emplace_back();

  return node;
}

/* Add a edge connecting two nodes  */
MuxEdgeId MuxGraph::add_edge(const MuxNodeId& from_node, const MuxNodeId& to_node) {
  MuxEdgeId edge = MuxEdgeId(edge_ids_.size());
  /* Push to the node list */
  edge_ids_.push_back(edge);
  /* Resize the other node-related vectors */
  edge_models_.push_back(CircuitModelId::INVALID());
  edge_mem_ids_.push_back(MuxMemId::INVALID());
  edge_inv_mem_.push_back(false);

  /* update the edge-node connections */
  VTR_ASSERT(valid_node_id(from_node));
  edge_src_nodes_.emplace_back();
  edge_src_nodes_[edge].push_back(from_node);
  node_out_edges_[from_node].push_back(edge);

  VTR_ASSERT(valid_node_id(to_node));
  edge_sink_nodes_.emplace_back();
  edge_sink_nodes_[edge].push_back(to_node);
  node_in_edges_[to_node].push_back(edge);

  return edge;
}

/* Add a memory bit to the MuxGraph */
MuxMemId MuxGraph::add_mem() {
  MuxMemId mem = MuxMemId(mem_ids_.size());
  /* Push to the node list */
  mem_ids_.push_back(mem);
  /* Resize the other node-related vectors */

  return mem;
}

/* Link an edge to a memory bit */
void MuxGraph::set_edge_mem_id(const MuxEdgeId& edge, const MuxMemId& mem) {
  /* Make sure we have valid edge and mem */
  VTR_ASSERT( valid_edge_id(edge) && valid_mem_id(mem) );

  edge_mem_ids_[edge] = mem;
}

/**************************************************
 * Private mutators: graph builders  
 *************************************************/

/* Build a graph for a multi-level multiplexer implementation
 * support both generic multi-level and tree-like multiplexers
 *
 * a N:1 multi-level MUX 
 * ----------------------
 * 
 *  input_node --->+
 *                 |
 *  input_node --->|
 *                 |--->+
 *             ... |    |
 *                 |    |
 *  input_node --->+    |---> ...
 *                      |
 *             ...  --->+        --->+
 *                                   |
 *                  ...          ... |---> output_node
 *                                   |
 *             ...  --->+        --->+
 *                      |
 *  input_node --->+    |---> ...
 *                 |    |
 *  input_node --->|    |
 *                 |--->+
 *             ... |     
 *                 |     
 *  input_node --->+     
 *
 * tree-like multiplexer graph will look like:
 * --------------------------------------------
 *
 *   input_node --->+
 *                  |--->+
 *   input_node --->+    |--->  ...
 *                       |
 *                   --->+         --->+
 *     ...            ...  ...         |----> output_node
 *              ...  --->+         --->+
 *                       |---> ...
 *   input_node --->+    |
 *                  |--->+
 *   input_node --->+    
 *
 */
void MuxGraph::build_multilevel_mux_graph(const size_t& mux_size, 
                                          const size_t& num_levels, const size_t& num_inputs_per_branch,
                                          const CircuitModelId& pgl_model) {
  /* Make sure mux_size for each branch is valid */
  VTR_ASSERT(valid_mux_implementation_num_inputs(num_inputs_per_branch));

  /* In regular cases, there is 1 mem bit for each input of a branch */
  size_t num_mems_per_level = num_inputs_per_branch;
  /* For 2-input branch, only 1 mem bit is needed for each level! */
  if (2 == num_inputs_per_branch) { 
    num_mems_per_level = 1; 
  }
  /* Number of memory bits is definite, add them */
  for (size_t i = 0; i < num_mems_per_level * num_levels; ++i) {
    add_mem();
  }

  /* Create a fast node lookup locally.
   * Only used for building the graph
   * it sorts the nodes by levels and ids at each level
   */
  std::vector<std::vector<MuxNodeId>> node_lookup; /* [num_levels][num_nodes_per_level] */
  node_lookup.resize(num_levels + 1); 

  /* Number of outputs is definite, add and configure */
  MuxNodeId output_node = add_node(MUX_OUTPUT_NODE);
  node_levels_[output_node] = num_levels; 
  /* Update node lookup */
  node_lookup[num_levels].push_back(output_node);

  /* keep a list of node ids which can be candidates for input nodes */
  std::vector<MuxNodeId> input_node_ids;
 
  /* Add internal nodes level by level, 
   * we start from the last level, following a strategy like tree growing
   */
  for (size_t lvl = num_levels - 1; ; --lvl) {
    /* Expand from the existing nodes 
     * Last level should expand from output_node 
     * Other levels will expand from internal nodes!
     */
    for (MuxNodeId seed_node : node_lookup[lvl + 1]) {
      /* Add a new node and connect to seed_node, until we reach the num_inputs_per_branch */
      for (size_t i = 0; i < num_inputs_per_branch; ++i) {
        /* We deposite a type of INTERNAL_NODE, 
         * later it will be configured to INPUT if it is in the input list 
         */
        MuxNodeId expand_node = add_node(MUX_INTERNAL_NODE); 

        /* Node level is deterministic */
        node_levels_[expand_node] = lvl; 
       
        /* Create an edge and connect the two nodes */
        MuxEdgeId edge = add_edge(expand_node, seed_node); 
        /* Configure the edge */
        edge_models_[edge] = pgl_model;

        /* Memory id depends on the level and offset in the current branch 
         * if number of inputs per branch is 2, it indicates a tree-like multiplexer, 
         * every two edges will share one memory bit 
         * otherwise, each edge corresponds to a memory bit
         */
        
        if ( 2 == num_inputs_per_branch) {
          MuxMemId mem_id = MuxMemId(lvl);
          set_edge_mem_id(edge, mem_id);
          /* If this is a second edge in the branch, we will assign it to an inverted edge */
          if (0 != i % num_inputs_per_branch) {
            edge_inv_mem_[edge] = true;
          }
        } else {
          MuxMemId mem_id = MuxMemId( lvl * num_inputs_per_branch + i );
          set_edge_mem_id(edge, mem_id);
        }

        /* Update node lookup */
        node_lookup[lvl].push_back(expand_node);

        /* Push the node to input list, and then remove the seed_node from the list */ 
        input_node_ids.push_back(expand_node);
        /* Remove the node if the seed node is the list */
        std::vector<MuxNodeId>::iterator it = find(input_node_ids.begin(), input_node_ids.end(), seed_node);
        if (it != input_node_ids.end()) {
          input_node_ids.erase(it);
        }

        /* Check the number of input nodes, if already meet the demand, we can finish here */
        if (mux_size != input_node_ids.size()) {
          continue; /* We need more inputs, keep looping */
        }

        /* The graph is done, we configure the input nodes and then we can return */
        /* We must be in level 0 !*/
        VTR_ASSERT( 0 == lvl ) ;
        for (MuxNodeId input_node : input_node_ids) {
          node_types_[input_node] = MUX_INPUT_NODE;
        }

        /* Sort the nodes by the levels and offset */
        size_t input_cnt = 0;
        for (auto lvl_nodes : node_lookup) {
          for (MuxNodeId cand_node : lvl_nodes) {
            if (MUX_INPUT_NODE != node_types_[cand_node]) {
               continue;
            }
            /* Update the input node ids */
            node_input_ids_[cand_node] = MuxInputId(input_cnt);
            /* Update the counter */
            input_cnt++;
          }
        }
        /* Make sure we visited all the inputs in the cache */
        VTR_ASSERT(input_cnt == input_node_ids.size());
        /* Finish building the graph for a multi-level multiplexer */
        return;
      }
    }
  } 
  /* Finish building the graph for a multi-level multiplexer */
} 

/* Build the graph for a given one-level multiplexer implementation 
 * a N:1 one-level MUX
 * 
 *  input_node --->+
 *                 |
 *  input_node --->|
 *                 |--> output_node
 *             ... |
 *                 |
 *  input_node --->+
 */
void MuxGraph::build_onelevel_mux_graph(const size_t& mux_size, 
                                        const CircuitModelId& pgl_model) {
  /* Make sure mux_size is valid */
  VTR_ASSERT(valid_mux_implementation_num_inputs(mux_size));

  /* We definitely know how many nodes we need, 
   * N inputs, 1 output and 0 internal nodes
   */
  MuxNodeId output_node = add_node(MUX_OUTPUT_NODE);
  node_levels_[output_node] = 1; 

  for (size_t i = 0; i < mux_size; ++i) {
    MuxNodeId input_node = add_node(MUX_INPUT_NODE);
    /* All the node belong to level 0 (we have only 1 level) */
    node_input_ids_[input_node] = MuxInputId(i);
    node_levels_[input_node] = 0; 

    /* We definitely know how many edges we need, 
     * the same as mux_size, add a edge connecting two nodes
     */
    MuxEdgeId edge = add_edge(input_node, output_node); 
    /* Configure the edge */
    edge_models_[edge] = pgl_model;

    /* Create a memory bit*/
    MuxMemId mem = add_mem(); 
    /* Link the edge to a memory bit */
    set_edge_mem_id(edge, mem);
  }
  /* Finish building the graph for a one-level multiplexer */
} 

/* Build the graph for a given multiplexer model */
void MuxGraph::build_mux_graph(const CircuitLibrary& circuit_lib, 
                               const CircuitModelId& circuit_model,
                               const size_t& mux_size) {
  /* Make sure this model is a MUX */
  VTR_ASSERT((SPICE_MODEL_MUX == circuit_lib.model_type(circuit_model))
          || (SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model)) );

  /* Make sure mux_size is valid */
  VTR_ASSERT(valid_mux_implementation_num_inputs(mux_size));

  size_t impl_mux_size = find_mux_implementation_num_inputs(circuit_lib, circuit_model, mux_size);

  /* Depends on the mux size, the implemented multiplexer structure may change! */
  enum e_spice_model_structure impl_structure = find_mux_implementation_structure(circuit_lib, circuit_model, impl_mux_size);

  /* Branch on multiplexer structures, leading to different building strategies */
  switch (impl_structure) {
  case SPICE_MODEL_STRUCTURE_TREE: {
    /* Find the number of levels */
    size_t num_levels = find_treelike_mux_num_levels(impl_mux_size);

    /* Find the number of inputs per branch, this is not final */
    size_t num_inputs_per_branch = 2;

    /* Build a multilevel mux graph */
    build_multilevel_mux_graph(impl_mux_size, num_levels, num_inputs_per_branch, circuit_lib.pass_gate_logic_model(circuit_model));
    break;
  }
  case SPICE_MODEL_STRUCTURE_ONELEVEL: {
    build_onelevel_mux_graph(impl_mux_size, circuit_lib.pass_gate_logic_model(circuit_model));
    break;
  }
  case SPICE_MODEL_STRUCTURE_MULTILEVEL: {
    /* Find the number of inputs per branch, this is not final */
    size_t num_inputs_per_branch = find_multilevel_mux_branch_num_inputs(impl_mux_size, circuit_lib.mux_num_levels(circuit_model));

    /* Build a multilevel mux graph */
    build_multilevel_mux_graph(impl_mux_size, circuit_lib.mux_num_levels(circuit_model), 
                               num_inputs_per_branch,
                               circuit_lib.pass_gate_logic_model(circuit_model));
    break;
  }
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid multiplexer structure for circuit model (name=%s)!\n",
              __FILE__, __LINE__, circuit_lib.model_name(circuit_model));
    exit(1);
  }

  /* Since the graph is finalized, it is time to build the fast look-up */
  build_node_lookup();
}

/* Build fast node lookup */
void MuxGraph::build_node_lookup() {
  /* Invalidate the node lookup if necessary */
  invalidate_node_lookup();

  /* Find the maximum number of levels */
  size_t num_levels = 0;
  for (auto node : nodes()) {
    num_levels = std::max((int)node_levels_[node], (int)num_levels);
  }

  /* Resize node_lookup */
  node_lookup_.resize(num_levels + 1);
  for (size_t lvl = 0; lvl < node_lookup_.size(); ++lvl) {
    /* Resize by number of node types */
    node_lookup_[lvl].resize(NUM_MUX_NODE_TYPES);
  }

  /* Fill the node lookup */
  for (auto node : nodes()) {
    node_lookup_[node_levels_[node]][size_t(node_types_[node])].push_back(node);
  }
}

/* Invalidate (empty) the node fast lookup*/
void MuxGraph::invalidate_node_lookup() {
  node_lookup_.clear();
}
 
/**************************************************
 * Private validators
 *************************************************/

/* valid ids */
bool MuxGraph::valid_node_id(const MuxNodeId& node) const {
  return size_t(node) < node_ids_.size() && node_ids_[node] == node;
}

bool MuxGraph::valid_edge_id(const MuxEdgeId& edge) const {
  return size_t(edge) < edge_ids_.size() && edge_ids_[edge] == edge;
}

bool MuxGraph::valid_mem_id(const MuxMemId& mem) const {
  return size_t(mem) < mem_ids_.size() && mem_ids_[mem] == mem;
}

/* validate an input id (from which data path signal will be progagated to the output */
bool MuxGraph::valid_input_id(const MuxInputId& input_id) const {
  for (const auto& lvl : node_lookup_) {
    for (const auto& node : lvl[MUX_INPUT_NODE]) {
      if (size_t(input_id) > size_t(node_input_ids_[node])) {
        return false;
      }
    }
  } 

  return true;
}

bool MuxGraph::valid_node_lookup() const {
  return node_lookup_.empty();
}

/* validate a mux graph and see if it is valid */
bool MuxGraph::valid_mux_graph() const {
  /* A valid MUX graph should be
   * 1. every node has 1 fan-out except output node
   * 2. every input can be routed to the output node 
   */
  for (const auto& node : nodes()) {
    /* output node has 0 fan-out*/
    if (MUX_OUTPUT_NODE == node_types_[node]) {
      continue;
    }
    /* other nodes should have 1 fan-out */
    if (1 != node_out_edges_[node].size()) {
      return false;
    }
  }

  /* Try to route to output */
  for (const auto& node : nodes()) {
    if (MUX_INPUT_NODE == node_types_[node]) {
      MuxNodeId next_node = node;
      while ( 0 < node_out_edges_[next_node].size() ) {
        MuxEdgeId edge = node_out_edges_[next_node][0];
        /* each edge must have 1 fan-out */
        if (1 != edge_sink_nodes_[edge].size()) {
          return false;
        }
        next_node = edge_sink_nodes_[edge][0]; 
      }
      if (MUX_OUTPUT_NODE != node_types_[next_node]) {
        return false;
      }
    }
  } 

  return true;
}

/**************************************************
 * End of Member functions for the class MuxGraph
 *************************************************/
