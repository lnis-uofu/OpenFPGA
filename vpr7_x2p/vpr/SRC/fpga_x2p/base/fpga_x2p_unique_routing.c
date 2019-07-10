/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    fpga_x2p_unique_routing.c
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/25  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file contains builders for the data structures
 *  1. RRGSB: General Switch Block (GSB).
 *  2. RRChan: Generic routing channels
 *  We also include functions to identify unique modules of
 *  Switch Blocks and Connection Blocks based on the data structures
 *  t_sb and t_cb
 ***********************************************************************/

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
#include "write_rr_blocks.h"
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
    get_rr_node_side_and_index_in_cb_info(src_rr_node->drive_rr_nodes[inode], *src_cb, IN_PORT, &src_node_side, &src_node_id);
    get_rr_node_side_and_index_in_cb_info(des_rr_node->drive_rr_nodes[inode], *des_cb, IN_PORT, &des_node_side, &des_node_id);
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
    if (src->num_opin_rr_nodes[side] != des->num_opin_rr_nodes[side]) {
      return FALSE;
    }
  }

  /* Make sure the number of conf bits are the same 
   * TODO: the check should be done when conf_bits are initialized when creating SBs
  if ( (src->conf_bits_msb - src->conf_bits_lsb) 
     != (des->conf_bits_msb - des->conf_bits_lsb)) {
    return FALSE;
  }
  */

  return TRUE;
}

/* Walk through all the switch blocks,
 * Make one-to-one comparison,
 * If we have a pair, update the 1st SB to be the base and label the 2nd as a mirror
 * If the 1st SB is already a mirror to another, we will trace back to the upstream base and update the 2nd SB
 */
void assign_mirror_switch_blocks() {
  /* A vector of coordinators of  mirrors */
  std::vector<t_sb*> mirror;

  /* Walkthrough each column, and find mirrors */
  for (int ix = 0; ix < (nx + 1); ++ix) {
    for (int iy = 0; iy < (ny + 1); ++iy) {
      bool is_unique_mirror = true;
      for (size_t imirror = 0; imirror < mirror.size(); ++imirror) {
        /* Do one-to-one comparison */
        if (TRUE == is_two_switch_blocks_mirror(mirror[imirror], &(sb_info[ix][iy]))) {
          /* Find two equivalent switch blocks */
          is_unique_mirror = false;
          /* configure the mirror of the second switch block */
          assign_switch_block_mirror(mirror[imirror], &(sb_info[ix][iy]));
          break;
        }
      }
      if (true == is_unique_mirror) {
        /* add to unique mirror list */
        mirror.push_back(&(sb_info[ix][iy]));
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
  /* update_mirror_switch_blocks(); */

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
  std::vector<t_cb*> cbx_mirror;
  std::vector<t_cb*> cby_mirror;

  /* Make sure a clean start */
  cbx_mirror.clear();

  /* X - channels [1...nx][0..ny]*/
  for (int iy = 0; iy < (ny + 1); ++iy) {
    for (int ix = 1; ix < (nx + 1); ++ix) {
      bool is_unique_mirror = true;
      for (size_t id = 0; id < cbx_mirror.size(); ++id) {
        /* Do one-to-one comparison */
        if (TRUE == is_two_connection_blocks_mirror(cbx_mirror[id], &(cbx_info[ix][iy]))) {
          /* configure the mirror of the second switch block */
          assign_connection_block_mirror(cbx_mirror[id], &(cbx_info[ix][iy]));
          /* Raise a flag and later add it to the unique mirror list if the two switch blocks are not equivalent */
          is_unique_mirror = false;
          break;
        }
      }
      /* Update the mirror list if necessary */
      if (true == is_unique_mirror) {
        cbx_mirror.push_back(&(cbx_info[ix][iy]));
      }
    }
  }

  /* Make sure a clean start */
  cby_mirror.clear();

  /* Y - channels [1...ny][0..nx]*/
  for (int ix = 0; ix < (nx + 1); ++ix) {
    for (int iy = 1; iy < (ny + 1); ++iy) {
      bool is_unique_mirror = true;
      for (size_t id = 0; id < cby_mirror.size(); ++id) {
        /* Do one-to-one comparison */
        if (TRUE == is_two_connection_blocks_mirror(cby_mirror[id], &(cby_info[ix][iy]))) {
          /* configure the mirror of the second switch block */
          assign_connection_block_mirror(cby_mirror[id], &(cby_info[ix][iy]));
          /* Raise a flag and later add it to the unique mirror list if the two switch blocks are not equivalent */
          is_unique_mirror = false;
          break;
        }
      }
      /* Update the mirror list if necessary */
      if (true == is_unique_mirror) {
        cby_mirror.push_back(&(cby_info[ix][iy]));
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
  /*
  update_mirror_connection_blocks();
  */

  /* Validate the mirror of switch blocks, everyone should be the upstream */
  assert(TRUE == validate_mirror_connection_blocks());

  /* print the stats */
  print_mirror_connection_block_stats();

  return;
}

/* Build a RRChan Object with the given channel type and coorindators */
static 
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

/* Build a General Switch Block (GSB) 
 * which includes:
 * [I] A Switch Box subckt consists of following ports:
 * 1. Channel Y [x][y] inputs 
 * 2. Channel X [x+1][y] inputs
 * 3. Channel Y [x][y-1] outputs
 * 4. Channel X [x][y] outputs
 * 5. Grid[x][y+1] Right side outputs pins
 * 6. Grid[x+1][y+1] Left side output pins
 * 7. Grid[x+1][y+1] Bottom side output pins
 * 8. Grid[x+1][y] Top side output pins
 * 9. Grid[x+1][y] Left side output pins
 * 10. Grid[x][y] Right side output pins
 * 11. Grid[x][y] Top side output pins
 * 12. Grid[x][y+1] Bottom side output pins
 *
 *    --------------          --------------
 *    |            |   CBY    |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *     ChanX & CBX  | Switch |     ChanX 
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 * For channels chanY with INC_DIRECTION on the top side, they should be marked as outputs
 * For channels chanY with DEC_DIRECTION on the top side, they should be marked as inputs
 * For channels chanY with INC_DIRECTION on the bottom side, they should be marked as inputs
 * For channels chanY with DEC_DIRECTION on the bottom side, they should be marked as outputs
 * For channels chanX with INC_DIRECTION on the left side, they should be marked as inputs
 * For channels chanX with DEC_DIRECTION on the left side, they should be marked as outputs
 * For channels chanX with INC_DIRECTION on the right side, they should be marked as outputs
 * For channels chanX with DEC_DIRECTION on the right side, they should be marked as inputs
 *
 * [II] A X-direction Connection Block [x][y]
 * The connection block shares the same routing channel[x][y] with the Switch Block
 * We just need to fill the ipin nodes at TOP and BOTTOM sides 
 * as well as properly fill the ipin_grid_side information
 * [III] A Y-direction Connection Block [x][y+1]
 * The connection block shares the same routing channel[x][y+1] with the Switch Block
 * We just need to fill the ipin nodes at LEFT and RIGHT sides 
 * as well as properly fill the ipin_grid_side information
 */
static 
RRGSB build_rr_gsb(DeviceCoordinator& device_range, 
                   size_t sb_x, size_t sb_y, 
                   int LL_num_rr_nodes, t_rr_node* LL_rr_node, 
                   t_ivec*** LL_rr_node_indices, int num_segments,
                   t_rr_indexed_data* LL_rr_indexed_data) {
  /* Create an object to return */
  RRGSB rr_gsb;

  /* Check */
  assert(sb_x <= device_range.get_x()); 
  assert(sb_y <= device_range.get_y()); 

  /* Coordinator initialization */
  rr_gsb.set_coordinator(sb_x, sb_y);

  /* Basic information*/
  rr_gsb.init_num_sides(4); /* Fixed number of sides */

  /* Find all rr_nodes of channels */
  /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    /* Local variables inside this for loop */
    Side side_manager(side);
    DeviceCoordinator coordinator = rr_gsb.get_side_block_coordinator(side_manager.get_side());
    size_t ix = coordinator.get_x(); 
    size_t iy = coordinator.get_y(); 
    RRChan rr_chan;
    int temp_num_opin_rr_nodes[2] = {0,0};
    t_rr_node** temp_opin_rr_node[2] = {NULL, NULL};
    enum e_side opin_grid_side[2] = {NUM_SIDES, NUM_SIDES};
    enum PORTS chan_dir_to_port_dir_mapping[2] = {OUT_PORT, IN_PORT}; /* 0: INC_DIRECTION => ?; 1: DEC_DIRECTION => ? */

    switch (side) {
    case TOP: /* TOP = 0 */
      /* For the bording, we should take special care */
      if (sb_y == device_range.get_y()) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_rr_chan(CHANY, ix, iy, 
                                  LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                  num_segments, LL_rr_indexed_data);
      chan_dir_to_port_dir_mapping[0] = OUT_PORT; /* INC_DIRECTION => OUT_PORT */
      chan_dir_to_port_dir_mapping[1] =  IN_PORT; /* DEC_DIRECTION => IN_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* Include Grid[x][y+1] RIGHT side outputs pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x, sb_y + 1, 1,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Include Grid[x+1][y+1] Left side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x + 1, sb_y + 1, 3,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

      /* Assign grid side of OPIN */
      /* Grid[x][y+1] RIGHT side outputs pins */
      opin_grid_side[0] = RIGHT;
      /* Grid[x+1][y+1] left side outputs pins */
      opin_grid_side[1] = LEFT; 
      break;
    case RIGHT: /* RIGHT = 1 */
      /* For the bording, we should take special care */
      if (sb_x == device_range.get_x()) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Collect rr_nodes for Tracks for top: chany[x][y+1] */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_rr_chan(CHANX, ix, iy, 
                                  LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                  num_segments, LL_rr_indexed_data);
      chan_dir_to_port_dir_mapping[0] = OUT_PORT; /* INC_DIRECTION => OUT_PORT */
      chan_dir_to_port_dir_mapping[1] =  IN_PORT; /* DEC_DIRECTION => IN_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* include Grid[x+1][y+1] Bottom side output pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x + 1, sb_y + 1, 2,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* include Grid[x+1][y] Top side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x + 1, sb_y, 0,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Assign grid side of OPIN */
      /* Grid[x+1][y+1] BOTTOM side outputs pins */
      opin_grid_side[0] = BOTTOM;
      /* Grid[x+1][y] TOP side outputs pins */
      opin_grid_side[1] = TOP;
      break;
    case BOTTOM: /* BOTTOM = 2*/
      /* For the bording, we should take special care */
      if (sb_y == 0) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Collect rr_nodes for Tracks for bottom: chany[x][y] */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_rr_chan(CHANY, ix, iy, 
                                  LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                  num_segments, LL_rr_indexed_data);
      chan_dir_to_port_dir_mapping[0] =  IN_PORT; /* INC_DIRECTION => IN_PORT */
      chan_dir_to_port_dir_mapping[1] = OUT_PORT; /* DEC_DIRECTION => OUT_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* include Grid[x+1][y] Left side output pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x + 1, sb_y, 3,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* include Grid[x][y] Right side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x, sb_y, 1,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Assign grid side of OPIN */
      /* Grid[x+1][y] LEFT side outputs pins */
      opin_grid_side[0] = LEFT;
      /* Grid[x][y] RIGHT side outputs pins */
      opin_grid_side[1] = RIGHT;
      break;
    case LEFT: /* LEFT = 3 */
      /* For the bording, we should take special care */
      if (sb_x == 0) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Collect rr_nodes for Tracks for left: chanx[x][y] */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_rr_chan(CHANX, ix, iy, 
                                  LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                  num_segments, LL_rr_indexed_data);
      chan_dir_to_port_dir_mapping[0] =  IN_PORT; /* INC_DIRECTION => IN_PORT */
      chan_dir_to_port_dir_mapping[1] = OUT_PORT; /* DEC_DIRECTION => OUT_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* include Grid[x][y+1] Bottom side outputs pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x, sb_y + 1, 2,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* include Grid[x][y] Top side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x, sb_y, 0,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

      /* Grid[x][y+1] BOTTOM side outputs pins */
      opin_grid_side[0] = BOTTOM;
      /* Grid[x][y] TOP side outputs pins */
      opin_grid_side[1] = TOP;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d])Invalid side index!\n", 
                 __FILE__, __LINE__);
      exit(1);
    }

    /* Organize a vector of port direction */
    if (0 < rr_chan.get_chan_width()) {
      std::vector<enum PORTS> rr_chan_dir;
      rr_chan_dir.resize(rr_chan.get_chan_width());
      for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
        /* Identify the directionality, record it in rr_node_direction */
        if (INC_DIRECTION == rr_chan.get_node(itrack)->direction) {
          rr_chan_dir[itrack] = chan_dir_to_port_dir_mapping[0];
        } else {
          assert (DEC_DIRECTION == rr_chan.get_node(itrack)->direction);
          rr_chan_dir[itrack] = chan_dir_to_port_dir_mapping[1];
        }
      }
      /* Fill chan_rr_nodes */
      rr_gsb.add_chan_node(side_manager.get_side(), rr_chan, rr_chan_dir);
    }

    /* Fill opin_rr_nodes */
    /* Copy from temp_opin_rr_node to opin_rr_node */
    for (int inode = 0; inode < temp_num_opin_rr_nodes[0]; ++inode) {
      /* Grid[x+1][y+1] Bottom side outputs pins */
      rr_gsb.add_opin_node(temp_opin_rr_node[0][inode], side_manager.get_side(), opin_grid_side[0]);
    }
    for (int inode = 0; inode < temp_num_opin_rr_nodes[1]; ++inode) {
      /* Grid[x+1][y] TOP side outputs pins */
      rr_gsb.add_opin_node(temp_opin_rr_node[1][inode], side_manager.get_side(), opin_grid_side[1]);
    }

    /* Clean ipin_rr_nodes */
    /* We do not have any IPIN for a Switch Block */
    rr_gsb.clear_ipin_nodes(side_manager.get_side());

    /* Free */
    temp_num_opin_rr_nodes[0] = 0;
    my_free(temp_opin_rr_node[0]);
    temp_num_opin_rr_nodes[1] = 0;
    my_free(temp_opin_rr_node[1]);
    /* Set them to NULL, avoid double free errors */
    temp_opin_rr_node[0] = NULL;
    temp_opin_rr_node[1] = NULL;
    opin_grid_side[0] = NUM_SIDES;
    opin_grid_side[1] = NUM_SIDES;
  }

  /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    /* Local variables inside this for loop */
    Side side_manager(side);
    size_t ix; 
    size_t iy; 
    enum e_side chan_side;
    int num_temp_ipin_rr_nodes = 0;
    t_rr_node** temp_ipin_rr_node = NULL;
    enum e_side ipin_rr_node_grid_side;
   
    switch (side) {
    case TOP: /* TOP = 0 */
      /* For the bording, we should take special care */
      /* Check if left side chan width is 0 or not */
      chan_side = LEFT;
      /* Build the connection block: ipin and ipin_grid_side */
      /* BOTTOM side INPUT Pins of Grid[x][y+1] */
      ix = rr_gsb.get_sb_x(); 
      iy = rr_gsb.get_sb_y() + 1; 
      ipin_rr_node_grid_side = BOTTOM; 
      break;
    case RIGHT: /* RIGHT = 1 */
      /* For the bording, we should take special care */
      /* Check if TOP side chan width is 0 or not */
      chan_side = TOP;
      /* Build the connection block: ipin and ipin_grid_side */
      /* LEFT side INPUT Pins of Grid[x+1][y+1] */
      ix = rr_gsb.get_sb_x() + 1; 
      iy = rr_gsb.get_sb_y() + 1; 
      ipin_rr_node_grid_side = LEFT; 
      break;
    case BOTTOM: /* BOTTOM = 2*/
      /* For the bording, we should take special care */
      /* Check if left side chan width is 0 or not */
      chan_side = LEFT;
      /* Build the connection block: ipin and ipin_grid_side */
      /* TOP side INPUT Pins of Grid[x][y] */
      ix = rr_gsb.get_sb_x(); 
      iy = rr_gsb.get_sb_y(); 
      ipin_rr_node_grid_side = TOP; 
      break;
    case LEFT: /* LEFT = 3 */
      /* For the bording, we should take special care */
      /* Check if left side chan width is 0 or not */
      chan_side = TOP;
      /* Build the connection block: ipin and ipin_grid_side */
      /* RIGHT side INPUT Pins of Grid[x][y+1] */
      ix = rr_gsb.get_sb_x(); 
      iy = rr_gsb.get_sb_y() + 1; 
      ipin_rr_node_grid_side = RIGHT; 
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d])Invalid side index!\n", 
                 __FILE__, __LINE__);
      exit(1);
    }
    
    /* If there is no channel at this side, we skip ipin_node annotation */
    if (0 == rr_gsb.get_chan_width(chan_side)) {
      continue;
    }
    /* Collect IPIN rr_nodes*/ 
    temp_ipin_rr_node = get_grid_side_pin_rr_nodes(&(num_temp_ipin_rr_nodes), 
                                                   IPIN, ix, iy, ipin_rr_node_grid_side,
                                                   LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    /* Fill the ipin nodes of RRGSB */ 
    for (int inode = 0; inode < num_temp_ipin_rr_nodes; ++inode) {
      rr_gsb.add_ipin_node(temp_ipin_rr_node[inode], side_manager.get_side(), ipin_rr_node_grid_side);
    }
    /* Free */
    num_temp_ipin_rr_nodes = 0;
    my_free(temp_ipin_rr_node);
  }

  return rr_gsb;
}

/* sort drive_rr_nodes of a rr_node inside rr_gsb subject to the index of rr_gsb array */
static  
void sort_rr_gsb_one_ipin_node_drive_rr_nodes(const RRGSB& rr_gsb, 
                                              t_rr_node* ipin_node, 
                                              enum e_side ipin_chan_side) {
  /* Create a copy of the edges and switches of this node */
  std::vector<t_rr_node*> sorted_drive_nodes;
  std::vector<int> sorted_drive_switches;

  /* Ensure a clean start */
  sorted_drive_nodes.clear();
  sorted_drive_switches.clear();

  /* Build the vectors w.r.t. to the order of node_type and ptc_num */
  for (int i_from_node = 0; i_from_node < ipin_node->num_drive_rr_nodes; ++i_from_node) {
    /* For blank edges: directly push_back */
    if (0 == sorted_drive_nodes.size()) {
      sorted_drive_nodes.push_back(ipin_node->drive_rr_nodes[i_from_node]);
      sorted_drive_switches.push_back(ipin_node->drive_switches[i_from_node]);
      continue;
    }

    /* Start sorting since the edges are not empty */
    size_t insert_pos = sorted_drive_nodes.size(); /* the pos to insert. By default, it is the last element */
    for (size_t j_from_node = 0; j_from_node < sorted_drive_nodes.size(); ++j_from_node) {
      /* Sort by node_type and ptc_num */
      if (ipin_node->drive_rr_nodes[i_from_node]->type < sorted_drive_nodes[j_from_node]->type) {
        /* iedge should be ahead of jedge */
        insert_pos = j_from_node;
        break; /* least type should stay in the front of the vector */
      } else if (ipin_node->drive_rr_nodes[i_from_node]->type 
              == sorted_drive_nodes[j_from_node]->type) {
        int i_from_node_track_index = rr_gsb.get_chan_node_index(ipin_chan_side, ipin_node->drive_rr_nodes[i_from_node]); 
        int j_from_node_track_index = rr_gsb.get_chan_node_index(ipin_chan_side, sorted_drive_nodes[j_from_node]); 
        /* We must have a valide node index */
        assert ( (-1 != i_from_node_track_index) && (-1 != j_from_node_track_index) );
        /* Now a lower ptc_num will win */ 
        if ( i_from_node_track_index < j_from_node_track_index ) { 
          insert_pos = j_from_node;
          break; /* least type should stay in the front of the vector */
        }
      }
    }
    /* We find the position, inserted to the vector */
    sorted_drive_nodes.insert(sorted_drive_nodes.begin() + insert_pos, ipin_node->drive_rr_nodes[i_from_node]); 
    sorted_drive_switches.insert(sorted_drive_switches.begin() + insert_pos, ipin_node->drive_switches[i_from_node]); 
  }

  /* Overwrite the edges and switches with sorted numbers */
  for (size_t iedge = 0; iedge < sorted_drive_nodes.size(); ++iedge) {
    ipin_node->drive_rr_nodes[iedge] = sorted_drive_nodes[iedge];
  }
  for (size_t iedge = 0; iedge < sorted_drive_switches.size(); ++iedge) {
    ipin_node->drive_switches[iedge] = sorted_drive_switches[iedge];
  }

  return;
}

/* sort drive_rr_nodes of a rr_node inside rr_gsb subject to the index of rr_gsb array */
static  
void sort_rr_gsb_one_chan_node_drive_rr_nodes(const RRGSB& rr_gsb, 
                                              enum e_side chan_side, 
                                              size_t track_id) {

  /* If this is a passing wire, we return directly.
   * The passing wire will be handled in other GSBs
   */
  if (true == rr_gsb.is_sb_node_passing_wire(chan_side, track_id)) {
     return;
  }

  /* Get the chan_node */
  t_rr_node* chan_node = rr_gsb.get_chan_node(chan_side, track_id);

  /* Create a copy of the edges and switches of this node */
  std::vector<t_rr_node*> sorted_drive_nodes;
  std::vector<int> sorted_drive_switches;

  /* Ensure a clean start */
  sorted_drive_nodes.clear();
  sorted_drive_switches.clear();

  /* Build the vectors w.r.t. to the order of node_type and ptc_num */
  for (int i_from_node = 0; i_from_node < chan_node->num_drive_rr_nodes; ++i_from_node) {
    /* For blank edges: directly push_back */
    if (0 == sorted_drive_nodes.size()) {
      sorted_drive_nodes.push_back(chan_node->drive_rr_nodes[i_from_node]);
      sorted_drive_switches.push_back(chan_node->drive_switches[i_from_node]);
      continue;
    }

    /* Start sorting since the edges are not empty */
    size_t insert_pos = sorted_drive_nodes.size(); /* the pos to insert. By default, it is the last element */
    for (size_t j_from_node = 0; j_from_node < sorted_drive_nodes.size(); ++j_from_node) {
      /* Sort by node_type and ptc_num */
      if (chan_node->drive_rr_nodes[i_from_node]->type < sorted_drive_nodes[j_from_node]->type) {
        /* iedge should be ahead of jedge */
        insert_pos = j_from_node;
        break; /* least type should stay in the front of the vector */
      } else if (chan_node->drive_rr_nodes[i_from_node]->type 
              == sorted_drive_nodes[j_from_node]->type) {
        /* For channel node, we do not know the node direction
         * But we are pretty sure it is either IN_PORT or OUT_PORT
         * So we just try and find what is valid
         */
        enum e_side i_from_node_side = NUM_SIDES;
        int i_from_node_index = -1;
        rr_gsb.get_node_side_and_index(chan_node->drive_rr_nodes[i_from_node], 
                                       IN_PORT, &i_from_node_side, &i_from_node_index);
        /* check */
        if (! ( (NUM_SIDES != i_from_node_side) && (-1 != i_from_node_index) ) )
        assert ( (NUM_SIDES != i_from_node_side) && (-1 != i_from_node_index) );

        enum e_side j_from_node_side = NUM_SIDES;
        int j_from_node_index = -1;
        rr_gsb.get_node_side_and_index(sorted_drive_nodes[j_from_node], 
                                       IN_PORT, &j_from_node_side, &j_from_node_index);
        /* check */
        assert ( (NUM_SIDES != j_from_node_side) && (-1 != j_from_node_index) );
        /* Now a lower ptc_num will win */ 
        if ( i_from_node_index < j_from_node_index) { 
          insert_pos = j_from_node;
          break; /* least type should stay in the front of the vector */
        }
      }
    }
    /* We find the position, inserted to the vector */
    sorted_drive_nodes.insert(sorted_drive_nodes.begin() + insert_pos, chan_node->drive_rr_nodes[i_from_node]); 
    sorted_drive_switches.insert(sorted_drive_switches.begin() + insert_pos, chan_node->drive_switches[i_from_node]); 
  }

  /* Overwrite the edges and switches with sorted numbers */
  for (size_t iedge = 0; iedge < sorted_drive_nodes.size(); ++iedge) {
    chan_node->drive_rr_nodes[iedge] = sorted_drive_nodes[iedge];
  }
  for (size_t iedge = 0; iedge < sorted_drive_switches.size(); ++iedge) {
    chan_node->drive_switches[iedge] = sorted_drive_switches[iedge];
  }

  return;

}

/* sort drive_rr_nodes of each rr_node subject to the index of rr_gsb array */
static  
void sort_rr_gsb_drive_rr_nodes(const RRGSB& rr_gsb) {
  /* Sort the drive_rr_nodes for each rr_node */ 
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side gsb_side_manager(side);
    enum e_side gsb_side = gsb_side_manager.get_side();
    /* For IPIN node: sort drive_rr_nodes according to the index in the routing channels */
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(gsb_side); ++inode) {
      /* Get the chan side, so we have the routing tracks */
      enum e_side ipin_chan_side = rr_gsb.get_cb_chan_side(gsb_side);
      sort_rr_gsb_one_ipin_node_drive_rr_nodes(rr_gsb, 
                                               rr_gsb.get_ipin_node(gsb_side, inode),
                                               ipin_chan_side);
    } 
    /* For CHANX | CHANY node: sort drive_rr_nodes according to the index in the routing channels */
    for (size_t inode = 0; inode < rr_gsb.get_chan_width(gsb_side); ++inode) {
      /* Bypass IN_PORT */
      if (IN_PORT == rr_gsb.get_chan_node_direction(gsb_side, inode)) {
        continue;
      }
      /* Get the chan side, so we have the routing tracks */
      sort_rr_gsb_one_chan_node_drive_rr_nodes(rr_gsb, 
                                               gsb_side,
                                               inode);
    } 
  }

  return;
} 

/* Build a list of Switch blocks, each of which contains a collection of rr_nodes
 * We will maintain a list of unique switch blocks, which will be outputted as a Verilog module
 * Each switch block in the FPGA fabric will be an instance of these modules.
 * We maintain a map from each instance to each module
 */
DeviceRRGSB build_device_rr_gsb(boolean output_sb_xml, char* sb_xml_dir,
                                int LL_num_rr_nodes, t_rr_node* LL_rr_node, 
                                t_ivec*** LL_rr_node_indices, int num_segments,
                                t_rr_indexed_data* LL_rr_indexed_data) {
  /* Timer */
  clock_t t_start;
  clock_t t_end;
  float run_time_sec;

  clock_t t_start_profiling;
  clock_t t_end_profiling;
  float run_time_sec_profiling = 0.;

  /* Start time count */
  t_start = clock();

  /* Create an object */
  DeviceRRGSB LL_device_rr_gsb;

  /* Initialize */  
  DeviceCoordinator sb_range((size_t)nx, (size_t)ny);
  DeviceCoordinator reserve_range((size_t)nx + 1, (size_t)ny + 1);
  LL_device_rr_gsb.reserve(reserve_range);

  /* For each switch block, determine the size of array */
  for (size_t ix = 0; ix <= sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy <= sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = build_rr_gsb(sb_range, ix, iy,
                                         LL_num_rr_nodes, LL_rr_node, 
                                         LL_rr_node_indices, 
                                         num_segments, LL_rr_indexed_data);

      /* For profiling */
      t_start_profiling = clock();
      /* sort drive_rr_nodes */
      sort_rr_gsb_drive_rr_nodes(rr_gsb);
      /* End time count */
      t_end_profiling = clock();
      run_time_sec_profiling += (float)(t_end_profiling - t_start_profiling) / CLOCKS_PER_SEC;
 
      /* Add to device_rr_gsb */
      DeviceCoordinator sb_coordinator = rr_gsb.get_sb_coordinator();
      LL_device_rr_gsb.add_rr_gsb(sb_coordinator, rr_gsb);
    }
  }
  /* Report number of unique mirrors */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Backannotated %d switch blocks.\n",
             (nx + 1) * (ny + 1) );

  /* End time count */
  t_end = clock();
 
  run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, "Backannotation of Switch Block took %g seconds\n\n", run_time_sec);  

  vpr_printf(TIO_MESSAGE_INFO, "Edge sorting for Switch Block took %g seconds\n\n", run_time_sec_profiling);  


  if (TRUE == output_sb_xml) {
    create_dir_path(sb_xml_dir);
    write_device_rr_gsb_to_xml(sb_xml_dir, LL_device_rr_gsb);

    /* Skip rotating mirror searching */ 
    vpr_printf(TIO_MESSAGE_INFO, 
               "Output XML description of Switch Blocks to %s.\n",
               sb_xml_dir);

  }

  /* Build a list of unique modules for each Switch Block */
  /* Build a list of unique modules for each side of each Switch Block */
  LL_device_rr_gsb.build_unique_module();

  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %lu routing segments used by switch blocks.\n",
             LL_device_rr_gsb.get_num_segments());

  /* Report number of unique CB Modules */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent connection blocks from %d X-channel connection blocks.\n",
             LL_device_rr_gsb.get_num_cb_unique_module(CHANX), (nx + 0) * (ny + 1) );

  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent connection blocks from %d Y-channel connection blocks.\n",
             LL_device_rr_gsb.get_num_cb_unique_module(CHANY), (nx + 1) * (ny + 0) );


  /* Report number of unique SB modules */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Detect %d independent switch blocks from %d switch blocks.\n",
             LL_device_rr_gsb.get_num_sb_unique_module(), (nx + 1) * (ny + 1) );

  /* Report number of unique mirrors */
  for (size_t side = 0; side < LL_device_rr_gsb.get_max_num_sides(); ++side) {
    Side side_manager(side); 
    /* get segment ids */
    for (size_t iseg = 0; iseg < LL_device_rr_gsb.get_num_segments(); ++iseg) { 
      vpr_printf(TIO_MESSAGE_INFO, 
                 "For side %s, segment id %lu: Detect %d independent switch blocks from %d switch blocks.\n",
                 side_manager.c_str(), LL_device_rr_gsb.get_segment_id(iseg), 
                 LL_device_rr_gsb.get_num_sb_unique_submodule(side_manager.get_side(), iseg), 
                 (nx + 1) * (ny + 1) );
    }
  }

  /* End time count */
  t_end = clock();
 
  run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, "Routing architecture uniqifying took %g seconds\n\n", run_time_sec);  

  return LL_device_rr_gsb;
}

/************************************************************************
 * End of file : fpga_x2p_unique_routing.c 
 ***********************************************************************/
