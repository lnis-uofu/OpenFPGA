#ifndef LB_ROUTER_H
#define LB_ROUTER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>

#include "vtr_vector.h"
#include "vtr_strong_id.h"

#include "physical_types.h"
#include "vpr_context.h"

#include "vpr_device_annotation.h"
#include "lb_rr_graph.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A connection-driven router for programmable logic blocks
 * The router supports routing multiple nets on a LbRRGraph object
 * which models the routing resources in a programmable logic block
 *
 * Note: 
 *   - This router will not build/allocate a LbRRGraph object
 *     Users must do it OUTSIDE this object!!!
 *   - This router supports multiple sources for single net
 *     which is more capable the original VPR lb_router
 *
 * How to use the router:
 *
 *  // Create your own routing resource graph 
 *  LbRRGraph lb_rr_graph = <your_lb_rr_graph_builder>();
 *
 *  // Create a router object
 *  LbRouter lb_router(lb_rr_graph);
 *
 *  // Add nets to be routed 
 *  std::vector<LbRRNodeId> source_nodes = <find_your_source_node_in_your_lb_rr_graph>();
 *  std::vector<LbRRNodeId> sink_nodes = <find_your_sink_node_in_your_lb_rr_graph>();
 *  LbNetId net = lb_router.create_net_to_route(source_nodes, sink_nodes);
 *  // Add more nets
 *
 *  // Initialize the modes to expand routing
 *  // This is a must-do before running the router in the purpose of repacking!!!
 *  lb_router.set_physical_pb_modes(lb_rr_graph, device_annotation); 
 *
 *  // Run the router 
 *  bool route_success = lb_router.try_route(lb_rr_graph, atom_ctx.nlist, verbose);
 *
 *  // Check routing status 
 *  if (true == route_success) {
 *    // Succeed
 *  }
 *
 *  // Read out routing results
 *  // Here is an example to check which nodes are mapped to the 'net' created before
 *  std::vector<LbRRNodeId> routed_nodes = lb_router.net_routed_nodes(net);
 *
 *******************************************************************/


class LbRouter {
  public: /* Strong ids */
    struct net_id_tag;
    typedef vtr::StrongId<net_id_tag> NetId;
  public: /* Types and ranges */
    typedef vtr::vector<NetId, NetId>::const_iterator net_iterator;
    typedef vtr::Range<net_iterator> net_range;
  public: /* Intra-Logic Block Routing Data Structures (by instance) */
    /**************************************************************************
     * Describes the status of a logic cluster_ctx.blocks routing resource node 
     * for a given logic cluster_ctx.blocks instance
     ***************************************************************************/
    struct t_routing_status {
      int occ;  /* Number of nets currently using this lb_rr_node */
      t_mode* mode; /* Mode that this rr_node is set to */
    
      int historical_usage; /* Historical usage of using this node */
    
      t_routing_status() {
        occ = 0;
        mode = nullptr;
        historical_usage = 0;
      }
    };

    /**************************************************************************
     * Data structure forming the route tree of a net within one logic cluster_ctx.blocks.
     * A net is implemented using routing resource nodes. 
     * The t_lb_trace data structure records one of the nodes used by the net and the connections
     * to other nodes
     ***************************************************************************/
    struct t_trace {
      LbRRNodeId current_node;            /* current t_lb_type_rr_node used by net */
      std::vector<t_trace> next_nodes; /* index of previous edge that drives current node */
    };

    /**************************************************************************
     * Stores tuning parameters used by intra-logic cluster_ctx.blocks router 
     ***************************************************************************/
    struct t_option {
      int max_iterations;
      float pres_fac;
      float pres_fac_mult;
      float hist_fac;
    };

    /**************************************************************************
     * Node expanded by router 
     ***************************************************************************/
    struct t_expansion_node {
      LbRRNodeId node_index; /* Index of logic cluster_ctx.blocks rr node this expansion node represents */
      LbRRNodeId prev_index; /* Index of logic cluster_ctx.blocks rr node that drives this expansion node */
      float cost;
    
      t_expansion_node() {
        node_index = LbRRNodeId::INVALID();
        prev_index = LbRRNodeId::INVALID();
        cost = 0;
      }
    };

    class compare_expansion_node {
      public:
        /* Returns true if t1 is earlier than t2 */
        bool operator()(t_expansion_node& e1, t_expansion_node& e2) {
          if (e1.cost > e2.cost) {
            return true;
          }
          return false;
        }
    };

    /**************************************************************************
     * Stores explored nodes by router 
     ***************************************************************************/
    struct t_explored_node_stats {
      LbRRNodeId prev_index;     /* Prevous node that drives this one */
      int explored_id;    /* ID used to determine if this node has been explored */
      NetId inet;           /* net index of route tree */
      int enqueue_id;     /* ID used ot determine if this node has been pushed on exploration priority queue */
      float enqueue_cost; /* cost of node pused on exploration priority queue */
    
      t_explored_node_stats() {
        prev_index = LbRRNodeId::INVALID();
        explored_id = OPEN;
        enqueue_id = OPEN;
        inet = NetId::INVALID();
        enqueue_cost = 0;
      }
    };

    /**************************************************************************
     * Stores status of mode selection during clustering 
     ***************************************************************************/
    struct t_mode_selection_status {
      bool is_mode_conflict = false;
      bool try_expand_all_modes = false;
      bool expand_all_modes = false;
    
      bool is_mode_issue() {
        return is_mode_conflict || try_expand_all_modes;
      }
    };

    // TODO: check if this hacky class memory reserve thing is still necessary, if not, then delete
    /* Packing uses a priority queue that requires a large number of elements.  
     * This backdoor
     * allows me to use a priority queue where I can pre-allocate the # of elements 
     * in the underlying container
     * for efficiency reasons.  Note: Must use vector with this
     */
    template<class T, class U, class V>
    class reservable_pq : public std::priority_queue<T, U, V> {
      public:
        typedef typename std::priority_queue<T>::size_type size_type;
        reservable_pq(size_type capacity = 0) {
            reserve(capacity);
            cur_cap = capacity;
        }
        void reserve(size_type capacity) {
            this->c.reserve(capacity);
            cur_cap = capacity;
        }
        void clear() {
            this->c.clear();
            this->c.reserve(cur_cap);
        }

      private:
        size_type cur_cap;
    };
 
    enum e_commit_remove {
      RT_COMMIT,
      RT_REMOVE
    };

  public :  /* Public constructors */
    LbRouter(const LbRRGraph& lb_rr_graph, t_logical_block_type_ptr lb_type);
  
  public :  /* Public accessors */
    /* Return the ids for all the nets to be routed */ 
    net_range nets() const;

    /* Return the atom net id for a net to be routed */
    AtomNetId net_atom_net_id(const NetId& net) const;    

    /**
     * Find all the routing resource nodes that are over-used, which they are used more than their capacity
     * This function is call to collect the nodes and router can reroute these net
     */   
    std::vector<LbRRNodeId> find_congested_rr_nodes(const LbRRGraph& lb_rr_graph) const;

    /* Show if a valid routing solution has been founded or not */
    bool is_routed() const;

    /**
     * Get the routing results for a Net 
     */
    std::vector<LbRRNodeId> net_routed_nodes(const NetId& net) const;

  public : /* Public mutators */
    /**
     * Add net to be routed
     */ 
    NetId create_net_to_route(const LbRRNodeId& source, const std::vector<LbRRNodeId>& terminals);
    void add_net_atom_net_id(const NetId& net, const AtomNetId& atom_net);
    void add_net_atom_pins(const NetId& net, const AtomPinId& src_pin, const std::vector<AtomPinId>& terminal_pins);

    /* TODO: Initialize all the modes in routing status with the mode set in pb
     * This is function used for general purpose packing
     */

    /* Set all the modes in routing status with the physical mode defined in device annotation
     * This method is used only in repacking for physical logical blocks
     * Do NOT use it during the general purpose packing
     */ 
    void set_physical_pb_modes(const LbRRGraph& lb_rr_graph,
                               const VprDeviceAnnotation& device_annotation);

    /**
     * Perform routing algorithm on a given logical tile routing resource graph
     * Note: the lb_rr_graph must be the same as you initilized the router!!!
     */
    bool try_route(const LbRRGraph& lb_rr_graph,
                   const AtomNetlist& atom_nlist,
                   const int& verbosity);

  private :  /* Private accessors */
    /**
     * Report if the routing is successfully done on a logical block routing resource graph
     */  
    bool is_route_success(const LbRRGraph& lb_rr_graph) const;

    /**
     * Try to find a node in the routing traces recursively
     * If not found, will return an empty pointer
     */
    t_trace* find_node_in_rt(t_trace* rt, const LbRRNodeId& rt_index);

    bool route_has_conflict(const LbRRGraph& lb_rr_graph, t_trace* rt) const;

    /* Recursively find all the nodes in the trace */
    void rec_collect_trace_nodes(const t_trace* trace, std::vector<LbRRNodeId>& routed_nodes) const;

  private : /* Private mutators */
    /*It is possible that a net may connect multiple times to a logically equivalent set of primitive pins.
     *The cluster router will only route one connection for a particular net to the common sink of the
     *equivalent pins.
     *
     *To work around this, we fix all but one of these duplicate connections to route to specific pins,
     *(instead of the common sink). This ensures a legal routing is produced and that the duplicate pins
     *are not 'missing' in the clustered netlist.
     */
    void fix_duplicate_equivalent_pins(const AtomContext& atom_ctx,
                                       const LbRRGraph& lb_rr_graph);
    bool check_edge_for_route_conflicts(std::unordered_map<const t_pb_graph_node*, const t_mode*>& mode_map,
                                        const t_pb_graph_pin* driver_pin,
                                        const t_pb_graph_pin* pin);
    void commit_remove_rt(const LbRRGraph& lb_rr_graph,
                          t_trace* rt,
                          const e_commit_remove& op,
                          std::unordered_map<const t_pb_graph_node*, const t_mode*>& mode_map);
    bool is_skip_route_net(const LbRRGraph& lb_rr_graph, t_trace* rt);
    bool add_to_rt(t_trace* rt, const LbRRNodeId& node_index, const NetId& irt_net);
    void add_source_to_rt(const NetId& inet);
    void expand_rt_rec(t_trace* rt,
                       const LbRRNodeId& prev_index, 
                       const NetId& irt_net,
                       const int& explore_id_index);
    void expand_rt(const NetId& inet,
                   const NetId& irt_net);
    void expand_edges(const LbRRGraph& lb_rr_graph,
                      t_mode* mode,
                      const LbRRNodeId& cur_inode,
                      float cur_cost,
                      int net_fanout);
    void expand_node(const LbRRGraph& lb_rr_graph,
                     const t_expansion_node& exp_node,
                     const int& net_fanout);
    void expand_node_all_modes(const LbRRGraph& lb_rr_graph,
                               const t_expansion_node& exp_node,
                               const int& net_fanout);
    bool try_expand_nodes(const AtomNetlist& atom_nlist,
                          const LbRRGraph& lb_rr_graph,
                          const NetId& lb_net,
                          t_expansion_node& exp_node,
                          const int& itarget,
                          const bool& try_other_modes,
                          const int& verbosity);

    bool try_route_net(const LbRRGraph& lb_rr_graph,
                       const AtomNetlist& atom_nlist,
                       const NetId& net_idx,
                       t_expansion_node& exp_node,
                       std::unordered_map<const t_pb_graph_node*, const t_mode*>& mode_map,
                       const int& verbosity);

  private :  /* Private validators */
    /** 
     * Validate if the rr_graph is the one we used to initialize the router 
     */
    bool matched_lb_rr_graph(const LbRRGraph& lb_rr_graph) const;

    bool valid_net_id(const NetId& net_id) const;

    /* Validate that all the nets have 
     * - valid source, terminal nodes in lb routing resource graph
     * - valid atom net and pin ids in atom netlist
     */
    bool check_net(const LbRRGraph& lb_rr_graph,
                   const AtomNetlist& atom_nlist,
                   const NetId& net) const;

  private :  /* Private initializer and cleaner */
    void reset_explored_node_tb();
    void reset_net_rt();
    void reset_routing_status();
    void reset_illegal_modes();

    void clear_nets();
    void free_net_rt(t_trace* lb_trace);
    void free_lb_trace(t_trace* lb_trace);

  private : /* Stores all data needed by intra-logic cluster_ctx.blocks router */
    /* Logical Netlist Info */
    vtr::vector<NetId, NetId> lb_net_ids_; /* Pointer to vector of intra logic cluster_ctx.blocks nets and their connections */
    vtr::vector<NetId, AtomNetId> lb_net_atom_net_ids_;             /* index of atom net this intra_lb_net represents */
    vtr::vector<NetId, std::vector<AtomPinId>> lb_net_atom_pins_;  /* AtomPin's associated with each terminal */
    vtr::vector<NetId, std::vector<LbRRNodeId>> lb_net_terminals_; /* endpoints of the intra_lb_net, 0th position is the source, all others are sinks */
    vtr::vector<NetId, t_trace*> lb_net_rt_trees_;               /* Route tree head */

    /* Logical-to-physical mapping info */
    vtr::vector<LbRRNodeId, t_routing_status> routing_status_; /* [0..lb_type_graph->size()-1] Stats for each logic cluster_ctx.blocks rr node instance */

    /* Stores state info during Pathfinder iterative routing */
    vtr::vector<LbRRNodeId, t_explored_node_stats> explored_node_tb_; /* [0..lb_type_graph->size()-1] Stores mode exploration and traceback info for nodes */

    int explore_id_index_;                 /* used in conjunction with node_traceback to determine whether or not a location has been explored.  By using a unique identifier every route, I don't have to clear the previous route exploration */

    /* Current type */
    t_logical_block_type_ptr lb_type_;

    /* Parameters used by router */
    t_option params_;

    /* Stores whether or not the current logical-to-physical mapping has a routed solution */
    bool is_routed_; 

    /* Stores the mode selection status when expanding the edges */
    t_mode_selection_status mode_status_;

    /* Stores state info of the priority queue in expanding edges during route */
    reservable_pq<t_expansion_node, std::vector<t_expansion_node>, compare_expansion_node> pq_;

    /* Store the illegal modes for each pb_graph_node that is involved in the routing resource graph */
    std::map<const t_pb_graph_node*, std::vector<const t_mode*>> illegal_modes_;

    /* current congestion factor */
    float pres_con_fac_;
};

} /* end namespace openfpga */

#endif
