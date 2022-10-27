//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: clb
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- BEGIN Physical programmable logic block Verilog module: clb -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_clb_ -----
module logical_tile_clb_mode_clb_(prog_clk,
                                  set,
                                  reset,
                                  clk,
                                  clb_I,
                                  clb_clk,
                                  ccff_head,
                                  clb_O,
                                  ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GLOBAL PORTS -----
input [0:0] set;
//----- GLOBAL PORTS -----
input [0:0] reset;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:9] clb_I;
//----- INPUT PORTS -----
input [0:0] clb_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:3] clb_O;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:9] clb_I;
wire [0:0] clb_clk;
wire [0:3] clb_O;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [0:0] direct_interc_7_out;
wire [0:0] logical_tile_clb_mode_default__fle_0_ccff_tail;
wire [0:0] logical_tile_clb_mode_default__fle_0_fle_out;
wire [0:0] logical_tile_clb_mode_default__fle_1_ccff_tail;
wire [0:0] logical_tile_clb_mode_default__fle_1_fle_out;
wire [0:0] logical_tile_clb_mode_default__fle_2_ccff_tail;
wire [0:0] logical_tile_clb_mode_default__fle_2_fle_out;
wire [0:0] logical_tile_clb_mode_default__fle_3_ccff_tail;
wire [0:0] logical_tile_clb_mode_default__fle_3_fle_out;
wire [0:0] mux_tree_size14_0_out;
wire [0:3] mux_tree_size14_0_sram;
wire [0:3] mux_tree_size14_0_sram_inv;
wire [0:0] mux_tree_size14_10_out;
wire [0:3] mux_tree_size14_10_sram;
wire [0:3] mux_tree_size14_10_sram_inv;
wire [0:0] mux_tree_size14_11_out;
wire [0:3] mux_tree_size14_11_sram;
wire [0:3] mux_tree_size14_11_sram_inv;
wire [0:0] mux_tree_size14_12_out;
wire [0:3] mux_tree_size14_12_sram;
wire [0:3] mux_tree_size14_12_sram_inv;
wire [0:0] mux_tree_size14_13_out;
wire [0:3] mux_tree_size14_13_sram;
wire [0:3] mux_tree_size14_13_sram_inv;
wire [0:0] mux_tree_size14_14_out;
wire [0:3] mux_tree_size14_14_sram;
wire [0:3] mux_tree_size14_14_sram_inv;
wire [0:0] mux_tree_size14_15_out;
wire [0:3] mux_tree_size14_15_sram;
wire [0:3] mux_tree_size14_15_sram_inv;
wire [0:0] mux_tree_size14_1_out;
wire [0:3] mux_tree_size14_1_sram;
wire [0:3] mux_tree_size14_1_sram_inv;
wire [0:0] mux_tree_size14_2_out;
wire [0:3] mux_tree_size14_2_sram;
wire [0:3] mux_tree_size14_2_sram_inv;
wire [0:0] mux_tree_size14_3_out;
wire [0:3] mux_tree_size14_3_sram;
wire [0:3] mux_tree_size14_3_sram_inv;
wire [0:0] mux_tree_size14_4_out;
wire [0:3] mux_tree_size14_4_sram;
wire [0:3] mux_tree_size14_4_sram_inv;
wire [0:0] mux_tree_size14_5_out;
wire [0:3] mux_tree_size14_5_sram;
wire [0:3] mux_tree_size14_5_sram_inv;
wire [0:0] mux_tree_size14_6_out;
wire [0:3] mux_tree_size14_6_sram;
wire [0:3] mux_tree_size14_6_sram_inv;
wire [0:0] mux_tree_size14_7_out;
wire [0:3] mux_tree_size14_7_sram;
wire [0:3] mux_tree_size14_7_sram_inv;
wire [0:0] mux_tree_size14_8_out;
wire [0:3] mux_tree_size14_8_sram;
wire [0:3] mux_tree_size14_8_sram_inv;
wire [0:0] mux_tree_size14_9_out;
wire [0:3] mux_tree_size14_9_sram;
wire [0:3] mux_tree_size14_9_sram_inv;
wire [0:0] mux_tree_size14_mem_0_ccff_tail;
wire [0:0] mux_tree_size14_mem_10_ccff_tail;
wire [0:0] mux_tree_size14_mem_11_ccff_tail;
wire [0:0] mux_tree_size14_mem_12_ccff_tail;
wire [0:0] mux_tree_size14_mem_13_ccff_tail;
wire [0:0] mux_tree_size14_mem_14_ccff_tail;
wire [0:0] mux_tree_size14_mem_1_ccff_tail;
wire [0:0] mux_tree_size14_mem_2_ccff_tail;
wire [0:0] mux_tree_size14_mem_3_ccff_tail;
wire [0:0] mux_tree_size14_mem_4_ccff_tail;
wire [0:0] mux_tree_size14_mem_5_ccff_tail;
wire [0:0] mux_tree_size14_mem_6_ccff_tail;
wire [0:0] mux_tree_size14_mem_7_ccff_tail;
wire [0:0] mux_tree_size14_mem_8_ccff_tail;
wire [0:0] mux_tree_size14_mem_9_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_0 (
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_tree_size14_0_out, mux_tree_size14_1_out, mux_tree_size14_2_out, mux_tree_size14_3_out}),
		.fle_clk(direct_interc_4_out),
		.ccff_head(ccff_head),
		.fle_out(logical_tile_clb_mode_default__fle_0_fle_out),
		.ccff_tail(logical_tile_clb_mode_default__fle_0_ccff_tail));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_1 (
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_tree_size14_4_out, mux_tree_size14_5_out, mux_tree_size14_6_out, mux_tree_size14_7_out}),
		.fle_clk(direct_interc_5_out),
		.ccff_head(logical_tile_clb_mode_default__fle_0_ccff_tail),
		.fle_out(logical_tile_clb_mode_default__fle_1_fle_out),
		.ccff_tail(logical_tile_clb_mode_default__fle_1_ccff_tail));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_2 (
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_tree_size14_8_out, mux_tree_size14_9_out, mux_tree_size14_10_out, mux_tree_size14_11_out}),
		.fle_clk(direct_interc_6_out),
		.ccff_head(logical_tile_clb_mode_default__fle_1_ccff_tail),
		.fle_out(logical_tile_clb_mode_default__fle_2_fle_out),
		.ccff_tail(logical_tile_clb_mode_default__fle_2_ccff_tail));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_3 (
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_tree_size14_12_out, mux_tree_size14_13_out, mux_tree_size14_14_out, mux_tree_size14_15_out}),
		.fle_clk(direct_interc_7_out),
		.ccff_head(logical_tile_clb_mode_default__fle_2_ccff_tail),
		.fle_out(logical_tile_clb_mode_default__fle_3_fle_out),
		.ccff_tail(logical_tile_clb_mode_default__fle_3_ccff_tail));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_mode_default__fle_0_fle_out),
		.out(clb_O[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_clb_mode_default__fle_1_fle_out),
		.out(clb_O[1]));

	direct_interc direct_interc_2_ (
		.in(logical_tile_clb_mode_default__fle_2_fle_out),
		.out(clb_O[2]));

	direct_interc direct_interc_3_ (
		.in(logical_tile_clb_mode_default__fle_3_fle_out),
		.out(clb_O[3]));

	direct_interc direct_interc_4_ (
		.in(clb_clk),
		.out(direct_interc_4_out));

	direct_interc direct_interc_5_ (
		.in(clb_clk),
		.out(direct_interc_5_out));

	direct_interc direct_interc_6_ (
		.in(clb_clk),
		.out(direct_interc_6_out));

	direct_interc direct_interc_7_ (
		.in(clb_clk),
		.out(direct_interc_7_out));

	mux_tree_size14 mux_fle_0_in_0 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_0_sram[0:3]),
		.sram_inv(mux_tree_size14_0_sram_inv[0:3]),
		.out(mux_tree_size14_0_out));

	mux_tree_size14 mux_fle_0_in_1 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_1_sram[0:3]),
		.sram_inv(mux_tree_size14_1_sram_inv[0:3]),
		.out(mux_tree_size14_1_out));

	mux_tree_size14 mux_fle_0_in_2 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_2_sram[0:3]),
		.sram_inv(mux_tree_size14_2_sram_inv[0:3]),
		.out(mux_tree_size14_2_out));

	mux_tree_size14 mux_fle_0_in_3 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_3_sram[0:3]),
		.sram_inv(mux_tree_size14_3_sram_inv[0:3]),
		.out(mux_tree_size14_3_out));

	mux_tree_size14 mux_fle_1_in_0 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_4_sram[0:3]),
		.sram_inv(mux_tree_size14_4_sram_inv[0:3]),
		.out(mux_tree_size14_4_out));

	mux_tree_size14 mux_fle_1_in_1 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_5_sram[0:3]),
		.sram_inv(mux_tree_size14_5_sram_inv[0:3]),
		.out(mux_tree_size14_5_out));

	mux_tree_size14 mux_fle_1_in_2 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_6_sram[0:3]),
		.sram_inv(mux_tree_size14_6_sram_inv[0:3]),
		.out(mux_tree_size14_6_out));

	mux_tree_size14 mux_fle_1_in_3 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_7_sram[0:3]),
		.sram_inv(mux_tree_size14_7_sram_inv[0:3]),
		.out(mux_tree_size14_7_out));

	mux_tree_size14 mux_fle_2_in_0 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_8_sram[0:3]),
		.sram_inv(mux_tree_size14_8_sram_inv[0:3]),
		.out(mux_tree_size14_8_out));

	mux_tree_size14 mux_fle_2_in_1 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_9_sram[0:3]),
		.sram_inv(mux_tree_size14_9_sram_inv[0:3]),
		.out(mux_tree_size14_9_out));

	mux_tree_size14 mux_fle_2_in_2 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_10_sram[0:3]),
		.sram_inv(mux_tree_size14_10_sram_inv[0:3]),
		.out(mux_tree_size14_10_out));

	mux_tree_size14 mux_fle_2_in_3 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_11_sram[0:3]),
		.sram_inv(mux_tree_size14_11_sram_inv[0:3]),
		.out(mux_tree_size14_11_out));

	mux_tree_size14 mux_fle_3_in_0 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_12_sram[0:3]),
		.sram_inv(mux_tree_size14_12_sram_inv[0:3]),
		.out(mux_tree_size14_12_out));

	mux_tree_size14 mux_fle_3_in_1 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_13_sram[0:3]),
		.sram_inv(mux_tree_size14_13_sram_inv[0:3]),
		.out(mux_tree_size14_13_out));

	mux_tree_size14 mux_fle_3_in_2 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_14_sram[0:3]),
		.sram_inv(mux_tree_size14_14_sram_inv[0:3]),
		.out(mux_tree_size14_14_out));

	mux_tree_size14 mux_fle_3_in_3 (
		.in({clb_I[0:9], logical_tile_clb_mode_default__fle_0_fle_out, logical_tile_clb_mode_default__fle_1_fle_out, logical_tile_clb_mode_default__fle_2_fle_out, logical_tile_clb_mode_default__fle_3_fle_out}),
		.sram(mux_tree_size14_15_sram[0:3]),
		.sram_inv(mux_tree_size14_15_sram_inv[0:3]),
		.out(mux_tree_size14_15_out));

	mux_tree_size14_mem mem_fle_0_in_0 (
		.prog_clk(prog_clk),
		.ccff_head(logical_tile_clb_mode_default__fle_3_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_0_ccff_tail),
		.mem_out(mux_tree_size14_0_sram[0:3]),
		.mem_outb(mux_tree_size14_0_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_0_in_1 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_0_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_1_ccff_tail),
		.mem_out(mux_tree_size14_1_sram[0:3]),
		.mem_outb(mux_tree_size14_1_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_0_in_2 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_1_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_2_ccff_tail),
		.mem_out(mux_tree_size14_2_sram[0:3]),
		.mem_outb(mux_tree_size14_2_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_0_in_3 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_2_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_3_ccff_tail),
		.mem_out(mux_tree_size14_3_sram[0:3]),
		.mem_outb(mux_tree_size14_3_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_1_in_0 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_3_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_4_ccff_tail),
		.mem_out(mux_tree_size14_4_sram[0:3]),
		.mem_outb(mux_tree_size14_4_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_1_in_1 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_4_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_5_ccff_tail),
		.mem_out(mux_tree_size14_5_sram[0:3]),
		.mem_outb(mux_tree_size14_5_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_1_in_2 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_5_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_6_ccff_tail),
		.mem_out(mux_tree_size14_6_sram[0:3]),
		.mem_outb(mux_tree_size14_6_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_1_in_3 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_6_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_7_ccff_tail),
		.mem_out(mux_tree_size14_7_sram[0:3]),
		.mem_outb(mux_tree_size14_7_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_2_in_0 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_7_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_8_ccff_tail),
		.mem_out(mux_tree_size14_8_sram[0:3]),
		.mem_outb(mux_tree_size14_8_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_2_in_1 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_8_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_9_ccff_tail),
		.mem_out(mux_tree_size14_9_sram[0:3]),
		.mem_outb(mux_tree_size14_9_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_2_in_2 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_9_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_10_ccff_tail),
		.mem_out(mux_tree_size14_10_sram[0:3]),
		.mem_outb(mux_tree_size14_10_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_2_in_3 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_10_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_11_ccff_tail),
		.mem_out(mux_tree_size14_11_sram[0:3]),
		.mem_outb(mux_tree_size14_11_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_3_in_0 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_11_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_12_ccff_tail),
		.mem_out(mux_tree_size14_12_sram[0:3]),
		.mem_outb(mux_tree_size14_12_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_3_in_1 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_12_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_13_ccff_tail),
		.mem_out(mux_tree_size14_13_sram[0:3]),
		.mem_outb(mux_tree_size14_13_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_3_in_2 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_13_ccff_tail),
		.ccff_tail(mux_tree_size14_mem_14_ccff_tail),
		.mem_out(mux_tree_size14_14_sram[0:3]),
		.mem_outb(mux_tree_size14_14_sram_inv[0:3]));

	mux_tree_size14_mem mem_fle_3_in_3 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_size14_mem_14_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mux_tree_size14_15_sram[0:3]),
		.mem_outb(mux_tree_size14_15_sram_inv[0:3]));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_clb_ -----

//----- Default net type -----
`default_nettype none



// ----- END Physical programmable logic block Verilog module: clb -----
