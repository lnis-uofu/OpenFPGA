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
#include "vpr_types.h"
#include "globals.h"
#include "vpr_utils.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "route_common.h"
#include "fpga_x2p_types.h"
#include "rr_graph_tileable_builder.h"

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
   ***********************************************************************/
   /* Check the channel width */
   int nodes_per_chan = chan_width;
   assert(chan_width > 0);
   t_seg_details *seg_details = NULL;
   seg_details = alloc_and_load_seg_details(&nodes_per_chan,
                                            std::max(L_nx, L_ny),
                                            num_seg_types, segment_inf,
                                            TRUE, FALSE, UNI_DIRECTIONAL);

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
  rr_graph.num_rr_nodes = 0;
  rr_graph.rr_node_indices = alloc_and_load_rr_node_indices(nodes_per_chan, L_nx, L_ny,
                                                            &(rr_graph.num_rr_nodes), seg_details);
  rr_graph.rr_node = (t_rr_node *) my_malloc(sizeof(t_rr_node) * rr_graph.num_rr_nodes);
  memset(rr_node, 0, sizeof(t_rr_node) * rr_graph.num_rr_nodes);
  boolean* L_rr_edge_done = (boolean *) my_malloc(sizeof(boolean) * rr_graph.num_rr_nodes);
  memset(L_rr_edge_done, 0, sizeof(boolean) * rr_graph.num_rr_nodes);

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
