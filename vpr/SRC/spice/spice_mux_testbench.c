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

/* Include spice support headers*/
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_routing.h"
#include "spice_subckt.h"
#include "spice_netlist_utils.h"
#include "spice_mux_testbench.h"

/** In this test bench. 
 * All the multiplexers (Local routing, Switch Boxes, Connection Blocks) in the FPGA are examined 
 * All the multiplexers are hanged with equivalent capactive loads in their context.
 */

/* Global variables in this C-source file */
static int testbench_mux_cnt = 0;
static int testbench_sram_cnt = 0;
static int testbench_load_cnt = 0;
static int testbench_pb_mux_cnt = 0;
static int testbench_cb_mux_cnt = 0;
static int testbench_sb_mux_cnt = 0;
static t_llist* testbench_muxes_head = NULL; 
static int num_segments = 0;
static t_segment_inf* segments = NULL;
static int upbound_sim_num_clock_cycles = 2;
static int max_sim_num_clock_cycles = 2;
static int auto_select_max_sim_num_clock_cycles = TRUE;

static float total_pb_mux_input_density = 0.;
static float total_cb_mux_input_density = 0.;
static float total_sb_mux_input_density = 0.;

/***** Local Subroutines Declaration *****/
static 
void fprint_spice_mux_testbench_global_ports(FILE* fp,
                                             t_spice spice);

float find_spice_mux_testbench_pb_pin_mux_load_inv_size(t_spice_model* fan_out_spice_model);

float find_spice_mux_testbench_rr_mux_load_inv_size(t_rr_node* load_rr_node,
                                                    int switch_index);

static 
void fprint_spice_mux_testbench_one_mux(FILE* fp,
                                        char* meas_tag,
                                        t_spice_model* mux_spice_model,
                                        int mux_size,
                                        int* input_init_value,
                                        float* input_density,
                                        float* input_probability,
                                        int path_id);

static 
void fprint_spice_mux_testbench_pb_pin_mux(FILE* fp,
                                           t_rr_node* pb_rr_nodes,
                                           t_pb* des_pb,
                                           t_mode* cur_mode,
                                           t_pb_graph_pin* des_pb_graph_pin,
                                           t_interconnect* cur_interc,
                                           int fan_in,
                                           int select_edge,
                                           int grid_x, int grid_y,
                                           t_ivec*** LL_rr_node_indices);

static 
void fprint_spice_mux_testbench_pb_pin_interc(FILE* fp,
                                              t_rr_node* pb_rr_nodes,
                                              t_pb* des_pb,
                                              enum e_pin2pin_interc_type pin2pin_interc_type,
                                              t_pb_graph_pin* des_pb_graph_pin,
                                              t_mode* cur_mode,
                                              int select_path_id,
                                              int grid_x, int grid_y,
                                              t_ivec*** LL_rr_node_indices);

static 
void fprint_spice_mux_testbench_pb_interc(FILE* fp,
                                          t_pb* cur_pb,
                                          int grid_x, int grid_y,
                                          t_ivec*** LL_rr_node_indices);

static 
void fprint_spice_mux_testbench_pb_muxes_rec(FILE* fp,
                                             t_pb* cur_pb,
                                             int grid_x, int grid_y,
                                             t_ivec*** LL_rr_node_indices);

static 
void fprint_spice_mux_testbench_cb_one_mux(FILE* fp,
                                           t_rr_type chan_type,
                                           int cb_x,
                                           int cb_y,
                                           t_rr_node* src_rr_node,
                                           t_ivec*** LL_rr_node_indices);

static 
void fprint_spice_mux_testbench_cb_interc(FILE* fp, 
                                          t_rr_type chan_type,
                                          int cb_x, int cb_y,
                                          t_rr_node* src_rr_node,
                                          t_ivec*** LL_rr_node_indices);

static 
int fprint_spice_mux_testbench_one_grid_cb_muxes(FILE* fp, 
                                                 t_rr_type chan_type,
                                                 int x, int y,
                                                 t_ivec*** LL_rr_node_indices);

static 
int fprint_spice_mux_testbench_sb_one_mux(FILE* fp,
                                          int switch_box_x, int switch_box_y,
                                          int chan_side,
                                          t_rr_node* src_rr_node);

static 
int fprint_spice_mux_testbench_call_one_grid_sb_muxes(FILE* fp, 
                                                      int x, int y, 
                                                      t_ivec*** LL_rr_node_indices);

/***** Local Subroutines *****/
static void init_spice_mux_testbench_globals(t_spice spice) {
  testbench_mux_cnt = 0;
  testbench_sram_cnt = 0;
  testbench_load_cnt = 0;
  testbench_muxes_head = NULL; 
  testbench_pb_mux_cnt = 0;
  testbench_cb_mux_cnt = 0;
  testbench_sb_mux_cnt = 0;
  auto_select_max_sim_num_clock_cycles = spice.spice_params.meas_params.auto_select_sim_num_clk_cycle;
  upbound_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  if (FALSE == auto_select_max_sim_num_clock_cycles) {
    max_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  } else {
    max_sim_num_clock_cycles = 2;
  }
}

static 
void fprint_spice_mux_testbench_global_ports(FILE* fp,
                                             t_spice spice) {
  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  fprintf(fp, "*.global gvdd gset greset\n");
  fprintf(fp, "*.global gvdd_sram\n");
  fprintf(fp, "*.global gvdd_load\n");
  fprintf(fp, "*.global %s->in\n", sram_spice_model->prefix);

  return;
}

float find_spice_mux_testbench_pb_pin_mux_load_inv_size(t_spice_model* fan_out_spice_model) {
  float load_inv_size = 0;

  /* Check */
  assert(NULL != fan_out_spice_model);
  assert(NULL != fan_out_spice_model->input_buffer);

  /* Special: this is a LUT, we should consider more inv size */
  if (SPICE_MODEL_LUT == fan_out_spice_model->type) {
    assert(1 == fan_out_spice_model->lut_input_buffer->exist);
    assert(SPICE_MODEL_BUF_INV == fan_out_spice_model->lut_input_buffer->type);
    assert(TRUE == fan_out_spice_model->lut_input_buffer->tapered_buf);
    assert(2 == fan_out_spice_model->lut_input_buffer->tap_buf_level);
    load_inv_size = fan_out_spice_model->lut_input_buffer->size 
                  + fan_out_spice_model->lut_input_buffer->f_per_stage;
    return load_inv_size;
  }

  /* depend on the input_buffer type */
  if (1 == fan_out_spice_model->input_buffer->exist) {
    switch(fan_out_spice_model->input_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      load_inv_size = fan_out_spice_model->input_buffer->size;
      break;
    case SPICE_MODEL_BUF_BUF:
      load_inv_size = 1.;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid fanout spice_model input_buffer type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  } else {
    /* TODO: If there is no inv/buffer at input, we should traversal until there is one 
     * However, now we just simply give a minimum sized inverter
     */
    load_inv_size = 1.;
  }
 
  return load_inv_size;
}

float find_spice_mux_testbench_rr_mux_load_inv_size(t_rr_node* load_rr_node,
                                                    int switch_index) {
  float load_inv_size = 0;
  t_spice_model* fan_out_spice_model = NULL;

  fan_out_spice_model = switch_inf[switch_index].spice_model;

  /* Check */
  assert(NULL != fan_out_spice_model);
  assert(NULL != fan_out_spice_model->input_buffer);

  /* depend on the input_buffer type */
  if (1 == fan_out_spice_model->input_buffer->exist) {
    switch(fan_out_spice_model->input_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      load_inv_size = fan_out_spice_model->input_buffer->size;
      break;
    case SPICE_MODEL_BUF_BUF:
      load_inv_size = 1;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid fanout spice_model input_buffer type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  } else {
    /* TODO: If there is no inv/buffer at input, we should traversal until there is one 
     * However, now we just simply give a minimum sized inverter
     */
    load_inv_size = 1;
  }
 
  return load_inv_size;

}

void fprint_spice_mux_testbench_pb_graph_pin_inv_loads_rec(FILE* fp, 
                                                           int grid_x, int grid_y,
                                                           t_pb_graph_pin* src_pb_graph_pin, 
                                                           t_pb* src_pb, 
                                                           char* outport_name,
                                                           boolean consider_parent_node,
                                                           t_ivec*** LL_rr_node_indices) {
  int iedge, mode_index, ipb, jpb;
  t_interconnect* cur_interc = NULL;
  char* rec_outport_name = NULL;
  t_pb* des_pb = NULL;
  int src_rr_node_index = -1;
  float load_inv_size = 0.;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(NULL != src_pb_graph_pin);
  
  if (TRUE == consider_parent_node) {
    if (NULL != src_pb_graph_pin->parent_node->pb_type->spice_model) {
      load_inv_size = find_spice_mux_testbench_pb_pin_mux_load_inv_size(src_pb_graph_pin->parent_node->pb_type->spice_model);
      fprintf(fp, "Xload_inv[%d] %s %s_out[0] gvdd_load 0 inv size=%g\n",
               testbench_load_cnt, outport_name, outport_name, load_inv_size);
      testbench_load_cnt++;
      return;
    }
  }

  /* Get the mode_index */
  if (NULL == src_pb) {
    mode_index = find_pb_type_idle_mode_index(*(src_pb_graph_pin->parent_node->pb_type)); 
  } else {
    mode_index = src_pb->mode;
  }

  /* If this pb belongs to a pb_graph_head, 
   * the src_pb_graph_pin is a OPIN, we should find the rr_node */
  if ((OUT_PORT == src_pb_graph_pin->port->type)
     &&(NULL == src_pb_graph_pin->parent_node->parent_pb_graph_node)) {
    /* Find the corresponding rr_node */
    assert(grid[grid_x][grid_y].type->pb_graph_head == src_pb_graph_pin->parent_node);
    src_rr_node_index = get_rr_node_index(grid_x, grid_y, OPIN, src_pb_graph_pin->pin_count_in_cluster, LL_rr_node_indices); 
    for (iedge = 0; iedge < rr_node[src_rr_node_index].num_edges; iedge++) {
      /* Detect its input buffers */
      load_inv_size = find_spice_mux_testbench_rr_mux_load_inv_size(&rr_node[rr_node[src_rr_node_index].edges[iedge]], rr_node[src_rr_node_index].switches[iedge]);
      /* Print an inverter */
      fprintf(fp, "Xload_inv[%d] %s load_inv[%d]_out gvdd_load 0 inv size=%g\n",
              testbench_load_cnt, outport_name, testbench_load_cnt, load_inv_size);
      testbench_load_cnt++;
    }
    return;
  }

  /* Search output edges */
  for (iedge = 0; iedge < src_pb_graph_pin->num_output_edges; iedge++) {
    check_pb_graph_edge(*(src_pb_graph_pin->output_edges[iedge])); 
    /* We care only the edges in selected mode */
    cur_interc = src_pb_graph_pin->output_edges[iedge]->interconnect;
    assert(NULL != cur_interc);
    if (mode_index == cur_interc->parent_mode_index) {
      rec_outport_name = (char*)my_malloc(sizeof(char)* (strlen(outport_name) + 5 + strlen(my_itoa(iedge)) +2 ));
      sprintf(rec_outport_name, "%s_out[%d]", outport_name, iedge);
      /* check the interc has spice_model and if it is buffered */
      assert(NULL != cur_interc->spice_model);
      if (TRUE == cur_interc->spice_model->input_buffer->exist) {
        /* Print a inverter, and we stop this branch */
        load_inv_size = find_spice_mux_testbench_pb_pin_mux_load_inv_size(cur_interc->spice_model);
        fprintf(fp, "Xload_inv[%d] %s %s gvdd_load 0 inv size=%g\n",
                testbench_load_cnt, outport_name, rec_outport_name, load_inv_size);
        testbench_load_cnt++;
      } else {
        /*
        fprintf(fp, "R%s_to_%s %s %s 0\n",
                outport_name, rec_outport_name,  
                outport_name, rec_outport_name); 
        */
        /* Go recursively */
        if (NULL == src_pb) {
          des_pb = NULL;
        } else {
          if (IN_PORT == src_pb_graph_pin->port->type) {
            ipb = src_pb_graph_pin->output_edges[iedge]->output_pins[0]->parent_node->pb_type 
                  - src_pb_graph_pin->parent_node->pb_type->modes[mode_index].pb_type_children; 
            jpb = src_pb_graph_pin->output_edges[iedge]->output_pins[0]->parent_node->placement_index; 
            if ((NULL != src_pb->child_pbs[ipb])&&(NULL != src_pb->child_pbs[ipb][jpb].name)) {
              des_pb = &(src_pb->child_pbs[ipb][jpb]);
            } else {
              des_pb = NULL;
            }
          } else if (OUT_PORT == src_pb_graph_pin->port->type) {
            des_pb = src_pb->parent_pb;
          } else if (INOUT_PORT == src_pb_graph_pin->port->type) {
            des_pb = NULL; /* I don't know what to do...*/
          }
        }
        fprint_spice_mux_testbench_pb_graph_pin_inv_loads_rec(fp, grid_x, grid_y, src_pb_graph_pin->output_edges[iedge]->output_pins[0],
                                                              des_pb, outport_name, TRUE, LL_rr_node_indices);
        
      }
    }
  }

  return;
}

static 
void fprint_spice_mux_testbench_pb_mux_meas(FILE* fp,
                                            char* meas_tag) {
  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  if (0 == testbench_pb_mux_cnt) {
    fprintf(fp, ".meas tran sum_leakage_power_pb_mux[0to%d] \n", testbench_pb_mux_cnt);
    fprintf(fp, "+          param=\'leakage_%s\'\n", meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_pb_mux[0to%d] \n", testbench_pb_mux_cnt);
    fprintf(fp, "+          param=\'energy_per_cycle_%s\'\n", meas_tag);
  } else {
    fprintf(fp, ".meas tran sum_leakage_power_pb_mux[0to%d] \n", testbench_pb_mux_cnt);
    fprintf(fp, "+          param=\'sum_leakage_power_pb_mux[0to%d]+leakage_%s\'\n", testbench_pb_mux_cnt-1, meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_pb_mux[0to%d] \n", testbench_pb_mux_cnt);
    fprintf(fp, "+          param=\'sum_energy_per_cycle_pb_mux[0to%d]+energy_per_cycle_%s\'\n", testbench_pb_mux_cnt-1, meas_tag);
  }

  /* Update the counter */
  testbench_pb_mux_cnt++;

  return;
}

static 
void fprint_spice_mux_testbench_cb_mux_meas(FILE* fp,
                                            char* meas_tag) {
  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  if (0 == testbench_cb_mux_cnt) {
    fprintf(fp, ".meas tran sum_leakage_power_cb_mux[0to%d] \n", testbench_cb_mux_cnt);
    fprintf(fp, "+          param=\'leakage_%s\'\n", meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_cb_mux[0to%d] \n", testbench_cb_mux_cnt);
    fprintf(fp, "+          param=\'energy_per_cycle_%s\'\n", meas_tag);
  } else {
    fprintf(fp, ".meas tran sum_leakage_power_cb_mux[0to%d] \n", testbench_cb_mux_cnt);
    fprintf(fp, "+          param=\'sum_leakage_power_cb_mux[0to%d]+leakage_%s\'\n", testbench_cb_mux_cnt-1, meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_cb_mux[0to%d] \n", testbench_cb_mux_cnt);
    fprintf(fp, "+          param=\'sum_energy_per_cycle_cb_mux[0to%d]+energy_per_cycle_%s\'\n", testbench_cb_mux_cnt-1, meas_tag);
  }

  /* Update the counter */
  testbench_cb_mux_cnt++;

  return;
}

static 
void fprint_spice_mux_testbench_sb_mux_meas(FILE* fp,
                                            char* meas_tag) {
  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  if (0 == testbench_sb_mux_cnt) {
    fprintf(fp, ".meas tran sum_leakage_power_sb_mux[0to%d] \n", testbench_sb_mux_cnt);
    fprintf(fp, "+          param=\'leakage_%s\'\n", meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_sb_mux[0to%d] \n", testbench_sb_mux_cnt);
    fprintf(fp, "+          param=\'energy_per_cycle_%s\'\n", meas_tag);
  } else {
    fprintf(fp, ".meas tran sum_leakage_power_sb_mux[0to%d] \n", testbench_sb_mux_cnt);
    fprintf(fp, "+          param=\'sum_leakage_power_sb_mux[0to%d]+leakage_%s\'\n", testbench_sb_mux_cnt-1, meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_sb_mux[0to%d] \n", testbench_sb_mux_cnt);
    fprintf(fp, "+          param=\'sum_energy_per_cycle_sb_mux[0to%d]+energy_per_cycle_%s\'\n", testbench_sb_mux_cnt-1, meas_tag);
  }

  /* Update the counter */
  testbench_sb_mux_cnt++;

  return;
}


static 
void fprint_spice_mux_testbench_one_mux(FILE* fp,
                                        char* meas_tag,
                                        t_spice_model* mux_spice_model,
                                        int mux_size,
                                        int* input_init_value,
                                        float* input_density,
                                        float* input_probability,
                                        int path_id) {
  int inode, mux_level, ilevel, cur_num_sram;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL; 
  t_llist* found_mux_node = NULL;
  t_spice_mux_model* cur_mux = NULL;
  int num_sim_clock_cycles = 0;
  float average_density = 0.;
  int avg_density_cnt = 0;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert(NULL != mux_spice_model);
  assert((2 < mux_size)||(2 == mux_size));
  assert(NULL != input_density);
  assert(NULL != input_probability);

  /* Add to linked list */
  check_and_add_mux_to_linked_list(&(testbench_muxes_head), mux_size, mux_spice_model);
  found_mux_node = search_mux_linked_list(testbench_muxes_head, mux_size, mux_spice_model);
  /* Check */
  assert(NULL != found_mux_node);
  cur_mux = (t_spice_mux_model*)(found_mux_node->dptr);
  assert(mux_spice_model == cur_mux->spice_model);

  /* Call the subckt that has already been defined before */
  fprintf(fp, "X%s_size%d[%d] ", mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* input port*/
  for (inode = 0; inode < mux_size; inode++) {
    fprintf(fp, "%s_size%d[%d]->in[%d] ", 
            mux_spice_model->prefix, mux_size, testbench_mux_cnt, inode);
  }
  /* Output port */
  fprintf(fp, "%s_size%d[%d]->out ", 
          mux_spice_model->prefix, mux_size, testbench_mux_cnt);

  /* SRAMs */
  /* Print SRAM configurations, 
   * we should have a global SRAM vdd, AND it should be connected to a real sram subckt !!!
   */
  /* Configuration bits for MUX*/
  assert((-1 != path_id)&&(path_id < mux_size));

  /* 1. Get the mux level*/
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
  cur_num_sram = testbench_sram_cnt;
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    /* Pull Up/Down the SRAM outputs*/
    /* TODO: I change the sram_bits gen function, so it is different.
     * I need to adapt those in spice_pbtypes.c as well !!!
     */
    /* switch (mux_sram_bits[mux_level - ilevel - 1]) { */
    switch (mux_sram_bits[ilevel]) { 
    case 0:
      /* Pull down power is considered as a part of subckt (CB or SB)*/
      fprintf(fp, "%s[%d]->out ", sram_spice_model->prefix, cur_num_sram);
      fprintf(fp, "%s[%d]->outb ", sram_spice_model->prefix, cur_num_sram);
      break;
    case 1:
      /* Pull down power is considered as a part of subckt (CB or SB)*/
      fprintf(fp, "%s[%d]->outb ", sram_spice_model->prefix, cur_num_sram);
      fprintf(fp, "%s[%d]->out ", sram_spice_model->prefix, cur_num_sram);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s,[LINE%d])Invalid sram_bit(=%d)! Should be [0|1].\n",
                 __FILE__, __LINE__, mux_sram_bits[ilevel]);
      exit(1);
    }
    cur_num_sram++;
  }
  /* End with svdd and sgnd, subckt name*/
  /* Local vdd and gnd, we should have an independent VDD for all local interconnections*/
  fprintf(fp, "gvdd_%s_size%d[%d] 0 ", mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* End with spice_model name */
  fprintf(fp, "%s_size%d\n", mux_spice_model->name, mux_size);


  /* Print the encoding in SPICE netlist for debugging */
  fprintf(fp, "***** SRAM bits for MUX[%d], level=%d, select_path_id=%d. *****\n", 
          testbench_mux_cnt, mux_level, path_id);
  fprintf(fp, "*****");
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprintf(fp, "%d", mux_sram_bits[ilevel]);
  }
  fprintf(fp, "*****\n");
  
  /* Force SRAM bits */
  /* cur_num_sram = testbench_sram_cnt; */
  /* 
  for (ilevel = 0; ilevel < mux_level; ilevel++) {
    fprintf(fp,"V%s[%d]->in %s[%d]->in 0 ", 
          sram_spice_model->prefix, cur_num_sram, sram_spice_model->prefix, cur_num_sram);
    fprintf(fp, "0\n");
    cur_num_sram++;
  }
  */

  /* Call SRAM subckts*/
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprintf(fp, "X%s[%d] ", sram_spice_model->prefix, testbench_sram_cnt);
    /* fprintf(fp, "%s[%d]->in ", sram_spice_model->prefix, testbench_sram_cnt); */
    fprintf(fp, "%s->in ", sram_spice_model->prefix);
    fprintf(fp,"%s[%d]->out %s[%d]->outb ", 
          sram_spice_model->prefix, testbench_sram_cnt, sram_spice_model->prefix, testbench_sram_cnt);
    /* Configure the SRAMs*/
    fprintf(fp, "gvdd_sram 0 %s\n", sram_spice_model->name);
    /* Add nodeset to help convergence */ 
    fprintf(fp, ".nodeset V(%s[%d]->out) 0\n", sram_spice_model->prefix, testbench_sram_cnt);
    fprintf(fp, ".nodeset V(%s[%d]->outb) vsp\n", sram_spice_model->prefix, testbench_sram_cnt);
    testbench_sram_cnt++;
  }

  /* Check SRAM counters */
  assert(cur_num_sram == testbench_sram_cnt);

  /* Test bench : Add voltage sources */
  for (inode = 0; inode < mux_size; inode++) {
    /* Print voltage source */
    fprintf(fp, "***** Signal %s_size%d[%d]->in[%d] density = %g, probability=%g.*****\n",
            mux_spice_model->prefix, mux_size, testbench_mux_cnt, inode, input_density[inode], input_probability[inode]);
    fprintf(fp, "V%s_size%d[%d]->in[%d] %s_size%d[%d]->in[%d] 0 \n", 
            mux_spice_model->prefix, mux_size, testbench_mux_cnt, inode,
            mux_spice_model->prefix, mux_size, testbench_mux_cnt, inode);
    fprint_voltage_pulse_params(fp, input_init_value[inode], input_density[inode], input_probability[inode]);
    /* fprint_voltage_pulse_params(fp, input_init_value[inode], 1, 0.5); */
  }
  /* global voltage supply */
  fprintf(fp, "Vgvdd_%s_size%d[%d] gvdd_%s_size%d[%d] 0 vsp\n",
          mux_spice_model->prefix, mux_size, testbench_mux_cnt,
          mux_spice_model->prefix, mux_size, testbench_mux_cnt);

  /* Calculate average density of this MUX */
  average_density = 0.;
  avg_density_cnt = 0;
  for (inode = 0; inode < mux_size; inode++) {
    assert(!(0 > input_density[inode]));
    if (0. < input_density[inode]) {
      average_density += input_density[inode];
      avg_density_cnt++;
    }
  }
  /* Calculate the num_sim_clock_cycle for this MUX, update global max_sim_clock_cycle in this testbench */
  if (0 < avg_density_cnt) {
    average_density = average_density/avg_density_cnt;
    num_sim_clock_cycles = (int)(1/average_density) + 1;
  } else {
    assert(0 == avg_density_cnt);
    average_density = 0.;
    num_sim_clock_cycles = 2;
  }
  if (TRUE == auto_select_max_sim_num_clock_cycles) {
    /* for idle blocks, 2 clock cycle is well enough... */
    if (2 < num_sim_clock_cycles) {
      num_sim_clock_cycles = upbound_sim_num_clock_cycles;
    } else {
      num_sim_clock_cycles = 2;
    }
    if (max_sim_num_clock_cycles < num_sim_clock_cycles) {
      max_sim_num_clock_cycles = num_sim_clock_cycles;
    }
  } else {
    num_sim_clock_cycles = max_sim_num_clock_cycles;
  }

  /* Measurements */
  /* Measure the delay of MUX */
  fprintf(fp, "***** Measurements *****\n");
  /* Rise delay */
  fprintf(fp, "***** Rise delay *****\n");
  fprintf(fp, ".meas tran delay_rise_%s trig v(%s_size%d[%d]->in[%d]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'\n", meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, path_id);
  fprintf(fp, "+          targ v(%s_size%d[%d]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'\n",
          mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* Fall delay */
  fprintf(fp, "***** Fall delay *****\n");
  fprintf(fp, ".meas tran delay_fall_%s trig v(%s_size%d[%d]->in[%d]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'\n", meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, path_id);
  fprintf(fp, "+          targ v(%s_size%d[%d]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'\n",
          mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* Measure timing period of MUX switching */
  /* Rise */
  fprintf(fp, "***** Rise timing period *****\n");
  fprintf(fp, ".meas start_rise_%s when v(%s_size%d[%d]->in[%d])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'\n",
              meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, path_id);
  fprintf(fp, ".meas tran switch_rise_%s trig v(%s_size%d[%d]->in[%d]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'\n", meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, path_id);
  fprintf(fp, "+          targ v(%s_size%d[%d]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'\n",
          mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* Fall */
  fprintf(fp, "***** Fall timing period *****\n");
  fprintf(fp, ".meas start_fall_%s when v(%s_size%d[%d]->in[%d])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'\n",
              meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, path_id);
  fprintf(fp, ".meas tran switch_fall_%s trig v(%s_size%d[%d]->in[%d]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'\n", meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, path_id);
  fprintf(fp, "+          targ v(%s_size%d[%d]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'\n",
          mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* Measure the leakage power of MUX */
  fprintf(fp, "***** Leakage Power Measurement *****\n");
  fprintf(fp, ".meas tran %s_size%d[%d]_leakage_power avg p(Vgvdd_%s_size%d[%d]) from=0 to='clock_period'\n",
          mux_spice_model->prefix, mux_size, testbench_mux_cnt,
          mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  fprintf(fp, ".meas tran leakage_%s param='%s_size%d[%d]_leakage_power'\n",
          meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* Measure the dynamic power of MUX */
  fprintf(fp, "***** Dynamic Power Measurement *****\n");
  fprintf(fp, ".meas tran %s_size%d[%d]_dynamic_power avg p(Vgvdd_%s_size%d[%d]) from='clock_period' to='%d*clock_period'\n",
           mux_spice_model->prefix, mux_size, testbench_mux_cnt,
           mux_spice_model->prefix, mux_size, testbench_mux_cnt, num_sim_clock_cycles);
  fprintf(fp, ".meas tran %s_size%d[%d]_energy_per_cycle param='%s_size%d[%d]_dynamic_power*clock_period'\n",
           mux_spice_model->prefix, mux_size, testbench_mux_cnt,
           mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  /* Important: to give dynamic power measurement per toggle !!!! 
   * it is not fair to compare dynamic power when clock_period is different from designs to designs !!!
   */
  fprintf(fp, ".meas tran dynamic_power_%s  param='%s_size%d[%d]_dynamic_power'\n",
          meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  fprintf(fp, ".meas tran energy_per_cycle_%s  param='dynamic_power_%s*clock_period'\n",
          meas_tag, meas_tag);
  fprintf(fp, ".meas tran dynamic_rise_%s avg p(Vgvdd_%s_size%d[%d]) from='start_rise_%s' to='start_rise_%s+switch_rise_%s'\n",
          meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, meas_tag, meas_tag, meas_tag);
  fprintf(fp, ".meas tran dynamic_fall_%s avg p(Vgvdd_%s_size%d[%d]) from='start_fall_%s' to='start_fall_%s+switch_fall_%s'\n",
          meas_tag, mux_spice_model->prefix, mux_size, testbench_mux_cnt, meas_tag, meas_tag, meas_tag);
  /*
  fprintf(fp, ".meas tran %s_size%d[%d]_dynamic_power param='(dynamic_rise_%s)*(%g*%d)'\n",
           mux_spice_model->prefix, mux_size, testbench_mux_cnt,
           meas_tag, input_density[path_id], sim_num_clock_cycle-1);
  fprintf(fp, ".meas tran dynamic_%s  param='(dynamic_rise_%s)*(%g*%d)'\n",
          meas_tag, meas_tag, input_density[path_id], sim_num_clock_cycle-1);
  */
  if (0 == testbench_mux_cnt) {
    fprintf(fp, ".meas tran sum_leakage_power_mux[0to%d] \n", testbench_mux_cnt);
    fprintf(fp, "+          param=\'leakage_%s\'\n", meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_mux[0to%d] \n", testbench_mux_cnt);
    fprintf(fp, "+          param=\'energy_per_cycle_%s\'\n", meas_tag);
  } else {
    fprintf(fp, ".meas tran sum_leakage_power_mux[0to%d] \n", testbench_mux_cnt);
    fprintf(fp, "+          param=\'sum_leakage_power_mux[0to%d]+leakage_%s\'\n", testbench_mux_cnt-1, meas_tag);
    fprintf(fp, ".meas tran sum_energy_per_cycle_mux[0to%d] \n", testbench_mux_cnt);
    fprintf(fp, "+          param=\'sum_energy_per_cycle_mux[0to%d]+energy_per_cycle_%s\'\n", testbench_mux_cnt-1, meas_tag);
  }

  /* Update the counter */
  cur_mux->cnt++;

  /* Free */
  my_free(mux_sram_bits);

  return;
}

/** Print a mulitplexer testbench of a given pb pin
 * --|---|
 * --| M |
 * ..| U |--- des_pb_graph_in
 * --| X |
 * --|---|
 */
static 
void fprint_spice_mux_testbench_pb_graph_node_pin_mux(FILE* fp,
                                                      t_mode* cur_mode,
                                                      t_pb_graph_pin* des_pb_graph_pin,
                                                      t_interconnect* cur_interc,
                                                      int fan_in,
                                                      int select_edge,
                                                      int grid_x, int grid_y,
                                                      t_ivec*** LL_rr_node_indices) {
  int cur_input = 0;  
  float* input_density = NULL;
  float* input_probability = NULL;
  int* input_init_value = NULL; 
  int iedge;
  int* sram_bits = NULL; 
  char* meas_tag = NULL;
  char* outport_name = NULL;
  
  float average_pb_mux_input_density = 0.;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  /* Check : 
   * MUX should have at least 2 fan_in
   */
  assert((2 == fan_in)||(2 < fan_in));
  /* 2. spice_model is a wire */ 
  assert(NULL != cur_interc->spice_model);
  assert(SPICE_MODEL_MUX == cur_interc->spice_model->type);
 
  /* Test bench : Add voltage sources */
  cur_input = 0;
  input_density = (float*)my_malloc(sizeof(float)*fan_in);
  input_probability = (float*)my_malloc(sizeof(float)*fan_in);
  input_init_value = (int*)my_malloc(sizeof(int)*fan_in);
  for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
    if (cur_mode != des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
       continue;
    }
    check_pb_graph_edge(*(des_pb_graph_pin->input_edges[iedge]));
    /* Find activity information */
    input_density[cur_input] = pb_pin_density(NULL, des_pb_graph_pin->input_edges[iedge]->input_pins[0]); 
    input_probability[cur_input] = pb_pin_probability(NULL, des_pb_graph_pin->input_edges[iedge]->input_pins[0]); 
    input_init_value[cur_input] = pb_pin_init_value(NULL, des_pb_graph_pin->input_edges[iedge]->input_pins[0]); 
    average_pb_mux_input_density += input_density[cur_input];
    cur_input++;
  }
  average_pb_mux_input_density = average_pb_mux_input_density/fan_in;
  total_pb_mux_input_density += average_pb_mux_input_density;
  /* Check fan-in number is correct */
  assert(fan_in == cur_input);

  /* Build a measurement tag: <des_pb:spice_name_tag>_<des_port>[pin_index]_<interc_name> */
  meas_tag = (char*)my_malloc(sizeof(char)* (10 + strlen(my_itoa(fan_in)) + strlen(my_itoa(testbench_mux_cnt)) + 2
                                            + strlen(des_pb_graph_pin->port->name) + 1 
                                            + strlen(my_itoa(des_pb_graph_pin->pin_number)) + 2
                                            + strlen(cur_interc->name) + 1)); /* Add '0'*/
  sprintf(meas_tag, "idle_mux%d[%d]_%s[%d]_%s", 
          fan_in, testbench_mux_cnt, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number, cur_interc->name);
  /* Print the main part of a single MUX testbench */
  fprint_spice_mux_testbench_one_mux(fp, meas_tag, cur_interc->spice_model,
                                     fan_in, input_init_value, input_density, input_probability, select_edge);

  /* Test bench : Capactive load */
  /* TODO: Search all the fan-outs of des_pb_graph_pin */
  outport_name = (char*)my_malloc(sizeof(char)*( strlen(cur_interc->spice_model->prefix) + 5 
                                  + strlen(my_itoa(fan_in)) + 1 + strlen(my_itoa(testbench_mux_cnt)) 
                                  + 7 ));
  sprintf(outport_name, "%s_size%d[%d]->out",
                        cur_interc->spice_model->prefix, fan_in, testbench_mux_cnt);
  fprint_spice_mux_testbench_pb_graph_pin_inv_loads_rec(fp, grid_x, grid_y, des_pb_graph_pin, NULL, outport_name, TRUE, LL_rr_node_indices); 

  fprint_spice_mux_testbench_pb_mux_meas(fp, meas_tag);
  /* Update the counter */
  testbench_mux_cnt++;

  /* Free */
  my_free(sram_bits);
  my_free(input_init_value);
  my_free(input_density);
  my_free(input_probability);
  my_free(outport_name);

  return;
}


/** Print a mulitplexer testbench of a given pb pin
 * --|---|
 * --| M |
 * ..| U |--- des_pb_graph_in
 * --| X |
 * --|---|
 */
static 
void fprint_spice_mux_testbench_pb_pin_mux(FILE* fp,
                                           t_rr_node* pb_rr_graph,
                                           t_pb* des_pb,
                                           t_mode* cur_mode,
                                           t_pb_graph_pin* des_pb_graph_pin,
                                           t_interconnect* cur_interc,
                                           int fan_in,
                                           int select_edge,
                                           int grid_x, int grid_y,
                                           t_ivec*** LL_rr_node_indices) {
  int cur_input = 0;  
  float* input_density = NULL;
  float* input_probability = NULL;
  int* input_init_value = NULL; 
  int iedge;
  int* sram_bits = NULL; 
  char* meas_tag = NULL;
  char* outport_name = NULL;
  float average_pb_mux_input_density = 0.;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  /* Check : 
   * MUX should have at least 2 fan_in
   */
  assert((2 == fan_in)||(2 < fan_in));
  /* 2. spice_model is a wire */ 
  assert(NULL != cur_interc->spice_model);
  assert(SPICE_MODEL_MUX == cur_interc->spice_model->type);
 
  /* Test bench : Add voltage sources */
  cur_input = 0;
  input_density = (float*)my_malloc(sizeof(float)*fan_in);
  input_probability = (float*)my_malloc(sizeof(float)*fan_in);
  input_init_value = (int*)my_malloc(sizeof(int)*fan_in);
  for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
    if (cur_mode != des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
       continue;
    }
    check_pb_graph_edge(*(des_pb_graph_pin->input_edges[iedge]));
    /* Find activity information */
    input_density[cur_input] = pb_pin_density(pb_rr_graph, des_pb_graph_pin->input_edges[iedge]->input_pins[0]); 
    input_probability[cur_input] = pb_pin_probability(pb_rr_graph, des_pb_graph_pin->input_edges[iedge]->input_pins[0]); 
    input_init_value[cur_input] = pb_pin_init_value(pb_rr_graph, des_pb_graph_pin->input_edges[iedge]->input_pins[0]); 
    average_pb_mux_input_density += input_density[cur_input];
    cur_input++;
  }
  average_pb_mux_input_density = average_pb_mux_input_density/fan_in;
  total_pb_mux_input_density += average_pb_mux_input_density;
  /* Check fan-in number is correct */
  assert(fan_in == cur_input);

  /* Build a measurement tag: <des_pb:spice_name_tag>_<des_port>[pin_index]_<interc_name> */
  meas_tag = (char*)my_malloc(sizeof(char)* (strlen(des_pb->spice_name_tag) + 1
                                            + strlen(des_pb_graph_pin->port->name) + 1 
                                            + strlen(my_itoa(des_pb_graph_pin->pin_number)) + 2
                                            + strlen(cur_interc->name) + 1)); /* Add '0'*/
  sprintf(meas_tag, "%s_%s[%d]_%s", 
          des_pb->spice_name_tag, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number, cur_interc->name);
  /* Print the main part of a single MUX testbench */
  fprint_spice_mux_testbench_one_mux(fp, meas_tag, cur_interc->spice_model,
                                     fan_in, input_init_value, input_density, input_probability, select_edge);

  /* Test bench : Capactive load */
  /* Search all the fan-outs of des_pb_graph_pin */
  outport_name = (char*)my_malloc(sizeof(char)*( strlen(cur_interc->spice_model->prefix) + 5 
                                  + strlen(my_itoa(fan_in)) + 1 + strlen(my_itoa(testbench_mux_cnt)) 
                                  + 7 ));
  sprintf(outport_name, "%s_size%d[%d]->out",
                        cur_interc->spice_model->prefix, fan_in, testbench_mux_cnt);
  fprint_spice_mux_testbench_pb_graph_pin_inv_loads_rec(fp, grid_x, grid_y,
                                                        des_pb_graph_pin, des_pb, outport_name, TRUE, LL_rr_node_indices);


  fprint_spice_mux_testbench_pb_mux_meas(fp, meas_tag);

  /* Update the counter */
  testbench_mux_cnt++;

  /* Free */
  my_free(sram_bits);
  my_free(input_init_value);
  my_free(input_density);
  my_free(input_probability);
  my_free(outport_name);

  return;
}

static 
void fprint_spice_mux_testbench_pb_graph_node_pin_interc(FILE* fp,
                                                         enum e_pin2pin_interc_type pin2pin_interc_type,
                                                         t_pb_graph_pin* des_pb_graph_pin,
                                                         t_mode* cur_mode,
                                                         int select_path_id,
                                                         int grid_x, int grid_y,
                                                         t_ivec*** LL_rr_node_indices) {
  int fan_in;
  int iedge;
  t_interconnect* cur_interc = NULL;
  enum e_interconnect spice_interc_type;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Select and print the SPICE netlist
   */
  fan_in = 0;
  cur_interc = NULL;
  /* Search the input edges only, stats on the size of MUX we may need (fan-in) */
  for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
    /* 1. First, we should make sure this interconnect is in the selected mode!!!*/
    if (cur_mode == des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
      /* Check this edge*/
      check_pb_graph_edge(*(des_pb_graph_pin->input_edges[iedge]));
      /* Record the interconnection*/
      if (NULL == cur_interc) {
        cur_interc = des_pb_graph_pin->input_edges[iedge]->interconnect;
      } else { /* Make sure the interconnections for this pin is the same!*/
        assert(cur_interc == des_pb_graph_pin->input_edges[iedge]->interconnect);
      }
      /* Search the input_pins of input_edges only*/
      fan_in += des_pb_graph_pin->input_edges[iedge]->num_input_pins;
    }
  }
  if (NULL == cur_interc) { 
    /* No interconnection matched */
    return;
  }
  /* Initialize the interconnection type that will be implemented in SPICE netlist*/
  switch (cur_interc->type) {
    case DIRECT_INTERC:
      assert(1 == fan_in);
      spice_interc_type = DIRECT_INTERC;
      break;
    case COMPLETE_INTERC:
      if (1 == fan_in) {
        spice_interc_type = DIRECT_INTERC;
      } else {
        assert((2 == fan_in)||(2 < fan_in));
        spice_interc_type = MUX_INTERC;
      }
      break;
    case MUX_INTERC:
      assert((2 == fan_in)||(2 < fan_in));
      spice_interc_type = MUX_INTERC;
      break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }

  /* Print all the multiplexers at current level */
  switch (spice_interc_type) {
  case DIRECT_INTERC:
    break;
  case MUX_INTERC:
    assert(SPICE_MODEL_MUX == cur_interc->spice_model->type);
    fprint_spice_mux_testbench_pb_graph_node_pin_mux(fp, cur_mode, des_pb_graph_pin, 
                                                     cur_interc, fan_in, select_path_id, grid_x, grid_y, LL_rr_node_indices); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid interconnection type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}


static 
void fprint_spice_mux_testbench_pb_pin_interc(FILE* fp,
                                              t_rr_node* pb_rr_graph,
                                              t_pb* des_pb,
                                              enum e_pin2pin_interc_type pin2pin_interc_type,
                                              t_pb_graph_pin* des_pb_graph_pin,
                                              t_mode* cur_mode,
                                              int select_path_id,
                                              int grid_x, int grid_y,
                                              t_ivec*** LL_rr_node_indices) {
  int fan_in;
  int iedge;
  t_interconnect* cur_interc = NULL;
  enum e_interconnect spice_interc_type;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Select and print the SPICE netlist
   */
  fan_in = 0;
  cur_interc = NULL;
  /* Search the input edges only, stats on the size of MUX we may need (fan-in) */
  for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
    /* 1. First, we should make sure this interconnect is in the selected mode!!!*/
    if (cur_mode == des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
      /* Check this edge*/
      check_pb_graph_edge(*(des_pb_graph_pin->input_edges[iedge]));
      /* Record the interconnection*/
      if (NULL == cur_interc) {
        cur_interc = des_pb_graph_pin->input_edges[iedge]->interconnect;
      } else { /* Make sure the interconnections for this pin is the same!*/
        assert(cur_interc == des_pb_graph_pin->input_edges[iedge]->interconnect);
      }
      /* Search the input_pins of input_edges only*/
      fan_in += des_pb_graph_pin->input_edges[iedge]->num_input_pins;
    }
  }
  if (NULL == cur_interc) { 
    /* No interconnection matched */
    return;
  }
  /* Initialize the interconnection type that will be implemented in SPICE netlist*/
  switch (cur_interc->type) {
    case DIRECT_INTERC:
      assert(1 == fan_in);
      spice_interc_type = DIRECT_INTERC;
      break;
    case COMPLETE_INTERC:
      if (1 == fan_in) {
        spice_interc_type = DIRECT_INTERC;
      } else {
        assert((2 == fan_in)||(2 < fan_in));
        spice_interc_type = MUX_INTERC;
      }
      break;
    case MUX_INTERC:
      assert((2 == fan_in)||(2 < fan_in));
      spice_interc_type = MUX_INTERC;
      break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }

  /* Print all the multiplexers at current level */
  switch (spice_interc_type) {
  case DIRECT_INTERC:
    break;
  case MUX_INTERC:
    assert(SPICE_MODEL_MUX == cur_interc->spice_model->type);
    fprint_spice_mux_testbench_pb_pin_mux(fp, pb_rr_graph, des_pb, cur_mode, des_pb_graph_pin, 
                                          cur_interc, fan_in, select_path_id, grid_x, grid_y, LL_rr_node_indices); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid interconnection type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* For each pb, we search the input pins and output pins for local interconnections */
static 
void fprint_spice_mux_testbench_pb_graph_node_interc(FILE* fp,
                                                     t_pb_graph_node* cur_pb_graph_node,
                                                     int grid_x, int grid_y,
                                                     t_ivec*** LL_rr_node_indices) {
  int iport, ipin;
  int ipb, jpb;
  t_pb_type* cur_pb_type = NULL;
  t_mode* cur_mode = NULL;
  t_pb_graph_node* child_pb_graph_node = NULL;
  int select_mode_index = -1;

  int path_id = -1;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  
  assert(NULL != cur_pb_graph_node);
  cur_pb_type = cur_pb_graph_node->pb_type;
  assert(NULL != cur_pb_type);
  select_mode_index = find_pb_type_idle_mode_index(*(cur_pb_type));
  cur_mode = &(cur_pb_type->modes[select_mode_index]);
  assert(NULL != cur_mode);

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      path_id = 0; 
      fprint_spice_mux_testbench_pb_graph_node_pin_interc(fp, 
                                                          OUTPUT2OUTPUT_INTERC,
                                                          &(cur_pb_graph_node->output_pins[iport][ipin]),
                                                          cur_mode,
                                                          path_id, grid_x, grid_y, LL_rr_node_indices);
    }
  }
  
  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (ipb = 0; ipb < cur_pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      child_pb_graph_node = &(cur_pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]);
      /* For each child_pb_graph_node input pins*/
      for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
          path_id = 0;
          /* Write the interconnection*/
          fprint_spice_mux_testbench_pb_graph_node_pin_interc(fp, 
                                                              INPUT2INPUT_INTERC,
                                                              &(child_pb_graph_node->input_pins[iport][ipin]),
                                                              cur_mode,
                                                              path_id,
                                                              grid_x, grid_y, LL_rr_node_indices);
        }
      }
      /* TODO: for clock pins, we should do the same work */
      for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
          path_id = 0;
          /* Write the interconnection*/
          fprint_spice_mux_testbench_pb_graph_node_pin_interc(fp, 
                                                              INPUT2INPUT_INTERC,
                                                              &(child_pb_graph_node->clock_pins[iport][ipin]),
                                                              cur_mode,
                                                              path_id,
                                                              grid_x, grid_y, LL_rr_node_indices);
        }
      }
    }
  }

  return; 
}

void fprint_spice_mux_testbench_idle_pb_graph_node_muxes_rec(FILE* fp, 
                                                             t_pb_graph_node* cur_pb_graph_node,
                                                             int grid_x, int grid_y,
                                                             t_ivec*** LL_rr_node_indices) {
  int ipb, jpb;
  int mode_index;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert(NULL != cur_pb_graph_node);

  /* If we touch the leaf, there is no need print interc*/
  if (NULL == cur_pb_graph_node->pb_type->spice_model) {
    /* Print MUX interc at current-level pb*/
    fprint_spice_mux_testbench_pb_graph_node_interc(fp, cur_pb_graph_node, grid_x, grid_y, LL_rr_node_indices);
  } else {
    return;
  }
  
  /* Go recursively ... */
  mode_index = find_pb_type_idle_mode_index(*(cur_pb_graph_node->pb_type));
  for (ipb = 0; ipb < cur_pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Print idle muxes */
      fprint_spice_mux_testbench_idle_pb_graph_node_muxes_rec(fp, &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]), grid_x, grid_y, LL_rr_node_indices);
    }
  }
  
  return;
}

/* For each pb, we search the input pins and output pins for local interconnections */
static 
void fprint_spice_mux_testbench_pb_interc(FILE* fp,
                                          t_pb* cur_pb,
                                          int grid_x, int grid_y,
                                          t_ivec*** LL_rr_node_indices) {
  int iport, ipin;
  int ipb, jpb;
  t_pb_graph_node* cur_pb_graph_node = NULL;
  t_pb_type* cur_pb_type = NULL;
  t_mode* cur_mode = NULL;
  t_pb_graph_node* child_pb_graph_node = NULL;
  t_pb* child_pb = NULL;
  int select_mode_index = -1;

  int node_index = -1;
  int prev_node = -1;
  int prev_edge = -1;
  int path_id = -1;
  t_rr_node* pb_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  
  assert(NULL != cur_pb);
  cur_pb_graph_node = cur_pb->pb_graph_node;
  assert(NULL != cur_pb_graph_node);
  cur_pb_type = cur_pb_graph_node->pb_type;
  assert(NULL != cur_pb_type);
  select_mode_index = cur_pb->mode;
  cur_mode = &(cur_pb_type->modes[select_mode_index]);
  assert(NULL != cur_mode);

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      /* Get the selected edge of current pin*/
      assert(NULL != cur_pb);
      pb_rr_nodes = cur_pb->rr_graph;
      node_index = cur_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      /* Bypass unmapped interc */
      /* 
      if (OPEN == pb_rr_nodes[node_index].net_num) {
        continue;
      }
      */ 
      prev_node = pb_rr_nodes[node_index].prev_node;
      /* prev_edge is the index of edge of prev_node !!! */
      prev_edge = pb_rr_nodes[node_index].prev_edge;
      /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
      if (OPEN == prev_node) {
        path_id = 0; 
        /* Determine the child_pb */
      } else {
        /* Find the path_id */
        path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
        assert(-1 != path_id);
      } 
      fprint_spice_mux_testbench_pb_pin_interc(fp, pb_rr_nodes ,cur_pb, /* TODO: find out the child_pb*/
                                               OUTPUT2OUTPUT_INTERC,
                                               &(cur_pb_graph_node->output_pins[iport][ipin]),
                                               cur_mode,
                                               path_id, 
                                               grid_x, grid_y, LL_rr_node_indices);
    }
  }
  
  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (ipb = 0; ipb < cur_pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      child_pb_graph_node = &(cur_pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]);
      child_pb = &(cur_pb->child_pbs[ipb][jpb]);
      /* Check if child_pb is empty */
      if (NULL == child_pb->name) { 
        //continue; /* by pass*/
        /* For each child_pb_graph_node input pins*/
        for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
          for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
            path_id = 0;
            /* Write the interconnection*/
            fprint_spice_mux_testbench_pb_graph_node_pin_interc(fp, 
                                                                INPUT2INPUT_INTERC,
                                                                &(child_pb_graph_node->input_pins[iport][ipin]),
                                                                cur_mode,
                                                                path_id,
                                                                grid_x, grid_y, LL_rr_node_indices);
          }
        }
        /* TODO: for clock pins, we should do the same work */
        for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
          for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
            path_id = 0;
            /* Write the interconnection*/
            fprint_spice_mux_testbench_pb_graph_node_pin_interc(fp, 
                                                                INPUT2INPUT_INTERC,
                                                                &(child_pb_graph_node->clock_pins[iport][ipin]),
                                                                cur_mode,
                                                                path_id,
                                                                grid_x, grid_y, LL_rr_node_indices);
          }  
        }
        continue;
      }
      /* Get pb_rr_graph of current pb*/
      pb_rr_nodes = child_pb->rr_graph;
      /* For each child_pb_graph_node input pins*/
      for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
          /* Get the index of the edge that are selected to pass signal*/
          node_index = child_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
          prev_node = pb_rr_nodes[node_index].prev_node;
          prev_edge = pb_rr_nodes[node_index].prev_edge;
          /* Bypass unmapped interc */
          /* 
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            continue;
          } 
          */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = 0;
            //break; /* TODO: if there exist parasitic input waveforms, we should print the interc */
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            assert(-1 != path_id);
          }
          /* Write the interconnection*/
          fprint_spice_mux_testbench_pb_pin_interc(fp, pb_rr_nodes, child_pb,
                                                   INPUT2INPUT_INTERC,
                                                   &(child_pb_graph_node->input_pins[iport][ipin]),
                                                   cur_mode,
                                                   path_id,
                                                   grid_x, grid_y, LL_rr_node_indices);
        }
      }
      /* TODO: for clock pins, we should do the same work */
      for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
          /* Get the index of the edge that are selected to pass signal*/
          node_index = child_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
          prev_node = pb_rr_nodes[node_index].prev_node;
          prev_edge = pb_rr_nodes[node_index].prev_edge;
          /* Bypass unmapped interc */
          /* 
          if (OPEN == pb_rr_nodes[node_index].net_num) {
            continue;
          }
          */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = 0;
            //break; /* TODO: if there exist parasitic input waveforms, we should print the interc */
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            assert(-1 != path_id);
          }
          /* Write the interconnection*/
          fprint_spice_mux_testbench_pb_pin_interc(fp, pb_rr_nodes, child_pb,
                                                   INPUT2INPUT_INTERC,
                                                   &(child_pb_graph_node->clock_pins[iport][ipin]),
                                                   cur_mode,
                                                   path_id,
                                                   grid_x, grid_y, LL_rr_node_indices);
        }
      }
    }
  }

  return; 
}

static 
void fprint_spice_mux_testbench_pb_muxes_rec(FILE* fp,
                                             t_pb* cur_pb,
                                             int grid_x, int grid_y,
                                             t_ivec*** LL_rr_node_indices) {
  int ipb, jpb;
  int mode_index;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert(NULL != cur_pb);

  /* If we touch the leaf, there is no need print interc*/
  if (OPEN == cur_pb->logical_block) {
    /* Print MUX interc at current-level pb*/
    fprint_spice_mux_testbench_pb_interc(fp, cur_pb, grid_x, grid_y, LL_rr_node_indices);
  } else {
    return;
  }
  
  /* Go recursively ... */
  mode_index = cur_pb->mode;
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        fprint_spice_mux_testbench_pb_muxes_rec(fp, &(cur_pb->child_pbs[ipb][jpb]), grid_x, grid_y, LL_rr_node_indices);
      } else {
        /* Print idle muxes */
        /* Bypass idle muxes */
        fprint_spice_mux_testbench_idle_pb_graph_node_muxes_rec(fp, cur_pb->child_pbs[ipb][jpb].pb_graph_node, grid_x, grid_y, LL_rr_node_indices);
      }
    }
  }
  
  return;
}

static 
void fprint_spice_mux_testbench_one_cb_mux_loads(FILE* fp,
                                                 t_rr_node* src_rr_node,
                                                 char* outport_name,
                                                 t_ivec*** LL_rr_node_indices) {
  t_type_ptr cb_out_grid_type = NULL;
  t_pb_graph_pin* cb_out_pb_graph_pin = NULL;              
  t_pb* cb_out_pb = NULL;

  assert(IPIN == src_rr_node->type);
  assert(src_rr_node->xlow == src_rr_node->xhigh);
  assert(src_rr_node->ylow == src_rr_node->yhigh);

  cb_out_grid_type = grid[src_rr_node->xlow][src_rr_node->ylow].type; 
  assert(NULL != cb_out_grid_type);

  cb_out_pb_graph_pin = src_rr_node->pb_graph_pin;
  assert(NULL != cb_out_pb_graph_pin);
   
  /* Get the pb ! Get the mode_index */
  cb_out_pb = src_rr_node->pb;

  if (IO_TYPE == cb_out_grid_type) {
    fprintf(fp, "******* IO_TYPE loads *******\n");
  } else {
    fprintf(fp, "******* Normal TYPE loads *******\n");
  }

  /* Recursively find all the inv load inside pb_graph_node */
  fprint_spice_mux_testbench_pb_graph_pin_inv_loads_rec(fp, src_rr_node->xlow, src_rr_node->ylow, cb_out_pb_graph_pin, cb_out_pb, outport_name, TRUE, LL_rr_node_indices);
  

  fprintf(fp, "******* END loads *******\n");
  return;
}

static 
void fprint_spice_mux_testbench_cb_one_mux(FILE* fp,
                                           t_rr_type chan_type,
                                           int cb_x,
                                           int cb_y,
                                           t_rr_node* src_rr_node,
                                           t_ivec*** LL_rr_node_indices) {
  int mux_size;
  int inode, path_id, switch_index;
  t_rr_node** drive_rr_nodes = NULL;
  t_spice_model* mux_spice_model = NULL;
  int* mux_sram_bits = NULL;
  float* input_density = NULL;
  float* input_probability = NULL;
  int* input_init_value = NULL;
  char* meas_tag = NULL;
  char* outport_name = NULL;
  float average_cb_mux_input_density = 0.;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > cb_x))&&(!(cb_x > (nx + 1)))); 
  assert((!(0 > cb_y))&&(!(cb_y > (ny + 1)))); 

  assert(IPIN == src_rr_node->type);

  /* Find drive_rr_nodes*/
  mux_size = src_rr_node->num_drive_rr_nodes;
  assert(mux_size == src_rr_node->fan_in);
  drive_rr_nodes = src_rr_node->drive_rr_nodes; 

  /* Find path_id */
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

  /* Check : 
   * MUX should have at least 2 fan_in
   */
  assert((2 == mux_size)||(2 < mux_size));
  /* 2. spice_model is a wire */ 
  assert(SPICE_MODEL_MUX == mux_spice_model->type);

  input_density = (float*)my_malloc(sizeof(float)*mux_size);
  input_probability = (float*)my_malloc(sizeof(float)*mux_size);
  input_init_value = (int*)my_malloc(sizeof(int)*mux_size);
  for (inode = 0; inode < mux_size; inode++) {
    /* Find activity information */
    input_density[inode] = get_rr_node_net_density(*(drive_rr_nodes[inode]));
    input_probability[inode] = get_rr_node_net_probability(*(drive_rr_nodes[inode]));
    input_init_value[inode] = get_rr_node_net_init_value(*(drive_rr_nodes[inode]));
    average_cb_mux_input_density += input_density[inode];
  }
  average_cb_mux_input_density = average_cb_mux_input_density/mux_size;
  total_cb_mux_input_density += average_cb_mux_input_density;

  /* Build meas_tag: cb_mux[cb_x][cb_y]_rrnode[node]*/
  meas_tag = (char*)my_malloc(sizeof(char)*(7 + strlen(my_itoa(cb_x)) + 2
                                            + strlen(my_itoa(cb_y)) + 9 
                                            + strlen(my_itoa(src_rr_node-rr_node)) + 2)); /* Add '0'*/
  sprintf(meas_tag, "cb_mux[%d][%d]_rrnode[%ld]", cb_x, cb_y, src_rr_node-rr_node);
  /* Print the main part of a single MUX testbench */
  fprint_spice_mux_testbench_one_mux(fp, meas_tag, mux_spice_model, src_rr_node->fan_in, 
                                     input_init_value, input_density, input_probability, path_id);

  /* Generate loads */
  outport_name = (char*)my_malloc(sizeof(char)*( strlen(mux_spice_model->prefix) + 5 
                                  + strlen(my_itoa(mux_size)) + 1 + strlen(my_itoa(testbench_mux_cnt)) 
                                  + 7 ));
  sprintf(outport_name, "%s_size%d[%d]->out",
                        mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  fprint_spice_mux_testbench_one_cb_mux_loads(fp, src_rr_node, outport_name, LL_rr_node_indices);

  fprint_spice_mux_testbench_cb_mux_meas(fp, meas_tag);
  /* Update the counter */
  testbench_mux_cnt++;

  /* Free */
  my_free(mux_sram_bits);
  my_free(input_init_value);
  my_free(input_density);
  my_free(input_probability);

  return;
}

void fprint_spice_mux_testbench_cb_interc(FILE* fp, 
                                          t_rr_type chan_type,
                                          int cb_x, int cb_y,
                                          t_rr_node* src_rr_node,
                                          t_ivec*** LL_rr_node_indices) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cb_x))&&(!(cb_x > (nx + 1)))); 
  assert((!(0 > cb_y))&&(!(cb_y > (ny + 1)))); 

  assert(NULL != src_rr_node);

  /* Skip non-mapped CB MUX */
  if (OPEN == src_rr_node->net_num) {
    //return;
  }

  assert((0 < src_rr_node->fan_in)||(0 == src_rr_node->fan_in));
  if (1 == src_rr_node->fan_in) {
    /* By-pass a direct connection*/
    return;
  } else if ((2 < src_rr_node->fan_in)||(2 == src_rr_node->fan_in)) {
    /* Print a MUX */
    fprint_spice_mux_testbench_cb_one_mux(fp, chan_type, cb_x, cb_y, src_rr_node, LL_rr_node_indices);
  } 
   
  return;
}

/* Assume each connection box has a regional power-on/off switch */
static 
int fprint_spice_mux_testbench_one_grid_cb_muxes(FILE* fp, 
                                                 t_rr_type chan_type,
                                                 int x, int y,
                                                 t_ivec*** LL_rr_node_indices) {
  int inode, side;
  int side_cnt = 0;
  int num_ipin_rr_node = 0;
  t_rr_node** ipin_rr_nodes = NULL;
  int num_temp_rr_node = 0;
  t_rr_node** temp_rr_nodes = NULL;
  int used = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  /* Print the ports of grids*/
  side_cnt = 0;
  num_ipin_rr_node = 0;  
  for (side = 0; side < 4; side++) {
    switch (side) {
    case 0: /* TOP */
      switch(chan_type) { 
      case CHANX:
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
  }
  /* Check */
  assert(2 == side_cnt);

  /* Print multiplexers */
  /* Check if there is at least one rr_node with a net_name*/
  used = 0;
  for (inode = 0; inode < num_ipin_rr_node; inode++) {
    if (OPEN != ipin_rr_nodes[inode]->net_num) {
      used = 1;
      break;
    }
  }
  
  //if (1 == used) {
    for (inode = 0; inode < num_ipin_rr_node; inode++) {
      fprint_spice_mux_testbench_cb_interc(fp, chan_type, x, y, ipin_rr_nodes[inode], LL_rr_node_indices);
    } 
  //}

  /* Free */
  my_free(ipin_rr_nodes);

  return used;
}

static 
char* fprint_spice_mux_testbench_rr_node_load_version(FILE* fp,
                                                      int load_level,
                                                      t_rr_node cur_rr_node, 
                                                      char* outport_name) {
  char* ret_outport_name = NULL;
  char* mid_outport_name = NULL;
  int cost_index;
  int iseg, i, iedge, chan_wire_length, cur_x, cur_y;
  t_rr_node to_node;
  t_spice_model* wire_spice_model = NULL;
  float load_inv_size = 0.;

  /* We only process CHANX or CHANY*/
  if (!((CHANX == cur_rr_node.type)
    ||(CHANY == cur_rr_node.type))) {
    ret_outport_name = my_strdup(outport_name);
    return ret_outport_name;
  }

  cost_index = cur_rr_node.cost_index;
  iseg = rr_indexed_data[cost_index].seg_index;
  assert((!(iseg < 0))&&(iseg < num_segments));
  wire_spice_model = segments[iseg].spice_model;
  assert(SPICE_MODEL_CHAN_WIRE == wire_spice_model->type);
  chan_wire_length = cur_rr_node.xhigh - cur_rr_node.xlow 
                   + cur_rr_node.yhigh - cur_rr_node.ylow;
  assert((0 == cur_rr_node.xhigh - cur_rr_node.xlow)
        ||(0 == cur_rr_node.yhigh - cur_rr_node.ylow));
  for (i = 0; i < chan_wire_length + 1; i++) { 
    ret_outport_name = (char*)my_malloc(sizeof(char)*( strlen(outport_name)
                       + 9 + strlen(my_itoa(load_level + i)) + 6
                       + 1 ));
    sprintf(ret_outport_name,"%s_loadlvl[%d]_out",
            outport_name, load_level + i);
    mid_outport_name = (char*)my_malloc(sizeof(char)*( strlen(outport_name)
                       + 9 + strlen(my_itoa(load_level + i)) + 9
                       + 1 ));
    sprintf(mid_outport_name,"%s_loadlvl[%d]_midout",
            outport_name, load_level + i);
    if (0 == i) {
      fprintf(fp, "Xchan_%s %s %s %s gvdd_load 0 %s_seg%d\n",
              ret_outport_name, outport_name, ret_outport_name, mid_outport_name, 
              wire_spice_model->name, iseg); 
    } else {
      fprintf(fp, "Xchan_%s %s_loadlvl[%d]_out %s %s gvdd_load 0 %s_seg%d\n",
              ret_outport_name, outport_name, load_level + i -1, ret_outport_name, mid_outport_name,
              wire_spice_model->name, iseg); 
    }
    /* Print CB inv loads connected to the mid_out */
    switch (cur_rr_node.type) {
    case CHANX:
      /* Update the cur_x & cur_y */
      if (INC_DIRECTION == cur_rr_node.direction) { 
        cur_x = cur_rr_node.xlow + i;
        cur_y = cur_rr_node.ylow;
      } else {
        assert(DEC_DIRECTION == cur_rr_node.direction);
        cur_x = cur_rr_node.xhigh - i;
        cur_y = cur_rr_node.ylow;
      }
      for (iedge = 0; iedge < cur_rr_node.num_edges; iedge++) {
        /*Identify if the des node is a IPIN and fit the current(x,y)*/
        to_node = rr_node[cur_rr_node.edges[iedge]]; 
        switch (to_node.type) {
        case IPIN:
          assert(to_node.xhigh == to_node.xlow);
          assert(to_node.yhigh == to_node.ylow);
          if (((cur_x == to_node.xlow)&&(cur_y == to_node.ylow))
             ||((cur_x == to_node.xlow)&&((cur_y + 1) == to_node.ylow))) {
            /* We find the CB! */
            /* Detect its input buffers */
            load_inv_size = find_spice_mux_testbench_rr_mux_load_inv_size(&to_node, cur_rr_node.switches[iedge]);
            assert(0. < load_inv_size);
            /* Print an inverter */
            fprintf(fp, "Xload_inv[%d] %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                    testbench_load_cnt, mid_outport_name, mid_outport_name, iedge,
                    load_inv_size);
            testbench_load_cnt++;
          }
          break;
        case CHANX:
        case CHANY:
          /* We find the CB! */
          /* Detect its input buffers */
          load_inv_size = find_spice_mux_testbench_rr_mux_load_inv_size(&to_node, cur_rr_node.switches[iedge]);
          assert(0. < load_inv_size);
          /* Print an inverter */
          fprintf(fp, "Xload_inv[%d] %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                  testbench_load_cnt, ret_outport_name, ret_outport_name, iedge,
                  load_inv_size);
          testbench_load_cnt++;
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid src_rr_node_type!\n",
                     __FILE__, __LINE__);
          exit(1);
        }
      }
      break;
    case CHANY:
      /* Update the cur_x & cur_y */
      if (INC_DIRECTION == cur_rr_node.direction) { 
        cur_x = cur_rr_node.xlow;
        cur_y = cur_rr_node.ylow + i;
      } else {
        assert(DEC_DIRECTION == cur_rr_node.direction);
        cur_x = cur_rr_node.xlow;
        cur_y = cur_rr_node.yhigh - i;
      }
      for (iedge = 0; iedge < cur_rr_node.num_edges; iedge++) {
        /*Identify if the des node is a IPIN and fit the current(x,y)*/
        to_node = rr_node[cur_rr_node.edges[iedge]]; 
        switch (to_node.type) {
        case IPIN:
          assert(to_node.xhigh == to_node.xlow);
          assert(to_node.yhigh == to_node.ylow);
          if (((cur_y == to_node.ylow)&&(cur_x == to_node.xlow))
             ||((cur_y == to_node.xlow)&&((cur_x + 1) == to_node.xlow))) {
            /* We find the CB! */
            /* Detect its input buffers */
            load_inv_size = find_spice_mux_testbench_rr_mux_load_inv_size(&to_node, cur_rr_node.switches[iedge]);
            assert(0. < load_inv_size);
            /* Print an inverter */
            fprintf(fp, "Xload_inv[%d] %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                    testbench_load_cnt, mid_outport_name, mid_outport_name, iedge,
                    load_inv_size);
            testbench_load_cnt++;
          }
          break;
        case CHANX:
        case CHANY:
          /* We find the CB! */
          /* Detect its input buffers */
          load_inv_size = find_spice_mux_testbench_rr_mux_load_inv_size(&to_node, cur_rr_node.switches[iedge]);
          assert(0. < load_inv_size);
          /* Print an inverter */
          fprintf(fp, "Xload_inv[%d] %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                  testbench_load_cnt, ret_outport_name, ret_outport_name, iedge,
                  load_inv_size);
          testbench_load_cnt++;
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid src_rr_node_type!\n",
                     __FILE__, __LINE__);
          exit(1);
        }
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid src_rr_node_type!\n",
                 __FILE__, __LINE__);
      exit(1);
    } 
  }

  return ret_outport_name;
} 

static 
int fprint_spice_mux_testbench_sb_one_mux(FILE* fp,
                                          int switch_box_x, int switch_box_y,
                                          int chan_side,
                                          t_rr_node* src_rr_node) {
  int inode, switch_index, mux_size;
  t_spice_model* mux_spice_model = NULL;
  float* input_density = NULL;
  float* input_probability = NULL;
  int* input_init_value = NULL;
  int path_id = -1;
  char* meas_tag = NULL;
  int num_drive_rr_nodes = 0;  
  t_rr_node** drive_rr_nodes = NULL;
  char* outport_name = NULL;
  char* rr_node_outport_name = NULL;
  int used = 0;

  float average_sb_mux_input_density = 0.;

  /* Check */
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 

  if (NULL == src_rr_node) {
    return 0;
  }

  /* ignore idle sb mux */
  if (OPEN == src_rr_node->net_num) {
    //return used;
  }
  /*
  find_drive_rr_nodes_switch_box(switch_box_x, switch_box_y, src_rr_node, chan_side, 0, 
                                 &num_drive_rr_nodes, &drive_rr_nodes, &switch_index);
  */
  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (1 == is_sb_interc_between_segments(switch_box_x, switch_box_y, src_rr_node, chan_side)) {
    num_drive_rr_nodes = 0;
    drive_rr_nodes = NULL;
  } else {
    num_drive_rr_nodes = src_rr_node->num_drive_rr_nodes;
    drive_rr_nodes = src_rr_node->drive_rr_nodes;
    switch_index = src_rr_node->drive_switches[0];
  }

  /* Print MUX only when fan-in >= 2 */
  if (2 > num_drive_rr_nodes) {
    return used;
  }

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }


  /* Find mux_spice_model, mux_size */
  mux_size = num_drive_rr_nodes;

  mux_spice_model = switch_inf[switch_index].spice_model;
  assert(NULL != mux_spice_model);
  assert(SPICE_MODEL_MUX == mux_spice_model->type);

  /* input_density, input_probability */
  input_density = (float*)my_malloc(sizeof(float)*mux_size);
  input_probability = (float*)my_malloc(sizeof(float)*mux_size);
  input_init_value = (int*)my_malloc(sizeof(int)*mux_size);
  for (inode = 0; inode < mux_size; inode++) {
    input_density[inode] = get_rr_node_net_density(*(drive_rr_nodes[inode]));
    input_probability[inode] = get_rr_node_net_probability(*(drive_rr_nodes[inode]));
    input_init_value[inode] = get_rr_node_net_init_value(*(drive_rr_nodes[inode]));
    average_sb_mux_input_density += input_density[inode];
  }
  average_sb_mux_input_density = average_sb_mux_input_density/mux_size;
  total_sb_mux_input_density += average_sb_mux_input_density;

  /* Find path_id */
  path_id = -1;
  for (inode = 0; inode < mux_size; inode++) {
    if (drive_rr_nodes[inode] == &(rr_node[src_rr_node->prev_node])) { 
      path_id = inode;
      break;
    } 
  }
  assert((-1 != path_id)&&(path_id < mux_size));  

  /* Build meas_tag: sb_mux[sb_x][sb_y]_rrnode[node]*/
  meas_tag = (char*)my_malloc(sizeof(char)*(7 + strlen(my_itoa(switch_box_x)) + 2
                                            + strlen(my_itoa(switch_box_y)) + 9 
                                            + strlen(my_itoa(src_rr_node-rr_node)) + 2)); /* Add '0'*/
  sprintf(meas_tag, "sb_mux[%d][%d]_rrnode[%ld]", switch_box_x, switch_box_y, src_rr_node-rr_node);
  /* Print MUX */
  fprint_spice_mux_testbench_one_mux(fp, meas_tag, mux_spice_model, mux_size,
                                     input_init_value, input_density, input_probability, path_id);

  /* Print a channel wire !*/
  outport_name = (char*)my_malloc(sizeof(char)*( strlen(mux_spice_model->prefix)
                                 + 5 + strlen(my_itoa(mux_size)) + 1 
                                 + strlen(my_itoa(testbench_mux_cnt))
                                 + 6 + 1 ));
  sprintf(outport_name, "%s_size%d[%d]->out", mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  rr_node_outport_name = fprint_spice_mux_testbench_rr_node_load_version(fp, 0, (*src_rr_node), outport_name); 

  fprint_spice_mux_testbench_sb_mux_meas(fp, meas_tag);

  /* Update the counter */
  testbench_mux_cnt++;

  /* Free */
  my_free(input_init_value);
  my_free(input_density);
  my_free(input_probability);

  return 1;
}

static 
int fprint_spice_mux_testbench_call_one_grid_sb_muxes(FILE* fp, 
                                                      int x, int y, 
                                                      t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side, ix, iy;
  t_rr_node*** chan_rr_nodes = (t_rr_node***)my_malloc(sizeof(t_rr_node**)*4); /* 4 sides*/
  int* chan_width = (int*)my_malloc(sizeof(int)*4); /* 4 sides */
  int used = 0;

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
        if (INC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          /* used += fprint_spice_mux_testbench_sb_one_mux(fp, ix, iy, side, chan_rr_nodes[side][itrack]); */
        } else { 
          chan_rr_nodes[side][itrack] = NULL;
        }
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
        if (INC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          /* used += fprint_spice_mux_testbench_sb_one_mux(fp, ix, iy, side, chan_rr_nodes[side][itrack]); */
        } else { 
          chan_rr_nodes[side][itrack] = NULL;
        }
      }
      /* Print MUXes of RIGHT side */
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
        if (DEC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          /*used += fprint_spice_mux_testbench_sb_one_mux(fp, ix, iy, side, chan_rr_nodes[side][itrack]); */
        } else { 
          chan_rr_nodes[side][itrack] = NULL;
        }
      }
      /* Print MUXes of BOTTOM side */
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
        if (DEC_DIRECTION == chan_rr_nodes[side][itrack]->direction) {
          /* used += fprint_spice_mux_testbench_sb_one_mux(fp, ix, iy, side, chan_rr_nodes[side][itrack]); */
        } else { 
          chan_rr_nodes[side][itrack] = NULL;
        }
      }
      /* Print MUXes of LEFT side */
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index!\n", __FILE__, __LINE__);
      exit(1);
    }
  }

  /* print all the rr_nodes in the switch boxes if there is at least one rr_node with a net_num */
  used = 0;
  for (side = 0; side < 4; side++) {
    for (itrack = 0; itrack < chan_width[side]; itrack++) {
      if ((NULL != chan_rr_nodes[side][itrack])&&(OPEN != chan_rr_nodes[side][itrack]->net_num)) {
        used++; 
      }
    }
  }

  //if (used > 0) {
    for (side = 0; side < 4; side++) {
      /*
      switch (side) {
      case 0:
        ix = x; 
        iy = y + 1;
        break;
      case 1:
        ix = x + 1; 
        iy = y;
        break;
      case 2:
      case 3:
        ix = x; 
        iy = y;
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index!\n", __FILE__, __LINE__);
        exit(1);
      }
      */
      for (itrack = 0; itrack < chan_width[side]; itrack++) {
        fprint_spice_mux_testbench_sb_one_mux(fp, ix, iy, side, chan_rr_nodes[side][itrack]);
      }
    }
  //}

  /* Free */
  my_free(chan_width);
  for (side = 0; side < 4; side++) {
    my_free(chan_rr_nodes[side]);
  }
  my_free(chan_rr_nodes);

  if (0 < used) {
    used = 1;
  }

  return used;
}

/*
static 
void fprint_spice_mux_testbench_call_cb_muxes(FILE* fp,
                                              t_ivec*** LL_rr_node_indices) {
  int ix, iy;

  for (iy = 0; iy < (ny + 1); iy++) { 
    for (ix = 1; ix < (nx + 1); ix++) {
      fprint_spice_mux_testbench_one_grid_cb_muxes(fp, CHANX, ix, iy, LL_rr_node_indices);
    }
  }

  for (ix = 0; ix < (nx + 1); ix++) { 
    for (iy = 1; iy < (ny + 1); iy++) {
      fprint_spice_mux_testbench_one_grid_cb_muxes(fp, CHANY, ix, iy, LL_rr_node_indices);
    }
  }

  return;
}
*/

/*
static 
void fprint_spice_mux_testbench_call_sb_muxes(FILE* fp,
                                              t_ivec*** LL_rr_node_indices) {
  int ix, iy;

  for (iy = 0; iy < (ny + 1); iy++) { 
    for (ix = 0; ix < (nx + 1); ix++) {
      fprint_spice_mux_testbench_call_one_grid_sb_muxes(fp, ix, iy, LL_rr_node_indices);
    }
  }

  return;
}
*/

static 
int fprint_spice_mux_testbench_call_one_grid_pb_muxes(FILE* fp, int ix, int iy,
                                                      t_ivec*** LL_rr_node_indices) {
  int iblk;
  int used = 0;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  /* Print all the grid */
  if ((NULL == grid[ix][iy].type)||(0 != grid[ix][iy].offset)) {
    return used;
  }
  /* Used blocks */
  for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
    /* Only for mapped block */
    assert(NULL != block[grid[ix][iy].blocks[iblk]].pb);
    /* Mark the temporary net_num for the type pins*/
    mark_grid_type_pb_graph_node_pins_temp_net_num(ix, iy);
    fprint_spice_mux_testbench_pb_muxes_rec(fp, block[grid[ix][iy].blocks[iblk]].pb, ix, iy, LL_rr_node_indices); 
    used = 1;
  }  
  /* By pass Unused blocks */
  for (iblk = grid[ix][iy].usage; iblk < grid[ix][iy].type->capacity; iblk++) {
    /* Mark the temporary net_num for the type pins*/
    mark_grid_type_pb_graph_node_pins_temp_net_num(ix, iy);
    fprint_spice_mux_testbench_idle_pb_graph_node_muxes_rec(fp, grid[ix][iy].type->pb_graph_head, ix, iy, LL_rr_node_indices);
  } 

  return used;
}

/*
static 
void fprint_spice_mux_testbench_call_pb_muxes(FILE* fp, 
                                              t_ivec*** LL_rr_node_indices) {
  int ix, iy;
  
  for (iy = 1; iy < (ny + 1); iy++) { 
    for (ix = 1; ix < (nx + 1); ix++) {
      fprint_spice_mux_testbench_call_one_grid_pb_muxes(fp, ix, iy, LL_rr_node_indices);
    }
  }
  return;
}
*/

static 
void fprint_spice_mux_testbench_stimulations(FILE* fp, 
                                             t_spice spice) {
  /* Voltage sources of Multiplexers are already generated during printing the netlist 
   * We just need global stimulations Here.
   */

  /* Give global vdd, gnd, voltage sources*/
  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }
  /* Global GND */
  fprintf(fp, "***** Global GND port *****\n");
  fprintf(fp, "*Rggnd ggnd 0 0\n");

  /* Global set and reset */
  fprintf(fp, "***** Global Net for reset signal *****\n");
  fprintf(fp, "*Vgvreset greset 0 0\n");
  fprintf(fp, "***** Global Net for set signal *****\n");
  fprintf(fp, "*Vgvset gset 0 0\n");

  /* Global Vdd ports */
  fprintf(fp, "***** Global VDD ports *****\n");
  fprintf(fp, "*Vgvdd gvdd 0 vsp\n");
  fprintf(fp, "***** Global VDD for SRAMs *****\n");
  fprintf(fp, "Vgvdd_sram gvdd_sram 0 vsp\n");
  fprintf(fp, "***** Global VDD for load inverters *****\n");
  fprintf(fp, "Vgvdd_load gvdd_load 0 vsp\n");
  fprintf(fp, "***** Global Force for all SRAMs *****\n");
  fprintf(fp, "V%s->in %s->in 0 0\n", 
          sram_spice_model->prefix, sram_spice_model->prefix);
  fprintf(fp, ".nodeset V(%s->in) 0\n", sram_spice_model->prefix);

  return;
}

static 
void fprint_spice_mux_testbench_measurements(FILE* fp, 
                                             enum e_spice_mux_tb_type mux_tb_type,
                                             t_spice spice) {
  int num_clock_cycle = max_sim_num_clock_cycles;
    
  /*
  int i;
  t_llist* head = NULL;
  t_spice_mux_model* spice_mux_model = NULL;  
  */

  /* Give global vdd, gnd, voltage sources*/
  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprint_spice_netlist_transient_setting(fp, spice, num_clock_cycle, FALSE);
  /* Measure the leakage and dynamic power of SRAMs*/
  fprintf(fp, ".meas tran total_leakage_srams avg p(Vgvdd_sram) from=0 to=\'clock_period\'\n");
  fprintf(fp, ".meas tran total_dynamic_srams avg p(Vgvdd_sram) from=\'clock_period\' to=\'%d*clock_period\'\n", num_clock_cycle);
  fprintf(fp, ".meas tran total_energy_per_cycle_srams param=\'total_dynamic_srams*clock_period\'\n");

  /* Measure the total leakage and dynamic power */
  fprintf(fp, ".meas tran total_leakage_power_mux[0to%d] \n", testbench_mux_cnt - 1);
  fprintf(fp, "+          param=\'sum_leakage_power_mux[0to%d]\'\n", testbench_mux_cnt-1);
  fprintf(fp, ".meas tran total_energy_per_cycle_mux[0to%d] \n", testbench_mux_cnt - 1);
  fprintf(fp, "+          param=\'sum_energy_per_cycle_mux[0to%d]\'\n", testbench_mux_cnt-1);

  switch (mux_tb_type) {
  case SPICE_PB_MUX_TB:
    /* pb_muxes */
    fprintf(fp, ".meas tran total_leakage_power_pb_mux \n");
    fprintf(fp, "+          param=\'sum_leakage_power_pb_mux[0to%d]\'\n", testbench_pb_mux_cnt-1);
    fprintf(fp, ".meas tran total_energy_per_cycle_pb_mux \n");
    fprintf(fp, "+          param=\'sum_energy_per_cycle_pb_mux[0to%d]\'\n", testbench_pb_mux_cnt-1);
    break;
  case SPICE_CB_MUX_TB:
    /* cb_muxes */
    fprintf(fp, ".meas tran total_leakage_power_cb_mux \n");
    fprintf(fp, "+          param=\'sum_leakage_power_cb_mux[0to%d]\'\n", testbench_cb_mux_cnt-1);
    fprintf(fp, ".meas tran total_energy_per_cycle_cb_mux \n");
    fprintf(fp, "+          param=\'sum_energy_per_cycle_cb_mux[0to%d]\'\n", testbench_cb_mux_cnt-1);
    break;
  case SPICE_SB_MUX_TB:
    /* sb_muxes */
    fprintf(fp, ".meas tran total_leakage_power_sb_mux \n");
    fprintf(fp, "+          param=\'sum_leakage_power_sb_mux[0to%d]\'\n", testbench_sb_mux_cnt-1);
    fprintf(fp, ".meas tran total_energy_per_cycle_sb_mux \n");
    fprintf(fp, "+          param=\'sum_energy_per_cycle_sb_mux[0to%d]\'\n", testbench_sb_mux_cnt-1);
    break;
   default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid mux_tb_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Top-level function in this source file */
int fprint_spice_one_mux_testbench(char* formatted_spice_dir,
                                   char* circuit_name,
                                   char* mux_testbench_name, 
                                   char* include_dir_path,
                                   char* subckt_dir_path,
                                   t_ivec*** LL_rr_node_indices,
                                   int num_clocks,
                                   t_arch arch,
                                   int grid_x, int grid_y, t_rr_type cb_type,
                                   enum e_spice_mux_tb_type mux_tb_type,
                                   boolean leakage_only) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* title = my_strcat("FPGA SPICE Routing MUX Test Bench for Design: ", circuit_name);
  char* mux_testbench_file_path = my_strcat(formatted_spice_dir, mux_testbench_name);
  char* mux_tb_name = NULL;
  int used = 0;
  t_llist* temp = NULL;

  switch (mux_tb_type) {
  case SPICE_PB_MUX_TB:
    mux_tb_name = "CLB MUX";
    break;
  case SPICE_CB_MUX_TB:
    mux_tb_name = "Connection Box MUX";
    break;
  case SPICE_SB_MUX_TB:
    mux_tb_name = "Switch Block MUX";
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid mux_tb_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check if the path exists*/
  fp = fopen(mux_testbench_file_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE %s Test bench netlist %s!\n", 
               __FILE__, __LINE__, mux_tb_name, mux_testbench_file_path); 
    exit(1);
  }

  /*
  vpr_printf(TIO_MESSAGE_INFO, "Writing Grid[%d][%d] SPICE %s MUX Test Bench for %s...\n", 
             grid_x, grid_y, mux_tb_name, circuit_name);
  */

  /* Load global vars in this source file */
  num_segments = arch.num_segments;
  segments = arch.Segments;
 
  /* Print the title */
  fprint_spice_head(fp, title);
  my_free(title);

  /* print technology library and design parameters*/
  fprint_tech_lib(fp, arch.spice->tech_lib);

  /* Include parameter header files */
  fprint_spice_include_param_headers(fp, include_dir_path);

  /* Include Key subckts */
  fprint_spice_include_key_subckts(fp, formatted_subckt_dir_path);

  /* Include user-defined sub-circuit netlist */
  init_include_user_defined_netlists(*(arch.spice));
  fprint_include_user_defined_netlists(fp, *(arch.spice));
  
  /* Special subckts for Top-level SPICE netlist */
  /*
  fprintf(fp, "****** Include subckt netlists: Look-Up Tables (LUTs) *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, luts_spice_file_name);
  fprintf(fp, ".include %s\n", temp_include_file_path);
  my_free(temp_include_file_path);
  */

  /* Print simulation temperature and other options for SPICE */
  fprint_spice_options(fp, arch.spice->spice_params);

  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  fprint_spice_mux_testbench_global_ports(fp, *(arch.spice));
 
  /* Quote defined Logic blocks subckts (Grids) */
  init_spice_mux_testbench_globals(*(arch.spice));

  switch (mux_tb_type) {
  case SPICE_PB_MUX_TB:
    total_pb_mux_input_density = 0.;
    used = fprint_spice_mux_testbench_call_one_grid_pb_muxes(fp, grid_x, grid_y, LL_rr_node_indices);
    total_pb_mux_input_density = total_pb_mux_input_density/testbench_pb_mux_cnt;
    vpr_printf(TIO_MESSAGE_INFO,"Average density of PB MUX inputs is %.2g.\n", total_pb_mux_input_density);
    break;
  case SPICE_CB_MUX_TB:
    /* one cbx, one cby*/
    switch (cb_type) {
    case CHANX:
    case CHANY:
      total_cb_mux_input_density = 0.;
      used = fprint_spice_mux_testbench_one_grid_cb_muxes(fp, cb_type, grid_x, grid_y, LL_rr_node_indices);
      total_cb_mux_input_density = total_cb_mux_input_density/testbench_cb_mux_cnt;
      vpr_printf(TIO_MESSAGE_INFO,"Average density of CB MUX inputs is %.2g.\n", total_cb_mux_input_density);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid connection_box_type!\n", __FILE__, __LINE__);
      exit(1);
    }
    break;
  case SPICE_SB_MUX_TB:
    total_sb_mux_input_density = 0.;
    used = fprint_spice_mux_testbench_call_one_grid_sb_muxes(fp, grid_x, grid_y, LL_rr_node_indices);
    total_sb_mux_input_density = total_sb_mux_input_density/testbench_sb_mux_cnt;
    vpr_printf(TIO_MESSAGE_INFO,"Average density of SB MUX inputs is %.2g.\n", total_sb_mux_input_density);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid mux_tb_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Add stimulations */
  fprint_spice_mux_testbench_stimulations(fp, *(arch.spice));

  /* Add measurements */  
  fprint_spice_mux_testbench_measurements(fp, mux_tb_type, *(arch.spice));

  /* SPICE ends*/
  fprintf(fp, ".end\n");

  /* Close the file*/
  fclose(fp);

  /* Free */
  //my_free(formatted_subckt_dir_path);
  //my_free(mux_testbench_file_path);
  //my_free(title);
  free_muxes_llist(testbench_muxes_head);

  if (0 < testbench_mux_cnt) {
    vpr_printf(TIO_MESSAGE_INFO, "Writing Grid[%d][%d] SPICE %s Test Bench for %s...\n", 
               grid_x, grid_y, mux_tb_name, circuit_name);
    if (NULL == tb_head) {
      tb_head = create_llist(1);
      tb_head->dptr = my_malloc(sizeof(t_spicetb_info));
      ((t_spicetb_info*)(tb_head->dptr))->tb_name = my_strdup(mux_testbench_file_path);
      ((t_spicetb_info*)(tb_head->dptr))->num_sim_clock_cycles = max_sim_num_clock_cycles;
    } else {
      temp = insert_llist_node(tb_head);
      temp->dptr = my_malloc(sizeof(t_spicetb_info));
      ((t_spicetb_info*)(temp->dptr))->tb_name = my_strdup(mux_testbench_file_path);
      ((t_spicetb_info*)(temp->dptr))->num_sim_clock_cycles = max_sim_num_clock_cycles;
    }
    used = 1;
  } else {
    /* Remove the file generated */
    my_remove_file(mux_testbench_file_path);
    used = 0;
  }

  return used;
}

void fprint_spice_mux_testbench(char* formatted_spice_dir,
                                char* circuit_name,
                                char* include_dir_path,
                                char* subckt_dir_path,
                                t_ivec*** LL_rr_node_indices,
                                int num_clocks,
                                t_arch arch,
                                enum e_spice_mux_tb_type mux_tb_type,
                                boolean leakage_only) {
  char* mux_testbench_name = NULL; 
  int ix, iy;
  int cnt = 0;
  int used = 0;

  /* Depend on the type of testbench, we generate the a list of testbenches */
  switch (mux_tb_type) {
  case SPICE_PB_MUX_TB:
    cnt = 0;
    for (ix = 1; ix < (nx+1); ix++) {
      for (iy = 1; iy < (ny+1); iy++) {
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 6 + strlen(my_itoa(cnt)) + 1
                                              + strlen(spice_pb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_grid%d%s",
                circuit_name, cnt, spice_pb_mux_testbench_postfix);
        used = fprint_spice_one_mux_testbench(formatted_spice_dir, circuit_name, mux_testbench_name, 
                                              include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                              num_clocks, arch, ix, iy, NUM_RR_TYPES, SPICE_PB_MUX_TB, 
                                              leakage_only);
        if (1 == used) {
          cnt += used;
        }
        /* free */
        my_free(mux_testbench_name);
      }  
    } 
    /* Update the global counter */
    num_used_grid_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,"No. of generated PB_MUX testbench = %d\n", num_used_grid_tb);
    break;
  case SPICE_CB_MUX_TB:
    cnt = 0;
    for (iy = 0; iy < (ny+1); iy++) {
      for (ix = 1; ix < (nx+1); ix++) {
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 10 + strlen(my_itoa(cnt)) + 1 
                                              + strlen(spice_pb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_cb%d%s",
                circuit_name, cnt, spice_cb_mux_testbench_postfix);
        used = fprint_spice_one_mux_testbench(formatted_spice_dir, circuit_name, mux_testbench_name, 
                                              include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                              num_clocks, arch, ix, iy, CHANX, SPICE_CB_MUX_TB, 
                                              leakage_only);
        if (1 == used) {
          cnt += used;
        }
        /* free */
        my_free(mux_testbench_name);
      }  
    } 
    for (ix = 0; ix < (nx+1); ix++) {
      for (iy = 1; iy < (ny+1); iy++) {
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 10 + strlen(my_itoa(cnt)) + 1
                                              + strlen(spice_pb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_cb%d%s",
                circuit_name, cnt, spice_cb_mux_testbench_postfix);
        used = fprint_spice_one_mux_testbench(formatted_spice_dir, circuit_name, mux_testbench_name, 
                                              include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                              num_clocks, arch, ix, iy, CHANY, SPICE_CB_MUX_TB, 
                                              leakage_only);
        if (1 == used) {
          cnt += used;
        }
        /* free */
        my_free(mux_testbench_name);
      }  
    } 
    /* Update the global counter */
    num_used_cb_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,"No. of generated CB_MUX testbench = %d\n", num_used_cb_tb);
    break;
  case SPICE_SB_MUX_TB:
    cnt = 0;
    for (ix = 0; ix < (nx+1); ix++) {
      for (iy = 0; iy < (ny+1); iy++) {
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 9 + strlen(my_itoa(cnt)) + 1
                                              + strlen(spice_sb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_sb%d%s",
                circuit_name, cnt, spice_sb_mux_testbench_postfix);
        used = fprint_spice_one_mux_testbench(formatted_spice_dir, circuit_name, mux_testbench_name, 
                                              include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                              num_clocks, arch, ix, iy, NUM_RR_TYPES, SPICE_SB_MUX_TB, 
                                              leakage_only);
        if (1 == used) {
          cnt += used;
        }
        /* free */
        my_free(mux_testbench_name);
      }  
    } 
    /* Update the global counter */
    num_used_sb_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,"No. of generated SB_MUX testbench = %d\n", num_used_sb_tb);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid mux_tb_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return;
}
