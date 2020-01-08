#ifndef OPTIONTOKENS_H
#define OPTIONTOKENS_H

/* The order of this does NOT matter, but do not give things specific values
 * or you will screw up the ability to count things properly */
enum e_OptionBaseToken {
	OT_SETTINGS_FILE,
	OT_NODISP,
	OT_AUTO,
	OT_RECOMPUTE_CRIT_ITER,
	OT_INNER_LOOP_RECOMPUTE_DIVIDER,
	OT_FIX_PINS,
	OT_FULL_STATS,
	OT_READ_PLACE_ONLY,
	OT_FAST,
	OT_CREATE_ECHO_FILE,
	OT_TIMING_ANALYSIS,
	OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY,
	OT_GENERATE_POST_SYNTHESIS_NETLIST,
	OT_INIT_T,
	OT_ALPHA_T,
	OT_EXIT_T,
	OT_INNER_NUM,
	OT_SEED,
	OT_PLACE_COST_EXP,
	OT_TD_PLACE_EXP_FIRST,
	OT_TD_PLACE_EXP_LAST,
	OT_PLACE_ALGORITHM,
	OT_TIMING_TRADEOFF,
	OT_ENABLE_TIMING_COMPUTATIONS,
	OT_BLOCK_DIST,
	OT_PLACE_CHAN_WIDTH,
	OT_MAX_ROUTER_ITERATIONS,
	OT_BB_FACTOR,
	OT_ROUTER_ALGORITHM,
	OT_FIRST_ITER_PRES_FAC,
	OT_INITIAL_PRES_FAC,
	OT_PRES_FAC_MULT,
	OT_ACC_FAC,
	OT_ASTAR_FAC,
	OT_MAX_CRITICALITY,
	OT_CRITICALITY_EXP,
	OT_BASE_COST_TYPE,
	OT_BEND_COST,
	OT_ROUTE_TYPE,
	OT_ROUTE_CHAN_WIDTH,
	OT_ROUTE,
	OT_PLACE,
	OT_VERIFY_BINARY_SEARCH,
	OT_OUTFILE_PREFIX,
	OT_BLIF_FILE,
	OT_NET_FILE,
	OT_PLACE_FILE,
	OT_ROUTE_FILE,
	OT_SDC_FILE,
	OT_GLOBAL_CLOCKS,
	OT_HILL_CLIMBING_FLAG,
	OT_SWEEP_HANGING_NETS_AND_INPUTS,
	OT_SKIP_CLUSTERING,
	OT_ALLOW_UNRELATED_CLUSTERING,
	OT_ALLOW_EARLY_EXIT,
	OT_CONNECTION_DRIVEN_CLUSTERING,
	OT_TIMING_DRIVEN_CLUSTERING,
	OT_CLUSTER_SEED,
	OT_ALPHA_CLUSTERING,
	OT_BETA_CLUSTERING,
	OT_RECOMPUTE_TIMING_AFTER,
	OT_CLUSTER_BLOCK_DELAY,
	OT_INTRA_CLUSTER_NET_DELAY,
	OT_INTER_CLUSTER_NET_DELAY,
	OT_PACK,
	OT_PACKER_ALGORITHM,
	OT_POWER,
	OT_ACTIVITY_FILE,
	OT_POWER_OUT_FILE,
	OT_CMOS_TECH_BEHAVIOR_FILE,
    /* Xifan Tang: Tileable routing support !!! */
    OT_USE_TILEABLE_ROUTE_CHAN_WIDTH,
    /* General FPGA_X2P: FPGA-SPICE/Verilog/Bitstream Options */
    OT_FPGA_X2P_RENAME_ILLEGAL_PORT, 
    OT_FPGA_X2P_SIGNAL_DENSITY_WEIGHT, /* The weight of signal density in determining number of clock cycles in simulation */
    OT_FPGA_X2P_SIM_WINDOW_SIZE, /* Window size in determining number of clock cycles in simulation */
    OT_FPGA_X2P_COMPACT_ROUTING_HIERARCHY, /* use a compact routing hierarchy in SPICE/Verilog generation */
    OT_FPGA_X2P_OUTPUT_SB_XML, /* output switch blocks to XML files */
    OT_FPGA_X2P_DUPLICATE_GRID_PIN, /* Duplicate the pins at each side of a grid when generating SPICE and Verilog netlists */
    /* Xifan TANG: FPGA SPICE Support */
    OT_FPGA_SPICE, /* Xifan TANG: FPGA SPICE Model Support */
    OT_FPGA_SPICE_DIR, /* Xifan TANG: FPGA SPICE Model Support */
    OT_FPGA_SPICE_PRINT_TOP_TESTBENCH, /* Xifan TANG: Print Top-level SPICE Testbench */
    OT_FPGA_SPICE_PRINT_PB_MUX_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_FPGA_SPICE_PRINT_CB_MUX_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_FPGA_SPICE_PRINT_SB_MUX_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_FPGA_SPICE_PRINT_CB_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for CBs */
    OT_FPGA_SPICE_PRINT_SB_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for SBs */
    OT_FPGA_SPICE_PRINT_GRID_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for Grids */
    OT_FPGA_SPICE_PRINT_LUT_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for LUTs */
    OT_FPGA_SPICE_PRINT_HARDLOGIC_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for hard logic s */
    OT_FPGA_SPICE_PRINT_IO_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for hard logic s */
    OT_FPGA_SPICE_LEAKAGE_ONLY, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_FPGA_SPICE_PARASITIC_NET_ESTIMATION, /* Xifan TANG: turn on/off the parasitic net estimation*/
    OT_FPGA_SPICE_TESTBENCH_LOAD_EXTRACTION, /* Xifan TANG: turn on/off the testbench load extraction */
    OT_FPGA_SPICE_SIMULATOR_PATH,
    OT_FPGA_SPICE_SIM_MT_NUM, /* number of multi-thread used in simulation */
    /* Xifan TANG: Verilog Generation */
    OT_FPGA_VERILOG_SYN, /* Xifan TANG: Synthesizable Verilog Dump */
    OT_FPGA_VERILOG_SYN_DIR, /* Xifan TANG: Synthesizable Verilog Dump */
    OT_FPGA_VERILOG_SYN_EXPLICIT_MAPPING, /* Baudouin Chauviere: explicit pin mapping during verilog generation */
    OT_FPGA_VERILOG_SYN_PRINT_TOP_TESTBENCH, /* Xifan Tang: Synthesizable Verilog, turn on option: output testbench for top-level netlist */
    OT_FPGA_VERILOG_SYN_PRINT_AUTOCHECK_TOP_TESTBENCH, /* Xifan Tang: Synthesizable Verilog, turn on option: output testbench for top-level netlist */
    OT_FPGA_VERILOG_SYN_PRINT_INPUT_BLIF_TESTBENCH, /* Xifan Tang: Synthesizable Verilog, turn on option: output testbench for the orignial input blif  */
    OT_FPGA_VERILOG_SYN_PRINT_FORMAL_VERIFICATION_TOP_NETLIST, /* Xifan Tang: Synthesizable Verilog, turn on option: output netlists in a compact way */
    OT_FPGA_VERILOG_SYN_INCLUDE_TIMING, /* Xifan TANG: Include timing constraints in Verilog */
    OT_FPGA_VERILOG_SYN_INCLUDE_SIGNAL_INIT, /* Xifan TANG: Include timing constraints in Verilog */
	OT_FPGA_VERILOG_SYN_INCLUDE_ICARUS_SIMULATOR,
    OT_FPGA_VERILOG_SYN_PRINT_MODELSIM_AUTODECK,
    OT_FPGA_VERILOG_SYN_PRINT_USER_DEFINED_TEMPLATE,
    OT_FPGA_VERILOG_SYN_PRINT_REPORT_TIMING_TCL,
    OT_FPGA_VERILOG_SYN_REPORT_TIMING_RPT_PATH,
    OT_FPGA_VERILOG_SYN_PRINT_SDC_PNR,
    OT_FPGA_VERILOG_SYN_PRINT_SDC_ANALYSIS,
    OT_FPGA_VERILOG_SYN_PRINT_SIMULATION_INI,
    OT_FPGA_VERILOG_SYN_SIMULATION_INI_FILE,
    /* Xifan Tang: Bitstream generator */
    OT_FPGA_BITSTREAM_GENERATOR,
    OT_FPGA_BITSTREAM_OUTPUT_FILE,
    /* mrFPGA: Xifan TANG */
    OT_SHOW_SRAM,
    OT_SHOW_PASS_TRANS,
    /* CLB PIN REMAP */
    OT_PACK_CLB_PIN_REMAP,
    OT_PLACE_CLB_PIN_REMAP,
    /* END */
	OT_BASE_UNKNOWN /* Must be last since used for counting enum items */
};

enum e_OptionArgToken {
	OT_ON,
	OT_OFF,
	OT_RANDOM,
	OT_BOUNDING_BOX,
	OT_NET_TIMING_DRIVEN,
	OT_PATH_TIMING_DRIVEN,
	OT_BREADTH_FIRST,
	OT_TIMING_DRIVEN,
	OT_NO_TIMING,
	OT_INTRINSIC_DELAY,
	OT_DELAY_NORMALIZED,
	OT_DEMAND_ONLY,
	OT_GLOBAL,
	OT_DETAILED,
	OT_TIMING,
	OT_MAX_INPUTS,
	OT_GREEDY,
	OT_LP,
	OT_BRUTE_FORCE,
	OT_ARG_UNKNOWN /* Must be last since used for counting enum items */
};

extern struct s_TokenPair OptionBaseTokenList[];
extern struct s_TokenPair OptionArgTokenList[];

#endif
