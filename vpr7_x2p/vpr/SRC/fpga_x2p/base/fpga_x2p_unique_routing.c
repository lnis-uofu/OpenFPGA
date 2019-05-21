/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"
#include "route_common.h"

/* Include spice support headers*/
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "rr_blocks.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_unique_routing.h"

/***** subroutines declaration *****/
void assign_switch_block_mirror(t_sb* src, t_sb* des);

void assign_connection_block_mirror(t_cb* src, t_cb* des);

boolean is_two_sb_rr_nodes_mirror(t_sb* src_sb, t_sb* des_sb, int side, 
                                  t_rr_node* src_rr_node, t_rr_node* des_rr_node);

boolean is_two_cb_rr_nodes_mirror(t_cb* src_cb, t_cb* des_cb, 
                                  t_rr_node* src_rr_node, t_rr_node* des_rr_node);

boolean is_two_switch_blocks_mirror(t_sb* src, t_sb* des);

boolean is_two_connection_blocks_mirror(t_cb* src, t_cb* des);

void assign_mirror_switch_blocks();

void assign_mirror_connection_blocks();

boolean validate_one_switch_block_mirror(t_sb* cur_sb);

boolean validate_one_connection_block_mirror(t_cb* cur_cb);

void update_one_switch_block_mirror(t_sb* cur_sb);

void update_one_connection_block_mirror(t_cb* cur_cb);

boolean validate_mirror_switch_blocks();

boolean validate_mirror_connection_blocks();

void update_mirror_switch_blocks();

void update_mirror_connection_blocks();

void print_mirror_switch_block_stats();

void print_mirror_connection_block_stats();

RRChan build_one_rr_chan(t_rr_type chan_type, size_t chan_x, size_t chan_y,
                         int LL_num_rr_nodes, t_rr_node* LL_rr_node, 
                         t_ivec*** LL_rr_node_indices, int num_segments,
                         t_rr_indexed_data* LL_rr_indexed_data);

void print_device_rr_chan_stats(DeviceRRChan& device_rr_chan);

/***** subroutines *****/
void assign_switch_block_mirror(t_sb* src, t_sb* des) {
  assert ( (NULL != src) && (NULL != des) );
  /* check if the mirror of the first SB is assigned */
  if (NULL != src->mirror) {
    /* Assign mirror of the first SB to the second SB */
    /* traceback to the upstream */
    t_sb* temp = src->mirror;
    while (NULL != temp->mirror) {
      /* go to the next */
      temp = temp->mirror;
    }
    /* We reach the upstream, ensure its mirror is empty */
    assert(NULL == temp->mirror);
    des->mirror = temp;  
  } else {
    /* Assign the first SB as the mirror to the second SB */
    des->mirror = src;  
  }

  return;
}

void assign_connection_block_mirror(t_cb* src, t_cb* des) {
  assert ( (NULL != src) && (NULL != des) );
  /* check if the mirror of the first SB is assigned */
  if (NULL != src->mirror) {
    /* Assign mirror of the first SB to the second SB */
    /* traceback to the upstream */
    t_cb* temp = src->mirror;
    while (NULL != temp->mirror) {
      /* go to the next */
      temp = temp->mirror;
    }
    /* We reach the upstream, ensure its mirror is empty */
    assert(NULL == temp->mirror);
    des->mirror = temp;  
  } else {
    /* Assign the first SB as the mirror to the second SB */
    des->mirror = src;  
  }

  return;
}


/* check if two rr_nodes have a similar set of drive_rr_nodes 
 * for each drive_rr_node:
 * 1. CHANX or CHANY: should have the same side and index
 * 2. OPIN or IPIN: should have the same side and index
 * 3. each drive_rr_switch should be the same 
 */
boolean is_two_sb_rr_nodes_mirror(t_sb* src_sb, t_sb* des_sb, int side, 
                                  t_rr_node* src_rr_node, t_rr_node* des_rr_node) {

  /* Ensure rr_nodes are either the output of short-connection or multiplexer  */
  if (  check_drive_rr_node_imply_short(*src_sb, src_rr_node, side)
     != check_drive_rr_node_imply_short(*des_sb, des_rr_node, side)) {
    return FALSE;
  }
  /* Find the driving rr_node in this sb */
  if (TRUE == check_drive_rr_node_imply_short(*src_sb, src_rr_node, side)) {
    /* Ensure we have the same track id for the driving nodes */
    if ( is_rr_node_exist_opposite_side_in_sb_info(*src_sb, src_rr_node, side)
      != is_rr_node_exist_opposite_side_in_sb_info(*des_sb, des_rr_node, side)) {
      return FALSE;
    }
  } else { /* check driving rr_nodes */
    if ( src_rr_node->num_drive_rr_nodes != des_rr_node->num_drive_rr_nodes ) {
      return FALSE;
    }
    for (int inode = 0; inode < src_rr_node->num_drive_rr_nodes; ++inode) {
      /* node type should be the same  */
      if ( src_rr_node->drive_rr_nodes[inode]->type
        != des_rr_node->drive_rr_nodes[inode]->type) {
        return FALSE;
      }
      /* switch type should be the same  */
      if ( src_rr_node->drive_switches[inode]
        != des_rr_node->drive_switches[inode]) {
        return FALSE;
      }
      int src_node_id, des_node_id;
      int src_node_side, des_node_side; 
      get_rr_node_side_and_index_in_sb_info(src_rr_node->drive_rr_nodes[inode], *src_sb, OUT_PORT, &src_node_side, &src_node_id);
      get_rr_node_side_and_index_in_sb_info(des_rr_node->drive_rr_nodes[inode], *des_sb, OUT_PORT, &des_node_side, &des_node_id);
      if (src_node_id != des_node_id) {
        return FALSE;
      } 
      if (src_node_side != des_node_side) {
        return FALSE;
      } 
    }
  }

  return TRUE;
}

/* check if two rr_nodes have a similar set of drive_rr_nodes 
 * for each drive_rr_node:
 * 1. CHANX or CHANY: should have the same side and index
 * 2. OPIN or IPIN: should have the same side and index
 * 3. each drive_rr_switch should be the same 
 */
boolean is_two_cb_rr_nodes_mirror(t_cb* src_cb, t_cb* des_cb, 
                                  t_rr_node* src_rr_node, t_rr_node* des_rr_node) {

  /* check driving rr_nodes */
  if ( src_rr_node->num_drive_rr_nodes != des_rr_node->num_drive_rr_nodes ) {
    return FALSE;
  }
  for (int inode = 0; inode < src_rr_node->num_drive_rr_nodes; ++inode) {
    /* node type should be the same  */
    if ( src_rr_node->drive_rr_nodes[inode]->type
      != des_rr_node->drive_rr_nodes[inode]->type) {
      return FALSE;
    }
    /* switch type should be the same  */
    if ( src_rr_node->drive_switches[inode]
      != des_rr_node->drive_switches[inode]) {
      return FALSE;
    }
    int src_node_id, des_node_id;
    int src_node_side, des_node_side; 
    get_rr_node_side_and_index_in_cb_info(src_rr_node->drive_rr_nodes[inode], *src_cb, OUT_PORT, &src_node_side, &src_node_id);
    get_rr_node_side_and_index_in_cb_info(des_rr_node->drive_rr_nodes[inode], *des_cb, OUT_PORT, &des_node_side, &des_node_id);
    if (src_node_id != des_node_id) {
      return FALSE;
    } 
    if (src_node_side != des_node_side) {
      return FALSE;
    } 
  }

  return TRUE;
}


/* Idenify mirror Switch blocks 
 * Check each two switch blocks: 
 * 1. Number of channel/opin/ipin rr_nodes are same 
 * For channel rr_nodes
 * 2. check if their track_ids (ptc_num) are same
 * 3. Check if the switches (ids) are same
 * For opin/ipin rr_nodes, 
 * 4. check if their parent type_descriptors same, 
 * 5. check if pin class id and pin id are same 
 * If all above are satisfied, the two switch blocks are mirrors!
 */
boolean is_two_switch_blocks_mirror(t_sb* src, t_sb* des) {

  /* check the numbers of sides */
  if (src->num_sides != des->num_sides) {
    return FALSE;
  }

  /* check the numbers/directionality of channel rr_nodes */
  for (int side = 0; side < src->num_sides; ++side) {
    /* Ensure we have the same channel width on this side */
    if (src->chan_width[side] != des->chan_width[side]) {
      return FALSE;
    }
    for (int itrack = 0; itrack < src->chan_width[side]; ++itrack) {
      /* Check the directionality of each node */
      if (src->chan_rr_node_direction[side][itrack] != des->chan_rr_node_direction[side][itrack]) {
        return FALSE;
      }
      /* Check the track_id of each node */
      if (src->chan_rr_node[side][itrack]->ptc_num != des->chan_rr_node[side][itrack]->ptc_num) {
        return FALSE;
      }
      /* For OUT_PORT rr_node, we need to check fan-in */
      if (OUT_PORT != src->chan_rr_node_direction[side][itrack]) {
        continue; /* skip IN_PORT */
      }

      if (FALSE == is_two_sb_rr_nodes_mirror(src, des, side,
                                             src->chan_rr_node[side][itrack],
                                             des->chan_rr_node[side][itrack])) {
        return FALSE;
      } 
    }
  } 

  /* check the numbers of opin_rr_nodes */
  for (int side = 0; side < src->num_sides; ++side) {
    if (src->num_ipin_rr_nodes[side] != des->num_ipin_rr_nodes[side]) {
      return FALSE;
    }
  }

  /* Make sure the number of conf bits are the same */
  if ( (src->conf_bits_msb - src->conf_bits_lsb) 
     != (des->conf_bits_msb - des->conf_bits_lsb)) {
    return FALSE;
  }

  return TRUE;
}

/* Walk through all the switch blocks,
 * Make one-to-one comparison,
 * If we have a pair, update the 1st SB to be the base and label the 2nd as a mirror
 * If the 1st SB is already a mirror to another, we will trace back to the upstream base and update the 2nd SB
 */
void assign_mirror_switch_blocks() {

  /* Walkthrough each column, and find mirrors */
  for (int ix = 0; ix < (nx + 1); ++ix) {
    for (int iy = 0; iy < (ny + 1); ++iy) {
      for (int jy = iy; jy < (ny + 1); ++jy) {
        /* bypass the same one */
        if (iy == jy) {
          continue;
        }
        /* Do one-to-one comparison */
        if (FALSE == is_two_switch_blocks_mirror(&(sb_info[ix][iy]), &(sb_info[ix][jy]))) {
          /* Nothing to do if the two switch blocks are not equivalent */
          continue;
        }
        /* configure the mirror of the second switch block */
        assign_switch_block_mirror(&(sb_info[ix][iy]), &(sb_info[ix][jy]));
      }
    }
  }
  /* Now mirror switch blocks in each column has been annotated */

  /* Walkthrough each row, and find mirrors */
  for (int iy = 0; iy < (ny + 1); ++iy) {
    for (int ix = 0; ix < (nx + 1); ++ix) {
      for (int jx = ix; jx < (nx + 1); ++jx) {
        /* bypass the same one */
        if (ix == jx) {
          continue;
        }
        /* Do one-to-one comparison */
        if (FALSE == is_two_switch_blocks_mirror(&(sb_info[ix][iy]), &(sb_info[jx][iy]))) {
          /* Nothing to do if the two switch blocks are not equivalent */
          continue;
        }
        /* configure the mirror of the second switch block */
        assign_switch_block_mirror(&(sb_info[ix][iy]), &(sb_info[jx][iy]));
      }
    }
  }
 
  return;
}

/* Validate the mirror of a switch block is the upstream
 * with NULL mirror 
 */
boolean validate_one_switch_block_mirror(t_sb* cur_sb) {
  if (NULL == cur_sb->mirror) {
    /* This is the upstream */
    return TRUE;
  }
  /* If the upstream has a mirror, there is a bug */
  if (NULL != cur_sb->mirror->mirror) {
    return FALSE;
  }
  return TRUE;
}

/* Validate the mirror of a switch block is the upstream
 * with NULL mirror 
 */
boolean validate_one_connection_block_mirror(t_cb* cur_cb) {
  if (NULL == cur_cb->mirror) {
    /* This is the upstream */
    return TRUE;
  }
  /* If the upstream has a mirror, there is a bug */
  if (NULL != cur_cb->mirror->mirror) {
    return FALSE;
  }
  return TRUE;
}

/* update the mirror of each switch block */
void update_one_switch_block_mirror(t_sb* cur_sb) {

  if (NULL == cur_sb->mirror) {
    /* This is the upstream */
    return;
  }

  /* Assign mirror of the first SB to the second SB */
  /* traceback to the upstream */
  t_sb* temp = cur_sb->mirror;
  while (NULL != temp->mirror) {
    /* go to the next */
    temp = temp->mirror;
  }
  /* We reach the upstream, ensure its mirror is empty */
  assert(NULL == temp->mirror);
  cur_sb->mirror = temp;  

  return;
}

/* update the mirror of each switch block */
void update_one_connection_block_mirror(t_cb* cur_cb) {

  if (NULL == cur_cb->mirror) {
    /* This is the upstream */
    return;
  }

  /* Assign mirror of the first SB to the second SB */
  /* traceback to the upstream */
  t_cb* temp = cur_cb->mirror;
  while (NULL != temp->mirror) {
    /* go to the next */
    temp = temp->mirror;
  }
  /* We reach the upstream, ensure its mirror is empty */
  assert(NULL == temp->mirror);
  cur_cb->mirror = temp;  

  return;
}


/* Validate the mirror of each switch block is the upstream */
boolean validate_mirror_switch_blocks() {
  boolean ret = TRUE;

  /* Walkthrough each column, and find mirrors */
  for (int ix = 0; ix < (nx + 1); ++ix) {
    for (int iy = 0; iy < (ny + 1); ++iy) {
      if (FALSE == validate_one_switch_block_mirror(&(sb_info[ix][iy]))) {
        ret = FALSE;
      }
    }
  }

  return ret;
}


/* Validate the mirror of each connection block is the upstream */
boolean validate_mirror_connection_blocks() {
  boolean ret = TRUE;

  /* X - channels [1...nx][0..ny]*/
  for (int iy = 0; iy < (ny + 1); iy++) {
    for (int ix = 1; ix < (nx + 1); ix++) {
      if (FALSE == validate_one_connection_block_mirror(&(cbx_info[ix][iy]))) {
        ret = FALSE;
      }
    }
  }

  /* Y - channels [1...ny][0..nx]*/
  for (int ix = 0; ix < (nx + 1); ix++) {
    for (int iy = 1; iy < (ny + 1); iy++) {
      if (FALSE == validate_one_connection_block_mirror(&(cby_info[ix][iy]))) {
        ret = FALSE;
      }
    }
  }

  return ret;
}


/* Validate the mirror of each switch block is the upstream */
void update_mirror_switch_blocks() {

  /* Walkthrough each column, and find mirrors */
  for (int ix = 0; ix < (nx + 1); ++ix) {
    for (int iy = 0; iy < (ny + 1); ++iy) {
      update_one_switch_block_mirror(&(sb_info[ix][iy]));
    }
  }

  return;
}

/* Validate the mirror of each connection block is the upstream */
void update_mirror_connection_blocks() {

  /* X - channels [1...nx][0..ny]*/
  for (int iy = 0; iy < (ny + 1); iy++) {
    for (int ix = 1; ix < (nx + 1); ix++) {
      update_one_connection_block_mirror(&(cbx_info[ix][iy]));
    }
  }

  /* Y - channels [1...ny][0..nx]*/
  for (int ix = 0; ix < (nx + 1); ix++) {
    for (int iy = 1; iy < (ny + 1); iy++) {
      update_one_connection_block_mirror(&(cby_info[ix][iy]));
    }
  }

  return;
}


void print_mirror_switch_block_stats() {
  int num_mirror_sb = 0;

  /* Walkthrough each column, and find mirrors */
  for (int ix = 0; ix < (nx + 1); ++ix) {
    for (int iy = 0; iy < (ny + 1); ++iy) {
      if (NULL == sb_info[ix][iy].mirror) {
        num_mirror_sb++;
      }
    }
  }

  /* Print stats */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent switch blocks from %d switch blocks.\n",
             num_mirror_sb, (nx + 1) * (ny + 1) );

  return;
}

void print_mirror_connection_block_stats() {
  int num_mirror_cbx = 0;
  int num_mirror_cby = 0;

  /* X - channels [1...nx][0..ny]*/
  for (int iy = 0; iy < (ny + 1); iy++) {
    for (int ix = 1; ix < (nx + 1); ix++) {
      if (NULL == cbx_info[ix][iy].mirror) {
        num_mirror_cbx++;
      }
    }
  }

  /* Y - channels [1...ny][0..nx]*/
  for (int ix = 0; ix < (nx + 1); ix++) {
    for (int iy = 1; iy < (ny + 1); iy++) {
      if (NULL == cby_info[ix][iy].mirror) {
        num_mirror_cby++;
      }
    }
  }

  /* Print stats */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent connection blocks from %d X-channel connection blocks.\n",
             num_mirror_cbx, (nx + 0) * (ny + 1) );

  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent connection blocks from %d Y-channel connection blocks.\n",
             num_mirror_cby, (nx + 1) * (ny + 0) );

  return;
}

void identify_mirror_switch_blocks() {

  /* Assign the mirror of each switch block */
  assign_mirror_switch_blocks();

  /* Ensure all the mirror are the upstream */
  update_mirror_switch_blocks();

  /* Validate the mirror of switch blocks, everyone should be the upstream */
  assert(TRUE == validate_mirror_switch_blocks());

  /* print the stats */
  print_mirror_switch_block_stats();

  return;
}

/* Idenify mirror connection blocks 
 * Check each two connection blocks: 
 * 1. Number of channel/opin/ipin rr_nodes are same 
 * For channel rr_nodes
 * 2. check if their track_ids (ptc_num) are same
 * 3. Check if the switches (ids) are same
 * For opin/ipin rr_nodes, 
 * 4. check if their parent type_descriptors same, 
 * 5. check if pin class id and pin id are same 
 * If all above are satisfied, the two switch blocks are mirrors!
 */
boolean is_two_connection_blocks_mirror(t_cb* src, t_cb* des) {

  /* check the numbers of sides */
  if (src->num_sides != des->num_sides) {
    return FALSE;
  }

  /* check the numbers/directionality of channel rr_nodes */
  for (int side = 0; side < src->num_sides; ++side) {
    /* Ensure we have the same channel width on this side */
    if (src->chan_width[side] != des->chan_width[side]) {
      return FALSE;
    }
    for (int itrack = 0; itrack < src->chan_width[side]; ++itrack) {
      /* Check the directionality of each node */
      if (src->chan_rr_node_direction[side][itrack] != des->chan_rr_node_direction[side][itrack]) {
        return FALSE;
      }
      /* Check the track_id of each node */
      if (src->chan_rr_node[side][itrack]->ptc_num != des->chan_rr_node[side][itrack]->ptc_num) {
        return FALSE;
      }
    }
  }
  
  /* check the equivalence of ipins */
  for (int side = 0; side < src->num_sides; ++side) {
    /* Ensure we have the same number of IPINs on this side */
    if (src->num_ipin_rr_nodes[side] != des->num_ipin_rr_nodes[side]) {
      return FALSE;
    }
    for (int inode = 0; inode < src->num_ipin_rr_nodes[side]; ++inode) {
      if (FALSE == is_two_cb_rr_nodes_mirror(src, des, 
                                             src->ipin_rr_node[side][inode],
                                             des->ipin_rr_node[side][inode])) {
        return FALSE;
      }
    }
  }

  /* Make sure the number of conf bits are the same */
  if ( (src->conf_bits_msb - src->conf_bits_lsb) 
     != (des->conf_bits_msb - des->conf_bits_lsb)) {
    return FALSE;
  }
 
  return TRUE;
}

void assign_mirror_connection_blocks() {

  /* X - channels [1...nx][0..ny]*/
  for (int iy = 0; iy < (ny + 1); iy++) {
    for (int ix = 1; ix < (nx + 1); ix++) {
      for (int jx = ix; jx < (nx + 1); jx++) {
        /* bypass the same one */
        if (ix == jx) {
          continue;
        }
        /* Do one-to-one comparison */
        if (FALSE == is_two_connection_blocks_mirror(&(cbx_info[ix][iy]), &(cbx_info[jx][iy]))) {
          /* Nothing to do if the two switch blocks are not equivalent */
          continue;
        }
        /* configure the mirror of the second switch block */
        assign_connection_block_mirror(&(cbx_info[ix][iy]), &(cbx_info[jx][iy]));
      }
    }
  }

  for (int ix = 1; ix < (nx + 1); ix++) {
    for (int iy = 0; iy < (ny + 1); iy++) {
      for (int jy = iy; jy < (ny + 1); jy++) {
        /* bypass the same one */
        if (iy == jy) {
          continue;
        }
        /* Do one-to-one comparison */
        if (FALSE == is_two_connection_blocks_mirror(&(cbx_info[ix][iy]), &(cbx_info[ix][jy]))) {
          /* Nothing to do if the two switch blocks are not equivalent */
          continue;
        }
        /* configure the mirror of the second switch block */
        assign_connection_block_mirror(&(cbx_info[ix][iy]), &(cbx_info[ix][jy]));
      }
    }
  }

  /* Y - channels [1...ny][0..nx]*/
  for (int ix = 0; ix < (nx + 1); ix++) {
    for (int iy = 1; iy < (ny + 1); iy++) {
      for (int jy = iy; jy < (ny + 1); jy++) {
        /* bypass the same one */
        if (iy == jy) {
          continue;
        }
        /* Do one-to-one comparison */
        if (FALSE == is_two_connection_blocks_mirror(&(cby_info[ix][iy]), &(cby_info[ix][jy]))) {
          /* Nothing to do if the two switch blocks are not equivalent */
          continue;
        }
        /* configure the mirror of the second switch block */
        assign_connection_block_mirror(&(cby_info[ix][iy]), &(cby_info[ix][jy]));
      }
    }
  }

  for (int iy = 1; iy < (ny + 1); iy++) {
    for (int ix = 0; ix < (nx + 1); ix++) {
      for (int jx = ix; jx < (nx + 1); jx++) {
        /* bypass the same one */
        if (ix == jx) {
          continue;
        }
        /* Do one-to-one comparison */
        if (FALSE == is_two_connection_blocks_mirror(&(cby_info[ix][iy]), &(cby_info[jx][iy]))) {
          /* Nothing to do if the two switch blocks are not equivalent */
          continue;
        }
        /* configure the mirror of the second switch block */
        assign_connection_block_mirror(&(cby_info[ix][iy]), &(cby_info[jx][iy]));
      }
    }
  }

  return;
}

/* Idenify mirror Connection blocks */
void identify_mirror_connection_blocks() {

  /* Assign the mirror of each switch block */
  assign_mirror_connection_blocks();

  /* Ensure all the mirror are the upstream */
  update_mirror_connection_blocks();

  /* Validate the mirror of switch blocks, everyone should be the upstream */
  assert(TRUE == validate_mirror_connection_blocks());

  /* print the stats */
  print_mirror_connection_block_stats();

  return;
}

/* Build a RRChan Object with the given channel type and coorindators */
RRChan build_one_rr_chan(t_rr_type chan_type, size_t chan_x, size_t chan_y,
                         int LL_num_rr_nodes, t_rr_node* LL_rr_node, 
                         t_ivec*** LL_rr_node_indices, int num_segments,
                         t_rr_indexed_data* LL_rr_indexed_data) {
  int chan_width = 0;
  t_rr_node** chan_rr_nodes = NULL;

  /* Create a rr_chan object and check if it is unique in the graph */
  RRChan rr_chan;
  /* Fill the information */
  rr_chan.set_type(chan_type); 

  /* Collect rr_nodes for this channel */
  chan_rr_nodes = get_chan_rr_nodes(&chan_width, chan_type, chan_x, chan_y,
                                    LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  /* Reserve */
  /* rr_chan.reserve_node(size_t(chan_width)); */

  /* Fill the rr_chan */  
  for (size_t itrack = 0; itrack < size_t(chan_width); ++itrack) {
    int cost_index = chan_rr_nodes[itrack]->cost_index;
    int iseg = LL_rr_indexed_data[cost_index].seg_index; 
    /* Check */
    assert((!(iseg < 0))&&(iseg < num_segments));

    rr_chan.add_node(chan_rr_nodes[itrack], size_t(iseg));
  }

  /* Free rr_nodes */
  my_free(chan_rr_nodes);

  return rr_chan;
}

void print_device_rr_chan_stats(DeviceRRChan& LL_device_rr_chan) {
  /* Print stats */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent routing channel from %d X-direction routing channels.\n",
             LL_device_rr_chan.get_num_modules(CHANX), (nx + 0) * (ny + 1) );

  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent routing channel from %d Y-direction routing channels.\n",
             LL_device_rr_chan.get_num_modules(CHANY), (nx + 1) * (ny + 0) );

}

/* Build the list of unique routing channels */
DeviceRRChan build_device_rr_chan(int LL_num_rr_nodes, t_rr_node* LL_rr_node, 
                                  t_ivec*** LL_rr_node_indices, int num_segments,
                                  t_rr_indexed_data* LL_rr_indexed_data) {
  /* Create an object of DeviceRRChan */
  DeviceRRChan LL_device_rr_chan;

  /* Initialize array of rr_chan inside the device */
  LL_device_rr_chan.init_module_ids(nx + 1, ny + 1);

  /* For X-direction routing channel */
  for (size_t iy = 0; iy < size_t(ny + 1); iy++) {
    for (size_t ix = 1; ix < size_t(nx + 1); ix++) {
      /* Create a rr_chan object and check if it is unique in the graph */
      RRChan rr_chan = build_one_rr_chan(CHANX, ix, iy, 
                                         LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                         num_segments, LL_rr_indexed_data);
      /* check and add this rr_chan to the mirror list */ 
      LL_device_rr_chan.add_one_chan_module(CHANX, ix, iy, rr_chan);
    }
  }

  /* For X-direction routing channel */
  for (size_t ix = 0; ix < size_t(nx + 1); ix++) {
    for (size_t iy = 1; iy < size_t(ny + 1); iy++) {
      /* Create a rr_chan object and check if it is unique in the graph */
      RRChan rr_chan = build_one_rr_chan(CHANY, ix, iy, 
                                         LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                         num_segments, LL_rr_indexed_data);
      /* check and add this rr_chan to the mirror list */ 
      LL_device_rr_chan.add_one_chan_module(CHANY, ix, iy, rr_chan);
    }
  }

  print_device_rr_chan_stats(LL_device_rr_chan);

  return LL_device_rr_chan; 
}


/* Rotatable will be done in the next step 
void identify_rotatable_switch_blocks(); 
void identify_rotatable_connection_blocks(); 
*/
