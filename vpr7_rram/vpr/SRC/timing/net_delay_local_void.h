
/*********************** Subroutines local to this module ********************/

//static
t_rc_node *alloc_and_load_rc_tree(int inet,
		t_rc_node ** rc_node_free_list_ptr,
		t_linked_rc_edge ** rc_edge_free_list_ptr,
		t_linked_rc_ptr * rr_node_to_rc_node);

//static 
void add_to_rc_tree(t_rc_node * parent_rc, t_rc_node * child_rc,
		short iswitch, int inode, t_linked_rc_edge ** rc_edge_free_list_ptr);

//static 
t_rc_node *alloc_rc_node(t_rc_node ** rc_node_free_list_ptr);

//static 
void free_rc_node(t_rc_node * rc_node,
		t_rc_node ** rc_node_free_list_ptr);

//static 
t_linked_rc_edge *alloc_linked_rc_edge(
		t_linked_rc_edge ** rc_edge_free_list_ptr);

//static 
void free_linked_rc_edge(t_linked_rc_edge * rc_edge,
		t_linked_rc_edge ** rc_edge_free_list_ptr);

//static 
float load_rc_tree_C(t_rc_node * rc_node);

//static 
void load_rc_tree_T(t_rc_node * rc_node, float T_arrival);

//static 
void load_one_net_delay(float **net_delay, int inet, struct s_net *nets,
		t_linked_rc_ptr * rr_node_to_rc_node);

//static 
void load_one_constant_net_delay(float **net_delay, int inet,
		struct s_net *nets, float delay_value);

//static 
void free_rc_tree(t_rc_node * rc_root,
		t_rc_node ** rc_node_free_list_ptr,
		t_linked_rc_edge ** rc_edge_free_list_ptr);

//static 
void reset_rr_node_to_rc_node(t_linked_rc_ptr * rr_node_to_rc_node,
		int inet);

//static 
void free_rc_node_free_list(t_rc_node * rc_node_free_list);

//static 
void free_rc_edge_free_list(t_linked_rc_edge * rc_edge_free_list);

