//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Connection Blocks[1][2]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for cbx_1__2_ -----
module cbx_1__2_(pReset,
                 prog_clk,
                 chanx_in_0_,
                 chanx_in_1_,
                 chanx_in_2_,
                 chanx_in_3_,
                 chanx_in_4_,
                 chanx_in_5_,
                 chanx_in_6_,
                 chanx_in_7_,
                 chanx_in_8_,
                 chanx_in_9_,
                 chanx_in_10_,
                 chanx_in_11_,
                 chanx_in_12_,
                 chanx_in_13_,
                 chanx_in_14_,
                 chanx_in_15_,
                 chanx_in_16_,
                 chanx_in_17_,
                 enable,
                 address,
                 data_in,
                 chanx_out_0_,
                 chanx_out_1_,
                 chanx_out_2_,
                 chanx_out_3_,
                 chanx_out_4_,
                 chanx_out_5_,
                 chanx_out_6_,
                 chanx_out_7_,
                 chanx_out_8_,
                 chanx_out_9_,
                 chanx_out_10_,
                 chanx_out_11_,
                 chanx_out_12_,
                 chanx_out_13_,
                 chanx_out_14_,
                 chanx_out_15_,
                 chanx_out_16_,
                 chanx_out_17_,
                 top_grid_pin_0_,
                 top_grid_pin_2_,
                 top_grid_pin_4_,
                 top_grid_pin_6_,
                 top_grid_pin_8_,
                 top_grid_pin_10_,
                 top_grid_pin_12_,
                 top_grid_pin_14_,
                 bottom_grid_pin_0_,
                 bottom_grid_pin_4_,
                 bottom_grid_pin_8_);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] chanx_in_0_;
//----- INPUT PORTS -----
input [0:0] chanx_in_1_;
//----- INPUT PORTS -----
input [0:0] chanx_in_2_;
//----- INPUT PORTS -----
input [0:0] chanx_in_3_;
//----- INPUT PORTS -----
input [0:0] chanx_in_4_;
//----- INPUT PORTS -----
input [0:0] chanx_in_5_;
//----- INPUT PORTS -----
input [0:0] chanx_in_6_;
//----- INPUT PORTS -----
input [0:0] chanx_in_7_;
//----- INPUT PORTS -----
input [0:0] chanx_in_8_;
//----- INPUT PORTS -----
input [0:0] chanx_in_9_;
//----- INPUT PORTS -----
input [0:0] chanx_in_10_;
//----- INPUT PORTS -----
input [0:0] chanx_in_11_;
//----- INPUT PORTS -----
input [0:0] chanx_in_12_;
//----- INPUT PORTS -----
input [0:0] chanx_in_13_;
//----- INPUT PORTS -----
input [0:0] chanx_in_14_;
//----- INPUT PORTS -----
input [0:0] chanx_in_15_;
//----- INPUT PORTS -----
input [0:0] chanx_in_16_;
//----- INPUT PORTS -----
input [0:0] chanx_in_17_;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:6] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_0_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_1_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_2_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_3_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_4_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_5_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_6_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_7_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_8_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_9_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_10_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_11_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_12_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_13_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_14_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_15_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_16_;
//----- OUTPUT PORTS -----
output [0:0] chanx_out_17_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_0_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_2_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_4_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_6_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_8_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_10_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_12_;
//----- OUTPUT PORTS -----
output [0:0] top_grid_pin_14_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_pin_0_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_pin_4_;
//----- OUTPUT PORTS -----
output [0:0] bottom_grid_pin_8_;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:10] decoder4to11_0_data_out;
wire [0:1] mux_2level_tapbuf_size2_0_sram;
wire [0:1] mux_2level_tapbuf_size2_0_sram_inv;
wire [0:1] mux_2level_tapbuf_size2_1_sram;
wire [0:1] mux_2level_tapbuf_size2_1_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_0_sram;
wire [0:5] mux_2level_tapbuf_size6_0_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_1_sram;
wire [0:5] mux_2level_tapbuf_size6_1_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_2_sram;
wire [0:5] mux_2level_tapbuf_size6_2_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_3_sram;
wire [0:5] mux_2level_tapbuf_size6_3_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_4_sram;
wire [0:5] mux_2level_tapbuf_size6_4_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_5_sram;
wire [0:5] mux_2level_tapbuf_size6_5_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_6_sram;
wire [0:5] mux_2level_tapbuf_size6_6_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_7_sram;
wire [0:5] mux_2level_tapbuf_size6_7_sram_inv;
wire [0:5] mux_2level_tapbuf_size6_8_sram;
wire [0:5] mux_2level_tapbuf_size6_8_sram_inv;

// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_0_[0] = chanx_in_0_[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_1_[0] = chanx_in_1_[0];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_2_[0] = chanx_in_2_[0];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_3_[0] = chanx_in_3_[0];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_4_[0] = chanx_in_4_[0];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_5_[0] = chanx_in_5_[0];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_6_[0] = chanx_in_6_[0];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_7_[0] = chanx_in_7_[0];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_8_[0] = chanx_in_8_[0];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_9_[0] = chanx_in_9_[0];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_10_[0] = chanx_in_10_[0];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_11_[0] = chanx_in_11_[0];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_12_[0] = chanx_in_12_[0];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_13_[0] = chanx_in_13_[0];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_14_[0] = chanx_in_14_[0];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_15_[0] = chanx_in_15_[0];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_16_[0] = chanx_in_16_[0];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign chanx_out_17_[0] = chanx_in_17_[0];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size6 mux_bottom_ipin_0 (
		.in({chanx_in_0_[0], chanx_in_1_[0], chanx_in_8_[0], chanx_in_9_[0], chanx_in_16_[0], chanx_in_17_[0]}),
		.sram(mux_2level_tapbuf_size6_0_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_0_sram_inv[0:5]),
		.out(top_grid_pin_0_[0]));

	mux_2level_tapbuf_size6 mux_bottom_ipin_1 (
		.in({chanx_in_0_[0], chanx_in_1_[0], chanx_in_2_[0], chanx_in_3_[0], chanx_in_10_[0], chanx_in_11_[0]}),
		.sram(mux_2level_tapbuf_size6_1_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_1_sram_inv[0:5]),
		.out(top_grid_pin_2_[0]));

	mux_2level_tapbuf_size6 mux_bottom_ipin_2 (
		.in({chanx_in_2_[0], chanx_in_3_[0], chanx_in_4_[0], chanx_in_5_[0], chanx_in_12_[0], chanx_in_13_[0]}),
		.sram(mux_2level_tapbuf_size6_2_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_2_sram_inv[0:5]),
		.out(top_grid_pin_4_[0]));

	mux_2level_tapbuf_size6 mux_bottom_ipin_3 (
		.in({chanx_in_4_[0], chanx_in_5_[0], chanx_in_6_[0], chanx_in_7_[0], chanx_in_14_[0], chanx_in_15_[0]}),
		.sram(mux_2level_tapbuf_size6_3_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_3_sram_inv[0:5]),
		.out(top_grid_pin_6_[0]));

	mux_2level_tapbuf_size6 mux_bottom_ipin_4 (
		.in({chanx_in_6_[0], chanx_in_7_[0], chanx_in_8_[0], chanx_in_9_[0], chanx_in_16_[0], chanx_in_17_[0]}),
		.sram(mux_2level_tapbuf_size6_4_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_4_sram_inv[0:5]),
		.out(top_grid_pin_8_[0]));

	mux_2level_tapbuf_size6 mux_bottom_ipin_5 (
		.in({chanx_in_0_[0], chanx_in_1_[0], chanx_in_8_[0], chanx_in_9_[0], chanx_in_10_[0], chanx_in_11_[0]}),
		.sram(mux_2level_tapbuf_size6_5_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_5_sram_inv[0:5]),
		.out(top_grid_pin_10_[0]));

	mux_2level_tapbuf_size6 mux_bottom_ipin_6 (
		.in({chanx_in_2_[0], chanx_in_3_[0], chanx_in_10_[0], chanx_in_11_[0], chanx_in_12_[0], chanx_in_13_[0]}),
		.sram(mux_2level_tapbuf_size6_6_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_6_sram_inv[0:5]),
		.out(top_grid_pin_12_[0]));

	mux_2level_tapbuf_size6 mux_bottom_ipin_7 (
		.in({chanx_in_4_[0], chanx_in_5_[0], chanx_in_12_[0], chanx_in_13_[0], chanx_in_14_[0], chanx_in_15_[0]}),
		.sram(mux_2level_tapbuf_size6_7_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_7_sram_inv[0:5]),
		.out(top_grid_pin_14_[0]));

	mux_2level_tapbuf_size6 mux_top_ipin_0 (
		.in({chanx_in_6_[0], chanx_in_7_[0], chanx_in_14_[0], chanx_in_15_[0], chanx_in_16_[0], chanx_in_17_[0]}),
		.sram(mux_2level_tapbuf_size6_8_sram[0:5]),
		.sram_inv(mux_2level_tapbuf_size6_8_sram_inv[0:5]),
		.out(bottom_grid_pin_0_[0]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[0]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_0_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_0_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[1]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_1_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_1_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[2]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_2_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_2_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[3]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_3_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_3_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[4]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_4_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_4_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[5]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_5_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_5_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_6 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[6]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_6_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_6_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_bottom_ipin_7 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[7]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_7_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_7_sram_inv[0:5]));

	mux_2level_tapbuf_size6_mem mem_top_ipin_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[8]),
		.address(address[0:2]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size6_8_sram[0:5]),
		.mem_outb(mux_2level_tapbuf_size6_8_sram_inv[0:5]));

	mux_2level_tapbuf_size2 mux_top_ipin_1 (
		.in({chanx_in_0_[0], chanx_in_1_[0]}),
		.sram(mux_2level_tapbuf_size2_0_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_0_sram_inv[0:1]),
		.out(bottom_grid_pin_4_[0]));

	mux_2level_tapbuf_size2 mux_top_ipin_2 (
		.in({chanx_in_2_[0], chanx_in_3_[0]}),
		.sram(mux_2level_tapbuf_size2_1_sram[0:1]),
		.sram_inv(mux_2level_tapbuf_size2_1_sram_inv[0:1]),
		.out(bottom_grid_pin_8_[0]));

	mux_2level_tapbuf_size2_mem mem_top_ipin_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[9]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_0_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_0_sram_inv[0:1]));

	mux_2level_tapbuf_size2_mem mem_top_ipin_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(decoder4to11_0_data_out[10]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(mux_2level_tapbuf_size2_1_sram[0:1]),
		.mem_outb(mux_2level_tapbuf_size2_1_sram_inv[0:1]));

	decoder4to11 decoder4to11_0_ (
		.enable(enable[0]),
		.address(address[3:6]),
		.data_out(decoder4to11_0_data_out[0:10]));

endmodule
// ----- END Verilog module for cbx_1__2_ -----



