//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: clb]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:40 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- BEGIN Grid Verilog module: grid_clb -----
// ----- Verilog module for grid_clb -----
module grid_clb(pReset,
                prog_clk,
                set,
                reset,
                clk,
                top_width_0_height_0__pin_0_,
                top_width_0_height_0__pin_4_,
                top_width_0_height_0__pin_8_,
                right_width_0_height_0__pin_1_,
                right_width_0_height_0__pin_5_,
                right_width_0_height_0__pin_9_,
                bottom_width_0_height_0__pin_2_,
                bottom_width_0_height_0__pin_6_,
                bottom_width_0_height_0__pin_14_,
                left_width_0_height_0__pin_3_,
                left_width_0_height_0__pin_7_,
                enable,
                address,
                data_in,
                top_width_0_height_0__pin_12_,
                right_width_0_height_0__pin_13_,
                bottom_width_0_height_0__pin_10_,
                left_width_0_height_0__pin_11_);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GLOBAL PORTS -----
input [0:0] set;
//----- GLOBAL PORTS -----
input [0:0] reset;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_4_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_8_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_1_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_5_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_9_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_2_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_6_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_14_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0__pin_3_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0__pin_7_;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:9] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0__pin_12_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_13_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_10_;
//----- OUTPUT PORTS -----
output [0:0] left_width_0_height_0__pin_11_;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_clb_ logical_tile_clb_mode_clb__0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.set(set[0]),
		.reset(reset[0]),
		.clk(clk[0]),
		.clb_I({top_width_0_height_0__pin_0_[0], right_width_0_height_0__pin_1_[0], bottom_width_0_height_0__pin_2_[0], left_width_0_height_0__pin_3_[0], top_width_0_height_0__pin_4_[0], right_width_0_height_0__pin_5_[0], bottom_width_0_height_0__pin_6_[0], left_width_0_height_0__pin_7_[0], top_width_0_height_0__pin_8_[0], right_width_0_height_0__pin_9_[0]}),
		.clb_clk(bottom_width_0_height_0__pin_14_[0]),
		.enable(enable[0]),
		.address(address[0:9]),
		.data_in(data_in[0]),
		.clb_O({bottom_width_0_height_0__pin_10_[0], left_width_0_height_0__pin_11_[0], top_width_0_height_0__pin_12_[0], right_width_0_height_0__pin_13_[0]}));

endmodule
// ----- END Verilog module for grid_clb -----


// ----- END Grid Verilog module: grid_clb -----

