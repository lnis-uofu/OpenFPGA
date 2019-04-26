#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"


#include "vpr_types.h"
#include "vpr_utils.h"
#include "globals.h"
#include "pack.h"
#include "vpr_api.h"

#include "read_opt_types.h"
#include "read_opt.h"
#include "shell_types.h"

void shell_execute_vpr_pack(t_shell_env* env, 
                            t_opt_info* opts) {
  char* temp = NULL;


  vpr_printf(TIO_MESSAGE_INFO,
             "Setup vpr_packer...\n");
  /* Packer File Names */
  /* Post packing netlist */
  temp = get_opt_val(opts, "net_file");
  if (NULL != temp) {
    env->vpr_setup.FileNameOpts.NetFile = temp;
  }

  /* SDC constraints */
  temp = get_opt_val(opts, "sdc_file");
  if (NULL != temp) {
    env->vpr_setup.Timing.SDCFile = temp;
  }

  /* Packer Options */
  if (env->arch.clb_grid.IsAuto) {
    env->vpr_setup.PackerOpts.aspect = env->arch.clb_grid.Aspect;
  } else {
    env->vpr_setup.PackerOpts.aspect = (float) env->arch.clb_grid.H / (float) env->arch.clb_grid.W;
  }
  env->vpr_setup.PackerOpts.output_file = my_strdup(env->vpr_setup.FileNameOpts.NetFile);

  env->vpr_setup.PackerOpts.blif_file_name = my_strdup(env->vpr_setup.FileNameOpts.BlifFile);

  /* Call this program, it means that we need to do packing */
  env->vpr_setup.PackerOpts.doPacking = TRUE; 

  env->vpr_setup.PackerOpts.global_clocks = is_opt_set(opts, "global_clocks", TRUE);

  env->vpr_setup.PackerOpts.hill_climbing_flag = is_opt_set(opts, "hill_climbing", FALSE);

  env->vpr_setup.PackerOpts.sweep_hanging_nets_and_inputs = is_opt_set(opts, "sweep_hanging_nets_and_inputs", TRUE);

  env->vpr_setup.PackerOpts.skip_clustering = is_opt_set(opts, "skip_clustering", FALSE);

  env->vpr_setup.PackerOpts.allow_unrelated_clustering = is_opt_set(opts, "allow_unrelated_clustering", TRUE);

  env->vpr_setup.PackerOpts.allow_early_exit = is_opt_set(opts, "allow_early_exit", FALSE);

  env->vpr_setup.PackerOpts.connection_driven = is_opt_set(opts, "connection_driven", TRUE);

  env->vpr_setup.PackerOpts.timing_driven = is_opt_set(opts, "timing_driven", TRUE);

  env->vpr_setup.PackerOpts.cluster_seed_type = (
      env->vpr_setup.PackerOpts.timing_driven ? VPACK_TIMING : VPACK_MAX_INPUTS); /* DEFAULT */
  /* Get the value if we have any */
  temp = get_opt_val(opts, "cluster_seed_type");
  if (NULL != temp) {
    if (0 == strcmp(temp, "timing")) {
      env->vpr_setup.PackerOpts.cluster_seed_type = VPACK_TIMING;
    } else if (0 == strcmp(temp, "max_inputs")) {
      env->vpr_setup.PackerOpts.cluster_seed_type = VPACK_MAX_INPUTS;
    }
  }
  /* Free */
  my_free(temp);

  env->vpr_setup.PackerOpts.alpha = get_opt_float_val(opts, "alpha", 0.75); /* DEFAULT */

  env->vpr_setup.PackerOpts.beta = get_opt_float_val(opts, "beta", 0.9); /* DEFAULT */


  /* never recomputer timing */
  env->vpr_setup.PackerOpts.recompute_timing_after = get_opt_int_val(opts, "recompute_timing_after", MAX_SHORT); /* DEFAULT */

  vpr_printf(TIO_MESSAGE_INFO,
             "Launch pack_algorithm...\n");

  env->vpr_setup.PackerOpts.block_delay = get_opt_int_val(opts, "block_delay", 0) ; /* DEFAULT */


  env->vpr_setup.PackerOpts.intra_cluster_net_delay = get_opt_float_val(opts, "intra_cluster_net_delay", 0.); /* DEFAULT */

  env->vpr_setup.PackerOpts.inter_cluster_net_delay = get_opt_float_val(opts, "inter_cluster_net_delay", 1.0); /* DEFAULT */



  env->vpr_setup.PackerOpts.auto_compute_inter_cluster_net_delay = is_opt_set(opts, "auto_compute_inter_cluster_net_delay", TRUE);


  env->vpr_setup.PackerOpts.packer_algorithm = PACK_GREEDY; /* DEFAULT */
  /* Get the value if we have any */
  temp = get_opt_val(opts, "algorithm");
  if (NULL != temp) {
    if (0 == strcmp(temp, "greedy")) {
      env->vpr_setup.PackerOpts.packer_algorithm = PACK_GREEDY;
    } else if (0 == strcmp(temp, "brute_force")) {
      env->vpr_setup.PackerOpts.packer_algorithm = PACK_BRUTE_FORCE;
    }
  }
  /* Free */
  my_free(temp);

  /* Xifan TANG: PACK_CLB_PIN_REMAP */ 
  env->vpr_setup.PackerOpts.pack_clb_pin_remap = is_opt_set(opts, "clb_pin_remap", FALSE); /* DEFAULT */

  /* TODO: check if we have done read_blif and read_arch !!! */

  /* Run VPR packer */

  vpr_printf(TIO_MESSAGE_INFO,
             "Launch vpr_packer...\n");
  vpr_pack(env->vpr_setup, env->arch);

  return; 
}
