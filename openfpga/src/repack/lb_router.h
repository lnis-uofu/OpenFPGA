#ifndef LB_ROUTER_H
#define LB_ROUTER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <map>
#include <unordered_map>
#include <vector>

#include "vtr_vector.h"

#include "arch_types.h"
#include "vpr_types.h"
#include "atom_netlist_fwd.h"

#include "lb_rr_graph.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

class LbRouter {
  public: /* Intra-Logic Block Routing Data Structures (by instance) */
    /**************************************************************************
     * A routing traceback data structure, provides a logic cluster_ctx.blocks 
     * instance specific trace lookup directly from the t_lb_type_rr_node array index
     * After packing, routing info for each CLB will have an array of t_lb_traceback 
     * to store routing info within the CLB
     ***************************************************************************/
    struct t_traceback {
      int net;             /* net of flat, technology-mapped, netlist using this node */
      LbRRNodeId prev_lb_rr_node; /* index of previous node that drives current node */
      LbRREdgeId prev_edge;       /* index of previous edge that drives current node */
    };

    /**************************************************************************
     * Describes the status of a logic cluster_ctx.blocks routing resource node 
     * for a given logic cluster_ctx.blocks instance
     ***************************************************************************/
    struct t_routing_stats {
      int occ;  /* Number of nets currently using this lb_rr_node */
      t_mode* mode; /* Mode that this rr_node is set to */
    
      int historical_usage; /* Historical usage of using this node */
    
      t_lb_rr_graph_stats() {
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
      std::vector<t_lb_trace> next_nodes; /* index of previous edge that drives current node */
    };

    /**************************************************************************
     * Represents a net used inside a logic cluster_ctx.blocks and the 
     * physical nodes used by the net
     ***************************************************************************/
    struct t_net {
      AtomNetId atom_net_id;             /* index of atom net this intra_lb_net represents */
      std::vector<LbRRNodeId> terminals; /* endpoints of the intra_lb_net, 0th position is the source, all others are sinks */
      std::vector<AtomPinId> atom_pins;  /* AtomPin's associated with each terminal */
      std::vector<bool> fixed_terminals; /* Marks a terminal as having a fixed target (i.e. a pin not a sink) */
      t_lb_trace* rt_tree;               /* Route tree head */
    
      t_lb_rr_net() {
          atom_net_id = AtomNetId::INVALID();
          rt_tree = nullptr;
      }
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
      int inet;           /* net index of route tree */
      int enqueue_id;     /* ID used ot determine if this node has been pushed on exploration priority queue */
      float enqueue_cost; /* cost of node pused on exploration priority queue */
    
      t_explored_node_stats() {
        prev_index = LbRRNodeId::INVALID();
        explored_id = OPEN;
        enqueue_id = OPEN;
        inet = OPEN;
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

  private : /* Stores all data needed by intra-logic cluster_ctx.blocks router */
    /* Logical Netlist Info */
    std::vector<t_net> intra_lb_nets_; /* Pointer to vector of intra logic cluster_ctx.blocks nets and their connections */

    /* Saved nets */
    std::vector<t_net> saved_lb_nets_; /* Save vector of intra logic cluster_ctx.blocks nets and their connections */

    std::map<AtomBlockId, bool> atoms_added_; /* map that records which atoms are added to cluster router */

    /* Logical-to-physical mapping info */
    vtr::vector<LbRRNodeId, t_routing_stats> lb_rr_node_stats_; /* [0..lb_type_graph->size()-1] Stats for each logic cluster_ctx.blocks rr node instance */

    /* Stores state info during Pathfinder iterative routing */
    vtr::vector<LbRRNodeId, t_explored_node_stats> explored_node_tb_; /* [0..lb_type_graph->size()-1] Stores mode exploration and traceback info for nodes */
    int explore_id_index_;                 /* used in conjunction with node_traceback to determine whether or not a location has been explored.  By using a unique identifier every route, I don't have to clear the previous route exploration */

    /* Current type */
    t_logical_block_type_ptr lb_type_;

    /* Parameters used by router */
    t_option options_;

    bool is_routed_;                       /* Stores whether or not the current logical-to-physical mapping has a routed solution */

    /* current congestion factor */
    float pres_con_fac_;
};

} /* end namespace openfpga */

#endif
