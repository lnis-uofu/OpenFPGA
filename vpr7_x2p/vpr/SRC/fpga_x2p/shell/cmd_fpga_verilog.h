/* Command-line options for VPR place_and_route */
/* Add any option by following the format of t_opt_info */
t_opt_info fpga_verilog_opts[] = {
  {"output_dir", "--output_dir", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the output directory containing Verilog netlists"},
  {"print_top_testbench", "--print_top_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the top-level testbench for full FPGA fabric"},
  {"print_autocheck_top_testbench", "--print_autodeck_top_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the top-level testbench for full FPGA fabric with autocheck features"},
  {"reference_verilog_benchmark_file", "--reference_verilog_benchmark_file", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the reference Verilog benchmark file used by the autocheck top-level testbench"},
  {"print_input_blif_testbench", "--print_input_blif_testbench", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the testbench for the input blif benchmark"},
  {"print_formal_verification_top_netlist", "--print_formal_verification_top_netlist", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the top-level Verilog netlist for full FPGA fabric adapted to formal verification"},
  {"include_timing", "--include_timing", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Include timing annotation in the outputted Verilog netlists"},
  {"include_signal_init", "--include_signal_init", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Include signal initialization in the outputted Verilog netlists"},
  {"print_modelsim_autodeck", "--print_modelsim_autodeck", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Output the script to launch Modelsim simulation for the autocheck testbench"},
  {"modelsim_ini_path", "--modelsim_ini_path", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the path of modelsim.ini file"},
  {"print_user_defined_template", "--print_user_defined_template", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the Verilog template for user-defined modules"},
  {"print_report_timing_tcl", "--print_report_timing_tcl", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the TCL script in report timing purpose"},
  {"report_timing_dir_path", "--report_timing_dir_path", 0, OPT_WITHVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Specify the directory path of report timing results"},
  {"print_sdc_pnr", "--print_sdc_pnr", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the SDC file in P&R purpose"},
  {"print_sdc_analysis", "--print_sdc_analysis", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable output the SDC file in Timing/Power analysis purpose"},
  {"include_icarus_simulator", "--include_icarus_simulator", 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Enable Verilog preprocessing flags and features for Icarus simulator"},
  {HELP_OPT_TAG, HELP_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"},
  {LAST_OPT_TAG, LAST_OPT_NAME, 0, OPT_NONVAL, OPT_CHAR, OPT_OPT, OPT_NONDEF, "Launch help desk"}
};

/* Function to execute the command */
void shell_execute_fpga_verilog(t_shell_env* env, t_opt_info* opts);
