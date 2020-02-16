#ifndef VERILOG_CONSTANTS_H
#define VERILOG_CONSTANTS_H

/* global parameters for dumping synthesizable verilog */

constexpr char* VERILOG_NETLIST_FILE_POSTFIX = ".v";
constexpr float VERILOG_SIM_TIMESCALE = 1e-9; // Verilog Simulation time scale (minimum time unit) : 1ns

constexpr char* VERILOG_TIMING_PREPROC_FLAG = "ENABLE_TIMING"; // the flag to enable timing definition during compilation
constexpr char* VERILOG_SIGNAL_INIT_PREPROC_FLAG = "ENABLE_SIGNAL_INITIALIZATION"; // the flag to enable signal initialization during compilation
constexpr char* VERILOG_FORMAL_VERIFICATION_PREPROC_FLAG = "ENABLE_FORMAL_VERIFICATION"; // the flag to enable formal verification during compilation
constexpr char* INITIAL_SIMULATION_FLAG = "INITIAL_SIMULATION"; // the flag to enable initial functional verification
constexpr char* AUTOCHECKED_SIMULATION_FLAG = "AUTOCHECKED_SIMULATION"; // the flag to enable autochecked functional verification
constexpr char* FORMAL_SIMULATION_FLAG = "FORMAL_SIMULATION"; // the flag to enable formal functional verification

constexpr char* DEFAULT_LB_DIR_NAME = "lb/";
constexpr char* DEFAULT_RR_DIR_NAME = "routing/";
constexpr char* DEFAULT_SUBMODULE_DIR_NAME = "sub_module/";
constexpr char* MODELSIM_SIMULATION_TIME_UNIT = "ms";

// Icarus variables and flag
constexpr char* ICARUS_SIMULATOR_FLAG = "ICARUS_SIMULATOR"; // the flag to enable specific Verilog code in testbenches
// End of Icarus variables and flag

constexpr char* VERILOG_TOP_POSTFIX = "_top.v";
constexpr char* FORMAL_VERIFICATION_VERILOG_FILE_POSTFIX = "_top_formal_verification.v"; 
constexpr char* TOP_TESTBENCH_VERILOG_FILE_POSTFIX = "_top_tb.v"; /* !!! must be consist with the modelsim_testbench_module_postfix */ 
constexpr char* AUTOCHECK_TOP_TESTBENCH_VERILOG_FILE_POSTFIX = "_autocheck_top_tb.v"; /* !!! must be consist with the modelsim_autocheck_testbench_module_postfix */ 
constexpr char* RANDOM_TOP_TESTBENCH_VERILOG_FILE_POSTFIX = "_formal_random_top_tb.v"; 
constexpr char* DEFINES_VERILOG_FILE_NAME = "fpga_defines.v";
constexpr char* DEFINES_VERILOG_SIMULATION_FILE_NAME = "define_simulation.v";
constexpr char* SUBMODULE_VERILOG_FILE_NAME = "sub_module.v";
constexpr char* LOGIC_BLOCK_VERILOG_FILE_NAME = "logic_blocks.v";
constexpr char* LUTS_VERILOG_FILE_NAME = "luts.v";
constexpr char* ROUTING_VERILOG_FILE_NAME = "routing.v";
constexpr char* MUXES_VERILOG_FILE_NAME = "muxes.v";
constexpr char* LOCAL_ENCODER_VERILOG_FILE_NAME = "local_encoder.v";
constexpr char* MEMORIES_VERILOG_FILE_NAME = "memories.v";
constexpr char* WIRES_VERILOG_FILE_NAME = "wires.v";
constexpr char* ESSENTIALS_VERILOG_FILE_NAME = "inv_buf_passgate.v";
constexpr char* CONFIG_PERIPHERAL_VERILOG_FILE_NAME = "config_peripherals.v";
constexpr char* USER_DEFINED_TEMPLATE_VERILOG_FILE_NAME = "user_defined_templates.v";

constexpr char* VERILOG_MUX_BASIS_POSTFIX = "_basis";

#endif
