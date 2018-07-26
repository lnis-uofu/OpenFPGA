#include <stdio.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"
/* Xifan TANG: move the data structures to another header file so that to share them*/
#include "net_delay_types.h"
#include "net_delay_local_void.h"
/* END */
#include "net_delay.h"

/* mrFPGA: Xifan TANG */
#include "mrfpga_globals.h"
#include "cal_capacitance.h"
/* end */

/*************************** Subroutine definitions **************************/

float **
alloc_net_delay(t_chunk *chunk_list_ptr, struct s_net *nets,
		int n_nets){

	/* Allocates space for the net_delay data structure                          *
	 * [0..num_nets-1][1..num_pins-1].  I chunk the data to save space on large  *
	 * problems.                                                                 */

	float **net_delay; /* [0..num_nets-1][1..num_pins-1] */
	float *tmp_ptr;
	int inet;

	net_delay = (float **) my_malloc(n_nets * sizeof(float *));

	for (inet = 0; inet < n_nets; inet++) {
		tmp_ptr = (float *) my_chunk_malloc(
				((nets[inet].num_sinks + 1) - 1) * sizeof(float),
				chunk_list_ptr);

		net_delay[inet] = tmp_ptr - 1; /* [1..num_pins-1] */
	}

	return (net_delay);
}

void free_net_delay(float **net_delay,
		t_chunk *chunk_list_ptr){

	/* Frees the net_delay structure.  Assumes it was chunk allocated.          */

	free(net_delay);
	free_chunk_memory(chunk_list_ptr);
}

void load_net_delay_from_routing(float **net_delay, struct s_net *nets,
		int n_nets) {

	/* This routine loads net_delay[0..num_nets-1][1..num_pins-1].  Each entry   *
	 * is the Elmore delay from the net source to the appropriate sink.  Both    *
	 * the rr_graph and the routing traceback must be completely constructed     *
	 * before this routine is called, and the net_delay array must have been     *
	 * allocated.                                                                */

	t_rc_node *rc_node_free_list, *rc_root;
	t_linked_rc_edge *rc_edge_free_list;
	int inet;
	t_linked_rc_ptr *rr_node_to_rc_node; /* [0..num_rr_nodes-1]  */

	rr_node_to_rc_node = (t_linked_rc_ptr *) my_calloc(num_rr_nodes,
			sizeof(t_linked_rc_ptr));

	rc_node_free_list = NULL;
	rc_edge_free_list = NULL;

	for (inet = 0; inet < n_nets; inet++) {
		if (nets[inet].is_global) {
			load_one_constant_net_delay(net_delay, inet, nets, 0.);
		} else {
			rc_root = alloc_and_load_rc_tree(inet, &rc_node_free_list,
					&rc_edge_free_list, rr_node_to_rc_node);
			load_rc_tree_C(rc_root);
			load_rc_tree_T(rc_root, 0.);
			load_one_net_delay(net_delay, inet, nets, rr_node_to_rc_node);
			free_rc_tree(rc_root, &rc_node_free_list, &rc_edge_free_list);
			reset_rr_node_to_rc_node(rr_node_to_rc_node, inet);
		}
	}

	free_rc_node_free_list(rc_node_free_list);
	free_rc_edge_free_list(rc_edge_free_list);
	free(rr_node_to_rc_node);
}

void load_constant_net_delay(float **net_delay, float delay_value,
		struct s_net *nets, int n_nets) {

	/* Loads the net_delay array with delay_value for every source - sink        *
	 * connection that is not on a global resource, and with 0. for every source *
	 * - sink connection on a global net.  (This can be used to allow timing     *
	 * analysis before routing is done with a constant net delay model).         */

	int inet;

	for (inet = 0; inet < n_nets; inet++) {
		if (nets[inet].is_global) {
			load_one_constant_net_delay(net_delay, inet, nets, 0.);
		} else {
			load_one_constant_net_delay(net_delay, inet, nets, delay_value);
		}
	}
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
t_rc_node *
alloc_and_load_rc_tree(int inet, t_rc_node ** rc_node_free_list_ptr,
		t_linked_rc_edge ** rc_edge_free_list_ptr,
		t_linked_rc_ptr * rr_node_to_rc_node) {

	/* Builds a tree describing the routing of net inet.  Allocates all the data *
	 * and inserts all the connections in the tree.                              */

	t_rc_node *curr_rc, *prev_rc, *root_rc;
	struct s_trace *tptr;
	int inode, prev_node;
	short iswitch;
	t_linked_rc_ptr *linked_rc_ptr;

	root_rc = alloc_rc_node(rc_node_free_list_ptr);
	tptr = trace_head[inet];

	if (tptr == NULL) {
		vpr_printf(TIO_MESSAGE_ERROR, "in alloc_and_load_rc_tree: Traceback for net %d does not exist.\n", inet);
		exit(1);
	}

	inode = tptr->index;
	iswitch = tptr->iswitch;
	root_rc->inode = inode;
	root_rc->u.child_list = NULL;
	rr_node_to_rc_node[inode].rc_node = root_rc;

	prev_rc = root_rc;
	tptr = tptr->next;

	while (tptr != NULL) {
		inode = tptr->index;

		/* Is this node a "stitch-in" point to part of the existing routing or a   *
		 * new piece of routing along the current routing "arm?"                   */

		if (rr_node_to_rc_node[inode].rc_node == NULL) { /* Part of current "arm" */
			curr_rc = alloc_rc_node(rc_node_free_list_ptr);
			add_to_rc_tree(prev_rc, curr_rc, iswitch, inode,
					rc_edge_free_list_ptr);
			rr_node_to_rc_node[inode].rc_node = curr_rc;
			prev_rc = curr_rc;
		}

		else if (rr_node[inode].type != SINK) { /* Connection to old stuff. */

#ifdef DEBUG
			prev_node = prev_rc->inode;
			if (rr_node[prev_node].type != SINK) {
				vpr_printf(TIO_MESSAGE_ERROR, "in alloc_and_load_rc_tree: Routing of net %d is not a tree.\n", inet);
				exit(1);
			}
#endif

			prev_rc = rr_node_to_rc_node[inode].rc_node;
		}

		else { /* SINK that this net has connected to more than once. */

			/* I can connect to a SINK node more than once in some weird architectures. *
			 * That means the routing isn't really a tree -- there is reconvergent      *
			 * fanout from two or more IPINs into one SINK.  I convert this structure   *
			 * into a true RC tree on the fly by creating a new rc_node each time I hit *
			 * the same sink.  This means I need to keep a linked list of the rc_nodes  *
			 * associated with the rr_node (inode) associated with that SINK.           */

			curr_rc = alloc_rc_node(rc_node_free_list_ptr);
			add_to_rc_tree(prev_rc, curr_rc, iswitch, inode,
					rc_edge_free_list_ptr);

			linked_rc_ptr = (t_linked_rc_ptr *) my_malloc(
					sizeof(t_linked_rc_ptr));
			linked_rc_ptr->next = rr_node_to_rc_node[inode].next;
			rr_node_to_rc_node[inode].next = linked_rc_ptr;
			linked_rc_ptr->rc_node = curr_rc;

			prev_rc = curr_rc;
		}
		iswitch = tptr->iswitch;
		tptr = tptr->next;
	}

	return (root_rc);
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void add_to_rc_tree(t_rc_node * parent_rc, t_rc_node * child_rc,
		short iswitch, int inode, t_linked_rc_edge ** rc_edge_free_list_ptr) {

	/* Adds child_rc to the child list of parent_rc, and sets the switch between *
	 * them to iswitch.  This routine also intitializes the child_rc properly    *
	 * and sets its node value to inode.                                         */

	t_linked_rc_edge *linked_rc_edge;

	linked_rc_edge = alloc_linked_rc_edge(rc_edge_free_list_ptr);

	linked_rc_edge->next = parent_rc->u.child_list;
	parent_rc->u.child_list = linked_rc_edge;

	linked_rc_edge->child = child_rc;
	linked_rc_edge->iswitch = iswitch;

	child_rc->u.child_list = NULL;
	child_rc->inode = inode;
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
t_rc_node *
alloc_rc_node(t_rc_node ** rc_node_free_list_ptr) {

	/* Allocates a new rc_node, from the free list if possible, from the free   *
	 * store otherwise.                                                         */

	t_rc_node *rc_node;

	rc_node = *rc_node_free_list_ptr;

	if (rc_node != NULL) {
		*rc_node_free_list_ptr = rc_node->u.next;
	} else {
		rc_node = (t_rc_node *) my_malloc(sizeof(t_rc_node));
	}

	return (rc_node);
}


/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void free_rc_node(t_rc_node * rc_node,
		t_rc_node ** rc_node_free_list_ptr) {

	/* Adds rc_node to the proper free list.          */

	rc_node->u.next = *rc_node_free_list_ptr;
	*rc_node_free_list_ptr = rc_node;
}


/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
t_linked_rc_edge *
alloc_linked_rc_edge(t_linked_rc_edge ** rc_edge_free_list_ptr) {

	/* Allocates a new linked_rc_edge, from the free list if possible, from the  *
	 * free store otherwise.                                                     */

	t_linked_rc_edge *linked_rc_edge;

	linked_rc_edge = *rc_edge_free_list_ptr;

	if (linked_rc_edge != NULL) {
		*rc_edge_free_list_ptr = linked_rc_edge->next;
	} else {
		linked_rc_edge = (t_linked_rc_edge *) my_malloc(
				sizeof(t_linked_rc_edge));
	}

	return (linked_rc_edge);
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void free_linked_rc_edge(t_linked_rc_edge * rc_edge,
		t_linked_rc_edge ** rc_edge_free_list_ptr) {

	/* Adds the rc_edge to the rc_edge free list.                       */

	rc_edge->next = *rc_edge_free_list_ptr;
	*rc_edge_free_list_ptr = rc_edge;
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
float load_rc_tree_C(t_rc_node * rc_node) {

	/* Does a post-order traversal of the rc tree to load each node's           *
	 * C_downstream with the proper sum of all the downstream capacitances.     *
	 * This routine calls itself recursively to perform the traversal.          */

	t_linked_rc_edge *linked_rc_edge;
	t_rc_node *child_node;
	int inode;
	short iswitch;
	float C, C_downstream;

	linked_rc_edge = rc_node->u.child_list;
	inode = rc_node->inode;
	C = rr_node[inode].C;

	while (linked_rc_edge != NULL) { /* For all children */
		iswitch = linked_rc_edge->iswitch;
		child_node = linked_rc_edge->child;
		C_downstream = load_rc_tree_C(child_node);
 
        /* mrFPGA : Xifan TANG */
        if (is_isolation) {
          C_downstream += switch_inf[iswitch].Cout;
          C += switch_inf[iswitch].Cin;
        }
        /* end */

		if (switch_inf[iswitch].buffered == FALSE)
			C += C_downstream;

		linked_rc_edge = linked_rc_edge->next;
	}

    /* mrFPGA : Xifan TANG */
    if (is_mrFPGA && rr_node[inode].buffered) {
      rc_node->Tdel = (wire_buffer_inf.R + 0.5 * rr_node[inode].R) * C;
      C = wire_buffer_inf.C;
      if ((CHANX == rr_node[inode].type && rr_node[child_node->inode].yhigh <= rr_node[inode].ylow)
         ||(rr_node[inode].type == CHANY && rr_node[child_node->inode].xhigh <= rr_node[inode].xlow)) {   
        rr_node[inode].buffered = -1;
      }
    } else {
      rc_node->Tdel = 0.;
    }
    /* end */

	rc_node->C_downstream = C;
	return (C);
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void load_rc_tree_T(t_rc_node * rc_node, float T_arrival) {

	/* This routine does a pre-order depth-first traversal of the rc tree to    *
	 * compute the Tdel to each node in the rc tree.  The T_arrival is the time *
	 * at which the signal hits the input to this node.  This routine calls     *
	 * itself recursively to perform the traversal.                             */

	float Tdel, Rmetal, Tchild;
	t_linked_rc_edge *linked_rc_edge;
	t_rc_node *child_node;
	short iswitch;
	int inode;

	Tdel = T_arrival;
	inode = rc_node->inode;
	Rmetal = rr_node[inode].R;

	/* NB:  rr_node[inode].C gives the capacitance of this node, while          *
	 * rc_node->C_downstream gives the unbuffered downstream capacitance rooted *
	 * at this node, including the C of the node itself.  I want to multiply    *
	 * the C of this node by 0.5 Rmetal, since it's a distributed RC line.      *
	 * Hence 0.5 Rmetal * Cnode is a pessimistic estimate of delay (i.e. end to *
	 * end).  For the downstream capacitance rooted at this node (not including *
	 * the capacitance of the node itself), I assume it is, on average,         *
	 * connected halfway along the line, so I also multiply by 0.5 Rmetal.  To  *
	 * be totally pessimistic I would multiply the downstream part of the       *
	 * capacitance by Rmetal.  Play with this equation if you like.             */

	/* Rmetal is distributed so x0.5 */
    /* mrFPGA : Xifan TANG */
    if (is_mrFPGA && rr_node[inode].buffered) {
      Tdel += wire_buffer_inf.Tdel;
      rc_node->Tdel += Tdel;
      Tdel = rc_node->Tdel;
    /* end  */
    } else {
      /* Original VPR */
	  Tdel += 0.5 * rc_node->C_downstream * Rmetal;
	  rc_node->Tdel = Tdel;
      /* end  */
    }

	/* Now expand the children of this node to load their Tdel values.       */

	linked_rc_edge = rc_node->u.child_list;

	while (linked_rc_edge != NULL) { /* For all children */
		iswitch = linked_rc_edge->iswitch;
		child_node = linked_rc_edge->child;

		Tchild = Tdel + switch_inf[iswitch].R * child_node->C_downstream;
		Tchild += switch_inf[iswitch].Tdel; /* Intrinsic switch delay. */
		load_rc_tree_T(child_node, Tchild);

		linked_rc_edge = linked_rc_edge->next;
	}
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void load_one_net_delay(float **net_delay, int inet, struct s_net* nets,
		t_linked_rc_ptr * rr_node_to_rc_node) {

	/* Loads the net delay array for net inet.  The rc tree for that net must  *
	 * have already been completely built and loaded.                          */

	int ipin, inode;
	float Tmax;
	t_rc_node *rc_node;
	t_linked_rc_ptr *linked_rc_ptr, *next_ptr;

	for (ipin = 1; ipin < (nets[inet].num_sinks + 1); ipin++) {

		inode = net_rr_terminals[inet][ipin];

		linked_rc_ptr = rr_node_to_rc_node[inode].next;
		rc_node = rr_node_to_rc_node[inode].rc_node;
		Tmax = rc_node->Tdel;

		/* If below only executes when one net connects several times to the      *
		 * same SINK.  In this case, I can't tell which net pin each connection   *
		 * to this SINK corresponds to (I can just choose arbitrarily).  To make  *
		 * sure the timing behaviour converges, I pessimistically set the delay   *
		 * for all of the connections to this SINK by this net to be the max. of  *
		 * the delays from this net to this SINK.  NB:  This code only occurs     *
		 * when a net connect more than once to the same pin class on the same    *
		 * logic block.  Only a weird architecture would allow this.              */

		if (linked_rc_ptr != NULL) {

			/* The first time I hit a multiply-used SINK, I choose the largest delay  *
			 * from this net to this SINK and use it for every connection to this     *
			 * SINK by this net.                                                      */

			do {
				rc_node = linked_rc_ptr->rc_node;
				if (rc_node->Tdel > Tmax) {
					Tmax = rc_node->Tdel;
					rr_node_to_rc_node[inode].rc_node = rc_node;
				}
				next_ptr = linked_rc_ptr->next;
				free(linked_rc_ptr);
				linked_rc_ptr = next_ptr;
			} while (linked_rc_ptr != NULL); /* End do while */

			rr_node_to_rc_node[inode].next = NULL;
		}
		/* End of if multiply-used SINK */
		net_delay[inet][ipin] = Tmax;
	}
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void load_one_constant_net_delay(float **net_delay, int inet,
		struct s_net *nets, float delay_value) {

	/* Sets each entry of the net_delay array for net inet to delay_value.     */

	int ipin;

	for (ipin = 1; ipin < (nets[inet].num_sinks + 1); ipin++)
		net_delay[inet][ipin] = delay_value;
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void free_rc_tree(t_rc_node * rc_root,
		t_rc_node ** rc_node_free_list_ptr,
		t_linked_rc_edge ** rc_edge_free_list_ptr) {

	/* Puts the rc tree pointed to by rc_root back on the free list.  Depth-     *
	 * first post-order traversal via recursion.                                 */

	t_rc_node *rc_node, *child_node;
	t_linked_rc_edge *rc_edge, *next_edge;

	rc_node = rc_root;
	rc_edge = rc_node->u.child_list;

	while (rc_edge != NULL) { /* For all children */
		child_node = rc_edge->child;
		free_rc_tree(child_node, rc_node_free_list_ptr, rc_edge_free_list_ptr);
		next_edge = rc_edge->next;
		free_linked_rc_edge(rc_edge, rc_edge_free_list_ptr);
		rc_edge = next_edge;
	}

	free_rc_node(rc_node, rc_node_free_list_ptr);
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void reset_rr_node_to_rc_node(t_linked_rc_ptr * rr_node_to_rc_node,
		int inet) {

	/* Resets the rr_node_to_rc_node mapping entries that were set during       *
	 * construction of the RC tree for net inet.  Any extra linked list entries *
	 * added to deal with a SINK being connected to multiple times have already *
	 * been freed by load_one_net_delay.                                        */

	struct s_trace *tptr;
	int inode;

	tptr = trace_head[inet];

	while (tptr != NULL) {
		inode = tptr->index;
		rr_node_to_rc_node[inode].rc_node = NULL;
		tptr = tptr->next;
	}
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void free_rc_node_free_list(t_rc_node * rc_node_free_list) {

	/* Really frees (i.e. calls free()) all the rc_nodes on the free list.   */

	t_rc_node *rc_node, *next_node;

	rc_node = rc_node_free_list;

	while (rc_node != NULL) {
		next_node = rc_node->u.next;
		free(rc_node);
		rc_node = next_node;
	}
}

/* mrFPGA: Xifan TANG, let's share these functions*/
//static 
/* END */
void free_rc_edge_free_list(t_linked_rc_edge * rc_edge_free_list) {

	/* Really frees (i.e. calls free()) all the rc_edges on the free list.   */

	t_linked_rc_edge *rc_edge, *next_edge;

	rc_edge = rc_edge_free_list;

	while (rc_edge != NULL) {
		next_edge = rc_edge->next;
		free(rc_edge);
		rc_edge = next_edge;
	}
}
