//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Look-Up Tables
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for lut4 -----
module lut4(in,
            sram,
            sram_inv,
            out);
//----- INPUT PORTS -----
input [0:3] in;
//----- INPUT PORTS -----
input [0:15] sram;
//----- INPUT PORTS -----
input [0:15] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
wire [0:3] in;
wire [0:0] out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] buf4_0_out;
wire [0:0] buf4_1_out;
wire [0:0] buf4_2_out;
wire [0:0] buf4_3_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out[0]));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out[0]));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out[0]));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out[0]));

	buf4 buf4_0_ (
		.in(in[0]),
		.out(buf4_0_out[0]));

	buf4 buf4_1_ (
		.in(in[1]),
		.out(buf4_1_out[0]));

	buf4 buf4_2_ (
		.in(in[2]),
		.out(buf4_2_out[0]));

	buf4 buf4_3_ (
		.in(in[3]),
		.out(buf4_3_out[0]));

	lut4_mux lut4_mux_0_ (
		.in(sram[0:15]),
		.sram({buf4_0_out[0], buf4_1_out[0], buf4_2_out[0], buf4_3_out[0]}),
		.sram_inv({INVTX1_0_out[0], INVTX1_1_out[0], INVTX1_2_out[0], INVTX1_3_out[0]}),
		.out(out[0]));

endmodule
// ----- END Verilog module for lut4 -----


