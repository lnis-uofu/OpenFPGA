//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexer primitives
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
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
		.A(in[0]),
		.S(mem),
		.SI(mem_inv),
		.Y(out));

	TGATE TGATE_1_ (
		.A(in[1]),
		.S(mem_inv),
		.SI(mem),
		.Y(out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_basis_input2_mem1 -----

//----- Default net type -----
`default_nettype wire




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
		.A(in[0]),
		.S(mem[0]),
		.SI(mem_inv[0]),
		.Y(out));

	TGATE TGATE_1_ (
		.A(in[1]),
		.S(mem[1]),
		.SI(mem_inv[1]),
		.Y(out));

	TGATE TGATE_2_ (
		.A(in[2]),
		.S(mem[2]),
		.SI(mem_inv[2]),
		.Y(out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_basis_input3_mem3 -----

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
		.A(in[0]),
		.S(mem[0]),
		.SI(mem_inv[0]),
		.Y(out));

	TGATE TGATE_1_ (
		.A(in[1]),
		.S(mem[1]),
		.SI(mem_inv[1]),
		.Y(out));

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
		.A(in[0]),
		.S(mem[0]),
		.SI(mem_inv[0]),
		.Y(out));

	TGATE TGATE_1_ (
		.A(in[1]),
		.S(mem[1]),
		.SI(mem_inv[1]),
		.Y(out));

	TGATE TGATE_2_ (
		.A(in[2]),
		.S(mem[2]),
		.SI(mem_inv[2]),
		.Y(out));

	TGATE TGATE_3_ (
		.A(in[3]),
		.S(mem[3]),
		.SI(mem_inv[3]),
		.Y(out));

	TGATE TGATE_4_ (
		.A(in[4]),
		.S(mem[4]),
		.SI(mem_inv[4]),
		.Y(out));

endmodule
// ----- END Verilog module for mux_2level_basis_input5_mem5 -----

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
		.A(in[0]),
		.S(mem[0]),
		.SI(mem_inv[0]),
		.Y(out));

	TGATE TGATE_1_ (
		.A(in[1]),
		.S(mem[1]),
		.SI(mem_inv[1]),
		.Y(out));

	TGATE TGATE_2_ (
		.A(in[2]),
		.S(mem[2]),
		.SI(mem_inv[2]),
		.Y(out));

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
		.A(in[0]),
		.S(mem),
		.SI(mem_inv),
		.Y(out));

	TGATE TGATE_1_ (
		.A(in[1]),
		.S(mem_inv),
		.SI(mem),
		.Y(out));

endmodule
// ----- END Verilog module for frac_lut4_mux_basis_input2_mem1 -----

//----- Default net type -----
`default_nettype wire




