/************************************************************************
 * This file introduces a class to model a Routing Resource Graph (RRGraph or RRG) 
 * which is used by packer.
 *
 * Overview
 * ========
 * RRGraph aims to describe in a general way how routing resources are connected
 * inside a pb_graph 
 * 
 * A Routing Resource Graph (RRGraph or RRG) is a directed graph (has many cycles),
 * which consists of a number of nodes and edges.
 *
 * Node
 * ----
 * Each node represents a routing resource, which could be 
 * 1. an intermediate node(INTERMEDIATE_NODE), which are input/output pins of non-primitive and non-root pb_graph_nodes. It represents a pb_graph_pin that exists in a pb_graph 
 * 2. a virtual source (SOURCE), which are inputs of root pb_graph_nodes or outputs of primitive pb_graph_node 
 * 3. a sink node (SINK), which are outputs of root pb_graph_nodes or inputs or primitive pb_graph_node
 *
 * Edge
 * ----
 * Each edge represents a connection between two pb_graph_pins 
 * It represents a pb_graph_edge in a pb_graph 
 *
 * Guidlines on using the LbRRGraph data structure 
 * =============================================
 *
 * For those want to access data from RRGraph
 * ------------------------------------------
 * Some examples for most frequent data query:
 * 
 *     // Strongly suggest to use a read-only lb_rr_graph object
 *     const LbRRGraph& lb_rr_graph;
 *
 *     // Access type of a node with a given node id
 *     // Get the unique node id that you may get 
 *     // from other data structures or functions 
 *     LbRRNodeId node_id;                                    
 *     e_lb_rr_type node_type = lb_rr_graph.node_type(node_id);
 *
 *     // Access all the fan-out edges from a given node  
 *     for (const RREdgeId& out_edge_id : rr_graph.node_out_edges(node_id)) {
 *       // Do something with out_edge
 *     }
 *     // If you only want to learn the number of fan-out edges
 *     size_t num_out_edges = rr_graph.node_fan_out(node_id);
 *
 * Please refer to the detailed comments on each public accessors
 *
 * For those want to build/modify a LbRRGraph
 * -----------------------------------------
 * Do NOT add a builder to this data structure!
 * Builders should be kept as free functions that use the public mutators
 * We suggest developers to create builders in separated C/C++ source files
 * outside the rr_graph header and source files
 *
 * After build/modify a RRGraph, please do run a fundamental check, a public accessor.
 * to ensure that your RRGraph does not include invalid nodes/edges/switches/segements 
 * as well as connections.
 * The validate() function gurantees the consistency between internal data structures, 
 * such as the id cross-reference between nodes and edges etc.,
 * so failing it indicates a fatal bug!
 * This is a must-do check! 
 *
 * Example: 
 *    RRGraph lb_rr_graph;
 *    ... // Building RRGraph
 *    lb_rr_graph.validate(); 
 *
 * Optionally, we strongly recommend developers to run an advance check in check_rr_graph()  
 * This guarantees legal and routable RRGraph for VPR routers.
 *
 * This checks for connectivity or other information in the RRGraph that is unexpected 
 * or unusual in an FPGA, and likely indicates a problem in your graph generation. 
 * However, if you are intentionally creating an RRGraph with this unusual, 
 * buts still technically legal, behaviour, you can write your own check_rr_graph() with weaker assumptions. 
 *
 * Note: Do NOT modify the coordinate system for nodes, they are designed for downstream drawers and routers
 *
 * For those want to extend RRGraph data structure
 * --------------------------------------------------------------------------
 * Please avoid modifying any existing public/private accessors/mutators
 * in order to keep a stable RRGraph object in the framework
 * Developers may add more internal data to RRGraph as well as associate accessors/mutators
 * Please update and comment on the added features properly to keep this data structure friendly to be extended.
 *
 * Try to keep your extension within only graph-related internal data to RRGraph.
 * In other words, extension is necessary when the new node/edge attributes are needed.
 * RRGraph should NOT include other data which are shared by other data structures outside.
 * The rr-graph is the single largest data structure in VPR,
 * so avoid adding unnecessary information per node or per edge to it, as it will impact memory footprint.
 * Instead, using indices to point to the outside data source instead of embedding to RRGraph
 * For example: 
 *   For any placement/routing cost related information, try to extend t_rr_indexed_data, but not RRGraph
 *   For any placement/routing results, try to extend PlaceContext and RoutingContext, but not RRGraph
 * 
 * For those want to develop placers or routers
 * --------------------------------------------------------------------------
 * The RRGraph is designed to be a read-only database/graph, once created.
 * Placement and routing should NOT change any attributes of RRGraph.
 * Any placement and routing results should be stored in other data structures, such as PlaceContext and RoutingContext. 
 *
 * Tracing Cross-Reference 
 * =======================
 * RRGraph is designed to a self-contained data structure as much as possible.
 * It includes the switch information (rr_switch) and segment_information (rr_segment)
 * which are necessary to build-up any external data structures.
 *
 * Internal cross-reference
 * ------------------------
 *
 *  +--------+                  +--------+
 *  |        |  node_in_edges   |        |
 *  |        |  node_out_edges  |        |
 *  |  Node  |----------------->|  Edge  |
 *  |        |<-----------------|        | 
 *  |        |  edge_src_node   |        | 
 *  +--------+  edge_sink_node  +--------+ 
 *
 *
 * External cross-reference
 * ------------------------
 * The only cross-reference to outside data structures is the cost_index
 * corresponding to the data structure t_rr_index_data
 * Details can be found in the definition of t_rr_index_data
 * This allows rapid look up by the router of additional information it needs for this node, using a flyweight pattern.
 *
 * +---------+  pb_graph_pin   +----------------+
 * | RRGraph |---------------->| Pb_graph_node  | 
 * +---------+  pb_graph_edge  +----------------+
 *
 ***********************************************************************/
#ifndef LB_RR_GRAPH_OBJ_H
#define LB_RR_GRAPH_OBJ_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <limits>
#include <vector>

/* Header from vtrutil library */
#include "vtr_range.h"
#include "vtr_vector.h"

/* Header from readarch library */
#include "physical_types.h"

/* Header from vpr library */
#include "lb_rr_graph_types.h"

#include "lb_rr_graph_fwd.h"

/* begin namespace openfpga */
namespace openfpga {

class LbRRGraph {
  public: /* Types */
    /* Iterators used to create iterator-based loop for nodes/edges/switches/segments */
    typedef vtr::vector<LbRRNodeId, LbRRNodeId>::const_iterator node_iterator;
    typedef vtr::vector<LbRREdgeId, LbRREdgeId>::const_iterator edge_iterator;

    /* Ranges used to create range-based loop for nodes/edges/switches/segments */
    typedef vtr::Range<node_iterator> node_range;
    typedef vtr::Range<edge_iterator> edge_range;

  public: /* Constructors */
    LbRRGraph();

  public: /* Accessors */
    /* Aggregates: create range-based loops for nodes/edges/switches/segments
     * To iterate over the nodes/edges/switches/segments in a RRGraph, 
     *    using a range-based loop is suggested.
     *  -----------------------------------------------------------------
     *    Example: iterate over all the nodes
     *      // Strongly suggest to use a read-only rr_graph object
     *      const LbRRGraph& lb_rr_graph;
     *      for (const LbRRNodeId& node : lb_rr_graph.nodes()) {
     *        // Do something with each node
     *      }
     *
     *      for (const LbRREdgeId& edge : lb_rr_graph.edges()) {
     *        // Do something with each edge
     *      }
     *
     */
    node_range nodes() const;
    edge_range edges() const;

    /* Node-level attributes */
    e_lb_rr_type node_type(const LbRRNodeId& node) const;
    short node_capacity(const LbRRNodeId& node) const;
    t_pb_graph_pin* node_pb_graph_pin(const LbRRNodeId& node) const;
    float node_intrinsic_cost(const LbRRNodeId& node) const;

    /* Get a list of edge ids, which are incoming edges to a node */
    std::vector<LbRREdgeId> node_in_edges(const LbRRNodeId& node) const;
    std::vector<LbRREdgeId> node_in_edges(const LbRRNodeId& node, t_mode* mode) const;

    /* Get a list of edge ids, which are outgoing edges from a node */
    std::vector<LbRREdgeId> node_out_edges(const LbRRNodeId& node) const;
    std::vector<LbRREdgeId> node_out_edges(const LbRRNodeId& node, t_mode* mode) const;

    /* General method to look up a node with type and only pb_graph_pin information */
    LbRRNodeId find_node(const e_lb_rr_type& type, const t_pb_graph_pin* pb_graph_pin) const;
    /* Method to find special node */
    LbRRNodeId ext_source_node() const;
    LbRRNodeId ext_sink_node() const;

    /* General method to look up a edge with source and sink nodes */
    std::vector<LbRREdgeId> find_edge(const LbRRNodeId& src_node, const LbRRNodeId& sink_node) const;
    /* Get the source node which drives a edge */
    LbRRNodeId edge_src_node(const LbRREdgeId& edge) const;
    /* Get the sink node which a edge ends to */
    LbRRNodeId edge_sink_node(const LbRREdgeId& edge) const;

    float edge_intrinsic_cost(const LbRREdgeId& edge) const;
    t_mode* edge_mode(const LbRREdgeId& edge) const;

  public: /* Mutators */
    /* Reserve the lists of nodes, edges, switches etc. to be memory efficient. 
     * This function is mainly used to reserve memory space inside RRGraph,
     * when adding a large number of nodes/edge/switches/segments,
     * in order to avoid memory fragements
     * For example: 
     *    LbRRGraph lb_rr_graph;
     *    // Add 1000 CHANX nodes to the LbRRGraph object
     *    rr_graph.reserve_nodes(1000);
     *    for (size_t i = 0; i < 1000; ++i) {
     *      rr_graph.create_node(CHANX);
     *    }
     */
    void reserve_nodes(const unsigned long& num_nodes);
    void reserve_edges(const unsigned long& num_edges);

    /* Add new elements (node, edge, switch, etc.) to RRGraph */
    /* Add a node to the RRGraph with a deposited type 
     * Detailed node-level information should be added using the set_node_* functions
     * For example: 
     *   RRNodeId node = create_node();
     *   set_node_xlow(node, 0);
     */
    LbRRNodeId create_node(const e_lb_rr_type& type);
  
    /* Create special nodes */
    LbRRNodeId create_ext_source_node(const e_lb_rr_type& type);
    LbRRNodeId create_ext_sink_node(const e_lb_rr_type& type);

    /* Set node-level information */
    void set_node_type(const LbRRNodeId& node, const e_lb_rr_type& type);

    void set_node_capacity(const LbRRNodeId& node, const short& capacity);

    void set_node_pb_graph_pin(const LbRRNodeId& node, t_pb_graph_pin* pb_graph_pin);

    void set_node_intrinsic_cost(const LbRRNodeId& node, const float& cost);

    /* Add a edge to the RRGraph, by providing the source and sink node 
     * This function will automatically create a node and
     * configure the nodes and edges in connection   
     */
    LbRREdgeId create_edge(const LbRRNodeId& source, const LbRRNodeId& sink, t_mode* mode);
    void set_edge_intrinsic_cost(const LbRREdgeId& edge, const float& cost);

  public: /* Public validators */
    /* Validate is the node id does exist in the RRGraph */
    bool valid_node_id(const LbRRNodeId& node) const;

    /* Validate is the edge id does exist in the RRGraph */
    bool valid_edge_id(const LbRREdgeId& edge) const;

    bool validate() const;

    bool empty() const;

  private: /* Private Validators */
    bool validate_node_sizes() const;
    bool validate_edge_sizes() const;
    bool validate_sizes() const;
    bool validate_node_is_edge_src(const LbRRNodeId& node, const LbRREdgeId& edge) const;
    bool validate_node_is_edge_sink(const LbRRNodeId& node, const LbRREdgeId& edge) const;
    bool validate_node_in_edges(const LbRRNodeId& node) const;
    bool validate_node_out_edges(const LbRRNodeId& node) const;
    bool validate_nodes_in_edges() const;
    bool validate_nodes_out_edges() const;
    bool validate_nodes_edges() const;

  private: /* Internal Data */
    /* Node related data */
    vtr::vector<LbRRNodeId, LbRRNodeId> node_ids_;

    vtr::vector<LbRRNodeId, e_lb_rr_type> node_types_;

    vtr::vector<LbRRNodeId, short> node_capacities_;

    vtr::vector<LbRRNodeId, t_pb_graph_pin*> node_pb_graph_pins_;

    vtr::vector<LbRRNodeId, float> node_intrinsic_costs_;

    /* Edges per node is sorted by modes: [<mode_id>][<in_edges...><out_edges>] */
    vtr::vector<LbRRNodeId, std::vector<LbRREdgeId>> node_in_edges_;
    vtr::vector<LbRRNodeId, std::vector<LbRREdgeId>> node_out_edges_;

    /* Edge related data */
    /* Range of edge ids, use the unsigned long as 
     * the number of edges could be >10 times larger than the number of nodes! 
     */
    vtr::vector<LbRREdgeId, LbRREdgeId> edge_ids_;
    vtr::vector<LbRREdgeId, LbRRNodeId> edge_src_nodes_;
    vtr::vector<LbRREdgeId, LbRRNodeId> edge_sink_nodes_;
    vtr::vector<LbRREdgeId, float> edge_intrinsic_costs_;
    vtr::vector<LbRREdgeId, t_mode*> edge_modes_;

    /* Fast look-up to search a node by its type, coordinator and ptc_num 
     * Indexing of fast look-up: [0..NUM_TYPES-1][t_pb_graph_pin*] 
     */
    typedef std::vector<std::map<const t_pb_graph_pin*, LbRRNodeId>> NodeLookup;
    mutable NodeLookup node_lookup_;

    /* Special node look-up */
    LbRRNodeId ext_source_node_;
    LbRRNodeId ext_sink_node_;
};

} /* end namespace openfpga */

#endif
