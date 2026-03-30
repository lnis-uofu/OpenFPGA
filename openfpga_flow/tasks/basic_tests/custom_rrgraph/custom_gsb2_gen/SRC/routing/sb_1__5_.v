//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[1][5]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_1__5_ -----
module sb_1__5_(pReset,
                prog_clk,
                chany_top_in,
                chanx_right_in,
                chany_bottom_in,
                bottom_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_,
                bottom_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_,
                bottom_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_,
                bottom_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_,
                bottom_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_,
                bottom_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_,
                bottom_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_,
                bottom_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_,
                chanx_left_in,
                ccff_head,
                chany_top_out,
                chanx_right_out,
                chany_bottom_out,
                chanx_left_out,
                ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [15:0] chany_top_in;
//----- INPUT PORTS -----
input [15:0] chanx_right_in;
//----- INPUT PORTS -----
input [15:0] chany_bottom_in;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_;
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
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [13:0] mux_2level_tapbuf_size40_0_sram;
wire [13:0] mux_2level_tapbuf_size40_0_sram_inv;
wire [13:0] mux_2level_tapbuf_size40_1_sram;
wire [13:0] mux_2level_tapbuf_size40_1_sram_inv;
wire [13:0] mux_2level_tapbuf_size40_2_sram;
wire [13:0] mux_2level_tapbuf_size40_2_sram_inv;
wire [13:0] mux_2level_tapbuf_size40_3_sram;
wire [13:0] mux_2level_tapbuf_size40_3_sram_inv;
wire [55:0] sb_6__config_group_mem_size56_0_mem_out;
wire [55:0] sb_6__config_group_mem_size56_0_mem_outb;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[1] = chany_top_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[2] = chany_top_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[3] = chany_top_in[3];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out[5] = chany_top_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out[6] = chany_top_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chany_bottom_out[7] = chany_top_in[7];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_bottom_out[9] = chany_top_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_bottom_out[10] = chany_top_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 3 -----
	assign chany_bottom_out[11] = chany_top_in[11];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_bottom_out[13] = chany_top_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_bottom_out[14] = chany_top_in[14];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chany_bottom_out[15] = chany_top_in[15];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[1] = chanx_right_in[1];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[2] = chanx_right_in[2];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[3] = chanx_right_in[3];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[5] = chanx_right_in[5];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[6] = chanx_right_in[6];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[7] = chanx_right_in[7];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[9] = chanx_right_in[9];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[10] = chanx_right_in[10];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[11] = chanx_right_in[11];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[13] = chanx_right_in[13];
// ----- Local connection due to Wire 30 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[14] = chanx_right_in[14];
// ----- Local connection due to Wire 31 -----
// ----- Net source id 0 -----
// ----- Net sink id 4 -----
	assign chanx_left_out[15] = chanx_right_in[15];
// ----- Local connection due to Wire 32 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[0] = chany_bottom_in[0];
// ----- Local connection due to Wire 33 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[1] = chany_bottom_in[1];
// ----- Local connection due to Wire 34 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[2] = chany_bottom_in[2];
// ----- Local connection due to Wire 35 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[3] = chany_bottom_in[3];
// ----- Local connection due to Wire 36 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[4] = chany_bottom_in[4];
// ----- Local connection due to Wire 37 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[5] = chany_bottom_in[5];
// ----- Local connection due to Wire 38 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[6] = chany_bottom_in[6];
// ----- Local connection due to Wire 39 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[7] = chany_bottom_in[7];
// ----- Local connection due to Wire 40 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[8] = chany_bottom_in[8];
// ----- Local connection due to Wire 41 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[9] = chany_bottom_in[9];
// ----- Local connection due to Wire 42 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[10] = chany_bottom_in[10];
// ----- Local connection due to Wire 43 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[11] = chany_bottom_in[11];
// ----- Local connection due to Wire 44 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[12] = chany_bottom_in[12];
// ----- Local connection due to Wire 45 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[13] = chany_bottom_in[13];
// ----- Local connection due to Wire 46 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[14] = chany_bottom_in[14];
// ----- Local connection due to Wire 47 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[15] = chany_bottom_in[15];
// ----- Local connection due to Wire 56 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[0] = chanx_left_in[0];
// ----- Local connection due to Wire 57 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[1] = chanx_left_in[1];
// ----- Local connection due to Wire 58 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[2] = chanx_left_in[2];
// ----- Local connection due to Wire 59 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[3] = chanx_left_in[3];
// ----- Local connection due to Wire 60 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[4] = chanx_left_in[4];
// ----- Local connection due to Wire 61 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[5] = chanx_left_in[5];
// ----- Local connection due to Wire 62 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[6] = chanx_left_in[6];
// ----- Local connection due to Wire 63 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[7] = chanx_left_in[7];
// ----- Local connection due to Wire 64 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[8] = chanx_left_in[8];
// ----- Local connection due to Wire 65 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[9] = chanx_left_in[9];
// ----- Local connection due to Wire 66 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[10] = chanx_left_in[10];
// ----- Local connection due to Wire 67 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[11] = chanx_left_in[11];
// ----- Local connection due to Wire 68 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[12] = chanx_left_in[12];
// ----- Local connection due to Wire 69 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[13] = chanx_left_in[13];
// ----- Local connection due to Wire 70 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[14] = chanx_left_in[14];
// ----- Local connection due to Wire 71 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_right_out[15] = chanx_left_in[15];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40 mux_bottom_track_1 (
		.in({bottom_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_0_sram[13], mux_2level_tapbuf_size40_0_sram[12], mux_2level_tapbuf_size40_0_sram[11], mux_2level_tapbuf_size40_0_sram[10], mux_2level_tapbuf_size40_0_sram[9], mux_2level_tapbuf_size40_0_sram[8], mux_2level_tapbuf_size40_0_sram[7], mux_2level_tapbuf_size40_0_sram[6], mux_2level_tapbuf_size40_0_sram[5], mux_2level_tapbuf_size40_0_sram[4], mux_2level_tapbuf_size40_0_sram[3], mux_2level_tapbuf_size40_0_sram[2], mux_2level_tapbuf_size40_0_sram[1], mux_2level_tapbuf_size40_0_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_0_sram_inv[13], mux_2level_tapbuf_size40_0_sram_inv[12], mux_2level_tapbuf_size40_0_sram_inv[11], mux_2level_tapbuf_size40_0_sram_inv[10], mux_2level_tapbuf_size40_0_sram_inv[9], mux_2level_tapbuf_size40_0_sram_inv[8], mux_2level_tapbuf_size40_0_sram_inv[7], mux_2level_tapbuf_size40_0_sram_inv[6], mux_2level_tapbuf_size40_0_sram_inv[5], mux_2level_tapbuf_size40_0_sram_inv[4], mux_2level_tapbuf_size40_0_sram_inv[3], mux_2level_tapbuf_size40_0_sram_inv[2], mux_2level_tapbuf_size40_0_sram_inv[1], mux_2level_tapbuf_size40_0_sram_inv[0]}),
		.out(chany_bottom_out[0]));

	mux_2level_tapbuf_size40 mux_bottom_track_9 (
		.in({bottom_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_1_sram[13], mux_2level_tapbuf_size40_1_sram[12], mux_2level_tapbuf_size40_1_sram[11], mux_2level_tapbuf_size40_1_sram[10], mux_2level_tapbuf_size40_1_sram[9], mux_2level_tapbuf_size40_1_sram[8], mux_2level_tapbuf_size40_1_sram[7], mux_2level_tapbuf_size40_1_sram[6], mux_2level_tapbuf_size40_1_sram[5], mux_2level_tapbuf_size40_1_sram[4], mux_2level_tapbuf_size40_1_sram[3], mux_2level_tapbuf_size40_1_sram[2], mux_2level_tapbuf_size40_1_sram[1], mux_2level_tapbuf_size40_1_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_1_sram_inv[13], mux_2level_tapbuf_size40_1_sram_inv[12], mux_2level_tapbuf_size40_1_sram_inv[11], mux_2level_tapbuf_size40_1_sram_inv[10], mux_2level_tapbuf_size40_1_sram_inv[9], mux_2level_tapbuf_size40_1_sram_inv[8], mux_2level_tapbuf_size40_1_sram_inv[7], mux_2level_tapbuf_size40_1_sram_inv[6], mux_2level_tapbuf_size40_1_sram_inv[5], mux_2level_tapbuf_size40_1_sram_inv[4], mux_2level_tapbuf_size40_1_sram_inv[3], mux_2level_tapbuf_size40_1_sram_inv[2], mux_2level_tapbuf_size40_1_sram_inv[1], mux_2level_tapbuf_size40_1_sram_inv[0]}),
		.out(chany_bottom_out[4]));

	mux_2level_tapbuf_size40 mux_bottom_track_17 (
		.in({bottom_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_2_sram[13], mux_2level_tapbuf_size40_2_sram[12], mux_2level_tapbuf_size40_2_sram[11], mux_2level_tapbuf_size40_2_sram[10], mux_2level_tapbuf_size40_2_sram[9], mux_2level_tapbuf_size40_2_sram[8], mux_2level_tapbuf_size40_2_sram[7], mux_2level_tapbuf_size40_2_sram[6], mux_2level_tapbuf_size40_2_sram[5], mux_2level_tapbuf_size40_2_sram[4], mux_2level_tapbuf_size40_2_sram[3], mux_2level_tapbuf_size40_2_sram[2], mux_2level_tapbuf_size40_2_sram[1], mux_2level_tapbuf_size40_2_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_2_sram_inv[13], mux_2level_tapbuf_size40_2_sram_inv[12], mux_2level_tapbuf_size40_2_sram_inv[11], mux_2level_tapbuf_size40_2_sram_inv[10], mux_2level_tapbuf_size40_2_sram_inv[9], mux_2level_tapbuf_size40_2_sram_inv[8], mux_2level_tapbuf_size40_2_sram_inv[7], mux_2level_tapbuf_size40_2_sram_inv[6], mux_2level_tapbuf_size40_2_sram_inv[5], mux_2level_tapbuf_size40_2_sram_inv[4], mux_2level_tapbuf_size40_2_sram_inv[3], mux_2level_tapbuf_size40_2_sram_inv[2], mux_2level_tapbuf_size40_2_sram_inv[1], mux_2level_tapbuf_size40_2_sram_inv[0]}),
		.out(chany_bottom_out[8]));

	mux_2level_tapbuf_size40 mux_bottom_track_25 (
		.in({bottom_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, bottom_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, chanx_right_in[15], chanx_right_in[14], chanx_right_in[13], chanx_right_in[12], chanx_right_in[11], chanx_right_in[10], chanx_right_in[9], chanx_right_in[8], chanx_right_in[7], chanx_right_in[6], chanx_right_in[5], chanx_right_in[4], chanx_right_in[3], chanx_right_in[2], chanx_right_in[1], chanx_right_in[0], chany_top_in[15], chany_top_in[14], chany_top_in[13], chany_top_in[12], chany_top_in[11], chany_top_in[10], chany_top_in[9], chany_top_in[8], chany_top_in[7], chany_top_in[6], chany_top_in[5], chany_top_in[4], chany_top_in[3], chany_top_in[2], chany_top_in[1], chany_top_in[0]}),
		.sram({mux_2level_tapbuf_size40_3_sram[13], mux_2level_tapbuf_size40_3_sram[12], mux_2level_tapbuf_size40_3_sram[11], mux_2level_tapbuf_size40_3_sram[10], mux_2level_tapbuf_size40_3_sram[9], mux_2level_tapbuf_size40_3_sram[8], mux_2level_tapbuf_size40_3_sram[7], mux_2level_tapbuf_size40_3_sram[6], mux_2level_tapbuf_size40_3_sram[5], mux_2level_tapbuf_size40_3_sram[4], mux_2level_tapbuf_size40_3_sram[3], mux_2level_tapbuf_size40_3_sram[2], mux_2level_tapbuf_size40_3_sram[1], mux_2level_tapbuf_size40_3_sram[0]}),
		.sram_inv({mux_2level_tapbuf_size40_3_sram_inv[13], mux_2level_tapbuf_size40_3_sram_inv[12], mux_2level_tapbuf_size40_3_sram_inv[11], mux_2level_tapbuf_size40_3_sram_inv[10], mux_2level_tapbuf_size40_3_sram_inv[9], mux_2level_tapbuf_size40_3_sram_inv[8], mux_2level_tapbuf_size40_3_sram_inv[7], mux_2level_tapbuf_size40_3_sram_inv[6], mux_2level_tapbuf_size40_3_sram_inv[5], mux_2level_tapbuf_size40_3_sram_inv[4], mux_2level_tapbuf_size40_3_sram_inv[3], mux_2level_tapbuf_size40_3_sram_inv[2], mux_2level_tapbuf_size40_3_sram_inv[1], mux_2level_tapbuf_size40_3_sram_inv[0]}),
		.out(chany_bottom_out[12]));

	mux_2level_tapbuf_size40_feedthrough_mem feedthrough_mem_bottom_track_1 (
		.feedthrough_mem_in({sb_6__config_group_mem_size56_0_mem_out[13], sb_6__config_group_mem_size56_0_mem_out[12], sb_6__config_group_mem_size56_0_mem_out[11], sb_6__config_group_mem_size56_0_mem_out[10], sb_6__config_group_mem_size56_0_mem_out[9], sb_6__config_group_mem_size56_0_mem_out[8], sb_6__config_group_mem_size56_0_mem_out[7], sb_6__config_group_mem_size56_0_mem_out[6], sb_6__config_group_mem_size56_0_mem_out[5], sb_6__config_group_mem_size56_0_mem_out[4], sb_6__config_group_mem_size56_0_mem_out[3], sb_6__config_group_mem_size56_0_mem_out[2], sb_6__config_group_mem_size56_0_mem_out[1], sb_6__config_group_mem_size56_0_mem_out[0]}),
		.feedthrough_mem_inb({sb_6__config_group_mem_size56_0_mem_outb[13], sb_6__config_group_mem_size56_0_mem_outb[12], sb_6__config_group_mem_size56_0_mem_outb[11], sb_6__config_group_mem_size56_0_mem_outb[10], sb_6__config_group_mem_size56_0_mem_outb[9], sb_6__config_group_mem_size56_0_mem_outb[8], sb_6__config_group_mem_size56_0_mem_outb[7], sb_6__config_group_mem_size56_0_mem_outb[6], sb_6__config_group_mem_size56_0_mem_outb[5], sb_6__config_group_mem_size56_0_mem_outb[4], sb_6__config_group_mem_size56_0_mem_outb[3], sb_6__config_group_mem_size56_0_mem_outb[2], sb_6__config_group_mem_size56_0_mem_outb[1], sb_6__config_group_mem_size56_0_mem_outb[0]}),
		.mem_out({mux_2level_tapbuf_size40_0_sram[13], mux_2level_tapbuf_size40_0_sram[12], mux_2level_tapbuf_size40_0_sram[11], mux_2level_tapbuf_size40_0_sram[10], mux_2level_tapbuf_size40_0_sram[9], mux_2level_tapbuf_size40_0_sram[8], mux_2level_tapbuf_size40_0_sram[7], mux_2level_tapbuf_size40_0_sram[6], mux_2level_tapbuf_size40_0_sram[5], mux_2level_tapbuf_size40_0_sram[4], mux_2level_tapbuf_size40_0_sram[3], mux_2level_tapbuf_size40_0_sram[2], mux_2level_tapbuf_size40_0_sram[1], mux_2level_tapbuf_size40_0_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_0_sram_inv[13], mux_2level_tapbuf_size40_0_sram_inv[12], mux_2level_tapbuf_size40_0_sram_inv[11], mux_2level_tapbuf_size40_0_sram_inv[10], mux_2level_tapbuf_size40_0_sram_inv[9], mux_2level_tapbuf_size40_0_sram_inv[8], mux_2level_tapbuf_size40_0_sram_inv[7], mux_2level_tapbuf_size40_0_sram_inv[6], mux_2level_tapbuf_size40_0_sram_inv[5], mux_2level_tapbuf_size40_0_sram_inv[4], mux_2level_tapbuf_size40_0_sram_inv[3], mux_2level_tapbuf_size40_0_sram_inv[2], mux_2level_tapbuf_size40_0_sram_inv[1], mux_2level_tapbuf_size40_0_sram_inv[0]}));

	mux_2level_tapbuf_size40_feedthrough_mem feedthrough_mem_bottom_track_9 (
		.feedthrough_mem_in({sb_6__config_group_mem_size56_0_mem_out[27], sb_6__config_group_mem_size56_0_mem_out[26], sb_6__config_group_mem_size56_0_mem_out[25], sb_6__config_group_mem_size56_0_mem_out[24], sb_6__config_group_mem_size56_0_mem_out[23], sb_6__config_group_mem_size56_0_mem_out[22], sb_6__config_group_mem_size56_0_mem_out[21], sb_6__config_group_mem_size56_0_mem_out[20], sb_6__config_group_mem_size56_0_mem_out[19], sb_6__config_group_mem_size56_0_mem_out[18], sb_6__config_group_mem_size56_0_mem_out[17], sb_6__config_group_mem_size56_0_mem_out[16], sb_6__config_group_mem_size56_0_mem_out[15], sb_6__config_group_mem_size56_0_mem_out[14]}),
		.feedthrough_mem_inb({sb_6__config_group_mem_size56_0_mem_outb[27], sb_6__config_group_mem_size56_0_mem_outb[26], sb_6__config_group_mem_size56_0_mem_outb[25], sb_6__config_group_mem_size56_0_mem_outb[24], sb_6__config_group_mem_size56_0_mem_outb[23], sb_6__config_group_mem_size56_0_mem_outb[22], sb_6__config_group_mem_size56_0_mem_outb[21], sb_6__config_group_mem_size56_0_mem_outb[20], sb_6__config_group_mem_size56_0_mem_outb[19], sb_6__config_group_mem_size56_0_mem_outb[18], sb_6__config_group_mem_size56_0_mem_outb[17], sb_6__config_group_mem_size56_0_mem_outb[16], sb_6__config_group_mem_size56_0_mem_outb[15], sb_6__config_group_mem_size56_0_mem_outb[14]}),
		.mem_out({mux_2level_tapbuf_size40_1_sram[13], mux_2level_tapbuf_size40_1_sram[12], mux_2level_tapbuf_size40_1_sram[11], mux_2level_tapbuf_size40_1_sram[10], mux_2level_tapbuf_size40_1_sram[9], mux_2level_tapbuf_size40_1_sram[8], mux_2level_tapbuf_size40_1_sram[7], mux_2level_tapbuf_size40_1_sram[6], mux_2level_tapbuf_size40_1_sram[5], mux_2level_tapbuf_size40_1_sram[4], mux_2level_tapbuf_size40_1_sram[3], mux_2level_tapbuf_size40_1_sram[2], mux_2level_tapbuf_size40_1_sram[1], mux_2level_tapbuf_size40_1_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_1_sram_inv[13], mux_2level_tapbuf_size40_1_sram_inv[12], mux_2level_tapbuf_size40_1_sram_inv[11], mux_2level_tapbuf_size40_1_sram_inv[10], mux_2level_tapbuf_size40_1_sram_inv[9], mux_2level_tapbuf_size40_1_sram_inv[8], mux_2level_tapbuf_size40_1_sram_inv[7], mux_2level_tapbuf_size40_1_sram_inv[6], mux_2level_tapbuf_size40_1_sram_inv[5], mux_2level_tapbuf_size40_1_sram_inv[4], mux_2level_tapbuf_size40_1_sram_inv[3], mux_2level_tapbuf_size40_1_sram_inv[2], mux_2level_tapbuf_size40_1_sram_inv[1], mux_2level_tapbuf_size40_1_sram_inv[0]}));

	mux_2level_tapbuf_size40_feedthrough_mem feedthrough_mem_bottom_track_17 (
		.feedthrough_mem_in({sb_6__config_group_mem_size56_0_mem_out[41], sb_6__config_group_mem_size56_0_mem_out[40], sb_6__config_group_mem_size56_0_mem_out[39], sb_6__config_group_mem_size56_0_mem_out[38], sb_6__config_group_mem_size56_0_mem_out[37], sb_6__config_group_mem_size56_0_mem_out[36], sb_6__config_group_mem_size56_0_mem_out[35], sb_6__config_group_mem_size56_0_mem_out[34], sb_6__config_group_mem_size56_0_mem_out[33], sb_6__config_group_mem_size56_0_mem_out[32], sb_6__config_group_mem_size56_0_mem_out[31], sb_6__config_group_mem_size56_0_mem_out[30], sb_6__config_group_mem_size56_0_mem_out[29], sb_6__config_group_mem_size56_0_mem_out[28]}),
		.feedthrough_mem_inb({sb_6__config_group_mem_size56_0_mem_outb[41], sb_6__config_group_mem_size56_0_mem_outb[40], sb_6__config_group_mem_size56_0_mem_outb[39], sb_6__config_group_mem_size56_0_mem_outb[38], sb_6__config_group_mem_size56_0_mem_outb[37], sb_6__config_group_mem_size56_0_mem_outb[36], sb_6__config_group_mem_size56_0_mem_outb[35], sb_6__config_group_mem_size56_0_mem_outb[34], sb_6__config_group_mem_size56_0_mem_outb[33], sb_6__config_group_mem_size56_0_mem_outb[32], sb_6__config_group_mem_size56_0_mem_outb[31], sb_6__config_group_mem_size56_0_mem_outb[30], sb_6__config_group_mem_size56_0_mem_outb[29], sb_6__config_group_mem_size56_0_mem_outb[28]}),
		.mem_out({mux_2level_tapbuf_size40_2_sram[13], mux_2level_tapbuf_size40_2_sram[12], mux_2level_tapbuf_size40_2_sram[11], mux_2level_tapbuf_size40_2_sram[10], mux_2level_tapbuf_size40_2_sram[9], mux_2level_tapbuf_size40_2_sram[8], mux_2level_tapbuf_size40_2_sram[7], mux_2level_tapbuf_size40_2_sram[6], mux_2level_tapbuf_size40_2_sram[5], mux_2level_tapbuf_size40_2_sram[4], mux_2level_tapbuf_size40_2_sram[3], mux_2level_tapbuf_size40_2_sram[2], mux_2level_tapbuf_size40_2_sram[1], mux_2level_tapbuf_size40_2_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_2_sram_inv[13], mux_2level_tapbuf_size40_2_sram_inv[12], mux_2level_tapbuf_size40_2_sram_inv[11], mux_2level_tapbuf_size40_2_sram_inv[10], mux_2level_tapbuf_size40_2_sram_inv[9], mux_2level_tapbuf_size40_2_sram_inv[8], mux_2level_tapbuf_size40_2_sram_inv[7], mux_2level_tapbuf_size40_2_sram_inv[6], mux_2level_tapbuf_size40_2_sram_inv[5], mux_2level_tapbuf_size40_2_sram_inv[4], mux_2level_tapbuf_size40_2_sram_inv[3], mux_2level_tapbuf_size40_2_sram_inv[2], mux_2level_tapbuf_size40_2_sram_inv[1], mux_2level_tapbuf_size40_2_sram_inv[0]}));

	mux_2level_tapbuf_size40_feedthrough_mem feedthrough_mem_bottom_track_25 (
		.feedthrough_mem_in({sb_6__config_group_mem_size56_0_mem_out[55], sb_6__config_group_mem_size56_0_mem_out[54], sb_6__config_group_mem_size56_0_mem_out[53], sb_6__config_group_mem_size56_0_mem_out[52], sb_6__config_group_mem_size56_0_mem_out[51], sb_6__config_group_mem_size56_0_mem_out[50], sb_6__config_group_mem_size56_0_mem_out[49], sb_6__config_group_mem_size56_0_mem_out[48], sb_6__config_group_mem_size56_0_mem_out[47], sb_6__config_group_mem_size56_0_mem_out[46], sb_6__config_group_mem_size56_0_mem_out[45], sb_6__config_group_mem_size56_0_mem_out[44], sb_6__config_group_mem_size56_0_mem_out[43], sb_6__config_group_mem_size56_0_mem_out[42]}),
		.feedthrough_mem_inb({sb_6__config_group_mem_size56_0_mem_outb[55], sb_6__config_group_mem_size56_0_mem_outb[54], sb_6__config_group_mem_size56_0_mem_outb[53], sb_6__config_group_mem_size56_0_mem_outb[52], sb_6__config_group_mem_size56_0_mem_outb[51], sb_6__config_group_mem_size56_0_mem_outb[50], sb_6__config_group_mem_size56_0_mem_outb[49], sb_6__config_group_mem_size56_0_mem_outb[48], sb_6__config_group_mem_size56_0_mem_outb[47], sb_6__config_group_mem_size56_0_mem_outb[46], sb_6__config_group_mem_size56_0_mem_outb[45], sb_6__config_group_mem_size56_0_mem_outb[44], sb_6__config_group_mem_size56_0_mem_outb[43], sb_6__config_group_mem_size56_0_mem_outb[42]}),
		.mem_out({mux_2level_tapbuf_size40_3_sram[13], mux_2level_tapbuf_size40_3_sram[12], mux_2level_tapbuf_size40_3_sram[11], mux_2level_tapbuf_size40_3_sram[10], mux_2level_tapbuf_size40_3_sram[9], mux_2level_tapbuf_size40_3_sram[8], mux_2level_tapbuf_size40_3_sram[7], mux_2level_tapbuf_size40_3_sram[6], mux_2level_tapbuf_size40_3_sram[5], mux_2level_tapbuf_size40_3_sram[4], mux_2level_tapbuf_size40_3_sram[3], mux_2level_tapbuf_size40_3_sram[2], mux_2level_tapbuf_size40_3_sram[1], mux_2level_tapbuf_size40_3_sram[0]}),
		.mem_outb({mux_2level_tapbuf_size40_3_sram_inv[13], mux_2level_tapbuf_size40_3_sram_inv[12], mux_2level_tapbuf_size40_3_sram_inv[11], mux_2level_tapbuf_size40_3_sram_inv[10], mux_2level_tapbuf_size40_3_sram_inv[9], mux_2level_tapbuf_size40_3_sram_inv[8], mux_2level_tapbuf_size40_3_sram_inv[7], mux_2level_tapbuf_size40_3_sram_inv[6], mux_2level_tapbuf_size40_3_sram_inv[5], mux_2level_tapbuf_size40_3_sram_inv[4], mux_2level_tapbuf_size40_3_sram_inv[3], mux_2level_tapbuf_size40_3_sram_inv[2], mux_2level_tapbuf_size40_3_sram_inv[1], mux_2level_tapbuf_size40_3_sram_inv[0]}));

	sb_6__config_group_mem_size56 sb_6__config_group_mem_size56 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.mem_out({sb_6__config_group_mem_size56_0_mem_out[55], sb_6__config_group_mem_size56_0_mem_out[54], sb_6__config_group_mem_size56_0_mem_out[53], sb_6__config_group_mem_size56_0_mem_out[52], sb_6__config_group_mem_size56_0_mem_out[51], sb_6__config_group_mem_size56_0_mem_out[50], sb_6__config_group_mem_size56_0_mem_out[49], sb_6__config_group_mem_size56_0_mem_out[48], sb_6__config_group_mem_size56_0_mem_out[47], sb_6__config_group_mem_size56_0_mem_out[46], sb_6__config_group_mem_size56_0_mem_out[45], sb_6__config_group_mem_size56_0_mem_out[44], sb_6__config_group_mem_size56_0_mem_out[43], sb_6__config_group_mem_size56_0_mem_out[42], sb_6__config_group_mem_size56_0_mem_out[41], sb_6__config_group_mem_size56_0_mem_out[40], sb_6__config_group_mem_size56_0_mem_out[39], sb_6__config_group_mem_size56_0_mem_out[38], sb_6__config_group_mem_size56_0_mem_out[37], sb_6__config_group_mem_size56_0_mem_out[36], sb_6__config_group_mem_size56_0_mem_out[35], sb_6__config_group_mem_size56_0_mem_out[34], sb_6__config_group_mem_size56_0_mem_out[33], sb_6__config_group_mem_size56_0_mem_out[32], sb_6__config_group_mem_size56_0_mem_out[31], sb_6__config_group_mem_size56_0_mem_out[30], sb_6__config_group_mem_size56_0_mem_out[29], sb_6__config_group_mem_size56_0_mem_out[28], sb_6__config_group_mem_size56_0_mem_out[27], sb_6__config_group_mem_size56_0_mem_out[26], sb_6__config_group_mem_size56_0_mem_out[25], sb_6__config_group_mem_size56_0_mem_out[24], sb_6__config_group_mem_size56_0_mem_out[23], sb_6__config_group_mem_size56_0_mem_out[22], sb_6__config_group_mem_size56_0_mem_out[21], sb_6__config_group_mem_size56_0_mem_out[20], sb_6__config_group_mem_size56_0_mem_out[19], sb_6__config_group_mem_size56_0_mem_out[18], sb_6__config_group_mem_size56_0_mem_out[17], sb_6__config_group_mem_size56_0_mem_out[16], sb_6__config_group_mem_size56_0_mem_out[15], sb_6__config_group_mem_size56_0_mem_out[14], sb_6__config_group_mem_size56_0_mem_out[13], sb_6__config_group_mem_size56_0_mem_out[12], sb_6__config_group_mem_size56_0_mem_out[11], sb_6__config_group_mem_size56_0_mem_out[10], sb_6__config_group_mem_size56_0_mem_out[9], sb_6__config_group_mem_size56_0_mem_out[8], sb_6__config_group_mem_size56_0_mem_out[7], sb_6__config_group_mem_size56_0_mem_out[6], sb_6__config_group_mem_size56_0_mem_out[5], sb_6__config_group_mem_size56_0_mem_out[4], sb_6__config_group_mem_size56_0_mem_out[3], sb_6__config_group_mem_size56_0_mem_out[2], sb_6__config_group_mem_size56_0_mem_out[1], sb_6__config_group_mem_size56_0_mem_out[0]}),
		.mem_outb({sb_6__config_group_mem_size56_0_mem_outb[55], sb_6__config_group_mem_size56_0_mem_outb[54], sb_6__config_group_mem_size56_0_mem_outb[53], sb_6__config_group_mem_size56_0_mem_outb[52], sb_6__config_group_mem_size56_0_mem_outb[51], sb_6__config_group_mem_size56_0_mem_outb[50], sb_6__config_group_mem_size56_0_mem_outb[49], sb_6__config_group_mem_size56_0_mem_outb[48], sb_6__config_group_mem_size56_0_mem_outb[47], sb_6__config_group_mem_size56_0_mem_outb[46], sb_6__config_group_mem_size56_0_mem_outb[45], sb_6__config_group_mem_size56_0_mem_outb[44], sb_6__config_group_mem_size56_0_mem_outb[43], sb_6__config_group_mem_size56_0_mem_outb[42], sb_6__config_group_mem_size56_0_mem_outb[41], sb_6__config_group_mem_size56_0_mem_outb[40], sb_6__config_group_mem_size56_0_mem_outb[39], sb_6__config_group_mem_size56_0_mem_outb[38], sb_6__config_group_mem_size56_0_mem_outb[37], sb_6__config_group_mem_size56_0_mem_outb[36], sb_6__config_group_mem_size56_0_mem_outb[35], sb_6__config_group_mem_size56_0_mem_outb[34], sb_6__config_group_mem_size56_0_mem_outb[33], sb_6__config_group_mem_size56_0_mem_outb[32], sb_6__config_group_mem_size56_0_mem_outb[31], sb_6__config_group_mem_size56_0_mem_outb[30], sb_6__config_group_mem_size56_0_mem_outb[29], sb_6__config_group_mem_size56_0_mem_outb[28], sb_6__config_group_mem_size56_0_mem_outb[27], sb_6__config_group_mem_size56_0_mem_outb[26], sb_6__config_group_mem_size56_0_mem_outb[25], sb_6__config_group_mem_size56_0_mem_outb[24], sb_6__config_group_mem_size56_0_mem_outb[23], sb_6__config_group_mem_size56_0_mem_outb[22], sb_6__config_group_mem_size56_0_mem_outb[21], sb_6__config_group_mem_size56_0_mem_outb[20], sb_6__config_group_mem_size56_0_mem_outb[19], sb_6__config_group_mem_size56_0_mem_outb[18], sb_6__config_group_mem_size56_0_mem_outb[17], sb_6__config_group_mem_size56_0_mem_outb[16], sb_6__config_group_mem_size56_0_mem_outb[15], sb_6__config_group_mem_size56_0_mem_outb[14], sb_6__config_group_mem_size56_0_mem_outb[13], sb_6__config_group_mem_size56_0_mem_outb[12], sb_6__config_group_mem_size56_0_mem_outb[11], sb_6__config_group_mem_size56_0_mem_outb[10], sb_6__config_group_mem_size56_0_mem_outb[9], sb_6__config_group_mem_size56_0_mem_outb[8], sb_6__config_group_mem_size56_0_mem_outb[7], sb_6__config_group_mem_size56_0_mem_outb[6], sb_6__config_group_mem_size56_0_mem_outb[5], sb_6__config_group_mem_size56_0_mem_outb[4], sb_6__config_group_mem_size56_0_mem_outb[3], sb_6__config_group_mem_size56_0_mem_outb[2], sb_6__config_group_mem_size56_0_mem_outb[1], sb_6__config_group_mem_size56_0_mem_outb[0]}),
		.ccff_tail(ccff_tail));

endmodule
// ----- END Verilog module for sb_1__5_ -----

//----- Default net type -----
`default_nettype wire



