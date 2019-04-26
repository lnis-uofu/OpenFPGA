/* Command-line options for VPR place_and_route */
/* Add any option by following the format of t_opt_info */
t_opt_info fpga_bitstream_opts[] = {
  {"output_file", "--output_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the output file containing bitstream"},
  {HELP_OPT_TAG, HELP_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"},
  {LAST_OPT_TAG, LAST_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"}
};

/* Function to execute the command */
void shell_execute_fpga_bitstream(t_shell_env* env, t_opt_info* opts);
