//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexers
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size6 -----
module mux_tree_tapbuf_size6(in,
                             sram,
                             sram_inv,
                             out);
//----- INPUT PORTS -----
input [0:5] in;
//----- INPUT PORTS -----
input [0:2] sram;
//----- INPUT PORTS -----
input [0:2] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_0_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_1_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_2_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_3_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_4_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_5_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_tree_tapbuf_basis_input2_mem1_5_out),
		.out(out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_tapbuf_basis_input2_mem1_0_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l1_in_1_ (
		.in({INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_tapbuf_basis_input2_mem1_1_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l1_in_2_ (
		.in({INVTX1_4_out, INVTX1_5_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_tapbuf_basis_input2_mem1_2_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l2_in_0_ (
		.in({mux_tree_tapbuf_basis_input2_mem1_0_out, mux_tree_tapbuf_basis_input2_mem1_1_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_tapbuf_basis_input2_mem1_3_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l2_in_1_ (
		.in({mux_tree_tapbuf_basis_input2_mem1_2_out, const1_0_const1}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_tapbuf_basis_input2_mem1_4_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l3_in_0_ (
		.in({mux_tree_tapbuf_basis_input2_mem1_3_out, mux_tree_tapbuf_basis_input2_mem1_4_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(mux_tree_tapbuf_basis_input2_mem1_5_out));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size6 -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size4 -----
module mux_tree_tapbuf_size4(in,
                             sram,
                             sram_inv,
                             out);
//----- INPUT PORTS -----
input [0:3] in;
//----- INPUT PORTS -----
input [0:2] sram;
//----- INPUT PORTS -----
input [0:2] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_0_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_1_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_2_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_3_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_tree_tapbuf_basis_input2_mem1_3_out),
		.out(out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_tapbuf_basis_input2_mem1_0_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l2_in_0_ (
		.in({mux_tree_tapbuf_basis_input2_mem1_0_out, INVTX1_2_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_tapbuf_basis_input2_mem1_1_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l2_in_1_ (
		.in({INVTX1_3_out, const1_0_const1}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_tapbuf_basis_input2_mem1_2_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l3_in_0_ (
		.in({mux_tree_tapbuf_basis_input2_mem1_1_out, mux_tree_tapbuf_basis_input2_mem1_2_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(mux_tree_tapbuf_basis_input2_mem1_3_out));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size4 -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size3 -----
module mux_tree_tapbuf_size3(in,
                             sram,
                             sram_inv,
                             out);
//----- INPUT PORTS -----
input [0:2] in;
//----- INPUT PORTS -----
input [0:1] sram;
//----- INPUT PORTS -----
input [0:1] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_0_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_1_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_2_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_tree_tapbuf_basis_input2_mem1_2_out),
		.out(out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_tapbuf_basis_input2_mem1_0_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l1_in_1_ (
		.in({INVTX1_2_out, const1_0_const1}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_tapbuf_basis_input2_mem1_1_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l2_in_0_ (
		.in({mux_tree_tapbuf_basis_input2_mem1_0_out, mux_tree_tapbuf_basis_input2_mem1_1_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_tapbuf_basis_input2_mem1_2_out));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size3 -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size2 -----
module mux_tree_tapbuf_size2(in,
                             sram,
                             sram_inv,
                             out);
//----- INPUT PORTS -----
input [0:1] in;
//----- INPUT PORTS -----
input [0:1] sram;
//----- INPUT PORTS -----
input [0:1] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_1_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_0_out;
wire [0:0] mux_tree_tapbuf_basis_input2_mem1_1_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_tree_tapbuf_basis_input2_mem1_1_out),
		.out(out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_tapbuf_basis_input2_mem1_0_out));

	mux_tree_tapbuf_basis_input2_mem1 mux_l2_in_0_ (
		.in({mux_tree_tapbuf_basis_input2_mem1_0_out, const1_0_const1}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_tapbuf_basis_input2_mem1_1_out));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size2 -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_size14 -----
module mux_tree_size14(in,
                       sram,
                       sram_inv,
                       out);
//----- INPUT PORTS -----
input [0:13] in;
//----- INPUT PORTS -----
input [0:3] sram;
//----- INPUT PORTS -----
input [0:3] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_tree_basis_input2_mem1_0_out;
wire [0:0] mux_tree_basis_input2_mem1_10_out;
wire [0:0] mux_tree_basis_input2_mem1_11_out;
wire [0:0] mux_tree_basis_input2_mem1_12_out;
wire [0:0] mux_tree_basis_input2_mem1_13_out;
wire [0:0] mux_tree_basis_input2_mem1_1_out;
wire [0:0] mux_tree_basis_input2_mem1_2_out;
wire [0:0] mux_tree_basis_input2_mem1_3_out;
wire [0:0] mux_tree_basis_input2_mem1_4_out;
wire [0:0] mux_tree_basis_input2_mem1_5_out;
wire [0:0] mux_tree_basis_input2_mem1_6_out;
wire [0:0] mux_tree_basis_input2_mem1_7_out;
wire [0:0] mux_tree_basis_input2_mem1_8_out;
wire [0:0] mux_tree_basis_input2_mem1_9_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(mux_tree_basis_input2_mem1_13_out),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	mux_tree_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_basis_input2_mem1_0_out));

	mux_tree_basis_input2_mem1 mux_l1_in_1_ (
		.in({INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_basis_input2_mem1_1_out));

	mux_tree_basis_input2_mem1 mux_l1_in_2_ (
		.in({INVTX1_4_out, INVTX1_5_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_basis_input2_mem1_2_out));

	mux_tree_basis_input2_mem1 mux_l1_in_3_ (
		.in({INVTX1_6_out, INVTX1_7_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_basis_input2_mem1_3_out));

	mux_tree_basis_input2_mem1 mux_l1_in_4_ (
		.in({INVTX1_8_out, INVTX1_9_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_basis_input2_mem1_4_out));

	mux_tree_basis_input2_mem1 mux_l1_in_5_ (
		.in({INVTX1_10_out, INVTX1_11_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_basis_input2_mem1_5_out));

	mux_tree_basis_input2_mem1 mux_l1_in_6_ (
		.in({INVTX1_12_out, INVTX1_13_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_tree_basis_input2_mem1_6_out));

	mux_tree_basis_input2_mem1 mux_l2_in_0_ (
		.in({mux_tree_basis_input2_mem1_0_out, mux_tree_basis_input2_mem1_1_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_basis_input2_mem1_7_out));

	mux_tree_basis_input2_mem1 mux_l2_in_1_ (
		.in({mux_tree_basis_input2_mem1_2_out, mux_tree_basis_input2_mem1_3_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_basis_input2_mem1_8_out));

	mux_tree_basis_input2_mem1 mux_l2_in_2_ (
		.in({mux_tree_basis_input2_mem1_4_out, mux_tree_basis_input2_mem1_5_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_basis_input2_mem1_9_out));

	mux_tree_basis_input2_mem1 mux_l2_in_3_ (
		.in({mux_tree_basis_input2_mem1_6_out, const1_0_const1}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_tree_basis_input2_mem1_10_out));

	mux_tree_basis_input2_mem1 mux_l3_in_0_ (
		.in({mux_tree_basis_input2_mem1_7_out, mux_tree_basis_input2_mem1_8_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(mux_tree_basis_input2_mem1_11_out));

	mux_tree_basis_input2_mem1 mux_l3_in_1_ (
		.in({mux_tree_basis_input2_mem1_9_out, mux_tree_basis_input2_mem1_10_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(mux_tree_basis_input2_mem1_12_out));

	mux_tree_basis_input2_mem1 mux_l4_in_0_ (
		.in({mux_tree_basis_input2_mem1_11_out, mux_tree_basis_input2_mem1_12_out}),
		.mem(sram[3]),
		.mem_inv(sram_inv[3]),
		.out(mux_tree_basis_input2_mem1_13_out));

endmodule
// ----- END Verilog module for mux_tree_size14 -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for lut4_mux -----
module lut4_mux(in,
                sram,
                sram_inv,
                out);
//----- INPUT PORTS -----
input [0:15] in;
//----- INPUT PORTS -----
input [0:3] sram;
//----- INPUT PORTS -----
input [0:3] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] lut4_mux_basis_input2_mem1_0_out;
wire [0:0] lut4_mux_basis_input2_mem1_10_out;
wire [0:0] lut4_mux_basis_input2_mem1_11_out;
wire [0:0] lut4_mux_basis_input2_mem1_12_out;
wire [0:0] lut4_mux_basis_input2_mem1_13_out;
wire [0:0] lut4_mux_basis_input2_mem1_14_out;
wire [0:0] lut4_mux_basis_input2_mem1_1_out;
wire [0:0] lut4_mux_basis_input2_mem1_2_out;
wire [0:0] lut4_mux_basis_input2_mem1_3_out;
wire [0:0] lut4_mux_basis_input2_mem1_4_out;
wire [0:0] lut4_mux_basis_input2_mem1_5_out;
wire [0:0] lut4_mux_basis_input2_mem1_6_out;
wire [0:0] lut4_mux_basis_input2_mem1_7_out;
wire [0:0] lut4_mux_basis_input2_mem1_8_out;
wire [0:0] lut4_mux_basis_input2_mem1_9_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	INVTX1 INVTX1_16_ (
		.in(lut4_mux_basis_input2_mem1_14_out),
		.out(out));

	lut4_mux_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_0_out));

	lut4_mux_basis_input2_mem1 mux_l1_in_1_ (
		.in({INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_1_out));

	lut4_mux_basis_input2_mem1 mux_l1_in_2_ (
		.in({INVTX1_4_out, INVTX1_5_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_2_out));

	lut4_mux_basis_input2_mem1 mux_l1_in_3_ (
		.in({INVTX1_6_out, INVTX1_7_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_3_out));

	lut4_mux_basis_input2_mem1 mux_l1_in_4_ (
		.in({INVTX1_8_out, INVTX1_9_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_4_out));

	lut4_mux_basis_input2_mem1 mux_l1_in_5_ (
		.in({INVTX1_10_out, INVTX1_11_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_5_out));

	lut4_mux_basis_input2_mem1 mux_l1_in_6_ (
		.in({INVTX1_12_out, INVTX1_13_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_6_out));

	lut4_mux_basis_input2_mem1 mux_l1_in_7_ (
		.in({INVTX1_14_out, INVTX1_15_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_input2_mem1_7_out));

	lut4_mux_basis_input2_mem1 mux_l2_in_0_ (
		.in({lut4_mux_basis_input2_mem1_0_out, lut4_mux_basis_input2_mem1_1_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_input2_mem1_8_out));

	lut4_mux_basis_input2_mem1 mux_l2_in_1_ (
		.in({lut4_mux_basis_input2_mem1_2_out, lut4_mux_basis_input2_mem1_3_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_input2_mem1_9_out));

	lut4_mux_basis_input2_mem1 mux_l2_in_2_ (
		.in({lut4_mux_basis_input2_mem1_4_out, lut4_mux_basis_input2_mem1_5_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_input2_mem1_10_out));

	lut4_mux_basis_input2_mem1 mux_l2_in_3_ (
		.in({lut4_mux_basis_input2_mem1_6_out, lut4_mux_basis_input2_mem1_7_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_input2_mem1_11_out));

	lut4_mux_basis_input2_mem1 mux_l3_in_0_ (
		.in({lut4_mux_basis_input2_mem1_8_out, lut4_mux_basis_input2_mem1_9_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(lut4_mux_basis_input2_mem1_12_out));

	lut4_mux_basis_input2_mem1 mux_l3_in_1_ (
		.in({lut4_mux_basis_input2_mem1_10_out, lut4_mux_basis_input2_mem1_11_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(lut4_mux_basis_input2_mem1_13_out));

	lut4_mux_basis_input2_mem1 mux_l4_in_0_ (
		.in({lut4_mux_basis_input2_mem1_12_out, lut4_mux_basis_input2_mem1_13_out}),
		.mem(sram[3]),
		.mem_inv(sram_inv[3]),
		.out(lut4_mux_basis_input2_mem1_14_out));

endmodule
// ----- END Verilog module for lut4_mux -----

//----- Default net type -----
`default_nettype none




