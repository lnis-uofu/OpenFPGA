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

/* Include spice support headers*/
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_lut.h"

/***** Recursively Backannotate parasitic_net_num for a rr_node*****/
void rec_backannotate_rr_node_net_num(int LL_num_rr_nodes,
                                      t_rr_node* LL_rr_node,
                                      int src_node_index) {
  int iedge, to_node;
  
  /* Traversal until 
   * 1. we meet a sink
   * 2. None of the edges propagates this net_num 
   */
  for (iedge = 0; iedge < LL_rr_node[src_node_index].num_edges; iedge++) {
    to_node = LL_rr_node[src_node_index].edges[iedge];
    assert(OPEN != LL_rr_node[to_node].prev_node);
    if (src_node_index == LL_rr_node[to_node].prev_node) {
      assert(iedge == LL_rr_node[to_node].prev_edge);
      /* assert(LL_rr_node[src_node_index].net_num == LL_rr_node[to_node].net_num); */
      /* Propagate the net_num */
      LL_rr_node[to_node].net_num = LL_rr_node[src_node_index].net_num; 
      LL_rr_node[to_node].vpack_net_num = LL_rr_node[src_node_index].vpack_net_num; 
      /* Go recursively */ 
      rec_backannotate_rr_node_net_num(LL_num_rr_nodes, LL_rr_node, to_node);
    }
  }
  
  return;
} 

/***** Backannotate activity information to nets *****/
/* Mark mapped rr_nodes with net_num*/
static 
void backannotate_rr_nodes_net_info() {
  int iblk, ipin, class_id, pin_global_rr_node_id;
  t_rr_node* pb_rr_graph = NULL;
 
  /* Start from all the pb OPINs, and forward to all the rr_nodes */
  for (iblk = 0; iblk < num_blocks; iblk++) {
    assert(NULL != block[iblk].pb);
    /* load pb_rr_graph */
    pb_rr_graph = block[iblk].pb->rr_graph;
    /* For each output pin, we find the pb net_num */
    for (ipin = 0; ipin < block[iblk].type->num_pins; ipin++) {
      class_id = block[iblk].type->pin_class[ipin];
      /* Bypass not OPINs*/
      if (DRIVER != block[iblk].type->class_inf[class_id].type) {
        continue;
      }
      /* get the global rr_node */ 
      pin_global_rr_node_id = get_rr_node_index(block[iblk].x, block[iblk].y, OPIN, ipin, rr_node_indices);
      /* Give IO_TYPE vpack_net_num */
      if (IO_TYPE == block[iblk].type) {
        if (OPEN == rr_node[pin_global_rr_node_id].net_num) {
          rr_node[pin_global_rr_node_id].net_num = OPEN;
          rr_node[pin_global_rr_node_id].vpack_net_num = OPEN;
        } else if (OPEN == block[iblk].nets[ipin]){
          rr_node[pin_global_rr_node_id].net_num = OPEN;
          rr_node[pin_global_rr_node_id].vpack_net_num = OPEN;
        } else {
          rr_node[pin_global_rr_node_id].net_num = block[iblk].nets[ipin];
          rr_node[pin_global_rr_node_id].vpack_net_num = clb_to_vpack_net_mapping[block[iblk].nets[ipin]];
        }
      } else {
        assert(rr_node[pin_global_rr_node_id].vpack_net_num == pb_rr_graph[ipin].vpack_net_num);
      }
      /* Bypass unmapped pins */
      if (OPEN == rr_node[pin_global_rr_node_id].vpack_net_num) {
        continue;
      }
      //printf("Updating all traces with vpack_net_num(name:%s)...\n", 
      //       vpack_net[rr_node[pin_global_rr_node_id].vpack_net_num].name); 
      //assert(rr_node[pin_global_rr_node_id].net_num == vpack_to_clb_net_mapping[pb_rr_graph[ipin].net_num]);
      /* Forward to all the downstream rr_nodes */
      rec_backannotate_rr_node_net_num(num_rr_nodes, rr_node, pin_global_rr_node_id); 
    }
  }
 
  return;
}

static 
void backannotate_clb_nets_init_val() {
  int inet, iblk, isink;
  int iter_cnt, iter_end;

  /* Analysis init values !!! */
  for (inet = 0; inet < num_logical_nets; inet++) {
    assert (NULL != vpack_net[inet].spice_net_info);
    /* if the source is a inpad or dff, we update the initial value */ 
    iblk = vpack_net[inet].node_block[0];
    switch (logical_block[iblk].type) {
    case VPACK_INPAD:
      logical_block[iblk].init_val = vpack_net[inet].spice_net_info->init_val;
      assert((0 == logical_block[iblk].init_val)||(1 == logical_block[iblk].init_val));
      break;
    case VPACK_LATCH:
      vpack_net[inet].spice_net_info->init_val = 0;
      /*TODO:may be more flexible, for ff, set or reset may be used in first cock cycle */
      logical_block[iblk].init_val = vpack_net[inet].spice_net_info->init_val;
      assert((0 == logical_block[iblk].init_val)||(1 == logical_block[iblk].init_val));
      break;
    case VPACK_OUTPAD:
    case VPACK_COMB:
    case VPACK_EMPTY:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid logical block type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }
  /* Iteratively Update LUT init_val */
  iter_cnt = 0;
  while(1) {
    iter_end = 1;
    for (inet = 0; inet < num_logical_nets; inet++) {
      assert(NULL != vpack_net[inet].spice_net_info);
      /* if the source is a inpad or dff, we update the initial value */ 
      iblk = vpack_net[inet].node_block[0];
      switch (logical_block[iblk].type) {
      case VPACK_COMB:
        vpack_net[inet].spice_net_info->init_val = get_lut_output_init_val(&(logical_block[iblk]));
        if (logical_block[iblk].init_val != vpack_net[inet].spice_net_info->init_val) {
          iter_end = 0;
        }
        logical_block[iblk].init_val = vpack_net[inet].spice_net_info->init_val;
        break;
      case VPACK_INPAD:
      case VPACK_LATCH:
      case VPACK_OUTPAD:
      case VPACK_EMPTY:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid logical block type!\n",
                 __FILE__, __LINE__);
        exit(1);
      }
    }
    iter_cnt++;
    if (1 == iter_end) {
      break;
    }
  }
  vpr_printf(TIO_MESSAGE_INFO,"Determine LUTs initial outputs ends in %d iterations.\n", iter_cnt);
  /* Update OUTPAD init_val */
  for (inet = 0; inet < num_logical_nets; inet++) {
    assert(NULL != vpack_net[inet].spice_net_info);
    /* if the source is a inpad or dff, we update the initial value */ 
    for (isink = 0; isink < vpack_net[inet].num_sinks; isink++) {
      iblk = vpack_net[inet].node_block[isink];
      switch (logical_block[iblk].type) {
      case VPACK_OUTPAD:
        logical_block[iblk].init_val = vpack_net[inet].spice_net_info->init_val;
        break;
      case VPACK_COMB:
      case VPACK_INPAD:
      case VPACK_LATCH:
      case VPACK_EMPTY:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid logical block type!\n",
                   __FILE__, __LINE__);
        exit(1);
      }
    }
  }

  /* Initial values for clb nets !!! */
  for (inet = 0; inet < num_nets; inet++) {
    assert (NULL != clb_net[inet].spice_net_info);
    /* if the source is a inpad or dff, we update the initial value */ 
    clb_net[inet].spice_net_info->init_val = vpack_net[clb_to_vpack_net_mapping[inet]].spice_net_info->init_val;
  }

  return;
}

static 
void backannotate_clb_nets_act_info() {
  int inet;

  /* Free all spice_net_info and reallocate */
  for (inet = 0; inet < num_logical_nets; inet++) {
    if (NULL == vpack_net[inet].spice_net_info) {
      /* Allocate */
      vpack_net[inet].spice_net_info = (t_spice_net_info*)my_malloc(sizeof(t_spice_net_info));
    } 
    /* Initialize to zero */
    init_spice_net_info(vpack_net[inet].spice_net_info);
    /* Load activity info */
    vpack_net[inet].spice_net_info->probability = vpack_net[inet].net_power->probability;
    vpack_net[inet].spice_net_info->density = vpack_net[inet].net_power->density;
    /* SPECIAL for SPICE simulator: init_value is opposite to probability 
     * when density is not zero.
     */
    /*
    if (0. != vpack_net[inet].spice_net_info->density) {
      vpack_net[inet].spice_net_info->init_val = 1 - vpack_net[inet].spice_net_info->probability; 
    }
    */
  }
  
  /* Free all spice_net_info and reallocate */
  for (inet = 0; inet < num_nets; inet++) {
    if (NULL == clb_net[inet].spice_net_info) {
      /* Allocate */
      clb_net[inet].spice_net_info = (t_spice_net_info*)my_malloc(sizeof(t_spice_net_info));
    } 
    /* Initialize to zero */
    init_spice_net_info(clb_net[inet].spice_net_info);
    /* Load activity info */
    clb_net[inet].spice_net_info->probability = vpack_net[clb_to_vpack_net_mapping[inet]].spice_net_info->probability;
    clb_net[inet].spice_net_info->density = vpack_net[clb_to_vpack_net_mapping[inet]].spice_net_info->density;
    clb_net[inet].spice_net_info->init_val = vpack_net[clb_to_vpack_net_mapping[inet]].spice_net_info->init_val;
  }

  return;
}

void free_clb_nets_spice_net_info() {
  int inet;
  
  /* Free all spice_net_info and reallocate */
  for (inet = 0; inet < num_nets; inet++) {
    my_free(clb_net[inet].spice_net_info);
  }

  for (inet = 0; inet < num_logical_nets; inet++) {
    my_free(vpack_net[inet].spice_net_info);
  }

  return;
}

static 
void build_prev_node_list_rr_nodes(int LL_num_rr_nodes,
                                   t_rr_node* LL_rr_node) {
  int inode, iedge, to_node, cur;
  /* int jnode, switch_box_x, switch_box_y, chan_side, switch_index; */
  int* cur_index = (int*)my_malloc(sizeof(int)*LL_num_rr_nodes);
  
  /* This function is not timing-efficient, I comment it */
  /*
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    find_prev_rr_nodes_with_src(&(LL_rr_nodes[inode]), 
                                &(LL_rr_nodes[inode].num_drive_rr_nodes),
                                &(LL_rr_nodes[inode].drive_rr_nodes),
                                &(LL_rr_nodes[inode].drive_switches));
  }
  */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    /* Malloc */
    LL_rr_node[inode].num_drive_rr_nodes = LL_rr_node[inode].fan_in;
    LL_rr_node[inode].drive_rr_nodes = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*LL_rr_node[inode].num_drive_rr_nodes);
    LL_rr_node[inode].drive_switches = (int*)my_malloc(sizeof(int)*LL_rr_node[inode].num_drive_rr_nodes);
  }
  /* Initialize */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    cur_index[inode] = 0;
    for (iedge = 0; iedge < LL_rr_node[inode].num_drive_rr_nodes; iedge++) {
      LL_rr_node[inode].drive_rr_nodes[iedge] = NULL;
      LL_rr_node[inode].drive_switches[iedge] = -1;
    }
  }
  /* Fill */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    for (iedge = 0; iedge < LL_rr_node[inode].num_edges; iedge++) {
      to_node = LL_rr_node[inode].edges[iedge]; 
      cur = cur_index[to_node];
      LL_rr_node[to_node].drive_rr_nodes[cur] = &(LL_rr_node[inode]);
      LL_rr_node[to_node].drive_switches[cur] = LL_rr_node[inode].switches[iedge];
      /* Update cur_index[to_node]*/
      assert(NULL != LL_rr_node[to_node].drive_rr_nodes[cur]);
      cur_index[to_node]++;
    }
  }
  /* Check */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    assert(cur_index[inode] == LL_rr_node[inode].num_drive_rr_nodes);
  }

  /* TODO: fill the sb_drive_rr_nodes */
  //for (inode = 0; inode < LL_num_rr_nodes; inode++) {
  //  /* Initial */
  //  LL_rr_node[inode].sb_num_drive_rr_nodes = 0;
  //  LL_rr_node[inode].sb_drive_rr_nodes = NULL;
  //  LL_rr_node[inode].sb_drive_switches = NULL;
  //  /* Find SB source rr nodes:  channels*/
  //  switch (LL_rr_node[inode].type) {
  //  case CHANX:
  //    assert(LL_rr_node[inode].ylow == LL_rr_node[inode].yhigh);
  //    switch (LL_rr_node[inode].direction) {
  //    case INC_DIRECTION:
  //      switch_box_x = LL_rr_node[inode].xlow-1;
  //      switch_box_y = LL_rr_node[inode].ylow;
  //      chan_side = RIGHT;
  //      break;
  //    case DEC_DIRECTION:
  //      switch_box_x = LL_rr_node[inode].xhigh;
  //      switch_box_y = LL_rr_node[inode].yhigh;
  //      chan_side = LEFT;
  //      break;
  //    case BI_DIRECTION:
  //      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Bidirectional routing wires are not supported!\n",
  //                 __FILE__, __LINE__);
  //      exit(1);
  //    default:
  //      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid rr_node direction!\n",
  //                 __FILE__, __LINE__);
  //      exit(1);
  //    }
  //    find_drive_rr_nodes_switch_box(switch_box_x, switch_box_y, &(LL_rr_node[inode]), chan_side, 0, 
  //                                   &(LL_rr_node[inode].sb_num_drive_rr_nodes), 
  //                                   &(LL_rr_node[inode].sb_drive_rr_nodes), &switch_index);
  //    /* fill the sb_drive_switches */
  //    LL_rr_node[inode].sb_drive_switches = (int*)my_malloc(sizeof(int)*LL_rr_node[inode].sb_num_drive_rr_nodes);
  //    for (jnode = 0; jnode < LL_rr_node[inode].sb_num_drive_rr_nodes; jnode++) {
  //      LL_rr_node[inode].sb_drive_switches[jnode] = switch_index;
  //    }
  //    break;
  //  case CHANY:
  //    /* TODO: fill the sb_drive_rr_nodes */
  //    assert(LL_rr_node[inode].xlow == LL_rr_node[inode].xhigh);
  //    switch (LL_rr_node[inode].direction) {
  //    case INC_DIRECTION:
  //      switch_box_x = LL_rr_node[inode].xlow;
  //      switch_box_y = LL_rr_node[inode].ylow-1;
  //      chan_side = TOP;
  //      break;
  //    case DEC_DIRECTION:
  //      switch_box_x = LL_rr_node[inode].xhigh;
  //      switch_box_y = LL_rr_node[inode].yhigh;
  //      chan_side = BOTTOM;
  //      break;
  //    case BI_DIRECTION:
  //      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Bidirectional routing wires are not supported!\n",
  //                 __FILE__, __LINE__);
  //      exit(1);
  //    default:
  //      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid rr_node direction!\n",
  //                 __FILE__, __LINE__);
  //      exit(1);
  //    }
  //    find_drive_rr_nodes_switch_box(switch_box_x, switch_box_y, &(LL_rr_node[inode]), chan_side, 0, 
  //                                   &(LL_rr_node[inode].sb_num_drive_rr_nodes), 
  //                                   &(LL_rr_node[inode].sb_drive_rr_nodes), &switch_index);
  //    /* fill the sb_drive_switches */
  //    LL_rr_node[inode].sb_drive_switches = (int*)my_malloc(sizeof(int)*LL_rr_node[inode].sb_num_drive_rr_nodes);
  //    for (jnode = 0; jnode < LL_rr_node[inode].sb_num_drive_rr_nodes; jnode++) {
  //      LL_rr_node[inode].sb_drive_switches[jnode] = switch_index;
  //    }
  //    break;
  //  case SOURCE:
  //  case OPIN:
  //  case SINK:
  //  case IPIN:
  //  case NUM_RR_TYPES:
  //    break;
  //  default:
  //    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid rr_node type!\n",
  //               __FILE__, __LINE__);
  //    exit(1);
  //  }
  //}

  return;
}

static
void set_one_pb_rr_node_default_prev_node_edge(t_rr_node* pb_rr_graph, 
                                               t_pb_graph_pin* des_pb_graph_pin) {
  int iedge, node_index, prev_node, prev_edge;

  assert(NULL != des_pb_graph_pin);
  assert(NULL != pb_rr_graph);

  node_index = des_pb_graph_pin->pin_count_in_cluster;
  assert(OPEN == pb_rr_graph[node_index].net_num);
   
  /* if this pin has 0 driver, return OPEN */
  if (0 == des_pb_graph_pin->num_input_edges) {
    pb_rr_graph[node_index].prev_node = OPEN;
    pb_rr_graph[node_index].prev_edge = OPEN;
    return;
  }

  prev_node = OPEN;
  prev_edge = OPEN;

  /* Set default prev_node */
  check_pb_graph_edge(*(des_pb_graph_pin->input_edges[0]));
  prev_node = des_pb_graph_pin->input_edges[0]->input_pins[0]->pin_count_in_cluster;
  /* Find prev_edge */
  for (iedge = 0; iedge < pb_rr_graph[prev_node].pb_graph_pin->num_output_edges; iedge++) {
    check_pb_graph_edge(*(pb_rr_graph[prev_node].pb_graph_pin->output_edges[iedge]));
    if (node_index == pb_rr_graph[prev_node].pb_graph_pin->output_edges[iedge]->output_pins[0]->pin_count_in_cluster) {
      prev_edge = iedge;
      break;
    }
  } 
  /* Make sure we succeed */
  assert(OPEN != prev_node);
  assert(OPEN != prev_edge);
  /* backannotate */
  pb_rr_graph[node_index].prev_node = prev_node;
  pb_rr_graph[node_index].prev_edge = prev_edge;

  return;
}       

/* Mark the prev_edge and prev_node of all the rr_nodes in complex blocks */
static
void back_annotate_one_pb_rr_node_map_info_rec(t_pb* cur_pb) {
  int ipb, jpb, select_mode_index;
  int iport, ipin, node_index;
  t_rr_node* pb_rr_nodes = NULL;
  t_pb_graph_node* child_pb_graph_node;
 
  /* Return when we meet a null pb */ 
  if (NULL == cur_pb) {
    return;
  }

  /* Reach a leaf, return */
  if ((0 == cur_pb->pb_graph_node->pb_type->num_modes)
     ||(NULL == cur_pb->child_pbs)) {
    return;
  }

  select_mode_index = cur_pb->mode; 
  
  /* For all the input/output/clock pins of this pb,
   * check the net_num and assign default prev_node, prev_edge 
   */

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (iport = 0; iport < cur_pb->pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_output_pins[iport]; ipin++) {
      /* Get the selected edge of current pin*/
      pb_rr_nodes = cur_pb->rr_graph;
      node_index = cur_pb->pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      /* If we find an OPEN net, try to find the parasitic net_num*/
      if (OPEN == pb_rr_nodes[node_index].net_num) {
        set_one_pb_rr_node_default_prev_node_edge(pb_rr_nodes, &(cur_pb->pb_graph_node->output_pins[iport][ipin])); 
      } else {
        pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
      }
    }
  }

  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      child_pb_graph_node = &(cur_pb->pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]);
      /* For each child_pb_graph_node input pins*/
      for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
          /* Get the selected edge of current pin*/
          pb_rr_nodes = cur_pb->rr_graph;
          node_index = child_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_default_prev_node_edge(pb_rr_nodes, &(child_pb_graph_node->input_pins[iport][ipin])); 
           } else {
             pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
          }
        }
      }
      /* For each child_pb_graph_node clock pins*/
      for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
          /* Get the selected edge of current pin*/
          pb_rr_nodes = cur_pb->rr_graph;
          node_index = child_pb_graph_node->clock_pins[iport][ipin].pin_count_in_cluster;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_default_prev_node_edge(pb_rr_nodes, &(child_pb_graph_node->clock_pins[iport][ipin])); 
          } else {
            pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
          }
        }
      }
    }
  }
  
  /* Go recursively */ 
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        back_annotate_one_pb_rr_node_map_info_rec(&(cur_pb->child_pbs[ipb][jpb]));
      }
    }
  }

  return;
}

/* Mark all prev_node & prev_edge for pb_rr_nodes */
static 
void back_annotate_pb_rr_node_map_info() {
  int iblk;
  
  /* Foreach grid */
  for (iblk = 0; iblk < num_blocks; iblk++) {
    /* By pass IO */
    /*
    if (IO_TYPE == block[iblk].type) {
      continue;
    }
    */
    back_annotate_one_pb_rr_node_map_info_rec(block[iblk].pb);
  }  

  return;
}

/* Set the net_num for one pb_rr_node according to prev_node */
static
void set_one_pb_rr_node_net_num(t_rr_node* pb_rr_graph, 
                                t_pb_graph_pin* des_pb_graph_pin) {

  int node_index, prev_node, prev_edge;

  assert(NULL != des_pb_graph_pin);
  assert(NULL != pb_rr_graph);

  node_index = des_pb_graph_pin->pin_count_in_cluster;
  assert(OPEN == pb_rr_graph[node_index].net_num);
   
  /* if this pin has 0 driver, return OPEN */
  if (0 == des_pb_graph_pin->num_input_edges) {
    pb_rr_graph[node_index].net_num= OPEN;
    pb_rr_graph[node_index].vpack_net_num= OPEN;
    return;
  }

  prev_node = pb_rr_graph[node_index].prev_node;
  prev_edge = pb_rr_graph[node_index].prev_edge;
  assert(OPEN != prev_node); 
  assert(OPEN != prev_edge); 

  /* Set default prev_node */
  check_pb_graph_edge(*(pb_rr_graph[prev_node].pb_graph_pin->output_edges[prev_edge]));
  assert(node_index == pb_rr_graph[prev_node].pb_graph_pin->output_edges[prev_edge]->output_pins[0]->pin_count_in_cluster);
  pb_rr_graph[node_index].net_num = pb_rr_graph[prev_node].net_num;
  pb_rr_graph[node_index].vpack_net_num = pb_rr_graph[prev_node].net_num;

  return;
}

/* Mark the net_num of all the rr_nodes in complex blocks */
static
void backannotate_one_pb_rr_nodes_net_info_rec(t_pb* cur_pb) {
  int ipb, jpb, select_mode_index;
  int iport, ipin, node_index;
  t_rr_node* pb_rr_nodes = NULL;
  t_pb_graph_node* child_pb_graph_node = NULL;
 
  /* Return when we meet a null pb */ 
  assert (NULL != cur_pb);

  /* Reach a leaf, return */
  if ((0 == cur_pb->pb_graph_node->pb_type->num_modes)
     ||(NULL == cur_pb->child_pbs)) {
    return;
  }

  select_mode_index = cur_pb->mode; 

  /* For all the input/output/clock pins of this pb,
   * check the net_num and assign default prev_node, prev_edge 
   */

  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      child_pb_graph_node = &(cur_pb->pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]);
      /* For each child_pb_graph_node input pins*/
      for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
          /* Get the selected edge of current pin*/
          pb_rr_nodes = cur_pb->rr_graph;
          node_index = child_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_net_num(pb_rr_nodes, &(child_pb_graph_node->input_pins[iport][ipin])); 
          } else {
            assert(pb_rr_nodes[node_index].net_num == pb_rr_nodes[node_index].vpack_net_num);
          }
        }
      }
      /* For each child_pb_graph_node clock pins*/
      for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
          /* Get the selected edge of current pin*/
          pb_rr_nodes = cur_pb->rr_graph;
          node_index = child_pb_graph_node->clock_pins[iport][ipin].pin_count_in_cluster;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_net_num(pb_rr_nodes, &(child_pb_graph_node->clock_pins[iport][ipin])); 
          } else {
            assert(pb_rr_nodes[node_index].net_num == pb_rr_nodes[node_index].vpack_net_num);
          }
        }
      }
    }
  }

  /* Go recursively, prior to update the output pins.
   * Because output pins are depend on the deepest pbs
   */ 
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        backannotate_one_pb_rr_nodes_net_info_rec(&(cur_pb->child_pbs[ipb][jpb]));
      }
    }
  }

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (iport = 0; iport < cur_pb->pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_output_pins[iport]; ipin++) {
      /* Get the selected edge of current pin*/
      pb_rr_nodes = cur_pb->rr_graph;
      node_index = cur_pb->pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      /* If we find an OPEN net, try to find the parasitic net_num*/
      if (OPEN == pb_rr_nodes[node_index].net_num) {
        set_one_pb_rr_node_net_num(pb_rr_nodes, &(cur_pb->pb_graph_node->output_pins[iport][ipin])); 
      } else {
        assert(pb_rr_nodes[node_index].net_num == pb_rr_nodes[node_index].vpack_net_num);
      }
    }
  }
  
  return;
}

static 
void backannotate_pb_rr_nodes_net_info() {
  int iblk;
  
  /* Foreach grid */
  for (iblk = 0; iblk < num_blocks; iblk++) {
    /* By pass IO */
    /*
    if (IO_TYPE == block[iblk].type) {
      continue;
    }
    */
    backannotate_one_pb_rr_nodes_net_info_rec(block[iblk].pb);
  }  

  return;
}

/* Mark the prev_edge and prev_node of all the rr_nodes in global routing */
static 
void back_annotate_rr_node_map_info() {
  int inode, jnode, inet, xlow, ylow;
  int next_node, iedge;
  t_trace* tptr;
  t_rr_type rr_type;

  /* 1st step: Set all the configurations to default.
   * rr_nodes select edge[0]
   */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    rr_node[inode].prev_node = OPEN;
    /* set 0 if we want print all unused mux!!!*/
    rr_node[inode].prev_edge = OPEN;
    /* Initial all the net_num*/
    rr_node[inode].net_num = OPEN;
    rr_node[inode].vpack_net_num = OPEN;
  }
  for (inode = 0; inode < num_rr_nodes; inode++) {
    if (0 == rr_node[inode].num_edges) {
      continue;
    }  
    assert(0 < rr_node[inode].num_edges);
    for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
      jnode = rr_node[inode].edges[iedge];
      if (&(rr_node[inode]) == rr_node[jnode].drive_rr_nodes[0]) {
        rr_node[jnode].prev_node = inode;
        rr_node[jnode].prev_edge = iedge;
      }
    }
  }

  /* 2nd step: With the help of trace, we back-annotate */
  for (inet = 0; inet < num_nets; inet++) {
    //if (TRUE == clb_net[inet].is_global) {
    //  continue;
    //}
    tptr = trace_head[inet];
    while (tptr != NULL) {
      inode = tptr->index;
      rr_type = rr_node[inode].type;
      xlow = rr_node[inode].xlow;
      ylow = rr_node[inode].ylow;
      /* Net num */
      rr_node[inode].net_num = inet;
      rr_node[inode].vpack_net_num = clb_to_vpack_net_mapping[inet];
      //printf("Mark rr_node net_num for vpack_net(name=%s)..\n",
      //        vpack_net[rr_node[inode].vpack_net_num].name);
      assert(OPEN != rr_node[inode].net_num);
      assert(OPEN != rr_node[inode].vpack_net_num);
      switch (rr_type) {
      case SINK: 
        /* Nothing should be done. This supposed to the end of a trace*/
        break;
      case IPIN: 
      case CHANX: 
      case CHANY: 
      case OPIN: 
      case SOURCE: 
        /* SINK(IO/Pad) is the end of a routing path. Should configure its prev_edge and prev_node*/
        /* We care the next rr_node, this one is driving, which we have to configure 
         */
        assert(NULL != tptr->next);
        next_node = tptr->next->index;
        assert((!(0 > next_node))&&(next_node < num_rr_nodes));
        /* Prev_node */
        rr_node[next_node].prev_node = inode;
        /* Prev_edge */
        rr_node[next_node].prev_edge = OPEN;
        for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
          if (next_node == rr_node[inode].edges[iedge]) {
            rr_node[next_node].prev_edge = iedge;
            break;
          }
        }
        assert(OPEN != rr_node[next_node].prev_edge);
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid traceback element type.\n");
        exit(1);
      }
      tptr = tptr->next;
    }
  }

  return;
}

void backup_one_pb_rr_node_pack_prev_node_edge(t_rr_node* pb_rr_node) {

  pb_rr_node->prev_node_in_pack = pb_rr_node->prev_node; 
  pb_rr_node->prev_edge_in_pack = pb_rr_node->prev_edge; 
  pb_rr_node->net_num_in_pack = pb_rr_node->net_num; 
  pb_rr_node->prev_node = OPEN; 
  pb_rr_node->prev_edge = OPEN; 

  return;
}

/* During routing stage, VPR swap logic equivalent pins
 * which potentially changes the packing results (prev_node, prev_edge) in local routing
 * The following functions are to update the local routing results to match them with routing results
 */
void update_one_grid_pack_prev_node_edge(int x, int y) {
  int iblk, blk_id, ipin, iedge, jedge, inode;
  int pin_global_rr_node_id, vpack_net_id, class_id;
  t_type_ptr type = NULL;
  t_pb* pb = NULL;
  t_rr_node* local_rr_graph = NULL;

  /* Assert */
  assert((!(x < 0))&&(x < (nx + 2)));  
  assert((!(y < 0))&&(y < (ny + 2)));  

  type = grid[x][y].type;
  /* Bypass IO_TYPE*/
  if ((EMPTY_TYPE == type)||(IO_TYPE == type)) {
  //if ((EMPTY_TYPE == type)) {
    return;
  }   
  for (iblk = 0; iblk < grid[x][y].usage; iblk++) {
    blk_id = grid[x][y].blocks[iblk];
    if ((IO_TYPE != type)) {
      assert(block[blk_id].x == x);
      assert(block[blk_id].y == y);
    }
    pb = block[blk_id].pb;
    assert(NULL != pb);
    local_rr_graph = pb->rr_graph; 
    /* Foreach local rr_node*/
    for (ipin = 0; ipin < type->num_pins; ipin++) {
      class_id = type->pin_class[ipin];
      if (DRIVER == type->class_inf[class_id].type) {
        /* Find the pb net_num and update OPIN net_num */
        pin_global_rr_node_id = get_rr_node_index(x, y, OPIN, ipin, rr_node_indices);
        if (OPEN == rr_node[pin_global_rr_node_id].net_num) {
          local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
          local_rr_graph[ipin].net_num = OPEN;
          local_rr_graph[ipin].vpack_net_num = OPEN;
          //local_rr_graph[ipin].prev_node = 0;
          //local_rr_graph[ipin].prev_edge = 0;
          continue; /* bypass non-mapped OPIN */
        } 
        /* back annotate pb ! */
        rr_node[pin_global_rr_node_id].pb = pb;
        vpack_net_id = clb_to_vpack_net_mapping[rr_node[pin_global_rr_node_id].net_num];
        //printf("Update post-route pb_rr_graph output: vpack_net_name = %s\n", 
        //        vpack_net[vpack_net_id].name);
        /* Special for IO_TYPE */
        if (IO_TYPE == type) {
          assert(local_rr_graph[ipin].net_num == rr_node[pin_global_rr_node_id].vpack_net_num);
          local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
          continue;
        }
        assert(ipin == local_rr_graph[ipin].pb_graph_pin->pin_count_in_cluster);
        /* Update net_num */
        local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
        local_rr_graph[ipin].net_num = vpack_net_id;
        local_rr_graph[ipin].vpack_net_num = vpack_net_id;
        /* TODO: this is not so efficient... */
        for (iedge = 0; iedge < local_rr_graph[ipin].pb_graph_pin->num_input_edges; iedge++) {
          check_pb_graph_edge(*(local_rr_graph[ipin].pb_graph_pin->input_edges[iedge]));
          inode = local_rr_graph[ipin].pb_graph_pin->input_edges[iedge]->input_pins[0]->pin_count_in_cluster;
          /* Update prev_node, prev_edge if needed*/
          if (vpack_net_id == local_rr_graph[inode].net_num) {
            /* Backup prev_node, prev_edge */ 
            backup_one_pb_rr_node_pack_prev_node_edge(&(local_rr_graph[ipin]));
            local_rr_graph[ipin].prev_node = inode;
            for (jedge = 0; jedge < local_rr_graph[inode].pb_graph_pin->num_output_edges; jedge++) {
              if (local_rr_graph[ipin].pb_graph_pin == local_rr_graph[inode].pb_graph_pin->output_edges[jedge]->output_pins[0]) {
                local_rr_graph[ipin].prev_edge = jedge;
                break;
              }
            }
            break;
          }
        }
      } else if (RECEIVER == type->class_inf[class_id].type) {
        /* Find the global rr_node net_num and update pb net_num */
        pin_global_rr_node_id = get_rr_node_index(x, y, IPIN, ipin, rr_node_indices);
        /* Get the index of Vpack net from global rr_node net_num (clb_net index)*/
        if (OPEN == rr_node[pin_global_rr_node_id].net_num) {
          local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
          local_rr_graph[ipin].net_num = OPEN;
          local_rr_graph[ipin].vpack_net_num = OPEN;
          //local_rr_graph[ipin].prev_node = 0;
          //local_rr_graph[ipin].prev_edge = 0;
          continue; /* bypass non-mapped IPIN */
        }
        /* back annotate pb ! */
        rr_node[pin_global_rr_node_id].pb = pb;
        vpack_net_id = clb_to_vpack_net_mapping[rr_node[pin_global_rr_node_id].net_num];
        //printf("Update post-route pb_rr_graph input: vpack_net_name = %s\n", 
        //        vpack_net[vpack_net_id].name);
        /* Special for IO_TYPE */
        if (IO_TYPE == type) {
          assert(local_rr_graph[ipin].net_num == rr_node[pin_global_rr_node_id].vpack_net_num);
          local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
          continue;
        }
        assert(ipin == local_rr_graph[ipin].pb_graph_pin->pin_count_in_cluster);
        /* Update net_num */
        local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
        local_rr_graph[ipin].net_num = vpack_net_id;
        local_rr_graph[ipin].vpack_net_num = vpack_net_id;
        /* TODO: this is not so efficient... */
        for (iedge = 0; iedge < local_rr_graph[ipin].pb_graph_pin->num_output_edges; iedge++) {
          check_pb_graph_edge(*(local_rr_graph[ipin].pb_graph_pin->output_edges[iedge]));
          inode = local_rr_graph[ipin].pb_graph_pin->output_edges[iedge]->output_pins[0]->pin_count_in_cluster;
          /* Update prev_node, prev_edge if needed*/
          if (vpack_net_id == local_rr_graph[inode].net_num) {
            /* Backup prev_node, prev_edge */ 
            backup_one_pb_rr_node_pack_prev_node_edge(&(local_rr_graph[inode]));
            local_rr_graph[inode].prev_node = ipin;
            local_rr_graph[inode].prev_edge = iedge;
          }
        }
      } else {
        continue; /* OPEN PIN */
      }
    }
    /* Second run to backannoate parasitic OPIN net_num*/
    //for (ipin = 0; ipin < type->num_pins; ipin++) {
    //  class_id = type->pin_class[ipin];
    //  if (DRIVER == type->class_inf[class_id].type) {
    //    /* Find the pb net_num and update OPIN net_num */
    //    pin_global_rr_node_id = get_rr_node_index(x, y, OPIN, ipin, rr_node_indices);
    //    if (OPEN == local_rr_graph[ipin].net_num) {
    //      continue; /* bypass non-mapped OPIN */
    //    } 
    //    rr_node[pin_global_rr_node_id].net_num = vpack_to_clb_net_mapping[local_rr_graph[ipin].net_num];
    //    rr_node[pin_global_rr_node_id].vpack_net_num = vpack_to_clb_net_mapping[local_rr_graph[ipin].net_num];
    //  }
    //}
  }
 
  return;
}

void update_grid_pbs_post_route_rr_graph() {
  int ix, iy;
  t_type_ptr type = NULL;

  for (ix = 0; ix < (nx + 2); ix++) {
    for (iy = 0; iy < (ny + 2); iy++) {
      type = grid[ix][iy].type;
      if (NULL != type) {
        /* Backup the packing prev_node and prev_edge */
        update_one_grid_pack_prev_node_edge(ix, iy);
      }
    }
  }

  return;
}

/* SPEICAL: to assign parasitic nets, I modify the net_num in global
 * global routing nets to be vpack_net_num. The reason is some vpack nets 
 * are absorbed into CLBs during packing, therefore they are invisible in 
 * clb_nets. But indeed, they exist in global routing as parasitic nets.
 */
void update_one_grid_pb_pins_parasitic_nets(int x, int y) {
  int iblk, blk_id, ipin; 
  int pin_global_rr_node_id,class_id;
  t_type_ptr type = NULL;
  t_pb* pb = NULL;
  t_rr_node* local_rr_graph = NULL;

  /* Assert */
  assert((!(x < 0))&&(x < (nx + 2)));  
  assert((!(y < 0))&&(y < (ny + 2)));  

  type = grid[x][y].type;
  /* Bypass IO_TYPE*/
  if ((EMPTY_TYPE == type)||(IO_TYPE == type)) {
    return;
  }   
  for (iblk = 0; iblk < grid[x][y].usage; iblk++) {
    blk_id = grid[x][y].blocks[iblk];
    assert(block[blk_id].x == x);
    assert(block[blk_id].y == y);
    pb = block[blk_id].pb;
    assert(NULL != pb);
    local_rr_graph = pb->rr_graph; 
    for (ipin = 0; ipin < type->num_pins; ipin++) {
      class_id = type->pin_class[ipin];
      if (DRIVER == type->class_inf[class_id].type) {
        /* Find the pb net_num and update OPIN net_num */
        pin_global_rr_node_id = get_rr_node_index(x, y, OPIN, ipin, rr_node_indices);
        assert(local_rr_graph[ipin].vpack_net_num == local_rr_graph[ipin].net_num);
        if (OPEN == local_rr_graph[ipin].net_num) {
          assert(OPEN == local_rr_graph[ipin].vpack_net_num);
          rr_node[pin_global_rr_node_id].net_num = OPEN; 
          rr_node[pin_global_rr_node_id].vpack_net_num = OPEN; 
          continue; /* bypass non-mapped OPIN */
        } 
        assert(ipin == local_rr_graph[ipin].pb_graph_pin->pin_count_in_cluster);
        //rr_node[pin_global_rr_node_id].net_num = vpack_to_clb_net_mapping[local_rr_graph[ipin].net_num]; 
        rr_node[pin_global_rr_node_id].vpack_net_num = local_rr_graph[ipin].vpack_net_num;
      } else if (RECEIVER == type->class_inf[class_id].type) {
        /* Find the global rr_node net_num and update pb net_num */
        pin_global_rr_node_id = get_rr_node_index(x, y, IPIN, ipin, rr_node_indices);
        /* Get the index of Vpack net from global rr_node net_num (clb_net index)*/
        if (OPEN == rr_node[pin_global_rr_node_id].vpack_net_num) {
          local_rr_graph[ipin].net_num = OPEN;
          local_rr_graph[ipin].vpack_net_num = OPEN;
          continue; /* bypass non-mapped IPIN */
        }
        assert(ipin == local_rr_graph[ipin].pb_graph_pin->pin_count_in_cluster);
        local_rr_graph[ipin].net_num = rr_node[pin_global_rr_node_id].vpack_net_num;
        local_rr_graph[ipin].vpack_net_num = rr_node[pin_global_rr_node_id].vpack_net_num;
      } else {
        continue; /* OPEN PIN */
      }
    }
  }
 
  return;
}

void update_grid_pb_pins_parasitic_nets() {
  int ix, iy;
  t_type_ptr type = NULL;

  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      type = grid[ix][iy].type;
      if (NULL != type) {
        /* Backup the packing prev_node and prev_edge */
        update_one_grid_pb_pins_parasitic_nets(ix, iy);
      }
    }
  }

  return;
}

void spice_backannotate_vpr_post_route_info(boolean parasitic_net_estimation_off) {

  vpr_printf(TIO_MESSAGE_INFO, "Start backannotating post route information for SPICE modeling...\n");
  /* Give spice_name_tag for each pb*/
  vpr_printf(TIO_MESSAGE_INFO, "Generate SPICE name tags for pbs...\n");
  gen_spice_name_tags_all_pbs();
  /* Build previous node lists for each rr_node */
  vpr_printf(TIO_MESSAGE_INFO, "Building previous node list for all Routing Resource Nodes...\n");
  build_prev_node_list_rr_nodes(num_rr_nodes, rr_node);
  /* This function should go very first because it gives all the net_num */
  vpr_printf(TIO_MESSAGE_INFO,"Back annotating mapping information to global routing resource nodes...\n");
  back_annotate_rr_node_map_info();
  /* Update local_rr_graphs to match post-route results*/
  vpr_printf(TIO_MESSAGE_INFO, "Update CLB local routing graph to match post-route results...\n");
  update_grid_pbs_post_route_rr_graph();
  vpr_printf(TIO_MESSAGE_INFO,"Back annotating mapping information to local routing resource nodes...\n");
  back_annotate_pb_rr_node_map_info();

  /* Backannotate activity information, initialize the waveform information */
  vpr_printf(TIO_MESSAGE_INFO, "Backannoating local routing net (1st time: for output pins)...\n");
  backannotate_pb_rr_nodes_net_info();
  if (FALSE == parasitic_net_estimation_off) {
    vpr_printf(TIO_MESSAGE_INFO, "Update CLB pins parasitic nets (1st time: for output pins)...\n");
    update_grid_pb_pins_parasitic_nets();
    vpr_printf(TIO_MESSAGE_INFO, "Backannoating global routing net...\n");
    backannotate_rr_nodes_net_info();
    vpr_printf(TIO_MESSAGE_INFO, "Update CLB pins parasitic nets (2nd time: for input pins)...\n");
    update_grid_pb_pins_parasitic_nets();
  } else {
    vpr_printf(TIO_MESSAGE_WARNING, "Parasitic Net Estimation is turned off...Accuracy loss may be expected!\n");
  }
  vpr_printf(TIO_MESSAGE_INFO, "Backannoating local routing net (2nd time: for input pins)...\n");
  backannotate_pb_rr_nodes_net_info();

  vpr_printf(TIO_MESSAGE_INFO, "Backannoating Net activities...\n");
  backannotate_clb_nets_act_info();
  vpr_printf(TIO_MESSAGE_INFO, "Determine Net initial values...\n");
  backannotate_clb_nets_init_val();

  vpr_printf(TIO_MESSAGE_INFO, "Finish backannotating post route information for SPICE modeling.\n");

  return;
}
