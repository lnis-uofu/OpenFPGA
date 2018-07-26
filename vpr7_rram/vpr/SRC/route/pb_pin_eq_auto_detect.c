/* Xifan TANG: Auto detect the pin equivalence of type_descriptor
 * by re-construct the class of pins in type_descriptor
 */
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "util.h"
#include "vpr_types.h"
#include "physical_types.h"
#include "globals.h"
#include "vpr_utils.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "linkedlist.h"

typedef struct s_clb_to_clb_directs {
	t_type_descriptor *from_clb_type;
	int from_clb_pin_start_index;
	int from_clb_pin_end_index;
	t_type_descriptor *to_clb_type;
	int to_clb_pin_start_index;
	int to_clb_pin_end_index;
} t_clb_to_clb_directs;

typedef struct s_num_mapped_opins_stats t_num_mapped_opins_stats;
struct s_num_mapped_opins_stats{
  int num_mapped_opins;
  int net_cnt;
};


/***** Subroutines *****/

boolean is_opin_in_direct_list(t_type_ptr cur_type_descriptor,
                               int opin_index,
                               int num_directs,
	                           t_clb_to_clb_directs *clb_to_clb_directs) {
  int idirect = 0;
   
  /* Corner case 1: no directs, return false */
  if (0 == num_directs) {
    assert(NULL == clb_to_clb_directs);
    return FALSE;
  }

  for (idirect = 0; idirect < num_directs; idirect++) {
    if ((cur_type_descriptor == clb_to_clb_directs[idirect].from_clb_type) 
       && ((clb_to_clb_directs[idirect].from_clb_pin_start_index < opin_index)||(clb_to_clb_directs[idirect].from_clb_pin_start_index == opin_index)) 
       && ((opin_index < clb_to_clb_directs[idirect].from_clb_pin_end_index)||(opin_index == clb_to_clb_directs[idirect].from_clb_pin_end_index))) {
       return TRUE;
    }
  }

  return FALSE;
}
                               

boolean is_des_rr_node_in_src_rr_node_edges(t_rr_node* src_rr_node,
                                            t_rr_node* des_rr_node) {
  int iedge;
  boolean found = FALSE;

  for (iedge = 0; iedge < src_rr_node->num_edges; iedge++) {
    if (des_rr_node == &(rr_node[src_rr_node->edges[iedge]])) {
      found = TRUE;
      break;
    } 
  }

  return found;
}

int alloc_and_add_fully_capacity_rr_edges_to_source_opin(t_type_ptr cur_type_descriptor,
                                                         int num_source_rr_node, 
                                                         t_rr_node** source_rr_node, 
                                                         t_rr_node** opin_rr_node) {
  int ret = 0;
  int isrc, iopin, offset, iedge;

  for (isrc = 0; isrc < num_source_rr_node; isrc++) {
    source_rr_node[isrc]->capacity = 1;
    source_rr_node[isrc]->num_edges = 0;
  }

  /* We only do for two parts, first 0... num/2 and num/2 + 1 ... end */
  /* First part */
  for (iopin = 0; iopin < num_source_rr_node / 2; iopin++) {
    offset = iopin;
    /* Increase fan_in for opin_rr_node[iopin] */ 
    opin_rr_node[iopin]->fan_in = num_source_rr_node / 2 + 1; 
    /* Add rr_edges one by one */
    for (isrc = offset; isrc < offset + num_source_rr_node / 2 + 1; isrc++) {
      /* Add rr_edges from source_rr_node[isrc] to  opin_rr_node[iopin] */
      /* Make sure the to_node is not already in the child edges */
      if (TRUE == is_des_rr_node_in_src_rr_node_edges(source_rr_node[isrc], opin_rr_node[iopin])) {
        continue;
      } 
       /* Update fan_in for all the child rr_nodes */
      /* Increase num_edges for source_rr_node[isrc] */ 
      source_rr_node[isrc]->num_edges++;
      source_rr_node[isrc]->edges = (int*)my_realloc(source_rr_node[isrc]->edges, source_rr_node[isrc]->num_edges * sizeof(int));
      source_rr_node[isrc]->switches = (short*)my_realloc(source_rr_node[isrc]->switches, source_rr_node[isrc]->num_edges * sizeof(short));
      source_rr_node[isrc]->edges[source_rr_node[isrc]->num_edges - 1] = opin_rr_node[iopin] - rr_node;
      /* Add switches for src_rr_node */
      /* TODO: to be smarter */
      source_rr_node[isrc]->switches[source_rr_node[isrc]->num_edges - 1] = source_rr_node[isrc]->switches[0];
      /* Update capacity for src_rr_node */
      //source_rr_node[isrc]->capacity++;
      /* Update counter */
      ret++;
    } 
  }  

  /* Second part */
  for (iopin = num_source_rr_node/2; iopin < num_source_rr_node; iopin++) {
    offset = iopin - num_source_rr_node / 2;
    /* Increase fan_in for opin_rr_node[iopin] */ 
    opin_rr_node[iopin]->fan_in = num_source_rr_node / 2 + 1; 
    /* Add rr_edges one by one */
    for (isrc = offset; isrc < offset + num_source_rr_node / 2 + 1; isrc++) {
      /* Add rr_edges from source_rr_node[isrc] to  opin_rr_node[iopin] */
      /* Make sure the to_node is not already in the child edges */
      if (TRUE == is_des_rr_node_in_src_rr_node_edges(source_rr_node[isrc], opin_rr_node[iopin])) {
        continue;
      } 
      /* Update fan_in for all the child rr_nodes */
      /* Increase num_edges for source_rr_node[isrc] */ 
      source_rr_node[isrc]->num_edges++;
      source_rr_node[isrc]->edges = (int*)my_realloc(source_rr_node[isrc]->edges, source_rr_node[isrc]->num_edges * sizeof(int));
      source_rr_node[isrc]->switches = (short*)my_realloc(source_rr_node[isrc]->switches, source_rr_node[isrc]->num_edges * sizeof(short));
      source_rr_node[isrc]->edges[source_rr_node[isrc]->num_edges - 1] = opin_rr_node[iopin] - rr_node;
      /* Add switches for src_rr_node */
      /* TODO: to be smarter */
      source_rr_node[isrc]->switches[source_rr_node[isrc]->num_edges - 1] = source_rr_node[isrc]->switches[0];
      /* Update capacity for src_rr_node */
      //source_rr_node[isrc]->capacity++;
      /* Update counter */
      ret++;
    } 
  }  
  
  /* Print debug information */
  /*
  vpr_printf(TIO_MESSAGE_INFO, "OPIN node:"); 
  for (iopin = 0; iopin < num_source_rr_node; iopin++) {
    vpr_printf(TIO_MESSAGE_INFO, "%d,", opin_rr_node[iopin] - rr_node); 
  }
  vpr_printf(TIO_MESSAGE_INFO, "\n"); 
  for (isrc = 0; isrc < num_source_rr_node; isrc++) {
    vpr_printf(TIO_MESSAGE_INFO, "Source node: index=%d, num_edges=%d.\n", source_rr_node[isrc] - rr_node, source_rr_node[isrc]->num_edges); 
    vpr_printf(TIO_MESSAGE_INFO, "Edges:"); 
    for (iedge = 0; iedge < source_rr_node[isrc]->num_edges; iedge++) { 
      vpr_printf(TIO_MESSAGE_INFO, "%d,", source_rr_node[isrc]->edges[iedge]); 
    }
    vpr_printf(TIO_MESSAGE_INFO, "\n"); 
  }
  */

  return ret;
}

/* Build arrays for all the SOURCE rr_nodes, OPIN rr_nodes of a grid
 * Also build arrays for all the OUT_PORT pb_graph_pins corresponding to SINK and OPIN 
 */
int alloc_and_add_fully_capacity_rr_edges_to_one_grid(int grid_x, int grid_y, int grid_z,
                                                      t_block* cur_block, 
                                                      t_ivec*** LL_rr_node_indices,
                                                      int num_directs,
	                                                  t_clb_to_clb_directs *clb_to_clb_directs) {
  int ret = 0;
  int ipin, iclass, offset, inode; 
  t_type_ptr grid_type_descriptor = NULL;
  t_pb_graph_node* top_pb_graph_node = NULL;
  int num_opin_rr_nodes = 0;
  int num_source_rr_nodes = 0;
  t_rr_node** opin_rr_node = NULL;
  t_rr_node** source_rr_node = NULL;

  /* Check */
  assert((!(0 > grid_x))&&(!(grid_x > (nx + 1)))); 
  assert((!(0 > grid_y))&&(!(grid_y > (ny + 1)))); 

  /* Get grid type descriptor */
  grid_type_descriptor = grid[grid_x][grid_y].type;
  top_pb_graph_node = grid[grid_x][grid_y].type->pb_graph_head;

  /* Check */
  assert(NULL != grid_type_descriptor);
  assert(NULL != top_pb_graph_node);
  
  /* Search all the OPIN rr_nodes */
  for (ipin = 0; ipin < grid_type_descriptor->num_pins; ipin++) {
    iclass = grid_type_descriptor->pin_class[ipin];
    /* Skip IPINs */
    if (DRIVER != grid_type_descriptor->class_inf[iclass].type) {
      continue;
    }
    /* Skip direct pins */
    if (TRUE == is_opin_in_direct_list(grid_type_descriptor, ipin, num_directs, clb_to_clb_directs)) {
      continue;
    }
    /* Get SOURCE rr_nodes */
    inode = get_rr_node_index(grid_x, grid_y, OPIN, 
                              ipin, LL_rr_node_indices);
    /* Check */
    assert((-1 < inode)&&(inode < num_rr_nodes));
    /* Update counter */
    num_opin_rr_nodes++;
  }

  for (iclass = 0; iclass < grid_type_descriptor->num_class; iclass++) {
    /* Skip IPINs */
    if (DRIVER != grid_type_descriptor->class_inf[iclass].type) {
      continue;
    }
    /* Get SOURCE rr_node */
    inode = get_rr_node_index(grid_x, grid_y, SOURCE, 
                              iclass, LL_rr_node_indices);
    /* Check */
    assert((-1 < inode)&&(inode < num_rr_nodes));  
    if (1 < rr_node[inode].num_edges) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])When output pin logic equivalence auto-detect is turned on.\nLogic equivalence of outputs should not be defined in arch. XML (type name: %s).\n",
                 __FILE__, __LINE__, grid_type_descriptor->name);
      exit(1);
    }
    /* Skip direct pins */
    if (TRUE == is_opin_in_direct_list(grid_type_descriptor, rr_node[rr_node[inode].edges[0]].ptc_num, num_directs, clb_to_clb_directs)) {
      continue;
    }
    /* Update counter */
    num_source_rr_nodes++;
  }

  /* Check ? num_opin_rr_nodes == num_source_rr_nodes */
  if (num_opin_rr_nodes != num_source_rr_nodes) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])When output pin logic equivalence auto-detect is turned on.\nLogic equivalence of outputs should not be defined in arch. XML (type name: %s).\n",
               __FILE__, __LINE__, grid_type_descriptor->name);
    exit(1);
  }

  /* Allocate */
  opin_rr_node = (t_rr_node**)my_malloc(num_opin_rr_nodes * sizeof(t_rr_node*)); 
  source_rr_node = (t_rr_node**)my_malloc(num_source_rr_nodes * sizeof(t_rr_node*)); 

  /* Fill the array */
  offset = 0;
  for (ipin = 0; ipin < grid_type_descriptor->num_pins; ipin++) {
    iclass = grid_type_descriptor->pin_class[ipin];
    if (DRIVER != grid_type_descriptor->class_inf[iclass].type) {
      continue;
    }
    /* Skip direct pins */
    if (TRUE == is_opin_in_direct_list(grid_type_descriptor, ipin, num_directs, clb_to_clb_directs)) {
      continue;
    }
    /* Get SOURCE rr_nodes */
    inode = get_rr_node_index(grid_x, grid_y, OPIN, 
                              ipin, LL_rr_node_indices);
    /* Check */
    assert((-1 < inode)&&(inode < num_rr_nodes));
    assert(1 == rr_node[inode].fan_in);
    /* Fill opin_rr_node array */
    opin_rr_node[offset] = &(rr_node[inode]);
    offset++;
  }
  assert(offset == num_opin_rr_nodes);

  /* Fill the array */
  offset = 0;
  for (iclass = 0; iclass < grid_type_descriptor->num_class; iclass++) {
    if (DRIVER != grid_type_descriptor->class_inf[iclass].type) {
      continue;
    }
    /* Get SOURCE rr_node */
    inode = get_rr_node_index(grid_x, grid_y, SOURCE, 
                              iclass, LL_rr_node_indices);
    /* Check */
    assert((-1 < inode)&&(inode < num_rr_nodes));
    if (1 < rr_node[inode].num_edges) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])When output pin logic equivalence auto-detect is turned on.\nLogic equivalence of outputs should not be defined in arch. XML (type name: %s).\n",
                 __FILE__, __LINE__, grid_type_descriptor->name);
      exit(1);
    }
    /* Skip direct pins */
    if (TRUE == is_opin_in_direct_list(grid_type_descriptor, rr_node[rr_node[inode].edges[0]].ptc_num, num_directs, clb_to_clb_directs)) {
      continue;
    }
    /* Fill opin_rr_node array */
    source_rr_node[offset] = &(rr_node[inode]);
    offset++;
  }
  assert(offset == num_source_rr_nodes);
 
  /* Add full-capacity network between the two arrays */
  ret += alloc_and_add_fully_capacity_rr_edges_to_source_opin(grid_type_descriptor, num_source_rr_nodes, source_rr_node, opin_rr_node);

  /* Update num_pins in the class */
  for (inode = 0; inode < num_source_rr_nodes; inode++) {
    iclass = source_rr_node[inode]->ptc_num;
    if (DRIVER != grid_type_descriptor->class_inf[iclass].type) {
      continue;
    }
    /* Skip direct pins */
    if (TRUE == is_opin_in_direct_list(grid_type_descriptor, opin_rr_node[inode]->ptc_num, num_directs, clb_to_clb_directs)) {
      continue;
    }
    grid_type_descriptor->class_inf[iclass].num_pins = source_rr_node[inode]->capacity;
  }

  /* Free */
  if (NULL != opin_rr_node) {
    free(opin_rr_node);
  }
  if (NULL != source_rr_node) {
    free(source_rr_node);
  }

  return ret;
}

/* Find the corresponding pb_graph_pin in a CLB with a net_num */

/*  Top-level function 
 *  Add additional edges for all the SOURCE rr_nodes with a net num
 */
int alloc_and_add_grids_fully_capacity_rr_edges(t_ivec*** LL_rr_node_indices,
                                                int num_directs,
	                                            t_clb_to_clb_directs *clb_to_clb_directs) {
  int ix, iy, iz, iblk, used_blk_id;
  t_block* mapped_block = NULL; 
  int ret = 0;

  /* Go by grid to grid */
  for (ix = 0; ix < (nx + 2); ix++) {
    for (iy = 0; iy < (ny + 2); iy++) {
      /* Skip EMPTY_TYPE */
      if (EMPTY_TYPE == grid[ix][iy].type) {
        continue;
      }
      /* Skip IO_TYPE */
      if (IO_TYPE == grid[ix][iy].type) {
        continue;
      }
      /* We only care core grids */
      /* If not specified, we do not modify anything */
      if (FALSE == grid[ix][iy].type->output_ports_eq_auto_detect) {
        continue;
      }
      for (iz = 0; iz < grid[ix][iy].type->capacity; iz++) {
        /* Try to identify if this z block is used or not */
        mapped_block = NULL;
        for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
          used_blk_id = grid[ix][iy].blocks[iblk];
          if (iz == block[used_blk_id].z) {
            mapped_block = &(block[used_blk_id]);
            break;
          }
        } 
        /* Allocate the add edges to this source node */
        ret += alloc_and_add_fully_capacity_rr_edges_to_one_grid(ix, iy, iz, mapped_block, LL_rr_node_indices, num_directs, clb_to_clb_directs);
      }
    }
  }

  return ret;
}

/* Detect which OPINs are used and update routing cost */
void auto_detect_and_reserve_used_opins(float pres_fac) {
  return;
}

/* Search the linked list first,
 * 1. If we find a matched element, increase the counter
 * 2. if we cannot find, allocate a new element and initialize the counter 
 */
t_llist* search_and_add_num_mapped_opin_llist(t_llist* head, 
                                              int cand_num_mapped_opins) {

  t_llist* new_head = NULL; 
  t_llist* temp = NULL;
  t_num_mapped_opins_stats* cur_stats = NULL;
  boolean found = FALSE;

  /* Search the linked list, try to find a matched element */
  temp = head;
  while (temp) {
    assert(NULL != temp->dptr);
    cur_stats = (t_num_mapped_opins_stats*)(temp->dptr);
    /* Find what we want! */
    if (cand_num_mapped_opins == cur_stats->num_mapped_opins) { 
      found = TRUE;
      /* update counter */
      cur_stats->net_cnt++;
    }
    /* go to next */
    temp = temp->next;
  }

  /* decide the next step */
  if (TRUE == found) {
    /* We do not allocate anything, return the old head */
    return head;
  }

  /* Initialize the data */
  cur_stats = (t_num_mapped_opins_stats*)my_malloc(sizeof(t_num_mapped_opins_stats));
  cur_stats->num_mapped_opins = cand_num_mapped_opins;
  cur_stats->net_cnt = 1;

  /* Reach here. We need to allocate a new node */
  if (NULL == head) {
    /* Empty llist, create a new llist, containing 1 node  */
    new_head = create_llist(1);
    new_head->dptr = (void*)(cur_stats);
  } else {
    /* Creata a new node */ 
    insert_llist_node(head);
    head->next->dptr = (void*)(cur_stats);
    new_head = head;
  }

  return new_head;
}

/* Print the stats, and once we print one node, we remove and free it */
void print_net_opin_llist(t_llist* head) {
  t_llist* cur_head = head;
  t_llist* temp = NULL;
  t_llist* min_node = NULL;
  t_num_mapped_opins_stats* cur_stats = NULL;
  
  vpr_printf(TIO_MESSAGE_INFO, "Stats for OPIN occupancy: (number of nets = %d)\n", num_nets); 

  /* Loop until the llist is empty */
  while (1) {
    temp = cur_head;
    /* Initialize min_node */
    min_node = NULL;
    cur_stats = NULL;
    /* each time, we find the lowest num_mapped_opins and print the node*/
    while (temp) {
      assert(NULL != temp->dptr);
      cur_stats = (t_num_mapped_opins_stats*)(temp->dptr);
      if ((NULL == min_node)||(((t_num_mapped_opins_stats*)(min_node->dptr))->num_mapped_opins > cur_stats->num_mapped_opins)) {
        min_node = temp;
      }
      /* go to next */
      temp = temp->next;
    }
    /* No min_node, the llist is empty */
    if (NULL == min_node) {
      break;
    }
    /* Print */
    cur_stats = (t_num_mapped_opins_stats*)(min_node->dptr);
    vpr_printf(TIO_MESSAGE_INFO, "\t%d (%g%) Nets is mapped to %d OPINs.\n", 
                                 cur_stats->net_cnt, 
                                 (float)(100 * cur_stats->net_cnt / num_nets), 
                                 cur_stats->num_mapped_opins);
    /* Remove the node we find */
    /* If this is the head */
    if (min_node == cur_head) {
      cur_head = cur_head->next; 
      free(min_node->dptr);
      free(min_node);
      continue;
    }
    /* Otherwise, we need a traversal */
    temp = cur_head;
    while (temp) {
      if (min_node == temp->next) {
        free(min_node->dptr);
        remove_llist_node(temp); 
        break;
      }
      /* go to next */
      temp = temp->next;
    }
  }

  return;
}

/* Reset all the net_num of rr_nodes and update them according to routing results */
void reassign_rr_node_net_num_from_scratch() {

	/* This routine updates the occ field in the rr_node structure according to *
	 * the resource usage of the current routing.  It does a brute force        *
	 * recompute from scratch that is useful for sanity checking.               */

	int inode, inet;
	struct s_trace *tptr;

	/* First set the occupancy of everything to zero. */

	for (inode = 0; inode < num_rr_nodes; inode++)
		rr_node[inode].net_num = OPEN;

	/* Now go through each net and count the tracks and pins used everywhere */

	for (inet = 0; inet < num_nets; inet++) {

		if (clb_net[inet].is_global) /* Skip global nets. */
			continue;

		tptr = trace_head[inet];
		if (tptr == NULL)
			continue;

		for (;;) {
			inode = tptr->index;
			rr_node[inode].net_num = inet;

			if (rr_node[inode].type == SINK) {
				tptr = tptr->next; /* Skip next segment. */
				if (tptr == NULL)
					break;
			}

			tptr = tptr->next;
		}
	}

	return;
}


/* Xifan TANG: Statisitcs for each net which has occupied more than 1 OPIN */
void print_net_opin_occupancy() {
  int inode, inet;
  t_llist* head = NULL; 

  reassign_rr_node_net_num_from_scratch();

  /* Initialize counters */
  for (inet = 0; inet < num_nets; inet++) {
    clb_net[inet].num_mapped_opins = 0; 
  }

  /* Search each OPIN in the rr_graph */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    if (OPIN != rr_node[inode].type) {
      continue;
    }
    /* Find the net_num */
    inet = rr_node[inode].net_num;
    if (OPEN == inet) {
      continue;
    }
    /* Update the counter in clb_net */
    assert((-1 < inet)&&(inet < num_nets));
    clb_net[inet].num_mapped_opins++; 
  }

  /* Print stats */
  for (inet = 0; inet < num_nets; inet++) {
    /* Global nets is not routed, set to 0 */
    if (TRUE == clb_net[inet].is_global) {
      clb_net[inet].num_mapped_opins = 0; 
    }
    /* Search in the linked list if there exists an element witht the same num_mapped_opins */
    head = search_and_add_num_mapped_opin_llist(head, clb_net[inet].num_mapped_opins);
  }

  /* Print */ 
  print_net_opin_llist(head);

  /* Free */

  return;
}


/* Functions to add fully-capable routing network to a SB connections */
/* Add edges to a CHANX|CHANY node, establishing connections to a OPIN node list */
int add_opin_rr_edges_to_chan_rr_node(t_rr_node* chan_rr_node,
                                      int num_opin_rr_nodes,
                                      t_rr_node** opin_rr_node) {
  int inode;
  int ret = 0; 
 
  /* Check */
  assert((CHANX == chan_rr_node->type)||(CHANY == chan_rr_node->type));
  for (inode = 0; inode < num_opin_rr_nodes; inode++) {
    assert(OPIN == opin_rr_node[inode]->type);
  }
   
  for (inode = 0; inode < num_opin_rr_nodes; inode++) {
    /* 1. For each OPIN_rr_node, check if the chan_rr_node is already connected ! */
    if (TRUE == is_des_rr_node_in_src_rr_node_edges(opin_rr_node[inode], chan_rr_node)) {
      continue; /* Bypass this opin_rr_node then */
    }
    /* 2. Reach here implying a new edge is needed */
    /* Update fan-in */
    chan_rr_node->fan_in++;
    /* Reallocate the edges */
    /* Increase num_edges for source_rr_node[isrc] */ 
    opin_rr_node[inode]->num_edges++;
    opin_rr_node[inode]->edges = (int*)my_realloc(opin_rr_node[inode]->edges, opin_rr_node[inode]->num_edges * sizeof(int));
    opin_rr_node[inode]->switches = (short*)my_realloc(opin_rr_node[inode]->switches, opin_rr_node[inode]->num_edges * sizeof(short));
    opin_rr_node[inode]->edges[opin_rr_node[inode]->num_edges - 1] = chan_rr_node - rr_node;
    /* Add switches for src_rr_node */
    /* TODO: to be smarter */
    opin_rr_node[inode]->switches[opin_rr_node[inode]->num_edges - 1] = opin_rr_node[inode]->switches[0];
    /* Update counter */
    ret++;
  }

  return ret;
}

/* For the OPINs in each block that turns on output_ports_eq_auto_detect
 * 1. Gather OPIN rr_nodes 
 * 2. Find the CHANX or CHANY that is driven by OPINs 
 * 3. For each CHANX|CHANY, create a opin_rr_node list 
 * 4. Add rr_edges 
 */
int alloc_and_add_fully_capacity_sb_rr_edges_to_one_grid(int grid_x, int grid_y, int grid_z,
                                                         t_block* cur_block, 
                                                         t_ivec*** LL_rr_node_indices,
                                                         int num_directs,
	                                                     t_clb_to_clb_directs *clb_to_clb_directs) {
  int ret = 0;
  int ipin, offset, from_node, to_node, inode, iedge, iclass; 
  t_type_ptr grid_type_descriptor = NULL;
  t_pb_graph_node* top_pb_graph_node = NULL;
  int num_opin_rr_nodes = 0;
  int num_chan_rr_nodes = 0;
  t_rr_node** opin_rr_node = NULL;
  t_rr_node** chan_rr_node = NULL;
  boolean is_in_list = FALSE;

  /* Check */
  assert((!(0 > grid_x))&&(!(grid_x > (nx + 1)))); 
  assert((!(0 > grid_y))&&(!(grid_y > (ny + 1)))); 

  /* Get grid type descriptor */
  grid_type_descriptor = grid[grid_x][grid_y].type;
  top_pb_graph_node = grid[grid_x][grid_y].type->pb_graph_head;

  /* Check */
  assert(NULL != grid_type_descriptor);
  assert(NULL != top_pb_graph_node);
  
  /* 1. Gather OPIN rr_nodes */ 
  num_opin_rr_nodes = 0;
  /* Search all the OPIN rr_nodes */
  for (ipin = 0; ipin < grid_type_descriptor->num_pins; ipin++) {
    iclass = grid_type_descriptor->pin_class[ipin];
    /* Skip IPINs */
    if (DRIVER != grid_type_descriptor->class_inf[iclass].type) {
      continue;
    }
    /* Skip direct pins */
    if (TRUE == is_opin_in_direct_list(grid_type_descriptor, ipin, num_directs, clb_to_clb_directs)) {
      continue;
    }
    /* Get SOURCE rr_nodes */
    inode = get_rr_node_index(grid_x, grid_y, OPIN, 
                              ipin, LL_rr_node_indices);
    /* Check */
    assert((-1 < inode)&&(inode < num_rr_nodes));
    /* Update counter */
    num_opin_rr_nodes++;
  }
  /* Allocate */
  opin_rr_node = (t_rr_node**)my_malloc(num_opin_rr_nodes * sizeof(t_rr_node*)); 

  /* Fill the array */
  offset = 0;
  for (ipin = 0; ipin < grid_type_descriptor->num_pins; ipin++) {
    iclass = grid_type_descriptor->pin_class[ipin];
    /* Skip IPINs */
    if (DRIVER != grid_type_descriptor->class_inf[iclass].type) {
      continue;
    }
    /* Skip direct pins */
    if (TRUE == is_opin_in_direct_list(grid_type_descriptor, ipin, num_directs, clb_to_clb_directs)) {
      continue;
    }
    /* Get SOURCE rr_nodes */
    inode = get_rr_node_index(grid_x, grid_y, OPIN, 
                              ipin, LL_rr_node_indices);
    /* Check */
    assert((-1 < inode)&&(inode < num_rr_nodes));
    /* Update counter */
    /* Fill opin_rr_node array */
    opin_rr_node[offset] = &(rr_node[inode]);
    offset++;
  }
  assert(offset == num_opin_rr_nodes);

  /* 2. Get chan_rr_nodes */  
  /* Get the CHANX or CHANY that each OPIN rr_node is driving */
  for (from_node = 0; from_node < num_opin_rr_nodes; from_node++) {
    /* Initialize chan_rr_node */
    num_chan_rr_nodes = 0;
    chan_rr_node = NULL;
    for (iedge = 0; iedge < opin_rr_node[from_node]->num_edges; iedge++) {
      /* if CHANX or CHANY, increase the counter */
      to_node = opin_rr_node[from_node]->edges[iedge]; 
      if ((CHANX != rr_node[to_node].type)
         &&(CHANY != rr_node[to_node].type)) {
        continue;
      }
      /* Check */
      assert((CHANX == rr_node[to_node].type)||(CHANY == rr_node[to_node].type));
      /* Check if this node is already inside the list */
      is_in_list = FALSE;
      for (inode = 0; inode < num_chan_rr_nodes; inode++) {
        if (&(rr_node[to_node]) == chan_rr_node[inode]) {
          is_in_list = TRUE;
          break;
        }
      }
      if (TRUE == is_in_list) {
        continue;
      }
      /* Increase the counter and reallocate */
      num_chan_rr_nodes++;
      /* Allocate */
      chan_rr_node = (t_rr_node**)my_realloc(chan_rr_node, num_chan_rr_nodes * sizeof(t_rr_node*)); 
      /* Update the array */
      chan_rr_node[num_chan_rr_nodes - 1] = &(rr_node[to_node]);
    }
    /* Add edges */
    /* generate offset */
    if (from_node < num_opin_rr_nodes / 2) {
      offset = from_node;
    } else {
      offset = from_node - num_opin_rr_nodes / 2;
    }
    for (inode = 0; inode < num_chan_rr_nodes; inode++) {
      ret += add_opin_rr_edges_to_chan_rr_node(chan_rr_node[inode], num_opin_rr_nodes / 2 + 1, opin_rr_node + offset);  
    }
    /* Free */
    if (NULL != chan_rr_node) {
      free(chan_rr_node);
    }
  }

  /* Free */
  if (NULL != opin_rr_node) {
    free(opin_rr_node);
  }

  return ret;
}


int alloc_and_add_grids_fully_capacity_sb_rr_edges(t_ivec*** LL_rr_node_indices,
                                                   int num_directs,
	                                               t_clb_to_clb_directs *clb_to_clb_directs) {
  int ix, iy, iz, iblk, used_blk_id;
  t_block* mapped_block = NULL; 
  int ret = 0;

  /* Go by grid to grid */
  for (ix = 0; ix < (nx + 2); ix++) {
    for (iy = 0; iy < (ny + 2); iy++) {
      /* Skip IO_TYPE */
      if (IO_TYPE == grid[ix][iy].type) {
        continue;
      }
      /* We only care core grids */
      /* If not specified, we do not modify anything */
      if (FALSE == grid[ix][iy].type->output_ports_eq_auto_detect) {
        continue;
      }
      for (iz = 0; iz < grid[ix][iy].type->capacity; iz++) {
        /* Try to identify if this z block is used or not */
        mapped_block = NULL;
        for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
          used_blk_id = grid[ix][iy].blocks[iblk];
          if (iz == block[used_blk_id].z) {
            mapped_block = &(block[used_blk_id]);
            break;
          }
        } 
        /* Allocate the add edges to this source node */
        ret += alloc_and_add_fully_capacity_sb_rr_edges_to_one_grid(ix, iy, iz, mapped_block, LL_rr_node_indices, num_directs, clb_to_clb_directs);
      }
    }
  }

  return ret;
}

