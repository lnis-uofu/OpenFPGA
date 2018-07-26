
#ifndef PB_PIN_EQ_AUTO_DETECT_H 
#define PB_PIN_EQ_AUTO_DETECT_H 
int alloc_and_add_grids_fully_capacity_rr_edges(t_ivec*** LL_rr_node_indices,
                                                int num_directs,
	                                            t_clb_to_clb_directs *clb_to_clb_directs);

void print_net_opin_occupancy();

void auto_detect_and_reserve_used_opins(float pres_fac);

int alloc_and_add_grids_fully_capacity_sb_rr_edges(t_ivec*** LL_rr_node_indices,
                                                   int num_directs,
	                                               t_clb_to_clb_directs *clb_to_clb_directs);
#endif
