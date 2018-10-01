/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
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

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_lut.h"
#include "spice_primitives.h"
#include "spice_routing.h"


void fprint_routing_chan_subckt(FILE* fp,
                                int x,
                                int y,
                                t_rr_type chan_type, 
                                int chan_width,
                                t_ivec*** LL_rr_node_indices,
                                int num_segment,
                                t_segment_inf* segments) {
  int itrack, inode, iseg, cost_index;
  char* chan_prefix = NULL;
  t_rr_node** chan_rr_nodes = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert((CHANX == chan_type)||(CHANY == chan_type));

  /* Initial chan_prefix*/
  switch (chan_type) {
  case CHANX:
    chan_prefix = "chanx";
    fprintf(fp, "***** Subckt for Channel X [%d][%d] *****\n", x, y);
    break;
  case CHANY:
    chan_prefix = "chany";
    fprintf(fp, "***** Subckt for Channel Y [%d][%d] *****\n", x, y);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid Channel type! Should be CHANX or CHANY.\n",
               __FILE__, __LINE__);
    exit(1);
  }
  
  /* Collect rr_nodes for Tracks for chanx[ix][iy] */
  for (itrack = 0; itrack < chan_width; itrack++) {
    inode = get_rr_node_index(x, y, chan_type, itrack, LL_rr_node_indices);
    chan_rr_nodes[itrack] = &(rr_node[inode]);
  }

  /* Chan subckt definition */
  fprintf(fp, ".subckt %s[%d][%d] \n", chan_prefix, x, y);
  /* Inputs and outputs,
   * Rules for CHANX:
   * print left-hand ports(in) first, then right-hand ports(out)
   * Rules for CHANX:
   * print bottom ports(in) first, then top ports(out)
   */
  fprintf(fp, "+ ");
  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "in%d ", itrack);
  }
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "out%d ", itrack);
  }
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  /* Middle point output for connection box inputs */
  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "mid_out%d ", itrack);
  }
  fprintf(fp, "\n");
  /* End with svdd and sgnd */
  fprintf(fp, "+ svdd sgnd\n");

  /* Print segments models*/
  for (itrack = 0; itrack < chan_width; itrack++) {
    cost_index = chan_rr_nodes[itrack]->cost_index;
    iseg = rr_indexed_data[cost_index].seg_index; 
    /* Check */
    assert((!(iseg < 0))&&(iseg < num_segment));
    assert(NULL != segments[iseg].spice_model);
    assert(SPICE_MODEL_CHAN_WIRE == segments[iseg].spice_model->type);
    fprintf(fp, "X%s[%d] ", segments[iseg].spice_model->prefix, segments[iseg].spice_model->cnt); /*Call subckt*/
    /* Update counter of SPICE model*/
    segments[iseg].spice_model->cnt++;
    /* Inputs and ouputs*/
    fprintf(fp, "in%d out%d mid_out%d ", itrack, itrack, itrack);
    /* End with svdd, sgnd and Subckt name*/
    fprintf(fp, "svdd sgnd %s_seg%d\n", segments[iseg].spice_model->name, iseg);
  }

  fprintf(fp, ".eom\n");

  /* Free */
  my_free(chan_rr_nodes);
  
  return;
}

t_rr_node** get_grid_side_pin_rr_nodes(int* num_pin_rr_nodes,
                                       t_rr_type pin_type,
                                       int x,
                                       int y,
                                       int side,
                                       t_ivec*** LL_rr_node_indices) {
  int height, ipin, class_id, inode;
  t_type_ptr type = NULL;
  t_rr_node** ret = NULL;
  enum e_pin_type pin_class_type;
  int cur;
  
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  type = grid[x][y].type;
  assert(NULL != type);
  /* Assign the type of PIN*/ 
  switch (pin_type) {
  case IPIN:
  /* case SINK: */
    pin_class_type = RECEIVER; /* This is the end of a route path*/ 
    break;
  /*case SOURCE:*/
  case OPIN:
    pin_class_type = DRIVER; /* This is the start of a route path */ 
    break;
  /* SINK and SOURCE are hypothesis nodes */
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid pin_type!\n", __FILE__, __LINE__);
    exit(1); 
  }

  /* Output the pins on the side*/
  (*num_pin_rr_nodes) = 0;
  height = grid[x][y].offset;
  for (ipin = 0; ipin < type->num_pins; ipin++) {
    class_id = type->pin_class[ipin];
    if ((1 == type->pinloc[height][side][ipin])&&(pin_class_type == type->class_inf[class_id].type)) {
      (*num_pin_rr_nodes)++;
    }
  } 
  /* Malloc */
  ret = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*(*num_pin_rr_nodes)); 

  /* Fill the return array*/
  cur = 0;
  height = grid[x][y].offset;
  for (ipin = 0; ipin < type->num_pins; ipin++) {
    class_id = type->pin_class[ipin];
    if ((1 == type->pinloc[height][side][ipin])&&(pin_class_type == type->class_inf[class_id].type)) {
      inode = get_rr_node_index(x, y, pin_type, ipin, LL_rr_node_indices);
      ret[cur] = &(rr_node[inode]); 
      cur++;
    }
  } 
  assert(cur == (*num_pin_rr_nodes));
  
  return ret;
}

void fprint_grid_side_in_with_given_index(FILE* fp,
                                          int pin_index,
                                          int side,
                                          int x,
                                          int y) {
  int height, class_id;
  t_type_ptr type = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  type = grid[x][y].type;
  assert(NULL != type);

  assert((!(0 > pin_index))&&(pin_index < type->num_pins));
  assert((!(0 > side))&&(!(side > 3)));

  /* Output the pins on the side*/ 
  height = grid[x][y].offset;
  class_id = type->pin_class[pin_index];
  if ((1 == type->pinloc[height][side][pin_index])) {
    fprintf(fp, "grid[%d][%d]_pin[%d][%d][%d] ", x, y, height, side, pin_index);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Fail to print a grid pin (x=%d, y=%d, height=%d, side=%d, index=%d)",
              __FILE__, __LINE__, x, y, height, side, pin_index);
    exit(1);
  } 

  return;
}

void fprint_grid_side_pins(FILE* fp,
                           t_rr_type pin_type,
                           int x,
                           int y,
                           int side) {
  int height, ipin, class_id;
  t_type_ptr type = NULL;
  enum e_pin_type pin_class_type;
  
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  type = grid[x][y].type;
  assert(NULL != type);
 
  /* Assign the type of PIN*/ 
  switch (pin_type) {
  case IPIN:
  /* case SINK: */
    pin_class_type = RECEIVER; /* This is the end of a route path*/ 
    break;
  /* case SOURCE: */
  case OPIN:
    pin_class_type = DRIVER; /* This is the start of a route path */ 
    break;
  /* SINK and SOURCE are hypothesis nodes */
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid pin_type!\n", __FILE__, __LINE__);
    exit(1); 
  }


  /* Output the pins on the side*/ 
  height = grid[x][y].offset;
  for (ipin = 0; ipin < type->num_pins; ipin++) {
    class_id = type->pin_class[ipin];
    if ((1 == type->pinloc[height][side][ipin])&&(pin_class_type == type->class_inf[class_id].type)) {
      fprintf(fp, "grid[%d][%d]_pin[%d][%d][%d] ", x, y, height, side, ipin);
    }
  } 
  
  return;
}

/* Determine the channel coordinates in switch box subckt */
void determine_src_chan_coordinate_switch_box(t_rr_node* src_rr_node,
                                              t_rr_node* des_rr_node,
                                              int side,
                                              int switch_box_x,
                                              int switch_box_y,
                                              int* src_chan_x,
                                              int* src_chan_y,
                                              char** src_chan_port_name) {
  /* Check */ 
  assert((!(0 > side))&&(side < 4));
  assert((CHANX == src_rr_node->type)||(CHANY == src_rr_node->type));
  assert((CHANX == des_rr_node->type)||(CHANY == des_rr_node->type));
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 
 
  /* Initialize*/
  (*src_chan_x) = 0;
  (*src_chan_y) = 0;
  (*src_chan_port_name) = NULL;

  switch (side) {
  case 0: /*TOP*/
    /* The destination rr_node only have one condition!!! */
    assert((INC_DIRECTION == des_rr_node->direction)&&(CHANY == des_rr_node->type));
    /* Following cases:
     *               |
     *             / | \
     */
    if ((INC_DIRECTION == src_rr_node->direction)&&(CHANY == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "out";
    } else if ((INC_DIRECTION == src_rr_node->direction)&&(CHANX == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "out";
    } else if ((DEC_DIRECTION == src_rr_node->direction)&&(CHANX == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x + 1;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "in";
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid source channel!\n", __FILE__, __LINE__);
      exit(1);
    }
    break; 
  case 1: /*RIGHT*/
    /* The destination rr_node only have one condition!!! */
    assert((INC_DIRECTION == des_rr_node->direction)&&(CHANX == des_rr_node->type));
    /* Following cases:
     *          \               
     *       ---  ----  
     *          /
     */
    if ((DEC_DIRECTION == src_rr_node->direction)&&(CHANY == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y + 1;
      (*src_chan_port_name) = "in";
    } else if ((INC_DIRECTION == src_rr_node->direction)&&(CHANX == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "out";
    } else if ((INC_DIRECTION == src_rr_node->direction)&&(CHANY == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "out";
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid source channel!\n", __FILE__, __LINE__);
      exit(1);
    }
    break; 
  case 2: /*BOTTOM*/
    /* The destination rr_node only have one condition!!! */
    assert((DEC_DIRECTION == des_rr_node->direction)&&(CHANY == des_rr_node->type));
    /* Following cases:
     *          |               
     *        \   /  
     *          |
     */
    if ((DEC_DIRECTION == src_rr_node->direction)&&(CHANY == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y + 1;
      (*src_chan_port_name) = "in";
    } else if ((INC_DIRECTION == src_rr_node->direction)&&(CHANX == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "out";
    } else if ((DEC_DIRECTION == src_rr_node->direction)&&(CHANX == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x + 1;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "in";
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid source channel!\n", __FILE__, __LINE__);
      exit(1);
    }
    break; 
  case 3: /*LEFT*/
    /* The destination rr_node only have one condition!!! */
    assert((DEC_DIRECTION == des_rr_node->direction)&&(CHANX == des_rr_node->type));
    /* Following cases:
     *           /               
     *       ---  ----  
     *           \
     */
    if ((DEC_DIRECTION == src_rr_node->direction)&&(CHANX == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x + 1;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "in";
    } else if ((INC_DIRECTION == src_rr_node->direction)&&(CHANY == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y;
      (*src_chan_port_name) = "out";
    } else if ((DEC_DIRECTION == src_rr_node->direction)&&(CHANY == src_rr_node->type)) {
      (*src_chan_x) = switch_box_x;
      (*src_chan_y) = switch_box_y + 1;
      (*src_chan_port_name) = "in";
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid source channel!\n", __FILE__, __LINE__);
      exit(1);
    }
    break; 
  default: 
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid side!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Make sure the source rr_node (channel) is in the range*/
  assert((!((*src_chan_x) < src_rr_node->xlow))&&(!((*src_chan_x) > src_rr_node->xhigh)));
  if (!((!((*src_chan_y) < src_rr_node->ylow))&&(!((*src_chan_y) > src_rr_node->yhigh)))) {
  assert((!((*src_chan_y) < src_rr_node->ylow))&&(!((*src_chan_y) > src_rr_node->yhigh)));
  }

  return; 
}

void fprint_switch_box_chan_port(FILE* fp,
                                 int switch_box_x, 
                                 int switch_box_y, 
                                 int chan_side,
                                 t_rr_node* cur_rr_node) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  switch (chan_side) {
  case 0: /*TOP*/
    /* The destination rr_node only have one condition!!! */
    assert((INC_DIRECTION == cur_rr_node->direction)&&(CHANY == cur_rr_node->type));
    fprintf(fp, "chany[%d][%d]_in[%d] ", switch_box_x, switch_box_y + 1, cur_rr_node->ptc_num);
    break;
  case 1: /*RIGHT*/
    /* The destination rr_node only have one condition!!! */
    assert((INC_DIRECTION == cur_rr_node->direction)&&(CHANX == cur_rr_node->type));
    fprintf(fp, "chanx[%d][%d]_in[%d] ", switch_box_x + 1, switch_box_y, cur_rr_node->ptc_num);
    break;
  case 2: /*BOTTOM*/
    /* The destination rr_node only have one condition!!! */
    assert((DEC_DIRECTION == cur_rr_node->direction)&&(CHANY == cur_rr_node->type));
    fprintf(fp, "chany[%d][%d]_out[%d] ", switch_box_x, switch_box_y, cur_rr_node->ptc_num);
    break;
  case 3: /*LEFT*/
    /* The destination rr_node only have one condition!!! */
    assert((DEC_DIRECTION == cur_rr_node->direction)&&(CHANX == cur_rr_node->type));
    fprintf(fp, "chanx[%d][%d]_out[%d] ", switch_box_x, switch_box_y, cur_rr_node->ptc_num);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid side!\n", __FILE__, __LINE__);
    exit(1);
  }
  return;
}

/* Print a short interconneciton in switch box
 * There are two cases should be noticed.
 * 1. The actual fan-in of cur_rr_node is 0. In this case,
      the cur_rr_node need to be short connected to itself which is on the opposite side of this switch
 * 2. The actual fan-in of cur_rr_node is 0. In this case,
 *    The cur_rr_node need to connected to the drive_rr_node
 */
void fprint_switch_box_short_interc(FILE* fp, 
                                    int switch_box_x, 
                                    int switch_box_y, 
                                    int chan_side,
                                    t_rr_node* cur_rr_node,
                                    int actual_fan_in,
                                    t_rr_node* drive_rr_node) {
  int side; 
  int track_index;
  int grid_x, grid_y, pin_index, height, class_id, pin_written_times;
  t_type_ptr type = NULL;
  int src_chan_x, src_chan_y;
  char* src_chan_port_name = NULL;
  char* chan_name = NULL;
  char* des_chan_port_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 
  assert((0 == actual_fan_in)||(1 == actual_fan_in));

  switch(cur_rr_node->type) {
  case CHANX:
    chan_name = "chanx";
    break;
  case CHANY:
    chan_name = "chany";
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid cur_rr_node_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  switch (cur_rr_node->direction) {
  case INC_DIRECTION:
    des_chan_port_name = "in"; 
    break;
  case DEC_DIRECTION:
    des_chan_port_name = "out"; 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid cur_rr_node directionality!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "V%s[%d][%d]_%s[%d] ", 
          chan_name, switch_box_x, switch_box_y, des_chan_port_name, cur_rr_node->ptc_num);

  /* Check the driver*/
  if (0 == actual_fan_in) {
    assert(drive_rr_node == cur_rr_node);
  } else {
    /* drive_rr_node = &(rr_node[cur_rr_node->prev_node]); */
    assert(1 == rr_node_drive_switch_box(drive_rr_node, cur_rr_node, switch_box_x, switch_box_y, chan_side));
  }
  switch (drive_rr_node->type) {
  /* case SOURCE: */
  case OPIN:
    /* Indicate a CLB Outpin*/
    /* Get grid information*/
    grid_x = drive_rr_node->xlow; 
    grid_y = drive_rr_node->ylow; /*Plus the offset*/
    assert((switch_box_x == grid_x)||((switch_box_x+1) == grid_x));
    assert((switch_box_y == grid_y)||((switch_box_y+1) == grid_y));
    height = grid[grid_x][grid_y].offset;
    grid_y = grid_y + height; /* May Cause Prob.*/
    type = grid[grid_x][grid_y].type;
    assert(NULL != type);
    /* Get pin information*/
    pin_index = drive_rr_node->ptc_num;
    class_id = type->pin_class[pin_index];
    assert(DRIVER == type->class_inf[class_id].type);
    pin_written_times = 0;
    side = determine_io_grid_side(grid_x, grid_y);
    if (1 == type->pinloc[height][side][pin_index]) {
      fprintf(fp, "grid[%d][%d]_pin[%d][%d][%d] ", 
              grid_x, grid_y, height, side, pin_index);
      pin_written_times++;
    }
    /* Make sure this pin is printed only once!!! (TODO: make sure for IO PAD, this remains ok)*/
    assert(1 == pin_written_times);
    break;
  case CHANX:
    determine_src_chan_coordinate_switch_box(drive_rr_node, cur_rr_node, chan_side,
                                             switch_box_x, switch_box_y, &src_chan_x, &src_chan_y, &src_chan_port_name);
    /* For channels, ptc_num is the track_index*/
    track_index = drive_rr_node->ptc_num; 
    fprintf(fp, "chanx[%d][%d]_%s[%d] ", src_chan_x, src_chan_y, src_chan_port_name, track_index);
    //my_free(src_chan_port_name);
    break;
  case CHANY:
    determine_src_chan_coordinate_switch_box(drive_rr_node, cur_rr_node, chan_side,
                                             switch_box_x, switch_box_y, &src_chan_x, &src_chan_y, &src_chan_port_name);
    /* For channels, ptc_num is the track_index*/
    track_index = drive_rr_node->ptc_num; 
    fprintf(fp, "chany[%d][%d]_%s[%d] ", src_chan_x, src_chan_y, src_chan_port_name, track_index);
    //my_free(src_chan_port_name);
    break;
  /* SOURCE is invalid as well */
  default: /* IPIN, SINK are invalid*/
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Output port */
  fprint_switch_box_chan_port(fp, switch_box_x, switch_box_y, chan_side, cur_rr_node);

  /* END */
  fprintf(fp, "0\n");

  return;
}

/* Print the SPICE netlist of multiplexer that drive this rr_node */
void fprint_switch_box_mux(FILE* fp, 
                           int switch_box_x, 
                           int switch_box_y, 
                           int chan_side,
                           t_rr_node* cur_rr_node,
                           int mux_size,
                           t_rr_node** drive_rr_nodes,
                           int switch_index) {
  int inode, side;
  int track_index;
  int grid_x, grid_y, pin_index, height, class_id, pin_written_times;
  t_type_ptr type = NULL;
  t_spice_model* spice_model = NULL;
  int src_chan_x, src_chan_y;
  char* src_chan_port_name = NULL;
  int mux_level, path_id, cur_num_sram, ilevel;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 

  /* Check current rr_node is CHANX or CHANY*/
  assert((CHANX == cur_rr_node->type)||(CHANY == cur_rr_node->type));
  
  /* Allocate drive_rr_nodes according to the fan-in*/
  assert((2 == mux_size)||(2 < mux_size));

  /* Get spice model*/
  spice_model = switch_inf[switch_index].spice_model;
  /* Now it is the time print the SPICE netlist of MUX*/
  fprintf(fp, "X%s_size%d[%d] ", spice_model->prefix, mux_size, spice_model->cnt);
  spice_model->cnt++;
  /* Input ports*/
  for (inode = 0; inode < mux_size; inode++) {
    switch (drive_rr_nodes[inode]->type) {
    /* case SOURCE: */
    case OPIN:
      /* Indicate a CLB Outpin*/
      /* Get grid information*/
      grid_x = drive_rr_nodes[inode]->xlow; 
      grid_y = drive_rr_nodes[inode]->ylow; /*Plus the offset*/
      assert((switch_box_x == grid_x)||((switch_box_x+1) == grid_x));
      assert((switch_box_y == grid_y)||((switch_box_y+1) == grid_y));
      height = grid[grid_x][grid_y].offset;
      grid_y = grid_y + height; /* May Cause Prob.*/
      type = grid[grid_x][grid_y].type;
      assert(NULL != type);
      /* Get pin information*/
      pin_index = drive_rr_nodes[inode]->ptc_num;
      class_id = type->pin_class[pin_index];
      assert(DRIVER == type->class_inf[class_id].type);
      pin_written_times = 0;
      /* See the channel type and then determine the side*/
      switch (cur_rr_node->type) {
      case CHANX:
        if (switch_box_y == grid_y) {
          side = TOP;
        } else if ((switch_box_y+1) == grid_y) {
          side = BOTTOM;
        } else {
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid grid_y!\n", __FILE__, __LINE__);
          exit(1);
        }
        break;
      case CHANY:
        if (switch_box_x == grid_x) {
          side = RIGHT;
        } else if ((switch_box_x+1) == grid_x) {
          side = LEFT;
        } else {
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid grid_y!\n", __FILE__, __LINE__);
          exit(1);
        }
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid cur_rr_node type!\n", __FILE__, __LINE__);
        exit(1);
      }
      /* Find the pin to be printed*/
      if (1 == type->pinloc[height][side][pin_index]) {
        fprintf(fp, "grid[%d][%d]_pin[%d][%d][%d] ", grid_x, grid_y, height, side, pin_index);
        pin_written_times++;
      }
      /* Make sure this pin is printed only once!!! (TODO: make sure for IO PAD, this remains ok)*/
      assert(1 == pin_written_times);
      break;
    case CHANX:
      determine_src_chan_coordinate_switch_box(drive_rr_nodes[inode], cur_rr_node, chan_side,
                                               switch_box_x, switch_box_y, &src_chan_x, &src_chan_y, &src_chan_port_name);
      /* For channels, ptc_num is the track_index*/
      track_index = drive_rr_nodes[inode]->ptc_num; 
      fprintf(fp, "chanx[%d][%d]_%s[%d] ", src_chan_x, src_chan_y, src_chan_port_name, track_index);
      //my_free(src_chan_port_name);
      break;
    case CHANY:
      determine_src_chan_coordinate_switch_box(drive_rr_nodes[inode], cur_rr_node, chan_side,
                                               switch_box_x, switch_box_y, &src_chan_x, &src_chan_y, &src_chan_port_name);
      /* For channels, ptc_num is the track_index*/
      track_index = drive_rr_nodes[inode]->ptc_num; 
      fprintf(fp, "chany[%d][%d]_%s[%d] ", src_chan_x, src_chan_y, src_chan_port_name, track_index);
      //my_free(src_chan_port_name);
      break;
    default: /* IPIN, SINK are invalid*/
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  /* Output port */
  fprint_switch_box_chan_port(fp, switch_box_x, switch_box_y, chan_side, cur_rr_node);

  /* Configuration bits for this MUX*/
  path_id = -1;
  for (inode = 0; inode < mux_size; inode++) {
    if (drive_rr_nodes[inode] == &(rr_node[cur_rr_node->prev_node])) {
      path_id = inode;
      break;
    }
  }

  if (!((-1 != path_id)&&(path_id < mux_size))) {
  assert((-1 != path_id)&&(path_id < mux_size));
  }

  switch (spice_model->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    mux_level = determine_tree_mux_level(mux_size);
    num_mux_sram_bits = mux_level;
    mux_sram_bits = decode_tree_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    mux_level = 1;
    num_mux_sram_bits = mux_size;
    mux_sram_bits = decode_onelevel_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    mux_level = spice_model->mux_num_level;
    num_mux_sram_bits = determine_num_input_basis_multilevel_mux(mux_size, mux_level) * mux_level;
    mux_sram_bits = decode_multilevel_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model->name);
    exit(1);
  } 

  /* Print SRAMs that configure this MUX */
  /* TODO: What about RRAM-based MUX? */
  cur_num_sram = sram_spice_model->cnt;
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    /* Configure the SRAMs*/
    switch (mux_sram_bits[ilevel]) {
    case 0:
      fprintf(fp,"%s[%d]->out %s[%d]->outb ", 
              sram_spice_model->prefix, cur_num_sram, sram_spice_model->prefix, cur_num_sram);
      break;
    case 1:
      fprintf(fp,"%s[%d]->outb %s[%d]->out ", 
              sram_spice_model->prefix, cur_num_sram, sram_spice_model->prefix, cur_num_sram);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s,[LINE%d])Invalid sram_bit(=%d)! Should be [0|1].\n",
                 __FILE__, __LINE__, mux_sram_bits[ilevel]);
      exit(1);
    }
    cur_num_sram++;
  }

  /* End with svdd and sgnd, subckt name*/
  fprintf(fp, "svdd sgnd %s_size%d\n", spice_model->name, mux_size);
  
  /* Print the encoding in SPICE netlist for debugging */
  fprintf(fp, "***** SRAM bits for MUX[%d], level=%d, select_path_id=%d. *****\n", 
          spice_model->cnt, mux_level, path_id);
  fprintf(fp, "*****");
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprintf(fp, "%d", mux_sram_bits[ilevel]);
  }
  fprintf(fp, "*****\n");

  /* Call SRAM subckts*/
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprintf(fp, "X%s[%d] ", sram_spice_model->prefix, sram_spice_model->cnt);
    /*fprintf(fp, "%s[%d]->in ", sram_spice_model->prefix, sram_spice_model->cnt);*/
    fprintf(fp, "%s->in ", sram_spice_model->prefix); /* Input*/
    fprintf(fp, "%s[%d]->out ", sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, "%s[%d]->outb ", sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, "gvdd_sram_sbs sgnd %s\n", sram_spice_model->name);
    /* Add nodeset to help convergence */ 
    fprintf(fp, ".nodeset V(%s[%d]->out) 0\n", sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, ".nodeset V(%s[%d]->outb) vsp\n", sram_spice_model->prefix, sram_spice_model->cnt);
    /* Pull Up/Down the SRAM outputs*/
    sram_spice_model->cnt++;
  }

  /* Free */
  my_free(mux_sram_bits);

  return;
}

void fprint_switch_box_interc(FILE* fp, 
                              int switch_box_x, 
                              int switch_box_y, 
                              int chan_side,
                              t_rr_node* cur_rr_node) {
  int num_drive_rr_nodes = 0;  
  t_rr_node** drive_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 
  /*
  find_drive_rr_nodes_switch_box(switch_box_x, switch_box_y, cur_rr_node, chan_side, 0, 
                                 &num_drive_rr_nodes, &drive_rr_nodes, &switch_index);
  */

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (1 == is_sb_interc_between_segments(switch_box_x, switch_box_y, cur_rr_node, chan_side)) {
    num_drive_rr_nodes = 0;
    drive_rr_nodes = NULL;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
    drive_rr_nodes = cur_rr_node->drive_rr_nodes;
  }

  if (0 == num_drive_rr_nodes) {
    /* Print a special direct connection*/
    fprint_switch_box_short_interc(fp, switch_box_x, switch_box_y, chan_side, cur_rr_node, 
                                   num_drive_rr_nodes, cur_rr_node);
  } else if (1 == num_drive_rr_nodes) {
    /* Print a direct connection*/
    fprint_switch_box_short_interc(fp, switch_box_x, switch_box_y, chan_side, cur_rr_node, 
                                   num_drive_rr_nodes, drive_rr_nodes[0]);
  } else if (1 < num_drive_rr_nodes) {
    /* Print the multiplexer, fan_in >= 2 */
    fprint_switch_box_mux(fp, switch_box_x, switch_box_y, chan_side, cur_rr_node, 
                          num_drive_rr_nodes, drive_rr_nodes, 
                          cur_rr_node->drive_switches[0]);
  } /*Nothing should be done else*/ 

  /* Free */

  return;
}

/* Task: Print the subckt of a Switch Box.
 * A Switch Box subckt consists of following ports:
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
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *       ChanX      | Switch |     ChanX 
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 */
void fprint_routing_switch_box_subckt(FILE* fp, 
                                      int x, 
                                      int y, 
                                      t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side, ix, iy;
  t_rr_node*** chan_rr_nodes = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*4); /* 4 sides*/
  int* chan_width = (int*)my_malloc(sizeof(int)*4); /* 4 sides */
  /*
  t_rr_node** top_chan_rr_nodes = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width);
  t_rr_node** right_chan_rr_nodes = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width);
  t_rr_node** bottom_chan_rr_nodes = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width);
  t_rr_node** left_chan_rr_nodes = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width);
  */

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  /* Find all rr_nodes of channels */
  for (side = 0; side < 4; side++) {
    switch (side) {
    case 0:
      /* For the bording, we should take special care */
      if (y == ny) {
        chan_width[side] = 0;
        chan_rr_nodes[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = x; 
      iy = y + 1;
      /* Channel width */
      chan_width[side] = chan_width_y[ix];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      chan_rr_nodes[side] = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width[side]);
      /* Collect rr_nodes for Tracks for top: chany[x][y+1] */
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        inode = get_rr_node_index(ix, iy, CHANY, itrack, LL_rr_node_indices);
        chan_rr_nodes[0][itrack] = &(rr_node[inode]);
      }
      break;
    case 1:
      /* For the bording, we should take speical care */
      if (x == nx) {
        chan_width[side] = 0;
        chan_rr_nodes[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = x + 1; 
      iy = y;
      /* Channel width */
      chan_width[side] = chan_width_x[iy];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      chan_rr_nodes[side] = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width[side]);
      /* Collect rr_nodes for Tracks for right: chanX[x+1][y] */
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        inode = get_rr_node_index(ix, iy, CHANX, itrack, LL_rr_node_indices);
        chan_rr_nodes[1][itrack] = &(rr_node[inode]);
      }
      break;
    case 2:
      /* For the bording, we should take speical care */
      if (y == 0) {
        chan_width[side] = 0;
        chan_rr_nodes[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = x; 
      iy = y;
      /* Channel width */
      chan_width[side] = chan_width_y[ix];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      chan_rr_nodes[side] = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width[side]);
      /* Collect rr_nodes for Tracks for bottom: chany[x][y] */
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        inode = get_rr_node_index(ix, iy, CHANY, itrack, LL_rr_node_indices);
        chan_rr_nodes[2][itrack] = &(rr_node[inode]);
      }
      break;
    case 3:
      /* For the bording, we should take speical care */
      if (x == 0) {
        chan_width[side] = 0;
        chan_rr_nodes[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = x; 
      iy = y;
      /* Channel width */
      chan_width[side] = chan_width_x[iy];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      chan_rr_nodes[side] = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*chan_width[side]);
      /* Collect rr_nodes for Tracks for left: chanx[x][y] */
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        inode = get_rr_node_index(ix, iy, CHANX, itrack, LL_rr_node_indices);
        chan_rr_nodes[3][itrack] = &(rr_node[inode]);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index!\n", __FILE__, __LINE__);
      exit(1);
    }
  }

  /* Print the definition of subckt*/
  fprintf(fp, "***** Switch Box[%d][%d] Sub-Circuit *****\n", x, y);
  fprintf(fp, ".subckt sb[%d][%d] ", x, y);
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  /* 1. Channel Y [x][y+1] inputs */
  for (itrack = 0; itrack < chan_width[0]; itrack++) {
    fprintf(fp, "chany[%d][%d]_in[%d] ", x, y + 1, itrack);
  }
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  /* 2. Channel X [x+1][y] inputs */
  for (itrack = 0; itrack < chan_width[1]; itrack++) {
    fprintf(fp, "chanx[%d][%d]_in[%d] ", x + 1, y, itrack);
  }
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  /* 3. Channel Y [x][y] outputs */
  for (itrack = 0; itrack < chan_width[2]; itrack++) {
    fprintf(fp, "chany[%d][%d]_out[%d] ", x, y, itrack);
  }
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  /* 4. Channel X [x][y] outputs */
  for (itrack = 0; itrack < chan_width[3]; itrack++) {
    fprintf(fp, "chanx[%d][%d]_out[%d] ", x, y, itrack);
  }
  fprintf(fp, "\n");
  fprintf(fp, "+ ");

  /* Considering the border */
  if (ny != y) {
    /* 5. Grid[x][y+1] Right side outputs pins */
    fprint_grid_side_pins(fp, OPIN, x, y+1, 1);
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }
  if (0 != x) {
    /* 6. Grid[x][y+1] Bottom side outputs pins */
    fprint_grid_side_pins(fp, OPIN, x, y+1, 2);
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  if (ny != y) {
    /* 7. Grid[x+1][y+1] Left side output pins */
    fprint_grid_side_pins(fp, OPIN, x+1, y+1, 3);
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }
  if (nx != x) {
    /* 8. Grid[x+1][y+1] Bottom side output pins */
    fprint_grid_side_pins(fp, OPIN, x+1, y+1, 2);
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  if (nx != x) {
    /* 9. Grid[x+1][y] Top side output pins */
    fprint_grid_side_pins(fp, OPIN, x+1, y, 0);
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }
  if (0 != y) {
    /* 10. Grid[x+1][y] Left side output pins */
    fprint_grid_side_pins(fp, OPIN, x+1, y, 3);
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  if (0 != y) {
    /* 11. Grid[x][y] Right side output pins */
    fprint_grid_side_pins(fp, OPIN, x, y, 1);
  } 
  if (0 != x) {
    /* 12. Grid[x][y] Top side output pins */
    fprint_grid_side_pins(fp, OPIN, x, y, 0);
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  /* Local Vdd and Gnd */
  fprintf(fp, "svdd sgnd\n");

  /* Put down all the multiplexers */
  for (side = 0; side < 4; side++) {
    switch (side) {
    case 0:
      /* For the bording, we should take speical care */
      if (y == ny) {
        break;
      }
      /* Start from the TOP side*/
      fprintf(fp, "***** TOP side Multiplexers *****\n");
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        assert(CHANY == chan_rr_nodes[side][itrack]->type);
        /* We care INC_DIRECTION tracks at this side*/
        if (INC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          fprint_switch_box_interc(fp, x, y, side, chan_rr_nodes[side][itrack]);
        } 
      }
      break;
    case 1:
      /* For the bording, we should take speical care */
      if (x == nx) {
        break;
      }
      /* RIGHT side*/
      fprintf(fp, "***** RIGHT side Multiplexers *****\n");
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        assert(CHANX == chan_rr_nodes[side][itrack]->type);
        /* We care INC_DIRECTION tracks at this side*/
        if (INC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          fprint_switch_box_interc(fp, x, y, side, chan_rr_nodes[side][itrack]);
        } 
      }
      break;
    case 2:
      /* For the bording, we should take speical care */
      if (y == 0) {
        break;
      }
      /* BOTTOM side*/
      fprintf(fp, "***** BOTTOM side Multiplexers *****\n");
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        assert(CHANY == chan_rr_nodes[side][itrack]->type);
        /* We care DEC_DIRECTION tracks at this side*/
        if (DEC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          fprint_switch_box_interc(fp, x, y, side, chan_rr_nodes[side][itrack]);
        } 
      }
      break;
    case 3:
      /* For the bording, we should take speical care */
      if (x == 0) {
        break;
      }
      /* LEFT side*/
      fprintf(fp, "***** LEFT side Multiplexers *****\n");
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        assert(CHANX == chan_rr_nodes[side][itrack]->type);
        /* We care DEC_DIRECTION tracks at this side*/
        if (DEC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          fprint_switch_box_interc(fp, x, y, side, chan_rr_nodes[side][itrack]);
        } 
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index!\n", __FILE__, __LINE__);
      exit(1);
    }
  }
 
  fprintf(fp, ".eom\n");

  /* Free chan_rr_nodes */
  my_free(chan_width);
  for (side = 0; side < 4; side++) {
    my_free(chan_rr_nodes[side]);
  }
  my_free(chan_rr_nodes);

  return;
}

/* SRC rr_node is the IPIN of a grid.*/
void fprint_connection_box_short_interc(FILE* fp,
                                        t_rr_type chan_type,
                                        int cb_x,
                                        int cb_y,
                                        t_rr_node* src_rr_node) {
  t_rr_node* drive_rr_node = NULL;
  int iedge, check_flag;
  int xlow, ylow, height, side;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cb_x))&&(!(cb_x > (nx + 1)))); 
  assert((!(0 > cb_y))&&(!(cb_y > (ny + 1)))); 
  assert(1 == src_rr_node->fan_in);

  /* Check the driver*/
  drive_rr_node = &(rr_node[src_rr_node->prev_node]); 
  assert((CHANX == drive_rr_node->type)||(CHANY == drive_rr_node->type));
  check_flag = 0;
  for (iedge = 0; iedge < drive_rr_node->num_edges; iedge++) {
    if (src_rr_node == &(rr_node[drive_rr_node->edges[iedge]])) {
      check_flag++;
    }
  }
  assert(1 == check_flag);

  xlow = src_rr_node->xlow;
  ylow = src_rr_node->ylow;
  height = grid[xlow][ylow].offset;

  /* Call the zero-resistance model */
  switch(chan_type) {
  case CHANX:
    fprintf(fp, "Vcbx[%d][%d]_grid[%d][%d]_pin[%d] ", cb_x, cb_y, xlow, ylow + height, src_rr_node->ptc_num);
    break;
  case CHANY:
    fprintf(fp, "Vcby[%d][%d]_grid[%d][%d]_pin[%d] ", cb_x, cb_y, xlow, ylow + height, src_rr_node->ptc_num);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
  /* Input port*/
  switch(src_rr_node->type) {
  case IPIN:
  /* case SINK: */
    switch (drive_rr_node->type) {
    case CHANX: 
      assert(cb_x == xlow);
      if (cb_y == (ylow + height)) {
        side  = TOP;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x, cb_y);
      } else if ((cb_y + 1) == (ylow + height)) {
        side  = BOTTOM;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x, cb_y);
      } else {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid coordinator of cb_y(=%d) and ylow+offset(=%d)!\n", __FILE__, __LINE__, cb_y, ylow+height);
        exit(1);
      }
      break;
    case CHANY: 
      assert(cb_y == (ylow + height));
      if (cb_x == xlow) {
        side  = RIGHT;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x, cb_y);
      } else if ((cb_x + 1) == xlow) {
        side  = LEFT;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x, cb_y);
      } else {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid coordinator of cb_x(=%d) and xlow(=%d)!\n", __FILE__, __LINE__, cb_x, xlow);
        exit(1);
      }
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of drive_rr_node!\n", __FILE__, __LINE__);
      exit(1);
    }
    break;
  /* SINK is a hypothesis node */
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of src_rr_node!\n", __FILE__, __LINE__);
    exit(1);
  }
  /* output porti -- > connect to the output at middle point of a channel */
  switch(drive_rr_node->type) {
  case CHANX:
    fprintf(fp, "chanx[%d][%d]_midout[%d] ", cb_x, cb_y, drive_rr_node->ptc_num);
    break;
  case CHANY:
    fprintf(fp, "chany[%d][%d]_midout[%d] ", cb_x, cb_y, drive_rr_node->ptc_num);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of drive_rr_node!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* End */
  fprintf(fp, "0\n");


  return;
}

void fprint_connection_box_mux(FILE* fp,
                               t_rr_type chan_type,
                               int cb_x,
                               int cb_y,
                               t_rr_node* src_rr_node) {
  int mux_size, cur_num_sram, ilevel;
  t_rr_node** drive_rr_nodes = NULL;
  int inode, mux_level, path_id, switch_index;
  t_spice_model* mux_spice_model = NULL;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  t_rr_type drive_rr_node_type = NUM_RR_TYPES;
  int xlow, ylow, offset, side;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > cb_x))&&(!(cb_x > (nx + 1)))); 
  assert((!(0 > cb_y))&&(!(cb_y > (ny + 1)))); 

  /* Find drive_rr_nodes*/
  mux_size = src_rr_node->num_drive_rr_nodes;
  drive_rr_nodes = src_rr_node->drive_rr_nodes; 

  /* Configuration bits for MUX*/
  path_id = -1;
  for (inode = 0; inode < mux_size; inode++) {
    if (drive_rr_nodes[inode] == &(rr_node[src_rr_node->prev_node])) {
      path_id = inode;
      break;
    }
  }
  assert((-1 != path_id)&&(path_id < mux_size));

  switch_index = src_rr_node->drive_switches[path_id];

  mux_spice_model = switch_inf[switch_index].spice_model;

  /* Call the MUX SPICE model */
  fprintf(fp, "X%s_size%d[%d] ", mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
  mux_spice_model->cnt++;
  /* Check drive_rr_nodes type, should be the same*/
  for (inode = 0; inode < mux_size; inode++) {
    if (NUM_RR_TYPES == drive_rr_node_type) { 
      drive_rr_node_type = drive_rr_nodes[inode]->type;
    } else {
      assert(drive_rr_node_type == drive_rr_nodes[inode]->type);
      assert((CHANX == drive_rr_nodes[inode]->type)||(CHANY == drive_rr_nodes[inode]->type));
    }
  } 
  /* input port*/
  for (inode = 0; inode < mux_size; inode++) {
    switch(drive_rr_nodes[inode]->type) {
    case CHANX:
      fprintf(fp, "chanx[%d][%d]_midout[%d] ", cb_x, cb_y, drive_rr_nodes[inode]->ptc_num);
      break;
    case CHANY:
      fprintf(fp, "chany[%d][%d]_midout[%d] ", cb_x, cb_y, drive_rr_nodes[inode]->ptc_num);
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of drive_rr_node!\n", __FILE__, __LINE__);
      exit(1);
    }
  }
  /* output port*/
  xlow = src_rr_node->xlow;
  ylow = src_rr_node->ylow;
  offset = grid[xlow][ylow].offset;
  switch(src_rr_node->type) {
  case IPIN:
  /* case SINK: */
    switch (drive_rr_node_type) {
    case CHANX:  
      assert(cb_x == xlow);
      if (cb_y == (ylow + offset)) { /* CHANX is on the TOP side of grid */
        side  = TOP;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x, cb_y);
      } else if ((cb_y + 1) == (ylow + offset)) {
        side  = BOTTOM;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x, cb_y + 1);
      } else {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid coordinator of cb_y(=%d) and ylow+offset(=%d)!\n", __FILE__, __LINE__, cb_y, ylow+offset);
        exit(1);
      }
      break;
    case CHANY: 
      assert(cb_y == (ylow + offset));
      if (cb_x == xlow) {
        side  = RIGHT;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x, cb_y);
      } else if ((cb_x + 1) == xlow) {
        side  = LEFT;
        fprint_grid_side_in_with_given_index(fp, src_rr_node->ptc_num, side, cb_x + 1, cb_y);
      } else {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid coordinator of cb_x(=%d) and xlow(=%d)!\n", __FILE__, __LINE__, cb_x, xlow);
        exit(1);
      }
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of drive_rr_node!\n", __FILE__, __LINE__);
      exit(1);
    }
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of src_rr_node!\n", __FILE__, __LINE__);
    exit(1);
  }

  switch (mux_spice_model->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    mux_level = determine_tree_mux_level(mux_size);
    num_mux_sram_bits = mux_level;
    mux_sram_bits = decode_tree_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    mux_level = 1;
    num_mux_sram_bits = mux_size;
    mux_sram_bits = decode_onelevel_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    mux_level = mux_spice_model->mux_num_level;
    num_mux_sram_bits = determine_num_input_basis_multilevel_mux(mux_size, mux_level) * mux_level;
    mux_sram_bits = decode_multilevel_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, mux_spice_model->name);
    exit(1);
  } 
 
  /* Print SRAMs that configure this MUX */
  /* TODO: What about RRAM-based MUX? */
  cur_num_sram = sram_spice_model->cnt;
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    /* Configure the SRAMs*/
    /* Pull Up/Down the SRAM outputs*/
    switch (mux_sram_bits[ilevel]) {
    case 0:
      /* Pull down power is considered as a part of subckt (CB or SB)*/
      fprintf(fp,"%s[%d]->out %s[%d]->outb ", 
            sram_spice_model->prefix, cur_num_sram, sram_spice_model->prefix, cur_num_sram);
      break;
    case 1:
      /* Pull down power is considered as a part of subckt (CB or SB)*/
      fprintf(fp,"%s[%d]->outb %s[%d]->out ", 
            sram_spice_model->prefix, cur_num_sram, sram_spice_model->prefix, cur_num_sram);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s,[LINE%d])Invalid sram_bit(=%d)! Should be [0|1].\n",
                 __FILE__, __LINE__, mux_sram_bits[ilevel]);
      exit(1);
    }
    cur_num_sram++;
  }

  /* End with svdd and sgnd, subckt name*/
  fprintf(fp, "svdd sgnd %s_size%d\n", mux_spice_model->name, mux_size);

  /* Print the encoding in SPICE netlist for debugging */
  fprintf(fp, "***** SRAM bits for MUX[%d], level=%d, select_path_id=%d. *****\n", 
          mux_spice_model->cnt, mux_level, path_id);
  fprintf(fp, "*****");
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprintf(fp, "%d", mux_sram_bits[ilevel]);
  }
  fprintf(fp, "*****\n");

  /* Call SRAM subckts*/
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprintf(fp, "X%s[%d] ", sram_spice_model->prefix, sram_spice_model->cnt);
    /*fprintf(fp, "%s[%d]->in ", sram_spice_model->prefix, sram_spice_model->cnt);*/
    fprintf(fp, "%s->in ", sram_spice_model->prefix); /* Input*/
    fprintf(fp, "%s[%d]->out ", sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, "%s[%d]->outb ", sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, "gvdd_sram_cbs sgnd %s\n", sram_spice_model->name);
    /* Add nodeset to help convergence */ 
    fprintf(fp, ".nodeset V(%s[%d]->out) 0\n", sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, ".nodeset V(%s[%d]->outb) vsp\n", sram_spice_model->prefix, sram_spice_model->cnt);
    sram_spice_model->cnt++;
  }

  /* Check SRAM counters */
  assert(cur_num_sram == sram_spice_model->cnt);

  /* Free */
  my_free(mux_sram_bits);

  return;
}

void fprint_connection_box_interc(FILE* fp,
                                  t_rr_type chan_type,
                                  int cb_x,
                                  int cb_y,
                                  t_rr_node* src_rr_node) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cb_x))&&(!(cb_x > (nx + 1)))); 
  assert((!(0 > cb_y))&&(!(cb_y > (ny + 1)))); 

  if (1 == src_rr_node->fan_in) {
    /* Print a direct connection*/
    fprint_connection_box_short_interc(fp, chan_type, cb_x, cb_y, src_rr_node);
  } else if (1 < src_rr_node->fan_in) {
    /* Print the multiplexer, fan_in >= 2 */
    fprint_connection_box_mux(fp, chan_type, cb_x, cb_y, src_rr_node);
  } /*Nothing should be done else*/ 
   
  return;
}

/* Print connection boxes
 * Print the sub-circuit of a connection Box (Type: [CHANX|CHANY])
 * Actually it is very similiar to switch box but
 * the difference is connection boxes connect Grid INPUT Pins to channels
 * TODO: merge direct connections into CB 
 *    --------------             --------------
 *    |            |             |            |
 *    |    Grid    |   ChanY     |    Grid    |
 *    |  [x][y+1]  |   [x][y]    | [x+1][y+1] |
 *    |            | Connection  |            |
 *    -------------- Box_Y[x][y] --------------
 *                   ----------
 *       ChanX       | Switch |        ChanX 
 *       [x][y]      |   Box  |       [x+1][y]
 *     Connection    | [x][y] |      Connection 
 *    Box_X[x][y]    ----------     Box_X[x+1][y]
 *    --------------             --------------
 *    |            |             |            |
 *    |    Grid    |  ChanY      |    Grid    |
 *    |   [x][y]   | [x][y-1]    |  [x+1][y]  |
 *    |            | Connection  |            |
 *    --------------Box_Y[x][y-1]--------------
 */
void fprint_routing_connection_box_subckt(FILE* fp,
                                          t_rr_type chan_type,
                                          int x,
                                          int y,
                                          int chan_width,
                                          t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side;
  int side_cnt = 0;
  int num_ipin_rr_node = 0;
  t_rr_node** ipin_rr_nodes = NULL;
  int num_temp_rr_node = 0;
  t_rr_node** temp_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  
  /* Print the definition of subckt*/
  fprintf(fp, ".subckt ");
  /* Identify the type of connection box */
  switch(chan_type) {
  case CHANX:
    fprintf(fp, "cbx[%d][%d] ", x, y);
    break;
  case CHANY:
    fprintf(fp, "cby[%d][%d] ", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
 
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  /* Print the ports of channels*/
  /*connect to the mid point of a track*/
  for (itrack = 0; itrack < chan_width; itrack++) {
    switch(chan_type) { 
    case CHANX:
      fprintf(fp, "chanx[%d][%d]_midout[%d] ", x, y, itrack);
      break;
    case CHANY:
      fprintf(fp, "chany[%d][%d]_midout[%d] ", x, y, itrack);
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
      exit(1);
    }
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }
  /* Print the ports of grids*/
  side_cnt = 0;
  num_ipin_rr_node = 0;  
  for (side = 0; side < 4; side++) {
    switch (side) {
    case 0: /* TOP */
      switch(chan_type) { 
      case CHANX:
        /* BOTTOM INPUT Pins of Grid[x][y+1] */
        fprint_grid_side_pins(fp, IPIN, x, y + 1, 2);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x, y + 1, 2, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break; 
      case CHANY:
        /* Nothing should be done */
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 1: /* RIGHT */
      switch(chan_type) { 
      case CHANX:
        /* Nothing should be done */
        break; 
      case CHANY:
        /* LEFT INPUT Pins of Grid[x+1][y] */
        fprint_grid_side_pins(fp, IPIN, x + 1, y, 3);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x + 1, y, 3, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 2: /* BOTTOM */
      switch(chan_type) { 
      case CHANX:
        /* TOP INPUT Pins of Grid[x][y] */
        fprint_grid_side_pins(fp, IPIN, x, y, 0);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x, y, 0, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break; 
      case CHANY:
        /* Nothing should be done */
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 3: /* LEFT */
      switch(chan_type) { 
      case CHANX:
        /* Nothing should be done */
        break; 
      case CHANY:
        /* RIGHT INPUT Pins of Grid[x][y] */
        fprint_grid_side_pins(fp, IPIN, x, y, 1);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x, y, 1, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index!\n", __FILE__, __LINE__);
      exit(1);
    }
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }
  /* Check */
  assert(2 == side_cnt);
  /* subckt definition ends with svdd and sgnd*/
  fprintf(fp, "svdd sgnd\n");


  /* Print multiplexers or direct interconnect*/
  for (inode = 0; inode < num_ipin_rr_node; inode++) {
    fprint_connection_box_interc(fp, ipin_rr_nodes[inode]->type, x, y, ipin_rr_nodes[inode]);
  } 

  fprintf(fp, ".eom\n");

  /* Free */
  my_free(ipin_rr_nodes);
 
  return;
}


/* Top Function*/
/* Build the routing resource SPICE sub-circuits*/
void generate_spice_routing_resources(char* subckt_dir,
                                      t_arch arch,
                                      t_det_routing_arch* routing_arch,
                                      t_ivec*** LL_rr_node_indices) {
  FILE* fp = NULL;
  char* sp_name = my_strcat(subckt_dir, routing_spice_file_name);
  int ix, iy; 
  int chan_width;
 
  assert(UNI_DIRECTIONAL == routing_arch->directionality);

  /* Create FILE */
  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE netlist %s",__FILE__, __LINE__, wires_spice_file_name); 
    exit(1);
  } 
  fprint_spice_head(fp,"Routing Resources");
  
  /* Two major tasks: 
   * 1. Generate sub-circuits for Routing Channels 
   * 2. Generate sub-circuits for Switch Boxes
   */ 
  /* Now: First task: Routing channels
   * Sub-circuits are named as chanx[ix][iy] or chany[ix][iy] for horizontal or vertical channels
   * each channels consist of a number of routing tracks. (Actually they are metal wires)
   * We only support single-driver routing architecture. 
   * The direction is defined as INC_DIRECTION ------> and DEC_DIRECTION <-------- for chanx
   * The direction is defined as INC_DIRECTION /|\ and DEC_DIRECTION | for chany
   *                                            |                    |
   *                                            |                    |
   *                                            |                   \|/
   * For INC_DIRECTION chanx, the inputs are at the left of channels, the outputs are at the right of channels
   * For DEC_DIRECTION chanx, the inputs are at the right of channels, the outputs are at the left of channels
   * For INC_DIRECTION chany, the inputs are at the bottom of channels, the outputs are at the top of channels
   * For DEC_DIRECTION chany, the inputs are at the top of channels, the outputs are at the bottom of channels
   */
  /* X - channels [1...nx][0..ny]*/
  vpr_printf(TIO_MESSAGE_INFO, "Writing X-direction Channels...\n");
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      chan_width = chan_width_x[iy];
      fprint_routing_chan_subckt(fp, ix, iy, CHANX, chan_width, LL_rr_node_indices, arch.num_segments, arch.Segments);
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  vpr_printf(TIO_MESSAGE_INFO, "Writing Y-direction Channels...\n");
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      chan_width = chan_width_y[ix];
      fprint_routing_chan_subckt(fp, ix, iy, CHANY, chan_width, LL_rr_node_indices, arch.num_segments, arch.Segments);
    }
  }

  /* Switch Boxes*/
  vpr_printf(TIO_MESSAGE_INFO, "Writing Switch Boxes...\n");
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      fprint_routing_switch_box_subckt(fp, ix, iy, LL_rr_node_indices);
    }
  }

  /* Connection Boxes */
  vpr_printf(TIO_MESSAGE_INFO, "Writing Connection Boxes...\n");
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      chan_width = chan_width_x[iy];
      fprint_routing_connection_box_subckt(fp, CHANX, ix, iy, chan_width, LL_rr_node_indices);
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      chan_width = chan_width_y[ix];
      fprint_routing_connection_box_subckt(fp, CHANY, ix, iy, chan_width, LL_rr_node_indices);
    }
  }
  
  /* Close the file*/
  fclose(fp);
  
  return;
}
