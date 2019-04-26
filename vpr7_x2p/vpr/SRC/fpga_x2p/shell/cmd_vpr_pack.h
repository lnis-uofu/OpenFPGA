/* Command-line options for Packer */
t_opt_info vpr_pack_opts[] = {
  /* Packer Options should be listed here */
  /* Packer File Options  */
  {"sdc_file", "--sdc_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "File name of SDC constraints"},
  {"net_file", "--net_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "File name of post-packing netlist"},
  /* Packer Options  */
  {"global_clocks", "--global_clocks", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"hill_climb", "--hill_climb", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"sweep_hanging_nets_and_inputs", "-s,--sweep", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"timing_driven", "--timing_driven", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"cluster_seed_type", "--seed_type", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"alpha", "--alpha", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"beta", "--beta", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"recompute_timing_after", "--recompute_timing_after", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"block_delay", "--block_delay", 0, OPT_WITHVAL, OPT_INT, OPT_OPT, OPT_NONDEF, ""},
  {"intra_cluster_net_delay", "--intra_cluster_net_delay", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"inter_cluster_net_delay", "--inter_cluster_net_delay", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, ""},
  {"auto_compute_inter_cluster_net_delay", "--auto_compute_inter_cluster_net_delay", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"skip_clustering", "--skip_clustering", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"allow_unrelated_clustering", "--allow_unrelated_clustering", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"allow_early_exit", "--allow_early_exit", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"connection_driven", "--connnection_driven", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"aspect", "--aspect", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {"algorithm", "--algorithm", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, ""},
  {HELP_OPT_TAG, HELP_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"},
  {LAST_OPT_TAG, LAST_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"}
};

/* Function to execute the command */
void shell_execute_vpr_pack(t_shell_env* env, 
                            t_opt_info* opts);
