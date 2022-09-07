//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: iopad
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_io_mode_physical__iopad -----
module logical_tile_io_mode_physical__iopad(prog_clk,
                                            gfpga_pad_GPIO_PAD,
                                            iopad_outpad,
                                            ccff_head,
                                            iopad_inpad,
                                            ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GPIO PORTS -----
inout [0:0] gfpga_pad_GPIO_PAD;
//----- INPUT PORTS -----
input [0:0] iopad_outpad;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] iopad_inpad;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:0] iopad_outpad;
wire [0:0] iopad_inpad;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] GPIO_0_DIR;
wire [0:0] GPIO_DFF_mem_undriven_mem_outb;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	GPIO GPIO_0_ (
		.PAD(gfpga_pad_GPIO_PAD),
		.A(iopad_outpad),
		.DIR(GPIO_0_DIR),
		.Y(iopad_inpad));

	GPIO_DFF_mem GPIO_DFF_mem (
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(ccff_tail),
		.mem_out(GPIO_0_DIR),
		.mem_outb(GPIO_DFF_mem_undriven_mem_outb));

endmodule
// ----- END Verilog module for logical_tile_io_mode_physical__iopad -----

//----- Default net type -----
`default_nettype none



