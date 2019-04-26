/* Command-line options for VPR place_and_route */
/* Add any option by following the format of t_opt_info */
t_opt_info help_opts[] = {
  {LAST_OPT_TAG, LAST_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"}
};


void shell_execute_help(t_shell_env* env, t_opt_info* opts);
