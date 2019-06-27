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
 * Filename:    rr_graph_tileable_builder.cpp
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
#include <algorithm>

#include "vtr_ndmatrix.h"

#include "vpr_types.h"
#include "globals.h"
#include "vpr_utils.h"
#include "rr_graph_util.h"
#include "ReadOptions.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "check_rr_graph.h"
#include "route_common.h"
#include "fpga_x2p_types.h"

#include "rr_blocks.h"
#include "chan_node_details.h"
#include "device_coordinator.h"

#include "rr_graph_builder_utils.h"
#include "tileable_chan_details_builder.h"
#include "tileable_rr_graph_gsb.h"
#include "tileable_rr_graph_builder.h"

/************************************************************************
 * Local data stuctures in the file 
 ***********************************************************************/

/************************************************************************
 * Local function in the file 
 ***********************************************************************/

/************************************************************************
 * Estimate the number of rr_nodes per category:
 * CHANX, CHANY, IPIN, OPIN, SOURCE, SINK 
 ***********************************************************************/
static 
std::vector<size_t> estimate_num_rr_nodes_per_type(const DeviceCoordinator& device_size,
                                                   const std::vector<std::vector<t_grid_tile>> grids,
                                                   const std::vector<size_t> chan_width,
                                                   const std::vector<t_segment_inf> segment_infs) {
  std::vector<size_t> num_rr_nodes_per_type;
  /* reserve the vector: 
   * we have the follow type:
   * SOURCE = 0, SINK, IPIN, OPIN, CHANX, CHANY, INTRA_CLUSTER_EDGE, NUM_RR_TYPES
   * NUM_RR_TYPES and INTRA_CLUSTER_EDGE will be 0
   */
  num_rr_nodes_per_type.resize(NUM_RR_TYPES);
  /* Make sure a clean start */
  for (size_t i = 0; i < NUM_RR_TYPES; ++i) {
    num_rr_nodes_per_type[i] = 0;
  }

  /************************************************************************
   * 1. Search the grid and find the number OPINs and IPINs per grid
   *    Note that the number of SOURCE nodes are the same as OPINs
   *    and the number of SINK nodes are the same as IPINs
   ***********************************************************************/
  for (size_t ix = 0; ix < device_size.get_x(); ++ix) {
    for (size_t iy = 0; iy < device_size.get_y(); ++iy) { 
      /* Skip EMPTY tiles */
      if (EMPTY_TYPE == grids[ix][iy].type) {
        continue;
      }
      /* Skip height>1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[ix][iy].offset) {
        continue;
      }
      enum e_side io_side = NUM_SIDES;
      /* If this is the block on borders, we consider IO side */
      if (IO_TYPE == grid[ix][iy].type) {
        DeviceCoordinator io_device_size(device_size.get_x() - 1, device_size.get_y() - 1);
        DeviceCoordinator grid_coordinator(ix, iy);
        io_side = determine_io_grid_pin_side(io_device_size, grid_coordinator);
      }
      /* get the number of OPINs */
      num_rr_nodes_per_type[OPIN] += get_grid_num_pins(grids[ix][iy], DRIVER, io_side);
      /* get the number of IPINs */
      num_rr_nodes_per_type[IPIN] += get_grid_num_pins(grids[ix][iy], RECEIVER, io_side);
      /* SOURCE: number of classes whose type is DRIVER */
      num_rr_nodes_per_type[SOURCE] += get_grid_num_classes(grid[ix][iy], DRIVER);
      /* SINK: number of classes whose type is RECEIVER */
      num_rr_nodes_per_type[SINK]   += get_grid_num_classes(grid[ix][iy], RECEIVER);
    }
  }

  /************************************************************************
   * 2. Assign the segments for each routing channel,
   *    To be specific, for each routing track, we assign a routing segment.
   *    The assignment is subject to users' specifications, such as 
   *    a. length of each type of segment
   *    b. frequency of each type of segment.
   *    c. routing channel width
   *
   *    SPECIAL for fringes:
   *    All segments will start and ends with no exception
   *
   *    IMPORTANT: we should be aware that channel width maybe different 
   *    in X-direction and Y-direction channels!!!
   *    So we will load segment details for different channels 
   ***********************************************************************/
  /* For X-direction Channel: CHANX */
  for (size_t iy = 0; iy < device_size.get_y() - 1; ++iy) { 
    for (size_t ix = 1; ix < device_size.get_x() - 1; ++ix) {
      enum e_side chan_side = NUM_SIDES;
      /* For LEFT side of FPGA */
      if (1 == ix) {
        chan_side = LEFT;
      }
      /* For RIGHT side of FPGA */
      if (device_size.get_x() - 2 == ix) {
        chan_side = RIGHT;
      }
      ChanNodeDetails chanx_details = build_unidir_chan_node_details(chan_width[0], device_size.get_x() - 2, chan_side, segment_infs); 
      /* When an INC_DIRECTION CHANX starts, we need a new rr_node */
      num_rr_nodes_per_type[CHANX] += chanx_details.get_num_starting_tracks(INC_DIRECTION);
      /* When an DEC_DIRECTION CHANX ends, we need a new rr_node */
      num_rr_nodes_per_type[CHANX] += chanx_details.get_num_ending_tracks(DEC_DIRECTION);
    }
  }

  /* For Y-direction Channel: CHANX */
  for (size_t ix = 0; ix < device_size.get_x() - 1; ++ix) {
    for (size_t iy = 1; iy < device_size.get_y() - 1; ++iy) { 
      enum e_side chan_side = NUM_SIDES;
      /* For LEFT side of FPGA */
      if (1 == iy) {
        chan_side = BOTTOM;
      }
      /* For RIGHT side of FPGA */
      if (device_size.get_y() - 2 == iy) {
        chan_side = TOP;
      }
      ChanNodeDetails chany_details = build_unidir_chan_node_details(chan_width[1], device_size.get_y() - 2, chan_side, segment_infs); 
      /* When an INC_DIRECTION CHANX starts, we need a new rr_node */
      num_rr_nodes_per_type[CHANY] += chany_details.get_num_starting_tracks(INC_DIRECTION);
      /* When an DEC_DIRECTION CHANX ends, we need a new rr_node */
      num_rr_nodes_per_type[CHANY] += chany_details.get_num_ending_tracks(DEC_DIRECTION);
    }
  }

  return num_rr_nodes_per_type;
}

/************************************************************************
 * Configure one rr_node to the fast-look up of a rr_graph 
 ***********************************************************************/
static 
void load_one_node_to_rr_graph_fast_lookup(t_rr_graph* rr_graph, const int node_index,
                                           const t_rr_type node_type, 
                                           const int x, const int y, 
                                           const int ptc_num) {
  /* check the size of ivec (nelem), 
   * if the ptc_num exceeds the size limit, we realloc the ivec */
  if (ptc_num + 1 > rr_graph->rr_node_indices[node_type][x][y].nelem) {
    rr_graph->rr_node_indices[node_type][x][y].nelem = ptc_num + 1;
    rr_graph->rr_node_indices[node_type][x][y].list = (int*) my_realloc(rr_graph->rr_node_indices[node_type][x][y].list, sizeof(int) * (ptc_num + 1)); 
  }
  /* fill the lookup table */
  rr_graph->rr_node_indices[node_type][x][y].list[ptc_num] = node_index;
  return;
}

/************************************************************************
 * Configure rr_nodes for this grid 
 * coordinators: xlow, ylow, xhigh, yhigh, 
 * features: capacity, ptc_num (pin_num),
 ***********************************************************************/
static 
void load_one_grid_rr_nodes_basic_info(const DeviceCoordinator& grid_coordinator, 
                                       const t_grid_tile& cur_grid, 
                                       const enum e_side io_side, 
                                       t_rr_graph* rr_graph, 
                                       size_t* cur_node_id,
                                       const int wire_to_ipin_switch, const int delayless_switch) {
  Side io_side_manager(io_side);

  /* Walk through the height of each grid,
   * get pins and configure the rr_nodes */
  for (int height = 0; height < cur_grid.type->height; ++height) {
    /* Walk through sides */
    for (size_t side = 0; side < NUM_SIDES; ++side) {
      Side side_manager(side);
      /* skip unwanted sides */
      if ( (IO_TYPE == cur_grid.type)
        && (side != io_side_manager.to_size_t()) ) { 
        continue;
      }
      /* Find OPINs */
      /* Configure pins by pins */
      std::vector<int> opin_list = get_grid_side_pins(cur_grid, DRIVER, side_manager.get_side(), height);
      for (size_t pin = 0; pin < opin_list.size(); ++pin) {
        /* Configure the rr_node for the OPIN */
        rr_graph->rr_node[*cur_node_id].type  = OPIN; 
        rr_graph->rr_node[*cur_node_id].xlow  = grid_coordinator.get_x(); 
        rr_graph->rr_node[*cur_node_id].xhigh = grid_coordinator.get_x(); 
        rr_graph->rr_node[*cur_node_id].ylow  = grid_coordinator.get_y(); 
        rr_graph->rr_node[*cur_node_id].yhigh = grid_coordinator.get_y(); 
        rr_graph->rr_node[*cur_node_id].ptc_num  = opin_list[pin]; 
        rr_graph->rr_node[*cur_node_id].capacity = 1; 
        rr_graph->rr_node[*cur_node_id].occ = 0; 
        /* cost index is a FIXED value for OPIN */
        rr_graph->rr_node[*cur_node_id].cost_index = OPIN_COST_INDEX; 
        /* Switch info */
        rr_graph->rr_node[*cur_node_id].driver_switch = delayless_switch; 
        /* fill fast look-up table */
        load_one_node_to_rr_graph_fast_lookup(rr_graph, *cur_node_id, 
                                              rr_graph->rr_node[*cur_node_id].type, 
                                              rr_graph->rr_node[*cur_node_id].xlow, 
                                              rr_graph->rr_node[*cur_node_id].ylow,
                                              rr_graph->rr_node[*cur_node_id].ptc_num);
        /* Update node counter */
        (*cur_node_id)++;
      } /* End of loading OPIN rr_nodes */
      /* Find IPINs */
      /* Configure pins by pins */
      std::vector<int> ipin_list = get_grid_side_pins(cur_grid, RECEIVER, side_manager.get_side(), height);
      for (size_t pin = 0; pin < ipin_list.size(); ++pin) {
        rr_graph->rr_node[*cur_node_id].type  = IPIN; 
        rr_graph->rr_node[*cur_node_id].xlow  = grid_coordinator.get_x(); 
        rr_graph->rr_node[*cur_node_id].xhigh = grid_coordinator.get_x(); 
        rr_graph->rr_node[*cur_node_id].ylow  = grid_coordinator.get_y(); 
        rr_graph->rr_node[*cur_node_id].yhigh = grid_coordinator.get_y(); 
        rr_graph->rr_node[*cur_node_id].ptc_num  = ipin_list[pin]; 
        rr_graph->rr_node[*cur_node_id].capacity = 1; 
        rr_graph->rr_node[*cur_node_id].occ = 0; 
        /* cost index is a FIXED value for IPIN */
        rr_graph->rr_node[*cur_node_id].cost_index = IPIN_COST_INDEX; 
        /* Switch info */
        rr_graph->rr_node[*cur_node_id].driver_switch = wire_to_ipin_switch; 
        /* fill fast look-up table */
        load_one_node_to_rr_graph_fast_lookup(rr_graph, *cur_node_id, 
                                              rr_graph->rr_node[*cur_node_id].type, 
                                              rr_graph->rr_node[*cur_node_id].xlow, 
                                              rr_graph->rr_node[*cur_node_id].ylow,
                                              rr_graph->rr_node[*cur_node_id].ptc_num);
        /* Update node counter */
        (*cur_node_id)++;
      } /* End of loading IPIN rr_nodes */
    } /* End of side enumeration */
  } /* End of height enumeration */

  /* Walk through the height of each grid,
   * get pins and configure the rr_nodes */
  for (int height = 0; height < cur_grid.type->height; ++height) {
    /* Set a SOURCE or a SINK rr_node for each class */
    for (int iclass = 0; iclass < cur_grid.type->num_class; ++iclass) {
      /* Set a SINK rr_node for the OPIN */
      if ( DRIVER == cur_grid.type->class_inf[iclass].type) {
        rr_graph->rr_node[*cur_node_id].type  = SOURCE; 
      } 
      if ( RECEIVER == cur_grid.type->class_inf[iclass].type) {
        rr_graph->rr_node[*cur_node_id].type  = SINK; 
      }
      rr_graph->rr_node[*cur_node_id].xlow  = grid_coordinator.get_x(); 
      rr_graph->rr_node[*cur_node_id].xhigh = grid_coordinator.get_x(); 
      rr_graph->rr_node[*cur_node_id].ylow  = grid_coordinator.get_y(); 
      rr_graph->rr_node[*cur_node_id].yhigh = grid_coordinator.get_y(); 
      rr_graph->rr_node[*cur_node_id].ptc_num  = iclass; 
      /* FIXME: need to confirm if the capacity should be the number of pins in this class*/ 
      rr_graph->rr_node[*cur_node_id].capacity = cur_grid.type->class_inf[iclass].num_pins; 
      rr_graph->rr_node[*cur_node_id].occ = 0; 
      /* cost index is a FIXED value for SOURCE and SINK */
      if (SOURCE == rr_graph->rr_node[*cur_node_id].type) {
        rr_graph->rr_node[*cur_node_id].cost_index = SOURCE_COST_INDEX; 
      }
      if (SINK == rr_graph->rr_node[*cur_node_id].type) {
        rr_graph->rr_node[*cur_node_id].cost_index = SINK_COST_INDEX; 
      }
      /* Switch info */
      rr_graph->rr_node[*cur_node_id].driver_switch = delayless_switch; 
      /* TODO: should we set pb_graph_pin here? */
      /* fill fast look-up table */
      load_one_node_to_rr_graph_fast_lookup(rr_graph, *cur_node_id, 
                                            rr_graph->rr_node[*cur_node_id].type, 
                                            rr_graph->rr_node[*cur_node_id].xlow, 
                                            rr_graph->rr_node[*cur_node_id].ylow,
                                            rr_graph->rr_node[*cur_node_id].ptc_num);
      /* Update node counter */
      (*cur_node_id)++;
    } /* End of height enumeration */
  } /* End of pin_class enumeration */

  return;
}

/************************************************************************
 * Initialize the basic information of routing track rr_nodes
 * coordinators: xlow, ylow, xhigh, yhigh, 
 * features: capacity, track_ids, ptc_num, direction 
 ***********************************************************************/
static 
void load_one_chan_rr_nodes_basic_info(const DeviceCoordinator& chan_coordinator, 
                                       const t_rr_type chan_type,
                                       ChanNodeDetails* chan_details,
                                       const std::vector<t_segment_inf> segment_infs,
                                       const int cost_index_offset,
                                       t_rr_graph* rr_graph,
                                       size_t* cur_node_id) {
  /* Check each node_id(potential ptc_num) in the channel :
   * If this is a starting point, we set a new rr_node with xlow/ylow, ptc_num
   * If this is a ending point, we set xhigh/yhigh and track_ids
   * For other nodes, we set changes in track_ids
   */
  for (size_t itrack = 0; itrack < chan_details->get_chan_width(); ++itrack) {
    /* For INC direction, a starting point requires a new chan rr_node  */
    if ( ( (true == chan_details->is_track_start(itrack))
        && (INC_DIRECTION == chan_details->get_track_direction(itrack)) ) 
    /* For DEC direction, an ending point requires a new chan rr_node  */
      ||
       ( (true == chan_details->is_track_end(itrack))
      && (DEC_DIRECTION == chan_details->get_track_direction(itrack)) ) ) {
      /* Use a new chan rr_node  */
      rr_graph->rr_node[*cur_node_id].type  = chan_type; 
      rr_graph->rr_node[*cur_node_id].xlow  = chan_coordinator.get_x(); 
      rr_graph->rr_node[*cur_node_id].ylow  = chan_coordinator.get_y(); 
      rr_graph->rr_node[*cur_node_id].direction = chan_details->get_track_direction(itrack); 
      rr_graph->rr_node[*cur_node_id].ptc_num  = itrack; 
      rr_graph->rr_node[*cur_node_id].track_ids.push_back(itrack); 
      rr_graph->rr_node[*cur_node_id].capacity = 1; 
      rr_graph->rr_node[*cur_node_id].occ = 0; 
      /* assign switch id */
      size_t seg_id = chan_details->get_track_segment_id(itrack);
      rr_graph->rr_node[*cur_node_id].driver_switch = segment_infs[seg_id].opin_switch; 
      /* Update chan_details with node_id */
      chan_details->set_track_node_id(itrack, *cur_node_id);
      /* cost index depends on the segment index */
      rr_graph->rr_node[*cur_node_id].cost_index = cost_index_offset + seg_id; 

      /* Update node counter */
      (*cur_node_id)++;
      /* Finish here, go to next */
    }

    /* For INC direction, an ending point requires an update on xhigh and yhigh  */
    if (   ( (true == chan_details->is_track_end(itrack))
          && (INC_DIRECTION == chan_details->get_track_direction(itrack)) ) 
       ||
       /* For DEC direction, an starting point requires an update on xlow and ylow  */
           ( (true == chan_details->is_track_start(itrack))
          && (DEC_DIRECTION == chan_details->get_track_direction(itrack)) ) ) {
      /* Get the node_id */
      size_t rr_node_id = chan_details->get_track_node_id(itrack);
      /* Do a quick check, make sure we do not mistakenly modify other nodes */
      assert(chan_type == rr_graph->rr_node[rr_node_id].type);
      assert(chan_details->get_track_direction(itrack) == rr_graph->rr_node[rr_node_id].direction);
      /* set xhigh/yhigh and push changes to track_ids */
      rr_graph->rr_node[rr_node_id].xhigh = chan_coordinator.get_x();
      rr_graph->rr_node[rr_node_id].yhigh = chan_coordinator.get_y();
      /* Do not update track_ids for length-1 wires, they should have only 1 track_id */
      if ( (rr_graph->rr_node[rr_node_id].xhigh > rr_graph->rr_node[rr_node_id].xlow)
        || (rr_graph->rr_node[rr_node_id].yhigh > rr_graph->rr_node[rr_node_id].ylow) ) {
        rr_graph->rr_node[rr_node_id].track_ids.push_back(itrack); 
      }
      /* Finish here, go to next */
    }

    /* Finish processing starting and ending tracks */
    if ( (true== chan_details->is_track_start(itrack))
      || (true == chan_details->is_track_end(itrack)) ) {
      /* Finish here, go to next */
      continue;
    }
    /* For other nodes, we get the node_id and just update track_ids */
    /* Ensure those nodes are neither starting nor ending points */
    assert( (false == chan_details->is_track_start(itrack))
         && (false == chan_details->is_track_end(itrack)) );
    /* Get the node_id */
    size_t rr_node_id = chan_details->get_track_node_id(itrack);
    /* Do a quick check, make sure we do not mistakenly modify other nodes */
    assert(chan_type == rr_graph->rr_node[rr_node_id].type);
    assert(chan_details->get_track_direction(itrack) == rr_graph->rr_node[rr_node_id].direction);
    /* Update track_ids */
    rr_graph->rr_node[rr_node_id].track_ids.push_back(itrack); 
    /* Finish here, go to next */
  }

  for (size_t itrack = 0; itrack < chan_details->get_chan_width(); ++itrack) {
    /* fill fast look-up table */
    /* Get node_id */
    int track_node_id = chan_details->get_track_node_id(itrack);
    /* CHANY requires a reverted (x,y) in the fast look-up table */
    if (CHANX == chan_type) {
      load_one_node_to_rr_graph_fast_lookup(rr_graph, track_node_id, 
                                            chan_type, 
                                            chan_coordinator.get_y(),
                                            chan_coordinator.get_x(), 
                                            itrack);
    }
    /* CHANX follows a regular (x,y) in the fast look-up table */
    if (CHANY == chan_type) {
      load_one_node_to_rr_graph_fast_lookup(rr_graph, track_node_id, 
                                            chan_type, 
                                            chan_coordinator.get_x(), 
                                            chan_coordinator.get_y(),
                                            itrack);
    }
  }

  return;
} 

/************************************************************************
 * Initialize the basic information of rr_nodes:
 * coordinators: xlow, ylow, xhigh, yhigh, 
 * features: capacity, track_ids, ptc_num, direction 
 * grid_info : pb_graph_pin
 ***********************************************************************/
static 
void load_rr_nodes_basic_info(t_rr_graph* rr_graph, 
                              const DeviceCoordinator& device_size,
                              const std::vector<std::vector<t_grid_tile>> grids,
                              const std::vector<size_t> chan_width,
                              const std::vector<t_segment_inf> segment_infs,
                              const int wire_to_ipin_switch, const int delayless_switch) {
  /* counter */
  size_t cur_node_id = 0;
  /* configure by node type */ 
  /* SOURCE, SINK, OPIN and IPIN */
  /************************************************************************
   * Search the grid and find the number OPINs and IPINs per grid
   * Note that the number of SOURCE nodes are the same as OPINs
   * and the number of SINK nodes are the same as IPINs
   ***********************************************************************/
  for (size_t ix = 0; ix < device_size.get_x(); ++ix) {
    for (size_t iy = 0; iy < device_size.get_y(); ++iy) { 
      /* Skip EMPTY tiles */
      if (EMPTY_TYPE == grids[ix][iy].type) {
        continue;
      }
      /* We only build rr_nodes for grids with offset=0 */
      if (0 < grids[ix][iy].offset) {
        continue;
      }
      DeviceCoordinator grid_coordinator(ix, iy);
      enum e_side io_side = NUM_SIDES;
      /* If this is the block on borders, we consider IO side */
      if (IO_TYPE == grid[ix][iy].type) {
        DeviceCoordinator io_device_size(device_size.get_x() - 1, device_size.get_y() - 1);
        io_side = determine_io_grid_pin_side(io_device_size, grid_coordinator);
      }
      /* Configure rr_nodes for this grid */
      load_one_grid_rr_nodes_basic_info(grid_coordinator, grid[ix][iy], io_side, 
                                        rr_graph, &cur_node_id, 
                                        wire_to_ipin_switch, delayless_switch);
    }
  }

  /* FIXME: DEBUG CODES TO BE REMOVED
  std::vector<size_t> node_cnt;
  node_cnt.resize(NUM_RR_TYPES);
  for (int inode = 0; inode < rr_graph->num_rr_nodes; ++inode) {
    node_cnt[rr_graph->rr_node[inode].type]++;
  }
  vpr_printf(TIO_MESSAGE_INFO, "Load basic information to %lu SOURCE NODE.\n", node_cnt[SOURCE]);
  vpr_printf(TIO_MESSAGE_INFO, "Load basic information to %lu SINK   NODE.\n", node_cnt[SINK]);
  vpr_printf(TIO_MESSAGE_INFO, "Load basic information to %lu OPIN   NODE.\n", node_cnt[OPIN]);
  vpr_printf(TIO_MESSAGE_INFO, "Load basic information to %lu IPIN   NODE.\n", node_cnt[IPIN]);
  */

  /* For X-direction Channel: CHANX */
  for (size_t iy = 0; iy < device_size.get_y() - 1; ++iy) { 
    /* Keep a vector of node_ids for the channels, because we will rotate them when walking through ix */
    std::vector<size_t> track_node_ids;
    /* Make sure a clean start */
    track_node_ids.clear();
    for (size_t ix = 1; ix < device_size.get_x() - 1; ++ix) {
      DeviceCoordinator chan_coordinator(ix, iy);
      enum e_side chan_side = NUM_SIDES;
      /* For LEFT side of FPGA */
      if (1 == ix) {
        chan_side = LEFT;
      }
      /* For RIGHT side of FPGA */
      if (device_size.get_x() - 2 == ix) {
        chan_side = RIGHT;
      }
      ChanNodeDetails chanx_details = build_unidir_chan_node_details(chan_width[0], device_size.get_x() - 2, chan_side, segment_infs); 
      /* Force node_ids from the previous chanx */
      if (0 < track_node_ids.size()) {
        /* Rotate should be done based on a typical case of routing tracks.
         * Tracks on the borders are not regularly started and ended, 
         * which causes the node_rotation malfunction  
         */
        ChanNodeDetails chanx_details_tt = build_unidir_chan_node_details(chan_width[0], device_size.get_x() - 2, NUM_SIDES, segment_infs); 
        chanx_details_tt.set_track_node_ids(track_node_ids);

        /* Rotate the chanx_details by an offset of ix - 1, the distance to the most left channel */
        /* For INC_DIRECTION, we use clockwise rotation 
         * node_id A ---->   -----> node_id D
         * node_id B ---->  / ----> node_id A
         * node_id C ----> /  ----> node_id B
         * node_id D ---->    ----> node_id C 
         */
        chanx_details_tt.rotate_track_node_id(1, INC_DIRECTION, true);
        /* For DEC_DIRECTION, we use clockwise rotation 
         * node_id A <-----    <----- node_id B
         * node_id B <----- \  <----- node_id C
         * node_id C <-----  \ <----- node_id D
         * node_id D <-----    <----- node_id A 
         */
        chanx_details_tt.rotate_track_node_id(1, DEC_DIRECTION, false);

        track_node_ids = chanx_details_tt.get_track_node_ids();
        chanx_details.set_track_node_ids(track_node_ids);
      }

      /* Configure CHANX in this channel */
      load_one_chan_rr_nodes_basic_info(chan_coordinator, CHANX, 
                                        &chanx_details, 
                                        segment_infs, 
                                        CHANX_COST_INDEX_START, 
                                        rr_graph, &cur_node_id);
      /* Get a copy of node_ids */
      track_node_ids = chanx_details.get_track_node_ids();
    }
  }

  /* For Y-direction Channel: CHANX */
  for (size_t ix = 0; ix < device_size.get_x() - 1; ++ix) {
    /* Keep a vector of node_ids for the channels, because we will rotate them when walking through ix */
    std::vector<size_t> track_node_ids;
    /* Make sure a clean start */
    track_node_ids.clear();
    for (size_t iy = 1; iy < device_size.get_y() - 1; ++iy) { 
      DeviceCoordinator chan_coordinator(ix, iy);
      enum e_side chan_side = NUM_SIDES;
      /* For BOTTOM side of FPGA */
      if (1 == iy) {
        chan_side = BOTTOM;
      }
      /* For RIGHT side of FPGA */
      if (device_size.get_y() - 2 == iy) {
        chan_side = TOP;
      }
      ChanNodeDetails chany_details = build_unidir_chan_node_details(chan_width[1], device_size.get_y() - 2, chan_side, segment_infs); 
      /* Force node_ids from the previous chanx */
      if (0 < track_node_ids.size()) {
        /* Rotate should be done based on a typical case of routing tracks.
         * Tracks on the borders are not regularly started and ended, 
         * which causes the node_rotation malfunction  
         */
        ChanNodeDetails chany_details_tt = build_unidir_chan_node_details(chan_width[1], device_size.get_y() - 2, NUM_SIDES, segment_infs); 

        chany_details_tt.set_track_node_ids(track_node_ids);
        /* Rotate the chany_details by an offset of 1*/
        /* For INC_DIRECTION, we use clockwise rotation 
         * node_id A ---->   -----> node_id D
         * node_id B ---->  / ----> node_id A
         * node_id C ----> /  ----> node_id B
         * node_id D ---->    ----> node_id C 
         */
        chany_details_tt.rotate_track_node_id(1, INC_DIRECTION, true);
        /* For DEC_DIRECTION, we use clockwise rotation 
         * node_id A <-----    <----- node_id B
         * node_id B <----- \  <----- node_id C
         * node_id C <-----  \ <----- node_id D
         * node_id D <-----    <----- node_id A 
         */
        chany_details_tt.rotate_track_node_id(1, DEC_DIRECTION, false);

        track_node_ids = chany_details_tt.get_track_node_ids();
        chany_details.set_track_node_ids(track_node_ids);
      }
      /* Configure CHANX in this channel */
      load_one_chan_rr_nodes_basic_info(chan_coordinator, CHANY, 
                                        &chany_details, 
                                        segment_infs, 
                                        CHANX_COST_INDEX_START + segment_infs.size(), 
                                        rr_graph, &cur_node_id);
      /* Get a copy of node_ids */
      track_node_ids = chany_details.get_track_node_ids();
    }
  }

  /* A quick check */
  assert ((int)cur_node_id == rr_graph->num_rr_nodes);
  for (int inode = 0; inode < rr_graph->num_rr_nodes; ++inode) {
    /* Check: we only support straight wires now.
     * CHANY: xlow=xhigh CHANY:ylow=yhigh  
     */
    if (CHANX == rr_graph->rr_node[inode].type) {
      assert (rr_graph->rr_node[inode].ylow == rr_graph->rr_node[inode].yhigh);
      assert (rr_graph->rr_node[inode].xlow <= rr_graph->rr_node[inode].xhigh);
    } else if (CHANY == rr_graph->rr_node[inode].type) {
      assert (rr_graph->rr_node[inode].xlow == rr_graph->rr_node[inode].xhigh);
      assert (rr_graph->rr_node[inode].ylow <= rr_graph->rr_node[inode].yhigh);
    } else {
      assert ( (SOURCE == rr_graph->rr_node[inode].type)
            || (SINK == rr_graph->rr_node[inode].type)
            || (OPIN == rr_graph->rr_node[inode].type)
            || (IPIN == rr_graph->rr_node[inode].type));
      assert (rr_graph->rr_node[inode].xlow == rr_graph->rr_node[inode].xhigh);
      assert (rr_graph->rr_node[inode].ylow == rr_graph->rr_node[inode].yhigh);
    }
  }

  /* Reverse the track_ids of CHANX and CHANY nodes in DEC_DIRECTION*/
  for (int inode = 0; inode < rr_graph->num_rr_nodes; ++inode) {
    /* Bypass condition: only focus on CHANX and CHANY in DEC_DIRECTION */
    if ( (CHANX != rr_graph->rr_node[inode].type)
      && (CHANY != rr_graph->rr_node[inode].type) ) {
      continue;
    }
    /* Reach here, we must have a node of CHANX or CHANY */
    if (DEC_DIRECTION != rr_graph->rr_node[inode].direction) {
      continue;
    }
    std::reverse(rr_graph->rr_node[inode].track_ids.begin(),
                 rr_graph->rr_node[inode].track_ids.end() );
  }

  return;
}

/************************************************************************
 * Build a fast look-up for the rr_nodes
 * it is a 4-dimension array to categorize rr_nodes in terms of 
 * types, coordinators and ptc_num (feature number)
 * The results will be stored in rr_node_indices[type][x][y]
 ***********************************************************************/
static 
void alloc_rr_graph_fast_lookup(const DeviceCoordinator& device_size,
                                t_rr_graph* rr_graph) {
  /* Allocates and loads all the structures needed for fast lookups of the   *
   * index of an rr_node.  rr_node_indices is a matrix containing the index  *
   * of the *first* rr_node at a given (i,j) location.                       */

  /* Alloc the lookup table */
  rr_graph->rr_node_indices = (t_ivec ***) my_malloc(sizeof(t_ivec **) * NUM_RR_TYPES);

  /* For OPINs, IPINs, SOURCE, SINKs, CHANX and CHANY */
  for (int type = 0; type < NUM_RR_TYPES; ++type) {
    /* Skip SOURCE and OPIN, they will share with SOURCE and SINK
   * SOURCE and SINK have unique ptc values so their data can be shared.
   * IPIN and OPIN have unique ptc values so their data can be shared. 
   */
    if ((SOURCE == type) || (OPIN == type) ) {
      continue;
    }
    rr_graph->rr_node_indices[type] = (t_ivec **) my_malloc(sizeof(t_ivec *) * device_size.get_x());
    for (size_t i = 0; i < device_size.get_x(); ++i) {
      rr_graph->rr_node_indices[type][i] = (t_ivec *) my_malloc(sizeof(t_ivec) * device_size.get_y());
      for (size_t j = 0; j < device_size.get_y(); ++j) {
        rr_graph->rr_node_indices[type][i][j].nelem = 0;
        rr_graph->rr_node_indices[type][i][j].list = NULL;
      }
    }
  }

  /* SOURCE and SINK have unique ptc values so their data can be shared.
   * IPIN and OPIN have unique ptc values so their data can be shared. */
  rr_graph->rr_node_indices[SOURCE] = rr_graph->rr_node_indices[SINK];
  rr_graph->rr_node_indices[OPIN] = rr_graph->rr_node_indices[IPIN];

  return;
}

/************************************************************************
 * Build the edges for all the SOURCE and SINKs nodes:
 * 1. create edges between SOURCE and OPINs
 ***********************************************************************/
static 
void build_rr_graph_edges_for_source_nodes(t_rr_graph* rr_graph,
                                           const std::vector< std::vector<t_grid_tile> > grids) {
  for (int inode = 0; inode < rr_graph->num_rr_nodes; ++inode) {
    /* Bypass all the non OPIN nodes */
    if (OPIN != rr_graph->rr_node[inode].type) {
      continue;
    }
    /* Now, we have an OPIN node, we get the source node index */
    int xlow = rr_graph->rr_node[inode].xlow; 
    int ylow = rr_graph->rr_node[inode].ylow; 
    int src_node_ptc_num = get_grid_pin_class_index(grids[xlow][ylow], 
                                                    rr_graph->rr_node[inode].ptc_num);
    /* 1. create edges between SOURCE and OPINs */
    int src_node_id = get_rr_node_index(xlow, ylow, 
                                        SOURCE, src_node_ptc_num,
                                        rr_graph->rr_node_indices);
    /* add edges to the src_node */
    add_one_edge_for_two_rr_nodes(rr_graph, src_node_id, inode,
                                  rr_graph->rr_node[inode].driver_switch);
  }
  return;
}

/************************************************************************
 * Build the edges for all the SINKs nodes:
 * 1. create edges between IPINs and SINKs
 ***********************************************************************/
static 
void build_rr_graph_edges_for_sink_nodes(t_rr_graph* rr_graph,
                                         const std::vector< std::vector<t_grid_tile> > grids) {
  for (int inode = 0; inode < rr_graph->num_rr_nodes; ++inode) {
    /* Bypass all the non IPIN nodes */
    if (IPIN != rr_graph->rr_node[inode].type) {
      continue;
    }
    /* Now, we have an OPIN node, we get the source node index */
    int xlow = rr_graph->rr_node[inode].xlow; 
    int ylow = rr_graph->rr_node[inode].ylow; 
    int sink_node_ptc_num = get_grid_pin_class_index(grids[xlow][ylow], 
                                                     rr_graph->rr_node[inode].ptc_num);
    /* 1. create edges between IPINs and SINKs */
    int sink_node_id = get_rr_node_index(xlow, ylow, 
                                         SINK, sink_node_ptc_num,
                                         rr_graph->rr_node_indices);
    /* add edges to connect the IPIN node to SINK nodes */
    add_one_edge_for_two_rr_nodes(rr_graph, inode, sink_node_id,
                                  rr_graph->rr_node[inode].driver_switch);
  }

  return;
}

/************************************************************************
 * Build the edges of each rr_node tile by tile:
 * We classify rr_nodes into a general switch block (GSB) data structure
 * where we create edges to each rr_nodes in the GSB with respect to
 * Fc_in and Fc_out, switch block patterns 
 * For each GSB: 
 * 1. create edges between CHANX | CHANY and IPINs (connections inside connection blocks)
 * 2. create edges between OPINs, CHANX and CHANY (connections inside switch blocks)
 * 3. create edges between OPINs and IPINs (direct-connections)
 ***********************************************************************/
static 
void build_rr_graph_edges(t_rr_graph* rr_graph, 
                          const DeviceCoordinator& device_size, 
                          const std::vector< std::vector<t_grid_tile> > grids,
                          const std::vector<size_t> device_chan_width, 
                          const std::vector<t_segment_inf> segment_inf,
                          int** Fc_in, int** Fc_out,
                          const enum e_switch_block_type sb_type, const int Fs) {

  /* Create edges for SOURCE and SINK nodes for a tileable rr_graph */
  build_rr_graph_edges_for_source_nodes(rr_graph, grids);
  build_rr_graph_edges_for_sink_nodes(rr_graph, grids);

  DeviceCoordinator gsb_range(device_size.get_x() - 2, device_size.get_y() - 2);

  /* Go Switch Block by Switch Block */
  for (size_t ix = 0; ix <= gsb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy <= gsb_range.get_y(); ++iy) { 
      //vpr_printf(TIO_MESSAGE_INFO, "Building edges for GSB[%lu][%lu]\n", ix, iy);

      DeviceCoordinator gsb_coordinator(ix, iy);
      /* Create a GSB object */
      RRGSB rr_gsb = build_one_tileable_rr_gsb(gsb_range, device_chan_width, segment_inf, gsb_coordinator, rr_graph);

      /* adapt the track_to_ipin_lookup for the GSB nodes */      
      t_track2pin_map track2ipin_map; /* [0..track_gsb_side][0..num_tracks][ipin_indices] */
      track2ipin_map = build_gsb_track_to_ipin_map(rr_graph, rr_gsb, grids, segment_inf, Fc_in);

      /* adapt the opin_to_track_map for the GSB nodes */      
      t_pin2track_map opin2track_map; /* [0..gsb_side][0..num_opin_node][track_indices] */
      opin2track_map = build_gsb_opin_to_track_map(rr_graph, rr_gsb, grids, segment_inf, Fc_out);

      /* adapt the switch_block_conn for the GSB nodes */      
      t_track2track_map sb_conn; /* [0..from_gsb_side][0..chan_width-1][track_indices] */
      sb_conn = build_gsb_track_to_track_map(rr_graph, rr_gsb, sb_type, Fs, segment_inf);

      /* Build edges for a GSB */
      build_edges_for_one_tileable_rr_gsb(rr_graph, &rr_gsb,
                                          track2ipin_map, opin2track_map, 
                                          sb_conn);
      /* Finish this GSB, go to the next*/
    }
  }

  return;
}

/************************************************************************
 * Build direct edges for Grids *
 ***********************************************************************/
static 
void build_rr_graph_direct_connections(t_rr_graph* rr_graph, 
                                       const DeviceCoordinator& device_size,
                                       const std::vector< std::vector<t_grid_tile> > grids, 
                                       const int delayless_switch, 
                                       const int num_directs, 
                                       const t_direct_inf *directs, 
                                       const t_clb_to_clb_directs *clb_to_clb_directs) {
  for (size_t ix = 0; ix < device_size.get_x(); ++ix) {
    for (size_t iy = 0; iy < device_size.get_y(); ++iy) { 
      /* Skip EMPTY tiles */
      if (EMPTY_TYPE == grids[ix][iy].type) {
        continue;
      }
      /* Skip height>1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[ix][iy].offset) {
        continue;
      }
      DeviceCoordinator from_grid_coordinator(ix, iy);
      build_direct_connections_for_one_gsb(rr_graph, device_size, grids,
                                           from_grid_coordinator,
                                           grids[ix][iy],
                                           delayless_switch, 
                                           num_directs, directs, clb_to_clb_directs);
    }
  }

  return;
}

/************************************************************************ 
 * Reset driver switch of a rr_graph  
 ***********************************************************************/
static 
void clear_rr_graph_driver_switch(const t_rr_graph* rr_graph) {
  for (int inode = 0; inode < rr_graph->num_rr_nodes; ++inode) {
    rr_graph->rr_node[inode].driver_switch = 0;
  }
  return;
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
void build_tileable_unidir_rr_graph(INP const int L_num_types,
                                    INP t_type_ptr types, INP const int L_nx, INP const int L_ny,
                                    INP struct s_grid_tile **L_grid, INP const int chan_width,
                                    INP const enum e_switch_block_type sb_type, INP const int Fs, 
                                    INP const int num_seg_types,
                                    INP const t_segment_inf * segment_inf,
                                    INP const int num_switches, INP const int delayless_switch, 
                                    INP const t_timing_inf timing_inf, 
                                    INP const int wire_to_ipin_switch,
                                    INP const enum e_base_cost_type base_cost_type, 
                                    INP const t_direct_inf *directs, 
                                    INP const int num_directs, INP const boolean ignore_Fc_0, 
                                    OUTP int *Warnings) { 
  /* Create an empty graph */
  t_rr_graph rr_graph; 
  rr_graph.rr_node_indices = NULL;
  rr_graph.rr_node = NULL;
  rr_graph.num_rr_nodes = 0;

  /* Reset warning flag */
  *Warnings = RR_GRAPH_NO_WARN;

  /* Print useful information on screen */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Creating tileable Routing Resource(RR) graph...\n");

  /* Create a matrix of grid */
  DeviceCoordinator device_size(L_nx + 2, L_ny + 2);
  std::vector< std::vector<t_grid_tile> > grids;
  /* reserve vector capacity to be memory efficient */
  grids.resize(L_nx + 2);
  for (int ix = 0; ix < (L_nx + 2); ++ix) {
    grids[ix].resize(L_ny + 2);
    for (int iy = 0; iy < (L_ny + 2); ++iy) {
      grids[ix][iy] = L_grid[ix][iy];
    }
  }
  /* Create a vector of channel width, we support X-direction and Y-direction has different W */
  std::vector<size_t> device_chan_width;
  device_chan_width.push_back(chan_width);
  device_chan_width.push_back(chan_width);

  /* Create a vector of segment_inf */
  std::vector<t_segment_inf> segment_infs;
  for (int iseg = 0; iseg < num_seg_types; ++iseg) {
    segment_infs.push_back(segment_inf[iseg]);
  }

  /************************************************************************
   * 2. Estimate the number of nodes in the rr_graph
   *    This will estimate the number of 
   *    a. IPINs, input pins of each grid
   *    b. OPINs, output pins of each grid
   *    c. SOURCE, virtual node which drives OPINs
   *    d. SINK, virtual node which is connected to IPINs
   *    e. CHANX and CHANY, routing segments of each channel
   ***********************************************************************/
  std::vector<size_t> num_rr_nodes_per_type = estimate_num_rr_nodes_per_type(device_size, grids, device_chan_width, segment_infs); 

  /************************************************************************
   * 3. Allocate the rr_nodes 
   ***********************************************************************/
  rr_graph.num_rr_nodes = 0;
  for (size_t i = 0; i < num_rr_nodes_per_type.size(); ++i) {
    rr_graph.num_rr_nodes += num_rr_nodes_per_type[i];
  }
  /* use calloc and memset to initialize everything to be zero */
  rr_graph.rr_node = (t_rr_node*)my_calloc(rr_graph.num_rr_nodes, sizeof(t_rr_node));
  for (int i = 0; i < rr_graph.num_rr_nodes; ++i) {
    tileable_rr_graph_init_rr_node(&(rr_graph.rr_node[i]));
  }

  /************************************************************************
   * 4. Initialize the basic information of rr_nodes:
   *    coordinators: xlow, ylow, xhigh, yhigh, 
   *    features: capacity, track_ids, ptc_num, direction 
   *    grid_info : pb_graph_pin
   ***********************************************************************/
  alloc_rr_graph_fast_lookup(device_size, &rr_graph);

  load_rr_nodes_basic_info(&rr_graph, device_size, grids, device_chan_width, segment_infs,
                           wire_to_ipin_switch, delayless_switch); 

  /************************************************************************
   * 5.1 Create the connectivity of OPINs
   *     a. Evenly assign connections to OPINs to routing tracks
   *     b. the connection pattern should be same across the fabric
   *
   * 5.2 Create the connectivity of IPINs 
   *     a. Evenly assign connections from routing tracks to IPINs
   *     b. the connection pattern should be same across the fabric
   ***********************************************************************/
  int **Fc_in = NULL; /* [0..num_types-1][0..num_pins-1] */
  boolean Fc_clipped;
  Fc_clipped = FALSE;
  Fc_in = alloc_and_load_actual_fc(L_num_types, types, chan_width,
                                   FALSE, UNI_DIRECTIONAL, &Fc_clipped, ignore_Fc_0);
  if (Fc_clipped) {
    *Warnings |= RR_GRAPH_WARN_FC_CLIPPED;
  }

  int **Fc_out = NULL; /* [0..num_types-1][0..num_pins-1] */
  Fc_clipped = FALSE;
  Fc_out = alloc_and_load_actual_fc(L_num_types, types, chan_width,
                                   TRUE, UNI_DIRECTIONAL, &Fc_clipped, ignore_Fc_0);

  if (Fc_clipped) {
    *Warnings |= RR_GRAPH_WARN_FC_CLIPPED;
  }

  /************************************************************************
   * 6.1 Build the connections tile by tile:
   *     We classify rr_nodes into a general switch block (GSB) data structure
   *     where we create edges to each rr_nodes in the GSB with respect to
   *     Fc_in and Fc_out, switch block patterns 
   *     In addition, we will also handle direct-connections:
   *     Add edges that bridge OPINs and IPINs to the rr_graph
   ***********************************************************************/
  /* Create edges for a tileable rr_graph */
  build_rr_graph_edges(&rr_graph, device_size, grids, device_chan_width, segment_infs, 
                       Fc_in, Fc_out,
                       sb_type, Fs);

  /************************************************************************
   * 6.2 Build direction connection lists
   ***********************************************************************/
  /* Create data structure of direct-connections */
  t_clb_to_clb_directs* clb_to_clb_directs = NULL;
  if (num_directs > 0) {
    clb_to_clb_directs = alloc_and_load_clb_to_clb_directs(directs, num_directs);
  }
  build_rr_graph_direct_connections(&rr_graph, device_size, grids, delayless_switch, 
                                    num_directs, directs, clb_to_clb_directs);

  print_rr_graph_stats(rr_graph);

  /* Clear driver switches of the rr_graph */
  clear_rr_graph_driver_switch(&rr_graph);

  /************************************************************************
   * 7. Allocate external data structures
   *    a. cost_index
   *    b. RC tree
   ***********************************************************************/
  /* We set global variables for rr_nodes here, they will be updated by rr_graph_external */
  num_rr_nodes = rr_graph.num_rr_nodes;
  rr_node = rr_graph.rr_node;
  rr_node_indices = rr_graph.rr_node_indices;  

  rr_graph_externals(timing_inf, segment_inf, num_seg_types, chan_width,
                     wire_to_ipin_switch, base_cost_type);

  /************************************************************************
   * 8. Sanitizer for the rr_graph, check connectivities of rr_nodes
   ***********************************************************************/

  /* Print useful information on screen */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Create a tileable RR graph with %d nodes\n", 
             num_rr_nodes);

  check_rr_graph(GRAPH_UNIDIR_TILEABLE, L_nx, L_ny,
                 num_switches, Fc_in);

  /* Print useful information on screen */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Tileable Routing Resource(RR) graph pass checking.\n");


  /************************************************************************
   * 9. Free all temp stucts 
   ***********************************************************************/

  /* Free all temp structs */
  if (Fc_in) {
    free_matrix(Fc_in,0, L_num_types, 0, sizeof(int));
    Fc_in = NULL;
  }
  if (Fc_out) {
    free_matrix(Fc_out,0, L_num_types, 0, sizeof(int));
    Fc_out = NULL;
  }
  if(clb_to_clb_directs != NULL) {
    free(clb_to_clb_directs);
  }

  return;
}

/************************************************************************
 * End of file : rr_graph_tileable_builder.c 
 ***********************************************************************/
