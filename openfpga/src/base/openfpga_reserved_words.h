/********************************************************************
 * This file includes all the reserved words that are used in
 * naming module, blocks, instances and cells in FPGA X2P support,
 * including:
 * Verilog generation, SPICE generation and bitstream generation
 *******************************************************************/
#ifndef OPENFPGA_RESERVED_WORDS_H
#define OPENFPGA_RESERVED_WORDS_H

/* Grid naming constant strings */
constexpr char* GRID_MODULE_NAME_PREFIX = "grid_"; 

/* Memory naming constant strings */
constexpr char* GRID_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* SWITCH_BLOCK_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* CONNECTION_BLOCK_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* MEMORY_MODULE_POSTFIX = "_mem";

/* Multiplexer naming constant strings */
constexpr char* MUX_BASIS_MODULE_POSTFIX = "_basis";
constexpr char* GRID_MUX_INSTANCE_PREFIX = "mux_"; 
constexpr char* SWITCH_BLOCK_MUX_INSTANCE_PREFIX = "mux_"; 
constexpr char* CONNECTION_BLOCK_MUX_INSTANCE_PREFIX = "mux_"; 

/* Bitstream file strings */
constexpr char* BITSTREAM_XML_FILE_NAME_POSTFIX = "_bitstream.xml";

#endif
