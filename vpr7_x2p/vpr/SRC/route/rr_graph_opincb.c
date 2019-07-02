#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "rr_graph_sbox.h"
#include "check_rr_graph.h"
#include "rr_graph_timing_params.h"
#include "rr_graph_indexed_data.h"
#include "vpr_utils.h"
#include "read_xml_arch_file.h"
#include "ReadOptions.h"
#include "rr_graph_opincb.h"

/* Subroutines */
static 
int add_rr_graph_one_grid_fast_edge_opin_to_cb(int grid_x, 
                                               int grid_y,
                                               t_ivec*** LL_rr_node_indices);

static 
void get_grid_side_pins(int grid_x, int grid_y, 
                        int side, enum e_pin_type pin_type,
                        t_ivec*** LL_rr_node_indices,
                        int *num_pins, t_rr_node** *pin_list);

static 
int add_opin_list_ipin_list_fast_edge(int num_opins, t_rr_node** opin_list, 
                                      int num_ipins, t_rr_node** ipin_list);

static 
int add_opin_fast_edge_to_ipin(t_rr_node* opin, 
                               t_rr_node* ipin);

static 
int get_ipin_switch_index(t_rr_node* ipin);

static 
void find_rr_nodes_ipin_driver_switch();

static 
void recover_rr_nodes_ipin_driver_switch();

/* Xifan TANG: Create Fast Interconnection between LB OPIN and CB INPUT*/
/* TOP function */
int add_rr_graph_fast_edge_opin_to_cb(t_ivec*** LL_rr_node_indices) {
  int ix, iy;
  int add_edge_counter = 0;
  int run_opintocb = 0;

  /* Decide if we run this part */

  /* For each grid, include I/O*/
  for (ix = 1; ix < (nx+1); ix++) {
    for (iy = 1; iy < (ny+1); iy++) {
      if ((EMPTY_TYPE != grid[ix][iy].type)&&(TRUE == grid[ix][iy].type->opin_to_cb)) {
        run_opintocb = 1;
        break;
      }
    }
    if (1 == run_opintocb) {
      break;
    }
  }

  if (0 == run_opintocb) {
    return add_edge_counter;
  }

  find_rr_nodes_ipin_driver_switch();

  /* For each grid, include I/O*/
  for (ix = 1; ix < (nx+1); ix++) {
    for (iy = 1; iy < (ny+1); iy++) {
      add_edge_counter += add_rr_graph_one_grid_fast_edge_opin_to_cb(ix, iy, LL_rr_node_indices);
    }
  }

  recover_rr_nodes_ipin_driver_switch();

  return add_edge_counter;
}

/* For one grid, add fast edge from a LB OPIN to CB*/
static 
int add_rr_graph_one_grid_fast_edge_opin_to_cb(int grid_x, 
                                               int grid_y,
                                               t_ivec*** LL_rr_node_indices) {
  int opin_side, ipin_side;
  boolean early_stop = FALSE;
  int num_opins = 0;
  t_rr_node** opin_list = NULL;
  int num_ipins = 0;
  t_rr_node** ipin_list = NULL;
  int add_edge_counter = 0;
  t_type_ptr type_descriptor = NULL;

  /* Make sure this is a valid grid*/
  assert((-1 < grid_x) && (grid_x < (nx+1)));
  assert((-1 < grid_y) && (grid_y < (ny+1)));

  /* Get the type_descriptor and determine if this grid should add edges */
  type_descriptor = grid[grid_x][grid_y].type;
  if (NULL == type_descriptor) {
    return add_edge_counter;
  } else if (FALSE == type_descriptor->opin_to_cb) {
    return add_edge_counter;
  }

  /* Check each side of this grid,
   * determine how many adjacent LB should be taken into account.
   */
  for (opin_side = 0; opin_side < 4; opin_side++) {
    early_stop = FALSE;
    switch(opin_side) {
    case TOP:
      /* Only I/O on the bottom side of FPGA allows to continue */
      if ((0 == grid_x)||(ny + 1 == grid_y)||(nx + 1 == grid_x)) {
        early_stop = TRUE;
      }
      break;
    case RIGHT:
      /* Only I/O on the left side of FPGA allows to continue */
      if ((0 == grid_y)||(ny + 1 == grid_y)||(nx + 1 == grid_x)) {
        early_stop = TRUE;
      }
      break;
    case BOTTOM:
      /* Only I/O on the top side of FPGA allows to continue */
      if ((0 == grid_x)||(0 == grid_y)||(nx + 1 == grid_x)) {
        early_stop = TRUE;
      }
      break;
    case LEFT:
      /* Only I/O on the right side of FPGA allows to continue */
      if ((0 == grid_x)||(ny + 1 == grid_y)||(0 == grid_y)) {
        early_stop = TRUE;
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s, [LINE%d]) Invalid opin_side(%d)!\n", 
                 __FILE__, __LINE__, opin_side);
      exit(1);
    }
    /* Decide if we should do something or not */
    if (TRUE == early_stop) {
      continue;
    }
      
    /* Find the opins on the opin_side of current grid */   
    get_grid_side_pins(grid_x, grid_y, opin_side, DRIVER, LL_rr_node_indices, &num_opins, &opin_list);
    if (0 == num_opins) { /* Do nothing if there is no OPIN available */
      continue;
    }

    switch(opin_side) {
    case TOP:
      /* Create fast connection to the grid above*/
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the bottom side */
        if (((grid_y + 1) == (ny + 1))&&(BOTTOM != ipin_side)) { 
          continue;
        }
        /* We also skip the top side because we believe a IPIN at the top side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (TOP == ipin_side) {
          continue; 
        }
        get_grid_side_pins(grid_x, grid_y+1, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      /* If this is already the bottom side of FPGA, we skip */
      if (0 == grid_y) { 
        break;
      }
      /* Create fast connection to the grid below */
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the top side */
        if (((grid_y - 1) == 0)&&(TOP != ipin_side)) { 
          continue;
        }
        /* We also skip the bottom side because we believe a IPIN at the bottom side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (BOTTOM == ipin_side) {
          continue; 
        }
        get_grid_side_pins(grid_x, grid_y-1, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      break;
    case RIGHT:
      /* Create fast connection to the grid on the right side */
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the left side */
        if (((grid_x + 1) == (nx + 1))&&(LEFT != ipin_side)) { 
          continue;
        }
        /* We also skip the top side because we believe a IPIN at the top side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (RIGHT == ipin_side) {
          continue; 
        }
        get_grid_side_pins(grid_x+1, grid_y, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      /* If this is already the left side of FPGA, we skip */
      if (0 == grid_x) { 
        break;
      }
      /* Create fast connection to the grid on the left side */
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the rigth side */
        if (((grid_x - 1) == 0)&&(RIGHT != ipin_side)) { 
          continue;
        }
        /* We also skip the bottom side because we believe a IPIN at the bottom side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (LEFT == ipin_side) {
          continue; 
        }
        get_grid_side_pins(grid_x-1, grid_y, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      break;
    case BOTTOM:
      /* Create fast connection to the grid on the bottom side */
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the left side */
        if (((grid_y - 1) == 0)&&(TOP != ipin_side)) { 
          continue;
        }
        /* We also skip the top side because we believe a IPIN at the bottom side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (BOTTOM == ipin_side) {
          continue; 
        }
        get_grid_side_pins(grid_x, grid_y-1, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      /* If this is already the left side of FPGA, we skip */
      if ((ny + 1) == grid_y) { 
        break;
      }
      /* Create fast connection to the grid on the top side */
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the rigth side */
        if (((grid_y + 1) == (ny + 1))&&(BOTTOM != ipin_side)) { 
          continue;
        }
        /* We also skip the bottom side because we believe a IPIN at the bottom side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (TOP == ipin_side) {
          continue; 
        }
        if (grid_y + 1 == ny + 1) {
          assert(BOTTOM == ipin_side);
        }
        get_grid_side_pins(grid_x, grid_y+1, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      break;
    case LEFT:
      /* Create fast connection to the grid on the left side */
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the left side */
        if (((grid_x - 1) == 0)&&(RIGHT != ipin_side)) { 
          continue;
        }
        /* We also skip the top side because we believe a IPIN at the top side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (LEFT == ipin_side) {
          continue; 
        }
        get_grid_side_pins(grid_x-1, grid_y, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      /* If this is already the right side of FPGA, we skip */
      if ((nx + 1) == grid_x) { 
        break;
      }
      /* Create fast connection to the grid on the right side */
      for (ipin_side = 0; ipin_side < 4; ipin_side++) {
        /* For the grid on the boundary, we only create connections to the right side */
        if (((grid_x + 1) == (nx + 1))&&(LEFT != ipin_side)) { 
          continue;
        }
        /* We also skip the bottom side because we believe a IPIN at the bottom side 
         * can be swapped to others by using logic equivalence. Which is faster. 
         */
        if (RIGHT == ipin_side) {
          continue; 
        }
        get_grid_side_pins(grid_x+1, grid_y, ipin_side, RECEIVER, LL_rr_node_indices, &num_ipins, &ipin_list);
        /* Create fast edge from the OPINs to IPINs*/
        add_edge_counter += add_opin_list_ipin_list_fast_edge(num_opins, opin_list, num_ipins, ipin_list);
        /* Free ipin_list */
        num_ipins = 0;
        free(ipin_list);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s, [LINE%d]) Invalid opin_side(%d)!\n", 
                 __FILE__, __LINE__, opin_side);
      exit(1);
    }
    /* Free opin_list */
    num_opins = 0;
    free(opin_list);
  }

  return add_edge_counter;
}

/* Find all the OPINs at a side of one grid. 
 * Return num_opins, and opin_list
 */
static 
void get_grid_side_pins(int grid_x, int grid_y, 
                        int side, enum e_pin_type pin_type,
                        t_ivec*** LL_rr_node_indices,
                        int *num_pins, t_rr_node** *pin_list) {
  int ipin, iheight, class_idx, cur_pin, inode;
  t_type_ptr type_descriptor = NULL;
  t_rr_type pin_rr_type;

  switch (pin_type) {
  case DRIVER:
    pin_rr_type = OPIN;
    break;
  case RECEIVER:
    pin_rr_type = IPIN;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s, [LINE%d]) Invalid pin_type(%d)!\n", 
               __FILE__, __LINE__, pin_type);
    exit(1);
  }

  /* Initialization */
  (*num_pins) = 0;
  (*pin_list) = NULL;
  /* (*pin_class_list) = NULL; */

  /* Make sure this is a valid grid*/
  assert((-1 < grid_x) && (grid_x < (nx+2)));
  assert((-1 < grid_y) && (grid_y < (ny+2)));

  assert((-1 < side) && (side < 4));

  type_descriptor = grid[grid_x][grid_y].type;
 
  /* Kick out corner cases */
  switch(side) {
  case TOP:
    /* If this is the most top grid, return*/ 
    if ((ny+1 == grid_y)||(0 == grid_x)||(nx + 1 == grid_x)) {
      return;
    }
    break;
  case RIGHT:
    /* If this is the most right grid, return*/ 
    if ((nx + 1 == grid_x)||(ny + 1 == grid_y)||(0 == grid_y)) {
      return;
    }
    break;
  case BOTTOM: 
    /* If this is the most bottom grid, return*/ 
    if ((0 == grid_y)||(0 == grid_x)||(nx + 1 == grid_x)) {
      return;
    }
    break;
  case LEFT:
    /* If this is the most left grid, return*/ 
    if ((0 == grid_x)||(0 == grid_y)||(ny + 1 == grid_y)) {
      return;
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s, [LINE%d]) Invalid side(%d)!\n", 
               __FILE__, __LINE__, side);
    exit(1);
  }

  /* Count num_opins */
  for (iheight = 0; iheight < type_descriptor->height; iheight++) {
    for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
      /* Check if this is an OPIN at the side we want*/
      class_idx = type_descriptor->pin_class[ipin];
      assert((-1 < class_idx) && (class_idx < type_descriptor->num_class));
      if ((1 == type_descriptor->pinloc[iheight][side][ipin])
         &&(pin_type == type_descriptor->class_inf[class_idx].type)
         &&(TRUE == type_descriptor->opin_to_cb)) {
        (*num_pins)++;
      } 
    }   
  }
    
  /* Alloc array */
  cur_pin = 0;
  (*pin_list) = (t_rr_node**)my_malloc((*num_pins)*sizeof(t_rr_node*));
  /* (*pin_class_list) = (int*)my_malloc((*num_pins)*sizeof(int)); */

  /* Fill array */
  for (iheight = 0; iheight < type_descriptor->height; iheight++) {
    for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
      /* Check if this is an OPIN at the side we want*/
      class_idx = type_descriptor->pin_class[ipin];
      assert((-1 < class_idx) && (class_idx < type_descriptor->num_class));
      if ((1 == type_descriptor->pinloc[iheight][side][ipin])
         &&(pin_type == type_descriptor->class_inf[class_idx].type)
         &&(TRUE == type_descriptor->opin_to_cb)) {
        inode = get_rr_node_index(grid_x, grid_y, pin_rr_type, ipin, LL_rr_node_indices);
        (*pin_list)[cur_pin] = &rr_node[inode];
        /* (*pin_class_list)[cur_pin] = class_idx; */
        cur_pin++;
      } 
    }
  }
  assert(cur_pin == (*num_pins));
    
  return;
}

/* We have one list of OPIN, and one list of IPIN, 
 * decide which two rr_nodes should have a fast edge 
 */
static 
int add_opin_list_ipin_list_fast_edge(int num_opins, t_rr_node** opin_list, 
                                      int num_ipins, t_rr_node** ipin_list) {
  int ipin, iedge, to_node, jpin;
  int num_ipin_sink = 0;
  t_linked_int* ipin_sink_head = NULL;
  int* ipin_sink_index = (int*)my_malloc(sizeof(int)*num_ipins);
  int add_edge_counter = 0;

  if (0 == num_opins) {
    assert(NULL == opin_list);
    return add_edge_counter;
  }

  if (0 == num_ipins) {
    assert(NULL == ipin_list);
    return add_edge_counter;
  }

  /* Count the number of common source of these ipin */
  for (ipin = 0; ipin < num_ipins; ipin++) {
    ipin_sink_index[ipin] = OPEN;
    assert(IPIN == ipin_list[ipin]->type);
    for (iedge = 0; iedge < ipin_list[ipin]->num_edges; iedge++) {
      to_node = ipin_list[ipin]->edges[iedge];
      if (SINK == rr_node[to_node].type) {
        /* Search if the source exists in the list */
        if (NULL == search_in_int_list(ipin_sink_head, to_node)) {
          /* Do not exist, add one */
          ipin_sink_head = insert_node_to_int_list(ipin_sink_head, to_node);
          /* Update counter */
          num_ipin_sink++;
          /* Update the index list */
          ipin_sink_index[ipin] = to_node;
        }
      }
    }
  }

  /* For each OPIN node, create a edge to each ipin whose ipin_src is different */
  for (ipin = 0; ipin < num_opins; ipin++) {
    for (jpin = 0; jpin < num_ipins; jpin++) {
      //if (OPEN != ipin_sink_index[jpin]) {
        /* Find its ipin source node*/
        //ipin_sink_list_node = search_in_int_list(ipin_sink_head, ipin_sink_index[jpin]);
        //if (NULL != ipin_sink_list_node) {
          /* Add a fast edge */
          add_edge_counter += add_opin_fast_edge_to_ipin(opin_list[ipin], ipin_list[jpin]);
          /* Make the sink list node invalid */
        //  ipin_sink_list_node->data = OPEN;
       // }
      //}
    }
  } 
  
  /* Free  */
  free_int_list(&ipin_sink_head); 

  return add_edge_counter;
}

/* Add an edge from opin to ipin, use the switch of ipin*/
static 
int add_opin_fast_edge_to_ipin(t_rr_node* opin, 
                               t_rr_node* ipin) {
  int ipin_switch_index = OPEN;
  int iedge, to_node;

  /* Make sure we have an OPIN and an IPIN*/
  assert(OPIN == opin->type);
  assert(IPIN == ipin->type);

  /* Get the switch index of ipin, and check it is valid */
  ipin_switch_index = get_ipin_switch_index(ipin); 
  if (OPEN == ipin_switch_index) {
    return 0;
  }

  /* Check if OPIN has a fan-out to IPIN already*/
  for (iedge = 0; iedge < opin->num_edges; iedge++) {
    to_node = opin->edges[iedge];
    if ((IPIN == rr_node[to_node].type)&&(ipin == &rr_node[to_node])) {
      vpr_printf(TIO_MESSAGE_WARNING, "(File:%s,[LINE%d])OPIN rr_node[%d] has an fan-out to IPIN rr_node[%d]. Fast edge will not be created.\n", __FILE__, __LINE__, opin-rr_node, ipin-rr_node);
      return 0;
    }
  }

  /* create a fast edge */
  /* 1. re-alloc a edge list switch list */
  opin->num_edges++;
  opin->edges = (int*)my_realloc(opin->edges, opin->num_edges*sizeof(int));
  opin->switches = (short*)my_realloc(opin->switches, opin->num_edges*sizeof(short));
  /* 2. Update edge and switch info */
  opin->edges[opin->num_edges-1] = ipin - rr_node; 
  opin->switches[opin->num_edges-1] = ipin_switch_index; 
  /* 3. Increase the fan-in of IPIN */
  ipin->fan_in++;
  /* Finish*/ 

  return 1;
}

/* This is not efficent, should be improved by using LL_rr_node_indices*/
static 
int get_ipin_switch_index(t_rr_node* ipin) {
  return ipin->driver_switch;
} 

static 
void find_rr_nodes_ipin_driver_switch() {
  int inode, jnode, iedge, to_node, fan_in_counter;
  int ipin_switch_index = OPEN;

  vpr_printf(TIO_MESSAGE_INFO, "Building rr_node driver switches...\n");

  for (inode = 0; inode < num_rr_nodes; inode++) {
    if (IPIN != rr_node[inode].type) {
      continue;
    }
    /* Create a counter, check the correctness*/
    fan_in_counter = 0;
    ipin_switch_index = OPEN;
    for (jnode = 0; jnode < num_rr_nodes; jnode++) {
      for (iedge = 0; iedge < rr_node[jnode].num_edges; iedge++) {
        to_node = rr_node[jnode].edges[iedge];
        if (inode == to_node) {
          fan_in_counter++;
          if (OPEN == ipin_switch_index) {
            ipin_switch_index = rr_node[jnode].switches[iedge];
          } else {
            assert(ipin_switch_index == rr_node[jnode].switches[iedge]);
          }
        }
      }
    }
    assert(fan_in_counter == rr_node[inode].fan_in);
    if (0 != fan_in_counter) {
      assert(OPEN != ipin_switch_index);
    }
    rr_node[inode].driver_switch = ipin_switch_index;
  }
   
  return;
}

static 
void recover_rr_nodes_ipin_driver_switch() {
  int inode;

  vpr_printf(TIO_MESSAGE_INFO, "Recovering rr_node driver switches...\n");

  for (inode = 0; inode < num_rr_nodes; inode++) {
    if (IPIN == rr_node[inode].type) {
      rr_node[inode].driver_switch = OPEN;
    }
  }
  return;
}

