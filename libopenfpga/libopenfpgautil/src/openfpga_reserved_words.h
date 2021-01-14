/********************************************************************
 * This file includes all the reserved words that are used in
 * naming module, blocks, instances and cells in FPGA X2P support,
 * including:
 * Verilog generation, SPICE generation and bitstream generation
 *******************************************************************/
#ifndef OPENFPGA_RESERVED_WORDS_H
#define OPENFPGA_RESERVED_WORDS_H

/* begin namespace openfpga */
namespace openfpga {

/* Top-level module name */
constexpr char* FPGA_TOP_MODULE_NAME = "fpga_top";

constexpr char* CONFIGURABLE_MEMORY_CHAIN_IN_NAME = "ccff_head";
constexpr char* CONFIGURABLE_MEMORY_CHAIN_OUT_NAME = "ccff_tail";
constexpr char* CONFIGURABLE_MEMORY_DATA_OUT_NAME = "mem_out";
constexpr char* CONFIGURABLE_MEMORY_INVERTED_DATA_OUT_NAME = "mem_outb";

/* IO PORT */
/* Prefix of global input, output and inout ports of FPGA fabric */
constexpr char* GIO_INOUT_PREFIX = "gfpga_pad_";

/* Grid naming constant strings */
constexpr char* GRID_MODULE_NAME_PREFIX = "grid_"; 
constexpr char* LOGICAL_MODULE_NAME_PREFIX = "logical_tile_"; 

/* Memory naming constant strings */
constexpr char* GRID_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* SWITCH_BLOCK_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* CONNECTION_BLOCK_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* MEMORY_MODULE_POSTFIX = "_mem";
constexpr char* MEMORY_BL_PORT_NAME = "bl";
constexpr char* MEMORY_WL_PORT_NAME = "wl";

/* Multiplexer naming constant strings */
constexpr char* MUX_BASIS_MODULE_POSTFIX = "_basis";
constexpr char* GRID_MUX_INSTANCE_PREFIX = "mux_"; 
constexpr char* SWITCH_BLOCK_MUX_INSTANCE_PREFIX = "mux_"; 
constexpr char* CONNECTION_BLOCK_MUX_INSTANCE_PREFIX = "mux_"; 

/* Decoder naming constant strings */
constexpr char* DECODER_ENABLE_PORT_NAME = "enable";
constexpr char* DECODER_ADDRESS_PORT_NAME = "address";
constexpr char* DECODER_DATA_IN_PORT_NAME = "data_in";
constexpr char* DECODER_DATA_OUT_PORT_NAME = "data_out";
constexpr char* DECODER_DATA_OUT_INV_PORT_NAME = "data_out_inv";
constexpr char* DECODER_BL_ADDRESS_PORT_NAME = "bl_address";
constexpr char* DECODER_WL_ADDRESS_PORT_NAME = "wl_address";

/* Inverted port naming */
constexpr char* INV_PORT_POSTFIX = "_inv";

/* Bitstream file strings */
constexpr char* BITSTREAM_XML_FILE_NAME_POSTFIX = "_bitstream.xml";

constexpr char* DEFAULT_LB_DIR_NAME = "lb/";
constexpr char* DEFAULT_RR_DIR_NAME = "routing/";
constexpr char* DEFAULT_SUBMODULE_DIR_NAME = "sub_module/";

} /* end namespace openfpga */

#endif
