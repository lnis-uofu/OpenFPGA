#ifndef VERILOG_CONSTANTS_H
#define VERILOG_CONSTANTS_H

/* global parameters for dumping synthesizable verilog */

constexpr const char* VERILOG_NETLIST_FILE_POSTFIX = ".v";
constexpr float VERILOG_SIM_TIMESCALE =
  1e-9;  // Verilog Simulation time scale (minimum time unit) : 1ns

constexpr const char* VERILOG_TIMING_PREPROC_FLAG =
  "ENABLE_TIMING";  // the flag to enable timing definition during compilation
constexpr const char* MODELSIM_SIMULATION_TIME_UNIT = "ms";

constexpr const char* FABRIC_INCLUDE_VERILOG_NETLIST_FILE_NAME =
  "fabric_netlists.v";
constexpr const char* TOP_VERILOG_TESTBENCH_INCLUDE_NETLIST_FILE_NAME_POSTFIX =
  "_include_netlists.v";
constexpr const char* VERILOG_TOP_POSTFIX = "_top.v";
constexpr const char* FORMAL_VERIFICATION_VERILOG_FILE_POSTFIX =
  "_top_formal_verification.v";
constexpr const char* TOP_TESTBENCH_VERILOG_FILE_POSTFIX =
  "_top_tb.v"; /* !!! must be consist with the modelsim_testbench_module_postfix
                */
constexpr const char* AUTOCHECK_TOP_TESTBENCH_VERILOG_FILE_POSTFIX =
  "_autocheck_top_tb.v"; /* !!! must be consist with the
                            modelsim_autocheck_testbench_module_postfix */
constexpr const char* RANDOM_TOP_TESTBENCH_VERILOG_FILE_POSTFIX =
  "_formal_random_top_tb.v";
constexpr const char* DEFINES_VERILOG_FILE_NAME = "fpga_defines.v";
constexpr const char* SUBMODULE_VERILOG_FILE_NAME = "sub_module.v";
constexpr const char* LOGIC_BLOCK_VERILOG_FILE_NAME = "logic_blocks.v";
constexpr const char* LUTS_VERILOG_FILE_NAME = "luts.v";
constexpr const char* ROUTING_VERILOG_FILE_NAME = "routing.v";
constexpr const char* MUX_PRIMITIVES_VERILOG_FILE_NAME = "mux_primitives.v";
constexpr const char* MUXES_VERILOG_FILE_NAME = "muxes.v";
constexpr const char* LOCAL_ENCODER_VERILOG_FILE_NAME = "local_encoder.v";
constexpr const char* ARCH_ENCODER_VERILOG_FILE_NAME = "arch_encoder.v";
constexpr const char* MEMORIES_VERILOG_FILE_NAME = "memories.v";
constexpr const char* SHIFT_REGISTER_BANKS_VERILOG_FILE_NAME =
  "shift_register_banks.v";
constexpr const char* WIRES_VERILOG_FILE_NAME = "wires.v";
constexpr const char* ESSENTIALS_VERILOG_FILE_NAME = "inv_buf_passgate.v";
constexpr const char* CONFIG_PERIPHERAL_VERILOG_FILE_NAME =
  "config_peripherals.v";
constexpr const char* USER_DEFINED_TEMPLATE_VERILOG_FILE_NAME =
  "user_defined_templates.v";

constexpr const char* VERILOG_MUX_BASIS_POSTFIX = "_basis";
constexpr const char* VERILOG_MEM_POSTFIX = "_mem";

constexpr const char* SB_VERILOG_FILE_NAME_PREFIX = "sb_";
constexpr const char* LOGICAL_MODULE_VERILOG_FILE_NAME_PREFIX = "logical_tile_";
constexpr const char* GRID_VERILOG_FILE_NAME_PREFIX = "grid_";

constexpr const char* FORMAL_VERIFICATION_TOP_MODULE_POSTFIX =
  "_top_formal_verification";
constexpr const char* FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX = "_fm";
constexpr const char* FORMAL_VERIFICATION_TOP_MODULE_UUT_NAME =
  "U0_formal_verification";

constexpr const char* FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX =
  "_top_formal_verification_random_tb";

constexpr const char* VERILOG_FSDB_PREPROC_FLAG =
  "DUMP_FSDB";  // the flag to enable fsdb waveform output during compilation
constexpr const char* VERILOG_VCD_PREPROC_FLAG =
  "DUMP_VCD";  // the flag to enable vcd waveform output during compilation

#define VERILOG_DEFAULT_SIGNAL_INIT_VALUE 0

#endif
