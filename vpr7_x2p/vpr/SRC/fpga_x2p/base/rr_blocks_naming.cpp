/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <string.h>

#include "rr_blocks_naming.h"

char* convert_cb_type_to_string(t_rr_type chan_type) {
  switch(chan_type) {
  case CHANX:
    return "cbx";
    break;
  case CHANY:
    return "cby";
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid type of channel!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
}

char* convert_chan_type_to_string(t_rr_type chan_type) {
  switch(chan_type) {
  case CHANX:
    return "chanx";
    break;
  case CHANY:
    return "chany";
    break;
  default:    
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid type of channel!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
}

char* convert_chan_rr_node_direction_to_string(enum PORTS chan_rr_node_direction) {
  switch(chan_rr_node_direction) {
  case IN_PORT:
    return "in";
    break;
  case OUT_PORT:
    return "out";
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of port!\n", __FILE__, __LINE__);
    exit(1);
  }
}


