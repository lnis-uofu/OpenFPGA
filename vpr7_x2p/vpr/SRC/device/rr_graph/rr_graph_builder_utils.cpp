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
 * Filename:    rr_graph_builder_utils.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/23  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file contains most utilized functions for rr_graph builders 
 ***********************************************************************/

#include <cstdlib>
#include <cassert>

#include <vector>
#include <algorithm>

#include "rr_graph_builder_utils.h"
#include "globals.h"

/************************************************************************
 * Initialize a rr_node 
 ************************************************************************/
void tileable_rr_graph_init_rr_node(t_rr_node* cur_rr_node) {
  cur_rr_node->xlow = 0;
  cur_rr_node->xhigh = 0;
  cur_rr_node->ylow = 0;
  cur_rr_node->xhigh = 0;

  cur_rr_node->ptc_num = 0; 
  cur_rr_node->track_ids.clear();

  cur_rr_node->cost_index = 0; 
  cur_rr_node->occ = 0; 
  cur_rr_node->fan_in = 0; 
  cur_rr_node->num_edges = 0; 
  cur_rr_node->type = NUM_RR_TYPES; 
  cur_rr_node->edges = NULL; 
  cur_rr_node->switches = NULL; 

  cur_rr_node->driver_switch = 0; 
  cur_rr_node->unbuf_switched = 0; 
  cur_rr_node->buffered = 0; 
  cur_rr_node->R = 0.; 
  cur_rr_node->C = 0.; 

  cur_rr_node->direction = BI_DIRECTION; /* Give an invalid value, easy to check errors */ 
  cur_rr_node->drivers = SINGLE; 
  cur_rr_node->num_wire_drivers = 0; 
  cur_rr_node->num_opin_drivers = 0; 

  cur_rr_node->num_drive_rr_nodes = 0; 
  cur_rr_node->drive_rr_nodes = NULL; 
  cur_rr_node->drive_switches = NULL; 

  cur_rr_node->vpack_net_num_changed = FALSE; 
  cur_rr_node->is_parasitic_net = FALSE; 
  cur_rr_node->is_in_heap = FALSE; 

  cur_rr_node->sb_num_drive_rr_nodes = 0; 
  cur_rr_node->sb_drive_rr_nodes = NULL; 
  cur_rr_node->sb_drive_switches = NULL; 

  cur_rr_node->pb = NULL; 

  cur_rr_node->name_mux = NULL; 
  cur_rr_node->id_path = -1; 

  cur_rr_node->prev_node = -1; 
  cur_rr_node->prev_edge = -1; 
  cur_rr_node->net_num = -1; 
  cur_rr_node->vpack_net_num = -1; 

  cur_rr_node->prev_node_in_pack = -1; 
  cur_rr_node->prev_edge_in_pack = -1; 
  cur_rr_node->net_num_in_pack = -1; 

  cur_rr_node->pb_graph_pin = NULL; 
  cur_rr_node->tnode = NULL; 
  
  cur_rr_node->pack_intrinsic_cost = 0.; 
  cur_rr_node->z = 0; 

  return;
}


/************************************************************************
 * Get the class index of a grid pin 
 ***********************************************************************/
int get_grid_pin_class_index(const t_grid_tile& cur_grid,
                             const int pin_index) {
  /* check */
  assert ( pin_index < cur_grid.type->num_pins);
  return cur_grid.type->pin_class[pin_index];
}

/* Deteremine the side of a io grid */
enum e_side determine_io_grid_pin_side(const DeviceCoordinator& device_size, 
                                       const DeviceCoordinator& grid_coordinator) {
  /* TOP side IO of FPGA */
  if (device_size.get_y() == grid_coordinator.get_y()) {
    return BOTTOM; /* Such I/O has only Bottom side pins */
  } else if (device_size.get_x() == grid_coordinator.get_x()) { /* RIGHT side IO of FPGA */
    return LEFT; /* Such I/O has only Left side pins */
  } else if (0 == grid_coordinator.get_y()) { /* BOTTOM side IO of FPGA */
    return TOP; /* Such I/O has only Top side pins */
  } else if (0 == grid_coordinator.get_x()) { /* LEFT side IO of FPGA */
    return RIGHT; /* Such I/O has only Right side pins */
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) I/O Grid is in the center part of FPGA! Currently unsupported!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/************************************************************************
 * Get a list of pin_index for a grid (either OPIN or IPIN)
 * For IO_TYPE, only one side will be used, we consider one side of pins 
 * For others, we consider all the sides  
 ***********************************************************************/
std::vector<int> get_grid_side_pins(const t_grid_tile& cur_grid, 
                                    const enum e_pin_type pin_type, 
                                    const enum e_side pin_side, 
                                    const int pin_height) {
  std::vector<int> pin_list; 
  /* Make sure a clear start */
  pin_list.clear();

  for (int ipin = 0; ipin < cur_grid.type->num_pins; ++ipin) {
    int class_id = cur_grid.type->pin_class[ipin];
    if ( (1 == cur_grid.type->pinloc[pin_height][pin_side][ipin]) 
      && (pin_type == cur_grid.type->class_inf[class_id].type) ) {
      pin_list.push_back(ipin);
    }
  }
  return pin_list;
}

/************************************************************************
 * Get the number of pins for a grid (either OPIN or IPIN)
 * For IO_TYPE, only one side will be used, we consider one side of pins 
 * For others, we consider all the sides  
 ***********************************************************************/
size_t get_grid_num_pins(const t_grid_tile& cur_grid, 
                         const enum e_pin_type pin_type, 
                         const enum e_side io_side) {
  size_t num_pins = 0;
  Side io_side_manager(io_side);

  /* For IO_TYPE sides */
  for (size_t side = 0; side < NUM_SIDES; ++side) {
    Side side_manager(side);
    /* skip unwanted sides */
    if ( (IO_TYPE == cur_grid.type)
      && (side != io_side_manager.to_size_t()) ) { 
      continue;
    }
    /* Get pin list */
    for (int height = 0; height < cur_grid.type->height; ++height) {
      std::vector<int> pin_list = get_grid_side_pins(cur_grid, pin_type, side_manager.get_side(), height);
      num_pins += pin_list.size();
    } 
  }

  return num_pins;
}

/************************************************************************
 * Get the number of pins for a grid (either OPIN or IPIN)
 * For IO_TYPE, only one side will be used, we consider one side of pins 
 * For others, we consider all the sides  
 ***********************************************************************/
size_t get_grid_num_classes(const t_grid_tile& cur_grid, 
                            const enum e_pin_type pin_type) {
  size_t num_classes = 0;

  for (int iclass = 0; iclass < cur_grid.type->num_class; ++iclass) {
    /* Bypass unmatched pin_type */
    if (pin_type != cur_grid.type->class_inf[iclass].type) {
      continue;
    }
    num_classes++;
  }

  return num_classes;
}



/************************************************************************
 * Add a edge connecting two rr_nodes
 * For src rr_node, update the edge list and update switch_id,
 * For des rr_node, update the fan_in
 ***********************************************************************/
void add_one_edge_for_two_rr_nodes(const t_rr_graph* rr_graph,
                                   const int src_rr_node_id, 
                                   const int des_rr_node_id,
                                   const short switch_id) {
  /* Check */
  assert ( (-1 < src_rr_node_id) && (src_rr_node_id < rr_graph->num_rr_nodes) );
  assert ( (-1 < des_rr_node_id) && (des_rr_node_id < rr_graph->num_rr_nodes) );
  
  t_rr_node* src_rr_node = &(rr_graph->rr_node[src_rr_node_id]);
  t_rr_node* des_rr_node = &(rr_graph->rr_node[des_rr_node_id]);

  /* Allocate edge and switch to src_rr_node */
  src_rr_node->num_edges++;
  if (NULL == src_rr_node->edges) {
    /* calloc */
    src_rr_node->edges = (int*) my_calloc( src_rr_node->num_edges, sizeof(int) ); 
    src_rr_node->switches = (short*) my_calloc( src_rr_node->num_edges, sizeof(short) ); 
  } else {
    /* realloc */
    src_rr_node->edges = (int*) my_realloc(src_rr_node->edges, 
                                           src_rr_node->num_edges * sizeof(int)); 
    src_rr_node->switches = (short*) my_realloc(src_rr_node->switches, 
                                                src_rr_node->num_edges * sizeof(short)); 
  }
  /* Fill edge and switch info */
  src_rr_node->edges[src_rr_node->num_edges - 1] = des_rr_node_id;
  src_rr_node->switches[src_rr_node->num_edges - 1] = switch_id;

  /* Update the des_rr_node */ 
  des_rr_node->fan_in++;

  return;
}


/************************************************************************
 * Add a set of edges for a source rr_node 
 * For src rr_node, update the edge list and update switch_id,
 * For des rr_node, update the fan_in
 ***********************************************************************/
void add_edges_for_two_rr_nodes(const t_rr_graph* rr_graph,  
                                const int src_rr_node_id, 
                                const std::vector<int> des_rr_node_ids, 
                                const std::vector<short> driver_switches) {
  /* Check src_rr_node id is in range */
  assert ( (-1 < src_rr_node_id) && (src_rr_node_id < rr_graph->num_rr_nodes) );

  t_rr_node* src_rr_node = &(rr_graph->rr_node[src_rr_node_id]);

  /* Check des_rr_node and driver_switches should match in size  */
  assert ( des_rr_node_ids.size() == driver_switches.size() );

  /* Get a stamp of the current num_edges of src_rr_node */
  int start_edge_id = src_rr_node->num_edges;

  /* To avoid adding redundant edges, 
   * we will search the edge list and 
   * check if each des_rr_node_id already exists 
   * We rebuild a vector des_rr_node_ids_to_add where redundancy is removed 
   */
  std::vector<int> des_rr_node_ids_to_add; 
  std::vector<short> driver_switches_to_add; 
  for (size_t inode = 0; inode < des_rr_node_ids.size(); ++inode) {
    /* search */
    bool is_redundant = false;
    for (int iedge = 0; iedge < src_rr_node->num_edges; ++iedge) {
      if (des_rr_node_ids[inode] == src_rr_node->edges[iedge]) {
        is_redundant = true;
        break;
      } 
    } 
    /* add or skip */
    if (true == is_redundant) {
      continue; /* go to the next  */
    }
    assert (false == is_redundant);
    /* add to the list */
    des_rr_node_ids_to_add.push_back(des_rr_node_ids[inode]);
    driver_switches_to_add.push_back(driver_switches[inode]);
  }

  /* Allocate edge and switch to src_rr_node */
  src_rr_node->num_edges += des_rr_node_ids_to_add.size();
  if (NULL == src_rr_node->edges) {
    /* calloc */
    src_rr_node->edges = (int*) my_calloc( src_rr_node->num_edges, sizeof(int) ); 
    src_rr_node->switches = (short*) my_calloc( src_rr_node->num_edges, sizeof(short) ); 
  } else {
    /* realloc */
    src_rr_node->edges = (int*) my_realloc(src_rr_node->edges, 
                                           src_rr_node->num_edges * sizeof(int)); 
    src_rr_node->switches = (short*) my_realloc(src_rr_node->switches, 
                                                src_rr_node->num_edges * sizeof(short)); 
  }

  for (size_t inode = 0; inode < des_rr_node_ids_to_add.size(); ++inode) {
    /* Check des_rr_node id is in range */
    int des_rr_node_id = des_rr_node_ids_to_add[inode];
    assert ( (-1 < des_rr_node_id) && (des_rr_node_id < rr_graph->num_rr_nodes) );
    
    t_rr_node* des_rr_node = &(rr_graph->rr_node[des_rr_node_id]);
  
    /* Fill edge and switch info */
    src_rr_node->edges[start_edge_id] = des_rr_node_id;
    src_rr_node->switches[start_edge_id] = driver_switches_to_add[inode];
  
    /* Update the des_rr_node */ 
    des_rr_node->fan_in++;
    /* Increment the start_edge_id */
    start_edge_id++;
  }

  /* Check */
  assert( start_edge_id == src_rr_node->num_edges );

  return;

}

/************************************************************************
 * Get the coordinator of a starting point of a routing track 
 * For routing tracks in INC_DIRECTION
 * (xlow, ylow) should be the starting point 
 *
 * For routing tracks in DEC_DIRECTION
 * (xhigh, yhigh) should be the starting point 
 ***********************************************************************/
DeviceCoordinator get_track_rr_node_start_coordinator(const t_rr_node* track_rr_node) {
  /* Make sure we have CHANX or CHANY */
  assert ( (CHANX == track_rr_node->type) ||(CHANY == track_rr_node->type) );
 
  DeviceCoordinator start_coordinator;

  if (INC_DIRECTION == track_rr_node->direction) {
    start_coordinator.set(track_rr_node->xlow, track_rr_node->ylow);
  } else {
    assert (DEC_DIRECTION == track_rr_node->direction);
    start_coordinator.set(track_rr_node->xhigh, track_rr_node->yhigh);
  }

  return start_coordinator;
}

/************************************************************************
 * Get the coordinator of a end point of a routing track 
 * For routing tracks in INC_DIRECTION
 * (xhigh, yhigh) should be the starting point 
 *
 * For routing tracks in DEC_DIRECTION
 * (xlow, ylow) should be the starting point 
 ***********************************************************************/
DeviceCoordinator get_track_rr_node_end_coordinator(const t_rr_node* track_rr_node) {
  /* Make sure we have CHANX or CHANY */
  assert ( (CHANX == track_rr_node->type) ||(CHANY == track_rr_node->type) );
 
  DeviceCoordinator end_coordinator;

  if (INC_DIRECTION == track_rr_node->direction) {
    end_coordinator.set(track_rr_node->xhigh, track_rr_node->yhigh);
  } else {
    assert (DEC_DIRECTION == track_rr_node->direction);
    end_coordinator.set(track_rr_node->xlow, track_rr_node->ylow);
  }

  return end_coordinator;
}

/************************************************************************
 * Get the ptc of a routing track in the channel where it ends
 * For routing tracks in INC_DIRECTION
 * the ptc is the last of track_ids
 *
 * For routing tracks in DEC_DIRECTION
 * the ptc is the first of track_ids
 ***********************************************************************/
short get_track_rr_node_end_track_id(const t_rr_node* track_rr_node) {
  /* Make sure we have CHANX or CHANY */
  assert ( (CHANX == track_rr_node->type) ||(CHANY == track_rr_node->type) );
 
  if (INC_DIRECTION == track_rr_node->direction) {
    return track_rr_node->track_ids.back(); 
  }
  assert (DEC_DIRECTION == track_rr_node->direction);
  return track_rr_node->track_ids.front(); 
}

