//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[6][2]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- Verilog module for sb_6__2_ -----
module sb_6__2_(pReset,
                prog_clk,
                chany_top_in,
                chany_bottom_in,
                chanx_left_in,
                left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_,
                left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_,
                left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_,
                left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_,
                left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_,
                left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_,
                left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_,
                left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_,
                ccff_head,
                chany_top_out,
                chany_bottom_out,
                chanx_left_out,
                bottom_grid_top_width_0_height_0_subtile_0__pin_outpad_0_,
                bottom_grid_top_width_0_height_0_subtile_1__pin_outpad_0_,
                bottom_grid_top_width_0_height_0_subtile_2__pin_outpad_0_,
                bottom_grid_top_width_0_height_0_subtile_3__pin_outpad_0_,
                bottom_grid_top_width_0_height_0_subtile_4__pin_outpad_0_,
                bottom_grid_top_width_0_height_0_subtile_5__pin_outpad_0_,
                bottom_grid_top_width_0_height_0_subtile_6__pin_outpad_0_,
                bottom_grid_top_width_0_height_0_subtile_7__pin_outpad_0_,
                ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [15:0] chany_top_in;
//----- INPUT PORTS -----
input [15:0] chany_bottom_in;
//----- INPUT PORTS -----
input [15:0] chanx_left_in;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [15:0] chany_top_out;
//----- OUTPUT PORTS -----
output [15:0] chany_bottom_out;
//----- OUTPUT PORTS -----
output [15:0] chanx_left_out;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_1__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_2__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_3__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_4__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_5__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_6__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_7__pin_outpad_0_;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [5:0] mux_2level_tapbuf_size40_0_sram;
wire [5:0] mux_2level_tapbuf_size40_0_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_10_sram;
wire [5:0] mux_2level_tapbuf_size40_10_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_11_sram;
wire [5:0] mux_2level_tapbuf_size40_11_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_12_sram;
wire [5:0] mux_2level_tapbuf_size40_12_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_13_sram;
wire [5:0] mux_2level_tapbuf_size40_13_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_14_sram;
wire [5:0] mux_2level_tapbuf_size40_14_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_15_sram;
wire [5:0] mux_2level_tapbuf_size40_15_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_16_sram;
wire [5:0] mux_2level_tapbuf_size40_16_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_17_sram;
wire [5:0] mux_2level_tapbuf_size40_17_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_18_sram;
wire [5:0] mux_2level_tapbuf_size40_18_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_19_sram;
wire [5:0] mux_2level_tapbuf_size40_19_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_1_sram;
wire [5:0] mux_2level_tapbuf_size40_1_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_20_sram;
wire [5:0] mux_2level_tapbuf_size40_20_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_21_sram;
wire [5:0] mux_2level_tapbuf_size40_21_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_22_sram;
wire [5:0] mux_2level_tapbuf_size40_22_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_23_sram;
wire [5:0] mux_2level_tapbuf_size40_23_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_2_sram;
wire [5:0] mux_2level_tapbuf_size40_2_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_3_sram;
wire [5:0] mux_2level_tapbuf_size40_3_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_4_sram;
wire [5:0] mux_2level_tapbuf_size40_4_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_5_sram;
wire [5:0] mux_2level_tapbuf_size40_5_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_6_sram;
wire [5:0] mux_2level_tapbuf_size40_6_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_7_sram;
wire [5:0] mux_2level_tapbuf_size40_7_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_8_sram;
wire [5:0] mux_2level_tapbuf_size40_8_sram_inv;
wire [5:0] mux_2level_tapbuf_size40_9_sram;
wire [5:0] mux_2level_tapbuf_size40_9_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_0_sram;
wire [5:0] mux_2level_tapbuf_size48_0_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_1_sram;
wire [5:0] mux_2level_tapbuf_size48_1_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_2_sram;
wire [5:0] mux_2level_tapbuf_size48_2_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_3_sram;
wire [5:0] mux_2level_tapbuf_size48_3_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_4_sram;
wire [5:0] mux_2level_tapbuf_size48_4_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_5_sram;
wire [5:0] mux_2level_tapbuf_size48_5_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_6_sram;
wire [5:0] mux_2level_tapbuf_size48_6_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_7_sram;
wire [5:0] mux_2level_tapbuf_size48_7_sram_inv;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[1] = chany_top_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[2] = chany_top_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[3] = chany_top_in[2];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out[5] = chany_top_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out[6] = chany_top_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out[7] = chany_top_in[6];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_bottom_out[9] = chany_top_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_bottom_out[10] = chany_top_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_bottom_out[11] = chany_top_in[10];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_bottom_out[13] = chany_top_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_bottom_out[14] = chany_top_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_bottom_out[15] = chany_top_in[14];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out[1] = chany_bottom_in[0];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out[2] = chany_bottom_in[1];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out[3] = chany_bottom_in[2];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_top_out[5] = chany_bottom_in[4];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_top_out[6] = chany_bottom_in[5];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_top_out[7] = chany_bottom_in[6];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_top_out[9] = chany_bottom_in[8];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_top_out[10] = chany_bottom_in[9];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_top_out[11] = chany_bottom_in[10];
// ----- Local connection due to Wire 28 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_top_out[13] = chany_bottom_in[12];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_top_out[14] = chany_bottom_in[13];
// ----- Local connection due to Wire 30 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_top_out[15] = chany_bottom_in[14];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40 mux_top_track_0 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size40_0_sram[5], mux_2level_tapbuf_size40_0_sram[4], mux_2level_tapbuf_size40_0_sram[3], mux_2level_tapbuf_size40_0_sram[2], mux_2level_tapbuf_size40_0_sram[1], mux_2level_tapbuf_size40_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_0_sram_inv[5], mux_2level_tapbuf_size40_0_sram_inv[4], mux_2level_tapbuf_size40_0_sram_inv[3], mux_2level_tapbuf_size40_0_sram_inv[2], mux_2level_tapbuf_size40_0_sram_inv[1], mux_2level_tapbuf_size40_0_sram_inv[0]}),
		.out(chany_top_out[0]));

	mux_2level_tapbuf_size40 mux_top_track_8 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size40_1_sram[5], mux_2level_tapbuf_size40_1_sram[4], mux_2level_tapbuf_size40_1_sram[3], mux_2level_tapbuf_size40_1_sram[2], mux_2level_tapbuf_size40_1_sram[1], mux_2level_tapbuf_size40_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_1_sram_inv[5], mux_2level_tapbuf_size40_1_sram_inv[4], mux_2level_tapbuf_size40_1_sram_inv[3], mux_2level_tapbuf_size40_1_sram_inv[2], mux_2level_tapbuf_size40_1_sram_inv[1], mux_2level_tapbuf_size40_1_sram_inv[0]}),
		.out(chany_top_out[4]));

	mux_2level_tapbuf_size40 mux_top_track_16 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size40_2_sram[5], mux_2level_tapbuf_size40_2_sram[4], mux_2level_tapbuf_size40_2_sram[3], mux_2level_tapbuf_size40_2_sram[2], mux_2level_tapbuf_size40_2_sram[1], mux_2level_tapbuf_size40_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_2_sram_inv[5], mux_2level_tapbuf_size40_2_sram_inv[4], mux_2level_tapbuf_size40_2_sram_inv[3], mux_2level_tapbuf_size40_2_sram_inv[2], mux_2level_tapbuf_size40_2_sram_inv[1], mux_2level_tapbuf_size40_2_sram_inv[0]}),
		.out(chany_top_out[8]));

	mux_2level_tapbuf_size40 mux_top_track_24 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0]}),
		.sram({mux_2level_tapbuf_size40_3_sram[5], mux_2level_tapbuf_size40_3_sram[4], mux_2level_tapbuf_size40_3_sram[3], mux_2level_tapbuf_size40_3_sram[2], mux_2level_tapbuf_size40_3_sram[1], mux_2level_tapbuf_size40_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_3_sram_inv[5], mux_2level_tapbuf_size40_3_sram_inv[4], mux_2level_tapbuf_size40_3_sram_inv[3], mux_2level_tapbuf_size40_3_sram_inv[2], mux_2level_tapbuf_size40_3_sram_inv[1], mux_2level_tapbuf_size40_3_sram_inv[0]}),
		.out(chany_top_out[12]));

	mux_2level_tapbuf_size40 mux_bottom_track_1 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_4_sram[5], mux_2level_tapbuf_size40_4_sram[4], mux_2level_tapbuf_size40_4_sram[3], mux_2level_tapbuf_size40_4_sram[2], mux_2level_tapbuf_size40_4_sram[1], mux_2level_tapbuf_size40_4_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_4_sram_inv[5], mux_2level_tapbuf_size40_4_sram_inv[4], mux_2level_tapbuf_size40_4_sram_inv[3], mux_2level_tapbuf_size40_4_sram_inv[2], mux_2level_tapbuf_size40_4_sram_inv[1], mux_2level_tapbuf_size40_4_sram_inv[0]}),
		.out(chany_bottom_out[0]));

	mux_2level_tapbuf_size40 mux_bottom_track_9 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_5_sram[5], mux_2level_tapbuf_size40_5_sram[4], mux_2level_tapbuf_size40_5_sram[3], mux_2level_tapbuf_size40_5_sram[2], mux_2level_tapbuf_size40_5_sram[1], mux_2level_tapbuf_size40_5_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_5_sram_inv[5], mux_2level_tapbuf_size40_5_sram_inv[4], mux_2level_tapbuf_size40_5_sram_inv[3], mux_2level_tapbuf_size40_5_sram_inv[2], mux_2level_tapbuf_size40_5_sram_inv[1], mux_2level_tapbuf_size40_5_sram_inv[0]}),
		.out(chany_bottom_out[4]));

	mux_2level_tapbuf_size40 mux_bottom_track_17 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_6_sram[5], mux_2level_tapbuf_size40_6_sram[4], mux_2level_tapbuf_size40_6_sram[3], mux_2level_tapbuf_size40_6_sram[2], mux_2level_tapbuf_size40_6_sram[1], mux_2level_tapbuf_size40_6_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_6_sram_inv[5], mux_2level_tapbuf_size40_6_sram_inv[4], mux_2level_tapbuf_size40_6_sram_inv[3], mux_2level_tapbuf_size40_6_sram_inv[2], mux_2level_tapbuf_size40_6_sram_inv[1], mux_2level_tapbuf_size40_6_sram_inv[0]}),
		.out(chany_bottom_out[8]));

	mux_2level_tapbuf_size40 mux_bottom_track_25 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_7_sram[5], mux_2level_tapbuf_size40_7_sram[4], mux_2level_tapbuf_size40_7_sram[3], mux_2level_tapbuf_size40_7_sram[2], mux_2level_tapbuf_size40_7_sram[1], mux_2level_tapbuf_size40_7_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_7_sram_inv[5], mux_2level_tapbuf_size40_7_sram_inv[4], mux_2level_tapbuf_size40_7_sram_inv[3], mux_2level_tapbuf_size40_7_sram_inv[2], mux_2level_tapbuf_size40_7_sram_inv[1], mux_2level_tapbuf_size40_7_sram_inv[0]}),
		.out(chany_bottom_out[12]));

	mux_2level_tapbuf_size40 mux_left_track_1 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_8_sram[5], mux_2level_tapbuf_size40_8_sram[4], mux_2level_tapbuf_size40_8_sram[3], mux_2level_tapbuf_size40_8_sram[2], mux_2level_tapbuf_size40_8_sram[1], mux_2level_tapbuf_size40_8_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_8_sram_inv[5], mux_2level_tapbuf_size40_8_sram_inv[4], mux_2level_tapbuf_size40_8_sram_inv[3], mux_2level_tapbuf_size40_8_sram_inv[2], mux_2level_tapbuf_size40_8_sram_inv[1], mux_2level_tapbuf_size40_8_sram_inv[0]}),
		.out(chanx_left_out[0]));

	mux_2level_tapbuf_size40 mux_left_track_3 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_9_sram[5], mux_2level_tapbuf_size40_9_sram[4], mux_2level_tapbuf_size40_9_sram[3], mux_2level_tapbuf_size40_9_sram[2], mux_2level_tapbuf_size40_9_sram[1], mux_2level_tapbuf_size40_9_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_9_sram_inv[5], mux_2level_tapbuf_size40_9_sram_inv[4], mux_2level_tapbuf_size40_9_sram_inv[3], mux_2level_tapbuf_size40_9_sram_inv[2], mux_2level_tapbuf_size40_9_sram_inv[1], mux_2level_tapbuf_size40_9_sram_inv[0]}),
		.out(chanx_left_out[1]));

	mux_2level_tapbuf_size40 mux_left_track_5 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_10_sram[5], mux_2level_tapbuf_size40_10_sram[4], mux_2level_tapbuf_size40_10_sram[3], mux_2level_tapbuf_size40_10_sram[2], mux_2level_tapbuf_size40_10_sram[1], mux_2level_tapbuf_size40_10_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_10_sram_inv[5], mux_2level_tapbuf_size40_10_sram_inv[4], mux_2level_tapbuf_size40_10_sram_inv[3], mux_2level_tapbuf_size40_10_sram_inv[2], mux_2level_tapbuf_size40_10_sram_inv[1], mux_2level_tapbuf_size40_10_sram_inv[0]}),
		.out(chanx_left_out[2]));

	mux_2level_tapbuf_size40 mux_left_track_7 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_11_sram[5], mux_2level_tapbuf_size40_11_sram[4], mux_2level_tapbuf_size40_11_sram[3], mux_2level_tapbuf_size40_11_sram[2], mux_2level_tapbuf_size40_11_sram[1], mux_2level_tapbuf_size40_11_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_11_sram_inv[5], mux_2level_tapbuf_size40_11_sram_inv[4], mux_2level_tapbuf_size40_11_sram_inv[3], mux_2level_tapbuf_size40_11_sram_inv[2], mux_2level_tapbuf_size40_11_sram_inv[1], mux_2level_tapbuf_size40_11_sram_inv[0]}),
		.out(chanx_left_out[3]));

	mux_2level_tapbuf_size40 mux_left_track_9 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_12_sram[5], mux_2level_tapbuf_size40_12_sram[4], mux_2level_tapbuf_size40_12_sram[3], mux_2level_tapbuf_size40_12_sram[2], mux_2level_tapbuf_size40_12_sram[1], mux_2level_tapbuf_size40_12_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_12_sram_inv[5], mux_2level_tapbuf_size40_12_sram_inv[4], mux_2level_tapbuf_size40_12_sram_inv[3], mux_2level_tapbuf_size40_12_sram_inv[2], mux_2level_tapbuf_size40_12_sram_inv[1], mux_2level_tapbuf_size40_12_sram_inv[0]}),
		.out(chanx_left_out[4]));

	mux_2level_tapbuf_size40 mux_left_track_11 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_13_sram[5], mux_2level_tapbuf_size40_13_sram[4], mux_2level_tapbuf_size40_13_sram[3], mux_2level_tapbuf_size40_13_sram[2], mux_2level_tapbuf_size40_13_sram[1], mux_2level_tapbuf_size40_13_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_13_sram_inv[5], mux_2level_tapbuf_size40_13_sram_inv[4], mux_2level_tapbuf_size40_13_sram_inv[3], mux_2level_tapbuf_size40_13_sram_inv[2], mux_2level_tapbuf_size40_13_sram_inv[1], mux_2level_tapbuf_size40_13_sram_inv[0]}),
		.out(chanx_left_out[5]));

	mux_2level_tapbuf_size40 mux_left_track_13 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_14_sram[5], mux_2level_tapbuf_size40_14_sram[4], mux_2level_tapbuf_size40_14_sram[3], mux_2level_tapbuf_size40_14_sram[2], mux_2level_tapbuf_size40_14_sram[1], mux_2level_tapbuf_size40_14_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_14_sram_inv[5], mux_2level_tapbuf_size40_14_sram_inv[4], mux_2level_tapbuf_size40_14_sram_inv[3], mux_2level_tapbuf_size40_14_sram_inv[2], mux_2level_tapbuf_size40_14_sram_inv[1], mux_2level_tapbuf_size40_14_sram_inv[0]}),
		.out(chanx_left_out[6]));

	mux_2level_tapbuf_size40 mux_left_track_15 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_15_sram[5], mux_2level_tapbuf_size40_15_sram[4], mux_2level_tapbuf_size40_15_sram[3], mux_2level_tapbuf_size40_15_sram[2], mux_2level_tapbuf_size40_15_sram[1], mux_2level_tapbuf_size40_15_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_15_sram_inv[5], mux_2level_tapbuf_size40_15_sram_inv[4], mux_2level_tapbuf_size40_15_sram_inv[3], mux_2level_tapbuf_size40_15_sram_inv[2], mux_2level_tapbuf_size40_15_sram_inv[1], mux_2level_tapbuf_size40_15_sram_inv[0]}),
		.out(chanx_left_out[7]));

	mux_2level_tapbuf_size40 mux_left_track_17 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_16_sram[5], mux_2level_tapbuf_size40_16_sram[4], mux_2level_tapbuf_size40_16_sram[3], mux_2level_tapbuf_size40_16_sram[2], mux_2level_tapbuf_size40_16_sram[1], mux_2level_tapbuf_size40_16_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_16_sram_inv[5], mux_2level_tapbuf_size40_16_sram_inv[4], mux_2level_tapbuf_size40_16_sram_inv[3], mux_2level_tapbuf_size40_16_sram_inv[2], mux_2level_tapbuf_size40_16_sram_inv[1], mux_2level_tapbuf_size40_16_sram_inv[0]}),
		.out(chanx_left_out[8]));

	mux_2level_tapbuf_size40 mux_left_track_19 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_17_sram[5], mux_2level_tapbuf_size40_17_sram[4], mux_2level_tapbuf_size40_17_sram[3], mux_2level_tapbuf_size40_17_sram[2], mux_2level_tapbuf_size40_17_sram[1], mux_2level_tapbuf_size40_17_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_17_sram_inv[5], mux_2level_tapbuf_size40_17_sram_inv[4], mux_2level_tapbuf_size40_17_sram_inv[3], mux_2level_tapbuf_size40_17_sram_inv[2], mux_2level_tapbuf_size40_17_sram_inv[1], mux_2level_tapbuf_size40_17_sram_inv[0]}),
		.out(chanx_left_out[9]));

	mux_2level_tapbuf_size40 mux_left_track_21 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_18_sram[5], mux_2level_tapbuf_size40_18_sram[4], mux_2level_tapbuf_size40_18_sram[3], mux_2level_tapbuf_size40_18_sram[2], mux_2level_tapbuf_size40_18_sram[1], mux_2level_tapbuf_size40_18_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_18_sram_inv[5], mux_2level_tapbuf_size40_18_sram_inv[4], mux_2level_tapbuf_size40_18_sram_inv[3], mux_2level_tapbuf_size40_18_sram_inv[2], mux_2level_tapbuf_size40_18_sram_inv[1], mux_2level_tapbuf_size40_18_sram_inv[0]}),
		.out(chanx_left_out[10]));

	mux_2level_tapbuf_size40 mux_left_track_23 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_19_sram[5], mux_2level_tapbuf_size40_19_sram[4], mux_2level_tapbuf_size40_19_sram[3], mux_2level_tapbuf_size40_19_sram[2], mux_2level_tapbuf_size40_19_sram[1], mux_2level_tapbuf_size40_19_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_19_sram_inv[5], mux_2level_tapbuf_size40_19_sram_inv[4], mux_2level_tapbuf_size40_19_sram_inv[3], mux_2level_tapbuf_size40_19_sram_inv[2], mux_2level_tapbuf_size40_19_sram_inv[1], mux_2level_tapbuf_size40_19_sram_inv[0]}),
		.out(chanx_left_out[11]));

	mux_2level_tapbuf_size40 mux_left_track_25 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_20_sram[5], mux_2level_tapbuf_size40_20_sram[4], mux_2level_tapbuf_size40_20_sram[3], mux_2level_tapbuf_size40_20_sram[2], mux_2level_tapbuf_size40_20_sram[1], mux_2level_tapbuf_size40_20_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_20_sram_inv[5], mux_2level_tapbuf_size40_20_sram_inv[4], mux_2level_tapbuf_size40_20_sram_inv[3], mux_2level_tapbuf_size40_20_sram_inv[2], mux_2level_tapbuf_size40_20_sram_inv[1], mux_2level_tapbuf_size40_20_sram_inv[0]}),
		.out(chanx_left_out[12]));

	mux_2level_tapbuf_size40 mux_left_track_27 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_21_sram[5], mux_2level_tapbuf_size40_21_sram[4], mux_2level_tapbuf_size40_21_sram[3], mux_2level_tapbuf_size40_21_sram[2], mux_2level_tapbuf_size40_21_sram[1], mux_2level_tapbuf_size40_21_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_21_sram_inv[5], mux_2level_tapbuf_size40_21_sram_inv[4], mux_2level_tapbuf_size40_21_sram_inv[3], mux_2level_tapbuf_size40_21_sram_inv[2], mux_2level_tapbuf_size40_21_sram_inv[1], mux_2level_tapbuf_size40_21_sram_inv[0]}),
		.out(chanx_left_out[13]));

	mux_2level_tapbuf_size40 mux_left_track_29 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_22_sram[5], mux_2level_tapbuf_size40_22_sram[4], mux_2level_tapbuf_size40_22_sram[3], mux_2level_tapbuf_size40_22_sram[2], mux_2level_tapbuf_size40_22_sram[1], mux_2level_tapbuf_size40_22_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_22_sram_inv[5], mux_2level_tapbuf_size40_22_sram_inv[4], mux_2level_tapbuf_size40_22_sram_inv[3], mux_2level_tapbuf_size40_22_sram_inv[2], mux_2level_tapbuf_size40_22_sram_inv[1], mux_2level_tapbuf_size40_22_sram_inv[0]}),
		.out(chanx_left_out[14]));

	mux_2level_tapbuf_size40 mux_left_track_31 (
		.in({left_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, left_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_23_sram[5], mux_2level_tapbuf_size40_23_sram[4], mux_2level_tapbuf_size40_23_sram[3], mux_2level_tapbuf_size40_23_sram[2], mux_2level_tapbuf_size40_23_sram[1], mux_2level_tapbuf_size40_23_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_23_sram_inv[5], mux_2level_tapbuf_size40_23_sram_inv[4], mux_2level_tapbuf_size40_23_sram_inv[3], mux_2level_tapbuf_size40_23_sram_inv[2], mux_2level_tapbuf_size40_23_sram_inv[1], mux_2level_tapbuf_size40_23_sram_inv[0]}),
		.out(chanx_left_out[15]));

	mux_2level_tapbuf_size40_mem mem_top_track_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_0_sram[5], mux_2level_tapbuf_size40_0_sram[4], mux_2level_tapbuf_size40_0_sram[3], mux_2level_tapbuf_size40_0_sram[2], mux_2level_tapbuf_size40_0_sram[1], mux_2level_tapbuf_size40_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_0_sram_inv[5], mux_2level_tapbuf_size40_0_sram_inv[4], mux_2level_tapbuf_size40_0_sram_inv[3], mux_2level_tapbuf_size40_0_sram_inv[2], mux_2level_tapbuf_size40_0_sram_inv[1], mux_2level_tapbuf_size40_0_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_top_track_8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_1_sram[5], mux_2level_tapbuf_size40_1_sram[4], mux_2level_tapbuf_size40_1_sram[3], mux_2level_tapbuf_size40_1_sram[2], mux_2level_tapbuf_size40_1_sram[1], mux_2level_tapbuf_size40_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_1_sram_inv[5], mux_2level_tapbuf_size40_1_sram_inv[4], mux_2level_tapbuf_size40_1_sram_inv[3], mux_2level_tapbuf_size40_1_sram_inv[2], mux_2level_tapbuf_size40_1_sram_inv[1], mux_2level_tapbuf_size40_1_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_top_track_16 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_2_sram[5], mux_2level_tapbuf_size40_2_sram[4], mux_2level_tapbuf_size40_2_sram[3], mux_2level_tapbuf_size40_2_sram[2], mux_2level_tapbuf_size40_2_sram[1], mux_2level_tapbuf_size40_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_2_sram_inv[5], mux_2level_tapbuf_size40_2_sram_inv[4], mux_2level_tapbuf_size40_2_sram_inv[3], mux_2level_tapbuf_size40_2_sram_inv[2], mux_2level_tapbuf_size40_2_sram_inv[1], mux_2level_tapbuf_size40_2_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_top_track_24 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_3_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_3_sram[5], mux_2level_tapbuf_size40_3_sram[4], mux_2level_tapbuf_size40_3_sram[3], mux_2level_tapbuf_size40_3_sram[2], mux_2level_tapbuf_size40_3_sram[1], mux_2level_tapbuf_size40_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_3_sram_inv[5], mux_2level_tapbuf_size40_3_sram_inv[4], mux_2level_tapbuf_size40_3_sram_inv[3], mux_2level_tapbuf_size40_3_sram_inv[2], mux_2level_tapbuf_size40_3_sram_inv[1], mux_2level_tapbuf_size40_3_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_4_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_4_sram[5], mux_2level_tapbuf_size40_4_sram[4], mux_2level_tapbuf_size40_4_sram[3], mux_2level_tapbuf_size40_4_sram[2], mux_2level_tapbuf_size40_4_sram[1], mux_2level_tapbuf_size40_4_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_4_sram_inv[5], mux_2level_tapbuf_size40_4_sram_inv[4], mux_2level_tapbuf_size40_4_sram_inv[3], mux_2level_tapbuf_size40_4_sram_inv[2], mux_2level_tapbuf_size40_4_sram_inv[1], mux_2level_tapbuf_size40_4_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_5_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_5_sram[5], mux_2level_tapbuf_size40_5_sram[4], mux_2level_tapbuf_size40_5_sram[3], mux_2level_tapbuf_size40_5_sram[2], mux_2level_tapbuf_size40_5_sram[1], mux_2level_tapbuf_size40_5_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_5_sram_inv[5], mux_2level_tapbuf_size40_5_sram_inv[4], mux_2level_tapbuf_size40_5_sram_inv[3], mux_2level_tapbuf_size40_5_sram_inv[2], mux_2level_tapbuf_size40_5_sram_inv[1], mux_2level_tapbuf_size40_5_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_6_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_6_sram[5], mux_2level_tapbuf_size40_6_sram[4], mux_2level_tapbuf_size40_6_sram[3], mux_2level_tapbuf_size40_6_sram[2], mux_2level_tapbuf_size40_6_sram[1], mux_2level_tapbuf_size40_6_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_6_sram_inv[5], mux_2level_tapbuf_size40_6_sram_inv[4], mux_2level_tapbuf_size40_6_sram_inv[3], mux_2level_tapbuf_size40_6_sram_inv[2], mux_2level_tapbuf_size40_6_sram_inv[1], mux_2level_tapbuf_size40_6_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_6_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_7_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_7_sram[5], mux_2level_tapbuf_size40_7_sram[4], mux_2level_tapbuf_size40_7_sram[3], mux_2level_tapbuf_size40_7_sram[2], mux_2level_tapbuf_size40_7_sram[1], mux_2level_tapbuf_size40_7_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_7_sram_inv[5], mux_2level_tapbuf_size40_7_sram_inv[4], mux_2level_tapbuf_size40_7_sram_inv[3], mux_2level_tapbuf_size40_7_sram_inv[2], mux_2level_tapbuf_size40_7_sram_inv[1], mux_2level_tapbuf_size40_7_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_7_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_8_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_8_sram[5], mux_2level_tapbuf_size40_8_sram[4], mux_2level_tapbuf_size40_8_sram[3], mux_2level_tapbuf_size40_8_sram[2], mux_2level_tapbuf_size40_8_sram[1], mux_2level_tapbuf_size40_8_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_8_sram_inv[5], mux_2level_tapbuf_size40_8_sram_inv[4], mux_2level_tapbuf_size40_8_sram_inv[3], mux_2level_tapbuf_size40_8_sram_inv[2], mux_2level_tapbuf_size40_8_sram_inv[1], mux_2level_tapbuf_size40_8_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_8_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_9_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_9_sram[5], mux_2level_tapbuf_size40_9_sram[4], mux_2level_tapbuf_size40_9_sram[3], mux_2level_tapbuf_size40_9_sram[2], mux_2level_tapbuf_size40_9_sram[1], mux_2level_tapbuf_size40_9_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_9_sram_inv[5], mux_2level_tapbuf_size40_9_sram_inv[4], mux_2level_tapbuf_size40_9_sram_inv[3], mux_2level_tapbuf_size40_9_sram_inv[2], mux_2level_tapbuf_size40_9_sram_inv[1], mux_2level_tapbuf_size40_9_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_9_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_10_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_10_sram[5], mux_2level_tapbuf_size40_10_sram[4], mux_2level_tapbuf_size40_10_sram[3], mux_2level_tapbuf_size40_10_sram[2], mux_2level_tapbuf_size40_10_sram[1], mux_2level_tapbuf_size40_10_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_10_sram_inv[5], mux_2level_tapbuf_size40_10_sram_inv[4], mux_2level_tapbuf_size40_10_sram_inv[3], mux_2level_tapbuf_size40_10_sram_inv[2], mux_2level_tapbuf_size40_10_sram_inv[1], mux_2level_tapbuf_size40_10_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_10_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_11_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_11_sram[5], mux_2level_tapbuf_size40_11_sram[4], mux_2level_tapbuf_size40_11_sram[3], mux_2level_tapbuf_size40_11_sram[2], mux_2level_tapbuf_size40_11_sram[1], mux_2level_tapbuf_size40_11_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_11_sram_inv[5], mux_2level_tapbuf_size40_11_sram_inv[4], mux_2level_tapbuf_size40_11_sram_inv[3], mux_2level_tapbuf_size40_11_sram_inv[2], mux_2level_tapbuf_size40_11_sram_inv[1], mux_2level_tapbuf_size40_11_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_11_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_12_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_12_sram[5], mux_2level_tapbuf_size40_12_sram[4], mux_2level_tapbuf_size40_12_sram[3], mux_2level_tapbuf_size40_12_sram[2], mux_2level_tapbuf_size40_12_sram[1], mux_2level_tapbuf_size40_12_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_12_sram_inv[5], mux_2level_tapbuf_size40_12_sram_inv[4], mux_2level_tapbuf_size40_12_sram_inv[3], mux_2level_tapbuf_size40_12_sram_inv[2], mux_2level_tapbuf_size40_12_sram_inv[1], mux_2level_tapbuf_size40_12_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_11 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_12_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_13_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_13_sram[5], mux_2level_tapbuf_size40_13_sram[4], mux_2level_tapbuf_size40_13_sram[3], mux_2level_tapbuf_size40_13_sram[2], mux_2level_tapbuf_size40_13_sram[1], mux_2level_tapbuf_size40_13_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_13_sram_inv[5], mux_2level_tapbuf_size40_13_sram_inv[4], mux_2level_tapbuf_size40_13_sram_inv[3], mux_2level_tapbuf_size40_13_sram_inv[2], mux_2level_tapbuf_size40_13_sram_inv[1], mux_2level_tapbuf_size40_13_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_13 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_13_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_14_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_14_sram[5], mux_2level_tapbuf_size40_14_sram[4], mux_2level_tapbuf_size40_14_sram[3], mux_2level_tapbuf_size40_14_sram[2], mux_2level_tapbuf_size40_14_sram[1], mux_2level_tapbuf_size40_14_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_14_sram_inv[5], mux_2level_tapbuf_size40_14_sram_inv[4], mux_2level_tapbuf_size40_14_sram_inv[3], mux_2level_tapbuf_size40_14_sram_inv[2], mux_2level_tapbuf_size40_14_sram_inv[1], mux_2level_tapbuf_size40_14_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_15 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_14_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_15_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_15_sram[5], mux_2level_tapbuf_size40_15_sram[4], mux_2level_tapbuf_size40_15_sram[3], mux_2level_tapbuf_size40_15_sram[2], mux_2level_tapbuf_size40_15_sram[1], mux_2level_tapbuf_size40_15_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_15_sram_inv[5], mux_2level_tapbuf_size40_15_sram_inv[4], mux_2level_tapbuf_size40_15_sram_inv[3], mux_2level_tapbuf_size40_15_sram_inv[2], mux_2level_tapbuf_size40_15_sram_inv[1], mux_2level_tapbuf_size40_15_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_15_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_16_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_16_sram[5], mux_2level_tapbuf_size40_16_sram[4], mux_2level_tapbuf_size40_16_sram[3], mux_2level_tapbuf_size40_16_sram[2], mux_2level_tapbuf_size40_16_sram[1], mux_2level_tapbuf_size40_16_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_16_sram_inv[5], mux_2level_tapbuf_size40_16_sram_inv[4], mux_2level_tapbuf_size40_16_sram_inv[3], mux_2level_tapbuf_size40_16_sram_inv[2], mux_2level_tapbuf_size40_16_sram_inv[1], mux_2level_tapbuf_size40_16_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_19 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_16_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_17_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_17_sram[5], mux_2level_tapbuf_size40_17_sram[4], mux_2level_tapbuf_size40_17_sram[3], mux_2level_tapbuf_size40_17_sram[2], mux_2level_tapbuf_size40_17_sram[1], mux_2level_tapbuf_size40_17_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_17_sram_inv[5], mux_2level_tapbuf_size40_17_sram_inv[4], mux_2level_tapbuf_size40_17_sram_inv[3], mux_2level_tapbuf_size40_17_sram_inv[2], mux_2level_tapbuf_size40_17_sram_inv[1], mux_2level_tapbuf_size40_17_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_21 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_17_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_18_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_18_sram[5], mux_2level_tapbuf_size40_18_sram[4], mux_2level_tapbuf_size40_18_sram[3], mux_2level_tapbuf_size40_18_sram[2], mux_2level_tapbuf_size40_18_sram[1], mux_2level_tapbuf_size40_18_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_18_sram_inv[5], mux_2level_tapbuf_size40_18_sram_inv[4], mux_2level_tapbuf_size40_18_sram_inv[3], mux_2level_tapbuf_size40_18_sram_inv[2], mux_2level_tapbuf_size40_18_sram_inv[1], mux_2level_tapbuf_size40_18_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_23 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_18_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_19_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_19_sram[5], mux_2level_tapbuf_size40_19_sram[4], mux_2level_tapbuf_size40_19_sram[3], mux_2level_tapbuf_size40_19_sram[2], mux_2level_tapbuf_size40_19_sram[1], mux_2level_tapbuf_size40_19_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_19_sram_inv[5], mux_2level_tapbuf_size40_19_sram_inv[4], mux_2level_tapbuf_size40_19_sram_inv[3], mux_2level_tapbuf_size40_19_sram_inv[2], mux_2level_tapbuf_size40_19_sram_inv[1], mux_2level_tapbuf_size40_19_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_19_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_20_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_20_sram[5], mux_2level_tapbuf_size40_20_sram[4], mux_2level_tapbuf_size40_20_sram[3], mux_2level_tapbuf_size40_20_sram[2], mux_2level_tapbuf_size40_20_sram[1], mux_2level_tapbuf_size40_20_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_20_sram_inv[5], mux_2level_tapbuf_size40_20_sram_inv[4], mux_2level_tapbuf_size40_20_sram_inv[3], mux_2level_tapbuf_size40_20_sram_inv[2], mux_2level_tapbuf_size40_20_sram_inv[1], mux_2level_tapbuf_size40_20_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_27 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_20_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_21_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_21_sram[5], mux_2level_tapbuf_size40_21_sram[4], mux_2level_tapbuf_size40_21_sram[3], mux_2level_tapbuf_size40_21_sram[2], mux_2level_tapbuf_size40_21_sram[1], mux_2level_tapbuf_size40_21_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_21_sram_inv[5], mux_2level_tapbuf_size40_21_sram_inv[4], mux_2level_tapbuf_size40_21_sram_inv[3], mux_2level_tapbuf_size40_21_sram_inv[2], mux_2level_tapbuf_size40_21_sram_inv[1], mux_2level_tapbuf_size40_21_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_29 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_21_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_22_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_22_sram[5], mux_2level_tapbuf_size40_22_sram[4], mux_2level_tapbuf_size40_22_sram[3], mux_2level_tapbuf_size40_22_sram[2], mux_2level_tapbuf_size40_22_sram[1], mux_2level_tapbuf_size40_22_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_22_sram_inv[5], mux_2level_tapbuf_size40_22_sram_inv[4], mux_2level_tapbuf_size40_22_sram_inv[3], mux_2level_tapbuf_size40_22_sram_inv[2], mux_2level_tapbuf_size40_22_sram_inv[1], mux_2level_tapbuf_size40_22_sram_inv[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_31 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_22_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_23_ccff_tail),
		.mem_out({mux_2level_tapbuf_size40_23_sram[5], mux_2level_tapbuf_size40_23_sram[4], mux_2level_tapbuf_size40_23_sram[3], mux_2level_tapbuf_size40_23_sram[2], mux_2level_tapbuf_size40_23_sram[1], mux_2level_tapbuf_size40_23_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_23_sram_inv[5], mux_2level_tapbuf_size40_23_sram_inv[4], mux_2level_tapbuf_size40_23_sram_inv[3], mux_2level_tapbuf_size40_23_sram_inv[2], mux_2level_tapbuf_size40_23_sram_inv[1], mux_2level_tapbuf_size40_23_sram_inv[0]}));

	mux_2level_tapbuf_size48 mux_top_ipin_0 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_0_sram[5], mux_2level_tapbuf_size48_0_sram[4], mux_2level_tapbuf_size48_0_sram[3], mux_2level_tapbuf_size48_0_sram[2], mux_2level_tapbuf_size48_0_sram[1], mux_2level_tapbuf_size48_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_0_sram_inv[5], mux_2level_tapbuf_size48_0_sram_inv[4], mux_2level_tapbuf_size48_0_sram_inv[3], mux_2level_tapbuf_size48_0_sram_inv[2], mux_2level_tapbuf_size48_0_sram_inv[1], mux_2level_tapbuf_size48_0_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_outpad_0_));

	mux_2level_tapbuf_size48 mux_top_ipin_1 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_1_sram[5], mux_2level_tapbuf_size48_1_sram[4], mux_2level_tapbuf_size48_1_sram[3], mux_2level_tapbuf_size48_1_sram[2], mux_2level_tapbuf_size48_1_sram[1], mux_2level_tapbuf_size48_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_1_sram_inv[5], mux_2level_tapbuf_size48_1_sram_inv[4], mux_2level_tapbuf_size48_1_sram_inv[3], mux_2level_tapbuf_size48_1_sram_inv[2], mux_2level_tapbuf_size48_1_sram_inv[1], mux_2level_tapbuf_size48_1_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_1__pin_outpad_0_));

	mux_2level_tapbuf_size48 mux_top_ipin_2 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_2_sram[5], mux_2level_tapbuf_size48_2_sram[4], mux_2level_tapbuf_size48_2_sram[3], mux_2level_tapbuf_size48_2_sram[2], mux_2level_tapbuf_size48_2_sram[1], mux_2level_tapbuf_size48_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_2_sram_inv[5], mux_2level_tapbuf_size48_2_sram_inv[4], mux_2level_tapbuf_size48_2_sram_inv[3], mux_2level_tapbuf_size48_2_sram_inv[2], mux_2level_tapbuf_size48_2_sram_inv[1], mux_2level_tapbuf_size48_2_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_2__pin_outpad_0_));

	mux_2level_tapbuf_size48 mux_top_ipin_3 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_3_sram[5], mux_2level_tapbuf_size48_3_sram[4], mux_2level_tapbuf_size48_3_sram[3], mux_2level_tapbuf_size48_3_sram[2], mux_2level_tapbuf_size48_3_sram[1], mux_2level_tapbuf_size48_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_3_sram_inv[5], mux_2level_tapbuf_size48_3_sram_inv[4], mux_2level_tapbuf_size48_3_sram_inv[3], mux_2level_tapbuf_size48_3_sram_inv[2], mux_2level_tapbuf_size48_3_sram_inv[1], mux_2level_tapbuf_size48_3_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_3__pin_outpad_0_));

	mux_2level_tapbuf_size48 mux_top_ipin_4 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_4_sram[5], mux_2level_tapbuf_size48_4_sram[4], mux_2level_tapbuf_size48_4_sram[3], mux_2level_tapbuf_size48_4_sram[2], mux_2level_tapbuf_size48_4_sram[1], mux_2level_tapbuf_size48_4_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_4_sram_inv[5], mux_2level_tapbuf_size48_4_sram_inv[4], mux_2level_tapbuf_size48_4_sram_inv[3], mux_2level_tapbuf_size48_4_sram_inv[2], mux_2level_tapbuf_size48_4_sram_inv[1], mux_2level_tapbuf_size48_4_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_4__pin_outpad_0_));

	mux_2level_tapbuf_size48 mux_top_ipin_5 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_5_sram[5], mux_2level_tapbuf_size48_5_sram[4], mux_2level_tapbuf_size48_5_sram[3], mux_2level_tapbuf_size48_5_sram[2], mux_2level_tapbuf_size48_5_sram[1], mux_2level_tapbuf_size48_5_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_5_sram_inv[5], mux_2level_tapbuf_size48_5_sram_inv[4], mux_2level_tapbuf_size48_5_sram_inv[3], mux_2level_tapbuf_size48_5_sram_inv[2], mux_2level_tapbuf_size48_5_sram_inv[1], mux_2level_tapbuf_size48_5_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_5__pin_outpad_0_));

	mux_2level_tapbuf_size48 mux_top_ipin_6 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_6_sram[5], mux_2level_tapbuf_size48_6_sram[4], mux_2level_tapbuf_size48_6_sram[3], mux_2level_tapbuf_size48_6_sram[2], mux_2level_tapbuf_size48_6_sram[1], mux_2level_tapbuf_size48_6_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_6_sram_inv[5], mux_2level_tapbuf_size48_6_sram_inv[4], mux_2level_tapbuf_size48_6_sram_inv[3], mux_2level_tapbuf_size48_6_sram_inv[2], mux_2level_tapbuf_size48_6_sram_inv[1], mux_2level_tapbuf_size48_6_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_6__pin_outpad_0_));

	mux_2level_tapbuf_size48 mux_top_ipin_7 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[11], chany_bottom_in[7], chany_bottom_in[3], chany_top_in[15], chany_bottom_in[14], chany_top_in[14], chany_bottom_in[13], chany_top_in[13], chany_bottom_in[12], chany_top_in[12], chany_top_in[11], chany_bottom_in[10], chany_top_in[10], chany_bottom_in[9], chany_top_in[9], chany_bottom_in[8], chany_top_in[8], chany_top_in[7], chany_bottom_in[6], chany_top_in[6], chany_bottom_in[5], chany_top_in[5], chany_bottom_in[4], chany_top_in[4], chany_top_in[3], chany_bottom_in[2], chany_top_in[2], chany_bottom_in[1], chany_top_in[1], chany_bottom_in[0], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_7_sram[5], mux_2level_tapbuf_size48_7_sram[4], mux_2level_tapbuf_size48_7_sram[3], mux_2level_tapbuf_size48_7_sram[2], mux_2level_tapbuf_size48_7_sram[1], mux_2level_tapbuf_size48_7_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_7_sram_inv[5], mux_2level_tapbuf_size48_7_sram_inv[4], mux_2level_tapbuf_size48_7_sram_inv[3], mux_2level_tapbuf_size48_7_sram_inv[2], mux_2level_tapbuf_size48_7_sram_inv[1], mux_2level_tapbuf_size48_7_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_7__pin_outpad_0_));

	mux_2level_tapbuf_size48_mem mem_top_ipin_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_23_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_0_sram[5], mux_2level_tapbuf_size48_0_sram[4], mux_2level_tapbuf_size48_0_sram[3], mux_2level_tapbuf_size48_0_sram[2], mux_2level_tapbuf_size48_0_sram[1], mux_2level_tapbuf_size48_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_0_sram_inv[5], mux_2level_tapbuf_size48_0_sram_inv[4], mux_2level_tapbuf_size48_0_sram_inv[3], mux_2level_tapbuf_size48_0_sram_inv[2], mux_2level_tapbuf_size48_0_sram_inv[1], mux_2level_tapbuf_size48_0_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_ipin_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_1_sram[5], mux_2level_tapbuf_size48_1_sram[4], mux_2level_tapbuf_size48_1_sram[3], mux_2level_tapbuf_size48_1_sram[2], mux_2level_tapbuf_size48_1_sram[1], mux_2level_tapbuf_size48_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_1_sram_inv[5], mux_2level_tapbuf_size48_1_sram_inv[4], mux_2level_tapbuf_size48_1_sram_inv[3], mux_2level_tapbuf_size48_1_sram_inv[2], mux_2level_tapbuf_size48_1_sram_inv[1], mux_2level_tapbuf_size48_1_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_ipin_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_2_sram[5], mux_2level_tapbuf_size48_2_sram[4], mux_2level_tapbuf_size48_2_sram[3], mux_2level_tapbuf_size48_2_sram[2], mux_2level_tapbuf_size48_2_sram[1], mux_2level_tapbuf_size48_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_2_sram_inv[5], mux_2level_tapbuf_size48_2_sram_inv[4], mux_2level_tapbuf_size48_2_sram_inv[3], mux_2level_tapbuf_size48_2_sram_inv[2], mux_2level_tapbuf_size48_2_sram_inv[1], mux_2level_tapbuf_size48_2_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_ipin_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_3_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_3_sram[5], mux_2level_tapbuf_size48_3_sram[4], mux_2level_tapbuf_size48_3_sram[3], mux_2level_tapbuf_size48_3_sram[2], mux_2level_tapbuf_size48_3_sram[1], mux_2level_tapbuf_size48_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_3_sram_inv[5], mux_2level_tapbuf_size48_3_sram_inv[4], mux_2level_tapbuf_size48_3_sram_inv[3], mux_2level_tapbuf_size48_3_sram_inv[2], mux_2level_tapbuf_size48_3_sram_inv[1], mux_2level_tapbuf_size48_3_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_ipin_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_4_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_4_sram[5], mux_2level_tapbuf_size48_4_sram[4], mux_2level_tapbuf_size48_4_sram[3], mux_2level_tapbuf_size48_4_sram[2], mux_2level_tapbuf_size48_4_sram[1], mux_2level_tapbuf_size48_4_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_4_sram_inv[5], mux_2level_tapbuf_size48_4_sram_inv[4], mux_2level_tapbuf_size48_4_sram_inv[3], mux_2level_tapbuf_size48_4_sram_inv[2], mux_2level_tapbuf_size48_4_sram_inv[1], mux_2level_tapbuf_size48_4_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_ipin_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_5_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_5_sram[5], mux_2level_tapbuf_size48_5_sram[4], mux_2level_tapbuf_size48_5_sram[3], mux_2level_tapbuf_size48_5_sram[2], mux_2level_tapbuf_size48_5_sram[1], mux_2level_tapbuf_size48_5_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_5_sram_inv[5], mux_2level_tapbuf_size48_5_sram_inv[4], mux_2level_tapbuf_size48_5_sram_inv[3], mux_2level_tapbuf_size48_5_sram_inv[2], mux_2level_tapbuf_size48_5_sram_inv[1], mux_2level_tapbuf_size48_5_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_ipin_6 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_6_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_6_sram[5], mux_2level_tapbuf_size48_6_sram[4], mux_2level_tapbuf_size48_6_sram[3], mux_2level_tapbuf_size48_6_sram[2], mux_2level_tapbuf_size48_6_sram[1], mux_2level_tapbuf_size48_6_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_6_sram_inv[5], mux_2level_tapbuf_size48_6_sram_inv[4], mux_2level_tapbuf_size48_6_sram_inv[3], mux_2level_tapbuf_size48_6_sram_inv[2], mux_2level_tapbuf_size48_6_sram_inv[1], mux_2level_tapbuf_size48_6_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_ipin_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_7_sram[5], mux_2level_tapbuf_size48_7_sram[4], mux_2level_tapbuf_size48_7_sram[3], mux_2level_tapbuf_size48_7_sram[2], mux_2level_tapbuf_size48_7_sram[1], mux_2level_tapbuf_size48_7_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_7_sram_inv[5], mux_2level_tapbuf_size48_7_sram_inv[4], mux_2level_tapbuf_size48_7_sram_inv[3], mux_2level_tapbuf_size48_7_sram_inv[2], mux_2level_tapbuf_size48_7_sram_inv[1], mux_2level_tapbuf_size48_7_sram_inv[0]}));

endmodule
// ----- END Verilog module for sb_6__2_ -----



