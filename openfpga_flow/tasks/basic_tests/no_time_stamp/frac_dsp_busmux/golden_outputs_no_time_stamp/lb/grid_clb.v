//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: clb]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Grid Verilog module: grid_clb -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_clb -----
module grid_clb(pReset,
                set,
                reset,
                clk,
                top_width_0_height_0_subtile_0__pin_cin_0_,
                right_width_0_height_0_subtile_0__pin_I_0_,
                right_width_0_height_0_subtile_0__pin_I_1_,
                right_width_0_height_0_subtile_0__pin_I_2_,
                right_width_0_height_0_subtile_0__pin_I_3_,
                right_width_0_height_0_subtile_0__pin_I_4_,
                right_width_0_height_0_subtile_0__pin_I_5_,
                bottom_width_0_height_0_subtile_0__pin_I_6_,
                bottom_width_0_height_0_subtile_0__pin_I_7_,
                bottom_width_0_height_0_subtile_0__pin_I_8_,
                bottom_width_0_height_0_subtile_0__pin_I_9_,
                bottom_width_0_height_0_subtile_0__pin_I_10_,
                bottom_width_0_height_0_subtile_0__pin_I_11_,
                left_width_0_height_0_subtile_0__pin_clk_0_,
                enable,
                address,
                data_in,
                right_width_0_height_0_subtile_0__pin_O_0_,
                right_width_0_height_0_subtile_0__pin_O_1_,
                right_width_0_height_0_subtile_0__pin_O_2_,
                right_width_0_height_0_subtile_0__pin_O_3_,
                bottom_width_0_height_0_subtile_0__pin_O_4_,
                bottom_width_0_height_0_subtile_0__pin_O_5_,
                bottom_width_0_height_0_subtile_0__pin_O_6_,
                bottom_width_0_height_0_subtile_0__pin_O_7_,
                bottom_width_0_height_0_subtile_0__pin_cout_0_);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] set;
//----- GLOBAL PORTS -----
input [0:0] reset;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0_subtile_0__pin_cin_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_1_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_2_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_3_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_4_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0_subtile_0__pin_I_5_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_6_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_7_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_8_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_9_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_10_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0_subtile_0__pin_I_11_;
//----- INPUT PORTS -----
input [0:0] left_width_0_height_0_subtile_0__pin_clk_0_;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:13] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_0_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_1_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_2_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0_subtile_0__pin_O_3_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0_subtile_0__pin_O_4_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0_subtile_0__pin_O_5_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0_subtile_0__pin_O_6_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0_subtile_0__pin_O_7_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0_subtile_0__pin_cout_0_;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_clb_ logical_tile_clb_mode_clb__0 (
		.pReset(pReset),
		.set(set),
		.reset(reset),
		.clk(clk),
		.clb_I({right_width_0_height_0_subtile_0__pin_I_0_, right_width_0_height_0_subtile_0__pin_I_1_, right_width_0_height_0_subtile_0__pin_I_2_, right_width_0_height_0_subtile_0__pin_I_3_, right_width_0_height_0_subtile_0__pin_I_4_, right_width_0_height_0_subtile_0__pin_I_5_, bottom_width_0_height_0_subtile_0__pin_I_6_, bottom_width_0_height_0_subtile_0__pin_I_7_, bottom_width_0_height_0_subtile_0__pin_I_8_, bottom_width_0_height_0_subtile_0__pin_I_9_, bottom_width_0_height_0_subtile_0__pin_I_10_, bottom_width_0_height_0_subtile_0__pin_I_11_}),
		.clb_cin(top_width_0_height_0_subtile_0__pin_cin_0_),
		.clb_clk(left_width_0_height_0_subtile_0__pin_clk_0_),
		.enable(enable),
		.address(address[0:13]),
		.data_in(data_in),
		.clb_O({right_width_0_height_0_subtile_0__pin_O_0_, right_width_0_height_0_subtile_0__pin_O_1_, right_width_0_height_0_subtile_0__pin_O_2_, right_width_0_height_0_subtile_0__pin_O_3_, bottom_width_0_height_0_subtile_0__pin_O_4_, bottom_width_0_height_0_subtile_0__pin_O_5_, bottom_width_0_height_0_subtile_0__pin_O_6_, bottom_width_0_height_0_subtile_0__pin_O_7_}),
		.clb_cout(bottom_width_0_height_0_subtile_0__pin_cout_0_));

endmodule
// ----- END Verilog module for grid_clb -----

//----- Default net type -----
`default_nettype wire



// ----- END Grid Verilog module: grid_clb -----

