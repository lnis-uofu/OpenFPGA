#ifndef CHECK_RR_GRAPH_H
#define CHECK_RR_GRAPH_H

void check_rr_graph(INP const t_graph_type graph_type,
					INP const int L_nx, INP const int L_ny,
					INP const int num_switches,
					int **Fc_in);

void check_node(int inode, enum e_route_type route_type);

#endif

