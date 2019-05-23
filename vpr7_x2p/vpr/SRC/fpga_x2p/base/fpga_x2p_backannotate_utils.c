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
#include "route_common.h"

/* Include spice support headers*/
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "rr_blocks.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_rr_graph_utils.h"
#include "fpga_x2p_lut_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_pb_rr_graph.h"
#include "fpga_x2p_router.h"
#include "fpga_x2p_unique_routing.h"

/* Get initial value of a Latch/FF output*/
int get_ff_output_init_val(t_logical_block* ff_logical_block) {
  assert((0 == ff_logical_block->init_val)||(1 == ff_logical_block->init_val));  

  return ff_logical_block->init_val;
}

int determine_rr_node_default_prev_node(t_rr_node* cur_rr_node) {
  int default_prev_node = DEFAULT_PREV_NODE;

  /* Judge if the prev_node should be */
  if ((NULL != switch_inf[cur_rr_node->driver_switch].spice_model) 
     && (TRUE == switch_inf[cur_rr_node->driver_switch].spice_model->design_tech_info.mux_info->add_const_input)) {
    default_prev_node = OPEN; /* The constant input will be the last input!!! */
  } 

  return default_prev_node;
}

/* Get initial value of a mapped  LUT output*/
int get_lut_output_init_val(t_logical_block* lut_logical_block) {
  int i;
  int* sram_bits = NULL; /* decoded SRAM bits */ 
  int truth_table_length = 0;
  char** truth_table = NULL;
  int lut_size = 0;
  int input_net_index = OPEN;
  int* input_init_val = NULL;
  int init_path_id = 0;
  int output_init_val = 0;

  t_spice_model* lut_spice_model = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == lut_logical_block) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid LUT logical block!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Get SPICE model */
  assert((NULL != lut_logical_block->pb)
        && ( NULL != lut_logical_block->pb->pb_graph_node)
        && ( NULL != lut_logical_block->pb->pb_graph_node->pb_type));
  lut_spice_model = lut_logical_block->pb->pb_graph_node->pb_type->parent_mode->parent_pb_type->phy_pb_type->spice_model;

  assert(SPICE_MODEL_LUT == lut_spice_model->type);

  sram_ports = find_spice_model_ports(lut_spice_model, SPICE_MODEL_PORT_SRAM, 
                                      &num_sram_port, TRUE);
  assert((1 == num_sram_port) || (2 == num_sram_port));

  /* Get the truth table */
  truth_table = assign_lut_truth_table(lut_logical_block, &truth_table_length); 
  lut_size = lut_logical_block->used_input_pins;
  assert(!(0 > lut_size));
  /* Special for LUT_size = 0 */
  if (0 == lut_size) {
    /* Generate sram bits*/
    sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, 
                                       1, sram_ports[0]->default_val);
    /* This is constant generator, SRAM bits should be the same */
    output_init_val = sram_bits[0];
    for (i = 0; i < (int)pow(2.,(double)lut_size); i++) { 
      assert(sram_bits[i] == output_init_val);
    } 
  } else { 
    /* Generate sram bits*/
    sram_bits = generate_lut_sram_bits(truth_table_length, truth_table,
                                       lut_size, sram_ports[0]->default_val);

    assert(1 == lut_logical_block->pb->pb_graph_node->num_input_ports);
    assert(1 == lut_logical_block->pb->pb_graph_node->num_output_ports);
    /* Get the initial path id */
    input_init_val = (int*)my_malloc(sizeof(int)*lut_size);
    for (i = 0; i < lut_size; i++) {
      input_net_index = lut_logical_block->input_nets[0][i]; 
      input_init_val[i] = vpack_net[input_net_index].spice_net_info->init_val;
      if ((1 != input_init_val[i]) && (0 != input_init_val[i])) {
        assert ((1 == input_init_val[i]) || (0 == input_init_val[i]));
      }
    } 
    
    init_path_id = determine_lut_path_id(lut_size, input_init_val);
    /* Check */  
    assert((!(0 > init_path_id))&&(init_path_id < (int)pow(2.,(double)lut_size)));
    output_init_val = sram_bits[init_path_id]; 
  }

  /* Check */
  if ((1 != output_init_val) && (0 != output_init_val)) {
    assert ((1 == output_init_val) || (0 == output_init_val));
  }
   
  /*Free*/
  for (i = 0; i < truth_table_length; i++) {
    free(truth_table[i]);
  }
  free(truth_table);
  my_free(sram_bits);
  my_free(input_init_val);

  return output_init_val;
}

/* Deteremine the initial value of an output of a logical block 
 * The logical block could be a LUT, a memory block or a multiplier 
 */
int get_logical_block_output_init_val(t_logical_block* cur_logical_block) {
  int output_init_val = 0;
  t_spice_model* cur_spice_model = NULL;

  /* Get the spice_model of current logical_block */
  assert((NULL != cur_logical_block->pb)
        && ( NULL != cur_logical_block->pb->pb_graph_node)
        && ( NULL != cur_logical_block->pb->pb_graph_node->pb_type));

  /* We only care LUT here, for other blocks we cannot force now, for others, we just give zero. */
  if (0 == strcmp(cur_logical_block->model->name, BLIF_LUT_KEYWORD)) {
    cur_spice_model = cur_logical_block->pb->pb_graph_node->pb_type->parent_mode->parent_pb_type->phy_pb_type->spice_model;
  } else {
    return get_ff_output_init_val(cur_logical_block);
  }

  /* Switch to specific cases*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
    /* Determine the initial value from LUT inputs */
    output_init_val = get_lut_output_init_val(cur_logical_block);
    break;
  case SPICE_MODEL_HARDLOGIC:
    /* We have no information, give a default 0 now... 
     * TODO: find a smarter way!
     */
    output_init_val = get_ff_output_init_val(cur_logical_block);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SPICE MODEL (name=%s) in determining the initial output value of logical block(name=%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name, cur_logical_block->name);
    exit(1); 
  }
  
  return output_init_val;
}


/* Alloc, initialize and free functions for sb_info & cb_info */
/* Initialize a SB_info */
void init_one_sb_info(t_sb* cur_sb) { 
  cur_sb->x = -1; /* give an invalid value, which means OPEN */ 
  cur_sb->y = -1; /* give an invalid value, which means OPEN */ 
  cur_sb->directionality = UNI_DIRECTIONAL; 
  cur_sb->fs = -1; /* give an invalid value, which means OPEN */ 
  cur_sb->fc_out = -1; /* give an invalid value, which means OPEN */ 
  cur_sb->num_sides = 4; /* Should be fixed to 4 */  
  cur_sb->chan_width = NULL;
  cur_sb->num_ipin_rr_nodes = NULL;
  cur_sb->num_opin_rr_nodes = NULL;
  cur_sb->chan_rr_node = NULL;
  cur_sb->chan_rr_node_direction = NULL;
  cur_sb->ipin_rr_node = NULL;
  cur_sb->ipin_rr_node_grid_side = NULL;
  cur_sb->opin_rr_node = NULL;
  cur_sb->opin_rr_node_grid_side = NULL;
  cur_sb->num_reserved_conf_bits = 0;
  cur_sb->conf_bits_lsb = 0;
  cur_sb->conf_bits_msb = 0;

  cur_sb->mirror = NULL;
  cur_sb->rotatable = NULL;
  cur_sb->offset_ipin = NULL;
  cur_sb->offset_opin = NULL;
  cur_sb->offset_chan = NULL;

  return;
}

/* Free everything (lists) inside a sb_info */
void free_one_sb_info(t_sb* cur_sb) {
  int i;

  if ((-1 == cur_sb->x)||(-1 == cur_sb->y)) {
    /*NULL struct: bypass free */
    return;
  }

  /* Free chan_width, input/output_rr_nodes, rr_nodes */
  my_free(cur_sb->chan_width);
  for (i = 0; i < cur_sb->num_sides; i++) {
    my_free(cur_sb->chan_rr_node_direction[i]);
    my_free(cur_sb->chan_rr_node[i]);
    my_free(cur_sb->ipin_rr_node[i]);
    my_free(cur_sb->ipin_rr_node_grid_side[i]);
    my_free(cur_sb->opin_rr_node[i]);
    my_free(cur_sb->opin_rr_node_grid_side[i]);
  }
  my_free(cur_sb->num_ipin_rr_nodes);
  my_free(cur_sb->num_opin_rr_nodes);
  my_free(cur_sb->chan_rr_node_direction);
  my_free(cur_sb->chan_rr_node);
  my_free(cur_sb->ipin_rr_node);
  my_free(cur_sb->ipin_rr_node_grid_side);
  my_free(cur_sb->opin_rr_node);
  my_free(cur_sb->opin_rr_node_grid_side);

  my_free(cur_sb->offset_ipin);  
  my_free(cur_sb->offset_opin);  
  my_free(cur_sb->offset_chan);  

  return;
}

/* Alloc sb_info */
t_sb** alloc_sb_info_array(int LL_nx, int LL_ny) {
  int ix, iy;
  t_sb** LL_sb_info = NULL;

  /* Allocate a two-dimension array for sb_info */
  LL_sb_info = (t_sb**)my_malloc(sizeof(t_sb*) * (LL_nx+1)); /* [0 ... nx] */
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    LL_sb_info[ix] = (t_sb*)my_malloc(sizeof(t_sb) * (LL_ny+1)); /* [0 ... ny] */
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      init_one_sb_info(&(LL_sb_info[ix][iy])); /* Initialize to NULL pointer */
    }
  }

  return LL_sb_info;
}

/* Free an sb_info_array */
void free_sb_info_array(t_sb*** LL_sb_info, int LL_nx, int LL_ny) {
  int ix, iy;

  if (NULL == (*LL_sb_info)) {
    return;
  } 

  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      free_one_sb_info(&((*LL_sb_info)[ix][iy]));
    }
    my_free((*LL_sb_info)[ix]);
    (*LL_sb_info)[ix] = NULL;
  }

  (*LL_sb_info) = NULL;

  return;
}

/* Initialize a CB_info */
void init_one_cb_info(t_cb* cur_cb) { 
  cur_cb->x = -1; /* give an invalid value, which means OPEN */ 
  cur_cb->y = -1; /* give an invalid value, which means OPEN */ 
  cur_cb->type = NUM_RR_TYPES;
  cur_cb->directionality = UNI_DIRECTIONAL; 
  cur_cb->fc_in = -1; /* give an invalid value, which means OPEN */ 
  cur_cb->num_sides = 4; /* Should be fixed to 4 */  
  cur_cb->chan_width = NULL;
  cur_cb->num_ipin_rr_nodes = NULL;
  cur_cb->num_opin_rr_nodes = NULL;
  cur_cb->chan_rr_node = NULL;
  cur_cb->chan_rr_node_direction = NULL;
  cur_cb->ipin_rr_node = NULL;
  cur_cb->ipin_rr_node_grid_side = NULL;
  cur_cb->opin_rr_node = NULL;
  cur_cb->opin_rr_node_grid_side = NULL;
  cur_cb->num_reserved_conf_bits = 0;
  cur_cb->conf_bits_lsb = 0;
  cur_cb->conf_bits_msb = 0;

  cur_cb->mirror = NULL;
  cur_cb->rotatable = NULL;
  cur_cb->offset_ipin = NULL;
  cur_cb->offset_opin = NULL;
  cur_cb->offset_chan = NULL;

  return;
}

/* Free everything (lists) inside a sb_info */
void free_one_cb_info(t_cb* cur_cb) {
  int i;

  if ((-1 == cur_cb->x)||(-1 == cur_cb->y)) {
    /*NULL struct: bypass free */
    return;
  }

  /* Free chan_width, input/output_rr_nodes, rr_nodes */
  my_free(cur_cb->chan_width);
  for (i = 0; i < cur_cb->num_sides; i++) {
    my_free(cur_cb->chan_rr_node[i]);
    my_free(cur_cb->chan_rr_node_direction[i]);
    my_free(cur_cb->ipin_rr_node[i]);
    my_free(cur_cb->ipin_rr_node_grid_side[i]);
    my_free(cur_cb->opin_rr_node[i]);
    my_free(cur_cb->opin_rr_node_grid_side[i]);
  }
  my_free(cur_cb->chan_rr_node);
  my_free(cur_cb->num_ipin_rr_nodes);
  my_free(cur_cb->num_opin_rr_nodes);
  my_free(cur_cb->chan_rr_node_direction);
  my_free(cur_cb->ipin_rr_node);
  my_free(cur_cb->ipin_rr_node_grid_side);
  my_free(cur_cb->opin_rr_node);
  my_free(cur_cb->opin_rr_node_grid_side);

  my_free(cur_cb->offset_ipin);
  my_free(cur_cb->offset_opin);
  my_free(cur_cb->offset_chan);

  return;
}

/* Alloc cb_info: need to call this function twice for X-channel and Y-channel */
t_cb** alloc_cb_info_array(int LL_nx, int LL_ny) {
  int ix, iy;
  t_cb** LL_cb_info = NULL;

  /* Allocate a two-dimension array for cb_info */
  LL_cb_info = (t_cb**)my_malloc(sizeof(t_cb*) * (LL_nx+1)); /* [0 ... nx] */
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    LL_cb_info[ix] = (t_cb*)my_malloc(sizeof(t_cb) * (LL_ny+1)); /* [0 ... ny] */
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      init_one_cb_info(&(LL_cb_info[ix][iy])); /* Initialize to NULL pointer */
    }
  }

  return LL_cb_info;
}

/* Free an sb_info_array */
void free_cb_info_array(t_cb*** LL_cb_info, int LL_nx, int LL_ny) {
  int ix, iy;

  if (NULL == (*LL_cb_info)) {
    return;
  } 

  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      free_one_cb_info(&((*LL_cb_info)[ix][iy]));
    }
    my_free((*LL_cb_info)[ix]);
    (*LL_cb_info)[ix] = NULL;
  }

  (*LL_cb_info) = NULL;

  return;
}

/* Get the index of a given rr_node in a SB_info */
int get_rr_node_index_in_sb_info(t_rr_node* cur_rr_node,
                                 t_sb cur_sb_info, 
                                 int chan_side, enum PORTS rr_node_direction) {
  int inode, cnt, ret; 

  cnt = 0;
  ret = OPEN;

  /* Depending on the type of rr_node, we search different arrays */
  switch (cur_rr_node->type) {
  case CHANX:
  case CHANY:
    for (inode = 0; inode < cur_sb_info.chan_width[chan_side]; inode++) {
      if ((cur_rr_node == cur_sb_info.chan_rr_node[chan_side][inode])
        /* Check if direction meets specification */
        &&(rr_node_direction == cur_sb_info.chan_rr_node_direction[chan_side][inode])) {
        cnt++;
        ret = inode;
      }
    }
    break;
  case IPIN:
    for (inode = 0; inode < cur_sb_info.num_ipin_rr_nodes[chan_side]; inode++) {
      if (cur_rr_node == cur_sb_info.ipin_rr_node[chan_side][inode]) {
        cnt++;
        ret = inode;
      }
    }
    break;
  case OPIN:
    for (inode = 0; inode < cur_sb_info.num_opin_rr_nodes[chan_side]; inode++) {
      if (cur_rr_node == cur_sb_info.opin_rr_node[chan_side][inode]) {
        cnt++;
        ret = inode;
      }
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid cur_rr_node type! Should be [CHANX|CHANY|IPIN|OPIN]\n", __FILE__, __LINE__);
    exit(1);
  }

  assert((0 == cnt)||(1 == cnt));

  return ret; /* Return an invalid value: nonthing is found*/
}

/* Check if the src_rr_node is just a wire crossing this switch box
 *        ---------
 *        |       |
 *    ------------------>
 *        |       |
 *        ---------
 * Strategy:
 * Check each driver rr_node of this src_rr_node,
 * see if they are in the opin_rr_node, chan_rr_node lists of sb_rr_info
 */
int is_rr_node_exist_opposite_side_in_sb_info(t_sb cur_sb_info,
                                              t_rr_node* src_rr_node, 
                                              int chan_side) {
  int oppo_chan_side = -1;
  int interc, index;

  assert((CHANX == src_rr_node->type) || (CHANY == src_rr_node->type));

  oppo_chan_side = get_opposite_side(chan_side); 

  /* See if we can find the same src_rr_node in the opposite chan_side 
   * if there is one, it means a shorted wire across the SB 
   */
  index = get_rr_node_index_in_sb_info(src_rr_node, cur_sb_info, oppo_chan_side, IN_PORT);

  interc = 0;
  if (-1 != index) {
    interc = 1;
  }

  return interc;
}


/* Get the side and index of a given rr_node in a SB_info 
 * Return cur_rr_node_side & cur_rr_node_index
 */
void get_rr_node_side_and_index_in_sb_info(t_rr_node* cur_rr_node,
                                          t_sb cur_sb_info,
                                          enum PORTS rr_node_direction,
                                          OUTP int* cur_rr_node_side, 
                                          OUTP int* cur_rr_node_index) {
  int index, side;
  
  /* Count the number of existence of cur_rr_node in cur_sb_info
   * It could happen that same cur_rr_node appears on different sides of a SB
   * For example, a routing track go vertically across the SB.
   * Then its corresponding rr_node appears on both TOP and BOTTOM sides of this SB. 
   * We need to ensure that the found rr_node has the same direction as user want.
   * By specifying the direction of rr_node, There should be only one rr_node can satisfy!
   */
  index = -1;

  for (side = 0; side < cur_sb_info.num_sides; side++) {
    index = get_rr_node_index_in_sb_info(cur_rr_node, cur_sb_info, side, rr_node_direction);
    if (-1 != index) {
      break;
    }
  }

  if (side == cur_sb_info.num_sides) {
    /* we find nothing */
    side = -1;
  }

  (*cur_rr_node_side) = side;
  (*cur_rr_node_index) = index;

  return;
}

/* Check if the drivers for cur_rr_node imply a short connection in this Switch block 
 */
boolean check_drive_rr_node_imply_short(t_sb cur_sb_info,
                                        t_rr_node* src_rr_node, 
                                        int chan_side) {
  int inode, index, side; 

  assert((CHANX == src_rr_node->type) || (CHANY == src_rr_node->type));
  
  for (inode = 0; inode < src_rr_node->num_drive_rr_nodes; inode++) {
    get_rr_node_side_and_index_in_sb_info(src_rr_node->drive_rr_nodes[inode], cur_sb_info, IN_PORT, &side, &index);
    /* We need to be sure that drive_rr_node is part of the SB */
    if (((-1 == index)||(-1 == side)) 
       && ((CHANX == src_rr_node->drive_rr_nodes[inode]->type)||(CHANY == src_rr_node->drive_rr_nodes[inode]->type))) {
      return TRUE;
    }
  }

  return FALSE;
} 


/* Get the index of a given rr_node in a CB_info */
int get_rr_node_index_in_cb_info(t_rr_node* cur_rr_node,
                                 t_cb cur_cb_info, 
                                 int chan_side, enum PORTS rr_node_direction) {
  int inode, cnt, ret; 

  cnt = 0;
  ret = -1;

  /* Depending on the type of rr_node, we search different arrays */
  switch (cur_rr_node->type) {
  case CHANX:
  case CHANY:
    for (inode = 0; inode < cur_cb_info.chan_width[chan_side]; inode++) {
      if ((cur_rr_node == cur_cb_info.chan_rr_node[chan_side][inode])
        /* Check if direction meets specification */
        &&(rr_node_direction == cur_cb_info.chan_rr_node_direction[chan_side][inode])) {
        cnt++;
        ret = inode;
      }
    }
    break;
  case IPIN:
    for (inode = 0; inode < cur_cb_info.num_ipin_rr_nodes[chan_side]; inode++) {
      if (cur_rr_node == cur_cb_info.ipin_rr_node[chan_side][inode]) {
        cnt++;
        ret = inode;
      }
    }
    break;
  case OPIN:
    for (inode = 0; inode < cur_cb_info.num_opin_rr_nodes[chan_side]; inode++) {
      if (cur_rr_node == cur_cb_info.opin_rr_node[chan_side][inode]) {
        cnt++;
        ret = inode;
      }
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid cur_rr_node type! Should be [CHANX|CHANY|IPIN|OPIN]\n", __FILE__, __LINE__);
    exit(1);
  }

  assert((0 == cnt)||(1 == cnt));

  return ret; /* Return an invalid value: nonthing is found*/
}

/* Determine the coordinate of a chan_rr_node in a SB_info 
 * Return chan_type & chan_rr_node_x & chan_rr_node_y
 */
void get_chan_rr_node_coorindate_in_sb_info(t_sb cur_sb_info,
                                            int chan_rr_node_side,
                                            t_rr_type* chan_type,
                                            int* chan_rr_node_x, int* chan_rr_node_y) {
  int sb_x = cur_sb_info.x;
  int sb_y = cur_sb_info.y;

  switch (chan_rr_node_side) {
  case 0: /*TOP*/
    (*chan_type) = CHANY;
    (*chan_rr_node_x) = sb_x; 
    (*chan_rr_node_y) = sb_y + 1; 
    break; 
  case 1: /*RIGHT*/
    (*chan_type) = CHANX;
    (*chan_rr_node_x) = sb_x + 1; 
    (*chan_rr_node_y) = sb_y; 
    break; 
  case 2: /*BOTTOM*/
    (*chan_type) = CHANY;
    (*chan_rr_node_x) = sb_x; 
    (*chan_rr_node_y) = sb_y; 
    break; 
  case 3: /*LEFT*/
    (*chan_type) = CHANX;
    (*chan_rr_node_x) = sb_x; 
    (*chan_rr_node_y) = sb_y; 
    break; 
  default: 
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid side!\n", __FILE__, __LINE__);
    exit(1);
  }

  return;  
}

/* Get the side and index of a given rr_node in a SB_info 
 * Return cur_rr_node_side & cur_rr_node_index
 */
void get_rr_node_side_and_index_in_cb_info(t_rr_node* cur_rr_node,
                                           t_cb cur_cb_info,
                                           enum PORTS rr_node_direction,
                                           OUTP int* cur_rr_node_side, 
                                           OUTP int* cur_rr_node_index) {
  int index, side;
  
  index = -1;

  for (side = 0; side < cur_cb_info.num_sides; side++) {
    index = get_rr_node_index_in_cb_info(cur_rr_node, cur_cb_info, side, rr_node_direction);
    if (-1 != index) {
      break;
    }
  }

  if (side == cur_cb_info.num_sides) {
    /* we find nothing */
    side = -1;
  }

  (*cur_rr_node_side) = side;
  (*cur_rr_node_index) = index;

  return;
}


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
    /* assert(OPEN != LL_rr_node[to_node].prev_node); */
    if (src_node_index == LL_rr_node[to_node].prev_node) {
      assert(iedge == LL_rr_node[to_node].prev_edge);
      /* assert(LL_rr_node[src_node_index].net_num == LL_rr_node[to_node].net_num); */
      /* Label parasitic net */
      if ((OPEN == LL_rr_node[to_node].net_num) 
        && (OPEN != LL_rr_node[src_node_index].net_num)) { 
        LL_rr_node[to_node].is_parasitic_net = TRUE;
      }
      /* Propagate the net_num */
      LL_rr_node[to_node].net_num = LL_rr_node[src_node_index].net_num; 
      /* Make the flag which indicates a changing has been made */
      if (LL_rr_node[to_node].vpack_net_num != LL_rr_node[src_node_index].vpack_net_num) {
        LL_rr_node[to_node].vpack_net_num_changed = TRUE;
      }
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
void backannotate_rr_nodes_parasitic_net_info() {
  int inode;
 
  /* Start from all the SOURCEs */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    /* We care only OPINs
     * or a contant generator */
    if ((OPIN !=  rr_node[inode].type) 
       || (!(SOURCE != rr_node[inode].type)
          && (0 == rr_node[inode].num_drive_rr_nodes))) {
      continue;
    }
    /* Bypass unmapped pins */
    if (OPEN == rr_node[inode].vpack_net_num) {
      continue;
    }
    /* Forward to all the downstream rr_nodes */
    rec_backannotate_rr_node_net_num(num_rr_nodes, rr_node, inode); 
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
        vpack_net[inet].spice_net_info->init_val = get_logical_block_output_init_val(&(logical_block[iblk]));
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
      vpack_net[inet].spice_net_info = (t_spice_net_info*)my_calloc(1, sizeof(t_spice_net_info));
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
      clb_net[inet].spice_net_info = (t_spice_net_info*)my_calloc(1, sizeof(t_spice_net_info));
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
void set_one_pb_rr_node_default_prev_node_edge(t_rr_node* pb_rr_graph, 
                                               t_pb_graph_pin* des_pb_graph_pin,
                                               int mode_index) {
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
  for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
    if (mode_index != des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode_index) {
      continue;
    }
    prev_node = des_pb_graph_pin->input_edges[iedge]->input_pins[0]->pin_count_in_cluster;
    break;
  }

  /* prev_node may not exist since some pb_graph_pin is accessible in some mode, we do not need to find a prev_edge  */
  if (OPEN == prev_node) { 
    /* backannotate */
    pb_rr_graph[node_index].prev_node = prev_node;
    pb_rr_graph[node_index].prev_edge = prev_edge;
    return;
  }

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
void back_annotate_one_pb_rr_node_map_info_rec(t_pb* cur_pb,
                                               t_pb_graph_node* cur_pb_graph_node, 
                                               t_rr_node* pb_rr_nodes) {
  int imode, ipb, jpb, select_mode_index;
  int iport, ipin, node_index;
  t_pb_graph_node* child_pb_graph_node;
  t_pb_type* cur_pb_type = cur_pb_graph_node->pb_type;
 
  /* Return when we meet a null pb */ 
  if (NULL == cur_pb) {
    /* Wired LUT does not has a pb but has a net_num */
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        node_index = cur_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
        if (OPEN != pb_rr_nodes[node_index].net_num) {
          pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
        }
      }
    }

    /* Wired LUT does not has a pb but has a net_num */
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        node_index = cur_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
        if (OPEN != pb_rr_nodes[node_index].net_num) {
          pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
        }
      }
    }
    /* Return if this is a leaf node */
    if (TRUE == is_primitive_pb_type(cur_pb_type)) { 
      return;
    }

    /* Go recusrively */
    for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
      for (ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
        for (jpb = 0; jpb < cur_pb_type->modes[imode].pb_type_children[ipb].num_pb; jpb++) {
          /* For wired LUT */  
          if (FALSE == is_pb_used_for_wiring(&(cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]),  
                                            &(cur_pb_graph_node->pb_type->modes[imode].pb_type_children[ipb]),
                                            pb_rr_nodes)) {        
            continue;
          }
          /* Reach here means that this LUT is in wired mode (a buffer)  
           * synchronize the net num 
           */
          back_annotate_one_pb_rr_node_map_info_rec(NULL,
                                                    &(cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]),
                                                    pb_rr_nodes);
        }
      }
    }

    return;
  }
  
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
  select_mode_index = cur_pb->mode; 
  for (iport = 0; iport < cur_pb->pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_output_pins[iport]; ipin++) {
      /* Get the selected edge of current pin*/
      node_index = cur_pb->pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      /* If we find an OPEN net, try to find the parasitic net_num*/
      if (OPEN == pb_rr_nodes[node_index].net_num) {
        set_one_pb_rr_node_default_prev_node_edge(pb_rr_nodes, 
                                                  &(cur_pb->pb_graph_node->output_pins[iport][ipin]),
                                                  select_mode_index); 
      } else {
        pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
      }
    }
  }

  /* Reach a leaf, return */
  if ((0 == cur_pb->pb_graph_node->pb_type->num_modes)
     ||(NULL == cur_pb->child_pbs)) {
    return;
  }
  
  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  select_mode_index = cur_pb->mode; 
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      child_pb_graph_node = &(cur_pb->pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]);
      /* For each child_pb_graph_node input pins*/
      for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
          /* Get the selected edge of current pin*/
          node_index = child_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_default_prev_node_edge(pb_rr_nodes, 
                                                      &(child_pb_graph_node->input_pins[iport][ipin]),
                                                      select_mode_index); 
          } else {
             pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
          }
        }
      }
      /* For each child_pb_graph_node clock pins*/
      for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
          /* Get the selected edge of current pin*/
          node_index = child_pb_graph_node->clock_pins[iport][ipin].pin_count_in_cluster;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_default_prev_node_edge(pb_rr_nodes, 
                                                      &(child_pb_graph_node->clock_pins[iport][ipin]),
                                                      select_mode_index); 
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
        back_annotate_one_pb_rr_node_map_info_rec(&(cur_pb->child_pbs[ipb][jpb]),
                                                  &(cur_pb->pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]),
                                                  cur_pb->rr_graph);
      } else if (TRUE == is_pb_used_for_wiring(&(cur_pb->pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]),  
                                               &(cur_pb->pb_graph_node->pb_type->modes[select_mode_index].pb_type_children[ipb]),
                                               cur_pb->rr_graph)) {        
        /* For wired LUT */
          /* Reach here means that this LUT is in wired mode (a buffer)  
           * synchronize the net num 
           */
          back_annotate_one_pb_rr_node_map_info_rec(NULL,
                                                    &(cur_pb->pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]),
                                                    cur_pb->rr_graph);
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
    if (IO_TYPE == block[iblk].type) {
     continue; 
    }
    back_annotate_one_pb_rr_node_map_info_rec(block[iblk].pb, 
                                              block[iblk].pb->pb_graph_node, 
                                              block[iblk].pb->rr_graph);
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

  node_index = des_pb_graph_pin->rr_node_index_physical_pb;
  assert(OPEN == pb_rr_graph[node_index].net_num);
   
  /* if this pin has 0 driver, return OPEN */
  if (0 == des_pb_graph_pin->num_input_edges) {
    pb_rr_graph[node_index].net_num= OPEN;
    pb_rr_graph[node_index].vpack_net_num= OPEN;
    return;
  }

  prev_node = pb_rr_graph[node_index].prev_node;
  prev_edge = pb_rr_graph[node_index].prev_edge;
  
  /* Some node may be disconnected */
  if (OPEN == prev_node) {
    pb_rr_graph[node_index].net_num = OPEN;
    pb_rr_graph[node_index].vpack_net_num = OPEN;
    return;
  }

  assert(OPEN != prev_node); 
  assert(OPEN != prev_edge); 

  /* Set default prev_node */
  check_pb_graph_edge(*(pb_rr_graph[prev_node].pb_graph_pin->output_edges[prev_edge]));
  assert(node_index == pb_rr_graph[prev_node].pb_graph_pin->output_edges[prev_edge]->output_pins[0]->rr_node_index_physical_pb);
  pb_rr_graph[node_index].net_num = pb_rr_graph[prev_node].net_num;
  pb_rr_graph[node_index].vpack_net_num = pb_rr_graph[prev_node].net_num;

  return;
}

/* Mark the net_num of all the rr_nodes in complex blocks */
static
void backannotate_one_pb_rr_nodes_net_info_rec(t_phy_pb* cur_pb) {
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
          pb_rr_nodes = cur_pb->rr_graph->rr_node;
          node_index = child_pb_graph_node->input_pins[iport][ipin].rr_node_index_physical_pb;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_net_num(pb_rr_nodes, &(child_pb_graph_node->input_pins[iport][ipin])); 
            /* Label this net as parasitic net */
            pb_rr_nodes[node_index].is_parasitic_net = TRUE;
          }
        }
      }
      /* For each child_pb_graph_node clock pins*/
      for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
          /* Get the selected edge of current pin*/
          pb_rr_nodes = cur_pb->rr_graph->rr_node;
          node_index = child_pb_graph_node->clock_pins[iport][ipin].rr_node_index_physical_pb;
          /* If we find an OPEN net, try to find the parasitic net_num*/
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            set_one_pb_rr_node_net_num(pb_rr_nodes, &(child_pb_graph_node->clock_pins[iport][ipin])); 
            /* Label this net as parasitic net */
            pb_rr_nodes[node_index].is_parasitic_net = TRUE;
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
      pb_rr_nodes = cur_pb->rr_graph->rr_node;
      node_index = cur_pb->pb_graph_node->output_pins[iport][ipin].rr_node_index_physical_pb;
      /* If we find an OPEN net, try to find the parasitic net_num*/
      if (OPEN == pb_rr_nodes[node_index].net_num) {
        set_one_pb_rr_node_net_num(pb_rr_nodes, &(cur_pb->pb_graph_node->output_pins[iport][ipin])); 
        /* Label this net as parasitic net */
        pb_rr_nodes[node_index].is_parasitic_net = TRUE;
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
    if (IO_TYPE == block[iblk].type) {
      continue;
    }
    backannotate_one_pb_rr_nodes_net_info_rec((t_phy_pb*)block[iblk].phy_pb);
  }  

  return;
}

/* Mark the prev_edge and prev_node of all the rr_nodes in global routing */
static 
void back_annotate_rr_node_map_info() {
  int inode, jnode, inet, default_prev_node;
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
      jnode =  rr_node[inode].edges[iedge];
      default_prev_node = determine_rr_node_default_prev_node(&rr_node[jnode]);
      if (DEFAULT_PREV_NODE == default_prev_node) {
        rr_node[jnode].prev_node = default_prev_node;
      } else if (&(rr_node[inode]) == rr_node[jnode].drive_rr_nodes[default_prev_node]) {
        rr_node[jnode].prev_node = inode;
        rr_node[jnode].prev_edge = iedge;
      }
    }
  }

  /* 2nd step: With the help of trace, we back-annotate */
  for (inet = 0; inet < num_nets; inet++) {
    if (TRUE == clb_net[inet].is_global) {
      continue;
    }
    tptr = trace_head[inet];
    while (tptr != NULL) {
      inode = tptr->index;
      rr_type = rr_node[inode].type;
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
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid traceback element type.\n",
                   __FILE__, __LINE__);
        exit(1);
      }
      tptr = tptr->next;
    }
  }

  return;
}

void rec_sync_pb_post_routing_vpack_net_num(t_pb* cur_pb) {
  int ipb, jpb, select_mode_index;
  int iport, ipin, node_index;
  t_rr_node* pb_rr_nodes = NULL;
 
  /* Return when we meet a null pb */ 
  if (NULL == cur_pb) {
    return;
  }
  
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
  select_mode_index = cur_pb->mode; 
  for (iport = 0; iport < cur_pb->pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_input_pins[iport]; ipin++) {
      /* Get the selected edge of current pin*/
      pb_rr_nodes = cur_pb->rr_graph;
      node_index = cur_pb->pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
      pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
    }
  }

  for (iport = 0; iport < cur_pb->pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_output_pins[iport]; ipin++) {
      /* Get the selected edge of current pin*/
      pb_rr_nodes = cur_pb->rr_graph;
      node_index = cur_pb->pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
    }
  }

  for (iport = 0; iport < cur_pb->pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_clock_pins[iport]; ipin++) {
      /* Get the selected edge of current pin*/
      pb_rr_nodes = cur_pb->rr_graph;
      node_index = cur_pb->pb_graph_node->clock_pins[iport][ipin].pin_count_in_cluster;
      pb_rr_nodes[node_index].vpack_net_num = pb_rr_nodes[node_index].net_num;
    }
  }

  /* Reach a leaf, return */
  if ((0 == cur_pb->pb_graph_node->pb_type->num_modes)
     ||(NULL == cur_pb->child_pbs)) {
    return;
  }

  /* Go recursively */ 
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        rec_sync_pb_post_routing_vpack_net_num(&(cur_pb->child_pbs[ipb][jpb]));
      }
    }
  }
  return;
}

/* IO blocks are special:
 * each pb only contain 1 pb_graph_node (1 io)
 * while the top-level type_descriptor consider 8 io in counting the pins
 * so we just update the vpack_net_num and net_num in all the hierachy level 
 */
void update_one_io_grid_pack_net_num(int x, int y) {
  int iblk, blk_id;
  t_type_ptr type = NULL;
  t_pb* pb = NULL;

  /* Assert */
  assert((!(x < 0))&&(x < (nx + 2)));  
  assert((!(y < 0))&&(y < (ny + 2)));  

  type = grid[x][y].type;
  /* check */
  assert (IO_TYPE == type);
 
  for (iblk = 0; iblk < grid[x][y].usage; iblk++) {
    blk_id = grid[x][y].blocks[iblk];
    if ((IO_TYPE != type)) {
      assert(block[blk_id].x == x);
      assert(block[blk_id].y == y);
      assert(OPEN != blk_id);
    }
    pb = block[blk_id].pb;
    assert(NULL != pb);
    rec_sync_pb_post_routing_vpack_net_num(pb);
  }
 
  return;
}


/* During routing stage, VPR swap logic equivalent pins
 * which potentially changes the packing results (net_num and vpack_net_num) in local routing
 * The following functions are to update the local routing results to match them with routing results
 */
void update_one_grid_pack_net_num(int x, int y) {
  int iblk, blk_id, ipin, iedge, jedge, inode;
  int pin_global_rr_node_id, vpack_net_id, class_id;
  t_type_ptr type = NULL;
  t_pb* pb = NULL;
  t_rr_node* local_rr_graph = NULL;

  /* Assert */
  assert((!(x < 0))&&(x < (nx + 2)));  
  assert((!(y < 0))&&(y < (ny + 2)));  

  type = grid[x][y].type;

  /* check */
  assert ((NULL != type) 
       && (EMPTY_TYPE != type) 
       && (IO_TYPE != type));
 
  for (iblk = 0; iblk < grid[x][y].usage; iblk++) {
    blk_id = grid[x][y].blocks[iblk];
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
          if ( (OPEN != local_rr_graph[ipin].net_num)
              && (TRUE == vpack_net[local_rr_graph[ipin].net_num].is_global)) {
            local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
            local_rr_graph[ipin].vpack_net_num = local_rr_graph[ipin].net_num;
            local_rr_graph[ipin].net_num = vpack_to_clb_net_mapping[local_rr_graph[ipin].net_num];
          } else {
            local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
            local_rr_graph[ipin].net_num = OPEN;
            local_rr_graph[ipin].vpack_net_num = OPEN;
          //local_rr_graph[ipin].prev_node = 0;
          //local_rr_graph[ipin].prev_edge = 0;
          }
          continue; /* bypass non-mapped OPIN */
        } 
        /* back annotate pb ! */
        rr_node[pin_global_rr_node_id].pb = pb;
        vpack_net_id = clb_to_vpack_net_mapping[rr_node[pin_global_rr_node_id].net_num];
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
        /* Special for global net, preserve them in the local rr_graph */
        /* Get the index of Vpack net from global rr_node net_num (clb_net index)*/
        if (OPEN == rr_node[pin_global_rr_node_id].net_num) {
          if ( (OPEN != local_rr_graph[ipin].net_num)
              && (TRUE == vpack_net[local_rr_graph[ipin].net_num].is_global)) {
            local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
            local_rr_graph[ipin].vpack_net_num = local_rr_graph[ipin].net_num;
            local_rr_graph[ipin].net_num = vpack_to_clb_net_mapping[local_rr_graph[ipin].net_num];
          } else {
            local_rr_graph[ipin].net_num_in_pack = local_rr_graph[ipin].net_num;
            local_rr_graph[ipin].net_num = OPEN;
            local_rr_graph[ipin].vpack_net_num = OPEN;
          //local_rr_graph[ipin].prev_node = 0;
          //local_rr_graph[ipin].prev_edge = 0;
          }
          continue; /* bypass non-mapped IPIN */
        }
        /* back annotate pb ! */
        rr_node[pin_global_rr_node_id].pb = pb;
        vpack_net_id = clb_to_vpack_net_mapping[rr_node[pin_global_rr_node_id].net_num];
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
  }
 
  return;
}

void update_grid_pbs_post_route_rr_graph() {
  int ix, iy;
  t_type_ptr type = NULL;

  for (ix = 0; ix < (nx + 2); ix++) {
    for (iy = 0; iy < (ny + 2); iy++) {
      type = grid[ix][iy].type;
      /* bypass EMPTY type */
      if ((NULL == type) || (EMPTY_TYPE == type)) {
        continue;
      }
      if (IO_TYPE == type) {
        update_one_io_grid_pack_net_num(ix, iy);
      } else {
        /* Backup the packing prev_node and prev_edge */
        update_one_grid_pack_net_num(ix, iy);
      }
    }
  }

  return;
}

/* In this function, we update the vpack_net_num in global rr_graph
 * from the temp_net_num stored in the top_pb_graph_head
 */
void update_one_unused_grid_output_pins_parasitic_nets(int ix, int iy) {
  int iport, ipin; 
  int pin_global_rr_node_id, class_id, type_pin_index;
  t_type_ptr type = NULL;
  t_pb_graph_node* top_pb_graph_node = NULL;

  /* Assert */
  assert((!(ix < 0))&&(ix < (nx + 2)));  
  assert((!(iy < 0))&&(iy < (ny + 2)));  

  type = grid[ix][iy].type;
  /* Bypass IO_TYPE*/
  if ((EMPTY_TYPE == type)||(IO_TYPE == type)) {
    return;
  }   

  /* Use the temp_net_num of each pb_graph_pin at the top-level
   * Update the global rr_graph
   */
  top_pb_graph_node = type->pb_graph_head;
  assert(NULL != top_pb_graph_node);

  /* We only care the outputs, since the inputs are updated in function
   * mark_grid_type_pb_graph_node_pins_temp_net_num(ix, iy);
   */
  for (iport = 0; iport < top_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_output_pins[iport]; ipin++) {
      top_pb_graph_node->output_pins[iport][ipin].temp_net_num = OPEN;
      type_pin_index = top_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      class_id = type->pin_class[type_pin_index];
      assert(DRIVER == type->class_inf[class_id].type);
      /* Find the pb net_num and update OPIN net_num */
      pin_global_rr_node_id = get_rr_node_index(ix, iy, OPIN, type_pin_index, rr_node_indices);
      /* Avoid mistakenly overwrite */
      if (OPEN != rr_node[pin_global_rr_node_id].vpack_net_num) {
        continue;
      }
      rr_node[pin_global_rr_node_id].vpack_net_num = top_pb_graph_node->output_pins[iport][ipin].temp_net_num;
    } 
  }
 
  return;
}


/* SPEICAL: to assign parasitic nets, I modify the net_num in global
 * global routing nets to be vpack_net_num. The reason is some vpack nets 
 * are absorbed into CLBs during packing, therefore they are invisible in 
 * clb_nets. But indeed, they exist in global routing as parasitic nets.
 */
void update_one_used_grid_pb_pins_parasitic_nets(t_phy_pb* cur_pb,
                                                 int ix, int iy) {
  int ipin, cur_pin; 
  int pin_global_rr_node_id,class_id;
  t_type_ptr type = NULL;
  t_rr_node* local_rr_graph = NULL;

  /* Assert */
  assert((!(ix < 0))&&(ix < (nx + 2)));  
  assert((!(iy < 0))&&(iy < (ny + 2)));  

  type = grid[ix][iy].type;
  /* Bypass IO_TYPE*/
  if ((EMPTY_TYPE == type)||(IO_TYPE == type)) {
    return;
  }   

  assert(NULL != cur_pb);
  local_rr_graph = cur_pb->rr_graph->rr_node; 
  for (ipin = 0; ipin < type->num_pins; ipin++) {
    class_id = type->pin_class[ipin];
    if (DRIVER == type->class_inf[class_id].type) {
      /* Find the pb net_num and update OPIN net_num */
      pin_global_rr_node_id = get_rr_node_index(ix, iy, OPIN, ipin, rr_node_indices);
      if (OPEN == local_rr_graph[ipin].net_num) {
        assert(OPEN == local_rr_graph[ipin].vpack_net_num);
        rr_node[pin_global_rr_node_id].net_num = OPEN; 
        rr_node[pin_global_rr_node_id].vpack_net_num = OPEN; 
        continue; /* bypass non-mapped OPIN */
      } 
      cur_pin = local_rr_graph[ipin].pb_graph_pin->rr_node_index_physical_pb;
      //rr_node[pin_global_rr_node_id].net_num = vpack_to_clb_net_mapping[local_rr_graph[ipin].net_num]; 
      rr_node[pin_global_rr_node_id].vpack_net_num = local_rr_graph[cur_pin].vpack_net_num;
      if ( (OPEN == rr_node[pin_global_rr_node_id].vpack_net_num) 
        && (OPEN != local_rr_graph[cur_pin].vpack_net_num)) {
        /* Label this net as parasitic net */
        rr_node[pin_global_rr_node_id].is_parasitic_net = TRUE;
      }
    } else if (RECEIVER == type->class_inf[class_id].type) {
      /* Find the global rr_node net_num and update pb net_num */
      pin_global_rr_node_id = get_rr_node_index(ix, iy, IPIN, ipin, rr_node_indices);
      /* Get the index of Vpack net from global rr_node net_num (clb_net index)*/
      if (OPEN == rr_node[pin_global_rr_node_id].vpack_net_num) {
        local_rr_graph[ipin].net_num = OPEN;
        local_rr_graph[ipin].vpack_net_num = OPEN;
        continue; /* bypass non-mapped IPIN */
      }
      cur_pin = local_rr_graph[ipin].pb_graph_pin->rr_node_index_physical_pb;
      local_rr_graph[cur_pin].net_num = rr_node[pin_global_rr_node_id].vpack_net_num;
      local_rr_graph[cur_pin].vpack_net_num = rr_node[pin_global_rr_node_id].vpack_net_num;
      if ( (OPEN == local_rr_graph[cur_pin].vpack_net_num) 
        && (OPEN == rr_node[pin_global_rr_node_id].vpack_net_num)) { 
        /* Label this net as parasitic net */
        local_rr_graph[cur_pin].is_parasitic_net = TRUE;
      }
    } else {
      continue; /* OPEN PIN */
    }
  }
 
  return;
}


void update_one_grid_pb_pins_parasitic_nets(int ix, int iy) {
  int iblk;

  /* Print all the grid */
  if ((NULL == grid[ix][iy].type)||(0 != grid[ix][iy].offset)) {
    return;
  }
  /* Used blocks */
  for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
    /* Only for mapped block */
    assert(NULL != block[grid[ix][iy].blocks[iblk]].pb);
    /* Mark the temporary net_num for the type pins*/
    mark_one_pb_parasitic_nets((t_phy_pb*)block[grid[ix][iy].blocks[iblk]].phy_pb);
    /* Update parasitic nets */
    update_one_used_grid_pb_pins_parasitic_nets((t_phy_pb*)block[grid[ix][iy].blocks[iblk]].phy_pb,
                                                ix, iy);
    /* update parasitic nets in each pb */
    backannotate_one_pb_rr_nodes_net_info_rec((t_phy_pb*)block[grid[ix][iy].blocks[iblk]].phy_pb);
  }  
  /* By pass Unused blocks */
  for (iblk = grid[ix][iy].usage; iblk < grid[ix][iy].type->capacity; iblk++) {
    /* Mark the temporary net_num for the type pins*/
    mark_grid_type_pb_graph_node_pins_temp_net_num(ix, iy);
    /* Update parasitic nets */
    update_one_unused_grid_output_pins_parasitic_nets(ix, iy);
  } 

  return;
}


void update_grid_pb_pins_parasitic_nets() {
  int ix, iy;
  t_type_ptr type = NULL;

  for (ix = 0; ix < (nx + 2); ix++) {
    for (iy = 0; iy < (ny + 2); iy++) {
      type = grid[ix][iy].type;
      if ((EMPTY_TYPE == type)||(IO_TYPE == type)) {
        continue;
      }
      /* Backup the packing prev_node and prev_edge */
      update_one_grid_pb_pins_parasitic_nets(ix, iy);
    }
  }

  return;
}

/* Update the driver switch for each rr_node*/
static 
void identify_rr_node_driver_switch(t_det_routing_arch RoutingArch,
                                    int LL_num_rr_nodes,
                                    t_rr_node* LL_rr_node) {
  int inode, iedge;

  /* Current Version: Support Uni-directional routing architecture only*/ 
  if (UNI_DIRECTIONAL != RoutingArch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Assign rr_node driver switch only support uni-directional routing architecture.\n",__FILE__, __LINE__);
    exit(1);
  }

  /* I can do a simple job here: 
   * just assign driver_switch from drive_switches[0]
   * which has been done in backannotation_vpr_post_route_info
   */
  /* update_rr_nodes_driver_switch(routing_arch->directionality); */
  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    if (0 == LL_rr_node[inode].fan_in) {
      assert(0 == LL_rr_node[inode].num_drive_rr_nodes);
      assert(NULL == LL_rr_node[inode].drive_switches);
      continue;
    }
    LL_rr_node[inode].driver_switch = LL_rr_node[inode].drive_switches[0];
    for (iedge = 0; iedge < LL_rr_node[inode].num_drive_rr_nodes; iedge++) {
      assert (LL_rr_node[inode].driver_switch == LL_rr_node[inode].drive_switches[iedge]);
    }
  }

  return;
}

/* Find all the rr_nodes of a channel 
 * Return an array of rr_node pointers, and length of the array (num_pin_rr_nodes)
 */
t_rr_node** get_chan_rr_nodes(int* num_chan_rr_nodes,
                              t_rr_type chan_type,
                              int x, int y,
                              int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                              t_ivec*** LL_rr_node_indices) {
  int itrack, inode;
  t_rr_node** chan_rr_nodes = NULL;

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  switch (chan_type) {
  case CHANX:
    /* Get the channel width */
    (*num_chan_rr_nodes) = chan_width_x[y];
    /* Allocate */
    chan_rr_nodes = (t_rr_node**)my_malloc((*num_chan_rr_nodes)*sizeof(t_rr_node*));
    /* Fill the array */
    for (itrack = 0; itrack < (*num_chan_rr_nodes); itrack++) {
      inode = get_rr_node_index(x, y, CHANX, itrack, LL_rr_node_indices);
      chan_rr_nodes[itrack] = &(LL_rr_node[inode]);
    }
    break;
  case CHANY:
    /* Get the channel width */
    (*num_chan_rr_nodes) = chan_width_y[x];
    /* Allocate */
    chan_rr_nodes = (t_rr_node**)my_malloc((*num_chan_rr_nodes)*sizeof(t_rr_node*));
    /* Fill the array */
    for (itrack = 0; itrack < (*num_chan_rr_nodes); itrack++) {
      inode = get_rr_node_index(x, y, CHANY, itrack, LL_rr_node_indices);
      chan_rr_nodes[itrack] = &(LL_rr_node[inode]);
    }
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }

  return chan_rr_nodes;
}


/* Find all the rr_nodes at a certain side of a grid
 * Return an array of rr_node pointers, and length of the array (num_pin_rr_nodes)
 */
t_rr_node** get_grid_side_pin_rr_nodes(int* num_pin_rr_nodes,
                                       t_rr_type pin_type,
                                       int x, int y, int side,
                                       int LL_num_rr_nodes, t_rr_node* LL_rr_node,
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
      ret[cur] = &(LL_rr_node[inode]); 
      cur++;
    }
  } 
  assert(cur == (*num_pin_rr_nodes));
  
  return ret;
}

/* Build arrays for rr_nodes of each Switch Blocks 
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
void build_one_switch_block_info(t_sb* cur_sb, int sb_x, int sb_y, 
                                 t_det_routing_arch RoutingArch,
                                 int LL_num_rr_nodes,
                                 t_rr_node* LL_rr_node,
                                 t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side, ix, iy;
  int temp_num_opin_rr_nodes[2] = {0,0};
  t_rr_node** temp_opin_rr_node[2] = {NULL, NULL};

  /* Check */
  assert((!(0 > sb_x))&&(!(sb_x > (nx + 1)))); 
  assert((!(0 > sb_y))&&(!(sb_y > (ny + 1)))); 

  /* Basic information*/
  cur_sb->x = sb_x;
  cur_sb->y = sb_y;
  cur_sb->directionality = RoutingArch.directionality; /* Could be more flexible, Currently we only support uni-directionalal routing architecture. */ 
  cur_sb->fs = RoutingArch.Fs;
  cur_sb->num_sides = 4; /* Fixed */

  /* Record the channel width of each side*/
  cur_sb->chan_width = (int*)my_calloc(cur_sb->num_sides, sizeof(int)); /* 4 sides */
  cur_sb->chan_rr_node_direction = (enum PORTS**)my_malloc(sizeof(enum PORTS*)*cur_sb->num_sides); /* 4 sides */
  cur_sb->chan_rr_node = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*cur_sb->num_sides); /* 4 sides*/
  cur_sb->num_ipin_rr_nodes = (int*)my_calloc(cur_sb->num_sides, sizeof(int)); /* 4 sides */
  cur_sb->ipin_rr_node = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*cur_sb->num_sides); /* 4 sides */
  cur_sb->ipin_rr_node_grid_side = (int**)my_malloc(sizeof(int*)*cur_sb->num_sides); /* 4 sides */
  cur_sb->num_opin_rr_nodes = (int*)my_calloc(cur_sb->num_sides, sizeof(int)); /* 4 sides */
  cur_sb->opin_rr_node = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*cur_sb->num_sides); /* 4 sides */
  cur_sb->opin_rr_node_grid_side = (int**)my_malloc(sizeof(int*)*cur_sb->num_sides); /* 4 sides */

  /* Find all rr_nodes of channels */
  /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
  for (side = 0; side < 4; side++) {
    switch (side) {
    case 0:
      /* For the bording, we should take special care */
      if (sb_y == ny) {
        cur_sb->chan_width[side] = 0;
        cur_sb->chan_rr_node[side] = NULL;
        cur_sb->chan_rr_node_direction[side] = NULL;
        cur_sb->num_ipin_rr_nodes[side] = 0;
        cur_sb->ipin_rr_node[side] = NULL;
        cur_sb->ipin_rr_node_grid_side[side] = NULL;
        cur_sb->num_opin_rr_nodes[side] = 0;
        cur_sb->opin_rr_node[side] = NULL;
        cur_sb->opin_rr_node_grid_side[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = sb_x; 
      iy = sb_y + 1;
      /* Channel width */
      cur_sb->chan_width[side] = chan_width_y[ix];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      cur_sb->chan_rr_node[side] = get_chan_rr_nodes(&(cur_sb->chan_width[side]), CHANY, ix, iy, 
                                                     LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Alloc */
      cur_sb->chan_rr_node_direction[side] = (enum PORTS*)my_malloc(sizeof(enum PORTS)*cur_sb->chan_width[side]);
      /* Collect rr_nodes for Tracks for top: chany[x][y+1] */
      for (itrack = 0; itrack < cur_sb->chan_width[side]; itrack++) {
        /* Identify the directionality, record it in rr_node_direction */
        if (INC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction) {
          cur_sb->chan_rr_node_direction[side][itrack] = OUT_PORT;
        } else {
          assert (DEC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction);
          cur_sb->chan_rr_node_direction[side][itrack] = IN_PORT;
        }
      }
      /* Include Grid[x][y+1] RIGHT side outputs pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x, sb_y + 1, 1,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Include Grid[x+1][y+1] Left side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x + 1, sb_y + 1, 3,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Allocate opin_rr_node */
      cur_sb->num_opin_rr_nodes[side] = temp_num_opin_rr_nodes[0] + temp_num_opin_rr_nodes[1];
      cur_sb->opin_rr_node[side] = (t_rr_node**)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(t_rr_node*));
      cur_sb->opin_rr_node_grid_side[side] = (int*)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(int));
      /* Copy from temp_opin_rr_node to opin_rr_node */
      for (inode = 0; inode < temp_num_opin_rr_nodes[0]; inode++) {
        cur_sb->opin_rr_node[side][inode] = temp_opin_rr_node[0][inode];
        cur_sb->opin_rr_node_grid_side[side][inode] = 1; /* Grid[x][y+1] RIGHT side outputs pins */
      }
      for (inode = 0; inode < temp_num_opin_rr_nodes[1]; inode++) {
        cur_sb->opin_rr_node[side][inode + temp_num_opin_rr_nodes[0]] = temp_opin_rr_node[1][inode];
        cur_sb->opin_rr_node_grid_side[side][inode + temp_num_opin_rr_nodes[0]] = 3; /* Grid[x+1][y+1] left side outputs pins */
      }
      /* We do not have any IPIN for a Switch Block */
      cur_sb->num_ipin_rr_nodes[side] = 0;
      cur_sb->ipin_rr_node[side] = NULL;
      cur_sb->ipin_rr_node_grid_side[side] = NULL;
      break;
    case 1:
      /* For the bording, we should take special care */
      if (sb_x == nx) {
        cur_sb->chan_width[side] = 0;
        cur_sb->chan_rr_node[side] = NULL;
        cur_sb->chan_rr_node_direction[side] = NULL;
        cur_sb->num_ipin_rr_nodes[side] = 0;
        cur_sb->ipin_rr_node[side] = NULL;
        cur_sb->ipin_rr_node_grid_side[side] = NULL;
        cur_sb->num_opin_rr_nodes[side] = 0;
        cur_sb->opin_rr_node[side] = NULL;
        cur_sb->opin_rr_node_grid_side[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = sb_x + 1; 
      iy = sb_y;
      /* Channel width */
      cur_sb->chan_width[side] = chan_width_x[iy];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Alloc */
      cur_sb->chan_rr_node[side] = get_chan_rr_nodes(&(cur_sb->chan_width[side]), CHANX, ix, iy, 
                                                     LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      cur_sb->chan_rr_node_direction[side] = (enum PORTS*)my_malloc(sizeof(enum PORTS)*cur_sb->chan_width[side]);
      /* Collect rr_nodes for Tracks for right: chanX[x+1][y] */
      for (itrack = 0; itrack < cur_sb->chan_width[side]; itrack++) {
        /* Identify the directionality, record it in rr_node_direction */
        if (INC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction) {
          cur_sb->chan_rr_node_direction[side][itrack] = OUT_PORT;
        } else {
          assert (DEC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction);
          cur_sb->chan_rr_node_direction[side][itrack] = IN_PORT;
        }
      }
      /* include Grid[x+1][y+1] Bottom side output pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x + 1, sb_y + 1, 2,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* include Grid[x+1][y] Top side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x + 1, sb_y, 0,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Allocate opin_rr_node */
      cur_sb->num_opin_rr_nodes[side] = temp_num_opin_rr_nodes[0] + temp_num_opin_rr_nodes[1];
      cur_sb->opin_rr_node[side] = (t_rr_node**)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(t_rr_node*));
      cur_sb->opin_rr_node_grid_side[side] = (int*)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(int));
      /* Copy from temp_opin_rr_node to opin_rr_node */
      for (inode = 0; inode < temp_num_opin_rr_nodes[0]; inode++) {
        cur_sb->opin_rr_node[side][inode] = temp_opin_rr_node[0][inode];
        cur_sb->opin_rr_node_grid_side[side][inode] = 2; /* Grid[x+1][y+1] Bottom side outputs pins */
      }
      for (inode = 0; inode < temp_num_opin_rr_nodes[1]; inode++) {
        cur_sb->opin_rr_node[side][inode + temp_num_opin_rr_nodes[0]] = temp_opin_rr_node[1][inode];
        cur_sb->opin_rr_node_grid_side[side][inode + temp_num_opin_rr_nodes[0]] = 0; /* Grid[x+1][y] TOP side outputs pins */
      }
      /* We do not have any IPIN for a Switch Block */
      cur_sb->num_ipin_rr_nodes[side] = 0;
      cur_sb->ipin_rr_node[side] = NULL;
      cur_sb->ipin_rr_node_grid_side[side] = NULL;
      break;
    case 2:
      /* For the bording, we should take special care */
      if (sb_y == 0) {
        cur_sb->chan_width[side] = 0;
        cur_sb->chan_rr_node[side] = NULL;
        cur_sb->chan_rr_node_direction[side] = NULL;
        cur_sb->num_ipin_rr_nodes[side] = 0;
        cur_sb->ipin_rr_node[side] = NULL;
        cur_sb->ipin_rr_node_grid_side[side] = NULL;
        cur_sb->num_opin_rr_nodes[side] = 0;
        cur_sb->opin_rr_node[side] = NULL;
        cur_sb->opin_rr_node_grid_side[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = sb_x; 
      iy = sb_y;
      /* Channel width */
      cur_sb->chan_width[side] = chan_width_y[ix];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Alloc */
      cur_sb->chan_rr_node[side] = get_chan_rr_nodes(&(cur_sb->chan_width[side]), CHANY, ix, iy, 
                                                 LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      cur_sb->chan_rr_node_direction[side] = (enum PORTS*)my_malloc(sizeof(enum PORTS)*cur_sb->chan_width[side]);
      /* Collect rr_nodes for Tracks for bottom: chany[x][y] */
      for (itrack = 0; itrack < cur_sb->chan_width[side]; itrack++) {
        /* Identify the directionality, record it in rr_node_direction */
        if (DEC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction) {
          cur_sb->chan_rr_node_direction[side][itrack] = OUT_PORT;
        } else {
          assert (INC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction);
          cur_sb->chan_rr_node_direction[side][itrack] = IN_PORT;
        }
      }
      /* TODO: include Grid[x+1][y] Left side output pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x + 1, sb_y, 3,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* TODO: include Grid[x][y] Right side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x, sb_y, 1,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Allocate opin_rr_node */
      cur_sb->num_opin_rr_nodes[side] = temp_num_opin_rr_nodes[0] + temp_num_opin_rr_nodes[1];
      cur_sb->opin_rr_node[side] = (t_rr_node**)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(t_rr_node*));
      cur_sb->opin_rr_node_grid_side[side] = (int*)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(int));
      /* Copy from temp_opin_rr_node to opin_rr_node */
      for (inode = 0; inode < temp_num_opin_rr_nodes[0]; inode++) {
        cur_sb->opin_rr_node[side][inode] = temp_opin_rr_node[0][inode];
        cur_sb->opin_rr_node_grid_side[side][inode] = 3; /* Grid[x+1][y] LEFT side outputs pins */
      }
      for (inode = 0; inode < temp_num_opin_rr_nodes[1]; inode++) {
        cur_sb->opin_rr_node[side][inode + temp_num_opin_rr_nodes[0]] = temp_opin_rr_node[1][inode];
        cur_sb->opin_rr_node_grid_side[side][inode + temp_num_opin_rr_nodes[0]] = 1; /* Grid[x][y] RIGHT side outputs pins */
      }
      /* We do not have any IPIN for a Switch Block */
      cur_sb->num_ipin_rr_nodes[side] = 0;
      cur_sb->ipin_rr_node[side] = NULL;
      cur_sb->ipin_rr_node_grid_side[side] = NULL;
      break;
    case 3:
      /* For the bording, we should take special care */
      if (sb_x == 0) {
        cur_sb->chan_width[side] = 0;
        cur_sb->chan_rr_node[side] = NULL;
        cur_sb->chan_rr_node_direction[side] = NULL;
        cur_sb->num_ipin_rr_nodes[side] = 0;
        cur_sb->ipin_rr_node[side] = NULL;
        cur_sb->ipin_rr_node_grid_side[side] = NULL;
        cur_sb->num_opin_rr_nodes[side] = 0;
        cur_sb->opin_rr_node[side] = NULL;
        cur_sb->opin_rr_node_grid_side[side] = NULL;
        break;
      }
      /* Routing channels*/
      ix = sb_x; 
      iy = sb_y;
      /* Channel width */
      cur_sb->chan_width[side] = chan_width_x[iy];
      /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
      /* Alloc */
      cur_sb->chan_rr_node[side] = get_chan_rr_nodes(&(cur_sb->chan_width[side]), CHANX, ix, iy, 
                                                     LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      cur_sb->chan_rr_node_direction[side] = (enum PORTS*)my_malloc(sizeof(enum PORTS)*cur_sb->chan_width[side]);
      /* Collect rr_nodes for Tracks for left: chanx[x][y] */
      for (itrack = 0; itrack < cur_sb->chan_width[side]; itrack++) {
        /* Identify the directionality, record it in rr_node_direction */
        if (DEC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction) {
          cur_sb->chan_rr_node_direction[side][itrack] = OUT_PORT;
        } else {
          assert (INC_DIRECTION == cur_sb->chan_rr_node[side][itrack]->direction);
          cur_sb->chan_rr_node_direction[side][itrack] = IN_PORT;
        }
      }
      /* include Grid[x][y+1] Bottom side outputs pins */
      temp_opin_rr_node[0] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[0], 
                                                        OPIN, sb_x, sb_y + 1, 2,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* include Grid[x][y] Top side output pins */
      temp_opin_rr_node[1] = get_grid_side_pin_rr_nodes(&temp_num_opin_rr_nodes[1], 
                                                        OPIN, sb_x, sb_y, 0,
                                                        LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
      /* Allocate opin_rr_node */
      cur_sb->num_opin_rr_nodes[side] = temp_num_opin_rr_nodes[0] + temp_num_opin_rr_nodes[1];
      cur_sb->opin_rr_node[side] = (t_rr_node**)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(t_rr_node*));
      cur_sb->opin_rr_node_grid_side[side] = (int*)my_calloc(cur_sb->num_opin_rr_nodes[side], sizeof(int));
      /* Copy from temp_opin_rr_node to opin_rr_node */
      for (inode = 0; inode < temp_num_opin_rr_nodes[0]; inode++) {
        cur_sb->opin_rr_node[side][inode] = temp_opin_rr_node[0][inode];
        cur_sb->opin_rr_node_grid_side[side][inode] = 2; /* Grid[x][y+1] BOTTOM side outputs pins */
      }
      for (inode = 0; inode < temp_num_opin_rr_nodes[1]; inode++) {
        cur_sb->opin_rr_node[side][inode + temp_num_opin_rr_nodes[0]] = temp_opin_rr_node[1][inode];
        cur_sb->opin_rr_node_grid_side[side][inode + temp_num_opin_rr_nodes[0]] = 0; /* Grid[x][y] TOP side outputs pins */
      }
      /* We do not have any IPIN for a Switch Block */
      cur_sb->num_ipin_rr_nodes[side] = 0;
      cur_sb->ipin_rr_node[side] = NULL;
      cur_sb->ipin_rr_node_grid_side[side] = NULL;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index!\n", __FILE__, __LINE__);
      exit(1);
    }
    /* Free */
    temp_num_opin_rr_nodes[0] = 0;
    my_free(temp_opin_rr_node[0]);
    temp_num_opin_rr_nodes[1] = 0;
    my_free(temp_opin_rr_node[1]);
    /* Set them to NULL, avoid double free errors */
    temp_opin_rr_node[0] = NULL;
    temp_opin_rr_node[1] = NULL;
  }

  return;
}

/* Build arrays for rr_nodes of each Switch Blocks 
 * Return a two-dimension array (t_rr_node**), 
 * each element of which is the entry of a two-dimension array of rr_node pointers (t_rr_node***)
 * according to their location in a switch block
 * Therefore, the return data type is (t_rr_node*****)
 */
static 
void alloc_and_build_switch_blocks_info(t_det_routing_arch RoutingArch,
                                        int LL_num_rr_nodes,
                                        t_rr_node* LL_rr_node,
                                        t_ivec*** LL_rr_node_indices) {
  int ix, iy;

  sb_info = alloc_sb_info_array(nx, ny);

  /* For each switch block, determine the size of array */
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      build_one_switch_block_info(&sb_info[ix][iy], ix, iy, RoutingArch, 
                                  LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    }
  }

  return;
}

/* Collect rr_nodes information for a connection box
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
void build_one_connection_block_info(t_cb* cur_cb, int cb_x, int cb_y, t_rr_type cb_type,
                                     int LL_num_rr_nodes,
                                     t_rr_node* LL_rr_node,
                                     t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side;

  /* Check */
  assert((!(0 > cb_x))&&(!(cb_x > (nx + 1)))); 
  assert((!(0 > cb_y))&&(!(cb_y > (ny + 1)))); 

  /* Fill basic information */
  cur_cb->x = cb_x;
  cur_cb->y = cb_y;
  cur_cb->type = cb_type;

  /* Record the channel width of each side*/
  cur_cb->chan_width = (int*)my_calloc(cur_cb->num_sides, sizeof(int)); /* 4 sides */
  cur_cb->chan_rr_node_direction = (enum PORTS**)my_malloc(sizeof(enum PORTS*)*cur_cb->num_sides); /* 4 sides */
  cur_cb->chan_rr_node = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*cur_cb->num_sides); /* 4 sides*/
  cur_cb->num_ipin_rr_nodes = (int*)my_calloc(cur_cb->num_sides, sizeof(int)); /* 4 sides */
  cur_cb->ipin_rr_node = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*cur_cb->num_sides); /* 4 sides*/
  cur_cb->ipin_rr_node_grid_side = (int**)my_calloc(cur_cb->num_sides, sizeof(int*)); /* 4 sides */
  cur_cb->num_opin_rr_nodes = (int*)my_calloc(cur_cb->num_sides, sizeof(int)); /* 4 sides */
  cur_cb->opin_rr_node = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*cur_cb->num_sides); /* 4 sides*/
  cur_cb->opin_rr_node_grid_side = (int**)my_calloc(cur_cb->num_sides, sizeof(int*)); /* 4 sides */

  /* Identify the type of connection box, the rr_nodes are different depending on the type */
  /* Side: TOP => 0, RIGHT => 1, BOTTOM => 2, LEFT => 3 */
  for (side = 0; side < 4; side++) {
    switch (side) {
    case 0: /* TOP */
      switch(cb_type) { 
      case CHANX:
        /* BOTTOM INPUT Pins of Grid[x][y+1] */
        /* Collect IPIN rr_nodes*/ 
        cur_cb->ipin_rr_node[side] = get_grid_side_pin_rr_nodes(&(cur_cb->num_ipin_rr_nodes[side]), 
                                                                IPIN, cb_x, cb_y + 1, 2,
                                                                LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

        cur_cb->ipin_rr_node_grid_side[side] = (int*)my_calloc(cur_cb->num_ipin_rr_nodes[side], sizeof(int));
        for (inode = 0; inode < cur_cb->num_ipin_rr_nodes[side]; inode++) {
          cur_cb->ipin_rr_node_grid_side[side][inode] = 2; /* BOTTOM IPINs */
        }
        /* Update channel width, num_opin_rr_nodes */
        cur_cb->chan_width[side] = 0;
        /* Identify the directionality, record it in rr_node_direction */
        cur_cb->chan_rr_node[side] = NULL;
        cur_cb->chan_rr_node_direction[side] = NULL;
        /* There is no OPIN nodes at this side */
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
        break; 
      case CHANY:
        /* Collect channel-Y [x][y] rr_nodes*/
        /* Update channel width */
        cur_cb->chan_rr_node[side] = get_chan_rr_nodes(&(cur_cb->chan_width[side]), CHANY, cb_x, cb_y, 
                                                       LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
        /* Alloc */
        cur_cb->chan_rr_node_direction[side] = (enum PORTS*)my_malloc(sizeof(enum PORTS)*cur_cb->chan_width[side]);
        /* Collect rr_nodes for Tracks for left: chanx[x][y] */
        for (itrack = 0; itrack < cur_cb->chan_width[side]; itrack++) {
          /* Identify the directionality, record it in rr_node_direction */
          cur_cb->chan_rr_node_direction[side][itrack] = IN_PORT;
        }
        /* There is no IPIN nodes at this side */
        cur_cb->num_ipin_rr_nodes[side] = 0;
        cur_cb->ipin_rr_node[side] = NULL;
        cur_cb->ipin_rr_node_grid_side[side] = NULL;
        /* There is no OPIN nodes at this side */
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 1: /* RIGHT */
      switch(cb_type) { 
      case CHANX:
        /* Collect channel-X [x][y] rr_nodes*/
        /* Update channel width */
        cur_cb->chan_rr_node[side] = get_chan_rr_nodes(&(cur_cb->chan_width[side]), CHANX, cb_x, cb_y, 
                                                       LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
        /* Alloc */
        cur_cb->chan_rr_node_direction[side] = (enum PORTS*)my_malloc(sizeof(enum PORTS)*cur_cb->chan_width[side]);
        /* Collect rr_nodes for Tracks for left: chanx[x][y] */
        for (itrack = 0; itrack < cur_cb->chan_width[side]; itrack++) {
          /* Identify the directionality, record it in rr_node_direction */
          cur_cb->chan_rr_node_direction[side][itrack] = IN_PORT;
        }
        /* There is no IPIN nodes at this side */
        cur_cb->num_ipin_rr_nodes[side] = 0;
        cur_cb->ipin_rr_node[side] = NULL;
        cur_cb->ipin_rr_node_grid_side[side] = NULL;
        /* There is no OPIN nodes at this side */
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
        break; 
      case CHANY:
        /* LEFT INPUT Pins of Grid[x+1][y] */
        /* Collect IPIN rr_nodes*/ 
        cur_cb->ipin_rr_node[side] = get_grid_side_pin_rr_nodes(&(cur_cb->num_ipin_rr_nodes[side]), 
                                                                IPIN, cb_x + 1, cb_y, 3,
                                                                LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
        cur_cb->ipin_rr_node_grid_side[side] = (int*)my_calloc(cur_cb->num_ipin_rr_nodes[side], sizeof(int));
        for (inode = 0; inode < cur_cb->num_ipin_rr_nodes[side]; inode++) {
          cur_cb->ipin_rr_node_grid_side[side][inode] = 3; /* LEFT IPINs */
        }
        /* Update channel width, num_opin_rr_nodes */
        cur_cb->chan_width[side] = 0;
        /* Identify the directionality, record it in rr_node_direction */
        cur_cb->chan_rr_node[side] = NULL;
        cur_cb->chan_rr_node_direction[side] = NULL;
        /* There is no OPIN nodes at this side */
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 2: /* BOTTOM */
      switch(cb_type) { 
      case CHANX:
        /* TOP INPUT Pins of Grid[x][y] */
        /* Collect IPIN rr_nodes*/ 
        cur_cb->ipin_rr_node[side] = get_grid_side_pin_rr_nodes(&(cur_cb->num_ipin_rr_nodes[side]), 
                                                                IPIN, cb_x, cb_y, 0, 
                                                                LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
        cur_cb->ipin_rr_node_grid_side[side] = (int*)my_calloc(cur_cb->num_ipin_rr_nodes[side], sizeof(int));
        for (inode = 0; inode < cur_cb->num_ipin_rr_nodes[side]; inode++) {
          cur_cb->ipin_rr_node_grid_side[side][inode] = 0; /* TOP IPINs */
        }
        /* Update channel width, num_opin_rr_nodes */
        cur_cb->chan_width[side] = 0;
        /* Identify the directionality, record it in rr_node_direction */
        cur_cb->chan_rr_node[side] = NULL;
        cur_cb->chan_rr_node_direction[side] = NULL;
        /* There is no OPIN nodes at this side */
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
        break; 
      case CHANY:
        /* Nothing should be done other than setting NULL pointers and zero counter*/
        /* There is no input and output rr_nodes at this side */
        cur_cb->chan_width[side] = 0;
        cur_cb->chan_rr_node[side] = NULL;
        cur_cb->chan_rr_node_direction[side] = NULL;
        cur_cb->num_ipin_rr_nodes[side] = 0;
        cur_cb->ipin_rr_node[side] = NULL;
        cur_cb->ipin_rr_node_grid_side[side] = NULL;
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 3: /* LEFT */
      switch(cb_type) { 
      case CHANX:
        /* Nothing should be done other than setting NULL pointers and zero counter*/
        /* There is no input and output rr_nodes at this side */
        cur_cb->chan_width[side] = 0;
        cur_cb->chan_rr_node[side] = NULL;
        cur_cb->chan_rr_node_direction[side] = NULL;
        cur_cb->num_ipin_rr_nodes[side] = 0;
        cur_cb->ipin_rr_node[side] = NULL;
        cur_cb->ipin_rr_node_grid_side[side] = NULL;
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
        break; 
      case CHANY:
        /* RIGHT INPUT Pins of Grid[x][y] */
        /* Collect IPIN rr_nodes*/ 
        cur_cb->ipin_rr_node[side] = get_grid_side_pin_rr_nodes(&(cur_cb->num_ipin_rr_nodes[side]), 
                                                                IPIN, cur_cb->x, cur_cb->y, 1, 
                                                                LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
        cur_cb->ipin_rr_node_grid_side[side] = (int*)my_calloc(cur_cb->num_ipin_rr_nodes[side], sizeof(int));
        for (inode = 0; inode < cur_cb->num_ipin_rr_nodes[side]; inode++) {
          cur_cb->ipin_rr_node_grid_side[side][inode] = 1; /* RIGHT IPINs */
        }
        /* Update channel width, num_opin_rr_nodes */
        cur_cb->chan_width[side] = 0;
        /* Identify the directionality, record it in rr_node_direction */
        cur_cb->chan_rr_node[side] = NULL;
        cur_cb->chan_rr_node_direction[side] = NULL;
        /* There is no OPIN nodes at this side */
        cur_cb->num_opin_rr_nodes[side] = 0;
        cur_cb->opin_rr_node[side] = NULL;
        cur_cb->opin_rr_node_grid_side[side] = NULL;
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
  }
  
  return;
}
 

/* Build arrays for rr_nodes of each Connection Blocks 
 * Different from Switch blocks, we have two types of Connection blocks:
 * 1. Connecting X-channels to CLB inputs 
 * 2. Connection Y-channels to CLB inputs 
 */
static 
void alloc_and_build_connection_blocks_info(t_det_routing_arch RoutingArch,
                                            int LL_num_rr_nodes,
                                            t_rr_node* LL_rr_node,
                                            t_ivec*** LL_rr_node_indices) {
  int ix, iy;

  /* alloc CB for X-channels */
  cbx_info = alloc_cb_info_array(nx, ny);
  /* Fill information for each CBX*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      build_one_connection_block_info(&(cbx_info[ix][iy]), ix, iy, CHANX,
                                      LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    }
  }

  /* alloc CB for Y-channels */
  cby_info = alloc_cb_info_array(nx, ny);
  /* Fill information for each CBX*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      build_one_connection_block_info(&(cby_info[ix][iy]), ix, iy, CHANY,
                                      LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    }
  }

  return;
}

/* Free all the allocated memories by function:
 * either spice_backannotate_vpr_post_route_info or backannotate_vpr_post_route_info*/
void free_backannotate_vpr_post_route_info() {
  /* Free spice_net_info */
  free_clb_nets_spice_net_info();
  /* Free CB and SB info */
  free_sb_info_array(&sb_info, nx, ny); 
  free_cb_info_array(&cbx_info, nx, ny); 
  free_cb_info_array(&cby_info, nx, ny); 
}

static 
boolean ipin_rr_nodes_vpack_net_num_changed(int LL_num_rr_nodes,
                                            t_rr_node* LL_rr_node) {
  int inode;

  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    if (IPIN != LL_rr_node[inode].type) {
      continue;
    }
    /* We only care IPINs */
    if (TRUE == LL_rr_node[inode].vpack_net_num_changed) {
      return TRUE;
    }
  }

  return FALSE;
}

static 
void parasitic_net_estimation() {
  int iter_cnt = 0;
  boolean iter_continue = FALSE;


  vpr_printf(TIO_MESSAGE_INFO, "Start backannotating global and local routing nets iteratively...\n");
  while(1) {
    iter_cnt++;

    init_rr_nodes_vpack_net_num_changed(num_rr_nodes,
                                        rr_node);

    init_rr_nodes_is_parasitic_net(num_rr_nodes,
                                   rr_node);

    /*
     * vpr_printf(TIO_MESSAGE_INFO, "Backannotating local routing net...\n");
    backannotate_pb_rr_nodes_net_info();
     */

    /* Update CLB pins parasitic nets:
     * Traverse from inputs of CLBs to outputs.
     * Mark parasitics nets propagated from input pins to output pins inside CLBs
     */ 
    update_grid_pb_pins_parasitic_nets();

    /* Backannoating global routing parasitic net... 
     * Traverse from OPINs in rr_graph to IPINs.
     * Mark parasitic nets in the rr_graph 
     */
    backannotate_rr_nodes_parasitic_net_info();

    /* Check vpack_net_num of all the OPINs are consistant */
    /* Consistency means iterations end here */
    iter_continue = ipin_rr_nodes_vpack_net_num_changed(num_rr_nodes, 
                                                        rr_node);

    if (FALSE == iter_continue) {
      break;
    }
  }

  vpr_printf(TIO_MESSAGE_INFO,"Parasitic net estimation ends in %d iterations.\n", iter_cnt);

  return;
}

/* Annotate the physical_mode_pin in pb_type ports,
 * Go recursively until we reach a primtiive node 
 */
void rec_annotate_pb_type_primitive_node_physical_mode_pin(t_pb_type* top_pb_type,
                                                           t_pb_type* cur_pb_type) {
  int imode, ipb;

  /* See if this is a primitive pb_graph_node */
  if (FALSE == is_primitive_pb_type(cur_pb_type)) { 
    /* Check each mode*/
    for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
      /* bypass physical modes only when there are more than 1 modes available ! */
      if ((1 == cur_pb_type->modes[imode].define_physical_mode) 
          &&(1 < cur_pb_type->num_modes)) {
        continue;
      }
      /* Quote all child pb_types */
      for (ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
        /* we should make sure this placement index == child_pb_type[jpb]*/
        rec_annotate_pb_type_primitive_node_physical_mode_pin(top_pb_type,
                                                              &(cur_pb_type->modes[imode].pb_type_children[ipb]));
      }
    }
    return;
  }

  /* Reach here, it means a primitive mode */ 
  assert (TRUE == is_primitive_pb_type(cur_pb_type)); 
  /* Check the physical pb_type  */
  /* Find the physical_pb_type with the name provided */
  cur_pb_type->phy_pb_type = rec_get_pb_type_by_name(top_pb_type, cur_pb_type->physical_pb_type_name);
  /* Overwrite class type */
  if (UNKNOWN_CLASS != cur_pb_type->class_type) {
    cur_pb_type->phy_pb_type->class_type = cur_pb_type->class_type;
  }
  vpr_printf(TIO_MESSAGE_INFO,
             "Link physical pb_type (name=%s) for pb_type (name=%s)!\n",
             cur_pb_type->physical_pb_type_name, cur_pb_type->name);
  /* Check: We should find one! */
  if (NULL == cur_pb_type->phy_pb_type) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Fail to find physical pb_type (name=%s) for pb_type (name=%s)!\n",
                __FILE__, __LINE__, cur_pb_type->physical_pb_type_name, cur_pb_type->name);
    exit(1);
  }
  /* Check: the one should be linked to a SPICE model ! */
  if (NULL == cur_pb_type->phy_pb_type->spice_model) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Found physical pb_type (name=%s) for pb_type (name=%s) does not have a SPICE model definition!\n",
                __FILE__, __LINE__, cur_pb_type->physical_pb_type_name, cur_pb_type->name);
    exit(1);
  }
  /* Check: the one should be in a physical mode! */
  if (FALSE == cur_pb_type->phy_pb_type->parent_mode->define_physical_mode) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Found physical pb_type (name=%s) for pb_type (name=%s) does not belong to a physical mode!\n",
                __FILE__, __LINE__, cur_pb_type->physical_pb_type_name, cur_pb_type->name);
    exit(1);
  }

  /* Now we are sure about the phy_pb_type that is found */
  /* Find matched port one by one */
  annotate_pb_type_port_to_phy_pb_type(cur_pb_type, cur_pb_type->phy_pb_type);

  return;
}

/* Annotate the physical_mode_pin in pb_type ports,
 * Go recursively until we reach a primtiive node 
 */
void rec_annotate_phy_pb_type_primitive_node_physical_mode_pin(t_pb_type* top_pb_type,
                                                               t_pb_type* cur_pb_type) {
  int phy_mode_idx, ipb, iport;
  boolean is_top_pb_type = (top_pb_type == cur_pb_type) ? TRUE : FALSE;

  /* For top_pb_type: itself is a physical pb_type */
  if ((TRUE == is_top_pb_type) 
    || (TRUE == is_primitive_pb_type(cur_pb_type))) { 
    /* Reach here, it means a primitive mode */ 
    /* Check the physical pb_type  */
    /* Find the physical_pb_type with the name provided */
    cur_pb_type->phy_pb_type = cur_pb_type;
    /* Overwrite class type */
    if (UNKNOWN_CLASS != cur_pb_type->class_type) {
      cur_pb_type->phy_pb_type->class_type = cur_pb_type->class_type;
    }
    /* Now we are sure about the phy_pb_type that is found */
    /* Find matched port one by one */
    for (iport = 0; iport < cur_pb_type->num_ports; iport++) { 
      cur_pb_type->ports[iport].phy_pb_type_port = &(cur_pb_type->ports[iport]); 
      cur_pb_type->ports[iport].phy_pb_type_port_lsb = 0; 
      cur_pb_type->ports[iport].phy_pb_type_port_msb = cur_pb_type->ports[iport].num_pins - 1; 
    }
    /* return when it is a primitive node */
    if (TRUE == is_primitive_pb_type(cur_pb_type)) { 
      /* ONLY FOR PRIMITIVE NODE: Copy the pb_type_name from the name of cur_pb_type */
      cur_pb_type->physical_pb_type_name = my_strdup(cur_pb_type->name);
      /* Copy the name of ports to the physical_mode_pin to the port itself  */
      for (iport = 0; iport < cur_pb_type->num_ports; iport++) { 
        cur_pb_type->ports[iport].physical_mode_pin = my_strdup(cur_pb_type->ports[iport].name); 
      }
      return;
    }
  }

  /* See if this is a primitive pb_graph_node */
  if (FALSE == is_primitive_pb_type(cur_pb_type)) { 
    if (FALSE == is_top_pb_type) {
      cur_pb_type->phy_pb_type = NULL; /* Special for top_pb_type */
    }
    /* we only care physical modes */
    phy_mode_idx = find_pb_type_physical_mode_index((*cur_pb_type));
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[phy_mode_idx].num_pb_type_children; ipb++) {
      /* we should make sure this placement index == child_pb_type[jpb]*/
      rec_annotate_phy_pb_type_primitive_node_physical_mode_pin(top_pb_type,
                                                                &(cur_pb_type->modes[phy_mode_idx].pb_type_children[ipb]));
    }
  }

  return;
}

/* Go recursively visiting each primitive node in the pb_graph_node 
 * Label the primitive node with a placement index which is unique at the top-level node 
 */
void rec_mark_pb_graph_node_primitive_placement_index_in_top_node(t_pb_graph_node* cur_pb_graph_node) {
  int imode, ipb, jpb;
  t_pb_type* cur_pb_type = NULL;

  cur_pb_type = cur_pb_graph_node->pb_type;

  /* See if this is a primitive pb_graph_node */
  if (FALSE == is_primitive_pb_type(cur_pb_type)) { 
    /* Check each mode*/
    for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
      /* Quote all child pb_types */
      for (ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
        /* Each child may exist multiple times in the hierarchy*/
        for (jpb = 0; jpb < cur_pb_type->modes[imode].pb_type_children[ipb].num_pb; jpb++) {
          /* we should make sure this placement index == child_pb_type[jpb]*/
          assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb].placement_index);
          rec_mark_pb_graph_node_primitive_placement_index_in_top_node(&(cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]));
        }
      }
    }
    return;
  }

  /* Reach here, it means a primitive mode */ 
  assert (NULL != cur_pb_type->phy_pb_type );
  /* Label placement_index_in_top_node */ 
  cur_pb_graph_node->placement_index_in_top_node = cur_pb_type->temp_placement_index;
  cur_pb_type->temp_placement_index++;

  return;
}

/* Recursively go to the primitive pb_graph_node
 * create a link from the primitive pb_graph_node to its physical pb_graph_pin */
void rec_link_primitive_pb_graph_node_pin_to_phy_pb_graph_pin(t_pb_graph_node* top_pb_graph_node,
                                                              t_pb_graph_node* cur_pb_graph_node) {
  int imode, ipb, jpb;
  int physical_pb_graph_node_placement_index = -1;
  t_pb_type* cur_pb_type = NULL;
  t_pb_graph_node* phy_pb_graph_node = NULL;
  boolean is_top_pb_graph_node = (top_pb_graph_node == cur_pb_graph_node) ? TRUE : FALSE;

  cur_pb_type = cur_pb_graph_node->pb_type;

  /* For top_pb_type: itself is a physical pb_type */
  if (TRUE == is_top_pb_graph_node) {
    /* Reach here, it means a primitive mode */ 
    assert (NULL != cur_pb_type->phy_pb_type);
    /* Create linkes between pb_graph_pins and pb_graph_nodes */
    cur_pb_graph_node->physical_pb_graph_node = top_pb_graph_node; 
    link_pb_graph_node_pins_to_phy_pb_graph_pins(top_pb_graph_node, top_pb_graph_node->physical_pb_graph_node);
  }

  /* For primitive node */
  if (TRUE == is_primitive_pb_type(cur_pb_type)) { 
    /* Reach here, it means a primitive mode */ 
    assert (NULL != cur_pb_type->phy_pb_type);
    /* Get the physical pb_graph_node with the scaled placement_index! */
    physical_pb_graph_node_placement_index = (int) (cur_pb_type->physical_pb_type_index_factor 
                                                    * (float) cur_pb_graph_node->placement_index_in_top_node)
                                                    + cur_pb_type->physical_pb_type_index_offset;
    phy_pb_graph_node = rec_get_pb_graph_node_by_pb_type_and_placement_index_in_top_node(top_pb_graph_node, 
                                                                                         cur_pb_type->phy_pb_type,
                                                                                         physical_pb_graph_node_placement_index);
    assert(NULL != phy_pb_graph_node);
    /* Create linkes between pb_graph_pins and pb_graph_nodes */
    cur_pb_graph_node->physical_pb_graph_node = phy_pb_graph_node; 
    link_pb_graph_node_pins_to_phy_pb_graph_pins(cur_pb_graph_node, cur_pb_graph_node->physical_pb_graph_node);
    return;
  }

  /* See if this is a primitive pb_graph_node */
  if (FALSE == is_primitive_pb_type(cur_pb_type)) { 
    /* Check each mode*/
    for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
      /* bypass physical modes only when there are more than 1 modes available ! */
      if ((1 == cur_pb_type->modes[imode].define_physical_mode) 
          &&(1 < cur_pb_type->num_modes)) {
        continue;
      }
      /* Quote all child pb_types */
      for (ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
        /* Each child may exist multiple times in the hierarchy*/
        for (jpb = 0; jpb < cur_pb_type->modes[imode].pb_type_children[ipb].num_pb; jpb++) {
          /* we should make sure this placement index == child_pb_type[jpb]*/
          assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb].placement_index);
          rec_link_primitive_pb_graph_node_pin_to_phy_pb_graph_pin(top_pb_graph_node,
                                                                   &(cur_pb_graph_node->child_pb_graph_nodes[imode][ipb][jpb]));
        }
      }
    }
  }

  
  return;
}

/* Back-annotate the physical mode pins defined in pb_type to pb_graph_node
 * For each pb_graph_node available in type->pb_graph_head 
 * 1. Label each primitive nodes with the placement_index_top_node
 * 2. find the pb_graph_pin of primitive nodes in the pb_graph_node
 * 3. identify the physical mode pin definition (in pb_type)
 * 4. create a link to the pb_graph_pin in physical mode
 */
void annotate_physical_mode_pins_in_pb_graph_node() {
  int itype;
  
  for (itype = 0; itype < num_types; itype++) {
    /* Bybass NULL/EMPTY_TYPE */
    if (EMPTY_TYPE == &type_descriptors[itype]) {
      continue; 
    }
    /* reset phy_pb_type in all the pb_types  */
    rec_reset_pb_type_phy_pb_type(type_descriptors[itype].pb_type);
    /* reset the rr_node_index_physical_pb of each pb_graph_pin to be OPEN ! */
    rec_reset_pb_graph_node_rr_node_index_physical_pb(type_descriptors[itype].pb_graph_head);
    /* annotate the physical mode pins in the physical primitive pb_type*/
    rec_annotate_phy_pb_type_primitive_node_physical_mode_pin(type_descriptors[itype].pb_type, type_descriptors[itype].pb_type);
    /* annotate the physical mode pins in the primitive pb_type*/
    rec_annotate_pb_type_primitive_node_physical_mode_pin(type_descriptors[itype].pb_type, type_descriptors[itype].pb_type);
    /* reset temp_placement_index in all the pb_types  */
    rec_reset_pb_type_temp_placement_index(type_descriptors[itype].pb_type);
    /* Recursively find the primitive pb_grpah_nodes */
    rec_mark_pb_graph_node_primitive_placement_index_in_top_node(type_descriptors[itype].pb_graph_head);
    /* Recursively link the pb_graph_pin of primitive nodes in operating pb to physical pb */
    rec_link_primitive_pb_graph_node_pin_to_phy_pb_graph_pin(type_descriptors[itype].pb_graph_head, type_descriptors[itype].pb_graph_head);
  }
 
  return;
}

/* Allocate pb in mapped blocks, corresponding to physical modes  */
void alloc_and_load_phy_pb_for_mapped_block(int num_mapped_blocks, t_block* mapped_block,
                                            int L_num_vpack_nets, t_net* L_vpack_net) {
  int iblk;
  t_phy_pb* top_phy_pb = NULL;
  boolean route_success = FALSE;

  for (iblk = 0; iblk < num_mapped_blocks; iblk++) {
    vpr_printf(TIO_MESSAGE_INFO, 
               "Start backannotate clb (%s) to its physical pb!\n", 
               mapped_block[iblk].pb->name);
    top_phy_pb = (t_phy_pb*) my_calloc(1, sizeof(t_phy_pb)); 
    /* Create a pristine pb for pb_graph_nodes in the physical modes */
    top_phy_pb->pb_graph_node = mapped_block[iblk].type->pb_graph_head;
    /* alloc_and_load_pb_stats(maped_block[iblk].phy_pb, num_models, max_nets_in_pb_type); */
    top_phy_pb->parent_pb = NULL;
    top_phy_pb->mode = 0; /* Top-level should have only one mode!!! */
    /* Allocate rr_graph for the phy_pb */
    /* vpr_printf(TIO_MESSAGE_INFO, "Allocating routing resource graph for %d physical pb!\r", iblk);
    */
    alloc_and_load_rr_graph_for_phy_pb(mapped_block[iblk].pb, top_phy_pb, L_num_vpack_nets, L_vpack_net); 
    /* Perform routing for the phy_pb !!! */
    route_success = try_breadth_first_route_pb_rr_graph(top_phy_pb->rr_graph);
    if (TRUE == route_success) { 
      /* vpr_printf(TIO_MESSAGE_INFO, "Route successfully for %d physical pbs!\r", iblk); */
    } else {
      assert(FALSE == route_success);
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(File:%s,[LINE%d]) Route fail for physical pb (x=%d, y=%d, type_name=%s)!\n",
                  __FILE__, __LINE__, mapped_block[iblk].x, mapped_block[iblk].y, mapped_block[iblk].type->name);
      exit(1);
    }
    /* Backannotate routing results to physical pb_rr_graph */
    backannotate_rr_graph_routing_results_to_net_name(top_phy_pb->rr_graph);
    vpr_printf(TIO_MESSAGE_INFO, 
               "Backannotate routing results successfully for physical pb (%s)!\n",
               mapped_block[iblk].pb->name);
    /* Allocate and load child_pb graphs */
    alloc_and_load_phy_pb_children_for_one_mapped_block(mapped_block[iblk].pb, top_phy_pb);
    /* Give top_phy_pb to grid */
    mapped_block[iblk].phy_pb = (void*)top_phy_pb;
    /* Create a link from pb to phy_pb */
    mapped_block[iblk].pb->phy_pb = (void*)top_phy_pb;
  }
  vpr_printf(TIO_MESSAGE_INFO, "\n");
  vpr_printf(TIO_MESSAGE_INFO, "\n");

  return;
}

/* This function does the following tasks:
 * 1. Find the wired LUTs in pb recursively(used as buffers)
 * 2. Allocate the pb (if not allocated)
 * 3. Update the mapping information (net_num) in the rr_graph of pb
 * 4. Create the wired LUTs in logical block array
 * 5. Create new vpack nets to rewire the logical blocks
 */
void rec_backannotate_one_pb_wired_luts_and_adapt_graph(t_pb* cur_pb,
                                                        int* L_num_logical_blocks, t_net** L_logical_block,
                                                        int* L_num_vpack_nets, t_net** L_vpack_net) {
  int mode_index, ipb, jpb; 
  t_pb_type* cur_pb_type = NULL;
  t_pb* lut_child_pb = NULL;

  cur_pb_type = cur_pb->pb_graph_node->pb_type;
  mode_index = cur_pb->mode; 

  /* Go recursively until we reach a primitive node which is a LUT */

  /* Return if we reach the primitive  */
  if (TRUE == is_primitive_pb_type(cur_pb_type)) {
    /* We only care the LUTs, that is in wired mode */
    if ((LUT_CLASS != cur_pb_type->class_type)
       || (WIRED_LUT_MODE_INDEX != mode_index)) { 
      return;
    }
    /* Reach here means that this LUT is in wired mode (a buffer)  
     * Check if we need to allocate new logical block  
     */
    lut_child_pb = get_lut_child_pb(cur_pb, mode_index);
    assert (NULL != lut_child_pb);
    if (OPEN != lut_child_pb->logical_block) {
      return;
    }
    /* We need to 
     * 1. Add a new logical block 
     * 2. Add a new vpack_net 
     * 3. Update pb rr_graph 
     */
  }

  /* recursive for the child_pbs*/
  assert (FALSE == is_primitive_pb_type(cur_pb_type));
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Recursive*/
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        rec_backannotate_one_pb_wired_luts_and_adapt_graph(&(cur_pb->child_pbs[ipb][jpb]), 
                                                           L_num_logical_blocks, L_logical_block,
                                                           L_num_vpack_nets, L_vpack_net);
      } else {
        if (TRUE == is_pb_wired_lut(&(cur_pb->pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]),  
                                    &(cur_pb_type->modes[mode_index].pb_type_children[ipb]),
                                    cur_pb->rr_graph)) {        
          /* Reach here means that this LUT is in wired mode (a buffer) */ 
          /* 1. Allocate a child pb */
          allocate_wired_lut_pbs(&(cur_pb->child_pbs),
                                 cur_pb_type->modes[mode_index].num_pb_type_children,
                                 cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb,
                                 ipb, jpb);
          /* Load the LUT information to the pb */
          /* 2. Add a new logical block */ 
          /* 3. Add a new vpack_net */ 
          /*
          load_wired_lut_pbs(&(cur_pb->child_pbs[ipb][jpb]),
                             &(cur_pb->pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]),  
                             &(cur_pb_type->modes[mode_index].pb_type_children[ipb]),
                             cur_pb->rr_graph,    
                             L_num_logical_blocks, L_logical_block,
                             L_num_vpack_nets, L_vpack_net);
          */
          /* 4. Update pb rr_graph */ 
        }
      }
    }
  }

  return;
}

/* Back-annotate all the wired LUTs (used as buffers) in pbs 
 * and adapt BLIF graph and pb graph!
 */
static 
void backannotate_pb_wired_luts(int num_mapped_blocks, t_block* mapped_block,
                                int* L_num_logical_blocks, t_net** L_logical_block,
                                int* L_num_vpack_nets, t_net** L_vpack_net) {

  int iblk;

  for (iblk = 0; iblk < num_mapped_blocks; iblk++) {
    /* Find wired LUTs in each pb recusively */
    rec_backannotate_one_pb_wired_luts_and_adapt_graph(mapped_block[iblk].pb,
                                                       L_num_logical_blocks, L_logical_block,
                                                       L_num_vpack_nets, L_vpack_net);
  }

  vpr_printf(TIO_MESSAGE_INFO, "\n");

  return;
}

int find_matched_block_id_for_one_grid(int x, int y) {
  int iblk, jblk, blk_id;
  boolean already_exist = FALSE;

  /* We need to find a valid block ID here */
  for (iblk = 0; iblk < num_blocks; iblk++) {
    /* We only care the matched x,y coordinate */
    if ( (x != block[iblk].x)
      || (y != block[iblk].y)) {
      continue;
    }
    /* Now, we double check if the block is already in the list */
    already_exist = FALSE;
    for (jblk = 0; jblk < grid[x][y].usage; jblk++) {
      blk_id = grid[x][y].blocks[jblk];
      if (iblk != blk_id) {
        continue;
      }
      /* Already in the list, we do not return the value */
      already_exist = TRUE;
      break;
    }
    /* Return if does not exist */
    if (FALSE == already_exist) {
      return iblk;
    }
  } 

  return OPEN;
}

/* Some IO blocks has an invalid BLOCK ID but with a >0 usage 
 * We go through the block list and find the missing block ID
 */
void annotate_grid_block_info() {
  int ix, iy;
  t_type_ptr type = NULL;
  int iblk, blk_id;

  for (ix = 0; ix < (nx + 2); ix++) {
    for (iy = 0; iy < (ny + 2); iy++) {
      type = grid[ix][iy].type;
      /* bypass EMPTY type */
      if ((NULL == type) || (EMPTY_TYPE == type)) {
        continue;
      }
      if (IO_TYPE != type) {
        continue;
      } 
      for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
        blk_id = grid[ix][iy].blocks[iblk];
        if (OPEN != blk_id) {
          continue;
        }
        vpr_printf(TIO_MESSAGE_INFO, 
                   "Detect an invalid block id for grid[%d][%d] (usage:%d, blk_id:%d), trying to find a matched block from list!\n",
                   ix, iy, grid[ix][iy].usage, iblk);
        /* We detect an invalid blk_id, try to find one in the block list  */
        grid[ix][iy].blocks[iblk] = find_matched_block_id_for_one_grid(ix, iy);
        if (OPEN == grid[ix][iy].blocks[iblk]) { 
          vpr_printf(TIO_MESSAGE_WARNING,
                     "Fail to find a valid block id for grid[%d][%d] (usage:%d, blk_id:%d) in the block list!\n",
                     ix, iy, grid[ix][iy].usage, iblk);
        } else {
          vpr_printf(TIO_MESSAGE_INFO,
                     "Manage to find a valid block id (=%d) for grid[%d][%d]!\n",
                     grid[ix][iy].blocks[iblk], ix, iy);
        }
      }
    }
  }

  return;
}

/* Back-Annotate post routing results to the VPR routing-resource graphs */
void spice_backannotate_vpr_post_route_info(t_det_routing_arch RoutingArch,
                                            boolean read_activity_file,
                                            boolean parasitic_net_estimation_on) {

  vpr_printf(TIO_MESSAGE_INFO, "Start backannotating post route information for SPICE modeling...\n");

  /* Back-annotate the wired LUTs in pbs */
  /* Paused due to difficulties in identify which part of the LUT fan-out is wired....
  vpr_printf(TIO_MESSAGE_INFO, "Back-annotate wired LUTs in pbs...\n");
  backannotate_pb_wired_luts(num_blocks, block, 
                             &num_logical_blocks, &logical_block,
                             &num_logical_nets, &vpack_net);
  */

  /* Give spice_name_tag for each pb*/
  vpr_printf(TIO_MESSAGE_INFO, "Generate SPICE name tags for pbs...\n");
  gen_spice_name_tags_all_pbs();

  /* Build previous node lists for each rr_node */
  vpr_printf(TIO_MESSAGE_INFO, "Building previous node list for all Routing Resource Nodes...\n");
  build_prev_node_list_rr_nodes(num_rr_nodes, rr_node);

  /* Build driver switches for each rr_node*/
  vpr_printf(TIO_MESSAGE_INFO, "Identifying driver switches for all Routing Resource Nodes...\n");
  identify_rr_node_driver_switch(RoutingArch, num_rr_nodes, rr_node);

  /* Build Array for each Switch block and Connection block */ 
  vpr_printf(TIO_MESSAGE_INFO, "Collecting detailed information for each Switch block...\n");
  alloc_and_build_switch_blocks_info(RoutingArch, num_rr_nodes, rr_node, rr_node_indices);
  device_rr_switch_block = build_device_rr_switch_blocks(RoutingArch, num_rr_nodes, rr_node, rr_node_indices);

  vpr_printf(TIO_MESSAGE_INFO, "Collecting detailed information for each to Connection block...\n");
  alloc_and_build_connection_blocks_info(RoutingArch, num_rr_nodes, rr_node, rr_node_indices);

  /* This function should go very first because it gives all the net_num */
  vpr_printf(TIO_MESSAGE_INFO,"Back annotating mapping information to global routing resource nodes...\n");
  back_annotate_rr_node_map_info();

  /* Update local_rr_graphs to match post-route results*/
  vpr_printf(TIO_MESSAGE_INFO, "Update CLB local routing graph to match post-route results...\n");
  /* Complete the grid block information */
  annotate_grid_block_info();
  update_grid_pbs_post_route_rr_graph();

  vpr_printf(TIO_MESSAGE_INFO,"Back annotating mapping information to local routing resource nodes...\n");
  back_annotate_pb_rr_node_map_info();

  /* Annotate physical mode pins defined in each primitive pb_type/pb_graph_node */
  vpr_printf(TIO_MESSAGE_INFO, "Annotate physical mode pins for pbs ...\n");
  annotate_physical_mode_pins_in_pb_graph_node();
  alloc_and_load_phy_pb_for_mapped_block(num_blocks, block, num_logical_nets, vpack_net);

  vpr_printf(TIO_MESSAGE_INFO, "Generate SPICE name tags for phy_pbs...\n");
  gen_spice_name_tags_all_phy_pbs();

  /* Backannotate activity information, initialize the waveform information */
  /* Parasitic Net Activity Estimation */
  if (TRUE == parasitic_net_estimation_on) {
    vpr_printf(TIO_MESSAGE_INFO, "Parasitic Net Estimation starts...\n");
    parasitic_net_estimation();
  } else {
    vpr_printf(TIO_MESSAGE_WARNING, "Parasitic Net Estimation is turned off...Accuracy loss may be expected!\n");
  }

  /* Net activities */
  if (TRUE == read_activity_file) {
    vpr_printf(TIO_MESSAGE_INFO, "Backannoating Net activities...\n");
    backannotate_clb_nets_act_info();
    vpr_printf(TIO_MESSAGE_INFO, "Determine Net initial values...\n");
    backannotate_clb_nets_init_val();
  } else {
    vpr_printf(TIO_MESSAGE_INFO, "Net activity backannoation is bypassed...\n");
  }

  vpr_printf(TIO_MESSAGE_INFO, "Finish backannotating post route information for SPICE modeling.\n");

  return;
}

