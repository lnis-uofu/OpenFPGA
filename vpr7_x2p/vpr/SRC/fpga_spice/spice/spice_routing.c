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

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_spice_globals.h"
#include "spice_globals.h"
#include "fpga_spice_utils.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_lut.h"
#include "spice_primitives.h"
#include "fpga_spice_backannotate_utils.h"
#include "spice_routing.h"


void fprint_routing_chan_subckt(char* subckt_dir,
                                int x, int y, t_rr_type chan_type, 
                                int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                t_ivec*** LL_rr_node_indices,
                                int num_segment, t_segment_inf* segments) {
  int itrack, iseg, cost_index;
  char* chan_prefix = NULL;
  int chan_width = 0;
  t_rr_node** chan_rr_nodes = NULL;
  FILE* fp = NULL;
  char* fname = NULL;
  
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert((CHANX == chan_type)||(CHANY == chan_type));

  /* Initial chan_prefix*/
  switch (chan_type) {
  case CHANX:
    /* Create file handler */
    fp = spice_create_one_subckt_file(subckt_dir, "Channel X-direction ", chanx_spice_file_name_prefix, x, y, &fname); 
    chan_prefix = "chanx";
    fprintf(fp, "***** Subckt for Channel X [%d][%d] *****\n", x, y);
    break;
  case CHANY:
    /* Create file handler */
    fp = spice_create_one_subckt_file(subckt_dir, "Channel Y-direction ", chany_spice_file_name_prefix, x, y, &fname); 
    chan_prefix = "chany";
    fprintf(fp, "***** Subckt for Channel Y [%d][%d] *****\n", x, y);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid Channel type! Should be CHANX or CHANY.\n",
               __FILE__, __LINE__);
    exit(1);
  }
  
  /* Collect rr_nodes for Tracks for chanx[ix][iy] */
  chan_rr_nodes = get_chan_rr_nodes(&chan_width, chan_type, x, y, 
                                    LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  /* Chan subckt definition */
  fprintf(fp, ".subckt %s[%d][%d] \n", chan_prefix, x, y);
  /* Inputs and outputs,
   * Rules for CHANX:
   * print left-hand ports(in) first, then right-hand ports(out)
   * Rules for CHANX:
   * print bottom ports(in) first, then top ports(out)
   */
  fprintf(fp, "+ ");
  /* LEFT/BOTTOM side port of CHANX/CHANY */
  for (itrack = 0; itrack < chan_width; itrack++) {
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "in%d ", itrack); /* INC_DIRECTION: input on the left/bottom side */
      break;
    case DEC_DIRECTION:
      fprintf(fp, "out%d ", itrack); /* DEC_DIRECTION: output on the left/bottom side*/
      break;
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of %s[%d][%d]_track[%d]!\n",
                 __FILE__, __LINE__, chan_prefix, x, y, itrack);
      exit(1);
    }
  }
  fprintf(fp, "\n");
  fprintf(fp, "+ ");
  /* RIGHT/TOP side port of CHANX/CHANY */
  for (itrack = 0; itrack < chan_width; itrack++) {
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "out%d ", itrack); /* INC_DIRECTION: output on the right/top side*/
      break;
    case DEC_DIRECTION:
      fprintf(fp, "in%d ", itrack); /* DEC_DIRECTION: input on the right/top side */
      break;
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of rr_node %s[%d][%d]_track[%d]!\n",
                 __FILE__, __LINE__, chan_prefix, x, y, itrack);
      exit(1);
    }
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

  /* Close the file*/
  fclose(fp);

  /* Add fname to the linked list */
  routing_spice_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_spice_subckt_file_path_head, fname);  

  /* Free */
  my_free(chan_rr_nodes);
  my_free(fname);
  
  return;
}

void fprint_grid_side_pin_with_given_index(FILE* fp,
                                           int pin_index, int side,
                                           int x, int y) {
  int height/*, class_id*/;
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
  height = get_grid_pin_height(x, y, pin_index);
  /* class_id = type->pin_class[pin_index]; */
  if ((1 == type->pinloc[height][side][pin_index])) {
    /* Not sure if we need to plus a height */
    /* fprintf(fp, "grid[%d][%d]_pin[%d][%d][%d] ", x, y, height, side, pin_index); */
    fprintf(fp, "grid[%d][%d]_pin[%d][%d][%d] ", x, y, height, side, pin_index);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Fail to print a grid pin (x=%d, y=%d, height=%d, side=%d, index=%d)\n",
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
  for (ipin = 0; ipin < type->num_pins; ipin++) {
    height = get_grid_pin_height(x, y, ipin);
    class_id = type->pin_class[ipin];
    if ((1 == type->pinloc[height][side][ipin])&&(pin_class_type == type->class_inf[class_id].type)) {
      /* Not sure if we need to plus a height */
      fprintf(fp, "grid[%d][%d]_pin[%d][%d][%d] ", x, y + height, height, side, ipin);
    }
  } 
  
  return;
}

void fprint_switch_box_chan_port(FILE* fp,
                                 t_sb cur_sb_info, 
                                 int chan_side,
                                 t_rr_node* cur_rr_node,
                                 enum PORTS cur_rr_node_direction) {
  int index = -1;
  t_rr_type chan_rr_node_type;
  int chan_rr_node_x, chan_rr_node_y;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get the index in sb_info of cur_rr_node */
  index = get_rr_node_index_in_sb_info(cur_rr_node, cur_sb_info, chan_side, cur_rr_node_direction);
  /* Make sure this node is included in this sb_info */
  assert((-1 != index)&&(-1 != chan_side));

  get_chan_rr_node_coorindate_in_sb_info(cur_sb_info, chan_side, 
                                         &chan_rr_node_type, &chan_rr_node_x, &chan_rr_node_y);

  assert(cur_rr_node->type == chan_rr_node_type);
     
  fprintf(fp, "%s[%d][%d]_%s[%d] ", 
          convert_chan_type_to_string(chan_rr_node_type),
          chan_rr_node_x, chan_rr_node_y, 
          convert_chan_rr_node_direction_to_string(cur_sb_info.chan_rr_node_direction[chan_side][index]),
          cur_rr_node->ptc_num);

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
                                    t_sb cur_sb_info,
                                    int chan_side,
                                    t_rr_node* cur_rr_node,
                                    int actual_fan_in,
                                    t_rr_node* drive_rr_node) {
  int side, index; 
  int grid_x, grid_y/*, height*/;
  char* chan_name = NULL;
  char* des_chan_port_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_sb_info.x))&&(!(cur_sb_info.x > (nx + 1)))); 
  assert((!(0 > cur_sb_info.y))&&(!(cur_sb_info.y > (ny + 1)))); 
  assert((0 == actual_fan_in)||(1 == actual_fan_in));

  chan_name = convert_chan_type_to_string(cur_rr_node->type);
  
  /* Get the index in sb_info of cur_rr_node */
  index = get_rr_node_index_in_sb_info(cur_rr_node, cur_sb_info, chan_side, OUT_PORT);
  des_chan_port_name = "out"; 
  
  fprintf(fp, "R%s[%d][%d]_%s[%d] ", 
          chan_name, cur_sb_info.x, cur_sb_info.y, des_chan_port_name, cur_rr_node->ptc_num);

  /* Check the driver*/
  if (0 == actual_fan_in) {
    assert(drive_rr_node == cur_rr_node);
  } else {
    /* drive_rr_node = &(rr_node[cur_rr_node->prev_node]); */
    assert(1 == rr_node_drive_switch_box(drive_rr_node, cur_rr_node, cur_sb_info.x, cur_sb_info.y, chan_side));
  }
  switch (drive_rr_node->type) {
  /* case SOURCE: */
  case OPIN:
    /* Indicate a CLB Outpin*/
    /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
    get_rr_node_side_and_index_in_sb_info(drive_rr_node, cur_sb_info, IN_PORT, &side, &index);
    /* We need to be sure that drive_rr_node is part of the SB */
    assert((-1 != index)&&(-1 != side));
    /* Find grid_x and grid_y */
    grid_x = drive_rr_node->xlow; 
    grid_y = drive_rr_node->ylow; /*Plus the offset in function fprint_grid_side_pin_with_given_index */
    /*height = grid[grid_x][grid_y].offset;*/
    /* Print a grid pin */
    fprint_grid_side_pin_with_given_index(fp, drive_rr_node->ptc_num, 
                                          cur_sb_info.opin_rr_node_grid_side[side][index],
                                          grid_x, grid_y);
    break;
  case CHANX:
  case CHANY:
    /* Should be an input */
    if (cur_rr_node == drive_rr_node) {
      /* To be strict, the input should locate on the opposite side. 
       * Use the else part if this may change in some architecture.
       */
      side = get_opposite_side(chan_side); 
      index = get_rr_node_index_in_sb_info(drive_rr_node, cur_sb_info, side, IN_PORT);
    } else {
      get_rr_node_side_and_index_in_sb_info(drive_rr_node, cur_sb_info, IN_PORT, &side, &index);
    }
    /* We need to be sure that drive_rr_node is part of the SB */
    assert((-1 != index)&&(-1 != side));
    fprint_switch_box_chan_port(fp, cur_sb_info, side, drive_rr_node, IN_PORT);
    break;
  default: /* IPIN, SINK are invalid*/
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Output port */
  /* fprint_switch_box_chan_port(fp, switch_box_x, switch_box_y, chan_side, cur_rr_node); */
  fprint_switch_box_chan_port(fp, cur_sb_info, chan_side, cur_rr_node, OUT_PORT);

  /* END */
  fprintf(fp, "0\n");

  return;
}

/* Print the SPICE netlist of multiplexer that drive this rr_node */
void fprint_switch_box_mux(FILE* fp, 
                           t_sb cur_sb_info, 
                           int chan_side,
                           t_rr_node* cur_rr_node,
                           int mux_size,
                           t_rr_node** drive_rr_nodes,
                           int switch_index) {
  int inode, side, index;
  int grid_x, grid_y/*, height*/;
  t_spice_model* spice_model = NULL;
  int mux_level, path_id, cur_num_sram, ilevel;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  char* sram_vdd_port_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_sb_info.x))&&(!(cur_sb_info.x > (nx + 1)))); 
  assert((!(0 > cur_sb_info.y))&&(!(cur_sb_info.y > (ny + 1)))); 

  /* Check current rr_node is CHANX or CHANY*/
  assert((CHANX == cur_rr_node->type)||(CHANY == cur_rr_node->type));
  
  /* Allocate drive_rr_nodes according to the fan-in*/
  assert((2 == mux_size)||(2 < mux_size));

  /* Get spice model*/
  spice_model = switch_inf[switch_index].spice_model;
  /* Now it is the time print the SPICE netlist of MUX*/
  fprintf(fp, "X%s_size%d[%d] ", spice_model->prefix, mux_size, spice_model->cnt);
  /* Global ports */
  if (0 < rec_fprint_spice_model_global_ports(fp, spice_model, FALSE)) { 
    fprintf(fp, "+ ");
  }
  /* Input ports*/
  for (inode = 0; inode < mux_size; inode++) {
    switch (drive_rr_nodes[inode]->type) {
    /* case SOURCE: */
    case OPIN:
      /* Indicate a CLB Outpin*/
      /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
      get_rr_node_side_and_index_in_sb_info(drive_rr_nodes[inode], cur_sb_info, IN_PORT, &side, &index);
      /* We need to be sure that drive_rr_node is part of the SB */
      assert((-1 != index)&&(-1 != side));
      /* Find grid_x and grid_y */
      grid_x = drive_rr_nodes[inode]->xlow; 
      grid_y = drive_rr_nodes[inode]->ylow; /*Plus the offset in function fprint_grid_side_pin_with_given_index */
      /* height = grid[grid_x][grid_y].offset; */
      /* Print a grid pin */
      fprint_grid_side_pin_with_given_index(fp, drive_rr_nodes[inode]->ptc_num, 
                                           cur_sb_info.opin_rr_node_grid_side[side][index],
                                           grid_x, grid_y);
      break;
    case CHANX:
    case CHANY:
      /* Should be an input! */
      get_rr_node_side_and_index_in_sb_info(drive_rr_nodes[inode], cur_sb_info, IN_PORT, &side, &index);
      /* We need to be sure that drive_rr_node is part of the SB */
      if (!((-1 != index)&&(-1 != side))) {
      assert((-1 != index)&&(-1 != side));
      }
      fprint_switch_box_chan_port(fp, cur_sb_info, side, drive_rr_nodes[inode], IN_PORT);
      break;
    default: /* IPIN, SINK are invalid*/
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  /* Output port */
  fprint_switch_box_chan_port(fp, cur_sb_info, chan_side, cur_rr_node, OUT_PORT);

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

  switch (spice_model->design_tech_info.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    mux_level = determine_tree_mux_level(mux_size);
    num_mux_sram_bits = mux_level;
    mux_sram_bits = decode_tree_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    mux_level = 1;
    /* Special for 2-input MUX */
    if (2 == mux_size) {
      num_mux_sram_bits = 1;
      mux_sram_bits = decode_tree_mux_sram_bits(mux_size, mux_level, path_id); 
    } else {
      num_mux_sram_bits = mux_size;
      mux_sram_bits = decode_onelevel_mux_sram_bits(mux_size, mux_level, path_id); 
    }
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    /* Take care of corner case: MUX size = 2 */
    if (2 == mux_size) {
      mux_level = 1;
      num_mux_sram_bits = 1;
      mux_sram_bits = decode_tree_mux_sram_bits(mux_size, 1, path_id); 
    } else {
      mux_level = spice_model->design_tech_info.mux_num_level;
      num_mux_sram_bits = determine_num_input_basis_multilevel_mux(mux_size, mux_level) * mux_level;
      mux_sram_bits = decode_multilevel_mux_sram_bits(mux_size, mux_level, path_id); 
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model->name);
    exit(1);
  } 
  /* Print SRAMs that configure this MUX */
  /* Get current counter of mem_bits, bl and wl */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_spice_orgz_info); 
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    assert( (0 == mux_sram_bits[ilevel]) || (1 == mux_sram_bits[ilevel]) );
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, 
                                  cur_num_sram + ilevel, mux_sram_bits[ilevel]);
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, 
                                  cur_num_sram + ilevel, 1 - mux_sram_bits[ilevel]);
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
  /* Give the VDD port name for SRAMs */
  sram_vdd_port_name = (char*)my_malloc(sizeof(char)*
                                       (strlen(spice_tb_global_vdd_sb_sram_port_name) 
                                        + 1 ));
  sprintf(sram_vdd_port_name, "%s",
                              spice_tb_global_vdd_sb_sram_port_name);
  /* Now Print SRAMs one by one */
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprint_spice_one_sram_subckt(fp, sram_spice_orgz_info, 
                                 spice_model, sram_vdd_port_name);
  }

  /* Store the configuraion bit to linked-list */
  add_sram_conf_bits_to_llist(sram_spice_orgz_info, cur_num_sram,
                              num_mux_sram_bits, mux_sram_bits);

  /* Update spice_model counter */
  spice_model->cnt++;

  /* Free */
  my_free(mux_sram_bits);
  my_free(sram_vdd_port_name);

  return;
}

void fprint_switch_box_interc(FILE* fp, 
                              t_sb cur_sb_info,
                              int chan_side,
                              t_rr_node* cur_rr_node) {
  int sb_x, sb_y;
  int num_drive_rr_nodes = 0;  
  t_rr_node** drive_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  sb_x = cur_sb_info.x;
  sb_y = cur_sb_info.y;

  /* Check */
  assert((!(0 > sb_x))&&(!(sb_x > (nx + 1)))); 
  assert((!(0 > sb_y))&&(!(sb_y > (ny + 1)))); 

  /* Check each num_drive_rr_nodes, see if they appear in the cur_sb_info */
  if (TRUE == check_drive_rr_node_imply_short(cur_sb_info, cur_rr_node, chan_side)) {
    /* Double check if the interc lies inside a channel wire, that is interc between segments */
    assert(1 == is_rr_node_exist_opposite_side_in_sb_info(cur_sb_info, cur_rr_node, chan_side));
    num_drive_rr_nodes = 0;
    drive_rr_nodes = NULL;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
    drive_rr_nodes = cur_rr_node->drive_rr_nodes;
  }

  if (0 == num_drive_rr_nodes) {
    /* Print a special direct connection*/
    fprint_switch_box_short_interc(fp, cur_sb_info, chan_side, cur_rr_node, 
                                   num_drive_rr_nodes, cur_rr_node);
  } else if (1 == num_drive_rr_nodes) {
    /* Print a direct connection*/
    fprint_switch_box_short_interc(fp, cur_sb_info, chan_side, cur_rr_node, 
                                   num_drive_rr_nodes, drive_rr_nodes[0]);
  } else if (1 < num_drive_rr_nodes) {
    /* Print the multiplexer, fan_in >= 2 */
    fprint_switch_box_mux(fp, cur_sb_info, chan_side, cur_rr_node, 
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
 * For channels chanY with INC_DIRECTION on the top side, they should be marked as outputs
 * For channels chanY with DEC_DIRECTION on the top side, they should be marked as inputs
 * For channels chanY with INC_DIRECTION on the bottom side, they should be marked as inputs
 * For channels chanY with DEC_DIRECTION on the bottom side, they should be marked as outputs
 * For channels chanX with INC_DIRECTION on the left side, they should be marked as inputs
 * For channels chanX with DEC_DIRECTION on the left side, they should be marked as outputs
 * For channels chanX with INC_DIRECTION on the right side, they should be marked as outputs
 * For channels chanX with DEC_DIRECTION on the right side, they should be marked as inputs
 */
void fprint_routing_switch_box_subckt(char* subckt_dir, 
                                      t_sb cur_sb_info,
                                      int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side, ix, iy, x, y;
  FILE* fp = NULL;
  char* fname = NULL;

  /* Check */
  assert((!(0 > cur_sb_info.x))&&(!(cur_sb_info.x > (nx + 1)))); 
  assert((!(0 > cur_sb_info.y))&&(!(cur_sb_info.y > (ny + 1)))); 

  /* Create file handler */
  fp = spice_create_one_subckt_file(subckt_dir, "Switch Block ", sb_spice_file_name_prefix, cur_sb_info.x, cur_sb_info.y, &fname); 

  x = cur_sb_info.x;
  y = cur_sb_info.y;

  /* Print the definition of subckt*/
  fprintf(fp, "***** Switch Box[%d][%d] Sub-Circuit *****\n", cur_sb_info.x, cur_sb_info.y);
  fprintf(fp, ".subckt sb[%d][%d] ", cur_sb_info.x, cur_sb_info.y);
  fprintf(fp, "\n");
  for (side = 0; side < cur_sb_info.num_sides; side++) {
    fprintf(fp, "***** Inputs/outputs of %s side *****\n",convert_side_index_to_string(side));
    determine_sb_port_coordinator(cur_sb_info, side, &ix, &iy); 
    fprintf(fp, "+ ");

    for (itrack = 0; itrack < cur_sb_info.chan_width[side]; itrack++) {
      switch (cur_sb_info.chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        fprintf(fp, "%s[%d][%d]_out[%d] ", 
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack); 
        break;
      case IN_PORT:
        fprintf(fp, "%s[%d][%d]_in[%d] ",
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack); 
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of port sb[%d][%d] Channel node[%d] track[%d]!\n",
                   __FILE__, __LINE__, cur_sb_info.x, cur_sb_info.y, side, itrack);
        exit(1);
      }
    }
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
    /* Dump OPINs of adjacent CLBs */
    for (inode = 0; inode < cur_sb_info.num_opin_rr_nodes[side]; inode++) {
      fprint_grid_side_pin_with_given_index(fp, cur_sb_info.opin_rr_node[side][inode]->ptc_num,
                                            cur_sb_info.opin_rr_node_grid_side[side][inode],
                                            cur_sb_info.opin_rr_node[side][inode]->xlow,
                                            cur_sb_info.opin_rr_node[side][inode]->ylow); 
    } 
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }
  
  /* Local Vdd and Gnd */
  fprintf(fp, "svdd sgnd\n");

  /* Specify the head of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Head of scan-chain *****\n");
    fprintf(fp, "Rsb[%d][%d]_sc_head sb[%d][%d]_sc_head %s[%d]->in 0\n",
            x, y, x, y, sram_spice_model->prefix, sram_spice_model->cnt);
  }

  /* Put down all the multiplexers */
  for (side = 0; side < cur_sb_info.num_sides; side++) {
    fprintf(fp, "***** %s side Multiplexers *****\n", 
            convert_side_index_to_string(side));
    for (itrack = 0; itrack < cur_sb_info.chan_width[side]; itrack++) {
      assert((CHANX == cur_sb_info.chan_rr_node[side][itrack]->type)
           ||(CHANY == cur_sb_info.chan_rr_node[side][itrack]->type));
      /* We care INC_DIRECTION tracks at this side*/
      if (OUT_PORT != cur_sb_info.chan_rr_node_direction[side][itrack]) {
        continue;
      }
      fprint_switch_box_interc(fp, cur_sb_info, side, cur_sb_info.chan_rr_node[side][itrack]);
    }
  }

  /* Specify the tail of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Tail of scan-chain *****\n");
    fprintf(fp, "Rsb[%d][%d]_sc_tail sb[%d][%d]_sc_tail %s[%d]->in 0\n",
            x, y, x, y, sram_spice_model->prefix, sram_spice_model->cnt);
  }
 
  fprintf(fp, ".eom\n");

  /* Close the file*/
  fclose(fp);

  /* Add fname to the linked list */
  routing_spice_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_spice_subckt_file_path_head, fname);  

  /* Free */
  my_free(fname);

  return;
}

/* SRC rr_node is the IPIN of a grid.*/
void fprint_connection_box_short_interc(FILE* fp,
                                        t_cb cur_cb_info,
                                        t_rr_node* src_rr_node) {
  t_rr_node* drive_rr_node = NULL;
  int iedge, check_flag;
  int xlow, ylow, height, side, index;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 
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
  switch(cur_cb_info.type) {
  case CHANX:
    fprintf(fp, "Rcbx[%d][%d]_grid[%d][%d]_pin[%d] ", 
            cur_cb_info.x, cur_cb_info.y, 
            xlow, ylow + height, src_rr_node->ptc_num);
    break;
  case CHANY:
    fprintf(fp, "Rcby[%d][%d]_grid[%d][%d]_pin[%d] ", 
            cur_cb_info.x, cur_cb_info.y, 
            xlow, ylow + height, src_rr_node->ptc_num);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
  /* Input port*/
  assert(IPIN == src_rr_node->type);
  /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
  get_rr_node_side_and_index_in_cb_info(src_rr_node, cur_cb_info, OUT_PORT, &side, &index);
  /* We need to be sure that drive_rr_node is part of the SB */
  assert((-1 != index)&&(-1 != side));
  fprint_grid_side_pin_with_given_index(fp, cur_cb_info.ipin_rr_node[side][index]->ptc_num, 
                                        cur_cb_info.ipin_rr_node_grid_side[side][index], 
                                        xlow, ylow);
  
  /* output port -- > connect to the output at middle point of a channel */
  fprintf(fp, "%s[%d][%d]_midout[%d] ", 
          convert_chan_type_to_string(drive_rr_node->type),
          cur_cb_info.x, cur_cb_info.y, drive_rr_node->ptc_num);

  /* End */
  fprintf(fp, "0\n");


  return;
}

void fprint_connection_box_mux(FILE* fp,
                               t_cb cur_cb_info,
                               t_rr_node* src_rr_node) {
  int mux_size, cur_num_sram, ilevel;
  t_rr_node** drive_rr_nodes = NULL;
  int inode, mux_level, path_id, switch_index;
  t_spice_model* mux_spice_model = NULL;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  t_rr_type drive_rr_node_type = NUM_RR_TYPES;
  int xlow, ylow, /* offset,*/ side, index;
  char* sram_vdd_port_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 

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

  /* Global ports */
  if (0 < rec_fprint_spice_model_global_ports(fp, mux_spice_model, FALSE)) { 
    fprintf(fp, "+ ");
  }

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
    fprintf(fp, "%s[%d][%d]_midout[%d] ", 
            convert_chan_type_to_string(drive_rr_nodes[inode]->type),
            cur_cb_info.x, cur_cb_info.y, drive_rr_nodes[inode]->ptc_num);
  }
  /* output port*/
  xlow = src_rr_node->xlow;
  ylow = src_rr_node->ylow;
  /* offset = grid[xlow][ylow].offset; */

  assert(IPIN == src_rr_node->type);
  /* Search all the sides of a CB, see this drive_rr_node is an INPUT of this SB */
  get_rr_node_side_and_index_in_cb_info(src_rr_node, cur_cb_info, OUT_PORT, &side, &index);
  /* We need to be sure that drive_rr_node is part of the CB */
  assert((-1 != index)&&(-1 != side));
  fprint_grid_side_pin_with_given_index(fp, cur_cb_info.ipin_rr_node[side][index]->ptc_num, 
                                        cur_cb_info.ipin_rr_node_grid_side[side][index], 
                                        xlow, ylow);

  switch (mux_spice_model->design_tech_info.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    mux_level = determine_tree_mux_level(mux_size);
    num_mux_sram_bits = mux_level;
    mux_sram_bits = decode_tree_mux_sram_bits(mux_size, mux_level, path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    mux_level = 1;
    /* Special for 2-input MUX */
    if (2 == mux_size) {
      num_mux_sram_bits = 1;
      mux_sram_bits = decode_tree_mux_sram_bits(mux_size, mux_level, path_id); 
    } else {
      num_mux_sram_bits = mux_size;
      mux_sram_bits = decode_onelevel_mux_sram_bits(mux_size, mux_level, path_id); 
    }
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    /* Take care of corner case: MUX size = 2 */
    if (2 == mux_size) {
      num_mux_sram_bits = 1;
      mux_sram_bits = decode_tree_mux_sram_bits(mux_size, 1, path_id); 
    } else {
      mux_level = mux_spice_model->design_tech_info.mux_num_level;
      num_mux_sram_bits = determine_num_input_basis_multilevel_mux(mux_size, mux_level) * mux_level;
      mux_sram_bits = decode_multilevel_mux_sram_bits(mux_size, mux_level, path_id); 
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, mux_spice_model->name);
    exit(1);
  } 
 
  /* Print SRAMs that configure this MUX */
  /* Get current counter of mem_bits, bl and wl */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_spice_orgz_info); 
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    assert( (0 == mux_sram_bits[ilevel]) || (1 == mux_sram_bits[ilevel]) );
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, 
                                  cur_num_sram + ilevel, mux_sram_bits[ilevel]);
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, 
                                  cur_num_sram + ilevel, 1 - mux_sram_bits[ilevel]);
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
  /* Give the VDD port name for SRAMs */
  sram_vdd_port_name = (char*)my_malloc(sizeof(char)*
                                       (strlen(spice_tb_global_vdd_cb_sram_port_name) 
                                        + 1 ));
  sprintf(sram_vdd_port_name, "%s",
                              spice_tb_global_vdd_cb_sram_port_name);
  /* Now Print SRAMs one by one */
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprint_spice_one_sram_subckt(fp, sram_spice_orgz_info, 
                                 mux_spice_model, sram_vdd_port_name);
  }

  /* Store the configuraion bit to linked-list */
  add_sram_conf_bits_to_llist(sram_spice_orgz_info, cur_num_sram,
                              num_mux_sram_bits, mux_sram_bits);

  /* Update spice_model counter */
  mux_spice_model->cnt++;

  /* Free */
  my_free(mux_sram_bits);
  my_free(sram_vdd_port_name);

  return;
}

void fprint_connection_box_interc(FILE* fp,
                                  t_cb cur_cb_info,
                                  t_rr_node* src_rr_node) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 

  if (1 == src_rr_node->fan_in) {
    /* Print a direct connection*/
    fprint_connection_box_short_interc(fp, cur_cb_info, src_rr_node);
  } else if (1 < src_rr_node->fan_in) {
    /* Print the multiplexer, fan_in >= 2 */
    fprint_connection_box_mux(fp, cur_cb_info, src_rr_node);
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
void fprint_routing_connection_box_subckt(char* subckt_dir,
                                          t_cb cur_cb_info,
                                          int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                          t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side, x, y;
  int side_cnt = 0;
  FILE* fp = NULL; 
  char* fname = NULL;

  /* Identify the type of connection box 
   * Create file handler 
   */
  switch(cur_cb_info.type) {
  case CHANX:
    fp = spice_create_one_subckt_file(subckt_dir, "Connection Block X-channel ", cbx_spice_file_name_prefix, cur_cb_info.x, cur_cb_info.y, &fname); 
    break;
  case CHANY:
    fp = spice_create_one_subckt_file(subckt_dir, "Connection Block Y-channel ", cby_spice_file_name_prefix, cur_cb_info.x, cur_cb_info.y, &fname); 
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of connection box!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 

  x = cur_cb_info.x;
  y = cur_cb_info.y;
  
  /* Print the definition of subckt*/
  fprintf(fp, ".subckt ");
  /* Identify the type of connection box */
  switch(cur_cb_info.type) {
  case CHANX:
    fprintf(fp, "cbx[%d][%d] ", cur_cb_info.x, cur_cb_info.y);
    break;
  case CHANY:
    fprintf(fp, "cby[%d][%d] ", cur_cb_info.x, cur_cb_info.y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of connection box!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
  fprintf(fp, "\n");

  /* Print the ports of channels*/
  /*connect to the mid point of a track*/
  /* Get the chan_rr_nodes: Only one side of a cb_info has chan_rr_nodes*/
  side_cnt = 0;
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero channel width */
    if (0 == cur_cb_info.chan_width[side]) {
      continue;
    }
    assert (0 < cur_cb_info.chan_width[side]);
    side_cnt++;
    for (itrack = 0; itrack < cur_cb_info.chan_width[side]; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "%s[%d][%d]_midout[%d] ", 
              convert_chan_type_to_string(cur_cb_info.type),
              cur_cb_info.x, cur_cb_info.y, itrack);
      fprintf(fp, "\n");
    }
  }
  /*check side_cnt */
  assert(1 == side_cnt);

  side_cnt = 0;
  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info.num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info.num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info.ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info.num_ipin_rr_nodes[side]; inode++) {
      fprintf(fp, "+ ");
      /* Print each INPUT Pins of a grid */
      fprint_grid_side_pin_with_given_index(fp, cur_cb_info.ipin_rr_node[side][inode]->ptc_num,
                                            cur_cb_info.ipin_rr_node_grid_side[side][inode],
                                            cur_cb_info.ipin_rr_node[side][inode]->xlow,
                                            cur_cb_info.ipin_rr_node[side][inode]->ylow); 
      fprintf(fp, "\n");
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));

  /* subckt definition ends with svdd and sgnd*/
  fprintf(fp, "+ ");
  fprintf(fp, "svdd sgnd\n");

  /* Specify the head of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    switch(cur_cb_info.type) {
    case CHANX:
      fprintf(fp, "***** Head of scan-chain *****\n");
      fprintf(fp, "Rcbx[%d][%d]_sc_head cbx[%d][%d]_sc_head %s[%d]->in 0\n",
              x, y, x, y, sram_spice_model->prefix, sram_spice_model->cnt);
    case CHANY:
      fprintf(fp, "***** Head of scan-chain *****\n");
      fprintf(fp, "Rcby[%d][%d]_sc_head cby[%d][%d]_sc_head %s[%d]->in 0\n",
              x, y, x, y, sram_spice_model->prefix, sram_spice_model->cnt);
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
      exit(1);
    }
  }

  /* Print multiplexers or direct interconnect,
   * According to the rr_node lists in cbx_info or cby_info 
   */
  side_cnt = 0;
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info.num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info.num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info.ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info.num_ipin_rr_nodes[side]; inode++) { 
      fprint_connection_box_interc(fp, cur_cb_info, cur_cb_info.ipin_rr_node[side][inode]);
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));

  /* Specify the tail of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    switch(cur_cb_info.type) {
    case CHANX:
      fprintf(fp, "***** Tail of scan-chain *****\n");
      fprintf(fp, "Rcbx[%d][%d]_sc_tail cbx[%d][%d]_sc_tail %s[%d]->in 0\n",
              x, y, x, y, sram_spice_model->prefix, sram_spice_model->cnt);
    case CHANY:
      fprintf(fp, "***** Tail of scan-chain *****\n");
      fprintf(fp, "Rcby[%d][%d]_sc_tail cby[%d][%d]_sc_tail %s[%d]->in 0\n",
              x, y, x, y, sram_spice_model->prefix, sram_spice_model->cnt);
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
      exit(1);
    }
  }

  fprintf(fp, ".eom\n");

  /* Close the file*/
  fclose(fp);

  /* Add fname to the linked list */
  routing_spice_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_spice_subckt_file_path_head, fname);  

  /* Free */
  my_free(fname);

  return;
}


/* Top Function*/
/* Build the routing resource SPICE sub-circuits*/
void generate_spice_routing_resources(char* subckt_dir,
                                      t_arch arch,
                                      t_det_routing_arch* routing_arch,
                                      int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices) {
  int ix, iy; 
 
  assert(UNI_DIRECTIONAL == routing_arch->directionality);
  
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
      fprint_routing_chan_subckt(subckt_dir, ix, iy, CHANX, 
                                 LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                 arch.num_segments, arch.Segments);
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  vpr_printf(TIO_MESSAGE_INFO, "Writing Y-direction Channels...\n");
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      fprint_routing_chan_subckt(subckt_dir, ix, iy, CHANY, 
                                 LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, 
                                 arch.num_segments, arch.Segments);
    }
  }

  /* Switch Boxes*/
  vpr_printf(TIO_MESSAGE_INFO, "Writing Switch Boxes...\n");
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      update_spice_models_routing_index_low(ix, iy, SOURCE, arch.spice->num_spice_model, arch.spice->spice_models);
      fprint_routing_switch_box_subckt(subckt_dir, sb_info[ix][iy],
                                       LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices); 
      update_spice_models_routing_index_high(ix, iy, SOURCE, arch.spice->num_spice_model, arch.spice->spice_models);
    }
  }

  /* Connection Boxes */
  vpr_printf(TIO_MESSAGE_INFO, "Writing Connection Boxes...\n");
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      update_spice_models_routing_index_low(ix, iy, CHANX, arch.spice->num_spice_model, arch.spice->spice_models);
      /* Check if this cby_info exists, it may be covered by a heterogenous block */
      if ((TRUE == is_cb_exist(CHANX, ix, iy)) 
         &&(0 < count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy]))) {
        fprint_routing_connection_box_subckt(subckt_dir, cbx_info[ix][iy],
                                             LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices); 
      }
      update_spice_models_routing_index_high(ix, iy, CHANX, arch.spice->num_spice_model, arch.spice->spice_models);
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      update_spice_models_routing_index_low(ix, iy, CHANY, arch.spice->num_spice_model, arch.spice->spice_models);
      /* Check if this cby_info exists, it may be covered by a heterogenous block */
      if ((TRUE == is_cb_exist(CHANY, ix, iy))
         &&(0 < count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
        fprint_routing_connection_box_subckt(subckt_dir, cby_info[ix][iy],
                                             LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices); 
      }
      update_spice_models_routing_index_high(ix, iy, CHANY, arch.spice->num_spice_model, arch.spice->spice_models);
    }
  }
  
  /* Output a header file for all the routing blocks */
  vpr_printf(TIO_MESSAGE_INFO,"Generating header file for routing submodules...\n");
  spice_print_subckt_header_file(routing_spice_subckt_file_path_head,
                                 subckt_dir,
                                 routing_spice_file_name);
  return;
}

