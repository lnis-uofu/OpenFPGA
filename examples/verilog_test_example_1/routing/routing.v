//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Header file 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

`include "./verilog_test_example_1/routing/cby_1_1.v"
`include "./verilog_test_example_1/routing/cby_0_1.v"
`include "./verilog_test_example_1/routing/cbx_1_1.v"
`include "./verilog_test_example_1/routing/cbx_1_0.v"
`include "./verilog_test_example_1/routing/sb_1_1.v"
`include "./verilog_test_example_1/routing/sb_1_0.v"
`include "./verilog_test_example_1/routing/sb_0_1.v"
`include "./verilog_test_example_1/routing/sb_0_0.v"
`include "./verilog_test_example_1/routing/chany_1_1.v"
`include "./verilog_test_example_1/routing/chany_0_1.v"
`include "./verilog_test_example_1/routing/chanx_1_1.v"
`include "./verilog_test_example_1/routing/chanx_1_0.v"
