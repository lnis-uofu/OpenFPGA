//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[2][2]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- Verilog module for sb_2__2_ -----
module sb_2__2_(pReset,
                prog_clk,
                chany_top_in,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_0_,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_1_,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_2_,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_3_,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_4_,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_5_,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_6_,
                top_left_grid_right_width_0_height_0_subtile_0__pin_O_7_,
                chanx_right_in,
                chany_bottom_in,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_0_,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_1_,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_2_,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_3_,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_4_,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_5_,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_6_,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_7_,
                chanx_left_in,
                ccff_head,
                chany_top_out,
                chanx_right_out,
                chany_bottom_out,
                chanx_left_out,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_0_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_1_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_2_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_3_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_4_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_5_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_6_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_7_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_8_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_9_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_10_,
                bottom_grid_top_width_0_height_0_subtile_0__pin_I_11_,
                ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [15:0] chany_top_in;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_0_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_1_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_2_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_3_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_4_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_5_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_6_;
//----- INPUT PORTS -----
input [0:0] top_left_grid_right_width_0_height_0_subtile_0__pin_O_7_;
//----- INPUT PORTS -----
input [15:0] chanx_right_in;
//----- INPUT PORTS -----
input [15:0] chany_bottom_in;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_1_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_2_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_3_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_4_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_5_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_6_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_7_;
//----- INPUT PORTS -----
input [15:0] chanx_left_in;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [15:0] chany_top_out;
//----- OUTPUT PORTS -----
output [15:0] chanx_right_out;
//----- OUTPUT PORTS -----
output [15:0] chany_bottom_out;
//----- OUTPUT PORTS -----
output [15:0] chanx_left_out;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_1_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_2_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_3_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_4_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_5_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_6_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_7_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_8_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_9_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_10_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_top_width_0_height_0_subtile_0__pin_I_11_;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [4:0] mux_2level_tapbuf_size16_0_sram;
wire [4:0] mux_2level_tapbuf_size16_0_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_10_sram;
wire [4:0] mux_2level_tapbuf_size16_10_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_11_sram;
wire [4:0] mux_2level_tapbuf_size16_11_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_1_sram;
wire [4:0] mux_2level_tapbuf_size16_1_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_2_sram;
wire [4:0] mux_2level_tapbuf_size16_2_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_3_sram;
wire [4:0] mux_2level_tapbuf_size16_3_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_4_sram;
wire [4:0] mux_2level_tapbuf_size16_4_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_5_sram;
wire [4:0] mux_2level_tapbuf_size16_5_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_6_sram;
wire [4:0] mux_2level_tapbuf_size16_6_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_7_sram;
wire [4:0] mux_2level_tapbuf_size16_7_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_8_sram;
wire [4:0] mux_2level_tapbuf_size16_8_sram_inv;
wire [4:0] mux_2level_tapbuf_size16_9_sram;
wire [4:0] mux_2level_tapbuf_size16_9_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_0_sram;
wire [5:0] mux_2level_tapbuf_size48_0_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_10_sram;
wire [5:0] mux_2level_tapbuf_size48_10_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_11_sram;
wire [5:0] mux_2level_tapbuf_size48_11_sram_inv;
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
wire [5:0] mux_2level_tapbuf_size48_8_sram;
wire [5:0] mux_2level_tapbuf_size48_8_sram_inv;
wire [5:0] mux_2level_tapbuf_size48_9_sram;
wire [5:0] mux_2level_tapbuf_size48_9_sram_inv;
wire [5:0] mux_2level_tapbuf_size56_0_sram;
wire [5:0] mux_2level_tapbuf_size56_0_sram_inv;
wire [5:0] mux_2level_tapbuf_size56_1_sram;
wire [5:0] mux_2level_tapbuf_size56_1_sram_inv;
wire [5:0] mux_2level_tapbuf_size56_2_sram;
wire [5:0] mux_2level_tapbuf_size56_2_sram_inv;
wire [5:0] mux_2level_tapbuf_size56_3_sram;
wire [5:0] mux_2level_tapbuf_size56_3_sram_inv;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 5 -----
	assign chany_bottom_out[1] = chany_top_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 5 -----
	assign chany_bottom_out[2] = chany_top_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 5 -----
	assign chany_bottom_out[3] = chany_top_in[2];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 6 -----
	assign chany_bottom_out[5] = chany_top_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 6 -----
	assign chany_bottom_out[6] = chany_top_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 6 -----
	assign chany_bottom_out[7] = chany_top_in[6];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 7 -----
	assign chany_bottom_out[9] = chany_top_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 7 -----
	assign chany_bottom_out[10] = chany_top_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 7 -----
	assign chany_bottom_out[11] = chany_top_in[10];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 8 -----
	assign chany_bottom_out[13] = chany_top_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 8 -----
	assign chany_bottom_out[14] = chany_top_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 8 -----
	assign chany_bottom_out[15] = chany_top_in[14];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 9 -----
	assign chanx_left_out[1] = chanx_right_in[0];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 9 -----
	assign chanx_left_out[2] = chanx_right_in[1];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 9 -----
	assign chanx_left_out[3] = chanx_right_in[2];
// ----- Local connection due to Wire 28 -----
// ----- Net source id 0 -----
// ----- Net sink id 10 -----
	assign chanx_left_out[5] = chanx_right_in[4];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 10 -----
	assign chanx_left_out[6] = chanx_right_in[5];
// ----- Local connection due to Wire 30 -----
// ----- Net source id 0 -----
// ----- Net sink id 10 -----
	assign chanx_left_out[7] = chanx_right_in[6];
// ----- Local connection due to Wire 32 -----
// ----- Net source id 0 -----
// ----- Net sink id 11 -----
	assign chanx_left_out[9] = chanx_right_in[8];
// ----- Local connection due to Wire 33 -----
// ----- Net source id 0 -----
// ----- Net sink id 11 -----
	assign chanx_left_out[10] = chanx_right_in[9];
// ----- Local connection due to Wire 34 -----
// ----- Net source id 0 -----
// ----- Net sink id 11 -----
	assign chanx_left_out[11] = chanx_right_in[10];
// ----- Local connection due to Wire 36 -----
// ----- Net source id 0 -----
// ----- Net sink id 12 -----
	assign chanx_left_out[13] = chanx_right_in[12];
// ----- Local connection due to Wire 37 -----
// ----- Net source id 0 -----
// ----- Net sink id 12 -----
	assign chanx_left_out[14] = chanx_right_in[13];
// ----- Local connection due to Wire 38 -----
// ----- Net source id 0 -----
// ----- Net sink id 12 -----
	assign chanx_left_out[15] = chanx_right_in[14];
// ----- Local connection due to Wire 40 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out[1] = chany_bottom_in[0];
// ----- Local connection due to Wire 41 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out[2] = chany_bottom_in[1];
// ----- Local connection due to Wire 42 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_top_out[3] = chany_bottom_in[2];
// ----- Local connection due to Wire 44 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_top_out[5] = chany_bottom_in[4];
// ----- Local connection due to Wire 45 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_top_out[6] = chany_bottom_in[5];
// ----- Local connection due to Wire 46 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_top_out[7] = chany_bottom_in[6];
// ----- Local connection due to Wire 48 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_top_out[9] = chany_bottom_in[8];
// ----- Local connection due to Wire 49 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_top_out[10] = chany_bottom_in[9];
// ----- Local connection due to Wire 50 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_top_out[11] = chany_bottom_in[10];
// ----- Local connection due to Wire 52 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_top_out[13] = chany_bottom_in[12];
// ----- Local connection due to Wire 53 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_top_out[14] = chany_bottom_in[13];
// ----- Local connection due to Wire 54 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_top_out[15] = chany_bottom_in[14];
// ----- Local connection due to Wire 64 -----
// ----- Net source id 0 -----
// ----- Net sink id 5 -----
	assign chanx_right_out[1] = chanx_left_in[0];
// ----- Local connection due to Wire 65 -----
// ----- Net source id 0 -----
// ----- Net sink id 5 -----
	assign chanx_right_out[2] = chanx_left_in[1];
// ----- Local connection due to Wire 66 -----
// ----- Net source id 0 -----
// ----- Net sink id 5 -----
	assign chanx_right_out[3] = chanx_left_in[2];
// ----- Local connection due to Wire 68 -----
// ----- Net source id 0 -----
// ----- Net sink id 6 -----
	assign chanx_right_out[5] = chanx_left_in[4];
// ----- Local connection due to Wire 69 -----
// ----- Net source id 0 -----
// ----- Net sink id 6 -----
	assign chanx_right_out[6] = chanx_left_in[5];
// ----- Local connection due to Wire 70 -----
// ----- Net source id 0 -----
// ----- Net sink id 6 -----
	assign chanx_right_out[7] = chanx_left_in[6];
// ----- Local connection due to Wire 72 -----
// ----- Net source id 0 -----
// ----- Net sink id 7 -----
	assign chanx_right_out[9] = chanx_left_in[8];
// ----- Local connection due to Wire 73 -----
// ----- Net source id 0 -----
// ----- Net sink id 7 -----
	assign chanx_right_out[10] = chanx_left_in[9];
// ----- Local connection due to Wire 74 -----
// ----- Net source id 0 -----
// ----- Net sink id 7 -----
	assign chanx_right_out[11] = chanx_left_in[10];
// ----- Local connection due to Wire 76 -----
// ----- Net source id 0 -----
// ----- Net sink id 8 -----
	assign chanx_right_out[13] = chanx_left_in[12];
// ----- Local connection due to Wire 77 -----
// ----- Net source id 0 -----
// ----- Net sink id 8 -----
	assign chanx_right_out[14] = chanx_left_in[13];
// ----- Local connection due to Wire 78 -----
// ----- Net source id 0 -----
// ----- Net sink id 8 -----
	assign chanx_right_out[15] = chanx_left_in[14];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size48 mux_top_track_0 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0]}),
		.sram({mux_2level_tapbuf_size48_0_sram[5], mux_2level_tapbuf_size48_0_sram[4], mux_2level_tapbuf_size48_0_sram[3], mux_2level_tapbuf_size48_0_sram[2], mux_2level_tapbuf_size48_0_sram[1], mux_2level_tapbuf_size48_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_0_sram_inv[5], mux_2level_tapbuf_size48_0_sram_inv[4], mux_2level_tapbuf_size48_0_sram_inv[3], mux_2level_tapbuf_size48_0_sram_inv[2], mux_2level_tapbuf_size48_0_sram_inv[1], mux_2level_tapbuf_size48_0_sram_inv[0]}),
		.out(chany_top_out[0]));

	mux_2level_tapbuf_size48 mux_top_track_8 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0]}),
		.sram({mux_2level_tapbuf_size48_1_sram[5], mux_2level_tapbuf_size48_1_sram[4], mux_2level_tapbuf_size48_1_sram[3], mux_2level_tapbuf_size48_1_sram[2], mux_2level_tapbuf_size48_1_sram[1], mux_2level_tapbuf_size48_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_1_sram_inv[5], mux_2level_tapbuf_size48_1_sram_inv[4], mux_2level_tapbuf_size48_1_sram_inv[3], mux_2level_tapbuf_size48_1_sram_inv[2], mux_2level_tapbuf_size48_1_sram_inv[1], mux_2level_tapbuf_size48_1_sram_inv[0]}),
		.out(chany_top_out[4]));

	mux_2level_tapbuf_size48 mux_top_track_16 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0]}),
		.sram({mux_2level_tapbuf_size48_2_sram[5], mux_2level_tapbuf_size48_2_sram[4], mux_2level_tapbuf_size48_2_sram[3], mux_2level_tapbuf_size48_2_sram[2], mux_2level_tapbuf_size48_2_sram[1], mux_2level_tapbuf_size48_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_2_sram_inv[5], mux_2level_tapbuf_size48_2_sram_inv[4], mux_2level_tapbuf_size48_2_sram_inv[3], mux_2level_tapbuf_size48_2_sram_inv[2], mux_2level_tapbuf_size48_2_sram_inv[1], mux_2level_tapbuf_size48_2_sram_inv[0]}),
		.out(chany_top_out[8]));

	mux_2level_tapbuf_size48 mux_top_track_24 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0]}),
		.sram({mux_2level_tapbuf_size48_3_sram[5], mux_2level_tapbuf_size48_3_sram[4], mux_2level_tapbuf_size48_3_sram[3], mux_2level_tapbuf_size48_3_sram[2], mux_2level_tapbuf_size48_3_sram[1], mux_2level_tapbuf_size48_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_3_sram_inv[5], mux_2level_tapbuf_size48_3_sram_inv[4], mux_2level_tapbuf_size48_3_sram_inv[3], mux_2level_tapbuf_size48_3_sram_inv[2], mux_2level_tapbuf_size48_3_sram_inv[1], mux_2level_tapbuf_size48_3_sram_inv[0]}),
		.out(chany_top_out[12]));

	mux_2level_tapbuf_size48 mux_right_track_0 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_4_sram[5], mux_2level_tapbuf_size48_4_sram[4], mux_2level_tapbuf_size48_4_sram[3], mux_2level_tapbuf_size48_4_sram[2], mux_2level_tapbuf_size48_4_sram[1], mux_2level_tapbuf_size48_4_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_4_sram_inv[5], mux_2level_tapbuf_size48_4_sram_inv[4], mux_2level_tapbuf_size48_4_sram_inv[3], mux_2level_tapbuf_size48_4_sram_inv[2], mux_2level_tapbuf_size48_4_sram_inv[1], mux_2level_tapbuf_size48_4_sram_inv[0]}),
		.out(chanx_right_out[0]));

	mux_2level_tapbuf_size48 mux_right_track_8 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_5_sram[5], mux_2level_tapbuf_size48_5_sram[4], mux_2level_tapbuf_size48_5_sram[3], mux_2level_tapbuf_size48_5_sram[2], mux_2level_tapbuf_size48_5_sram[1], mux_2level_tapbuf_size48_5_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_5_sram_inv[5], mux_2level_tapbuf_size48_5_sram_inv[4], mux_2level_tapbuf_size48_5_sram_inv[3], mux_2level_tapbuf_size48_5_sram_inv[2], mux_2level_tapbuf_size48_5_sram_inv[1], mux_2level_tapbuf_size48_5_sram_inv[0]}),
		.out(chanx_right_out[4]));

	mux_2level_tapbuf_size48 mux_right_track_16 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_6_sram[5], mux_2level_tapbuf_size48_6_sram[4], mux_2level_tapbuf_size48_6_sram[3], mux_2level_tapbuf_size48_6_sram[2], mux_2level_tapbuf_size48_6_sram[1], mux_2level_tapbuf_size48_6_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_6_sram_inv[5], mux_2level_tapbuf_size48_6_sram_inv[4], mux_2level_tapbuf_size48_6_sram_inv[3], mux_2level_tapbuf_size48_6_sram_inv[2], mux_2level_tapbuf_size48_6_sram_inv[1], mux_2level_tapbuf_size48_6_sram_inv[0]}),
		.out(chanx_right_out[8]));

	mux_2level_tapbuf_size48 mux_right_track_24 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_7_sram[5], mux_2level_tapbuf_size48_7_sram[4], mux_2level_tapbuf_size48_7_sram[3], mux_2level_tapbuf_size48_7_sram[2], mux_2level_tapbuf_size48_7_sram[1], mux_2level_tapbuf_size48_7_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_7_sram_inv[5], mux_2level_tapbuf_size48_7_sram_inv[4], mux_2level_tapbuf_size48_7_sram_inv[3], mux_2level_tapbuf_size48_7_sram_inv[2], mux_2level_tapbuf_size48_7_sram_inv[1], mux_2level_tapbuf_size48_7_sram_inv[0]}),
		.out(chanx_right_out[12]));

	mux_2level_tapbuf_size48 mux_left_track_1 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_8_sram[5], mux_2level_tapbuf_size48_8_sram[4], mux_2level_tapbuf_size48_8_sram[3], mux_2level_tapbuf_size48_8_sram[2], mux_2level_tapbuf_size48_8_sram[1], mux_2level_tapbuf_size48_8_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_8_sram_inv[5], mux_2level_tapbuf_size48_8_sram_inv[4], mux_2level_tapbuf_size48_8_sram_inv[3], mux_2level_tapbuf_size48_8_sram_inv[2], mux_2level_tapbuf_size48_8_sram_inv[1], mux_2level_tapbuf_size48_8_sram_inv[0]}),
		.out(chanx_left_out[0]));

	mux_2level_tapbuf_size48 mux_left_track_9 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_9_sram[5], mux_2level_tapbuf_size48_9_sram[4], mux_2level_tapbuf_size48_9_sram[3], mux_2level_tapbuf_size48_9_sram[2], mux_2level_tapbuf_size48_9_sram[1], mux_2level_tapbuf_size48_9_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_9_sram_inv[5], mux_2level_tapbuf_size48_9_sram_inv[4], mux_2level_tapbuf_size48_9_sram_inv[3], mux_2level_tapbuf_size48_9_sram_inv[2], mux_2level_tapbuf_size48_9_sram_inv[1], mux_2level_tapbuf_size48_9_sram_inv[0]}),
		.out(chanx_left_out[4]));

	mux_2level_tapbuf_size48 mux_left_track_17 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_10_sram[5], mux_2level_tapbuf_size48_10_sram[4], mux_2level_tapbuf_size48_10_sram[3], mux_2level_tapbuf_size48_10_sram[2], mux_2level_tapbuf_size48_10_sram[1], mux_2level_tapbuf_size48_10_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_10_sram_inv[5], mux_2level_tapbuf_size48_10_sram_inv[4], mux_2level_tapbuf_size48_10_sram_inv[3], mux_2level_tapbuf_size48_10_sram_inv[2], mux_2level_tapbuf_size48_10_sram_inv[1], mux_2level_tapbuf_size48_10_sram_inv[0]}),
		.out(chanx_left_out[8]));

	mux_2level_tapbuf_size48 mux_left_track_25 (
		.in({chany_bottom_in[15], chany_bottom_in[14], chany_bottom_in[13], chany_bottom_in[12], chany_bottom_in[11], chany_bottom_in[10], chany_bottom_in[9], chany_bottom_in[8], chany_bottom_in[7], chany_bottom_in[6], chany_bottom_in[5], chany_bottom_in[4], chany_bottom_in[3], chany_bottom_in[2], chany_bottom_in[1], chany_bottom_in[0], chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size48_11_sram[5], mux_2level_tapbuf_size48_11_sram[4], mux_2level_tapbuf_size48_11_sram[3], mux_2level_tapbuf_size48_11_sram[2], mux_2level_tapbuf_size48_11_sram[1], mux_2level_tapbuf_size48_11_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size48_11_sram_inv[5], mux_2level_tapbuf_size48_11_sram_inv[4], mux_2level_tapbuf_size48_11_sram_inv[3], mux_2level_tapbuf_size48_11_sram_inv[2], mux_2level_tapbuf_size48_11_sram_inv[1], mux_2level_tapbuf_size48_11_sram_inv[0]}),
		.out(chanx_left_out[12]));

	mux_2level_tapbuf_size48_mem mem_top_track_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_0_sram[5], mux_2level_tapbuf_size48_0_sram[4], mux_2level_tapbuf_size48_0_sram[3], mux_2level_tapbuf_size48_0_sram[2], mux_2level_tapbuf_size48_0_sram[1], mux_2level_tapbuf_size48_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_0_sram_inv[5], mux_2level_tapbuf_size48_0_sram_inv[4], mux_2level_tapbuf_size48_0_sram_inv[3], mux_2level_tapbuf_size48_0_sram_inv[2], mux_2level_tapbuf_size48_0_sram_inv[1], mux_2level_tapbuf_size48_0_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_track_8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_1_sram[5], mux_2level_tapbuf_size48_1_sram[4], mux_2level_tapbuf_size48_1_sram[3], mux_2level_tapbuf_size48_1_sram[2], mux_2level_tapbuf_size48_1_sram[1], mux_2level_tapbuf_size48_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_1_sram_inv[5], mux_2level_tapbuf_size48_1_sram_inv[4], mux_2level_tapbuf_size48_1_sram_inv[3], mux_2level_tapbuf_size48_1_sram_inv[2], mux_2level_tapbuf_size48_1_sram_inv[1], mux_2level_tapbuf_size48_1_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_track_16 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_2_sram[5], mux_2level_tapbuf_size48_2_sram[4], mux_2level_tapbuf_size48_2_sram[3], mux_2level_tapbuf_size48_2_sram[2], mux_2level_tapbuf_size48_2_sram[1], mux_2level_tapbuf_size48_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_2_sram_inv[5], mux_2level_tapbuf_size48_2_sram_inv[4], mux_2level_tapbuf_size48_2_sram_inv[3], mux_2level_tapbuf_size48_2_sram_inv[2], mux_2level_tapbuf_size48_2_sram_inv[1], mux_2level_tapbuf_size48_2_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_top_track_24 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_3_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_3_sram[5], mux_2level_tapbuf_size48_3_sram[4], mux_2level_tapbuf_size48_3_sram[3], mux_2level_tapbuf_size48_3_sram[2], mux_2level_tapbuf_size48_3_sram[1], mux_2level_tapbuf_size48_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_3_sram_inv[5], mux_2level_tapbuf_size48_3_sram_inv[4], mux_2level_tapbuf_size48_3_sram_inv[3], mux_2level_tapbuf_size48_3_sram_inv[2], mux_2level_tapbuf_size48_3_sram_inv[1], mux_2level_tapbuf_size48_3_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_right_track_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_4_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_4_sram[5], mux_2level_tapbuf_size48_4_sram[4], mux_2level_tapbuf_size48_4_sram[3], mux_2level_tapbuf_size48_4_sram[2], mux_2level_tapbuf_size48_4_sram[1], mux_2level_tapbuf_size48_4_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_4_sram_inv[5], mux_2level_tapbuf_size48_4_sram_inv[4], mux_2level_tapbuf_size48_4_sram_inv[3], mux_2level_tapbuf_size48_4_sram_inv[2], mux_2level_tapbuf_size48_4_sram_inv[1], mux_2level_tapbuf_size48_4_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_right_track_8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_5_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_5_sram[5], mux_2level_tapbuf_size48_5_sram[4], mux_2level_tapbuf_size48_5_sram[3], mux_2level_tapbuf_size48_5_sram[2], mux_2level_tapbuf_size48_5_sram[1], mux_2level_tapbuf_size48_5_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_5_sram_inv[5], mux_2level_tapbuf_size48_5_sram_inv[4], mux_2level_tapbuf_size48_5_sram_inv[3], mux_2level_tapbuf_size48_5_sram_inv[2], mux_2level_tapbuf_size48_5_sram_inv[1], mux_2level_tapbuf_size48_5_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_right_track_16 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_6_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_6_sram[5], mux_2level_tapbuf_size48_6_sram[4], mux_2level_tapbuf_size48_6_sram[3], mux_2level_tapbuf_size48_6_sram[2], mux_2level_tapbuf_size48_6_sram[1], mux_2level_tapbuf_size48_6_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_6_sram_inv[5], mux_2level_tapbuf_size48_6_sram_inv[4], mux_2level_tapbuf_size48_6_sram_inv[3], mux_2level_tapbuf_size48_6_sram_inv[2], mux_2level_tapbuf_size48_6_sram_inv[1], mux_2level_tapbuf_size48_6_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_right_track_24 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_6_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_7_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_7_sram[5], mux_2level_tapbuf_size48_7_sram[4], mux_2level_tapbuf_size48_7_sram[3], mux_2level_tapbuf_size48_7_sram[2], mux_2level_tapbuf_size48_7_sram[1], mux_2level_tapbuf_size48_7_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_7_sram_inv[5], mux_2level_tapbuf_size48_7_sram_inv[4], mux_2level_tapbuf_size48_7_sram_inv[3], mux_2level_tapbuf_size48_7_sram_inv[2], mux_2level_tapbuf_size48_7_sram_inv[1], mux_2level_tapbuf_size48_7_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_8_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_8_sram[5], mux_2level_tapbuf_size48_8_sram[4], mux_2level_tapbuf_size48_8_sram[3], mux_2level_tapbuf_size48_8_sram[2], mux_2level_tapbuf_size48_8_sram[1], mux_2level_tapbuf_size48_8_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_8_sram_inv[5], mux_2level_tapbuf_size48_8_sram_inv[4], mux_2level_tapbuf_size48_8_sram_inv[3], mux_2level_tapbuf_size48_8_sram_inv[2], mux_2level_tapbuf_size48_8_sram_inv[1], mux_2level_tapbuf_size48_8_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_8_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_9_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_9_sram[5], mux_2level_tapbuf_size48_9_sram[4], mux_2level_tapbuf_size48_9_sram[3], mux_2level_tapbuf_size48_9_sram[2], mux_2level_tapbuf_size48_9_sram[1], mux_2level_tapbuf_size48_9_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_9_sram_inv[5], mux_2level_tapbuf_size48_9_sram_inv[4], mux_2level_tapbuf_size48_9_sram_inv[3], mux_2level_tapbuf_size48_9_sram_inv[2], mux_2level_tapbuf_size48_9_sram_inv[1], mux_2level_tapbuf_size48_9_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_9_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_10_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_10_sram[5], mux_2level_tapbuf_size48_10_sram[4], mux_2level_tapbuf_size48_10_sram[3], mux_2level_tapbuf_size48_10_sram[2], mux_2level_tapbuf_size48_10_sram[1], mux_2level_tapbuf_size48_10_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_10_sram_inv[5], mux_2level_tapbuf_size48_10_sram_inv[4], mux_2level_tapbuf_size48_10_sram_inv[3], mux_2level_tapbuf_size48_10_sram_inv[2], mux_2level_tapbuf_size48_10_sram_inv[1], mux_2level_tapbuf_size48_10_sram_inv[0]}));

	mux_2level_tapbuf_size48_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_10_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_11_ccff_tail),
		.mem_out({mux_2level_tapbuf_size48_11_sram[5], mux_2level_tapbuf_size48_11_sram[4], mux_2level_tapbuf_size48_11_sram[3], mux_2level_tapbuf_size48_11_sram[2], mux_2level_tapbuf_size48_11_sram[1], mux_2level_tapbuf_size48_11_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size48_11_sram_inv[5], mux_2level_tapbuf_size48_11_sram_inv[4], mux_2level_tapbuf_size48_11_sram_inv[3], mux_2level_tapbuf_size48_11_sram_inv[2], mux_2level_tapbuf_size48_11_sram_inv[1], mux_2level_tapbuf_size48_11_sram_inv[0]}));

	mux_2level_tapbuf_size56 mux_bottom_track_1 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_7_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_6_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_5_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_4_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_3_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_2_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_1_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size56_0_sram[5], mux_2level_tapbuf_size56_0_sram[4], mux_2level_tapbuf_size56_0_sram[3], mux_2level_tapbuf_size56_0_sram[2], mux_2level_tapbuf_size56_0_sram[1], mux_2level_tapbuf_size56_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size56_0_sram_inv[5], mux_2level_tapbuf_size56_0_sram_inv[4], mux_2level_tapbuf_size56_0_sram_inv[3], mux_2level_tapbuf_size56_0_sram_inv[2], mux_2level_tapbuf_size56_0_sram_inv[1], mux_2level_tapbuf_size56_0_sram_inv[0]}),
		.out(chany_bottom_out[0]));

	mux_2level_tapbuf_size56 mux_bottom_track_9 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_7_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_6_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_5_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_4_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_3_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_2_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_1_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size56_1_sram[5], mux_2level_tapbuf_size56_1_sram[4], mux_2level_tapbuf_size56_1_sram[3], mux_2level_tapbuf_size56_1_sram[2], mux_2level_tapbuf_size56_1_sram[1], mux_2level_tapbuf_size56_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size56_1_sram_inv[5], mux_2level_tapbuf_size56_1_sram_inv[4], mux_2level_tapbuf_size56_1_sram_inv[3], mux_2level_tapbuf_size56_1_sram_inv[2], mux_2level_tapbuf_size56_1_sram_inv[1], mux_2level_tapbuf_size56_1_sram_inv[0]}),
		.out(chany_bottom_out[4]));

	mux_2level_tapbuf_size56 mux_bottom_track_17 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_7_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_6_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_5_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_4_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_3_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_2_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_1_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size56_2_sram[5], mux_2level_tapbuf_size56_2_sram[4], mux_2level_tapbuf_size56_2_sram[3], mux_2level_tapbuf_size56_2_sram[2], mux_2level_tapbuf_size56_2_sram[1], mux_2level_tapbuf_size56_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size56_2_sram_inv[5], mux_2level_tapbuf_size56_2_sram_inv[4], mux_2level_tapbuf_size56_2_sram_inv[3], mux_2level_tapbuf_size56_2_sram_inv[2], mux_2level_tapbuf_size56_2_sram_inv[1], mux_2level_tapbuf_size56_2_sram_inv[0]}),
		.out(chany_bottom_out[8]));

	mux_2level_tapbuf_size56 mux_bottom_track_25 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0], bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_7_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_6_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_5_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_4_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_3_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_2_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_1_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_O_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size56_3_sram[5], mux_2level_tapbuf_size56_3_sram[4], mux_2level_tapbuf_size56_3_sram[3], mux_2level_tapbuf_size56_3_sram[2], mux_2level_tapbuf_size56_3_sram[1], mux_2level_tapbuf_size56_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size56_3_sram_inv[5], mux_2level_tapbuf_size56_3_sram_inv[4], mux_2level_tapbuf_size56_3_sram_inv[3], mux_2level_tapbuf_size56_3_sram_inv[2], mux_2level_tapbuf_size56_3_sram_inv[1], mux_2level_tapbuf_size56_3_sram_inv[0]}),
		.out(chany_bottom_out[12]));

	mux_2level_tapbuf_size56_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_7_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_0_ccff_tail),
		.mem_out({mux_2level_tapbuf_size56_0_sram[5], mux_2level_tapbuf_size56_0_sram[4], mux_2level_tapbuf_size56_0_sram[3], mux_2level_tapbuf_size56_0_sram[2], mux_2level_tapbuf_size56_0_sram[1], mux_2level_tapbuf_size56_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size56_0_sram_inv[5], mux_2level_tapbuf_size56_0_sram_inv[4], mux_2level_tapbuf_size56_0_sram_inv[3], mux_2level_tapbuf_size56_0_sram_inv[2], mux_2level_tapbuf_size56_0_sram_inv[1], mux_2level_tapbuf_size56_0_sram_inv[0]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_1_ccff_tail),
		.mem_out({mux_2level_tapbuf_size56_1_sram[5], mux_2level_tapbuf_size56_1_sram[4], mux_2level_tapbuf_size56_1_sram[3], mux_2level_tapbuf_size56_1_sram[2], mux_2level_tapbuf_size56_1_sram[1], mux_2level_tapbuf_size56_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size56_1_sram_inv[5], mux_2level_tapbuf_size56_1_sram_inv[4], mux_2level_tapbuf_size56_1_sram_inv[3], mux_2level_tapbuf_size56_1_sram_inv[2], mux_2level_tapbuf_size56_1_sram_inv[1], mux_2level_tapbuf_size56_1_sram_inv[0]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_2_ccff_tail),
		.mem_out({mux_2level_tapbuf_size56_2_sram[5], mux_2level_tapbuf_size56_2_sram[4], mux_2level_tapbuf_size56_2_sram[3], mux_2level_tapbuf_size56_2_sram[2], mux_2level_tapbuf_size56_2_sram[1], mux_2level_tapbuf_size56_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size56_2_sram_inv[5], mux_2level_tapbuf_size56_2_sram_inv[4], mux_2level_tapbuf_size56_2_sram_inv[3], mux_2level_tapbuf_size56_2_sram_inv[2], mux_2level_tapbuf_size56_2_sram_inv[1], mux_2level_tapbuf_size56_2_sram_inv[0]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_3_ccff_tail),
		.mem_out({mux_2level_tapbuf_size56_3_sram[5], mux_2level_tapbuf_size56_3_sram[4], mux_2level_tapbuf_size56_3_sram[3], mux_2level_tapbuf_size56_3_sram[2], mux_2level_tapbuf_size56_3_sram[1], mux_2level_tapbuf_size56_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size56_3_sram_inv[5], mux_2level_tapbuf_size56_3_sram_inv[4], mux_2level_tapbuf_size56_3_sram_inv[3], mux_2level_tapbuf_size56_3_sram_inv[2], mux_2level_tapbuf_size56_3_sram_inv[1], mux_2level_tapbuf_size56_3_sram_inv[0]}));

	mux_2level_tapbuf_size16 mux_top_ipin_0 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_0_sram[4], mux_2level_tapbuf_size16_0_sram[3], mux_2level_tapbuf_size16_0_sram[2], mux_2level_tapbuf_size16_0_sram[1], mux_2level_tapbuf_size16_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_0_sram_inv[4], mux_2level_tapbuf_size16_0_sram_inv[3], mux_2level_tapbuf_size16_0_sram_inv[2], mux_2level_tapbuf_size16_0_sram_inv[1], mux_2level_tapbuf_size16_0_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_0_));

	mux_2level_tapbuf_size16 mux_top_ipin_1 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_1_sram[4], mux_2level_tapbuf_size16_1_sram[3], mux_2level_tapbuf_size16_1_sram[2], mux_2level_tapbuf_size16_1_sram[1], mux_2level_tapbuf_size16_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_1_sram_inv[4], mux_2level_tapbuf_size16_1_sram_inv[3], mux_2level_tapbuf_size16_1_sram_inv[2], mux_2level_tapbuf_size16_1_sram_inv[1], mux_2level_tapbuf_size16_1_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_1_));

	mux_2level_tapbuf_size16 mux_top_ipin_2 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_2_sram[4], mux_2level_tapbuf_size16_2_sram[3], mux_2level_tapbuf_size16_2_sram[2], mux_2level_tapbuf_size16_2_sram[1], mux_2level_tapbuf_size16_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_2_sram_inv[4], mux_2level_tapbuf_size16_2_sram_inv[3], mux_2level_tapbuf_size16_2_sram_inv[2], mux_2level_tapbuf_size16_2_sram_inv[1], mux_2level_tapbuf_size16_2_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_2_));

	mux_2level_tapbuf_size16 mux_top_ipin_3 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_3_sram[4], mux_2level_tapbuf_size16_3_sram[3], mux_2level_tapbuf_size16_3_sram[2], mux_2level_tapbuf_size16_3_sram[1], mux_2level_tapbuf_size16_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_3_sram_inv[4], mux_2level_tapbuf_size16_3_sram_inv[3], mux_2level_tapbuf_size16_3_sram_inv[2], mux_2level_tapbuf_size16_3_sram_inv[1], mux_2level_tapbuf_size16_3_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_3_));

	mux_2level_tapbuf_size16 mux_top_ipin_4 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_4_sram[4], mux_2level_tapbuf_size16_4_sram[3], mux_2level_tapbuf_size16_4_sram[2], mux_2level_tapbuf_size16_4_sram[1], mux_2level_tapbuf_size16_4_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_4_sram_inv[4], mux_2level_tapbuf_size16_4_sram_inv[3], mux_2level_tapbuf_size16_4_sram_inv[2], mux_2level_tapbuf_size16_4_sram_inv[1], mux_2level_tapbuf_size16_4_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_4_));

	mux_2level_tapbuf_size16 mux_top_ipin_5 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_5_sram[4], mux_2level_tapbuf_size16_5_sram[3], mux_2level_tapbuf_size16_5_sram[2], mux_2level_tapbuf_size16_5_sram[1], mux_2level_tapbuf_size16_5_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_5_sram_inv[4], mux_2level_tapbuf_size16_5_sram_inv[3], mux_2level_tapbuf_size16_5_sram_inv[2], mux_2level_tapbuf_size16_5_sram_inv[1], mux_2level_tapbuf_size16_5_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_5_));

	mux_2level_tapbuf_size16 mux_top_ipin_6 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_6_sram[4], mux_2level_tapbuf_size16_6_sram[3], mux_2level_tapbuf_size16_6_sram[2], mux_2level_tapbuf_size16_6_sram[1], mux_2level_tapbuf_size16_6_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_6_sram_inv[4], mux_2level_tapbuf_size16_6_sram_inv[3], mux_2level_tapbuf_size16_6_sram_inv[2], mux_2level_tapbuf_size16_6_sram_inv[1], mux_2level_tapbuf_size16_6_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_6_));

	mux_2level_tapbuf_size16 mux_top_ipin_7 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_7_sram[4], mux_2level_tapbuf_size16_7_sram[3], mux_2level_tapbuf_size16_7_sram[2], mux_2level_tapbuf_size16_7_sram[1], mux_2level_tapbuf_size16_7_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_7_sram_inv[4], mux_2level_tapbuf_size16_7_sram_inv[3], mux_2level_tapbuf_size16_7_sram_inv[2], mux_2level_tapbuf_size16_7_sram_inv[1], mux_2level_tapbuf_size16_7_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_7_));

	mux_2level_tapbuf_size16 mux_top_ipin_8 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_8_sram[4], mux_2level_tapbuf_size16_8_sram[3], mux_2level_tapbuf_size16_8_sram[2], mux_2level_tapbuf_size16_8_sram[1], mux_2level_tapbuf_size16_8_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_8_sram_inv[4], mux_2level_tapbuf_size16_8_sram_inv[3], mux_2level_tapbuf_size16_8_sram_inv[2], mux_2level_tapbuf_size16_8_sram_inv[1], mux_2level_tapbuf_size16_8_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_8_));

	mux_2level_tapbuf_size16 mux_top_ipin_9 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_9_sram[4], mux_2level_tapbuf_size16_9_sram[3], mux_2level_tapbuf_size16_9_sram[2], mux_2level_tapbuf_size16_9_sram[1], mux_2level_tapbuf_size16_9_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_9_sram_inv[4], mux_2level_tapbuf_size16_9_sram_inv[3], mux_2level_tapbuf_size16_9_sram_inv[2], mux_2level_tapbuf_size16_9_sram_inv[1], mux_2level_tapbuf_size16_9_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_9_));

	mux_2level_tapbuf_size16 mux_top_ipin_10 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_10_sram[4], mux_2level_tapbuf_size16_10_sram[3], mux_2level_tapbuf_size16_10_sram[2], mux_2level_tapbuf_size16_10_sram[1], mux_2level_tapbuf_size16_10_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_10_sram_inv[4], mux_2level_tapbuf_size16_10_sram_inv[3], mux_2level_tapbuf_size16_10_sram_inv[2], mux_2level_tapbuf_size16_10_sram_inv[1], mux_2level_tapbuf_size16_10_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_10_));

	mux_2level_tapbuf_size16 mux_top_ipin_11 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_11_sram[4], mux_2level_tapbuf_size16_11_sram[3], mux_2level_tapbuf_size16_11_sram[2], mux_2level_tapbuf_size16_11_sram[1], mux_2level_tapbuf_size16_11_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_11_sram_inv[4], mux_2level_tapbuf_size16_11_sram_inv[3], mux_2level_tapbuf_size16_11_sram_inv[2], mux_2level_tapbuf_size16_11_sram_inv[1], mux_2level_tapbuf_size16_11_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_11_));

	mux_2level_tapbuf_size16_mem mem_top_ipin_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_11_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_0_sram[4], mux_2level_tapbuf_size16_0_sram[3], mux_2level_tapbuf_size16_0_sram[2], mux_2level_tapbuf_size16_0_sram[1], mux_2level_tapbuf_size16_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_0_sram_inv[4], mux_2level_tapbuf_size16_0_sram_inv[3], mux_2level_tapbuf_size16_0_sram_inv[2], mux_2level_tapbuf_size16_0_sram_inv[1], mux_2level_tapbuf_size16_0_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_1_sram[4], mux_2level_tapbuf_size16_1_sram[3], mux_2level_tapbuf_size16_1_sram[2], mux_2level_tapbuf_size16_1_sram[1], mux_2level_tapbuf_size16_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_1_sram_inv[4], mux_2level_tapbuf_size16_1_sram_inv[3], mux_2level_tapbuf_size16_1_sram_inv[2], mux_2level_tapbuf_size16_1_sram_inv[1], mux_2level_tapbuf_size16_1_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_2_sram[4], mux_2level_tapbuf_size16_2_sram[3], mux_2level_tapbuf_size16_2_sram[2], mux_2level_tapbuf_size16_2_sram[1], mux_2level_tapbuf_size16_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_2_sram_inv[4], mux_2level_tapbuf_size16_2_sram_inv[3], mux_2level_tapbuf_size16_2_sram_inv[2], mux_2level_tapbuf_size16_2_sram_inv[1], mux_2level_tapbuf_size16_2_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_3_sram[4], mux_2level_tapbuf_size16_3_sram[3], mux_2level_tapbuf_size16_3_sram[2], mux_2level_tapbuf_size16_3_sram[1], mux_2level_tapbuf_size16_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_3_sram_inv[4], mux_2level_tapbuf_size16_3_sram_inv[3], mux_2level_tapbuf_size16_3_sram_inv[2], mux_2level_tapbuf_size16_3_sram_inv[1], mux_2level_tapbuf_size16_3_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_4_sram[4], mux_2level_tapbuf_size16_4_sram[3], mux_2level_tapbuf_size16_4_sram[2], mux_2level_tapbuf_size16_4_sram[1], mux_2level_tapbuf_size16_4_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_4_sram_inv[4], mux_2level_tapbuf_size16_4_sram_inv[3], mux_2level_tapbuf_size16_4_sram_inv[2], mux_2level_tapbuf_size16_4_sram_inv[1], mux_2level_tapbuf_size16_4_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_5_sram[4], mux_2level_tapbuf_size16_5_sram[3], mux_2level_tapbuf_size16_5_sram[2], mux_2level_tapbuf_size16_5_sram[1], mux_2level_tapbuf_size16_5_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_5_sram_inv[4], mux_2level_tapbuf_size16_5_sram_inv[3], mux_2level_tapbuf_size16_5_sram_inv[2], mux_2level_tapbuf_size16_5_sram_inv[1], mux_2level_tapbuf_size16_5_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_6 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_6_sram[4], mux_2level_tapbuf_size16_6_sram[3], mux_2level_tapbuf_size16_6_sram[2], mux_2level_tapbuf_size16_6_sram[1], mux_2level_tapbuf_size16_6_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_6_sram_inv[4], mux_2level_tapbuf_size16_6_sram_inv[3], mux_2level_tapbuf_size16_6_sram_inv[2], mux_2level_tapbuf_size16_6_sram_inv[1], mux_2level_tapbuf_size16_6_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_7_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_7_sram[4], mux_2level_tapbuf_size16_7_sram[3], mux_2level_tapbuf_size16_7_sram[2], mux_2level_tapbuf_size16_7_sram[1], mux_2level_tapbuf_size16_7_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_7_sram_inv[4], mux_2level_tapbuf_size16_7_sram_inv[3], mux_2level_tapbuf_size16_7_sram_inv[2], mux_2level_tapbuf_size16_7_sram_inv[1], mux_2level_tapbuf_size16_7_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_7_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_8_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_8_sram[4], mux_2level_tapbuf_size16_8_sram[3], mux_2level_tapbuf_size16_8_sram[2], mux_2level_tapbuf_size16_8_sram[1], mux_2level_tapbuf_size16_8_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_8_sram_inv[4], mux_2level_tapbuf_size16_8_sram_inv[3], mux_2level_tapbuf_size16_8_sram_inv[2], mux_2level_tapbuf_size16_8_sram_inv[1], mux_2level_tapbuf_size16_8_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_8_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_9_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_9_sram[4], mux_2level_tapbuf_size16_9_sram[3], mux_2level_tapbuf_size16_9_sram[2], mux_2level_tapbuf_size16_9_sram[1], mux_2level_tapbuf_size16_9_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_9_sram_inv[4], mux_2level_tapbuf_size16_9_sram_inv[3], mux_2level_tapbuf_size16_9_sram_inv[2], mux_2level_tapbuf_size16_9_sram_inv[1], mux_2level_tapbuf_size16_9_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_10 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_9_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_10_ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_10_sram[4], mux_2level_tapbuf_size16_10_sram[3], mux_2level_tapbuf_size16_10_sram[2], mux_2level_tapbuf_size16_10_sram[1], mux_2level_tapbuf_size16_10_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_10_sram_inv[4], mux_2level_tapbuf_size16_10_sram_inv[3], mux_2level_tapbuf_size16_10_sram_inv[2], mux_2level_tapbuf_size16_10_sram_inv[1], mux_2level_tapbuf_size16_10_sram_inv[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_11 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_10_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mux_2level_tapbuf_size16_11_sram[4], mux_2level_tapbuf_size16_11_sram[3], mux_2level_tapbuf_size16_11_sram[2], mux_2level_tapbuf_size16_11_sram[1], mux_2level_tapbuf_size16_11_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_11_sram_inv[4], mux_2level_tapbuf_size16_11_sram_inv[3], mux_2level_tapbuf_size16_11_sram_inv[2], mux_2level_tapbuf_size16_11_sram_inv[1], mux_2level_tapbuf_size16_11_sram_inv[0]}));

endmodule
// ----- END Verilog module for sb_2__2_ -----



