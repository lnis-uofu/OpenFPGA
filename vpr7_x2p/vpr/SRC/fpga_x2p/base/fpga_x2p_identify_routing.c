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
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"


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
  return FALSE;
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
  if (NULL == cur_sb->mirror->mirror) {
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



void identify_mirror_switch_blocks() {
  /* Assign the mirror of each switch block */
  assign_mirror_switch_blocks();

  /* Ensure all the mirror are the upstream */
  update_mirror_switch_blocks();

  /* Validate the mirror of switch blocks, everyone should be the upstream */
  assert(TRUE == validate_mirror_switch_blocks());
}

/* Idenify mirror Connection blocks */
void identify_mirror_connection_blocks() {
  return;
}


/* Rotatable will be done in the next step 
void identify_rotatable_switch_blocks(); 
void identify_rotatable_connection_blocks(); 
*/
