//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexers
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for mux_2level_tapbuf_size6_basis_size3 -----
module mux_2level_tapbuf_size6_basis_size3(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:2] in;
//----- INPUT PORTS -----
input [0:2] mem;
//----- INPUT PORTS -----
input [0:2] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size6_basis_size3 -----



// ----- Verilog module for mux_2level_tapbuf_size2_basis_size2 -----
module mux_2level_tapbuf_size2_basis_size2(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:1] in;
//----- INPUT PORTS -----
input [0:0] mem;
//----- INPUT PORTS -----
input [0:0] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem_inv[0]),
		.selb(mem[0]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size2_basis_size2 -----



// ----- Verilog module for mux_2level_tapbuf_size8_basis_size3 -----
module mux_2level_tapbuf_size8_basis_size3(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:2] in;
//----- INPUT PORTS -----
input [0:2] mem;
//----- INPUT PORTS -----
input [0:2] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size8_basis_size3 -----



// ----- Verilog module for mux_2level_tapbuf_size9_basis_size4 -----
module mux_2level_tapbuf_size9_basis_size4(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:3] in;
//----- INPUT PORTS -----
input [0:3] mem;
//----- INPUT PORTS -----
input [0:3] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

	TGATE TGATE_3_ (
		.in(in[3]),
		.sel(mem[3]),
		.selb(mem_inv[3]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size9_basis_size4 -----



// ----- Verilog module for mux_2level_tapbuf_size3_basis_size2 -----
module mux_2level_tapbuf_size3_basis_size2(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:1] in;
//----- INPUT PORTS -----
input [0:0] mem;
//----- INPUT PORTS -----
input [0:0] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem_inv[0]),
		.selb(mem[0]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size3_basis_size2 -----



// ----- Verilog module for mux_2level_tapbuf_size5_basis_size3 -----
module mux_2level_tapbuf_size5_basis_size3(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:2] in;
//----- INPUT PORTS -----
input [0:2] mem;
//----- INPUT PORTS -----
input [0:2] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size5_basis_size3 -----



// ----- Verilog module for mux_2level_tapbuf_size5_basis_size2 -----
module mux_2level_tapbuf_size5_basis_size2(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:1] in;
//----- INPUT PORTS -----
input [0:1] mem;
//----- INPUT PORTS -----
input [0:1] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size5_basis_size2 -----



// ----- Verilog module for mux_2level_tapbuf_size4_basis_size3 -----
module mux_2level_tapbuf_size4_basis_size3(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:2] in;
//----- INPUT PORTS -----
input [0:2] mem;
//----- INPUT PORTS -----
input [0:2] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size4_basis_size3 -----



// ----- Verilog module for mux_2level_size14_basis_size4 -----
module mux_2level_size14_basis_size4(in,
                                     mem,
                                     mem_inv,
                                     out);
//----- INPUT PORTS -----
input [0:3] in;
//----- INPUT PORTS -----
input [0:3] mem;
//----- INPUT PORTS -----
input [0:3] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

	TGATE TGATE_3_ (
		.in(in[3]),
		.sel(mem[3]),
		.selb(mem_inv[3]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_size14_basis_size4 -----



// ----- Verilog module for mux_2level_size14_basis_size3 -----
module mux_2level_size14_basis_size3(in,
                                     mem,
                                     mem_inv,
                                     out);
//----- INPUT PORTS -----
input [0:2] in;
//----- INPUT PORTS -----
input [0:2] mem;
//----- INPUT PORTS -----
input [0:2] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_2level_size14_basis_size3 -----



// ----- Verilog module for mux_1level_tapbuf_size2_basis_size3 -----
module mux_1level_tapbuf_size2_basis_size3(in,
                                           mem,
                                           mem_inv,
                                           out);
//----- INPUT PORTS -----
input [0:2] in;
//----- INPUT PORTS -----
input [0:2] mem;
//----- INPUT PORTS -----
input [0:2] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out[0]));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out[0]));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2_basis_size3 -----



// ----- Verilog module for lut4_mux_basis_size2 -----
module lut4_mux_basis_size2(in,
                            mem,
                            mem_inv,
                            out);
//----- INPUT PORTS -----
input [0:1] in;
//----- INPUT PORTS -----
input [0:0] mem;
//----- INPUT PORTS -----
input [0:0] mem_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	TGATE TGATE_0_ (
		.in(in[0]),
		.sel(mem[0]),
		.selb(mem_inv[0]),
		.out(out[0]));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem_inv[0]),
		.selb(mem[0]),
		.out(out[0]));

endmodule
// ----- END Verilog module for lut4_mux_basis_size2 -----



// ----- Verilog module for mux_2level_tapbuf_size6 -----
module mux_2level_tapbuf_size6(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [0:5] in;
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
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_size6_basis_size3_0_out;
wire [0:0] mux_2level_tapbuf_size6_basis_size3_1_out;
wire [0:0] mux_2level_tapbuf_size6_basis_size3_2_out;

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

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out[0]));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out[0]));

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_size6_basis_size3_2_out[0]),
		.out(out[0]));

	mux_2level_tapbuf_size6_basis_size3 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0], INVTX1_2_out[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_size6_basis_size3_0_out[0]));

	mux_2level_tapbuf_size6_basis_size3 mux_l1_in_1_ (
		.in({INVTX1_3_out[0], INVTX1_4_out[0], INVTX1_5_out[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_size6_basis_size3_1_out[0]));

	mux_2level_tapbuf_size6_basis_size3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_size6_basis_size3_0_out[0], mux_2level_tapbuf_size6_basis_size3_1_out[0], const1_0_const1[0]}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_size6_basis_size3_2_out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size6 -----



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
wire [0:0] mux_2level_tapbuf_size2_basis_size2_0_out;
wire [0:0] mux_2level_tapbuf_size2_basis_size2_1_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_size2_basis_size2_1_out[0]),
		.out(out[0]));

	mux_2level_tapbuf_size2_basis_size2 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_2level_tapbuf_size2_basis_size2_0_out[0]));

	mux_2level_tapbuf_size2_basis_size2 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_size2_basis_size2_0_out[0], const1_0_const1[0]}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_2level_tapbuf_size2_basis_size2_1_out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size2 -----



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
wire [0:0] mux_2level_tapbuf_size8_basis_size3_0_out;
wire [0:0] mux_2level_tapbuf_size8_basis_size3_1_out;
wire [0:0] mux_2level_tapbuf_size8_basis_size3_2_out;
wire [0:0] mux_2level_tapbuf_size8_basis_size3_3_out;

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

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out[0]));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out[0]));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out[0]));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out[0]));

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_size8_basis_size3_3_out[0]),
		.out(out[0]));

	mux_2level_tapbuf_size8_basis_size3 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0], INVTX1_2_out[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_size8_basis_size3_0_out[0]));

	mux_2level_tapbuf_size8_basis_size3 mux_l1_in_1_ (
		.in({INVTX1_3_out[0], INVTX1_4_out[0], INVTX1_5_out[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_size8_basis_size3_1_out[0]));

	mux_2level_tapbuf_size8_basis_size3 mux_l1_in_2_ (
		.in({INVTX1_6_out[0], INVTX1_7_out[0], const1_0_const1[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_size8_basis_size3_2_out[0]));

	mux_2level_tapbuf_size8_basis_size3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_size8_basis_size3_0_out[0], mux_2level_tapbuf_size8_basis_size3_1_out[0], mux_2level_tapbuf_size8_basis_size3_2_out[0]}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_size8_basis_size3_3_out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size8 -----



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
wire [0:0] mux_2level_tapbuf_size9_basis_size4_0_out;
wire [0:0] mux_2level_tapbuf_size9_basis_size4_1_out;
wire [0:0] mux_2level_tapbuf_size9_basis_size4_2_out;

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

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out[0]));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out[0]));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out[0]));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out[0]));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out[0]));

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_size9_basis_size4_2_out[0]),
		.out(out[0]));

	mux_2level_tapbuf_size9_basis_size4 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0], INVTX1_2_out[0], INVTX1_3_out[0]}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_size9_basis_size4_0_out[0]));

	mux_2level_tapbuf_size9_basis_size4 mux_l1_in_1_ (
		.in({INVTX1_4_out[0], INVTX1_5_out[0], INVTX1_6_out[0], INVTX1_7_out[0]}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_tapbuf_size9_basis_size4_1_out[0]));

	mux_2level_tapbuf_size9_basis_size4 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_size9_basis_size4_0_out[0], mux_2level_tapbuf_size9_basis_size4_1_out[0], INVTX1_8_out[0], const1_0_const1[0]}),
		.mem(sram[4:7]),
		.mem_inv(sram_inv[4:7]),
		.out(mux_2level_tapbuf_size9_basis_size4_2_out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size9 -----



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
wire [0:0] mux_2level_tapbuf_size3_basis_size2_0_out;
wire [0:0] mux_2level_tapbuf_size3_basis_size2_1_out;
wire [0:0] mux_2level_tapbuf_size3_basis_size2_2_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_size3_basis_size2_2_out[0]),
		.out(out[0]));

	mux_2level_tapbuf_size3_basis_size2 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_2level_tapbuf_size3_basis_size2_0_out[0]));

	mux_2level_tapbuf_size3_basis_size2 mux_l1_in_1_ (
		.in({INVTX1_2_out[0], const1_0_const1[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(mux_2level_tapbuf_size3_basis_size2_1_out[0]));

	mux_2level_tapbuf_size3_basis_size2 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_size3_basis_size2_0_out[0], mux_2level_tapbuf_size3_basis_size2_1_out[0]}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(mux_2level_tapbuf_size3_basis_size2_2_out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size3 -----



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
wire [0:0] mux_2level_tapbuf_size5_basis_size2_0_out;
wire [0:0] mux_2level_tapbuf_size5_basis_size3_0_out;
wire [0:0] mux_2level_tapbuf_size5_basis_size3_1_out;

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

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out[0]));

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_size5_basis_size3_1_out[0]),
		.out(out[0]));

	mux_2level_tapbuf_size5_basis_size3 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0], INVTX1_2_out[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_size5_basis_size3_0_out[0]));

	mux_2level_tapbuf_size5_basis_size3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_size5_basis_size3_0_out[0], mux_2level_tapbuf_size5_basis_size2_0_out[0], const1_0_const1[0]}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_size5_basis_size3_1_out[0]));

	mux_2level_tapbuf_size5_basis_size2 mux_l1_in_1_ (
		.in({INVTX1_3_out[0], INVTX1_4_out[0]}),
		.mem(sram[0:1]),
		.mem_inv(sram_inv[0:1]),
		.out(mux_2level_tapbuf_size5_basis_size2_0_out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size5 -----



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
wire [0:0] mux_2level_tapbuf_size4_basis_size3_0_out;
wire [0:0] mux_2level_tapbuf_size4_basis_size3_1_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_size4_basis_size3_1_out[0]),
		.out(out[0]));

	mux_2level_tapbuf_size4_basis_size3 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0], INVTX1_2_out[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_tapbuf_size4_basis_size3_0_out[0]));

	mux_2level_tapbuf_size4_basis_size3 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_size4_basis_size3_0_out[0], INVTX1_3_out[0], const1_0_const1[0]}),
		.mem(sram[3:5]),
		.mem_inv(sram_inv[3:5]),
		.out(mux_2level_tapbuf_size4_basis_size3_1_out[0]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size4 -----



// ----- Verilog module for mux_2level_size14 -----
module mux_2level_size14(in,
                         sram,
                         sram_inv,
                         out);
//----- INPUT PORTS -----
input [0:13] in;
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
wire [0:0] mux_2level_size14_basis_size3_0_out;
wire [0:0] mux_2level_size14_basis_size4_0_out;
wire [0:0] mux_2level_size14_basis_size4_1_out;
wire [0:0] mux_2level_size14_basis_size4_2_out;
wire [0:0] mux_2level_size14_basis_size4_3_out;

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

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out[0]));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out[0]));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out[0]));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out[0]));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out[0]));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out[0]));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out[0]));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out[0]));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out[0]));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out[0]));

	INVTX1 INVTX1_14_ (
		.in(mux_2level_size14_basis_size4_3_out[0]),
		.out(out[0]));

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	mux_2level_size14_basis_size4 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0], INVTX1_2_out[0], INVTX1_3_out[0]}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_size14_basis_size4_0_out[0]));

	mux_2level_size14_basis_size4 mux_l1_in_1_ (
		.in({INVTX1_4_out[0], INVTX1_5_out[0], INVTX1_6_out[0], INVTX1_7_out[0]}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_size14_basis_size4_1_out[0]));

	mux_2level_size14_basis_size4 mux_l1_in_2_ (
		.in({INVTX1_8_out[0], INVTX1_9_out[0], INVTX1_10_out[0], INVTX1_11_out[0]}),
		.mem(sram[0:3]),
		.mem_inv(sram_inv[0:3]),
		.out(mux_2level_size14_basis_size4_2_out[0]));

	mux_2level_size14_basis_size4 mux_l2_in_0_ (
		.in({mux_2level_size14_basis_size4_0_out[0], mux_2level_size14_basis_size4_1_out[0], mux_2level_size14_basis_size4_2_out[0], mux_2level_size14_basis_size3_0_out[0]}),
		.mem(sram[4:7]),
		.mem_inv(sram_inv[4:7]),
		.out(mux_2level_size14_basis_size4_3_out[0]));

	mux_2level_size14_basis_size3 mux_l1_in_3_ (
		.in({INVTX1_12_out[0], INVTX1_13_out[0], const1_0_const1[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_2level_size14_basis_size3_0_out[0]));

endmodule
// ----- END Verilog module for mux_2level_size14 -----



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
wire [0:0] mux_1level_tapbuf_size2_basis_size3_0_out;

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

	const1 const1_0_ (
		.const1(const1_0_const1[0]));

	tap_buf4 tap_buf4_0_ (
		.in(mux_1level_tapbuf_size2_basis_size3_0_out[0]),
		.out(out[0]));

	mux_1level_tapbuf_size2_basis_size3 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0], const1_0_const1[0]}),
		.mem(sram[0:2]),
		.mem_inv(sram_inv[0:2]),
		.out(mux_1level_tapbuf_size2_basis_size3_0_out[0]));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2 -----



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
wire [0:0] lut4_mux_basis_size2_0_out;
wire [0:0] lut4_mux_basis_size2_10_out;
wire [0:0] lut4_mux_basis_size2_11_out;
wire [0:0] lut4_mux_basis_size2_12_out;
wire [0:0] lut4_mux_basis_size2_13_out;
wire [0:0] lut4_mux_basis_size2_14_out;
wire [0:0] lut4_mux_basis_size2_1_out;
wire [0:0] lut4_mux_basis_size2_2_out;
wire [0:0] lut4_mux_basis_size2_3_out;
wire [0:0] lut4_mux_basis_size2_4_out;
wire [0:0] lut4_mux_basis_size2_5_out;
wire [0:0] lut4_mux_basis_size2_6_out;
wire [0:0] lut4_mux_basis_size2_7_out;
wire [0:0] lut4_mux_basis_size2_8_out;
wire [0:0] lut4_mux_basis_size2_9_out;

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

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out[0]));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out[0]));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out[0]));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out[0]));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out[0]));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out[0]));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out[0]));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out[0]));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out[0]));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out[0]));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out[0]));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out[0]));

	INVTX1 INVTX1_16_ (
		.in(lut4_mux_basis_size2_14_out[0]),
		.out(out[0]));

	lut4_mux_basis_size2 mux_l1_in_0_ (
		.in({INVTX1_0_out[0], INVTX1_1_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_0_out[0]));

	lut4_mux_basis_size2 mux_l1_in_1_ (
		.in({INVTX1_2_out[0], INVTX1_3_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_1_out[0]));

	lut4_mux_basis_size2 mux_l1_in_2_ (
		.in({INVTX1_4_out[0], INVTX1_5_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_2_out[0]));

	lut4_mux_basis_size2 mux_l1_in_3_ (
		.in({INVTX1_6_out[0], INVTX1_7_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_3_out[0]));

	lut4_mux_basis_size2 mux_l1_in_4_ (
		.in({INVTX1_8_out[0], INVTX1_9_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_4_out[0]));

	lut4_mux_basis_size2 mux_l1_in_5_ (
		.in({INVTX1_10_out[0], INVTX1_11_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_5_out[0]));

	lut4_mux_basis_size2 mux_l1_in_6_ (
		.in({INVTX1_12_out[0], INVTX1_13_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_6_out[0]));

	lut4_mux_basis_size2 mux_l1_in_7_ (
		.in({INVTX1_14_out[0], INVTX1_15_out[0]}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(lut4_mux_basis_size2_7_out[0]));

	lut4_mux_basis_size2 mux_l2_in_0_ (
		.in({lut4_mux_basis_size2_0_out[0], lut4_mux_basis_size2_1_out[0]}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_size2_8_out[0]));

	lut4_mux_basis_size2 mux_l2_in_1_ (
		.in({lut4_mux_basis_size2_2_out[0], lut4_mux_basis_size2_3_out[0]}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_size2_9_out[0]));

	lut4_mux_basis_size2 mux_l2_in_2_ (
		.in({lut4_mux_basis_size2_4_out[0], lut4_mux_basis_size2_5_out[0]}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_size2_10_out[0]));

	lut4_mux_basis_size2 mux_l2_in_3_ (
		.in({lut4_mux_basis_size2_6_out[0], lut4_mux_basis_size2_7_out[0]}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(lut4_mux_basis_size2_11_out[0]));

	lut4_mux_basis_size2 mux_l3_in_0_ (
		.in({lut4_mux_basis_size2_8_out[0], lut4_mux_basis_size2_9_out[0]}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(lut4_mux_basis_size2_12_out[0]));

	lut4_mux_basis_size2 mux_l3_in_1_ (
		.in({lut4_mux_basis_size2_10_out[0], lut4_mux_basis_size2_11_out[0]}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(lut4_mux_basis_size2_13_out[0]));

	lut4_mux_basis_size2 mux_l4_in_0_ (
		.in({lut4_mux_basis_size2_12_out[0], lut4_mux_basis_size2_13_out[0]}),
		.mem(sram[3]),
		.mem_inv(sram_inv[3]),
		.out(lut4_mux_basis_size2_14_out[0]));

endmodule
// ----- END Verilog module for lut4_mux -----



