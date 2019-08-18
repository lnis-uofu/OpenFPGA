/**************************************************
 * This file includes member functions for the 
 * data structures in mux_graph.h
 *************************************************/
#include <cmath>
#include <algorithm>

#include "util.h"
#include "vtr_assert.h"
#include "mux_utils.h"
#include "mux_graph.h"

/**************************************************
 * Member functions for the class MuxGraph
 *************************************************/

/**************************************************
 * Constructor 
 *************************************************/

/* Create an object based on a Circuit Model which is MUX */
MuxGraph::MuxGraph(const CircuitLibrary& circuit_lib, 
                   const CircuitModelId& circuit_model,
                   const size_t& mux_size) {
  /* Build the graph for a given multiplexer model */
  build_mux_graph(circuit_lib, circuit_model, mux_size);
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
  /* FIXME: need to check if the graph is valid or not */
  /* Sum up the number of INPUT nodes in each level */
  size_t num_inputs = 0;
  for (auto node_per_level : node_lookup_) {
    num_inputs += node_per_level[MUX_INPUT_NODE].size();
  }
  return num_inputs;
}

/* Find the number of levels in the MUX graph */
size_t MuxGraph::num_levels() const {
  /* FIXME: need to check if the graph is valid or not */
  return node_lookup_.size();
}

/* Find the number of configuration memories in the MUX graph */
size_t MuxGraph::num_memory_bits() const {
  /* FIXME: need to check if the graph is valid or not */
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
    if (it == branch.end()) {
      continue;
    }
    branch.push_back(branch_size);
  }

  /* Sort the branch by size */
  std::sort(branch.begin(), branch.end());

  return branch;
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
  node_input_ids_.push_back(-1);
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
  edge_types_.push_back(NUM_CIRCUIT_MODEL_PASS_GATE_TYPES);
  edge_mem_ids_.push_back(MuxMemId::INVALID());
  edge_inv_mem_.push_back(false);

  /* update the node_in_edges and node_out_edges */
  VTR_ASSERT(valid_node_id(from_node));
  node_out_edges_[from_node].push_back(edge);

  VTR_ASSERT(valid_node_id(to_node));
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
                                          const enum e_spice_model_pass_gate_logic_type& pgl_type) {
  /* Number of memory bits is definite, add them */
  for (size_t i = 0; i < num_inputs_per_branch * num_levels; ++i) {
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
        edge_types_[edge] = pgl_type;

        /* Memory id depends on the level and offset in the current branch 
         * if number of inputs per branch is 2, it indicates a tree-like multiplexer, 
         * every two edges will share one memory bit 
         * otherwise, each edge corresponds to a memory bit
         */
        
        if ( 2 == num_inputs_per_branch) {
          MuxMemId mem_id = MuxMemId( (lvl - 1) );
          set_edge_mem_id(edge, mem_id);
          /* If this is a second edge in the branch, we will assign it to an inverted edge */
          if (0 != i % num_inputs_per_branch) {
            edge_inv_mem_[edge] = true;
          }
        } else {
          MuxMemId mem_id = MuxMemId( (lvl - 1) * num_inputs_per_branch + i );
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
            node_input_ids_[cand_node] = input_cnt;
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
                                        const enum e_spice_model_pass_gate_logic_type& pgl_type) {
  /* We definitely know how many nodes we need, 
   * N inputs, 1 output and 0 internal nodes
   */
  MuxNodeId output_node = add_node(MUX_OUTPUT_NODE);
  node_levels_[output_node] = 1; 

  for (size_t i = 0; i < mux_size; ++i) {
    MuxNodeId input_node = add_node(MUX_INPUT_NODE);
    /* All the node belong to level 0 (we have only 1 level) */
    node_input_ids_[input_node] = i;
    node_levels_[input_node] = 0; 

    /* We definitely know how many edges we need, 
     * the same as mux_size, add a edge connecting two nodes
     */
    MuxEdgeId edge = add_edge(input_node, output_node); 
    /* Configure the edge */
    edge_types_[edge] = pgl_type;

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
  VTR_ASSERT(SPICE_MODEL_MUX == circuit_lib.circuit_model_type(circuit_model));

  /* Make sure mux_size is valid */
  VTR_ASSERT(valid_mux_implementation_num_inputs(mux_size));

  size_t impl_mux_size = find_mux_implementation_num_inputs(circuit_lib, circuit_model, mux_size);

  /* Depends on the mux size, the implemented multiplexer structure may change! */
  enum e_spice_model_structure impl_structure = find_mux_implementation_structure(circuit_lib, circuit_model, impl_mux_size);

  /* Branch on multiplexer structures, leading to different building strategies */
  switch (impl_structure) {
  case SPICE_MODEL_STRUCTURE_TREE: {
    /* Find the number of levels */
    size_t num_levels = find_treelike_mux_num_levels(mux_size);

    /* Find the number of inputs per branch, this is not final */
    size_t num_inputs_per_branch = 2;

    /* Build a multilevel mux graph */
    build_multilevel_mux_graph(impl_mux_size, num_levels, num_inputs_per_branch, circuit_lib.pass_gate_logic_type(circuit_model));
    break;
  }
  case SPICE_MODEL_STRUCTURE_ONELEVEL: {
    build_onelevel_mux_graph(impl_mux_size, circuit_lib.pass_gate_logic_type(circuit_model));
    break;
  }
  case SPICE_MODEL_STRUCTURE_MULTILEVEL: {
    /* Find the number of inputs per branch, this is not final */
    size_t num_inputs_per_branch = find_multilevel_mux_branch_num_inputs(mux_size, circuit_lib.mux_num_levels(circuit_model));

    /* Build a multilevel mux graph */
    build_multilevel_mux_graph(impl_mux_size, circuit_lib.mux_num_levels(circuit_model), 
                               num_inputs_per_branch,
                               circuit_lib.pass_gate_logic_type(circuit_model));
    break;
  }
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid multiplexer structure for circuit model (name=%s)!\n",
              __FILE__, __LINE__, circuit_lib.circuit_model_name(circuit_model));
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

bool MuxGraph::valid_node_lookup() const {
  return node_lookup_.empty();
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

/**************************************************
 * End of Member functions for the class MuxGraph
 *************************************************/

/**************************************************
 * Member functions for the class MuxLibrary
 *************************************************/

/**************************************************
 * End of Member functions for the class MuxLibrary
 *************************************************/

