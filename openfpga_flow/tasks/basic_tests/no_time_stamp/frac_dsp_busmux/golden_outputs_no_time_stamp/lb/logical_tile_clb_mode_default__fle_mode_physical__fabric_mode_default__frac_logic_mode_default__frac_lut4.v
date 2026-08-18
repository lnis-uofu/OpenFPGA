//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: frac_lut4
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4 -----
module logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4(pReset,
                                                                                                                 frac_lut4_in,
                                                                                                                 enable,
                                                                                                                 address,
                                                                                                                 data_in,
                                                                                                                 frac_lut4_lut3_out,
                                                                                                                 frac_lut4_lut4_out);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:3] frac_lut4_in;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:4] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:1] frac_lut4_lut3_out;
//----- OUTPUT PORTS -----
output [0:0] frac_lut4_lut4_out;

//----- BEGIN wire-connection ports -----
wire [0:3] frac_lut4_in;
wire [0:1] frac_lut4_lut3_out;
wire [0:0] frac_lut4_lut4_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] frac_lut4_0_mode;
wire [0:0] frac_lut4_0_mode_inv;
wire [0:15] frac_lut4_0_sram;
wire [0:15] frac_lut4_0_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	frac_lut4 frac_lut4_0_ (
		.in(frac_lut4_in[0:3]),
		.sram(frac_lut4_0_sram[0:15]),
		.sram_inv(frac_lut4_0_sram_inv[0:15]),
		.mode(frac_lut4_0_mode),
		.mode_inv(frac_lut4_0_mode_inv),
		.lut3_out(frac_lut4_lut3_out[0:1]),
		.lut4_out(frac_lut4_lut4_out));

	frac_lut4_LATCHR_mem frac_lut4_LATCHR_mem (
		.pReset(pReset),
		.enable(enable),
		.address(address[0:4]),
		.data_in(data_in),
		.mem_out({frac_lut4_0_sram[0:15], frac_lut4_0_mode}),
		.mem_outb({frac_lut4_0_sram_inv[0:15], frac_lut4_0_mode_inv}));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4 -----

//----- Default net type -----
`default_nettype wire



