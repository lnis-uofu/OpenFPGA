//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[0][2]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for sb_0__2_ -----
module sb_0__2_(pReset,
                prog_clk,
                chanx_right_in_1_,
                chanx_right_in_3_,
                chanx_right_in_5_,
                chanx_right_in_7_,
                chanx_right_in_9_,
                chanx_right_in_11_,
                chanx_right_in_13_,
                chanx_right_in_15_,
                chanx_right_in_17_,
                right_top_grid_pin_1_,
                right_top_grid_pin_3_,
                right_top_grid_pin_5_,
                right_top_grid_pin_7_,
                right_top_grid_pin_9_,
                right_top_grid_pin_11_,
                right_top_grid_pin_13_,
                right_top_grid_pin_15_,
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
                bottom_left_grid_pin_1_,
                bottom_left_grid_pin_3_,
                bottom_left_grid_pin_5_,
                bottom_left_grid_pin_7_,
                bottom_left_grid_pin_9_,
                bottom_left_grid_pin_11_,
                bottom_left_grid_pin_13_,
                bottom_left_grid_pin_15_,
                enable,
                address,
                data_in,
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
                chany_bottom_out_17_);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
input [0:0] right_top_grid_pin_1_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_3_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_5_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_7_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_9_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_11_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_13_;
//----- INPUT PORTS -----
input [0:0] right_top_grid_pin_15_;
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
input [0:0] bottom_left_grid_pin_1_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_3_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_5_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_7_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_9_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_11_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_13_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_pin_15_;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:5] address;
//----- INPUT PORTS -----
input [0:0] data_in;
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

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:17] decoder5to18_0_data_out;
wire [0:1] mux_2level_tapbuf_size2_0_sram;
wire [0:1] mux_2level_tapbuf_size2_0_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_10_sram;
wire [0:1] mux_2level_tapbuf_size2_10_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_11_sram;
wire [0:1] mux_2level_tapbuf_size2_11_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_12_sram;
wire [0:1] mux_2level_tapbuf_size2_12_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_13_sram;
wire [0:1] mux_2level_tapbuf_size2_13_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_14_sram;
wire [0:1] mux_2level_tapbuf_size2_14_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_15_sram;
wire [0:1] mux_2level_tapbuf_size2_15_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_16_sram;
wire [0:1] mux_2level_tapbuf_size2_16_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_17_sram;
wire [0:1] mux_2level_tapbuf_size2_17_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_1_sram;
wire [0:1] mux_2level_tapbuf_size2_1_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_2_sram;
wire [0:1] mux_2level_tapbuf_size2_2_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_3_sram;
wire [0:1] mux_2level_tapbuf_size2_3_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_4_sram;
wire [0:1] mux_2level_tapbuf_size2_4_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_5_sram;
wire [0:1] mux_2level_tapbuf_size2_5_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_6_sram;
wire [0:1] mux_2level_tapbuf_size2_6_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_7_sram;
wire [0:1] mux_2level_tapbuf_size2_7_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_8_sram;
wire [0:1] mux_2level_tapbuf_size2_8_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_9_sram;
wire [0:1] mux_2level_tapbuf_size2_9_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size2 mux_right_track_0 (
		.in({right_top_grid_pin_1_[0], chany_bottom_in_14_[0]}),
		.sram(mux_2level_tapbuf_size2_0_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_0_sram_inv[0:1]),
		.out(chanx_right_out_0_[0]));

	mux_2level_tapbuf_size2 mux_right_track_2 (
		.in({right_top_grid_pin_3_[0], chany_bottom_in_12_[0]}),
		.sram(mux_2level_tapbuf_size2_1_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_1_sram_inv[0:1]),
		.out(chanx_right_out_2_[0]));

	mux_2level_tapbuf_size2 mux_right_track_4 (
		.in({right_top_grid_pin_5_[0], chany_bottom_in_10_[0]}),
		.sram(mux_2level_tapbuf_size2_2_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_2_sram_inv[0:1]),
		.out(chanx_right_out_4_[0]));

	mux_2level_tapbuf_size2 mux_right_track_6 (
		.in({right_top_grid_pin_7_[0], chany_bottom_in_8_[0]}),
		.sram(mux_2level_tapbuf_size2_3_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_3_sram_inv[0:1]),
		.out(chanx_right_out_6_[0]));

	mux_2level_tapbuf_size2 mux_right_track_8 (
		.in({right_top_grid_pin_9_[0], chany_bottom_in_6_[0]}),
		.sram(mux_2level_tapbuf_size2_4_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_4_sram_inv[0:1]),
		.out(chanx_right_out_8_[0]));

	mux_2level_tapbuf_size2 mux_right_track_10 (
		.in({right_top_grid_pin_11_[0], chany_bottom_in_4_[0]}),
		.sram(mux_2level_tapbuf_size2_5_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_5_sram_inv[0:1]),
		.out(chanx_right_out_10_[0]));

	mux_2level_tapbuf_size2 mux_right_track_12 (
		.in({right_top_grid_pin_13_[0], chany_bottom_in_2_[0]}),
		.sram(mux_2level_tapbuf_size2_6_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_6_sram_inv[0:1]),
		.out(chanx_right_out_12_[0]));

	mux_2level_tapbuf_size2 mux_right_track_14 (
		.in({right_top_grid_pin_15_[0], chany_bottom_in_0_[0]}),
		.sram(mux_2level_tapbuf_size2_7_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_7_sram_inv[0:1]),
		.out(chanx_right_out_14_[0]));

	mux_2level_tapbuf_size2 mux_right_track_16 (
		.in({right_bottom_grid_pin_12_[0], chany_bottom_in_16_[0]}),
		.sram(mux_2level_tapbuf_size2_8_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_8_sram_inv[0:1]),
		.out(chanx_right_out_16_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_1 (
		.in({chanx_right_in_15_[0], bottom_right_grid_pin_11_[0]}),
		.sram(mux_2level_tapbuf_size2_9_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_9_sram_inv[0:1]),
		.out(chany_bottom_out_1_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_3 (
		.in({chanx_right_in_13_[0], bottom_left_grid_pin_1_[0]}),
		.sram(mux_2level_tapbuf_size2_10_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_10_sram_inv[0:1]),
		.out(chany_bottom_out_3_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_5 (
		.in({chanx_right_in_11_[0], bottom_left_grid_pin_3_[0]}),
		.sram(mux_2level_tapbuf_size2_11_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_11_sram_inv[0:1]),
		.out(chany_bottom_out_5_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_7 (
		.in({chanx_right_in_9_[0], bottom_left_grid_pin_5_[0]}),
		.sram(mux_2level_tapbuf_size2_12_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_12_sram_inv[0:1]),
		.out(chany_bottom_out_7_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_9 (
		.in({chanx_right_in_7_[0], bottom_left_grid_pin_7_[0]}),
		.sram(mux_2level_tapbuf_size2_13_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_13_sram_inv[0:1]),
		.out(chany_bottom_out_9_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_11 (
		.in({chanx_right_in_5_[0], bottom_left_grid_pin_9_[0]}),
		.sram(mux_2level_tapbuf_size2_14_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_14_sram_inv[0:1]),
		.out(chany_bottom_out_11_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_13 (
		.in({chanx_right_in_3_[0], bottom_left_grid_pin_11_[0]}),
		.sram(mux_2level_tapbuf_size2_15_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_15_sram_inv[0:1]),
		.out(chany_bottom_out_13_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_15 (
		.in({chanx_right_in_1_[0], bottom_left_grid_pin_13_[0]}),
		.sram(mux_2level_tapbuf_size2_16_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_16_sram_inv[0:1]),
		.out(chany_bottom_out_15_[0]));

	mux_2level_tapbuf_size2 mux_bottom_track_17 (
		.in({chanx_right_in_17_[0], bottom_left_grid_pin_15_[0]}),
		.sram(mux_2level_tapbuf_size2_17_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_17_sram_inv[0:1]),
		.out(chany_bottom_out_17_[0]));

	mux_2level_tapbuf_size2_mem mem_right_track_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[0]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_0_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_0_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[1]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_1_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_1_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[2]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_2_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_2_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_6 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[3]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_3_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_3_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_8 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[4]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_4_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_4_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_10 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[5]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_5_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_5_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_12 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[6]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_6_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_6_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_14 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[7]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_7_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_7_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_track_16 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[8]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_8_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_8_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[9]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_9_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_9_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[10]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_10_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_10_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[11]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_11_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_11_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_7 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[12]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_12_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_12_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_9 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[13]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_13_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_13_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_11 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[14]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_14_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_14_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_13 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[15]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_15_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_15_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_15 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[16]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_16_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_16_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_bottom_track_17 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder5to18_0_data_out[17]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_17_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_17_sram_inv[0:1]));

	decoder5to18 decoder5to18_0_ (
		.enable(enable[0]),
		.address(address[1:5]),
		.data_out(decoder5to18_0_data_out[0:17]));

endmodule
// ----- END Verilog module for sb_0__2_ -----


