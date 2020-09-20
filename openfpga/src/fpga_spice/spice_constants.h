#ifndef SPICE_CONSTANTS_H
#define SPICE_CONSTANTS_H

/* global parameters for dumping spice netlists */

constexpr char* SPICE_NETLIST_FILE_POSTFIX = ".sp";

constexpr char* TRANSISTOR_WRAPPER_POSTFIX = "_wrapper";

constexpr char* TRANSISTORS_SPICE_FILE_NAME = "transistor.sp";
constexpr char* SUPPLY_WRAPPER_SPICE_FILE_NAME = "supply_wrapper.sp";

constexpr char* SPICE_SUBCKT_VDD_PORT_NAME = "VDD";
constexpr char* SPICE_SUBCKT_GND_PORT_NAME = "VSS";

#endif
