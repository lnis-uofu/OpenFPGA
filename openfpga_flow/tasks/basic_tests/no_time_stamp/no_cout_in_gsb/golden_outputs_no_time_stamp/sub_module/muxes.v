//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexers
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size4 -----
module mux_2level_tapbuf_size4(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:3] in;
//----- INPUT PORTS -----
input [0:5] sram;
//----- INPUT PORTS -----
input [0:5] sram_inv;
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
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_0_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_1_out;

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
		.in(mux_2level_tapbuf_basis_input3_mem3_1_out),
		.out(out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_0_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input3_mem3_0_out, INVTX1_3_out, const1_0_const1}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_basis_input3_mem3_1_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size4 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size2 -----
module mux_2level_tapbuf_size2(in,
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
wire [0:0] mux_2level_tapbuf_basis_input2_mem1_0_out;
wire [0:0] mux_2level_tapbuf_basis_input2_mem1_1_out;

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
		.in(mux_2level_tapbuf_basis_input2_mem1_1_out),
		.out(out));

	mux_2level_tapbuf_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_2level_tapbuf_basis_input2_mem1_0_out));

	mux_2level_tapbuf_basis_input2_mem1 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input2_mem1_0_out, const1_0_const1}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_2level_tapbuf_basis_input2_mem1_1_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size2 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size3 -----
module mux_2level_tapbuf_size3(in,
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
wire [0:0] mux_2level_tapbuf_basis_input2_mem1_0_out;
wire [0:0] mux_2level_tapbuf_basis_input2_mem1_1_out;
wire [0:0] mux_2level_tapbuf_basis_input2_mem1_2_out;

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
		.in(mux_2level_tapbuf_basis_input2_mem1_2_out),
		.out(out));

	mux_2level_tapbuf_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_2level_tapbuf_basis_input2_mem1_0_out));

	mux_2level_tapbuf_basis_input2_mem1 mux_l1_in_1_ (
		.in({INVTX1_2_out, const1_0_const1}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_2level_tapbuf_basis_input2_mem1_1_out));

	mux_2level_tapbuf_basis_input2_mem1 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input2_mem1_0_out, mux_2level_tapbuf_basis_input2_mem1_1_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_2level_tapbuf_basis_input2_mem1_2_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size3 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size11 -----
module mux_2level_tapbuf_size11(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [0:10] in;
//----- INPUT PORTS -----
input [0:7] sram;
//----- INPUT PORTS -----
input [0:7] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
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
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_0_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_0_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_1_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_2_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input4_mem4_2_out),
		.out(out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_0_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_1_ (
		.in({INVTX1_4_out, INVTX1_5_out, INVTX1_6_out, INVTX1_7_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_1_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input4_mem4_0_out, mux_2level_tapbuf_basis_input4_mem4_1_out, mux_2level_tapbuf_basis_input3_mem3_0_out, const1_0_const1}),
		.mem(sram[4:7]),
		.mem_inv(sram_inv[4:7]),
		.out(mux_2level_tapbuf_basis_input4_mem4_2_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_2_ (
		.in({INVTX1_8_out, INVTX1_9_out, INVTX1_10_out}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_0_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size11 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size9 -----
module mux_2level_tapbuf_size9(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:8] in;
//----- INPUT PORTS -----
input [0:7] sram;
//----- INPUT PORTS -----
input [0:7] sram_inv;
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
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_0_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_1_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_2_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input4_mem4_2_out),
		.out(out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_0_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_1_ (
		.in({INVTX1_4_out, INVTX1_5_out, INVTX1_6_out, INVTX1_7_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_1_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input4_mem4_0_out, mux_2level_tapbuf_basis_input4_mem4_1_out, INVTX1_8_out, const1_0_const1}),
		.mem(sram[4:7]),
		.mem_inv(sram_inv[4:7]),
		.out(mux_2level_tapbuf_basis_input4_mem4_2_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size9 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size10 -----
module mux_2level_tapbuf_size10(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [0:9] in;
//----- INPUT PORTS -----
input [0:7] sram;
//----- INPUT PORTS -----
input [0:7] sram_inv;
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
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input2_mem2_0_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_0_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_1_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_2_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input4_mem4_2_out),
		.out(out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_0_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_1_ (
		.in({INVTX1_4_out, INVTX1_5_out, INVTX1_6_out, INVTX1_7_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_1_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input4_mem4_0_out, mux_2level_tapbuf_basis_input4_mem4_1_out, mux_2level_tapbuf_basis_input2_mem2_0_out, const1_0_const1}),
		.mem(sram[4:7]),
		.mem_inv(sram_inv[4:7]),
		.out(mux_2level_tapbuf_basis_input4_mem4_2_out));

	mux_2level_tapbuf_basis_input2_mem2 mux_l1_in_2_ (
		.in({INVTX1_8_out, INVTX1_9_out}),
		.mem(sram[0:1]),
		.mem_inv(sram_inv[0:1]),
		.out(mux_2level_tapbuf_basis_input2_mem2_0_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size10 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size12 -----
module mux_2level_tapbuf_size12(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [0:11] in;
//----- INPUT PORTS -----
input [0:7] sram;
//----- INPUT PORTS -----
input [0:7] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
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
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_0_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_1_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_2_out;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_3_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input4_mem4_3_out),
		.out(out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_0_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_1_ (
		.in({INVTX1_4_out, INVTX1_5_out, INVTX1_6_out, INVTX1_7_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_1_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_2_ (
		.in({INVTX1_8_out, INVTX1_9_out, INVTX1_10_out, INVTX1_11_out}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_basis_input4_mem4_2_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input4_mem4_0_out, mux_2level_tapbuf_basis_input4_mem4_1_out, mux_2level_tapbuf_basis_input4_mem4_2_out, const1_0_const1}),
		.mem(sram[4:7]),
		.mem_inv(sram_inv[4:7]),
		.out(mux_2level_tapbuf_basis_input4_mem4_3_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size12 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size7 -----
module mux_2level_tapbuf_size7(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:6] in;
//----- INPUT PORTS -----
input [0:5] sram;
//----- INPUT PORTS -----
input [0:5] sram_inv;
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
wire [0:0] INVTX1_6_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input2_mem2_0_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_0_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_1_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_2_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input3_mem3_2_out),
		.out(out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_0_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_1_ (
		.in({INVTX1_3_out, INVTX1_4_out, INVTX1_5_out}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_1_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input3_mem3_0_out, mux_2level_tapbuf_basis_input3_mem3_1_out, mux_2level_tapbuf_basis_input2_mem2_0_out}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_basis_input3_mem3_2_out));

	mux_2level_tapbuf_basis_input2_mem2 mux_l1_in_2_ (
		.in({INVTX1_6_out, const1_0_const1}),
		.mem(sram[0:1]),
		.mem_inv(sram_inv[0:1]),
		.out(mux_2level_tapbuf_basis_input2_mem2_0_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size7 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size8 -----
module mux_2level_tapbuf_size8(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:7] in;
//----- INPUT PORTS -----
input [0:5] sram;
//----- INPUT PORTS -----
input [0:5] sram_inv;
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
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_0_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_1_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_2_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_3_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input3_mem3_3_out),
		.out(out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_0_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_1_ (
		.in({INVTX1_3_out, INVTX1_4_out, INVTX1_5_out}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_1_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_2_ (
		.in({INVTX1_6_out, INVTX1_7_out, const1_0_const1}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_2_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input3_mem3_0_out, mux_2level_tapbuf_basis_input3_mem3_1_out, mux_2level_tapbuf_basis_input3_mem3_2_out}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_basis_input3_mem3_3_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size8 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size5 -----
module mux_2level_tapbuf_size5(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:4] in;
//----- INPUT PORTS -----
input [0:5] sram;
//----- INPUT PORTS -----
input [0:5] sram_inv;
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
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input2_mem2_0_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_0_out;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_1_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input3_mem3_1_out),
		.out(out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_basis_input3_mem3_0_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input3_mem3_0_out, mux_2level_tapbuf_basis_input2_mem2_0_out, const1_0_const1}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_basis_input3_mem3_1_out));

	mux_2level_tapbuf_basis_input2_mem2 mux_l1_in_1_ (
		.in({INVTX1_3_out, INVTX1_4_out}),
		.mem(sram[0:1]),
		.mem_inv(sram_inv[0:1]),
		.out(mux_2level_tapbuf_basis_input2_mem2_0_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size5 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_size20 -----
module mux_2level_size20(in,
                         sram,
                         sram_inv,
                         out);
//----- INPUT PORTS -----
input [0:19] in;
//----- INPUT PORTS -----
input [0:9] sram;
//----- INPUT PORTS -----
input [0:9] sram_inv;
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
wire [0:0] INVTX1_16_out;
wire [0:0] INVTX1_17_out;
wire [0:0] INVTX1_18_out;
wire [0:0] INVTX1_19_out;
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
wire [0:0] mux_2level_basis_input5_mem5_0_out;
wire [0:0] mux_2level_basis_input5_mem5_1_out;
wire [0:0] mux_2level_basis_input5_mem5_2_out;
wire [0:0] mux_2level_basis_input5_mem5_3_out;
wire [0:0] mux_2level_basis_input5_mem5_4_out;

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
		.in(in[16]),
		.out(INVTX1_16_out));

	INVTX1 INVTX1_17_ (
		.in(in[17]),
		.out(INVTX1_17_out));

	INVTX1 INVTX1_18_ (
		.in(in[18]),
		.out(INVTX1_18_out));

	INVTX1 INVTX1_19_ (
		.in(in[19]),
		.out(INVTX1_19_out));

	INVTX1 INVTX1_20_ (
		.in(mux_2level_basis_input5_mem5_4_out),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	mux_2level_basis_input5_mem5 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out, INVTX1_3_out, INVTX1_4_out}),
		.mem(sram[0:4]),
		.mem_inv(sram_inv[0:4]),
		.out(mux_2level_basis_input5_mem5_0_out));

	mux_2level_basis_input5_mem5 mux_l1_in_1_ (
		.in({INVTX1_5_out, INVTX1_6_out, INVTX1_7_out, INVTX1_8_out, INVTX1_9_out}),
		.mem(sram[0:4]),
		.mem_inv(sram_inv[0:4]),
		.out(mux_2level_basis_input5_mem5_1_out));

	mux_2level_basis_input5_mem5 mux_l1_in_2_ (
		.in({INVTX1_10_out, INVTX1_11_out, INVTX1_12_out, INVTX1_13_out, INVTX1_14_out}),
		.mem(sram[0:4]),
		.mem_inv(sram_inv[0:4]),
		.out(mux_2level_basis_input5_mem5_2_out));

	mux_2level_basis_input5_mem5 mux_l1_in_3_ (
		.in({INVTX1_15_out, INVTX1_16_out, INVTX1_17_out, INVTX1_18_out, INVTX1_19_out}),
		.mem(sram[0:4]),
		.mem_inv(sram_inv[0:4]),
		.out(mux_2level_basis_input5_mem5_3_out));

	mux_2level_basis_input5_mem5 mux_l2_in_0_ (
		.in({mux_2level_basis_input5_mem5_0_out, mux_2level_basis_input5_mem5_1_out, mux_2level_basis_input5_mem5_2_out, mux_2level_basis_input5_mem5_3_out, const1_0_const1}),
		.mem(sram[5:9]),
		.mem_inv(sram_inv[5:9]),
		.out(mux_2level_basis_input5_mem5_4_out));

endmodule
// ----- END Verilog module for mux_2level_size20 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_size3 -----
module mux_1level_tapbuf_size3(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:2] in;
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
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_1level_tapbuf_basis_input4_mem4_0_out;

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
		.in(mux_1level_tapbuf_basis_input4_mem4_0_out),
		.out(out));

	mux_1level_tapbuf_basis_input4_mem4 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, INVTX1_2_out, const1_0_const1}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_1level_tapbuf_basis_input4_mem4_0_out));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size3 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_size2 -----
module mux_1level_tapbuf_size2(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:1] in;
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
wire [0:0] const1_0_const1;
wire [0:0] mux_1level_tapbuf_basis_input3_mem3_0_out;

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
		.in(mux_1level_tapbuf_basis_input3_mem3_0_out),
		.out(out));

	mux_1level_tapbuf_basis_input3_mem3 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out, const1_0_const1}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_1level_tapbuf_basis_input3_mem3_0_out));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for frac_lut4_mux -----
module frac_lut4_mux(in,
                     sram,
                     sram_inv,
                     lut3_out,
                     lut4_out);
//----- INPUT PORTS -----
input [0:15] in;
//----- INPUT PORTS -----
input [0:3] sram;
//----- INPUT PORTS -----
input [0:3] sram_inv;
//----- OUTPUT PORTS -----
output [0:1] lut3_out;
//----- OUTPUT PORTS -----
output [0:0] lut4_out;

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
wire [0:0] buf4_0_out;
wire [0:0] buf4_1_out;
wire [0:0] buf4_2_out;
wire [0:0] buf4_3_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_0_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_10_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_11_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_12_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_13_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_14_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_1_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_2_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_3_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_4_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_5_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_6_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_7_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_8_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_9_out;

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
		.in(frac_lut4_mux_basis_input2_mem1_12_out),
		.out(lut3_out[0]));

	INVTX1 INVTX1_17_ (
		.in(frac_lut4_mux_basis_input2_mem1_13_out),
		.out(lut3_out[1]));

	INVTX1 INVTX1_18_ (
		.in(frac_lut4_mux_basis_input2_mem1_14_out),
		.out(lut4_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_0_out, INVTX1_1_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_0_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_1_ (
		.in({INVTX1_2_out, INVTX1_3_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_1_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_2_ (
		.in({INVTX1_4_out, INVTX1_5_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_2_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_3_ (
		.in({INVTX1_6_out, INVTX1_7_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_3_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_4_ (
		.in({INVTX1_8_out, INVTX1_9_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_4_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_5_ (
		.in({INVTX1_10_out, INVTX1_11_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_5_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_6_ (
		.in({INVTX1_12_out, INVTX1_13_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_6_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_7_ (
		.in({INVTX1_14_out, INVTX1_15_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_7_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_0_ (
		.in({frac_lut4_mux_basis_input2_mem1_0_out, frac_lut4_mux_basis_input2_mem1_1_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_8_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_1_ (
		.in({frac_lut4_mux_basis_input2_mem1_2_out, frac_lut4_mux_basis_input2_mem1_3_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_9_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_2_ (
		.in({frac_lut4_mux_basis_input2_mem1_4_out, frac_lut4_mux_basis_input2_mem1_5_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_10_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_3_ (
		.in({frac_lut4_mux_basis_input2_mem1_6_out, frac_lut4_mux_basis_input2_mem1_7_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_11_out));

	frac_lut4_mux_basis_input2_mem1 mux_l3_in_0_ (
		.in({buf4_0_out, buf4_1_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(frac_lut4_mux_basis_input2_mem1_12_out));

	frac_lut4_mux_basis_input2_mem1 mux_l3_in_1_ (
		.in({buf4_2_out, buf4_3_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(frac_lut4_mux_basis_input2_mem1_13_out));

	frac_lut4_mux_basis_input2_mem1 mux_l4_in_0_ (
		.in({frac_lut4_mux_basis_input2_mem1_12_out, frac_lut4_mux_basis_input2_mem1_13_out}),
		.mem(sram[3]),
		.mem_inv(sram_inv[3]),
		.out(frac_lut4_mux_basis_input2_mem1_14_out));

	buf4 buf4_0_ (
		.in(frac_lut4_mux_basis_input2_mem1_8_out),
		.out(buf4_0_out));

	buf4 buf4_1_ (
		.in(frac_lut4_mux_basis_input2_mem1_9_out),
		.out(buf4_1_out));

	buf4 buf4_2_ (
		.in(frac_lut4_mux_basis_input2_mem1_10_out),
		.out(buf4_2_out));

	buf4 buf4_3_ (
		.in(frac_lut4_mux_basis_input2_mem1_11_out),
		.out(buf4_3_out));

endmodule
// ----- END Verilog module for frac_lut4_mux -----

//----- Default net type -----
`default_nettype wire




