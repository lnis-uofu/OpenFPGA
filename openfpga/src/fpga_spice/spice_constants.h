#ifndef SPICE_CONSTANTS_H
#define SPICE_CONSTANTS_H

/* global parameters for dumping spice netlists */
constexpr size_t SPICE_NETLIST_MAX_NUM_PORTS_PER_LINE = 10;

constexpr const char* SPICE_NETLIST_FILE_POSTFIX = ".sp";

constexpr const char* TRANSISTOR_WRAPPER_POSTFIX = "_wrapper";

constexpr const char* TRANSISTORS_SPICE_FILE_NAME = "transistor.sp";
constexpr const char* SUPPLY_WRAPPER_SPICE_FILE_NAME = "supply_wrapper.sp";
constexpr const char* MUX_PRIMITIVES_SPICE_FILE_NAME = "mux_primitives.sp";
constexpr const char* MUXES_SPICE_FILE_NAME = "muxes.sp";
constexpr const char* LUTS_SPICE_FILE_NAME = "luts.sp";
constexpr const char* MEMORIES_SPICE_FILE_NAME = "memories.sp";
constexpr const char* FABRIC_INCLUDE_SPICE_NETLIST_FILE_NAME =
  "fabric_netlists.sp";

constexpr const char* SPICE_SUBCKT_VDD_PORT_NAME = "VDD";
constexpr const char* SPICE_SUBCKT_GND_PORT_NAME = "VSS";

constexpr const char* SPICE_MUX_BASIS_POSTFIX = "_basis";
constexpr const char* SPICE_MEM_POSTFIX = "_mem";

constexpr const char* SB_SPICE_FILE_NAME_PREFIX = "sb_";
constexpr const char* LOGICAL_MODULE_SPICE_FILE_NAME_PREFIX = "logical_tile_";
constexpr const char* GRID_SPICE_FILE_NAME_PREFIX = "grid_";

#endif
