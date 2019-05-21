/* Avoid repeated header inclusion */
#ifndef FPGA_X2P_IDENTIFY_ROUTING
#define FPGA_X2P_IDENTIFY_ROUTING

void identify_mirror_switch_blocks();
void identify_mirror_connection_blocks();

DeviceRRChan build_device_rr_chan(int LL_num_rr_nodes, t_rr_node* LL_rr_node, 
                                  t_ivec*** LL_rr_node_indices, int num_segments,
                                  t_rr_indexed_data* LL_rr_indexed_data);

/* Rotatable will be done in the next step 
identify_rotatable_switch_blocks(); 
identify_rotatable_connection_blocks(); 
*/

#endif
