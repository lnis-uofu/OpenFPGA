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
 * Filename:    tileable_rr_graph_gsb.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/19  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file contains a builder for track-to-track connections inside a 
 *  tileable General Switch Block (GSB).
 ***********************************************************************/

#include <cstdlib>
#include <cassert>

#include <vector>
#include <algorithm>

#include "vpr_types.h"
#include "globals.h"
#include "rr_graph_util.h"
#include "rr_graph2.h"

#include "rr_graph_builder_utils.h"
#include "tileable_chan_details_builder.h"
#include "tileable_rr_graph_gsb.h"

#include "fpga_x2p_backannotate_utils.h"

#include "my_free_fwd.h"

/************************************************************************
 * Internal data structures
 ***********************************************************************/
typedef std::vector<std::vector<int>> t_track_group;

/************************************************************************
 * A enumeration to list the status of a track inside a GSB
 * 1. start; 2. end; 3. passing
 * This is used to group tracks which ease the building of
 * track-to-track mapping matrix
 ***********************************************************************/
enum e_track_status {
  TRACK_START,
  TRACK_END,
  TRACK_PASS,
  NUM_TRACK_STATUS /* just a place holder to get the number of status */
};

/************************************************************************
 * Check if a track starts from this GSB or not 
 * (xlow, ylow) should be same as the GSB side coordinator 
 *
 * Check if a track ends at this GSB or not 
 * (xhigh, yhigh) should be same as the GSB side coordinator 
 ***********************************************************************/
static 
enum e_track_status determine_track_status_of_gsb(const RRGSB& rr_gsb, 
                                                  const enum e_side gsb_side, 
                                                  const size_t track_id) {
  enum e_track_status track_status = TRACK_PASS;
  /* Get the rr_node */
  t_rr_node* track_node = rr_gsb.get_chan_node(gsb_side, track_id);
  /* Get the coordinators */
  DeviceCoordinator side_coordinator = rr_gsb.get_side_block_coordinator(gsb_side); 

  /* Get the coordinator of where the track starts */
  DeviceCoordinator track_start = get_track_rr_node_start_coordinator(track_node);

  /* INC_DIRECTION start_track: (xlow, ylow) should be same as the GSB side coordinator */
  /* DEC_DIRECTION start_track: (xhigh, yhigh) should be same as the GSB side coordinator */
  if (  (track_start.get_x() == side_coordinator.get_x())
     && (track_start.get_y() == side_coordinator.get_y()) 
     && (OUT_PORT == rr_gsb.get_chan_node_direction(gsb_side, track_id)) ) {
    /* Double check: start track should be an OUTPUT PORT of the GSB */
    track_status = TRACK_START;
  }

  /* Get the coordinator of where the track ends */
  DeviceCoordinator track_end = get_track_rr_node_end_coordinator(track_node);

  /* INC_DIRECTION end_track: (xhigh, yhigh) should be same as the GSB side coordinator */ 
  /* DEC_DIRECTION end_track: (xlow, ylow) should be same as the GSB side coordinator */ 
  if (  (track_end.get_x() == side_coordinator.get_x())
     && (track_end.get_y() == side_coordinator.get_y()) 
     && (IN_PORT == rr_gsb.get_chan_node_direction(gsb_side, track_id)) ) {
    /* Double check: end track should be an INPUT PORT of the GSB */
    track_status = TRACK_END;
  }

  return track_status;
}


/************************************************************************
 * Check if the GSB is in the Connection Block (CB) population list of the segment
 * SB population of a L4 wire: 1 0 0 1
 *
 *                            +----+    +----+    +----+    +----+
 *                            | CB |--->| CB |--->| CB |--->| CB |
 *                            +----+    +----+    +----+    +----+
 *  Engage CB connection       Yes       No        No        Yes
 *
 *  We will find the offset between gsb_side_coordinator and (xlow,ylow) of the track
 *  Use the offset to check if the tracks should engage in this GSB connection
 ***********************************************************************/
static 
bool is_gsb_in_track_cb_population(const RRGSB& rr_gsb, 
                                   const enum e_side gsb_side, 
                                   const int track_id,
                                   const std::vector<t_segment_inf> segment_inf) {
  /* Get the rr_node */
  t_rr_node* track_node = rr_gsb.get_chan_node(gsb_side, track_id);
  /* Get the coordinators */
  DeviceCoordinator side_coordinator = rr_gsb.get_side_block_coordinator(gsb_side); 

  DeviceCoordinator track_start = get_track_rr_node_start_coordinator(track_node);

  /* Get the offset */
  size_t offset = std::abs((int)side_coordinator.get_x() - (int)track_start.get_x()) 
                + std::abs((int)side_coordinator.get_y() - (int)track_start.get_y()); 
  
  /* Get segment id */
  size_t seg_id = rr_gsb.get_chan_node_segment(gsb_side, track_id);
  /* validate offset */
  assert (offset < (size_t)segment_inf[seg_id].cb_len);

  /* Get the SB population */
  bool in_cb_population = false;
  if (TRUE == segment_inf[seg_id].cb[offset]) {  
    in_cb_population = true;
  }
  return in_cb_population;
}

/************************************************************************
 * Check if the GSB is in the Switch Block (SB) population list of the segment
 * SB population of a L3 wire: 1 0 0 1
 *
 *                            +----+    +----+    +----+    +----+
 *                            | SB |--->| SB |--->| SB |--->| SB |
 *                            +----+    +----+    +----+    +----+
 *  Engage SB connection       Yes       No        No        Yes
 *
 *  We will find the offset between gsb_side_coordinator and (xlow,ylow) of the track
 *  Use the offset to check if the tracks should engage in this GSB connection
 ***********************************************************************/
static 
bool is_gsb_in_track_sb_population(const RRGSB& rr_gsb, 
                                   const enum e_side gsb_side, 
                                   const int track_id,
                                   const std::vector<t_segment_inf> segment_inf) {
  /* Get the rr_node */
  t_rr_node* track_node = rr_gsb.get_chan_node(gsb_side, track_id);
  /* Get the coordinators */
  DeviceCoordinator side_coordinator = rr_gsb.get_side_block_coordinator(gsb_side); 

  DeviceCoordinator track_start = get_track_rr_node_start_coordinator(track_node);

  /* Get the offset */
  size_t offset = std::abs((int)side_coordinator.get_x() - (int)track_start.get_x()) 
                + std::abs((int)side_coordinator.get_y() - (int)track_start.get_y()); 
  
  /* Get segment id */
  size_t seg_id = rr_gsb.get_chan_node_segment(gsb_side, track_id);
  /* validate offset */
  assert (offset < (size_t)segment_inf[seg_id].sb_len);

  /* Get the SB population */
  bool in_sb_population = false;
  if (TRUE == segment_inf[seg_id].sb[offset]) {  
    in_sb_population = true;
  }
  return in_sb_population;
}

/************************************************************************
 * Create a list of track_id based on the to_track and num_to_tracks
 * We consider the following list [to_track, to_track + Fs/3 - 1]
 * if the [to_track + Fs/3 - 1] exceeds the num_to_tracks, we start over from 0!
***********************************************************************/
static 
std::vector<size_t> get_to_track_list(const int Fs, const int to_track, const int num_to_tracks) {
  std::vector<size_t> to_tracks;
  /* Ensure a clear start */
  to_tracks.clear();

  for (int i = 0; i < Fs; i = i + 3) {  
    /* TODO: currently, for Fs > 3, I always search the next from_track until Fs is satisfied 
     * The optimal track selection should be done in a more scientific way!!! 
     */
    int to_track_i = to_track + i;
    /* make sure the track id is still in range */
    if ( to_track_i > num_to_tracks - 1) {
      to_track_i = to_track_i % num_to_tracks; 
    }
    /* Ensure we are in the range */
    assert (to_track_i < num_to_tracks);
    /* from track must be connected */
    to_tracks.push_back(to_track_i);
  }
  return to_tracks;
}

/************************************************************************
 * This function aims to return the track indices that drive the from_track
 * in a Switch Block 
 * The track_ids to return will depend on different topologies of SB
 *  SUBSET, UNIVERSAL, and WILTON. 
 ***********************************************************************/
static 
std::vector<size_t> get_switch_block_to_track_id(const enum e_switch_block_type switch_block_type, 
                                                 const int Fs,
                                                 const enum e_side from_side,
                                                 const int from_track,
                                                 const enum e_side to_side, 
                                                 const int num_to_tracks) {

  /* This routine returns the track number to which the from_track should     
   * connect.  It supports any Fs % 3 == 0, switch blocks.
   */
  std::vector<size_t> to_tracks;
  /* Ensure a clear start */
  to_tracks.clear();

  /* TODO: currently, for Fs > 3, I always search the next from_track until Fs is satisfied 
   * The optimal track selection should be done in a more scientific way!!! 
   */

  assert (0 == Fs % 3);

  /* Adapt from_track to fit in the range of num_to_tracks */
  size_t actual_from_track = from_track % num_to_tracks; 

  switch (switch_block_type) {
  case SUBSET: /* NB:  Global routing uses SUBSET too */
    to_tracks = get_to_track_list(Fs, actual_from_track, num_to_tracks);
    /* Finish, we return */
    return to_tracks;
  case UNIVERSAL:
    if (  (from_side == LEFT)
       || (from_side == RIGHT) ) {  
     /* For the prev_side, to_track is from_track 
      * For the next_side, to_track is num_to_tracks - 1 - from_track 
      * For the opposite_side, to_track is always from_track
      */
      Side side_manager(from_side);
      if ( (to_side == side_manager.get_opposite()) 
        || (to_side == side_manager.get_rotate_counterclockwise()) ) {
        to_tracks = get_to_track_list(Fs, actual_from_track, num_to_tracks);
      } else if (to_side == side_manager.get_rotate_clockwise()) {
        to_tracks = get_to_track_list(Fs, num_to_tracks - 1 - actual_from_track, num_to_tracks);
      }
    }

    if (  (from_side ==  TOP)
       || (from_side == BOTTOM) ) {  
    /* For the next_side, to_track is from_track 
     * For the prev_side, to_track is num_to_tracks - 1 - from_track 
     * For the opposite_side, to_track is always from_track
     */
      Side side_manager(from_side);
      if ( (to_side == side_manager.get_opposite()) 
        || (to_side == side_manager.get_rotate_clockwise()) ) {
        to_tracks = get_to_track_list(Fs, actual_from_track, num_to_tracks);
      } else if (to_side == side_manager.get_rotate_counterclockwise()) {
        to_tracks = get_to_track_list(Fs, num_to_tracks - 1 - actual_from_track, num_to_tracks);
      }
    }
    /* Finish, we return */
    return to_tracks;
    /* End switch_block_type == UNIVERSAL case. */
  case WILTON:
    /* See S. Wilton Phd thesis, U of T, 1996 p. 103 for details on following. */
    if (from_side == LEFT) {
      if (to_side == RIGHT) { /* CHANX to CHANX */
        to_tracks = get_to_track_list(Fs, actual_from_track, num_to_tracks);
      } else if (to_side == TOP) { /* from CHANX to CHANY */
        to_tracks = get_to_track_list(Fs, (num_to_tracks - actual_from_track ) % num_to_tracks, num_to_tracks);
      } else if (to_side == BOTTOM) {
        to_tracks = get_to_track_list(Fs, (num_to_tracks + actual_from_track - 1) % num_to_tracks, num_to_tracks);
      }
    } else if (from_side == RIGHT) {
      if (to_side == LEFT) { /* CHANX to CHANX */
        to_tracks = get_to_track_list(Fs, actual_from_track, num_to_tracks);
      } else if (to_side == TOP) { /* from CHANX to CHANY */
        to_tracks = get_to_track_list(Fs, (num_to_tracks + actual_from_track - 1) % num_to_tracks, num_to_tracks);
      } else if (to_side == BOTTOM) {
        to_tracks = get_to_track_list(Fs, (2 * num_to_tracks - 2 - actual_from_track) % num_to_tracks, num_to_tracks);
      }
    } else if (from_side == BOTTOM) {
      if (to_side == TOP) { /* CHANY to CHANY */
        to_tracks = get_to_track_list(Fs, actual_from_track, num_to_tracks);
      } else if (to_side == LEFT) { /* from CHANY to CHANX */
        to_tracks = get_to_track_list(Fs, (actual_from_track + 1) % num_to_tracks, num_to_tracks);
      } else if (to_side == RIGHT) {
        to_tracks = get_to_track_list(Fs, (2 * num_to_tracks - 2 - actual_from_track) % num_to_tracks, num_to_tracks);
      }
    } else if (from_side == TOP) {
      if (to_side == BOTTOM) { /* CHANY to CHANY */
        to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
      } else if (to_side == LEFT) { /* from CHANY to CHANX */
        to_tracks = get_to_track_list(Fs, (num_to_tracks - actual_from_track) % num_to_tracks, num_to_tracks);
      } else if (to_side == RIGHT) {
        to_tracks = get_to_track_list(Fs, (actual_from_track + 1) % num_to_tracks, num_to_tracks);
      }
    }
    /* Finish, we return */
    return to_tracks;
  /* End switch_block_type == WILTON case. */
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid switch block pattern !\n",
                __FILE__, __LINE__);
    exit(1);
  }

  return to_tracks;
}


/************************************************************************
 * Build the track_to_track_map[from_side][0..chan_width-1][to_side][track_indices] 
 * For a group of from_track nodes and to_track nodes
 * For each side of from_tracks, we call a routine to get the list of to_tracks
 * Then, we fill the track2track_map
 ***********************************************************************/
static 
void build_gsb_one_group_track_to_track_map(const t_rr_graph* rr_graph, 
                                            const RRGSB& rr_gsb, 
                                            const enum e_switch_block_type sb_type, 
                                            const int Fs,
                                            const bool wire_opposite_side,
                                            const t_track_group from_tracks, /* [0..gsb_side][track_indices] */
                                            const t_track_group to_tracks, /* [0..gsb_side][track_indices] */
                                            t_track2track_map* track2track_map) {
  for (size_t side = 0; side < from_tracks.size(); ++side) {
    Side side_manager(side);
    enum e_side from_side = side_manager.get_side();
    /* Find the other sides where the start tracks will locate */
    std::vector<enum e_side> to_track_sides;
    /* 0. opposite side */
    to_track_sides.push_back(side_manager.get_opposite());
    /* 1. prev side */
    /* Previous side definition: TOP => LEFT; RIGHT=>TOP; BOTTOM=>RIGHT; LEFT=>BOTTOM */
    to_track_sides.push_back(side_manager.get_rotate_counterclockwise()); 
    /* 2. next side */
    /* Next side definition: TOP => RIGHT; RIGHT=>BOTTOM; BOTTOM=>LEFT; LEFT=>TOP */
    to_track_sides.push_back(side_manager.get_rotate_clockwise()); 
    
    for (size_t inode = 0; inode < from_tracks[side].size(); ++inode) {
      for (size_t to_side_id = 0; to_side_id < to_track_sides.size(); ++to_side_id) {
        enum e_side to_side = to_track_sides[to_side_id];
        Side to_side_manager(to_side);
        size_t to_side_index = to_side_manager.to_size_t();
        /* Bypass those to_sides have no nodes  */
        if (0 == to_tracks[to_side_index].size()) {
          continue;
        }
        /* Bypass those from_side is same as to_side */
        if (from_side == to_side) {
          continue;
        }
        /* Bypass those from_side is opposite to to_side if required */
        if ( (true == wire_opposite_side) 
          && (to_side_manager.get_opposite() == from_side) ) {
          continue;
        }
        /* Get other track_ids depending on the switch block pattern */
        /* Find the track ids that will start at the other sides */
        std::vector<size_t> to_track_ids = get_switch_block_to_track_id(sb_type, Fs, from_side, inode, 
                                                                        to_side, 
                                                                        to_tracks[to_side_index].size()); 
        /* Update the track2track_map: */
        for (size_t to_track_id = 0; to_track_id < to_track_ids.size(); ++to_track_id) {
          size_t from_side_index = side_manager.to_size_t();
          size_t from_track_index = from_tracks[side][inode];
          /* Check the id is still in the range !*/
          assert ( to_track_ids[to_track_id] < to_tracks[to_side_index].size() );
          size_t to_track_index = to_tracks[to_side_index][to_track_ids[to_track_id]];
          //printf("from_track(size=%lu): %lu , to_track_ids[%lu]:%lu, to_track_index: %lu in a group of %lu tracks\n", 
          //       from_tracks[side].size(), inode, to_track_id, to_track_ids[to_track_id], 
          //       to_track_index, to_tracks[to_side_index].size());
          t_rr_node* to_track_node = rr_gsb.get_chan_node(to_side, to_track_index);

          /* from_track should be IN_PORT */
          assert( IN_PORT == rr_gsb.get_chan_node_direction(from_side, from_track_index) ); 
          /* to_track should be OUT_PORT */
          assert( OUT_PORT == rr_gsb.get_chan_node_direction(to_side, to_track_index) ); 

          /* Check if the to_track_node is already in the list ! */
          std::vector<int>::iterator it = std::find((*track2track_map)[from_side_index][from_track_index].begin(),
                                                    (*track2track_map)[from_side_index][from_track_index].end(),
                                                    to_track_node - rr_graph->rr_node);
          if (it != (*track2track_map)[from_side_index][from_track_index].end()) {
             continue; /* the node_id is already in the list, go for the next */
          }
          /* Clear, we should add to the list */
          (*track2track_map)[from_side_index][from_track_index].push_back(to_track_node - rr_graph->rr_node);
        }
      }
    }
  }

  return;
}

/************************************************************************
 * Build the track_to_track_map[from_side][0..chan_width-1][to_side][track_indices] 
 * based on the existing routing resources in the General Switch Block (GSB)
 * The track_indices is the indices of tracks that the node at from_side and [0..chan_width-1] will drive 
 * IMPORTANT: the track_indices are the indicies in the GSB context, but not the rr_graph!!!
 * We separate the connections into two groups:
 * Group 1: the routing tracks start from this GSB 
 *          We will apply switch block patterns (SUBSET, UNIVERSAL, WILTON) 
 * Group 2: the routing tracks do not start from this GSB (bypassing wires)
 *          We will apply switch block patterns (SUBSET, UNIVERSAL, WILTON) 
 *          but we will check the Switch Block (SB) population of these 
 *          routing segments, and determine which requires connections
 *          
 *                         CHANY  CHANY  CHANY CHANY
 *                          [0]    [1]    [2]   [3]
 *                   start  yes    no     yes   no            
 *        end             +-------------------------+           start    Group 1      Group 2
 *         no    CHANX[0] |           TOP           |  CHANX[0]  yes   TOP/BOTTOM   TOP/BOTTOM
 *                        |                         |                  CHANY[0,2]   CHANY[1,3]
 *        yes    CHANX[1] |                         |  CHANX[1]  no
 *                        |  LEFT           RIGHT   |
 *         no    CHANX[2] |                         |  CHANX[2]  yes
 *                        |                         |
 *        yes    CHANX[3] |         BOTTOM          |  CHANX[3]  no
 *                        +-------------------------+           
 *                         CHANY  CHANY  CHANY CHANY
 *                          [0]    [1]    [2]   [3]
 *                   start  yes    no     yes   no            
 *            
 * The mapping is done in the following steps: (For each side of the GSB)
 * 1. Build a list of tracks that will start from this side
 *    if a track starts, its xlow/ylow is the same as the x,y of this gsb
 * 2. Build a list of tracks on the other sides belonging to Group 1.
 *    Take the example of RIGHT side, we will collect 
 *    a. tracks that will end at the LEFT side 
 *    b. tracks that will start at the TOP side
 *    c. tracks that will start at the BOTTOM side
 * 3. Apply switch block patterns to Group 1 (SUBSET, UNIVERSAL, WILTON) 
 * 4. Build a list of tracks on the other sides belonging to Group 1.
 *    Take the example of RIGHT side, we will collect 
 *    a. tracks that will bypass at the TOP side
 *    b. tracks that will bypass at the BOTTOM side
 * 5. Apply switch block patterns to Group 2 (SUBSET, UNIVERSAL, WILTON) 
 ***********************************************************************/
t_track2track_map build_gsb_track_to_track_map(const t_rr_graph* rr_graph,
                                               const RRGSB& rr_gsb,
                                               const enum e_switch_block_type sb_type, 
                                               const int Fs,
                                               const enum e_switch_block_type sb_subtype, 
                                               const int subFs,
                                               const bool wire_opposite_side,
                                               const std::vector<t_segment_inf> segment_inf) {
  t_track2track_map track2track_map; /* [0..gsb_side][0..chan_width][track_indices] */

  /* Categorize tracks into 3 groups: 
   * (1) tracks will start here 
   * (2) tracks will end here 
   * (2) tracks will just pass through the SB */
  t_track_group start_tracks; /* [0..gsb_side][track_indices] */
  t_track_group end_tracks; /* [0..gsb_side][track_indices] */
  t_track_group pass_tracks; /* [0..gsb_side][track_indices] */

  /* resize to the number of sides */
  start_tracks.resize(rr_gsb.get_num_sides());
  end_tracks.resize(rr_gsb.get_num_sides());
  pass_tracks.resize(rr_gsb.get_num_sides());

  /* Walk through each side */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    enum e_side gsb_side = side_manager.get_side();
    /* Build a list of tracks that will start from this side */
    for (size_t inode = 0; inode < rr_gsb.get_chan_width(gsb_side); ++inode) {
      /* We need to check Switch block population of this track 
       * The track node will not be considered if there supposed to be no SB at this position 
       */
      if (false == is_gsb_in_track_sb_population(rr_gsb, gsb_side, inode, segment_inf)) {
        continue; /* skip this node and go to the next */
      }
      /* check if this track will start from here */
      enum e_track_status track_status = determine_track_status_of_gsb(rr_gsb, gsb_side, inode);

      switch (track_status) {
      case TRACK_START:
        /* update starting track list */
        start_tracks[side].push_back(inode);
        break;
      case TRACK_END:
        /* Update end track list */
        end_tracks[side].push_back(inode);
        break;
      case TRACK_PASS:
        /* Update passing track list */
        /* Note that the pass_track should be IN_PORT only !!! */
        if (IN_PORT == rr_gsb.get_chan_node_direction(gsb_side, inode)) {
          pass_tracks[side].push_back(inode);
        }
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s, [LINE%d]) Invalid track status!\n",
                   __FILE__, __LINE__);
        exit(1);
      }
    }
  }

  /* Allocate track2track map */
  track2track_map.resize(rr_gsb.get_num_sides());
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    enum e_side gsb_side = side_manager.get_side();
    /* allocate track2track_map[gsb_side] */
    track2track_map[side].resize(rr_gsb.get_chan_width(gsb_side));
    for (size_t inode = 0; inode < rr_gsb.get_chan_width(gsb_side); ++inode) {
      /* allocate track2track_map[gsb_side][inode] */
      track2track_map[side][inode].clear();
    }
  }

  /* For Group 1: we build connections between end_tracks and start_tracks*/
  build_gsb_one_group_track_to_track_map(rr_graph, rr_gsb, 
                                         sb_type, Fs,
                                         true, /* End tracks should always to wired to start tracks */ 
                                         end_tracks, start_tracks,
                                         &track2track_map);

  /* For Group 2: we build connections between end_tracks and start_tracks*/
  /* Currently, I use the same Switch Block pattern for the passing tracks and end tracks,
   * TODO: This can be improved with different patterns! 
   */
  build_gsb_one_group_track_to_track_map(rr_graph, rr_gsb, 
                                         sb_subtype, subFs,
                                         wire_opposite_side, /* Pass tracks may not be wired to start tracks */ 
                                         pass_tracks, start_tracks, 
                                         &track2track_map);

  return track2track_map;
}

/* Build a RRChan Object with the given channel type and coorindators */
static 
RRChan build_one_tileable_rr_chan(const DeviceCoordinator& chan_coordinator, 
                                  const t_rr_type chan_type, 
                                  const t_rr_graph* rr_graph, 
                                  const ChanNodeDetails& chan_details) {
  int chan_width = 0;
  t_rr_node** chan_rr_nodes = NULL;

  /* Create a rr_chan object and check if it is unique in the graph */
  RRChan rr_chan;
  /* Fill the information */
  rr_chan.set_type(chan_type); 

  /* Collect rr_nodes for this channel */
  chan_rr_nodes = get_chan_rr_nodes(&chan_width, chan_type, 
                                    chan_coordinator.get_x(), chan_coordinator.get_y(),
                                    rr_graph->num_rr_nodes, rr_graph->rr_node, 
                                    rr_graph->rr_node_indices);

  /* Reserve */
  /* rr_chan.reserve_node(size_t(chan_width)); */

  /* Fill the rr_chan */  
  for (size_t itrack = 0; itrack < size_t(chan_width); ++itrack) {
    size_t iseg = chan_details.get_track_segment_id(itrack); 
    rr_chan.add_node(chan_rr_nodes[itrack], iseg);
  }

  /* Free rr_nodes */
  my_free(chan_rr_nodes);

  return rr_chan;
}

/***********************************************************************
 * Build a General Switch Block (GSB) 
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
 ***********************************************************************/
RRGSB build_one_tileable_rr_gsb(const DeviceCoordinator& device_range, 
                                const std::vector<size_t> device_chan_width, 
                                const std::vector<t_segment_inf> segment_inf,
                                const DeviceCoordinator& gsb_coordinator, 
                                t_rr_graph* rr_graph) {
  /* Create an object to return */
  RRGSB rr_gsb;

  /* Check */
  assert(gsb_coordinator.get_x() <= device_range.get_x()); 
  assert(gsb_coordinator.get_y() <= device_range.get_y()); 

  /* Coordinator initialization */
  rr_gsb.set_coordinator(gsb_coordinator.get_x(), gsb_coordinator.get_y());

  /* Basic information*/
  rr_gsb.init_num_sides(4); /* Fixed number of sides */

  /* Find all rr_nodes of channels */
  /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    /* Local variables inside this for loop */
    Side side_manager(side);
    DeviceCoordinator coordinator = rr_gsb.get_side_block_coordinator(side_manager.get_side());
    RRChan rr_chan;
    int temp_num_opin_rr_nodes[2] = {0,0};
    t_rr_node** temp_opin_rr_node[2] = {NULL, NULL};
    enum e_side opin_grid_side[2] = {NUM_SIDES, NUM_SIDES};
    enum PORTS chan_dir_to_port_dir_mapping[2] = {OUT_PORT, IN_PORT}; /* 0: INC_DIRECTION => ?; 1: DEC_DIRECTION => ? */
    /* Build a segment details, where we need the segment ids for building rr_chan  
     * We do not care starting and ending points here, so set chan_side as NUM_SIDES 
     */
    ChanNodeDetails chanx_details = build_unidir_chan_node_details(device_chan_width[0], device_range.get_x() - 1, 
                                                                   NUM_SIDES, segment_inf); 
    ChanNodeDetails chany_details = build_unidir_chan_node_details(device_chan_width[1], device_range.get_y() - 1, 
                                                                   NUM_SIDES, segment_inf); 

    switch (side) {
    case TOP: /* TOP = 0 */
      /* For the bording, we should take special care */
      if (gsb_coordinator.get_y() == device_range.get_y()) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_tileable_rr_chan(coordinator, CHANY, rr_graph, chany_details);
      chan_dir_to_port_dir_mapping[0] = OUT_PORT; /* INC_DIRECTION => OUT_PORT */
      chan_dir_to_port_dir_mapping[1] =  IN_PORT; /* DEC_DIRECTION => IN_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* Include Grid[x][y+1] RIGHT side outputs pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, gsb_coordinator.get_x(), gsb_coordinator.get_y() + 1, 1,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);
      /* Include Grid[x+1][y+1] Left side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, gsb_coordinator.get_x() + 1, gsb_coordinator.get_y() + 1, 3,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);

      /* Assign grid side of OPIN */
      /* Grid[x][y+1] RIGHT side outputs pins */
      opin_grid_side[0] = RIGHT;
      /* Grid[x+1][y+1] left side outputs pins */
      opin_grid_side[1] = LEFT; 
      break;
    case RIGHT: /* RIGHT = 1 */
      /* For the bording, we should take special care */
      if (gsb_coordinator.get_x() == device_range.get_x()) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Collect rr_nodes for Tracks for top: chany[x][y+1] */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_tileable_rr_chan(coordinator, CHANX, rr_graph, chanx_details);
      chan_dir_to_port_dir_mapping[0] = OUT_PORT; /* INC_DIRECTION => OUT_PORT */
      chan_dir_to_port_dir_mapping[1] =  IN_PORT; /* DEC_DIRECTION => IN_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* include Grid[x+1][y+1] Bottom side output pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, gsb_coordinator.get_x() + 1, gsb_coordinator.get_y() + 1, 2,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);
      /* include Grid[x+1][y] Top side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, gsb_coordinator.get_x() + 1, gsb_coordinator.get_y(), 0,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);
      /* Assign grid side of OPIN */
      /* Grid[x+1][y+1] BOTTOM side outputs pins */
      opin_grid_side[0] = BOTTOM;
      /* Grid[x+1][y] TOP side outputs pins */
      opin_grid_side[1] = TOP;
      break;
    case BOTTOM: /* BOTTOM = 2*/
      /* For the bording, we should take special care */
      if (gsb_coordinator.get_y() == 0) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Collect rr_nodes for Tracks for bottom: chany[x][y] */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_tileable_rr_chan(coordinator, CHANY, rr_graph, chany_details);
      chan_dir_to_port_dir_mapping[0] =  IN_PORT; /* INC_DIRECTION => IN_PORT */
      chan_dir_to_port_dir_mapping[1] = OUT_PORT; /* DEC_DIRECTION => OUT_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* include Grid[x+1][y] Left side output pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, gsb_coordinator.get_x() + 1, gsb_coordinator.get_y(), 3,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);
      /* include Grid[x][y] Right side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, gsb_coordinator.get_x(), gsb_coordinator.get_y(), 1,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);
      /* Assign grid side of OPIN */
      /* Grid[x+1][y] LEFT side outputs pins */
      opin_grid_side[0] = LEFT;
      /* Grid[x][y] RIGHT side outputs pins */
      opin_grid_side[1] = RIGHT;
      break;
    case LEFT: /* LEFT = 3 */
      /* For the bording, we should take special care */
      if (gsb_coordinator.get_x() == 0) {
        rr_gsb.clear_one_side(side_manager.get_side());
        break;
      }
      /* Routing channels*/
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Collect rr_nodes for Tracks for left: chanx[x][y] */
      /* Create a rr_chan object and check if it is unique in the graph */
      rr_chan = build_one_tileable_rr_chan(coordinator, CHANX, rr_graph, chanx_details);
      chan_dir_to_port_dir_mapping[0] =  IN_PORT; /* INC_DIRECTION => IN_PORT */
      chan_dir_to_port_dir_mapping[1] = OUT_PORT; /* DEC_DIRECTION => OUT_PORT */

      /* Build the Switch block: opin and opin_grid_side */
      /* include Grid[x][y+1] Bottom side outputs pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, gsb_coordinator.get_x(), gsb_coordinator.get_y() + 1, 2,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);
      /* include Grid[x][y] Top side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, gsb_coordinator.get_x(), gsb_coordinator.get_y(), 0,
                                                        rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);

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
                                                   rr_graph->num_rr_nodes, rr_graph->rr_node, rr_graph->rr_node_indices);
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

/************************************************************************
 * Create edges for each rr_node of a General Switch Blocks (GSB):
 * 1. create edges between CHANX | CHANY and IPINs (connections inside connection blocks) 
 * 2. create edges between OPINs, CHANX and CHANY (connections inside switch blocks) 
 * 3. create edges between OPINs and IPINs (direct-connections) 
 ***********************************************************************/
void build_edges_for_one_tileable_rr_gsb(const t_rr_graph* rr_graph, 
                                         const RRGSB* rr_gsb,
                                         const t_track2pin_map track2ipin_map,
                                         const t_pin2track_map opin2track_map,
                                         const t_track2track_map track2track_map) {
  /* Check rr_gsb */
  assert (NULL != rr_gsb);
  
  /* Walk through each sides */ 
  for (size_t side = 0; side < rr_gsb->get_num_sides(); ++side) {
    Side side_manager(side);
    enum e_side gsb_side = side_manager.get_side();

    /* Find OPINs */  
    for (size_t inode = 0; inode < rr_gsb->get_num_opin_nodes(gsb_side); ++inode) {
      t_rr_node* opin_node = rr_gsb->get_opin_node(gsb_side, inode); 

      /* 1. create edges between OPINs and CHANX|CHANY, using opin2track_map */
      std::vector<short> driver_switches;
      int num_edges = opin2track_map[gsb_side][inode].size();
      for (int iedge = 0; iedge < num_edges; ++iedge) {
        int track_node_id = opin2track_map[side_manager.to_size_t()][inode][iedge];
        driver_switches.push_back(rr_graph->rr_node[track_node_id].driver_switch); 
      }
      /* add edges to the opin_node */
      add_edges_for_two_rr_nodes(rr_graph, opin_node - rr_graph->rr_node, 
                                 opin2track_map[gsb_side][inode], driver_switches);
    }

    /* Find  CHANX or CHANY */
    /* For TRACKs to IPINs, we only care LEFT and TOP sides 
     * Skip RIGHT and BOTTOM for the ipin2track_map since they should be handled in other GSBs 
     */
    if ( (side_manager.get_side() == rr_gsb->get_cb_chan_side(CHANX))
      || (side_manager.get_side() == rr_gsb->get_cb_chan_side(CHANY)) ) {
      /* 2. create edges between CHANX|CHANY and IPINs, using ipin2track_map */
      for (size_t inode = 0; inode < rr_gsb->get_chan_width(gsb_side); ++inode) {
        t_rr_node* chan_node = rr_gsb->get_chan_node(gsb_side, inode); 
        std::vector<short> driver_switches;
        int num_edges = track2ipin_map[gsb_side][inode].size();
        for (int iedge = 0; iedge < num_edges; ++iedge) {
          int ipin_node_id = track2ipin_map[gsb_side][inode][iedge];
          driver_switches.push_back(rr_graph->rr_node[ipin_node_id].driver_switch); 
        }
        /* add edges to the chan_node */
        add_edges_for_two_rr_nodes(rr_graph, chan_node - rr_graph->rr_node, 
                                   track2ipin_map[gsb_side][inode], driver_switches);
      }
    }

    /* 3. create edges between CHANX|CHANY and CHANX|CHANY, using track2track_map */
    for (size_t inode = 0; inode < rr_gsb->get_chan_width(gsb_side); ++inode) {
      t_rr_node* chan_node = rr_gsb->get_chan_node(gsb_side, inode); 
      std::vector<short> driver_switches;
      int num_edges = track2track_map[gsb_side][inode].size();
      for (int iedge = 0; iedge < num_edges; ++iedge) {
        int track_node_id = track2track_map[gsb_side][inode][iedge];
        driver_switches.push_back(rr_graph->rr_node[track_node_id].driver_switch); 
      }
      /* add edges to the chan_node */
      add_edges_for_two_rr_nodes(rr_graph, chan_node - rr_graph->rr_node, 
                                 track2track_map[gsb_side][inode], driver_switches);
    }
  }

  return;
}

/************************************************************************
 * Build track2ipin_map for an IPIN  
 * 1. build a list of routing tracks which are allowed for connections
 *    We will check the Connection Block (CB) population of each routing track. 
 *    By comparing current chan_y - ylow, we can determine if a CB connection
 *    is required for each routing track
 * 2. Divide the routing tracks by segment types, so that we can balance
 *    the connections between IPINs and different types of routing tracks.
 * 3. Scale the Fc of each pin to the actual number of routing tracks
 *    actual_Fc = (int) Fc * num_tracks / chan_width
 ***********************************************************************/
static 
void build_gsb_one_ipin_track2pin_map(const t_rr_graph* rr_graph, 
                                      const RRGSB& rr_gsb, 
                                      const enum e_side ipin_side, 
                                      const size_t ipin_node_id, 
                                      const size_t Fc, 
                                      const size_t offset, 
                                      const std::vector<t_segment_inf> segment_inf, 
                                      t_track2pin_map* track2ipin_map) {
  /* Get a list of segment_ids*/
  enum e_side chan_side = rr_gsb.get_cb_chan_side(ipin_side);
  Side chan_side_manager(chan_side);
  std::vector<size_t> seg_list = rr_gsb.get_chan_segment_ids(chan_side);
  size_t chan_width = rr_gsb.get_chan_width(chan_side);
  Side ipin_side_manager(ipin_side);
  t_rr_node* ipin_node = rr_gsb.get_ipin_node(ipin_side, ipin_node_id);

  for (size_t iseg = 0; iseg < seg_list.size(); ++iseg) {
    /* Get a list of node that have the segment id */
    std::vector<size_t> track_list = rr_gsb.get_chan_node_ids_by_segment_ids(chan_side, seg_list[iseg]);
    /* Refine the track_list: keep those will have connection blocks in the GSB */
    std::vector<size_t> actual_track_list;
    for (size_t inode = 0; inode < track_list.size(); ++inode) {
      /* Check if tracks allow connection blocks in the GSB*/
      if (false == is_gsb_in_track_cb_population(rr_gsb, chan_side, track_list[inode], segment_inf)) {
         continue; /* Bypass condition */
      }
      /* Push the node to actual_track_list  */
      actual_track_list.push_back(track_list[inode]);
    }
    /* Check the actual track list */
    assert (0 == actual_track_list.size() % 2);
   
    /* Scale Fc  */
    int actual_Fc = std::ceil((float)Fc * (float)actual_track_list.size() / (float)chan_width); 
    /* Minimum Fc should be 2 : ensure we will connect to a pair of routing tracks */
    actual_Fc = std::max(1, actual_Fc);
    /* Compute the step between two connection from this IPIN to tracks: 
     * step = W' / Fc', W' and Fc' are the adapted W and Fc from actual_track_list and Fc_in 
     * For uni-directional arch, all the tracks are in pair, we will divide by 2 to be normalized (Fc counts pairs)
    */
    size_t track_step = std::ceil((float)(actual_track_list.size() / 2) / (float)actual_Fc);
    /* Make sure step should be at least 2 */
    track_step = std::max(1, (int)track_step);
    /* Adapt offset to the range of actual_track_list */
    size_t actual_offset = offset % actual_track_list.size();
    /* rotate the track list by an offset */
    if (0 < actual_offset) {
      std::rotate(actual_track_list.begin(), actual_track_list.begin() + actual_offset, actual_track_list.end());   
    }

    /* Assign tracks: since we assign 2 track per round, we increment itrack by 2* step  */
    int track_cnt = 0;
    for (size_t itrack = 0; itrack < actual_track_list.size(); itrack = itrack + 2 * track_step) {
      /* Update pin2track map */ 
      size_t chan_side_index = chan_side_manager.to_size_t();
      size_t ipin_index = ipin_node - rr_graph->rr_node;
      /* track_index may exceed the chan_width(), adapt it */
      size_t track_index = actual_track_list[itrack] % chan_width;

      (*track2ipin_map)[chan_side_index][track_index].push_back(ipin_index);

      /* track_index may exceed the chan_width(), adapt it */
      track_index = (actual_track_list[itrack] + 1) % chan_width;

      (*track2ipin_map)[chan_side_index][track_index].push_back(ipin_index);

      track_cnt += 2;
      /* Stop when we have enough Fc: this may lead to some tracks have zero drivers. 
       * So I comment it. And we just make sure its track_cnt >= actual_Fc
      if (actual_Fc == track_cnt) {
        break;
      }
       */
    }

    /* Ensure the number of tracks is similar to Fc */
    //printf("Fc_in=%d, track_cnt=%d\n", actual_Fc, track_cnt);
    assert (2 * actual_Fc <= track_cnt);
    /* Give a warning if Fc is < track_cnt */
    if (2 * actual_Fc < track_cnt) {
      vpr_printf(TIO_MESSAGE_INFO, 
                 "IPIN Node(%lu) will have a higher Fc(=%lu) than specified(=%lu)!\nThis is due to that the number of tracks is much larger than IPINs!\n",
                 ipin_node - rr_graph->rr_node, track_cnt, 2 * actual_Fc);
    }
  }
  
  return;
}

/************************************************************************
 * Build opin2track_map for an OPIN  
 * 1. build a list of routing tracks which are allowed for connections
 *    We will check the Switch Block (SB) population of each routing track. 
 *    By comparing current chan_y - ylow, we can determine if a SB connection
 *    is required for each routing track
 * 2. Divide the routing tracks by segment types, so that we can balance
 *    the connections between OPINs and different types of routing tracks.
 * 3. Scale the Fc of each pin to the actual number of routing tracks
 *    actual_Fc = (int) Fc * num_tracks / chan_width
 ***********************************************************************/
static 
void build_gsb_one_opin_pin2track_map(const t_rr_graph* rr_graph, 
                                      const RRGSB& rr_gsb, 
                                      const enum e_side opin_side, 
                                      const size_t opin_node_id, 
                                      const size_t Fc, 
                                      const size_t offset, 
                                      const std::vector<t_segment_inf> segment_inf, 
                                      t_pin2track_map* opin2track_map) {
  /* Get a list of segment_ids*/
  std::vector<size_t> seg_list = rr_gsb.get_chan_segment_ids(opin_side);
  enum e_side chan_side = opin_side;
  size_t chan_width = rr_gsb.get_chan_width(chan_side);
  Side opin_side_manager(opin_side);

  for (size_t iseg = 0; iseg < seg_list.size(); ++iseg) {
    /* Get a list of node that have the segment id */
    std::vector<size_t> track_list = rr_gsb.get_chan_node_ids_by_segment_ids(chan_side, seg_list[iseg]);
    /* Refine the track_list: keep those will have connection blocks in the GSB */
    std::vector<size_t> actual_track_list;
    for (size_t inode = 0; inode < track_list.size(); ++inode) {
      /* Check if tracks allow connection blocks in the GSB*/
      if (false == is_gsb_in_track_sb_population(rr_gsb, chan_side, 
                                                 track_list[inode], segment_inf)) {
         continue; /* Bypass condition */
      }
      if (TRACK_START != determine_track_status_of_gsb(rr_gsb, chan_side, track_list[inode])) {
         continue; /* Bypass condition */
      }
      /* Push the node to actual_track_list  */
      actual_track_list.push_back(track_list[inode]);
    }

    /* Go the next segment if offset is zero or actual_track_list is empty */    
    if (0 == actual_track_list.size()) {
      continue;
    }
   
    /* Scale Fc  */
    int actual_Fc = std::ceil((float)Fc * (float)actual_track_list.size() / (float)chan_width); 
    /* Minimum Fc should be 1 : ensure we will drive 1 routing track */
    actual_Fc = std::max(1, actual_Fc);
    /* Compute the step between two connection from this IPIN to tracks: 
     * step = W' / Fc', W' and Fc' are the adapted W and Fc from actual_track_list and Fc_in 
    */
    size_t track_step = std::ceil((float)actual_track_list.size() / (float)actual_Fc);
    /* Track step mush be a multiple of 2!!!*/
    /* Make sure step should be at least 1 */
    track_step = std::max(1, (int)track_step);
    /* Adapt offset to the range of actual_track_list */
    size_t actual_offset = offset % actual_track_list.size();

    /* No need to rotate if offset is zero */    
    if (0 < actual_offset) {
      /* rotate the track list by an offset */
      std::rotate(actual_track_list.begin(), actual_track_list.begin() + actual_offset, actual_track_list.end());   
    }

    /* Assign tracks  */
    int track_cnt = 0;
    for (size_t itrack = 0; itrack < actual_track_list.size(); itrack = itrack + track_step) {
      /* Update pin2track map */ 
      size_t opin_side_index = opin_side_manager.to_size_t();
      size_t track_index = actual_track_list[itrack];
      size_t track_rr_node_index = rr_gsb.get_chan_node(chan_side, track_index) - rr_graph->rr_node;
      (*opin2track_map)[opin_side_index][opin_node_id].push_back(track_rr_node_index);
      /* update track counter */
      track_cnt++;
      /* Stop when we have enough Fc: this may lead to some tracks have zero drivers. 
       * So I comment it. And we just make sure its track_cnt >= actual_Fc
      if (actual_Fc == track_cnt) {
        break;
      }
      */
    }

    /* Ensure the number of tracks is similar to Fc */
    //printf("Fc_out=%lu, scaled_Fc_out=%d, track_cnt=%d, actual_track_cnt=%lu/%lu\n", Fc, actual_Fc, track_cnt, actual_track_list.size(), chan_width);
    assert (actual_Fc <= track_cnt);
    /* Give a warning if Fc is < track_cnt */
    if (actual_Fc < track_cnt) {
      vpr_printf(TIO_MESSAGE_INFO, 
                 "OPIN Node(%lu) will have a higher Fc(=%lu) than specified(=%lu)!\nThis is due to that the number of tracks is much larger than OPINs!\n",
                 opin_node_id, track_cnt, actual_Fc);
    }
  }
  
  return;
}


/************************************************************************
 * Build the track_to_ipin_map[gsb_side][0..chan_width-1][ipin_indices] 
 * based on the existing routing resources in the General Switch Block (GSB)
 * This function supports both X-directional and Y-directional tracks 
 * The mapping is done in the following steps:
 * 1. Build ipin_to_track_map[gsb_side][0..num_ipin_nodes-1][track_indices]
 *    For each IPIN, we ensure at least one connection to the tracks.
 *    Then, we assign IPINs to tracks evenly while satisfying the actual_Fc 
 * 2. Convert the ipin_to_track_map to track_to_ipin_map
 ***********************************************************************/
t_track2pin_map build_gsb_track_to_ipin_map(t_rr_graph* rr_graph,
                                            const RRGSB& rr_gsb, 
                                            const std::vector<std::vector<t_grid_tile>> grids, 
                                            const std::vector<t_segment_inf> segment_inf, 
                                            int** Fc_in) {
  t_track2pin_map track2ipin_map;
  /* Resize the matrix */ 
  track2ipin_map.resize(rr_gsb.get_num_sides());
  
  /* offset counter: it aims to balance the track-to-IPIN for each connection block */
  size_t offset_size = 0;
  std::vector<size_t> offset;
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    enum e_side ipin_side = side_manager.get_side();
    /* Get the chan_side */
    enum e_side chan_side = rr_gsb.get_cb_chan_side(ipin_side);
    Side chan_side_manager(chan_side);
    /* resize offset to the maximum chan_side*/
    offset_size = std::max(offset_size, chan_side_manager.to_size_t() + 1);
  }
  /* Initial offset */
  offset.resize(offset_size);
  offset.assign(offset.size(), 0);
     
  /* Walk through each side */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    enum e_side ipin_side = side_manager.get_side();
    /* Get the chan_side */
    enum e_side chan_side = rr_gsb.get_cb_chan_side(ipin_side);
    Side chan_side_manager(chan_side);
    /* This track2pin mapping is for Connection Blocks, so we only care two sides! */
    /* Get channel width and resize the matrix */
    size_t chan_width = rr_gsb.get_chan_width(chan_side);
    track2ipin_map[chan_side_manager.to_size_t()].resize(chan_width); 
    /* Find the ipin/opin nodes */
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(ipin_side); ++inode) {
      t_rr_node* ipin_node = rr_gsb.get_ipin_node(ipin_side, inode);
      /* Skip EMPTY type */
      if (EMPTY_TYPE == grids[ipin_node->xlow][ipin_node->ylow].type) {
        continue;
      }
      int grid_type_index = grids[ipin_node->xlow][ipin_node->ylow].type->index; 
      /* Get Fc of the ipin */
      int ipin_Fc = Fc_in[grid_type_index][ipin_node->ptc_num];
      /* skip Fc = 0 */
      if ( (-1 == ipin_Fc)
        || (0 == ipin_Fc) ) { 
        continue;
      }
      /* Build track2ipin_map for this IPIN */
      build_gsb_one_ipin_track2pin_map(rr_graph, rr_gsb, ipin_side, inode, ipin_Fc, 
                                       /* Give an offset for the first track that this ipin will connect to */
                                       offset[chan_side_manager.to_size_t()], 
                                       segment_inf, &track2ipin_map);
      /* update offset */
      offset[chan_side_manager.to_size_t()] += 2;
      //printf("offset[%lu]=%lu\n", chan_side_manager.to_size_t(), offset[chan_side_manager.to_size_t()]);
    }
  }

  return track2ipin_map;
}

/************************************************************************
 * Build the opin_to_track_map[gsb_side][0..num_opin_nodes-1][track_indices] 
 * based on the existing routing resources in the General Switch Block (GSB)
 * This function supports both X-directional and Y-directional tracks 
 * The mapping is done in the following steps:
 * 1. Build a list of routing tracks whose starting points locate at this GSB
 *    (xlow - gsb_x == 0)
 * 2. Divide the routing tracks by segment types, so that we can balance
 *    the connections between OPINs and different types of routing tracks.
 * 3. Scale the Fc of each pin to the actual number of routing tracks
 *    actual_Fc = (int) Fc * num_tracks / chan_width
 ***********************************************************************/
t_pin2track_map build_gsb_opin_to_track_map(t_rr_graph* rr_graph,
                                            const RRGSB& rr_gsb, 
                                            const std::vector<std::vector<t_grid_tile>> grids, 
                                            const std::vector<t_segment_inf> segment_inf, 
                                            int** Fc_out) {
  t_pin2track_map opin2track_map;
  /* Resize the matrix */ 
  opin2track_map.resize(rr_gsb.get_num_sides());
  
  /* offset counter: it aims to balance the OPIN-to-track for each switch block */
  std::vector<size_t> offset;
  /* Get the chan_side: which is the same as the opin side  */
  offset.resize(rr_gsb.get_num_sides());
  /* Initial offset */
  offset.assign(offset.size(), 0);
     
  /* Walk through each side */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    enum e_side opin_side = side_manager.get_side();
    /* Get the chan_side */
    /* This track2pin mapping is for Connection Blocks, so we only care two sides! */
    /* Get channel width and resize the matrix */
    size_t num_opin_nodes = rr_gsb.get_num_opin_nodes(opin_side);
    opin2track_map[side].resize(num_opin_nodes); 
    /* Find the ipin/opin nodes */
    for (size_t inode = 0; inode < num_opin_nodes; ++inode) {
      t_rr_node* opin_node = rr_gsb.get_opin_node(opin_side, inode);
      /* Skip EMPTY type */
      if (EMPTY_TYPE == grids[opin_node->xlow][opin_node->ylow].type) {
        continue;
      }
      int grid_type_index = grids[opin_node->xlow][opin_node->ylow].type->index; 
      /* Get Fc of the ipin */
      int opin_Fc = Fc_out[grid_type_index][opin_node->ptc_num];
      /* skip Fc = 0 or unintialized, those pins are in the <directlist> */
      if ( (-1 == opin_Fc)
        || (0 == opin_Fc) ) { 
        continue;
      }
      /* Build track2ipin_map for this IPIN */
      build_gsb_one_opin_pin2track_map(rr_graph, rr_gsb, opin_side, inode, opin_Fc, 
                                       /* Give an offset for the first track that this ipin will connect to */
                                       offset[side_manager.to_size_t()], 
                                       segment_inf, &opin2track_map);
      /* update offset: aim to rotate starting tracks by 1*/
      offset[side_manager.to_size_t()] += 1;
    }

    /* Check:
     * 1. We want to ensure that each OPIN will drive at least one track
     * 2. We want to ensure that each track will be driven by at least 1 OPIN */
  }

  return opin2track_map;
}


/************************************************************************
 * Add all direct clb-pin-to-clb-pin edges to given opin  
 ***********************************************************************/
void build_direct_connections_for_one_gsb(t_rr_graph* rr_graph,
                                          const DeviceCoordinator& device_size,
                                          const std::vector<std::vector<t_grid_tile>> grids,
                                          const DeviceCoordinator& from_grid_coordinator,
                                          const t_grid_tile& from_grid,
                                          const int delayless_switch, 
                                          const int num_directs, 
                                          const t_direct_inf *directs, 
                                          const t_clb_to_clb_directs *clb_to_clb_directs) {
  t_type_ptr grid_type = from_grid.type;

  /* Iterate through all direct connections */
  for (int i = 0; i < num_directs; ++i) {
    /* Bypass unmatched direct clb-to-clb connections */
    if (grid_type != clb_to_clb_directs[i].from_clb_type) {
      continue;
    }

    /* This opin is specified to connect directly to an ipin, 
     * now compute which ipin to connect to 
     */
    DeviceCoordinator to_grid_coordinator(from_grid_coordinator.get_x() + directs[i].x_offset, 
                                          from_grid_coordinator.get_y() + directs[i].y_offset);

    /* Bypass unmatched direct clb-to-clb connections */
    t_type_ptr to_grid_type = grids[to_grid_coordinator.get_x()][to_grid_coordinator.get_y()].type;
    /* Check if to_grid if the same grid */
    if (to_grid_type != clb_to_clb_directs[i].to_clb_type) {
      continue;
    }

    bool swap;
    int max_index, min_index;
    /* Compute index of opin with regards to given pins */ 
    if ( clb_to_clb_directs[i].from_clb_pin_start_index 
        > clb_to_clb_directs[i].from_clb_pin_end_index) {
      swap = true;
      max_index = clb_to_clb_directs[i].from_clb_pin_start_index;
      min_index = clb_to_clb_directs[i].from_clb_pin_end_index;
    } else {
      swap = false;
      min_index = clb_to_clb_directs[i].from_clb_pin_start_index;
      max_index = clb_to_clb_directs[i].from_clb_pin_end_index;
    }

    /* get every opin in the range */
    for (int opin = min_index; opin <= max_index; ++opin) {
      int offset = opin - min_index;

      if (  (to_grid_coordinator.get_x() < device_size.get_x() - 1) 
         && (to_grid_coordinator.get_y() < device_size.get_y() - 1) ) { 
        int ipin = OPEN;
        if ( clb_to_clb_directs[i].to_clb_pin_start_index 
          > clb_to_clb_directs[i].to_clb_pin_end_index) {
          if (true == swap) {
            ipin = clb_to_clb_directs[i].to_clb_pin_end_index + offset;
          } else {
            ipin = clb_to_clb_directs[i].to_clb_pin_start_index - offset;
          }
        } else {
          if(true == swap) {
            ipin = clb_to_clb_directs[i].to_clb_pin_end_index - offset;
          } else {
            ipin = clb_to_clb_directs[i].to_clb_pin_start_index + offset;
          }
        }

        /* Get the pin index in the rr_graph */
        int from_grid_ofs = from_grid.offset;
        int to_grid_ofs = grids[to_grid_coordinator.get_x()][to_grid_coordinator.get_y()].offset;
        int opin_node_id = get_rr_node_index(from_grid_coordinator.get_x(), 
                                             from_grid_coordinator.get_y() - from_grid_ofs, 
                                             OPIN, opin, rr_graph->rr_node_indices);
        int ipin_node_id = get_rr_node_index(to_grid_coordinator.get_x(), 
                                             to_grid_coordinator.get_y() - to_grid_ofs, 
                                             IPIN, ipin, rr_graph->rr_node_indices);
        /* add edges to the opin_node */
        add_one_edge_for_two_rr_nodes(rr_graph, opin_node_id, ipin_node_id,
                                      delayless_switch);
      }
    }
  }

  return;
}

/************************************************************************
 * End of file : rr_graph_tileable_gsb.cpp 
 ***********************************************************************/
