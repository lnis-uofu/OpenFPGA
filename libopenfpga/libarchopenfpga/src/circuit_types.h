/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef CIRCUIT_TYPES_H
#define CIRCUIT_TYPES_H

#include <string>
#include <array>

/************************************************************************
 * This file includes basic enumeration types for circuit models
 ***********************************************************************/
/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */

/*Struct for a CIRCUIT model of a module*/
enum e_circuit_model_type {
  CIRCUIT_MODEL_CHAN_WIRE, 
  CIRCUIT_MODEL_WIRE, 
  CIRCUIT_MODEL_MUX, 
  CIRCUIT_MODEL_LUT, 
  CIRCUIT_MODEL_FF, 
  CIRCUIT_MODEL_SRAM, 
  CIRCUIT_MODEL_HARDLOGIC,
  CIRCUIT_MODEL_CCFF,
  CIRCUIT_MODEL_IOPAD, 
  CIRCUIT_MODEL_INVBUF, 
  CIRCUIT_MODEL_PASSGATE, 
  CIRCUIT_MODEL_GATE,
  NUM_CIRCUIT_MODEL_TYPES
};
/* Strings correspond to each port type */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_TYPES> CIRCUIT_MODEL_TYPE_STRING = {{"chan_wire", "wire", "mux", "lut", "ff", "sram", "hard_logic", "ccff", "iopad", "inv_buf", "pass_gate", "gate"}};

enum e_circuit_model_design_tech {
  CIRCUIT_MODEL_DESIGN_CMOS, 
  CIRCUIT_MODEL_DESIGN_RRAM,
  NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES
};
/* Strings correspond to each design technology type */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES> CIRCUIT_MODEL_DESIGN_TECH_TYPE_STRING = {{"cmos", "rram"}};

enum e_circuit_model_structure {
  CIRCUIT_MODEL_STRUCTURE_TREE, 
  CIRCUIT_MODEL_STRUCTURE_ONELEVEL, 
  CIRCUIT_MODEL_STRUCTURE_MULTILEVEL, 
  CIRCUIT_MODEL_STRUCTURE_CROSSBAR,
  NUM_CIRCUIT_MODEL_STRUCTURE_TYPES
};
/* Strings correspond to each type of mux structure */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_STRUCTURE_TYPES> CIRCUIT_MODEL_STRUCTURE_TYPE_STRING = {{"tree", "one_level", "multi_level", "crossbar"}};

enum e_circuit_model_buffer_type {
  CIRCUIT_MODEL_BUF_INV, 
  CIRCUIT_MODEL_BUF_BUF,
  NUM_CIRCUIT_MODEL_BUF_TYPES
};
/* Strings correspond to each type of buffer */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_BUF_TYPES> CIRCUIT_MODEL_BUFFER_TYPE_STRING = {{"inverter", "buffer"}};

enum e_circuit_model_pass_gate_logic_type {
  CIRCUIT_MODEL_PASS_GATE_TRANSMISSION, 
  CIRCUIT_MODEL_PASS_GATE_TRANSISTOR,
  CIRCUIT_MODEL_PASS_GATE_RRAM,         /* RRAM can be treated as a special type of pass-gate logic */
  CIRCUIT_MODEL_PASS_GATE_STDCELL,         /* Standard cell as a special type of pass-gate logic */
  NUM_CIRCUIT_MODEL_PASS_GATE_TYPES
};
/* Strings correspond to each type of buffer */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_PASS_GATE_TYPES> CIRCUIT_MODEL_PASSGATE_TYPE_STRING = {{"transmission_gate", "pass_transistor", "rram", "standard_cell"}};

enum e_circuit_model_gate_type {
  CIRCUIT_MODEL_GATE_AND, 
  CIRCUIT_MODEL_GATE_OR,
  CIRCUIT_MODEL_GATE_MUX2,
  NUM_CIRCUIT_MODEL_GATE_TYPES
};
/* Strings correspond to each type of logic gate */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_GATE_TYPES> CIRCUIT_MODEL_GATE_TYPE_STRING = {{"AND", "OR", "MUX2"}};

enum e_wire_model_type {
  WIRE_MODEL_PI,
  WIRE_MODEL_T,
  NUM_WIRE_MODEL_TYPES
};
/* Strings correspond to each type of logic gate */
constexpr std::array<const char*, NUM_WIRE_MODEL_TYPES> WIRE_MODEL_TYPE_STRING = {{"pi", "t"}};

enum e_circuit_model_port_type {
  CIRCUIT_MODEL_PORT_INPUT, 
  CIRCUIT_MODEL_PORT_OUTPUT, 
  CIRCUIT_MODEL_PORT_INOUT, 
  CIRCUIT_MODEL_PORT_CLOCK, 
  CIRCUIT_MODEL_PORT_SRAM,
  CIRCUIT_MODEL_PORT_BL,
  CIRCUIT_MODEL_PORT_BLB,
  CIRCUIT_MODEL_PORT_WL,
  CIRCUIT_MODEL_PORT_WLB,
  CIRCUIT_MODEL_PORT_WLR,
  NUM_CIRCUIT_MODEL_PORT_TYPES
};
/* Strings correspond to each port type */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_PORT_TYPES> CIRCUIT_MODEL_PORT_TYPE_STRING = {{"input", "output", "inout", "clock", "sram", "bl", "blb", "wl", "wlb", "wlr"}};

enum e_circuit_model_delay_type {
  CIRCUIT_MODEL_DELAY_RISE, 
  CIRCUIT_MODEL_DELAY_FALL,
  NUM_CIRCUIT_MODEL_DELAY_TYPES
};
/* Strings correspond to each delay type */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_DELAY_TYPES> CIRCUIT_MODEL_DELAY_TYPE_STRING = {{"rise", "fall"}};

/********************************************************************
 * Types of configuration protocol
 * 1. configurable memories are organized and accessed as standalone elements 
 * 2. configurable memories are organized and accessed by a scan-chain
 * 3. configurable memories are organized and accessed by memory bank 
 * 4. configurable memories are organized and accessed by frames 
 */
enum e_config_protocol_type {
  CONFIG_MEM_STANDALONE,   
  CONFIG_MEM_SCAN_CHAIN,   
  CONFIG_MEM_MEMORY_BANK,  
  CONFIG_MEM_QL_MEMORY_BANK,  
  CONFIG_MEM_FRAME_BASED,  
  NUM_CONFIG_PROTOCOL_TYPES
};

constexpr std::array<const char*, NUM_CONFIG_PROTOCOL_TYPES> CONFIG_PROTOCOL_TYPE_STRING = {{"standalone", "scan_chain", "memory_bank", "ql_memory_bank", "frame_based"}};

#endif
