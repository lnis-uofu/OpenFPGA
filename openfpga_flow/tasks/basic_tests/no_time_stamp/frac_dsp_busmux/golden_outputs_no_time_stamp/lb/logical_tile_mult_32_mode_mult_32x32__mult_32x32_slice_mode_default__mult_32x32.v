//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: mult_32x32
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32 -----
module logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32(pReset,
                                                                                       mult_32x32_a,
                                                                                       mult_32x32_b,
                                                                                       enable,
                                                                                       address,
                                                                                       data_in,
                                                                                       mult_32x32_out);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:31] mult_32x32_a;
//----- INPUT PORTS -----
input [0:31] mult_32x32_b;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:63] mult_32x32_out;

//----- BEGIN wire-connection ports -----
wire [0:31] mult_32x32_a;
wire [0:31] mult_32x32_b;
wire [0:63] mult_32x32_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] mult_32x32_0_mode;
wire [0:1] mult_32x32_LATCHR_mem_undriven_mem_outb;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mult_32x32 mult_32x32_0_ (
		.a(mult_32x32_a[0:31]),
		.b(mult_32x32_b[0:31]),
		.mode(mult_32x32_0_mode[0:1]),
		.out(mult_32x32_out[0:63]));

	mult_32x32_LATCHR_mem mult_32x32_LATCHR_mem (
		.pReset(pReset),
		.enable(enable),
		.address(address),
		.data_in(data_in),
		.mem_out(mult_32x32_0_mode[0:1]),
		.mem_outb(mult_32x32_LATCHR_mem_undriven_mem_outb[0:1]));

endmodule
// ----- END Verilog module for logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32 -----

//----- Default net type -----
`default_nettype wire



