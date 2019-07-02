/* Command-line options for VPR place_and_route */
/* Add any option by following the format of t_opt_info */
t_opt_info vpr_versapower_opts[] = {
  {"activity_file", "--activity_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the activity file"},
  {"power_properties", "--power_properties", 0, OPT_WITHVAL, OPT_CHAR, OPT_REQ, OPT_NONDEF, "Specify the power property XML file"},
  {"power_report_file", "--power_report_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the output power report file name"},
  {LAST_OPT_TAG, LAST_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"}
};

/* Function to execute the command */
void shell_execute_vpr_versapower(t_shell_env* env, t_opt_info* opts);
