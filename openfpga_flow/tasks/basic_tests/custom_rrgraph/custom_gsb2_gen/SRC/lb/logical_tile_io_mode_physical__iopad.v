//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: iopad
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_io_mode_physical__iopad -----
module logical_tile_io_mode_physical__iopad(gfpga_pad_GPIO_PAD,
                                            iopad_outpad,
                                            feedthrough_mem_in,
                                            feedthrough_mem_inb,
                                            iopad_inpad);
//----- GPIO PORTS -----
inout [0:0] gfpga_pad_GPIO_PAD;
//----- INPUT PORTS -----
input [0:0] iopad_outpad;
//----- INPUT PORTS -----
input [0:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [0:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [0:0] iopad_inpad;

//----- BEGIN wire-connection ports -----
wire [0:0] iopad_outpad;
wire [0:0] iopad_inpad;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] GPIO_0_DIR;
wire [0:0] GPIO_feedthrough_DFFR_mem_undriven_mem_outb;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	GPIO GPIO_0_ (
		.PAD(gfpga_pad_GPIO_PAD),
		.A(iopad_outpad),
		.DIR(GPIO_0_DIR),
		.Y(iopad_inpad));

	GPIO_feedthrough_DFFR_mem GPIO_feedthrough_DFFR_mem (
		.feedthrough_mem_in(feedthrough_mem_in),
		.feedthrough_mem_inb(feedthrough_mem_inb),
		.mem_out(GPIO_0_DIR),
		.mem_outb(GPIO_feedthrough_DFFR_mem_undriven_mem_outb));

endmodule
// ----- END Verilog module for logical_tile_io_mode_physical__iopad -----

//----- Default net type -----
`default_nettype wire



