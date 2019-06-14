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
 * Filename:    rr_graph_tileable_builder.c
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/11  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file contains a builder for the complex rr_graph data structure 
 *  Different from VPR rr_graph builders, this builder aims to create a 
 *  highly regular rr_graph, where each Connection Block (CB), Switch 
 *  Block (SB) is the same (except for those on the borders). Thus, the
 *  rr_graph is called tileable, which brings significant advantage in 
 *  producing large FPGA fabrics.
 ***********************************************************************/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include "vpr_types.h"
#include "globals.h"
#include "vpr_utils.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "route_common.h"
#include "fpga_x2p_types.h"
#include "rr_graph_tileable_builder.h"

#include "chan_node_details.h"

/************************************************************************
 * Local function in the file 
 ***********************************************************************/

/************************************************************************
 * Generate the number of tracks for each types of routing segments
 * w.r.t. the frequency of each of segments and channel width
 * Note that if we dertermine the number of tracks per type using
 *     chan_width * segment_frequency / total_freq may cause 
 * The total track num may not match the chan_width, 
 * therefore, we assign tracks one by one until we meet the frequency requirement
 * In this way, we can assign the number of tracks with repect to frequency 
 ***********************************************************************/
static 
std::vector<size_t> get_num_tracks_per_seg_type(size_t chan_width, 
                                                std::vector<t_segment_inf> segment_inf, 
                                                bool use_full_seg_groups) {
  std::vector<size_t> result;
  std::vector<double> demand;
  /* Make sure a clean start */
  result.resize(segment_inf.size());
  demand.resize(segment_inf.size());

  /* Scale factor so we can divide by any length
   * and still use integers */
  /* Get the sum of frequency */
  size_t scale = 1;
  size_t freq_sum = 0;
  for (size_t iseg = 0; iseg < segment_inf.size(); ++iseg) {
    scale *= segment_inf[iseg].length;
    freq_sum += segment_inf[iseg].frequency;
  }
  size_t reduce = scale * freq_sum;

  /* Init assignments to 0 and set the demand values */
  /* Get the fraction of each segment type considering the frequency:
   * num_track_per_seg = chan_width * (freq_of_seg / sum_freq)
   */
  for (size_t iseg = 0; iseg < segment_inf.size(); ++iseg) {
    result[iseg] = 0;
    demand[iseg] = scale * chan_width * segment_inf[iseg].frequency;
    if (true == use_full_seg_groups) {
      demand[iseg] /= segment_inf[iseg].length;
    }
  }

  /* check if the sum of num_tracks, matches the chan_width */
  /* Keep assigning tracks until we use them up */
  size_t assigned = 0;
  size_t size = 0;
  size_t imax = 0;
  while (assigned < chan_width) {
    /* Find current maximum demand */
    double max = 0;
    for (size_t iseg = 0; iseg < segment_inf.size(); ++iseg) {
      if (demand[iseg] > max) {
        imax = iseg;
      }
      max = std::max(demand[iseg], max); 
    }

    /* Assign tracks to the type and reduce the types demand */
    size = (use_full_seg_groups ? segment_inf[imax].length : 1);
    demand[imax] -= reduce;
    result[imax] += size;
    assigned += size;
  }

  /* Undo last assignment if we were closer to goal without it */
  if ((assigned - chan_width) > (size / 2)) {
    result[imax] -= size;
  }

  return result;
} 

/************************************************************************
 * Build details of routing tracks in a channel 
 * The function will 
 * 1. Assign the segments for each routing channel,
 *    To be specific, for each routing track, we assign a routing segment.
 *    The assignment is subject to users' specifications, such as 
 *    a. length of each type of segment
 *    b. frequency of each type of segment.
 *    c. routing channel width
 *
 * 2. The starting point of each segment in the channel will be assigned
 *    For each segment group with same directionality (tracks have the same length),
 *    every L track will be a starting point (where L denotes the length of segments)
 *    In this case, if the number of tracks is not a multiple of L,
 *    indeed we may have some <L segments. This can be considered as a side effect.
 *    But still the rr_graph is tileable, which is the first concern!
 *
 *    Here is a quick example of Length-4 wires in a W=12 routing channel
 *    +---------------------------------+
 *    | Index | Direction | Start Point |
 *    +---------------------------------+
 *    |   0   | --------> |   Yes       |
 *    +---------------------------------+
 *    |   1   | <-------- |   Yes       |
 *    +---------------------------------+
 *    |   2   | --------> |   No        |
 *    +---------------------------------+
 *    |   3   | <-------- |   No        |
 *    +---------------------------------+
 *    |   4   | --------> |   No        |
 *    +---------------------------------+
 *    |   5   | <-------- |   No        |
 *    +---------------------------------+
 *    |   7   | --------> |   No        |
 *    +---------------------------------+
 *    |   8   | <-------- |   No        |
 *    +---------------------------------+
 *    |   9   | --------> |   Yes       |
 *    +---------------------------------+
 *    |   10  | <-------- |   Yes       |
 *    +---------------------------------+
 *    |   11  | --------> |   No        |
 *    +---------------------------------+
 *    |   12  | <-------- |   No        |
 *    +---------------------------------+
 *
 * 3. SPECIAL for fringes: TOP|RIGHT|BOTTOM|RIGHT
 *    if device_side is NUM_SIDES, we assume this channel does not locate on borders
 *    All segments will start and ends with no exception
 *
 * 4. IMPORTANT: we should be aware that channel width maybe different 
 *    in X-direction and Y-direction channels!!!
 *    So we will load segment details for different channels 
 ***********************************************************************/
static 
ChanNodeDetails build_unidir_chan_node_details(size_t chan_width, size_t max_seg_length,
                                               enum e_side device_side, 
                                               std::vector<t_segment_inf> segment_inf) {
  ChanNodeDetails chan_node_details;
  /* Correct the chan_width: it should be an even number */
  if (0 != chan_width % 2) {
    chan_width++; /* increment it to be even */
  }
  assert (0 == chan_width % 2);
  
  /* Reserve channel width */
  chan_node_details.reserve(chan_width);
  /* Return if zero width is forced */
  if (0 == chan_width) {
    return chan_node_details; 
  }

  /* Find the number of segments required by each group */
  std::vector<size_t> num_tracks = get_num_tracks_per_seg_type(chan_width/2, segment_inf, TRUE);  

  /* Add node to ChanNodeDetails */
  size_t cur_track = 0;
  for (size_t iseg = 0; iseg < segment_inf.size(); ++iseg) {
    /* segment length will be set to maxium segment length if this is a longwire */
    size_t seg_len = segment_inf[iseg].length;
    if (TRUE == segment_inf[iseg].longline) {
       seg_len = max_seg_length;
    } 
    for (size_t itrack = 0; itrack < num_tracks[iseg]; ++itrack) {
      bool seg_start = false;
      /* Every length of wire, we set a starting point */
      if (0 == itrack % seg_len) {
        seg_start = true;
      }
      /* Since this is a unidirectional routing architecture,
       * Add a pair of tracks, 1 INC_DIRECTION track and 1 DEC_DIRECTION track 
       */
      chan_node_details.add_track(cur_track, INC_DIRECTION, seg_len, seg_start, false);
      cur_track++;
      chan_node_details.add_track(cur_track, DEC_DIRECTION, seg_len, seg_start, false);
      cur_track++;
    }    
  }
  /* Check if all the tracks have been satisified */ 
  assert (cur_track == chan_width);

  /* If this is on the border of a device, segments should start */
  switch (device_side) {
  case TOP:
  case RIGHT:
    /* INC_DIRECTION should all end */
    chan_node_details.set_tracks_end(INC_DIRECTION);
    /* DEC_DIRECTION should all start */
    chan_node_details.set_tracks_start(DEC_DIRECTION);
    break;
  case BOTTOM:
  case LEFT:
    /* INC_DIRECTION should all start */
    chan_node_details.set_tracks_start(INC_DIRECTION);
    /* DEC_DIRECTION should all end */
    chan_node_details.set_tracks_end(DEC_DIRECTION);
    break;
  case NUM_SIDES:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid device_side!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return chan_node_details; 
}

/************************************************************************
 * Estimate the number of rr_nodes per category:
 * CHANX, CHANY, IPIN, OPIN, SOURCE, SINK 
 ***********************************************************************/
static 
std::vector<size_t> estimate_num_rr_nodes_per_type() {
  std::vector<size_t> num_rr_nodes_per_type;


  return num_rr_nodes_per_type;
}


/************************************************************************
 * Main function of this file
 * Builder for a detailed uni-directional tileable rr_graph
 * Global graph is not supported here, the VPR rr_graph generator can be used  
 * It follows the procedures to complete the rr_graph generation
 * 1. Assign the segments for each routing channel,
 *    To be specific, for each routing track, we assign a routing segment.
 *    The assignment is subject to users' specifications, such as 
 *    a. length of each type of segment
 *    b. frequency of each type of segment.
 *    c. routing channel width
 * 2. Estimate the number of nodes in the rr_graph
 *    This will estimate the number of 
 *    a. IPINs, input pins of each grid
 *    b. OPINs, output pins of each grid
 *    c. SOURCE, virtual node which drives OPINs
 *    d. SINK, virtual node which is connected to IPINs
 *    e. CHANX and CHANY, routing segments of each channel
 * 3. Create the connectivity of OPINs
 *    a. Evenly assign connections to OPINs to routing tracks
 *    b. the connection pattern should be same across the fabric
 * 4. Create the connectivity of IPINs 
 *    a. Evenly assign connections from routing tracks to IPINs
 *    b. the connection pattern should be same across the fabric
 * 5. Create the switch block patterns, 
 *    It is based on the type of switch block, the supported patterns are 
 *    a. Disjoint, which connects routing track (i)th from (i)th and (i)th routing segments
 *    b. Universal, which connects routing track (i)th from (i)th and (M-i)th routing segments
 *    c. Wilton, which rotates the connection of Disjoint by 1 track
 * 6. Allocate rr_graph, fill the node information
 *    For each node, fill
 *    a. basic information: coordinator(xlow, xhigh, ylow, yhigh), ptc_num
 *    b. edges (both incoming and outcoming)
 *    c. handle direct-connections
 * 7. Build fast look-up for the rr_graph 
 * 8. Allocate external data structures
 *    a. cost_index
 *    b. RC tree
 ***********************************************************************/
t_rr_graph build_tileable_unidir_rr_graph(INP int L_num_types,
    INP t_type_ptr types, INP int L_nx, INP int L_ny,
    INP struct s_grid_tile **L_grid, INP int chan_width,
    INP struct s_chan_width_dist *chan_capacity_inf,
    INP enum e_switch_block_type sb_type, INP int Fs, INP int num_seg_types,
    INP int num_switches, INP t_segment_inf * segment_inf,
    INP int global_route_switch, INP int delayless_switch,
    INP t_timing_inf timing_inf, INP int wire_to_ipin_switch,
    INP enum e_base_cost_type base_cost_type, INP t_direct_inf *directs, 
    INP int num_directs, INP boolean ignore_Fc_0, OUTP int *Warnings,
    /*Xifan TANG: Switch Segment Pattern Support*/
    INP int num_swseg_pattern, INP t_swseg_pattern_inf* swseg_patterns,
    INP boolean opin_to_cb_fast_edges, INP boolean opin_logic_eq_edges) { 
  /* Create an empty graph */
  t_rr_graph rr_graph; 
  rr_graph.rr_node_indices = NULL;
  rr_graph.rr_node = NULL;
  rr_graph.num_rr_nodes = 0;

  /* Reset warning flag */
  *Warnings = RR_GRAPH_NO_WARN;

  /************************************************************************
   * 1. Assign the segments for each routing channel,
   *    To be specific, for each routing track, we assign a routing segment.
   *    The assignment is subject to users' specifications, such as 
   *    a. length of each type of segment
   *    b. frequency of each type of segment.
   *    c. routing channel width
   *
   *    The starting point of each segment in the channel will be assigned
   *    For each segment group with same directionality (tracks have the same length),
   *    every L track will be a starting point (where L denotes the length of segments)
   *    In this case, if the number of tracks is not a multiple of L,
   *    indeed we may have some <L segments. This can be considered as a side effect.
   *    But still the rr_graph is tileable, which is the first concern!
   *
   *    Here is a quick example of Length-4 wires in a W=12 routing channel
   *    +---------------------------------+
   *    | Index | Direction | Start Point |
   *    +---------------------------------+
   *    |   0   | --------> |   Yes       |
   *    +---------------------------------+
   *    |   1   | <-------- |   Yes       |
   *    +---------------------------------+
   *    |   2   | --------> |   No        |
   *    +---------------------------------+
   *    |   3   | <-------- |   No        |
   *    +---------------------------------+
   *    |   4   | --------> |   No        |
   *    +---------------------------------+
   *    |   5   | <-------- |   No        |
   *    +---------------------------------+
   *    |   7   | --------> |   No        |
   *    +---------------------------------+
   *    |   8   | <-------- |   No        |
   *    +---------------------------------+
   *    |   9   | --------> |   Yes       |
   *    +---------------------------------+
   *    |   10  | <-------- |   Yes       |
   *    +---------------------------------+
   *    |   11  | --------> |   No        |
   *    +---------------------------------+
   *    |   12  | <-------- |   No        |
   *    +---------------------------------+
   *
   *    SPECIAL for fringes:
   *    All segments will start and ends with no exception
   *
   *    IMPORTANT: we should be aware that channel width maybe different 
   *    in X-direction and Y-direction channels!!!
   *    So we will load segment details for different channels 
   ***********************************************************************/
   /* Check the channel width */
   int nodes_per_chan = chan_width;
   assert(chan_width > 0);

   /* Create a vector of segment_inf */
   std::vector<t_segment_inf> segment_infs;
   for (int iseg = 0; iseg < num_seg_types; ++iseg) {
     segment_infs.push_back(segment_inf[iseg]);
   }

   ChanNodeDetails chanx_details = build_unidir_chan_node_details(nodes_per_chan, L_nx, NUM_SIDES, segment_infs); 
   ChanNodeDetails chany_details = build_unidir_chan_node_details(nodes_per_chan, L_ny, NUM_SIDES, segment_infs); 

   /* Predict the track index of each channel,
    * The track index, also called ptc_num of each CHANX and CHANY rr_node
    * Will rotate by 2 in a uni-directional tileable routing architecture
    * Vectors are built here to record the ptc_num sequence in each channel 
    */

  /************************************************************************
   * 2. Estimate the number of nodes in the rr_graph
   *    This will estimate the number of 
   *    a. IPINs, input pins of each grid
   *    b. OPINs, output pins of each grid
   *    c. SOURCE, virtual node which drives OPINs
   *    d. SINK, virtual node which is connected to IPINs
   *    e. CHANX and CHANY, routing segments of each channel
   ***********************************************************************/
  std::vector<size_t> num_rr_nodes_per_type = estimate_num_rr_nodes_per_type();


  /************************************************************************
   * 3. Create the connectivity of OPINs
   *    a. Evenly assign connections to OPINs to routing tracks
   *    b. the connection pattern should be same across the fabric
   ***********************************************************************/
  int **Fc_in = NULL; /* [0..num_types-1][0..num_pins-1] */
  boolean Fc_clipped;
  Fc_clipped = FALSE;
  Fc_in = alloc_and_load_actual_fc(L_num_types, types, nodes_per_chan,
                                   FALSE, UNI_DIRECTIONAL, &Fc_clipped, ignore_Fc_0);
  if (Fc_clipped) {
    *Warnings |= RR_GRAPH_WARN_FC_CLIPPED;
  }

  /************************************************************************
   * 4. Create the connectivity of IPINs 
   *    a. Evenly assign connections from routing tracks to IPINs
   *    b. the connection pattern should be same across the fabric
   ***********************************************************************/
  int **Fc_out = NULL; /* [0..num_types-1][0..num_pins-1] */
  Fc_clipped = FALSE;
  Fc_out = alloc_and_load_actual_fc(L_num_types, types, nodes_per_chan,
                                   TRUE, UNI_DIRECTIONAL, &Fc_clipped, ignore_Fc_0);

  /************************************************************************
   * 6. Allocate rr_graph, fill the node information
   *    For each node, fill
   *    a. basic information: coordinator(xlow, xhigh, ylow, yhigh), ptc_num
   *    b. edges (both incoming and outcoming)
   *    c. handle direct-connections
   ***********************************************************************/
  /* Alloc node lookups, count nodes, alloc rr nodes */
  /*
  rr_graph.num_rr_nodes = 0;
  rr_graph.rr_node_indices = alloc_and_load_rr_node_indices(nodes_per_chan, L_nx, L_ny,
                                                            &(rr_graph.num_rr_nodes), seg_details);
  rr_graph.rr_node = (t_rr_node *) my_malloc(sizeof(t_rr_node) * rr_graph.num_rr_nodes);
  memset(rr_node, 0, sizeof(t_rr_node) * rr_graph.num_rr_nodes);
  boolean* L_rr_edge_done = (boolean *) my_malloc(sizeof(boolean) * rr_graph.num_rr_nodes);
  memset(L_rr_edge_done, 0, sizeof(boolean) * rr_graph.num_rr_nodes);
  */

  /* handle direct-connections */
  t_clb_to_clb_directs* clb_to_clb_directs = NULL;
  if (num_directs > 0) {
    clb_to_clb_directs = alloc_and_load_clb_to_clb_directs(directs, num_directs);
  }

  /************************************************************************
   * 8. Allocate external data structures
   *    a. cost_index
   *    b. RC tree
   ***********************************************************************/
  rr_graph_externals(timing_inf, segment_inf, num_seg_types, nodes_per_chan,
                     wire_to_ipin_switch, base_cost_type);

  return rr_graph;
}


/************************************************************************
 * End of file : rr_graph_tileable_builder.c 
 ***********************************************************************/
