//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for logical tile: clb_spypad]
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Sun May  3 00:45:58 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6 -----
module logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6(pReset,
                                                                                                   prog_clk,
                                                                                                   frac_lut6_in,
                                                                                                   ccff_head,
                                                                                                   frac_lut6_lut4_out,
                                                                                                   frac_lut6_lut5_out,
                                                                                                   frac_lut6_lut6_out,
                                                                                                   ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:5] frac_lut6_in;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:3] frac_lut6_lut4_out;
//----- OUTPUT PORTS -----
output [0:1] frac_lut6_lut5_out;
//----- OUTPUT PORTS -----
output [0:0] frac_lut6_lut6_out;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:5] frac_lut6_in;
wire [0:3] frac_lut6_lut4_out;
wire [0:1] frac_lut6_lut5_out;
wire [0:0] frac_lut6_lut6_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] frac_lut6_0_mode;
wire [0:1] frac_lut6_0_mode_inv;
wire [0:63] frac_lut6_0_sram;
wire [0:63] frac_lut6_0_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	frac_lut6 frac_lut6_0_ (
		.in(frac_lut6_in[0:5]),
		.sram(frac_lut6_0_sram[0:63]),
		.sram_inv(frac_lut6_0_sram_inv[0:63]),
		.mode(frac_lut6_0_mode[0:1]),
		.mode_inv(frac_lut6_0_mode_inv[0:1]),
		.lut4_out(frac_lut6_lut4_out[0:3]),
		.lut5_out(frac_lut6_lut5_out[0:1]),
		.lut6_out(frac_lut6_lut6_out[0]));

	frac_lut6_CCFFX1_mem frac_lut6_CCFFX1_mem (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(ccff_head[0]),
		.ccff_tail(ccff_tail[0]),
		.mem_out({frac_lut6_0_sram[0:63], frac_lut6_0_mode[0:1]}),
		.mem_outb({frac_lut6_0_sram_inv[0:63], frac_lut6_0_mode_inv[0:1]}));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6 -----



// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy -----
module logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy(adder_phy_a,
                                                                                                   adder_phy_b,
                                                                                                   adder_phy_cin,
                                                                                                   adder_phy_cout,
                                                                                                   adder_phy_sumout);
//----- INPUT PORTS -----
input [0:0] adder_phy_a;
//----- INPUT PORTS -----
input [0:0] adder_phy_b;
//----- INPUT PORTS -----
input [0:0] adder_phy_cin;
//----- OUTPUT PORTS -----
output [0:0] adder_phy_cout;
//----- OUTPUT PORTS -----
output [0:0] adder_phy_sumout;

//----- BEGIN wire-connection ports -----
wire [0:0] adder_phy_a;
wire [0:0] adder_phy_b;
wire [0:0] adder_phy_cin;
wire [0:0] adder_phy_cout;
wire [0:0] adder_phy_sumout;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	ADDF_X1N_A10P5PP84TR_C14 ADDF_X1N_A10P5PP84TR_C14_0_ (
		.A(adder_phy_a[0]),
		.B(adder_phy_b[0]),
		.CI(adder_phy_cin[0]),
		.S(adder_phy_sumout[0]),
		.CO(adder_phy_cout[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy -----



// ----- BEGIN Physical programmable logic block Verilog module: frac_logic -----
// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic -----
module logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic(pReset,
                                                                           prog_clk,
                                                                           frac_logic_in,
                                                                           frac_logic_cin,
                                                                           frac_logic_regin,
                                                                           frac_logic_regchain,
                                                                           ccff_head,
                                                                           frac_logic_out,
                                                                           frac_logic_cout,
                                                                           ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:5] frac_logic_in;
//----- INPUT PORTS -----
input [0:0] frac_logic_cin;
//----- INPUT PORTS -----
input [0:0] frac_logic_regin;
//----- INPUT PORTS -----
input [0:0] frac_logic_regchain;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:1] frac_logic_out;
//----- OUTPUT PORTS -----
output [0:0] frac_logic_cout;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:5] frac_logic_in;
wire [0:0] frac_logic_cin;
wire [0:0] frac_logic_regin;
wire [0:0] frac_logic_regchain;
wire [0:1] frac_logic_out;
wire [0:0] frac_logic_cout;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_10_out;
wire [0:0] direct_interc_11_out;
wire [0:0] direct_interc_12_out;
wire [0:0] direct_interc_1_out;
wire [0:0] direct_interc_2_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [0:0] direct_interc_7_out;
wire [0:0] direct_interc_8_out;
wire [0:0] direct_interc_9_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_cout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_sumout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_cout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_sumout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_ccff_tail;
wire [0:3] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_out;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut6_out;
wire [0:1] mux_tree_like_size3_0_sram;
wire [0:1] mux_tree_like_size3_0_sram_inv;
wire [0:0] mux_tree_like_size3_mem_0_ccff_tail;
wire [0:2] mux_tree_like_size4_0_sram;
wire [0:2] mux_tree_like_size4_0_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6 logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.frac_lut6_in({direct_interc_1_out[0], direct_interc_2_out[0], direct_interc_3_out[0], direct_interc_4_out[0], direct_interc_5_out[0], direct_interc_6_out[0]}),
		.ccff_head(ccff_head[0]),
		.frac_lut6_lut4_out(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_out[0:3]),
		.frac_lut6_lut5_out(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_out[0:1]),
		.frac_lut6_lut6_out(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut6_out[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_0 (
		.adder_phy_a(direct_interc_7_out[0]),
		.adder_phy_b(direct_interc_8_out[0]),
		.adder_phy_cin(direct_interc_9_out[0]),
		.adder_phy_cout(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_cout[0]),
		.adder_phy_sumout(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_sumout[0]));

	logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_1 (
		.adder_phy_a(direct_interc_10_out[0]),
		.adder_phy_b(direct_interc_11_out[0]),
		.adder_phy_cin(direct_interc_12_out[0]),
		.adder_phy_cout(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_cout[0]),
		.adder_phy_sumout(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_sumout[0]));

	mux_tree_like_size3 mux_frac_logic_out_0 (
		.in({logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_sumout[0], logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_out[0], frac_logic_regin[0]}),
		.sram(mux_tree_like_size3_0_sram[0:1]),
		.sram_inv(mux_tree_like_size3_0_sram_inv[0:1]),
		.out(frac_logic_out[0]));

	mux_tree_like_size3_mem mem_frac_logic_out_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size3_mem_0_ccff_tail[0]),
		.mem_out(mux_tree_like_size3_0_sram[0:1]),
		.mem_outb(mux_tree_like_size3_0_sram_inv[0:1]));

	mux_tree_like_size4 mux_frac_logic_out_1 (
		.in({logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_sumout[0], logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_out[1], logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut6_out[0], frac_logic_regchain[0]}),
		.sram(mux_tree_like_size4_0_sram[0:2]),
		.sram_inv(mux_tree_like_size4_0_sram_inv[0:2]),
		.out(frac_logic_out[1]));

	mux_tree_like_size4_mem mem_frac_logic_out_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size3_mem_0_ccff_tail[0]),
		.ccff_tail(ccff_tail[0]),
		.mem_out(mux_tree_like_size4_0_sram[0:2]),
		.mem_outb(mux_tree_like_size4_0_sram_inv[0:2]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_cout[0]),
		.out(frac_logic_cout[0]));

	direct_interc direct_interc_1_ (
		.in(frac_logic_in[0]),
		.out(direct_interc_1_out[0]));

	direct_interc direct_interc_2_ (
		.in(frac_logic_in[1]),
		.out(direct_interc_2_out[0]));

	direct_interc direct_interc_3_ (
		.in(frac_logic_in[2]),
		.out(direct_interc_3_out[0]));

	direct_interc direct_interc_4_ (
		.in(frac_logic_in[3]),
		.out(direct_interc_4_out[0]));

	direct_interc direct_interc_5_ (
		.in(frac_logic_in[4]),
		.out(direct_interc_5_out[0]));

	direct_interc direct_interc_6_ (
		.in(frac_logic_in[5]),
		.out(direct_interc_6_out[0]));

	direct_interc direct_interc_7_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_out[0]),
		.out(direct_interc_7_out[0]));

	direct_interc direct_interc_8_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_out[1]),
		.out(direct_interc_8_out[0]));

	direct_interc direct_interc_9_ (
		.in(frac_logic_cin[0]),
		.out(direct_interc_9_out[0]));

	direct_interc direct_interc_10_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_out[2]),
		.out(direct_interc_10_out[0]));

	direct_interc direct_interc_11_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_out[3]),
		.out(direct_interc_11_out[0]));

	direct_interc direct_interc_12_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_cout[0]),
		.out(direct_interc_12_out[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic -----


// ----- END Physical programmable logic block Verilog module: frac_logic -----

// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy -----
module logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy(Reset,
                                                                       Test_en,
                                                                       clk,
                                                                       ff_phy_D,
                                                                       ff_phy_D_chain,
                                                                       ff_phy_Q,
                                                                       ff_phy_clk);
//----- GLOBAL PORTS -----
input [0:0] Reset;
//----- GLOBAL PORTS -----
input [0:0] Test_en;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:0] ff_phy_D;
//----- INPUT PORTS -----
input [0:0] ff_phy_D_chain;
//----- OUTPUT PORTS -----
output [0:0] ff_phy_Q;
//----- CLOCK PORTS -----
input [0:0] ff_phy_clk;

//----- BEGIN wire-connection ports -----
wire [0:0] ff_phy_D;
wire [0:0] ff_phy_D_chain;
wire [0:0] ff_phy_Q;
wire [0:0] ff_phy_clk;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	SDFFRPQ_X1N_A10P5PP84TR_C14 SDFFRPQ_X1N_A10P5PP84TR_C14_0_ (
		.R(Reset[0]),
		.SE(Test_en[0]),
		.CK(clk[0]),
		.D(ff_phy_D[0]),
		.SI(ff_phy_D_chain[0]),
		.Q(ff_phy_Q[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy -----



// ----- BEGIN Physical programmable logic block Verilog module: fle -----
// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle -----
module logical_tile_clb_spypad_mode_default__fle(pReset,
                                                 prog_clk,
                                                 Reset,
                                                 Test_en,
                                                 clk,
                                                 fle_in,
                                                 fle_cin,
                                                 fle_sc_in,
                                                 fle_regin,
                                                 fle_clk,
                                                 ccff_head,
                                                 fle_out,
                                                 fle_cout,
                                                 fle_sc_out,
                                                 fle_regout,
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
//----- INPUT PORTS -----
input [0:5] fle_in;
//----- INPUT PORTS -----
input [0:0] fle_cin;
//----- INPUT PORTS -----
input [0:0] fle_sc_in;
//----- INPUT PORTS -----
input [0:0] fle_regin;
//----- INPUT PORTS -----
input [0:0] fle_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:1] fle_out;
//----- OUTPUT PORTS -----
output [0:0] fle_cout;
//----- OUTPUT PORTS -----
output [0:0] fle_sc_out;
//----- OUTPUT PORTS -----
output [0:0] fle_regout;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:5] fle_in;
wire [0:0] fle_cin;
wire [0:0] fle_sc_in;
wire [0:0] fle_regin;
wire [0:0] fle_clk;
wire [0:1] fle_out;
wire [0:0] fle_cout;
wire [0:0] fle_sc_out;
wire [0:0] fle_regout;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_10_out;
wire [0:0] direct_interc_11_out;
wire [0:0] direct_interc_12_out;
wire [0:0] direct_interc_13_out;
wire [0:0] direct_interc_14_out;
wire [0:0] direct_interc_15_out;
wire [0:0] direct_interc_16_out;
wire [0:0] direct_interc_17_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [0:0] direct_interc_7_out;
wire [0:0] direct_interc_8_out;
wire [0:0] direct_interc_9_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_0_ff_phy_Q;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_1_ff_phy_Q;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_out;
wire [0:1] mux_tree_like_size2_0_sram;
wire [0:1] mux_tree_like_size2_0_sram_inv;
wire [0:1] mux_tree_like_size2_1_sram;
wire [0:1] mux_tree_like_size2_1_sram_inv;
wire [0:0] mux_tree_like_size2_mem_0_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.frac_logic_in({direct_interc_3_out[0], direct_interc_4_out[0], direct_interc_5_out[0], direct_interc_6_out[0], direct_interc_7_out[0], direct_interc_8_out[0]}),
		.frac_logic_cin(direct_interc_9_out[0]),
		.frac_logic_regin(direct_interc_10_out[0]),
		.frac_logic_regchain(direct_interc_11_out[0]),
		.ccff_head(ccff_head[0]),
		.frac_logic_out(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_out[0:1]),
		.frac_logic_cout(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_cout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_0 (
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.ff_phy_D(direct_interc_12_out[0]),
		.ff_phy_D_chain(direct_interc_13_out[0]),
		.ff_phy_Q(logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_0_ff_phy_Q[0]),
		.ff_phy_clk(direct_interc_14_out[0]));

	logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_1 (
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.ff_phy_D(direct_interc_15_out[0]),
		.ff_phy_D_chain(direct_interc_16_out[0]),
		.ff_phy_Q(logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_1_ff_phy_Q[0]),
		.ff_phy_clk(direct_interc_17_out[0]));

	mux_tree_like_size2 mux_fle_out_0 (
		.in({logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_0_ff_phy_Q[0], logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_out[0]}),
		.sram(mux_tree_like_size2_0_sram[0:1]),
		.sram_inv(mux_tree_like_size2_0_sram_inv[0:1]),
		.out(fle_out[0]));

	mux_tree_like_size2 mux_fle_out_1 (
		.in({logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_1_ff_phy_Q[0], logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_out[1]}),
		.sram(mux_tree_like_size2_1_sram[0:1]),
		.sram_inv(mux_tree_like_size2_1_sram_inv[0:1]),
		.out(fle_out[1]));

	mux_tree_like_size2_mem mem_fle_out_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size2_mem_0_ccff_tail[0]),
		.mem_out(mux_tree_like_size2_0_sram[0:1]),
		.mem_outb(mux_tree_like_size2_0_sram_inv[0:1]));

	mux_tree_like_size2_mem mem_fle_out_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size2_mem_0_ccff_tail[0]),
		.ccff_tail(ccff_tail[0]),
		.mem_out(mux_tree_like_size2_1_sram[0:1]),
		.mem_outb(mux_tree_like_size2_1_sram_inv[0:1]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_cout[0]),
		.out(fle_cout[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_1_ff_phy_Q[0]),
		.out(fle_sc_out[0]));

	direct_interc direct_interc_2_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_1_ff_phy_Q[0]),
		.out(fle_regout[0]));

	direct_interc direct_interc_3_ (
		.in(fle_in[0]),
		.out(direct_interc_3_out[0]));

	direct_interc direct_interc_4_ (
		.in(fle_in[1]),
		.out(direct_interc_4_out[0]));

	direct_interc direct_interc_5_ (
		.in(fle_in[2]),
		.out(direct_interc_5_out[0]));

	direct_interc direct_interc_6_ (
		.in(fle_in[3]),
		.out(direct_interc_6_out[0]));

	direct_interc direct_interc_7_ (
		.in(fle_in[4]),
		.out(direct_interc_7_out[0]));

	direct_interc direct_interc_8_ (
		.in(fle_in[5]),
		.out(direct_interc_8_out[0]));

	direct_interc direct_interc_9_ (
		.in(fle_cin[0]),
		.out(direct_interc_9_out[0]));

	direct_interc direct_interc_10_ (
		.in(fle_regin[0]),
		.out(direct_interc_10_out[0]));

	direct_interc direct_interc_11_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_0_ff_phy_Q[0]),
		.out(direct_interc_11_out[0]));

	direct_interc direct_interc_12_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_out[0]),
		.out(direct_interc_12_out[0]));

	direct_interc direct_interc_13_ (
		.in(fle_sc_in[0]),
		.out(direct_interc_13_out[0]));

	direct_interc direct_interc_14_ (
		.in(fle_clk[0]),
		.out(direct_interc_14_out[0]));

	direct_interc direct_interc_15_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__frac_logic_0_frac_logic_out[1]),
		.out(direct_interc_15_out[0]));

	direct_interc direct_interc_16_ (
		.in(logical_tile_clb_spypad_mode_default__fle_mode_physical__ff_phy_0_ff_phy_Q[0]),
		.out(direct_interc_16_out[0]));

	direct_interc direct_interc_17_ (
		.in(fle_clk[0]),
		.out(direct_interc_17_out[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle -----


// ----- END Physical programmable logic block Verilog module: fle -----

// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6 -----
module logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6(pReset,
                                                                                                          prog_clk,
                                                                                                          gfpga_pad_frac_lut6_spypad_lut4_spy,
                                                                                                          gfpga_pad_frac_lut6_spypad_lut5_spy,
                                                                                                          gfpga_pad_frac_lut6_spypad_lut6_spy,
                                                                                                          frac_lut6_in,
                                                                                                          ccff_head,
                                                                                                          frac_lut6_lut4_spy,
                                                                                                          frac_lut6_lut5_spy,
                                                                                                          frac_lut6_lut6_spy,
                                                                                                          ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GPOUT PORTS -----
output [0:3] gfpga_pad_frac_lut6_spypad_lut4_spy;
//----- GPOUT PORTS -----
output [0:1] gfpga_pad_frac_lut6_spypad_lut5_spy;
//----- GPOUT PORTS -----
output [0:0] gfpga_pad_frac_lut6_spypad_lut6_spy;
//----- INPUT PORTS -----
input [0:5] frac_lut6_in;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:3] frac_lut6_lut4_spy;
//----- OUTPUT PORTS -----
output [0:1] frac_lut6_lut5_spy;
//----- OUTPUT PORTS -----
output [0:0] frac_lut6_lut6_spy;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:5] frac_lut6_in;
wire [0:3] frac_lut6_lut4_spy;
wire [0:1] frac_lut6_lut5_spy;
wire [0:0] frac_lut6_lut6_spy;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] frac_lut6_spypad_0_mode;
wire [0:1] frac_lut6_spypad_0_mode_inv;
wire [0:63] frac_lut6_spypad_0_sram;
wire [0:63] frac_lut6_spypad_0_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign gfpga_pad_frac_lut6_spypad_lut4_spy[0] = frac_lut6_lut4_spy[0];
	assign gfpga_pad_frac_lut6_spypad_lut4_spy[1] = frac_lut6_lut4_spy[1];
	assign gfpga_pad_frac_lut6_spypad_lut4_spy[2] = frac_lut6_lut4_spy[2];
	assign gfpga_pad_frac_lut6_spypad_lut4_spy[3] = frac_lut6_lut4_spy[3];
	assign gfpga_pad_frac_lut6_spypad_lut5_spy[0] = frac_lut6_lut5_spy[0];
	assign gfpga_pad_frac_lut6_spypad_lut5_spy[1] = frac_lut6_lut5_spy[1];
	assign gfpga_pad_frac_lut6_spypad_lut6_spy[0] = frac_lut6_lut6_spy[0];
// ----- END Local output short connections -----

	frac_lut6_spypad frac_lut6_spypad_0_ (
		.in(frac_lut6_in[0:5]),
		.sram(frac_lut6_spypad_0_sram[0:63]),
		.sram_inv(frac_lut6_spypad_0_sram_inv[0:63]),
		.mode(frac_lut6_spypad_0_mode[0:1]),
		.mode_inv(frac_lut6_spypad_0_mode_inv[0:1]),
		.lut4_spy(frac_lut6_lut4_spy[0:3]),
		.lut5_spy(frac_lut6_lut5_spy[0:1]),
		.lut6_spy(frac_lut6_lut6_spy[0]));

	frac_lut6_spypad_CCFFX1_mem frac_lut6_spypad_CCFFX1_mem (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(ccff_head[0]),
		.ccff_tail(ccff_tail[0]),
		.mem_out({frac_lut6_spypad_0_sram[0:63], frac_lut6_spypad_0_mode[0:1]}),
		.mem_outb({frac_lut6_spypad_0_sram_inv[0:63], frac_lut6_spypad_0_mode_inv[0:1]}));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6 -----



// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy -----
module logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy(adder_phy_a,
                                                                                                          adder_phy_b,
                                                                                                          adder_phy_cin,
                                                                                                          adder_phy_cout,
                                                                                                          adder_phy_sumout);
//----- INPUT PORTS -----
input [0:0] adder_phy_a;
//----- INPUT PORTS -----
input [0:0] adder_phy_b;
//----- INPUT PORTS -----
input [0:0] adder_phy_cin;
//----- OUTPUT PORTS -----
output [0:0] adder_phy_cout;
//----- OUTPUT PORTS -----
output [0:0] adder_phy_sumout;

//----- BEGIN wire-connection ports -----
wire [0:0] adder_phy_a;
wire [0:0] adder_phy_b;
wire [0:0] adder_phy_cin;
wire [0:0] adder_phy_cout;
wire [0:0] adder_phy_sumout;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	ADDF_X1N_A10P5PP84TR_C14 ADDF_X1N_A10P5PP84TR_C14_0_ (
		.A(adder_phy_a[0]),
		.B(adder_phy_b[0]),
		.CI(adder_phy_cin[0]),
		.S(adder_phy_sumout[0]),
		.CO(adder_phy_cout[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy -----



// ----- BEGIN Physical programmable logic block Verilog module: frac_logic -----
// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic -----
module logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic(pReset,
                                                                                  prog_clk,
                                                                                  gfpga_pad_frac_lut6_spypad_lut4_spy,
                                                                                  gfpga_pad_frac_lut6_spypad_lut5_spy,
                                                                                  gfpga_pad_frac_lut6_spypad_lut6_spy,
                                                                                  frac_logic_in,
                                                                                  frac_logic_cin,
                                                                                  frac_logic_regin,
                                                                                  frac_logic_regchain,
                                                                                  ccff_head,
                                                                                  frac_logic_out,
                                                                                  frac_logic_cout,
                                                                                  ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GPOUT PORTS -----
output [0:3] gfpga_pad_frac_lut6_spypad_lut4_spy;
//----- GPOUT PORTS -----
output [0:1] gfpga_pad_frac_lut6_spypad_lut5_spy;
//----- GPOUT PORTS -----
output [0:0] gfpga_pad_frac_lut6_spypad_lut6_spy;
//----- INPUT PORTS -----
input [0:5] frac_logic_in;
//----- INPUT PORTS -----
input [0:0] frac_logic_cin;
//----- INPUT PORTS -----
input [0:0] frac_logic_regin;
//----- INPUT PORTS -----
input [0:0] frac_logic_regchain;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:1] frac_logic_out;
//----- OUTPUT PORTS -----
output [0:0] frac_logic_cout;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:5] frac_logic_in;
wire [0:0] frac_logic_cin;
wire [0:0] frac_logic_regin;
wire [0:0] frac_logic_regchain;
wire [0:1] frac_logic_out;
wire [0:0] frac_logic_cout;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_10_out;
wire [0:0] direct_interc_11_out;
wire [0:0] direct_interc_12_out;
wire [0:0] direct_interc_1_out;
wire [0:0] direct_interc_2_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [0:0] direct_interc_7_out;
wire [0:0] direct_interc_8_out;
wire [0:0] direct_interc_9_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_cout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_sumout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_cout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_sumout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_ccff_tail;
wire [0:3] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_spy;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_spy;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut6_spy;
wire [0:1] mux_tree_like_size3_0_sram;
wire [0:1] mux_tree_like_size3_0_sram_inv;
wire [0:0] mux_tree_like_size3_mem_0_ccff_tail;
wire [0:2] mux_tree_like_size4_0_sram;
wire [0:2] mux_tree_like_size4_0_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6 logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_frac_lut6_spypad_lut4_spy(gfpga_pad_frac_lut6_spypad_lut4_spy[0:3]),
		.gfpga_pad_frac_lut6_spypad_lut5_spy(gfpga_pad_frac_lut6_spypad_lut5_spy[0:1]),
		.gfpga_pad_frac_lut6_spypad_lut6_spy(gfpga_pad_frac_lut6_spypad_lut6_spy[0]),
		.frac_lut6_in({direct_interc_1_out[0], direct_interc_2_out[0], direct_interc_3_out[0], direct_interc_4_out[0], direct_interc_5_out[0], direct_interc_6_out[0]}),
		.ccff_head(ccff_head[0]),
		.frac_lut6_lut4_spy(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_spy[0:3]),
		.frac_lut6_lut5_spy(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_spy[0:1]),
		.frac_lut6_lut6_spy(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut6_spy[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_0 (
		.adder_phy_a(direct_interc_7_out[0]),
		.adder_phy_b(direct_interc_8_out[0]),
		.adder_phy_cin(direct_interc_9_out[0]),
		.adder_phy_cout(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_cout[0]),
		.adder_phy_sumout(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_sumout[0]));

	logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_1 (
		.adder_phy_a(direct_interc_10_out[0]),
		.adder_phy_b(direct_interc_11_out[0]),
		.adder_phy_cin(direct_interc_12_out[0]),
		.adder_phy_cout(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_cout[0]),
		.adder_phy_sumout(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_sumout[0]));

	mux_tree_like_size3 mux_frac_logic_out_0 (
		.in({logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_sumout[0], logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_spy[0], frac_logic_regin[0]}),
		.sram(mux_tree_like_size3_0_sram[0:1]),
		.sram_inv(mux_tree_like_size3_0_sram_inv[0:1]),
		.out(frac_logic_out[0]));

	mux_tree_like_size3_mem mem_frac_logic_out_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size3_mem_0_ccff_tail[0]),
		.mem_out(mux_tree_like_size3_0_sram[0:1]),
		.mem_outb(mux_tree_like_size3_0_sram_inv[0:1]));

	mux_tree_like_size4 mux_frac_logic_out_1 (
		.in({logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_sumout[0], logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut5_spy[1], logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut6_spy[0], frac_logic_regchain[0]}),
		.sram(mux_tree_like_size4_0_sram[0:2]),
		.sram_inv(mux_tree_like_size4_0_sram_inv[0:2]),
		.out(frac_logic_out[1]));

	mux_tree_like_size4_mem mem_frac_logic_out_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size3_mem_0_ccff_tail[0]),
		.ccff_tail(ccff_tail[0]),
		.mem_out(mux_tree_like_size4_0_sram[0:2]),
		.mem_outb(mux_tree_like_size4_0_sram_inv[0:2]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_1_adder_phy_cout[0]),
		.out(frac_logic_cout[0]));

	direct_interc direct_interc_1_ (
		.in(frac_logic_in[0]),
		.out(direct_interc_1_out[0]));

	direct_interc direct_interc_2_ (
		.in(frac_logic_in[1]),
		.out(direct_interc_2_out[0]));

	direct_interc direct_interc_3_ (
		.in(frac_logic_in[2]),
		.out(direct_interc_3_out[0]));

	direct_interc direct_interc_4_ (
		.in(frac_logic_in[3]),
		.out(direct_interc_4_out[0]));

	direct_interc direct_interc_5_ (
		.in(frac_logic_in[4]),
		.out(direct_interc_5_out[0]));

	direct_interc direct_interc_6_ (
		.in(frac_logic_in[5]),
		.out(direct_interc_6_out[0]));

	direct_interc direct_interc_7_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_spy[0]),
		.out(direct_interc_7_out[0]));

	direct_interc direct_interc_8_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_spy[1]),
		.out(direct_interc_8_out[0]));

	direct_interc direct_interc_9_ (
		.in(frac_logic_cin[0]),
		.out(direct_interc_9_out[0]));

	direct_interc direct_interc_10_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_spy[2]),
		.out(direct_interc_10_out[0]));

	direct_interc direct_interc_11_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__frac_lut6_0_frac_lut6_lut4_spy[3]),
		.out(direct_interc_11_out[0]));

	direct_interc direct_interc_12_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_mode_default__adder_phy_0_adder_phy_cout[0]),
		.out(direct_interc_12_out[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic -----


// ----- END Physical programmable logic block Verilog module: frac_logic -----

// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy -----
module logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy(Reset,
                                                                              Test_en,
                                                                              clk,
                                                                              ff_phy_D,
                                                                              ff_phy_D_chain,
                                                                              ff_phy_Q,
                                                                              ff_phy_clk);
//----- GLOBAL PORTS -----
input [0:0] Reset;
//----- GLOBAL PORTS -----
input [0:0] Test_en;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:0] ff_phy_D;
//----- INPUT PORTS -----
input [0:0] ff_phy_D_chain;
//----- OUTPUT PORTS -----
output [0:0] ff_phy_Q;
//----- CLOCK PORTS -----
input [0:0] ff_phy_clk;

//----- BEGIN wire-connection ports -----
wire [0:0] ff_phy_D;
wire [0:0] ff_phy_D_chain;
wire [0:0] ff_phy_Q;
wire [0:0] ff_phy_clk;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	SDFFRPQ_X1N_A10P5PP84TR_C14 SDFFRPQ_X1N_A10P5PP84TR_C14_0_ (
		.R(Reset[0]),
		.SE(Test_en[0]),
		.CK(clk[0]),
		.D(ff_phy_D[0]),
		.SI(ff_phy_D_chain[0]),
		.Q(ff_phy_Q[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy -----



// ----- BEGIN Physical programmable logic block Verilog module: fle_spypad -----
// ----- Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad -----
module logical_tile_clb_spypad_mode_default__fle_spypad(pReset,
                                                        prog_clk,
                                                        Reset,
                                                        Test_en,
                                                        clk,
                                                        gfpga_pad_frac_lut6_spypad_lut4_spy,
                                                        gfpga_pad_frac_lut6_spypad_lut5_spy,
                                                        gfpga_pad_frac_lut6_spypad_lut6_spy,
                                                        fle_spypad_in,
                                                        fle_spypad_cin,
                                                        fle_spypad_sc_in,
                                                        fle_spypad_regin,
                                                        fle_spypad_clk,
                                                        ccff_head,
                                                        fle_spypad_out,
                                                        fle_spypad_cout,
                                                        fle_spypad_sc_out,
                                                        fle_spypad_regout,
                                                        fle_spypad_lut_perf,
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
input [0:5] fle_spypad_in;
//----- INPUT PORTS -----
input [0:0] fle_spypad_cin;
//----- INPUT PORTS -----
input [0:0] fle_spypad_sc_in;
//----- INPUT PORTS -----
input [0:0] fle_spypad_regin;
//----- INPUT PORTS -----
input [0:0] fle_spypad_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:1] fle_spypad_out;
//----- OUTPUT PORTS -----
output [0:0] fle_spypad_cout;
//----- OUTPUT PORTS -----
output [0:0] fle_spypad_sc_out;
//----- OUTPUT PORTS -----
output [0:0] fle_spypad_regout;
//----- OUTPUT PORTS -----
output [0:0] fle_spypad_lut_perf;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:5] fle_spypad_in;
wire [0:0] fle_spypad_cin;
wire [0:0] fle_spypad_sc_in;
wire [0:0] fle_spypad_regin;
wire [0:0] fle_spypad_clk;
wire [0:1] fle_spypad_out;
wire [0:0] fle_spypad_cout;
wire [0:0] fle_spypad_sc_out;
wire [0:0] fle_spypad_regout;
wire [0:0] fle_spypad_lut_perf;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_10_out;
wire [0:0] direct_interc_11_out;
wire [0:0] direct_interc_12_out;
wire [0:0] direct_interc_13_out;
wire [0:0] direct_interc_14_out;
wire [0:0] direct_interc_15_out;
wire [0:0] direct_interc_16_out;
wire [0:0] direct_interc_17_out;
wire [0:0] direct_interc_18_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [0:0] direct_interc_7_out;
wire [0:0] direct_interc_8_out;
wire [0:0] direct_interc_9_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_0_ff_phy_Q;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_1_ff_phy_Q;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_out;
wire [0:1] mux_tree_like_size2_0_sram;
wire [0:1] mux_tree_like_size2_0_sram_inv;
wire [0:1] mux_tree_like_size2_1_sram;
wire [0:1] mux_tree_like_size2_1_sram_inv;
wire [0:0] mux_tree_like_size2_mem_0_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_frac_lut6_spypad_lut4_spy(gfpga_pad_frac_lut6_spypad_lut4_spy[0:3]),
		.gfpga_pad_frac_lut6_spypad_lut5_spy(gfpga_pad_frac_lut6_spypad_lut5_spy[0:1]),
		.gfpga_pad_frac_lut6_spypad_lut6_spy(gfpga_pad_frac_lut6_spypad_lut6_spy[0]),
		.frac_logic_in({direct_interc_4_out[0], direct_interc_5_out[0], direct_interc_6_out[0], direct_interc_7_out[0], direct_interc_8_out[0], direct_interc_9_out[0]}),
		.frac_logic_cin(direct_interc_10_out[0]),
		.frac_logic_regin(direct_interc_11_out[0]),
		.frac_logic_regchain(direct_interc_12_out[0]),
		.ccff_head(ccff_head[0]),
		.frac_logic_out(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_out[0:1]),
		.frac_logic_cout(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_cout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_0 (
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.ff_phy_D(direct_interc_13_out[0]),
		.ff_phy_D_chain(direct_interc_14_out[0]),
		.ff_phy_Q(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_0_ff_phy_Q[0]),
		.ff_phy_clk(direct_interc_15_out[0]));

	logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_1 (
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.ff_phy_D(direct_interc_16_out[0]),
		.ff_phy_D_chain(direct_interc_17_out[0]),
		.ff_phy_Q(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_1_ff_phy_Q[0]),
		.ff_phy_clk(direct_interc_18_out[0]));

	mux_tree_like_size2 mux_fle_spypad_out_0 (
		.in({logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_0_ff_phy_Q[0], logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_out[0]}),
		.sram(mux_tree_like_size2_0_sram[0:1]),
		.sram_inv(mux_tree_like_size2_0_sram_inv[0:1]),
		.out(fle_spypad_out[0]));

	mux_tree_like_size2 mux_fle_spypad_out_1 (
		.in({logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_1_ff_phy_Q[0], logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_out[1]}),
		.sram(mux_tree_like_size2_1_sram[0:1]),
		.sram_inv(mux_tree_like_size2_1_sram_inv[0:1]),
		.out(fle_spypad_out[1]));

	mux_tree_like_size2_mem mem_fle_spypad_out_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size2_mem_0_ccff_tail[0]),
		.mem_out(mux_tree_like_size2_0_sram[0:1]),
		.mem_outb(mux_tree_like_size2_0_sram_inv[0:1]));

	mux_tree_like_size2_mem mem_fle_spypad_out_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size2_mem_0_ccff_tail[0]),
		.ccff_tail(ccff_tail[0]),
		.mem_out(mux_tree_like_size2_1_sram[0:1]),
		.mem_outb(mux_tree_like_size2_1_sram_inv[0:1]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_cout[0]),
		.out(fle_spypad_cout[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_1_ff_phy_Q[0]),
		.out(fle_spypad_sc_out[0]));

	direct_interc direct_interc_2_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_1_ff_phy_Q[0]),
		.out(fle_spypad_regout[0]));

	direct_interc direct_interc_3_ (
		.in(fle_spypad_in[0]),
		.out(fle_spypad_lut_perf[0]));

	direct_interc direct_interc_4_ (
		.in(fle_spypad_in[0]),
		.out(direct_interc_4_out[0]));

	direct_interc direct_interc_5_ (
		.in(fle_spypad_in[1]),
		.out(direct_interc_5_out[0]));

	direct_interc direct_interc_6_ (
		.in(fle_spypad_in[2]),
		.out(direct_interc_6_out[0]));

	direct_interc direct_interc_7_ (
		.in(fle_spypad_in[3]),
		.out(direct_interc_7_out[0]));

	direct_interc direct_interc_8_ (
		.in(fle_spypad_in[4]),
		.out(direct_interc_8_out[0]));

	direct_interc direct_interc_9_ (
		.in(fle_spypad_in[5]),
		.out(direct_interc_9_out[0]));

	direct_interc direct_interc_10_ (
		.in(fle_spypad_cin[0]),
		.out(direct_interc_10_out[0]));

	direct_interc direct_interc_11_ (
		.in(fle_spypad_regin[0]),
		.out(direct_interc_11_out[0]));

	direct_interc direct_interc_12_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_0_ff_phy_Q[0]),
		.out(direct_interc_12_out[0]));

	direct_interc direct_interc_13_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_out[0]),
		.out(direct_interc_13_out[0]));

	direct_interc direct_interc_14_ (
		.in(fle_spypad_sc_in[0]),
		.out(direct_interc_14_out[0]));

	direct_interc direct_interc_15_ (
		.in(fle_spypad_clk[0]),
		.out(direct_interc_15_out[0]));

	direct_interc direct_interc_16_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__frac_logic_0_frac_logic_out[1]),
		.out(direct_interc_16_out[0]));

	direct_interc direct_interc_17_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_mode_physical__ff_phy_0_ff_phy_Q[0]),
		.out(direct_interc_17_out[0]));

	direct_interc direct_interc_18_ (
		.in(fle_spypad_clk[0]),
		.out(direct_interc_18_out[0]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_default__fle_spypad -----


// ----- END Physical programmable logic block Verilog module: fle_spypad -----

// ----- BEGIN Physical programmable logic block Verilog module: clb_spypad -----
// ----- Verilog module for logical_tile_clb_spypad_mode_clb_spypad_ -----
module logical_tile_clb_spypad_mode_clb_spypad_(pReset,
                                                prog_clk,
                                                Reset,
                                                Test_en,
                                                clk,
                                                gfpga_pad_frac_lut6_spypad_lut4_spy,
                                                gfpga_pad_frac_lut6_spypad_lut5_spy,
                                                gfpga_pad_frac_lut6_spypad_lut6_spy,
                                                clb_spypad_I0,
                                                clb_spypad_I1,
                                                clb_spypad_I2,
                                                clb_spypad_I3,
                                                clb_spypad_sc_in,
                                                clb_spypad_cin,
                                                clb_spypad_cin_trick,
                                                clb_spypad_regin,
                                                clb_spypad_clk,
                                                ccff_head,
                                                clb_spypad_O,
                                                clb_spypad_sc_out,
                                                clb_spypad_cout,
                                                clb_spypad_cout_copy,
                                                clb_spypad_regout,
                                                clb_spypad_lut_perf,
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
input [0:9] clb_spypad_I0;
//----- INPUT PORTS -----
input [0:9] clb_spypad_I1;
//----- INPUT PORTS -----
input [0:9] clb_spypad_I2;
//----- INPUT PORTS -----
input [0:9] clb_spypad_I3;
//----- INPUT PORTS -----
input [0:0] clb_spypad_sc_in;
//----- INPUT PORTS -----
input [0:0] clb_spypad_cin;
//----- INPUT PORTS -----
input [0:0] clb_spypad_cin_trick;
//----- INPUT PORTS -----
input [0:0] clb_spypad_regin;
//----- INPUT PORTS -----
input [0:0] clb_spypad_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:19] clb_spypad_O;
//----- OUTPUT PORTS -----
output [0:0] clb_spypad_sc_out;
//----- OUTPUT PORTS -----
output [0:0] clb_spypad_cout;
//----- OUTPUT PORTS -----
output [0:0] clb_spypad_cout_copy;
//----- OUTPUT PORTS -----
output [0:0] clb_spypad_regout;
//----- OUTPUT PORTS -----
output [0:0] clb_spypad_lut_perf;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:9] clb_spypad_I0;
wire [0:9] clb_spypad_I1;
wire [0:9] clb_spypad_I2;
wire [0:9] clb_spypad_I3;
wire [0:0] clb_spypad_sc_in;
wire [0:0] clb_spypad_cin;
wire [0:0] clb_spypad_cin_trick;
wire [0:0] clb_spypad_regin;
wire [0:0] clb_spypad_clk;
wire [0:19] clb_spypad_O;
wire [0:0] clb_spypad_sc_out;
wire [0:0] clb_spypad_cout;
wire [0:0] clb_spypad_cout_copy;
wire [0:0] clb_spypad_regout;
wire [0:0] clb_spypad_lut_perf;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_25_out;
wire [0:0] direct_interc_26_out;
wire [0:0] direct_interc_27_out;
wire [0:0] direct_interc_28_out;
wire [0:0] direct_interc_29_out;
wire [0:0] direct_interc_30_out;
wire [0:0] direct_interc_31_out;
wire [0:0] direct_interc_32_out;
wire [0:0] direct_interc_33_out;
wire [0:0] direct_interc_34_out;
wire [0:0] direct_interc_35_out;
wire [0:0] direct_interc_36_out;
wire [0:0] direct_interc_37_out;
wire [0:0] direct_interc_38_out;
wire [0:0] direct_interc_39_out;
wire [0:0] direct_interc_40_out;
wire [0:0] direct_interc_41_out;
wire [0:0] direct_interc_42_out;
wire [0:0] direct_interc_43_out;
wire [0:0] direct_interc_44_out;
wire [0:0] direct_interc_45_out;
wire [0:0] direct_interc_46_out;
wire [0:0] direct_interc_47_out;
wire [0:0] direct_interc_48_out;
wire [0:0] direct_interc_49_out;
wire [0:0] direct_interc_50_out;
wire [0:0] direct_interc_51_out;
wire [0:0] direct_interc_52_out;
wire [0:0] direct_interc_53_out;
wire [0:0] direct_interc_54_out;
wire [0:0] direct_interc_55_out;
wire [0:0] direct_interc_56_out;
wire [0:0] direct_interc_57_out;
wire [0:0] direct_interc_58_out;
wire [0:0] direct_interc_59_out;
wire [0:0] direct_interc_60_out;
wire [0:0] direct_interc_61_out;
wire [0:0] direct_interc_62_out;
wire [0:0] direct_interc_63_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_0_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_0_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_0_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_0_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_0_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_1_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_1_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_1_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_1_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_1_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_2_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_2_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_2_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_2_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_2_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_3_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_3_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_3_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_3_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_3_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_4_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_4_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_4_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_4_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_4_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_5_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_5_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_5_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_5_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_5_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_6_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_6_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_6_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_6_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_6_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_7_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_7_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_7_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_7_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_7_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_8_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_8_fle_cout;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_8_fle_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_8_fle_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_8_fle_sc_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_0_ccff_tail;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_cout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_lut_perf;
wire [0:1] logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_regout;
wire [0:0] logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_sc_out;
wire [0:0] mux_tree_like_size22_0_out;
wire [0:4] mux_tree_like_size22_0_sram;
wire [0:4] mux_tree_like_size22_0_sram_inv;
wire [0:0] mux_tree_like_size40_0_out;
wire [0:5] mux_tree_like_size40_0_sram;
wire [0:5] mux_tree_like_size40_0_sram_inv;
wire [0:0] mux_tree_like_size40_10_out;
wire [0:5] mux_tree_like_size40_10_sram;
wire [0:5] mux_tree_like_size40_10_sram_inv;
wire [0:0] mux_tree_like_size40_11_out;
wire [0:5] mux_tree_like_size40_11_sram;
wire [0:5] mux_tree_like_size40_11_sram_inv;
wire [0:0] mux_tree_like_size40_12_out;
wire [0:5] mux_tree_like_size40_12_sram;
wire [0:5] mux_tree_like_size40_12_sram_inv;
wire [0:0] mux_tree_like_size40_13_out;
wire [0:5] mux_tree_like_size40_13_sram;
wire [0:5] mux_tree_like_size40_13_sram_inv;
wire [0:0] mux_tree_like_size40_14_out;
wire [0:5] mux_tree_like_size40_14_sram;
wire [0:5] mux_tree_like_size40_14_sram_inv;
wire [0:0] mux_tree_like_size40_15_out;
wire [0:5] mux_tree_like_size40_15_sram;
wire [0:5] mux_tree_like_size40_15_sram_inv;
wire [0:0] mux_tree_like_size40_16_out;
wire [0:5] mux_tree_like_size40_16_sram;
wire [0:5] mux_tree_like_size40_16_sram_inv;
wire [0:0] mux_tree_like_size40_17_out;
wire [0:5] mux_tree_like_size40_17_sram;
wire [0:5] mux_tree_like_size40_17_sram_inv;
wire [0:0] mux_tree_like_size40_18_out;
wire [0:5] mux_tree_like_size40_18_sram;
wire [0:5] mux_tree_like_size40_18_sram_inv;
wire [0:0] mux_tree_like_size40_19_out;
wire [0:5] mux_tree_like_size40_19_sram;
wire [0:5] mux_tree_like_size40_19_sram_inv;
wire [0:0] mux_tree_like_size40_1_out;
wire [0:5] mux_tree_like_size40_1_sram;
wire [0:5] mux_tree_like_size40_1_sram_inv;
wire [0:0] mux_tree_like_size40_20_out;
wire [0:5] mux_tree_like_size40_20_sram;
wire [0:5] mux_tree_like_size40_20_sram_inv;
wire [0:0] mux_tree_like_size40_21_out;
wire [0:5] mux_tree_like_size40_21_sram;
wire [0:5] mux_tree_like_size40_21_sram_inv;
wire [0:0] mux_tree_like_size40_22_out;
wire [0:5] mux_tree_like_size40_22_sram;
wire [0:5] mux_tree_like_size40_22_sram_inv;
wire [0:0] mux_tree_like_size40_23_out;
wire [0:5] mux_tree_like_size40_23_sram;
wire [0:5] mux_tree_like_size40_23_sram_inv;
wire [0:0] mux_tree_like_size40_24_out;
wire [0:5] mux_tree_like_size40_24_sram;
wire [0:5] mux_tree_like_size40_24_sram_inv;
wire [0:0] mux_tree_like_size40_25_out;
wire [0:5] mux_tree_like_size40_25_sram;
wire [0:5] mux_tree_like_size40_25_sram_inv;
wire [0:0] mux_tree_like_size40_26_out;
wire [0:5] mux_tree_like_size40_26_sram;
wire [0:5] mux_tree_like_size40_26_sram_inv;
wire [0:0] mux_tree_like_size40_27_out;
wire [0:5] mux_tree_like_size40_27_sram;
wire [0:5] mux_tree_like_size40_27_sram_inv;
wire [0:0] mux_tree_like_size40_28_out;
wire [0:5] mux_tree_like_size40_28_sram;
wire [0:5] mux_tree_like_size40_28_sram_inv;
wire [0:0] mux_tree_like_size40_29_out;
wire [0:5] mux_tree_like_size40_29_sram;
wire [0:5] mux_tree_like_size40_29_sram_inv;
wire [0:0] mux_tree_like_size40_2_out;
wire [0:5] mux_tree_like_size40_2_sram;
wire [0:5] mux_tree_like_size40_2_sram_inv;
wire [0:0] mux_tree_like_size40_30_out;
wire [0:5] mux_tree_like_size40_30_sram;
wire [0:5] mux_tree_like_size40_30_sram_inv;
wire [0:0] mux_tree_like_size40_31_out;
wire [0:5] mux_tree_like_size40_31_sram;
wire [0:5] mux_tree_like_size40_31_sram_inv;
wire [0:0] mux_tree_like_size40_32_out;
wire [0:5] mux_tree_like_size40_32_sram;
wire [0:5] mux_tree_like_size40_32_sram_inv;
wire [0:0] mux_tree_like_size40_33_out;
wire [0:5] mux_tree_like_size40_33_sram;
wire [0:5] mux_tree_like_size40_33_sram_inv;
wire [0:0] mux_tree_like_size40_34_out;
wire [0:5] mux_tree_like_size40_34_sram;
wire [0:5] mux_tree_like_size40_34_sram_inv;
wire [0:0] mux_tree_like_size40_35_out;
wire [0:5] mux_tree_like_size40_35_sram;
wire [0:5] mux_tree_like_size40_35_sram_inv;
wire [0:0] mux_tree_like_size40_36_out;
wire [0:5] mux_tree_like_size40_36_sram;
wire [0:5] mux_tree_like_size40_36_sram_inv;
wire [0:0] mux_tree_like_size40_37_out;
wire [0:5] mux_tree_like_size40_37_sram;
wire [0:5] mux_tree_like_size40_37_sram_inv;
wire [0:0] mux_tree_like_size40_38_out;
wire [0:5] mux_tree_like_size40_38_sram;
wire [0:5] mux_tree_like_size40_38_sram_inv;
wire [0:0] mux_tree_like_size40_39_out;
wire [0:5] mux_tree_like_size40_39_sram;
wire [0:5] mux_tree_like_size40_39_sram_inv;
wire [0:0] mux_tree_like_size40_3_out;
wire [0:5] mux_tree_like_size40_3_sram;
wire [0:5] mux_tree_like_size40_3_sram_inv;
wire [0:0] mux_tree_like_size40_40_out;
wire [0:5] mux_tree_like_size40_40_sram;
wire [0:5] mux_tree_like_size40_40_sram_inv;
wire [0:0] mux_tree_like_size40_41_out;
wire [0:5] mux_tree_like_size40_41_sram;
wire [0:5] mux_tree_like_size40_41_sram_inv;
wire [0:0] mux_tree_like_size40_42_out;
wire [0:5] mux_tree_like_size40_42_sram;
wire [0:5] mux_tree_like_size40_42_sram_inv;
wire [0:0] mux_tree_like_size40_43_out;
wire [0:5] mux_tree_like_size40_43_sram;
wire [0:5] mux_tree_like_size40_43_sram_inv;
wire [0:0] mux_tree_like_size40_44_out;
wire [0:5] mux_tree_like_size40_44_sram;
wire [0:5] mux_tree_like_size40_44_sram_inv;
wire [0:0] mux_tree_like_size40_45_out;
wire [0:5] mux_tree_like_size40_45_sram;
wire [0:5] mux_tree_like_size40_45_sram_inv;
wire [0:0] mux_tree_like_size40_46_out;
wire [0:5] mux_tree_like_size40_46_sram;
wire [0:5] mux_tree_like_size40_46_sram_inv;
wire [0:0] mux_tree_like_size40_47_out;
wire [0:5] mux_tree_like_size40_47_sram;
wire [0:5] mux_tree_like_size40_47_sram_inv;
wire [0:0] mux_tree_like_size40_48_out;
wire [0:5] mux_tree_like_size40_48_sram;
wire [0:5] mux_tree_like_size40_48_sram_inv;
wire [0:0] mux_tree_like_size40_49_out;
wire [0:5] mux_tree_like_size40_49_sram;
wire [0:5] mux_tree_like_size40_49_sram_inv;
wire [0:0] mux_tree_like_size40_4_out;
wire [0:5] mux_tree_like_size40_4_sram;
wire [0:5] mux_tree_like_size40_4_sram_inv;
wire [0:0] mux_tree_like_size40_50_out;
wire [0:5] mux_tree_like_size40_50_sram;
wire [0:5] mux_tree_like_size40_50_sram_inv;
wire [0:0] mux_tree_like_size40_51_out;
wire [0:5] mux_tree_like_size40_51_sram;
wire [0:5] mux_tree_like_size40_51_sram_inv;
wire [0:0] mux_tree_like_size40_52_out;
wire [0:5] mux_tree_like_size40_52_sram;
wire [0:5] mux_tree_like_size40_52_sram_inv;
wire [0:0] mux_tree_like_size40_53_out;
wire [0:5] mux_tree_like_size40_53_sram;
wire [0:5] mux_tree_like_size40_53_sram_inv;
wire [0:0] mux_tree_like_size40_54_out;
wire [0:5] mux_tree_like_size40_54_sram;
wire [0:5] mux_tree_like_size40_54_sram_inv;
wire [0:0] mux_tree_like_size40_55_out;
wire [0:5] mux_tree_like_size40_55_sram;
wire [0:5] mux_tree_like_size40_55_sram_inv;
wire [0:0] mux_tree_like_size40_56_out;
wire [0:5] mux_tree_like_size40_56_sram;
wire [0:5] mux_tree_like_size40_56_sram_inv;
wire [0:0] mux_tree_like_size40_57_out;
wire [0:5] mux_tree_like_size40_57_sram;
wire [0:5] mux_tree_like_size40_57_sram_inv;
wire [0:0] mux_tree_like_size40_58_out;
wire [0:5] mux_tree_like_size40_58_sram;
wire [0:5] mux_tree_like_size40_58_sram_inv;
wire [0:0] mux_tree_like_size40_59_out;
wire [0:5] mux_tree_like_size40_59_sram;
wire [0:5] mux_tree_like_size40_59_sram_inv;
wire [0:0] mux_tree_like_size40_5_out;
wire [0:5] mux_tree_like_size40_5_sram;
wire [0:5] mux_tree_like_size40_5_sram_inv;
wire [0:0] mux_tree_like_size40_6_out;
wire [0:5] mux_tree_like_size40_6_sram;
wire [0:5] mux_tree_like_size40_6_sram_inv;
wire [0:0] mux_tree_like_size40_7_out;
wire [0:5] mux_tree_like_size40_7_sram;
wire [0:5] mux_tree_like_size40_7_sram_inv;
wire [0:0] mux_tree_like_size40_8_out;
wire [0:5] mux_tree_like_size40_8_sram;
wire [0:5] mux_tree_like_size40_8_sram_inv;
wire [0:0] mux_tree_like_size40_9_out;
wire [0:5] mux_tree_like_size40_9_sram;
wire [0:5] mux_tree_like_size40_9_sram_inv;
wire [0:0] mux_tree_like_size40_mem_0_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_10_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_11_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_12_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_13_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_14_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_15_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_16_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_17_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_18_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_19_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_1_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_20_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_21_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_22_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_23_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_24_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_25_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_26_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_27_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_28_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_29_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_2_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_30_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_31_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_32_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_33_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_34_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_35_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_36_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_37_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_38_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_39_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_3_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_40_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_41_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_42_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_43_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_44_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_45_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_46_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_47_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_48_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_49_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_4_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_50_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_51_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_52_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_53_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_54_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_55_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_56_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_57_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_58_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_59_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_5_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_6_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_7_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_8_ccff_tail;
wire [0:0] mux_tree_like_size40_mem_9_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_0_out[0], mux_tree_like_size40_1_out[0], mux_tree_like_size40_2_out[0], mux_tree_like_size40_3_out[0], mux_tree_like_size40_4_out[0], mux_tree_like_size40_5_out[0]}),
		.fle_cin(direct_interc_25_out[0]),
		.fle_sc_in(direct_interc_26_out[0]),
		.fle_regin(direct_interc_27_out[0]),
		.fle_clk(direct_interc_28_out[0]),
		.ccff_head(ccff_head[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_0_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_0_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_0_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_0_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_6_out[0], mux_tree_like_size40_7_out[0], mux_tree_like_size40_8_out[0], mux_tree_like_size40_9_out[0], mux_tree_like_size40_10_out[0], mux_tree_like_size40_11_out[0]}),
		.fle_cin(direct_interc_29_out[0]),
		.fle_sc_in(direct_interc_30_out[0]),
		.fle_regin(direct_interc_31_out[0]),
		.fle_clk(direct_interc_32_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_0_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_1_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_1_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_1_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_1_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_12_out[0], mux_tree_like_size40_13_out[0], mux_tree_like_size40_14_out[0], mux_tree_like_size40_15_out[0], mux_tree_like_size40_16_out[0], mux_tree_like_size40_17_out[0]}),
		.fle_cin(direct_interc_33_out[0]),
		.fle_sc_in(direct_interc_34_out[0]),
		.fle_regin(direct_interc_35_out[0]),
		.fle_clk(direct_interc_36_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_1_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_2_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_2_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_2_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_2_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_18_out[0], mux_tree_like_size40_19_out[0], mux_tree_like_size40_20_out[0], mux_tree_like_size40_21_out[0], mux_tree_like_size40_22_out[0], mux_tree_like_size40_23_out[0]}),
		.fle_cin(direct_interc_37_out[0]),
		.fle_sc_in(direct_interc_38_out[0]),
		.fle_regin(direct_interc_39_out[0]),
		.fle_clk(direct_interc_40_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_2_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_3_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_3_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_3_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_3_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_24_out[0], mux_tree_like_size40_25_out[0], mux_tree_like_size40_26_out[0], mux_tree_like_size40_27_out[0], mux_tree_like_size40_28_out[0], mux_tree_like_size40_29_out[0]}),
		.fle_cin(direct_interc_41_out[0]),
		.fle_sc_in(direct_interc_42_out[0]),
		.fle_regin(direct_interc_43_out[0]),
		.fle_clk(direct_interc_44_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_3_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_4_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_4_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_4_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_4_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_30_out[0], mux_tree_like_size40_31_out[0], mux_tree_like_size40_32_out[0], mux_tree_like_size40_33_out[0], mux_tree_like_size40_34_out[0], mux_tree_like_size40_35_out[0]}),
		.fle_cin(direct_interc_45_out[0]),
		.fle_sc_in(direct_interc_46_out[0]),
		.fle_regin(direct_interc_47_out[0]),
		.fle_clk(direct_interc_48_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_4_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_5_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_5_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_5_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_5_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_6 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_36_out[0], mux_tree_like_size40_37_out[0], mux_tree_like_size40_38_out[0], mux_tree_like_size40_39_out[0], mux_tree_like_size40_40_out[0], mux_tree_like_size40_41_out[0]}),
		.fle_cin(direct_interc_49_out[0]),
		.fle_sc_in(direct_interc_50_out[0]),
		.fle_regin(direct_interc_51_out[0]),
		.fle_clk(direct_interc_52_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_5_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_6_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_6_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_6_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_6_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_7 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_42_out[0], mux_tree_like_size40_43_out[0], mux_tree_like_size40_44_out[0], mux_tree_like_size40_45_out[0], mux_tree_like_size40_46_out[0], mux_tree_like_size40_47_out[0]}),
		.fle_cin(direct_interc_53_out[0]),
		.fle_sc_in(direct_interc_54_out[0]),
		.fle_regin(direct_interc_55_out[0]),
		.fle_clk(direct_interc_56_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_6_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_7_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_7_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_7_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_7_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle logical_tile_clb_spypad_mode_default__fle_8 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.fle_in({mux_tree_like_size40_48_out[0], mux_tree_like_size40_49_out[0], mux_tree_like_size40_50_out[0], mux_tree_like_size40_51_out[0], mux_tree_like_size40_52_out[0], mux_tree_like_size40_53_out[0]}),
		.fle_cin(direct_interc_57_out[0]),
		.fle_sc_in(direct_interc_58_out[0]),
		.fle_regin(direct_interc_59_out[0]),
		.fle_clk(direct_interc_60_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_7_ccff_tail[0]),
		.fle_out(logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1]),
		.fle_cout(logical_tile_clb_spypad_mode_default__fle_8_fle_cout[0]),
		.fle_sc_out(logical_tile_clb_spypad_mode_default__fle_8_fle_sc_out[0]),
		.fle_regout(logical_tile_clb_spypad_mode_default__fle_8_fle_regout[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_8_ccff_tail[0]));

	logical_tile_clb_spypad_mode_default__fle_spypad logical_tile_clb_spypad_mode_default__fle_spypad_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.Reset(Reset[0]),
		.Test_en(Test_en[0]),
		.clk(clk[0]),
		.gfpga_pad_frac_lut6_spypad_lut4_spy(gfpga_pad_frac_lut6_spypad_lut4_spy[0:3]),
		.gfpga_pad_frac_lut6_spypad_lut5_spy(gfpga_pad_frac_lut6_spypad_lut5_spy[0:1]),
		.gfpga_pad_frac_lut6_spypad_lut6_spy(gfpga_pad_frac_lut6_spypad_lut6_spy[0]),
		.fle_spypad_in({mux_tree_like_size40_54_out[0], mux_tree_like_size40_55_out[0], mux_tree_like_size40_56_out[0], mux_tree_like_size40_57_out[0], mux_tree_like_size40_58_out[0], mux_tree_like_size40_59_out[0]}),
		.fle_spypad_cin(mux_tree_like_size22_0_out[0]),
		.fle_spypad_sc_in(direct_interc_61_out[0]),
		.fle_spypad_regin(direct_interc_62_out[0]),
		.fle_spypad_clk(direct_interc_63_out[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_8_ccff_tail[0]),
		.fle_spypad_out(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]),
		.fle_spypad_cout(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_cout[0]),
		.fle_spypad_sc_out(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_sc_out[0]),
		.fle_spypad_regout(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_regout[0]),
		.fle_spypad_lut_perf(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_lut_perf[0]),
		.ccff_tail(logical_tile_clb_spypad_mode_default__fle_spypad_0_ccff_tail[0]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0]),
		.out(clb_spypad_O[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_clb_spypad_mode_default__fle_0_fle_out[0]),
		.out(clb_spypad_O[1]));

	direct_interc direct_interc_2_ (
		.in(logical_tile_clb_spypad_mode_default__fle_1_fle_out[0]),
		.out(clb_spypad_O[2]));

	direct_interc direct_interc_3_ (
		.in(logical_tile_clb_spypad_mode_default__fle_2_fle_out[0]),
		.out(clb_spypad_O[3]));

	direct_interc direct_interc_4_ (
		.in(logical_tile_clb_spypad_mode_default__fle_3_fle_out[0]),
		.out(clb_spypad_O[4]));

	direct_interc direct_interc_5_ (
		.in(logical_tile_clb_spypad_mode_default__fle_4_fle_out[0]),
		.out(clb_spypad_O[5]));

	direct_interc direct_interc_6_ (
		.in(logical_tile_clb_spypad_mode_default__fle_5_fle_out[0]),
		.out(clb_spypad_O[6]));

	direct_interc direct_interc_7_ (
		.in(logical_tile_clb_spypad_mode_default__fle_6_fle_out[0]),
		.out(clb_spypad_O[7]));

	direct_interc direct_interc_8_ (
		.in(logical_tile_clb_spypad_mode_default__fle_7_fle_out[0]),
		.out(clb_spypad_O[8]));

	direct_interc direct_interc_9_ (
		.in(logical_tile_clb_spypad_mode_default__fle_8_fle_out[0]),
		.out(clb_spypad_O[9]));

	direct_interc direct_interc_10_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[1]),
		.out(clb_spypad_O[10]));

	direct_interc direct_interc_11_ (
		.in(logical_tile_clb_spypad_mode_default__fle_0_fle_out[1]),
		.out(clb_spypad_O[11]));

	direct_interc direct_interc_12_ (
		.in(logical_tile_clb_spypad_mode_default__fle_1_fle_out[1]),
		.out(clb_spypad_O[12]));

	direct_interc direct_interc_13_ (
		.in(logical_tile_clb_spypad_mode_default__fle_2_fle_out[1]),
		.out(clb_spypad_O[13]));

	direct_interc direct_interc_14_ (
		.in(logical_tile_clb_spypad_mode_default__fle_3_fle_out[1]),
		.out(clb_spypad_O[14]));

	direct_interc direct_interc_15_ (
		.in(logical_tile_clb_spypad_mode_default__fle_4_fle_out[1]),
		.out(clb_spypad_O[15]));

	direct_interc direct_interc_16_ (
		.in(logical_tile_clb_spypad_mode_default__fle_5_fle_out[1]),
		.out(clb_spypad_O[16]));

	direct_interc direct_interc_17_ (
		.in(logical_tile_clb_spypad_mode_default__fle_6_fle_out[1]),
		.out(clb_spypad_O[17]));

	direct_interc direct_interc_18_ (
		.in(logical_tile_clb_spypad_mode_default__fle_7_fle_out[1]),
		.out(clb_spypad_O[18]));

	direct_interc direct_interc_19_ (
		.in(logical_tile_clb_spypad_mode_default__fle_8_fle_out[1]),
		.out(clb_spypad_O[19]));

	direct_interc direct_interc_20_ (
		.in(logical_tile_clb_spypad_mode_default__fle_8_fle_sc_out[0]),
		.out(clb_spypad_sc_out[0]));

	direct_interc direct_interc_21_ (
		.in(logical_tile_clb_spypad_mode_default__fle_8_fle_cout[0]),
		.out(clb_spypad_cout[0]));

	direct_interc direct_interc_22_ (
		.in(logical_tile_clb_spypad_mode_default__fle_8_fle_cout[0]),
		.out(clb_spypad_cout_copy[0]));

	direct_interc direct_interc_23_ (
		.in(logical_tile_clb_spypad_mode_default__fle_8_fle_regout[0]),
		.out(clb_spypad_regout[0]));

	direct_interc direct_interc_24_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_lut_perf[0]),
		.out(clb_spypad_lut_perf[0]));

	direct_interc direct_interc_25_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_cout[0]),
		.out(direct_interc_25_out[0]));

	direct_interc direct_interc_26_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_sc_out[0]),
		.out(direct_interc_26_out[0]));

	direct_interc direct_interc_27_ (
		.in(logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_regout[0]),
		.out(direct_interc_27_out[0]));

	direct_interc direct_interc_28_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_28_out[0]));

	direct_interc direct_interc_29_ (
		.in(logical_tile_clb_spypad_mode_default__fle_0_fle_cout[0]),
		.out(direct_interc_29_out[0]));

	direct_interc direct_interc_30_ (
		.in(logical_tile_clb_spypad_mode_default__fle_0_fle_sc_out[0]),
		.out(direct_interc_30_out[0]));

	direct_interc direct_interc_31_ (
		.in(logical_tile_clb_spypad_mode_default__fle_0_fle_regout[0]),
		.out(direct_interc_31_out[0]));

	direct_interc direct_interc_32_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_32_out[0]));

	direct_interc direct_interc_33_ (
		.in(logical_tile_clb_spypad_mode_default__fle_1_fle_cout[0]),
		.out(direct_interc_33_out[0]));

	direct_interc direct_interc_34_ (
		.in(logical_tile_clb_spypad_mode_default__fle_1_fle_sc_out[0]),
		.out(direct_interc_34_out[0]));

	direct_interc direct_interc_35_ (
		.in(logical_tile_clb_spypad_mode_default__fle_1_fle_regout[0]),
		.out(direct_interc_35_out[0]));

	direct_interc direct_interc_36_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_36_out[0]));

	direct_interc direct_interc_37_ (
		.in(logical_tile_clb_spypad_mode_default__fle_2_fle_cout[0]),
		.out(direct_interc_37_out[0]));

	direct_interc direct_interc_38_ (
		.in(logical_tile_clb_spypad_mode_default__fle_2_fle_sc_out[0]),
		.out(direct_interc_38_out[0]));

	direct_interc direct_interc_39_ (
		.in(logical_tile_clb_spypad_mode_default__fle_2_fle_regout[0]),
		.out(direct_interc_39_out[0]));

	direct_interc direct_interc_40_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_40_out[0]));

	direct_interc direct_interc_41_ (
		.in(logical_tile_clb_spypad_mode_default__fle_3_fle_cout[0]),
		.out(direct_interc_41_out[0]));

	direct_interc direct_interc_42_ (
		.in(logical_tile_clb_spypad_mode_default__fle_3_fle_sc_out[0]),
		.out(direct_interc_42_out[0]));

	direct_interc direct_interc_43_ (
		.in(logical_tile_clb_spypad_mode_default__fle_3_fle_regout[0]),
		.out(direct_interc_43_out[0]));

	direct_interc direct_interc_44_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_44_out[0]));

	direct_interc direct_interc_45_ (
		.in(logical_tile_clb_spypad_mode_default__fle_4_fle_cout[0]),
		.out(direct_interc_45_out[0]));

	direct_interc direct_interc_46_ (
		.in(logical_tile_clb_spypad_mode_default__fle_4_fle_sc_out[0]),
		.out(direct_interc_46_out[0]));

	direct_interc direct_interc_47_ (
		.in(logical_tile_clb_spypad_mode_default__fle_4_fle_regout[0]),
		.out(direct_interc_47_out[0]));

	direct_interc direct_interc_48_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_48_out[0]));

	direct_interc direct_interc_49_ (
		.in(logical_tile_clb_spypad_mode_default__fle_5_fle_cout[0]),
		.out(direct_interc_49_out[0]));

	direct_interc direct_interc_50_ (
		.in(logical_tile_clb_spypad_mode_default__fle_5_fle_sc_out[0]),
		.out(direct_interc_50_out[0]));

	direct_interc direct_interc_51_ (
		.in(logical_tile_clb_spypad_mode_default__fle_5_fle_regout[0]),
		.out(direct_interc_51_out[0]));

	direct_interc direct_interc_52_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_52_out[0]));

	direct_interc direct_interc_53_ (
		.in(logical_tile_clb_spypad_mode_default__fle_6_fle_cout[0]),
		.out(direct_interc_53_out[0]));

	direct_interc direct_interc_54_ (
		.in(logical_tile_clb_spypad_mode_default__fle_6_fle_sc_out[0]),
		.out(direct_interc_54_out[0]));

	direct_interc direct_interc_55_ (
		.in(logical_tile_clb_spypad_mode_default__fle_6_fle_regout[0]),
		.out(direct_interc_55_out[0]));

	direct_interc direct_interc_56_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_56_out[0]));

	direct_interc direct_interc_57_ (
		.in(logical_tile_clb_spypad_mode_default__fle_7_fle_cout[0]),
		.out(direct_interc_57_out[0]));

	direct_interc direct_interc_58_ (
		.in(logical_tile_clb_spypad_mode_default__fle_7_fle_sc_out[0]),
		.out(direct_interc_58_out[0]));

	direct_interc direct_interc_59_ (
		.in(logical_tile_clb_spypad_mode_default__fle_7_fle_regout[0]),
		.out(direct_interc_59_out[0]));

	direct_interc direct_interc_60_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_60_out[0]));

	direct_interc direct_interc_61_ (
		.in(clb_spypad_sc_in[0]),
		.out(direct_interc_61_out[0]));

	direct_interc direct_interc_62_ (
		.in(clb_spypad_regin[0]),
		.out(direct_interc_62_out[0]));

	direct_interc direct_interc_63_ (
		.in(clb_spypad_clk[0]),
		.out(direct_interc_63_out[0]));

	mux_tree_like_size40 mux_fle_0_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_0_sram[0:5]),
		.sram_inv(mux_tree_like_size40_0_sram_inv[0:5]),
		.out(mux_tree_like_size40_0_out[0]));

	mux_tree_like_size40 mux_fle_0_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_1_sram[0:5]),
		.sram_inv(mux_tree_like_size40_1_sram_inv[0:5]),
		.out(mux_tree_like_size40_1_out[0]));

	mux_tree_like_size40 mux_fle_0_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_2_sram[0:5]),
		.sram_inv(mux_tree_like_size40_2_sram_inv[0:5]),
		.out(mux_tree_like_size40_2_out[0]));

	mux_tree_like_size40 mux_fle_0_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_3_sram[0:5]),
		.sram_inv(mux_tree_like_size40_3_sram_inv[0:5]),
		.out(mux_tree_like_size40_3_out[0]));

	mux_tree_like_size40 mux_fle_0_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_4_sram[0:5]),
		.sram_inv(mux_tree_like_size40_4_sram_inv[0:5]),
		.out(mux_tree_like_size40_4_out[0]));

	mux_tree_like_size40 mux_fle_0_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_5_sram[0:5]),
		.sram_inv(mux_tree_like_size40_5_sram_inv[0:5]),
		.out(mux_tree_like_size40_5_out[0]));

	mux_tree_like_size40 mux_fle_1_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_6_sram[0:5]),
		.sram_inv(mux_tree_like_size40_6_sram_inv[0:5]),
		.out(mux_tree_like_size40_6_out[0]));

	mux_tree_like_size40 mux_fle_1_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_7_sram[0:5]),
		.sram_inv(mux_tree_like_size40_7_sram_inv[0:5]),
		.out(mux_tree_like_size40_7_out[0]));

	mux_tree_like_size40 mux_fle_1_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_8_sram[0:5]),
		.sram_inv(mux_tree_like_size40_8_sram_inv[0:5]),
		.out(mux_tree_like_size40_8_out[0]));

	mux_tree_like_size40 mux_fle_1_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_9_sram[0:5]),
		.sram_inv(mux_tree_like_size40_9_sram_inv[0:5]),
		.out(mux_tree_like_size40_9_out[0]));

	mux_tree_like_size40 mux_fle_1_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_10_sram[0:5]),
		.sram_inv(mux_tree_like_size40_10_sram_inv[0:5]),
		.out(mux_tree_like_size40_10_out[0]));

	mux_tree_like_size40 mux_fle_1_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_11_sram[0:5]),
		.sram_inv(mux_tree_like_size40_11_sram_inv[0:5]),
		.out(mux_tree_like_size40_11_out[0]));

	mux_tree_like_size40 mux_fle_2_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_12_sram[0:5]),
		.sram_inv(mux_tree_like_size40_12_sram_inv[0:5]),
		.out(mux_tree_like_size40_12_out[0]));

	mux_tree_like_size40 mux_fle_2_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_13_sram[0:5]),
		.sram_inv(mux_tree_like_size40_13_sram_inv[0:5]),
		.out(mux_tree_like_size40_13_out[0]));

	mux_tree_like_size40 mux_fle_2_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_14_sram[0:5]),
		.sram_inv(mux_tree_like_size40_14_sram_inv[0:5]),
		.out(mux_tree_like_size40_14_out[0]));

	mux_tree_like_size40 mux_fle_2_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_15_sram[0:5]),
		.sram_inv(mux_tree_like_size40_15_sram_inv[0:5]),
		.out(mux_tree_like_size40_15_out[0]));

	mux_tree_like_size40 mux_fle_2_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_16_sram[0:5]),
		.sram_inv(mux_tree_like_size40_16_sram_inv[0:5]),
		.out(mux_tree_like_size40_16_out[0]));

	mux_tree_like_size40 mux_fle_2_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_17_sram[0:5]),
		.sram_inv(mux_tree_like_size40_17_sram_inv[0:5]),
		.out(mux_tree_like_size40_17_out[0]));

	mux_tree_like_size40 mux_fle_3_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_18_sram[0:5]),
		.sram_inv(mux_tree_like_size40_18_sram_inv[0:5]),
		.out(mux_tree_like_size40_18_out[0]));

	mux_tree_like_size40 mux_fle_3_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_19_sram[0:5]),
		.sram_inv(mux_tree_like_size40_19_sram_inv[0:5]),
		.out(mux_tree_like_size40_19_out[0]));

	mux_tree_like_size40 mux_fle_3_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_20_sram[0:5]),
		.sram_inv(mux_tree_like_size40_20_sram_inv[0:5]),
		.out(mux_tree_like_size40_20_out[0]));

	mux_tree_like_size40 mux_fle_3_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_21_sram[0:5]),
		.sram_inv(mux_tree_like_size40_21_sram_inv[0:5]),
		.out(mux_tree_like_size40_21_out[0]));

	mux_tree_like_size40 mux_fle_3_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_22_sram[0:5]),
		.sram_inv(mux_tree_like_size40_22_sram_inv[0:5]),
		.out(mux_tree_like_size40_22_out[0]));

	mux_tree_like_size40 mux_fle_3_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_23_sram[0:5]),
		.sram_inv(mux_tree_like_size40_23_sram_inv[0:5]),
		.out(mux_tree_like_size40_23_out[0]));

	mux_tree_like_size40 mux_fle_4_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_24_sram[0:5]),
		.sram_inv(mux_tree_like_size40_24_sram_inv[0:5]),
		.out(mux_tree_like_size40_24_out[0]));

	mux_tree_like_size40 mux_fle_4_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_25_sram[0:5]),
		.sram_inv(mux_tree_like_size40_25_sram_inv[0:5]),
		.out(mux_tree_like_size40_25_out[0]));

	mux_tree_like_size40 mux_fle_4_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_26_sram[0:5]),
		.sram_inv(mux_tree_like_size40_26_sram_inv[0:5]),
		.out(mux_tree_like_size40_26_out[0]));

	mux_tree_like_size40 mux_fle_4_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_27_sram[0:5]),
		.sram_inv(mux_tree_like_size40_27_sram_inv[0:5]),
		.out(mux_tree_like_size40_27_out[0]));

	mux_tree_like_size40 mux_fle_4_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_28_sram[0:5]),
		.sram_inv(mux_tree_like_size40_28_sram_inv[0:5]),
		.out(mux_tree_like_size40_28_out[0]));

	mux_tree_like_size40 mux_fle_4_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_29_sram[0:5]),
		.sram_inv(mux_tree_like_size40_29_sram_inv[0:5]),
		.out(mux_tree_like_size40_29_out[0]));

	mux_tree_like_size40 mux_fle_5_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_30_sram[0:5]),
		.sram_inv(mux_tree_like_size40_30_sram_inv[0:5]),
		.out(mux_tree_like_size40_30_out[0]));

	mux_tree_like_size40 mux_fle_5_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_31_sram[0:5]),
		.sram_inv(mux_tree_like_size40_31_sram_inv[0:5]),
		.out(mux_tree_like_size40_31_out[0]));

	mux_tree_like_size40 mux_fle_5_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_32_sram[0:5]),
		.sram_inv(mux_tree_like_size40_32_sram_inv[0:5]),
		.out(mux_tree_like_size40_32_out[0]));

	mux_tree_like_size40 mux_fle_5_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_33_sram[0:5]),
		.sram_inv(mux_tree_like_size40_33_sram_inv[0:5]),
		.out(mux_tree_like_size40_33_out[0]));

	mux_tree_like_size40 mux_fle_5_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_34_sram[0:5]),
		.sram_inv(mux_tree_like_size40_34_sram_inv[0:5]),
		.out(mux_tree_like_size40_34_out[0]));

	mux_tree_like_size40 mux_fle_5_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_35_sram[0:5]),
		.sram_inv(mux_tree_like_size40_35_sram_inv[0:5]),
		.out(mux_tree_like_size40_35_out[0]));

	mux_tree_like_size40 mux_fle_6_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_36_sram[0:5]),
		.sram_inv(mux_tree_like_size40_36_sram_inv[0:5]),
		.out(mux_tree_like_size40_36_out[0]));

	mux_tree_like_size40 mux_fle_6_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_37_sram[0:5]),
		.sram_inv(mux_tree_like_size40_37_sram_inv[0:5]),
		.out(mux_tree_like_size40_37_out[0]));

	mux_tree_like_size40 mux_fle_6_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_38_sram[0:5]),
		.sram_inv(mux_tree_like_size40_38_sram_inv[0:5]),
		.out(mux_tree_like_size40_38_out[0]));

	mux_tree_like_size40 mux_fle_6_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_39_sram[0:5]),
		.sram_inv(mux_tree_like_size40_39_sram_inv[0:5]),
		.out(mux_tree_like_size40_39_out[0]));

	mux_tree_like_size40 mux_fle_6_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_40_sram[0:5]),
		.sram_inv(mux_tree_like_size40_40_sram_inv[0:5]),
		.out(mux_tree_like_size40_40_out[0]));

	mux_tree_like_size40 mux_fle_6_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_41_sram[0:5]),
		.sram_inv(mux_tree_like_size40_41_sram_inv[0:5]),
		.out(mux_tree_like_size40_41_out[0]));

	mux_tree_like_size40 mux_fle_7_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_42_sram[0:5]),
		.sram_inv(mux_tree_like_size40_42_sram_inv[0:5]),
		.out(mux_tree_like_size40_42_out[0]));

	mux_tree_like_size40 mux_fle_7_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_43_sram[0:5]),
		.sram_inv(mux_tree_like_size40_43_sram_inv[0:5]),
		.out(mux_tree_like_size40_43_out[0]));

	mux_tree_like_size40 mux_fle_7_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_44_sram[0:5]),
		.sram_inv(mux_tree_like_size40_44_sram_inv[0:5]),
		.out(mux_tree_like_size40_44_out[0]));

	mux_tree_like_size40 mux_fle_7_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_45_sram[0:5]),
		.sram_inv(mux_tree_like_size40_45_sram_inv[0:5]),
		.out(mux_tree_like_size40_45_out[0]));

	mux_tree_like_size40 mux_fle_7_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_46_sram[0:5]),
		.sram_inv(mux_tree_like_size40_46_sram_inv[0:5]),
		.out(mux_tree_like_size40_46_out[0]));

	mux_tree_like_size40 mux_fle_7_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_47_sram[0:5]),
		.sram_inv(mux_tree_like_size40_47_sram_inv[0:5]),
		.out(mux_tree_like_size40_47_out[0]));

	mux_tree_like_size40 mux_fle_8_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_48_sram[0:5]),
		.sram_inv(mux_tree_like_size40_48_sram_inv[0:5]),
		.out(mux_tree_like_size40_48_out[0]));

	mux_tree_like_size40 mux_fle_8_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_49_sram[0:5]),
		.sram_inv(mux_tree_like_size40_49_sram_inv[0:5]),
		.out(mux_tree_like_size40_49_out[0]));

	mux_tree_like_size40 mux_fle_8_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_50_sram[0:5]),
		.sram_inv(mux_tree_like_size40_50_sram_inv[0:5]),
		.out(mux_tree_like_size40_50_out[0]));

	mux_tree_like_size40 mux_fle_8_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_51_sram[0:5]),
		.sram_inv(mux_tree_like_size40_51_sram_inv[0:5]),
		.out(mux_tree_like_size40_51_out[0]));

	mux_tree_like_size40 mux_fle_8_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_52_sram[0:5]),
		.sram_inv(mux_tree_like_size40_52_sram_inv[0:5]),
		.out(mux_tree_like_size40_52_out[0]));

	mux_tree_like_size40 mux_fle_8_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_53_sram[0:5]),
		.sram_inv(mux_tree_like_size40_53_sram_inv[0:5]),
		.out(mux_tree_like_size40_53_out[0]));

	mux_tree_like_size40 mux_fle_spypad_0_in_0 (
		.in({clb_spypad_I2[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_54_sram[0:5]),
		.sram_inv(mux_tree_like_size40_54_sram_inv[0:5]),
		.out(mux_tree_like_size40_54_out[0]));

	mux_tree_like_size40 mux_fle_spypad_0_in_1 (
		.in({clb_spypad_I1[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_55_sram[0:5]),
		.sram_inv(mux_tree_like_size40_55_sram_inv[0:5]),
		.out(mux_tree_like_size40_55_out[0]));

	mux_tree_like_size40 mux_fle_spypad_0_in_2 (
		.in({clb_spypad_I0[0:9], clb_spypad_I1[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_56_sram[0:5]),
		.sram_inv(mux_tree_like_size40_56_sram_inv[0:5]),
		.out(mux_tree_like_size40_56_out[0]));

	mux_tree_like_size40 mux_fle_spypad_0_in_3 (
		.in({clb_spypad_I1[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_57_sram[0:5]),
		.sram_inv(mux_tree_like_size40_57_sram_inv[0:5]),
		.out(mux_tree_like_size40_57_out[0]));

	mux_tree_like_size40 mux_fle_spypad_0_in_4 (
		.in({clb_spypad_I0[0:9], clb_spypad_I2[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_58_sram[0:5]),
		.sram_inv(mux_tree_like_size40_58_sram_inv[0:5]),
		.out(mux_tree_like_size40_58_out[0]));

	mux_tree_like_size40 mux_fle_spypad_0_in_5 (
		.in({clb_spypad_I0[0:9], clb_spypad_I3[0:9], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size40_59_sram[0:5]),
		.sram_inv(mux_tree_like_size40_59_sram_inv[0:5]),
		.out(mux_tree_like_size40_59_out[0]));

	mux_tree_like_size40_mem mem_fle_0_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(logical_tile_clb_spypad_mode_default__fle_spypad_0_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_0_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_0_sram[0:5]),
		.mem_outb(mux_tree_like_size40_0_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_0_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_0_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_1_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_1_sram[0:5]),
		.mem_outb(mux_tree_like_size40_1_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_0_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_1_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_2_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_2_sram[0:5]),
		.mem_outb(mux_tree_like_size40_2_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_0_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_2_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_3_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_3_sram[0:5]),
		.mem_outb(mux_tree_like_size40_3_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_0_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_3_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_4_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_4_sram[0:5]),
		.mem_outb(mux_tree_like_size40_4_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_0_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_4_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_5_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_5_sram[0:5]),
		.mem_outb(mux_tree_like_size40_5_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_1_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_5_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_6_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_6_sram[0:5]),
		.mem_outb(mux_tree_like_size40_6_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_1_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_6_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_7_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_7_sram[0:5]),
		.mem_outb(mux_tree_like_size40_7_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_1_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_7_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_8_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_8_sram[0:5]),
		.mem_outb(mux_tree_like_size40_8_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_1_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_8_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_9_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_9_sram[0:5]),
		.mem_outb(mux_tree_like_size40_9_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_1_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_9_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_10_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_10_sram[0:5]),
		.mem_outb(mux_tree_like_size40_10_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_1_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_10_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_11_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_11_sram[0:5]),
		.mem_outb(mux_tree_like_size40_11_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_2_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_11_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_12_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_12_sram[0:5]),
		.mem_outb(mux_tree_like_size40_12_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_2_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_12_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_13_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_13_sram[0:5]),
		.mem_outb(mux_tree_like_size40_13_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_2_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_13_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_14_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_14_sram[0:5]),
		.mem_outb(mux_tree_like_size40_14_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_2_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_14_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_15_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_15_sram[0:5]),
		.mem_outb(mux_tree_like_size40_15_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_2_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_15_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_16_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_16_sram[0:5]),
		.mem_outb(mux_tree_like_size40_16_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_2_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_16_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_17_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_17_sram[0:5]),
		.mem_outb(mux_tree_like_size40_17_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_3_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_17_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_18_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_18_sram[0:5]),
		.mem_outb(mux_tree_like_size40_18_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_3_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_18_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_19_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_19_sram[0:5]),
		.mem_outb(mux_tree_like_size40_19_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_3_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_19_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_20_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_20_sram[0:5]),
		.mem_outb(mux_tree_like_size40_20_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_3_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_20_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_21_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_21_sram[0:5]),
		.mem_outb(mux_tree_like_size40_21_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_3_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_21_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_22_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_22_sram[0:5]),
		.mem_outb(mux_tree_like_size40_22_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_3_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_22_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_23_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_23_sram[0:5]),
		.mem_outb(mux_tree_like_size40_23_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_4_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_23_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_24_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_24_sram[0:5]),
		.mem_outb(mux_tree_like_size40_24_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_4_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_24_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_25_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_25_sram[0:5]),
		.mem_outb(mux_tree_like_size40_25_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_4_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_25_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_26_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_26_sram[0:5]),
		.mem_outb(mux_tree_like_size40_26_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_4_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_26_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_27_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_27_sram[0:5]),
		.mem_outb(mux_tree_like_size40_27_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_4_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_27_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_28_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_28_sram[0:5]),
		.mem_outb(mux_tree_like_size40_28_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_4_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_28_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_29_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_29_sram[0:5]),
		.mem_outb(mux_tree_like_size40_29_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_5_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_29_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_30_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_30_sram[0:5]),
		.mem_outb(mux_tree_like_size40_30_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_5_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_30_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_31_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_31_sram[0:5]),
		.mem_outb(mux_tree_like_size40_31_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_5_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_31_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_32_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_32_sram[0:5]),
		.mem_outb(mux_tree_like_size40_32_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_5_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_32_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_33_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_33_sram[0:5]),
		.mem_outb(mux_tree_like_size40_33_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_5_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_33_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_34_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_34_sram[0:5]),
		.mem_outb(mux_tree_like_size40_34_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_5_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_34_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_35_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_35_sram[0:5]),
		.mem_outb(mux_tree_like_size40_35_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_6_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_35_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_36_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_36_sram[0:5]),
		.mem_outb(mux_tree_like_size40_36_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_6_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_36_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_37_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_37_sram[0:5]),
		.mem_outb(mux_tree_like_size40_37_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_6_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_37_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_38_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_38_sram[0:5]),
		.mem_outb(mux_tree_like_size40_38_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_6_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_38_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_39_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_39_sram[0:5]),
		.mem_outb(mux_tree_like_size40_39_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_6_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_39_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_40_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_40_sram[0:5]),
		.mem_outb(mux_tree_like_size40_40_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_6_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_40_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_41_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_41_sram[0:5]),
		.mem_outb(mux_tree_like_size40_41_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_7_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_41_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_42_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_42_sram[0:5]),
		.mem_outb(mux_tree_like_size40_42_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_7_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_42_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_43_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_43_sram[0:5]),
		.mem_outb(mux_tree_like_size40_43_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_7_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_43_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_44_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_44_sram[0:5]),
		.mem_outb(mux_tree_like_size40_44_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_7_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_44_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_45_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_45_sram[0:5]),
		.mem_outb(mux_tree_like_size40_45_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_7_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_45_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_46_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_46_sram[0:5]),
		.mem_outb(mux_tree_like_size40_46_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_7_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_46_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_47_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_47_sram[0:5]),
		.mem_outb(mux_tree_like_size40_47_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_8_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_47_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_48_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_48_sram[0:5]),
		.mem_outb(mux_tree_like_size40_48_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_8_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_48_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_49_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_49_sram[0:5]),
		.mem_outb(mux_tree_like_size40_49_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_8_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_49_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_50_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_50_sram[0:5]),
		.mem_outb(mux_tree_like_size40_50_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_8_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_50_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_51_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_51_sram[0:5]),
		.mem_outb(mux_tree_like_size40_51_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_8_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_51_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_52_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_52_sram[0:5]),
		.mem_outb(mux_tree_like_size40_52_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_8_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_52_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_53_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_53_sram[0:5]),
		.mem_outb(mux_tree_like_size40_53_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_spypad_0_in_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_53_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_54_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_54_sram[0:5]),
		.mem_outb(mux_tree_like_size40_54_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_spypad_0_in_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_54_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_55_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_55_sram[0:5]),
		.mem_outb(mux_tree_like_size40_55_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_spypad_0_in_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_55_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_56_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_56_sram[0:5]),
		.mem_outb(mux_tree_like_size40_56_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_spypad_0_in_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_56_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_57_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_57_sram[0:5]),
		.mem_outb(mux_tree_like_size40_57_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_spypad_0_in_4 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_57_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_58_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_58_sram[0:5]),
		.mem_outb(mux_tree_like_size40_58_sram_inv[0:5]));

	mux_tree_like_size40_mem mem_fle_spypad_0_in_5 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_58_ccff_tail[0]),
		.ccff_tail(mux_tree_like_size40_mem_59_ccff_tail[0]),
		.mem_out(mux_tree_like_size40_59_sram[0:5]),
		.mem_outb(mux_tree_like_size40_59_sram_inv[0:5]));

	mux_tree_like_size22 mux_fle_spypad_0_cin_0 (
		.in({clb_spypad_cin[0], clb_spypad_cin_trick[0], logical_tile_clb_spypad_mode_default__fle_0_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_1_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_2_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_3_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_4_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_5_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_6_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_7_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_8_fle_out[0:1], logical_tile_clb_spypad_mode_default__fle_spypad_0_fle_spypad_out[0:1]}),
		.sram(mux_tree_like_size22_0_sram[0:4]),
		.sram_inv(mux_tree_like_size22_0_sram_inv[0:4]),
		.out(mux_tree_like_size22_0_out[0]));

	mux_tree_like_size22_mem mem_fle_spypad_0_cin_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.ccff_head(mux_tree_like_size40_mem_59_ccff_tail[0]),
		.ccff_tail(ccff_tail[0]),
		.mem_out(mux_tree_like_size22_0_sram[0:4]),
		.mem_outb(mux_tree_like_size22_0_sram_inv[0:4]));

endmodule
// ----- END Verilog module for logical_tile_clb_spypad_mode_clb_spypad_ -----


// ----- END Physical programmable logic block Verilog module: clb_spypad -----


