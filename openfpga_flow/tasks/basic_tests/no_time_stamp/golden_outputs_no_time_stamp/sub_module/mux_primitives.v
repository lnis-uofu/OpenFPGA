//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexer primitives
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_basis_input2_mem1 -----
module mux_tree_tapbuf_basis_input2_mem1(in,
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
// ----- END Verilog module for mux_tree_tapbuf_basis_input2_mem1 -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_basis_input2_mem1 -----
module mux_tree_basis_input2_mem1(in,
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
// ----- END Verilog module for mux_tree_basis_input2_mem1 -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for lut4_mux_basis_input2_mem1 -----
module lut4_mux_basis_input2_mem1(in,
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
// ----- END Verilog module for lut4_mux_basis_input2_mem1 -----

//----- Default net type -----
`default_nettype none




