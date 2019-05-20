/* Avoid repeated header inclusion */
#ifndef FPGA_X2P_IDENTIFY_ROUTING
#define FPGA_X2P_IDENTIFY_ROUTING

void identify_mirror_switch_blocks();
void identify_mirror_connection_blocks();

/* Rotatable will be done in the next step 
identify_rotatable_switch_blocks(); 
identify_rotatable_connection_blocks(); 
*/

#endif
