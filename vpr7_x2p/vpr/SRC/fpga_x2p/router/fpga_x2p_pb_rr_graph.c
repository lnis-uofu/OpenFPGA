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
#include "route_common.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_lut_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_rr_graph_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_globals.h"

/* Count the number of rr_graph nodes that should be allocated
 *   (a) INPUT pins at the top-level pb_graph_node should be a local_rr_node and plus a SOURCE
 *   (b) CLOCK pins at the top-level pb_graph_node should be a local_rr_node and plus a SOURCE
 *   (c) OUTPUT pins at the top-level pb_graph_node should be a local_rr_node and plus a SINK
 *   (e) INPUT pins at a primitive pb_graph_node should be a local_rr_node and plus a SINK
 *   (f) CLOCK pins at a primitive pb_graph_node should be a local_rr_node and plus a SINK
 *   (g) OUTPUT pins at a primitive pb_graph_node should be a local_rr_node and plus a SOURCE
 *   (h) all the other pins should be a local_rr_node 
 */
int rec_count_rr_graph_nodes_for_phy_pb_graph_node(t_pb_graph_node* cur_pb_graph_node) {
  boolean is_top_pb_graph_node = (boolean) (NULL == cur_pb_graph_node->parent_pb_graph_node);
  boolean is_primitive_pb_graph_node = (boolean) (NULL != cur_pb_graph_node->pb_type->spice_model);
  int phy_mode_idx = -1;
  int cur_num_rr_nodes = 0;
  int ichild, ipb;

  /* Count in INPUT/OUTPUT/CLOCK pins as regular rr_nodes */
  cur_num_rr_nodes = count_pin_number_one_pb_graph_node(cur_pb_graph_node);

  /* check if this is a top pb_graph_node or a primitive node */
  if ((TRUE == is_top_pb_graph_node)||(TRUE == is_primitive_pb_graph_node)) {
    /* Count in INPUT/OUTPUT/CLOCK pins as SINK/SOURCE rr_nodes */
    cur_num_rr_nodes += cur_num_rr_nodes;
  }

  /* Return when this is a primitive node */
  if (TRUE == is_primitive_pb_graph_node) {
    return cur_num_rr_nodes;
  }

  /* Go recursively to the lower levels */
  /* Find the physical mode in the next level! */
  phy_mode_idx = find_pb_type_physical_mode_index(*(cur_pb_graph_node->pb_type));
  for (ichild = 0; ichild < cur_pb_graph_node->pb_type->modes[phy_mode_idx].num_pb_type_children; ichild++) {
    /* num_pb is the number of such pb_type in a physical mode*/
    for (ipb = 0; ipb < cur_pb_graph_node->pb_type->modes[phy_mode_idx].pb_type_children[ichild].num_pb; ipb++) {
      cur_num_rr_nodes += rec_count_rr_graph_nodes_for_phy_pb_graph_node(&cur_pb_graph_node->child_pb_graph_nodes[phy_mode_idx][ichild][ipb]);
    }
  }

  return cur_num_rr_nodes;
}

void init_one_rr_node_pack_cost_for_phy_graph_node(INP t_pb_graph_pin* cur_pb_graph_pin,
                                                   INOUTP t_rr_graph* local_rr_graph,
                                                   int cur_rr_node_index,
                                                   enum PORTS port_type) {
  switch (port_type) {
  case IN_PORT:
    /* Routing costs : INPUT pins 
     * need to normalize better than 5 and 10, bias router to use earlier inputs pins 
     */
    local_rr_graph->rr_node[cur_rr_node_index].pack_intrinsic_cost = 1 + 1 * (float) local_rr_graph->rr_node[cur_rr_node_index].num_edges / 5 
                                                                       + 1 * ((float)cur_pb_graph_pin->pin_number / (float)cur_pb_graph_pin->port->num_pins) / (float)10; 
    break;
  case OUT_PORT:
    /* Routing costs : OUTPUT pins  
     * need to normalize better than 5 
     */
    local_rr_graph->rr_node[cur_rr_node_index].pack_intrinsic_cost = 1 + 1 * (float) local_rr_graph->rr_node[cur_rr_node_index].num_edges / 5; 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid rr_node_type (%d)! \n",
               __FILE__, __LINE__, port_type); 
    exit(1);
  }
}


/* Override the fan-in and fan-out for a top/primitive pb_graph_node */
void override_one_rr_node_for_top_primitive_phy_pb_graph_node(INP t_pb_graph_pin* cur_pb_graph_pin,
                                                              INOUTP t_rr_graph* local_rr_graph,
                                                              int cur_rr_node_index,
                                                              boolean is_top_pb_graph_node,
                                                              boolean is_primitive_pb_graph_node) {
  /* check : must be either a top_pb_graph_node or a primitive_pb_graph_node */
  if ((FALSE == is_top_pb_graph_node) && (FALSE == is_primitive_pb_graph_node)) {
    return;
  }

  /* depends on the port type  */
  switch (cur_pb_graph_pin->port->type) {
  case IN_PORT:
    if (TRUE == is_top_pb_graph_node) {
      /* Top-level IN_PORT should only has 1 fan-in */
      local_rr_graph->rr_node[cur_rr_node_index].fan_in = 1;
      return;
    }
    if (TRUE == is_primitive_pb_graph_node) {
      /* Primitive-level IN_PORT should only has 1 output edge */
      local_rr_graph->rr_node[cur_rr_node_index].num_edges = 1;
      return;
    }
    break;
  case OUT_PORT:
    if (TRUE == is_top_pb_graph_node) {
      /* Top-level OUT_PORT should only has 1 output edge */
      local_rr_graph->rr_node[cur_rr_node_index].num_edges = 1;
      return;
    }
    if (TRUE == is_primitive_pb_graph_node) {
      /* Primitive-level OUT_PORT should only has 1 fan-in */
      local_rr_graph->rr_node[cur_rr_node_index].fan_in = 1;
      return;
    }
    break;    
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid rr_node_type (%d)! \n",
               __FILE__, __LINE__, cur_pb_graph_pin->port->type); 
    exit(1);
  }

  return;
}

/* initialize a rr_node in a rr_graph of phyical pb_graph_node */
void init_one_rr_node_for_phy_pb_graph_node(INP t_pb_graph_pin* cur_pb_graph_pin,
                                            INOUTP t_rr_graph* local_rr_graph,
                                            int cur_rr_node_index,
                                            int fan_in_phy_mode_index, 
                                            int fan_out_phy_mode_index, 
                                            t_rr_type rr_node_type,
                                            boolean is_top_pb_graph_node,
                                            boolean is_primitive_pb_graph_node) {

  switch (rr_node_type) {
  case INTRA_CLUSTER_EDGE: 
    /* Link the rr_node to the pb_graph_pin*/
    cur_pb_graph_pin->rr_node_index_physical_pb = cur_rr_node_index;
    local_rr_graph->rr_node[cur_rr_node_index].pb_graph_pin = cur_pb_graph_pin;
    /* Get the number of input edges that belong to physical mode only! */
    local_rr_graph->rr_node[cur_rr_node_index].fan_in = count_pb_graph_node_input_edge_in_phy_mode(cur_pb_graph_pin, fan_in_phy_mode_index);
    /* Get the number of output edges that belong to physical mode only! */
    local_rr_graph->rr_node[cur_rr_node_index].num_edges = count_pb_graph_node_output_edge_in_phy_mode(cur_pb_graph_pin, fan_out_phy_mode_index);
    /* Override for special rr_nodes : at top-level and primitive nodes */
    override_one_rr_node_for_top_primitive_phy_pb_graph_node(cur_pb_graph_pin, local_rr_graph, cur_rr_node_index, 
                                                             is_top_pb_graph_node, is_primitive_pb_graph_node);
    /* Routing costs */ 
    init_one_rr_node_pack_cost_for_phy_graph_node(cur_pb_graph_pin, local_rr_graph, cur_rr_node_index, cur_pb_graph_pin->port->type);
    break;
  case SOURCE:
    /* SOURCE only has one output and zero inputs */
    local_rr_graph->rr_node[cur_rr_node_index].pb_graph_pin = cur_pb_graph_pin;
    local_rr_graph->rr_node[cur_rr_node_index].fan_in = 0;
    local_rr_graph->rr_node[cur_rr_node_index].num_edges = 1;
    /* Routing costs : SOURCE */
    init_one_rr_node_pack_cost_for_phy_graph_node(cur_pb_graph_pin, local_rr_graph, cur_rr_node_index, OUT_PORT);
    break;
  case SINK:
    /* SINK only has one input and zero outputs */
    local_rr_graph->rr_node[cur_rr_node_index].pb_graph_pin = cur_pb_graph_pin;
    local_rr_graph->rr_node[cur_rr_node_index].fan_in = 1;
    local_rr_graph->rr_node[cur_rr_node_index].num_edges = 0;
    /* Routing costs : SINK */
    local_rr_graph->rr_node[cur_rr_node_index].pack_intrinsic_cost = 1;
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid rr_node_type (%d)! \n",
               __FILE__, __LINE__, rr_node_type); 
    exit(1);
  }

  /* Routing connectivity */
  local_rr_graph->rr_node[cur_rr_node_index].edges = (int *) my_calloc(local_rr_graph->rr_node[cur_rr_node_index].num_edges, sizeof(int));
  local_rr_graph->rr_node[cur_rr_node_index].switches = (short *) my_calloc(local_rr_graph->rr_node[cur_rr_node_index].num_edges,  sizeof(short));
  local_rr_graph->rr_node[cur_rr_node_index].net_num = OPEN;
  local_rr_graph->rr_node[cur_rr_node_index].prev_node = OPEN;
  local_rr_graph->rr_node[cur_rr_node_index].prev_edge = OPEN;

  local_rr_graph->rr_node[cur_rr_node_index].capacity = 1;
  local_rr_graph->rr_node[cur_rr_node_index].type = rr_node_type;
  
  return;
} 

/* Connect a rr_node in a rr_graph of phyical pb_graph_node,
 * Assign the edges and switches 
 */
void connect_one_rr_node_for_phy_pb_graph_node(INP t_pb_graph_pin* cur_pb_graph_pin,
                                               INOUTP t_rr_graph* local_rr_graph,
                                               int cur_rr_node_index,
                                               int phy_mode_index, 
                                               t_rr_type rr_node_type) {
  int iedge;

  /* Check: if type matches! */
  assert(rr_node_type == local_rr_graph->rr_node[cur_rr_node_index].type);

  switch (rr_node_type) {
  case INTRA_CLUSTER_EDGE: 
    /* Check out all the output_edges belonging to the same physical mode */
    for (iedge = 0; iedge < cur_pb_graph_pin->num_output_edges; iedge++) {
      check_pb_graph_edge(*(cur_pb_graph_pin->output_edges[iedge]));
      if (phy_mode_index == cur_pb_graph_pin->output_edges[iedge]->interconnect->parent_mode_index) {
        local_rr_graph->rr_node[cur_rr_node_index].edges[iedge] = cur_pb_graph_pin->output_edges[iedge]->output_pins[0]->rr_node_index_physical_pb;   
        local_rr_graph->rr_node[cur_rr_node_index].switches[iedge] = local_rr_graph->delayless_switch_index;   
      }
    }
    break;
  case SOURCE:
    /* Connect the SOURCE nodes to the rr_node of cur_pb_graph_pin */
    assert (0 == local_rr_graph->rr_node[cur_rr_node_index].fan_in);
    assert (1 == local_rr_graph->rr_node[cur_rr_node_index].num_edges);
    local_rr_graph->rr_node[cur_rr_node_index].edges[0] = cur_pb_graph_pin->rr_node_index_physical_pb;
    local_rr_graph->rr_node[cur_rr_node_index].switches[0] = local_rr_graph->delayless_switch_index;
    /* Fix the connection */
    local_rr_graph->rr_node[cur_pb_graph_pin->rr_node_index_physical_pb].prev_node = cur_rr_node_index;
    local_rr_graph->rr_node[cur_pb_graph_pin->rr_node_index_physical_pb].prev_edge = 0;
    break;
  case SINK:
    /* Connect the rr_node of cur_pb_graph_pin to the SINK */
    assert (1 == local_rr_graph->rr_node[cur_rr_node_index].fan_in);
    assert (0 == local_rr_graph->rr_node[cur_rr_node_index].num_edges);
    assert (1 == local_rr_graph->rr_node[cur_pb_graph_pin->rr_node_index_physical_pb].num_edges);
    local_rr_graph->rr_node[cur_pb_graph_pin->rr_node_index_physical_pb].edges[0] = cur_rr_node_index;
    local_rr_graph->rr_node[cur_pb_graph_pin->rr_node_index_physical_pb].switches[0] = local_rr_graph->delayless_switch_index;
    /* Fix the connection */
    local_rr_graph->rr_node[cur_rr_node_index].prev_node = cur_pb_graph_pin->rr_node_index_physical_pb;
    local_rr_graph->rr_node[cur_rr_node_index].prev_edge = 0;
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid rr_node_type (%d)! \n",
               __FILE__, __LINE__, rr_node_type); 
    exit(1);
  }
  
  return;
} 


/* Recursively configure all the rr_nodes in the rr_graph
 * Initialize the routing cost, fan-in rr_nodes and fan-out rr_nodes, and switches  
 */
void rec_init_rr_graph_for_phy_pb_graph_node(INP t_pb_graph_node* cur_pb_graph_node, 
                                            INOUTP t_rr_graph* local_rr_graph,
                                            int* cur_rr_node_index) {
  boolean is_top_pb_graph_node = (boolean) (NULL == cur_pb_graph_node->parent_pb_graph_node);
  boolean is_primitive_pb_graph_node = (boolean) (NULL != cur_pb_graph_node->pb_type->spice_model);
  int phy_mode_idx = -1;
  int parent_phy_mode_idx = -1;
  int iport, ipin;
  int ichild, ipb;

  /* Find the physical mode in the next level! */
  phy_mode_idx = find_pb_type_physical_mode_index(*(cur_pb_graph_node->pb_type));
  /* There is no parent mode index for top-level node */
  if (FALSE == is_top_pb_graph_node) { 
    parent_phy_mode_idx = find_pb_type_physical_mode_index(*(cur_pb_graph_node->parent_pb_graph_node->pb_type));
  } else {
    parent_phy_mode_idx = phy_mode_idx;
  }

  /* Configure rr_nodes with the information of pb_graph_pin  */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->input_pins[iport][ipin], local_rr_graph,  
                                             *cur_rr_node_index, parent_phy_mode_idx, phy_mode_idx, INTRA_CLUSTER_EDGE,
                                             is_top_pb_graph_node, is_primitive_pb_graph_node);
      (*cur_rr_node_index)++;
    }
  }

  /* Importantly, the interconnection for output ports belong to the parent pb_graph_node */
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->output_pins[iport][ipin], local_rr_graph,  
                                             *cur_rr_node_index, phy_mode_idx, parent_phy_mode_idx, INTRA_CLUSTER_EDGE,
                                             is_top_pb_graph_node, is_primitive_pb_graph_node);
      (*cur_rr_node_index)++;
    }
  }

  for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
      init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->clock_pins[iport][ipin], local_rr_graph,  
                                             *cur_rr_node_index, parent_phy_mode_idx, phy_mode_idx, INTRA_CLUSTER_EDGE,
                                             is_top_pb_graph_node, is_primitive_pb_graph_node);
      (*cur_rr_node_index)++;
    }
  }

  /* check if this is a top pb_graph_node */
  if ((TRUE == is_top_pb_graph_node)) {
    /* Configure SOURCE and SINK rr_node: 
     * input_pins should have a SOURCE node
     */
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->input_pins[iport][ipin], local_rr_graph,  
                                               *cur_rr_node_index, phy_mode_idx, phy_mode_idx, SOURCE,
                                               is_top_pb_graph_node, is_primitive_pb_graph_node);

        (*cur_rr_node_index)++;
      }
    }
    /* Configure SOURCE and SINK rr_node: 
     * clock_pins should have a SOURCE node
     */
    for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
        init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->clock_pins[iport][ipin], local_rr_graph,  
                                               *cur_rr_node_index, phy_mode_idx, phy_mode_idx, SOURCE,
                                               is_top_pb_graph_node, is_primitive_pb_graph_node);

        (*cur_rr_node_index)++;
      }
    }

    /* Configure SOURCE and SINK rr_node: 
     * output_pins should have a SINK node 
     */
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->output_pins[iport][ipin], local_rr_graph,  
                                               *cur_rr_node_index, phy_mode_idx, phy_mode_idx, SINK,
                                               is_top_pb_graph_node, is_primitive_pb_graph_node);

        (*cur_rr_node_index)++;
      }
    }
    /* Finish adding SOURCE and SINKs */
  }

  /* Return when this is a primitive node */
  if (TRUE == is_primitive_pb_graph_node) {
    /* Configure SOURCE and SINK rr_node: 
     * output_pins should have a SOURCE node
     */
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->output_pins[iport][ipin], local_rr_graph,  
                                               *cur_rr_node_index, phy_mode_idx, parent_phy_mode_idx,  SOURCE,
                                               is_top_pb_graph_node, is_primitive_pb_graph_node);

        (*cur_rr_node_index)++;
      }
    }
    /* Configure SOURCE and SINK rr_node: 
     * input_pins should have a SINK node 
     */
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->input_pins[iport][ipin], local_rr_graph,  
                                               *cur_rr_node_index, parent_phy_mode_idx, phy_mode_idx, SINK,
                                               is_top_pb_graph_node, is_primitive_pb_graph_node);

        (*cur_rr_node_index)++;
      }
    }
    /* Configure SOURCE and SINK rr_node: 
     * clock_pins should have a SINK node 
     */
    for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
        init_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->clock_pins[iport][ipin], local_rr_graph,  
                                               *cur_rr_node_index, parent_phy_mode_idx, phy_mode_idx, SINK,
                                               is_top_pb_graph_node, is_primitive_pb_graph_node);

        (*cur_rr_node_index)++;
      }
    }
    /* Finish adding SOURCE and SINKs */

    return;
  }

  /* Go recursively to the lower levels */
  for (ichild = 0; ichild < cur_pb_graph_node->pb_type->modes[phy_mode_idx].num_pb_type_children; ichild++) {
    /* num_pb is the number of such pb_type in a physical mode*/
    for (ipb = 0; ipb < cur_pb_graph_node->pb_type->modes[phy_mode_idx].pb_type_children[ichild].num_pb; ipb++) {
      rec_init_rr_graph_for_phy_pb_graph_node(&cur_pb_graph_node->child_pb_graph_nodes[phy_mode_idx][ichild][ipb],
                                              local_rr_graph, cur_rr_node_index);
    }
  }

  return;
}

/* Recursively connect all the rr_nodes in the rr_graph
 * output_edges, output_switches 
 */
void rec_connect_rr_graph_for_phy_pb_graph_node(INP t_pb_graph_node* cur_pb_graph_node, 
                                                INOUTP t_rr_graph* local_rr_graph,
                                                int* cur_rr_node_index) {
  boolean is_top_pb_graph_node = (boolean) (NULL == cur_pb_graph_node->parent_pb_graph_node);
  boolean is_primitive_pb_graph_node = (boolean) (NULL != cur_pb_graph_node->pb_type->spice_model);
  int phy_mode_idx = -1;
  int parent_phy_mode_idx = -1;
  int iport, ipin;
  int ichild, ipb;

  /* Find the physical mode in the next level! */
  phy_mode_idx = find_pb_type_physical_mode_index(*(cur_pb_graph_node->pb_type));
  /* There is no parent mode index for top-level node */
  if (FALSE == is_top_pb_graph_node) { 
    parent_phy_mode_idx = find_pb_type_physical_mode_index(*(cur_pb_graph_node->parent_pb_graph_node->pb_type));
  } else {
    parent_phy_mode_idx = phy_mode_idx;
  }

  /* Configure rr_nodes with the information of pb_graph_pin  */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->input_pins[iport][ipin], local_rr_graph,  
                                                *cur_rr_node_index, phy_mode_idx, INTRA_CLUSTER_EDGE);
      (*cur_rr_node_index)++;
    }
  }

  /* Importantly, the interconnection for output ports belong to the parent pb_graph_node */
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->output_pins[iport][ipin], local_rr_graph,  
                                                *cur_rr_node_index, parent_phy_mode_idx, INTRA_CLUSTER_EDGE);
      (*cur_rr_node_index)++;
    }
  }

  for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
      connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->clock_pins[iport][ipin], local_rr_graph,  
                                                *cur_rr_node_index, phy_mode_idx, INTRA_CLUSTER_EDGE);
      (*cur_rr_node_index)++;
    }
  }

  /* check if this is a top pb_graph_node */
  if ((TRUE == is_top_pb_graph_node)) {
    /* Configure SOURCE and SINK rr_node: 
     * input_pins should have a SOURCE node
     */
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->input_pins[iport][ipin], local_rr_graph,  
                                                  *cur_rr_node_index, phy_mode_idx, SOURCE);

        (*cur_rr_node_index)++;
      }
    }
    /* Configure SOURCE and SINK rr_node: 
     * clock_pins should have a SOURCE node
     */
    for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
        connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->clock_pins[iport][ipin], local_rr_graph,  
                                                  *cur_rr_node_index, phy_mode_idx, SOURCE);

        (*cur_rr_node_index)++;
      }
    }
    /* Configure SOURCE and SINK rr_node: 
     * output_pins should have a SINK node 
     */
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->output_pins[iport][ipin], local_rr_graph,  
                                                  *cur_rr_node_index, parent_phy_mode_idx, SINK);

        (*cur_rr_node_index)++;
      }
    }
    /* Finish adding SOURCE and SINKs */
  }

  /* Return when this is a primitive node */
  if (TRUE == is_primitive_pb_graph_node) {
    /* Configure SOURCE and SINK rr_node: 
     * output_pins should have a SOURCE node
     */
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->output_pins[iport][ipin], local_rr_graph,  
                                                  *cur_rr_node_index, parent_phy_mode_idx, SOURCE);

        (*cur_rr_node_index)++;
      }
    }
    /* Configure SOURCE and SINK rr_node: 
     * input_pins should have a SINK node 
     */
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->input_pins[iport][ipin], local_rr_graph,  
                                                  *cur_rr_node_index, phy_mode_idx, SINK);

        (*cur_rr_node_index)++;
      }
    }
    /* Configure SOURCE and SINK rr_node: 
     * clock_pins should have a SINK node 
     */
    for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
        connect_one_rr_node_for_phy_pb_graph_node(&cur_pb_graph_node->clock_pins[iport][ipin], local_rr_graph,  
                                                  *cur_rr_node_index, phy_mode_idx, SINK);

        (*cur_rr_node_index)++;
      }
    }
    /* Finish adding SOURCE and SINKs */

    return;
  }

  /* Go recursively to the lower levels */
  for (ichild = 0; ichild < cur_pb_graph_node->pb_type->modes[phy_mode_idx].num_pb_type_children; ichild++) {
    /* num_pb is the number of such pb_type in a physical mode*/
    for (ipb = 0; ipb < cur_pb_graph_node->pb_type->modes[phy_mode_idx].pb_type_children[ichild].num_pb; ipb++) {
      rec_connect_rr_graph_for_phy_pb_graph_node(&cur_pb_graph_node->child_pb_graph_nodes[phy_mode_idx][ichild][ipb],
                                                 local_rr_graph, cur_rr_node_index);
    }
  }

  return;
}

/* Allocate a rr_graph for a given pb_graph node 
 * This is function is a copy of alloc_and_load_rr_graph_for_pb_graph_node
 * The major difference lies in removing the use of global variables 
 * This function does the following tasks:
 * 1. Count all the pins in pb_graph_node (only those in physical modes) that can be a rr_node in the local rr_graph
 *   (a) INPUT pins at the top-level pb_graph_node should be a local_rr_node and plus a SOURCE
 *   (b) CLOCK pins at the top-level pb_graph_node should be a local_rr_node and plus a SOURCE
 *   (c) OUTPUT pins at the top-level pb_graph_node should be a local_rr_node and plus a SINK
 *   (e) INPUT pins at a primitive pb_graph_node should be a local_rr_node and plus a SINK
 *   (f) CLOCK pins at a primitive pb_graph_node should be a local_rr_node and plus a SINK
 *   (g) OUTPUT pins at a primitive pb_graph_node should be a local_rr_node and plus a SOURCE
 *   (h) all the other pins should be a local_rr_node 
 * 2. Allocate the rr_graph and initialize its properties according to the pb_graph_pin interconnections
 *   capacity, mapped net name,  edges, switches, routing cost information
 * 3. Synchronize mapped_net_name from a mapped pb:
 *   (a) give mapped_net_name to INPUT/OUTPUT/CLOCK pins of the top-level pb_graph_node
 *   (b) give mapped_net_name to INPUT/OUTPUT/CLOCK pins of the primitive pb_graph_nodes
 */
void alloc_and_load_rr_graph_for_phy_pb_graph_node(INP t_pb_graph_node* top_pb_graph_node, 
                                                  OUTP t_rr_graph* local_rr_graph) {

  int phy_pb_num_rr_nodes, check_point;

  /* Count the number of rr_nodes that are required */
  phy_pb_num_rr_nodes = rec_count_rr_graph_nodes_for_phy_pb_graph_node(top_pb_graph_node);

  /* Allocate rr_graph */
  alloc_and_load_rr_graph_rr_node(local_rr_graph, phy_pb_num_rr_nodes);

  /* Fill basic information for the rr_graph  */ 
  check_point = 0;
  rec_init_rr_graph_for_phy_pb_graph_node(top_pb_graph_node, local_rr_graph, &check_point);
  assert (check_point == local_rr_graph->num_rr_nodes); 

  /* Fill edges and switches for the rr_graph */
  check_point = 0;
  rec_connect_rr_graph_for_phy_pb_graph_node(top_pb_graph_node, local_rr_graph, &check_point);
  assert (check_point == local_rr_graph->num_rr_nodes); 

  return;
}

/* Check the vpack_net_num of a rr_node mapped to a pb_graph_pin and 
 * mark the used vpack_net_num in the list
 */
void mark_vpack_net_used_in_pb_pin(t_pb* cur_op_pb, t_pb_graph_pin* cur_pb_graph_pin,
                                   int L_num_vpack_nets, boolean* vpack_net_used_in_pb) {
  int inode;

  assert (NULL != cur_pb_graph_pin);

  inode = cur_pb_graph_pin->pin_count_in_cluster;

  /* bypass unmapped rr_node */
  if (OPEN == cur_op_pb->rr_graph[inode].vpack_net_num) {
    return;
  }
  /* Reach here, it means this net is used in this pb */
  assert(   (-1 < cur_op_pb->rr_graph[inode].vpack_net_num)
         && ( cur_op_pb->rr_graph[inode].vpack_net_num < L_num_vpack_nets));
  vpack_net_used_in_pb[cur_op_pb->rr_graph[inode].vpack_net_num] = TRUE; 

  return;
}

/* Recursively visit all the child pbs and 
 * mark the used vpack_net_num in the list
 */
void mark_vpack_net_used_in_pb(t_pb* cur_op_pb,
                               int L_num_vpack_nets, boolean* vpack_net_used_in_pb) {
  int mode_index, ipb, jpb; 
  int iport, ipin;
  t_pb_type* cur_pb_type = NULL;

  cur_pb_type = cur_op_pb->pb_graph_node->pb_type;
  mode_index = cur_op_pb->mode; 
  
  /* Mark all the nets at this level */
  for (iport = 0; iport < cur_op_pb->pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_op_pb->pb_graph_node->num_input_pins[iport]; ipin++) {
      mark_vpack_net_used_in_pb_pin(cur_op_pb, &(cur_op_pb->pb_graph_node->input_pins[iport][ipin]),
                                    L_num_vpack_nets, vpack_net_used_in_pb);
    }
  }

  for (iport = 0; iport < cur_op_pb->pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_op_pb->pb_graph_node->num_output_pins[iport]; ipin++) {
      mark_vpack_net_used_in_pb_pin(cur_op_pb, &(cur_op_pb->pb_graph_node->output_pins[iport][ipin]),
                                    L_num_vpack_nets, vpack_net_used_in_pb);
    }
  }

  for (iport = 0; iport < cur_op_pb->pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_op_pb->pb_graph_node->num_clock_pins[iport]; ipin++) {
      mark_vpack_net_used_in_pb_pin(cur_op_pb, &(cur_op_pb->pb_graph_node->clock_pins[iport][ipin]),
                                    L_num_vpack_nets, vpack_net_used_in_pb);
    }
  }

  /* recursive for the child_pbs*/
  if (FALSE == is_primitive_pb_type(cur_pb_type)) { 
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Recursive*/
        /* Refer to pack/output_clustering.c [LINE 392] */
        if ((NULL != cur_op_pb->child_pbs[ipb])&&(NULL != cur_op_pb->child_pbs[ipb][jpb].name)) {
          mark_vpack_net_used_in_pb(&(cur_op_pb->child_pbs[ipb][jpb]), 
                                    L_num_vpack_nets, vpack_net_used_in_pb); 
        }
      }
    }
  }

  return;
}

/* Find the vpack_nets used by this pb
 * And allocate an array for those nets and load it to pb_rr_graph
 */
void alloc_and_load_phy_pb_rr_graph_nets(INP t_pb* cur_op_pb,
                                         t_rr_graph* local_rr_graph,
                                         int L_num_vpack_nets, t_net* L_vpack_net) {
  /* Create an array labeling which vpack_net is used in this pb */
  int num_vpack_net_used_in_pb = 0;
  boolean* vpack_net_used_in_pb = NULL; 
  int inet, net_index;
  
  /* Allocate */ 
  vpack_net_used_in_pb = (boolean*)my_malloc(sizeof(boolean) * L_num_vpack_nets);
  /* Initial to FALSE */
  for (inet = 0; inet < L_num_vpack_nets; inet++) { 
    vpack_net_used_in_pb[inet] = FALSE;
  }

  /* Build vpack_net_used_in_pb */
  mark_vpack_net_used_in_pb(cur_op_pb, L_num_vpack_nets, vpack_net_used_in_pb);

  /* Count the number of vpack_net used in this pb  */
  num_vpack_net_used_in_pb = 0;
  for (inet = 0; inet < L_num_vpack_nets; inet++) { 
    if (TRUE == vpack_net_used_in_pb[inet]) {
      num_vpack_net_used_in_pb++;
    } 
  }

  /* Allocate net for rr_graph */
  local_rr_graph->num_nets = num_vpack_net_used_in_pb;
  local_rr_graph->net = (t_net**) my_malloc(sizeof(t_net*) * local_rr_graph->num_nets);
  local_rr_graph->net_to_vpack_net_mapping = (int*) my_malloc(sizeof(int) * local_rr_graph->num_nets);
  
  /* Fill the net array and net_to_net_mapping */
  net_index = 0;
  for (inet = 0; inet < L_num_vpack_nets; inet++) { 
    if (TRUE == vpack_net_used_in_pb[inet]) {
      local_rr_graph->net[net_index] = &L_vpack_net[inet]; 
      local_rr_graph->net_to_vpack_net_mapping[net_index] = inet; 
      net_index++;
    }
  }
  assert( net_index == local_rr_graph->num_nets );

  return;
}

/* Find the rr_node in the primitive node of a pb_rr_graph*/ 
void sync_pb_graph_pin_vpack_net_num_to_phy_pb(t_rr_node* cur_op_pb_rr_graph, 
                                               t_pb_graph_pin* cur_pb_graph_pin,
                                               t_rr_graph* local_rr_graph) {
  int inode, jnode, iedge, next_node;
  int rr_node_net_num;

  inode = cur_pb_graph_pin->pin_count_in_cluster;
  /* bypass non-exist physical pb_graph_pins */
  if (NULL == cur_pb_graph_pin->physical_pb_graph_pin) {
    return;
  }
  jnode = cur_pb_graph_pin->physical_pb_graph_pin->rr_node_index_physical_pb; 
   
  /* If we have a valid vpack_net_num */
  if (OPEN == cur_op_pb_rr_graph[inode].vpack_net_num) {
    /* Do not overwrite because this rr_node may have been updated! */
    /*  
    local_rr_graph->rr_node[jnode].net_num = OPEN;
    local_rr_graph->rr_node[jnode].vpack_net_num = OPEN;
    */
    return;
  }

  /* Synchronize depending on the rr_node type */
  switch (local_rr_graph->rr_node[jnode].type) {
  case SOURCE:
  case SINK:
    /* SOURCE or SINK: we are done, just synchronize the vpack_net_num and we can return*/
    rr_node_net_num = get_rr_graph_net_index_with_vpack_net(local_rr_graph, cur_op_pb_rr_graph[inode].vpack_net_num);
    assert (( -1 < rr_node_net_num ) && (rr_node_net_num < local_rr_graph->num_nets));
    local_rr_graph->rr_node[jnode].net_num = rr_node_net_num;
    local_rr_graph->rr_node[jnode].vpack_net_num = cur_op_pb_rr_graph[inode].vpack_net_num;
    break;
  case INTRA_CLUSTER_EDGE:
    /* We need to find a SOURCE or a SINK nodes! */
    /* Check driving rr_nodes */
    for (iedge = 0; iedge < local_rr_graph->rr_node[jnode].num_drive_rr_nodes; iedge++) {
      if (SOURCE != local_rr_graph->rr_node[jnode].drive_rr_nodes[iedge]->type) {
        continue;
      }
      /* Give the vpack_net_num to the SOURCE nodes */
      rr_node_net_num = get_rr_graph_net_index_with_vpack_net(local_rr_graph, cur_op_pb_rr_graph[inode].vpack_net_num);
      assert (( -1 < rr_node_net_num ) && (rr_node_net_num < local_rr_graph->num_nets));
      local_rr_graph->rr_node[jnode].drive_rr_nodes[iedge]->net_num = rr_node_net_num;
      local_rr_graph->rr_node[jnode].drive_rr_nodes[iedge]->vpack_net_num = cur_op_pb_rr_graph[inode].vpack_net_num;
    }
    /* Check the output rr_nodes */
    for (iedge = 0; iedge < local_rr_graph->rr_node[jnode].num_edges; iedge++) {
      next_node = local_rr_graph->rr_node[jnode].edges[iedge];
      if (SINK != local_rr_graph->rr_node[next_node].type) {
        continue;
      }
      /* Give the vpack_net_num to the SOURCE nodes */
      rr_node_net_num = get_rr_graph_net_index_with_vpack_net(local_rr_graph, cur_op_pb_rr_graph[inode].vpack_net_num);
      assert (( -1 < rr_node_net_num ) && (rr_node_net_num < local_rr_graph->num_nets));
      local_rr_graph->rr_node[next_node].net_num = rr_node_net_num;
      local_rr_graph->rr_node[next_node].vpack_net_num = cur_op_pb_rr_graph[inode].vpack_net_num;
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid rr_node_type (%d)! \n",
               __FILE__, __LINE__, local_rr_graph->rr_node[jnode].type); 
    exit(1);
  }

  return;
} 

void rec_sync_wired_pb_vpack_net_num_to_phy_pb_rr_graph(t_pb_graph_node* cur_pb_graph_node,
                                                        t_rr_node* op_pb_rr_graph,
                                                        t_rr_graph* local_rr_graph) {
  int imode, ipb, jpb;
  int ipin, iport;
  t_pb_type* cur_pb_type = cur_pb_graph_node->pb_type;

  /* Copy LUT information if this is a leaf node */
  if ((TRUE == is_primitive_pb_type(cur_pb_type)) 
    && (LUT_CLASS == cur_pb_type->class_type)) {
    /* Mark all the nets at this level */
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        sync_pb_graph_pin_vpack_net_num_to_phy_pb(op_pb_rr_graph, 
                                                  &(cur_pb_graph_node->input_pins[iport][ipin]),
                                                  local_rr_graph);
      }
    }
   
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        sync_pb_graph_pin_vpack_net_num_to_phy_pb(op_pb_rr_graph, 
                                                  &(cur_pb_graph_node->output_pins[iport][ipin]),
                                                  local_rr_graph);
      }
    }
   
    for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
        sync_pb_graph_pin_vpack_net_num_to_phy_pb(op_pb_rr_graph, 
                                                  &(cur_pb_graph_node->clock_pins[iport][ipin]),
                                                  local_rr_graph);
      }
    }
  
    /* Finish here */
    return;
  }

  /* Go recursively */
  assert (FALSE == is_primitive_pb_type(cur_pb_type));
  for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
    for (ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[imode].pb_type_children[ipb].num_pb; jpb++) {
        /* We care only those have been used for wiring */
        if (FALSE == is_pb_used_for_wiring(&(cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]),
                                           &(cur_pb_type->modes[imode].pb_type_children[ipb]), 
                                           op_pb_rr_graph)) {
          continue;
        }
        rec_sync_wired_pb_vpack_net_num_to_phy_pb_rr_graph(&(cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]),
                                                           op_pb_rr_graph,
                                                           local_rr_graph);
      }
    }
  }

  return;
}

/* Recursively visit all the child pbs and 
 * synchronize the vpack_net_num of the top-level/primitive pb_graph_pin
 * to the physical pb rr_node nodes 
 */
void rec_sync_pb_vpack_net_num_to_phy_pb_rr_graph(t_pb* cur_op_pb,
                                                  t_rr_graph* local_rr_graph) {
  int mode_index, ipb, jpb; 
  int iport, ipin;
  t_pb_type* cur_pb_type = NULL;

  cur_pb_type = cur_op_pb->pb_graph_node->pb_type;
  mode_index = cur_op_pb->mode;
  
  /* Mark all the nets at this level */
  for (iport = 0; iport < cur_op_pb->pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_op_pb->pb_graph_node->num_input_pins[iport]; ipin++) {
      sync_pb_graph_pin_vpack_net_num_to_phy_pb(cur_op_pb->rr_graph, 
                                                &(cur_op_pb->pb_graph_node->input_pins[iport][ipin]),
                                                local_rr_graph);
    }
  }
 
  for (iport = 0; iport < cur_op_pb->pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_op_pb->pb_graph_node->num_output_pins[iport]; ipin++) {
      sync_pb_graph_pin_vpack_net_num_to_phy_pb(cur_op_pb->rr_graph, 
                                                &(cur_op_pb->pb_graph_node->output_pins[iport][ipin]),
                                                local_rr_graph);
    }
  }
 
  for (iport = 0; iport < cur_op_pb->pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_op_pb->pb_graph_node->num_clock_pins[iport]; ipin++) {
      sync_pb_graph_pin_vpack_net_num_to_phy_pb(cur_op_pb->rr_graph, 
                                                &(cur_op_pb->pb_graph_node->clock_pins[iport][ipin]),
                                                local_rr_graph);
    }
  }
   
  /* Return if we reach the primitive  */
  if (TRUE == is_primitive_pb_type(cur_pb_type)) { 
    return;
  }
  
  /* recursive for the child_pbs*/
  assert (FALSE == is_primitive_pb_type(cur_pb_type));
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_op_pb->child_pbs[ipb])&&(NULL != cur_op_pb->child_pbs[ipb][jpb].name)) {
        rec_sync_pb_vpack_net_num_to_phy_pb_rr_graph(&(cur_op_pb->child_pbs[ipb][jpb]), 
                                                     local_rr_graph);
      } else if (TRUE == is_pb_used_for_wiring(&(cur_op_pb->pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]),
                                               &(cur_pb_type->modes[mode_index].pb_type_children[ipb]), 
                                               cur_op_pb->rr_graph)) {
        /* Identify pbs and LUTs that are used for wiring purpose */
        rec_sync_wired_pb_vpack_net_num_to_phy_pb_rr_graph(&(cur_op_pb->pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]),
                                                           cur_op_pb->rr_graph,
                                                           local_rr_graph);
      }
    }
  }

  return;
}


/* Load mapping information from an op_pb to the net_rr_terminals of a phy_pb rr_graph 
 * This function should do the following tasks:
 * 1. Find mapped pb_graph_pin in the rr_graph of cur_op_pb 
 * 2. Locate the pin-to-pin annotation in the pb_graph_pin of cur_op_pb
 *    and find the corresponding pb_graph_pin in local_rr_graph (phy_pb)
 * 3. Find the SOURCE and SINK rr_nodes related to the pb_graph_pin 
 * 4. Configure the net_rr_terminals with the SINK/SOURCE rr_nodes  
 */
void alloc_and_load_phy_pb_rr_graph_net_rr_terminals(INP t_pb* cur_op_pb,
                                                     t_rr_graph* local_rr_graph) {
  int inet, inode, rr_node_net_name;
  int* net_cur_terminal = (int*) my_calloc(local_rr_graph->num_nets, sizeof(int));
  int* net_cur_source = (int*) my_calloc(local_rr_graph->num_nets, sizeof(int));
  int* net_cur_sink = (int*) my_calloc(local_rr_graph->num_nets, sizeof(int));

  /* Initialize */
  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    /* SINK index starts from 1!!!*/
    net_cur_terminal[inet] = 1;
    net_cur_source[inet] = 0;
    net_cur_sink[inet] = 0;
  }
  
  /* Check each net in the local_rr_graph,
   * Find the routing resource node in the pb_rr_graph of the cur_op_pb
   * and annotate in the local_rr_graph
   * assign the net_rr_terminal with the node index in the local_rr_graph
   * Two steps to go:
   * 1. synchronize the vpack_net_num of pb_graph_pin (rr_node) in op_pb
   *    to the local rr_graph (SINK and SOURCE nodes!)
   * 2. Annotate the net_rr_terminals by sweeping the rr_node in local_rr_graph
   */ 
  rec_sync_pb_vpack_net_num_to_phy_pb_rr_graph(cur_op_pb,
                                               local_rr_graph);

  /* Allocate net_rr_terminals */
  alloc_rr_graph_net_rr_terminals(local_rr_graph);
  /* Some nets may have two sources nodes to route
   * We store the sources node in the net_rr_sources list 
   * We keep a list for the sink nodes
   * in the net_rr_sinks list 
   */
  alloc_rr_graph_net_rr_sources_and_sinks(local_rr_graph);

  for (inode = 0; inode < local_rr_graph->num_rr_nodes; inode++) {
    /* We only care the SOURCE and SINK nodes */
    switch (local_rr_graph->rr_node[inode].type) {
    case SOURCE:
      /* SOURCE is easy: give the rr_node id to first terminal of the vpack_net */
      rr_node_net_name = local_rr_graph->rr_node[inode].net_num;
      if (OPEN == rr_node_net_name) {
        break;
      }
      local_rr_graph->net_rr_terminals[rr_node_net_name][0] = inode; 
      /* Store in the source and sink lists */
      local_rr_graph->net_rr_sources[rr_node_net_name][net_cur_source[rr_node_net_name]] = inode; 
      /* Update the counter */
      net_cur_source[rr_node_net_name]++;
      break;
    case SINK:
      /* SINK: we need to record the sink we considered */
      rr_node_net_name = local_rr_graph->rr_node[inode].net_num;
      if (OPEN == rr_node_net_name) {
        break;
      }
      /* Make sure we do not overwrite on the source */
      assert ( 0 < net_cur_terminal[rr_node_net_name] );
      local_rr_graph->net_rr_terminals[rr_node_net_name][net_cur_terminal[rr_node_net_name]] = inode; 
      net_cur_terminal[rr_node_net_name]++;
      /* Store in the source and sink lists */
      local_rr_graph->net_rr_sinks[rr_node_net_name][net_cur_sink[rr_node_net_name]] = inode; 
      /* Update the counter */
      net_cur_sink[rr_node_net_name]++;
      break;
    case INTRA_CLUSTER_EDGE:
      /* Nothing to do */
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d]) Invalid rr_node_type (%d)! \n",
                 __FILE__, __LINE__, local_rr_graph->rr_node[inode].type); 
      exit(1);
    }
  }
  /* Check */
  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    /* Count in the first node is SOURCE not the SINK */
    assert ( net_cur_terminal[inet] == local_rr_graph->net_num_sinks[inet] + 1);
    assert ( net_cur_source[inet] == local_rr_graph->net_num_sources[inet]);
    assert ( net_cur_sink[inet] == local_rr_graph->net_num_sinks[inet]);
  }

  /* Free */
  my_free(net_cur_terminal);
  my_free(net_cur_source);
  my_free(net_cur_sink);

  return;
}

void alloc_pb_rr_graph_rr_indexed_data(t_rr_graph* local_rr_graph) {
  /* inside a cluster, I do not consider rr_indexed_data cost, set to 1 since other costs are multiplied by it */
  alloc_rr_graph_rr_indexed_data(local_rr_graph, 1);
  local_rr_graph->rr_indexed_data[0].base_cost = 1;

  return;
}

/* Reach here means that this LUT is in wired mode (a buffer)  
 * Add an output edge to the rr_node of the used input
 * connect it to the rr_node of the used LUT output   
 */
void add_rr_node_edge_to_one_wired_lut(t_pb_graph_node* cur_pb_graph_node,
                                       t_pb_type* cur_pb_type,
                                       t_rr_node* op_pb_rr_graph,
                                       t_rr_graph* local_rr_graph) {
  int iport, ipin;
  int jport, jpin;
  int iedge;
  int lut_input_rr_node_index, lut_output_rr_node_index;
  int num_used_lut_input_pins = 0;
  int num_used_lut_output_pins = 0;
  int temp_rr_node_index;
  int wired_lut_net_num = OPEN;
  boolean exist = FALSE;
  int cnt = 0;

  num_used_lut_input_pins = 0;
  lut_input_rr_node_index = OPEN;

  /* Find the used input pin of this LUT and rr_node in the graph */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      temp_rr_node_index = cur_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
      /* Force the vpack_net_num from net_name here */
      if ((OPEN != op_pb_rr_graph[temp_rr_node_index].net_num) 
         && (OPEN == op_pb_rr_graph[temp_rr_node_index].vpack_net_num)) {
        op_pb_rr_graph[temp_rr_node_index].vpack_net_num = op_pb_rr_graph[temp_rr_node_index].net_num;
      }
      if (OPEN != op_pb_rr_graph[temp_rr_node_index].vpack_net_num) {
        num_used_lut_input_pins++;
        wired_lut_net_num = op_pb_rr_graph[temp_rr_node_index].vpack_net_num;
        lut_input_rr_node_index = cur_pb_graph_node->input_pins[iport][ipin].physical_pb_graph_pin->rr_node_index_physical_pb;
      }
      /* If we did not find one, finish this round, otherwise set it to zero and edges */
      if (0 == num_used_lut_input_pins) {
        continue; 
      } else {
        assert (1 == num_used_lut_input_pins);
        num_used_lut_input_pins = 0;
      } 
 
      /* Find the used output*/ 
      num_used_lut_output_pins = 0;
      lut_output_rr_node_index = OPEN;
      /* Find the used output pin of this LUT and rr_node in the graph */
      for (jport = 0; jport < cur_pb_graph_node->num_output_ports; jport++) {
        for (jpin = 0; jpin < cur_pb_graph_node->num_output_pins[jport]; jpin++) {
          temp_rr_node_index = cur_pb_graph_node->output_pins[jport][jpin].pin_count_in_cluster;
          /* Force the vpack_net_num from net_name here */
          if ((OPEN != op_pb_rr_graph[temp_rr_node_index].net_num) 
             && (OPEN == op_pb_rr_graph[temp_rr_node_index].vpack_net_num)) {
            op_pb_rr_graph[temp_rr_node_index].vpack_net_num = op_pb_rr_graph[temp_rr_node_index].net_num;
          }
          if (wired_lut_net_num == op_pb_rr_graph[temp_rr_node_index].vpack_net_num) { 
            num_used_lut_output_pins++;
            lut_output_rr_node_index = cur_pb_graph_node->output_pins[jport][jpin].physical_pb_graph_pin->rr_node_index_physical_pb;
          }
        }
      }
      /* Make sure we only have 1 used output pin */
      assert ((1 == num_used_lut_output_pins) 
              && (OPEN != lut_output_rr_node_index)); 
    
      /* Add a special edge between the two rr_nodes */
      /* Check if the lut_output_rr_node is already in the edge list */
      exist = FALSE;
      for (iedge = 0; iedge < local_rr_graph->rr_node[lut_input_rr_node_index].num_edges; iedge++) {
        if (lut_output_rr_node_index == local_rr_graph->rr_node[lut_input_rr_node_index].edges[iedge]) {
          exist = TRUE;
          break;
        }
      } 
      if (FALSE == exist) {
        /* Modify the input(source) node */
        local_rr_graph->rr_node[lut_input_rr_node_index].num_edges++;
        local_rr_graph->rr_node[lut_input_rr_node_index].edges = (int*) my_realloc(local_rr_graph->rr_node[lut_input_rr_node_index].edges, 
                                                                                   local_rr_graph->rr_node[lut_input_rr_node_index].num_edges * sizeof(int));
        local_rr_graph->rr_node[lut_input_rr_node_index].edges[local_rr_graph->rr_node[lut_input_rr_node_index].num_edges -1] = lut_output_rr_node_index;
        local_rr_graph->rr_node[lut_input_rr_node_index].switches = (short*) my_realloc(local_rr_graph->rr_node[lut_input_rr_node_index].switches,                                                                                   
                                                                                        local_rr_graph->rr_node[lut_input_rr_node_index].num_edges * sizeof(short));
        local_rr_graph->rr_node[lut_input_rr_node_index].switches[local_rr_graph->rr_node[lut_input_rr_node_index].num_edges -1] = DEFAULT_SWITCH_ID;
      }
    
      /* Check if the lut_input_rr_node is already in the drive_rr_node list */
      exist = FALSE;
      for (iedge = 0; iedge < local_rr_graph->rr_node[lut_output_rr_node_index].num_drive_rr_nodes; iedge++) {
        if (&(local_rr_graph->rr_node[lut_input_rr_node_index]) == local_rr_graph->rr_node[lut_output_rr_node_index].drive_rr_nodes[iedge]) {
          exist = TRUE;
          break;
        }
      } 
      if (FALSE == exist) {
        /* Modify the output(destination) node */
        local_rr_graph->rr_node[lut_output_rr_node_index].fan_in++;
        local_rr_graph->rr_node[lut_output_rr_node_index].num_drive_rr_nodes++;
        local_rr_graph->rr_node[lut_output_rr_node_index].drive_rr_nodes = (t_rr_node**) my_realloc(local_rr_graph->rr_node[lut_output_rr_node_index].drive_rr_nodes,                                                                         
                                                                                           local_rr_graph->rr_node[lut_output_rr_node_index].num_drive_rr_nodes * sizeof(t_rr_node*));
        local_rr_graph->rr_node[lut_output_rr_node_index].drive_rr_nodes[local_rr_graph->rr_node[lut_output_rr_node_index].num_drive_rr_nodes - 1] = &(local_rr_graph->rr_node[lut_input_rr_node_index]);
    
        local_rr_graph->rr_node[lut_output_rr_node_index].drive_switches = (int*) my_realloc(local_rr_graph->rr_node[lut_output_rr_node_index].drive_switches,                                                                         
                                                                                             local_rr_graph->rr_node[lut_output_rr_node_index].num_drive_rr_nodes * sizeof(int));
        local_rr_graph->rr_node[lut_output_rr_node_index].drive_switches[local_rr_graph->rr_node[lut_output_rr_node_index].num_drive_rr_nodes - 1] = DEFAULT_SWITCH_ID;
      }
      /* Update counter */
      cnt++;
    }
  }

  /* vpr_printf(TIO_MESSAGE_INFO, "Added %d rr_node edge for wired LUT\n", cnt); */

  return;
}

/* Add rr edges connecting from an input of a LUT to its output 
 * IMPORTANT: this is only applied to LUT which operates in wire mode (a buffer) 
 */
void rec_add_unused_rr_graph_wired_lut_rr_edges(INP t_pb_graph_node* cur_op_pb_graph_node,
                                                INP t_rr_node* cur_op_pb_rr_graph,
                                                INOUTP t_rr_graph* local_rr_graph) {
  int imode, ipb, jpb; 
  t_pb_type* cur_pb_type = NULL;

  cur_pb_type = cur_op_pb_graph_node->pb_type;

  /* Go recursively until we reach a primitive node which is a LUT */

  /* Return if we reach the primitive  */
  if (TRUE == is_primitive_pb_type(cur_pb_type)) {
    /* We only care the LUTs, that is in wired mode */
    if (TRUE == is_pb_wired_lut(cur_op_pb_graph_node,
                                cur_pb_type,
                                cur_op_pb_rr_graph)) {
 
      /* Reach here means that this LUT is in wired mode (a buffer)  
       * Add an output edge to the rr_node of the used input
       * connect it to the rr_node of the used LUT output   
       */
      add_rr_node_edge_to_one_wired_lut(cur_op_pb_graph_node,
                                        cur_pb_type, 
                                        cur_op_pb_rr_graph,
                                        local_rr_graph);
    }
    return;
  }
  
  /* recursive for the child_pbs*/
  assert (FALSE == is_primitive_pb_type(cur_pb_type));
  for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
    for (ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[imode].pb_type_children[ipb].num_pb; jpb++) {
        /* We only care those used for wiring */
        if (FALSE == is_pb_used_for_wiring(&(cur_op_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]),
                                          &(cur_pb_type->modes[imode].pb_type_children[ipb]), 
                                          cur_op_pb_rr_graph)) {
          continue;
        }
        rec_add_unused_rr_graph_wired_lut_rr_edges(&(cur_op_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]),
                                                   cur_op_pb_rr_graph,
                                                   local_rr_graph);
      }
    }
  }

  return;
}


/* Add rr edges connecting from an input of a LUT to its output 
 * IMPORTANT: this is only applied to LUT which operates in wire mode (a buffer) 
 */
void rec_add_rr_graph_wired_lut_rr_edges(INP t_pb* cur_op_pb,
                                         INOUTP t_rr_graph* local_rr_graph) {
  int mode_index, ipb, jpb, imode; 
  t_pb_type* cur_pb_type = NULL;

  cur_pb_type = cur_op_pb->pb_graph_node->pb_type;
  mode_index = cur_op_pb->mode; 

  /* Go recursively until we reach a primitive node which is a LUT */

  /* Return if we reach the primitive  */
  if (TRUE == is_primitive_pb_type(cur_pb_type)) {
    /* We only care the LUTs, that is in wired mode */
    if ((LUT_CLASS == cur_pb_type->class_type)
       && (WIRED_LUT_MODE_INDEX == mode_index)) { 
      /* Reach here means that this LUT is in wired mode (a buffer)  
       * Add an output edge to the rr_node of the used input
       * connect it to the rr_node of the used LUT output   
       */
      add_rr_node_edge_to_one_wired_lut(cur_op_pb->pb_graph_node,
                                        cur_pb_type, 
                                        cur_op_pb->rr_graph,
                                        local_rr_graph);
    }
    return;
  }
  
  /* recursive for the child_pbs*/
  assert (FALSE == is_primitive_pb_type(cur_pb_type));
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Recursive*/
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_op_pb->child_pbs[ipb])&&(NULL != cur_op_pb->child_pbs[ipb][jpb].name)) {
        rec_add_rr_graph_wired_lut_rr_edges(&(cur_op_pb->child_pbs[ipb][jpb]), 
                                            local_rr_graph);
      } else if (TRUE == is_pb_used_for_wiring(&(cur_op_pb->pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]),
                                               &(cur_pb_type->modes[mode_index].pb_type_children[ipb]), 
                                               cur_op_pb->rr_graph)) {
        /* We need to extend this part:
         * Some open op_pb contains wired LUTs
         * We need go further into the hierarchy and find out the wired LUTs 
         */
        for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
          rec_add_unused_rr_graph_wired_lut_rr_edges(&(cur_op_pb->pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]),
                                                     cur_op_pb->rr_graph,
                                                     local_rr_graph);
        }
      }
    }
  }

  return;
}

/* To avoid a messy multi-source routing that may never converge,
 * For each multiple-source net, I add a new source as the unique source in routing purpose 
 * As so, edges have to be added to the decendents of sources
 */
int add_virtual_sources_to_rr_graph_multi_sources(t_rr_graph* local_rr_graph) {
  int inet, isrc;
  int unique_src_node;
  int cnt = 0;

  for (inet = 0; inet < local_rr_graph->num_nets; inet++) {
    /* Bypass single-source nets */
    if (1 == local_rr_graph->net_num_sources[inet]) {
      continue;
    }
    /* Add a new source */
    local_rr_graph->num_rr_nodes++;
    local_rr_graph->rr_node = (t_rr_node*)my_realloc(local_rr_graph->rr_node,  
                                                     local_rr_graph->num_rr_nodes * sizeof(t_rr_node));
    /* Configure the unique source node */
    unique_src_node = local_rr_graph->num_rr_nodes - 1;
    local_rr_graph->rr_node[unique_src_node].type = SOURCE;
    local_rr_graph->rr_node[unique_src_node].capacity = 1;
    local_rr_graph->rr_node[unique_src_node].fan_in = 0;
    local_rr_graph->rr_node[unique_src_node].num_drive_rr_nodes = 0;
    local_rr_graph->rr_node[unique_src_node].drive_rr_nodes = NULL;
    local_rr_graph->rr_node[unique_src_node].pb_graph_pin = NULL;
    local_rr_graph->rr_node[unique_src_node].num_edges = local_rr_graph->net_num_sources[inet];
    local_rr_graph->rr_node[unique_src_node].edges = (int*) my_calloc(local_rr_graph->rr_node[unique_src_node].num_edges, sizeof(int));
    local_rr_graph->rr_node[unique_src_node].switches = (short*) my_calloc(local_rr_graph->rr_node[unique_src_node].num_edges, sizeof(short));
    /* Configure edges */   
    for (isrc = 0; isrc < local_rr_graph->net_num_sources[inet]; isrc++) { 
      /* Connect edges to sources */
      local_rr_graph->rr_node[unique_src_node].edges[isrc] = local_rr_graph->net_rr_sources[inet][isrc]; 
      local_rr_graph->rr_node[unique_src_node].switches[isrc] = DEFAULT_SWITCH_ID; 
    }
    /* Replace the sources with the new source node */
    local_rr_graph->net_num_sources[inet] = 1;
    local_rr_graph->net_rr_sources[inet] = (int*)my_realloc(local_rr_graph->net_rr_sources[inet],
                                                            local_rr_graph->net_num_sources[inet] * sizeof(int));  
    local_rr_graph->net_rr_sources[inet][0] = unique_src_node;
    /* Replace the sources in the net_rr_terminals */
    local_rr_graph->net_rr_terminals[inet][0] = unique_src_node;
    /* Update counter */
    cnt++;
  }

  vpr_printf(TIO_MESSAGE_INFO, 
             "Added %d virtual source nodes for routing.\n",
             cnt);

  return cnt;
}

/* Allocate and load a local rr_graph for a pb
 * 1. Allocate the rr_graph nodes and configure with pb_graph_node connectivity
 * 2. load all the routing statisitics required by the router
 * 3. load the net to be routed into the rr_graph  
 */
void alloc_and_load_rr_graph_for_phy_pb(INP t_pb* cur_op_pb,
                                        INP t_phy_pb* cur_phy_pb,
                                        int L_num_vpack_nets, t_net* L_vpack_net) {

  /* Allocate rr_graph*/
  cur_phy_pb->rr_graph = (t_rr_graph*) my_calloc(1, sizeof(t_rr_graph));

  /* Create rr_graph */
  alloc_and_load_rr_graph_for_phy_pb_graph_node(cur_phy_pb->pb_graph_node, cur_phy_pb->rr_graph);

  /* Build prev nodes list for rr_nodes */
  alloc_and_load_prev_node_list_rr_graph_rr_nodes(cur_phy_pb->rr_graph);

  /* Find the nets inside the pb and initialize the rr_graph */
  alloc_and_load_phy_pb_rr_graph_nets(cur_op_pb, cur_phy_pb->rr_graph,
                                      L_num_vpack_nets, L_vpack_net); 
  
  /* Add edges to connected rr_nodes for a wired LUT */
  /* 
  rec_add_rr_graph_wired_lut_rr_edges(cur_op_pb, cur_phy_pb->rr_graph);
  */

  /* Allocate trace in rr_graph */
  alloc_rr_graph_route_static_structs(cur_phy_pb->rr_graph, nx * ny); /* TODO: nx * ny should be reduced for pb-only routing */

  alloc_pb_rr_graph_rr_indexed_data(cur_phy_pb->rr_graph);

  /* Fill the net_rr_terminals with 
   * 1. pin-to-pin mapping in pb_graph_node in cur_op_pb
   * 2. rr_graph in the cur_op_pb
   */ 
  alloc_and_load_phy_pb_rr_graph_net_rr_terminals(cur_op_pb, cur_phy_pb->rr_graph); 

  add_virtual_sources_to_rr_graph_multi_sources(cur_phy_pb->rr_graph);

  /* Allocate structs routing information */
  alloc_and_load_rr_graph_route_structs(cur_phy_pb->rr_graph);

  return;
}

