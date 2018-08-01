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
#include "fpga_spice_globals.h"
#include "spice_globals.h"
#include "fpga_spice_utils.h"
#include "fpga_spice_backannotate_utils.h"
#include "spice_utils.h"
#include "spice_routing.h"
#include "spice_subckt.h"
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
static int num_segments;
static t_segment_inf* segments;
static t_llist* testbench_muxes_head = NULL; 
static int upbound_sim_num_clock_cycles = 2;
static int max_sim_num_clock_cycles = 2;
static int auto_select_max_sim_num_clock_cycles = TRUE;

static float total_pb_mux_input_density = 0.;
static float total_cb_mux_input_density = 0.;
static float total_sb_mux_input_density = 0.;

/***** Local Subroutines Declaration *****/

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

  /* Print generic global ports*/
  fprint_spice_generic_testbench_global_ports(fp, 
                                              sram_spice_orgz_info,
                                              global_ports_head); 

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
  char* sram_vdd_port_name = NULL;

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
 /* Global ports */
  if (0 < rec_fprint_spice_model_global_ports(fp, mux_spice_model, FALSE)) { 
    fprintf(fp, "+ ");
  }
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
    /* Special for 2-input MUX */
    if (2 == mux_size) {
      mux_level = 1;
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
  cur_num_sram = testbench_sram_cnt; 
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    assert( (0 == mux_sram_bits[ilevel]) || (1 == mux_sram_bits[ilevel]) );
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, 
                                  cur_num_sram + ilevel, mux_sram_bits[ilevel]);
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, 
                                  cur_num_sram + ilevel, 1 - mux_sram_bits[ilevel]);
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
  /* Give the VDD port name for SRAMs */
  sram_vdd_port_name = (char*)my_malloc(sizeof(char)*
                                       (strlen(spice_tb_global_vdd_sram_port_name) 
                                        + 1 ));
  sprintf(sram_vdd_port_name, "%s",
                              spice_tb_global_vdd_sram_port_name);
  /* Now Print SRAMs one by one */
  for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
    fprint_spice_one_specific_sram_subckt(fp, sram_spice_orgz_info, mux_spice_model, 
                                          sram_vdd_port_name, testbench_sram_cnt);
    testbench_sram_cnt++;
  }

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

  /* Free */
  my_free(mux_sram_bits);
  my_free(sram_vdd_port_name);

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

  if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
    fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, &testbench_load_cnt,
                                                      grid_x, grid_y, 
                                                      des_pb_graph_pin, NULL, 
                                                      outport_name, TRUE, 
                                                      LL_rr_node_indices); 
  }

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

  if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
    fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, &testbench_load_cnt,
                                                      grid_x, grid_y,
                                                      des_pb_graph_pin, des_pb, 
                                                      outport_name, TRUE, 
                                                      LL_rr_node_indices);
  }

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
                                                     cur_interc, fan_in, 
                                                     select_path_id, 
                                                     grid_x, grid_y, 
                                                     LL_rr_node_indices); 
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
    fprint_spice_mux_testbench_pb_pin_mux(fp, 
                                          pb_rr_graph, des_pb, 
                                          cur_mode, des_pb_graph_pin, 
                                          cur_interc, fan_in, select_path_id, 
                                          grid_x, grid_y, 
                                          LL_rr_node_indices); 
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
                                                          path_id, 
                                                          grid_x, grid_y, 
                                                          LL_rr_node_indices);
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
                                                              grid_x, grid_y, 
                                                              LL_rr_node_indices);
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
                                                              grid_x, grid_y, 
                                                              LL_rr_node_indices);
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
    fprint_spice_mux_testbench_pb_graph_node_interc(fp, cur_pb_graph_node, 
                                                    grid_x, grid_y, 
                                                    LL_rr_node_indices);
  } else {
    return;
  }
  
  /* Go recursively ... */
  mode_index = find_pb_type_idle_mode_index(*(cur_pb_graph_node->pb_type));
  for (ipb = 0; ipb < cur_pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Print idle muxes */
      fprint_spice_mux_testbench_idle_pb_graph_node_muxes_rec(fp,
                                                              &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]), 
                                                              grid_x, grid_y, 
                                                              LL_rr_node_indices);
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
                                               grid_x, grid_y, 
                                               LL_rr_node_indices);
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
                                                                grid_x, grid_y, 
                                                                LL_rr_node_indices);
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
                                                                grid_x, grid_y, 
                                                                LL_rr_node_indices);
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
                                                   grid_x, grid_y, 
                                                   LL_rr_node_indices);
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
                                                   grid_x, grid_y, 
                                                   LL_rr_node_indices);
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
    fprint_spice_mux_testbench_pb_interc(fp, cur_pb, 
                                         grid_x, grid_y, 
                                         LL_rr_node_indices);
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
        fprint_spice_mux_testbench_idle_pb_graph_node_muxes_rec(fp, 
                                                                cur_pb->child_pbs[ipb][jpb].pb_graph_node, 
                                                                grid_x, grid_y, 
                                                                LL_rr_node_indices);
      }
    }
  }
  
  return;
}


static 
void fprint_spice_mux_testbench_cb_one_mux(FILE* fp,
                                           t_cb cur_cb_info,
                                           t_rr_node* src_rr_node,
                                           t_ivec*** LL_rr_node_indices) {
  int mux_size, cb_x, cb_y;
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
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 

  cb_x = cur_cb_info.x;
  cb_y = cur_cb_info.y;

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
  fprint_spice_mux_testbench_one_mux(fp, meas_tag, 
                                     mux_spice_model, src_rr_node->fan_in, 
                                     input_init_value, input_density, 
                                     input_probability, path_id);

  /* Generate loads */
  outport_name = (char*)my_malloc(sizeof(char)*( strlen(mux_spice_model->prefix) + 5 
                                  + strlen(my_itoa(mux_size)) + 1 + strlen(my_itoa(testbench_mux_cnt)) 
                                  + 7 ));
  sprintf(outport_name, "%s_size%d[%d]->out",
                        mux_spice_model->prefix, 
                        mux_size, 
                        testbench_mux_cnt);

  if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
    fprint_spice_testbench_one_cb_mux_loads(fp, &testbench_load_cnt, src_rr_node, 
                                            outport_name, LL_rr_node_indices);
  }

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
                                          t_cb cur_cb_info,
                                          t_rr_node* src_rr_node,
                                          t_ivec*** LL_rr_node_indices) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 

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
    fprint_spice_mux_testbench_cb_one_mux(fp, 
                                          cur_cb_info, 
                                          src_rr_node, 
                                          LL_rr_node_indices);
  } 
   
  return;
}


/* Assume each connection box has a regional power-on/off switch */
static 
int fprint_spice_mux_testbench_call_one_grid_cb_muxes(FILE* fp, 
                                                      t_cb cur_cb_info,
                                                      t_ivec*** LL_rr_node_indices) {
  int inode, side;
  int side_cnt = 0;
  int used = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 

  side_cnt = 0;
  used = 0;
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
    /* Print multiplexers */
    /* Check if there is at least one rr_node with a net_name*/
      if (OPEN != cur_cb_info.ipin_rr_node[side][inode]->net_num) {
        used = 1;
      }
      fprint_spice_mux_testbench_cb_interc(fp, cur_cb_info, 
                                           cur_cb_info.ipin_rr_node[side][inode],
                                           LL_rr_node_indices);
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));

  /* Free */

  return used;
}

static 
int fprint_spice_mux_testbench_sb_one_mux(FILE* fp,
                                          t_sb cur_sb_info,
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

  int switch_box_x, switch_box_y;

  switch_box_x = cur_sb_info.x;
  switch_box_y = cur_sb_info.y;

  /* Check */
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 

  if (NULL == src_rr_node) {
    return 0;
  }

  /* ignore idle sb mux 
  if (OPEN == src_rr_node->net_num) {
    return used;
  }
  */

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (1 == is_rr_node_exist_opposite_side_in_sb_info(cur_sb_info, src_rr_node, chan_side)) {
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
  fprint_spice_mux_testbench_one_mux(fp, meas_tag, 
                                     mux_spice_model, mux_size,
                                     input_init_value, input_density, 
                                     input_probability, path_id);

  /* Print a channel wire !*/
  outport_name = (char*)my_malloc(sizeof(char)*( strlen(mux_spice_model->prefix)
                                 + 5 + strlen(my_itoa(mux_size)) + 1 
                                 + strlen(my_itoa(testbench_mux_cnt))
                                 + 6 + 1 ));
  sprintf(outport_name, "%s_size%d[%d]->out", mux_spice_model->prefix, mux_size, testbench_mux_cnt);
  
  if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
    fprintf(fp, "***** Load for rr_node[%d] *****\n", src_rr_node - rr_node);
    rr_node_outport_name = fprint_spice_testbench_rr_node_load_version(fp, 
                                                                       &testbench_load_cnt,
                                                                       num_segments,
                                                                       segments,
                                                                       0, /* load size */
                                                                       (*src_rr_node), 
                                                                       outport_name); 
  }

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
                                                      t_sb cur_sb_info,
                                                      t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side;
  int used = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > cur_sb_info.x))&&(!(cur_sb_info.x > (nx + 1)))); 
  assert((!(0 > cur_sb_info.y))&&(!(cur_sb_info.y > (ny + 1)))); 

  
  /* print all the rr_nodes in the switch boxes if there is at least one rr_node with a net_num */
  used = 0;
  for (side = 0; side < cur_sb_info.num_sides; side++) {
    for (itrack = 0; itrack < cur_sb_info.chan_width[side]; itrack++) {
      switch (cur_sb_info.chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        fprint_spice_mux_testbench_sb_one_mux(fp, 
                                              cur_sb_info, 
                                              side, 
                                              cur_sb_info.chan_rr_node[side][itrack]);
        used++;
        break;
      case IN_PORT:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of port sb[%d][%d] Channel node[%d] track[%d]!\n",
                   __FILE__, __LINE__, cur_sb_info.x, cur_sb_info.y, side, itrack);
        exit(1);
      }
    }
  }

  /* Free */
  if (0 < used) {
    used = 1;
  }

  return used;
}

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
  if ((NULL == grid[ix][iy].type)
     ||(EMPTY_TYPE == grid[ix][iy].type)
     ||(0 != grid[ix][iy].offset)) {
    return used;
  }
  /* Used blocks */
  for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
    /* Only for mapped block */
    assert(NULL != block[grid[ix][iy].blocks[iblk]].pb);
    /* Mark the temporary net_num for the type pins*/
    mark_one_pb_parasitic_nets(block[grid[ix][iy].blocks[iblk]].pb);
    fprint_spice_mux_testbench_pb_muxes_rec(fp, 
                                            block[grid[ix][iy].blocks[iblk]].pb, 
                                            ix, iy, 
                                            LL_rr_node_indices); 
    used = 1;
  }  
  /* By pass Unused blocks */
  for (iblk = grid[ix][iy].usage; iblk < grid[ix][iy].type->capacity; iblk++) {
    /* Mark the temporary net_num for the type pins*/
    mark_grid_type_pb_graph_node_pins_temp_net_num(ix, iy);
    fprint_spice_mux_testbench_idle_pb_graph_node_muxes_rec(fp, 
                                                            grid[ix][iy].type->pb_graph_head, 
                                                            ix, iy, 
                                                            LL_rr_node_indices);
  } 

  return used;
}

static 
void fprint_spice_mux_testbench_stimulations(FILE* fp, 
                                             int num_clocks) {
  /* Voltage sources of Multiplexers are already generated during printing the netlist 
   * We just need global stimulations Here.
   */

  /* Give global vdd, gnd, voltage sources*/
  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Print generic stimuli */
  fprint_spice_testbench_generic_global_ports_stimuli(fp, num_clocks);
  
  /* Generate global ports stimuli */
  fprint_spice_testbench_global_ports_stimuli(fp, global_ports_head);

  /* SRAM ports */
  fprintf(fp, "***** Global Inputs for SRAMs *****\n");
  fprint_spice_testbench_global_sram_inport_stimuli(fp, sram_spice_orgz_info);

  fprintf(fp, "***** Global VDD for SRAMs *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_sram_port_name,
                                                 "vsp");

  fprintf(fp, "***** Global VDD for load inverters *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_load_port_name,
                                                 "vsp");

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
  fprint_spice_netlist_generic_measurements(fp, spice.spice_params.mc_params, spice.num_spice_model, spice.spice_models);

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
  /*fprint_tech_lib(fp, arch.spice->tech_lib);*/

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
    /* Output a pb_mux testbench */
    used = fprint_spice_mux_testbench_call_one_grid_pb_muxes(fp, grid_x, grid_y, LL_rr_node_indices);

    /* Check and output info. */
    assert((0 == testbench_pb_mux_cnt)||(0 < testbench_pb_mux_cnt));
    if (0 < testbench_pb_mux_cnt) {
      total_pb_mux_input_density = total_pb_mux_input_density/testbench_pb_mux_cnt;
      /* Add stimulations */
      fprint_spice_mux_testbench_stimulations(fp, num_clocks);
      /* Add measurements */  
      fprint_spice_mux_testbench_measurements(fp, mux_tb_type, *(arch.spice));
    }
    /* 
    vpr_printf(TIO_MESSAGE_INFO,"Average density of PB MUX inputs is %.2g.\n", total_pb_mux_input_density);
    */
    break;
  case SPICE_CB_MUX_TB:
    /* one cbx or  one cby*/
    total_cb_mux_input_density = 0.;
    /* Output a cb_mux testbench */
    switch (cb_type) {
    case CHANX:
      used = fprint_spice_mux_testbench_call_one_grid_cb_muxes(fp, cbx_info[grid_x][grid_y], LL_rr_node_indices);
      break;
    case CHANY:
      used = fprint_spice_mux_testbench_call_one_grid_cb_muxes(fp, cby_info[grid_x][grid_y], LL_rr_node_indices);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid connection_box_type!\n", __FILE__, __LINE__);
      exit(1);
    }
    /* Check and output info. */
    assert((0 == testbench_cb_mux_cnt)||(0 < testbench_cb_mux_cnt));
    if (0 < testbench_cb_mux_cnt) {
      total_cb_mux_input_density = total_cb_mux_input_density/testbench_cb_mux_cnt;
      /* Add stimulations */
      fprint_spice_mux_testbench_stimulations(fp, num_clocks);
      /* Add measurements */  
      fprint_spice_mux_testbench_measurements(fp, mux_tb_type, *(arch.spice));
    }
    /* 
    vpr_printf(TIO_MESSAGE_INFO,"Average density of CB MUX inputs is %.2g.\n", total_cb_mux_input_density);
    */
    break;
  case SPICE_SB_MUX_TB:
    total_sb_mux_input_density = 0.;
    /* Output a sb_mux testbench */
    used = fprint_spice_mux_testbench_call_one_grid_sb_muxes(fp, sb_info[grid_x][grid_y], LL_rr_node_indices);
    /* Check and output info. */
    assert((0 == testbench_sb_mux_cnt)||(0 < testbench_sb_mux_cnt));
    if (0 < testbench_sb_mux_cnt) {
      total_sb_mux_input_density = total_sb_mux_input_density/testbench_sb_mux_cnt;
      /* Add stimulations */
      fprint_spice_mux_testbench_stimulations(fp, num_clocks);
      /* Add measurements */  
      fprint_spice_mux_testbench_measurements(fp, mux_tb_type, *(arch.spice));
    }
    /* 
    vpr_printf(TIO_MESSAGE_INFO,"Average density of SB MUX inputs is %.2g.\n", total_sb_mux_input_density);
    */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid mux_tb_type!\n", __FILE__, __LINE__);
    exit(1);
  }

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
    /* vpr_printf(TIO_MESSAGE_INFO, "Writing Grid[%d][%d] SPICE %s Test Bench for %s...\n", 
               grid_x, grid_y, mux_tb_name, circuit_name);
    */
    /* Push the testbench to the linked list */
    tb_head = add_one_spice_tb_info_to_llist(tb_head, mux_testbench_file_path, max_sim_num_clock_cycles); 
    used = 1;
  } else {
    /* Remove the file generated */
    my_remove_file(mux_testbench_file_path);
    used = 0;
  }

  return used;
}

void spice_print_mux_testbench(char* formatted_spice_dir,
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
  int bypass_cnt = 0;

  /* Depend on the type of testbench, we generate the a list of testbenches */
  switch (mux_tb_type) {
  case SPICE_PB_MUX_TB:
    cnt = 0;
    vpr_printf(TIO_MESSAGE_INFO,"Generating Grid multiplexer testbench...\n");
    for (ix = 1; ix < (nx+1); ix++) {
      for (iy = 1; iy < (ny+1); iy++) {
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 5 + strlen(my_itoa(ix)) + 1
                                              + strlen(my_itoa(iy)) + 1
                                              + strlen(spice_pb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_grid%d_%d%s",
                circuit_name, ix, iy, spice_pb_mux_testbench_postfix);
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
    num_used_grid_mux_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,"No. of generated Grid multiplexer testbench = %d\n", num_used_grid_mux_tb);
    break;
  case SPICE_CB_MUX_TB:
    cnt = 0;
    /* X-channel Connection Blocks */
    vpr_printf(TIO_MESSAGE_INFO,"Generating X-channel Connection Block multiplexer testbench...\n");
    for (iy = 0; iy < (ny+1); iy++) {
      for (ix = 1; ix < (nx+1); ix++) {
        /* Bypass non-exist CBs */
        if ((FALSE == is_cb_exist(CHANX, ix, iy))
           ||(0 == count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy]))) {
          bypass_cnt++;
          continue;
        }
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 1
                                              + strlen(spice_cb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_cbx%d_%d%s",
                circuit_name, ix, iy, spice_cb_mux_testbench_postfix);
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

    /* Y-channel Connection Blocks */
    vpr_printf(TIO_MESSAGE_INFO,"Generating Y-channel Connection Block multiplexer testbench...\n");
    for (ix = 0; ix < (nx+1); ix++) {
      for (iy = 1; iy < (ny+1); iy++) {
        /* Bypass non-exist CBs */
        if ((FALSE == is_cb_exist(CHANY, ix, iy))
           ||(0 == count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
          bypass_cnt++;
          continue;
        }
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 1
                                              + strlen(spice_cb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_cby%d_%d%s",
                circuit_name, ix, iy, spice_cb_mux_testbench_postfix);
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
    num_used_cb_mux_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,"No. of generated Connection Block multiplexer testbench = %d\n", num_used_cb_mux_tb);
    vpr_printf(TIO_MESSAGE_INFO, "Bypass %d Connection Blocks that does no exist in the architecture.\n",
               bypass_cnt);
    break;
  case SPICE_SB_MUX_TB:
    cnt = 0;
    vpr_printf(TIO_MESSAGE_INFO,"Generating Switch Block multiplexer testbench...\n");
    for (ix = 0; ix < (nx+1); ix++) {
      for (iy = 0; iy < (ny+1); iy++) {
        mux_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                              + 4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 1
                                              + strlen(spice_sb_mux_testbench_postfix)  + 1 ));
        sprintf(mux_testbench_name, "%s_sb%d_%d%s",
                circuit_name, ix, iy, spice_sb_mux_testbench_postfix);
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
    num_used_sb_mux_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,"No. of generated Switch Block multiplexer testbench = %d\n", num_used_sb_mux_tb);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid mux_tb_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return;
}
