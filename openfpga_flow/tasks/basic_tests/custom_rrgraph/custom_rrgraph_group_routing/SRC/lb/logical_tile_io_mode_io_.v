//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: io
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: io -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_io_mode_io_ -----
module logical_tile_io_mode_io_(gfpga_pad_GPIO_PAD,
                                io_outpad,
                                feedthrough_mem_in,
                                feedthrough_mem_inb,
                                io_inpad);
//----- GPIO PORTS -----
inout [0:0] gfpga_pad_GPIO_PAD;
//----- INPUT PORTS -----
input [0:0] io_outpad;
//----- INPUT PORTS -----
input [0:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [0:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [0:0] io_inpad;

//----- BEGIN wire-connection ports -----
wire [0:0] io_outpad;
wire [0:0] io_inpad;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_1_out;
wire [0:0] logical_tile_io_mode_physical__iopad_0_iopad_inpad;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_io_mode_physical__iopad logical_tile_io_mode_physical__iopad_0 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD),
		.iopad_outpad(direct_interc_1_out),
		.feedthrough_mem_in(feedthrough_mem_in),
		.feedthrough_mem_inb(feedthrough_mem_inb),
		.iopad_inpad(logical_tile_io_mode_physical__iopad_0_iopad_inpad));

	direct_interc direct_interc_0_ (
		.in(logical_tile_io_mode_physical__iopad_0_iopad_inpad),
		.out(io_inpad));

	direct_interc direct_interc_1_ (
		.in(io_outpad),
		.out(direct_interc_1_out));

endmodule
// ----- END Verilog module for logical_tile_io_mode_io_ -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: io -----
