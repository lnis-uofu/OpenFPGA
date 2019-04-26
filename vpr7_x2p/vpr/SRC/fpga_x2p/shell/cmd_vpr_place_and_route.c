#include <string.h>
/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"
#include "vpr_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"

#include "read_opt_types.h"
#include "read_opt.h"
#include "shell_types.h"

#include "vpr_api.h"

void shell_init_vpr_placer(t_shell_env* env, t_opt_info* opts) {
  char* temp = NULL;

  /* Placer File names */
  /* Post packing netlist */
  temp = get_opt_val(opts, "net_file");
  if (NULL != temp) {
    env->vpr_setup.FileNameOpts.NetFile = temp;
  }

  /* Post placement netlist */
  temp = get_opt_val(opts, "place_file");
  if (NULL != temp) {
    env->vpr_setup.FileNameOpts.PlaceFile = temp;
  }

  /* Placer Options */
  env->vpr_setup.TimingEnabled = is_opt_set(opts, "timing_driven", TRUE); /* DEFAULT */

  /* Set seed for pseudo-random placement, default seed to 1 */
  env->vpr_setup.PlacerOpts.seed = get_opt_int_val(opts, "seed", 1); /* DEFAULT */
  my_srandom(env->vpr_setup.PlacerOpts.seed);

  env->vpr_setup.PlacerOpts.block_dist = get_opt_int_val(opts, "block_dist", 1); /* DEFAULT */

  env->vpr_setup.PlacerOpts.inner_loop_recompute_divider = get_opt_int_val(opts, "inner_loop_recompute_divider", 0); /* DEFAULT */

  env->vpr_setup.PlacerOpts.place_cost_exp = get_opt_float_val(opts, "place_cost_exp", 1.); /* DEFAULT */

  env->vpr_setup.PlacerOpts.td_place_exp_first = get_opt_float_val(opts, "place_exp_first", 1.); /* DEFAULT */

  env->vpr_setup.PlacerOpts.td_place_exp_last = get_opt_float_val(opts, "td_place_exp_last", 8.); /* DEFAULT */

  env->vpr_setup.PlacerOpts.place_algorithm = BOUNDING_BOX_PLACE; /* DEFAULT */
  if (TRUE == env->vpr_setup.TimingEnabled) { /* DEFAULT */
    env->vpr_setup.PlacerOpts.place_algorithm = PATH_TIMING_DRIVEN_PLACE; /* DEFAULT */
  }
  temp = get_opt_val(opts, "placer_algorithm");
  if (NULL != temp) {
    if (0 == strcmp(temp, "bounding_box")) {
      env->vpr_setup.PlacerOpts.place_algorithm = BOUNDING_BOX_PLACE;
    } else if (0 == strcmp(temp, "net_timing_driven")) {
      env->vpr_setup.PlacerOpts.place_algorithm = NET_TIMING_DRIVEN_PLACE;
    } else if (0 == strcmp(temp, "path_timing_driven")) {
      env->vpr_setup.PlacerOpts.place_algorithm = PATH_TIMING_DRIVEN_PLACE;;
    }
  }
  /* Free */
  my_free(temp);

  env->vpr_setup.PlacerOpts.pad_loc_file = get_opt_val(opts, "pad_loc_file");

  env->vpr_setup.PlacerOpts.pad_loc_type = FREE; /* DEFAULT */
  temp = get_opt_val(opts, "pad_loc_type");
  if (NULL != temp) {
    if (0 == strcmp(temp, "free")) {
      env->vpr_setup.PlacerOpts.pad_loc_type = FREE;
    } else if (0 == strcmp(temp, "user")) {
      env->vpr_setup.PlacerOpts.pad_loc_type = USER;
    } else if (0 == strcmp(temp, "random")) {
      env->vpr_setup.PlacerOpts.pad_loc_type = RANDOM;
    }
  }
  /* Free */
  my_free(temp);

  /* Check */
  if (  (USER == env->vpr_setup.PlacerOpts.pad_loc_type) 
     && (NULL == env->vpr_setup.PlacerOpts.pad_loc_file)) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "VPR placer is configured to user-defined pad location while the location file is missing!\n");
    return;
  }

  env->vpr_setup.PlacerOpts.place_chan_width = get_opt_int_val(opts, "place_chan_width", 100); /* DEFAULT */

  env->vpr_setup.PlacerOpts.recompute_crit_iter = get_opt_int_val(opts, "place_chan_width", 1); /* DEFAULT */

  env->vpr_setup.PlacerOpts.timing_tradeoff = get_opt_float_val(opts, "timing_tradeoff", 0.5); /* DEFAULT */

  /* Xifan TANG : PLACE_CLB_PIN_REMAP */
  env->vpr_setup.PlacerOpts.place_clb_pin_remap = is_opt_set(opts, "place_clb_pin_remap", FALSE); /* DEFAULT */
  /* END */

  /* Depends on env->vpr_setup.PlacerOpts.place_algorithm */
  env->vpr_setup.PlacerOpts.enable_timing_computations = FALSE; /* DEFAULT */
  if ((env->vpr_setup.PlacerOpts.place_algorithm == PATH_TIMING_DRIVEN_PLACE)
      || (env->vpr_setup.PlacerOpts.place_algorithm == NET_TIMING_DRIVEN_PLACE)) {
    env->vpr_setup.PlacerOpts.enable_timing_computations = TRUE; /* DEFAULT */
  }

  /* Annealing options */
  env->vpr_setup.AnnealSched.alpha_t = get_opt_float_val(opts, "alpha_t", 0.8); /* DEFAULT */
  if (env->vpr_setup.AnnealSched.alpha_t >= 1. || env->vpr_setup.AnnealSched.alpha_t <= 0.) {
    vpr_printf(TIO_MESSAGE_ERROR,
        "alpha_t(%.2g) must be between 0 and 1 exclusive.\n",
        env->vpr_setup.AnnealSched.alpha_t);
    return;
  }

  env->vpr_setup.AnnealSched.exit_t = get_opt_float_val(opts, "exit_t", 0.01); /* DEFAULT */
  if (env->vpr_setup.AnnealSched.exit_t <= 0.) {
    vpr_printf(TIO_MESSAGE_ERROR, "exit_t must be greater than 0.\n");
    return;
  }

  env->vpr_setup.AnnealSched.init_t = get_opt_float_val(opts, "init_t", 100.0); /* DEFAULT */
  if (env->vpr_setup.AnnealSched.init_t <= 0.) {
    vpr_printf(TIO_MESSAGE_ERROR, "init_t must be greater than 0.\n");
    return;
  }
  if (env->vpr_setup.AnnealSched.init_t < env->vpr_setup.AnnealSched.exit_t) {
    vpr_printf(TIO_MESSAGE_ERROR,
        "init_t must be greater or equal to than exit_t.\n");
    return;
  }

  env->vpr_setup.AnnealSched.inner_num = get_opt_float_val(opts, "inner_num", 1.0); /* DEFAULT */
  if (env->vpr_setup.AnnealSched.inner_num <= 0) {
    vpr_printf(TIO_MESSAGE_ERROR, "init_t must be greater than 0.\n");
    return;
  }

  env->vpr_setup.AnnealSched.type = AUTO_SCHED; /* DEFAULT */
  if (   (TRUE == is_opt_set(opts, "alpha_t", FALSE)) 
      || (TRUE == is_opt_set(opts, "exit_it", FALSE))
      || (TRUE == is_opt_set(opts, "init_it", FALSE))) {
    env->vpr_setup.AnnealSched.type = USER_SCHED;
  }

  /* Enable Placer*/
  env->vpr_setup.PlacerOpts.doPlacement = TRUE;

  return;
}

void shell_init_vpr_router(t_shell_env* env, t_opt_info* opts) {
  char* temp = NULL;

  /* Router File names */
  /* Post packing netlist */
  temp = get_opt_val(opts, "net_file");
  if (NULL != temp) {
    env->vpr_setup.FileNameOpts.NetFile = temp;
  }

  /* Post placement netlist */
  temp = get_opt_val(opts, "place_file");
  if (NULL != temp) {
    env->vpr_setup.FileNameOpts.PlaceFile = temp;
  }

  /* Post placement netlist */
  temp = get_opt_val(opts, "route_file");
  if (NULL != temp) {
    env->vpr_setup.FileNameOpts.RouteFile = temp;
  }

  /* Router Options */
  env->vpr_setup.TimingEnabled = is_opt_set(opts, "timing_driven", TRUE); /* DEFAULT */

  env->vpr_setup.RouterOpts.astar_fac = get_opt_float_val(opts, "astar_fac", 1.2); /* DEFAULT */

  if (TRUE == is_opt_set(opts, "fast", FALSE)) {
    env->vpr_setup.RouterOpts.bb_factor = get_opt_int_val(opts, "astar_fac", 0); /* DEFAULT */
  } else { 
    env->vpr_setup.RouterOpts.bb_factor = get_opt_int_val(opts, "astar_fac", 3); /* DEFAULT */
  }

  env->vpr_setup.RouterOpts.criticality_exp = get_opt_float_val(opts, "criticality_exp", 1.0); /* DEFAULT */

  env->vpr_setup.RouterOpts.max_criticality = get_opt_float_val(opts, "max_criticality", 0.99); /* DEFAULT */

  if (TRUE == is_opt_set(opts, "fast", FALSE)) {
    env->vpr_setup.RouterOpts.max_router_iterations = get_opt_int_val(opts, "max_router_iterations", 10); /* DEFAULT */
  } else {
    env->vpr_setup.RouterOpts.max_router_iterations = get_opt_int_val(opts, "max_router_iterations", 50); /* DEFAULT */
  }

  env->vpr_setup.RouterOpts.pres_fac_mult = get_opt_float_val(opts, "pres_fac_mult", 1.3); /* DEFAULT */

  env->vpr_setup.RouterOpts.route_type = DETAILED; /* DEFAULT */
  temp = get_opt_val(opts, "router_type");
  if (NULL != temp) {
    if ( 0 == strcmp(temp, "detailed") ) {
      env->vpr_setup.RouterOpts.route_type = DETAILED;
    } else if ( 0 == strcmp(temp, "global") ) {
      env->vpr_setup.RouterOpts.route_type = GLOBAL;
    }
  }
  /* Free */
  my_free(temp);

  env->vpr_setup.RouterOpts.full_stats = is_opt_set(opts, "full_stats", FALSE); /* DEFAULT */

  env->vpr_setup.RouterOpts.verify_binary_search = is_opt_set(opts, "verify_binary_search", FALSE); /* DEFAULT */

  /* Depends on RouteOpts->route_type */
  env->vpr_setup.RouterOpts.router_algorithm = NO_TIMING; /* DEFAULT */
  if (env->vpr_setup.TimingEnabled) {
    env->vpr_setup.RouterOpts.router_algorithm = TIMING_DRIVEN; /* DEFAULT */
  }
  if (GLOBAL == env->vpr_setup.RouterOpts.route_type) {
    env->vpr_setup.RouterOpts.router_algorithm = NO_TIMING; /* DEFAULT */
  }
  temp = get_opt_val(opts, "router_algorithm");
  if (NULL != temp) {
    if ( 0 == strcmp(temp, "no_timing") ) {
      env->vpr_setup.RouterOpts.router_algorithm = NO_TIMING;
    } else if ( 0 == strcmp(temp, "timing_driven") ) {
      env->vpr_setup.RouterOpts.router_algorithm = TIMING_DRIVEN;
    } else if ( 0 == strcmp(temp, "breadth_first") ) {
      env->vpr_setup.RouterOpts.router_algorithm = BREADTH_FIRST;
    }
  }
  /* Free */
  my_free(temp);

  env->vpr_setup.RouterOpts.fixed_channel_width = NO_FIXED_CHANNEL_WIDTH; /* DEFAULT */
  temp = get_opt_val(opts, "route_chan_width");
  if (NULL != temp) {
    env->vpr_setup.RouterOpts.fixed_channel_width = process_int_arg(temp);
  }
  /* Free */
  my_free(temp);

  /* mrFPGA: Xifan TANG */
  is_show_sram = is_opt_set(opts, "show_sram", FALSE);
  is_show_pass_trans = is_opt_set(opts, "show_pass_trans", FALSE);
  /* END */

  /* Depends on env->vpr_setup.RouterOpts.router_algorithm */
  if (NO_TIMING == env->vpr_setup.RouterOpts.router_algorithm || is_opt_set(opts, "fast", FALSE)) {
    env->vpr_setup.RouterOpts.initial_pres_fac = get_opt_int_val(opts, "initial_pres_fac", 10000.0); /* DEFAULT */
  } else {
    env->vpr_setup.RouterOpts.initial_pres_fac = get_opt_int_val(opts, "initial_pres_fac", 0.5); /* DEFAULT */
  }

  /* Depends on env->vpr_setup.RouterOpts.router_algorithm */
  env->vpr_setup.RouterOpts.base_cost_type = DELAY_NORMALIZED; /* DEFAULT */
  if (BREADTH_FIRST == env->vpr_setup.RouterOpts.router_algorithm) {
    env->vpr_setup.RouterOpts.base_cost_type = DEMAND_ONLY; /* DEFAULT */
  }
  if (NO_TIMING == env->vpr_setup.RouterOpts.router_algorithm) {
    env->vpr_setup.RouterOpts.base_cost_type = DEMAND_ONLY; /* DEFAULT */
  }
  temp = get_opt_val(opts, "base_cost_type");
  if (NULL != temp) {
    if ( 0 == strcmp(temp, "demand_only") ) {
      env->vpr_setup.RouterOpts.base_cost_type = DEMAND_ONLY;
    } else if ( 0 == strcmp(temp, "intrinsic_delay") ) {
      env->vpr_setup.RouterOpts.base_cost_type = INTRINSIC_DELAY;
    } else if ( 0 == strcmp(temp, "delay_normalized") ) {
      env->vpr_setup.RouterOpts.base_cost_type = DELAY_NORMALIZED;
    }
  }
  /* Free */
  my_free(temp);

  /* Depends on env->vpr_setup.RouterOpts.router_algorithm */
  if (BREADTH_FIRST == env->vpr_setup.RouterOpts.router_algorithm) {
    env->vpr_setup.RouterOpts.first_iter_pres_fac = get_opt_float_val(opts, "first_iter_pres_fac", 0.0); /* DEFAULT */
  } else if ( (NO_TIMING == env->vpr_setup.RouterOpts.router_algorithm)
    || (TRUE == is_opt_set(opts, "fast", FALSE)) ) {
    env->vpr_setup.RouterOpts.first_iter_pres_fac = get_opt_float_val(opts, "first_iter_pres_fac", 10000.0); /* DEFAULT */
  } else {
    env->vpr_setup.RouterOpts.first_iter_pres_fac = get_opt_float_val(opts, "first_iter_pres_fac", 0.5); /* DEFAULT */
  }

  /* Depends on env->vpr_setup.RouterOpts.router_algorithm */
  if (BREADTH_FIRST == env->vpr_setup.RouterOpts.router_algorithm) {
    env->vpr_setup.RouterOpts.acc_fac = get_opt_float_val(opts, "acc_fac", 0.2);
  } else {
    env->vpr_setup.RouterOpts.acc_fac = get_opt_float_val(opts, "acc_fac", 1.0);
  }

  /* Depends on env->vpr_setup.RouterOpts.route_type */
  if (GLOBAL == env->vpr_setup.RouterOpts.route_type) {
    env->vpr_setup.RouterOpts.bend_cost = get_opt_float_val(opts, "bend_cost", 1.0); /* DEFAULT */
  } else {
    env->vpr_setup.RouterOpts.bend_cost = get_opt_float_val(opts, "bend_cost", 0.0); /* DEFAULT */
  }

  /* Enable router */
  env->vpr_setup.RouterOpts.doRouting = TRUE;

  return;
}

void shell_init_vpr_place_and_route(t_shell_env* env, t_opt_info* opts) {

  shell_init_vpr_placer(env, opts);

  shell_init_vpr_router(env, opts);

  env->vpr_setup.PlacerOpts.place_freq = PLACE_ONCE; /* DEFAULT */
  if ( (TRUE == is_opt_set(opts, "place_chan_width", FALSE))
    || (TRUE == is_opt_set(opts, "route_chan_width", FALSE)) ) {
    env->vpr_setup.PlacerOpts.place_freq = PLACE_ONCE;
  }

  return;
}

void shell_execute_vpr_place_and_route(t_shell_env* env, t_opt_info* opts) {

  shell_init_vpr_place_and_route(env, opts);
  
  vpr_init_pre_place_and_route(env->vpr_setup, env->arch);

  vpr_place_and_route(env->vpr_setup, env->arch);

  return;
}
