#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "route_common.h"

#include "fpga_x2p_types.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_rr_graph_utils.h"
#include "fpga_x2p_pb_rr_graph.h"

void breadth_first_expand_rr_graph_trace_segment(t_rr_graph* local_rr_graph,
                                                 t_trace *start_ptr, 
                                                 int remaining_connections_to_sink) {

  /* Adds all the rr_nodes in the traceback segment starting at tptr (and     *
   * continuing to the end of the traceback) to the heap with a cost of zero. *
   * This allows expansion to begin from the existing wiring.  The            *
   * remaining_connections_to_sink value is 0 if the route segment ending     *
   * at this location is the last one to connect to the SINK ending the route *
   * segment.  This is the usual case.  If it is not the last connection this *
   * net must make to this SINK, I have a hack to ensure the next connection  *
   * to this SINK goes through a different IPIN.  Without this hack, the      *
   * router would always put all the connections from this net to this SINK   *
   * through the same IPIN.  With LUTs or cluster-based logic blocks, you     *
   * should never have a net connecting to two logically-equivalent pins on   *
   * the same logic block, so the hack will never execute.  If your logic     *
   * block is an and-gate, however, nets might connect to two and-inputs on   *
   * the same logic block, and since the and-inputs are logically-equivalent, *
   * this means two connections to the same SINK.                             */

  t_trace *tptr, *next_ptr;
  int inode, sink_node, last_ipin_node;

  tptr = start_ptr;

  if (remaining_connections_to_sink == 0) { /* Usual case. */
    while (tptr != NULL) {
      add_node_to_rr_graph_heap(local_rr_graph, tptr->index, 0., NO_PREVIOUS, NO_PREVIOUS, OPEN, OPEN);
      tptr = tptr->next;
    }
  } else { /* This case never executes for most logic blocks. */
    /* Weird case.  Lots of hacks. The cleanest way to do this would be to empty *
     * the heap, update the congestion due to the partially-completed route, put *
     * the whole route so far (excluding IPINs and SINKs) on the heap with cost  *
     * 0., and expand till you hit the next SINK.  That would be slow, so I      *
     * do some hacks to enable incremental wavefront expansion instead.          */

    if (tptr == NULL)
      return; /* No route yet */

    next_ptr = tptr->next;
    last_ipin_node = OPEN; /* Stops compiler from complaining. */

    /* Can't put last SINK on heap with NO_PREVIOUS, etc, since that won't let  *
     * us reach it again.  Instead, leave the last traceback element (SINK) off *
     * the heap.                                                                */

    while (next_ptr != NULL) {
      inode = tptr->index;
      add_node_to_rr_graph_heap(local_rr_graph, inode, 0., NO_PREVIOUS, NO_PREVIOUS, OPEN, OPEN);

      if (local_rr_graph->rr_node[inode].type == INTRA_CLUSTER_EDGE) {
        if ((local_rr_graph->rr_node[inode].pb_graph_pin != NULL)
         && (local_rr_graph->rr_node[inode].pb_graph_pin->num_output_edges == 0)) {
          last_ipin_node = inode;
        }
      }

      tptr = next_ptr;
      next_ptr = tptr->next;
    }

    /* This will stop the IPIN node used to get to this SINK from being         *
     * reexpanded for the remainder of this net's routing.  This will make us   *
     * hook up more IPINs to this SINK (which is what we want).  If IPIN        *
     * doglegs are allowed in the graph, we won't be able to use this IPIN to   *
     * do a dogleg, since it won't be re-expanded.  Shouldn't be a big problem. */
    assert(last_ipin_node != OPEN);
    local_rr_graph->rr_node_route_inf[last_ipin_node].path_cost = -HUGE_POSITIVE_FLOAT;

    /* Also need to mark the SINK as having high cost, so another connection can *
     * be made to it.                                                            */

    sink_node = tptr->index;
    local_rr_graph->rr_node_route_inf[sink_node].path_cost = HUGE_POSITIVE_FLOAT;

    /* Finally, I need to remove any pending connections to this SINK via the    *
     * IPIN I just used (since they would result in congestion).  Scan through   *
     * the heap to do this.                                                      */

    invalidate_rr_graph_heap_entries(local_rr_graph, sink_node, last_ipin_node);
  }

  return;
}

void breadth_first_expand_rr_graph_neighbours(t_rr_graph* local_rr_graph,
                                              int inode, float pcost,
                                              int inet, boolean first_time) {

  /* Puts all the rr_nodes adjacent to inode on the heap.  rr_nodes outside   *
   * the expanded bounding box specified in route_bb are not added to the     *
   * heap.  pcost is the path_cost to get to inode.                           */

  int iconn, to_node, num_edges;
  float tot_cost;

  num_edges = local_rr_graph->rr_node[inode].num_edges;
  for (iconn = 0; iconn < num_edges; iconn++) {
    to_node = local_rr_graph->rr_node[inode].edges[iconn];
    /*if (first_time) { */
    tot_cost = pcost + get_rr_graph_rr_cong_cost(local_rr_graph, to_node) 
               * get_rr_graph_rr_node_pack_intrinsic_cost(local_rr_graph, to_node);
    /*
     } else {
     tot_cost = pcost + get_rr_cong_cost(to_node);
     }*/
    add_node_to_rr_graph_heap(local_rr_graph, to_node, tot_cost, inode, iconn, OPEN, OPEN);
  }
}

/* A copy of breath_first_add_source_to_heap_cluster
 * I remove all the use of global variables
 */
void breadth_first_add_source_to_rr_graph_heap(t_rr_graph* local_rr_graph,
                                               int src_net_index) {

  /* Adds the SOURCE of this net to the heap.  Used to start a net's routing. */

  int inode;
  float cost;

  inode = local_rr_graph->net_rr_terminals[src_net_index][0]; /* SOURCE */
  cost = get_rr_graph_rr_cong_cost(local_rr_graph, inode);

  add_node_to_rr_graph_heap(local_rr_graph, inode, cost, NO_PREVIOUS, NO_PREVIOUS, OPEN, OPEN);
}

/* A copy of breath_first_add_source_to_heap_cluster
 * I remove all the use of global variables
 */
void breadth_first_add_one_source_to_rr_graph_heap(t_rr_graph* local_rr_graph,
                                                   int src_net_index,
                                                   int src_idx) {

  /* Adds the SOURCE of this net to the heap.  Used to start a net's routing. */
  int inode;
  float cost;

  inode = local_rr_graph->net_rr_sources[src_net_index][src_idx]; /* SOURCE */
  cost = get_rr_graph_rr_cong_cost(local_rr_graph, inode);

  add_node_to_rr_graph_heap(local_rr_graph, inode, cost, NO_PREVIOUS, NO_PREVIOUS, OPEN, OPEN);

  return;
}

/* A copy of breath_first_route_net_cluster
 * I remove all the use of global variables
 */
boolean breadth_first_route_one_net_pb_rr_graph(t_rr_graph* local_rr_graph, 
                                                     int inet) {

  /* Uses a maze routing (Dijkstra's) algorithm to route a net.  The net       *
   * begins at the net output, and expands outward until it hits a target      *
   * pin.  The algorithm is then restarted with the entire first wire segment  *
   * included as part of the source this time.  For an n-pin net, the maze     *
   * router is invoked n-1 times to complete all the connections.  Inet is     *
   * the index of the net to be routed.                                *
   * If this routine finds that a net *cannot* be connected (due to a complete *
   * lack of potential paths, rather than congestion), it returns FALSE, as    *
   * routing is impossible on this architecture.  Otherwise it returns TRUE.   */

  int i, inode, prev_node, remaining_connections_to_sink;
  float pcost, new_pcost;
  struct s_heap *current;
  struct s_trace *tptr;
  boolean first_time;

  free_rr_graph_traceback(local_rr_graph, inet);
  breadth_first_add_source_to_rr_graph_heap(local_rr_graph, inet);
  mark_rr_graph_ends(local_rr_graph, inet);

  tptr = NULL;
  remaining_connections_to_sink = 0;

  for (i = 1; i < local_rr_graph->net_num_sinks[inet] + 1; i++) { /* Need n-1 wires to connect n pins */

    /* Do not connect open terminals */
    if (local_rr_graph->net_rr_terminals[inet][i] == OPEN) {
      continue;
    }
    /* Expand and begin routing */
    breadth_first_expand_rr_graph_trace_segment(local_rr_graph, tptr, remaining_connections_to_sink);
    current = get_rr_graph_heap_head(local_rr_graph);

    if (current == NULL) { /* Infeasible routing.  No possible path for net. */
      reset_rr_graph_path_costs(local_rr_graph); /* Clean up before leaving. */
      return (FALSE);
    }

    inode = current->index;

    while (local_rr_graph->rr_node_route_inf[inode].target_flag == 0) {
      pcost = local_rr_graph->rr_node_route_inf[inode].path_cost;
      new_pcost = current->cost;
      if (pcost > new_pcost) { /* New path is lowest cost. */
        local_rr_graph->rr_node_route_inf[inode].path_cost = new_pcost;
        prev_node = current->u.prev_node;
        local_rr_graph->rr_node_route_inf[inode].prev_node = prev_node;
        local_rr_graph->rr_node_route_inf[inode].prev_edge = current->prev_edge;
        first_time = FALSE;

        if (pcost > 0.99 * HUGE_POSITIVE_FLOAT) /* First time touched. */{
          add_to_rr_graph_mod_list(local_rr_graph, &local_rr_graph->rr_node_route_inf[inode].path_cost);
          first_time = TRUE;
        }

        breadth_first_expand_rr_graph_neighbours(local_rr_graph, inode, new_pcost, inet, first_time);
      }

      free_rr_graph_heap_data(local_rr_graph, current);
      current = get_rr_graph_heap_head(local_rr_graph);

      if (current == NULL) { /* Impossible routing. No path for net. */
        reset_rr_graph_path_costs(local_rr_graph);
        return (FALSE);
      }

      inode = current->index;
    }

    local_rr_graph->rr_node_route_inf[inode].target_flag--; /* Connected to this SINK. */
    remaining_connections_to_sink = local_rr_graph->rr_node_route_inf[inode].target_flag;
    tptr = update_rr_graph_traceback(local_rr_graph, current, inet);
    free_rr_graph_heap_data(local_rr_graph, current);
  }

  empty_rr_graph_heap(local_rr_graph);
  reset_rr_graph_path_costs(local_rr_graph);
  return (TRUE);
}

/* Adapt for the multi-source rr_graph routing
 */
boolean breadth_first_route_one_single_source_net_pb_rr_graph(t_rr_graph* local_rr_graph, 
                                                              int inet, int isrc, 
                                                              int start_isink,
                                                              boolean* net_sink_routed) {

  /* Uses a maze routing (Dijkstra's) algorithm to route a net.  The net       *
   * begins at the net output, and expands outward until it hits a target      *
   * pin.  The algorithm is then restarted with the entire first wire segment  *
   * included as part of the source this time.  For an n-pin net, the maze     *
   * router is invoked n-1 times to complete all the connections.  Inet is     *
   * the index of the net to be routed.                                *
   * If this routine finds that a net *cannot* be connected (due to a complete *
   * lack of potential paths, rather than congestion), it returns FALSE, as    *
   * routing is impossible on this architecture.  Otherwise it returns TRUE.   */

  int isink, inode, jsink, prev_node, remaining_connections_to_sink;
  float pcost, new_pcost;
  struct s_heap *current;
  struct s_trace *tptr;
  boolean first_time;

  free_rr_graph_traceback(local_rr_graph, inet);
  breadth_first_add_one_source_to_rr_graph_heap(local_rr_graph, inet, isrc);
  mark_rr_graph_sinks(local_rr_graph, inet, start_isink, net_sink_routed);
  
  tptr = NULL;
  remaining_connections_to_sink = 0;

#ifdef VERBOSE
  printf("Sink nodes for net=%s to try: ",
         local_rr_graph->net[inet]->name);
  for (isink = start_isink; isink < local_rr_graph->net_num_sinks[inet]; isink++) {
    if (OPEN == local_rr_graph->net_rr_sinks[inet][isink]) {
      continue;
    }
    if (TRUE == net_sink_routed[isink]) {
      continue;
    }
    printf("%d,",
           isink);
  }
  printf("\n");
#endif
  
  for (isink = start_isink; isink < local_rr_graph->net_num_sinks[inet]; isink++) { /* Need n-1 wires to connect n pins */
    /* Do not connect open terminals */
    if (OPEN == local_rr_graph->net_rr_sinks[inet][isink]) {
      continue;
    }

    /* Expand and begin routing */
    breadth_first_expand_rr_graph_trace_segment(local_rr_graph, tptr, remaining_connections_to_sink);
    current = get_rr_graph_heap_head(local_rr_graph);

    /* Exit only when this is the last source node 
     * Infeasible routing.  No possible path for net. 
     */
    if (NULL == current) {
#ifdef VERBOSE
      printf("1. Fail Routing: net=%s, sink=%d\n",
             local_rr_graph->net[inet]->name,
             isink);
#endif
      reset_rr_graph_path_costs(local_rr_graph);
      return FALSE;
    }

    inode = current->index;

    while (local_rr_graph->rr_node_route_inf[inode].target_flag == 0) {
      pcost = local_rr_graph->rr_node_route_inf[inode].path_cost;
      new_pcost = current->cost;
      if (pcost > new_pcost) { /* New path is lowest cost. */
        local_rr_graph->rr_node_route_inf[inode].path_cost = new_pcost;
        prev_node = current->u.prev_node;
        local_rr_graph->rr_node_route_inf[inode].prev_node = prev_node;
        local_rr_graph->rr_node_route_inf[inode].prev_edge = current->prev_edge;
        first_time = FALSE;

        if (pcost > 0.99 * HUGE_POSITIVE_FLOAT) /* First time touched. */{
          add_to_rr_graph_mod_list(local_rr_graph, &local_rr_graph->rr_node_route_inf[inode].path_cost);
          first_time = TRUE;
        }

        breadth_first_expand_rr_graph_neighbours(local_rr_graph, inode, new_pcost, inet, first_time);

        if ( (0 == strcmp("_18363_", local_rr_graph->net[inet]->name))
           && (0 == strcmp("fle", local_rr_graph->rr_node[inode].pb_graph_pin->parent_node->pb_type->name))
           && (0 == strcmp("out", local_rr_graph->rr_node[inode].pb_graph_pin->port->name))
           && (2 == local_rr_graph->rr_node[inode].pb_graph_pin->parent_node->placement_index) 
           && (1 == local_rr_graph->rr_node[inode].pb_graph_pin->pin_number) ) {
          vpr_printf(TIO_MESSAGE_INFO,
                     "Expanding to node: %s/%s[%d], cost=%.5g\n",
                     get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node),
                     local_rr_graph->rr_node[inode].pb_graph_pin->port->name,
                     local_rr_graph->rr_node[inode].pb_graph_pin->pin_number,
                     new_pcost);
        }

        if ( (0 == strcmp("_18363_", local_rr_graph->net[inet]->name))
           && (0 == strcmp("fle", local_rr_graph->rr_node[inode].pb_graph_pin->parent_node->pb_type->name))
           && (0 == strcmp("out", local_rr_graph->rr_node[inode].pb_graph_pin->port->name))
           && (3 == local_rr_graph->rr_node[inode].pb_graph_pin->parent_node->placement_index) 
           && (0 == local_rr_graph->rr_node[inode].pb_graph_pin->pin_number) ) {
          vpr_printf(TIO_MESSAGE_INFO,
                     "Expanding to node: %s/%s[%d], cost=%.5g\n",
                     get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node),
                     local_rr_graph->rr_node[inode].pb_graph_pin->port->name,
                     local_rr_graph->rr_node[inode].pb_graph_pin->pin_number,
                     new_pcost);
        }

        if ( (0 == strcmp("_18363_", local_rr_graph->net[inet]->name))
           && (0 == strcmp("fle", local_rr_graph->rr_node[inode].pb_graph_pin->parent_node->pb_type->name))
           && (0 == strcmp("regout", local_rr_graph->rr_node[inode].pb_graph_pin->port->name))
           && (2 == local_rr_graph->rr_node[inode].pb_graph_pin->parent_node->placement_index) 
           && (0 == local_rr_graph->rr_node[inode].pb_graph_pin->pin_number) ) {
          vpr_printf(TIO_MESSAGE_INFO,
                     "Expanding to node: %s/%s[%d], cost=%.5g\n",
                     get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node),
                     local_rr_graph->rr_node[inode].pb_graph_pin->port->name,
                     local_rr_graph->rr_node[inode].pb_graph_pin->pin_number,
                     new_pcost);
        }
      }

      free_rr_graph_heap_data(local_rr_graph, current);
      current = get_rr_graph_heap_head(local_rr_graph);

      /* Impossible routing. No path for net. 
       */
      if (NULL == current) { 
        break;
      }

      inode = current->index;


    }

    /* Impossible routing, try another iteration */
    if (NULL == current) {
#ifdef VERBOSE
      printf("2. Fail Routing: net=%s, sink=%d\n",
             local_rr_graph->net[inet]->name,
             isink);
#endif
      reset_rr_graph_path_costs(local_rr_graph);
      continue;
    }
 
    local_rr_graph->rr_node_route_inf[inode].target_flag--; /* Connected to this SINK. */
    remaining_connections_to_sink = local_rr_graph->rr_node_route_inf[inode].target_flag;
    tptr = update_rr_graph_traceback(local_rr_graph, current, inet);
    free_rr_graph_heap_data(local_rr_graph, current);

    /* Update the flag
     * BE CAREFUL: the inode is one of the sink
     * it is not always the isink node!!!
     * In each iteration, the routing algorithm will route a sink with lowest cost,
     * This is out of the order of sinks 
     */
    for (jsink = start_isink; jsink < local_rr_graph->net_num_sinks[inet]; jsink++) { /* Need n-1 wires to connect n pins */
      if (inode != local_rr_graph->net_rr_sinks[inet][jsink]) {
        continue;
      }
      net_sink_routed[jsink] = TRUE;
#ifdef VERBOSE
      printf("Round %d, Success Routing: net=%s, sink=%d, port=%s[%d], pb_type=%s\n",
             isink, local_rr_graph->net[inet]->name,
             jsink, 
             local_rr_graph->rr_node[inode].pb_graph_pin->port->name,
             local_rr_graph->rr_node[inode].pb_graph_pin->pin_number,
             get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node));
#endif
      break;
    }
  } 

  return TRUE;
}


/* Adapt for the multi-source rr_graph routing
 */
boolean breadth_first_route_one_multi_source_net_pb_rr_graph(t_rr_graph* local_rr_graph, 
                                                             int inet) {

  /* Uses a maze routing (Dijkstra's) algorithm to route a net.  The net       *
   * begins at the net output, and expands outward until it hits a target      *
   * pin.  The algorithm is then restarted with the entire first wire segment  *
   * included as part of the source this time.  For an n-pin net, the maze     *
   * router is invoked n-1 times to complete all the connections.  Inet is     *
   * the index of the net to be routed.                                *
   * If this routine finds that a net *cannot* be connected (due to a complete *
   * lack of potential paths, rather than congestion), it returns FALSE, as    *
   * routing is impossible on this architecture.  Otherwise it returns TRUE.   */

  int isrc, isink, inode, start_isink;
  boolean* net_sink_routed = (boolean*) my_malloc(local_rr_graph->net_num_sinks[inet] * sizeof(boolean));
  boolean route_success = FALSE;

  /* Initialize */
  for (isink = 0; isink < local_rr_graph->net_num_sinks[inet]; isink++) { 
    net_sink_routed[isink] = FALSE;
    /* Exception for OPEN nets */
    if (OPEN == local_rr_graph->net_rr_sinks[inet][isink]) {
      net_sink_routed[isink] = TRUE;
    }
  }

  start_isink = 0;

  /* try each sources */
  for (isrc = 0; isrc < local_rr_graph->net_num_sources[inet]; isrc++) {
    /* Get the start sink index, 
     * when a sink is failed in routing,  
     * we update flags the net_sink_routed 
     * Next time, we will start from first sink is 
     */
#ifdef VERBOSE
    printf("\nnum_src=%d, isrc=%d\n", local_rr_graph->net_num_sources[inet], isrc);
#endif
    for (isink = 0; isink < local_rr_graph->net_num_sinks[inet]; isink++) {
      if (TRUE == net_sink_routed[isink]) {
        continue;
      }
      start_isink = isink;
      break; 
    }

#ifdef VERBOSE
    printf("\nstart_sink=%d\n", start_isink);
#endif
    /* Reset the target_flag for sinks to be routed */
    start_isink = 0;
    for (isink = start_isink; isink < local_rr_graph->net_num_sinks[inet]; isink++) {
      inode = local_rr_graph->net_rr_sinks[inet][isink];
      if (OPEN != inode) {
        local_rr_graph->rr_node_route_inf[inode].target_flag = 0; 
      }
    } 
    breadth_first_route_one_single_source_net_pb_rr_graph(local_rr_graph, inet, isrc, 
                                                          start_isink,
                                                          net_sink_routed); 
    /* Clean up routing data */
    empty_rr_graph_heap(local_rr_graph);
    reset_rr_graph_path_costs(local_rr_graph);
    /* See if we can early exit */
    /* Make sure every sink if routed */
    route_success = TRUE;
    for (isink = 0; isink < local_rr_graph->net_num_sinks[inet]; isink++) { 
      if ( (OPEN != local_rr_graph->net_rr_sinks[inet][isink])
          && (FALSE == net_sink_routed[isink])) {
        route_success = FALSE;
      }
    }
    if (TRUE == route_success) {
      break;
    }
  }

  /* Provide more information for users when route fails */
  if (FALSE == route_success) {
    for (isink = 0; isink < local_rr_graph->net_num_sinks[inet]; isink++) { 
      if ( (OPEN != local_rr_graph->net_rr_sinks[inet][isink])
          && (FALSE == net_sink_routed[isink])) {
        /* Give informatioin about the starting pin */
        vpr_printf(TIO_MESSAGE_ERROR, "Fail Routing:\n");

        for (isrc = 0; isrc < local_rr_graph->net_num_sources[inet]; isrc++) {
          inode = local_rr_graph->net_rr_sources[inet][isrc];
          vpr_printf(TIO_MESSAGE_INFO,
                     "Starting points %d: net=%s, source_rr_node=%d, port=%s[%d], pb_type=%s\n",
                     isrc, 
                     local_rr_graph->net[inet]->name, inode,
                     local_rr_graph->rr_node[inode].pb_graph_pin->port->name,
                     local_rr_graph->rr_node[inode].pb_graph_pin->pin_number,
                     get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node));
        }
        /* Give informatioin about the starting pin */
        inode = local_rr_graph->net_rr_sinks[inet][isink];
        vpr_printf(TIO_MESSAGE_INFO,
                   "Ending points: net=%s, sink=%d, port=%s[%d], pb_type=%s\n",
                   local_rr_graph->net[inet]->name,
                   isink, 
                   local_rr_graph->rr_node[inode].pb_graph_pin->port->name,
                   local_rr_graph->rr_node[inode].pb_graph_pin->pin_number,
                   get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node));
        vpr_printf(TIO_MESSAGE_INFO,
                   "Please double check your XML about if pins in operating modes are correctedly linked to physical mode!\n");
      }
    }
  }
  
  /* Free */
  free(net_sink_routed);

  return route_success;
}


boolean feasible_routing_rr_graph(t_rr_graph* local_rr_graph,  
                                  boolean verbose) {

  /* This routine checks to see if this is a resource-feasible routing.      *
   * That is, are all rr_node capacity limitations respected?  It assumes    *
   * that the occupancy arrays are up to date when it is called.             */

  int inode, inet;
  t_trace* tptr;
  int* rr_node_net_checker = (int*) my_calloc (local_rr_graph->num_rr_nodes, sizeof(int)); 
  int* rr_node_occ_checker = (int*) my_calloc (local_rr_graph->num_rr_nodes, sizeof(int)); 
  boolean feasible = TRUE;
  
  /* Initialize the checker */
  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    rr_node_net_checker[inode] = OPEN;
  }
  
  /* Check the trace:
   * We may have the same nets sharing the part of the traces
   * We will adapt the occupancy of rr_node to avoid errors in feasibility checking 
   */
  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    tptr = local_rr_graph->trace_head[inet];
    while (tptr != NULL) {
      inode = tptr->index;
      /* Update the checker for net num */
      if (OPEN == rr_node_net_checker[inode]) {    
        /* First visit, we assign a value */
        rr_node_net_checker[inode] = inet;
        /* Initialize occ */
        rr_node_occ_checker[inode] = 1;
      } else if (inet != rr_node_net_checker[inode]) {
        /* This means two traces share the same node
         * This is not a feasible routing, error out
         */
        if (TRUE == verbose) {
          vpr_printf(TIO_MESSAGE_ERROR, 
                     "(File:%s,[LINE%d]) Shared Trace Found for rr_node[%d] (pb pin:%s/%s[%d]) between net(%s) and net (%s)!\n",
                     __FILE__, __LINE__, 
                     inode,
                     get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node),  
                     local_rr_graph->rr_node[inode].pb_graph_pin->port->name, 
                     local_rr_graph->rr_node[inode].pb_graph_pin->pin_number, 
                     local_rr_graph->net[inet]->name, 
                     local_rr_graph->net[rr_node_net_checker[inode]]->name);
        }
        /* Increase occ */
        rr_node_occ_checker[inode]++;
      } else {
        assert (inet == rr_node_net_checker[inode]);  
        /* Try to increase the capacity of rr_node */
        if (TRUE == verbose) {
#ifdef VERBOSE
          vpr_printf(TIO_MESSAGE_INFO, 
                     "(File:%s,[LINE%d]) Detect rr_node[%d] (pb pin:%s/%s[%d]) for shared net(%s)!\n",
                     __FILE__, __LINE__, 
                     inode,
                     get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node),  
                     local_rr_graph->rr_node[inode].pb_graph_pin->port->name, 
                     local_rr_graph->rr_node[inode].pb_graph_pin->pin_number, 
                     local_rr_graph->net[inet]->name);
#endif
        }
      } 
      tptr = tptr->next;
    }
  }

  /* Update occ of rr_graph */
  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    local_rr_graph->rr_node[inode].occ = rr_node_occ_checker[inode];
  }
  /* Free */
  free(rr_node_occ_checker);
  free(rr_node_net_checker);

  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    if (local_rr_graph->rr_node[inode].occ > local_rr_graph->rr_node[inode].capacity) {
      if (TRUE == verbose) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d]) rr_node[%d] (pin:%s/%s[%d]) occupancy(%d) exceeds its capacity(%d)!\n",
                   __FILE__, __LINE__, 
                   inode, 
                   get_pb_graph_full_name_in_hierarchy(local_rr_graph->rr_node[inode].pb_graph_pin->parent_node),  
                   local_rr_graph->rr_node[inode].pb_graph_pin->port->name, 
                   local_rr_graph->rr_node[inode].pb_graph_pin->pin_number, 
                   local_rr_graph->rr_node[inode].occ, 
                   local_rr_graph->rr_node[inode].capacity);
      }
      feasible = FALSE;
    }
  }

  return feasible;
}

void pathfinder_update_rr_graph_one_cost(t_rr_graph* local_rr_graph, 
                                         t_trace *route_segment_start,
                                         int add_or_sub, float pres_fac) {

  /* This routine updates the occupancy and pres_cost of the rr_nodes that are *
   * affected by the portion of the routing of one net that starts at          *
   * route_segment_start.  If route_segment_start is trace_head[inet], the     *
   * cost of all the nodes in the routing of net inet are updated.  If         *
   * add_or_sub is -1 the net (or net portion) is ripped up, if it is 1 the    *
   * net is added to the routing.  The size of pres_fac determines how severly *
   * oversubscribed rr_nodes are penalized.                                    */

  struct s_trace *tptr;
  int inode, occ, capacity;

  tptr = route_segment_start;
  if (tptr == NULL) /* No routing yet. */
    return;

  for (;;) {
    inode = tptr->index;

    occ = local_rr_graph->rr_node[inode].occ + add_or_sub;
    capacity = local_rr_graph->rr_node[inode].capacity;

    local_rr_graph->rr_node[inode].occ = occ;

    /* pres_cost is Pn in the Pathfinder paper. I set my pres_cost according to *
     * the overuse that would result from having ONE MORE net use this routing  *
     * node.                                                                    */

    if (occ < capacity) {
      local_rr_graph->rr_node_route_inf[inode].pres_cost = 1.;
    } else {
      local_rr_graph->rr_node_route_inf[inode].pres_cost = 1. + (occ + 1 - capacity) * pres_fac;
    }

    if (local_rr_graph->rr_node[inode].type == SINK) {
      tptr = tptr->next; /* Skip next segment. */
      if (tptr == NULL)
        break;
    }

    tptr = tptr->next;

  } /* End while loop -- did an entire traceback. */

  return;
}

void pathfinder_update_rr_graph_cost(t_rr_graph* local_rr_graph,
                                     float pres_fac, float acc_fac) {

  /* This routine recomputes the pres_cost and acc_cost of each routing        *
   * resource for the pathfinder algorithm after all nets have been routed.    *
   * It updates the accumulated cost to by adding in the number of extra       *
   * signals sharing a resource right now (i.e. after each complete iteration) *
   * times acc_fac.  It also updates pres_cost, since pres_fac may have        *
   * changed.  THIS ROUTINE ASSUMES THE OCCUPANCY VALUES IN RR_NODE ARE UP TO  *
   * DATE.                                                                     */

  int inode, occ, capacity;

  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    occ = local_rr_graph->rr_node[inode].occ;
    capacity = local_rr_graph->rr_node[inode].capacity;

    if (occ > capacity) {
      local_rr_graph->rr_node_route_inf[inode].acc_cost += (occ - capacity) * acc_fac;
      local_rr_graph->rr_node_route_inf[inode].pres_cost = 1. + (occ + 1 - capacity) * pres_fac;
    } else if (occ == capacity) {
    /* If occ == capacity, we don't need to increase acc_cost, but a change    *
     * in pres_fac could have made it necessary to recompute the cost anyway.  */
      local_rr_graph->rr_node_route_inf[inode].pres_cost = 1. + pres_fac;
    }
  }

  return;
}


/** 
 * A Copy of try_breadth_first_route_cluster 
 * I remove all the use of global variables  
 * internal_nets: index of nets to route [0..num_internal_nets - 1]
 */
boolean try_breadth_first_route_pb_rr_graph(t_rr_graph* local_rr_graph) {

  /* Iterated maze router ala Pathfinder Negotiated Congestion algorithm,  *
   * (FPGA 95 p. 111).  Returns TRUE if it can route this FPGA, FALSE if   *
   * it can't.                                                             */

  /* For different modes, when a mode is turned on, I set the max occupancy of all rr_nodes in the mode to 1 and all others to 0 */
  /* TODO: There is a bug for route-throughs where edges in route-throughs do not get turned off because the rr_edge is in a particular mode but the two rr_nodes are outside */

  boolean success, is_routable;
  int itry, inet, net_index;
  struct s_router_opts router_opts;
  float pres_fac;

  /* Xifan TANG: Count runtime for routing in packing stage */
  clock_t begin, end;

  begin = clock();

  /* Usually the first iteration uses a very small (or 0) pres_fac to find  *
   * the shortest path and get a congestion map.  For fast compiles, I set  *
   * pres_fac high even for the first iteration.                            */

  /* sets up a fast breadth-first router */
  router_opts.first_iter_pres_fac = 10; 
  router_opts.max_router_iterations = 20;
  router_opts.initial_pres_fac = 10; 
  router_opts.pres_fac_mult = 2; 
  router_opts.acc_fac = 1; 

  /* Default breath-first router opts */
  /* 
  router_opts.first_iter_pres_fac = 0; 
  router_opts.max_router_iterations = 50;
  router_opts.initial_pres_fac = 0.5; 
  router_opts.pres_fac_mult = 1.3;
  router_opts.acc_fac = 0.2;
  */

  reset_rr_graph_rr_node_route_structs(local_rr_graph); /* Clear all prior rr_graph history */

  pres_fac = router_opts.first_iter_pres_fac;

  for (itry = 1; itry <= router_opts.max_router_iterations; itry++) {
    for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
      net_index = inet;

      pathfinder_update_rr_graph_one_cost(local_rr_graph, local_rr_graph->trace_head[net_index], -1, pres_fac);

      /* 
      is_routable = breadth_first_route_one_net_pb_rr_graph(local_rr_graph, net_index);
      */
      is_routable = breadth_first_route_one_multi_source_net_pb_rr_graph(local_rr_graph, net_index);

      /* Impossible to route? (disconnected rr_graph) */

      if (!is_routable) {
        /* TODO: Inelegant, can be more intelligent */
        vpr_printf(TIO_MESSAGE_INFO, "Failed routing net %s\n", local_rr_graph->net[net_index]->name);
        vpr_printf(TIO_MESSAGE_INFO, "Routing failed. Disconnected rr_graph.\n");
        return FALSE;
      } else {
        /*
        vpr_printf(TIO_MESSAGE_INFO, "Succeed routing net %s\n", local_rr_graph->net[net_index]->name);
        */
      }

      pathfinder_update_rr_graph_one_cost(local_rr_graph, local_rr_graph->trace_head[net_index], 1, pres_fac);

    }

    success = feasible_routing_rr_graph(local_rr_graph, FALSE);
    if (success) {
      /* End of packing routing */
      end = clock();
      /* accumulate the runtime for pack routing */
#ifdef CLOCKS_PER_SEC
      pack_route_time += (float)(end - begin)/ CLOCKS_PER_SEC; 
#else
      pack_route_time += (float)(end - begin)/ CLK_PER_SEC; 
#endif
      /* vpr_printf(TIO_MESSAGE_INFO, "Updated: Packing routing took %g seconds\n", pack_route_time); */
      return (TRUE);
    }

    if (itry == 1) {
      pres_fac = router_opts.initial_pres_fac;
    } else {
      pres_fac *= router_opts.pres_fac_mult;
    }

    pres_fac = std::min(pres_fac, static_cast<float>(HUGE_POSITIVE_FLOAT / 1e5));

    pathfinder_update_rr_graph_cost(local_rr_graph, pres_fac, router_opts.acc_fac);
  }
  /* End of packing routing */
  end = clock();
  /* accumulate the runtime for pack routing */
#ifdef CLOCKS_PER_SEC
  pack_route_time += (float)(end - begin)/ CLOCKS_PER_SEC; 
#else
  pack_route_time += (float)(end - begin)/ CLK_PER_SEC; 
#endif
  vpr_printf(TIO_MESSAGE_INFO, "Packing/Routing Physical Progammable Blocks took %g seconds\n", pack_route_time);

  /* Give error message when routing is failed */
  feasible_routing_rr_graph(local_rr_graph, TRUE);

  return (FALSE);
}



