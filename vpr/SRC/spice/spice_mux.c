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

/* Include spice support headers*/
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_lut.h"
#include "spice_pbtypes.h"
#include "spice_mux.h"

/***** Subroutines *****/
void init_spice_mux_arch(t_spice_mux_arch* spice_mux_arch,
                         int mux_size);

void fprint_spice_mux_model_basis_cmos_subckt(FILE* fp,
                                        char* subckt_name,
                                        t_spice_model spice_model);

void fprint_spice_mux_model_basis_rram_subckt(FILE* fp,
                                        char* subckt_name,
                                        t_spice_model spice_model);

void fprint_spice_mux_model_basis_subckt(FILE* fp, 
                                         t_spice_mux_model* spice_mux_model);

void fprint_spice_mux_model_cmos_subckt(FILE* fp,
                                        int mux_size,
                                        t_spice_model spice_model,
                                        t_spice_mux_arch spice_mux_arch);

void fprint_spice_mux_model_rram_subckt(FILE* fp,
                                        int mux_size,
                                        t_spice_model spice_model,
                                        t_spice_mux_arch spice_mux_arch);

void fprint_spice_mux_model_subckt(FILE* fp,
                                   t_spice_mux_model* spice_mux_model);

t_llist* stats_spice_muxes(int num_switch,
                           t_switch_inf* switches,
                           t_spice* spice,
                           t_det_routing_arch* routing_arch);


/***** Subroutines *****/
void init_spice_mux_arch(t_spice_model* spice_model,
                         t_spice_mux_arch* spice_mux_arch,
                         int mux_size) {
  int cur;
  int i;
  /* Make sure we have a valid pointer*/
  if (NULL == spice_mux_arch) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d])Invalid spice_mux_arch!\n",
               __FILE__, __LINE__);
    exit(1);
  } 

  /* Basic info*/
  spice_mux_arch->num_input = mux_size;
  /* For different structure */
  switch (spice_model->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    spice_mux_arch->num_level = determine_tree_mux_level(spice_mux_arch->num_input);
    spice_mux_arch->num_input_basis = 2;
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = tree_mux_last_level_input_num(spice_mux_arch->num_level, mux_size);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    spice_mux_arch->num_level = 1;
    spice_mux_arch->num_input_basis = mux_size;
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = mux_size;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    spice_mux_arch->num_level = spice_model->mux_num_level;
    spice_mux_arch->num_input_basis = determine_num_input_basis_multilevel_mux(mux_size, spice_mux_arch->num_level);
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = multilevel_mux_last_level_input_num(spice_mux_arch->num_level, spice_mux_arch->num_input_basis, mux_size);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model->name);
    exit(1);
  }

  /* Alloc*/
  spice_mux_arch->num_input_per_level = (int*) my_malloc(sizeof(int)*spice_mux_arch->num_level);
  spice_mux_arch->input_level = (int*)my_malloc(sizeof(int)*spice_mux_arch->num_input);
  spice_mux_arch->input_offset = (int*)my_malloc(sizeof(int)*spice_mux_arch->num_input);

  /* Assign inputs info for the last level first */
  for (i = 0; i < spice_mux_arch->num_input_last_level; i++) {
    spice_mux_arch->input_level[i] = spice_mux_arch->num_level;
    spice_mux_arch->input_offset[i] = i; 
  }
  /* For the last second level*/
  if (spice_mux_arch->num_input > spice_mux_arch->num_input_last_level) {
    cur = spice_mux_arch->num_input_last_level/spice_mux_arch->num_input_basis;
    /* Start from the input ports that are not occupied by the last level
     * last level has (cur) outputs
     */
    for (i = spice_mux_arch->num_input_last_level; i < spice_mux_arch->num_input; i++) {
      spice_mux_arch->input_level[i] = spice_mux_arch->num_level - 1;
      spice_mux_arch->input_offset[i] = cur; 
      cur++;
    }
    assert((cur < (int)pow((double)spice_mux_arch->num_input_basis, (double)(spice_mux_arch->num_level-1)))
           ||(cur == (int)pow((double)spice_mux_arch->num_input_basis, (double)(spice_mux_arch->num_level-1))));
  }
  /* Fill the num_input_per_level*/
  for (i = 0; i < spice_mux_arch->num_level; i++) {
    cur = i+1;
    spice_mux_arch->num_input_per_level[i] = (int)pow((double)spice_mux_arch->num_input_basis, (double)cur);
    if ((cur == spice_mux_arch->num_level)
       &&(spice_mux_arch->num_input_last_level < spice_mux_arch->num_input_per_level[i])) {
      spice_mux_arch->num_input_per_level[i] = spice_mux_arch->num_input_last_level;
    }
  }
  
  return; 
}

/* Search the linked list, if we have the same mux size and spice_model
 * return 1, if not we return 0
 */
t_llist* search_mux_linked_list(t_llist* mux_head,
                                int mux_size,
                                t_spice_model* spice_model) {
  t_llist* temp = mux_head;
  t_spice_mux_model* cur_mux = NULL;
  /* traversal the linked list*/ 
  while(temp) {
    cur_mux = (t_spice_mux_model*)(temp->dptr);
    if ((cur_mux->size == mux_size)
      &&(spice_model == cur_mux->spice_model)) {
      return temp;
    }
    /* next */
    temp = temp->next;
  }

  return NULL;
}

/* Check the linked list if we have a mux stored with same spice model
 * if not, we create a new one.
 */
void check_and_add_mux_to_linked_list(t_llist** muxes_head,
                                      int mux_size,
                                      t_spice_model* spice_model) {
  t_spice_mux_model* cur_mux = NULL;
  t_llist* temp = NULL;

  /* Check code: to avoid mistake, we should check the mux size
   * the mux_size should be at least 2 so that we need a mux
   */
  if (mux_size < 2) {
    printf("Warning:(File:%s,LINE[%d]) ilegal mux size (%d), expect to be at least 2!\n",
           __FILE__, __LINE__, mux_size);
    return;
  }

  /* Search the linked list */
  if (NULL != search_mux_linked_list((*muxes_head),mux_size,spice_model)) {
    /* We find one, there is no need to create a new one*/
    return;
  }
  /*Create a linked list, if head is NULL*/
  if (NULL == (*muxes_head)) {
    (*muxes_head) = create_llist(1); 
    (*muxes_head)->dptr = my_malloc(sizeof(t_spice_mux_model));
    cur_mux = (t_spice_mux_model*)((*muxes_head)->dptr);
  } else { 
    /* We have to create a new elment in linked list*/
    temp = insert_llist_node((*muxes_head)); 
    temp->dptr = my_malloc(sizeof(t_spice_mux_model));
    cur_mux = (t_spice_mux_model*)(temp->dptr);
  }
  /* Fill the new SPICE MUX Model*/
  cur_mux->size = mux_size;
  cur_mux->spice_model = spice_model;
  cur_mux->cnt = 1; /* Initialize the counter*/

  return;
}

/* Free muxes linked list
 */
void free_muxes_llist(t_llist* muxes_head) {
  t_llist* temp = muxes_head;
  while(temp) {
    /* Free the mux_spice_model, remember to set the pointer to NULL */
    free(temp->dptr);
    temp->dptr = NULL;
    /* Move on to the next pointer*/
    temp = temp->next;
  }
  free_llist(muxes_head);
  return;
}

void fprint_spice_mux_model_basis_cmos_subckt(FILE* fp, char* subckt_name,
                                              int num_input_per_level,
                                              t_spice_model spice_model) {
  char* pgl_name = NULL;
  int num_sram_bits = 0;
  int i;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  
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
  switch (spice_model.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    num_sram_bits = 1;
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_sram_bits = num_input_per_level;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }
  
  for (i = 0; i < num_sram_bits; i++) {
    fprintf(fp, "sel%d sel_inv%d ", i, i); 
  }
  fprintf(fp, "svdd sgnd\n");
  /* Identify the pass-gate logic*/
  switch (spice_model.pass_gate_logic->type) {
  case SPICE_MODEL_PASS_GATE_TRANSMISSION:
    pgl_name = cpt_subckt_name;
    switch (spice_model.structure) {
    case SPICE_MODEL_STRUCTURE_TREE:
      fprintf(fp,"X%s_0 in0 out sel0 sel_inv0 svdd sgnd %s nmos_size=%g pmos_size=%g\n",
              pgl_name, pgl_name, 
              spice_model.pass_gate_logic->nmos_size, spice_model.pass_gate_logic->pmos_size);
      fprintf(fp,"X%s_1 in1 out sel_inv0 sel0 svdd sgnd %s nmos_size=%g pmos_size=%g\n",
              pgl_name, pgl_name, 
              spice_model.pass_gate_logic->nmos_size, spice_model.pass_gate_logic->pmos_size);
      break;
    case SPICE_MODEL_STRUCTURE_ONELEVEL:
    case SPICE_MODEL_STRUCTURE_MULTILEVEL:
      for (i = 0; i < num_input_per_level; i++) {
        fprintf(fp,"X%s_%d in%d out sel%d sel_inv%d svdd sgnd %s nmos_size=%g pmos_size=%g\n",
                pgl_name, i, i, i, i, pgl_name, 
                spice_model.pass_gate_logic->nmos_size, spice_model.pass_gate_logic->pmos_size);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
                 __FILE__, __LINE__, spice_model.name);
      exit(1);
    }
    break;
  case SPICE_MODEL_PASS_GATE_TRANSISTOR:
    pgl_name = nmos_subckt_name;
    switch (spice_model.structure) {
    case SPICE_MODEL_STRUCTURE_TREE:
      fprintf(fp,"X%s_0 in0 sel0 out sgnd %s W=\'%g*wn\'\n",
                  pgl_name, pgl_name, spice_model.pass_gate_logic->nmos_size);
      fprintf(fp,"X%s_1 in1 sel_inv0 out sgnd %s W=\'%g*wn\'\n",
                  pgl_name, pgl_name, spice_model.pass_gate_logic->nmos_size);
      break;
    case SPICE_MODEL_STRUCTURE_ONELEVEL:
    case SPICE_MODEL_STRUCTURE_MULTILEVEL:
      for (i = 0; i < num_input_per_level; i++) {
        fprintf(fp,"X%s_%d in%d sel%d out sgnd %s W=\'%g*wn\'\n",
                  pgl_name, i, i, i, 
                  pgl_name, spice_model.pass_gate_logic->nmos_size);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
                 __FILE__, __LINE__, spice_model.name);
      exit(1);
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

void fprint_spice_mux_model_basis_rram_subckt(FILE* fp, char* subckt_name,
                                              int num_input_per_level,
                                              t_spice_model spice_model) {
  int i;
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  
  /* Ensure we have a CMOS MUX*/
  /* Exception: LUT require an auto-generation of netlist can run as well*/ 
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  switch(spice_model.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    /* TODO: This structure is going to be abolished! 
     * I will update this part of code to new 2T1R structure
     */
    /* Print the subckt */
    fprintf(fp, ".subckt %s ", subckt_name);
    fprintf(fp, "in0 in1 ");
    fprintf(fp, "out ");
    fprintf(fp, "sel sel_inv ");
    fprintf(fp, "svdd sgnd ron=\'%g\' roff=\'%g\' wprog=\'%g*wn\'\n",
            spice_model.ron, spice_model.roff, spice_model.prog_trans_size);
    /* Print resistors */
    fprintf(fp,"Xrram_0 in0 out sel sel_inv rram_behavior switch_thres=vsp ron=ron roff=roff\n");
    fprintf(fp,"Xrram_1 in1 out sel_inv sel rram_behavior switch_thres=vsp ron=ron roff=roff\n");
    /* Print programming transistor*/
    fprintf(fp,"Xprog_0 in0 sgnd in1 sgnd %s L=\'nl\' W=\'wprog\'\n", 
            nmos_subckt_name);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    fprintf(fp, ".subckt %s ", subckt_name);
    for (i = 0; i < num_input_per_level; i++) {
      fprintf(fp, "in%d ", i);
    }
    fprintf(fp, "out ");
    for (i = 0; i < num_input_per_level; i++) {
      fprintf(fp, "sel%d sel_inv%d ", i, i);
    }
    fprintf(fp, "svdd sgnd ron=\'%g\' roff=\'%g\' \n",
            spice_model.ron, spice_model.roff);
    /* Print the new 2T1R structure */ 
    for (i = 0; i < num_input_per_level; i++) {
      /* RRAMs */
      fprintf(fp, "Xrram_%d in%d out sel%d sel_inv%d rram_behavior switch_thres=vsp ron=ron roff=roff\n",
              i, i, i, i);
      /* Programming transistor pairs */
      fprintf(fp, "Xnmos_prog_pair%d in%d sgnd sgnd sgnd %s W=\'%g*wn\' \n",
              i, i, nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);
      fprintf(fp, "Xpmos_prog_pair%d in%d svdd sgnd svdd %s W=\'%g*wp\' \n",
              i, i, pmos_subckt_name, spice_model.pass_gate_logic->pmos_size);
    }
    /* Programming transistor pairs shared at the output */
    /*
    fprintf(fp, "Xnmos_prog_pair_out out sgnd sgnd sgnd %s W=\'%g*wn\' \n",
            nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);
    fprintf(fp, "Xpmos_prog_pair_out out svdd sgnd svdd %s W=\'%g*wp\' \n",
            pmos_subckt_name, spice_model.pass_gate_logic->pmos_size);
    */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }

  fprintf(fp,".eom\n");
  fprintf(fp,"\n");

  return;
}
 
/* Print the SPICE model of a 2:1 MUX which is the basis */
void fprint_spice_mux_model_basis_subckt(FILE* fp, 
                                         t_spice_mux_model* spice_mux_model) {
  char* mux_basis_subckt_name = NULL;
  int num_input_basis_subckt = 0;

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

  /* Prepare the basis subckt name */
  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_mux_model->spice_model->name) + 5 
                                           + strlen(my_itoa(spice_mux_model->size)) + strlen(mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_mux_model->spice_model->name, spice_mux_model->size, mux_basis_posfix);
  /* deteremine the number of inputs of basis subckt */ 
  num_input_basis_subckt = spice_mux_model->spice_mux_arch->num_input_basis;
  /* Print the basis subckt*/
  switch (spice_mux_model->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Give the subckt name*/
    fprint_spice_mux_model_basis_cmos_subckt(fp, mux_basis_subckt_name, 
                                             num_input_basis_subckt, *(spice_mux_model->spice_model));
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* RRAM LUT are not yet supported ! */
    if (SPICE_MODEL_LUT == spice_mux_model->spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])RRAM LUT is not supported!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    fprint_spice_mux_model_basis_rram_subckt(fp, mux_basis_subckt_name, 
                                             num_input_basis_subckt, *(spice_mux_model->spice_model));
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name); 
    exit(1);
  }

  /* Free */
  my_free(mux_basis_subckt_name);

  return;
}

void fprint_spice_cmos_mux_tree_structure(FILE* fp, char* mux_basis_subckt_name,
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
      fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, nextlevel, sram_port[0]->prefix, nextlevel); /* sram sram_inv */
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

void fprint_spice_cmos_mux_multilevel_structure(FILE* fp, char* mux_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, k, level, nextlevel, sram_idx;
  int out_idx;
  int mux_basis_cnt = 0;
  int special_basis_cnt = 0;
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
      if ((j + cur_num_input_basis) > spice_mux_arch.num_input_per_level[nextlevel]) {
        cur_num_input_basis = spice_mux_arch.num_input_per_level[nextlevel] - j;
        /* Print the special basis */
        switch (spice_model.pass_gate_logic->type) {
        case SPICE_MODEL_PASS_GATE_TRANSMISSION:
          for (k = 0; k < cur_num_input_basis; k++) {
            fprintf(fp, "Xspecial_basis_no%d ", k);
            fprintf(fp, "mux2_l%d_in%d  ", level, j + k);
            fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
            fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, sram_idx+k, sram_port[0]->prefix, sram_idx+k); /* sram sram_inv */
            fprintf(fp, "svdd sgnd %s nmos_size=%g pmos_size=%g\n",
                    cpt_subckt_name, spice_model.pass_gate_logic->nmos_size, spice_model.pass_gate_logic->pmos_size);
          }
          break;
        case SPICE_MODEL_PASS_GATE_TRANSISTOR:
          for (k = 0; k < cur_num_input_basis; k++) {
            fprintf(fp, "Xspecial_basis_no%d ", k);
            fprintf(fp, "mux2_l%d_in%d  ", level, j + k);
            fprintf(fp, "mux2_l%d_in%d ", nextlevel, out_idx); /* output */
            fprintf(fp, "%s%d ", sram_port[0]->prefix, sram_idx+k); /* sram sram_inv */
            fprintf(fp, "sgnd %s W=\'%g*wn\' \n",
                    nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);
          }
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid pass_gate_logic type for spice model (%s)!\n",
                     __FILE__, __LINE__, spice_model.name);
          exit(1);
        }
        special_basis_cnt++;
        continue;
      }
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
  assert(1 == special_basis_cnt);
  assert((mux_basis_cnt + special_basis_cnt) == (int)((spice_mux_arch.num_input - 1)/(spice_mux_arch.num_input_basis - 1)) + 1);

  return;
}

void fprint_spice_cmos_mux_onelevel_structure(FILE* fp, t_spice_model spice_model,
                                              t_spice_mux_arch spice_mux_arch,
                                              int num_sram_port, t_spice_model_port** sram_port) {
  int i;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);

  switch (spice_model.pass_gate_logic->type) {
  case SPICE_MODEL_PASS_GATE_TRANSMISSION:
    for (i = 0; i < spice_mux_arch.num_input; i++) {
      fprintf(fp, "Xmux_basis_no%d ", i); /* given_name */
      fprintf(fp, "mux2_l%d_in%d ", 1, i); /* input0  */
      fprintf(fp, "mux2_l%d_in%d ", 0, 0); /* output */
      fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, i, sram_port[0]->prefix, i); /* sram sram_inv */
      fprintf(fp, "svdd sgnd %s nmos_size=%g pmos_size=%g\n", 
              cpt_subckt_name, spice_model.pass_gate_logic->nmos_size, spice_model.pass_gate_logic->pmos_size);
    }
    break;
  case SPICE_MODEL_PASS_GATE_TRANSISTOR:
    for (i = 0; i < spice_mux_arch.num_input; i++) {
      fprintf(fp, "Xmux_basis_no%d ", i); /* given_name */
      fprintf(fp, "mux2_l%d_in%d ", 1, i); /* input0 */
      fprintf(fp, "%s%d ", sram_port[0]->prefix, i); /* sram sram_inv */
      fprintf(fp, "mux2_l%d_in%d ", 0, 0); /* output */
      fprintf(fp, "sgnd %s W=\'%g*wn\'\n", 
              nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(Fil: %s,[LINE%d])Invalid pass gate logic for spice model(name:%s)!\n", 
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }

  return;
}

void fprint_spice_rram_mux_tree_structure(FILE* fp, char* mux_basis_subckt_name,
                                          t_spice_model spice_model,
                                          t_spice_mux_arch spice_mux_arch,
                                          int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, level, nextlevel;
  int nextj, nextnextj, out_idx;
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
      fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, nextlevel, sram_port[0]->prefix, nextlevel); /* sram sram_inv */
      fprintf(fp, "svdd sgnd %s ron=\'ron\' roff=\'roff\' wprog=\'wprog\'\n", mux_basis_subckt_name); /* subckt_name */
      /* Add extra programming transistors to have the same delay for each input. 
       * Actually this prog. trans. does not exist...
       */
      if (0 == j) {
        fprintf(fp, "Xprog_extral%d mux2_l%d_in%d sgnd sgnd sgnd %s L=nl W=\'wprog\'\n",
                level, level, j, nmos_subckt_name);   
      }
      /* Add Programming transistors*/
      nextnextj = nextj + 1;
      /* Add programming transistors between basis mux2to1*/
      if (nextnextj < spice_mux_arch.num_input_per_level[nextlevel]) {
        fprintf(fp, "Xprog_no%d mux2_l%d_in%d sgnd mux2_l%d_in%d sgnd %s L=nl W=\'wprog\'\n",
                mux_basis_cnt, level, nextj, level, nextnextj, nmos_subckt_name);   
      } else { /* Add programming transistors that connects to scan-chan */
        fprintf(fp, "Xprog_no%d mux2_l%d_in%d sgnd sgnd sgnd %s L=nl W=\'wprog\'\n",
                mux_basis_cnt, level, nextj, nmos_subckt_name);   
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
  /* Add programming transistor for the output port*/
  fprintf(fp, "Xprog_out mux2_l%d_in%d sgnd sgnd sgnd %s L=nl W=\'wprog\'\n",
          nextlevel, out_idx, nmos_subckt_name);

  return;
}

void fprint_spice_rram_mux_multilevel_structure(FILE* fp, char* mux_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, k, level, nextlevel, sram_idx;
  int out_idx;
  int mux_basis_cnt = 0;
  int special_basis_cnt = 0;
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
      if ((j + cur_num_input_basis) > spice_mux_arch.num_input_per_level[nextlevel]) {
        cur_num_input_basis = spice_mux_arch.num_input_per_level[nextlevel] - j;
        /* Print the special basis */
        for (k = 0; k < cur_num_input_basis; k++) {
          /* Print a RRAM */
          fprintf(fp,"Xspeical_basis_rram%d mux2_l%d_in%d mux2_l%d_in%d ", 
                  k, level, j+k, nextlevel, out_idx);
          fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, sram_idx+k, sram_port[0]->prefix, sram_idx+k); /* sram sram_inv */ 
          fprintf(fp, "rram_behavior switch_thres=vsp ron=ron roff=roff\n");
          /* Print a pair of programming transistor: 1 PMOS and 1 NMOS */
          /* PMOS */
          fprintf(fp, "Xspecial_basis_pmos_prog_pair%d ", k); /* given_name */
          fprintf(fp, "mux2_l%d_in%d ", level, j+k); /* input0  */
          fprintf(fp, "svdd sgnd svdd %s W=\'%g*wp\' \n", 
                  pmos_subckt_name, spice_model.pass_gate_logic->pmos_size);
          /* NMOS */
          fprintf(fp, "Xspecial_basis_nmos_prog_pair%d ", k); /* given_name */
          fprintf(fp, "mux2_l%d_in%d ", level, j+k); /* input0  */
          fprintf(fp, "sgnd sgnd sgnd %s W=\'%g*wn\' \n", 
                  nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);
        }
        special_basis_cnt++;
        continue;
      }
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
  /* programming pair at the output port */
  fprintf(fp, "Xnmos_prog_pair_out mux2_l%d_in%d sgnd sgnd sgnd %s W=\'%g*wn\' \n",
          nextlevel, out_idx, nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);
  fprintf(fp, "Xpmos_prog_pair_out mux2_l%d_in%d svdd sgnd svdd %s W=\'%g*wp\' \n",
          nextlevel, out_idx, pmos_subckt_name, spice_model.pass_gate_logic->pmos_size);
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert(1 == special_basis_cnt);
  assert((mux_basis_cnt + special_basis_cnt) == (int)((spice_mux_arch.num_input - 1)/(spice_mux_arch.num_input_basis - 1)));

  return;
}

void fprint_spice_rram_mux_onelevel_structure(FILE* fp, t_spice_model spice_model,
                                              t_spice_mux_arch spice_mux_arch,
                                              int num_sram_port, t_spice_model_port** sram_port) {
  int i;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);
  assert(SPICE_MODEL_PASS_GATE_TRANSMISSION == spice_model.pass_gate_logic->type);

  for (i = 0; i < spice_mux_arch.num_input; i++) {
    /* Print a RRAM */
    fprintf(fp,"Xrram%d mux2_l%d_in%d mux2_l%d_in%d ", 
            i, 1, i, 0, 0);
    fprintf(fp, "%s%d %s_inv%d ", sram_port[0]->prefix, i, sram_port[0]->prefix, i); /* sram sram_inv */ 
    fprintf(fp, "rram_behavior switch_thres=vsp ron=ron roff=roff\n");
    /* Print a pair of programming transistor: 1 PMOS and 1 NMOS */
    /* PMOS */
    fprintf(fp, "Xpmos_prog_pair%d ", i); /* given_name */
    fprintf(fp, "mux2_l%d_in%d ", 1, i); /* input0  */
    fprintf(fp, "svdd sgnd svdd %s W=\'%g*wp\' \n", 
            pmos_subckt_name, spice_model.pass_gate_logic->pmos_size);
    /* NMOS */
    fprintf(fp, "Xnmos_prog_pair%d ", i); /* given_name */
    fprintf(fp, "mux2_l%d_in%d ", 1, i); /* input0  */
    fprintf(fp, "sgnd sgnd sgnd %s W=\'%g*wn\' \n", 
            nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);
  }

  /* Add the shared programmming pair at the output */
  /* PMOS */
  fprintf(fp, "Xpmos_prog_pair_out "); /* given_name */
  fprintf(fp, "mux2_l%d_in%d ", 0, 0); /* input0  */
  fprintf(fp, "svdd sgnd svdd %s W=\'%g*wp\' \n", 
          pmos_subckt_name, spice_model.pass_gate_logic->pmos_size);
  /* NMOS */
  fprintf(fp, "Xnmos_prog_pair_out "); /* given_name */
  fprintf(fp, "mux2_l%d_in%d ", 0, 0); /* input0  */
  fprintf(fp, "sgnd sgnd sgnd %s W=\'%g*wn\' \n", 
          nmos_subckt_name, spice_model.pass_gate_logic->nmos_size);

  return;
}

void fprint_spice_mux_model_cmos_subckt(FILE* fp,
                                        int mux_size,
                                        t_spice_model spice_model,
                                        t_spice_mux_arch spice_mux_arch) {
  int i, j;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                           + strlen(my_itoa(mux_size)) + strlen(mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_model.name, mux_size, mux_basis_posfix);

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
  input_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);
  sram_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port);

  /* Asserts*/
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  assert(1 == output_port[0]->size);

  /* Print the definition of subckt*/
  if (SPICE_MODEL_LUT == spice_model.type) {
    /* Special for LUT MUX*/
    fprintf(fp, "***** CMOS MUX info: spice_model_name= %s_MUX, size=%d *****\n", spice_model.name, mux_size);
    fprintf(fp, ".subckt %s_mux_size%d ", spice_model.name, mux_size);
  } else {
    fprintf(fp, "***** CMOS MUX info: spice_model_name=%s, size=%d, structure: %s *****\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.structure));
    fprintf(fp, ".subckt %s_size%d ", spice_model.name, mux_size);
  }
  /* Print input ports*/
  for (i = 0; i < mux_size; i++) {
    fprintf(fp, "%s%d ", input_port[0]->prefix, i);
  } 
  /* Print output ports*/
  fprintf(fp, "%s ", output_port[0]->prefix);
  switch (spice_model.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    /* Print sram ports*/
    for (i = 0; i < spice_mux_arch.num_level; i++) {
      fprintf(fp, "%s%d ", sram_port[0]->prefix, spice_mux_arch.num_level-i-1);
      fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, spice_mux_arch.num_level-i-1);
    } 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    /* Print sram ports*/
    for (i = 0; i < mux_size; i++) {
      fprintf(fp, "%s%d ", sram_port[0]->prefix, mux_size - i - 1);
      fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, mux_size - i - 1);
    } 
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    /* Print sram ports*/
    for (i = 0; i < spice_mux_arch.num_level; i++) {
      for (j = 0; j < spice_mux_arch.num_input_basis; j++) {
        fprintf(fp, "%s%d ", sram_port[0]->prefix, j + spice_mux_arch.num_input_basis*(spice_mux_arch.num_level-i-1));
        fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, j + spice_mux_arch.num_input_basis*(spice_mux_arch.num_level-i-1));
      }
    } 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }
  /* Print local vdd and gnd*/
  fprintf(fp, "svdd sgnd");
  fprintf(fp, "\n");
  
  /* Print internal architecture*/ 
  switch (spice_model.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    fprint_spice_cmos_mux_tree_structure(fp, mux_basis_subckt_name, spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    fprint_spice_cmos_mux_onelevel_structure(fp, spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    fprint_spice_cmos_mux_multilevel_structure(fp, mux_basis_subckt_name, spice_model, spice_mux_arch, num_sram_port, sram_port);
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
        fprintf(fp, "svdd sgnd inv size=\'%g\'", spice_model.input_buffer->size); /* subckt name */
        fprintf(fp, "\n");
        break;
      case SPICE_MODEL_BUF_BUF:
        /* TODO: what about tapered buffer, can we support? */
        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "Xbuf%d ", i); /* Given name*/
        fprintf(fp, "%s%d ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in%d ", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "svdd sgnd buf size=\'%g\'", spice_model.input_buffer->size); /* subckt name */
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
      fprintf(fp, "svdd sgnd inv size=\'%g\'", spice_model.output_buffer->size); /* subckt name */
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
      fprintf(fp, "svdd sgnd buf size=\'%g\'", spice_model.output_buffer->size); /* subckt name */
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
      fprintf(fp, "mux2_l%d_in%d ", 0, 0); /* input port */ 
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
  fprintf(fp, "***** END CMOS MUX info: spice_model_name=%s, size=%d *****\n", spice_model.name, mux_size);
  fprintf(fp, "\n");

  /* Free */
  my_free(mux_basis_subckt_name);
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
  int i, j;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                           + strlen(my_itoa(mux_size)) + strlen(mux_basis_posfix) + 1)); 
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
  input_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);
  sram_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port);

  /* Asserts*/
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  assert(1 == output_port[0]->size);

  /* Print the definition of subckt*/
  if (SPICE_MODEL_LUT == spice_model.type) {
    /* Special for LUT MUX*/
    fprintf(fp, "***** RRAM MUX info: spice_model_name= %s_MUX, size=%d *****\n", spice_model.name, mux_size);
    fprintf(fp, ".subckt %s_mux_size%d ", spice_model.name, mux_size);
  } else {
    fprintf(fp, "***** RRAM MUX info: spice_model_name=%s, size=%d, structure: %s *****\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.structure));
    fprintf(fp, ".subckt %s_size%d ", spice_model.name, mux_size);
  }
  /* Print input ports*/
  for (i = 0; i < mux_size; i++) {
    fprintf(fp, "%s%d ", input_port[0]->prefix, i);
  } 
  /* Print output ports*/
  fprintf(fp, "%s ", output_port[0]->prefix);
  switch (spice_model.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    /* Print sram ports*/
    for (i = 0; i < spice_mux_arch.num_level; i++) {
      fprintf(fp, "%s%d ", sram_port[0]->prefix, spice_mux_arch.num_level-i-1);
      fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, spice_mux_arch.num_level-i-1);
    } 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    /* Print sram ports*/
    for (i = 0; i < mux_size; i++) {
      fprintf(fp, "%s%d ", sram_port[0]->prefix, mux_size - i - 1);
      fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, mux_size - i - 1);
    } 
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    /* Print sram ports*/
    for (i = 0; i < spice_mux_arch.num_level; i++) {
      for (j = 0; j < spice_mux_arch.num_input_basis; j++) {
        fprintf(fp, "%s%d ", sram_port[0]->prefix, j + spice_mux_arch.num_input_basis*(spice_mux_arch.num_level-i-1));
        fprintf(fp, "%s_inv%d ", sram_port[0]->prefix, j + spice_mux_arch.num_input_basis*(spice_mux_arch.num_level-i-1));
      }
    } 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }
  /* Print local vdd and gnd*/
  fprintf(fp, "svdd sgnd ron=\'%g\' roff=\'%g\' wprog=\'%g*wn\'", 
          spice_model.ron, spice_model.roff, spice_model.prog_trans_size);
  fprintf(fp, "\n");
  
  /* Print internal architecture*/ 
  switch (spice_model.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    fprint_spice_rram_mux_tree_structure(fp, mux_basis_subckt_name, spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    fprint_spice_rram_mux_onelevel_structure(fp, spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    fprint_spice_rram_mux_multilevel_structure(fp, mux_basis_subckt_name, spice_model, spice_mux_arch, num_sram_port, sram_port);
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
        fprintf(fp, "svdd sgnd inv size=\'%g\'", spice_model.input_buffer->size); /* subckt name */
        fprintf(fp, "\n");
        break;
      case SPICE_MODEL_BUF_BUF:
        /* TODO: what about tapered buffer, can we support? */
        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "Xbuf%d ", i); /* Given name*/
        fprintf(fp, "%s%d ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in%d ", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "svdd sgnd buf size=\'%g\'", spice_model.input_buffer->size); /* subckt name */
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
      fprintf(fp, "svdd sgnd inv size=\'%g\'", spice_model.output_buffer->size); /* subckt name */
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
      fprintf(fp, "svdd sgnd buf size=\'%g\'", spice_model.output_buffer->size); /* subckt name */
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

  /* Check the design technology*/
  switch (spice_mux_model->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    fprint_spice_mux_model_cmos_subckt(fp, spice_mux_model->size,
                                        *(spice_mux_model->spice_model), *(spice_mux_model->spice_mux_arch));
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    fprint_spice_mux_model_rram_subckt(fp, spice_mux_model->size,
                                        *(spice_mux_model->spice_model), *(spice_mux_model->spice_mux_arch));
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name); 
    exit(1);
  }
  return;
}



t_llist* stats_spice_muxes(int num_switch,
                           t_switch_inf* switches,
                           t_spice* spice,
                           t_det_routing_arch* routing_arch) {
  /* Linked-list to store the information of Multiplexers*/
  t_llist* muxes_head = NULL; 
  //t_llist* temp = NULL;
  int inode;
  int itype;
  int imodel;
  t_rr_node* node;
  t_spice_model* sb_switch_spice_model = NULL;
  t_spice_model* cb_switch_spice_model = NULL;

  /* Current Version: Support Uni-directional routing architecture only*/ 
  if (UNI_DIRECTIONAL != routing_arch->directionality) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Spice Modeling Only support uni-directional routing architecture.\n",__FILE__, __LINE__);
    exit(1);
  }

  /* Step 1: We should check the multiplexer spice models defined in routing architecture.*/
  /* The routing path is. 
   *  OPIN ----> CHAN ----> ... ----> CHAN ----> IPIN
   *  Each edge is a switch, for IPIN, the switch is a connection block,
   *  for the rest is a switch box
   */
  /* Update the driver switch for each rr_node*/
  update_rr_nodes_driver_switch(routing_arch->directionality);
  /* Count the sizes of muliplexers in routing architecture */  
  /* Visit the global variable : num_rr_nodes, rr_node */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    node = &rr_node[inode]; 
    switch (node->type) {
    case IPIN: 
      /* Have to consider the fan_in only, it is a connection box(multiplexer)*/
      assert((node->fan_in > 0)||(0 == node->fan_in));
      if (0 == node->fan_in) {
        break; 
      }
      /* Find the spice_model for multiplexers in connection blocks */
      cb_switch_spice_model = switches[node->driver_switch].spice_model;
      /* we should select a spice model for the connection box*/
      assert(NULL != cb_switch_spice_model);
      check_and_add_mux_to_linked_list(&muxes_head, node->fan_in,cb_switch_spice_model);
      break;
    case CHANX:
    case CHANY: 
      /* Channels are the same, have to consider the fan_in as well, 
       * it could be a switch box if previous rr_node is a channel
       * or it could be a connection box if previous rr_node is a IPIN or OPIN
       */
      assert((node->fan_in > 0)||(0 == node->fan_in));
      if (0 == node->fan_in) {
        break; 
      }
      /* Find the spice_model for multiplexers in switch blocks*/
      sb_switch_spice_model = switches[node->driver_switch].spice_model;
      /* we should select a spice model for the Switch box*/
      assert(NULL != sb_switch_spice_model);
      check_and_add_mux_to_linked_list(&muxes_head, node->fan_in,sb_switch_spice_model);
      break;
    case OPIN: 
      /* Actually, in single driver routing architecture, the OPIN, source of a routing path,
       * is directly connected to Switch Box multiplexers
       */
      break;
    default:
      break;
    }
  }

  /* Statistics after search routing resources */
  /*
  temp = muxes_head;
  while(temp) {
    t_spice_mux_model* spice_mux_model = (t_spice_mux_model*)temp->dptr;
    vpr_printf(TIO_MESSAGE_INFO,"Routing multiplexers: size=%d\n",spice_mux_model->size);
    temp = temp->next;
  }
  */

  /* Step 2: Count the sizes of multiplexers in complex logic blocks */  
  for (itype = 0; itype < num_types; itype++) {
    if (NULL != type_descriptors[itype].pb_type) {
      stats_mux_spice_model_pb_type_rec(&muxes_head,type_descriptors[itype].pb_type);
    }
  }

  /* Step 3: count the size of multiplexer that will be used in LUTs*/
  for (imodel = 0; imodel < spice->num_spice_model; imodel++) {
    /* For those LUTs that netlists are not provided. We create a netlist and thus need a MUX*/
    if ((SPICE_MODEL_LUT == spice->spice_models[imodel].type)
      &&(NULL == spice->spice_models[imodel].model_netlist)) {
      stats_lut_spice_mux(&muxes_head, &(spice->spice_models[imodel])); 
    }
  }

  /* Statistics after search routing resources */
  /*
  temp = muxes_head;
  while(temp) {
    t_spice_mux_model* spice_mux_model = (t_spice_mux_model*)temp->dptr;
    vpr_printf(TIO_MESSAGE_INFO,"Pb_types multiplexers: size=%d\n",spice_mux_model->size);
    temp = temp->next;
  }
  */

  return muxes_head;
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
      input_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_INPUT, &num_input_ports);
      sram_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_ports);
      assert(0 == num_input_ports);
      assert(0 == num_sram_ports);
      /* Check the Input port size */
      if (cur_spice_mux_model->size != input_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) size(%d) unmatch with the architecture needs(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, input_ports[0]->size,cur_spice_mux_model->size);
        exit(1);
      }
      /* Check the SRAM port size */
      num_input_basis = determine_num_input_basis_multilevel_mux(cur_spice_mux_model->size, 
                                                                 cur_spice_mux_model->spice_model->mux_num_level);
      if ((num_input_basis * cur_spice_mux_model->spice_model->mux_num_level) != sram_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) SRAM size(%d) unmatch with the num of level(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, sram_ports[0]->size, cur_spice_mux_model->spice_model->mux_num_level*num_input_basis);
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

