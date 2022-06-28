//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[0][0]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_0__0_ -----
module sb_0__0_(prog_clk,
                chany_top_in,
                top_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_,
                top_right_grid_left_width_0_height_0_subtile_0__pin_O_1_,
                chanx_right_in,
                right_top_grid_bottom_width_0_height_0_subtile_0__pin_O_0_,
                right_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_,
                ccff_head,
                chany_top_out,
                chanx_right_out,
                ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:12] chany_top_in;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] top_right_grid_left_width_0_height_0_subtile_0__pin_O_1_;
//----- INPUT PORTS -----
input [0:12] chanx_right_in;
//----- INPUT PORTS -----
input [0:0] right_top_grid_bottom_width_0_height_0_subtile_0__pin_O_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] right_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:12] chany_top_out;
//----- OUTPUT PORTS -----
output [0:12] chanx_right_out;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] mux_tree_tapbuf_size2_0_sram;
wire [0:1] mux_tree_tapbuf_size2_0_sram_inv;
wire [0:1] mux_tree_tapbuf_size2_1_sram;
wire [0:1] mux_tree_tapbuf_size2_1_sram_inv;
wire [0:1] mux_tree_tapbuf_size2_2_sram;
wire [0:1] mux_tree_tapbuf_size2_2_sram_inv;
wire [0:1] mux_tree_tapbuf_size2_3_sram;
wire [0:1] mux_tree_tapbuf_size2_3_sram_inv;
wire [0:1] mux_tree_tapbuf_size2_4_sram;
wire [0:1] mux_tree_tapbuf_size2_4_sram_inv;
wire [0:1] mux_tree_tapbuf_size2_5_sram;
wire [0:1] mux_tree_tapbuf_size2_5_sram_inv;
wire [0:0] mux_tree_tapbuf_size2_mem_0_ccff_tail;
wire [0:0] mux_tree_tapbuf_size2_mem_1_ccff_tail;
wire [0:0] mux_tree_tapbuf_size2_mem_2_ccff_tail;
wire [0:0] mux_tree_tapbuf_size2_mem_3_ccff_tail;
wire [0:0] mux_tree_tapbuf_size2_mem_4_ccff_tail;
wire [0:0] mux_tree_tapbuf_size2_mem_5_ccff_tail;
wire [0:1] mux_tree_tapbuf_size3_0_sram;
wire [0:1] mux_tree_tapbuf_size3_0_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_10_sram;
wire [0:1] mux_tree_tapbuf_size3_10_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_11_sram;
wire [0:1] mux_tree_tapbuf_size3_11_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_12_sram;
wire [0:1] mux_tree_tapbuf_size3_12_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_13_sram;
wire [0:1] mux_tree_tapbuf_size3_13_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_1_sram;
wire [0:1] mux_tree_tapbuf_size3_1_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_2_sram;
wire [0:1] mux_tree_tapbuf_size3_2_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_3_sram;
wire [0:1] mux_tree_tapbuf_size3_3_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_4_sram;
wire [0:1] mux_tree_tapbuf_size3_4_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_5_sram;
wire [0:1] mux_tree_tapbuf_size3_5_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_6_sram;
wire [0:1] mux_tree_tapbuf_size3_6_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_7_sram;
wire [0:1] mux_tree_tapbuf_size3_7_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_8_sram;
wire [0:1] mux_tree_tapbuf_size3_8_sram_inv;
wire [0:1] mux_tree_tapbuf_size3_9_sram;
wire [0:1] mux_tree_tapbuf_size3_9_sram_inv;
wire [0:0] mux_tree_tapbuf_size3_mem_0_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_10_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_11_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_12_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_1_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_2_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_3_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_4_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_5_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_6_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_7_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_8_ccff_tail;
wire [0:0] mux_tree_tapbuf_size3_mem_9_ccff_tail;
wire [0:2] mux_tree_tapbuf_size4_0_sram;
wire [0:2] mux_tree_tapbuf_size4_0_sram_inv;
wire [0:2] mux_tree_tapbuf_size4_1_sram;
wire [0:2] mux_tree_tapbuf_size4_1_sram_inv;
wire [0:2] mux_tree_tapbuf_size4_2_sram;
wire [0:2] mux_tree_tapbuf_size4_2_sram_inv;
wire [0:2] mux_tree_tapbuf_size4_3_sram;
wire [0:2] mux_tree_tapbuf_size4_3_sram_inv;
wire [0:2] mux_tree_tapbuf_size4_4_sram;
wire [0:2] mux_tree_tapbuf_size4_4_sram_inv;
wire [0:2] mux_tree_tapbuf_size4_5_sram;
wire [0:2] mux_tree_tapbuf_size4_5_sram_inv;
wire [0:0] mux_tree_tapbuf_size4_mem_0_ccff_tail;
wire [0:0] mux_tree_tapbuf_size4_mem_1_ccff_tail;
wire [0:0] mux_tree_tapbuf_size4_mem_2_ccff_tail;
wire [0:0] mux_tree_tapbuf_size4_mem_3_ccff_tail;
wire [0:0] mux_tree_tapbuf_size4_mem_4_ccff_tail;
wire [0:0] mux_tree_tapbuf_size4_mem_5_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_tree_tapbuf_size4 mux_top_track_0 (
		.in({top_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, chanx_right_in[1]}),
		.sram(mux_tree_tapbuf_size4_0_sram[0:2]),
		.sram_inv(mux_tree_tapbuf_size4_0_sram_inv[0:2]),
		.out(chany_top_out[0]));

	mux_tree_tapbuf_size4 mux_top_track_12 (
		.in({top_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, chanx_right_in[7]}),
		.sram(mux_tree_tapbuf_size4_1_sram[0:2]),
		.sram_inv(mux_tree_tapbuf_size4_1_sram_inv[0:2]),
		.out(chany_top_out[6]));

	mux_tree_tapbuf_size4 mux_right_track_0 (
		.in({chany_top_in[12], right_top_grid_bottom_width_0_height_0_subtile_0__pin_O_0_, right_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size4_2_sram[0:2]),
		.sram_inv(mux_tree_tapbuf_size4_2_sram_inv[0:2]),
		.out(chanx_right_out[0]));

	mux_tree_tapbuf_size4 mux_right_track_2 (
		.in({chany_top_in[0], right_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size4_3_sram[0:2]),
		.sram_inv(mux_tree_tapbuf_size4_3_sram_inv[0:2]),
		.out(chanx_right_out[1]));

	mux_tree_tapbuf_size4 mux_right_track_12 (
		.in({chany_top_in[5], right_top_grid_bottom_width_0_height_0_subtile_0__pin_O_0_, right_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size4_4_sram[0:2]),
		.sram_inv(mux_tree_tapbuf_size4_4_sram_inv[0:2]),
		.out(chanx_right_out[6]));

	mux_tree_tapbuf_size4 mux_right_track_14 (
		.in({chany_top_in[6], right_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size4_5_sram[0:2]),
		.sram_inv(mux_tree_tapbuf_size4_5_sram_inv[0:2]),
		.out(chanx_right_out[7]));

	mux_tree_tapbuf_size4_mem mem_top_track_0 (
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_tree_tapbuf_size4_mem_0_ccff_tail),
		.mem_out(mux_tree_tapbuf_size4_0_sram[0:2]),
		.mem_outb(mux_tree_tapbuf_size4_0_sram_inv[0:2]));

	mux_tree_tapbuf_size4_mem mem_top_track_12 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_4_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size4_mem_1_ccff_tail),
		.mem_out(mux_tree_tapbuf_size4_1_sram[0:2]),
		.mem_outb(mux_tree_tapbuf_size4_1_sram_inv[0:2]));

	mux_tree_tapbuf_size4_mem mem_right_track_0 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_7_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size4_mem_2_ccff_tail),
		.mem_out(mux_tree_tapbuf_size4_2_sram[0:2]),
		.mem_outb(mux_tree_tapbuf_size4_2_sram_inv[0:2]));

	mux_tree_tapbuf_size4_mem mem_right_track_2 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size4_mem_2_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size4_mem_3_ccff_tail),
		.mem_out(mux_tree_tapbuf_size4_3_sram[0:2]),
		.mem_outb(mux_tree_tapbuf_size4_3_sram_inv[0:2]));

	mux_tree_tapbuf_size4_mem mem_right_track_12 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_11_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size4_mem_4_ccff_tail),
		.mem_out(mux_tree_tapbuf_size4_4_sram[0:2]),
		.mem_outb(mux_tree_tapbuf_size4_4_sram_inv[0:2]));

	mux_tree_tapbuf_size4_mem mem_right_track_14 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size4_mem_4_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size4_mem_5_ccff_tail),
		.mem_out(mux_tree_tapbuf_size4_5_sram[0:2]),
		.mem_outb(mux_tree_tapbuf_size4_5_sram_inv[0:2]));

	mux_tree_tapbuf_size3 mux_top_track_2 (
		.in({top_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, chanx_right_in[2]}),
		.sram(mux_tree_tapbuf_size3_0_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_0_sram_inv[0:1]),
		.out(chany_top_out[1]));

	mux_tree_tapbuf_size3 mux_top_track_4 (
		.in({top_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, chanx_right_in[3]}),
		.sram(mux_tree_tapbuf_size3_1_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_1_sram_inv[0:1]),
		.out(chany_top_out[2]));

	mux_tree_tapbuf_size3 mux_top_track_6 (
		.in({top_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, chanx_right_in[4]}),
		.sram(mux_tree_tapbuf_size3_2_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_2_sram_inv[0:1]),
		.out(chany_top_out[3]));

	mux_tree_tapbuf_size3 mux_top_track_8 (
		.in({top_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, chanx_right_in[5]}),
		.sram(mux_tree_tapbuf_size3_3_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_3_sram_inv[0:1]),
		.out(chany_top_out[4]));

	mux_tree_tapbuf_size3 mux_top_track_10 (
		.in({top_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, chanx_right_in[6]}),
		.sram(mux_tree_tapbuf_size3_4_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_4_sram_inv[0:1]),
		.out(chany_top_out[5]));

	mux_tree_tapbuf_size3 mux_top_track_14 (
		.in({top_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, chanx_right_in[8]}),
		.sram(mux_tree_tapbuf_size3_5_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_5_sram_inv[0:1]),
		.out(chany_top_out[7]));

	mux_tree_tapbuf_size3 mux_top_track_16 (
		.in({top_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, top_right_grid_left_width_0_height_0_subtile_0__pin_O_1_, chanx_right_in[9]}),
		.sram(mux_tree_tapbuf_size3_6_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_6_sram_inv[0:1]),
		.out(chany_top_out[8]));

	mux_tree_tapbuf_size3 mux_top_track_24 (
		.in({top_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, top_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, chanx_right_in[0]}),
		.sram(mux_tree_tapbuf_size3_7_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_7_sram_inv[0:1]),
		.out(chany_top_out[12]));

	mux_tree_tapbuf_size3 mux_right_track_4 (
		.in({chany_top_in[1], right_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size3_8_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_8_sram_inv[0:1]),
		.out(chanx_right_out[2]));

	mux_tree_tapbuf_size3 mux_right_track_6 (
		.in({chany_top_in[2], right_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size3_9_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_9_sram_inv[0:1]),
		.out(chanx_right_out[3]));

	mux_tree_tapbuf_size3 mux_right_track_8 (
		.in({chany_top_in[3], right_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size3_10_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_10_sram_inv[0:1]),
		.out(chanx_right_out[4]));

	mux_tree_tapbuf_size3 mux_right_track_10 (
		.in({chany_top_in[4], right_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size3_11_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_11_sram_inv[0:1]),
		.out(chanx_right_out[5]));

	mux_tree_tapbuf_size3 mux_right_track_16 (
		.in({chany_top_in[7], right_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, right_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size3_12_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_12_sram_inv[0:1]),
		.out(chanx_right_out[8]));

	mux_tree_tapbuf_size3 mux_right_track_24 (
		.in({chany_top_in[11], right_top_grid_bottom_width_0_height_0_subtile_0__pin_O_0_, right_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size3_13_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size3_13_sram_inv[0:1]),
		.out(chanx_right_out[12]));

	mux_tree_tapbuf_size3_mem mem_top_track_2 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size4_mem_0_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_0_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_0_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_0_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_top_track_4 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_0_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_1_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_1_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_1_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_top_track_6 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_1_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_2_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_2_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_2_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_top_track_8 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_2_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_3_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_3_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_3_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_top_track_10 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_3_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_4_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_4_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_4_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_top_track_14 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size4_mem_1_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_5_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_5_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_5_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_top_track_16 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_5_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_6_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_6_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_6_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_top_track_24 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size2_mem_2_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_7_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_7_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_7_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_right_track_4 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size4_mem_3_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_8_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_8_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_8_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_right_track_6 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_8_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_9_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_9_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_9_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_right_track_8 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_9_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_10_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_10_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_10_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_right_track_10 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_10_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_11_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_11_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_11_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_right_track_16 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size4_mem_5_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size3_mem_12_ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_12_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_12_sram_inv[0:1]));

	mux_tree_tapbuf_size3_mem mem_right_track_24 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size2_mem_5_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mux_tree_tapbuf_size3_13_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size3_13_sram_inv[0:1]));

	mux_tree_tapbuf_size2 mux_top_track_18 (
		.in({top_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, chanx_right_in[10]}),
		.sram(mux_tree_tapbuf_size2_0_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size2_0_sram_inv[0:1]),
		.out(chany_top_out[9]));

	mux_tree_tapbuf_size2 mux_top_track_20 (
		.in({top_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, chanx_right_in[11]}),
		.sram(mux_tree_tapbuf_size2_1_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size2_1_sram_inv[0:1]),
		.out(chany_top_out[10]));

	mux_tree_tapbuf_size2 mux_top_track_22 (
		.in({top_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, chanx_right_in[12]}),
		.sram(mux_tree_tapbuf_size2_2_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size2_2_sram_inv[0:1]),
		.out(chany_top_out[11]));

	mux_tree_tapbuf_size2 mux_right_track_18 (
		.in({chany_top_in[8], right_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size2_3_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size2_3_sram_inv[0:1]),
		.out(chanx_right_out[9]));

	mux_tree_tapbuf_size2 mux_right_track_20 (
		.in({chany_top_in[9], right_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size2_4_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size2_4_sram_inv[0:1]),
		.out(chanx_right_out[10]));

	mux_tree_tapbuf_size2 mux_right_track_22 (
		.in({chany_top_in[10], right_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_}),
		.sram(mux_tree_tapbuf_size2_5_sram[0:1]),
		.sram_inv(mux_tree_tapbuf_size2_5_sram_inv[0:1]),
		.out(chanx_right_out[11]));

	mux_tree_tapbuf_size2_mem mem_top_track_18 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_6_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size2_mem_0_ccff_tail),
		.mem_out(mux_tree_tapbuf_size2_0_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size2_0_sram_inv[0:1]));

	mux_tree_tapbuf_size2_mem mem_top_track_20 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size2_mem_0_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size2_mem_1_ccff_tail),
		.mem_out(mux_tree_tapbuf_size2_1_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size2_1_sram_inv[0:1]));

	mux_tree_tapbuf_size2_mem mem_top_track_22 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size2_mem_1_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size2_mem_2_ccff_tail),
		.mem_out(mux_tree_tapbuf_size2_2_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size2_2_sram_inv[0:1]));

	mux_tree_tapbuf_size2_mem mem_right_track_18 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size3_mem_12_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size2_mem_3_ccff_tail),
		.mem_out(mux_tree_tapbuf_size2_3_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size2_3_sram_inv[0:1]));

	mux_tree_tapbuf_size2_mem mem_right_track_20 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size2_mem_3_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size2_mem_4_ccff_tail),
		.mem_out(mux_tree_tapbuf_size2_4_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size2_4_sram_inv[0:1]));

	mux_tree_tapbuf_size2_mem mem_right_track_22 (
		.prog_clk(prog_clk),
		.ccff_head(mux_tree_tapbuf_size2_mem_4_ccff_tail),
		.ccff_tail(mux_tree_tapbuf_size2_mem_5_ccff_tail),
		.mem_out(mux_tree_tapbuf_size2_5_sram[0:1]),
		.mem_outb(mux_tree_tapbuf_size2_5_sram_inv[0:1]));

endmodule
// ----- END Verilog module for sb_0__0_ -----

//----- Default net type -----
`default_nettype none



