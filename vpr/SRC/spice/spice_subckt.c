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
#include "rr_graph.h"
#include "vpr_utils.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_lut.h"
#include "spice_pbtypes.h"
#include "spice_routing.h"
#include "spice_subckt.h"

/***** Local Subroutines *****/
static 
int search_tapbuf_llist_same_settings(t_llist* head,
                                      t_spice_model_buffer* output_buf);

static 
int generate_spice_basics(char* subckt_dir, t_spice spice);

/* Generate the NMOS and PMOS */
int generate_spice_nmos_pmos(char* subckt_dir,
                             t_spice_tech_lib tech_lib) {
  FILE* fp = NULL;
  char* sp_name = my_strcat(subckt_dir,nmos_pmos_spice_file_name);
  t_spice_transistor_type* nmos_trans = NULL;
  t_spice_transistor_type* pmos_trans = NULL;

  /* Spot NMOS*/
  nmos_trans = find_mosfet_tech_lib(tech_lib,SPICE_TRANS_NMOS);
  if (NULL == nmos_trans) {
    vpr_printf(TIO_MESSAGE_ERROR,"NMOS transistor is not defined in architecture XML!\n");
    exit(1);
  }

  /* Spot PMOS*/
  pmos_trans = find_mosfet_tech_lib(tech_lib,SPICE_TRANS_PMOS);
  if (NULL == pmos_trans) {
    vpr_printf(TIO_MESSAGE_ERROR,"PMOS transistor is not defined in architecture XML!\n");
    exit(1);
  }

  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top SPICE netlist %s",__FILE__, __LINE__, nmos_pmos_spice_file_name); 
    exit(1);
  } 
  fprint_spice_head(fp,"NMOS and PMOS");

  /* print sub circuit for PMOS*/
  fprintf(fp,"* NMOS\n");
  fprintf(fp,".subckt %s drain gate source bulk L=nl W=wn\n", nmos_subckt_name);
  fprintf(fp,"%s1 drain gate source bulk %s L=L W=W\n", tech_lib.model_ref, nmos_trans->model_name);
  fprintf(fp,".eom %s\n", nmos_subckt_name);

  fprintf(fp,"\n");
  /* Print sub circuit for PMOS*/
  fprintf(fp,"* PMOS\n");
  fprintf(fp,".subckt %s drain gate source bulk L=pl W=wp\n", pmos_subckt_name);
  fprintf(fp,"%s1 drain gate source bulk %s L=L W=W\n", tech_lib.model_ref, pmos_trans->model_name);
  fprintf(fp,".eom %s\n", pmos_subckt_name);

  fclose(fp);

  return 1;
}

static 
int search_tapbuf_llist_same_settings(t_llist* head,
                                      t_spice_model_buffer* output_buf) {
  t_llist* temp = head;
  t_spice_model_buffer* cur_out_buf = NULL;

  /* check */
  if ((NULL == output_buf)||(NULL == head)) {
    return 0;
  }
  assert(NULL != output_buf);
  assert(TRUE == output_buf->exist);
  assert(TRUE == output_buf->tapered_buf);

  while(temp) {
    cur_out_buf = (t_spice_model_buffer*)(temp->dptr);
    assert(TRUE == cur_out_buf->exist);
    assert(TRUE == cur_out_buf->tapered_buf);
    if ((cur_out_buf->tap_buf_level == output_buf->tap_buf_level)
       &&(cur_out_buf->f_per_stage == output_buf->f_per_stage)) { 
      return 1;
    } 
    temp = temp->next;
  }
  return 0;
}

void generate_spice_subckt_tapbuf(FILE* fp, 
                                  t_spice_model_buffer* output_buf) {
  int istage, j;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top SPICE netlist %s",__FILE__, __LINE__, basics_spice_file_name); 
    exit(1);
  } 

  assert(NULL != output_buf);
  assert(TRUE == output_buf->exist);
  assert(TRUE == output_buf->tapered_buf);
  assert(0 < output_buf->tap_buf_level);
  assert(0 < output_buf->f_per_stage);

  /* Definition line */
  fprintf(fp, ".subckt tapbuf_level%d_f%d in out svdd sgnd\n", output_buf->tap_buf_level, output_buf->f_per_stage); 
  /* Main body of tapered buffer */
  if (0 == output_buf->tap_buf_level%2) {
    fprintf(fp, "Xinv_in in in_lvl0 svdd sgnd inv size=1\n");
  } else {
    fprintf(fp, "Rinv_in in in_lvl0 0\n");
  }
  /* Print each stage */
  for (istage = 0; istage < output_buf->tap_buf_level; istage++) {
    if (istage == (output_buf->tap_buf_level - 1)) {
      for (j = 0; j < pow(output_buf->f_per_stage,istage); j++) {
        fprintf(fp, "Xinv_lvl%d_no%d in_lvl%d out svdd sgnd inv size=1\n",
                istage, j, istage);
      }
      continue;
    } 
    for (j = 0; j < pow(output_buf->f_per_stage,istage); j++) {
      fprintf(fp, "Xinv_lvl%d_no%d in_lvl%d in_lvl%d svdd sgnd inv size=1\n",
              istage, j, istage, istage + 1);
    }
  }
  /* End of subckt*/
  fprintf(fp, ".eom\n\n");

  return;
}

static 
int generate_spice_basics(char* subckt_dir, t_spice spice) {
  FILE* fp = NULL;
  char* sp_name = my_strcat(subckt_dir, basics_spice_file_name);
  int imodel = 0;
  t_llist* tapered_bufs_head = NULL;
  t_llist* temp = NULL;
 
  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top SPICE netlist %s",__FILE__, __LINE__, basics_spice_file_name); 
    exit(1);
  } 
  fprint_spice_head(fp,"Inverter, Buffer, Trans. Gate");
 
  /* Inverter */
  fprintf(fp,"* Inverter\n"); 
  fprintf(fp,".subckt inv in out svdd sgnd size=1\n");
  fprintf(fp,"Xn0_inv out in sgnd sgnd %s L=nl W=\'size*wn\'\n",nmos_subckt_name);
  fprintf(fp,"Xp0_inv out in svdd svdd %s L=pl W=\'size*beta*wp\'\n",pmos_subckt_name);
  fprintf(fp,".eom inv\n");
  fprintf(fp,"\n");
  /* Buffer */
  fprintf(fp,"* Buffer\n"); 
  fprintf(fp,".subckt buf in out svdd sgnd size=2\n");
  fprintf(fp,"Xinv0 in  mid svdd sgnd inv\n");
  fprintf(fp,"Xinv1 mid out svdd sgnd inv size=size\n");
  fprintf(fp,".eom buf\n");
  fprintf(fp,"\n");
  /* Transmission Gate*/
  fprintf(fp,"* Transmission Gate (Complementary Pass Transistor)\n"); 
  fprintf(fp,".subckt cpt in out sel sel_inv svdd sgnd nmos_size=1 pmos_size=1\n");
  fprintf(fp,"Xn0_cpt in sel out sgnd %s L=nl W=\'nmos_size*wn\'\n", nmos_subckt_name);
  fprintf(fp,"Xp0_cpt in sel_inv out svdd %s L=pl W=\'pmos_size*wp\'\n", pmos_subckt_name);
  fprintf(fp,".eom cpt\n");
  fprintf(fp,"\n");

  /* Tapered buffered support */
  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {
    assert(NULL != spice.spice_models[imodel].input_buffer);
    assert(NULL != spice.spice_models[imodel].output_buffer);
    if ((TRUE == spice.spice_models[imodel].output_buffer->exist)
      &&(TRUE == spice.spice_models[imodel].output_buffer->tapered_buf)) {
      if (NULL == tapered_bufs_head) {
        tapered_bufs_head = create_llist(1);
        tapered_bufs_head->dptr = (void*)spice.spice_models[imodel].output_buffer;
      } else if (FALSE == search_tapbuf_llist_same_settings(tapered_bufs_head, spice.spice_models[imodel].output_buffer)) {
        temp = insert_llist_node(tapered_bufs_head);
        temp->dptr = (void*)spice.spice_models[imodel].output_buffer;
      } 
    }
  }
  /* Print all the tapered_buf */
  temp = tapered_bufs_head;
  while(temp) {
    generate_spice_subckt_tapbuf(fp, (t_spice_model_buffer*)(temp->dptr));
    temp = temp->next;
  }

  fclose(fp);

  /* Free */
  temp = tapered_bufs_head;
  while(temp) {
    temp->dptr = NULL;
    temp = temp->next;
  }
  free_llist(tapered_bufs_head);

  return 1;
}

void generate_spice_rram_veriloga(char* subckt_dir, 
                                  t_spice spice) {
  FILE* fp = NULL;
  char* sp_name = NULL;
  int imodel, write_model;

  /* Check if we need such a verilogA model */
  write_model = 0;
  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {
    if (SPICE_MODEL_DESIGN_RRAM == spice.spice_models[imodel].design_tech) {
      write_model = 1;
      break;
    }
  }
  if (0 == write_model) {
    return;
  } else {
    rram_design_tech = 1;
  }
  
  sp_name = my_strcat(subckt_dir, rram_veriloga_file_name);
  /* Open a File */
  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE netlist %s",__FILE__, __LINE__, wires_spice_file_name); 
    exit(1);
  } 
  fprintf(fp,"// RRAM Behavior VerilogA Mode\n");
  
  /* Include the headers of VerilogA */
  fprintf(fp, "`include \"constants.vams\"\n");
  fprintf(fp, "`include \"disciplines.vams\"\n");

  /* Model */
  fprintf(fp, "\n");
  fprintf(fp, "module rram_behavior(TE, BE, SRAM, SRAM_INV);\n");
  fprintf(fp, "inout TE, BE, SRAM, SRAM_INV;\n");
  fprintf(fp, "electrical TE, BE, SRAM, SRAM_INV;\n");
  fprintf(fp, "// Design Parameters\n");
  fprintf(fp, "parameter integer initial_state = 1 from [0:1];\n");
  fprintf(fp, "parameter real switch_thres = 1.8 from (0:inf);\n");
  fprintf(fp, "parameter real ron = 1e3 from (0:inf);\n");
  fprintf(fp, "parameter real roff = 1e6 from (0:inf);\n");
  fprintf(fp, "// Local Parameters\n");
  fprintf(fp, "real res = 1e3;\n");
  fprintf(fp, "real voltage_tolerence = 0;\n");
  fprintf(fp, "integer state = 1;\n");
  fprintf(fp, "\n");
  fprintf(fp, "analog begin\n");
  fprintf(fp, "  // Initial\n");
  fprintf(fp, "  @(initial_step) begin\n");
  fprintf(fp, "    state = initial_state;\n");
  fprintf(fp, "  end\n");
  fprintf(fp, "  // State\n");
  fprintf(fp, "  if (V(SRAM,SRAM_INV) < voltage_tolerence*switch_thres) begin\n");
  fprintf(fp, "    state = 0;\n");
  fprintf(fp, "  end else begin\n");
  fprintf(fp, "    state = 1;\n");
  fprintf(fp, "  end\n");
  fprintf(fp, "  //LRS\n");
  fprintf(fp, "  if (1 == state) begin\n");
  fprintf(fp, "    res = ron;\n");
  fprintf(fp, "  //HRS\n");
  fprintf(fp, "  end else begin\n");
  fprintf(fp, "    res = roff;\n");
  fprintf(fp, "  end\n");
  fprintf(fp, "  // Correlated Resistance with TE and BE\n");
  fprintf(fp, "  I(TE,BE) <+ V(TE,BE) / res;\n");
  fprintf(fp, "end\n");
  fprintf(fp, "endmodule\n");

  /* Close File */ 
  fclose(fp);
 
  /* Free */
  my_free(sp_name);

  return;  
}

void fprint_spice_wire_model(FILE* fp,
                             char* wire_subckt_name,
                             t_spice_model spice_model,
                             float res_total,
                             float cap_total) {
  float res_per_level = 0.;
  float cap_per_level = 0.;
  int i;
  int num_input_port = 0;
  int num_output_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
 
  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  } 
  /* Check the wire model*/
  assert(NULL != spice_model.wire_param);
  assert(0 < spice_model.wire_param->level);
  /* Find the input port, output port*/
  input_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);

  /* Asserts*/
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == input_port[0]->size);
  assert(1 == output_port[0]->size);
  /* print the spice model*/
  fprintf(fp, "* Wire, spice_model_name=%s\n", spice_model.name);  
  switch (spice_model.type) {
  case SPICE_MODEL_CHAN_WIRE: 
    /* Add an output at middle point for connecting CB inputs */
    fprintf(fp, ".subckt %s %s %s mid_out svdd sgnd\n",
            wire_subckt_name, input_port[0]->prefix, output_port[0]->prefix);
    break;
  case SPICE_MODEL_WIRE: 
    /* Add an output at middle point for connecting CB inputs */
    fprintf(fp, ".subckt %s %s %s svdd sgnd\n",
            wire_subckt_name, input_port[0]->prefix, output_port[0]->prefix);
    /* Direct shortcut */
    if ((0. == cap_per_level)&&(0. == res_per_level)
      &&(0 == spice_model.input_buffer->exist)
      &&(0 == spice_model.output_buffer->exist)) {
      fprintf(fp, "Rshortcut %s %s 0\n", input_port[0]->prefix, output_port[0]->prefix);
      /* Finish*/ 
      fprintf(fp, ".eom\n");
      fprintf(fp, "\n");
      return;
    }
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of spice_model! Expect [chan_wire|wire].\n",
               __FILE__, __LINE__);
    exit(1);
  }
  

  /* Determine which type of model to print*/
  switch (spice_model.wire_param->type) {
  case WIRE_MODEL_PIE :
    /* Determine the resistance and capacitance of each level*/
    res_per_level = res_total/((float)(2*spice_model.wire_param->level));
    cap_per_level = cap_total/((float)(spice_model.wire_param->level + 1));
    if ((0. == cap_per_level)&&(0. == res_per_level)) {
      /* Speical: if R and C are all zeros, we use a zero-voltage source instead */
      fprintf(fp, "Vshortcut pie_wire_in%d pie_wire_in%d 0\n",
              0, spice_model.wire_param->level); 
      if (SPICE_MODEL_CHAN_WIRE == spice_model.type) {
        fprintf(fp, "* Connect the output of middle point\n");
        fprintf(fp, "Vmid_out_ckt pie_wire_in0 mid_out 0\n");
      }
      i = spice_model.wire_param->level;
      break;
    }
    if (0. != cap_per_level) {
      fprintf(fp, "Clvin pie_wire_in0 sgnd %g\n", cap_per_level);
    }
    for (i = 0; i < spice_model.wire_param->level; i++) {
      fprintf(fp, "Rlv%d_idx0 pie_wire_in%d pie_wire_in%d_inter %g\n",
              i, i, i, res_per_level); 
      fprintf(fp, "Rlv%d_idx1 pie_wire_in%d_inter pie_wire_in%d %g\n",
              i, i, i + 1, res_per_level); 
      if (0. != cap_per_level) {
        fprintf(fp, "Clv%d_idx1 pie_wire_in%d sgnd %g\n",
                i, i + 1, cap_per_level); 
      }
    }
    if (SPICE_MODEL_CHAN_WIRE == spice_model.type) {
      /*Connect the middle point */
      fprintf(fp, "* Connect the output of middle point\n");
      if (0 == spice_model.wire_param->level%2) {
        fprintf(fp, "Vmid_out_ckt pie_wire_in%d mid_out 0\n", spice_model.wire_param->level/2);
      } else if (1 == spice_model.wire_param->level%2) {
        fprintf(fp, "Vmid_out_ckt pie_wire_in%d_inter mid_out 0\n", spice_model.wire_param->level/2);
      }
    }
    break;
  case WIRE_MODEL_T : 
    /* Determine the resistance and capacitance of each level*/
    res_per_level = res_total/((float)(2*spice_model.wire_param->level));
    cap_per_level = cap_total/((float)(spice_model.wire_param->level));
    if ((0. == cap_per_level)&&(0. == res_per_level)) {
      fprintf(fp, "Vshortcut pie_wire_in%d pie_wire_in%d 0\n",
              0, spice_model.wire_param->level); 
      if (SPICE_MODEL_CHAN_WIRE == spice_model.type) {
        fprintf(fp, "* Connect the output of middle point\n");
        fprintf(fp, "Vmid_out_ckt pie_wire_in0 mid_out 0\n");
      }
      i = spice_model.wire_param->level;
      break;
    }
    for (i = 0; i < spice_model.wire_param->level; i++) {
      fprintf(fp, "Rlv%d_idx0 pie_wire_in%d pie_wire_in%d_inter %g\n",
              i, i, i, res_per_level); 
      if (0. != cap_per_level) {
        fprintf(fp, "Clv%d_idx1 pie_wire_in%d_inter sgnd %g\n",
                i, i, cap_per_level); 
      }
      fprintf(fp, "Rlv%d_idx2 pie_wire_in%d_inter pie_wire_in%d %g\n",
              i, i, i + 1, res_per_level); 
    }
    if (SPICE_MODEL_CHAN_WIRE == spice_model.type) {
      /*Connect the middle point */
      fprintf(fp, "* Connect the output of middle point\n");
      if (0 == spice_model.wire_param->level%2) {
        fprintf(fp, "Vmid_out_ckt pie_wire_in%d mid_out 0\n", spice_model.wire_param->level/2);
      } else if (1 == spice_model.wire_param->level%2) {
        fprintf(fp, "Vmid_out_ckt pie_wire_in%d_inter mid_out 0\n", spice_model.wire_param->level/2);
      }
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_INFO,"(File:%s,[LINE%d])Invalid SPICE Wire Model type of spice_model(%s).\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  } 
  assert(i == spice_model.wire_param->level);

  /* Add input, output buffers*/
  assert(NULL != spice_model.input_buffer);
  if (spice_model.input_buffer->exist) {
    switch (spice_model.input_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "Xinv_in "); /* Given name*/
      fprintf(fp, "%s ", input_port[0]->prefix); /* input port */ 
      fprintf(fp, "pie_wire_in0 "); /* output port*/
      fprintf(fp, "svdd sgnd inv size=\'%g\'", spice_model.input_buffer->size); /* subckt name */
      fprintf(fp, "\n");
      break;
    case SPICE_MODEL_BUF_BUF:
      /* TODO: what about tapered buffer, can we support? */
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "Xbuf_in "); /* Given name*/
      fprintf(fp, "%s ", input_port[0]->prefix); /* input port */ 
      fprintf(fp, "pie_wire_in0 "); /* output port*/
      fprintf(fp, "svdd sgnd buf size=\'%g\'", spice_model.input_buffer->size); /* subckt name */
      fprintf(fp, "\n");
      break;
      default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                 __FILE__, __LINE__);
      exit(1);   
    }
  } else {
    /* There is no buffer, I create a zero voltage source between*/
    /* V<given_name> <input> <output> 0*/
    fprintf(fp, "Rin %s pie_wire_in0 0\n", 
            input_port[0]->prefix);
  }

  assert(NULL != spice_model.input_buffer);
  if (spice_model.output_buffer->exist) {
    switch (spice_model.output_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "Xinv_out "); /* Given name*/
      fprintf(fp, "pie_wire_in%d ", spice_model.wire_param->level); /* input port */ 
      fprintf(fp, "%s ", output_port[0]->prefix); /* output port*/
      fprintf(fp, "svdd sgnd inv size=\'%g\'", spice_model.output_buffer->size); /* subckt name */
      fprintf(fp, "\n");
      break;
    case SPICE_MODEL_BUF_BUF:
      /* TODO: what about tapered buffer, can we support? */
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "Xbuf_out "); /* Given name*/
      fprintf(fp, "pie_wire_in%d ", spice_model.wire_param->level); /* input port */ 
      fprintf(fp, "%s ", output_port[0]->prefix); /* output port*/
      fprintf(fp, "svdd sgnd buf size=\'%g\'", spice_model.output_buffer->size); /* subckt name */
      fprintf(fp, "\n");
      break;
      default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                 __FILE__, __LINE__);
      exit(1);   
    }
  } else {
    /* There is no buffer, I create a zero voltage source between*/
    /* V<given_name> <input> <output> 0*/
    fprintf(fp, "Rout pie_wire_in%d %s 0\n", 
            spice_model.wire_param->level, output_port[0]->prefix);
  }

  /* Finish*/ 
  fprintf(fp, ".eom\n");
  fprintf(fp, "\n");

  return;
}

void generate_spice_wires(char* subckt_dir,
                          int num_segments,
                          t_segment_inf* segments,
                          int num_spice_model,
                          t_spice_model* spice_models) {
  FILE* fp = NULL;
  char* sp_name = my_strcat(subckt_dir, wires_spice_file_name);
  char* seg_wire_subckt_name = NULL;
  char* seg_index_str = NULL;
  int iseg, imodel, len_seg_subckt_name;
 
  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE netlist %s",__FILE__, __LINE__, wires_spice_file_name); 
    exit(1);
  } 
  fprint_spice_head(fp,"Wires");
  /* Output wire models*/
  for (imodel = 0; imodel < num_spice_model; imodel++) {
    if (SPICE_MODEL_WIRE == spice_models[imodel].type) {
      assert(NULL != spice_models[imodel].wire_param);
      fprint_spice_wire_model(fp, spice_models[imodel].name,
                              spice_models[imodel], 
                              spice_models[imodel].wire_param->res_val,
                              spice_models[imodel].wire_param->cap_val);
    }
  }
 
  /* Create wire models for routing segments*/
  fprintf(fp,"* Wire models for segments in routing \n");
  for (iseg = 0; iseg < num_segments; iseg++) {
    assert(NULL != segments[iseg].spice_model);
    assert(SPICE_MODEL_CHAN_WIRE == segments[iseg].spice_model->type);
    assert(NULL != segments[iseg].spice_model->wire_param);
    /* Give a unique name for subckt of wire_model of segment, 
     * spice_model name is unique, and segment name is unique as well
     */
    seg_index_str = my_itoa(iseg);
    len_seg_subckt_name = strlen(segments[iseg].spice_model->name)
                        + 4 + strlen(seg_index_str) + 1; /* '\0'*/
    seg_wire_subckt_name = (char*)my_malloc(sizeof(char)*len_seg_subckt_name);
    sprintf(seg_wire_subckt_name,"%s_seg%s",
            segments[iseg].spice_model->name, seg_index_str);
    fprint_spice_wire_model(fp, seg_wire_subckt_name,
                            *(segments[iseg].spice_model), 
                            segments[iseg].Rmetal,
                            segments[iseg].Cmetal);
  }
  
  /* Close the file handler */
  fclose(fp);

  /*Free*/
  my_free(seg_index_str);
  my_free(seg_wire_subckt_name);

  return;
}

/* Generate the sub circuits for FPGA SPICE Modeling
 * Tasks:
 * 1. NMOS and PMOS
 * 2. Basics: Inverter and Buffers, transmission gates, rc_segements
 * 3. Multiplexers
 * 4. Look-Up Tables
 * 5. Flip-flops
 */
void generate_spice_subckts(char* subckt_dir,
                            t_arch* arch,
                            t_det_routing_arch* routing_arch) {
  /* 1.Generate NMOS, PMOS and transmission gate */
  vpr_printf(TIO_MESSAGE_INFO,"Writing SPICE NMOS and PMOS...\n");
  generate_spice_nmos_pmos(subckt_dir, arch->spice->tech_lib);

  /* 2. Generate Inverter, Buffer, and transmission gates*/
  vpr_printf(TIO_MESSAGE_INFO,"Writing SPICE Basic subckts...\n");
  generate_spice_basics(subckt_dir, *(arch->spice));

  /* 2.5 Generate RRAM Verilog-A model*/
  vpr_printf(TIO_MESSAGE_INFO, "Writing RRAM Behavior Verilog-A model...\n");
  generate_spice_rram_veriloga(subckt_dir, (*(arch->spice)));

  /* 3. Generate Multiplexers */
  vpr_printf(TIO_MESSAGE_INFO,"Writing SPICE Multiplexers...\n");
  generate_spice_muxes(subckt_dir, routing_arch->num_switch, switch_inf, 
                       arch->spice, routing_arch);

  /* 4. Generate Wires*/
  vpr_printf(TIO_MESSAGE_INFO,"Writing SPICE Wires...\n");
  generate_spice_wires(subckt_dir, arch->num_segments, arch->Segments,
                       arch->spice->num_spice_model, arch->spice->spice_models);

  /*5. Generate LUTs */
  vpr_printf(TIO_MESSAGE_INFO,"Writing SPICE LUTs...\n");
  generate_spice_luts(subckt_dir, arch->spice->num_spice_model, arch->spice->spice_models);
 
  /* 6. Generate Logic Blocks */
  vpr_printf(TIO_MESSAGE_INFO,"Writing Logic Blocks...\n");
  generate_spice_logic_blocks(subckt_dir, arch);

  /* 7. Generate Routing architecture*/
  vpr_printf(TIO_MESSAGE_INFO, "Writing Routing Resources....\n");
  generate_spice_routing_resources(subckt_dir, (*arch), routing_arch, rr_node_indices);

  return;
}

