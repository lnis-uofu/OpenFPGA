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
 * Filename:    rr_graph_tileable_sbox.cpp
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

#include "vpr_types.h"
#include "rr_graph_tileable_sbox.h"

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

  /* INC_DIRECTION start_track: (xlow, ylow) should be same as the GSB side coordinator */
  if (  ((size_t)track_node->xlow == side_coordinator.get_x())
     && ((size_t)track_node->ylow == side_coordinator.get_y()) 
     && (OUT_PORT == rr_gsb.get_chan_node_direction(gsb_side, track_id)) ) {
    /* Double check: start track should be an OUTPUT PORT of the GSB */
    track_status = TRACK_START;
  }
  /* INC_DIRECTION end_track: (xhigh, yhigh) should be same as the GSB side coordinator */ 
  if (  ((size_t)track_node->xhigh == side_coordinator.get_x()) 
     && ((size_t)track_node->yhigh == side_coordinator.get_y()) 
     && (IN_PORT == rr_gsb.get_chan_node_direction(gsb_side, track_id)) ) {
    /* Double check: end track should be an INPUT PORT of the GSB */
    track_status = TRACK_END;
  }

  return track_status;
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

  /* Get the offset */
  size_t offset = (side_coordinator.get_x() - track_node->xlow) 
                + (side_coordinator.get_y() - track_node->ylow); 
  
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
  for (int i = 0; i < Fs; i = i + 3) {  
    /* TODO: currently, for Fs > 3, I always search the next from_track until Fs is satisfied 
     * The optimal track selection should be done in a more scientific way!!! 
     */
    size_t to_track_i = to_track + i;
    /* make sure the track id is still in range */
    if ( to_track_i > (size_t)num_to_tracks) {
      to_track_i = to_track_i % num_to_tracks; 
    }
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

  /* TODO: currently, for Fs > 3, I always search the next from_track until Fs is satisfied 
   * The optimal track selection should be done in a more scientific way!!! 
   */

  assert (0 == Fs % 3);

  switch (switch_block_type) {
  case SUBSET: /* NB:  Global routing uses SUBSET too */
    to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
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
        to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
      } else if (to_side == side_manager.get_rotate_clockwise()) {
        to_tracks = get_to_track_list(Fs, num_to_tracks - 1 - from_track, num_to_tracks);
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
        to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
      } else if (to_side == side_manager.get_rotate_counterclockwise()) {
        to_tracks = get_to_track_list(Fs, num_to_tracks - 1 - from_track, num_to_tracks);
      }
    }
    /* Finish, we return */
    return to_tracks;
    /* End switch_block_type == UNIVERSAL case. */
  case WILTON:
    /* See S. Wilton Phd thesis, U of T, 1996 p. 103 for details on following. */
    if (from_side == LEFT) {
      if (to_side == RIGHT) { /* CHANX to CHANX */
        to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
      } else if (to_side == TOP) { /* from CHANX to CHANY */
        to_tracks = get_to_track_list(Fs, (num_to_tracks - (from_track % num_to_tracks)) % num_to_tracks, num_to_tracks);
      } else if (to_side == BOTTOM) {
        to_tracks = get_to_track_list(Fs, (num_to_tracks + from_track - 1) % num_to_tracks, num_to_tracks);
      }
    } else if (from_side == RIGHT) {
      if (to_side == LEFT) { /* CHANX to CHANX */
        to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
      } else if (to_side == TOP) { /* from CHANX to CHANY */
        to_tracks = get_to_track_list(Fs, (num_to_tracks + from_track - 1) % num_to_tracks, num_to_tracks);
      } else if (to_side == BOTTOM) {
        to_tracks = get_to_track_list(Fs, (num_to_tracks - 2 - from_track) % num_to_tracks, num_to_tracks);
      }
    } else if (from_side == BOTTOM) {
      if (to_side == TOP) { /* CHANY to CHANY */
        to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
      } else if (to_side == LEFT) { /* from CHANY to CHANX */
        to_tracks = get_to_track_list(Fs, (from_track + 1) % num_to_tracks, num_to_tracks);
      } else if (to_side == RIGHT) {
        to_tracks = get_to_track_list(Fs, (2 * num_to_tracks - 2 - from_track) % num_to_tracks, num_to_tracks);
      }
    } else if (from_side == TOP) {
      if (to_side == BOTTOM) { /* CHANY to CHANY */
        to_tracks = get_to_track_list(Fs, from_track, num_to_tracks);
      } else if (to_side == LEFT) { /* from CHANY to CHANX */
        to_tracks = get_to_track_list(Fs, (num_to_tracks - (from_track % num_to_tracks)) % num_to_tracks, num_to_tracks);
      } else if (to_side == RIGHT) {
        to_tracks = get_to_track_list(Fs, (from_track + 1) % num_to_tracks, num_to_tracks);
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
                                            const t_track_group from_tracks, /* [0..gsb_side][track_indices] */
                                            const t_track_group to_tracks, /* [0..gsb_side][track_indices] */
                                            t_track2track_map* track2track_map) {
  for (size_t side = 0; side < from_tracks.size(); ++side) {
    Side side_manager(side);
    enum e_side gsb_side = side_manager.get_side();
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
        /* Get other track_ids depending on the switch block pattern */
        /* Find the track ids that will start at the other sides */
        std::vector<size_t> to_track_ids = get_switch_block_to_track_id(sb_type, Fs, gsb_side, inode, 
                                                                        to_side, 
                                                                        to_tracks[to_side_index].size()); 
        /* Update the track2track_map: */
        for (size_t to_track_id = 0; to_track_id < to_track_ids.size(); ++to_track_id) {
          size_t from_side_index = side_manager.to_size_t();
          size_t from_track_index = from_tracks[side][inode];
          size_t to_track_index = to_tracks[to_side_index][to_track_ids[to_track_id]];
          t_rr_node* to_track_node = rr_gsb.get_chan_node(to_side, to_track_index);
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
      /* check if this track will start from here */
      enum e_track_status track_status = determine_track_status_of_gsb(rr_gsb, gsb_side, inode);
      switch (track_status) {
      case TRACK_START:
        /* update starting track list */
        start_tracks[gsb_side].push_back(inode);
        break;
      case TRACK_END:
        /* Update end track list */
        end_tracks[gsb_side].push_back(inode);
        break;
      case TRACK_PASS:
        /* We need to check Switch block population of this track 
         * The track node will not be considered if there supposed to be no SB at this position 
         */
        if (true == is_gsb_in_track_sb_population(rr_gsb, gsb_side, inode, segment_inf)) {
          /* Update passing track list */
          pass_tracks[gsb_side].push_back(inode);
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
                                         end_tracks, start_tracks, 
                                         &track2track_map);

  /* For Group 2: we build connections between end_tracks and start_tracks*/
  /* Currently, I use the same Switch Block pattern for the passing tracks and end tracks,
   * TODO: This can be improved with different patterns! 
   */
  build_gsb_one_group_track_to_track_map(rr_graph, rr_gsb, 
                                         sb_type, Fs,
                                         pass_tracks, start_tracks, 
                                         &track2track_map);

  return track2track_map;
}

