#include <stdio.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"
#include "cal_capacitance.h"
/* Xifan TANG */
#include "mrfpga_globals.h"
#include "net_delay_types.h"
#include "net_delay_local_void.h"
/* end */

static float load_rc_tree_Ctotal (t_rc_node *rc_node);

void cal_capacitance_from_routing ( ) {

 t_rc_node *rc_node_free_list, *rc_root;
 t_linked_rc_edge *rc_edge_free_list;
 int inet;
 t_linked_rc_ptr *rr_node_to_rc_node;          /* [0..num_rr_nodes-1]  */

 float total_capacitance = 0;

 rc_node_free_list = NULL;
 rc_edge_free_list = NULL;

 rr_node_to_rc_node = (t_linked_rc_ptr *) my_calloc (num_rr_nodes, 
                      sizeof (t_linked_rc_ptr));

 for (inet=0;inet<num_nets;inet++) {
    if(clb_net[inet].is_global){
    }
    else {
       rc_root = alloc_and_load_rc_tree (inet, &rc_node_free_list, 
                  &rc_edge_free_list, rr_node_to_rc_node);
       total_capacitance += load_rc_tree_Ctotal (rc_root);
       free_rc_tree (rc_root, &rc_node_free_list, &rc_edge_free_list);
       reset_rr_node_to_rc_node (rr_node_to_rc_node, inet);
    }
 }
 printf("\ntotal capacitance in routing: %g\n", total_capacitance );
}

static float load_rc_tree_Ctotal (t_rc_node *rc_node) {

/* Does a post-order traversal of the rc tree to load each node's           *
 * C_downstream with the proper sum of all the downstream capacitances.     *
 * This routine calls itself recursively to perform the traversal.          */

 t_linked_rc_edge *linked_rc_edge;
 t_rc_node *child_node;
 int inode;
 short iswitch;
 float C; //,C_downstream;
 
 linked_rc_edge = rc_node->u.child_list;
 inode = rc_node->inode;
 C = rr_node[inode].C;

 if( rr_node[inode].buffered )
 {
    C += wire_buffer_inf.C * 3.0;
 }

 while (linked_rc_edge != NULL) {            /* For all children */
    iswitch = linked_rc_edge->iswitch;
    child_node = linked_rc_edge->child;
    C += load_rc_tree_Ctotal (child_node);

    if ( is_mrFPGA )
    {
        C += switch_inf[iswitch].Cin;
        C += switch_inf[iswitch].Cout;
    }
    else if ( switch_inf[iswitch].buffered )
    {
        C += switch_inf[iswitch].Cin * 2.0;
        C += switch_inf[iswitch].Cout * 2.0;
    }
    linked_rc_edge = linked_rc_edge->next;
 }
 return (C);
}

