//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Preprocessing flags to enable/disable features in FPGA Verilog modules
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

`define ENABLE_TIMING 1

`define ENABLE_SIGNAL_INITIALIZATION 1

`define ICARUS_SIMULATOR 1

