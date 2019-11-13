#include "util.h"
#include "vpr_types.h"
#include "OptionTokens.h"

/* OptionBaseTokenList is for command line arg tokens. We will track how 
 * many times each of these things exist in a file */
struct s_TokenPair OptionBaseTokenList[] = {
		{ "settings_file", OT_SETTINGS_FILE }, { "nodisp", OT_NODISP }, {
				"auto", OT_AUTO }, { "recompute_crit_iter",
				OT_RECOMPUTE_CRIT_ITER }, { "inner_loop_recompute_divider",
				OT_INNER_LOOP_RECOMPUTE_DIVIDER }, { "fix_pins", OT_FIX_PINS },
		{ "full_stats", OT_FULL_STATS }, { "fast", OT_FAST }, { "echo_file",
				OT_CREATE_ECHO_FILE }, { "gen_postsynthesis_netlist",
				OT_GENERATE_POST_SYNTHESIS_NETLIST }, { "timing_analysis",
				OT_TIMING_ANALYSIS }, { "timing_analyze_only_with_net_delay",
				OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY },
		{ "init_t", OT_INIT_T }, { "alpha_t", OT_ALPHA_T }, { "exit_t",
				OT_EXIT_T }, { "inner_num", OT_INNER_NUM }, { "seed", OT_SEED },
		{ "place_cost_exp", OT_PLACE_COST_EXP }, { "td_place_exp_first",
				OT_TD_PLACE_EXP_FIRST }, { "td_place_exp_last",
				OT_TD_PLACE_EXP_LAST },
		{ "place_algorithm", OT_PLACE_ALGORITHM }, { "timing_tradeoff",
				OT_TIMING_TRADEOFF }, { "enable_timing_computations",
				OT_ENABLE_TIMING_COMPUTATIONS },
		{ "block_dist", OT_BLOCK_DIST }, { "place_chan_width",
				OT_PLACE_CHAN_WIDTH }, { "max_router_iterations",
				OT_MAX_ROUTER_ITERATIONS }, { "bb_factor", OT_BB_FACTOR }, {
				"router_algorithm", OT_ROUTER_ALGORITHM }, {
				"first_iter_pres_fac", OT_FIRST_ITER_PRES_FAC }, {
				"initial_pres_fac", OT_INITIAL_PRES_FAC }, { "pres_fac_mult",
				OT_PRES_FAC_MULT }, { "acc_fac", OT_ACC_FAC }, { "astar_fac",
				OT_ASTAR_FAC }, { "max_criticality", OT_MAX_CRITICALITY }, {
				"criticality_exp", OT_CRITICALITY_EXP }, { "base_cost_type",
				OT_BASE_COST_TYPE }, { "bend_cost", OT_BEND_COST }, {
				"route_type", OT_ROUTE_TYPE }, { "route_chan_width",
				OT_ROUTE_CHAN_WIDTH }, { "route", OT_ROUTE }, { "place",
				OT_PLACE }, { "verify_binary_search", OT_VERIFY_BINARY_SEARCH },
		{ "outfile_prefix", OT_OUTFILE_PREFIX }, { "blif_file", OT_BLIF_FILE },
		{ "net_file", OT_NET_FILE }, { "place_file", OT_PLACE_FILE }, {
				"route_file", OT_ROUTE_FILE }, { "sdc_file", OT_SDC_FILE }, {
				"global_clocks", OT_GLOBAL_CLOCKS }, { "hill_climbing",
				OT_HILL_CLIMBING_FLAG }, { "sweep_hanging_nets_and_inputs",
				OT_SWEEP_HANGING_NETS_AND_INPUTS }, { "no_clustering",
				OT_SKIP_CLUSTERING }, { "allow_unrelated_clustering",
				OT_ALLOW_UNRELATED_CLUSTERING }, { "allow_early_exit",
				OT_ALLOW_EARLY_EXIT }, { "connection_driven_clustering",
				OT_CONNECTION_DRIVEN_CLUSTERING }, { "timing_driven_clustering",
				OT_TIMING_DRIVEN_CLUSTERING }, { "cluster_seed_type",
				OT_CLUSTER_SEED }, { "alpha_clustering", OT_ALPHA_CLUSTERING },
		{ "beta_clustering", OT_BETA_CLUSTERING }, { "recompute_timing_after",
				OT_RECOMPUTE_TIMING_AFTER }, { "cluster_block_delay",
				OT_CLUSTER_BLOCK_DELAY }, { "intra_cluster_net_delay",
				OT_INTRA_CLUSTER_NET_DELAY }, { "inter_cluster_net_delay",
				OT_INTER_CLUSTER_NET_DELAY }, { "pack", OT_PACK }, {
				"packer_algorithm", OT_PACKER_ALGORITHM }, /**/
		{ "activity_file", OT_ACTIVITY_FILE }, /* Activity file */
		{ "power_output_file", OT_POWER_OUT_FILE }, /* Output file for power results */
		{ "power", OT_POWER }, /* Run power estimation? */
		{ "tech_properties", OT_CMOS_TECH_BEHAVIOR_FILE }, /* Technology properties */
        /* Xifan Tang: Tileable routing support !!! */
		{ "use_tileable_route_chan_width", OT_USE_TILEABLE_ROUTE_CHAN_WIDTH}, /* Enable adaption to tileable route chan_width  */
        /* General FPGA_X2P: FPGA-SPICE/Verilog/Bitstream Options */
        { "fpga_x2p_rename_illegal_port", OT_FPGA_X2P_RENAME_ILLEGAL_PORT }, /* Xifan TANG: rename illegal port names */
        { "fpga_x2p_signal_density_weight", OT_FPGA_X2P_SIGNAL_DENSITY_WEIGHT }, /* The weight of signal density */
        { "fpga_x2p_sim_window_size", OT_FPGA_X2P_SIM_WINDOW_SIZE }, /* Window size in determining number of clock cycles in simulation */
        { "fpga_x2p_compact_routing_hierarchy", OT_FPGA_X2P_COMPACT_ROUTING_HIERARCHY }, /* use a compact routing hierarchy in SPICE/Verilog generation */
        { "fpga_x2p_output_sb_xml", OT_FPGA_X2P_OUTPUT_SB_XML }, /* use a compact routing hierarchy in SPICE/Verilog generation */
        /* Xifan TANG: FPGA SPICE Support */
        { "fpga_spice", OT_FPGA_SPICE },/* Xifan TANG: SPICE Model Support, turn on the functionality*/
        { "fpga_spice_dir", OT_FPGA_SPICE_DIR },/* Xifan TANG: SPICE Model Support, directory of spice netlists*/
        { "fpga_spice_print_top_testbench", OT_FPGA_SPICE_PRINT_TOP_TESTBENCH }, /* Print the SPICE TOP Testbench for MUXes */
        { "fpga_spice_print_pb_mux_testbench", OT_FPGA_SPICE_PRINT_PB_MUX_TESTBENCH }, /* Print the SPICE Testbench for MUXes */
        { "fpga_spice_print_cb_mux_testbench", OT_FPGA_SPICE_PRINT_CB_MUX_TESTBENCH }, /* Print the SPICE Testbench for MUXes */
        { "fpga_spice_print_sb_mux_testbench", OT_FPGA_SPICE_PRINT_SB_MUX_TESTBENCH }, /* Print the SPICE Testbench for MUXes */
        { "fpga_spice_print_cb_testbench", OT_FPGA_SPICE_PRINT_CB_TESTBENCH }, /* Print the SPICE Testbench for CBs */
        { "fpga_spice_print_sb_testbench", OT_FPGA_SPICE_PRINT_SB_TESTBENCH }, /* Print the SPICE Testbench for SBs */
        { "fpga_spice_print_grid_testbench", OT_FPGA_SPICE_PRINT_GRID_TESTBENCH }, /* Print the SPICE Testbench for Grids  */
        { "fpga_spice_print_lut_testbench", OT_FPGA_SPICE_PRINT_LUT_TESTBENCH }, /* Print the SPICE Testbench for Grids  */
        { "fpga_spice_print_hardlogic_testbench", OT_FPGA_SPICE_PRINT_HARDLOGIC_TESTBENCH }, /* Print the SPICE Testbench for Grids  */
        { "fpga_spice_print_io_testbench", OT_FPGA_SPICE_PRINT_IO_TESTBENCH }, /* Print the SPICE Testbench for Grids  */
        { "fpga_spice_leakage_only", OT_FPGA_SPICE_LEAKAGE_ONLY }, /* Only simulate leakage power in FPGA SPICE */
        { "fpga_spice_parasitic_net_estimation", OT_FPGA_SPICE_PARASITIC_NET_ESTIMATION}, /* Xifan TANG: turn on/off the parasitic net estimation*/
        { "fpga_spice_testbench_load_extraction", OT_FPGA_SPICE_TESTBENCH_LOAD_EXTRACTION}, /* Xifan TANG: turn on/off the parasitic net estimation*/
        { "fpga_spice_simulator_path", OT_FPGA_SPICE_SIMULATOR_PATH}, /* Specify simulator path for SPICE netlists */
        { "fpga_spice_sim_mt_num", OT_FPGA_SPICE_SIM_MT_NUM }, /* number of multi-thread used in simulation */
        /* Xifan TANG: Synthsizable Verilog */
        { "fpga_verilog", OT_FPGA_VERILOG_SYN },
        { "fpga_verilog_dir", OT_FPGA_VERILOG_SYN_DIR },
        { "fpga_verilog_explicit_mapping", OT_FPGA_VERILOG_SYN_EXPLICIT_MAPPING },
        { "fpga_verilog_print_top_testbench", OT_FPGA_VERILOG_SYN_PRINT_TOP_TESTBENCH },
        { "fpga_verilog_print_autocheck_top_testbench", OT_FPGA_VERILOG_SYN_PRINT_AUTOCHECK_TOP_TESTBENCH },
        { "fpga_verilog_print_input_blif_testbench", OT_FPGA_VERILOG_SYN_PRINT_INPUT_BLIF_TESTBENCH },
        { "fpga_verilog_print_formal_verification_top_netlist", OT_FPGA_VERILOG_SYN_PRINT_FORMAL_VERIFICATION_TOP_NETLIST },
        { "fpga_verilog_include_timing", OT_FPGA_VERILOG_SYN_INCLUDE_TIMING }, /* Include timing constraints in Verilog netlists */
        { "fpga_verilog_include_signal_init", OT_FPGA_VERILOG_SYN_INCLUDE_SIGNAL_INIT }, /* Include signal initialization in Verilog netlists */
        { "fpga_verilog_include_icarus_simulator", OT_FPGA_VERILOG_SYN_INCLUDE_ICARUS_SIMULATOR }, /* Include/activate Icarus required functions in Verilog netlists */
        { "fpga_verilog_print_modelsim_autodeck", OT_FPGA_VERILOG_SYN_PRINT_MODELSIM_AUTODECK }, /* Generate autodeck scripts for modelsim */
        { "fpga_verilog_print_user_defined_template", OT_FPGA_VERILOG_SYN_PRINT_USER_DEFINED_TEMPLATE }, /* Specify the simulator path for Verilog netlists */
        { "fpga_verilog_print_report_timing_tcl", OT_FPGA_VERILOG_SYN_PRINT_REPORT_TIMING_TCL }, /* Specify the simulator path for Verilog netlists */
        { "fpga_verilog_report_timing_rpt_path", OT_FPGA_VERILOG_SYN_REPORT_TIMING_RPT_PATH }, /* Specify the simulator path for Verilog netlists */
        { "fpga_verilog_print_sdc_pnr", OT_FPGA_VERILOG_SYN_PRINT_SDC_PNR }, /* Specify the simulator path for Verilog netlists */
        { "fpga_verilog_print_sdc_analysis", OT_FPGA_VERILOG_SYN_PRINT_SDC_ANALYSIS }, /* Specify the simulator path for Verilog netlists */
        { "fpga_verilog_print_simulation_ini", OT_FPGA_VERILOG_SYN_PRINT_SIMULATION_INI }, /* Specify the simulator path for Verilog netlists */
        { "fpga_verilog_simulation_ini_file", OT_FPGA_VERILOG_SYN_SIMULATION_INI_FILE }, /* Specify the simulator path for Verilog netlists */
        /* Xifan Tang: Bitstream generator */
        { "fpga_bitstream_generator", OT_FPGA_BITSTREAM_GENERATOR }, /* turn on bitstream generator, and specify the output file */
 //       { "fpga_bitstream_output_file", OT_FPGA_BITSTREAM_OUTPUT_FILE }, /* turn on bitstream generator, and specify the output file */ // AA: temporarily deprecated
        /* mrFPGA: Xifan TANG */
        {"show_sram", OT_SHOW_SRAM},
        {"show_pass_trans", OT_SHOW_PASS_TRANS},
        /* END */
        /* CLB PIN REMAP */
        {"pack_clb_pin_remap", OT_PACK_CLB_PIN_REMAP},
        {"place_clb_pin_remap", OT_PLACE_CLB_PIN_REMAP},
        /* END */
		{ NULL, OT_BASE_UNKNOWN } /* End of list marker */
};

struct s_TokenPair OptionArgTokenList[] = { { "on", OT_ON }, { "off", OT_OFF },
		{ "random", OT_RANDOM }, { "bounding_box", OT_BOUNDING_BOX }, {
				"net_timing_driven", OT_NET_TIMING_DRIVEN }, {
				"path_timing_driven", OT_PATH_TIMING_DRIVEN }, {
				"breadth_first", OT_BREADTH_FIRST }, { "timing_driven",
				OT_TIMING_DRIVEN }, { "NO_TIMING", OT_NO_TIMING }, {
				"intrinsic_delay", OT_INTRINSIC_DELAY }, { "delay_normalized",
				OT_DELAY_NORMALIZED }, { "demand_only", OT_DEMAND_ONLY }, {
				"global", OT_GLOBAL }, { "detailed", OT_DETAILED }, { "timing",
				OT_TIMING }, { "max_inputs", OT_MAX_INPUTS }, { "greedy",
				OT_GREEDY }, { "lp", OT_LP }, { "brute_force", OT_BRUTE_FORCE },
		{ NULL, OT_BASE_UNKNOWN } /* End of list marker */
};
