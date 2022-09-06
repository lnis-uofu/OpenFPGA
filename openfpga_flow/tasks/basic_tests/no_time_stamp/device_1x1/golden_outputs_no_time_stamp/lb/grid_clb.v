//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: clb]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- BEGIN Grid Verilog module: grid_clb -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_clb -----
module grid_clb(prog_clk,
                set,
                reset,
                clk,
                top_width_0_height_0_subtile_0__pin_I_0_,
                top_width_0_height_0_subtile_0__pin_I_4_,
                top_width_0_height_0_subtile_0__pin_I_8_,
                right_width_0_height_0_subtile_0__pin_I_1_,
                right_width_0_height_0_subtile_0__pin_I_5_,
                right_width_0_height_0_subtile_0__pin_I_9_,
                bottom_width_0_height_0_subtile_0__pin_I_2_,
                bottom_width_0_height_0_subtile_0__pin_I_6_,
                bottom_width_0_height_0_subtile_0__pin_clk_0_,
                left_width_0_height_0_subtile_0__pin_I_3_,
                left_width_0_height_0_subtile_0__pin_I_7_,
                ccff_head,
                top_width_0_height_0_subtile_0__pin_O_2_,
                right_width_0_height_0_subtile_0__pin_O_3_,
                bottom_width_0_height_0_subtile_0__pin_O_0_,
                left_width_0_height_0_subtile_0__pin_O_1_,
                ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GLOBAL PORTS -----
input [0:0] set;
//----- GLOBAL PORTS -----
input [0:0] reset;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_4_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_8_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_1_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_5_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_9_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_2_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_6_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_clk_0_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0_subtile_0__pin_I_3_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0_subtile_0__pin_I_7_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_0__pin_O_2_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_3_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0_subtile_0__pin_O_0_;
//----- OUTPUT PORTS -----
output [0:0] left_width_0_height_0_subtile_0__pin_O_1_;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_clb_ logical_tile_clb_mode_clb__0 (
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.clb_I({top_width_0_height_0_subtile_0__pin_I_0_, right_width_0_height_0_subtile_0__pin_I_1_, bottom_width_0_height_0_subtile_0__pin_I_2_, left_width_0_height_0_subtile_0__pin_I_3_, top_width_0_height_0_subtile_0__pin_I_4_, right_width_0_height_0_subtile_0__pin_I_5_, bottom_width_0_height_0_subtile_0__pin_I_6_, left_width_0_height_0_subtile_0__pin_I_7_, top_width_0_height_0_subtile_0__pin_I_8_, right_width_0_height_0_subtile_0__pin_I_9_}),
		.clb_clk(bottom_width_0_height_0_subtile_0__pin_clk_0_),
		.ccff_head(ccff_head),
		.clb_O({bottom_width_0_height_0_subtile_0__pin_O_0_, left_width_0_height_0_subtile_0__pin_O_1_, top_width_0_height_0_subtile_0__pin_O_2_, right_width_0_height_0_subtile_0__pin_O_3_}),
		.ccff_tail(ccff_tail));

endmodule
// ----- END Verilog module for grid_clb -----

//----- Default net type -----
`default_nettype none



// ----- END Grid Verilog module: grid_clb -----

