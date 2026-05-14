//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Look-Up Tables
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for frac_lut4 -----
module frac_lut4(in,
                 sram,
                 sram_inv,
                 mode,
                 mode_inv,
                 lut3_out,
                 lut4_out);
//----- INPUT PORTS -----
input [3:0] in;
//----- INPUT PORTS -----
input [15:0] sram;
//----- INPUT PORTS -----
input [15:0] sram_inv;
//----- INPUT PORTS -----
input [0:0] mode;
//----- INPUT PORTS -----
input [0:0] mode_inv;
//----- OUTPUT PORTS -----
output [1:0] lut3_out;
//----- OUTPUT PORTS -----
output [0:0] lut4_out;

//----- BEGIN wire-connection ports -----
wire [3:0] in;
wire [1:0] lut3_out;
wire [0:0] lut4_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] OR2_0_out;
wire [0:0] buf4_0_out;
wire [0:0] buf4_1_out;
wire [0:0] buf4_2_out;
wire [0:0] buf4_3_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	OR2 OR2_0_ (
		.a(mode),
		.b(in[3]),
		.out(OR2_0_out));

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
		.in(OR2_0_out),
		.out(INVTX1_3_out));

	buf4 buf4_0_ (
		.in(in[0]),
		.out(buf4_0_out));

	buf4 buf4_1_ (
		.in(in[1]),
		.out(buf4_1_out));

	buf4 buf4_2_ (
		.in(in[2]),
		.out(buf4_2_out));

	buf4 buf4_3_ (
		.in(OR2_0_out),
		.out(buf4_3_out));

	frac_lut4_mux frac_lut4_mux_0_ (
		.in({sram[15], sram[14], sram[13], sram[12], sram[11], sram[10], sram[9], sram[8], sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.sram({buf4_3_out, buf4_2_out, buf4_1_out, buf4_0_out}),
		.sram_inv({INVTX1_3_out, INVTX1_2_out, INVTX1_1_out, INVTX1_0_out}),
		.lut3_out({lut3_out[1], lut3_out[0]}),
		.lut4_out(lut4_out));

endmodule
// ----- END Verilog module for frac_lut4 -----

//----- Default net type -----
`default_nettype wire



