//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: io]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Grid Verilog module: grid_io_bottom -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_io_bottom -----
module grid_io_bottom(pReset,
                      gfpga_pad_GPIO_PAD,
                      top_width_0_height_0_subtile_0__pin_outpad_0_,
                      top_width_0_height_0_subtile_1__pin_outpad_0_,
                      top_width_0_height_0_subtile_2__pin_outpad_0_,
                      top_width_0_height_0_subtile_3__pin_outpad_0_,
                      top_width_0_height_0_subtile_4__pin_outpad_0_,
                      top_width_0_height_0_subtile_5__pin_outpad_0_,
                      top_width_0_height_0_subtile_6__pin_outpad_0_,
                      top_width_0_height_0_subtile_7__pin_outpad_0_,
                      enable,
                      address,
                      data_in,
                      top_width_0_height_0_subtile_0__pin_inpad_0_,
                      top_width_0_height_0_subtile_1__pin_inpad_0_,
                      top_width_0_height_0_subtile_2__pin_inpad_0_,
                      top_width_0_height_0_subtile_3__pin_inpad_0_,
                      top_width_0_height_0_subtile_4__pin_inpad_0_,
                      top_width_0_height_0_subtile_5__pin_inpad_0_,
                      top_width_0_height_0_subtile_6__pin_inpad_0_,
                      top_width_0_height_0_subtile_7__pin_inpad_0_);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GPIO PORTS -----
inout [0:7] gfpga_pad_GPIO_PAD;
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
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- INPUT PORTS -----
input [0:0] data_in;
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

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:7] decoder3to8_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_io_mode_io_ logical_tile_io_mode_io__0 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[0]),
		.io_outpad(top_width_0_height_0_subtile_0__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[0]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_0__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__1 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[1]),
		.io_outpad(top_width_0_height_0_subtile_1__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[1]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_1__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__2 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[2]),
		.io_outpad(top_width_0_height_0_subtile_2__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[2]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_2__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__3 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[3]),
		.io_outpad(top_width_0_height_0_subtile_3__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[3]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_3__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__4 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[4]),
		.io_outpad(top_width_0_height_0_subtile_4__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[4]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_4__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__5 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[5]),
		.io_outpad(top_width_0_height_0_subtile_5__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[5]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_5__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__6 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[6]),
		.io_outpad(top_width_0_height_0_subtile_6__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[6]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_6__pin_inpad_0_));

	logical_tile_io_mode_io_ logical_tile_io_mode_io__7 (
		.pReset(pReset),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[7]),
		.io_outpad(top_width_0_height_0_subtile_7__pin_outpad_0_),
		.enable(decoder3to8_0_data_out[7]),
		.address(address[0]),
		.data_in(data_in),
		.io_inpad(top_width_0_height_0_subtile_7__pin_inpad_0_));

	decoder3to8 decoder3to8_0_ (
		.enable(enable),
		.address(address[1:3]),
		.data_out(decoder3to8_0_data_out[0:7]));

endmodule
// ----- END Verilog module for grid_io_bottom -----

//----- Default net type -----
`default_nettype wire



// ----- END Grid Verilog module: grid_io_bottom -----

