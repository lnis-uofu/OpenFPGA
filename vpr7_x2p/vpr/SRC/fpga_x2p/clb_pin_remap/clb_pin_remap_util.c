#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"
#include "heapsort.h"
#include "net_delay.h"
#include "path_delay.h"

/* CLB PIN REMAP */
#include "clb_pin_remap_util.h"

int pin_side_count(int pin_side[]) {
  int cnt = 0;
  int side = 0;
  
  for (side = 0; side < 4; side++) {
    cnt += pin_side[side];
  }
    
  return cnt;
}

/* Malloc pin_side[4]
 * If the pin appear at this side, set pin_side[side] to 1
 */
void find_blk_net_pin_sides(t_block target_blk,
                            int pin_height,
                            int pin_index,
                            int** pin_side) {
  int side;

  /* Check */
  assert(NULL != target_blk.type);
  assert((!(0 > pin_height))&&(pin_height < target_blk.type->height));
  /* Allocate */
  (*pin_side) = NULL;
  (*pin_side) = (int*)my_malloc(sizeof(int)*4);

  for (side = 0; side < 4; side++) {
    /* Initialize */
    (*pin_side)[side] = 0;
    /* Special care for IO_TYPE */
    if (IO_TYPE == target_blk.type) {
      if ((0 == target_blk.x)&&(RIGHT == side)) {
        /* LEFT side IO only has RIGHT side ports */
        (*pin_side)[side] = target_blk.type->pinloc[pin_height][side][pin_index];
      } else if (((ny+1) == target_blk.y)&&(BOTTOM == side)) {
        /* TOP side IO only has BOTTOM side ports */
        (*pin_side)[side] = target_blk.type->pinloc[pin_height][side][pin_index];
      } else if (((nx+1) == target_blk.x)&&(LEFT == side)) {
        /* RIGHT side IO only has LEFT side ports */
        (*pin_side)[side] = target_blk.type->pinloc[pin_height][side][pin_index];
      } else if ((0 == target_blk.y)&&(TOP == side)) {
        /* BOTTOM side IO only has TOP side ports */
        (*pin_side)[side] = target_blk.type->pinloc[pin_height][side][pin_index];
      } 
    } else {
      (*pin_side)[side] = target_blk.type->pinloc[pin_height][side][pin_index];
    }
  }

  return; 
}

/* Find the pin side of one block net, 
 * Here the pin is allowed to appear at only one side 
 */
int find_blk_net_pin_side(t_block target_blk,
                          int pin_height,
                          int pin_index) {
  int* pin_sides = NULL;
  int i = -1;
  
  find_blk_net_pin_sides(target_blk, pin_height, 
                         pin_index, &pin_sides);

  assert(1 == pin_side_count(pin_sides));

  for (i = 0; i < 4; i++) {
    if (1 == pin_sides[i]) {
      return i;
    }
  }

  return -1;
}

void find_blk_net_type_pins(int n_blks, t_block* blk,
                            int net_index,
                            int blk_index,
                            int* num_pins,
                            enum e_side** pin_side,
                            int** pin_index) {
  t_type_ptr blk_type = NULL;
  int jpin, iside = 0;
  int net_pin_cnt = 0;

  /* Initialize */
  (*num_pins) = 0;

  /* Check index range */
  assert(blk_index < n_blks);
  /* Find the type_descriptor*/
  blk_type = blk[blk_index].type;
  assert(NULL != blk_type); 
  /* Find the port */
  for (jpin = 0; jpin < blk_type->num_pins; jpin++) {
    if (net_index == blk[blk_index].nets[jpin]) {
      /* Special for IO_TYPE */
      if (IO_TYPE == blk_type) {
        /* LEFT side IO only has RIGHT side ports */
        if (0 == blk[blk_index].x) {
          assert((0 < blk[blk_index].y)&&(blk[blk_index].y < (ny+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][RIGHT][jpin]);
          //(*pin_side) = RIGHT;
          (*num_pins)++;
        /* TOP side IO only has BOTTOM side ports */
        } else if ((ny+1) == blk[blk_index].y) {
          assert((0 < blk[blk_index].x)&&(blk[blk_index].x < (nx+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][BOTTOM][jpin]);
          //(*pin_side) = BOTTOM;
          (*num_pins)++;
        /* RIGHT side IO only has LEFT side ports */
        } else if ((nx+1) == blk[blk_index].x) {
          assert((0 < blk[blk_index].y)&&(blk[blk_index].y < (ny+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][LEFT][jpin]);
          //(*pin_side) = LEFT;
          (*num_pins)++;
        /* BOTTOM side IO only has TOP side ports */
        } else if (0 == blk[blk_index].y) {
          assert((0 < blk[blk_index].x)&&(blk[blk_index].x < (nx+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][TOP][jpin]);
          //(*pin_side) = TOP;
          (*num_pins)++;
        }
      } else {
        for (iside = 0; iside < 4; iside++) {
          if (1 == blk_type->pinloc[blk[blk_index].z][iside][jpin]) {
            (*num_pins)++;
          }
        }
      }
    }
  }

  /* Allocate */
  if (0 == (*num_pins)) {
    return;
  }
  (*pin_side) = (enum e_side*)my_malloc((*num_pins)*sizeof(enum e_side));
  (*pin_index) = (int*)my_malloc((*num_pins)*sizeof(int));

  /* Fill the array */
  for (jpin = 0; jpin < blk_type->num_pins; jpin++) {
    if (net_index == blk[blk_index].nets[jpin]) {
      /* Special for IO_TYPE */
      if (IO_TYPE == blk_type) {
        /* LEFT side IO only has RIGHT side ports */
        if (0 == blk[blk_index].x) {
          assert((0 < blk[blk_index].y)&&(blk[blk_index].y < (ny+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][RIGHT][jpin]);
          (*pin_side)[net_pin_cnt] = RIGHT;
          (*pin_index)[net_pin_cnt] = jpin;
          net_pin_cnt++;
        /* TOP side IO only has BOTTOM side ports */
        } else if ((ny+1) == blk[blk_index].y) {
          assert((0 < blk[blk_index].x)&&(blk[blk_index].x < (nx+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][BOTTOM][jpin]);
          (*pin_side)[net_pin_cnt] = BOTTOM;
          (*pin_index)[net_pin_cnt] = jpin;
          net_pin_cnt++;
        /* RIGHT side IO only has LEFT side ports */
        } else if ((nx+1) == blk[blk_index].x) {
          assert((0 < blk[blk_index].y)&&(blk[blk_index].y < (ny+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][LEFT][jpin]);
          (*pin_side)[net_pin_cnt] = LEFT;
          (*pin_index)[net_pin_cnt] = jpin;
          net_pin_cnt++;
        /* BOTTOM side IO only has TOP side ports */
        } else if (0 == blk[blk_index].y) {
          assert((0 < blk[blk_index].x)&&(blk[blk_index].x < (nx+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][TOP][jpin]);
          (*pin_side)[net_pin_cnt] = TOP;
          (*pin_index)[net_pin_cnt] = jpin;
          net_pin_cnt++;
        }
      } else {
        for (iside = 0; iside < 4; iside++) {
          if (1 == blk_type->pinloc[blk[blk_index].z][iside][jpin]) {
            (*pin_side)[net_pin_cnt] = (enum e_side)iside; 
            (*pin_index)[net_pin_cnt] = jpin;
            net_pin_cnt++;
          }
        }
      }
    }
  }
  
}

int check_src_blk_pin(int n_blks, t_block* blk,
                      int net_index,
                      int src_blk_index,
                      //int src_blk_port_index,
                      int src_blk_pin_index) {
  int ret = 1;
  t_type_ptr blk_type = NULL;
  int jpin, net_pin_class;
  int net_pin_cnt = 0;

  /* Check index range */
  assert(src_blk_index < n_blks);
  /* Find the type_descriptor*/
  blk_type = blk[src_blk_index].type;
  assert(NULL != blk_type); 
  /* Find the port */
  for (jpin = 0; jpin < blk_type->num_pins; jpin++) {
    if (net_index == blk[src_blk_index].nets[jpin]) {
      net_pin_class = blk_type->pin_class[jpin];
      assert(DRIVER == blk_type->class_inf[net_pin_class].type);
      /* Special for IO_TYPE */
      if (IO_TYPE == blk_type) {
        /* LEFT side IO only has RIGHT side ports */
        if (0 == blk[src_blk_index].x) {
          assert((0 < blk[src_blk_index].y)&&(blk[src_blk_index].y < (ny+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][RIGHT][jpin]);
          net_pin_cnt++;
        /* TOP side IO only has BOTTOM side ports */
        } else if ((ny+1) == blk[src_blk_index].y) {
          assert((0 < blk[src_blk_index].x)&&(blk[src_blk_index].x < (nx+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][BOTTOM][jpin]);
          net_pin_cnt++;
        /* RIGHT side IO only has LEFT side ports */
        } else if ((nx+1) == blk[src_blk_index].x) {
          assert((0 < blk[src_blk_index].y)&&(blk[src_blk_index].y < (ny+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][LEFT][jpin]);
          net_pin_cnt++;
        /* BOTTOM side IO only has TOP side ports */
        } else if (0 == blk[src_blk_index].y) {
          assert((0 < blk[src_blk_index].x)&&(blk[src_blk_index].x < (nx+1)));
          assert(1 == blk_type->pinloc[blk_type->pin_height[jpin]][TOP][jpin]);
          net_pin_cnt++;
        }
      } else {
        net_pin_cnt++;
      }
    }
  }
  assert(1 == net_pin_cnt); /* TODO: May conflict with I/O TYPE*/
  /* The port should be an output port */
  //assert(src_blk_port_index < blk[src_blk_index].pb->pb_graph_node->pb_type->num_ports);
  //assert(OUT_PORT == blk[src_blk_index].pb->pb_graph_node->pb_type->ports[src_blk_port_index].type);

  return ret;
}
                       
int check_des_blk_pin(int n_blks, t_block* blk,
                      int net_index,
                      int des_blk_index,
                      //int des_blk_port_index,
                      int des_blk_pin_index) {
  int ret = 1;
  t_type_ptr blk_type = NULL;
  int jpin, net_pin_class;
  int net_pin_cnt = 0;

  /* Check index range */
  assert(des_blk_index < n_blks);
  /* Find the type_descriptor*/
  blk_type = blk[des_blk_index].type;
  assert(NULL != blk_type); 
  /* Find the port */
  for (jpin = 0; jpin < blk_type->num_pins; jpin++) {
    if (net_index == blk[des_blk_index].nets[jpin]) {
      net_pin_class = blk_type->pin_class[jpin];
      assert(RECEIVER == blk_type->class_inf[net_pin_class].type);
      net_pin_cnt++;
    }
  }
  assert(1 == net_pin_cnt); /* TODO: May conflict with I/O TYPE*/
  /* The port should be an output port */
  //assert(des_blk_port_index < blk[des_blk_index].pb->pb_graph_node->pb_type->num_ports);
  //assert(IN_PORT == blk[des_blk_index].pb->pb_graph_node->pb_type->ports[des_blk_port_index].type);

  return ret;
}

void esti_pin_chan_coordinate(int* pin_x, int* pin_y,
                              t_block blk, int pin_side, int pin_index) {
  /* Initialize */
  (*pin_x) = blk.x;
  (*pin_y) = blk.y;

  /* For hetergenous block, pin_y should be added by offset */
  assert(NULL != blk.type);
  (*pin_y) += blk.type->pin_height[pin_index];

  switch (pin_side) {
  case TOP:
    (*pin_y)++;
    break;
  case BOTTOM:
    break;
  case LEFT:
    break;
  case RIGHT:
    (*pin_x)++;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, LINE[%d])Invalid side!\n", __FILE__, __LINE__);
    exit(1);
  }
}

int count_blk_one_class_num_conflict(t_block* target_blk, int class_index,
                                     int* is_pin_conflict) {
  int ipin, class_num_pins, blk_pin_index;
  int num_conflict = 0;

  assert(NULL != target_blk);
  assert(NULL != target_blk->type);

  class_num_pins = target_blk->type->class_inf[class_index].num_pins;
  /* Check conflict number */
  num_conflict = 0;
  for (ipin = 0; ipin < class_num_pins; ipin++) {
    blk_pin_index = target_blk->type->class_inf[class_index].pinlist[ipin];
    if (1 == is_pin_conflict[blk_pin_index]) {
      num_conflict++;
    }
  } 

  return num_conflict;
}

/* Return an array, each element is the index of pin in target_blk */
int* sort_one_class_conflict_pins_by_low_slack(t_block* target_blk, int class_index,
                                               int* is_pin_conflict, int* num_conflict, 
                                               t_slack* slack) {
  int* ret_list = NULL;
  int ipin, class_num_pins, blk_pin_index;
  int net_index, net_sink_index;
  int* slack_pin_index = NULL;
  int* sorted_slack_pin_index= NULL;
  float* slack_values = NULL;
  int cur = 0;

  assert(NULL != target_blk);
  assert(NULL != target_blk->type);
  assert(RECEIVER == target_blk->type->class_inf[class_index].type);

  class_num_pins = target_blk->type->class_inf[class_index].num_pins;
  /* Check conflict number */
  (*num_conflict) = count_blk_one_class_num_conflict(target_blk, class_index, is_pin_conflict);

  ret_list = (int*)my_malloc(sizeof(int)*(*num_conflict));
  slack_pin_index = (int*)my_malloc(sizeof(int)*(*num_conflict));
  sorted_slack_pin_index = (int*)my_malloc(sizeof(int)*(*num_conflict));
  slack_values = (float*)my_malloc(sizeof(float)*(*num_conflict));

  cur = 0;
  for (ipin = 0; ipin < class_num_pins; ipin++) {
    blk_pin_index = target_blk->type->class_inf[class_index].pinlist[ipin];
    if (1 == is_pin_conflict[blk_pin_index]) {
      net_index = target_blk->nets[blk_pin_index];
      net_sink_index = target_blk->nets_sink_index[blk_pin_index];
      slack_pin_index[cur] = blk_pin_index;
      slack_values[cur] = slack->slack[net_index][net_sink_index];
      cur++;
    }
  }
  assert(cur == (*num_conflict));
  /* Use heapsort */
  heapsort(sorted_slack_pin_index, slack_values, (*num_conflict), 1);

  /* load ret_list*/
  for (ipin = 0; ipin < (*num_conflict); ipin++) {
    ret_list[ipin] = slack_pin_index[sorted_slack_pin_index[(*num_conflict) - 1 - ipin]];
    //ret_list[ipin] = slack_pin_index[sorted_slack_pin_index[ipin]];
  }

  /* Free */
  my_free(slack_pin_index);
  my_free(sorted_slack_pin_index);
  my_free(slack_values);
 
  return ret_list;
}

/* Check if swap the 2 pins, each prefer_side can be satified.
 * return 1 if both can be satified.
 */
int is_swap2pins_match_prefer_side(int pin0_cur_side, int* pin0_prefer_side,
                                   int pin1_cur_side, int* pin1_prefer_side) {
  /* Check */
  assert((!(0 > pin0_cur_side)&&(pin0_cur_side < 4)));
  assert((!(0 > pin1_cur_side)&&(pin1_cur_side < 4)));
  /* match?*/
  if ((1 == pin0_prefer_side[pin1_cur_side])&&(1 == pin1_prefer_side[pin0_cur_side])) {
    return 1;
  }

  return 0;
}

int is_type_pin_in_class(t_type_ptr type,
                         int class_index, int pin_index) {
  int ipin, blk_pin_index;
  int is_pin_in_class = 0;

  assert(NULL != type);
  assert((!(0 > class_index))&&(class_index < type->num_class));
  assert((!(0 > pin_index))&&(pin_index < type->num_pins));
  /* Check pin in this class */
  for (ipin = 0; ipin < type->class_inf[class_index].num_pins; ipin++) {
    blk_pin_index = type->class_inf[class_index].pinlist[ipin];
    if (pin_index == blk_pin_index) {
      is_pin_in_class++;
    }
  }  

  return is_pin_in_class;
}

/* Find all the to_rr_nodes that connected to src_pb pin */
void find_src_pb_pin_to_rr_nodes(t_pb* src_pb,
                                 int src_rr_node_index,
                                 int* num_pin_to_rr_nodes,
                                 t_rr_node*** to_rr_node) {
  int cur, iedge, to_node, mode_index;
  t_rr_node* local_rr_graph = NULL;

  /* Check */
  assert(NULL != src_pb);
  /* Load local rr_graph */
  assert(NULL != src_pb->rr_graph);
  local_rr_graph = src_pb->rr_graph;
  /* Find the selected_mode */
  mode_index = src_pb->mode; 
 
  /* Initialize */
  (*num_pin_to_rr_nodes) = 0;
  (*to_rr_node) = NULL;

  /* Return if the net_info infers OPEN */
  if (OPEN == local_rr_graph[src_rr_node_index].net_num) {
    return;
  }

  /* Count the number */
  for (iedge = 0; iedge < local_rr_graph[src_rr_node_index].num_edges; iedge++) {
    to_node = local_rr_graph[src_rr_node_index].edges[iedge];
    /* Check if this infer a connection */
    if ((src_rr_node_index == local_rr_graph[to_node].prev_node)
      &&(iedge == local_rr_graph[to_node].prev_edge)
      /* Make sure in the same mode */
      &&(mode_index == local_rr_graph[to_node].pb_graph_pin->parent_node->pb_type->parent_mode->index)) {
      (*num_pin_to_rr_nodes)++;
    }
  }
  /* Malloc */
  (*to_rr_node) = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*(*num_pin_to_rr_nodes));
  /* Fill the return list */
  cur = 0;
  for (iedge = 0; iedge < local_rr_graph[src_rr_node_index].num_edges; iedge++) {
    to_node = local_rr_graph[src_rr_node_index].edges[iedge];
    /* Check if this infer a connection */
    if ((src_rr_node_index == local_rr_graph[to_node].prev_node)
      &&(iedge == local_rr_graph[to_node].prev_edge)
      /* Make sure in the same mode */
      &&(mode_index == local_rr_graph[to_node].pb_graph_pin->parent_node->pb_type->parent_mode->index)) {
      (*to_rr_node)[cur] = &(local_rr_graph[to_node]);
      cur++; 
    }
  }
  assert(cur == (*num_pin_to_rr_nodes));

  return;
}

void connect_pb_des_pin_to_src_pin(t_pb* src_pb,
                                   int src_rr_node_index,
                                   int des_rr_node_index) {
  int iedge, mode_index, prev_edge, edge_cnt; 
  t_rr_node* local_rr_graph = NULL;

  /* Check */
  assert(NULL != src_pb);
  /* Load local rr_graph */
  assert(NULL != src_pb->rr_graph);
  local_rr_graph = src_pb->rr_graph;
  /* Find the selected_mode */
  mode_index = src_pb->mode; 
  /* Make sure in the same mode */
  assert(mode_index == local_rr_graph[des_rr_node_index].pb_graph_pin->parent_node->pb_type->parent_mode->index);
 
  /* Make sure the src_pin has an edge to des_pin */
  edge_cnt = 0;
  for (iedge = 0; iedge < local_rr_graph[src_rr_node_index].num_edges; iedge++) {
    if (des_rr_node_index == local_rr_graph[src_rr_node_index].edges[iedge]) {
      prev_edge = iedge;
      edge_cnt++;
    }
  }
  assert(1 == edge_cnt);
  /* Connect: give prev_edge, prev_node */
  local_rr_graph[des_rr_node_index].prev_node = src_rr_node_index;
  local_rr_graph[des_rr_node_index].prev_edge = prev_edge;

  return;
}

/* Swap 2 pins, in the same block*/
void swap_blk_same_class_2pins(t_block* target_blk, int n_nets, t_net* nets,
                               int pin0_index, int pin1_index) {
  int pin0_class_index, pin1_class_index;
  int inode, net_swap, net_index, net_sink_index;
  t_rr_node* local_rr_graph = NULL;
  t_rr_node* pin0_rr_node = NULL;
  t_rr_node* pin1_rr_node = NULL;
  int* prefer_side_swap;
  /* pin0_to_rr_node is NOT one node, may have a few!!! */
  int num_pin0_to_rr_nodes = 0;
  t_rr_node** pin0_to_rr_node = NULL;
  /* pin1_to_rr_node is NOT one node, may have a few!!! */
  int num_pin1_to_rr_nodes = 0;
  t_rr_node** pin1_to_rr_node = NULL;

  /* Check */
  assert(NULL != target_blk);
  assert(NULL != target_blk->type);
  assert((!(0 > pin0_index))&&(pin0_index < target_blk->type->num_pins));
  assert((!(0 > pin1_index))&&(pin1_index < target_blk->type->num_pins));
  assert(pin0_index != pin1_index);

  /* If two pins are OPEN Net, return */
  if ((OPEN == target_blk->nets[pin0_index])
     &&(OPEN == target_blk->nets[pin1_index])) {
    /* There is no need to swap two OPEN pins*/
    return;
  }

  /* Only support INPUT pins */ 
  pin0_class_index = target_blk->type->pin_class[pin0_index];
  pin1_class_index = target_blk->type->pin_class[pin1_index];
  assert(RECEIVER == target_blk->type->class_inf[pin0_class_index].type);
  assert(RECEIVER == target_blk->type->class_inf[pin1_class_index].type);
  /* Two pins should be in the same class */
  assert(pin0_class_index == pin1_class_index);

  /* Update pb local routing graph */
  /* load local_rr_graph */
  assert(NULL != target_blk->pb);
  local_rr_graph = target_blk->pb->rr_graph;
  /* find corresponding rr_node and pb_graph_pin of pin0 & pin1 */
  pin0_rr_node = &(local_rr_graph[pin0_index]); 
  pin1_rr_node = &(local_rr_graph[pin1_index]); 
  /* Make sure we find the correct rr_node and pb_graph_pin... */
  assert(pin0_index == pin0_rr_node->pb_graph_pin->pin_count_in_cluster);
  assert(pin1_index == pin1_rr_node->pb_graph_pin->pin_count_in_cluster);
  assert(pin0_class_index == pin0_rr_node->pb_graph_pin->pin_class);
  assert(pin1_class_index == pin1_rr_node->pb_graph_pin->pin_class);
  assert(0 == pin0_rr_node->pb_graph_pin->num_input_edges);
  assert(0 == pin1_rr_node->pb_graph_pin->num_input_edges);
  
  /* Find to_rr_nodes for PIN0 */
  find_src_pb_pin_to_rr_nodes(target_blk->pb, pin0_index, &num_pin0_to_rr_nodes, &pin0_to_rr_node);
  /* Find to_rr_nodes for PIN1 */
  find_src_pb_pin_to_rr_nodes(target_blk->pb, pin1_index, &num_pin1_to_rr_nodes, &pin1_to_rr_node);
  
  /* To SWAP: modify prev_node and prev_edge of pin0_to_rr_nodes, switch to pin1_rr_node */
  for (inode = 0; inode < num_pin0_to_rr_nodes; inode++) {
    connect_pb_des_pin_to_src_pin(target_blk->pb, pin1_index, pin0_to_rr_node[inode]->pb_graph_pin->pin_count_in_cluster);
  } 

  /* To SWAP: modify prev_node and prev_edge of pin1_to_rr_node, switch to pin0_rr_node */
  for (inode = 0; inode < num_pin1_to_rr_nodes; inode++) {
    connect_pb_des_pin_to_src_pin(target_blk->pb, pin0_index, pin1_to_rr_node[inode]->pb_graph_pin->pin_count_in_cluster);
  } 

  /* Update nets node_block_pin, should be done before updating blk nets, nets_sink_index !!! */
  /* Update blk nets, nets_sink_index, prefer_sides */
  net_swap = target_blk->nets[pin0_index];
  target_blk->nets[pin0_index] = target_blk->nets[pin1_index];
  target_blk->nets[pin1_index] = net_swap;

  net_swap = target_blk->nets_sink_index[pin0_index];
  target_blk->nets_sink_index[pin0_index] = target_blk->nets_sink_index[pin1_index];
  target_blk->nets_sink_index[pin1_index] = net_swap;
  
  prefer_side_swap = target_blk->pin_prefer_side[pin0_index];
  target_blk->pin_prefer_side[pin0_index] = target_blk->pin_prefer_side[pin1_index];
  target_blk->pin_prefer_side[pin1_index] = prefer_side_swap;

  /* Configure pin0_index net info */
  net_index = target_blk->nets[pin0_index]; 
  net_sink_index = target_blk->nets_sink_index[pin0_index]; 
  /* For OPEN NET */
  if (OPEN != net_index) {
    nets[net_index].node_block_pin[net_sink_index] = pin0_index;
  }
  /* Configure pin1_index net info */
  net_index = target_blk->nets[pin1_index]; 
  net_sink_index = target_blk->nets_sink_index[pin1_index]; 
  /* For OPEN NET */
  if (OPEN != net_index) {
    nets[net_index].node_block_pin[net_sink_index] = pin1_index;
  }

  /* Swap local_rr_graph node net_num */
  /* Net_num in local_rr_graph follows the index of vpack_net !!! */
  net_swap = pin0_rr_node->net_num;
  pin0_rr_node->net_num = pin1_rr_node->net_num;
  pin1_rr_node->net_num = net_swap;
  //pin0_rr_node->net_num = clb_to_vpack_net_mapping[target_blk->nets[pin0_index]]; /* pin0 */
  //pin1_rr_node->net_num = clb_to_vpack_net_mapping[target_blk->nets[pin1_index]]; /* pin1 */
  /* Update all successor rr_nodes net info */
  /* Update net info of pin1_to_rr_node, now it is the rr_node pin0_connected to */
  if (0 != num_pin1_to_rr_nodes) { /* pin1_to_rr_node now connected to pin0_rr_node */
  rec_update_net_info_local_rr_node_tree(target_blk->pb, pin0_rr_node->pb_graph_pin->pin_count_in_cluster);
  }
  /* Update net info of pin0_to_rr_node, now it is the rr_node pin1_connected to */
  if (0 != num_pin0_to_rr_nodes) { /* pin0_to_rr_node now connected to pin1_rr_node */
  rec_update_net_info_local_rr_node_tree(target_blk->pb, pin1_rr_node->pb_graph_pin->pin_count_in_cluster);
  }

  return;
}

void mark_blk_pins_nets_sink_index(int n_nets, t_net* nets,
                                   int n_blks, t_block* blk) {
  int inet, isink;
  int iblk, ipin;

  /* Malloc and Initialize all block nets_sink_index */
  for (iblk = 0; iblk < n_blks; iblk++) {
    assert(NULL != blk[iblk].type);
    blk[iblk].nets_sink_index = (int*)my_malloc(sizeof(int)*blk[iblk].type->num_pins);
    for (ipin = 0; ipin < blk[iblk].type->num_pins; ipin++) {
      blk[iblk].nets_sink_index[ipin] = -1;
    }
  }
  /* Set sink index for each block pin*/
  for (inet = 0; inet < n_nets; inet++) {
    for (isink = 0; isink < (nets[inet].num_sinks + 1); isink++) {
      iblk = nets[inet].node_block[isink];
      ipin = nets[inet].node_block_pin[isink];
      assert(inet == blk[iblk].nets[ipin]);
      blk[iblk].nets_sink_index[ipin] = isink;
    }
  }
  
  return;
}

void rec_update_net_info_local_rr_node_tree(t_pb* src_pb,
                                            int src_node_index) {
  int ipb, jpb, iedge, to_node, mode_index;
  t_rr_node* local_rr_graph = NULL;
  t_pb_graph_pin* des_pb_graph_pin = NULL;
  t_pb_type* src_pb_type = NULL;

  /* Load local rr_graph */  
  assert(NULL != src_pb);
  local_rr_graph = src_pb->rr_graph;
  assert(NULL != local_rr_graph);
  mode_index = src_pb->mode;
  
  /* Find the destination node */
  for (iedge = 0; iedge < local_rr_graph[src_node_index].num_edges; iedge++) {
    to_node = local_rr_graph[src_node_index].edges[iedge];
    if ((src_node_index == local_rr_graph[to_node].prev_node)&&(iedge == local_rr_graph[to_node].prev_edge)
       /* Make sure in the same mode */
      &&(mode_index == local_rr_graph[to_node].pb_graph_pin->parent_node->pb_type->parent_mode->index)) {
      des_pb_graph_pin = local_rr_graph[to_node].pb_graph_pin; 
      local_rr_graph[to_node].net_num = local_rr_graph[src_node_index].net_num;
      /* Update recursively */
      src_pb_type = src_pb->pb_graph_node->pb_type;
      /* if reach leaf, stop */  
      if (NULL != src_pb_type->blif_model) {
        continue;
      }
      /* recursive for the child_pbs*/
      for (ipb = 0; ipb < src_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
        for (jpb = 0; jpb < src_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
          /* Refer to pack/output_clustering.c [LINE 392] */
          if ((NULL != src_pb->child_pbs[ipb])&&(NULL != src_pb->child_pbs[ipb][jpb].name)) {
            rec_update_net_info_local_rr_node_tree(&(src_pb->child_pbs[ipb][jpb]), des_pb_graph_pin->pin_count_in_cluster);
          }
        }
      }
    }
  }
  
  return;
}


