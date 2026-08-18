//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: iopad
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_io_mode_physical__iopad -----
module logical_tile_io_mode_physical__iopad(pReset,
                                            gfpga_pad_GPIO_PAD,
                                            iopad_outpad,
                                            enable,
                                            address,
                                            data_in,
                                            iopad_inpad);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GPIO PORTS -----
inout [0:0] gfpga_pad_GPIO_PAD;
//----- INPUT PORTS -----
input [0:0] iopad_outpad;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:0] iopad_inpad;

//----- BEGIN wire-connection ports -----
wire [0:0] iopad_outpad;
wire [0:0] iopad_inpad;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] GPIO_0_DIR;
wire [0:0] GPIO_LATCHR_mem_undriven_mem_outb;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	GPIO GPIO_0_ (
		.PAD(gfpga_pad_GPIO_PAD),
		.A(iopad_outpad),
		.DIR(GPIO_0_DIR),
		.Y(iopad_inpad));

	GPIO_LATCHR_mem GPIO_LATCHR_mem (
		.pReset(pReset),
		.enable(enable),
		.address(address),
		.data_in(data_in),
		.mem_out(GPIO_0_DIR),
		.mem_outb(GPIO_LATCHR_mem_undriven_mem_outb));

endmodule
// ----- END Verilog module for logical_tile_io_mode_physical__iopad -----

//----- Default net type -----
`default_nettype wire



