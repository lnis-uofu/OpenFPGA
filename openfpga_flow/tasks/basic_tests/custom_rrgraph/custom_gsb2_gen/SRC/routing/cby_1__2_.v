//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Connection Blocks[1][2]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for cby_1__2_ -----
module cby_1__2_(pReset,
                 prog_clk,
                 chany_bottom_in,
                 chany_top_in,
                 ccff_head,
                 chany_bottom_out,
                 chany_top_out,
                 left_grid_right_width_0_height_0_subtile_0__pin_outpad_0_,
                 left_grid_right_width_0_height_0_subtile_1__pin_outpad_0_,
                 left_grid_right_width_0_height_0_subtile_2__pin_outpad_0_,
                 left_grid_right_width_0_height_0_subtile_3__pin_outpad_0_,
                 left_grid_right_width_0_height_0_subtile_4__pin_outpad_0_,
                 left_grid_right_width_0_height_0_subtile_5__pin_outpad_0_,
                 left_grid_right_width_0_height_0_subtile_6__pin_outpad_0_,
                 left_grid_right_width_0_height_0_subtile_7__pin_outpad_0_,
                 ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [15:0] chany_bottom_in;
//----- INPUT PORTS -----
input [15:0] chany_top_in;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [15:0] chany_bottom_out;
//----- OUTPUT PORTS -----
output [15:0] chany_top_out;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_0__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_1__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_2__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_3__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_4__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_5__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_6__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_right_width_0_height_0_subtile_7__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [79:0] cby_1__config_group_mem_size80_0_mem_out;
wire [79:0] cby_1__config_group_mem_size80_0_mem_outb;
wire [9:0] mux_2level_tapbuf_size16_0_sram;
wire [9:0] mux_2level_tapbuf_size16_0_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_1_sram;
wire [9:0] mux_2level_tapbuf_size16_1_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_2_sram;
wire [9:0] mux_2level_tapbuf_size16_2_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_3_sram;
wire [9:0] mux_2level_tapbuf_size16_3_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_4_sram;
wire [9:0] mux_2level_tapbuf_size16_4_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_5_sram;
wire [9:0] mux_2level_tapbuf_size16_5_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_6_sram;
wire [9:0] mux_2level_tapbuf_size16_6_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_7_sram;
wire [9:0] mux_2level_tapbuf_size16_7_sram_inv;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[0] = chany_bottom_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[1] = chany_bottom_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[2] = chany_bottom_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[3] = chany_bottom_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[4] = chany_bottom_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[5] = chany_bottom_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[6] = chany_bottom_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[7] = chany_bottom_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[8] = chany_bottom_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[9] = chany_bottom_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[10] = chany_bottom_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[11] = chany_bottom_in[11];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[12] = chany_bottom_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[13] = chany_bottom_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[14] = chany_bottom_in[14];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[15] = chany_bottom_in[15];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[0] = chany_top_in[0];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[1] = chany_top_in[1];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[2] = chany_top_in[2];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[3] = chany_top_in[3];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[4] = chany_top_in[4];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[5] = chany_top_in[5];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[6] = chany_top_in[6];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[7] = chany_top_in[7];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[8] = chany_top_in[8];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[9] = chany_top_in[9];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[10] = chany_top_in[10];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[11] = chany_top_in[11];
// ----- Local connection due to Wire 28 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[12] = chany_top_in[12];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[13] = chany_top_in[13];
// ----- Local connection due to Wire 30 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[14] = chany_top_in[14];
// ----- Local connection due to Wire 31 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_bottom_out[15] = chany_top_in[15];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size16 mux_right_ipin_0 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_0_sram[9], mux_2level_tapbuf_size16_0_sram[8], mux_2level_tapbuf_size16_0_sram[7], mux_2level_tapbuf_size16_0_sram[6], mux_2level_tapbuf_size16_0_sram[5], mux_2level_tapbuf_size16_0_sram[4], mux_2level_tapbuf_size16_0_sram[3], mux_2level_tapbuf_size16_0_sram[2], mux_2level_tapbuf_size16_0_sram[1], mux_2level_tapbuf_size16_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_0_sram_inv[9], mux_2level_tapbuf_size16_0_sram_inv[8], mux_2level_tapbuf_size16_0_sram_inv[7], mux_2level_tapbuf_size16_0_sram_inv[6], mux_2level_tapbuf_size16_0_sram_inv[5], mux_2level_tapbuf_size16_0_sram_inv[4], mux_2level_tapbuf_size16_0_sram_inv[3], mux_2level_tapbuf_size16_0_sram_inv[2], mux_2level_tapbuf_size16_0_sram_inv[1], mux_2level_tapbuf_size16_0_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_0__pin_outpad_0_));

	mux_2level_tapbuf_size16 mux_right_ipin_1 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_1_sram[9], mux_2level_tapbuf_size16_1_sram[8], mux_2level_tapbuf_size16_1_sram[7], mux_2level_tapbuf_size16_1_sram[6], mux_2level_tapbuf_size16_1_sram[5], mux_2level_tapbuf_size16_1_sram[4], mux_2level_tapbuf_size16_1_sram[3], mux_2level_tapbuf_size16_1_sram[2], mux_2level_tapbuf_size16_1_sram[1], mux_2level_tapbuf_size16_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_1_sram_inv[9], mux_2level_tapbuf_size16_1_sram_inv[8], mux_2level_tapbuf_size16_1_sram_inv[7], mux_2level_tapbuf_size16_1_sram_inv[6], mux_2level_tapbuf_size16_1_sram_inv[5], mux_2level_tapbuf_size16_1_sram_inv[4], mux_2level_tapbuf_size16_1_sram_inv[3], mux_2level_tapbuf_size16_1_sram_inv[2], mux_2level_tapbuf_size16_1_sram_inv[1], mux_2level_tapbuf_size16_1_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_1__pin_outpad_0_));

	mux_2level_tapbuf_size16 mux_right_ipin_2 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_2_sram[9], mux_2level_tapbuf_size16_2_sram[8], mux_2level_tapbuf_size16_2_sram[7], mux_2level_tapbuf_size16_2_sram[6], mux_2level_tapbuf_size16_2_sram[5], mux_2level_tapbuf_size16_2_sram[4], mux_2level_tapbuf_size16_2_sram[3], mux_2level_tapbuf_size16_2_sram[2], mux_2level_tapbuf_size16_2_sram[1], mux_2level_tapbuf_size16_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_2_sram_inv[9], mux_2level_tapbuf_size16_2_sram_inv[8], mux_2level_tapbuf_size16_2_sram_inv[7], mux_2level_tapbuf_size16_2_sram_inv[6], mux_2level_tapbuf_size16_2_sram_inv[5], mux_2level_tapbuf_size16_2_sram_inv[4], mux_2level_tapbuf_size16_2_sram_inv[3], mux_2level_tapbuf_size16_2_sram_inv[2], mux_2level_tapbuf_size16_2_sram_inv[1], mux_2level_tapbuf_size16_2_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_2__pin_outpad_0_));

	mux_2level_tapbuf_size16 mux_right_ipin_3 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_3_sram[9], mux_2level_tapbuf_size16_3_sram[8], mux_2level_tapbuf_size16_3_sram[7], mux_2level_tapbuf_size16_3_sram[6], mux_2level_tapbuf_size16_3_sram[5], mux_2level_tapbuf_size16_3_sram[4], mux_2level_tapbuf_size16_3_sram[3], mux_2level_tapbuf_size16_3_sram[2], mux_2level_tapbuf_size16_3_sram[1], mux_2level_tapbuf_size16_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_3_sram_inv[9], mux_2level_tapbuf_size16_3_sram_inv[8], mux_2level_tapbuf_size16_3_sram_inv[7], mux_2level_tapbuf_size16_3_sram_inv[6], mux_2level_tapbuf_size16_3_sram_inv[5], mux_2level_tapbuf_size16_3_sram_inv[4], mux_2level_tapbuf_size16_3_sram_inv[3], mux_2level_tapbuf_size16_3_sram_inv[2], mux_2level_tapbuf_size16_3_sram_inv[1], mux_2level_tapbuf_size16_3_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_3__pin_outpad_0_));

	mux_2level_tapbuf_size16 mux_right_ipin_4 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_4_sram[9], mux_2level_tapbuf_size16_4_sram[8], mux_2level_tapbuf_size16_4_sram[7], mux_2level_tapbuf_size16_4_sram[6], mux_2level_tapbuf_size16_4_sram[5], mux_2level_tapbuf_size16_4_sram[4], mux_2level_tapbuf_size16_4_sram[3], mux_2level_tapbuf_size16_4_sram[2], mux_2level_tapbuf_size16_4_sram[1], mux_2level_tapbuf_size16_4_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_4_sram_inv[9], mux_2level_tapbuf_size16_4_sram_inv[8], mux_2level_tapbuf_size16_4_sram_inv[7], mux_2level_tapbuf_size16_4_sram_inv[6], mux_2level_tapbuf_size16_4_sram_inv[5], mux_2level_tapbuf_size16_4_sram_inv[4], mux_2level_tapbuf_size16_4_sram_inv[3], mux_2level_tapbuf_size16_4_sram_inv[2], mux_2level_tapbuf_size16_4_sram_inv[1], mux_2level_tapbuf_size16_4_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_4__pin_outpad_0_));

	mux_2level_tapbuf_size16 mux_right_ipin_5 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_5_sram[9], mux_2level_tapbuf_size16_5_sram[8], mux_2level_tapbuf_size16_5_sram[7], mux_2level_tapbuf_size16_5_sram[6], mux_2level_tapbuf_size16_5_sram[5], mux_2level_tapbuf_size16_5_sram[4], mux_2level_tapbuf_size16_5_sram[3], mux_2level_tapbuf_size16_5_sram[2], mux_2level_tapbuf_size16_5_sram[1], mux_2level_tapbuf_size16_5_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_5_sram_inv[9], mux_2level_tapbuf_size16_5_sram_inv[8], mux_2level_tapbuf_size16_5_sram_inv[7], mux_2level_tapbuf_size16_5_sram_inv[6], mux_2level_tapbuf_size16_5_sram_inv[5], mux_2level_tapbuf_size16_5_sram_inv[4], mux_2level_tapbuf_size16_5_sram_inv[3], mux_2level_tapbuf_size16_5_sram_inv[2], mux_2level_tapbuf_size16_5_sram_inv[1], mux_2level_tapbuf_size16_5_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_5__pin_outpad_0_));

	mux_2level_tapbuf_size16 mux_right_ipin_6 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_6_sram[9], mux_2level_tapbuf_size16_6_sram[8], mux_2level_tapbuf_size16_6_sram[7], mux_2level_tapbuf_size16_6_sram[6], mux_2level_tapbuf_size16_6_sram[5], mux_2level_tapbuf_size16_6_sram[4], mux_2level_tapbuf_size16_6_sram[3], mux_2level_tapbuf_size16_6_sram[2], mux_2level_tapbuf_size16_6_sram[1], mux_2level_tapbuf_size16_6_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_6_sram_inv[9], mux_2level_tapbuf_size16_6_sram_inv[8], mux_2level_tapbuf_size16_6_sram_inv[7], mux_2level_tapbuf_size16_6_sram_inv[6], mux_2level_tapbuf_size16_6_sram_inv[5], mux_2level_tapbuf_size16_6_sram_inv[4], mux_2level_tapbuf_size16_6_sram_inv[3], mux_2level_tapbuf_size16_6_sram_inv[2], mux_2level_tapbuf_size16_6_sram_inv[1], mux_2level_tapbuf_size16_6_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_6__pin_outpad_0_));

	mux_2level_tapbuf_size16 mux_right_ipin_7 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size16_7_sram[9], mux_2level_tapbuf_size16_7_sram[8], mux_2level_tapbuf_size16_7_sram[7], mux_2level_tapbuf_size16_7_sram[6], mux_2level_tapbuf_size16_7_sram[5], mux_2level_tapbuf_size16_7_sram[4], mux_2level_tapbuf_size16_7_sram[3], mux_2level_tapbuf_size16_7_sram[2], mux_2level_tapbuf_size16_7_sram[1], mux_2level_tapbuf_size16_7_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_7_sram_inv[9], mux_2level_tapbuf_size16_7_sram_inv[8], mux_2level_tapbuf_size16_7_sram_inv[7], mux_2level_tapbuf_size16_7_sram_inv[6], mux_2level_tapbuf_size16_7_sram_inv[5], mux_2level_tapbuf_size16_7_sram_inv[4], mux_2level_tapbuf_size16_7_sram_inv[3], mux_2level_tapbuf_size16_7_sram_inv[2], mux_2level_tapbuf_size16_7_sram_inv[1], mux_2level_tapbuf_size16_7_sram_inv[0]}),
		.out(left_grid_right_width_0_height_0_subtile_7__pin_outpad_0_));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_0 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[9], cby_1__config_group_mem_size80_0_mem_out[8], cby_1__config_group_mem_size80_0_mem_out[7], cby_1__config_group_mem_size80_0_mem_out[6], cby_1__config_group_mem_size80_0_mem_out[5], cby_1__config_group_mem_size80_0_mem_out[4], cby_1__config_group_mem_size80_0_mem_out[3], cby_1__config_group_mem_size80_0_mem_out[2], cby_1__config_group_mem_size80_0_mem_out[1], cby_1__config_group_mem_size80_0_mem_out[0]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[9], cby_1__config_group_mem_size80_0_mem_outb[8], cby_1__config_group_mem_size80_0_mem_outb[7], cby_1__config_group_mem_size80_0_mem_outb[6], cby_1__config_group_mem_size80_0_mem_outb[5], cby_1__config_group_mem_size80_0_mem_outb[4], cby_1__config_group_mem_size80_0_mem_outb[3], cby_1__config_group_mem_size80_0_mem_outb[2], cby_1__config_group_mem_size80_0_mem_outb[1], cby_1__config_group_mem_size80_0_mem_outb[0]}),
		.mem_out({mux_2level_tapbuf_size16_0_sram[9], mux_2level_tapbuf_size16_0_sram[8], mux_2level_tapbuf_size16_0_sram[7], mux_2level_tapbuf_size16_0_sram[6], mux_2level_tapbuf_size16_0_sram[5], mux_2level_tapbuf_size16_0_sram[4], mux_2level_tapbuf_size16_0_sram[3], mux_2level_tapbuf_size16_0_sram[2], mux_2level_tapbuf_size16_0_sram[1], mux_2level_tapbuf_size16_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_0_sram_inv[9], mux_2level_tapbuf_size16_0_sram_inv[8], mux_2level_tapbuf_size16_0_sram_inv[7], mux_2level_tapbuf_size16_0_sram_inv[6], mux_2level_tapbuf_size16_0_sram_inv[5], mux_2level_tapbuf_size16_0_sram_inv[4], mux_2level_tapbuf_size16_0_sram_inv[3], mux_2level_tapbuf_size16_0_sram_inv[2], mux_2level_tapbuf_size16_0_sram_inv[1], mux_2level_tapbuf_size16_0_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_1 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[19], cby_1__config_group_mem_size80_0_mem_out[18], cby_1__config_group_mem_size80_0_mem_out[17], cby_1__config_group_mem_size80_0_mem_out[16], cby_1__config_group_mem_size80_0_mem_out[15], cby_1__config_group_mem_size80_0_mem_out[14], cby_1__config_group_mem_size80_0_mem_out[13], cby_1__config_group_mem_size80_0_mem_out[12], cby_1__config_group_mem_size80_0_mem_out[11], cby_1__config_group_mem_size80_0_mem_out[10]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[19], cby_1__config_group_mem_size80_0_mem_outb[18], cby_1__config_group_mem_size80_0_mem_outb[17], cby_1__config_group_mem_size80_0_mem_outb[16], cby_1__config_group_mem_size80_0_mem_outb[15], cby_1__config_group_mem_size80_0_mem_outb[14], cby_1__config_group_mem_size80_0_mem_outb[13], cby_1__config_group_mem_size80_0_mem_outb[12], cby_1__config_group_mem_size80_0_mem_outb[11], cby_1__config_group_mem_size80_0_mem_outb[10]}),
		.mem_out({mux_2level_tapbuf_size16_1_sram[9], mux_2level_tapbuf_size16_1_sram[8], mux_2level_tapbuf_size16_1_sram[7], mux_2level_tapbuf_size16_1_sram[6], mux_2level_tapbuf_size16_1_sram[5], mux_2level_tapbuf_size16_1_sram[4], mux_2level_tapbuf_size16_1_sram[3], mux_2level_tapbuf_size16_1_sram[2], mux_2level_tapbuf_size16_1_sram[1], mux_2level_tapbuf_size16_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_1_sram_inv[9], mux_2level_tapbuf_size16_1_sram_inv[8], mux_2level_tapbuf_size16_1_sram_inv[7], mux_2level_tapbuf_size16_1_sram_inv[6], mux_2level_tapbuf_size16_1_sram_inv[5], mux_2level_tapbuf_size16_1_sram_inv[4], mux_2level_tapbuf_size16_1_sram_inv[3], mux_2level_tapbuf_size16_1_sram_inv[2], mux_2level_tapbuf_size16_1_sram_inv[1], mux_2level_tapbuf_size16_1_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_2 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[29], cby_1__config_group_mem_size80_0_mem_out[28], cby_1__config_group_mem_size80_0_mem_out[27], cby_1__config_group_mem_size80_0_mem_out[26], cby_1__config_group_mem_size80_0_mem_out[25], cby_1__config_group_mem_size80_0_mem_out[24], cby_1__config_group_mem_size80_0_mem_out[23], cby_1__config_group_mem_size80_0_mem_out[22], cby_1__config_group_mem_size80_0_mem_out[21], cby_1__config_group_mem_size80_0_mem_out[20]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[29], cby_1__config_group_mem_size80_0_mem_outb[28], cby_1__config_group_mem_size80_0_mem_outb[27], cby_1__config_group_mem_size80_0_mem_outb[26], cby_1__config_group_mem_size80_0_mem_outb[25], cby_1__config_group_mem_size80_0_mem_outb[24], cby_1__config_group_mem_size80_0_mem_outb[23], cby_1__config_group_mem_size80_0_mem_outb[22], cby_1__config_group_mem_size80_0_mem_outb[21], cby_1__config_group_mem_size80_0_mem_outb[20]}),
		.mem_out({mux_2level_tapbuf_size16_2_sram[9], mux_2level_tapbuf_size16_2_sram[8], mux_2level_tapbuf_size16_2_sram[7], mux_2level_tapbuf_size16_2_sram[6], mux_2level_tapbuf_size16_2_sram[5], mux_2level_tapbuf_size16_2_sram[4], mux_2level_tapbuf_size16_2_sram[3], mux_2level_tapbuf_size16_2_sram[2], mux_2level_tapbuf_size16_2_sram[1], mux_2level_tapbuf_size16_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_2_sram_inv[9], mux_2level_tapbuf_size16_2_sram_inv[8], mux_2level_tapbuf_size16_2_sram_inv[7], mux_2level_tapbuf_size16_2_sram_inv[6], mux_2level_tapbuf_size16_2_sram_inv[5], mux_2level_tapbuf_size16_2_sram_inv[4], mux_2level_tapbuf_size16_2_sram_inv[3], mux_2level_tapbuf_size16_2_sram_inv[2], mux_2level_tapbuf_size16_2_sram_inv[1], mux_2level_tapbuf_size16_2_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_3 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[39], cby_1__config_group_mem_size80_0_mem_out[38], cby_1__config_group_mem_size80_0_mem_out[37], cby_1__config_group_mem_size80_0_mem_out[36], cby_1__config_group_mem_size80_0_mem_out[35], cby_1__config_group_mem_size80_0_mem_out[34], cby_1__config_group_mem_size80_0_mem_out[33], cby_1__config_group_mem_size80_0_mem_out[32], cby_1__config_group_mem_size80_0_mem_out[31], cby_1__config_group_mem_size80_0_mem_out[30]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[39], cby_1__config_group_mem_size80_0_mem_outb[38], cby_1__config_group_mem_size80_0_mem_outb[37], cby_1__config_group_mem_size80_0_mem_outb[36], cby_1__config_group_mem_size80_0_mem_outb[35], cby_1__config_group_mem_size80_0_mem_outb[34], cby_1__config_group_mem_size80_0_mem_outb[33], cby_1__config_group_mem_size80_0_mem_outb[32], cby_1__config_group_mem_size80_0_mem_outb[31], cby_1__config_group_mem_size80_0_mem_outb[30]}),
		.mem_out({mux_2level_tapbuf_size16_3_sram[9], mux_2level_tapbuf_size16_3_sram[8], mux_2level_tapbuf_size16_3_sram[7], mux_2level_tapbuf_size16_3_sram[6], mux_2level_tapbuf_size16_3_sram[5], mux_2level_tapbuf_size16_3_sram[4], mux_2level_tapbuf_size16_3_sram[3], mux_2level_tapbuf_size16_3_sram[2], mux_2level_tapbuf_size16_3_sram[1], mux_2level_tapbuf_size16_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_3_sram_inv[9], mux_2level_tapbuf_size16_3_sram_inv[8], mux_2level_tapbuf_size16_3_sram_inv[7], mux_2level_tapbuf_size16_3_sram_inv[6], mux_2level_tapbuf_size16_3_sram_inv[5], mux_2level_tapbuf_size16_3_sram_inv[4], mux_2level_tapbuf_size16_3_sram_inv[3], mux_2level_tapbuf_size16_3_sram_inv[2], mux_2level_tapbuf_size16_3_sram_inv[1], mux_2level_tapbuf_size16_3_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_4 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[49], cby_1__config_group_mem_size80_0_mem_out[48], cby_1__config_group_mem_size80_0_mem_out[47], cby_1__config_group_mem_size80_0_mem_out[46], cby_1__config_group_mem_size80_0_mem_out[45], cby_1__config_group_mem_size80_0_mem_out[44], cby_1__config_group_mem_size80_0_mem_out[43], cby_1__config_group_mem_size80_0_mem_out[42], cby_1__config_group_mem_size80_0_mem_out[41], cby_1__config_group_mem_size80_0_mem_out[40]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[49], cby_1__config_group_mem_size80_0_mem_outb[48], cby_1__config_group_mem_size80_0_mem_outb[47], cby_1__config_group_mem_size80_0_mem_outb[46], cby_1__config_group_mem_size80_0_mem_outb[45], cby_1__config_group_mem_size80_0_mem_outb[44], cby_1__config_group_mem_size80_0_mem_outb[43], cby_1__config_group_mem_size80_0_mem_outb[42], cby_1__config_group_mem_size80_0_mem_outb[41], cby_1__config_group_mem_size80_0_mem_outb[40]}),
		.mem_out({mux_2level_tapbuf_size16_4_sram[9], mux_2level_tapbuf_size16_4_sram[8], mux_2level_tapbuf_size16_4_sram[7], mux_2level_tapbuf_size16_4_sram[6], mux_2level_tapbuf_size16_4_sram[5], mux_2level_tapbuf_size16_4_sram[4], mux_2level_tapbuf_size16_4_sram[3], mux_2level_tapbuf_size16_4_sram[2], mux_2level_tapbuf_size16_4_sram[1], mux_2level_tapbuf_size16_4_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_4_sram_inv[9], mux_2level_tapbuf_size16_4_sram_inv[8], mux_2level_tapbuf_size16_4_sram_inv[7], mux_2level_tapbuf_size16_4_sram_inv[6], mux_2level_tapbuf_size16_4_sram_inv[5], mux_2level_tapbuf_size16_4_sram_inv[4], mux_2level_tapbuf_size16_4_sram_inv[3], mux_2level_tapbuf_size16_4_sram_inv[2], mux_2level_tapbuf_size16_4_sram_inv[1], mux_2level_tapbuf_size16_4_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_5 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[59], cby_1__config_group_mem_size80_0_mem_out[58], cby_1__config_group_mem_size80_0_mem_out[57], cby_1__config_group_mem_size80_0_mem_out[56], cby_1__config_group_mem_size80_0_mem_out[55], cby_1__config_group_mem_size80_0_mem_out[54], cby_1__config_group_mem_size80_0_mem_out[53], cby_1__config_group_mem_size80_0_mem_out[52], cby_1__config_group_mem_size80_0_mem_out[51], cby_1__config_group_mem_size80_0_mem_out[50]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[59], cby_1__config_group_mem_size80_0_mem_outb[58], cby_1__config_group_mem_size80_0_mem_outb[57], cby_1__config_group_mem_size80_0_mem_outb[56], cby_1__config_group_mem_size80_0_mem_outb[55], cby_1__config_group_mem_size80_0_mem_outb[54], cby_1__config_group_mem_size80_0_mem_outb[53], cby_1__config_group_mem_size80_0_mem_outb[52], cby_1__config_group_mem_size80_0_mem_outb[51], cby_1__config_group_mem_size80_0_mem_outb[50]}),
		.mem_out({mux_2level_tapbuf_size16_5_sram[9], mux_2level_tapbuf_size16_5_sram[8], mux_2level_tapbuf_size16_5_sram[7], mux_2level_tapbuf_size16_5_sram[6], mux_2level_tapbuf_size16_5_sram[5], mux_2level_tapbuf_size16_5_sram[4], mux_2level_tapbuf_size16_5_sram[3], mux_2level_tapbuf_size16_5_sram[2], mux_2level_tapbuf_size16_5_sram[1], mux_2level_tapbuf_size16_5_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_5_sram_inv[9], mux_2level_tapbuf_size16_5_sram_inv[8], mux_2level_tapbuf_size16_5_sram_inv[7], mux_2level_tapbuf_size16_5_sram_inv[6], mux_2level_tapbuf_size16_5_sram_inv[5], mux_2level_tapbuf_size16_5_sram_inv[4], mux_2level_tapbuf_size16_5_sram_inv[3], mux_2level_tapbuf_size16_5_sram_inv[2], mux_2level_tapbuf_size16_5_sram_inv[1], mux_2level_tapbuf_size16_5_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_6 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[69], cby_1__config_group_mem_size80_0_mem_out[68], cby_1__config_group_mem_size80_0_mem_out[67], cby_1__config_group_mem_size80_0_mem_out[66], cby_1__config_group_mem_size80_0_mem_out[65], cby_1__config_group_mem_size80_0_mem_out[64], cby_1__config_group_mem_size80_0_mem_out[63], cby_1__config_group_mem_size80_0_mem_out[62], cby_1__config_group_mem_size80_0_mem_out[61], cby_1__config_group_mem_size80_0_mem_out[60]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[69], cby_1__config_group_mem_size80_0_mem_outb[68], cby_1__config_group_mem_size80_0_mem_outb[67], cby_1__config_group_mem_size80_0_mem_outb[66], cby_1__config_group_mem_size80_0_mem_outb[65], cby_1__config_group_mem_size80_0_mem_outb[64], cby_1__config_group_mem_size80_0_mem_outb[63], cby_1__config_group_mem_size80_0_mem_outb[62], cby_1__config_group_mem_size80_0_mem_outb[61], cby_1__config_group_mem_size80_0_mem_outb[60]}),
		.mem_out({mux_2level_tapbuf_size16_6_sram[9], mux_2level_tapbuf_size16_6_sram[8], mux_2level_tapbuf_size16_6_sram[7], mux_2level_tapbuf_size16_6_sram[6], mux_2level_tapbuf_size16_6_sram[5], mux_2level_tapbuf_size16_6_sram[4], mux_2level_tapbuf_size16_6_sram[3], mux_2level_tapbuf_size16_6_sram[2], mux_2level_tapbuf_size16_6_sram[1], mux_2level_tapbuf_size16_6_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_6_sram_inv[9], mux_2level_tapbuf_size16_6_sram_inv[8], mux_2level_tapbuf_size16_6_sram_inv[7], mux_2level_tapbuf_size16_6_sram_inv[6], mux_2level_tapbuf_size16_6_sram_inv[5], mux_2level_tapbuf_size16_6_sram_inv[4], mux_2level_tapbuf_size16_6_sram_inv[3], mux_2level_tapbuf_size16_6_sram_inv[2], mux_2level_tapbuf_size16_6_sram_inv[1], mux_2level_tapbuf_size16_6_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_right_ipin_7 (
		.feedthrough_mem_in({cby_1__config_group_mem_size80_0_mem_out[79], cby_1__config_group_mem_size80_0_mem_out[78], cby_1__config_group_mem_size80_0_mem_out[77], cby_1__config_group_mem_size80_0_mem_out[76], cby_1__config_group_mem_size80_0_mem_out[75], cby_1__config_group_mem_size80_0_mem_out[74], cby_1__config_group_mem_size80_0_mem_out[73], cby_1__config_group_mem_size80_0_mem_out[72], cby_1__config_group_mem_size80_0_mem_out[71], cby_1__config_group_mem_size80_0_mem_out[70]}),
		.feedthrough_mem_inb({cby_1__config_group_mem_size80_0_mem_outb[79], cby_1__config_group_mem_size80_0_mem_outb[78], cby_1__config_group_mem_size80_0_mem_outb[77], cby_1__config_group_mem_size80_0_mem_outb[76], cby_1__config_group_mem_size80_0_mem_outb[75], cby_1__config_group_mem_size80_0_mem_outb[74], cby_1__config_group_mem_size80_0_mem_outb[73], cby_1__config_group_mem_size80_0_mem_outb[72], cby_1__config_group_mem_size80_0_mem_outb[71], cby_1__config_group_mem_size80_0_mem_outb[70]}),
		.mem_out({mux_2level_tapbuf_size16_7_sram[9], mux_2level_tapbuf_size16_7_sram[8], mux_2level_tapbuf_size16_7_sram[7], mux_2level_tapbuf_size16_7_sram[6], mux_2level_tapbuf_size16_7_sram[5], mux_2level_tapbuf_size16_7_sram[4], mux_2level_tapbuf_size16_7_sram[3], mux_2level_tapbuf_size16_7_sram[2], mux_2level_tapbuf_size16_7_sram[1], mux_2level_tapbuf_size16_7_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_7_sram_inv[9], mux_2level_tapbuf_size16_7_sram_inv[8], mux_2level_tapbuf_size16_7_sram_inv[7], mux_2level_tapbuf_size16_7_sram_inv[6], mux_2level_tapbuf_size16_7_sram_inv[5], mux_2level_tapbuf_size16_7_sram_inv[4], mux_2level_tapbuf_size16_7_sram_inv[3], mux_2level_tapbuf_size16_7_sram_inv[2], mux_2level_tapbuf_size16_7_sram_inv[1], mux_2level_tapbuf_size16_7_sram_inv[0]}));

	cby_1__config_group_mem_size80 cby_1__config_group_mem_size80 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.mem_out({cby_1__config_group_mem_size80_0_mem_out[79], cby_1__config_group_mem_size80_0_mem_out[78], cby_1__config_group_mem_size80_0_mem_out[77], cby_1__config_group_mem_size80_0_mem_out[76], cby_1__config_group_mem_size80_0_mem_out[75], cby_1__config_group_mem_size80_0_mem_out[74], cby_1__config_group_mem_size80_0_mem_out[73], cby_1__config_group_mem_size80_0_mem_out[72], cby_1__config_group_mem_size80_0_mem_out[71], cby_1__config_group_mem_size80_0_mem_out[70], cby_1__config_group_mem_size80_0_mem_out[69], cby_1__config_group_mem_size80_0_mem_out[68], cby_1__config_group_mem_size80_0_mem_out[67], cby_1__config_group_mem_size80_0_mem_out[66], cby_1__config_group_mem_size80_0_mem_out[65], cby_1__config_group_mem_size80_0_mem_out[64], cby_1__config_group_mem_size80_0_mem_out[63], cby_1__config_group_mem_size80_0_mem_out[62], cby_1__config_group_mem_size80_0_mem_out[61], cby_1__config_group_mem_size80_0_mem_out[60], cby_1__config_group_mem_size80_0_mem_out[59], cby_1__config_group_mem_size80_0_mem_out[58], cby_1__config_group_mem_size80_0_mem_out[57], cby_1__config_group_mem_size80_0_mem_out[56], cby_1__config_group_mem_size80_0_mem_out[55], cby_1__config_group_mem_size80_0_mem_out[54], cby_1__config_group_mem_size80_0_mem_out[53], cby_1__config_group_mem_size80_0_mem_out[52], cby_1__config_group_mem_size80_0_mem_out[51], cby_1__config_group_mem_size80_0_mem_out[50], cby_1__config_group_mem_size80_0_mem_out[49], cby_1__config_group_mem_size80_0_mem_out[48], cby_1__config_group_mem_size80_0_mem_out[47], cby_1__config_group_mem_size80_0_mem_out[46], cby_1__config_group_mem_size80_0_mem_out[45], cby_1__config_group_mem_size80_0_mem_out[44], cby_1__config_group_mem_size80_0_mem_out[43], cby_1__config_group_mem_size80_0_mem_out[42], cby_1__config_group_mem_size80_0_mem_out[41], cby_1__config_group_mem_size80_0_mem_out[40], cby_1__config_group_mem_size80_0_mem_out[39], cby_1__config_group_mem_size80_0_mem_out[38], cby_1__config_group_mem_size80_0_mem_out[37], cby_1__config_group_mem_size80_0_mem_out[36], cby_1__config_group_mem_size80_0_mem_out[35], cby_1__config_group_mem_size80_0_mem_out[34], cby_1__config_group_mem_size80_0_mem_out[33], cby_1__config_group_mem_size80_0_mem_out[32], cby_1__config_group_mem_size80_0_mem_out[31], cby_1__config_group_mem_size80_0_mem_out[30], cby_1__config_group_mem_size80_0_mem_out[29], cby_1__config_group_mem_size80_0_mem_out[28], cby_1__config_group_mem_size80_0_mem_out[27], cby_1__config_group_mem_size80_0_mem_out[26], cby_1__config_group_mem_size80_0_mem_out[25], cby_1__config_group_mem_size80_0_mem_out[24], cby_1__config_group_mem_size80_0_mem_out[23], cby_1__config_group_mem_size80_0_mem_out[22], cby_1__config_group_mem_size80_0_mem_out[21], cby_1__config_group_mem_size80_0_mem_out[20], cby_1__config_group_mem_size80_0_mem_out[19], cby_1__config_group_mem_size80_0_mem_out[18], cby_1__config_group_mem_size80_0_mem_out[17], cby_1__config_group_mem_size80_0_mem_out[16], cby_1__config_group_mem_size80_0_mem_out[15], cby_1__config_group_mem_size80_0_mem_out[14], cby_1__config_group_mem_size80_0_mem_out[13], cby_1__config_group_mem_size80_0_mem_out[12], cby_1__config_group_mem_size80_0_mem_out[11], cby_1__config_group_mem_size80_0_mem_out[10], cby_1__config_group_mem_size80_0_mem_out[9], cby_1__config_group_mem_size80_0_mem_out[8], cby_1__config_group_mem_size80_0_mem_out[7], cby_1__config_group_mem_size80_0_mem_out[6], cby_1__config_group_mem_size80_0_mem_out[5], cby_1__config_group_mem_size80_0_mem_out[4], cby_1__config_group_mem_size80_0_mem_out[3], cby_1__config_group_mem_size80_0_mem_out[2], cby_1__config_group_mem_size80_0_mem_out[1], cby_1__config_group_mem_size80_0_mem_out[0]}),
		.mem_outb({cby_1__config_group_mem_size80_0_mem_outb[79], cby_1__config_group_mem_size80_0_mem_outb[78], cby_1__config_group_mem_size80_0_mem_outb[77], cby_1__config_group_mem_size80_0_mem_outb[76], cby_1__config_group_mem_size80_0_mem_outb[75], cby_1__config_group_mem_size80_0_mem_outb[74], cby_1__config_group_mem_size80_0_mem_outb[73], cby_1__config_group_mem_size80_0_mem_outb[72], cby_1__config_group_mem_size80_0_mem_outb[71], cby_1__config_group_mem_size80_0_mem_outb[70], cby_1__config_group_mem_size80_0_mem_outb[69], cby_1__config_group_mem_size80_0_mem_outb[68], cby_1__config_group_mem_size80_0_mem_outb[67], cby_1__config_group_mem_size80_0_mem_outb[66], cby_1__config_group_mem_size80_0_mem_outb[65], cby_1__config_group_mem_size80_0_mem_outb[64], cby_1__config_group_mem_size80_0_mem_outb[63], cby_1__config_group_mem_size80_0_mem_outb[62], cby_1__config_group_mem_size80_0_mem_outb[61], cby_1__config_group_mem_size80_0_mem_outb[60], cby_1__config_group_mem_size80_0_mem_outb[59], cby_1__config_group_mem_size80_0_mem_outb[58], cby_1__config_group_mem_size80_0_mem_outb[57], cby_1__config_group_mem_size80_0_mem_outb[56], cby_1__config_group_mem_size80_0_mem_outb[55], cby_1__config_group_mem_size80_0_mem_outb[54], cby_1__config_group_mem_size80_0_mem_outb[53], cby_1__config_group_mem_size80_0_mem_outb[52], cby_1__config_group_mem_size80_0_mem_outb[51], cby_1__config_group_mem_size80_0_mem_outb[50], cby_1__config_group_mem_size80_0_mem_outb[49], cby_1__config_group_mem_size80_0_mem_outb[48], cby_1__config_group_mem_size80_0_mem_outb[47], cby_1__config_group_mem_size80_0_mem_outb[46], cby_1__config_group_mem_size80_0_mem_outb[45], cby_1__config_group_mem_size80_0_mem_outb[44], cby_1__config_group_mem_size80_0_mem_outb[43], cby_1__config_group_mem_size80_0_mem_outb[42], cby_1__config_group_mem_size80_0_mem_outb[41], cby_1__config_group_mem_size80_0_mem_outb[40], cby_1__config_group_mem_size80_0_mem_outb[39], cby_1__config_group_mem_size80_0_mem_outb[38], cby_1__config_group_mem_size80_0_mem_outb[37], cby_1__config_group_mem_size80_0_mem_outb[36], cby_1__config_group_mem_size80_0_mem_outb[35], cby_1__config_group_mem_size80_0_mem_outb[34], cby_1__config_group_mem_size80_0_mem_outb[33], cby_1__config_group_mem_size80_0_mem_outb[32], cby_1__config_group_mem_size80_0_mem_outb[31], cby_1__config_group_mem_size80_0_mem_outb[30], cby_1__config_group_mem_size80_0_mem_outb[29], cby_1__config_group_mem_size80_0_mem_outb[28], cby_1__config_group_mem_size80_0_mem_outb[27], cby_1__config_group_mem_size80_0_mem_outb[26], cby_1__config_group_mem_size80_0_mem_outb[25], cby_1__config_group_mem_size80_0_mem_outb[24], cby_1__config_group_mem_size80_0_mem_outb[23], cby_1__config_group_mem_size80_0_mem_outb[22], cby_1__config_group_mem_size80_0_mem_outb[21], cby_1__config_group_mem_size80_0_mem_outb[20], cby_1__config_group_mem_size80_0_mem_outb[19], cby_1__config_group_mem_size80_0_mem_outb[18], cby_1__config_group_mem_size80_0_mem_outb[17], cby_1__config_group_mem_size80_0_mem_outb[16], cby_1__config_group_mem_size80_0_mem_outb[15], cby_1__config_group_mem_size80_0_mem_outb[14], cby_1__config_group_mem_size80_0_mem_outb[13], cby_1__config_group_mem_size80_0_mem_outb[12], cby_1__config_group_mem_size80_0_mem_outb[11], cby_1__config_group_mem_size80_0_mem_outb[10], cby_1__config_group_mem_size80_0_mem_outb[9], cby_1__config_group_mem_size80_0_mem_outb[8], cby_1__config_group_mem_size80_0_mem_outb[7], cby_1__config_group_mem_size80_0_mem_outb[6], cby_1__config_group_mem_size80_0_mem_outb[5], cby_1__config_group_mem_size80_0_mem_outb[4], cby_1__config_group_mem_size80_0_mem_outb[3], cby_1__config_group_mem_size80_0_mem_outb[2], cby_1__config_group_mem_size80_0_mem_outb[1], cby_1__config_group_mem_size80_0_mem_outb[0]}),
		.ccff_tail(ccff_tail));

endmodule
// ----- END Verilog module for cby_1__2_ -----

//----- Default net type -----
`default_nettype wire




