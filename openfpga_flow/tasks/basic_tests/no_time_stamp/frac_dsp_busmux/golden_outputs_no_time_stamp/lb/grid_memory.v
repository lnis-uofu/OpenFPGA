//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: memory]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Grid Verilog module: grid_memory -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_memory -----
module grid_memory(clk,
                   top_width_0_height_0_subtile_0__pin_waddr_0_,
                   top_width_0_height_0_subtile_0__pin_raddr_1_,
                   top_width_0_height_0_subtile_0__pin_d_in_2_,
                   top_width_0_height_0_subtile_0__pin_clk_0_,
                   top_width_0_height_1_subtile_0__pin_waddr_1_,
                   top_width_0_height_1_subtile_0__pin_raddr_2_,
                   top_width_0_height_1_subtile_0__pin_d_in_3_,
                   right_width_0_height_0_subtile_0__pin_waddr_2_,
                   right_width_0_height_0_subtile_0__pin_raddr_3_,
                   right_width_0_height_0_subtile_0__pin_d_in_4_,
                   right_width_0_height_1_subtile_0__pin_waddr_3_,
                   right_width_0_height_1_subtile_0__pin_raddr_4_,
                   right_width_0_height_1_subtile_0__pin_d_in_5_,
                   bottom_width_0_height_0_subtile_0__pin_waddr_4_,
                   bottom_width_0_height_0_subtile_0__pin_raddr_5_,
                   bottom_width_0_height_0_subtile_0__pin_d_in_6_,
                   bottom_width_0_height_1_subtile_0__pin_waddr_5_,
                   bottom_width_0_height_1_subtile_0__pin_raddr_6_,
                   bottom_width_0_height_1_subtile_0__pin_d_in_7_,
                   left_width_0_height_0_subtile_0__pin_waddr_6_,
                   left_width_0_height_0_subtile_0__pin_d_in_0_,
                   left_width_0_height_0_subtile_0__pin_wen_0_,
                   left_width_0_height_1_subtile_0__pin_raddr_0_,
                   left_width_0_height_1_subtile_0__pin_d_in_1_,
                   left_width_0_height_1_subtile_0__pin_ren_0_,
                   top_width_0_height_0_subtile_0__pin_d_out_0_,
                   top_width_0_height_1_subtile_0__pin_d_out_1_,
                   right_width_0_height_0_subtile_0__pin_d_out_2_,
                   right_width_0_height_1_subtile_0__pin_d_out_3_,
                   bottom_width_0_height_0_subtile_0__pin_d_out_4_,
                   bottom_width_0_height_1_subtile_0__pin_d_out_5_,
                   left_width_0_height_0_subtile_0__pin_d_out_6_,
                   left_width_0_height_1_subtile_0__pin_d_out_7_);
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_waddr_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_raddr_1_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_d_in_2_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_clk_0_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_1_subtile_0__pin_waddr_1_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_1_subtile_0__pin_raddr_2_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_1_subtile_0__pin_d_in_3_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_waddr_2_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_raddr_3_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_d_in_4_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_1_subtile_0__pin_waddr_3_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_1_subtile_0__pin_raddr_4_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_1_subtile_0__pin_d_in_5_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_waddr_4_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_raddr_5_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_d_in_6_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_1_subtile_0__pin_waddr_5_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_1_subtile_0__pin_raddr_6_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_1_subtile_0__pin_d_in_7_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0_subtile_0__pin_waddr_6_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0_subtile_0__pin_d_in_0_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0_subtile_0__pin_wen_0_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_1_subtile_0__pin_raddr_0_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_1_subtile_0__pin_d_in_1_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_1_subtile_0__pin_ren_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0_subtile_0__pin_d_out_0_;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_1_subtile_0__pin_d_out_1_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_d_out_2_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_1_subtile_0__pin_d_out_3_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0_subtile_0__pin_d_out_4_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_1_subtile_0__pin_d_out_5_;
//----- OUTPUT PORTS -----
output [0:0] left_width_0_height_0_subtile_0__pin_d_out_6_;
//----- OUTPUT PORTS -----
output [0:0] left_width_0_height_1_subtile_0__pin_d_out_7_;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_memory_mode_memory_ logical_tile_memory_mode_memory__0 (
		.clk(clk),
		.memory_waddr({top_width_0_height_0_subtile_0__pin_waddr_0_, top_width_0_height_1_subtile_0__pin_waddr_1_, right_width_0_height_0_subtile_0__pin_waddr_2_, right_width_0_height_1_subtile_0__pin_waddr_3_, bottom_width_0_height_0_subtile_0__pin_waddr_4_, bottom_width_0_height_1_subtile_0__pin_waddr_5_, left_width_0_height_0_subtile_0__pin_waddr_6_}),
		.memory_raddr({left_width_0_height_1_subtile_0__pin_raddr_0_, top_width_0_height_0_subtile_0__pin_raddr_1_, top_width_0_height_1_subtile_0__pin_raddr_2_, right_width_0_height_0_subtile_0__pin_raddr_3_, right_width_0_height_1_subtile_0__pin_raddr_4_, bottom_width_0_height_0_subtile_0__pin_raddr_5_, bottom_width_0_height_1_subtile_0__pin_raddr_6_}),
		.memory_d_in({left_width_0_height_0_subtile_0__pin_d_in_0_, left_width_0_height_1_subtile_0__pin_d_in_1_, top_width_0_height_0_subtile_0__pin_d_in_2_, top_width_0_height_1_subtile_0__pin_d_in_3_, right_width_0_height_0_subtile_0__pin_d_in_4_, right_width_0_height_1_subtile_0__pin_d_in_5_, bottom_width_0_height_0_subtile_0__pin_d_in_6_, bottom_width_0_height_1_subtile_0__pin_d_in_7_}),
		.memory_wen(left_width_0_height_0_subtile_0__pin_wen_0_),
		.memory_ren(left_width_0_height_1_subtile_0__pin_ren_0_),
		.memory_clk(top_width_0_height_0_subtile_0__pin_clk_0_),
		.memory_d_out({top_width_0_height_0_subtile_0__pin_d_out_0_, top_width_0_height_1_subtile_0__pin_d_out_1_, right_width_0_height_0_subtile_0__pin_d_out_2_, right_width_0_height_1_subtile_0__pin_d_out_3_, bottom_width_0_height_0_subtile_0__pin_d_out_4_, bottom_width_0_height_1_subtile_0__pin_d_out_5_, left_width_0_height_0_subtile_0__pin_d_out_6_, left_width_0_height_1_subtile_0__pin_d_out_7_}));

endmodule
// ----- END Verilog module for grid_memory -----

//----- Default net type -----
`default_nettype wire



// ----- END Grid Verilog module: grid_memory -----

