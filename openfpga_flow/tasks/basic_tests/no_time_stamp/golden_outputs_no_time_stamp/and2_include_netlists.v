//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Netlist Summary
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ------ Include fabric top-level netlists -----
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/fabric_netlists.v"

`include "and2_output_verilog.v"

`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/and2_top_formal_verification.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/and2_formal_random_top_tb.v"
