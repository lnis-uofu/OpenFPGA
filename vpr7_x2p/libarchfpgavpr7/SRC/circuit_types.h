/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    circuit_types.h
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/08  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

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

enum spice_model_delay_type {
  SPICE_MODEL_DELAY_RISE, 
  SPICE_MODEL_DELAY_FALL,
  NUM_CIRCUIT_MODEL_DELAY_TYPES
};

/*Struct for a SPICE model of a module*/
enum e_spice_model_type {
  SPICE_MODEL_CHAN_WIRE, 
  SPICE_MODEL_WIRE, 
  SPICE_MODEL_MUX, 
  SPICE_MODEL_LUT, 
  SPICE_MODEL_FF, 
  SPICE_MODEL_SRAM, 
  SPICE_MODEL_HARDLOGIC,
  SPICE_MODEL_CCFF,
  SPICE_MODEL_IOPAD, 
  SPICE_MODEL_INVBUF, 
  SPICE_MODEL_PASSGATE, 
  SPICE_MODEL_GATE,
  NUM_CIRCUIT_MODEL_TYPES
};
/* Strings correspond to each port type */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_TYPES> CIRCUIT_MODEL_TYPE_STRING = {{"CHAN_WIRE", "WIRE", "MUX", "LUT", "FF", "SRAM", "HARDLOGIC", "CCFF", "IOPAD", "INVBUF", "PASSGATE", "GATE"}};

enum e_spice_model_design_tech {
  SPICE_MODEL_DESIGN_CMOS, 
  SPICE_MODEL_DESIGN_RRAM,
  NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES
};

enum e_spice_model_structure {
  SPICE_MODEL_STRUCTURE_TREE, 
  SPICE_MODEL_STRUCTURE_ONELEVEL, 
  SPICE_MODEL_STRUCTURE_MULTILEVEL, 
  SPICE_MODEL_STRUCTURE_CROSSBAR,
  NUM_CIRCUIT_MODEL_STRUCTURE_TYPES
};
/* Strings correspond to each type of mux structure */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_STRUCTURE_TYPES> CIRCUIT_MODEL_STRUCTURE_TYPE_STRING = {{"TREE-LIKE", "ONE-LEVEL", "MULTI-LEVEL", "CROSSBAR"}};

enum e_spice_model_buffer_type {
  SPICE_MODEL_BUF_INV, 
  SPICE_MODEL_BUF_BUF,
  NUM_CIRCUIT_MODEL_BUF_TYPES
};

enum e_spice_model_pass_gate_logic_type {
  SPICE_MODEL_PASS_GATE_TRANSMISSION, 
  SPICE_MODEL_PASS_GATE_TRANSISTOR,
  SPICE_MODEL_PASS_GATE_RRAM,         /* RRAM can be treated as a special type of pass-gate logic */
  SPICE_MODEL_PASS_GATE_STDCELL,         /* Standard cell as a special type of pass-gate logic */
  NUM_CIRCUIT_MODEL_PASS_GATE_TYPES
};

enum e_spice_model_gate_type {
  SPICE_MODEL_GATE_AND, 
  SPICE_MODEL_GATE_OR,
  SPICE_MODEL_GATE_MUX2,
  NUM_SPICE_MODEL_GATE_TYPES
};

enum e_wire_model_type {
  WIRE_MODEL_PIE,
  WIRE_MODEL_T,
  NUM_WIRE_MODEL_TYPES
};

enum e_spice_model_port_type {
  SPICE_MODEL_PORT_INPUT, 
  SPICE_MODEL_PORT_OUTPUT, 
  SPICE_MODEL_PORT_INOUT, 
  SPICE_MODEL_PORT_CLOCK, 
  SPICE_MODEL_PORT_SRAM,
  SPICE_MODEL_PORT_BL,
  SPICE_MODEL_PORT_BLB,
  SPICE_MODEL_PORT_WL,
  SPICE_MODEL_PORT_WLB,
  NUM_CIRCUIT_MODEL_PORT_TYPES
};
/* Strings correspond to each port type */
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_PORT_TYPES> CIRCUIT_MODEL_PORT_TYPE_STRING = {{"INPUT", "OUTPUT", "INOUT", "CLOCK", "SRAM", "BL", "BLB", "WL", "WLB"}};

/* For SRAM */
enum e_sram_orgz {
  SPICE_SRAM_STANDALONE,   /* SRAMs are organized and accessed as standalone elements */
  SPICE_SRAM_SCAN_CHAIN,   /* SRAMs are organized and accessed by a scan-chain */
  SPICE_SRAM_MEMORY_BANK,  /* SRAMs are organized and accessed by memory bank */
  SPICE_SRAM_LOCAL_ENCODER,  /* SRAMs are organized and accessed by a local encoder */
  NUM_CIRCUIT_MODEL_SRAM_ORGZ_TYPES
};
constexpr std::array<const char*, NUM_CIRCUIT_MODEL_SRAM_ORGZ_TYPES> CIRCUIT_MODEL_SRAM_ORGZ_TYPE_STRING = {{"STANDALONE", "SCAN-CHAIN", "MEMORY_BANK", "LOCAL_ENCODER"}};


#endif

/************************************************************************
 * End of file : circuit_types.h
 ***********************************************************************/

