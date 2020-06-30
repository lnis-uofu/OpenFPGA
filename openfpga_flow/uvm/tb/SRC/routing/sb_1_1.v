//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[1][1]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for sb_1__1_ -----
module sb_1__1_(pReset,
                prog_clk,
                chany_top_in_1_,
                chany_top_in_3_,
                chany_top_in_5_,
                chany_top_in_7_,
                chany_top_in_9_,
                chany_top_in_11_,
                chany_top_in_13_,
                chany_top_in_15_,
                chany_top_in_17_,
                top_left_grid_pin_13_,
                top_right_grid_pin_11_,
                chanx_right_in_1_,
                chanx_right_in_3_,
                chanx_right_in_5_,
                chanx_right_in_7_,
                chanx_right_in_9_,
                chanx_right_in_11_,
                chanx_right_in_13_,
                chanx_right_in_15_,
                chanx_right_in_17_,
                right_top_grid_pin_10_,
                right_bottom_grid_pin_12_,
                chany_bottom_in_0_,
                chany_bottom_in_2_,
                chany_bottom_in_4_,
                chany_bottom_in_6_,
                chany_bottom_in_8_,
                chany_bottom_in_10_,
                chany_bottom_in_12_,
                chany_bottom_in_14_,
                chany_bottom_in_16_,
                bottom_right_grid_pin_11_,
                bottom_left_grid_pin_13_,
                chanx_left_in_0_,
                chanx_left_in_2_,
                chanx_left_in_4_,
                chanx_left_in_6_,
                chanx_left_in_8_,
                chanx_left_in_10_,
                chanx_left_in_12_,
                chanx_left_in_14_,
                chanx_left_in_16_,
                left_top_grid_pin_10_,
                left_bottom_grid_pin_12_,
                enable,
                address,
                data_in,
                chany_top_out_0_,
                chany_top_out_2_,
                chany_top_out_4_,
                chany_top_out_6_,
                chany_top_out_8_,
                chany_top_out_10_,
                chany_top_out_12_,
                chany_top_out_14_,
                chany_top_out_16_,
                chanx_right_out_0_,
                chanx_right_out_2_,
                chanx_right_out_4_,
                chanx_right_out_6_,
                chanx_right_out_8_,
                chanx_right_out_10_,
                chanx_right_out_12_,
                chanx_right_out_14_,
                chanx_right_out_16_,
                chany_bottom_out_1_,
                chany_bottom_out_3_,
                chany_bottom_out_5_,
                chany_bottom_out_7_,
                chany_bottom_out_9_,
                chany_bottom_out_11_,
                chany_bottom_out_13_,
                chany_bottom_out_15_,
                chany_bottom_out_17_,
                chanx_left_out_1_,
                chanx_left_out_3_,
                chanx_left_out_5_,
                chanx_left_out_7_,
                chanx_left_out_9_,
                chanx_left_out_11_,
                chanx_left_out_13_,
                chanx_left_out_15_,
                chanx_left_out_17_);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] chany_top_in_1_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_3_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_5_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_7_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_9_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_11_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_13_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_15_;
//----- INPUT PORTS -----
input [0:0] chany_top_in_17_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_pin_13_;
//----- INPUT PORTS -----
input [0:0] top_right_grid_pin_11_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_1_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_3_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_5_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_7_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_9_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_11_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_13_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_15_;
//----- INPUT PORTS -----
input [0:0] chanx_right_in_17_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_10_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_pin_12_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_0_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_2_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_4_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_6_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_8_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_10_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_12_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_14_;
//----- INPUT PORTS -----
input [0:0] chany_bottom_in_16_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_pin_11_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_13_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_0_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_2_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_4_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_6_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_8_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_10_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_12_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_14_;
//----- INPUT PORTS -----
input [0:0] chanx_left_in_16_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_pin_10_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_pin_12_;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:6] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_0_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_2_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_4_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_6_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_8_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_10_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_12_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_14_;
//----- OUTPUT PORTS -----
output [0:0] chany_top_out_16_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_0_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_2_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_4_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_6_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_8_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_10_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_12_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_14_;
//----- OUTPUT PORTS -----
output [0:0] chanx_right_out_16_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_1_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_3_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_5_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_7_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_9_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_11_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_13_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_15_;
//----- OUTPUT PORTS -----
output [0:0] chany_bottom_out_17_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_1_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_3_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_5_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_7_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_9_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_11_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_13_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_15_;
//----- OUTPUT PORTS -----
output [0:0] chanx_left_out_17_;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:11] decoder4to12_0_data_out;
wire [0:5] mux_2level_tapbuf_size8_0_sram;
wire [0:5] mux_2level_tapbuf_size8_0_sram_inv;
wire [0:5] mux_2level_tapbuf_size8_1_sram;
wire [0:5] mux_2level_tapbuf_size8_1_sram_inv;
wire [0:5] mux_2level_tapbuf_size8_2_sram;
wire [0:5] mux_2level_tapbuf_size8_2_sram_inv;
wire [0:5] mux_2level_tapbuf_size8_3_sram;
wire [0:5] mux_2level_tapbuf_size8_3_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_0_sram;
wire [0:7] mux_2level_tapbuf_size9_0_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_1_sram;
wire [0:7] mux_2level_tapbuf_size9_1_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_2_sram;
wire [0:7] mux_2level_tapbuf_size9_2_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_3_sram;
wire [0:7] mux_2level_tapbuf_size9_3_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_4_sram;
wire [0:7] mux_2level_tapbuf_size9_4_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_5_sram;
wire [0:7] mux_2level_tapbuf_size9_5_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_6_sram;
wire [0:7] mux_2level_tapbuf_size9_6_sram_inv;
wire [0:7] mux_2level_tapbuf_size9_7_sram;
wire [0:7] mux_2level_tapbuf_size9_7_sram_inv;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out_3_[0] = chany_top_in_1_[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out_5_[0] = chany_top_in_3_[0];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out_7_[0] = chany_top_in_5_[0];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out_11_[0] = chany_top_in_9_[0];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out_13_[0] = chany_top_in_11_[0];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out_15_[0] = chany_top_in_13_[0];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chanx_left_out_3_[0] = chanx_right_in_1_[0];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out_5_[0] = chanx_right_in_3_[0];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out_7_[0] = chanx_right_in_5_[0];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chanx_left_out_11_[0] = chanx_right_in_9_[0];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chanx_left_out_13_[0] = chanx_right_in_11_[0];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out_15_[0] = chanx_right_in_13_[0];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out_2_[0] = chany_bottom_in_0_[0];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out_4_[0] = chany_bottom_in_2_[0];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out_6_[0] = chany_bottom_in_4_[0];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out_10_[0] = chany_bottom_in_8_[0];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out_12_[0] = chany_bottom_in_10_[0];
// ----- Local connection due to Wire 28 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out_14_[0] = chany_bottom_in_12_[0];
// ----- Local connection due to Wire 33 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_right_out_2_[0] = chanx_left_in_0_[0];
// ----- Local connection due to Wire 34 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out_4_[0] = chanx_left_in_2_[0];
// ----- Local connection due to Wire 35 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out_6_[0] = chanx_left_in_4_[0];
// ----- Local connection due to Wire 37 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_right_out_10_[0] = chanx_left_in_8_[0];
// ----- Local connection due to Wire 38 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_right_out_12_[0] = chanx_left_in_10_[0];
// ----- Local connection due to Wire 39 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out_14_[0] = chanx_left_in_12_[0];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size9 mux_top_track_0 (
		.in({top_left_grid_pin_13_[0], chanx_right_in_3_[0], chanx_right_in_11_[0], chanx_right_in_15_[0], chany_bottom_in_0_[0], chany_bottom_in_8_[0], chanx_left_in_0_[0], chanx_left_in_6_[0], chanx_left_in_8_[0]}),
		.sram(mux_2level_tapbuf_size9_0_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_0_sram_inv[0:7]),
		.out(chany_top_out_0_[0]));

	mux_2level_tapbuf_size9 mux_top_track_8 (
		.in({top_right_grid_pin_11_[0], chanx_right_in_5_[0], chanx_right_in_13_[0], chanx_right_in_17_[0], chany_bottom_in_2_[0], chany_bottom_in_10_[0], chanx_left_in_4_[0], chanx_left_in_12_[0], chanx_left_in_16_[0]}),
		.sram(mux_2level_tapbuf_size9_1_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_1_sram_inv[0:7]),
		.out(chany_top_out_8_[0]));

	mux_2level_tapbuf_size9 mux_right_track_0 (
		.in({chany_top_in_5_[0], chany_top_in_13_[0], chany_top_in_17_[0], right_top_grid_pin_10_[0], chany_bottom_in_2_[0], chany_bottom_in_10_[0], chany_bottom_in_14_[0], chanx_left_in_0_[0], chanx_left_in_8_[0]}),
		.sram(mux_2level_tapbuf_size9_2_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_2_sram_inv[0:7]),
		.out(chanx_right_out_0_[0]));

	mux_2level_tapbuf_size9 mux_right_track_8 (
		.in({chany_top_in_1_[0], chany_top_in_7_[0], chany_top_in_9_[0], right_bottom_grid_pin_12_[0], chany_bottom_in_0_[0], chany_bottom_in_6_[0], chany_bottom_in_8_[0], chanx_left_in_2_[0], chanx_left_in_10_[0]}),
		.sram(mux_2level_tapbuf_size9_3_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_3_sram_inv[0:7]),
		.out(chanx_right_out_8_[0]));

	mux_2level_tapbuf_size9 mux_bottom_track_1 (
		.in({chany_top_in_1_[0], chany_top_in_9_[0], chanx_right_in_3_[0], chanx_right_in_11_[0], chanx_right_in_15_[0], bottom_right_grid_pin_11_[0], chanx_left_in_2_[0], chanx_left_in_10_[0], chanx_left_in_14_[0]}),
		.sram(mux_2level_tapbuf_size9_4_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_4_sram_inv[0:7]),
		.out(chany_bottom_out_1_[0]));

	mux_2level_tapbuf_size9 mux_bottom_track_9 (
		.in({chany_top_in_3_[0], chany_top_in_11_[0], chanx_right_in_1_[0], chanx_right_in_7_[0], chanx_right_in_9_[0], bottom_left_grid_pin_13_[0], chanx_left_in_4_[0], chanx_left_in_12_[0], chanx_left_in_16_[0]}),
		.sram(mux_2level_tapbuf_size9_5_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_5_sram_inv[0:7]),
		.out(chany_bottom_out_9_[0]));

	mux_2level_tapbuf_size9 mux_left_track_1 (
		.in({chany_top_in_1_[0], chany_top_in_7_[0], chany_top_in_9_[0], chanx_right_in_1_[0], chanx_right_in_9_[0], chany_bottom_in_4_[0], chany_bottom_in_12_[0], chany_bottom_in_16_[0], left_top_grid_pin_10_[0]}),
		.sram(mux_2level_tapbuf_size9_6_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_6_sram_inv[0:7]),
		.out(chanx_left_out_1_[0]));

	mux_2level_tapbuf_size9 mux_left_track_9 (
		.in({chany_top_in_5_[0], chany_top_in_13_[0], chany_top_in_17_[0], chanx_right_in_3_[0], chanx_right_in_11_[0], chany_bottom_in_0_[0], chany_bottom_in_6_[0], chany_bottom_in_8_[0], left_bottom_grid_pin_12_[0]}),
		.sram(mux_2level_tapbuf_size9_7_sram[0:7]),
		.sram_inv(mux_2level_tapbuf_size9_7_sram_inv[0:7]),
		.out(chanx_left_out_9_[0]));

	mux_2level_tapbuf_size9_mem mem_top_track_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[0]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_0_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_0_sram_inv[0:7]));

	mux_2level_tapbuf_size9_mem mem_top_track_8 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[1]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_1_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_1_sram_inv[0:7]));

	mux_2level_tapbuf_size9_mem mem_right_track_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[3]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_2_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_2_sram_inv[0:7]));

	mux_2level_tapbuf_size9_mem mem_right_track_8 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[4]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_3_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_3_sram_inv[0:7]));

	mux_2level_tapbuf_size9_mem mem_bottom_track_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[6]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_4_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_4_sram_inv[0:7]));

	mux_2level_tapbuf_size9_mem mem_bottom_track_9 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[7]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_5_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_5_sram_inv[0:7]));

	mux_2level_tapbuf_size9_mem mem_left_track_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[9]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_6_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_6_sram_inv[0:7]));

	mux_2level_tapbuf_size9_mem mem_left_track_9 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[10]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size9_7_sram[0:7]),
		.mem_outb(mux_2level_tapbuf_size9_7_sram_inv[0:7]));

	mux_2level_tapbuf_size8 mux_top_track_16 (
		.in({chanx_right_in_1_[0], chanx_right_in_7_[0], chanx_right_in_9_[0], chany_bottom_in_4_[0], chany_bottom_in_12_[0], chanx_left_in_2_[0], chanx_left_in_10_[0], chanx_left_in_14_[0]}),
		.sram(mux_2level_tapbuf_size8_0_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size8_0_sram_inv[0:5]),
		.out(chany_top_out_16_[0]));

	mux_2level_tapbuf_size8 mux_right_track_16 (
		.in({chany_top_in_3_[0], chany_top_in_11_[0], chany_top_in_15_[0], chany_bottom_in_4_[0], chany_bottom_in_12_[0], chany_bottom_in_16_[0], chanx_left_in_4_[0], chanx_left_in_12_[0]}),
		.sram(mux_2level_tapbuf_size8_1_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size8_1_sram_inv[0:5]),
		.out(chanx_right_out_16_[0]));

	mux_2level_tapbuf_size8 mux_bottom_track_17 (
		.in({chany_top_in_5_[0], chany_top_in_13_[0], chanx_right_in_5_[0], chanx_right_in_13_[0], chanx_right_in_17_[0], chanx_left_in_0_[0], chanx_left_in_6_[0], chanx_left_in_8_[0]}),
		.sram(mux_2level_tapbuf_size8_2_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size8_2_sram_inv[0:5]),
		.out(chany_bottom_out_17_[0]));

	mux_2level_tapbuf_size8 mux_left_track_17 (
		.in({chany_top_in_3_[0], chany_top_in_11_[0], chany_top_in_15_[0], chanx_right_in_5_[0], chanx_right_in_13_[0], chany_bottom_in_2_[0], chany_bottom_in_10_[0], chany_bottom_in_14_[0]}),
		.sram(mux_2level_tapbuf_size8_3_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size8_3_sram_inv[0:5]),
		.out(chanx_left_out_17_[0]));

	mux_2level_tapbuf_size8_mem mem_top_track_16 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[2]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size8_0_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size8_0_sram_inv[0:5]));

	mux_2level_tapbuf_size8_mem mem_right_track_16 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[5]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size8_1_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size8_1_sram_inv[0:5]));

	mux_2level_tapbuf_size8_mem mem_bottom_track_17 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[8]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size8_2_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size8_2_sram_inv[0:5]));

	mux_2level_tapbuf_size8_mem mem_left_track_17 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to12_0_data_out[11]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size8_3_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size8_3_sram_inv[0:5]));

	decoder4to12 decoder4to12_0_ (
		.enable(enable[0]),
		.address(address[3:6]),
		.data_out(decoder4to12_0_data_out[0:11]));

endmodule
// ----- END Verilog module for sb_1__1_ -----


