//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Connection Blocks[1][1]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for cby_1__1_ -----
module cby_1__1_(pReset,
                 prog_clk,
                 chany_in_0_,
                 chany_in_1_,
                 chany_in_2_,
                 chany_in_3_,
                 chany_in_4_,
                 chany_in_5_,
                 chany_in_6_,
                 chany_in_7_,
                 chany_in_8_,
                 chany_in_9_,
                 chany_in_10_,
                 chany_in_11_,
                 chany_in_12_,
                 chany_in_13_,
                 chany_in_14_,
                 chany_in_15_,
                 chany_in_16_,
                 chany_in_17_,
                 enable,
                 address,
                 data_in,
                 chany_out_0_,
                 chany_out_1_,
                 chany_out_2_,
                 chany_out_3_,
                 chany_out_4_,
                 chany_out_5_,
                 chany_out_6_,
                 chany_out_7_,
                 chany_out_8_,
                 chany_out_9_,
                 chany_out_10_,
                 chany_out_11_,
                 chany_out_12_,
                 chany_out_13_,
                 chany_out_14_,
                 chany_out_15_,
                 chany_out_16_,
                 chany_out_17_,
                 right_grid_pin_3_,
                 right_grid_pin_7_,
                 left_grid_pin_1_,
                 left_grid_pin_5_,
                 left_grid_pin_9_);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] chany_in_0_;
//----- INPUT PORTS -----
input [0:0] chany_in_1_;
//----- INPUT PORTS -----
input [0:0] chany_in_2_;
//----- INPUT PORTS -----
input [0:0] chany_in_3_;
//----- INPUT PORTS -----
input [0:0] chany_in_4_;
//----- INPUT PORTS -----
input [0:0] chany_in_5_;
//----- INPUT PORTS -----
input [0:0] chany_in_6_;
//----- INPUT PORTS -----
input [0:0] chany_in_7_;
//----- INPUT PORTS -----
input [0:0] chany_in_8_;
//----- INPUT PORTS -----
input [0:0] chany_in_9_;
//----- INPUT PORTS -----
input [0:0] chany_in_10_;
//----- INPUT PORTS -----
input [0:0] chany_in_11_;
//----- INPUT PORTS -----
input [0:0] chany_in_12_;
//----- INPUT PORTS -----
input [0:0] chany_in_13_;
//----- INPUT PORTS -----
input [0:0] chany_in_14_;
//----- INPUT PORTS -----
input [0:0] chany_in_15_;
//----- INPUT PORTS -----
input [0:0] chany_in_16_;
//----- INPUT PORTS -----
input [0:0] chany_in_17_;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:5] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:0] chany_out_0_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_1_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_2_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_3_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_4_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_5_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_6_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_7_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_8_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_9_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_10_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_11_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_12_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_13_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_14_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_15_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_16_;
//----- OUTPUT PORTS -----
output [0:0] chany_out_17_;
//----- OUTPUT PORTS -----
output [0:0] right_grid_pin_3_;
//----- OUTPUT PORTS -----
output [0:0] right_grid_pin_7_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_pin_1_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_pin_5_;
//----- OUTPUT PORTS -----
output [0:0] left_grid_pin_9_;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:4] decoder3to5_0_data_out;
wire [0:1] mux_2level_tapbuf_size2_0_sram;
wire [0:1] mux_2level_tapbuf_size2_0_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_1_sram;
wire [0:1] mux_2level_tapbuf_size2_1_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_2_sram;
wire [0:1] mux_2level_tapbuf_size2_2_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_0_sram;
wire [0:5] mux_2level_tapbuf_size6_0_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_1_sram;
wire [0:5] mux_2level_tapbuf_size6_1_sram_inv;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_0_[0] = chany_in_0_[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_1_[0] = chany_in_1_[0];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_2_[0] = chany_in_2_[0];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_3_[0] = chany_in_3_[0];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_4_[0] = chany_in_4_[0];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_5_[0] = chany_in_5_[0];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_6_[0] = chany_in_6_[0];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_7_[0] = chany_in_7_[0];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_8_[0] = chany_in_8_[0];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_9_[0] = chany_in_9_[0];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_10_[0] = chany_in_10_[0];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_11_[0] = chany_in_11_[0];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_12_[0] = chany_in_12_[0];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_13_[0] = chany_in_13_[0];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_14_[0] = chany_in_14_[0];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_15_[0] = chany_in_15_[0];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_16_[0] = chany_in_16_[0];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chany_out_17_[0] = chany_in_17_[0];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size6 mux_left_ipin_0 (
		.in({chany_in_0_[0], chany_in_1_[0], chany_in_8_[0], chany_in_9_[0], chany_in_16_[0], chany_in_17_[0]}),
		.sram(mux_2level_tapbuf_size6_0_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_0_sram_inv[0:5]),
		.out(right_grid_pin_3_[0]));

	mux_2level_tapbuf_size6 mux_right_ipin_0 (
		.in({chany_in_2_[0], chany_in_3_[0], chany_in_4_[0], chany_in_5_[0], chany_in_12_[0], chany_in_13_[0]}),
		.sram(mux_2level_tapbuf_size6_1_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_1_sram_inv[0:5]),
		.out(left_grid_pin_1_[0]));

	mux_2level_tapbuf_size6_mem mem_left_ipin_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder3to5_0_data_out[0]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_0_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_0_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_right_ipin_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder3to5_0_data_out[2]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_1_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_1_sram_inv[0:5]));

	mux_2level_tapbuf_size2 mux_left_ipin_1 (
		.in({chany_in_2_[0], chany_in_3_[0]}),
		.sram(mux_2level_tapbuf_size2_0_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_0_sram_inv[0:1]),
		.out(right_grid_pin_7_[0]));

	mux_2level_tapbuf_size2 mux_right_ipin_1 (
		.in({chany_in_6_[0], chany_in_7_[0]}),
		.sram(mux_2level_tapbuf_size2_1_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_1_sram_inv[0:1]),
		.out(left_grid_pin_5_[0]));

	mux_2level_tapbuf_size2 mux_right_ipin_2 (
		.in({chany_in_8_[0], chany_in_9_[0]}),
		.sram(mux_2level_tapbuf_size2_2_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_2_sram_inv[0:1]),
		.out(left_grid_pin_9_[0]));

	mux_2level_tapbuf_size2_mem mem_left_ipin_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder3to5_0_data_out[1]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_0_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_0_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_ipin_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder3to5_0_data_out[3]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_1_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_1_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_right_ipin_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder3to5_0_data_out[4]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_2_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_2_sram_inv[0:1]));

	decoder3to5 decoder3to5_0_ (
		.enable(enable[0]),
		.address(address[3:5]),
		.data_out(decoder3to5_0_data_out[0:4]));

endmodule
// ----- END Verilog module for cby_1__1_ -----



