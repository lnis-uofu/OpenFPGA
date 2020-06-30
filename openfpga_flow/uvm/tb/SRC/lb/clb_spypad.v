//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for physical tile: clb_spypad]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Sun May  3 00:45:58 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- BEGIN Grid Verilog module: grid_clb_spypad -----
// ----- Verilog module for grid_clb_spypad -----
module grid_clb_spypad(pReset,
                       prog_clk,
                       Reset,
                       Test_en,
                       clk,
                       gfpga_pad_frac_lut6_spypad_lut4_spy,
                       gfpga_pad_frac_lut6_spypad_lut5_spy,
                       gfpga_pad_frac_lut6_spypad_lut6_spy,
                       top_width_0_height_0__pin_40_,
                       top_width_0_height_0__pin_41_,
                       top_width_0_height_0__pin_42_,
                       top_width_0_height_0__pin_43_,
                       top_width_0_height_0__pin_69_,
                       right_width_0_height_0__pin_0_,
                       right_width_0_height_0__pin_1_,
                       right_width_0_height_0__pin_2_,
                       right_width_0_height_0__pin_3_,
                       right_width_0_height_0__pin_4_,
                       right_width_0_height_0__pin_5_,
                       right_width_0_height_0__pin_6_,
                       right_width_0_height_0__pin_7_,
                       right_width_0_height_0__pin_8_,
                       right_width_0_height_0__pin_9_,
                       right_width_0_height_0__pin_10_,
                       right_width_0_height_0__pin_11_,
                       right_width_0_height_0__pin_12_,
                       right_width_0_height_0__pin_13_,
                       right_width_0_height_0__pin_14_,
                       right_width_0_height_0__pin_15_,
                       right_width_0_height_0__pin_16_,
                       right_width_0_height_0__pin_17_,
                       right_width_0_height_0__pin_18_,
                       right_width_0_height_0__pin_19_,
                       bottom_width_0_height_0__pin_20_,
                       bottom_width_0_height_0__pin_21_,
                       bottom_width_0_height_0__pin_22_,
                       bottom_width_0_height_0__pin_23_,
                       bottom_width_0_height_0__pin_24_,
                       bottom_width_0_height_0__pin_25_,
                       bottom_width_0_height_0__pin_26_,
                       bottom_width_0_height_0__pin_27_,
                       bottom_width_0_height_0__pin_28_,
                       bottom_width_0_height_0__pin_29_,
                       bottom_width_0_height_0__pin_30_,
                       bottom_width_0_height_0__pin_31_,
                       bottom_width_0_height_0__pin_32_,
                       bottom_width_0_height_0__pin_33_,
                       bottom_width_0_height_0__pin_34_,
                       bottom_width_0_height_0__pin_35_,
                       bottom_width_0_height_0__pin_36_,
                       bottom_width_0_height_0__pin_37_,
                       bottom_width_0_height_0__pin_38_,
                       bottom_width_0_height_0__pin_39_,
                       ccff_head,
                       top_width_0_height_0__pin_68_,
                       right_width_0_height_0__pin_44_upper,
                       right_width_0_height_0__pin_44_lower,
                       right_width_0_height_0__pin_45_upper,
                       right_width_0_height_0__pin_45_lower,
                       right_width_0_height_0__pin_46_upper,
                       right_width_0_height_0__pin_46_lower,
                       right_width_0_height_0__pin_47_upper,
                       right_width_0_height_0__pin_47_lower,
                       right_width_0_height_0__pin_48_upper,
                       right_width_0_height_0__pin_48_lower,
                       right_width_0_height_0__pin_49_upper,
                       right_width_0_height_0__pin_49_lower,
                       right_width_0_height_0__pin_50_upper,
                       right_width_0_height_0__pin_50_lower,
                       right_width_0_height_0__pin_51_upper,
                       right_width_0_height_0__pin_51_lower,
                       right_width_0_height_0__pin_52_upper,
                       right_width_0_height_0__pin_52_lower,
                       right_width_0_height_0__pin_53_upper,
                       right_width_0_height_0__pin_53_lower,
                       bottom_width_0_height_0__pin_54_upper,
                       bottom_width_0_height_0__pin_54_lower,
                       bottom_width_0_height_0__pin_55_upper,
                       bottom_width_0_height_0__pin_55_lower,
                       bottom_width_0_height_0__pin_56_upper,
                       bottom_width_0_height_0__pin_56_lower,
                       bottom_width_0_height_0__pin_57_upper,
                       bottom_width_0_height_0__pin_57_lower,
                       bottom_width_0_height_0__pin_58_upper,
                       bottom_width_0_height_0__pin_58_lower,
                       bottom_width_0_height_0__pin_59_upper,
                       bottom_width_0_height_0__pin_59_lower,
                       bottom_width_0_height_0__pin_60_upper,
                       bottom_width_0_height_0__pin_60_lower,
                       bottom_width_0_height_0__pin_61_upper,
                       bottom_width_0_height_0__pin_61_lower,
                       bottom_width_0_height_0__pin_62_upper,
                       bottom_width_0_height_0__pin_62_lower,
                       bottom_width_0_height_0__pin_63_upper,
                       bottom_width_0_height_0__pin_63_lower,
                       bottom_width_0_height_0__pin_64_,
                       bottom_width_0_height_0__pin_65_,
                       bottom_width_0_height_0__pin_66_upper,
                       bottom_width_0_height_0__pin_66_lower,
                       bottom_width_0_height_0__pin_67_upper,
                       bottom_width_0_height_0__pin_67_lower,
                       ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GLOBAL PORTS -----
input [0:0] Reset;
//----- GLOBAL PORTS -----
input [0:0] Test_en;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- GPOUT PORTS -----
output [0:3] gfpga_pad_frac_lut6_spypad_lut4_spy;
//----- GPOUT PORTS -----
output [0:1] gfpga_pad_frac_lut6_spypad_lut5_spy;
//----- GPOUT PORTS -----
output [0:0] gfpga_pad_frac_lut6_spypad_lut6_spy;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_40_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_41_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_42_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_43_;
//----- INPUT PORTS -----
input [0:0] top_width_0_height_0__pin_69_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_0_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_1_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_2_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_3_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_4_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_5_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_6_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_7_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_8_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_9_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_10_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_11_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_12_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_13_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_14_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_15_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_16_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_17_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_18_;
//----- INPUT PORTS -----
input [0:0] right_width_0_height_0__pin_19_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_20_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_21_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_22_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_23_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_24_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_25_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_26_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_27_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_28_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_29_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_30_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_31_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_32_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_33_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_34_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_35_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_36_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_37_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_38_;
//----- INPUT PORTS -----
input [0:0] bottom_width_0_height_0__pin_39_;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] top_width_0_height_0__pin_68_;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_44_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_44_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_45_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_45_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_46_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_46_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_47_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_47_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_48_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_48_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_49_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_49_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_50_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_50_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_51_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_51_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_52_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_52_lower;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_53_upper;
//----- OUTPUT PORTS -----
output [0:0] right_width_0_height_0__pin_53_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_54_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_54_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_55_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_55_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_56_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_56_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_57_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_57_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_58_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_58_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_59_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_59_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_60_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_60_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_61_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_61_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_62_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_62_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_63_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_63_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_64_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_65_;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_66_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_66_lower;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_67_upper;
//----- OUTPUT PORTS -----
output [0:0] bottom_width_0_height_0__pin_67_lower;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign right_width_0_height_0__pin_44_lower[0] = right_width_0_height_0__pin_44_upper[0];
	assign right_width_0_height_0__pin_45_lower[0] = right_width_0_height_0__pin_45_upper[0];
	assign right_width_0_height_0__pin_46_lower[0] = right_width_0_height_0__pin_46_upper[0];
	assign right_width_0_height_0__pin_47_lower[0] = right_width_0_height_0__pin_47_upper[0];
	assign right_width_0_height_0__pin_48_lower[0] = right_width_0_height_0__pin_48_upper[0];
	assign right_width_0_height_0__pin_49_lower[0] = right_width_0_height_0__pin_49_upper[0];
	assign right_width_0_height_0__pin_50_lower[0] = right_width_0_height_0__pin_50_upper[0];
	assign right_width_0_height_0__pin_51_lower[0] = right_width_0_height_0__pin_51_upper[0];
	assign right_width_0_height_0__pin_52_lower[0] = right_width_0_height_0__pin_52_upper[0];
	assign right_width_0_height_0__pin_53_lower[0] = right_width_0_height_0__pin_53_upper[0];
	assign bottom_width_0_height_0__pin_54_lower[0] = bottom_width_0_height_0__pin_54_upper[0];
	assign bottom_width_0_height_0__pin_55_lower[0] = bottom_width_0_height_0__pin_55_upper[0];
	assign bottom_width_0_height_0__pin_56_lower[0] = bottom_width_0_height_0__pin_56_upper[0];
	assign bottom_width_0_height_0__pin_57_lower[0] = bottom_width_0_height_0__pin_57_upper[0];
	assign bottom_width_0_height_0__pin_58_lower[0] = bottom_width_0_height_0__pin_58_upper[0];
	assign bottom_width_0_height_0__pin_59_lower[0] = bottom_width_0_height_0__pin_59_upper[0];
	assign bottom_width_0_height_0__pin_60_lower[0] = bottom_width_0_height_0__pin_60_upper[0];
	assign bottom_width_0_height_0__pin_61_lower[0] = bottom_width_0_height_0__pin_61_upper[0];
	assign bottom_width_0_height_0__pin_62_lower[0] = bottom_width_0_height_0__pin_62_upper[0];
	assign bottom_width_0_height_0__pin_63_lower[0] = bottom_width_0_height_0__pin_63_upper[0];
	assign bottom_width_0_height_0__pin_66_lower[0] = bottom_width_0_height_0__pin_66_upper[0];
	assign bottom_width_0_height_0__pin_67_lower[0] = bottom_width_0_height_0__pin_67_upper[0];
// ----- END Local output short connections -----

	logical_tile_clb_spypad_mode_clb_spypad_ logical_tile_clb_spypad_mode_clb_spypad__0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.gfpga_pad_frac_lut6_spypad_lut4_spy(gfpga_pad_frac_lut6_spypad_lut4_spy[0:3]),
		.gfpga_pad_frac_lut6_spypad_lut5_spy(gfpga_pad_frac_lut6_spypad_lut5_spy[0:1]),
		.gfpga_pad_frac_lut6_spypad_lut6_spy(gfpga_pad_frac_lut6_spypad_lut6_spy[0]),
		.clb_spypad_I0({right_width_0_height_0__pin_0_[0], right_width_0_height_0__pin_1_[0], right_width_0_height_0__pin_2_[0], right_width_0_height_0__pin_3_[0], right_width_0_height_0__pin_4_[0], right_width_0_height_0__pin_5_[0], right_width_0_height_0__pin_6_[0], right_width_0_height_0__pin_7_[0], right_width_0_height_0__pin_8_[0], right_width_0_height_0__pin_9_[0]}),
		.clb_spypad_I1({right_width_0_height_0__pin_10_[0], right_width_0_height_0__pin_11_[0], right_width_0_height_0__pin_12_[0], right_width_0_height_0__pin_13_[0], right_width_0_height_0__pin_14_[0], right_width_0_height_0__pin_15_[0], right_width_0_height_0__pin_16_[0], right_width_0_height_0__pin_17_[0], right_width_0_height_0__pin_18_[0], right_width_0_height_0__pin_19_[0]}),
		.clb_spypad_I2({bottom_width_0_height_0__pin_20_[0], bottom_width_0_height_0__pin_21_[0], bottom_width_0_height_0__pin_22_[0], bottom_width_0_height_0__pin_23_[0], bottom_width_0_height_0__pin_24_[0], bottom_width_0_height_0__pin_25_[0], bottom_width_0_height_0__pin_26_[0], bottom_width_0_height_0__pin_27_[0], bottom_width_0_height_0__pin_28_[0], bottom_width_0_height_0__pin_29_[0]}),
		.clb_spypad_I3({bottom_width_0_height_0__pin_30_[0], bottom_width_0_height_0__pin_31_[0], bottom_width_0_height_0__pin_32_[0], bottom_width_0_height_0__pin_33_[0], bottom_width_0_height_0__pin_34_[0], bottom_width_0_height_0__pin_35_[0], bottom_width_0_height_0__pin_36_[0], bottom_width_0_height_0__pin_37_[0], bottom_width_0_height_0__pin_38_[0], bottom_width_0_height_0__pin_39_[0]}),
		.clb_spypad_sc_in(top_width_0_height_0__pin_40_[0]),
		.clb_spypad_cin(top_width_0_height_0__pin_41_[0]),
		.clb_spypad_cin_trick(top_width_0_height_0__pin_42_[0]),
		.clb_spypad_regin(top_width_0_height_0__pin_43_[0]),
		.clb_spypad_clk(top_width_0_height_0__pin_69_[0]),
		.ccff_head(ccff_head[0]),
		.clb_spypad_O({right_width_0_height_0__pin_44_upper[0], right_width_0_height_0__pin_45_upper[0], right_width_0_height_0__pin_46_upper[0], right_width_0_height_0__pin_47_upper[0], right_width_0_height_0__pin_48_upper[0], right_width_0_height_0__pin_49_upper[0], right_width_0_height_0__pin_50_upper[0], right_width_0_height_0__pin_51_upper[0], right_width_0_height_0__pin_52_upper[0], right_width_0_height_0__pin_53_upper[0], bottom_width_0_height_0__pin_54_upper[0], bottom_width_0_height_0__pin_55_upper[0], bottom_width_0_height_0__pin_56_upper[0], bottom_width_0_height_0__pin_57_upper[0], bottom_width_0_height_0__pin_58_upper[0], bottom_width_0_height_0__pin_59_upper[0], bottom_width_0_height_0__pin_60_upper[0], bottom_width_0_height_0__pin_61_upper[0], bottom_width_0_height_0__pin_62_upper[0], bottom_width_0_height_0__pin_63_upper[0]}),
		.clb_spypad_sc_out(bottom_width_0_height_0__pin_64_[0]),
		.clb_spypad_cout(bottom_width_0_height_0__pin_65_[0]),
		.clb_spypad_cout_copy(bottom_width_0_height_0__pin_66_upper[0]),
		.clb_spypad_regout(bottom_width_0_height_0__pin_67_upper[0]),
		.clb_spypad_lut_perf(top_width_0_height_0__pin_68_[0]),
		.ccff_tail(ccff_tail[0]));

endmodule
// ----- END Verilog module for grid_clb_spypad -----


// ----- END Grid Verilog module: grid_clb_spypad -----

