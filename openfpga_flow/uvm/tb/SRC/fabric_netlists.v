//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Fabric Netlist Summary
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:40 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ------ Include defines: preproc flags -----
`include "../SRC/fpga_defines.v"

// ------ Include user-defined netlists -----
`include "/var/tmp/AA_SC/openfpga/OpenFPGA/openfpga_flow/VerilogNetlists/ff.v"
`include "/var/tmp/AA_SC/openfpga/OpenFPGA/openfpga_flow/VerilogNetlists/config_latch.v"
`include "/var/tmp/AA_SC/openfpga/OpenFPGA/openfpga_flow/VerilogNetlists/io.v"
// ------ Include primitive module netlists -----
`include "../SRC/sub_module/inv_buf_passgate.v"
`include "../SRC/sub_module/arch_encoder.v"
`include "../SRC/sub_module/local_encoder.v"
`include "../SRC/sub_module/muxes.v"
`include "../SRC/sub_module/luts.v"
`include "../SRC/sub_module/wires.v"
`include "../SRC/sub_module/memories.v"

// ------ Include logic block netlists -----
`include "../SRC/lb/logical_tile_io.v"
`include "../SRC/lb/logical_tile_clb.v"
`include "../SRC/lb/io_top.v"
`include "../SRC/lb/io_right.v"
`include "../SRC/lb/io_bottom.v"
`include "../SRC/lb/io_left.v"
`include "../SRC/lb/clb.v"

// ------ Include routing module netlists -----
`include "../SRC/routing/sb_0_0.v"
`include "../SRC/routing/sb_0_1.v"
`include "../SRC/routing/sb_0_2.v"
`include "../SRC/routing/sb_1_0.v"
`include "../SRC/routing/sb_1_1.v"
`include "../SRC/routing/sb_1_2.v"
`include "../SRC/routing/sb_2_0.v"
`include "../SRC/routing/sb_2_1.v"
`include "../SRC/routing/sb_2_2.v"
`include "../SRC/routing/cbx_1_0.v"
`include "../SRC/routing/cbx_1_1.v"
`include "../SRC/routing/cbx_1_2.v"
`include "../SRC/routing/cby_0_1.v"
`include "../SRC/routing/cby_1_1.v"
`include "../SRC/routing/cby_2_1.v"

// ------ Include fabric top-level netlists -----
`include "../SRC/fpga_top.v"

