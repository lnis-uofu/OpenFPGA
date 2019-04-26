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
#include "rr_graph.h"
#include "vpr_utils.h"
#include "route_common.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_types.h"
#include "spice_globals.h"
#include "fpga_x2p_utils.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_subckt.h"

/* For mrFPGA */
#ifdef MRFPGA_H
#include "mrfpga_globals.h"
#endif

/***** Subroutines Declarations *****/
static 
void fprint_spice_meas_header(char* meas_file_name,
                              t_spice_meas_params spice_meas_params);

static 
void fprint_spice_stimulate_header(char* stimulate_file_name,
                                   t_spice_stimulate_params spice_stimulate_params,
                                   float vpr_clock_period,
                                   int num_clock);

/***** Subroutines *****/
/* Print SPICE Netlists header*/
/* Print parameters for measurements */
static 
void fprint_spice_meas_header(char* meas_file_name,
                              t_spice_meas_params spice_meas_params) {
  FILE* fp = NULL;
   
  /* Check */
  assert(NULL != meas_file_name);

  /* Create File */
  fp = fopen(meas_file_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Failure in create measure header file %s!\n",
               __FILE__, __LINE__, meas_file_name);
    exit(1);
  }

  /* Print parameters */
  fprint_spice_head(fp, "Parameters for measurement");
  fprintf(fp, "***** Parameters For Slew Measurement *****\n");
  fprintf(fp, "***** Rising Edge *****\n");
  fprintf(fp, ".param slew_upper_thres_pct_rise=%g\n", spice_meas_params.slew_upper_thres_pct_rise);
  fprintf(fp, ".param slew_lower_thres_pct_rise=%g\n", spice_meas_params.slew_lower_thres_pct_rise);
  fprintf(fp, "***** Falling Edge *****\n");
  fprintf(fp, ".param slew_upper_thres_pct_fall=%g\n", spice_meas_params.slew_upper_thres_pct_fall);
  fprintf(fp, ".param slew_lower_thres_pct_fall=%g\n", spice_meas_params.slew_lower_thres_pct_fall);

  fprintf(fp, "***** Parameters For Delay Measurement *****\n");
  fprintf(fp, "***** Rising Edge *****\n");
  fprintf(fp, ".param input_thres_pct_rise=%g\n", spice_meas_params.input_thres_pct_rise);
  fprintf(fp, ".param output_thres_pct_rise=%g\n", spice_meas_params.output_thres_pct_rise);
  fprintf(fp, "***** Falling Edge *****\n");
  fprintf(fp, ".param input_thres_pct_fall=%g\n", spice_meas_params.input_thres_pct_fall);
  fprintf(fp, ".param output_thres_pct_fall=%g\n", spice_meas_params.output_thres_pct_fall);

  /* Close File */
  fclose(fp);

  return;
}

/* Print parameters for measurements */
static 
void fprint_spice_stimulate_header(char* stimulate_file_name,
                                   t_spice_stimulate_params spice_stimulate_params,
                                   float vpr_clock_period,
                                   int num_clock) {
  FILE* fp = NULL;
  float sim_clock_freq = 0.;
  float sim_clock_period = 0.;
   
  /* Check */
  assert(NULL != stimulate_file_name);

  /* Create File */
  fp = fopen(stimulate_file_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Failure in create stimulate header file %s!\n",
               __FILE__, __LINE__, stimulate_file_name);
    exit(1);
  }

  fprint_spice_head(fp, "Parameters for Stimulations");

  /* if estimated clock frequency from VPR is 0. 
   * this is a combinational circuit, clock frequency will never be used 
   */
  /* if the clock frequency is not specified in architecture file,
   * We define the clock frequency with estimated value and slack
   */
  fprintf(fp, "***** Frequency *****\n");
  sim_clock_freq = spice_stimulate_params.op_clock_freq;
  /* Simulate clock frequency should be larger than 0 !*/
  assert(0. < sim_clock_freq); /*TODO: check this earlier!!! */
  /* vpr_printf(TIO_MESSAGE_INFO, "Use Clock freqency %.2f [MHz] in SPICE simulation.\n", sim_clock_freq/1e6); */
  fprintf(fp, ".param clock_period=%g\n", 1. / sim_clock_freq);
  sim_clock_period = 1./sim_clock_freq;

  /* Print parameters */
  fprintf(fp, "***** Parameters For Input Stimulations *****\n");
  switch (spice_stimulate_params.input_slew_rise_type) {
  case SPICE_ABS: 
    if (sim_clock_period < (spice_stimulate_params.input_slew_rise_time
       + spice_stimulate_params.input_slew_fall_time)) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid input_slew_rise_time(%.2g), should be smaller than clock period(%.2g)!\n",
                 __FILE__, __LINE__, spice_stimulate_params.input_slew_rise_time, sim_clock_period);
      exit(1);
    }
    fprintf(fp, ".param input_slew_pct_rise='%g/clock_period'\n", spice_stimulate_params.input_slew_rise_time);
    break;
  case SPICE_FRAC:
    fprintf(fp, ".param input_slew_pct_rise='%g'\n", spice_stimulate_params.input_slew_rise_time);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid input_slew_rise_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  switch (spice_stimulate_params.input_slew_fall_type) {
  case SPICE_ABS: 
    if (sim_clock_period < (spice_stimulate_params.input_slew_rise_time
       + spice_stimulate_params.input_slew_fall_time)) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid input_slew_fall_time(%.2g), should be smaller than clock period(%.2g)!\n",
                 __FILE__, __LINE__, spice_stimulate_params.input_slew_fall_time, sim_clock_period);
      exit(1);
    }
    fprintf(fp, ".param input_slew_pct_fall='%g/clock_period'\n", spice_stimulate_params.input_slew_fall_time);
    break;
  case SPICE_FRAC:
    fprintf(fp, ".param input_slew_pct_fall='%g'\n", spice_stimulate_params.input_slew_fall_time);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid input_slew_fall_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "***** Parameters For Clock Stimulations *****\n");
  fprintf(fp, "***** Slew *****\n");

  switch (spice_stimulate_params.clock_slew_rise_type) {
  case SPICE_ABS: 
    if (sim_clock_period < (spice_stimulate_params.clock_slew_rise_time
       + spice_stimulate_params.clock_slew_fall_time)) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid clock_slew_rise_time(%.2g)+clock_slew_fall_time(%.2g), should be smaller than clock period(%.2g)!\n",
                 __FILE__, __LINE__, spice_stimulate_params.clock_slew_rise_time,spice_stimulate_params.clock_slew_fall_time, sim_clock_period);
      exit(1);
    }
    fprintf(fp, ".param clock_slew_pct_rise='%g/clock_period'\n", spice_stimulate_params.clock_slew_rise_time);
    break;
  case SPICE_FRAC:
    fprintf(fp, ".param clock_slew_pct_rise='%g'\n", spice_stimulate_params.clock_slew_rise_time);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid clock_slew_rise_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  switch (spice_stimulate_params.clock_slew_fall_type) {
  case SPICE_ABS: 
    if (sim_clock_period < (spice_stimulate_params.clock_slew_rise_time
       + spice_stimulate_params.clock_slew_fall_time)) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid clock_slew_rise_time(%.2g)+clock_slew_fall_time(%.2g), should be smaller than clock period(%.2g)!\n",
                 __FILE__, __LINE__, spice_stimulate_params.clock_slew_rise_time,spice_stimulate_params.clock_slew_fall_time, sim_clock_period);
      exit(1);
    }
    fprintf(fp, ".param clock_slew_pct_fall='%g/clock_period'\n", spice_stimulate_params.clock_slew_fall_time);
    break;
  case SPICE_FRAC:
    fprintf(fp, ".param clock_slew_pct_fall='%g'\n", spice_stimulate_params.clock_slew_fall_time);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid input_slew_fall_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  fclose(fp);

  return;
}

/* Print parameters for circuit designs */
static 
void fprint_spice_design_param_header(char* design_param_file_name,
                                      t_spice spice) {
  FILE* fp = NULL;
   
  /* Check */
  assert(NULL != design_param_file_name);

  /* Create File */
  fp = fopen(design_param_file_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Failure in create design parameter header file %s!\n",
               __FILE__, __LINE__, design_param_file_name);
    exit(1);
  }

  fprint_spice_head(fp, "Parameters for Circuit Designs");

  fprint_tech_lib(fp, 
                  spice.spice_params.mc_params.cmos_variation, 
                  spice.tech_lib);

  /* For transistors */
  fprint_spice_circuit_param(fp, 
                             spice.spice_params.mc_params, 
                             spice.num_spice_model, 
                             spice.spice_models);

  fclose(fp);

  return;
}


void spice_print_headers(char* include_dir_path,
                          float vpr_clock_period,
                          int num_clock,
                          t_spice spice) {
  char* formatted_include_dir_path = format_dir_path(include_dir_path);
  char* meas_header_file_path = NULL;
  char* stimu_header_file_path = NULL;
  char* design_param_header_file_path = NULL;

  /* measurement header file */
  meas_header_file_path = my_strcat(formatted_include_dir_path, meas_header_file_name);
  fprint_spice_meas_header(meas_header_file_path, spice.spice_params.meas_params);
  
  /* stimulate header file */
  stimu_header_file_path = my_strcat(formatted_include_dir_path, stimu_header_file_name);
  fprint_spice_stimulate_header(stimu_header_file_path, spice.spice_params.stimulate_params, vpr_clock_period, num_clock);

  /* design parameter header file */
  design_param_header_file_path = my_strcat(formatted_include_dir_path, design_param_header_file_name);
  fprint_spice_design_param_header(design_param_header_file_path, spice);
  
  return;
}
