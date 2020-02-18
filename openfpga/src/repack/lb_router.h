/********************************************************************
 * Intra-logic block router determines if a candidate packing solution (or intermediate solution) can route.
 * Adapted from original VPR cluster router to the use of LbRRGraph object 
 *******************************************************************/
#ifndef LB_ROUTER_H
#define LB_ROUTER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "atom_netlist_fwd.h"
#include "pack_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* Constructors/Destructors */
t_lb_router_data* alloc_and_load_router_data(std::vector<t_lb_type_rr_node>* lb_type_graph, t_logical_block_type_ptr type);
void free_router_data(t_lb_router_data* router_data);
void free_intra_lb_nets(std::vector<t_intra_lb_net>* intra_lb_nets);

/* Routing Functions */
void add_atom_as_target(t_lb_router_data* router_data, const AtomBlockId blk_id);
void remove_atom_from_target(t_lb_router_data* router_data, const AtomBlockId blk_id);
void set_reset_pb_modes(t_lb_router_data* router_data, const t_pb* pb, const bool set);
bool try_intra_lb_route(t_lb_router_data* router_data, int verbosity, t_mode_selection_status* mode_status);
void reset_intra_lb_route(t_lb_router_data* router_data);

/* Accessor Functions */
t_pb_routes alloc_and_load_pb_route(const std::vector<t_intra_lb_net>* intra_lb_nets, t_pb_graph_node* pb_graph_head);
void free_pb_route(t_pb_route* free_pb_route);

} /* end namespace openfpga */

#endif
