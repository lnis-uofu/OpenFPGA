//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexer primitives
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_basis_input3_mem3 -----
module mux_2level_tapbuf_basis_input3_mem3(in,
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
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_basis_input3_mem3 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_basis_input2_mem1 -----
module mux_2level_tapbuf_basis_input2_mem1(in,
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
		.sel(mem),
		.selb(mem_inv),
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem_inv),
		.selb(mem),
		.out(out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_basis_input2_mem1 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_basis_input4_mem4 -----
module mux_2level_tapbuf_basis_input4_mem4(in,
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
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out));

	TGATE TGATE_3_ (
		.in(in[3]),
		.sel(mem[3]),
		.selb(mem_inv[3]),
		.out(out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_basis_input4_mem4 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_basis_input2_mem2 -----
module mux_2level_tapbuf_basis_input2_mem2(in,
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
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_basis_input2_mem2 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_basis_input5_mem5 -----
module mux_2level_basis_input5_mem5(in,
                                    mem,
                                    mem_inv,
                                    out);
//----- INPUT PORTS -----
input [0:4] in;
//----- INPUT PORTS -----
input [0:4] mem;
//----- INPUT PORTS -----
input [0:4] mem_inv;
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
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out));

	TGATE TGATE_3_ (
		.in(in[3]),
		.sel(mem[3]),
		.selb(mem_inv[3]),
		.out(out));

	TGATE TGATE_4_ (
		.in(in[4]),
		.sel(mem[4]),
		.selb(mem_inv[4]),
		.out(out));

endmodule
// ----- END Verilog module for mux_2level_basis_input5_mem5 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_basis_input4_mem4 -----
module mux_1level_tapbuf_basis_input4_mem4(in,
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
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out));

	TGATE TGATE_3_ (
		.in(in[3]),
		.sel(mem[3]),
		.selb(mem_inv[3]),
		.out(out));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_basis_input4_mem4 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_basis_input3_mem3 -----
module mux_1level_tapbuf_basis_input3_mem3(in,
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
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem[1]),
		.selb(mem_inv[1]),
		.out(out));

	TGATE TGATE_2_ (
		.in(in[2]),
		.sel(mem[2]),
		.selb(mem_inv[2]),
		.out(out));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_basis_input3_mem3 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for frac_lut4_mux_basis_input2_mem1 -----
module frac_lut4_mux_basis_input2_mem1(in,
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
		.sel(mem),
		.selb(mem_inv),
		.out(out));

	TGATE TGATE_1_ (
		.in(in[1]),
		.sel(mem_inv),
		.selb(mem),
		.out(out));

endmodule
// ----- END Verilog module for frac_lut4_mux_basis_input2_mem1 -----

//----- Default net type -----
`default_nettype wire




