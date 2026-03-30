//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Connection Blocks[2][2]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for cbx_2__2_ -----
module cbx_2__2_(pReset,
                 prog_clk,
                 chanx_left_in,
                 chanx_right_in,
                 ccff_head,
                 chanx_left_out,
                 chanx_right_out,
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
input [15:0] chanx_left_in;
//----- INPUT PORTS -----
input [15:0] chanx_right_in;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [15:0] chanx_left_out;
//----- OUTPUT PORTS -----
output [15:0] chanx_right_out;
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

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [119:0] cbx_2__config_group_mem_size120_0_mem_out;
wire [119:0] cbx_2__config_group_mem_size120_0_mem_outb;
wire [9:0] mux_2level_tapbuf_size16_0_sram;
wire [9:0] mux_2level_tapbuf_size16_0_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_10_sram;
wire [9:0] mux_2level_tapbuf_size16_10_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_11_sram;
wire [9:0] mux_2level_tapbuf_size16_11_sram_inv;
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
wire [9:0] mux_2level_tapbuf_size16_8_sram;
wire [9:0] mux_2level_tapbuf_size16_8_sram_inv;
wire [9:0] mux_2level_tapbuf_size16_9_sram;
wire [9:0] mux_2level_tapbuf_size16_9_sram_inv;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[0] = chanx_left_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[1] = chanx_left_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[2] = chanx_left_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[3] = chanx_left_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[4] = chanx_left_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[5] = chanx_left_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[6] = chanx_left_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[7] = chanx_left_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[8] = chanx_left_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[9] = chanx_left_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[10] = chanx_left_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[11] = chanx_left_in[11];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[12] = chanx_left_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[13] = chanx_left_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[14] = chanx_left_in[14];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[15] = chanx_left_in[15];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[0] = chanx_right_in[0];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[1] = chanx_right_in[1];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[2] = chanx_right_in[2];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[3] = chanx_right_in[3];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[4] = chanx_right_in[4];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[5] = chanx_right_in[5];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[6] = chanx_right_in[6];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[7] = chanx_right_in[7];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[8] = chanx_right_in[8];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[9] = chanx_right_in[9];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[10] = chanx_right_in[10];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[11] = chanx_right_in[11];
// ----- Local connection due to Wire 28 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[12] = chanx_right_in[12];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[13] = chanx_right_in[13];
// ----- Local connection due to Wire 30 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[14] = chanx_right_in[14];
// ----- Local connection due to Wire 31 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_left_out[15] = chanx_right_in[15];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size16 mux_top_ipin_0 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_0_sram[9], mux_2level_tapbuf_size16_0_sram[8], mux_2level_tapbuf_size16_0_sram[7], mux_2level_tapbuf_size16_0_sram[6], mux_2level_tapbuf_size16_0_sram[5], mux_2level_tapbuf_size16_0_sram[4], mux_2level_tapbuf_size16_0_sram[3], mux_2level_tapbuf_size16_0_sram[2], mux_2level_tapbuf_size16_0_sram[1], mux_2level_tapbuf_size16_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_0_sram_inv[9], mux_2level_tapbuf_size16_0_sram_inv[8], mux_2level_tapbuf_size16_0_sram_inv[7], mux_2level_tapbuf_size16_0_sram_inv[6], mux_2level_tapbuf_size16_0_sram_inv[5], mux_2level_tapbuf_size16_0_sram_inv[4], mux_2level_tapbuf_size16_0_sram_inv[3], mux_2level_tapbuf_size16_0_sram_inv[2], mux_2level_tapbuf_size16_0_sram_inv[1], mux_2level_tapbuf_size16_0_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_0_));

	mux_2level_tapbuf_size16 mux_top_ipin_1 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_1_sram[9], mux_2level_tapbuf_size16_1_sram[8], mux_2level_tapbuf_size16_1_sram[7], mux_2level_tapbuf_size16_1_sram[6], mux_2level_tapbuf_size16_1_sram[5], mux_2level_tapbuf_size16_1_sram[4], mux_2level_tapbuf_size16_1_sram[3], mux_2level_tapbuf_size16_1_sram[2], mux_2level_tapbuf_size16_1_sram[1], mux_2level_tapbuf_size16_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_1_sram_inv[9], mux_2level_tapbuf_size16_1_sram_inv[8], mux_2level_tapbuf_size16_1_sram_inv[7], mux_2level_tapbuf_size16_1_sram_inv[6], mux_2level_tapbuf_size16_1_sram_inv[5], mux_2level_tapbuf_size16_1_sram_inv[4], mux_2level_tapbuf_size16_1_sram_inv[3], mux_2level_tapbuf_size16_1_sram_inv[2], mux_2level_tapbuf_size16_1_sram_inv[1], mux_2level_tapbuf_size16_1_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_1_));

	mux_2level_tapbuf_size16 mux_top_ipin_2 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_2_sram[9], mux_2level_tapbuf_size16_2_sram[8], mux_2level_tapbuf_size16_2_sram[7], mux_2level_tapbuf_size16_2_sram[6], mux_2level_tapbuf_size16_2_sram[5], mux_2level_tapbuf_size16_2_sram[4], mux_2level_tapbuf_size16_2_sram[3], mux_2level_tapbuf_size16_2_sram[2], mux_2level_tapbuf_size16_2_sram[1], mux_2level_tapbuf_size16_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_2_sram_inv[9], mux_2level_tapbuf_size16_2_sram_inv[8], mux_2level_tapbuf_size16_2_sram_inv[7], mux_2level_tapbuf_size16_2_sram_inv[6], mux_2level_tapbuf_size16_2_sram_inv[5], mux_2level_tapbuf_size16_2_sram_inv[4], mux_2level_tapbuf_size16_2_sram_inv[3], mux_2level_tapbuf_size16_2_sram_inv[2], mux_2level_tapbuf_size16_2_sram_inv[1], mux_2level_tapbuf_size16_2_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_2_));

	mux_2level_tapbuf_size16 mux_top_ipin_3 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_3_sram[9], mux_2level_tapbuf_size16_3_sram[8], mux_2level_tapbuf_size16_3_sram[7], mux_2level_tapbuf_size16_3_sram[6], mux_2level_tapbuf_size16_3_sram[5], mux_2level_tapbuf_size16_3_sram[4], mux_2level_tapbuf_size16_3_sram[3], mux_2level_tapbuf_size16_3_sram[2], mux_2level_tapbuf_size16_3_sram[1], mux_2level_tapbuf_size16_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_3_sram_inv[9], mux_2level_tapbuf_size16_3_sram_inv[8], mux_2level_tapbuf_size16_3_sram_inv[7], mux_2level_tapbuf_size16_3_sram_inv[6], mux_2level_tapbuf_size16_3_sram_inv[5], mux_2level_tapbuf_size16_3_sram_inv[4], mux_2level_tapbuf_size16_3_sram_inv[3], mux_2level_tapbuf_size16_3_sram_inv[2], mux_2level_tapbuf_size16_3_sram_inv[1], mux_2level_tapbuf_size16_3_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_3_));

	mux_2level_tapbuf_size16 mux_top_ipin_4 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_4_sram[9], mux_2level_tapbuf_size16_4_sram[8], mux_2level_tapbuf_size16_4_sram[7], mux_2level_tapbuf_size16_4_sram[6], mux_2level_tapbuf_size16_4_sram[5], mux_2level_tapbuf_size16_4_sram[4], mux_2level_tapbuf_size16_4_sram[3], mux_2level_tapbuf_size16_4_sram[2], mux_2level_tapbuf_size16_4_sram[1], mux_2level_tapbuf_size16_4_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_4_sram_inv[9], mux_2level_tapbuf_size16_4_sram_inv[8], mux_2level_tapbuf_size16_4_sram_inv[7], mux_2level_tapbuf_size16_4_sram_inv[6], mux_2level_tapbuf_size16_4_sram_inv[5], mux_2level_tapbuf_size16_4_sram_inv[4], mux_2level_tapbuf_size16_4_sram_inv[3], mux_2level_tapbuf_size16_4_sram_inv[2], mux_2level_tapbuf_size16_4_sram_inv[1], mux_2level_tapbuf_size16_4_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_4_));

	mux_2level_tapbuf_size16 mux_top_ipin_5 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_5_sram[9], mux_2level_tapbuf_size16_5_sram[8], mux_2level_tapbuf_size16_5_sram[7], mux_2level_tapbuf_size16_5_sram[6], mux_2level_tapbuf_size16_5_sram[5], mux_2level_tapbuf_size16_5_sram[4], mux_2level_tapbuf_size16_5_sram[3], mux_2level_tapbuf_size16_5_sram[2], mux_2level_tapbuf_size16_5_sram[1], mux_2level_tapbuf_size16_5_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_5_sram_inv[9], mux_2level_tapbuf_size16_5_sram_inv[8], mux_2level_tapbuf_size16_5_sram_inv[7], mux_2level_tapbuf_size16_5_sram_inv[6], mux_2level_tapbuf_size16_5_sram_inv[5], mux_2level_tapbuf_size16_5_sram_inv[4], mux_2level_tapbuf_size16_5_sram_inv[3], mux_2level_tapbuf_size16_5_sram_inv[2], mux_2level_tapbuf_size16_5_sram_inv[1], mux_2level_tapbuf_size16_5_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_5_));

	mux_2level_tapbuf_size16 mux_top_ipin_6 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_6_sram[9], mux_2level_tapbuf_size16_6_sram[8], mux_2level_tapbuf_size16_6_sram[7], mux_2level_tapbuf_size16_6_sram[6], mux_2level_tapbuf_size16_6_sram[5], mux_2level_tapbuf_size16_6_sram[4], mux_2level_tapbuf_size16_6_sram[3], mux_2level_tapbuf_size16_6_sram[2], mux_2level_tapbuf_size16_6_sram[1], mux_2level_tapbuf_size16_6_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_6_sram_inv[9], mux_2level_tapbuf_size16_6_sram_inv[8], mux_2level_tapbuf_size16_6_sram_inv[7], mux_2level_tapbuf_size16_6_sram_inv[6], mux_2level_tapbuf_size16_6_sram_inv[5], mux_2level_tapbuf_size16_6_sram_inv[4], mux_2level_tapbuf_size16_6_sram_inv[3], mux_2level_tapbuf_size16_6_sram_inv[2], mux_2level_tapbuf_size16_6_sram_inv[1], mux_2level_tapbuf_size16_6_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_6_));

	mux_2level_tapbuf_size16 mux_top_ipin_7 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_7_sram[9], mux_2level_tapbuf_size16_7_sram[8], mux_2level_tapbuf_size16_7_sram[7], mux_2level_tapbuf_size16_7_sram[6], mux_2level_tapbuf_size16_7_sram[5], mux_2level_tapbuf_size16_7_sram[4], mux_2level_tapbuf_size16_7_sram[3], mux_2level_tapbuf_size16_7_sram[2], mux_2level_tapbuf_size16_7_sram[1], mux_2level_tapbuf_size16_7_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_7_sram_inv[9], mux_2level_tapbuf_size16_7_sram_inv[8], mux_2level_tapbuf_size16_7_sram_inv[7], mux_2level_tapbuf_size16_7_sram_inv[6], mux_2level_tapbuf_size16_7_sram_inv[5], mux_2level_tapbuf_size16_7_sram_inv[4], mux_2level_tapbuf_size16_7_sram_inv[3], mux_2level_tapbuf_size16_7_sram_inv[2], mux_2level_tapbuf_size16_7_sram_inv[1], mux_2level_tapbuf_size16_7_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_7_));

	mux_2level_tapbuf_size16 mux_top_ipin_8 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_8_sram[9], mux_2level_tapbuf_size16_8_sram[8], mux_2level_tapbuf_size16_8_sram[7], mux_2level_tapbuf_size16_8_sram[6], mux_2level_tapbuf_size16_8_sram[5], mux_2level_tapbuf_size16_8_sram[4], mux_2level_tapbuf_size16_8_sram[3], mux_2level_tapbuf_size16_8_sram[2], mux_2level_tapbuf_size16_8_sram[1], mux_2level_tapbuf_size16_8_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_8_sram_inv[9], mux_2level_tapbuf_size16_8_sram_inv[8], mux_2level_tapbuf_size16_8_sram_inv[7], mux_2level_tapbuf_size16_8_sram_inv[6], mux_2level_tapbuf_size16_8_sram_inv[5], mux_2level_tapbuf_size16_8_sram_inv[4], mux_2level_tapbuf_size16_8_sram_inv[3], mux_2level_tapbuf_size16_8_sram_inv[2], mux_2level_tapbuf_size16_8_sram_inv[1], mux_2level_tapbuf_size16_8_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_8_));

	mux_2level_tapbuf_size16 mux_top_ipin_9 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_9_sram[9], mux_2level_tapbuf_size16_9_sram[8], mux_2level_tapbuf_size16_9_sram[7], mux_2level_tapbuf_size16_9_sram[6], mux_2level_tapbuf_size16_9_sram[5], mux_2level_tapbuf_size16_9_sram[4], mux_2level_tapbuf_size16_9_sram[3], mux_2level_tapbuf_size16_9_sram[2], mux_2level_tapbuf_size16_9_sram[1], mux_2level_tapbuf_size16_9_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_9_sram_inv[9], mux_2level_tapbuf_size16_9_sram_inv[8], mux_2level_tapbuf_size16_9_sram_inv[7], mux_2level_tapbuf_size16_9_sram_inv[6], mux_2level_tapbuf_size16_9_sram_inv[5], mux_2level_tapbuf_size16_9_sram_inv[4], mux_2level_tapbuf_size16_9_sram_inv[3], mux_2level_tapbuf_size16_9_sram_inv[2], mux_2level_tapbuf_size16_9_sram_inv[1], mux_2level_tapbuf_size16_9_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_9_));

	mux_2level_tapbuf_size16 mux_top_ipin_10 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_10_sram[9], mux_2level_tapbuf_size16_10_sram[8], mux_2level_tapbuf_size16_10_sram[7], mux_2level_tapbuf_size16_10_sram[6], mux_2level_tapbuf_size16_10_sram[5], mux_2level_tapbuf_size16_10_sram[4], mux_2level_tapbuf_size16_10_sram[3], mux_2level_tapbuf_size16_10_sram[2], mux_2level_tapbuf_size16_10_sram[1], mux_2level_tapbuf_size16_10_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_10_sram_inv[9], mux_2level_tapbuf_size16_10_sram_inv[8], mux_2level_tapbuf_size16_10_sram_inv[7], mux_2level_tapbuf_size16_10_sram_inv[6], mux_2level_tapbuf_size16_10_sram_inv[5], mux_2level_tapbuf_size16_10_sram_inv[4], mux_2level_tapbuf_size16_10_sram_inv[3], mux_2level_tapbuf_size16_10_sram_inv[2], mux_2level_tapbuf_size16_10_sram_inv[1], mux_2level_tapbuf_size16_10_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_10_));

	mux_2level_tapbuf_size16 mux_top_ipin_11 (
		.in({chanx_left_in[15], chanx_left_in[14], chanx_left_in[13], chanx_left_in[12], chanx_left_in[11], chanx_left_in[10], chanx_left_in[9], chanx_left_in[8], chanx_left_in[7], chanx_left_in[6], chanx_left_in[5], chanx_left_in[4], chanx_left_in[3], chanx_left_in[2], chanx_left_in[1], chanx_left_in[0]}),
		.sram({mux_2level_tapbuf_size16_11_sram[9], mux_2level_tapbuf_size16_11_sram[8], mux_2level_tapbuf_size16_11_sram[7], mux_2level_tapbuf_size16_11_sram[6], mux_2level_tapbuf_size16_11_sram[5], mux_2level_tapbuf_size16_11_sram[4], mux_2level_tapbuf_size16_11_sram[3], mux_2level_tapbuf_size16_11_sram[2], mux_2level_tapbuf_size16_11_sram[1], mux_2level_tapbuf_size16_11_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size16_11_sram_inv[9], mux_2level_tapbuf_size16_11_sram_inv[8], mux_2level_tapbuf_size16_11_sram_inv[7], mux_2level_tapbuf_size16_11_sram_inv[6], mux_2level_tapbuf_size16_11_sram_inv[5], mux_2level_tapbuf_size16_11_sram_inv[4], mux_2level_tapbuf_size16_11_sram_inv[3], mux_2level_tapbuf_size16_11_sram_inv[2], mux_2level_tapbuf_size16_11_sram_inv[1], mux_2level_tapbuf_size16_11_sram_inv[0]}),
		.out(bottom_grid_top_width_0_height_0_subtile_0__pin_I_11_));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_0 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[9], cbx_2__config_group_mem_size120_0_mem_out[8], cbx_2__config_group_mem_size120_0_mem_out[7], cbx_2__config_group_mem_size120_0_mem_out[6], cbx_2__config_group_mem_size120_0_mem_out[5], cbx_2__config_group_mem_size120_0_mem_out[4], cbx_2__config_group_mem_size120_0_mem_out[3], cbx_2__config_group_mem_size120_0_mem_out[2], cbx_2__config_group_mem_size120_0_mem_out[1], cbx_2__config_group_mem_size120_0_mem_out[0]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[9], cbx_2__config_group_mem_size120_0_mem_outb[8], cbx_2__config_group_mem_size120_0_mem_outb[7], cbx_2__config_group_mem_size120_0_mem_outb[6], cbx_2__config_group_mem_size120_0_mem_outb[5], cbx_2__config_group_mem_size120_0_mem_outb[4], cbx_2__config_group_mem_size120_0_mem_outb[3], cbx_2__config_group_mem_size120_0_mem_outb[2], cbx_2__config_group_mem_size120_0_mem_outb[1], cbx_2__config_group_mem_size120_0_mem_outb[0]}),
		.mem_out({mux_2level_tapbuf_size16_0_sram[9], mux_2level_tapbuf_size16_0_sram[8], mux_2level_tapbuf_size16_0_sram[7], mux_2level_tapbuf_size16_0_sram[6], mux_2level_tapbuf_size16_0_sram[5], mux_2level_tapbuf_size16_0_sram[4], mux_2level_tapbuf_size16_0_sram[3], mux_2level_tapbuf_size16_0_sram[2], mux_2level_tapbuf_size16_0_sram[1], mux_2level_tapbuf_size16_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_0_sram_inv[9], mux_2level_tapbuf_size16_0_sram_inv[8], mux_2level_tapbuf_size16_0_sram_inv[7], mux_2level_tapbuf_size16_0_sram_inv[6], mux_2level_tapbuf_size16_0_sram_inv[5], mux_2level_tapbuf_size16_0_sram_inv[4], mux_2level_tapbuf_size16_0_sram_inv[3], mux_2level_tapbuf_size16_0_sram_inv[2], mux_2level_tapbuf_size16_0_sram_inv[1], mux_2level_tapbuf_size16_0_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_1 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[19], cbx_2__config_group_mem_size120_0_mem_out[18], cbx_2__config_group_mem_size120_0_mem_out[17], cbx_2__config_group_mem_size120_0_mem_out[16], cbx_2__config_group_mem_size120_0_mem_out[15], cbx_2__config_group_mem_size120_0_mem_out[14], cbx_2__config_group_mem_size120_0_mem_out[13], cbx_2__config_group_mem_size120_0_mem_out[12], cbx_2__config_group_mem_size120_0_mem_out[11], cbx_2__config_group_mem_size120_0_mem_out[10]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[19], cbx_2__config_group_mem_size120_0_mem_outb[18], cbx_2__config_group_mem_size120_0_mem_outb[17], cbx_2__config_group_mem_size120_0_mem_outb[16], cbx_2__config_group_mem_size120_0_mem_outb[15], cbx_2__config_group_mem_size120_0_mem_outb[14], cbx_2__config_group_mem_size120_0_mem_outb[13], cbx_2__config_group_mem_size120_0_mem_outb[12], cbx_2__config_group_mem_size120_0_mem_outb[11], cbx_2__config_group_mem_size120_0_mem_outb[10]}),
		.mem_out({mux_2level_tapbuf_size16_1_sram[9], mux_2level_tapbuf_size16_1_sram[8], mux_2level_tapbuf_size16_1_sram[7], mux_2level_tapbuf_size16_1_sram[6], mux_2level_tapbuf_size16_1_sram[5], mux_2level_tapbuf_size16_1_sram[4], mux_2level_tapbuf_size16_1_sram[3], mux_2level_tapbuf_size16_1_sram[2], mux_2level_tapbuf_size16_1_sram[1], mux_2level_tapbuf_size16_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_1_sram_inv[9], mux_2level_tapbuf_size16_1_sram_inv[8], mux_2level_tapbuf_size16_1_sram_inv[7], mux_2level_tapbuf_size16_1_sram_inv[6], mux_2level_tapbuf_size16_1_sram_inv[5], mux_2level_tapbuf_size16_1_sram_inv[4], mux_2level_tapbuf_size16_1_sram_inv[3], mux_2level_tapbuf_size16_1_sram_inv[2], mux_2level_tapbuf_size16_1_sram_inv[1], mux_2level_tapbuf_size16_1_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_2 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[29], cbx_2__config_group_mem_size120_0_mem_out[28], cbx_2__config_group_mem_size120_0_mem_out[27], cbx_2__config_group_mem_size120_0_mem_out[26], cbx_2__config_group_mem_size120_0_mem_out[25], cbx_2__config_group_mem_size120_0_mem_out[24], cbx_2__config_group_mem_size120_0_mem_out[23], cbx_2__config_group_mem_size120_0_mem_out[22], cbx_2__config_group_mem_size120_0_mem_out[21], cbx_2__config_group_mem_size120_0_mem_out[20]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[29], cbx_2__config_group_mem_size120_0_mem_outb[28], cbx_2__config_group_mem_size120_0_mem_outb[27], cbx_2__config_group_mem_size120_0_mem_outb[26], cbx_2__config_group_mem_size120_0_mem_outb[25], cbx_2__config_group_mem_size120_0_mem_outb[24], cbx_2__config_group_mem_size120_0_mem_outb[23], cbx_2__config_group_mem_size120_0_mem_outb[22], cbx_2__config_group_mem_size120_0_mem_outb[21], cbx_2__config_group_mem_size120_0_mem_outb[20]}),
		.mem_out({mux_2level_tapbuf_size16_2_sram[9], mux_2level_tapbuf_size16_2_sram[8], mux_2level_tapbuf_size16_2_sram[7], mux_2level_tapbuf_size16_2_sram[6], mux_2level_tapbuf_size16_2_sram[5], mux_2level_tapbuf_size16_2_sram[4], mux_2level_tapbuf_size16_2_sram[3], mux_2level_tapbuf_size16_2_sram[2], mux_2level_tapbuf_size16_2_sram[1], mux_2level_tapbuf_size16_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_2_sram_inv[9], mux_2level_tapbuf_size16_2_sram_inv[8], mux_2level_tapbuf_size16_2_sram_inv[7], mux_2level_tapbuf_size16_2_sram_inv[6], mux_2level_tapbuf_size16_2_sram_inv[5], mux_2level_tapbuf_size16_2_sram_inv[4], mux_2level_tapbuf_size16_2_sram_inv[3], mux_2level_tapbuf_size16_2_sram_inv[2], mux_2level_tapbuf_size16_2_sram_inv[1], mux_2level_tapbuf_size16_2_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_3 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[39], cbx_2__config_group_mem_size120_0_mem_out[38], cbx_2__config_group_mem_size120_0_mem_out[37], cbx_2__config_group_mem_size120_0_mem_out[36], cbx_2__config_group_mem_size120_0_mem_out[35], cbx_2__config_group_mem_size120_0_mem_out[34], cbx_2__config_group_mem_size120_0_mem_out[33], cbx_2__config_group_mem_size120_0_mem_out[32], cbx_2__config_group_mem_size120_0_mem_out[31], cbx_2__config_group_mem_size120_0_mem_out[30]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[39], cbx_2__config_group_mem_size120_0_mem_outb[38], cbx_2__config_group_mem_size120_0_mem_outb[37], cbx_2__config_group_mem_size120_0_mem_outb[36], cbx_2__config_group_mem_size120_0_mem_outb[35], cbx_2__config_group_mem_size120_0_mem_outb[34], cbx_2__config_group_mem_size120_0_mem_outb[33], cbx_2__config_group_mem_size120_0_mem_outb[32], cbx_2__config_group_mem_size120_0_mem_outb[31], cbx_2__config_group_mem_size120_0_mem_outb[30]}),
		.mem_out({mux_2level_tapbuf_size16_3_sram[9], mux_2level_tapbuf_size16_3_sram[8], mux_2level_tapbuf_size16_3_sram[7], mux_2level_tapbuf_size16_3_sram[6], mux_2level_tapbuf_size16_3_sram[5], mux_2level_tapbuf_size16_3_sram[4], mux_2level_tapbuf_size16_3_sram[3], mux_2level_tapbuf_size16_3_sram[2], mux_2level_tapbuf_size16_3_sram[1], mux_2level_tapbuf_size16_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_3_sram_inv[9], mux_2level_tapbuf_size16_3_sram_inv[8], mux_2level_tapbuf_size16_3_sram_inv[7], mux_2level_tapbuf_size16_3_sram_inv[6], mux_2level_tapbuf_size16_3_sram_inv[5], mux_2level_tapbuf_size16_3_sram_inv[4], mux_2level_tapbuf_size16_3_sram_inv[3], mux_2level_tapbuf_size16_3_sram_inv[2], mux_2level_tapbuf_size16_3_sram_inv[1], mux_2level_tapbuf_size16_3_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_4 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[49], cbx_2__config_group_mem_size120_0_mem_out[48], cbx_2__config_group_mem_size120_0_mem_out[47], cbx_2__config_group_mem_size120_0_mem_out[46], cbx_2__config_group_mem_size120_0_mem_out[45], cbx_2__config_group_mem_size120_0_mem_out[44], cbx_2__config_group_mem_size120_0_mem_out[43], cbx_2__config_group_mem_size120_0_mem_out[42], cbx_2__config_group_mem_size120_0_mem_out[41], cbx_2__config_group_mem_size120_0_mem_out[40]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[49], cbx_2__config_group_mem_size120_0_mem_outb[48], cbx_2__config_group_mem_size120_0_mem_outb[47], cbx_2__config_group_mem_size120_0_mem_outb[46], cbx_2__config_group_mem_size120_0_mem_outb[45], cbx_2__config_group_mem_size120_0_mem_outb[44], cbx_2__config_group_mem_size120_0_mem_outb[43], cbx_2__config_group_mem_size120_0_mem_outb[42], cbx_2__config_group_mem_size120_0_mem_outb[41], cbx_2__config_group_mem_size120_0_mem_outb[40]}),
		.mem_out({mux_2level_tapbuf_size16_4_sram[9], mux_2level_tapbuf_size16_4_sram[8], mux_2level_tapbuf_size16_4_sram[7], mux_2level_tapbuf_size16_4_sram[6], mux_2level_tapbuf_size16_4_sram[5], mux_2level_tapbuf_size16_4_sram[4], mux_2level_tapbuf_size16_4_sram[3], mux_2level_tapbuf_size16_4_sram[2], mux_2level_tapbuf_size16_4_sram[1], mux_2level_tapbuf_size16_4_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_4_sram_inv[9], mux_2level_tapbuf_size16_4_sram_inv[8], mux_2level_tapbuf_size16_4_sram_inv[7], mux_2level_tapbuf_size16_4_sram_inv[6], mux_2level_tapbuf_size16_4_sram_inv[5], mux_2level_tapbuf_size16_4_sram_inv[4], mux_2level_tapbuf_size16_4_sram_inv[3], mux_2level_tapbuf_size16_4_sram_inv[2], mux_2level_tapbuf_size16_4_sram_inv[1], mux_2level_tapbuf_size16_4_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_5 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[59], cbx_2__config_group_mem_size120_0_mem_out[58], cbx_2__config_group_mem_size120_0_mem_out[57], cbx_2__config_group_mem_size120_0_mem_out[56], cbx_2__config_group_mem_size120_0_mem_out[55], cbx_2__config_group_mem_size120_0_mem_out[54], cbx_2__config_group_mem_size120_0_mem_out[53], cbx_2__config_group_mem_size120_0_mem_out[52], cbx_2__config_group_mem_size120_0_mem_out[51], cbx_2__config_group_mem_size120_0_mem_out[50]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[59], cbx_2__config_group_mem_size120_0_mem_outb[58], cbx_2__config_group_mem_size120_0_mem_outb[57], cbx_2__config_group_mem_size120_0_mem_outb[56], cbx_2__config_group_mem_size120_0_mem_outb[55], cbx_2__config_group_mem_size120_0_mem_outb[54], cbx_2__config_group_mem_size120_0_mem_outb[53], cbx_2__config_group_mem_size120_0_mem_outb[52], cbx_2__config_group_mem_size120_0_mem_outb[51], cbx_2__config_group_mem_size120_0_mem_outb[50]}),
		.mem_out({mux_2level_tapbuf_size16_5_sram[9], mux_2level_tapbuf_size16_5_sram[8], mux_2level_tapbuf_size16_5_sram[7], mux_2level_tapbuf_size16_5_sram[6], mux_2level_tapbuf_size16_5_sram[5], mux_2level_tapbuf_size16_5_sram[4], mux_2level_tapbuf_size16_5_sram[3], mux_2level_tapbuf_size16_5_sram[2], mux_2level_tapbuf_size16_5_sram[1], mux_2level_tapbuf_size16_5_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_5_sram_inv[9], mux_2level_tapbuf_size16_5_sram_inv[8], mux_2level_tapbuf_size16_5_sram_inv[7], mux_2level_tapbuf_size16_5_sram_inv[6], mux_2level_tapbuf_size16_5_sram_inv[5], mux_2level_tapbuf_size16_5_sram_inv[4], mux_2level_tapbuf_size16_5_sram_inv[3], mux_2level_tapbuf_size16_5_sram_inv[2], mux_2level_tapbuf_size16_5_sram_inv[1], mux_2level_tapbuf_size16_5_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_6 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[69], cbx_2__config_group_mem_size120_0_mem_out[68], cbx_2__config_group_mem_size120_0_mem_out[67], cbx_2__config_group_mem_size120_0_mem_out[66], cbx_2__config_group_mem_size120_0_mem_out[65], cbx_2__config_group_mem_size120_0_mem_out[64], cbx_2__config_group_mem_size120_0_mem_out[63], cbx_2__config_group_mem_size120_0_mem_out[62], cbx_2__config_group_mem_size120_0_mem_out[61], cbx_2__config_group_mem_size120_0_mem_out[60]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[69], cbx_2__config_group_mem_size120_0_mem_outb[68], cbx_2__config_group_mem_size120_0_mem_outb[67], cbx_2__config_group_mem_size120_0_mem_outb[66], cbx_2__config_group_mem_size120_0_mem_outb[65], cbx_2__config_group_mem_size120_0_mem_outb[64], cbx_2__config_group_mem_size120_0_mem_outb[63], cbx_2__config_group_mem_size120_0_mem_outb[62], cbx_2__config_group_mem_size120_0_mem_outb[61], cbx_2__config_group_mem_size120_0_mem_outb[60]}),
		.mem_out({mux_2level_tapbuf_size16_6_sram[9], mux_2level_tapbuf_size16_6_sram[8], mux_2level_tapbuf_size16_6_sram[7], mux_2level_tapbuf_size16_6_sram[6], mux_2level_tapbuf_size16_6_sram[5], mux_2level_tapbuf_size16_6_sram[4], mux_2level_tapbuf_size16_6_sram[3], mux_2level_tapbuf_size16_6_sram[2], mux_2level_tapbuf_size16_6_sram[1], mux_2level_tapbuf_size16_6_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_6_sram_inv[9], mux_2level_tapbuf_size16_6_sram_inv[8], mux_2level_tapbuf_size16_6_sram_inv[7], mux_2level_tapbuf_size16_6_sram_inv[6], mux_2level_tapbuf_size16_6_sram_inv[5], mux_2level_tapbuf_size16_6_sram_inv[4], mux_2level_tapbuf_size16_6_sram_inv[3], mux_2level_tapbuf_size16_6_sram_inv[2], mux_2level_tapbuf_size16_6_sram_inv[1], mux_2level_tapbuf_size16_6_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_7 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[79], cbx_2__config_group_mem_size120_0_mem_out[78], cbx_2__config_group_mem_size120_0_mem_out[77], cbx_2__config_group_mem_size120_0_mem_out[76], cbx_2__config_group_mem_size120_0_mem_out[75], cbx_2__config_group_mem_size120_0_mem_out[74], cbx_2__config_group_mem_size120_0_mem_out[73], cbx_2__config_group_mem_size120_0_mem_out[72], cbx_2__config_group_mem_size120_0_mem_out[71], cbx_2__config_group_mem_size120_0_mem_out[70]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[79], cbx_2__config_group_mem_size120_0_mem_outb[78], cbx_2__config_group_mem_size120_0_mem_outb[77], cbx_2__config_group_mem_size120_0_mem_outb[76], cbx_2__config_group_mem_size120_0_mem_outb[75], cbx_2__config_group_mem_size120_0_mem_outb[74], cbx_2__config_group_mem_size120_0_mem_outb[73], cbx_2__config_group_mem_size120_0_mem_outb[72], cbx_2__config_group_mem_size120_0_mem_outb[71], cbx_2__config_group_mem_size120_0_mem_outb[70]}),
		.mem_out({mux_2level_tapbuf_size16_7_sram[9], mux_2level_tapbuf_size16_7_sram[8], mux_2level_tapbuf_size16_7_sram[7], mux_2level_tapbuf_size16_7_sram[6], mux_2level_tapbuf_size16_7_sram[5], mux_2level_tapbuf_size16_7_sram[4], mux_2level_tapbuf_size16_7_sram[3], mux_2level_tapbuf_size16_7_sram[2], mux_2level_tapbuf_size16_7_sram[1], mux_2level_tapbuf_size16_7_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_7_sram_inv[9], mux_2level_tapbuf_size16_7_sram_inv[8], mux_2level_tapbuf_size16_7_sram_inv[7], mux_2level_tapbuf_size16_7_sram_inv[6], mux_2level_tapbuf_size16_7_sram_inv[5], mux_2level_tapbuf_size16_7_sram_inv[4], mux_2level_tapbuf_size16_7_sram_inv[3], mux_2level_tapbuf_size16_7_sram_inv[2], mux_2level_tapbuf_size16_7_sram_inv[1], mux_2level_tapbuf_size16_7_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_8 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[89], cbx_2__config_group_mem_size120_0_mem_out[88], cbx_2__config_group_mem_size120_0_mem_out[87], cbx_2__config_group_mem_size120_0_mem_out[86], cbx_2__config_group_mem_size120_0_mem_out[85], cbx_2__config_group_mem_size120_0_mem_out[84], cbx_2__config_group_mem_size120_0_mem_out[83], cbx_2__config_group_mem_size120_0_mem_out[82], cbx_2__config_group_mem_size120_0_mem_out[81], cbx_2__config_group_mem_size120_0_mem_out[80]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[89], cbx_2__config_group_mem_size120_0_mem_outb[88], cbx_2__config_group_mem_size120_0_mem_outb[87], cbx_2__config_group_mem_size120_0_mem_outb[86], cbx_2__config_group_mem_size120_0_mem_outb[85], cbx_2__config_group_mem_size120_0_mem_outb[84], cbx_2__config_group_mem_size120_0_mem_outb[83], cbx_2__config_group_mem_size120_0_mem_outb[82], cbx_2__config_group_mem_size120_0_mem_outb[81], cbx_2__config_group_mem_size120_0_mem_outb[80]}),
		.mem_out({mux_2level_tapbuf_size16_8_sram[9], mux_2level_tapbuf_size16_8_sram[8], mux_2level_tapbuf_size16_8_sram[7], mux_2level_tapbuf_size16_8_sram[6], mux_2level_tapbuf_size16_8_sram[5], mux_2level_tapbuf_size16_8_sram[4], mux_2level_tapbuf_size16_8_sram[3], mux_2level_tapbuf_size16_8_sram[2], mux_2level_tapbuf_size16_8_sram[1], mux_2level_tapbuf_size16_8_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_8_sram_inv[9], mux_2level_tapbuf_size16_8_sram_inv[8], mux_2level_tapbuf_size16_8_sram_inv[7], mux_2level_tapbuf_size16_8_sram_inv[6], mux_2level_tapbuf_size16_8_sram_inv[5], mux_2level_tapbuf_size16_8_sram_inv[4], mux_2level_tapbuf_size16_8_sram_inv[3], mux_2level_tapbuf_size16_8_sram_inv[2], mux_2level_tapbuf_size16_8_sram_inv[1], mux_2level_tapbuf_size16_8_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_9 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[99], cbx_2__config_group_mem_size120_0_mem_out[98], cbx_2__config_group_mem_size120_0_mem_out[97], cbx_2__config_group_mem_size120_0_mem_out[96], cbx_2__config_group_mem_size120_0_mem_out[95], cbx_2__config_group_mem_size120_0_mem_out[94], cbx_2__config_group_mem_size120_0_mem_out[93], cbx_2__config_group_mem_size120_0_mem_out[92], cbx_2__config_group_mem_size120_0_mem_out[91], cbx_2__config_group_mem_size120_0_mem_out[90]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[99], cbx_2__config_group_mem_size120_0_mem_outb[98], cbx_2__config_group_mem_size120_0_mem_outb[97], cbx_2__config_group_mem_size120_0_mem_outb[96], cbx_2__config_group_mem_size120_0_mem_outb[95], cbx_2__config_group_mem_size120_0_mem_outb[94], cbx_2__config_group_mem_size120_0_mem_outb[93], cbx_2__config_group_mem_size120_0_mem_outb[92], cbx_2__config_group_mem_size120_0_mem_outb[91], cbx_2__config_group_mem_size120_0_mem_outb[90]}),
		.mem_out({mux_2level_tapbuf_size16_9_sram[9], mux_2level_tapbuf_size16_9_sram[8], mux_2level_tapbuf_size16_9_sram[7], mux_2level_tapbuf_size16_9_sram[6], mux_2level_tapbuf_size16_9_sram[5], mux_2level_tapbuf_size16_9_sram[4], mux_2level_tapbuf_size16_9_sram[3], mux_2level_tapbuf_size16_9_sram[2], mux_2level_tapbuf_size16_9_sram[1], mux_2level_tapbuf_size16_9_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_9_sram_inv[9], mux_2level_tapbuf_size16_9_sram_inv[8], mux_2level_tapbuf_size16_9_sram_inv[7], mux_2level_tapbuf_size16_9_sram_inv[6], mux_2level_tapbuf_size16_9_sram_inv[5], mux_2level_tapbuf_size16_9_sram_inv[4], mux_2level_tapbuf_size16_9_sram_inv[3], mux_2level_tapbuf_size16_9_sram_inv[2], mux_2level_tapbuf_size16_9_sram_inv[1], mux_2level_tapbuf_size16_9_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_10 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[109], cbx_2__config_group_mem_size120_0_mem_out[108], cbx_2__config_group_mem_size120_0_mem_out[107], cbx_2__config_group_mem_size120_0_mem_out[106], cbx_2__config_group_mem_size120_0_mem_out[105], cbx_2__config_group_mem_size120_0_mem_out[104], cbx_2__config_group_mem_size120_0_mem_out[103], cbx_2__config_group_mem_size120_0_mem_out[102], cbx_2__config_group_mem_size120_0_mem_out[101], cbx_2__config_group_mem_size120_0_mem_out[100]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[109], cbx_2__config_group_mem_size120_0_mem_outb[108], cbx_2__config_group_mem_size120_0_mem_outb[107], cbx_2__config_group_mem_size120_0_mem_outb[106], cbx_2__config_group_mem_size120_0_mem_outb[105], cbx_2__config_group_mem_size120_0_mem_outb[104], cbx_2__config_group_mem_size120_0_mem_outb[103], cbx_2__config_group_mem_size120_0_mem_outb[102], cbx_2__config_group_mem_size120_0_mem_outb[101], cbx_2__config_group_mem_size120_0_mem_outb[100]}),
		.mem_out({mux_2level_tapbuf_size16_10_sram[9], mux_2level_tapbuf_size16_10_sram[8], mux_2level_tapbuf_size16_10_sram[7], mux_2level_tapbuf_size16_10_sram[6], mux_2level_tapbuf_size16_10_sram[5], mux_2level_tapbuf_size16_10_sram[4], mux_2level_tapbuf_size16_10_sram[3], mux_2level_tapbuf_size16_10_sram[2], mux_2level_tapbuf_size16_10_sram[1], mux_2level_tapbuf_size16_10_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_10_sram_inv[9], mux_2level_tapbuf_size16_10_sram_inv[8], mux_2level_tapbuf_size16_10_sram_inv[7], mux_2level_tapbuf_size16_10_sram_inv[6], mux_2level_tapbuf_size16_10_sram_inv[5], mux_2level_tapbuf_size16_10_sram_inv[4], mux_2level_tapbuf_size16_10_sram_inv[3], mux_2level_tapbuf_size16_10_sram_inv[2], mux_2level_tapbuf_size16_10_sram_inv[1], mux_2level_tapbuf_size16_10_sram_inv[0]}));

	mux_2level_tapbuf_size16_feedthrough_mem feedthrough_mem_top_ipin_11 (
		.feedthrough_mem_in({cbx_2__config_group_mem_size120_0_mem_out[119], cbx_2__config_group_mem_size120_0_mem_out[118], cbx_2__config_group_mem_size120_0_mem_out[117], cbx_2__config_group_mem_size120_0_mem_out[116], cbx_2__config_group_mem_size120_0_mem_out[115], cbx_2__config_group_mem_size120_0_mem_out[114], cbx_2__config_group_mem_size120_0_mem_out[113], cbx_2__config_group_mem_size120_0_mem_out[112], cbx_2__config_group_mem_size120_0_mem_out[111], cbx_2__config_group_mem_size120_0_mem_out[110]}),
		.feedthrough_mem_inb({cbx_2__config_group_mem_size120_0_mem_outb[119], cbx_2__config_group_mem_size120_0_mem_outb[118], cbx_2__config_group_mem_size120_0_mem_outb[117], cbx_2__config_group_mem_size120_0_mem_outb[116], cbx_2__config_group_mem_size120_0_mem_outb[115], cbx_2__config_group_mem_size120_0_mem_outb[114], cbx_2__config_group_mem_size120_0_mem_outb[113], cbx_2__config_group_mem_size120_0_mem_outb[112], cbx_2__config_group_mem_size120_0_mem_outb[111], cbx_2__config_group_mem_size120_0_mem_outb[110]}),
		.mem_out({mux_2level_tapbuf_size16_11_sram[9], mux_2level_tapbuf_size16_11_sram[8], mux_2level_tapbuf_size16_11_sram[7], mux_2level_tapbuf_size16_11_sram[6], mux_2level_tapbuf_size16_11_sram[5], mux_2level_tapbuf_size16_11_sram[4], mux_2level_tapbuf_size16_11_sram[3], mux_2level_tapbuf_size16_11_sram[2], mux_2level_tapbuf_size16_11_sram[1], mux_2level_tapbuf_size16_11_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size16_11_sram_inv[9], mux_2level_tapbuf_size16_11_sram_inv[8], mux_2level_tapbuf_size16_11_sram_inv[7], mux_2level_tapbuf_size16_11_sram_inv[6], mux_2level_tapbuf_size16_11_sram_inv[5], mux_2level_tapbuf_size16_11_sram_inv[4], mux_2level_tapbuf_size16_11_sram_inv[3], mux_2level_tapbuf_size16_11_sram_inv[2], mux_2level_tapbuf_size16_11_sram_inv[1], mux_2level_tapbuf_size16_11_sram_inv[0]}));

	cbx_2__config_group_mem_size120 cbx_2__config_group_mem_size120 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.mem_out({cbx_2__config_group_mem_size120_0_mem_out[119], cbx_2__config_group_mem_size120_0_mem_out[118], cbx_2__config_group_mem_size120_0_mem_out[117], cbx_2__config_group_mem_size120_0_mem_out[116], cbx_2__config_group_mem_size120_0_mem_out[115], cbx_2__config_group_mem_size120_0_mem_out[114], cbx_2__config_group_mem_size120_0_mem_out[113], cbx_2__config_group_mem_size120_0_mem_out[112], cbx_2__config_group_mem_size120_0_mem_out[111], cbx_2__config_group_mem_size120_0_mem_out[110], cbx_2__config_group_mem_size120_0_mem_out[109], cbx_2__config_group_mem_size120_0_mem_out[108], cbx_2__config_group_mem_size120_0_mem_out[107], cbx_2__config_group_mem_size120_0_mem_out[106], cbx_2__config_group_mem_size120_0_mem_out[105], cbx_2__config_group_mem_size120_0_mem_out[104], cbx_2__config_group_mem_size120_0_mem_out[103], cbx_2__config_group_mem_size120_0_mem_out[102], cbx_2__config_group_mem_size120_0_mem_out[101], cbx_2__config_group_mem_size120_0_mem_out[100], cbx_2__config_group_mem_size120_0_mem_out[99], cbx_2__config_group_mem_size120_0_mem_out[98], cbx_2__config_group_mem_size120_0_mem_out[97], cbx_2__config_group_mem_size120_0_mem_out[96], cbx_2__config_group_mem_size120_0_mem_out[95], cbx_2__config_group_mem_size120_0_mem_out[94], cbx_2__config_group_mem_size120_0_mem_out[93], cbx_2__config_group_mem_size120_0_mem_out[92], cbx_2__config_group_mem_size120_0_mem_out[91], cbx_2__config_group_mem_size120_0_mem_out[90], cbx_2__config_group_mem_size120_0_mem_out[89], cbx_2__config_group_mem_size120_0_mem_out[88], cbx_2__config_group_mem_size120_0_mem_out[87], cbx_2__config_group_mem_size120_0_mem_out[86], cbx_2__config_group_mem_size120_0_mem_out[85], cbx_2__config_group_mem_size120_0_mem_out[84], cbx_2__config_group_mem_size120_0_mem_out[83], cbx_2__config_group_mem_size120_0_mem_out[82], cbx_2__config_group_mem_size120_0_mem_out[81], cbx_2__config_group_mem_size120_0_mem_out[80], cbx_2__config_group_mem_size120_0_mem_out[79], cbx_2__config_group_mem_size120_0_mem_out[78], cbx_2__config_group_mem_size120_0_mem_out[77], cbx_2__config_group_mem_size120_0_mem_out[76], cbx_2__config_group_mem_size120_0_mem_out[75], cbx_2__config_group_mem_size120_0_mem_out[74], cbx_2__config_group_mem_size120_0_mem_out[73], cbx_2__config_group_mem_size120_0_mem_out[72], cbx_2__config_group_mem_size120_0_mem_out[71], cbx_2__config_group_mem_size120_0_mem_out[70], cbx_2__config_group_mem_size120_0_mem_out[69], cbx_2__config_group_mem_size120_0_mem_out[68], cbx_2__config_group_mem_size120_0_mem_out[67], cbx_2__config_group_mem_size120_0_mem_out[66], cbx_2__config_group_mem_size120_0_mem_out[65], cbx_2__config_group_mem_size120_0_mem_out[64], cbx_2__config_group_mem_size120_0_mem_out[63], cbx_2__config_group_mem_size120_0_mem_out[62], cbx_2__config_group_mem_size120_0_mem_out[61], cbx_2__config_group_mem_size120_0_mem_out[60], cbx_2__config_group_mem_size120_0_mem_out[59], cbx_2__config_group_mem_size120_0_mem_out[58], cbx_2__config_group_mem_size120_0_mem_out[57], cbx_2__config_group_mem_size120_0_mem_out[56], cbx_2__config_group_mem_size120_0_mem_out[55], cbx_2__config_group_mem_size120_0_mem_out[54], cbx_2__config_group_mem_size120_0_mem_out[53], cbx_2__config_group_mem_size120_0_mem_out[52], cbx_2__config_group_mem_size120_0_mem_out[51], cbx_2__config_group_mem_size120_0_mem_out[50], cbx_2__config_group_mem_size120_0_mem_out[49], cbx_2__config_group_mem_size120_0_mem_out[48], cbx_2__config_group_mem_size120_0_mem_out[47], cbx_2__config_group_mem_size120_0_mem_out[46], cbx_2__config_group_mem_size120_0_mem_out[45], cbx_2__config_group_mem_size120_0_mem_out[44], cbx_2__config_group_mem_size120_0_mem_out[43], cbx_2__config_group_mem_size120_0_mem_out[42], cbx_2__config_group_mem_size120_0_mem_out[41], cbx_2__config_group_mem_size120_0_mem_out[40], cbx_2__config_group_mem_size120_0_mem_out[39], cbx_2__config_group_mem_size120_0_mem_out[38], cbx_2__config_group_mem_size120_0_mem_out[37], cbx_2__config_group_mem_size120_0_mem_out[36], cbx_2__config_group_mem_size120_0_mem_out[35], cbx_2__config_group_mem_size120_0_mem_out[34], cbx_2__config_group_mem_size120_0_mem_out[33], cbx_2__config_group_mem_size120_0_mem_out[32], cbx_2__config_group_mem_size120_0_mem_out[31], cbx_2__config_group_mem_size120_0_mem_out[30], cbx_2__config_group_mem_size120_0_mem_out[29], cbx_2__config_group_mem_size120_0_mem_out[28], cbx_2__config_group_mem_size120_0_mem_out[27], cbx_2__config_group_mem_size120_0_mem_out[26], cbx_2__config_group_mem_size120_0_mem_out[25], cbx_2__config_group_mem_size120_0_mem_out[24], cbx_2__config_group_mem_size120_0_mem_out[23], cbx_2__config_group_mem_size120_0_mem_out[22], cbx_2__config_group_mem_size120_0_mem_out[21], cbx_2__config_group_mem_size120_0_mem_out[20], cbx_2__config_group_mem_size120_0_mem_out[19], cbx_2__config_group_mem_size120_0_mem_out[18], cbx_2__config_group_mem_size120_0_mem_out[17], cbx_2__config_group_mem_size120_0_mem_out[16], cbx_2__config_group_mem_size120_0_mem_out[15], cbx_2__config_group_mem_size120_0_mem_out[14], cbx_2__config_group_mem_size120_0_mem_out[13], cbx_2__config_group_mem_size120_0_mem_out[12], cbx_2__config_group_mem_size120_0_mem_out[11], cbx_2__config_group_mem_size120_0_mem_out[10], cbx_2__config_group_mem_size120_0_mem_out[9], cbx_2__config_group_mem_size120_0_mem_out[8], cbx_2__config_group_mem_size120_0_mem_out[7], cbx_2__config_group_mem_size120_0_mem_out[6], cbx_2__config_group_mem_size120_0_mem_out[5], cbx_2__config_group_mem_size120_0_mem_out[4], cbx_2__config_group_mem_size120_0_mem_out[3], cbx_2__config_group_mem_size120_0_mem_out[2], cbx_2__config_group_mem_size120_0_mem_out[1], cbx_2__config_group_mem_size120_0_mem_out[0]}),
		.mem_outb({cbx_2__config_group_mem_size120_0_mem_outb[119], cbx_2__config_group_mem_size120_0_mem_outb[118], cbx_2__config_group_mem_size120_0_mem_outb[117], cbx_2__config_group_mem_size120_0_mem_outb[116], cbx_2__config_group_mem_size120_0_mem_outb[115], cbx_2__config_group_mem_size120_0_mem_outb[114], cbx_2__config_group_mem_size120_0_mem_outb[113], cbx_2__config_group_mem_size120_0_mem_outb[112], cbx_2__config_group_mem_size120_0_mem_outb[111], cbx_2__config_group_mem_size120_0_mem_outb[110], cbx_2__config_group_mem_size120_0_mem_outb[109], cbx_2__config_group_mem_size120_0_mem_outb[108], cbx_2__config_group_mem_size120_0_mem_outb[107], cbx_2__config_group_mem_size120_0_mem_outb[106], cbx_2__config_group_mem_size120_0_mem_outb[105], cbx_2__config_group_mem_size120_0_mem_outb[104], cbx_2__config_group_mem_size120_0_mem_outb[103], cbx_2__config_group_mem_size120_0_mem_outb[102], cbx_2__config_group_mem_size120_0_mem_outb[101], cbx_2__config_group_mem_size120_0_mem_outb[100], cbx_2__config_group_mem_size120_0_mem_outb[99], cbx_2__config_group_mem_size120_0_mem_outb[98], cbx_2__config_group_mem_size120_0_mem_outb[97], cbx_2__config_group_mem_size120_0_mem_outb[96], cbx_2__config_group_mem_size120_0_mem_outb[95], cbx_2__config_group_mem_size120_0_mem_outb[94], cbx_2__config_group_mem_size120_0_mem_outb[93], cbx_2__config_group_mem_size120_0_mem_outb[92], cbx_2__config_group_mem_size120_0_mem_outb[91], cbx_2__config_group_mem_size120_0_mem_outb[90], cbx_2__config_group_mem_size120_0_mem_outb[89], cbx_2__config_group_mem_size120_0_mem_outb[88], cbx_2__config_group_mem_size120_0_mem_outb[87], cbx_2__config_group_mem_size120_0_mem_outb[86], cbx_2__config_group_mem_size120_0_mem_outb[85], cbx_2__config_group_mem_size120_0_mem_outb[84], cbx_2__config_group_mem_size120_0_mem_outb[83], cbx_2__config_group_mem_size120_0_mem_outb[82], cbx_2__config_group_mem_size120_0_mem_outb[81], cbx_2__config_group_mem_size120_0_mem_outb[80], cbx_2__config_group_mem_size120_0_mem_outb[79], cbx_2__config_group_mem_size120_0_mem_outb[78], cbx_2__config_group_mem_size120_0_mem_outb[77], cbx_2__config_group_mem_size120_0_mem_outb[76], cbx_2__config_group_mem_size120_0_mem_outb[75], cbx_2__config_group_mem_size120_0_mem_outb[74], cbx_2__config_group_mem_size120_0_mem_outb[73], cbx_2__config_group_mem_size120_0_mem_outb[72], cbx_2__config_group_mem_size120_0_mem_outb[71], cbx_2__config_group_mem_size120_0_mem_outb[70], cbx_2__config_group_mem_size120_0_mem_outb[69], cbx_2__config_group_mem_size120_0_mem_outb[68], cbx_2__config_group_mem_size120_0_mem_outb[67], cbx_2__config_group_mem_size120_0_mem_outb[66], cbx_2__config_group_mem_size120_0_mem_outb[65], cbx_2__config_group_mem_size120_0_mem_outb[64], cbx_2__config_group_mem_size120_0_mem_outb[63], cbx_2__config_group_mem_size120_0_mem_outb[62], cbx_2__config_group_mem_size120_0_mem_outb[61], cbx_2__config_group_mem_size120_0_mem_outb[60], cbx_2__config_group_mem_size120_0_mem_outb[59], cbx_2__config_group_mem_size120_0_mem_outb[58], cbx_2__config_group_mem_size120_0_mem_outb[57], cbx_2__config_group_mem_size120_0_mem_outb[56], cbx_2__config_group_mem_size120_0_mem_outb[55], cbx_2__config_group_mem_size120_0_mem_outb[54], cbx_2__config_group_mem_size120_0_mem_outb[53], cbx_2__config_group_mem_size120_0_mem_outb[52], cbx_2__config_group_mem_size120_0_mem_outb[51], cbx_2__config_group_mem_size120_0_mem_outb[50], cbx_2__config_group_mem_size120_0_mem_outb[49], cbx_2__config_group_mem_size120_0_mem_outb[48], cbx_2__config_group_mem_size120_0_mem_outb[47], cbx_2__config_group_mem_size120_0_mem_outb[46], cbx_2__config_group_mem_size120_0_mem_outb[45], cbx_2__config_group_mem_size120_0_mem_outb[44], cbx_2__config_group_mem_size120_0_mem_outb[43], cbx_2__config_group_mem_size120_0_mem_outb[42], cbx_2__config_group_mem_size120_0_mem_outb[41], cbx_2__config_group_mem_size120_0_mem_outb[40], cbx_2__config_group_mem_size120_0_mem_outb[39], cbx_2__config_group_mem_size120_0_mem_outb[38], cbx_2__config_group_mem_size120_0_mem_outb[37], cbx_2__config_group_mem_size120_0_mem_outb[36], cbx_2__config_group_mem_size120_0_mem_outb[35], cbx_2__config_group_mem_size120_0_mem_outb[34], cbx_2__config_group_mem_size120_0_mem_outb[33], cbx_2__config_group_mem_size120_0_mem_outb[32], cbx_2__config_group_mem_size120_0_mem_outb[31], cbx_2__config_group_mem_size120_0_mem_outb[30], cbx_2__config_group_mem_size120_0_mem_outb[29], cbx_2__config_group_mem_size120_0_mem_outb[28], cbx_2__config_group_mem_size120_0_mem_outb[27], cbx_2__config_group_mem_size120_0_mem_outb[26], cbx_2__config_group_mem_size120_0_mem_outb[25], cbx_2__config_group_mem_size120_0_mem_outb[24], cbx_2__config_group_mem_size120_0_mem_outb[23], cbx_2__config_group_mem_size120_0_mem_outb[22], cbx_2__config_group_mem_size120_0_mem_outb[21], cbx_2__config_group_mem_size120_0_mem_outb[20], cbx_2__config_group_mem_size120_0_mem_outb[19], cbx_2__config_group_mem_size120_0_mem_outb[18], cbx_2__config_group_mem_size120_0_mem_outb[17], cbx_2__config_group_mem_size120_0_mem_outb[16], cbx_2__config_group_mem_size120_0_mem_outb[15], cbx_2__config_group_mem_size120_0_mem_outb[14], cbx_2__config_group_mem_size120_0_mem_outb[13], cbx_2__config_group_mem_size120_0_mem_outb[12], cbx_2__config_group_mem_size120_0_mem_outb[11], cbx_2__config_group_mem_size120_0_mem_outb[10], cbx_2__config_group_mem_size120_0_mem_outb[9], cbx_2__config_group_mem_size120_0_mem_outb[8], cbx_2__config_group_mem_size120_0_mem_outb[7], cbx_2__config_group_mem_size120_0_mem_outb[6], cbx_2__config_group_mem_size120_0_mem_outb[5], cbx_2__config_group_mem_size120_0_mem_outb[4], cbx_2__config_group_mem_size120_0_mem_outb[3], cbx_2__config_group_mem_size120_0_mem_outb[2], cbx_2__config_group_mem_size120_0_mem_outb[1], cbx_2__config_group_mem_size120_0_mem_outb[0]}),
		.ccff_tail(ccff_tail));

endmodule
// ----- END Verilog module for cbx_2__2_ -----

//----- Default net type -----
`default_nettype wire




