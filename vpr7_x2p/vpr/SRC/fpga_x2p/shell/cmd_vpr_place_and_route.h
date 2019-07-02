/* Command-line options for VPR place_and_route */
/* Add any option by following the format of t_opt_info */
t_opt_info vpr_place_and_route_opts[] = {
  /* File names */
  {"net_file", "--net_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the post-packing netlist"},
  /* General options */
  {"timing_driven", "--timing_driven", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify if algorithms are timing-driven"},
  /* Placer Options should be listed here */
  {"block_dist", "--block_dist", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"inner_loop_recompute_divider", "--inner_loop_recompute_divider", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"place_cost_exp", "--place_cost_exp", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"place_exp_first", "--place_exp_first", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"place_exp_last", "--place_exp_last", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"placer_algorithm", "--placer_algorithm", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Available algorithms of Placer: bounding_box|net_timing_driven|path_timing_driven"},
  {"pad_loc_type", "--pad_loc_type", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the location type of IO pads: free|user|random"},
  {"pad_loc_file", "--pad_loc_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the filename to constrain the location of IO pads"},
  {"place_chan_width", "--place_chan_width", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, "Specify the routing channel width used by placer"},
  {"recompute_crit_iter", "--recompute_crit_iter", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"timing_tradeoff", "--timing_tradeoff", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"seed", "--seed", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"place_clb_pin_remap", "--place_clb_pin_remap", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"place_freq", "--place_freq", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  /* For annealing */
  {"alpha_t", "--alpha_t", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"exit_t", "--exit_t", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"init_t", "--init_t", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"inner_num", "--inner_num", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  /* Router Options should be listed here */
  {"astar_fac", "--astar_fac", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"fast", "--fast", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"bb_factor", "--bb_factor", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"criticality_exp", "--criticality_exp", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"max_criticality", "--max_criticality", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"max_router_iterations", "--max_router_iterations", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"pres_fac_mult", "--pres_fac_mult", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"router_type", "--router_type", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"full_stats", "--full_stats", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"verify_binary_search", "--verify_binary_search", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"router_algorithm", "--router_algorithm", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"route_channel_width", "--route_chan_width", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"show_sram", "--show_sram", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"show_pass_trans", "--show_pass_trans", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"initial_pres_fac", "--initial_pres_fac", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"base_cost_type", "--base_cost_type", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"first_iter_pres_fac", "--first_iter_pres_fac", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"acc_fac", "--acc_fac", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"bend_cost", "--bend_cost", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {HELP_OPT_TAG, HELP_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"},
  {LAST_OPT_TAG, LAST_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"}
};

/* Function to execute the command */
void shell_execute_vpr_place_and_route(t_shell_env* env, t_opt_info* opts);



