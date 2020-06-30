//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for logical tile: io]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for logical_tile_io_mode_physical__iopad -----
module logical_tile_io_mode_physical__iopad(pReset,
                                            prog_clk,
                                            gfpga_pad_iopad_pad,
                                            iopad_outpad,
                                            enable,
                                            address,
                                            data_in,
                                            iopad_inpad);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GPIO PORTS -----
inout [0:0] gfpga_pad_iopad_pad;
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


wire [0:0] iopad_0_en;
wire [0:0] iopad_config_latch_mem_undriven_mem_outb;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	iopad iopad_0_ (
		.pad(gfpga_pad_iopad_pad[0]),
		.outpad(iopad_outpad[0]),
		.en(iopad_0_en[0]),
		.inpad(iopad_inpad[0]));

	iopad_config_latch_mem iopad_config_latch_mem (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.enable(enable[0]),
		.address(address[0]),
		.data_in(data_in[0]),
		.mem_out(iopad_0_en[0]),
		.mem_outb(iopad_config_latch_mem_undriven_mem_outb[0]));

endmodule
// ----- END Verilog module for logical_tile_io_mode_physical__iopad -----



// ----- BEGIN Physical programmable logic block Verilog module: io -----
// ----- Verilog module for logical_tile_io_mode_io_ -----
module logical_tile_io_mode_io_(pReset,
                                prog_clk,
                                gfpga_pad_iopad_pad,
                                io_outpad,
                                enable,
                                address,
                                data_in,
                                io_inpad);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GPIO PORTS -----
inout [0:0] gfpga_pad_iopad_pad;
//----- INPUT PORTS -----
input [0:0] io_outpad;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- INPUT PORTS -----
input [0:0] data_in;
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
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[0]),
		.iopad_outpad(direct_interc_1_out[0]),
		.enable(enable[0]),
		.address(address[0]),
		.data_in(data_in[0]),
		.iopad_inpad(logical_tile_io_mode_physical__iopad_0_iopad_inpad[0]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_io_mode_physical__iopad_0_iopad_inpad[0]),
		.out(io_inpad[0]));

	direct_interc direct_interc_1_ (
		.in(io_outpad[0]),
		.out(direct_interc_1_out[0]));

endmodule
// ----- END Verilog module for logical_tile_io_mode_io_ -----


// ----- END Physical programmable logic block Verilog module: io -----


