//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: clb_tile]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Grid Verilog module: grid_clb_tile -----
// ----- Verilog module for grid_clb_tile -----
module grid_clb_tile(pReset,
                     prog_clk,
                     set,
                     reset,
                     clk,
                     top_width_0_height_0_subtile_0__pin_I_0_,
                     top_width_0_height_0_subtile_0__pin_I_1_,
                     top_width_0_height_0_subtile_0__pin_I_2_,
                     top_width_0_height_0_subtile_0__pin_I_3_,
                     top_width_0_height_0_subtile_0__pin_I_4_,
                     top_width_0_height_0_subtile_0__pin_I_5_,
                     top_width_0_height_0_subtile_0__pin_I_6_,
                     top_width_0_height_0_subtile_0__pin_I_7_,
                     top_width_0_height_0_subtile_0__pin_I_8_,
                     top_width_0_height_0_subtile_0__pin_I_9_,
                     top_width_0_height_0_subtile_0__pin_I_10_,
                     top_width_0_height_0_subtile_0__pin_I_11_,
                     top_width_0_height_0_subtile_0__pin_clk_0_,
                     ccff_head,
                     right_width_0_height_0_subtile_0__pin_O_0_,
                     right_width_0_height_0_subtile_0__pin_O_1_,
                     right_width_0_height_0_subtile_0__pin_O_2_,
                     right_width_0_height_0_subtile_0__pin_O_3_,
                     right_width_0_height_0_subtile_0__pin_O_4_,
                     right_width_0_height_0_subtile_0__pin_O_5_,
                     right_width_0_height_0_subtile_0__pin_O_6_,
                     right_width_0_height_0_subtile_0__pin_O_7_,
                     ccff_tail);
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
input [0:0] top_width_0_height_0_subtile_0__pin_I_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_1_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_2_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_3_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_4_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_5_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_6_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_7_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_8_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_9_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_10_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_I_11_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_clk_0_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_1_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_2_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_3_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_4_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_5_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_6_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_7_;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_clb_ logical_tile_clb_mode_clb__0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.clb_I({top_width_0_height_0_subtile_0__pin_I_11_, top_width_0_height_0_subtile_0__pin_I_10_, top_width_0_height_0_subtile_0__pin_I_9_, top_width_0_height_0_subtile_0__pin_I_8_, top_width_0_height_0_subtile_0__pin_I_7_, top_width_0_height_0_subtile_0__pin_I_6_, top_width_0_height_0_subtile_0__pin_I_5_, top_width_0_height_0_subtile_0__pin_I_4_, top_width_0_height_0_subtile_0__pin_I_3_, top_width_0_height_0_subtile_0__pin_I_2_, top_width_0_height_0_subtile_0__pin_I_1_, top_width_0_height_0_subtile_0__pin_I_0_}),
		.clb_clk(top_width_0_height_0_subtile_0__pin_clk_0_),
		.ccff_head(ccff_head),
		.clb_O({right_width_0_height_0_subtile_0__pin_O_7_, right_width_0_height_0_subtile_0__pin_O_6_, right_width_0_height_0_subtile_0__pin_O_5_, right_width_0_height_0_subtile_0__pin_O_4_, right_width_0_height_0_subtile_0__pin_O_3_, right_width_0_height_0_subtile_0__pin_O_2_, right_width_0_height_0_subtile_0__pin_O_1_, right_width_0_height_0_subtile_0__pin_O_0_}),
		.ccff_tail(ccff_tail));

endmodule
// ----- END Verilog module for grid_clb_tile -----



// ----- END Grid Verilog module: grid_clb_tile -----

