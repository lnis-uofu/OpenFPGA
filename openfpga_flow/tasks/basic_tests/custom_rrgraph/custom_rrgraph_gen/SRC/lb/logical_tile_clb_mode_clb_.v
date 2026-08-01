//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: clb
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: clb -----
// ----- Verilog module for logical_tile_clb_mode_clb_ -----
module logical_tile_clb_mode_clb_(pReset,
                                  prog_clk,
                                  set,
                                  reset,
                                  clk,
                                  clb_I,
                                  clb_clk,
                                  ccff_head,
                                  clb_O,
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
input [11:0] clb_I;
//----- INPUT PORTS -----
input [0:0] clb_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [7:0] clb_O;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [1:0] logical_tile_clb_mode_default__fle_0_fle_out;
wire [1:0] logical_tile_clb_mode_default__fle_1_fle_out;
wire [1:0] logical_tile_clb_mode_default__fle_2_fle_out;
wire [1:0] logical_tile_clb_mode_default__fle_3_fle_out;
wire [9:0] mux_2level_size20_0_sram;
wire [9:0] mux_2level_size20_0_sram_inv;
wire [9:0] mux_2level_size20_10_sram;
wire [9:0] mux_2level_size20_10_sram_inv;
wire [9:0] mux_2level_size20_11_sram;
wire [9:0] mux_2level_size20_11_sram_inv;
wire [9:0] mux_2level_size20_12_sram;
wire [9:0] mux_2level_size20_12_sram_inv;
wire [9:0] mux_2level_size20_13_sram;
wire [9:0] mux_2level_size20_13_sram_inv;
wire [9:0] mux_2level_size20_14_sram;
wire [9:0] mux_2level_size20_14_sram_inv;
wire [9:0] mux_2level_size20_15_sram;
wire [9:0] mux_2level_size20_15_sram_inv;
wire [9:0] mux_2level_size20_1_sram;
wire [9:0] mux_2level_size20_1_sram_inv;
wire [9:0] mux_2level_size20_2_sram;
wire [9:0] mux_2level_size20_2_sram_inv;
wire [9:0] mux_2level_size20_3_sram;
wire [9:0] mux_2level_size20_3_sram_inv;
wire [9:0] mux_2level_size20_4_sram;
wire [9:0] mux_2level_size20_4_sram_inv;
wire [9:0] mux_2level_size20_5_sram;
wire [9:0] mux_2level_size20_5_sram_inv;
wire [9:0] mux_2level_size20_6_sram;
wire [9:0] mux_2level_size20_6_sram_inv;
wire [9:0] mux_2level_size20_7_sram;
wire [9:0] mux_2level_size20_7_sram_inv;
wire [9:0] mux_2level_size20_8_sram;
wire [9:0] mux_2level_size20_8_sram_inv;
wire [9:0] mux_2level_size20_9_sram;
wire [9:0] mux_2level_size20_9_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_3_out, mux_2level_size20_2_out, mux_2level_size20_1_out, mux_2level_size20_0_out}),
		.fle_clk(direct_interc_8_out),
		.ccff_head(ccff_head),
		.fle_out({logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0]}),
		.ccff_tail(logical_tile_clb_mode_default__fle_0_ccff_tail));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_7_out, mux_2level_size20_6_out, mux_2level_size20_5_out, mux_2level_size20_4_out}),
		.fle_clk(direct_interc_9_out),
		.ccff_head(logical_tile_clb_mode_default__fle_0_ccff_tail),
		.fle_out({logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0]}),
		.ccff_tail(logical_tile_clb_mode_default__fle_1_ccff_tail));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_11_out, mux_2level_size20_10_out, mux_2level_size20_9_out, mux_2level_size20_8_out}),
		.fle_clk(direct_interc_10_out),
		.ccff_head(logical_tile_clb_mode_default__fle_1_ccff_tail),
		.fle_out({logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0]}),
		.ccff_tail(logical_tile_clb_mode_default__fle_2_ccff_tail));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_15_out, mux_2level_size20_14_out, mux_2level_size20_13_out, mux_2level_size20_12_out}),
		.fle_clk(direct_interc_11_out),
		.ccff_head(logical_tile_clb_mode_default__fle_2_ccff_tail),
		.fle_out({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0]}),
		.ccff_tail(logical_tile_clb_mode_default__fle_3_ccff_tail));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_mode_default__fle_0_fle_out[0]),
		.out(clb_O[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_clb_mode_default__fle_1_fle_out[0]),
		.out(clb_O[1]));

	direct_interc direct_interc_2_ (
		.in(logical_tile_clb_mode_default__fle_2_fle_out[0]),
		.out(clb_O[2]));

	direct_interc direct_interc_3_ (
		.in(logical_tile_clb_mode_default__fle_3_fle_out[0]),
		.out(clb_O[3]));

	direct_interc direct_interc_4_ (
		.in(logical_tile_clb_mode_default__fle_0_fle_out[1]),
		.out(clb_O[4]));

	direct_interc direct_interc_5_ (
		.in(logical_tile_clb_mode_default__fle_1_fle_out[1]),
		.out(clb_O[5]));

	direct_interc direct_interc_6_ (
		.in(logical_tile_clb_mode_default__fle_2_fle_out[1]),
		.out(clb_O[6]));

	direct_interc direct_interc_7_ (
		.in(logical_tile_clb_mode_default__fle_3_fle_out[1]),
		.out(clb_O[7]));

	direct_interc direct_interc_8_ (
		.in(clb_clk),
		.out(direct_interc_8_out));

	direct_interc direct_interc_9_ (
		.in(clb_clk),
		.out(direct_interc_9_out));

	direct_interc direct_interc_10_ (
		.in(clb_clk),
		.out(direct_interc_10_out));

	direct_interc direct_interc_11_ (
		.in(clb_clk),
		.out(direct_interc_11_out));

	mux_2level_size20 mux_fle_0_in_0 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_0_sram[9], mux_2level_size20_0_sram[8], mux_2level_size20_0_sram[7], mux_2level_size20_0_sram[6], mux_2level_size20_0_sram[5], mux_2level_size20_0_sram[4], mux_2level_size20_0_sram[3], mux_2level_size20_0_sram[2], mux_2level_size20_0_sram[1], mux_2level_size20_0_sram[0]}),
		.sram_inv({mux_2level_size20_0_sram_inv[9], mux_2level_size20_0_sram_inv[8], mux_2level_size20_0_sram_inv[7], mux_2level_size20_0_sram_inv[6], mux_2level_size20_0_sram_inv[5], mux_2level_size20_0_sram_inv[4], mux_2level_size20_0_sram_inv[3], mux_2level_size20_0_sram_inv[2], mux_2level_size20_0_sram_inv[1], mux_2level_size20_0_sram_inv[0]}),
		.out(mux_2level_size20_0_out));

	mux_2level_size20 mux_fle_0_in_1 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_1_sram[9], mux_2level_size20_1_sram[8], mux_2level_size20_1_sram[7], mux_2level_size20_1_sram[6], mux_2level_size20_1_sram[5], mux_2level_size20_1_sram[4], mux_2level_size20_1_sram[3], mux_2level_size20_1_sram[2], mux_2level_size20_1_sram[1], mux_2level_size20_1_sram[0]}),
		.sram_inv({mux_2level_size20_1_sram_inv[9], mux_2level_size20_1_sram_inv[8], mux_2level_size20_1_sram_inv[7], mux_2level_size20_1_sram_inv[6], mux_2level_size20_1_sram_inv[5], mux_2level_size20_1_sram_inv[4], mux_2level_size20_1_sram_inv[3], mux_2level_size20_1_sram_inv[2], mux_2level_size20_1_sram_inv[1], mux_2level_size20_1_sram_inv[0]}),
		.out(mux_2level_size20_1_out));

	mux_2level_size20 mux_fle_0_in_2 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_2_sram[9], mux_2level_size20_2_sram[8], mux_2level_size20_2_sram[7], mux_2level_size20_2_sram[6], mux_2level_size20_2_sram[5], mux_2level_size20_2_sram[4], mux_2level_size20_2_sram[3], mux_2level_size20_2_sram[2], mux_2level_size20_2_sram[1], mux_2level_size20_2_sram[0]}),
		.sram_inv({mux_2level_size20_2_sram_inv[9], mux_2level_size20_2_sram_inv[8], mux_2level_size20_2_sram_inv[7], mux_2level_size20_2_sram_inv[6], mux_2level_size20_2_sram_inv[5], mux_2level_size20_2_sram_inv[4], mux_2level_size20_2_sram_inv[3], mux_2level_size20_2_sram_inv[2], mux_2level_size20_2_sram_inv[1], mux_2level_size20_2_sram_inv[0]}),
		.out(mux_2level_size20_2_out));

	mux_2level_size20 mux_fle_0_in_3 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_3_sram[9], mux_2level_size20_3_sram[8], mux_2level_size20_3_sram[7], mux_2level_size20_3_sram[6], mux_2level_size20_3_sram[5], mux_2level_size20_3_sram[4], mux_2level_size20_3_sram[3], mux_2level_size20_3_sram[2], mux_2level_size20_3_sram[1], mux_2level_size20_3_sram[0]}),
		.sram_inv({mux_2level_size20_3_sram_inv[9], mux_2level_size20_3_sram_inv[8], mux_2level_size20_3_sram_inv[7], mux_2level_size20_3_sram_inv[6], mux_2level_size20_3_sram_inv[5], mux_2level_size20_3_sram_inv[4], mux_2level_size20_3_sram_inv[3], mux_2level_size20_3_sram_inv[2], mux_2level_size20_3_sram_inv[1], mux_2level_size20_3_sram_inv[0]}),
		.out(mux_2level_size20_3_out));

	mux_2level_size20 mux_fle_1_in_0 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_4_sram[9], mux_2level_size20_4_sram[8], mux_2level_size20_4_sram[7], mux_2level_size20_4_sram[6], mux_2level_size20_4_sram[5], mux_2level_size20_4_sram[4], mux_2level_size20_4_sram[3], mux_2level_size20_4_sram[2], mux_2level_size20_4_sram[1], mux_2level_size20_4_sram[0]}),
		.sram_inv({mux_2level_size20_4_sram_inv[9], mux_2level_size20_4_sram_inv[8], mux_2level_size20_4_sram_inv[7], mux_2level_size20_4_sram_inv[6], mux_2level_size20_4_sram_inv[5], mux_2level_size20_4_sram_inv[4], mux_2level_size20_4_sram_inv[3], mux_2level_size20_4_sram_inv[2], mux_2level_size20_4_sram_inv[1], mux_2level_size20_4_sram_inv[0]}),
		.out(mux_2level_size20_4_out));

	mux_2level_size20 mux_fle_1_in_1 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_5_sram[9], mux_2level_size20_5_sram[8], mux_2level_size20_5_sram[7], mux_2level_size20_5_sram[6], mux_2level_size20_5_sram[5], mux_2level_size20_5_sram[4], mux_2level_size20_5_sram[3], mux_2level_size20_5_sram[2], mux_2level_size20_5_sram[1], mux_2level_size20_5_sram[0]}),
		.sram_inv({mux_2level_size20_5_sram_inv[9], mux_2level_size20_5_sram_inv[8], mux_2level_size20_5_sram_inv[7], mux_2level_size20_5_sram_inv[6], mux_2level_size20_5_sram_inv[5], mux_2level_size20_5_sram_inv[4], mux_2level_size20_5_sram_inv[3], mux_2level_size20_5_sram_inv[2], mux_2level_size20_5_sram_inv[1], mux_2level_size20_5_sram_inv[0]}),
		.out(mux_2level_size20_5_out));

	mux_2level_size20 mux_fle_1_in_2 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_6_sram[9], mux_2level_size20_6_sram[8], mux_2level_size20_6_sram[7], mux_2level_size20_6_sram[6], mux_2level_size20_6_sram[5], mux_2level_size20_6_sram[4], mux_2level_size20_6_sram[3], mux_2level_size20_6_sram[2], mux_2level_size20_6_sram[1], mux_2level_size20_6_sram[0]}),
		.sram_inv({mux_2level_size20_6_sram_inv[9], mux_2level_size20_6_sram_inv[8], mux_2level_size20_6_sram_inv[7], mux_2level_size20_6_sram_inv[6], mux_2level_size20_6_sram_inv[5], mux_2level_size20_6_sram_inv[4], mux_2level_size20_6_sram_inv[3], mux_2level_size20_6_sram_inv[2], mux_2level_size20_6_sram_inv[1], mux_2level_size20_6_sram_inv[0]}),
		.out(mux_2level_size20_6_out));

	mux_2level_size20 mux_fle_1_in_3 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_7_sram[9], mux_2level_size20_7_sram[8], mux_2level_size20_7_sram[7], mux_2level_size20_7_sram[6], mux_2level_size20_7_sram[5], mux_2level_size20_7_sram[4], mux_2level_size20_7_sram[3], mux_2level_size20_7_sram[2], mux_2level_size20_7_sram[1], mux_2level_size20_7_sram[0]}),
		.sram_inv({mux_2level_size20_7_sram_inv[9], mux_2level_size20_7_sram_inv[8], mux_2level_size20_7_sram_inv[7], mux_2level_size20_7_sram_inv[6], mux_2level_size20_7_sram_inv[5], mux_2level_size20_7_sram_inv[4], mux_2level_size20_7_sram_inv[3], mux_2level_size20_7_sram_inv[2], mux_2level_size20_7_sram_inv[1], mux_2level_size20_7_sram_inv[0]}),
		.out(mux_2level_size20_7_out));

	mux_2level_size20 mux_fle_2_in_0 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_8_sram[9], mux_2level_size20_8_sram[8], mux_2level_size20_8_sram[7], mux_2level_size20_8_sram[6], mux_2level_size20_8_sram[5], mux_2level_size20_8_sram[4], mux_2level_size20_8_sram[3], mux_2level_size20_8_sram[2], mux_2level_size20_8_sram[1], mux_2level_size20_8_sram[0]}),
		.sram_inv({mux_2level_size20_8_sram_inv[9], mux_2level_size20_8_sram_inv[8], mux_2level_size20_8_sram_inv[7], mux_2level_size20_8_sram_inv[6], mux_2level_size20_8_sram_inv[5], mux_2level_size20_8_sram_inv[4], mux_2level_size20_8_sram_inv[3], mux_2level_size20_8_sram_inv[2], mux_2level_size20_8_sram_inv[1], mux_2level_size20_8_sram_inv[0]}),
		.out(mux_2level_size20_8_out));

	mux_2level_size20 mux_fle_2_in_1 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_9_sram[9], mux_2level_size20_9_sram[8], mux_2level_size20_9_sram[7], mux_2level_size20_9_sram[6], mux_2level_size20_9_sram[5], mux_2level_size20_9_sram[4], mux_2level_size20_9_sram[3], mux_2level_size20_9_sram[2], mux_2level_size20_9_sram[1], mux_2level_size20_9_sram[0]}),
		.sram_inv({mux_2level_size20_9_sram_inv[9], mux_2level_size20_9_sram_inv[8], mux_2level_size20_9_sram_inv[7], mux_2level_size20_9_sram_inv[6], mux_2level_size20_9_sram_inv[5], mux_2level_size20_9_sram_inv[4], mux_2level_size20_9_sram_inv[3], mux_2level_size20_9_sram_inv[2], mux_2level_size20_9_sram_inv[1], mux_2level_size20_9_sram_inv[0]}),
		.out(mux_2level_size20_9_out));

	mux_2level_size20 mux_fle_2_in_2 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_10_sram[9], mux_2level_size20_10_sram[8], mux_2level_size20_10_sram[7], mux_2level_size20_10_sram[6], mux_2level_size20_10_sram[5], mux_2level_size20_10_sram[4], mux_2level_size20_10_sram[3], mux_2level_size20_10_sram[2], mux_2level_size20_10_sram[1], mux_2level_size20_10_sram[0]}),
		.sram_inv({mux_2level_size20_10_sram_inv[9], mux_2level_size20_10_sram_inv[8], mux_2level_size20_10_sram_inv[7], mux_2level_size20_10_sram_inv[6], mux_2level_size20_10_sram_inv[5], mux_2level_size20_10_sram_inv[4], mux_2level_size20_10_sram_inv[3], mux_2level_size20_10_sram_inv[2], mux_2level_size20_10_sram_inv[1], mux_2level_size20_10_sram_inv[0]}),
		.out(mux_2level_size20_10_out));

	mux_2level_size20 mux_fle_2_in_3 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_11_sram[9], mux_2level_size20_11_sram[8], mux_2level_size20_11_sram[7], mux_2level_size20_11_sram[6], mux_2level_size20_11_sram[5], mux_2level_size20_11_sram[4], mux_2level_size20_11_sram[3], mux_2level_size20_11_sram[2], mux_2level_size20_11_sram[1], mux_2level_size20_11_sram[0]}),
		.sram_inv({mux_2level_size20_11_sram_inv[9], mux_2level_size20_11_sram_inv[8], mux_2level_size20_11_sram_inv[7], mux_2level_size20_11_sram_inv[6], mux_2level_size20_11_sram_inv[5], mux_2level_size20_11_sram_inv[4], mux_2level_size20_11_sram_inv[3], mux_2level_size20_11_sram_inv[2], mux_2level_size20_11_sram_inv[1], mux_2level_size20_11_sram_inv[0]}),
		.out(mux_2level_size20_11_out));

	mux_2level_size20 mux_fle_3_in_0 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_12_sram[9], mux_2level_size20_12_sram[8], mux_2level_size20_12_sram[7], mux_2level_size20_12_sram[6], mux_2level_size20_12_sram[5], mux_2level_size20_12_sram[4], mux_2level_size20_12_sram[3], mux_2level_size20_12_sram[2], mux_2level_size20_12_sram[1], mux_2level_size20_12_sram[0]}),
		.sram_inv({mux_2level_size20_12_sram_inv[9], mux_2level_size20_12_sram_inv[8], mux_2level_size20_12_sram_inv[7], mux_2level_size20_12_sram_inv[6], mux_2level_size20_12_sram_inv[5], mux_2level_size20_12_sram_inv[4], mux_2level_size20_12_sram_inv[3], mux_2level_size20_12_sram_inv[2], mux_2level_size20_12_sram_inv[1], mux_2level_size20_12_sram_inv[0]}),
		.out(mux_2level_size20_12_out));

	mux_2level_size20 mux_fle_3_in_1 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_13_sram[9], mux_2level_size20_13_sram[8], mux_2level_size20_13_sram[7], mux_2level_size20_13_sram[6], mux_2level_size20_13_sram[5], mux_2level_size20_13_sram[4], mux_2level_size20_13_sram[3], mux_2level_size20_13_sram[2], mux_2level_size20_13_sram[1], mux_2level_size20_13_sram[0]}),
		.sram_inv({mux_2level_size20_13_sram_inv[9], mux_2level_size20_13_sram_inv[8], mux_2level_size20_13_sram_inv[7], mux_2level_size20_13_sram_inv[6], mux_2level_size20_13_sram_inv[5], mux_2level_size20_13_sram_inv[4], mux_2level_size20_13_sram_inv[3], mux_2level_size20_13_sram_inv[2], mux_2level_size20_13_sram_inv[1], mux_2level_size20_13_sram_inv[0]}),
		.out(mux_2level_size20_13_out));

	mux_2level_size20 mux_fle_3_in_2 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_14_sram[9], mux_2level_size20_14_sram[8], mux_2level_size20_14_sram[7], mux_2level_size20_14_sram[6], mux_2level_size20_14_sram[5], mux_2level_size20_14_sram[4], mux_2level_size20_14_sram[3], mux_2level_size20_14_sram[2], mux_2level_size20_14_sram[1], mux_2level_size20_14_sram[0]}),
		.sram_inv({mux_2level_size20_14_sram_inv[9], mux_2level_size20_14_sram_inv[8], mux_2level_size20_14_sram_inv[7], mux_2level_size20_14_sram_inv[6], mux_2level_size20_14_sram_inv[5], mux_2level_size20_14_sram_inv[4], mux_2level_size20_14_sram_inv[3], mux_2level_size20_14_sram_inv[2], mux_2level_size20_14_sram_inv[1], mux_2level_size20_14_sram_inv[0]}),
		.out(mux_2level_size20_14_out));

	mux_2level_size20 mux_fle_3_in_3 (
		.in({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0], logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0], logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0], logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0], clb_I[11], clb_I[10], clb_I[9], clb_I[8], clb_I[7], clb_I[6], clb_I[5], clb_I[4], clb_I[3], clb_I[2], clb_I[1], clb_I[0]}),
		.sram({mux_2level_size20_15_sram[9], mux_2level_size20_15_sram[8], mux_2level_size20_15_sram[7], mux_2level_size20_15_sram[6], mux_2level_size20_15_sram[5], mux_2level_size20_15_sram[4], mux_2level_size20_15_sram[3], mux_2level_size20_15_sram[2], mux_2level_size20_15_sram[1], mux_2level_size20_15_sram[0]}),
		.sram_inv({mux_2level_size20_15_sram_inv[9], mux_2level_size20_15_sram_inv[8], mux_2level_size20_15_sram_inv[7], mux_2level_size20_15_sram_inv[6], mux_2level_size20_15_sram_inv[5], mux_2level_size20_15_sram_inv[4], mux_2level_size20_15_sram_inv[3], mux_2level_size20_15_sram_inv[2], mux_2level_size20_15_sram_inv[1], mux_2level_size20_15_sram_inv[0]}),
		.out(mux_2level_size20_15_out));

	mux_2level_size20_mem mem_fle_0_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(logical_tile_clb_mode_default__fle_3_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_0_ccff_tail),
		.mem_out({mux_2level_size20_0_sram[9], mux_2level_size20_0_sram[8], mux_2level_size20_0_sram[7], mux_2level_size20_0_sram[6], mux_2level_size20_0_sram[5], mux_2level_size20_0_sram[4], mux_2level_size20_0_sram[3], mux_2level_size20_0_sram[2], mux_2level_size20_0_sram[1], mux_2level_size20_0_sram[0]}),
		.mem_outb({mux_2level_size20_0_sram_inv[9], mux_2level_size20_0_sram_inv[8], mux_2level_size20_0_sram_inv[7], mux_2level_size20_0_sram_inv[6], mux_2level_size20_0_sram_inv[5], mux_2level_size20_0_sram_inv[4], mux_2level_size20_0_sram_inv[3], mux_2level_size20_0_sram_inv[2], mux_2level_size20_0_sram_inv[1], mux_2level_size20_0_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_0_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_0_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_1_ccff_tail),
		.mem_out({mux_2level_size20_1_sram[9], mux_2level_size20_1_sram[8], mux_2level_size20_1_sram[7], mux_2level_size20_1_sram[6], mux_2level_size20_1_sram[5], mux_2level_size20_1_sram[4], mux_2level_size20_1_sram[3], mux_2level_size20_1_sram[2], mux_2level_size20_1_sram[1], mux_2level_size20_1_sram[0]}),
		.mem_outb({mux_2level_size20_1_sram_inv[9], mux_2level_size20_1_sram_inv[8], mux_2level_size20_1_sram_inv[7], mux_2level_size20_1_sram_inv[6], mux_2level_size20_1_sram_inv[5], mux_2level_size20_1_sram_inv[4], mux_2level_size20_1_sram_inv[3], mux_2level_size20_1_sram_inv[2], mux_2level_size20_1_sram_inv[1], mux_2level_size20_1_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_0_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_1_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_2_ccff_tail),
		.mem_out({mux_2level_size20_2_sram[9], mux_2level_size20_2_sram[8], mux_2level_size20_2_sram[7], mux_2level_size20_2_sram[6], mux_2level_size20_2_sram[5], mux_2level_size20_2_sram[4], mux_2level_size20_2_sram[3], mux_2level_size20_2_sram[2], mux_2level_size20_2_sram[1], mux_2level_size20_2_sram[0]}),
		.mem_outb({mux_2level_size20_2_sram_inv[9], mux_2level_size20_2_sram_inv[8], mux_2level_size20_2_sram_inv[7], mux_2level_size20_2_sram_inv[6], mux_2level_size20_2_sram_inv[5], mux_2level_size20_2_sram_inv[4], mux_2level_size20_2_sram_inv[3], mux_2level_size20_2_sram_inv[2], mux_2level_size20_2_sram_inv[1], mux_2level_size20_2_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_0_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_2_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_3_ccff_tail),
		.mem_out({mux_2level_size20_3_sram[9], mux_2level_size20_3_sram[8], mux_2level_size20_3_sram[7], mux_2level_size20_3_sram[6], mux_2level_size20_3_sram[5], mux_2level_size20_3_sram[4], mux_2level_size20_3_sram[3], mux_2level_size20_3_sram[2], mux_2level_size20_3_sram[1], mux_2level_size20_3_sram[0]}),
		.mem_outb({mux_2level_size20_3_sram_inv[9], mux_2level_size20_3_sram_inv[8], mux_2level_size20_3_sram_inv[7], mux_2level_size20_3_sram_inv[6], mux_2level_size20_3_sram_inv[5], mux_2level_size20_3_sram_inv[4], mux_2level_size20_3_sram_inv[3], mux_2level_size20_3_sram_inv[2], mux_2level_size20_3_sram_inv[1], mux_2level_size20_3_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_1_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_3_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_4_ccff_tail),
		.mem_out({mux_2level_size20_4_sram[9], mux_2level_size20_4_sram[8], mux_2level_size20_4_sram[7], mux_2level_size20_4_sram[6], mux_2level_size20_4_sram[5], mux_2level_size20_4_sram[4], mux_2level_size20_4_sram[3], mux_2level_size20_4_sram[2], mux_2level_size20_4_sram[1], mux_2level_size20_4_sram[0]}),
		.mem_outb({mux_2level_size20_4_sram_inv[9], mux_2level_size20_4_sram_inv[8], mux_2level_size20_4_sram_inv[7], mux_2level_size20_4_sram_inv[6], mux_2level_size20_4_sram_inv[5], mux_2level_size20_4_sram_inv[4], mux_2level_size20_4_sram_inv[3], mux_2level_size20_4_sram_inv[2], mux_2level_size20_4_sram_inv[1], mux_2level_size20_4_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_1_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_4_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_5_ccff_tail),
		.mem_out({mux_2level_size20_5_sram[9], mux_2level_size20_5_sram[8], mux_2level_size20_5_sram[7], mux_2level_size20_5_sram[6], mux_2level_size20_5_sram[5], mux_2level_size20_5_sram[4], mux_2level_size20_5_sram[3], mux_2level_size20_5_sram[2], mux_2level_size20_5_sram[1], mux_2level_size20_5_sram[0]}),
		.mem_outb({mux_2level_size20_5_sram_inv[9], mux_2level_size20_5_sram_inv[8], mux_2level_size20_5_sram_inv[7], mux_2level_size20_5_sram_inv[6], mux_2level_size20_5_sram_inv[5], mux_2level_size20_5_sram_inv[4], mux_2level_size20_5_sram_inv[3], mux_2level_size20_5_sram_inv[2], mux_2level_size20_5_sram_inv[1], mux_2level_size20_5_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_1_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_5_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_6_ccff_tail),
		.mem_out({mux_2level_size20_6_sram[9], mux_2level_size20_6_sram[8], mux_2level_size20_6_sram[7], mux_2level_size20_6_sram[6], mux_2level_size20_6_sram[5], mux_2level_size20_6_sram[4], mux_2level_size20_6_sram[3], mux_2level_size20_6_sram[2], mux_2level_size20_6_sram[1], mux_2level_size20_6_sram[0]}),
		.mem_outb({mux_2level_size20_6_sram_inv[9], mux_2level_size20_6_sram_inv[8], mux_2level_size20_6_sram_inv[7], mux_2level_size20_6_sram_inv[6], mux_2level_size20_6_sram_inv[5], mux_2level_size20_6_sram_inv[4], mux_2level_size20_6_sram_inv[3], mux_2level_size20_6_sram_inv[2], mux_2level_size20_6_sram_inv[1], mux_2level_size20_6_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_1_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_6_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_7_ccff_tail),
		.mem_out({mux_2level_size20_7_sram[9], mux_2level_size20_7_sram[8], mux_2level_size20_7_sram[7], mux_2level_size20_7_sram[6], mux_2level_size20_7_sram[5], mux_2level_size20_7_sram[4], mux_2level_size20_7_sram[3], mux_2level_size20_7_sram[2], mux_2level_size20_7_sram[1], mux_2level_size20_7_sram[0]}),
		.mem_outb({mux_2level_size20_7_sram_inv[9], mux_2level_size20_7_sram_inv[8], mux_2level_size20_7_sram_inv[7], mux_2level_size20_7_sram_inv[6], mux_2level_size20_7_sram_inv[5], mux_2level_size20_7_sram_inv[4], mux_2level_size20_7_sram_inv[3], mux_2level_size20_7_sram_inv[2], mux_2level_size20_7_sram_inv[1], mux_2level_size20_7_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_2_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_7_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_8_ccff_tail),
		.mem_out({mux_2level_size20_8_sram[9], mux_2level_size20_8_sram[8], mux_2level_size20_8_sram[7], mux_2level_size20_8_sram[6], mux_2level_size20_8_sram[5], mux_2level_size20_8_sram[4], mux_2level_size20_8_sram[3], mux_2level_size20_8_sram[2], mux_2level_size20_8_sram[1], mux_2level_size20_8_sram[0]}),
		.mem_outb({mux_2level_size20_8_sram_inv[9], mux_2level_size20_8_sram_inv[8], mux_2level_size20_8_sram_inv[7], mux_2level_size20_8_sram_inv[6], mux_2level_size20_8_sram_inv[5], mux_2level_size20_8_sram_inv[4], mux_2level_size20_8_sram_inv[3], mux_2level_size20_8_sram_inv[2], mux_2level_size20_8_sram_inv[1], mux_2level_size20_8_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_2_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_8_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_9_ccff_tail),
		.mem_out({mux_2level_size20_9_sram[9], mux_2level_size20_9_sram[8], mux_2level_size20_9_sram[7], mux_2level_size20_9_sram[6], mux_2level_size20_9_sram[5], mux_2level_size20_9_sram[4], mux_2level_size20_9_sram[3], mux_2level_size20_9_sram[2], mux_2level_size20_9_sram[1], mux_2level_size20_9_sram[0]}),
		.mem_outb({mux_2level_size20_9_sram_inv[9], mux_2level_size20_9_sram_inv[8], mux_2level_size20_9_sram_inv[7], mux_2level_size20_9_sram_inv[6], mux_2level_size20_9_sram_inv[5], mux_2level_size20_9_sram_inv[4], mux_2level_size20_9_sram_inv[3], mux_2level_size20_9_sram_inv[2], mux_2level_size20_9_sram_inv[1], mux_2level_size20_9_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_2_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_9_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_10_ccff_tail),
		.mem_out({mux_2level_size20_10_sram[9], mux_2level_size20_10_sram[8], mux_2level_size20_10_sram[7], mux_2level_size20_10_sram[6], mux_2level_size20_10_sram[5], mux_2level_size20_10_sram[4], mux_2level_size20_10_sram[3], mux_2level_size20_10_sram[2], mux_2level_size20_10_sram[1], mux_2level_size20_10_sram[0]}),
		.mem_outb({mux_2level_size20_10_sram_inv[9], mux_2level_size20_10_sram_inv[8], mux_2level_size20_10_sram_inv[7], mux_2level_size20_10_sram_inv[6], mux_2level_size20_10_sram_inv[5], mux_2level_size20_10_sram_inv[4], mux_2level_size20_10_sram_inv[3], mux_2level_size20_10_sram_inv[2], mux_2level_size20_10_sram_inv[1], mux_2level_size20_10_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_2_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_10_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_11_ccff_tail),
		.mem_out({mux_2level_size20_11_sram[9], mux_2level_size20_11_sram[8], mux_2level_size20_11_sram[7], mux_2level_size20_11_sram[6], mux_2level_size20_11_sram[5], mux_2level_size20_11_sram[4], mux_2level_size20_11_sram[3], mux_2level_size20_11_sram[2], mux_2level_size20_11_sram[1], mux_2level_size20_11_sram[0]}),
		.mem_outb({mux_2level_size20_11_sram_inv[9], mux_2level_size20_11_sram_inv[8], mux_2level_size20_11_sram_inv[7], mux_2level_size20_11_sram_inv[6], mux_2level_size20_11_sram_inv[5], mux_2level_size20_11_sram_inv[4], mux_2level_size20_11_sram_inv[3], mux_2level_size20_11_sram_inv[2], mux_2level_size20_11_sram_inv[1], mux_2level_size20_11_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_3_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_11_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_12_ccff_tail),
		.mem_out({mux_2level_size20_12_sram[9], mux_2level_size20_12_sram[8], mux_2level_size20_12_sram[7], mux_2level_size20_12_sram[6], mux_2level_size20_12_sram[5], mux_2level_size20_12_sram[4], mux_2level_size20_12_sram[3], mux_2level_size20_12_sram[2], mux_2level_size20_12_sram[1], mux_2level_size20_12_sram[0]}),
		.mem_outb({mux_2level_size20_12_sram_inv[9], mux_2level_size20_12_sram_inv[8], mux_2level_size20_12_sram_inv[7], mux_2level_size20_12_sram_inv[6], mux_2level_size20_12_sram_inv[5], mux_2level_size20_12_sram_inv[4], mux_2level_size20_12_sram_inv[3], mux_2level_size20_12_sram_inv[2], mux_2level_size20_12_sram_inv[1], mux_2level_size20_12_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_3_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_12_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_13_ccff_tail),
		.mem_out({mux_2level_size20_13_sram[9], mux_2level_size20_13_sram[8], mux_2level_size20_13_sram[7], mux_2level_size20_13_sram[6], mux_2level_size20_13_sram[5], mux_2level_size20_13_sram[4], mux_2level_size20_13_sram[3], mux_2level_size20_13_sram[2], mux_2level_size20_13_sram[1], mux_2level_size20_13_sram[0]}),
		.mem_outb({mux_2level_size20_13_sram_inv[9], mux_2level_size20_13_sram_inv[8], mux_2level_size20_13_sram_inv[7], mux_2level_size20_13_sram_inv[6], mux_2level_size20_13_sram_inv[5], mux_2level_size20_13_sram_inv[4], mux_2level_size20_13_sram_inv[3], mux_2level_size20_13_sram_inv[2], mux_2level_size20_13_sram_inv[1], mux_2level_size20_13_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_3_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_13_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_14_ccff_tail),
		.mem_out({mux_2level_size20_14_sram[9], mux_2level_size20_14_sram[8], mux_2level_size20_14_sram[7], mux_2level_size20_14_sram[6], mux_2level_size20_14_sram[5], mux_2level_size20_14_sram[4], mux_2level_size20_14_sram[3], mux_2level_size20_14_sram[2], mux_2level_size20_14_sram[1], mux_2level_size20_14_sram[0]}),
		.mem_outb({mux_2level_size20_14_sram_inv[9], mux_2level_size20_14_sram_inv[8], mux_2level_size20_14_sram_inv[7], mux_2level_size20_14_sram_inv[6], mux_2level_size20_14_sram_inv[5], mux_2level_size20_14_sram_inv[4], mux_2level_size20_14_sram_inv[3], mux_2level_size20_14_sram_inv[2], mux_2level_size20_14_sram_inv[1], mux_2level_size20_14_sram_inv[0]}));

	mux_2level_size20_mem mem_fle_3_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_14_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mux_2level_size20_15_sram[9], mux_2level_size20_15_sram[8], mux_2level_size20_15_sram[7], mux_2level_size20_15_sram[6], mux_2level_size20_15_sram[5], mux_2level_size20_15_sram[4], mux_2level_size20_15_sram[3], mux_2level_size20_15_sram[2], mux_2level_size20_15_sram[1], mux_2level_size20_15_sram[0]}),
		.mem_outb({mux_2level_size20_15_sram_inv[9], mux_2level_size20_15_sram_inv[8], mux_2level_size20_15_sram_inv[7], mux_2level_size20_15_sram_inv[6], mux_2level_size20_15_sram_inv[5], mux_2level_size20_15_sram_inv[4], mux_2level_size20_15_sram_inv[3], mux_2level_size20_15_sram_inv[2], mux_2level_size20_15_sram_inv[1], mux_2level_size20_15_sram_inv[0]}));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_clb_ -----



// ----- END Physical programmable logic block Verilog module: clb -----
