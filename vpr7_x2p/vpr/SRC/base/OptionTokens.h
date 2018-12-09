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
    /* Xifan TANG: FPGA SPICE Support */
    OT_FPGA_SPICE, /* Xifan TANG: FPGA SPICE Model Support */
    OT_FPGA_SPICE_RENAME_ILLEGAL_PORT, 
    OT_FPGA_SPICE_SIGNAL_DENSITY_WEIGHT, /* The weight of signal density in determining number of clock cycles in simulation */
    OT_FPGA_SPICE_SIM_WINDOW_SIZE, /* Window size in determining number of clock cycles in simulation */
    OT_FPGA_SPICE_SIM_MT_NUM, /* number of multi-thread used in simulation */
    OT_SPICE_DIR, /* Xifan TANG: FPGA SPICE Model Support */
    OT_SPICE_PRINT_TOP_TESTBENCH, /* Xifan TANG: Print Top-level SPICE Testbench */
    OT_SPICE_PRINT_PB_MUX_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_SPICE_PRINT_CB_MUX_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_SPICE_PRINT_SB_MUX_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_SPICE_PRINT_CB_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for CBs */
    OT_SPICE_PRINT_SB_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for SBs */
    OT_SPICE_PRINT_GRID_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for Grids */
    OT_SPICE_PRINT_LUT_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for LUTs */
    OT_SPICE_PRINT_HARDLOGIC_TESTBENCH, /* Xifan TANG: Print SPICE Testbench for hard logic s */
    OT_FPGA_SPICE_LEAKAGE_ONLY, /* Xifan TANG: Print SPICE Testbench for MUXes */
    OT_FPGA_SPICE_PARASITIC_NET_ESTIMATION_OFF, /* Xifan TANG: turn off the parasitic net estimation*/
    OT_FPGA_SPICE_TESTBENCH_LOAD_EXTRACTION_OFF, /* Xifan TANG: turn off the testbench load extraction */
    /* Xifan TANG: Verilog Generation */
    OT_FPGA_VERILOG_SYN, /* Xifan TANG: Synthesizable Verilog Dump */
    OT_FPGA_VERILOG_SYN_DIR, /* Xifan TANG: Synthesizable Verilog Dump */
    OT_FPGA_VERILOG_SYN_PRINT_TOP_TB, /* Xifan TANG: Synthesizable Verilog Dump */
    OT_FPGA_VERILOG_SYN_PRINT_TOP_AUTO_TB, /* Xifan TANG: Synthesizable Verilog Dump */
    OT_FPGA_VERILOG_SYN_PRINT_INPUT_BLIF_TB, /* Xifan TANG: Synthesizable Verilog Dump */
    OT_FPGA_VERILOG_SYN_TB_SERIAL_CONFIG_MODE, /* Xifan TANG: Synthesizable Verilog Dump */
	OT_FPGA_VERILOG_SYN_INCLUDE_TIMING,	/* Include timing constraint in Verilog*/
	OT_FPGA_VERILOG_INIT_SIM,			/* AA: to allow initialization in simulation */
    OT_FPGA_VERILOG_SYN_PRINT_MODELSIM_AUTODECK, // To allow modelsim script generation
    OT_FPGA_VERILOG_SYN_MODELSIM_INI_PATH,       // To set modelsim script path
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
