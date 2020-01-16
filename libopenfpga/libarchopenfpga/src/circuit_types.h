/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef CIRCUIT_TYPES_H
#define CIRCUIT_TYPES_H

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

enum e_circuit_model_delay_type {
  CIRCUIT_MODEL_DELAY_RISE, 
  CIRCUIT_MODEL_DELAY_FALL,
  NUM_CIRCUIT_MODEL_DELAY_TYPES
};

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
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_TYPES> CIRCUIT_MODEL_TYPE_STRING = {{"CHAN_WIRE", "WIRE", "MUX", "LUT", "FF", "SRAM", "HARDLOGIC", "CCFF", "IOPAD", "INVBUF", "PASSGATE", "GATE"}};

enum e_circuit_model_design_tech {
  CIRCUIT_MODEL_DESIGN_CMOS, 
  CIRCUIT_MODEL_DESIGN_RRAM,
  NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES
};

enum e_circuit_model_structure {
  CIRCUIT_MODEL_STRUCTURE_TREE, 
  CIRCUIT_MODEL_STRUCTURE_ONELEVEL, 
  CIRCUIT_MODEL_STRUCTURE_MULTILEVEL, 
  CIRCUIT_MODEL_STRUCTURE_CROSSBAR,
  NUM_CIRCUIT_MODEL_STRUCTURE_TYPES
};
/* Strings correspond to each type of mux structure */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_STRUCTURE_TYPES> CIRCUIT_MODEL_STRUCTURE_TYPE_STRING = {{"TREE-LIKE", "ONE-LEVEL", "MULTI-LEVEL", "CROSSBAR"}};

enum e_circuit_model_buffer_type {
  CIRCUIT_MODEL_BUF_INV, 
  CIRCUIT_MODEL_BUF_BUF,
  NUM_CIRCUIT_MODEL_BUF_TYPES
};

enum e_circuit_model_pass_gate_logic_type {
  CIRCUIT_MODEL_PASS_GATE_TRANSMISSION, 
  CIRCUIT_MODEL_PASS_GATE_TRANSISTOR,
  CIRCUIT_MODEL_PASS_GATE_RRAM,         /* RRAM can be treated as a special type of pass-gate logic */
  CIRCUIT_MODEL_PASS_GATE_STDCELL,         /* Standard cell as a special type of pass-gate logic */
  NUM_CIRCUIT_MODEL_PASS_GATE_TYPES
};

enum e_circuit_model_gate_type {
  CIRCUIT_MODEL_GATE_AND, 
  CIRCUIT_MODEL_GATE_OR,
  CIRCUIT_MODEL_GATE_MUX2,
  NUM_CIRCUIT_MODEL_GATE_TYPES
};

enum e_wire_model_type {
  WIRE_MODEL_PI,
  WIRE_MODEL_T,
  NUM_WIRE_MODEL_TYPES
};

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
  NUM_CIRCUIT_MODEL_PORT_TYPES
};
/* Strings correspond to each port type */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_PORT_TYPES> CIRCUIT_MODEL_PORT_TYPE_STRING = {{"INPUT", "OUTPUT", "INOUT", "CLOCK", "SRAM", "BL", "BLB", "WL", "WLB"}};

/* For SRAM */
enum e_sram_orgz {
  CIRCUIT_SRAM_STANDALONE,   /* SRAMs are organized and accessed as standalone elements */
  CIRCUIT_SRAM_SCAN_CHAIN,   /* SRAMs are organized and accessed by a scan-chain */
  CIRCUIT_SRAM_MEMORY_BANK,  /* SRAMs are organized and accessed by memory bank */
  CIRCUIT_SRAM_LOCAL_ENCODER,  /* SRAMs are organized and accessed by a local encoder */
  NUM_CIRCUIT_MODEL_SRAM_ORGZ_TYPES
};

constexpr std::array<const char*, NUM_CIRCUIT_MODEL_SRAM_ORGZ_TYPES> CIRCUIT_MODEL_SRAM_ORGZ_TYPE_STRING = {{"STANDALONE", "SCAN-CHAIN", "MEMORY_BANK", "LOCAL_ENCODER"}};

#endif
