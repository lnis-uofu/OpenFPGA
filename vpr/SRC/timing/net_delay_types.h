/***************** Types and defines local to this module ********************/

struct s_linked_rc_edge {
	struct s_rc_node *child;
	short iswitch;
	struct s_linked_rc_edge *next;
};

typedef struct s_linked_rc_edge t_linked_rc_edge;

/* Linked list listing the children of an rc_node.                           *
 * child:  Pointer to an rc_node (child of the current node).                *
 * iswitch:  Index of the switch type used to connect to the child node.     *
 * next:   Pointer to the next linked_rc_edge in the linked list (allows     *
 *         you to get the next child of the current rc_node).                */

struct s_rc_node {
	union {
		t_linked_rc_edge *child_list;
		struct s_rc_node *next;
	} u;
	int inode;
	float C_downstream;
	float Tdel;
};

typedef struct s_rc_node t_rc_node;

/* Structure describing one node in an RC tree (used to get net delays).     *
 * u.child_list:  Pointer to a linked list of linked_rc_edge.  Each one of   *
 *                the linked list entries gives a child of this node.        *
 * u.next:  Used only when this node is on the free list.  Gives the next    *
 *          node on the free list.                                           *
 * inode:  index (ID) of the rr_node that corresponds to this rc_node.       *
 * C_downstream:  Total downstream capacitance from this rc_node.  That is,  *
 *                the total C of the subtree rooted at the current node,     *
 *                including the C of the current node.                       *
 * Tdel:  Time delay for the signal to get from the net source to this node. *
 *        Includes the time to go through this node.                         */

struct s_linked_rc_ptr {
	struct s_rc_node *rc_node;
	struct s_linked_rc_ptr *next;
};

typedef struct s_linked_rc_ptr t_linked_rc_ptr;

/* Linked list of pointers to rc_nodes.                                      *
 * rc_node:  Pointer to an rc_node.                                          *
 * next:  Next list element.                                                 */

