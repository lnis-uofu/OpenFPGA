/********************************************************************
 * This file includes all the reserved words that are used in
 * naming module, blocks, instances and cells in FPGA X2P support,
 * including:
 * Verilog generation, SPICE generation and bitstream generation
 *******************************************************************/
#ifndef FPGA_X2P_RESERVED_WORDS_H
#define FPGA_X2P_RESERVED_WORDS_H

/* Grid naming constant strings */
constexpr char* GRID_MODULE_NAME_PREFIX = "grid_"; 

/* Memory naming constant strings */
constexpr char* GRID_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* SWITCH_BLOCK_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* CONNECTION_BLOCK_MEM_INSTANCE_PREFIX = "mem_"; 
constexpr char* MEMORY_MODULE_POSTFIX = "_mem";

#endif
