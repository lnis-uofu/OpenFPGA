//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: io_right_tile]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Grid Verilog module: grid_io_right_tile -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_io_right_tile -----
module grid_io_right_tile(pReset,
                          prog_clk,
                          gfpga_pad_GPIO_PAD,
                          top_width_0_height_0_subtile_0__pin_outpad_0_,
                          top_width_0_height_0_subtile_1__pin_outpad_0_,
                          top_width_0_height_0_subtile_2__pin_outpad_0_,
                          top_width_0_height_0_subtile_3__pin_outpad_0_,
                          top_width_0_height_0_subtile_4__pin_outpad_0_,
                          top_width_0_height_0_subtile_5__pin_outpad_0_,
                          top_width_0_height_0_subtile_6__pin_outpad_0_,
                          top_width_0_height_0_subtile_7__pin_outpad_0_,
                          ccff_head,
                          top_width_0_height_0_subtile_0__pin_inpad_0_,
                          top_width_0_height_0_subtile_1__pin_inpad_0_,
                          top_width_0_height_0_subtile_2__pin_inpad_0_,
                          top_width_0_height_0_subtile_3__pin_inpad_0_,
                          top_width_0_height_0_subtile_4__pin_inpad_0_,
                          top_width_0_height_0_subtile_5__pin_inpad_0_,
                          top_width_0_height_0_subtile_6__pin_inpad_0_,
                          top_width_0_height_0_subtile_7__pin_inpad_0_,
                          ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GPIO PORTS -----
inout [7:0] gfpga_pad_GPIO_PAD;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_1__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_2__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_3__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_4__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_5__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_6__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_7__pin_outpad_0_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_0__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_1__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_2__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_3__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_4__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_5__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_6__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_7__pin_inpad_0_;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [7:0] grid_io_right_tile_config_group_mem_size8_0_mem_out;
wire [7:0] grid_io_right_tile_config_group_mem_size8_0_mem_outb;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_io_mode_io_ logical_tile_io_mode_io__0 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[0]),
		.io_outpad(top_width_0_height_0_subtile_0__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[0]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[0]),
		.io_inpad(top_width_0_height_0_subtile_0__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__1 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[1]),
		.io_outpad(top_width_0_height_0_subtile_1__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[1]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[1]),
		.io_inpad(top_width_0_height_0_subtile_1__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__2 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[2]),
		.io_outpad(top_width_0_height_0_subtile_2__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[2]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[2]),
		.io_inpad(top_width_0_height_0_subtile_2__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__3 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[3]),
		.io_outpad(top_width_0_height_0_subtile_3__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[3]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[3]),
		.io_inpad(top_width_0_height_0_subtile_3__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__4 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[4]),
		.io_outpad(top_width_0_height_0_subtile_4__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[4]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[4]),
		.io_inpad(top_width_0_height_0_subtile_4__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__5 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[5]),
		.io_outpad(top_width_0_height_0_subtile_5__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[5]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[5]),
		.io_inpad(top_width_0_height_0_subtile_5__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__6 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[6]),
		.io_outpad(top_width_0_height_0_subtile_6__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[6]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[6]),
		.io_inpad(top_width_0_height_0_subtile_6__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__7 (
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[7]),
		.io_outpad(top_width_0_height_0_subtile_7__pin_outpad_0_),
		.feedthrough_mem_in(grid_io_right_tile_config_group_mem_size8_0_mem_out[7]),
		.feedthrough_mem_inb(grid_io_right_tile_config_group_mem_size8_0_mem_outb[7]),
		.io_inpad(top_width_0_height_0_subtile_7__pin_inpad_0_));

	grid_io_right_tile_config_group_mem_size8 grid_io_right_tile_config_group_mem_size8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.mem_out({grid_io_right_tile_config_group_mem_size8_0_mem_out[7], grid_io_right_tile_config_group_mem_size8_0_mem_out[6], grid_io_right_tile_config_group_mem_size8_0_mem_out[5], grid_io_right_tile_config_group_mem_size8_0_mem_out[4], grid_io_right_tile_config_group_mem_size8_0_mem_out[3], grid_io_right_tile_config_group_mem_size8_0_mem_out[2], grid_io_right_tile_config_group_mem_size8_0_mem_out[1], grid_io_right_tile_config_group_mem_size8_0_mem_out[0]}),
		.mem_outb({grid_io_right_tile_config_group_mem_size8_0_mem_outb[7], grid_io_right_tile_config_group_mem_size8_0_mem_outb[6], grid_io_right_tile_config_group_mem_size8_0_mem_outb[5], grid_io_right_tile_config_group_mem_size8_0_mem_outb[4], grid_io_right_tile_config_group_mem_size8_0_mem_outb[3], grid_io_right_tile_config_group_mem_size8_0_mem_outb[2], grid_io_right_tile_config_group_mem_size8_0_mem_outb[1], grid_io_right_tile_config_group_mem_size8_0_mem_outb[0]}),
		.ccff_tail(ccff_tail));

endmodule
// ----- END Verilog module for grid_io_right_tile -----

//----- Default net type -----
`default_nettype wire



// ----- END Grid Verilog module: grid_io_right_tile -----

