//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Netlist Summary
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ------ Include fabric top-level netlists -----
`include "fabric_netlists.v"

`include "and2_output_verilog.v"

`include "and2_top_formal_verification.v"
`include "and2_formal_random_top_tb.v"
