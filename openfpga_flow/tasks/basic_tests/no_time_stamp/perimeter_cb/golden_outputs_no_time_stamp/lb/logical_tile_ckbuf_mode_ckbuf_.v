//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: ckbuf
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: ckbuf -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_ckbuf_mode_ckbuf_ -----
module logical_tile_ckbuf_mode_ckbuf_(ckbuf_in,
                                      ckbuf_out);
//----- INPUT PORTS -----
input [0:0] ckbuf_in;
//----- OUTPUT PORTS -----
output [0:0] ckbuf_out;

//----- BEGIN wire-connection ports -----
wire [0:0] ckbuf_in;
wire [0:0] ckbuf_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_1_out;
wire [0:0] logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0_ckbuf_core_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0 (
		.ckbuf_core_in(direct_interc_1_out),
		.ckbuf_core_out(logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0_ckbuf_core_out));

	direct_interc direct_interc_0_ (
		.in(logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0_ckbuf_core_out),
		.out(ckbuf_out));

	direct_interc direct_interc_1_ (
		.in(ckbuf_in),
		.out(direct_interc_1_out));

endmodule
// ----- END Verilog module for logical_tile_ckbuf_mode_ckbuf_ -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: ckbuf -----
