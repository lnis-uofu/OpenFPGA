/* Command-line options for VPR place_and_route */
/* Add any option by following the format of t_opt_info */
t_opt_info fpga_spice_opts[] = {
  {"output_dir", "--output_dir", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the directory path of outputted SPICE netlists"},
  {"testbench_load_extraction", "-tle,--testbench_load_extraction", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable the load extraction when building SPICE testbenches"},
  {"parasitic_net_estimation", "-pne,--parasitic_net_estimation", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable the parasitic net estimation when building SPICE testbenches"},
  {"leakage_only", "--leakage_only", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Only measure leakage power when building SPICE testbenches"},
  {"print_top_testbench", "--print_top_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output top-level testbench for full FPGA fabrics"},
  {"print_pb_mux_testbench", "--print_pb_mux_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output multiplexer testbenches for Configurable Logic Blocks"},
  {"print_cb_mux_testbench", "--print_cb_mux_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output multiplexer testbenches for Connection Blocks"},
  {"print_sb_mux_testbench", "--print_sb_mux_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output multiplexer testbenches for Switch Blocks"},
  {"print_cb_testbench", "--print_cb_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output testbenches for Connection Blocks"},
  {"print_sb_testbench", "--print_sb_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output testbenches for Switch Blocks"},
  {"print_lut_testbench", "--print_lut_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output testbenches for Look-Up Tables"},
  {"print_hardlogic_testbench", "--print_hardlogic_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output testbenches for Hard Logics"},
  {"print_io_testbench", "--print_io_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output testbenches for IO pads"},
  {"print_grid_testbench", "--print_grid_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output testbenches for CLBs and Heterogeneous Blocks"},
  {"sim_multi_thread_num", "--sim_multi_thread_num", 0, OPT_WITHVAL, OPT_FLOAT, OPT_OPT, OPT_NONDEF, "Specify the number of threads used by simulator"},
  {"simulator_path", "--simulator_path", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the simulator path"},
  {HELP_OPT_TAG, HELP_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"},
  {LAST_OPT_TAG, LAST_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"}
};

/* Function to execute the command */
void shell_execute_fpga_spice(t_shell_env* env, t_opt_info* opts);
