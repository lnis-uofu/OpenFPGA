#ifndef VERILOG_TOP_TESTBENCH_CONSTANTS
#define VERILOG_TOP_TESTBENCH_CONSTANTS

/* begin namespace openfpga */
namespace openfpga {

constexpr char* TOP_TESTBENCH_REFERENCE_INSTANCE_NAME = "REF_DUT";
constexpr char* TOP_TESTBENCH_FPGA_INSTANCE_NAME = "FPGA_DUT";
constexpr char* TOP_TESTBENCH_SHARED_INPUT_POSTFIX = "_shared_input";
constexpr char* TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX = "_benchmark";
constexpr char* TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX = "_fpga";

constexpr char* TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX = "_flag";

constexpr char* TOP_TESTBENCH_PROG_TASK_NAME = "prog_cycle_task";

constexpr char* TOP_TESTBENCH_SIM_START_PORT_NAME = "sim_start";

constexpr char* TOP_TESTBENCH_ERROR_COUNTER = "nb_error";

constexpr char* TOP_TB_RESET_PORT_NAME = "__greset__";
constexpr char* TOP_TB_SET_PORT_NAME = "__gset__";
constexpr char* TOP_TB_PROG_RESET_PORT_NAME = "__prog_reset__";
constexpr char* TOP_TB_PROG_SET_PORT_NAME = "__prog_set_";
constexpr char* TOP_TB_CONFIG_DONE_PORT_NAME = "__config_done__";
constexpr char* TOP_TB_OP_CLOCK_PORT_NAME = "__op_clock__";
constexpr char* TOP_TB_OP_CLOCK_PORT_PREFIX = "__operating_clk_";
constexpr char* TOP_TB_PROG_CLOCK_PORT_NAME = "__prog_clock__";
constexpr char* TOP_TB_INOUT_REG_POSTFIX = "_reg__";
constexpr char* TOP_TB_CLOCK_REG_POSTFIX = "_reg__";
constexpr char* TOP_TB_BITSTREAM_LENGTH_VARIABLE = "BITSTREAM_LENGTH";
constexpr char* TOP_TB_BITSTREAM_WIDTH_VARIABLE = "BITSTREAM_WIDTH";
constexpr char* TOP_TB_BITSTREAM_MEM_REG_NAME = "bit_mem";
constexpr char* TOP_TB_BITSTREAM_INDEX_REG_NAME = "bit_index";
constexpr char* TOP_TB_BITSTREAM_ITERATOR_REG_NAME = "ibit";
constexpr char* TOP_TB_BITSTREAM_SKIP_FLAG_REG_NAME = "skip_bits";

constexpr char* AUTOCHECK_TOP_TESTBENCH_VERILOG_MODULE_POSTFIX = "_autocheck_top_tb";


} /* end namespace openfpga */

#endif
