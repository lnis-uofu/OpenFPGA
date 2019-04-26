/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "rr_graph_swseg.h"
#include "vpr_utils.h"
#include "route_common.h"

/* Include fpga_spice support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "spice_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_bitstream_utils.h"

/* Include spice support headers*/
#include "spice_utils.h"
#include "spice_lut.h"
#include "spice_pbtypes.h"
#include "spice_mux.h"

/***** Subroutines *****/

static 
void fprint_spice_mux_model_basis_cmos_subckt(FILE* fp, char* subckt_name,
                                              int num_input_per_level,
                                              t_spice_model spice_model,
                                              int mux_size,
                                              boolean special_basis) {
  char* pgl_name = NULL;
  int num_sram_bits = 0;
  int i;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert(1 < num_input_per_level);
  
  /* Ensure we have a CMOS MUX*/
  /* Exception: LUT require an auto-generation of netlist can run as well*/ 
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);
  assert(NULL != spice_model.pass_gate_logic);

  /* Print the subckt */
  fprintf(fp, ".subckt %s ", subckt_name);
  for (i = 0; i < num_input_per_level; i++) {
    fprintf(fp, "in%d ", i);
  }
  fprintf(fp, "out ");

  /* General cases */
  num_sram_bits = determine_num_sram_bits_mux_basis_subckt(&spice_model, mux_size, 
                                                           num_input_per_level, special_basis);
  
  for (i = 0; i < num_sram_bits; i++) {
    fprintf(fp, "sel%d sel_inv%d ", i, i); 
  }
  fprintf(fp, "svdd sgnd\n");
  /* Identify the pass-gate logic*/
  switch (spice_model.pass_gate_logic->type) {
  case SPICE_MODEL_PASS_GATE_TRANSMISSION:
    pgl_name = cpt_subckt_name;
    /* We do not need to know the structure of multiplexer, just follow the number of input 
     * Identify a special case: input_size = 2
     */
    if (1 == num_sram_bits) {
      fprintf(fp,"X%s_0 in0 out sel0 sel_inv0 svdd sgnd %s nmos_size=\'%s%s\' pmos_size=\'%s%s\'\n",
              pgl_name, pgl_name, 
              spice_model.name, design_param_postfix_pass_gate_logic_nmos_size,
              spice_model.name, design_param_postfix_pass_gate_logic_pmos_size);
      fprintf(fp,"X%s_1 in1 out sel_inv0 sel0 svdd sgnd %s nmos_size=\'%s%s\' pmos_size=\'%s%s\'\n",
              pgl_name, pgl_name, 
              spice_model.name, design_param_postfix_pass_gate_logic_nmos_size,
              spice_model.name, design_param_postfix_pass_gate_logic_pmos_size);
    } else {
      for (i = 0; i < num_input_per_level; i++) {
        fprintf(fp,"X%s_%d in%d out sel%d sel_inv%d svdd sgnd %s nmos_size=\'%s%s\' pmos_size=\'%s%s\'\n",
                pgl_name, i, i, i, i, pgl_name, 
                spice_model.name, design_param_postfix_pass_gate_logic_nmos_size,
                spice_model.name, design_param_postfix_pass_gate_logic_pmos_size);
      }
    }
    break;
  case SPICE_MODEL_PASS_GATE_TRANSISTOR:
    pgl_name = nmos_subckt_name;
    /* We do not need to know the structure of multiplexer, just follow the number of input 
     * Identify a special case: input_size = 2
     */
    if (1 == num_sram_bits) {
      fprintf(fp,"X%s_0 in0 sel0 out sgnd %s W=\'%s%s*wn\'\n",
                  pgl_name, pgl_name, 
                  spice_model.name, design_param_postfix_pass_gate_logic_nmos_size);
      fprintf(fp,"X%s_1 in1 sel_inv0 out sgnd %s W=\'%s%s*wn\'\n",
                  pgl_name, pgl_name, 
                  spice_model.name, design_param_postfix_pass_gate_logic_nmos_size);
    } else {
      for (i = 0; i < num_input_per_level; i++) {
        fprintf(fp,"X%s_%d in%d sel%d out sgnd %s W=\'%s%s*wn\'\n",
                  pgl_name, i, i, i, 
                  pgl_name, 
                  spice_model.name, design_param_postfix_pass_gate_logic_nmos_size);
      }
    }
    break; 
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File: %s,[LINE%d])Invalid pass gate logic for spice model(name:%s)!\n", 
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }

  fprintf(fp,".eom\n");
  fprintf(fp,"\n");
  
  return;
}

static 
void fprint_spice_mux_model_basis_rram_subckt(FILE* fp, char* subckt_name,
                                              int mux_size,
                                              int num_input_per_level,
                                              t_spice_model spice_model,
                                              boolean special_basis) {
  int i, num_sram_bits;
  char* prog_pmos_subckt_name = NULL;
  char* prog_nmos_subckt_name = NULL;
  char* prog_wp = NULL;
  char* prog_wn = NULL;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* assert(SPICE_MODEL_PASS_GATE_TRANSMISSION == spice_model.pass_gate_logic->type); */
  assert(0. < spice_model.design_tech_info.rram_info->wprog_set_pmos);
  assert(0. < spice_model.design_tech_info.rram_info->wprog_reset_pmos);
  assert(0. < spice_model.design_tech_info.rram_info->wprog_set_nmos);
  assert(0. < spice_model.design_tech_info.rram_info->wprog_reset_nmos);
  
  /* Ensure we have a CMOS MUX*/
  /* Exception: LUT require an auto-generation of netlist can run as well*/ 
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  /* Check */
  assert(1 < num_input_per_level);

  /* Determine the number of memory bit
   * The function considers a special case :
   * 2-input basis in tree-like MUX only requires 1 memory bit */
  num_sram_bits = determine_num_sram_bits_mux_basis_subckt(&spice_model, mux_size, num_input_per_level, special_basis);

  /* Consider advanced RRAM multiplexer design 
   * Advanced design employ normal logic transistors
   * Basic design employ IO transistors
   */
  if (TRUE == spice_model.design_tech_info.mux_info->advanced_rram_design) {
    prog_pmos_subckt_name = pmos_subckt_name;
    prog_nmos_subckt_name = nmos_subckt_name;
    prog_wp = "wp";
    prog_wn = "wn";
  } else {
    prog_pmos_subckt_name = io_pmos_subckt_name;
    prog_nmos_subckt_name = io_nmos_subckt_name;
    prog_wp = "io_wp";
    prog_wn = "io_wn";
  } 

  fprintf(fp, ".subckt %s ", subckt_name);
  for (i = 0; i < num_input_per_level; i++) {
    fprintf(fp, "in%d ", i);
  }
  fprintf(fp, "out ");
  for (i = 0; i < num_sram_bits; i++) {
    fprintf(fp, "sel%d sel_inv%d ", i, i);
  }
  fprintf(fp, "svdd sgnd ");
  fprintf(fp, "ron=\'%s%s\' roff=\'%s%s\' ",
               spice_model.name, design_param_postfix_rram_ron,
               spice_model.name, design_param_postfix_rram_roff);
  fprintf(fp, "wprog_set_nmos=\'%s%s*%s\' wprog_reset_nmos=\'%s%s*%s\' ",
               spice_model.name, design_param_postfix_rram_wprog_set_nmos, 
               prog_wn,
               spice_model.name, design_param_postfix_rram_wprog_reset_nmos,
               prog_wn);
  fprintf(fp, "wprog_set_pmos=\'%s%s*%s\' wprog_reset_pmos=\'%s%s*%s\' \n",
               spice_model.name, design_param_postfix_rram_wprog_set_pmos, 
               prog_wp,
               spice_model.name, design_param_postfix_rram_wprog_reset_pmos,
               prog_wp);
  /* Print the new 2T1R structure */ 
  /* Switch case: 
   * when there is only 1 SRAM bit */
  if (1 == num_sram_bits) { 
    /* RRAMs */
    fprintf(fp, "Xrram_0 in0 out sel0 sel_inv0 rram_behavior switch_thres=vsp ron=ron roff=roff\n");
    /* Programming transistor pairs */
    fprintf(fp, "Xnmos_prog_pair0 in0 sgnd sgnd sgnd %s W=\'wprog_reset_nmos\' \n",
                prog_nmos_subckt_name);
    fprintf(fp, "Xpmos_prog_pair0 in0 svdd svdd svdd %s W=\'wprog_set_pmos\' \n",
                prog_pmos_subckt_name);
    /* RRAMs */
    fprintf(fp, "Xrram_1 in1 out sel_inv0 sel0 rram_behavior switch_thres=vsp ron=ron roff=roff\n");
    /* Programming transistor pairs */
    fprintf(fp, "Xnmos_prog_pair_in1 in1 sgnd sgnd sgnd %s W=\'wprog_reset_nmos\' \n",
                prog_nmos_subckt_name);
    fprintf(fp, "Xpmos_prog_pair_in1 in1 svdd svdd svdd %s W=\'wprog_set_pmos\' \n",
                prog_pmos_subckt_name);
  } else {
    for (i = 0; i < num_input_per_level; i++) {
      /* RRAMs */
      fprintf(fp, "Xrram_%d in%d out sel%d sel_inv%d rram_behavior switch_thres=vsp ron=ron roff=roff\n",
              i, i, i, i);
      /* Programming transistor pairs */
      fprintf(fp, "Xnmos_prog_pair_in%d in%d sgnd sgnd sgnd %s W=\'wprog_reset_nmos\' \n",
              i, i, prog_nmos_subckt_name);
      fprintf(fp, "Xpmos_prog_pair_in%d in%d svdd svdd svdd %s W=\'wprog_set_pmos\' \n",
              i, i, prog_pmos_subckt_name);
    }
  }
  /* Programming transistor pairs shared at the output */
  fprintf(fp, "Xnmos_prog_pair_out out sgnd sgnd sgnd %s W=\'wprog_set_nmos\' \n",
          prog_nmos_subckt_name);
  fprintf(fp, "Xpmos_prog_pair_out out svdd svdd svdd %s W=\'wprog_reset_pmos\' \n",
          prog_pmos_subckt_name);
  fprintf(fp,".eom\n");
  fprintf(fp,"\n");

  return;
}
 
/* Print the SPICE model of a 2:1 MUX which is the basis */
static 
void fprint_spice_mux_model_basis_subckt(FILE* fp, 
                                         t_spice_mux_model* spice_mux_model) {
  char* mux_basis_subckt_name = NULL;
  char* mux_special_basis_subckt_name = NULL;
  int num_input_basis_subckt = 0;
  int num_input_special_basis_subckt = 0;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
 
  /* Try to find a mux in cmos technology, 
   * if we have, then build CMOS 2:1 MUX, and given cmos_mux2to1_subckt_name
   */
  /* Exception: LUT require an auto-generation of netlist can run as well*/ 
  assert((SPICE_MODEL_MUX == spice_mux_model->spice_model->type)
       ||(SPICE_MODEL_LUT == spice_mux_model->spice_model->type));

  /* Generate the spice_mux_arch */
  spice_mux_model->spice_mux_arch = (t_spice_mux_arch*)my_malloc(sizeof(t_spice_mux_arch));
  init_spice_mux_arch(spice_mux_model->spice_model, spice_mux_model->spice_mux_arch, spice_mux_model->size);

  /* Corner case: Error out  MUX_SIZE = 2, automatcially give a one-level structure */
  /*
  if ((2 == spice_mux_model->size)&&(SPICE_MODEL_STRUCTURE_ONELEVEL != spice_mux_model->spice_model->design_tech_info.structure)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Structure of SPICE model (%s) should be one-level because it is linked to a 2:1 MUX!\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name);
    exit(1);
  }
  */

  /* Prepare the basis subckt name */
  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_mux_model->spice_model->name) + 5 
                                                       + strlen(my_itoa(spice_mux_model->size)) 
                                                       + strlen(mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_mux_model->spice_model->name, spice_mux_model->size, mux_basis_posfix);

  /* deteremine the number of inputs of basis subckt */ 
  num_input_basis_subckt = spice_mux_model->spice_mux_arch->num_input_basis;

  /* Determine the input size of the spcial basis */
  num_input_special_basis_subckt = find_spice_mux_arch_special_basis_size(*(spice_mux_model->spice_mux_arch));

  /* Name the special basis subckt */
  mux_special_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_mux_model->spice_model->name) + 5 
                                                               + strlen(my_itoa(spice_mux_model->size)) 
                                                               + strlen(mux_special_basis_posfix) + 1)); 
  sprintf(mux_special_basis_subckt_name, "%s_size%d%s",
          spice_mux_model->spice_model->name, spice_mux_model->size, mux_special_basis_posfix);

  /* Print the basis subckt*/
  switch (spice_mux_model->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Give the subckt name*/
    fprint_spice_mux_model_basis_cmos_subckt(fp, mux_basis_subckt_name, 
                                             num_input_basis_subckt, 
                                             *(spice_mux_model->spice_model),
                                             spice_mux_model->size, 
                                             FALSE);
    /* Dump subckt of special basis if required */
    if (0 < num_input_special_basis_subckt) {
      fprint_spice_mux_model_basis_cmos_subckt(fp, mux_special_basis_subckt_name, 
                                               num_input_special_basis_subckt, 
                                               (*spice_mux_model->spice_model), 
                                               spice_mux_model->size, 
                                               TRUE);
    }
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* RRAM LUT are not yet supported ! */
    if (SPICE_MODEL_LUT == spice_mux_model->spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])RRAM LUT is not supported!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    fprint_spice_mux_model_basis_rram_subckt(fp, mux_basis_subckt_name, 
                                             spice_mux_model->size, 
                                             num_input_basis_subckt, 
                                             *(spice_mux_model->spice_model),
                                             FALSE);
    /* Dump subckt of special basis if required */
    if (0 < num_input_special_basis_subckt) {
      fprint_spice_mux_model_basis_rram_subckt(fp, mux_special_basis_subckt_name, 
                                               spice_mux_model->size, 
                                               num_input_special_basis_subckt, 
                                               (*spice_mux_model->spice_model),
                                               TRUE);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name); 
    exit(1);
  }

  /* Free */
  my_free(mux_basis_subckt_name);
  my_free(mux_special_basis_subckt_name);

  return;
}

void fprint_spice_cmos_mux_tree_structure(FILE* fp, char* mux_basis_subckt_name,
                                          t_spice_model spice_model,
                                          t_spice_mux_arch spice_mux_arch,
                                          int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, level, nextlevel;
  int nextj, out_idx;
  int mux_basis_cnt = 0;

  boolean* inter_buf_loc = NULL;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Intermediate buffer location map */
  inter_buf_loc = (boolean*)my_calloc(spice_mux_arch.num_level + 1, sizeof(boolean));
  for (i = 0; i < spice_mux_arch.num_level + 1; i++) {
    inter_buf_loc[i] = FALSE;
  }
  printf("location_map: %s", spice_model.lut_intermediate_buffer->location_map);
  if (NULL != spice_model.lut_intermediate_buffer->location_map) {
    assert (spice_mux_arch.num_level - 1 == strlen(spice_model.lut_intermediate_buffer->location_map));
    /* For intermediate buffers */ 
    for (i = 0; i < spice_mux_arch.num_level - 1; i++) {
      if ('1' == spice_model.lut_intermediate_buffer->location_map[i]) {
        inter_buf_loc[spice_mux_arch.num_level - i - 1] = TRUE;
      }
    }
  }

  mux_basis_cnt = 0;
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    /* Print basis mux2to1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j++) {
      nextj = j + 1;
      out_idx = j/2; 
      /* Each basis mux2to1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "Xmux_basis_no%d ", mux_basis_cnt); /* given_name */
      /* For intermediate buffers */ 
      if (TRUE == inter_buf_loc[level]) {
        fprintf(fp, "mux2_l%d_in%d_buf mux2_l%d_in%d_buf ", level, j, level, nextj); /* input0 input1 */
      } else {
        fprintf(fp, "mux2_l%d_in%d mux2_l%d_in%d ", level, j, level, nextj); /* input0 input1 */
      }
      fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
      /* fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, nextlevel, sram_port[0]->prefix, nextlevel);*/ /* sram sram_inv */
      fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, i, sram_port[0]->prefix, i); /* sram sram_inv */
      fprintf(fp, "svdd sgnd %s\n", mux_basis_subckt_name); /* subckt_name */
      /* For intermediate buffers */ 
      if (TRUE == inter_buf_loc[nextlevel]) {
        fprintf(fp, "X%s_%d_%d ",
                spice_model.lut_intermediate_buffer->spice_model->name, 
                nextlevel, out_idx); /* Given name*/
        fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
        fprintf(fp, "mux2_l%d_in%d_buf ", nextlevel, out_idx); /* input0 input1 */
        fprintf(fp, "svdd sgnd %s\n", spice_model.lut_intermediate_buffer->spice_model->name); /* subckt_name */
      }
      /* Update the counter */
      j = nextj;
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert(mux_basis_cnt == spice_mux_arch.num_input - 1);

  /* Free */
  my_free(inter_buf_loc);

  return;
}

void fprint_spice_cmos_mux_multilevel_structure(FILE* fp, 
                                                char* mux_basis_subckt_name,
                                                char* mux_special_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, k, level, nextlevel, sram_idx;
  int out_idx;
  int mux_basis_cnt = 0;
  int mux_special_basis_cnt = 0;
  int cur_num_input_basis = 0;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  mux_basis_cnt = 0;
  assert((2 == spice_mux_arch.num_input_basis)||(2 < spice_mux_arch.num_input_basis));
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    sram_idx = nextlevel * spice_mux_arch.num_input_basis;
    /* Check */
    assert(nextlevel > -1);
    /* Print basis muxQto1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j = j+cur_num_input_basis) {
      /* output index */
      out_idx = j/spice_mux_arch.num_input_basis; 
      /* Determine the number of input of this basis */
      cur_num_input_basis = spice_mux_arch.num_input_basis;
      /* See if we need a special basis */
      if ((j + cur_num_input_basis) > spice_mux_arch.num_input_per_level[nextlevel]) {
        cur_num_input_basis = spice_mux_arch.num_input_per_level[nextlevel] - j;
        /* Print the special basis subckt */
        /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
        fprintf(fp, "Xmux_special_basis_no%d ", mux_special_basis_cnt); /* given_name */
        for (k = 0; k < cur_num_input_basis; k++) {
          fprintf(fp, "mux2_l%d_in%d ", level, j + k); /* input0 input1 */
        }
        fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
        /* Print number of sram bits for this basis */
        for (k = sram_idx; k < (sram_idx + cur_num_input_basis); k++) {
          fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, k, sram_port[0]->prefix, k); /* sram sram_inv */
        }
        fprintf(fp, "svdd sgnd %s\n", mux_special_basis_subckt_name); /* subckt_name */
        /* update counter */  
        mux_special_basis_cnt++;
        continue;
      }
      /* Reach here, it means we need a normal basis subckt */
      /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "Xmux_basis_no%d ", mux_basis_cnt); /* given_name */
      for (k = 0; k < cur_num_input_basis; k++) {
        fprintf(fp, "mux2_l%d_in%d ", level, j + k); /* input0 input1 */
      }
      fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
      /* Print number of sram bits for this basis */
      for (k = sram_idx; k < (sram_idx + cur_num_input_basis); k++) {
        fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, k, sram_port[0]->prefix, k); /* sram sram_inv */
      }
      fprintf(fp, "svdd sgnd %s\n", mux_basis_subckt_name); /* subckt_name */
      /* Update the counter */
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert((1 == mux_special_basis_cnt)||(0 == mux_special_basis_cnt));
  /*
  assert((mux_basis_cnt + mux_special_basis_cnt) 
         == (int)((spice_mux_arch.num_input - 1)/(spice_mux_arch.num_input_basis - 1)) + 1);
   */

  return;
}

void fprint_spice_cmos_mux_onelevel_structure(FILE* fp, char* mux_basis_subckt_name,
                                              t_spice_model spice_model,
                                              t_spice_mux_arch spice_mux_arch,
                                              int num_sram_port, t_spice_model_port** sram_port) {
  int k, mux_basis_cnt;
  int level, nextlevel, out_idx;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);

  /* Initialize */
  mux_basis_cnt = 0;
  level = 1;
  nextlevel = 0;
  out_idx = 0;

  /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
  fprintf(fp, "Xmux_basis_no%d ", mux_basis_cnt); /* given_name */
  for (k = 0; k < spice_mux_arch.num_input; k++) {
    fprintf(fp, "mux2_l%d_in%d ", level, k); /* input0 input1 */
  }
  fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
  /* Print number of sram bits for this basis */
  for (k = 0; k < spice_mux_arch.num_input; k++) {
    fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, k, sram_port[0]->prefix, k); /* sram sram_inv */
  }
  fprintf(fp, "svdd sgnd %s\n", mux_basis_subckt_name); /* subckt_name */
  /* Update the counter */
  mux_basis_cnt++;

  /* Check */
  return;
}

/** This is an old tree-like RRAM MUX, which is not manufacturable 
 */
void fprint_spice_rram_mux_tree_structure(FILE* fp, char* mux_basis_subckt_name,
                                          t_spice_model spice_model,
                                          t_spice_mux_arch spice_mux_arch,
                                          int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, level, nextlevel;
  int nextj, out_idx;
  int mux_basis_cnt = 0;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  mux_basis_cnt = 0;
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    /* Print basis mux2to1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j++) {
      nextj = j + 1;
      out_idx = j/2; 
      /* Each basis mux2to1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "Xmux_basis_no%d ", mux_basis_cnt); /* given_name */
      fprintf(fp, "mux2_l%d_in%d mux2_l%d_in%d ", level, j, level, nextj); /* input0 input1 */
      fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
      fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, i, sram_port[0]->prefix, i); /* sram sram_inv */
      fprintf(fp, "svdd sgnd %s\n", mux_basis_subckt_name); /* subckt_name */
      /* Update the counter */
      j = nextj;
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert(mux_basis_cnt == spice_mux_arch.num_input - 1);

  return;
}

/** This is supposed to be a multi-level 4T1R RRAM MUX  
 */
void fprint_spice_rram_mux_multilevel_structure(FILE* fp, char* mux_basis_subckt_name,
                                                char* mux_special_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, k, level, nextlevel, sram_idx;
  int out_idx;
  int mux_basis_cnt = 0;
  int mux_special_basis_cnt = 0;
  int cur_num_input_basis = 0;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  mux_basis_cnt = 0;
  assert((2 == spice_mux_arch.num_input_basis)||(2 < spice_mux_arch.num_input_basis));
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    sram_idx = nextlevel * spice_mux_arch.num_input_basis;
    /* Check */
    assert(nextlevel > -1);
    /* Print basis muxQto1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j = j+cur_num_input_basis) {
      /* output index */
      out_idx = j/spice_mux_arch.num_input_basis; 
      /* Determine the number of input of this basis */
      cur_num_input_basis = spice_mux_arch.num_input_basis;
      /* See if we need a special basis */
      if ((j + cur_num_input_basis) > spice_mux_arch.num_input_per_level[nextlevel]) {
        cur_num_input_basis = spice_mux_arch.num_input_per_level[nextlevel] - j;
        /* Print the special basis subckt */
        /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
        fprintf(fp, "Xmux_special_basis_no%d ", mux_special_basis_cnt); /* given_name */
        for (k = 0; k < cur_num_input_basis; k++) {
          fprintf(fp, "mux2_l%d_in%d ", level, j + k); /* input0 input1 */
        }
        fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
        /* Print number of sram bits for this basis */
        for (k = sram_idx; k < (sram_idx + cur_num_input_basis); k++) {
          fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, k, sram_port[0]->prefix, k); /* sram sram_inv */
        }
        fprintf(fp, "svdd sgnd %s\n", mux_special_basis_subckt_name); /* subckt_name */
        /* update counter */  
        mux_special_basis_cnt++;
        continue;
      }
      /* Reach here, it means we need a normal basis subckt */
      /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "Xmux_basis_no%d ", mux_basis_cnt); /* given_name */
      for (k = 0; k < cur_num_input_basis; k++) {
        fprintf(fp, "mux2_l%d_in%d ", level, j + k); /* input0 input1 */
      }
      fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
      /* Print number of sram bits for this basis */
      for (k = sram_idx; k < (sram_idx + cur_num_input_basis); k++) {
        fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, k, sram_port[0]->prefix, k); /* sram sram_inv */
      }
      fprintf(fp, "svdd sgnd %s\n", mux_basis_subckt_name); /* subckt_name */
      /* Update the counter */
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert((1 == mux_special_basis_cnt)||(0 == mux_special_basis_cnt));
  assert((mux_basis_cnt + mux_special_basis_cnt)
          == (int)((spice_mux_arch.num_input - 1)/(spice_mux_arch.num_input_basis - 1)) + 1);

  return;
}

/** Generate the structure of 4T1R-based RRAM MUX structure
 * 4T1R-based RRAM MUX is optimal in area, delay and power only when it is built with one-level structure
 */
void fprint_spice_rram_mux_onelevel_structure(FILE* fp, 
                                              char* mux_basis_subckt_name,
                                              t_spice_model spice_model,
                                              t_spice_mux_arch spice_mux_arch,
                                              int num_sram_port, t_spice_model_port** sram_port) {
  int k, mux_basis_cnt;
  int level, nextlevel, out_idx, num_sram_bits;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  /* Initialize */
  mux_basis_cnt = 0;
  level = 1;
  nextlevel = 0;
  out_idx = 0;

  /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
  fprintf(fp, "Xmux_basis_no%d ", mux_basis_cnt); /* given_name */
  for (k = 0; k < spice_mux_arch.num_input; k++) {
    fprintf(fp, "mux2_l%d_in%d ", level, k); /* input0 input1 */
  }
  fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */

  /* Print number of sram bits for this basis */
  num_sram_bits = determine_num_sram_bits_mux_basis_subckt(&spice_model, spice_mux_arch.num_input, 
                                                           spice_mux_arch.num_input,
                                                           FALSE);

  for (k = 0; k < num_sram_bits; k++) {
    fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, k, sram_port[0]->prefix, k); /* sram sram_inv */
  }
  fprintf(fp, "svdd sgnd %s\n", mux_basis_subckt_name); /* subckt_name */
  /* Update the counter */
  mux_basis_cnt++;

  return;
}

void fprint_spice_mux_model_cmos_subckt(FILE* fp,
                                        int mux_size,
                                        t_spice_model spice_model,
                                        t_spice_mux_arch spice_mux_arch) {
  int iport, ipin;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;
  int num_mode_bits = 0;
  int num_conf_bits = 0;

  enum e_spice_model_structure cur_mux_structure;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  char* mux_special_basis_subckt_name = NULL;

  /* Basis is always needed */
  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                                       + strlen(my_itoa(mux_size)) 
                                                       + strlen(mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_model.name, mux_size, mux_basis_posfix);
  /* Special basis is on request, but anyway we prepare to call it.*/
  mux_special_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                                               + strlen(my_itoa(mux_size)) 
                                                               + strlen(mux_special_basis_posfix) + 1)); 
  sprintf(mux_special_basis_subckt_name, "%s_size%d%s",
          spice_model.name, mux_size, mux_special_basis_posfix);

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Ensure we have a CMOS MUX,
   * ATTENTION: support LUT as well
   */
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);

  /* Find the input port, output port, and sram port*/
  input_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts*/
  if ((SPICE_MODEL_MUX == spice_model.type)
    || ((SPICE_MODEL_LUT == spice_model.type)
       && (FALSE == spice_model.design_tech_info.lut_info->frac_lut))) {
    assert(1 == num_input_port);
    assert(1 == num_output_port);
    assert(1 == num_sram_port);
    assert(1 == output_port[0]->size); 
  } else {
    assert((SPICE_MODEL_LUT == spice_model.type) 
           && (TRUE == spice_model.design_tech_info.lut_info->frac_lut));
    assert(1 == num_input_port);
    assert(2 == num_sram_port);
    for (iport = 0; iport < num_output_port; iport++) {
      assert(0 < output_port[iport]->size);
    }
  }

  /* Setup a reasonable frac_out level for the output port*/
  for (iport = 0; iport < num_output_port; iport++) {
    /* We always initialize the lut_frac_level when there is only 1 output!
     * It should be pointed the last level! 
     */
    if ((OPEN == output_port[iport]->lut_frac_level) 
       || (1 == num_output_port)) {
      output_port[iport]->lut_frac_level = spice_mux_arch.num_level;
    } 
  }

  /* Add Fracturable LUT outputs */

  /* We have two types of naming rules in terms of the usage of MUXes: 
   * 1. MUXes, the naming rule is <mux_spice_model_name>_<structure>_size<input_size>
   * 2. LUTs, the naming rule is <lut_spice_model_name>_mux_size<sram_port_size>
   */
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      mux_size);
  num_mode_bits = count_num_mode_bits_one_spice_model(&spice_model); 
  /* Knock out the SRAM bits for the mode selection, they are separated dealed */
  num_conf_bits = num_conf_bits - num_mode_bits;

  if (SPICE_MODEL_LUT == spice_model.type) {
    /* Special for LUT MUX*/
    fprintf(fp, "***** CMOS MUX info: spice_model_name= %s_MUX, size=%d *****\n", 
            spice_model.name, mux_size);
    fprintf(fp, ".subckt %s_mux_size%d ", spice_model.name, mux_size);
    /* Global ports */
    if (0 < rec_fprint_spice_model_global_ports(fp, &spice_model, FALSE)) {
      fprintf(fp, "+ ");
    }
    /* Print input ports*/
    assert(mux_size == num_conf_bits);
    for (ipin = 0; ipin < num_conf_bits; ipin++) {
      fprintf(fp, "%s%d ", input_port[0]->prefix, ipin);
    } 
    /* Print output ports*/
    for (iport = 0; iport < num_output_port; iport++) {
      for (ipin = 0; ipin < output_port[iport]->size; ipin++) {
        fprintf(fp, "%s%d ", output_port[iport]->prefix, ipin);
      }
    }
    /* Print sram ports*/
    for (ipin = 0; ipin < input_port[0]->size; ipin++) {
      fprintf(fp, "%s%d ", sram_port[0]->prefix, ipin);
      fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, ipin);
    } 
  } else {
    fprintf(fp, "***** CMOS MUX info: spice_model_name=%s, size=%d, structure: %s *****\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.design_tech_info.mux_info->structure));
    fprintf(fp, ".subckt %s_size%d ", spice_model.name, mux_size);
    /* Global ports */
    if (0 < rec_fprint_spice_model_global_ports(fp, &spice_model, FALSE)) {
      fprintf(fp, "+ ");
    }
    /* Print input ports*/
    for (ipin = 0; ipin < mux_size; ipin++) {
      fprintf(fp, "%s%d ", input_port[0]->prefix, ipin);
    } 
    /* Print output ports*/
    fprintf(fp, "%s ", output_port[0]->prefix);
    /* Print sram ports*/
    for (ipin = 0; ipin < num_conf_bits; ipin++) {
      fprintf(fp, "%s%d ", sram_port[0]->prefix, ipin);
      fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, ipin);
    } 
  }
  /* Print local vdd and gnd*/
  fprintf(fp, "svdd sgnd");
  fprintf(fp, "\n");

  /* Handle the corner case: input size = 2 */
  cur_mux_structure = spice_model.design_tech_info.mux_info->structure;
  if (2 == spice_mux_arch.num_input) {
    cur_mux_structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
  }
  
  /* Print internal architecture*/ 
  switch (cur_mux_structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    fprint_spice_cmos_mux_tree_structure(fp, mux_basis_subckt_name, 
                                         spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    fprint_spice_cmos_mux_onelevel_structure(fp, mux_basis_subckt_name,
                                             spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    fprint_spice_cmos_mux_multilevel_structure(fp, mux_basis_subckt_name, mux_special_basis_subckt_name,
                                               spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }

  /* To connect the input ports*/
  for (ipin = 0; ipin < mux_size; ipin++) {
    if (1 == spice_model.input_buffer->exist) {
      switch (spice_model.input_buffer->type) {
      case SPICE_MODEL_BUF_INV:
        /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "Xinv%d ", ipin); /* Given name*/
        /* Global ports */
        if (0 < rec_fprint_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE)) {
          fprintf(fp, "+ ");
        }
        fprintf(fp, "%s%d ", input_port[0]->prefix, ipin); /* input port */ 
        fprintf(fp, "mux2_l%d_in%d ", spice_mux_arch.input_level[ipin], spice_mux_arch.input_offset[ipin]); /* output port*/
        fprintf(fp, "svdd sgnd inv size=\'%s%s\'", spice_model.name, design_param_postfix_input_buf_size); /* subckt name */
        fprintf(fp, "\n");
        break;
      case SPICE_MODEL_BUF_BUF:
        /* TODO: what about tapered buffer, can we support? */
        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "Xbuf%d ", ipin); /* Given name*/
        /* Global ports */
        if (0 < rec_fprint_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE)) {
          fprintf(fp, "+ ");
        }
        fprintf(fp, "%s%d ", input_port[0]->prefix, ipin); /* input port */ 
        fprintf(fp, "mux2_l%d_in%d ", spice_mux_arch.input_level[ipin], spice_mux_arch.input_offset[ipin]); /* output port*/
        fprintf(fp, "svdd sgnd buf size=\'%s%s\'", spice_model.name, design_param_postfix_input_buf_size); /* subckt name */
        fprintf(fp, "\n");
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                   __FILE__, __LINE__);
        exit(1);   
      }
    } else {
      /* There is no buffer, I create a zero resisitance between*/
      /* Resistance R<given_name> <input> <output> 0*/
      fprintf(fp, "Rin%d %s%d mux2_l%d_in%d 0\n", 
              ipin, input_port[0]->prefix, ipin, spice_mux_arch.input_level[ipin], 
              spice_mux_arch.input_offset[ipin]);
    }
  }
  /* Special: for the last inputs, we connect to VDD|GND 
   * TODO: create an option to select the connection VDD or GND  
   */
  if ((SPICE_MODEL_MUX == spice_model.type)
     && (TRUE == spice_model.design_tech_info.mux_info->add_const_input)) { 
    assert ( (0 == spice_model.design_tech_info.mux_info->const_input_val) 
            || (1 == spice_model.design_tech_info.mux_info->const_input_val) );
    fprintf(fp, "Rin%d mux2_l%d_in%d %s 0\n", 
            spice_mux_arch.num_input - 1,
            spice_mux_arch.input_level[spice_mux_arch.num_input - 1], 
            spice_mux_arch.input_offset[spice_mux_arch.num_input - 1], 
            convert_const_input_value_to_str(spice_model.design_tech_info.mux_info->const_input_val));
  }

  /* Output buffer*/
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_port[iport]->size; ipin++) {
      if (1 == spice_model.output_buffer->exist) {
        /* Tapered buffer support */
        if (TRUE == spice_model.output_buffer->tapered_buf) {
          /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
          fprintf(fp, "Xbuf_out_%d_%d ", 
                  iport, ipin); /* Given name*/
          /* Global ports */
          if (0 < rec_fprint_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE)) {
            fprintf(fp, "+ ");
          }
          fprintf(fp, "mux2_l%d_in%d ", 
                    spice_mux_arch.num_level - output_port[iport]->lut_frac_level, 
                    output_port[iport]->lut_output_mask[ipin]); /* input port */ 
          fprintf(fp, "%s%d ", output_port[iport]->prefix, ipin); /* Output port*/
          fprintf(fp, "svdd sgnd tapbuf_level%d_f%d", 
                  spice_model.output_buffer->tap_buf_level, spice_model.output_buffer->f_per_stage); /* subckt name */
          fprintf(fp, "\n");
          continue;
        }
        switch (spice_model.output_buffer->type) {
        case SPICE_MODEL_BUF_INV:
          if (TRUE == spice_model.output_buffer->tapered_buf) {
            break;
          }
          /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
          fprintf(fp, "Xinv_out_%d_%d ", iport, ipin); /* Given name*/
          /* Global ports */
          if (0 < rec_fprint_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE)) {
            fprintf(fp, "+ ");
          }
          fprintf(fp, "mux2_l%d_in%d ",
                  spice_mux_arch.num_level - output_port[iport]->lut_frac_level, 
                  output_port[iport]->lut_output_mask[ipin]); /* input port */ 
          fprintf(fp, "%s%d ", 
                  output_port[iport]->prefix, ipin); /* Output port*/
          fprintf(fp, "svdd sgnd inv size=\'%s%s\'", spice_model.name, design_param_postfix_output_buf_size); /* subckt name */
          fprintf(fp, "\n");
          break;
        case SPICE_MODEL_BUF_BUF:
          if (TRUE == spice_model.output_buffer->tapered_buf) {
            break;
          }
          /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
          fprintf(fp, "Xbuf_out_%d_%d ", iport, ipin); /* Given name*/
          /* Global ports */
          if (0 < rec_fprint_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE)) {
            fprintf(fp, "+ ");
          }
          fprintf(fp, "mux2_l%d_in%d ", 
                  spice_mux_arch.num_level - output_port[iport]->lut_frac_level, 
                  output_port[iport]->lut_output_mask[ipin]); /* input port */ 
          fprintf(fp, "%s%d ", output_port[iport]->prefix, ipin); /* Output port*/
          fprintf(fp, "svdd sgnd buf size=\'%s%s\'", spice_model.name, design_param_postfix_output_buf_size); /* subckt name */
          fprintf(fp, "\n");
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                     __FILE__, __LINE__);
          exit(1);   
        }
      } else {
        /* There is no buffer, I create a zero resisitance between*/
        /* Resistance R<given_name> <input> <output> 0*/
        fprintf(fp, "Rout mux2_l%d_in%d %s%d 0\n",
                spice_mux_arch.num_level - output_port[iport]->lut_frac_level,
                output_port[iport]->lut_output_mask[ipin],
                output_port[0]->prefix, ipin);
      }
    }
  }
 
  fprintf(fp, ".eom\n");
  fprintf(fp, "***** END CMOS MUX info: spice_model_name=%s, size=%d *****\n", spice_model.name, mux_size);
  fprintf(fp, "\n");

  /* Free */
  my_free(mux_basis_subckt_name);
  my_free(mux_special_basis_subckt_name);
  my_free(input_port);
  my_free(output_port);
  my_free(sram_port);

  return;
}

/* Print the RRAM MUX SPICE model.
 * The internal structures of CMOS and RRAM MUXes are similar. 
 * This one can be merged to CMOS function.
 * However I use another function, because in future the internal structure may change.
 * We will suffer less software problems.
 */
void fprint_spice_mux_model_rram_subckt(FILE* fp,
                                        int mux_size,
                                        t_spice_model spice_model,
                                        t_spice_mux_arch spice_mux_arch) {
  int i;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;
  int num_sram_bits = 0;

  enum e_spice_model_structure cur_mux_structure;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  char* mux_special_basis_subckt_name = NULL;
  /* Basis is always needed */
  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                                       + strlen(my_itoa(mux_size)) 
                                                       + strlen(mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_model.name, mux_size, mux_basis_posfix);
  /* Special basis is on request, but anyway we prepare to call it.*/
  mux_special_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                                               + strlen(my_itoa(mux_size)) 
                                                               + strlen(mux_special_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_model.name, mux_size, mux_basis_posfix);

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Ensure we have a RRAM MUX*/
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  /* Find the input port, output port, and sram port*/
  input_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts*/
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  assert(1 == output_port[0]->size);

  /* We have two types of naming rules in terms of the usage of MUXes: 
   * 1. MUXes, the naming rule is <mux_spice_model_name>_<structure>_size<input_size>
   * 2. LUTs, the naming rule is <lut_spice_model_name>_mux_size<sram_port_size>
   */
  num_sram_bits = count_num_conf_bits_one_spice_model(&spice_model, 
                                                      sram_spice_orgz_info->type,  
                                                      mux_size);

  /* Print the definition of subckt*/
  if (SPICE_MODEL_LUT == spice_model.type) {
    /* RRAM LUT is not supported now... */
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])RRAM LUT is not supported!\n",
               __FILE__, __LINE__);
    exit(1);
    /* Special for LUT MUX*/
    /*
    fprintf(fp, "***** RRAM MUX info: spice_model_name= %s_MUX, size=%d *****\n", spice_model.name, mux_size);
    fprintf(fp, ".subckt %s_mux_size%d ", spice_model.name, mux_size);
    */
  } else {
    fprintf(fp, "***** RRAM MUX info: spice_model_name=%s, size=%d, structure: %s *****\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.design_tech_info.mux_info->structure));
    fprintf(fp, ".subckt %s_size%d ", spice_model.name, mux_size);
  }

  /* Global ports */
  if (0 < rec_fprint_spice_model_global_ports(fp, &spice_model, FALSE)) { 
    fprintf(fp, "+ ");
  }

  /* Print input ports*/
  for (i = 0; i < mux_size; i++) {
    fprintf(fp, "%s%d ", input_port[0]->prefix, i);
  } 
  /* Print output ports*/
  fprintf(fp, "%s ", output_port[0]->prefix);
  /* Print sram ports*/
  for (i = 0; i < num_sram_bits; i++) {
    fprintf(fp, "%s%d ", sram_port[0]->prefix, i);
    fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, i);
  } 
  /* Print local vdd and gnd*/
  fprintf(fp, "svdd sgnd ");
  fprintf(fp, "ron=\'%s%s\' roff=\'%s%s\' ",
          spice_model.name, design_param_postfix_rram_ron, 
          spice_model.name, design_param_postfix_rram_roff);
  fprintf(fp, "\n");
  
  /* Print internal architecture*/ 
  /* Handle the corner case: input size = 2  */
  if (2 == mux_size) {
    cur_mux_structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
  } else {
    cur_mux_structure = spice_model.design_tech_info.mux_info->structure;
  }
  /* RRAM MUX is optimal in terms of area, delay and power for one-level structure.
   * Hence, we do not support the multi-level or tree-like RRAM MUX.
   */
  switch (cur_mux_structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    fprint_spice_rram_mux_tree_structure(fp, mux_basis_subckt_name, 
                                         spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    fprint_spice_rram_mux_multilevel_structure(fp, mux_basis_subckt_name, mux_special_basis_subckt_name,
                                               spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    fprint_spice_rram_mux_onelevel_structure(fp, mux_basis_subckt_name, 
                                             spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }

  /* To connect the input ports*/
  for (i = 0; i < mux_size; i++) {
    if (1 == spice_model.input_buffer->exist) {
      switch (spice_model.input_buffer->type) {
      case SPICE_MODEL_BUF_INV:
        /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "Xinv%d ", i); /* Given name*/
        fprintf(fp, "%s%d ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in%d ", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "svdd sgnd inv size=\'%s%s\'", spice_model.name, design_param_postfix_input_buf_size); /* subckt name */
        fprintf(fp, "\n");
        break;
      case SPICE_MODEL_BUF_BUF:
        /* TODO: what about tapered buffer, can we support? */
        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "Xbuf%d ", i); /* Given name*/
        fprintf(fp, "%s%d ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in%d ", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "svdd sgnd buf size=\'%s%s\'", spice_model.name, design_param_postfix_input_buf_size); /* subckt name */
        fprintf(fp, "\n");
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                   __FILE__, __LINE__);
        exit(1);   
      }
    } else {
      /* There is no buffer, I create a zero resisitance between*/
      /* Resistance R<given_name> <input> <output> 0*/
      fprintf(fp, "Rin%d %s%d mux2_l%d_in%d 0\n", 
              i, input_port[0]->prefix, i, spice_mux_arch.input_level[i], 
              spice_mux_arch.input_offset[i]);
    }
  }

  /* Output buffer*/
  if (1 == spice_model.output_buffer->exist) {
    switch (spice_model.output_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      if (TRUE == spice_model.output_buffer->tapered_buf) {
        break;
      }
      /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "Xinv_out "); /* Given name*/
      fprintf(fp, "mux2_l%d_in%d ", 0, 0); /* input port */ 
      fprintf(fp, "%s ", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "svdd sgnd inv size=\'%s%s\'", spice_model.name, design_param_postfix_output_buf_size); /* subckt name */
      fprintf(fp, "\n");
      break;
    case SPICE_MODEL_BUF_BUF:
      if (TRUE == spice_model.output_buffer->tapered_buf) {
        break;
      }
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "Xbuf_out "); /* Given name*/
      fprintf(fp, "mux2_l%d_in%d ", 0, 0); /* input port */ 
      fprintf(fp, "%s ", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "svdd sgnd buf size=\'%s%s\'", spice_model.name, design_param_postfix_output_buf_size); /* subckt name */
      fprintf(fp, "\n");
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                 __FILE__, __LINE__);
      exit(1);   
    }
    /* Tapered buffer support */
    if (TRUE == spice_model.output_buffer->tapered_buf) {
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "Xbuf_out "); /* Given name*/
      fprintf(fp, "mux2_l%d_in%d ", 0 , 0); /* input port */ 
      fprintf(fp, "%s ", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "svdd sgnd tapbuf_level%d_f%d", 
              spice_model.output_buffer->tap_buf_level, spice_model.output_buffer->f_per_stage); /* subckt name */
      fprintf(fp, "\n");
    }
  } else {
    /* There is no buffer, I create a zero resisitance between*/
    /* Resistance R<given_name> <input> <output> 0*/
    fprintf(fp, "Rout mux2_l0_in0 %s 0\n",output_port[0]->prefix);
  }
 
  fprintf(fp, ".eom\n");
  fprintf(fp, "***** END RRAM MUX info: spice_model_name=%s, size=%d *****\n", spice_model.name, mux_size);
  fprintf(fp, "\n");

  /* Free */
  my_free(mux_basis_subckt_name);
  my_free(input_port);
  my_free(output_port);
  my_free(sram_port);

  return;
}

/* Print the SPICE model of a multiplexer subckt with given info */
static 
void fprint_spice_mux_model_subckt(FILE* fp,
                                   t_spice_mux_model* spice_mux_model) {
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  /* Make sure we have a valid spice_model*/
  if (NULL == spice_mux_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid spice_mux_model!\n",__FILE__, __LINE__); 
    exit(1);
  }
  /* Make sure we have a valid spice_model*/
  if (NULL == spice_mux_model->spice_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid spice_model!\n",__FILE__, __LINE__); 
    exit(1);
  }

  /* Check the mux size*/
  if (spice_mux_model->size < 2) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid MUX size(=%d)! Should be at least 2.\n",
               __FILE__, __LINE__, spice_mux_model->size); 
    exit(1);
  }

  /* Corner case: Error out  MUX_SIZE = 2, automatcially give a one-level structure */
  /*
  if ((2 == spice_mux_model->size)&&(SPICE_MODEL_STRUCTURE_ONELEVEL != spice_mux_model->spice_model->design_tech_info.structure)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Structure of SPICE model (%s) should be one-level because it is linked to a 2:1 MUX!\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name);
    exit(1);
  }
  */

  /* Print the definition of subckt*/

  /* Check the design technology*/
  switch (spice_mux_model->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    fprint_spice_mux_model_cmos_subckt(fp, spice_mux_model->size,
                                        *(spice_mux_model->spice_model), 
                                        *(spice_mux_model->spice_mux_arch));
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    fprint_spice_mux_model_rram_subckt(fp, spice_mux_model->size,
                                        *(spice_mux_model->spice_model), 
                                        *(spice_mux_model->spice_mux_arch));
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name); 
    exit(1);
  }
  return;
}


/* We should count how many multiplexers with different sizes are needed */
void generate_spice_muxes(char* subckt_dir,
                          int num_switch,
                          t_switch_inf* switches,
                          t_spice* spice,
                          t_det_routing_arch* routing_arch) {
  /* We have linked list whichs stores spice model information of multiplexer*/
  t_llist* muxes_head = NULL; 
  t_llist* temp = NULL;
  int mux_cnt = 0;
  int max_mux_size = -1;
  int min_mux_size = -1;
  FILE* fp = NULL;
  char* sp_name = my_strcat(subckt_dir,muxes_spice_file_name);
  int num_input_ports = 0;
  t_spice_model_port** input_ports = NULL;
  int num_sram_ports = 0;
  t_spice_model_port** sram_ports = NULL;

  int num_input_basis = 0;
  t_spice_mux_model* cur_spice_mux_model = NULL;

  /* Alloc the muxes*/
  muxes_head = stats_spice_muxes(num_switch, switches, spice, routing_arch);

  /* Print the muxes netlist*/
  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create subckt SPICE netlist %s",__FILE__, __LINE__, sp_name); 
    exit(1);
  } 
  /* Generate the descriptions*/
  fprint_spice_head(fp,"MUXes used in FPGA");

  /* Print mux netlist one by one*/
  temp = muxes_head;
  while(temp) {
    assert(NULL != temp->dptr);
    cur_spice_mux_model = (t_spice_mux_model*)(temp->dptr);
    /* Bypass the spice models who has a user-defined subckt */
    if (NULL != cur_spice_mux_model->spice_model->model_netlist) {
      input_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_INPUT, &num_input_ports, TRUE);
      sram_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_ports, TRUE);
      assert(0 != num_input_ports);
      assert(0 != num_sram_ports);
      /* Check the Input port size */
      if (cur_spice_mux_model->size != input_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) size(%d) unmatch with the architecture needs(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, input_ports[0]->size,cur_spice_mux_model->size);
        exit(1);
      }
      /* Check the SRAM port size */
      num_input_basis = determine_num_input_basis_multilevel_mux(cur_spice_mux_model->size, 
                                                                 cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level);
      if ((num_input_basis * cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level) != sram_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) SRAM size(%d) unmatch with the num of level(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, sram_ports[0]->size, 
                   cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level * num_input_basis);
        exit(1);
      }
      /* Move on to the next*/
      temp = temp->next;
      continue;
    }
    /* Let's have a N:1 MUX as basis*/
    fprint_spice_mux_model_basis_subckt(fp, cur_spice_mux_model);
    /* Print the mux subckt */
    fprint_spice_mux_model_subckt(fp, cur_spice_mux_model);
    /* Update the statistics*/
    mux_cnt++;
    if ((-1 == max_mux_size)||(max_mux_size < cur_spice_mux_model->size)) {
      max_mux_size = cur_spice_mux_model->size;
    }
    if ((-1 == min_mux_size)||(min_mux_size > cur_spice_mux_model->size)) {
      min_mux_size = cur_spice_mux_model->size;
    }
    /* Move on to the next*/
    temp = temp->next;
  }

  vpr_printf(TIO_MESSAGE_INFO,"Generated %d Multiplexer subckts.\n",
             mux_cnt);
  vpr_printf(TIO_MESSAGE_INFO,"Max. MUX size = %d.\t",
             max_mux_size);
  vpr_printf(TIO_MESSAGE_INFO,"Min. MUX size = %d.\n",
             min_mux_size);
  

  /* remember to free the linked list*/
  free_muxes_llist(muxes_head);
  /* Free strings */
  free(sp_name);

  /* Close the file*/
  fclose(fp);

  return;
}

