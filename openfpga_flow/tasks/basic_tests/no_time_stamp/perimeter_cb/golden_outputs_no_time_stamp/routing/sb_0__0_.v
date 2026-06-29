//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[0][0]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_0__0_ -----
module sb_0__0_(pReset,
                prog_clk,
                chany_top_in,
                top_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_,
                top_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_,
                top_right_grid_left_width_0_height_0_subtile_1__pin_out_0_,
                top_right_grid_left_width_0_height_0_subtile_2__pin_out_0_,
                chanx_right_in,
                right_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_,
                right_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_,
                chany_bottom_in,
                bottom_right_grid_left_width_0_height_0_subtile_0__pin_inpad_0_,
                bottom_right_grid_left_width_0_height_0_subtile_1__pin_inpad_0_,
                bottom_right_grid_left_width_0_height_0_subtile_2__pin_inpad_0_,
                bottom_right_grid_left_width_0_height_0_subtile_3__pin_inpad_0_,
                bottom_right_grid_left_width_0_height_0_subtile_4__pin_inpad_0_,
                bottom_right_grid_left_width_0_height_0_subtile_5__pin_inpad_0_,
                bottom_right_grid_left_width_0_height_0_subtile_6__pin_inpad_0_,
                bottom_right_grid_left_width_0_height_0_subtile_7__pin_inpad_0_,
                chanx_left_in,
                left_top_grid_bottom_width_0_height_0_subtile_0__pin_inpad_0_,
                left_top_grid_bottom_width_0_height_0_subtile_1__pin_inpad_0_,
                left_top_grid_bottom_width_0_height_0_subtile_2__pin_inpad_0_,
                left_top_grid_bottom_width_0_height_0_subtile_3__pin_inpad_0_,
                left_top_grid_bottom_width_0_height_0_subtile_4__pin_inpad_0_,
                left_top_grid_bottom_width_0_height_0_subtile_5__pin_inpad_0_,
                left_top_grid_bottom_width_0_height_0_subtile_6__pin_inpad_0_,
                left_top_grid_bottom_width_0_height_0_subtile_7__pin_inpad_0_,
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
input [0:18] chany_top_in;
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
input [0:0] top_right_grid_left_width_0_height_0_subtile_1__pin_out_0_;
//----- INPUT PORTS -----
input [0:0] top_right_grid_left_width_0_height_0_subtile_2__pin_out_0_;
//----- INPUT PORTS -----
input [0:18] chanx_right_in;
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
input [0:18] chany_bottom_in;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_0__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_1__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_2__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_3__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_4__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_5__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_6__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] bottom_right_grid_left_width_0_height_0_subtile_7__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:18] chanx_left_in;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_0__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_1__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_2__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_3__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_4__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_5__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_6__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] left_top_grid_bottom_width_0_height_0_subtile_7__pin_inpad_0_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:18] chany_top_out;
//----- OUTPUT PORTS -----
output [0:18] chanx_right_out;
//----- OUTPUT PORTS -----
output [0:18] chany_bottom_out;
//----- OUTPUT PORTS -----
output [0:18] chanx_left_out;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] mux_2level_tapbuf_size3_0_sram;
wire [0:1] mux_2level_tapbuf_size3_0_sram_inv;
wire [0:1] mux_2level_tapbuf_size3_1_sram;
wire [0:1] mux_2level_tapbuf_size3_1_sram_inv;
wire [0:1] mux_2level_tapbuf_size3_2_sram;
wire [0:1] mux_2level_tapbuf_size3_2_sram_inv;
wire [0:1] mux_2level_tapbuf_size3_3_sram;
wire [0:1] mux_2level_tapbuf_size3_3_sram_inv;
wire [0:1] mux_2level_tapbuf_size3_4_sram;
wire [0:1] mux_2level_tapbuf_size3_4_sram_inv;
wire [0:1] mux_2level_tapbuf_size3_5_sram;
wire [0:1] mux_2level_tapbuf_size3_5_sram_inv;
wire [0:0] mux_2level_tapbuf_size3_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size3_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size3_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size3_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size3_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size3_mem_5_ccff_tail;
wire [0:5] mux_2level_tapbuf_size5_0_sram;
wire [0:5] mux_2level_tapbuf_size5_0_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_10_sram;
wire [0:5] mux_2level_tapbuf_size5_10_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_11_sram;
wire [0:5] mux_2level_tapbuf_size5_11_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_12_sram;
wire [0:5] mux_2level_tapbuf_size5_12_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_13_sram;
wire [0:5] mux_2level_tapbuf_size5_13_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_14_sram;
wire [0:5] mux_2level_tapbuf_size5_14_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_15_sram;
wire [0:5] mux_2level_tapbuf_size5_15_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_16_sram;
wire [0:5] mux_2level_tapbuf_size5_16_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_17_sram;
wire [0:5] mux_2level_tapbuf_size5_17_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_18_sram;
wire [0:5] mux_2level_tapbuf_size5_18_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_19_sram;
wire [0:5] mux_2level_tapbuf_size5_19_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_1_sram;
wire [0:5] mux_2level_tapbuf_size5_1_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_20_sram;
wire [0:5] mux_2level_tapbuf_size5_20_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_21_sram;
wire [0:5] mux_2level_tapbuf_size5_21_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_22_sram;
wire [0:5] mux_2level_tapbuf_size5_22_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_23_sram;
wire [0:5] mux_2level_tapbuf_size5_23_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_24_sram;
wire [0:5] mux_2level_tapbuf_size5_24_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_25_sram;
wire [0:5] mux_2level_tapbuf_size5_25_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_26_sram;
wire [0:5] mux_2level_tapbuf_size5_26_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_27_sram;
wire [0:5] mux_2level_tapbuf_size5_27_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_28_sram;
wire [0:5] mux_2level_tapbuf_size5_28_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_29_sram;
wire [0:5] mux_2level_tapbuf_size5_29_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_2_sram;
wire [0:5] mux_2level_tapbuf_size5_2_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_30_sram;
wire [0:5] mux_2level_tapbuf_size5_30_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_31_sram;
wire [0:5] mux_2level_tapbuf_size5_31_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_3_sram;
wire [0:5] mux_2level_tapbuf_size5_3_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_4_sram;
wire [0:5] mux_2level_tapbuf_size5_4_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_5_sram;
wire [0:5] mux_2level_tapbuf_size5_5_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_6_sram;
wire [0:5] mux_2level_tapbuf_size5_6_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_7_sram;
wire [0:5] mux_2level_tapbuf_size5_7_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_8_sram;
wire [0:5] mux_2level_tapbuf_size5_8_sram_inv;
wire [0:5] mux_2level_tapbuf_size5_9_sram;
wire [0:5] mux_2level_tapbuf_size5_9_sram_inv;
wire [0:0] mux_2level_tapbuf_size5_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_10_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_11_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_12_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_13_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_14_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_15_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_16_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_17_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_18_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_19_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_20_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_21_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_22_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_23_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_24_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_25_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_26_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_27_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_28_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_29_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_30_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_5_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_6_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_7_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_8_ccff_tail;
wire [0:0] mux_2level_tapbuf_size5_mem_9_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[1] = chany_top_in[0];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[3] = chany_top_in[2];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[5] = chany_top_in[4];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[7] = chany_top_in[6];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[9] = chany_top_in[8];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[11] = chany_top_in[10];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[13] = chany_top_in[12];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chany_bottom_out[15] = chany_top_in[14];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[1] = chanx_right_in[0];
// ----- Local connection due to Wire 31 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[3] = chanx_right_in[2];
// ----- Local connection due to Wire 33 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[5] = chanx_right_in[4];
// ----- Local connection due to Wire 35 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[7] = chanx_right_in[6];
// ----- Local connection due to Wire 37 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[9] = chanx_right_in[8];
// ----- Local connection due to Wire 39 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[11] = chanx_right_in[10];
// ----- Local connection due to Wire 41 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[13] = chanx_right_in[12];
// ----- Local connection due to Wire 43 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[15] = chanx_right_in[14];
// ----- Local connection due to Wire 45 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[16] = chanx_right_in[16];
// ----- Local connection due to Wire 46 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[17] = chanx_right_in[17];
// ----- Local connection due to Wire 47 -----
// ----- Net source id 0 -----
// ----- Net sink id 2 -----
	assign chanx_left_out[18] = chanx_right_in[18];
// ----- Local connection due to Wire 56 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[1] = chany_bottom_in[0];
// ----- Local connection due to Wire 58 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[3] = chany_bottom_in[2];
// ----- Local connection due to Wire 60 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[5] = chany_bottom_in[4];
// ----- Local connection due to Wire 62 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[7] = chany_bottom_in[6];
// ----- Local connection due to Wire 64 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[9] = chany_bottom_in[8];
// ----- Local connection due to Wire 66 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[11] = chany_bottom_in[10];
// ----- Local connection due to Wire 68 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[13] = chany_bottom_in[12];
// ----- Local connection due to Wire 70 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_top_out[15] = chany_bottom_in[14];
// ----- Local connection due to Wire 83 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[1] = chanx_left_in[0];
// ----- Local connection due to Wire 85 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[3] = chanx_left_in[2];
// ----- Local connection due to Wire 87 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[5] = chanx_left_in[4];
// ----- Local connection due to Wire 89 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[7] = chanx_left_in[6];
// ----- Local connection due to Wire 91 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[9] = chanx_left_in[8];
// ----- Local connection due to Wire 93 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[11] = chanx_left_in[10];
// ----- Local connection due to Wire 95 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[13] = chanx_left_in[12];
// ----- Local connection due to Wire 97 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[15] = chanx_left_in[14];
// ----- Local connection due to Wire 99 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[16] = chanx_left_in[16];
// ----- Local connection due to Wire 100 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[17] = chanx_left_in[17];
// ----- Local connection due to Wire 101 -----
// ----- Net source id 0 -----
// ----- Net sink id 1 -----
	assign chanx_right_out[18] = chanx_left_in[18];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size5 mux_top_track_0 (
		.in({top_left_grid_right_width_0_height_0_subtile_0__pin_inpad_0_, chanx_right_in[2:3], chanx_left_in[0:1]}),
		.sram(mux_2level_tapbuf_size5_0_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_0_sram_inv[0:5]),
		.out(chany_top_out[0]));

	mux_2level_tapbuf_size5 mux_top_track_4 (
		.in({top_left_grid_right_width_0_height_0_subtile_1__pin_inpad_0_, chanx_right_in[4:5], chanx_left_in[14:15]}),
		.sram(mux_2level_tapbuf_size5_1_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_1_sram_inv[0:5]),
		.out(chany_top_out[2]));

	mux_2level_tapbuf_size5 mux_top_track_8 (
		.in({top_left_grid_right_width_0_height_0_subtile_2__pin_inpad_0_, chanx_right_in[6:7], chanx_left_in[12:13]}),
		.sram(mux_2level_tapbuf_size5_2_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_2_sram_inv[0:5]),
		.out(chany_top_out[4]));

	mux_2level_tapbuf_size5 mux_top_track_12 (
		.in({top_left_grid_right_width_0_height_0_subtile_3__pin_inpad_0_, chanx_right_in[8:9], chanx_left_in[10:11]}),
		.sram(mux_2level_tapbuf_size5_3_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_3_sram_inv[0:5]),
		.out(chany_top_out[6]));

	mux_2level_tapbuf_size5 mux_top_track_16 (
		.in({top_left_grid_right_width_0_height_0_subtile_4__pin_inpad_0_, chanx_right_in[10:11], chanx_left_in[8:9]}),
		.sram(mux_2level_tapbuf_size5_4_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_4_sram_inv[0:5]),
		.out(chany_top_out[8]));

	mux_2level_tapbuf_size5 mux_top_track_20 (
		.in({top_left_grid_right_width_0_height_0_subtile_5__pin_inpad_0_, chanx_right_in[12:13], chanx_left_in[6:7]}),
		.sram(mux_2level_tapbuf_size5_5_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_5_sram_inv[0:5]),
		.out(chany_top_out[10]));

	mux_2level_tapbuf_size5 mux_top_track_24 (
		.in({top_left_grid_right_width_0_height_0_subtile_6__pin_inpad_0_, chanx_right_in[14:15], chanx_left_in[4:5]}),
		.sram(mux_2level_tapbuf_size5_6_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_6_sram_inv[0:5]),
		.out(chany_top_out[12]));

	mux_2level_tapbuf_size5 mux_top_track_28 (
		.in({top_left_grid_right_width_0_height_0_subtile_7__pin_inpad_0_, chanx_right_in[0:1], chanx_left_in[2:3]}),
		.sram(mux_2level_tapbuf_size5_7_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_7_sram_inv[0:5]),
		.out(chany_top_out[14]));

	mux_2level_tapbuf_size5 mux_right_track_0 (
		.in({chany_top_in[14:15], right_bottom_grid_top_width_0_height_0_subtile_0__pin_inpad_0_, chany_bottom_in[12:13]}),
		.sram(mux_2level_tapbuf_size5_8_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_8_sram_inv[0:5]),
		.out(chanx_right_out[0]));

	mux_2level_tapbuf_size5 mux_right_track_4 (
		.in({chany_top_in[0:1], right_bottom_grid_top_width_0_height_0_subtile_1__pin_inpad_0_, chany_bottom_in[10:11]}),
		.sram(mux_2level_tapbuf_size5_9_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_9_sram_inv[0:5]),
		.out(chanx_right_out[2]));

	mux_2level_tapbuf_size5 mux_right_track_8 (
		.in({chany_top_in[2:3], right_bottom_grid_top_width_0_height_0_subtile_2__pin_inpad_0_, chany_bottom_in[8:9]}),
		.sram(mux_2level_tapbuf_size5_10_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_10_sram_inv[0:5]),
		.out(chanx_right_out[4]));

	mux_2level_tapbuf_size5 mux_right_track_12 (
		.in({chany_top_in[4:5], right_bottom_grid_top_width_0_height_0_subtile_3__pin_inpad_0_, chany_bottom_in[6:7]}),
		.sram(mux_2level_tapbuf_size5_11_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_11_sram_inv[0:5]),
		.out(chanx_right_out[6]));

	mux_2level_tapbuf_size5 mux_right_track_16 (
		.in({chany_top_in[6:7], right_bottom_grid_top_width_0_height_0_subtile_4__pin_inpad_0_, chany_bottom_in[4:5]}),
		.sram(mux_2level_tapbuf_size5_12_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_12_sram_inv[0:5]),
		.out(chanx_right_out[8]));

	mux_2level_tapbuf_size5 mux_right_track_20 (
		.in({chany_top_in[8:9], right_bottom_grid_top_width_0_height_0_subtile_5__pin_inpad_0_, chany_bottom_in[2:3]}),
		.sram(mux_2level_tapbuf_size5_13_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_13_sram_inv[0:5]),
		.out(chanx_right_out[10]));

	mux_2level_tapbuf_size5 mux_right_track_24 (
		.in({chany_top_in[10:11], right_bottom_grid_top_width_0_height_0_subtile_6__pin_inpad_0_, chany_bottom_in[0:1]}),
		.sram(mux_2level_tapbuf_size5_14_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_14_sram_inv[0:5]),
		.out(chanx_right_out[12]));

	mux_2level_tapbuf_size5 mux_right_track_28 (
		.in({chany_top_in[12:13], right_bottom_grid_top_width_0_height_0_subtile_7__pin_inpad_0_, chany_bottom_in[14:15]}),
		.sram(mux_2level_tapbuf_size5_15_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_15_sram_inv[0:5]),
		.out(chanx_right_out[14]));

	mux_2level_tapbuf_size5 mux_bottom_track_1 (
		.in({chanx_right_in[12:13], bottom_right_grid_left_width_0_height_0_subtile_0__pin_inpad_0_, chanx_left_in[2:3]}),
		.sram(mux_2level_tapbuf_size5_16_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_16_sram_inv[0:5]),
		.out(chany_bottom_out[0]));

	mux_2level_tapbuf_size5 mux_bottom_track_5 (
		.in({chanx_right_in[10:11], bottom_right_grid_left_width_0_height_0_subtile_1__pin_inpad_0_, chanx_left_in[4:5]}),
		.sram(mux_2level_tapbuf_size5_17_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_17_sram_inv[0:5]),
		.out(chany_bottom_out[2]));

	mux_2level_tapbuf_size5 mux_bottom_track_9 (
		.in({chanx_right_in[8:9], bottom_right_grid_left_width_0_height_0_subtile_2__pin_inpad_0_, chanx_left_in[6:7]}),
		.sram(mux_2level_tapbuf_size5_18_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_18_sram_inv[0:5]),
		.out(chany_bottom_out[4]));

	mux_2level_tapbuf_size5 mux_bottom_track_13 (
		.in({chanx_right_in[6:7], bottom_right_grid_left_width_0_height_0_subtile_3__pin_inpad_0_, chanx_left_in[8:9]}),
		.sram(mux_2level_tapbuf_size5_19_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_19_sram_inv[0:5]),
		.out(chany_bottom_out[6]));

	mux_2level_tapbuf_size5 mux_bottom_track_17 (
		.in({chanx_right_in[4:5], bottom_right_grid_left_width_0_height_0_subtile_4__pin_inpad_0_, chanx_left_in[10:11]}),
		.sram(mux_2level_tapbuf_size5_20_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_20_sram_inv[0:5]),
		.out(chany_bottom_out[8]));

	mux_2level_tapbuf_size5 mux_bottom_track_21 (
		.in({chanx_right_in[2:3], bottom_right_grid_left_width_0_height_0_subtile_5__pin_inpad_0_, chanx_left_in[12:13]}),
		.sram(mux_2level_tapbuf_size5_21_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_21_sram_inv[0:5]),
		.out(chany_bottom_out[10]));

	mux_2level_tapbuf_size5 mux_bottom_track_25 (
		.in({chanx_right_in[0:1], bottom_right_grid_left_width_0_height_0_subtile_6__pin_inpad_0_, chanx_left_in[14:15]}),
		.sram(mux_2level_tapbuf_size5_22_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_22_sram_inv[0:5]),
		.out(chany_bottom_out[12]));

	mux_2level_tapbuf_size5 mux_bottom_track_29 (
		.in({chanx_right_in[14:15], bottom_right_grid_left_width_0_height_0_subtile_7__pin_inpad_0_, chanx_left_in[0:1]}),
		.sram(mux_2level_tapbuf_size5_23_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_23_sram_inv[0:5]),
		.out(chany_bottom_out[14]));

	mux_2level_tapbuf_size5 mux_left_track_1 (
		.in({chany_top_in[0:1], chany_bottom_in[14:15], left_top_grid_bottom_width_0_height_0_subtile_0__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_24_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_24_sram_inv[0:5]),
		.out(chanx_left_out[0]));

	mux_2level_tapbuf_size5 mux_left_track_5 (
		.in({chany_top_in[14:15], chany_bottom_in[0:1], left_top_grid_bottom_width_0_height_0_subtile_1__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_25_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_25_sram_inv[0:5]),
		.out(chanx_left_out[2]));

	mux_2level_tapbuf_size5 mux_left_track_9 (
		.in({chany_top_in[12:13], chany_bottom_in[2:3], left_top_grid_bottom_width_0_height_0_subtile_2__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_26_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_26_sram_inv[0:5]),
		.out(chanx_left_out[4]));

	mux_2level_tapbuf_size5 mux_left_track_13 (
		.in({chany_top_in[10:11], chany_bottom_in[4:5], left_top_grid_bottom_width_0_height_0_subtile_3__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_27_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_27_sram_inv[0:5]),
		.out(chanx_left_out[6]));

	mux_2level_tapbuf_size5 mux_left_track_17 (
		.in({chany_top_in[8:9], chany_bottom_in[6:7], left_top_grid_bottom_width_0_height_0_subtile_4__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_28_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_28_sram_inv[0:5]),
		.out(chanx_left_out[8]));

	mux_2level_tapbuf_size5 mux_left_track_21 (
		.in({chany_top_in[6:7], chany_bottom_in[8:9], left_top_grid_bottom_width_0_height_0_subtile_5__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_29_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_29_sram_inv[0:5]),
		.out(chanx_left_out[10]));

	mux_2level_tapbuf_size5 mux_left_track_25 (
		.in({chany_top_in[4:5], chany_bottom_in[10:11], left_top_grid_bottom_width_0_height_0_subtile_6__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_30_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_30_sram_inv[0:5]),
		.out(chanx_left_out[12]));

	mux_2level_tapbuf_size5 mux_left_track_29 (
		.in({chany_top_in[2:3], chany_bottom_in[12:13], left_top_grid_bottom_width_0_height_0_subtile_7__pin_inpad_0_}),
		.sram(mux_2level_tapbuf_size5_31_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size5_31_sram_inv[0:5]),
		.out(chanx_left_out[14]));

	mux_2level_tapbuf_size5_mem mem_top_track_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size5_mem_0_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_0_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_0_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_top_track_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_1_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_1_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_1_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_top_track_8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_2_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_2_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_2_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_top_track_12 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_3_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_3_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_3_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_top_track_16 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_4_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_4_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_4_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_top_track_20 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_5_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_5_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_5_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_top_track_24 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_6_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_6_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_6_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_top_track_28 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_6_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_7_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_7_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_7_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size3_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_8_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_8_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_8_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_8_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_9_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_9_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_9_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_9_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_10_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_10_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_10_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_12 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_10_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_11_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_11_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_11_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_16 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_11_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_12_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_12_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_12_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_20 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_12_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_13_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_13_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_13_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_24 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_13_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_14_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_14_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_14_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_right_track_28 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_14_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_15_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_15_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_15_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_15_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_16_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_16_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_16_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_16_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_17_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_17_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_17_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_17_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_18_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_18_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_18_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_13 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_18_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_19_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_19_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_19_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_19_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_20_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_20_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_20_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_21 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_20_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_21_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_21_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_21_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_21_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_22_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_22_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_22_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_bottom_track_29 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_22_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_23_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_23_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_23_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size3_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_24_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_24_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_24_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_24_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_25_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_25_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_25_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_25_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_26_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_26_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_26_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_13 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_26_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_27_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_27_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_27_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_27_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_28_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_28_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_28_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_21 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_28_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_29_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_29_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_29_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_29_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size5_mem_30_ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_30_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_30_sram_inv[0:5]));

	mux_2level_tapbuf_size5_mem mem_left_track_29 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_30_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mux_2level_tapbuf_size5_31_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size5_31_sram_inv[0:5]));

	mux_2level_tapbuf_size3 mux_top_track_32 (
		.in({chanx_right_in[16], chany_bottom_in[16], chanx_left_in[16]}),
		.sram(mux_2level_tapbuf_size3_0_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size3_0_sram_inv[0:1]),
		.out(chany_top_out[16]));

	mux_2level_tapbuf_size3 mux_top_track_34 (
		.in({chanx_right_in[17], chany_bottom_in[17], chanx_left_in[17]}),
		.sram(mux_2level_tapbuf_size3_1_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size3_1_sram_inv[0:1]),
		.out(chany_top_out[17]));

	mux_2level_tapbuf_size3 mux_top_track_36 (
		.in({chanx_right_in[18], chany_bottom_in[18], chanx_left_in[18]}),
		.sram(mux_2level_tapbuf_size3_2_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size3_2_sram_inv[0:1]),
		.out(chany_top_out[18]));

	mux_2level_tapbuf_size3 mux_bottom_track_33 (
		.in({chany_top_in[16], chanx_right_in[16], chanx_left_in[16]}),
		.sram(mux_2level_tapbuf_size3_3_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size3_3_sram_inv[0:1]),
		.out(chany_bottom_out[16]));

	mux_2level_tapbuf_size3 mux_bottom_track_35 (
		.in({chany_top_in[17], chanx_right_in[17], chanx_left_in[17]}),
		.sram(mux_2level_tapbuf_size3_4_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size3_4_sram_inv[0:1]),
		.out(chany_bottom_out[17]));

	mux_2level_tapbuf_size3 mux_bottom_track_37 (
		.in({chany_top_in[18], chanx_right_in[18], chanx_left_in[18]}),
		.sram(mux_2level_tapbuf_size3_5_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size3_5_sram_inv[0:1]),
		.out(chany_bottom_out[18]));

	mux_2level_tapbuf_size3_mem mem_top_track_32 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_7_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size3_mem_0_ccff_tail),
		.mem_out(mux_2level_tapbuf_size3_0_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size3_0_sram_inv[0:1]));

	mux_2level_tapbuf_size3_mem mem_top_track_34 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size3_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size3_mem_1_ccff_tail),
		.mem_out(mux_2level_tapbuf_size3_1_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size3_1_sram_inv[0:1]));

	mux_2level_tapbuf_size3_mem mem_top_track_36 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size3_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size3_mem_2_ccff_tail),
		.mem_out(mux_2level_tapbuf_size3_2_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size3_2_sram_inv[0:1]));

	mux_2level_tapbuf_size3_mem mem_bottom_track_33 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size5_mem_23_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size3_mem_3_ccff_tail),
		.mem_out(mux_2level_tapbuf_size3_3_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size3_3_sram_inv[0:1]));

	mux_2level_tapbuf_size3_mem mem_bottom_track_35 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size3_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size3_mem_4_ccff_tail),
		.mem_out(mux_2level_tapbuf_size3_4_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size3_4_sram_inv[0:1]));

	mux_2level_tapbuf_size3_mem mem_bottom_track_37 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size3_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size3_mem_5_ccff_tail),
		.mem_out(mux_2level_tapbuf_size3_5_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size3_5_sram_inv[0:1]));

endmodule
// ----- END Verilog module for sb_0__0_ -----

//----- Default net type -----
`default_nettype wire



