//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: io]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- BEGIN Grid Verilog module: grid_io_left -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_io_left -----
module grid_io_left(prog_clk,
                    gfpga_pad_GPIO_PAD,
                    right_width_0_height_0_subtile_0__pin_outpad_0_,
                    right_width_0_height_0_subtile_1__pin_outpad_0_,
                    right_width_0_height_0_subtile_2__pin_outpad_0_,
                    right_width_0_height_0_subtile_3__pin_outpad_0_,
                    right_width_0_height_0_subtile_4__pin_outpad_0_,
                    right_width_0_height_0_subtile_5__pin_outpad_0_,
                    right_width_0_height_0_subtile_6__pin_outpad_0_,
                    right_width_0_height_0_subtile_7__pin_outpad_0_,
                    ccff_head,
                    right_width_0_height_0_subtile_0__pin_inpad_0_,
                    right_width_0_height_0_subtile_1__pin_inpad_0_,
                    right_width_0_height_0_subtile_2__pin_inpad_0_,
                    right_width_0_height_0_subtile_3__pin_inpad_0_,
                    right_width_0_height_0_subtile_4__pin_inpad_0_,
                    right_width_0_height_0_subtile_5__pin_inpad_0_,
                    right_width_0_height_0_subtile_6__pin_inpad_0_,
                    right_width_0_height_0_subtile_7__pin_inpad_0_,
                    ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GPIO PORTS -----
inout [0:7] gfpga_pad_GPIO_PAD;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_1__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_2__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_3__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_4__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_5__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_6__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_7__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_1__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_2__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_3__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_4__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_5__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_6__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_7__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] logical_tile_io_mode_io__0_ccff_tail;
wire [0:0] logical_tile_io_mode_io__1_ccff_tail;
wire [0:0] logical_tile_io_mode_io__2_ccff_tail;
wire [0:0] logical_tile_io_mode_io__3_ccff_tail;
wire [0:0] logical_tile_io_mode_io__4_ccff_tail;
wire [0:0] logical_tile_io_mode_io__5_ccff_tail;
wire [0:0] logical_tile_io_mode_io__6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_io_mode_io_ logical_tile_io_mode_io__0 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[0]),
		.io_outpad(right_width_0_height_0_subtile_0__pin_outpad_0_),
		.ccff_head(ccff_head),
		.io_inpad(right_width_0_height_0_subtile_0__pin_inpad_0_),
		.ccff_tail(logical_tile_io_mode_io__0_ccff_tail));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__1 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[1]),
		.io_outpad(right_width_0_height_0_subtile_1__pin_outpad_0_),
		.ccff_head(logical_tile_io_mode_io__0_ccff_tail),
		.io_inpad(right_width_0_height_0_subtile_1__pin_inpad_0_),
		.ccff_tail(logical_tile_io_mode_io__1_ccff_tail));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__2 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[2]),
		.io_outpad(right_width_0_height_0_subtile_2__pin_outpad_0_),
		.ccff_head(logical_tile_io_mode_io__1_ccff_tail),
		.io_inpad(right_width_0_height_0_subtile_2__pin_inpad_0_),
		.ccff_tail(logical_tile_io_mode_io__2_ccff_tail));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__3 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[3]),
		.io_outpad(right_width_0_height_0_subtile_3__pin_outpad_0_),
		.ccff_head(logical_tile_io_mode_io__2_ccff_tail),
		.io_inpad(right_width_0_height_0_subtile_3__pin_inpad_0_),
		.ccff_tail(logical_tile_io_mode_io__3_ccff_tail));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__4 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[4]),
		.io_outpad(right_width_0_height_0_subtile_4__pin_outpad_0_),
		.ccff_head(logical_tile_io_mode_io__3_ccff_tail),
		.io_inpad(right_width_0_height_0_subtile_4__pin_inpad_0_),
		.ccff_tail(logical_tile_io_mode_io__4_ccff_tail));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__5 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[5]),
		.io_outpad(right_width_0_height_0_subtile_5__pin_outpad_0_),
		.ccff_head(logical_tile_io_mode_io__4_ccff_tail),
		.io_inpad(right_width_0_height_0_subtile_5__pin_inpad_0_),
		.ccff_tail(logical_tile_io_mode_io__5_ccff_tail));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__6 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[6]),
		.io_outpad(right_width_0_height_0_subtile_6__pin_outpad_0_),
		.ccff_head(logical_tile_io_mode_io__5_ccff_tail),
		.io_inpad(right_width_0_height_0_subtile_6__pin_inpad_0_),
		.ccff_tail(logical_tile_io_mode_io__6_ccff_tail));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__7 (
		.prog_clk(prog_clk),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[7]),
		.io_outpad(right_width_0_height_0_subtile_7__pin_outpad_0_),
		.ccff_head(logical_tile_io_mode_io__6_ccff_tail),
		.io_inpad(right_width_0_height_0_subtile_7__pin_inpad_0_),
		.ccff_tail(ccff_tail));

endmodule
// ----- END Verilog module for grid_io_left -----

//----- Default net type -----
`default_nettype none



// ----- END Grid Verilog module: grid_io_left -----

