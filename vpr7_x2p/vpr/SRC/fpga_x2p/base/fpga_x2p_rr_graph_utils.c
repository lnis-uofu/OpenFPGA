/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "vpr_utils.h"
#include "route_common.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_rr_graph_utils.h"

/* Initial rr_graph */
void init_rr_graph(INOUTP t_rr_graph* local_rr_graph) {
  /* Give zero and NULL to all the contents */
  local_rr_graph->num_rr_nodes = 0;
  local_rr_graph->rr_node = NULL;
  local_rr_graph->rr_node_indices = NULL;
  
  local_rr_graph->num_switch_inf = 0;
  local_rr_graph->switch_inf = NULL;
  local_rr_graph->delayless_switch_index = 0;

  local_rr_graph->num_nets = 0;
  local_rr_graph->net = NULL;
  local_rr_graph->net_to_vpack_net_mapping = NULL;
  local_rr_graph->net_num_sources = NULL;
  local_rr_graph->net_num_sinks = NULL;

  local_rr_graph->net_rr_sources = NULL;
  local_rr_graph->net_rr_sinks = NULL;
  local_rr_graph->net_rr_terminals = NULL;
  memset(&(local_rr_graph->rr_mem_ch), 0, sizeof(t_chunk)); 

  local_rr_graph->num_rr_indexed_data = 0;
  local_rr_graph->rr_indexed_data = NULL;

  local_rr_graph->rr_node_route_inf = NULL;
  local_rr_graph->route_bb = NULL;

  local_rr_graph->trace_head = NULL; 
  local_rr_graph->trace_tail = NULL;
  local_rr_graph->trace_free_head = NULL;     
  memset(&(local_rr_graph->trace_ch), 0, sizeof(t_chunk)); 

  local_rr_graph->heap = NULL;
  local_rr_graph->heap_size = 0;
  local_rr_graph->heap_tail = 0;

  local_rr_graph->heap_free_head = NULL;     
  memset(&(local_rr_graph->heap_ch), 0, sizeof(t_chunk)); 

  local_rr_graph->rr_modified_head = NULL;
  local_rr_graph->linked_f_pointer_free_head = NULL;

  //local_rr_graph->linked_f_pointer_ch = {NULL, 0, NULL}; 
  memset(&(local_rr_graph->linked_f_pointer_ch), 0, sizeof(t_chunk)); 

  #ifdef DEBUG
  local_rr_graph->num_trace_allocated = 0;
  local_rr_graph->num_heap_allocated = 0;
  local_rr_graph->num_linked_f_pointer_allocated = 0;
  #endif

  return;
}

void alloc_rr_graph_net_rr_sources_and_sinks(t_rr_graph* local_rr_graph) {
  int inet, isrc, isink, inode;
  int num_sources_in_rr_graph, num_sinks_in_rr_graph;

  local_rr_graph->net_num_sources = (int *) my_calloc(local_rr_graph->num_nets, sizeof(int));
  local_rr_graph->net_num_sinks = (int *) my_calloc(local_rr_graph->num_nets, sizeof(int));

  local_rr_graph->net_rr_sources = (int **) my_calloc(local_rr_graph->num_nets, sizeof(int*));
  local_rr_graph->net_rr_sinks = (int **) my_calloc(local_rr_graph->num_nets, sizeof(int*));

  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    /* Count how many SINKs we have in the rr_graph that is mapped to this net */
    num_sources_in_rr_graph = 0;
    num_sinks_in_rr_graph = 0;
    for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
      /* Only care the rr_node mapped to this net */
      if (inet != local_rr_graph->rr_node[inode].net_num) {
        continue; 
      }
      if (SOURCE == local_rr_graph->rr_node[inode].type) {
        num_sources_in_rr_graph++;
      }
      if (SINK == local_rr_graph->rr_node[inode].type) {
        num_sinks_in_rr_graph++;
      }
    }
    local_rr_graph->net_num_sources[inet] = num_sources_in_rr_graph; 
    local_rr_graph->net_num_sinks[inet] = num_sinks_in_rr_graph; 
    /* Consider the SOURCE (index=0), a special SINK */
    local_rr_graph->net_rr_sources[inet] = (int *) my_malloc(num_sources_in_rr_graph * sizeof(int));
    local_rr_graph->net_rr_sinks[inet] = (int *) my_malloc(num_sinks_in_rr_graph * sizeof(int));
    /* Initialize all terminal to be OPEN */
    for (isrc = 0; isrc < local_rr_graph->net_num_sources[inet]; isrc++) {
      local_rr_graph->net_rr_sources[inet][isrc] = OPEN;
    }
    for (isink = 0; isink < local_rr_graph->net_num_sinks[inet]; isink++) {
      local_rr_graph->net_rr_sinks[inet][isink] = OPEN;
    }
  }

  return;
}

void alloc_rr_graph_net_rr_terminals(t_rr_graph* local_rr_graph) {
  int inet, isink, inode, num_sinks_in_rr_graph;

  local_rr_graph->net_rr_terminals = (int **) my_malloc(local_rr_graph->num_nets * sizeof(int *));
  local_rr_graph->net_num_sinks = (int *) my_calloc(local_rr_graph->num_nets, sizeof(int));

  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    /* Count how many SINKs we have in the rr_graph that is mapped to this net */
    num_sinks_in_rr_graph = 0;
    for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
      if ((SINK == local_rr_graph->rr_node[inode].type)
        && (inet == local_rr_graph->rr_node[inode].net_num)) {
        num_sinks_in_rr_graph++;
      }
    }
    local_rr_graph->net_num_sinks[inet] = num_sinks_in_rr_graph; 
    /* Consider the SOURCE (index=0), a special SINK */
    local_rr_graph->net_rr_terminals[inet] = (int *) my_chunk_malloc((num_sinks_in_rr_graph + 1) * sizeof(int),
                                                                      &local_rr_graph->rr_mem_ch);
    /* Initialize all terminal to be OPEN */
    local_rr_graph->net_rr_terminals[inet][0] = OPEN;
    for (isink = 1; isink < local_rr_graph->net_num_sinks[inet] + 1; isink++) {
      local_rr_graph->net_rr_terminals[inet][isink] = OPEN;
    }
  }

  return;
}

void alloc_rr_graph_route_static_structs(t_rr_graph* local_rr_graph,
                                         int heap_size) {
  local_rr_graph->trace_head = (t_trace **) my_calloc(local_rr_graph->num_nets, sizeof(t_trace*));
  local_rr_graph->trace_tail = (t_trace **) my_malloc(local_rr_graph->num_nets * sizeof(t_trace*));

  local_rr_graph->heap_size = heap_size;
  local_rr_graph->heap = (t_heap **) my_malloc(local_rr_graph->heap_size * sizeof(t_heap*));
  local_rr_graph->heap--; /* heap stores from [1..heap_size] */
  local_rr_graph->heap_tail = 1;

  local_rr_graph->route_bb = (t_bb *) my_malloc(local_rr_graph->num_nets * sizeof(t_bb));

  return;
}

void alloc_and_load_rr_graph_rr_node(INOUTP t_rr_graph* local_rr_graph,
                                     int local_num_rr_nodes) {
  local_rr_graph->num_rr_nodes = local_num_rr_nodes;
  /* Allocate rr_graph */
  local_rr_graph->rr_node = (t_rr_node*) my_calloc(local_rr_graph->num_rr_nodes, sizeof(t_rr_node));

  return;
}

/* Returns the segment number at which the segment this track lies on        *
 * started.                                                                  */
int get_rr_graph_seg_start(INP t_seg_details * seg_details, INP int itrack,
		                   INP int chan_num, INP int seg_num) {

	int seg_start, length, start;

	seg_start = 1;
	if (FALSE == seg_details[itrack].longline) {

		length = seg_details[itrack].length;
		start = seg_details[itrack].start;

		/* Start is guaranteed to be between 1 and length.  Hence adding length to *
		 * the quantity in brackets below guarantees it will be nonnegative.       */
        /* Original VPR */
		assert(start > 0);
        /* end */
        /* mrFPGA: Xifan TANG */
        /* assert(is_stack ? start >= 0: start > 0); */
        /* end */
		assert(start <= length);

		/* NOTE: Start points are staggered between different channels.
		 * The start point must stagger backwards as chan_num increases.
		 * Unidirectional routing expects this to allow the N-to-N 
		 * assumption to be made with respect to ending wires in the core. */
        /* mrFPGA: Xifan TANG */
        /*
        seg_start = seg_num - (seg_num - (is_stack ? 1 : 0) + length + chan_num - start) % length;
        seg_start = std::max(seg_start, is_stack ? 0 : 1);
        */
        /* end */
        /* Original VPR */
		seg_start = seg_num - (seg_num + length + chan_num - start) % length;
		if (seg_start < 1) {
			seg_start = 1;
		}
        /* end */
	}

	return seg_start;
}


void load_rr_graph_chan_rr_indices(t_rr_graph* local_rr_graph,
                                   INP int nodes_per_chan, INP int chan_len,
                                   INP int num_chans, INP t_rr_type type, INP t_seg_details * seg_details,
                                   INOUTP int *index) {
  int chan, seg, track, start, inode;

  local_rr_graph->rr_node_indices[type] = (t_ivec **) my_malloc(sizeof(t_ivec *) * num_chans);
  for (chan = 0; chan < num_chans; ++chan) {
    local_rr_graph->rr_node_indices[type][chan] = (t_ivec *) my_malloc(sizeof(t_ivec) * chan_len);

    local_rr_graph->rr_node_indices[type][chan][0].nelem = 0;
    local_rr_graph->rr_node_indices[type][chan][0].list = NULL;

    for (seg = 1; seg < chan_len; ++seg) { 
      /* Alloc the track inode lookup list */
      local_rr_graph->rr_node_indices[type][chan][seg].nelem = nodes_per_chan;
      local_rr_graph->rr_node_indices[type][chan][seg].list = (int *) my_malloc(
          sizeof(int) * nodes_per_chan);
      for (track = 0; track < nodes_per_chan; ++track) {
        local_rr_graph->rr_node_indices[type][chan][seg].list[track] = OPEN;
      }
    }
  }

    /* Original VPR */
  for (chan = 0; chan < num_chans; ++chan) {
    for (seg = 1; seg < chan_len; ++seg) {
    /* end */
    /* mrFPGA: Xifan TANG */
    /*
  for (chan = (is_stack ? 1 : 0); chan < num_chans; ++chan) {
    for (seg = (is_stack ? 0 : 1); seg < chan_len; ++seg) {
    */
    /* end */
      /* Assign an inode to the starts of tracks */
      for (track = 0; track < local_rr_graph->rr_node_indices[type][chan][seg].nelem; ++track) {
        start = get_rr_graph_seg_start(seg_details, track, chan, seg);
                /* Original VPR */
           /* If the start of the wire doesn't have a inode, 
         * assign one to it. */
        inode = local_rr_graph->rr_node_indices[type][chan][start].list[track];
        if (OPEN == inode) {
          inode = *index;
          ++(*index);

          local_rr_graph->rr_node_indices[type][chan][start].list[track] = inode;
                }
                /* end */
        /* Assign inode of start of wire to current position */
        local_rr_graph->rr_node_indices[type][chan][seg].list[track] = inode;
      }
    }
  }
  return;
}


void alloc_and_load_rr_graph_rr_node_indices(t_rr_graph* local_rr_graph,
                                             INP int nodes_per_chan, 
                                             INP int L_nx, INP int L_ny, t_grid_tile** L_grid, 
                                             INOUTP int *index, INP t_seg_details * seg_details) {

  /* Allocates and loads all the structures needed for fast lookups of the   *
   * index of an rr_node.  rr_node_indices is a matrix containing the index  *
   * of the *first* rr_node at a given (i,j) location.                       */

  int i, j, k, ofs;
  t_ivec tmp;
  t_type_ptr type;

  /* Alloc the lookup table */
  local_rr_graph->rr_node_indices = (t_ivec ***) my_malloc(sizeof(t_ivec **) * NUM_RR_TYPES);
  local_rr_graph->rr_node_indices[IPIN] = (t_ivec **) my_malloc(sizeof(t_ivec *) * (L_nx + 2));
  local_rr_graph->rr_node_indices[SINK] = (t_ivec **) my_malloc(sizeof(t_ivec *) * (L_nx + 2));
  for (i = 0; i <= (L_nx + 1); ++i) {
    local_rr_graph->rr_node_indices[IPIN][i] = (t_ivec *) my_malloc(sizeof(t_ivec) * (L_ny + 2));
    local_rr_graph->rr_node_indices[SINK][i] = (t_ivec *) my_malloc(sizeof(t_ivec) * (L_ny + 2));
    for (j = 0; j <= (L_ny + 1); ++j) {
      local_rr_graph->rr_node_indices[IPIN][i][j].nelem = 0;
      local_rr_graph->rr_node_indices[IPIN][i][j].list = NULL;

      local_rr_graph->rr_node_indices[SINK][i][j].nelem = 0;
      local_rr_graph->rr_node_indices[SINK][i][j].list = NULL;
    }
  }

  /* Count indices for block nodes */
  for (i = 0; i <= (L_nx + 1); i++) {
    for (j = 0; j <= (L_ny + 1); j++) {
      ofs = L_grid[i][j].offset;
      if (0 == ofs) {
        type = L_grid[i][j].type;

        /* Load the pin class lookups. The ptc nums for SINK and SOURCE
         * are disjoint so they can share the list. */
        tmp.nelem = type->num_class;
        tmp.list = NULL;
        if (tmp.nelem > 0) {
          tmp.list = (int *) my_malloc(sizeof(int) * tmp.nelem);
          for (k = 0; k < tmp.nelem; ++k) {
            tmp.list[k] = *index;
            ++(*index);
          }
        }
        local_rr_graph->rr_node_indices[SINK][i][j] = tmp;

        /* Load the pin lookups. The ptc nums for IPIN and OPIN
         * are disjoint so they can share the list. */
        tmp.nelem = type->num_pins;
        tmp.list = NULL;
        if (tmp.nelem > 0) {
          tmp.list = (int *) my_malloc(sizeof(int) * tmp.nelem);
          for (k = 0; k < tmp.nelem; ++k) {
            tmp.list[k] = *index;
            ++(*index);
          }
        }
        local_rr_graph->rr_node_indices[IPIN][i][j] = tmp;
      }
    }
  }

  /* Point offset blocks of a large block to base block */
  for (i = 0; i <= (L_nx + 1); i++) {
    for (j = 0; j <= (L_ny + 1); j++) {
      ofs = L_grid[i][j].offset;
      if (ofs > 0) {
        /* NOTE: this only supports vertical large blocks */
        local_rr_graph->rr_node_indices[SINK][i][j] = local_rr_graph->rr_node_indices[SINK][i][j - ofs];
        local_rr_graph->rr_node_indices[IPIN][i][j] = local_rr_graph->rr_node_indices[IPIN][i][j - ofs];
      }
    }
  }

  /* SOURCE and SINK have unique ptc values so their data can be shared.
   * IPIN and OPIN have unique ptc values so their data can be shared. */
  local_rr_graph->rr_node_indices[SOURCE] = local_rr_graph->rr_node_indices[SINK];
  local_rr_graph->rr_node_indices[OPIN] = local_rr_graph->rr_node_indices[IPIN];

    /* Original VPR */
  /* Load the data for x and y channels */
  load_rr_graph_chan_rr_indices(local_rr_graph, nodes_per_chan, L_nx + 1, L_ny + 1, CHANX, seg_details,
                                index);
  load_rr_graph_chan_rr_indices(local_rr_graph, nodes_per_chan, L_ny + 1, L_nx + 1, CHANY, seg_details,
                                index);
    /* end */
    /* mrFPGA : Xifan TANG */
    /*
  load_rr_graph_chan_rr_indices(local_rr_graph, nodes_per_chan, (is_stack ? L_ny + 1 : L_nx + 1), (is_stack ? L_nx + 1 : L_ny + 1), 
                                CHANX, seg_details, index);
  load_rr_graph_chan_rr_indices(local_rr_graph, nodes_per_chan, (is_stack ? L_nx + 1 : L_ny + 1), (is_stack ? L_ny + 1 : L_nx + 1), 
                                CHANY, seg_details, index);
    */
    /* end */
  return;
}



void alloc_and_load_rr_graph_switch_inf(INOUTP t_rr_graph* local_rr_graph,
                                        int num_switch_inf,
                                        INP t_switch_inf* switch_inf) {
  local_rr_graph->num_switch_inf = num_switch_inf;
  /* Allocate memory */
  local_rr_graph->switch_inf = (t_switch_inf*) my_calloc(local_rr_graph->num_switch_inf, sizeof(t_switch_inf));
  /* Create a local copy */
  memcpy(local_rr_graph->switch_inf, switch_inf, sizeof(t_switch_inf));

  return;
}

/* Allocate a LL_rr_node route structs for a given rr_graph
 * This is function is a copy of alloc_and_load_rr_node_route_structs
 * The major difference lies in removing the use of global variables 
 */
void alloc_and_load_rr_graph_route_structs(t_rr_graph* local_rr_graph) {
  /* Allocates some extra information about each LL_rr_node that is used only   *
   * during routing.                                                         */

  int inode;

  local_rr_graph->rr_node_route_inf = (t_rr_node_route_inf *) my_malloc(local_rr_graph->num_rr_nodes * sizeof(t_rr_node_route_inf));

  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
   local_rr_graph->rr_node_route_inf[inode].prev_node = NO_PREVIOUS;
   local_rr_graph->rr_node_route_inf[inode].prev_edge = NO_PREVIOUS;
   local_rr_graph->rr_node_route_inf[inode].pres_cost = 1.;
   local_rr_graph->rr_node_route_inf[inode].acc_cost = 1.;
   local_rr_graph->rr_node_route_inf[inode].path_cost = HUGE_POSITIVE_FLOAT;
   local_rr_graph->rr_node_route_inf[inode].target_flag = 0;
  }

  return;
}

t_heap * get_rr_graph_heap_head(t_rr_graph* local_rr_graph) {

  /* Returns a pointer to the smallest element on the heap, or NULL if the     *
   * heap is empty.  Invalid (index == OPEN) entries on the heap are never     *
   * returned -- they are just skipped over.                                   */

  int ito, ifrom;
  t_heap *heap_head, *temp_ptr;

  do {
    if (local_rr_graph->heap_tail == 1) { /* Empty heap. */
      /*
      vpr_printf(TIO_MESSAGE_WARNING, "Empty heap occurred in get_heap_head.\n");
      vpr_printf(TIO_MESSAGE_WARNING, "Some blocks are impossible to connect in this architecture.\n");
      */
      return (NULL);
    }

    heap_head = local_rr_graph->heap[1]; /* Smallest element. */

    /* Now fix up the heap */

    local_rr_graph->heap_tail--;
    local_rr_graph->heap[1] = local_rr_graph->heap[local_rr_graph->heap_tail];
    ifrom = 1;
    ito = 2 * ifrom;

    while (ito < local_rr_graph->heap_tail) {
      if (local_rr_graph->heap[ito + 1]->cost < local_rr_graph->heap[ito]->cost)
        ito++;
      if (local_rr_graph->heap[ito]->cost > local_rr_graph->heap[ifrom]->cost)
        break;
      temp_ptr = local_rr_graph->heap[ito];
      local_rr_graph->heap[ito] = local_rr_graph->heap[ifrom];
      local_rr_graph->heap[ifrom] = temp_ptr;
      ifrom = ito;
      ito = 2 * ifrom;
    }

  } while (heap_head->index == OPEN); /* Get another one if invalid entry. */

  return (heap_head);
}

t_linked_f_pointer* alloc_rr_graph_linked_f_pointer(t_rr_graph* local_rr_graph) {

  /* This routine returns a linked list element with a float pointer as *
   * the node data.                                                     */

  /*int i;*/
  t_linked_f_pointer *temp_ptr;

  if (local_rr_graph->linked_f_pointer_free_head == NULL) {
    /* No elements on the free list */  
    local_rr_graph->linked_f_pointer_free_head = (t_linked_f_pointer *) my_chunk_malloc(sizeof(t_linked_f_pointer), &local_rr_graph->linked_f_pointer_ch);
    local_rr_graph->linked_f_pointer_free_head->next = NULL;
  }

  temp_ptr = local_rr_graph->linked_f_pointer_free_head;
  local_rr_graph->linked_f_pointer_free_head = local_rr_graph->linked_f_pointer_free_head->next;

#ifdef DEBUG
  local_rr_graph->num_linked_f_pointer_allocated++;
#endif

  return (temp_ptr);
}


void add_to_rr_graph_mod_list(t_rr_graph* local_rr_graph,
                              float *fptr) {

  /* This routine adds the floating point pointer (fptr) into a  *
   * linked list that indicates all the pathcosts that have been *
   * modified thus far.                                          */

  t_linked_f_pointer *mod_ptr;

  mod_ptr = alloc_rr_graph_linked_f_pointer(local_rr_graph);

  /* Add this element to the start of the modified list. */

  mod_ptr->next = local_rr_graph->rr_modified_head;
  mod_ptr->fptr = fptr;
  local_rr_graph->rr_modified_head = mod_ptr;
}

void free_rr_graph_heap_data(t_rr_graph* local_rr_graph,
                             t_heap *hptr) {

  hptr->u.next = local_rr_graph->heap_free_head;
  local_rr_graph->heap_free_head = hptr;
#ifdef DEBUG
  local_rr_graph->num_heap_allocated--;
#endif
}

t_trace* alloc_rr_graph_trace_data(t_rr_graph* local_rr_graph) {

  t_trace *temp_ptr;

  if (local_rr_graph->trace_free_head == NULL) { /* No elements on the free list */
    local_rr_graph->trace_free_head = (t_trace *) my_chunk_malloc(sizeof(t_trace), &local_rr_graph->trace_ch);
    local_rr_graph->trace_free_head->next = NULL;
  }
  temp_ptr = local_rr_graph->trace_free_head;
  local_rr_graph->trace_free_head = local_rr_graph->trace_free_head->next;
#ifdef DEBUG
  local_rr_graph->num_trace_allocated++;
#endif
  return (temp_ptr);
}

void empty_rr_graph_heap(t_rr_graph* local_rr_graph) {

  int i;

  for (i = 1; i < local_rr_graph->heap_tail; i++)
    free_rr_graph_heap_data(local_rr_graph, local_rr_graph->heap[i]);

  local_rr_graph->heap_tail = 1;

  return;
}

void reset_rr_graph_rr_node_route_structs(t_rr_graph* local_rr_graph) {

  /* Allocates some extra information about each rr_node that is used only   *
   * during routing.                                                         */

  int inode;

  assert(local_rr_graph->rr_node_route_inf != NULL);

  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    local_rr_graph->rr_node_route_inf[inode].prev_node = NO_PREVIOUS;
    local_rr_graph->rr_node_route_inf[inode].prev_edge = NO_PREVIOUS;
    local_rr_graph->rr_node_route_inf[inode].pres_cost = 1.;
    local_rr_graph->rr_node_route_inf[inode].acc_cost = 1.;
    local_rr_graph->rr_node_route_inf[inode].path_cost = HUGE_POSITIVE_FLOAT;
    local_rr_graph->rr_node_route_inf[inode].target_flag = 0;
  }

  return;
}


t_trace* update_rr_graph_traceback(t_rr_graph* local_rr_graph,
                                   t_heap *hptr, int inet) {

  /* This routine adds the most recently finished wire segment to the         *
   * traceback linked list.  The first connection starts with the net SOURCE  *
   * and begins at the structure pointed to by trace_head[inet]. Each         *
   * connection ends with a SINK.  After each SINK, the next connection       *
   * begins (if the net has more than 2 pins).  The first element after the   *
   * SINK gives the routing node on a previous piece of the routing, which is *
   * the link from the existing net to this new piece of the net.             *
   * In each traceback I start at the end of a path and trace back through    *
   * its predecessors to the beginning.  I have stored information on the     *
   * predecesser of each node to make traceback easy -- this sacrificies some *
   * memory for easier code maintenance.  This routine returns a pointer to   *
   * the first "new" node in the traceback (node not previously in trace).    */

  struct s_trace *tptr, *prevptr, *temptail, *ret_ptr;
  int inode;
  short iedge;

#ifdef DEBUG
  t_rr_type rr_type;
#endif

  inode = hptr->index;

#ifdef DEBUG
  rr_type = local_rr_graph->rr_node[inode].type;
  if (rr_type != SINK) {
    vpr_printf(TIO_MESSAGE_ERROR, "in update_traceback. Expected type = SINK (%d).\n", SINK);
    vpr_printf(TIO_MESSAGE_ERROR, "\tGot type = %d while tracing back net %d.\n", rr_type, inet);
    exit(1);
  }
#endif

  tptr = alloc_rr_graph_trace_data(local_rr_graph); /* SINK on the end of the connection */
  tptr->index = inode;
  tptr->iswitch = OPEN;
  tptr->next = NULL;
  temptail = tptr; /* This will become the new tail at the end */
  /* of the routine.                          */

  /* Now do it's predecessor. */

  inode = hptr->u.prev_node;
  iedge = hptr->prev_edge;

  while (inode != NO_PREVIOUS) {
    prevptr = alloc_rr_graph_trace_data(local_rr_graph);
    prevptr->index = inode;
    prevptr->iswitch = local_rr_graph->rr_node[inode].switches[iedge];
    prevptr->next = tptr;
    tptr = prevptr;

    iedge = local_rr_graph->rr_node_route_inf[inode].prev_edge;
    inode = local_rr_graph->rr_node_route_inf[inode].prev_node;
  }

  if (local_rr_graph->trace_tail[inet] != NULL) {
    local_rr_graph->trace_tail[inet]->next = tptr; /* Traceback ends with tptr */
    ret_ptr = tptr->next; /* First new segment.       */
  } else { /* This was the first "chunk" of the net's routing */
    local_rr_graph->trace_head[inet] = tptr;
    ret_ptr = tptr; /* Whole traceback is new. */
  }

  local_rr_graph->trace_tail[inet] = temptail;
  return (ret_ptr);
}


void reset_rr_graph_path_costs(t_rr_graph* local_rr_graph) {

  /* The routine sets the path_cost to HUGE_POSITIVE_FLOAT for all channel segments   *
   * touched by previous routing phases.                                     */

  t_linked_f_pointer *mod_ptr;

#ifdef DEBUG
  int num_mod_ptrs;
#endif

  /* The traversal method below is slightly painful to make it faster. */

  if (local_rr_graph->rr_modified_head != NULL) {
    mod_ptr = local_rr_graph->rr_modified_head;

#ifdef DEBUG
    num_mod_ptrs = 1;
#endif

    while (mod_ptr->next != NULL) {
      *(mod_ptr->fptr) = HUGE_POSITIVE_FLOAT;
      mod_ptr = mod_ptr->next;
#ifdef DEBUG
      num_mod_ptrs++;
#endif
    }
    *(mod_ptr->fptr) = HUGE_POSITIVE_FLOAT; /* Do last one. */

    /* Reset the modified list and put all the elements back in the free   *
     * list.                                                               */

    mod_ptr->next = local_rr_graph->linked_f_pointer_free_head;
    local_rr_graph->linked_f_pointer_free_head = local_rr_graph->rr_modified_head;
    local_rr_graph->rr_modified_head = NULL;

#ifdef DEBUG
    local_rr_graph->num_linked_f_pointer_allocated -= num_mod_ptrs;
#endif
  }

  return;
}

void alloc_rr_graph_rr_indexed_data(t_rr_graph* local_rr_graph, int L_num_rr_indexed_data) {
  local_rr_graph->num_rr_indexed_data = L_num_rr_indexed_data;
  local_rr_graph->rr_indexed_data = (t_rr_indexed_data *) my_calloc(L_num_rr_indexed_data, sizeof(t_rr_indexed_data));

  return;
}

/* a copy of get_rr_cong_cost, 
 * I remove all the use of global variables */
float get_rr_graph_rr_cong_cost(t_rr_graph* local_rr_graph,
                                int rr_node_index) {

  /* Returns the *congestion* cost of using this rr_node. */

  short cost_index;
  float cost;

  cost_index = local_rr_graph->rr_node[rr_node_index].cost_index;
  cost = local_rr_graph->rr_indexed_data[cost_index].base_cost
      * local_rr_graph->rr_node_route_inf[rr_node_index].acc_cost
      * local_rr_graph->rr_node_route_inf[rr_node_index].pres_cost;
  return (cost);
}

t_heap * alloc_rr_graph_heap_data(t_rr_graph* local_rr_graph) {

  t_heap *temp_ptr;

  if (local_rr_graph->heap_free_head == NULL) { /* No elements on the free list */
    local_rr_graph->heap_free_head = (t_heap *) my_chunk_malloc(sizeof(t_heap), &(local_rr_graph->heap_ch));
    local_rr_graph->heap_free_head->u.next = NULL;
  }

  temp_ptr = local_rr_graph->heap_free_head;
  local_rr_graph->heap_free_head = local_rr_graph->heap_free_head->u.next;
#ifdef DEBUG
  local_rr_graph->num_heap_allocated++;
#endif
  return (temp_ptr);
}

void add_heap_node_to_rr_graph_heap(t_rr_graph* local_rr_graph,
                                    t_heap *hptr) {

  /* Adds an item to the heap, expanding the heap if necessary.             */

  int ito, ifrom;
  t_heap *temp_ptr;

  if (local_rr_graph->heap_tail > local_rr_graph->heap_size) { /* Heap is full */
    local_rr_graph->heap_size *= 2;
    local_rr_graph->heap = (t_heap **) my_realloc((void *) (local_rr_graph->heap + 1),
        local_rr_graph->heap_size * sizeof(t_heap *));
    local_rr_graph->heap--; /* heap goes from [1..heap_size] */
  }

  local_rr_graph->heap[local_rr_graph->heap_tail] = hptr;
  ifrom = local_rr_graph->heap_tail;
  ito = ifrom / 2;
  local_rr_graph->heap_tail++;

  while ((ito >= 1) && (local_rr_graph->heap[ifrom]->cost < local_rr_graph->heap[ito]->cost)) {
    temp_ptr = local_rr_graph->heap[ito];
    local_rr_graph->heap[ito] = local_rr_graph->heap[ifrom];
    local_rr_graph->heap[ifrom] = temp_ptr;
    ifrom = ito;
    ito = ifrom / 2;
  }
  return;
}


void add_node_to_rr_graph_heap(t_rr_graph* local_rr_graph,
                               int inode, float cost, int prev_node, int prev_edge,
                               float backward_path_cost, float R_upstream) {

  /* Puts an rr_node on the heap, if the new cost given is lower than the     *
   * current path_cost to this channel segment.  The index of its predecessor *
   * is stored to make traceback easy.  The index of the edge used to get     *
   * from its predecessor to it is also stored to make timing analysis, etc.  *
   * easy.  The backward_path_cost and R_upstream values are used only by the *
   * timing-driven router -- the breadth-first router ignores them.           */

  struct s_heap *hptr;

  if (cost >= local_rr_graph->rr_node_route_inf[inode].path_cost) {
    return;
  }

  hptr = alloc_rr_graph_heap_data(local_rr_graph);
  hptr->index = inode;
  hptr->cost = cost;
  hptr->u.prev_node = prev_node;
  hptr->prev_edge = prev_edge;
  hptr->backward_path_cost = backward_path_cost;
  hptr->R_upstream = R_upstream;
  add_heap_node_to_rr_graph_heap(local_rr_graph, hptr);

  return;
}

void mark_rr_graph_sinks(t_rr_graph* local_rr_graph, 
                         int inet, int start_isink, boolean* net_sink_routed) {

  /* Mark all the SINKs of this net as targets by setting their target flags  *
   * to the number of times the net must connect to each SINK.  Note that     *
   * this number can occassionally be greater than 1 -- think of connecting   *
   * the same net to two inputs of an and-gate (and-gate inputs are logically *
   * equivalent, so both will connect to the same SINK).                      */

  int isink, inode;

  for (isink = start_isink; isink < local_rr_graph->net_num_sinks[inet]; isink++) {
    /* Bypass routed sinks */
    if (TRUE == net_sink_routed[isink]) {
      continue;
    }
    inode = local_rr_graph->net_rr_sinks[inet][isink];
    if (OPEN == inode) {
      continue;
    }
    local_rr_graph->rr_node_route_inf[inode].target_flag++;
    if ( ! ((local_rr_graph->rr_node_route_inf[inode].target_flag > 0) 
        && (local_rr_graph->rr_node_route_inf[inode].target_flag <= local_rr_graph->rr_node[inode].capacity))) {
    assert((local_rr_graph->rr_node_route_inf[inode].target_flag > 0) 
        && (local_rr_graph->rr_node_route_inf[inode].target_flag <= local_rr_graph->rr_node[inode].capacity));
    }
  }

  return;
}

void mark_rr_graph_ends(t_rr_graph* local_rr_graph, 
                        int inet) {

  /* Mark all the SINKs of this net as targets by setting their target flags  *
   * to the number of times the net must connect to each SINK.  Note that     *
   * this number can occassionally be greater than 1 -- think of connecting   *
   * the same net to two inputs of an and-gate (and-gate inputs are logically *
   * equivalent, so both will connect to the same SINK).                      */

  int ipin, inode;

  for (ipin = 1; ipin < local_rr_graph->net_num_sinks[inet] + 1; ipin++) {
    inode = local_rr_graph->net_rr_terminals[inet][ipin];
    if (inode == OPEN) {
      continue;
    }
    local_rr_graph->rr_node_route_inf[inode].target_flag++;
    assert((local_rr_graph->rr_node_route_inf[inode].target_flag > 0) 
        && (local_rr_graph->rr_node_route_inf[inode].target_flag <= local_rr_graph->rr_node[inode].capacity));
  }

  return;
}

void invalidate_rr_graph_heap_entries(t_rr_graph* local_rr_graph, 
                                      int sink_node, int ipin_node) {

  /* Marks all the heap entries consisting of sink_node, where it was reached *
   * via ipin_node, as invalid (OPEN).  Used only by the breadth_first router *
   * and even then only in rare circumstances.                                */

  int i;

  for (i = 1; i < local_rr_graph->heap_tail; i++) {
    if ((local_rr_graph->heap[i]->index == sink_node) 
        && (local_rr_graph->heap[i]->u.prev_node == ipin_node)) {
      local_rr_graph->heap[i]->index = OPEN; /* Invalid. */
    }
  }

  return;
}

float get_rr_graph_rr_node_pack_intrinsic_cost(t_rr_graph* local_rr_graph,
                                               int inode) {
  /* This is a tie breaker to avoid using nodes with more edges whenever possible */
  float value;
  value = local_rr_graph->rr_node[inode].pack_intrinsic_cost;
  return value;
}

/* Free rr_graph data structs */
void free_rr_graph_rr_nodes(t_rr_graph* local_rr_graph) {
  int i;
  
  /* Free edges and switches of all the rr_nodes */
  for (i = 0; i < local_rr_graph->num_rr_nodes; i++) {
    my_free(local_rr_graph->rr_node[i].edges);
    my_free(local_rr_graph->rr_node[i].switches);
    my_free(local_rr_graph->rr_node[i].drive_rr_nodes);
  }
  /* Free the rr_node list */
  my_free(local_rr_graph->rr_node);

  return;
}

void free_rr_graph_switch_inf(INOUTP t_rr_graph* local_rr_graph) {

  my_free(local_rr_graph->switch_inf);

  return;
}

void free_rr_graph_route_structs(t_rr_graph* local_rr_graph) { /* [0..num_rr_nodes-1] */

  /* Frees the extra information about each LL_rr_node that is needed only      *
   * during routing.                                                         */

  free(local_rr_graph->rr_node_route_inf);
  local_rr_graph->rr_node_route_inf = NULL; /* Mark as free */

  return;
}

void free_rr_graph_trace_data(t_rr_graph* local_rr_graph,
                              t_trace *tptr) {

  /* Puts the traceback structure pointed to by tptr on the free list. */

  tptr->next = local_rr_graph->trace_free_head;
  local_rr_graph->trace_free_head = tptr;
#ifdef DEBUG
  local_rr_graph->num_trace_allocated--;
#endif
}

void free_rr_graph_traceback(t_rr_graph* local_rr_graph, 
                             int inet) {

  /* Puts the entire traceback (old routing) for this net on the free list *
   * and sets the trace_head pointers etc. for the net to NULL.            */

  t_trace *tptr, *tempptr;

  if( local_rr_graph->trace_head == NULL) {
    return;
  }

  tptr = local_rr_graph->trace_head[inet];

  while (tptr != NULL) {
    tempptr = tptr->next;
    free_rr_graph_trace_data(local_rr_graph, tptr);
    tptr = tempptr;
  }

  local_rr_graph->trace_head[inet] = NULL;
  local_rr_graph->trace_tail[inet] = NULL;
}

/* TODO: Fully free a rr_graph data struct */
void free_rr_graph(t_rr_graph* local_rr_graph) {
  /* Free the internal data structs one by one */
  free_rr_graph_rr_nodes(local_rr_graph);
  free_rr_graph_switch_inf(local_rr_graph);
  free_rr_graph_route_structs(local_rr_graph);

  return;
}

void build_prev_node_list_rr_nodes(int LL_num_rr_nodes,
                                   t_rr_node* LL_rr_node) {
  int inode, iedge, to_node, cur;
  int* cur_index = (int*)my_malloc(sizeof(int)*LL_num_rr_nodes);
  
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    /* Malloc */
    LL_rr_node[inode].num_drive_rr_nodes = LL_rr_node[inode].fan_in;
    if (0 == LL_rr_node[inode].fan_in) {
     continue;
    }
    LL_rr_node[inode].drive_rr_nodes = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*LL_rr_node[inode].num_drive_rr_nodes);
    LL_rr_node[inode].drive_switches = (int*)my_malloc(sizeof(int)*LL_rr_node[inode].num_drive_rr_nodes);
  }
  /* Initialize */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    cur_index[inode] = 0;
    for (iedge = 0; iedge < LL_rr_node[inode].num_drive_rr_nodes; iedge++) {
      LL_rr_node[inode].drive_rr_nodes[iedge] = NULL;
      LL_rr_node[inode].drive_switches[iedge] = -1;
    }
  }
  /* Fill */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    for (iedge = 0; iedge < LL_rr_node[inode].num_edges; iedge++) {
      to_node = LL_rr_node[inode].edges[iedge]; 
      cur = cur_index[to_node];
      LL_rr_node[to_node].drive_rr_nodes[cur] = &(LL_rr_node[inode]);
      LL_rr_node[to_node].drive_switches[cur] = LL_rr_node[inode].switches[iedge];
      /* Update cur_index[to_node]*/
      assert(NULL != LL_rr_node[to_node].drive_rr_nodes[cur]);
      cur_index[to_node]++;
    }
  }
  /* Check */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    assert(cur_index[inode] == LL_rr_node[inode].num_drive_rr_nodes);
  }

  return;
}

void alloc_and_load_prev_node_list_rr_graph_rr_nodes(t_rr_graph* local_rr_graph) {
  build_prev_node_list_rr_nodes(local_rr_graph->num_rr_nodes, local_rr_graph->rr_node);

  return;
}

void backannotate_rr_graph_routing_results_to_net_name(t_rr_graph* local_rr_graph) {
  int inode, inet;
  int next_node, iedge;
  t_trace* tptr;
  t_rr_type rr_type;

  /* 1st step: Set all the configurations to default.
   * rr_nodes select edge[0]
   */
  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    local_rr_graph->rr_node[inode].prev_node = OPEN;
    /* set 0 if we want print all unused mux!!!*/
    local_rr_graph->rr_node[inode].prev_edge = OPEN;
    /* Initial all the net_num*/
    local_rr_graph->rr_node[inode].net_num = OPEN;
    local_rr_graph->rr_node[inode].vpack_net_num = OPEN;
  }
  /*
  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    if (0 == local_rr_graph->rr_node[inode].num_edges) {
      continue;
    }  
    assert(0 < local_rr_graph->rr_node[inode].num_edges);
    for (iedge = 0; iedge < local_rr_graph->rr_node[inode].num_edges; iedge++) {
      jnode = local_rr_graph->rr_node[inode].edges[iedge];
      if (&(local_rr_graph->rr_node[inode]) == local_rr_graph->rr_node[jnode].drive_rr_nodes[0]) {
        local_rr_graph->rr_node[jnode].prev_node = inode;
        local_rr_graph->rr_node[jnode].prev_edge = iedge;
      }
    }
  }
  */
  /* 2nd step: With the help of trace, we back-annotate */
  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    /*
    if (TRUE == local_rr_graph->net[inet]->is_global) {
      continue;
    }
    */
    tptr = local_rr_graph->trace_head[inet];
    while (tptr != NULL) {
      inode = tptr->index;
      rr_type = local_rr_graph->rr_node[inode].type;
      /* Net num */
      local_rr_graph->rr_node[inode].net_num = inet;
      local_rr_graph->rr_node[inode].vpack_net_num = local_rr_graph->net_to_vpack_net_mapping[inet];
      /* assert(OPEN != local_rr_graph->rr_node[inode].net_num); */
      assert(OPEN != local_rr_graph->rr_node[inode].vpack_net_num);
      switch (rr_type) {
      case SINK: 
        /* Nothing should be done. This supposed to the end of a trace*/
        break;
      case IPIN: 
      case CHANX: 
      case CHANY: 
      case OPIN: 
      case INTRA_CLUSTER_EDGE: 
      case SOURCE: 
        /* SINK(IO/Pad) is the end of a routing path. Should configure its prev_edge and prev_node*/
        /* We care the next rr_node, this one is driving, which we have to configure 
         */
        assert(NULL != tptr->next);
        next_node = tptr->next->index;
        assert((!(0 > next_node))&&(next_node < local_rr_graph->num_rr_nodes));
        /* Prev_node */
        local_rr_graph->rr_node[next_node].prev_node = inode;
        /* Prev_edge */
        local_rr_graph->rr_node[next_node].prev_edge = OPEN;
        for (iedge = 0; iedge < local_rr_graph->rr_node[inode].num_edges; iedge++) {
          if (next_node == local_rr_graph->rr_node[inode].edges[iedge]) {
            local_rr_graph->rr_node[next_node].prev_edge = iedge;
            break;
          }
        }
        assert(OPEN != local_rr_graph->rr_node[next_node].prev_edge);
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid traceback element type.\n",
                   __FILE__, __LINE__);
        exit(1);
      }
      tptr = tptr->next;
    }
  }

  return;
}

int get_rr_graph_net_vpack_net_index(t_rr_graph* local_rr_graph,
                                     int net_index) {
  return local_rr_graph->net_to_vpack_net_mapping[net_index];
}

int get_rr_graph_net_index_with_vpack_net(t_rr_graph* local_rr_graph,
                                          int vpack_net_index) {
  int inet, ret;
  int num_found = 0;

  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    if (vpack_net_index == local_rr_graph->net_to_vpack_net_mapping[inet]) {
      num_found = 0;
      ret = inet;
    }
  }
  /* assert */
  assert ((0 == num_found ) || (1 == num_found));

  return ret;
}

void get_chan_rr_node_start_coordinate(t_rr_node* chan_rr_node,
                                       int* x_start, int* y_start) {
  assert ( (CHANX == chan_rr_node->type)
         ||(CHANY == chan_rr_node->type));

  switch (chan_rr_node->direction) {
  case INC_DIRECTION:
    (*x_start) = chan_rr_node->xlow;
    (*y_start) = chan_rr_node->ylow;
    break;
  case DEC_DIRECTION:
    (*x_start) = chan_rr_node->xhigh;
    (*y_start) = chan_rr_node->yhigh;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File: %s [LINE%d]) Invalid direction of chan_rr_node!\n",
               __FILE__, __LINE__);
 
    exit(1);
  }

  return;
}

void get_chan_rr_node_end_coordinate(t_rr_node* chan_rr_node,
                                     int* x_end, int* y_end) {
  assert ( (CHANX == chan_rr_node->type)
         ||(CHANY == chan_rr_node->type));

  switch (chan_rr_node->direction) {
  case INC_DIRECTION:
    (*x_end) = chan_rr_node->xhigh;
    (*y_end) = chan_rr_node->yhigh;
    break;
  case DEC_DIRECTION:
    (*x_end) = chan_rr_node->xlow;
    (*y_end) = chan_rr_node->ylow;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File: %s [LINE%d]) Invalid direction of chan_rr_node!\n",
               __FILE__, __LINE__);
 
    exit(1);
  }

  return;
}

int get_rr_node_wire_length(t_rr_node* src_rr_node) {
  assert ( (CHANX == src_rr_node->type)
        || (CHANY == src_rr_node->type));

  return (abs(src_rr_node->xlow - src_rr_node->xhigh + src_rr_node->ylow - src_rr_node->yhigh) + 1);
}
