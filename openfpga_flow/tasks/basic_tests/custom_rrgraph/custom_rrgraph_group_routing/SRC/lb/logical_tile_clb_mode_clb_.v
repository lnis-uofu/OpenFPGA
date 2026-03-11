//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: clb
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: clb -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_clb_ -----
module logical_tile_clb_mode_clb_(set,
                                  reset,
                                  clk,
                                  clb_I,
                                  clb_clk,
                                  feedthrough_mem_in,
                                  feedthrough_mem_inb,
                                  clb_O);
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
input [263:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [263:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [7:0] clb_O;

//----- BEGIN wire-connection ports -----
wire [11:0] clb_I;
wire [0:0] clb_clk;
wire [7:0] clb_O;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_10_out;
wire [0:0] direct_interc_11_out;
wire [0:0] direct_interc_8_out;
wire [0:0] direct_interc_9_out;
wire [1:0] logical_tile_clb_mode_default__fle_0_fle_out;
wire [1:0] logical_tile_clb_mode_default__fle_1_fle_out;
wire [1:0] logical_tile_clb_mode_default__fle_2_fle_out;
wire [1:0] logical_tile_clb_mode_default__fle_3_fle_out;
wire [0:0] mux_2level_size20_0_out;
wire [9:0] mux_2level_size20_0_sram;
wire [9:0] mux_2level_size20_0_sram_inv;
wire [0:0] mux_2level_size20_10_out;
wire [9:0] mux_2level_size20_10_sram;
wire [9:0] mux_2level_size20_10_sram_inv;
wire [0:0] mux_2level_size20_11_out;
wire [9:0] mux_2level_size20_11_sram;
wire [9:0] mux_2level_size20_11_sram_inv;
wire [0:0] mux_2level_size20_12_out;
wire [9:0] mux_2level_size20_12_sram;
wire [9:0] mux_2level_size20_12_sram_inv;
wire [0:0] mux_2level_size20_13_out;
wire [9:0] mux_2level_size20_13_sram;
wire [9:0] mux_2level_size20_13_sram_inv;
wire [0:0] mux_2level_size20_14_out;
wire [9:0] mux_2level_size20_14_sram;
wire [9:0] mux_2level_size20_14_sram_inv;
wire [0:0] mux_2level_size20_15_out;
wire [9:0] mux_2level_size20_15_sram;
wire [9:0] mux_2level_size20_15_sram_inv;
wire [0:0] mux_2level_size20_1_out;
wire [9:0] mux_2level_size20_1_sram;
wire [9:0] mux_2level_size20_1_sram_inv;
wire [0:0] mux_2level_size20_2_out;
wire [9:0] mux_2level_size20_2_sram;
wire [9:0] mux_2level_size20_2_sram_inv;
wire [0:0] mux_2level_size20_3_out;
wire [9:0] mux_2level_size20_3_sram;
wire [9:0] mux_2level_size20_3_sram_inv;
wire [0:0] mux_2level_size20_4_out;
wire [9:0] mux_2level_size20_4_sram;
wire [9:0] mux_2level_size20_4_sram_inv;
wire [0:0] mux_2level_size20_5_out;
wire [9:0] mux_2level_size20_5_sram;
wire [9:0] mux_2level_size20_5_sram_inv;
wire [0:0] mux_2level_size20_6_out;
wire [9:0] mux_2level_size20_6_sram;
wire [9:0] mux_2level_size20_6_sram_inv;
wire [0:0] mux_2level_size20_7_out;
wire [9:0] mux_2level_size20_7_sram;
wire [9:0] mux_2level_size20_7_sram_inv;
wire [0:0] mux_2level_size20_8_out;
wire [9:0] mux_2level_size20_8_sram;
wire [9:0] mux_2level_size20_8_sram_inv;
wire [0:0] mux_2level_size20_9_out;
wire [9:0] mux_2level_size20_9_sram;
wire [9:0] mux_2level_size20_9_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_0 (
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_3_out, mux_2level_size20_2_out, mux_2level_size20_1_out, mux_2level_size20_0_out}),
		.fle_clk(direct_interc_8_out),
		.feedthrough_mem_in({feedthrough_mem_in[25], feedthrough_mem_in[24], feedthrough_mem_in[23], feedthrough_mem_in[22], feedthrough_mem_in[21], feedthrough_mem_in[20], feedthrough_mem_in[19], feedthrough_mem_in[18], feedthrough_mem_in[17], feedthrough_mem_in[16], feedthrough_mem_in[15], feedthrough_mem_in[14], feedthrough_mem_in[13], feedthrough_mem_in[12], feedthrough_mem_in[11], feedthrough_mem_in[10], feedthrough_mem_in[9], feedthrough_mem_in[8], feedthrough_mem_in[7], feedthrough_mem_in[6], feedthrough_mem_in[5], feedthrough_mem_in[4], feedthrough_mem_in[3], feedthrough_mem_in[2], feedthrough_mem_in[1], feedthrough_mem_in[0]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[25], feedthrough_mem_inb[24], feedthrough_mem_inb[23], feedthrough_mem_inb[22], feedthrough_mem_inb[21], feedthrough_mem_inb[20], feedthrough_mem_inb[19], feedthrough_mem_inb[18], feedthrough_mem_inb[17], feedthrough_mem_inb[16], feedthrough_mem_inb[15], feedthrough_mem_inb[14], feedthrough_mem_inb[13], feedthrough_mem_inb[12], feedthrough_mem_inb[11], feedthrough_mem_inb[10], feedthrough_mem_inb[9], feedthrough_mem_inb[8], feedthrough_mem_inb[7], feedthrough_mem_inb[6], feedthrough_mem_inb[5], feedthrough_mem_inb[4], feedthrough_mem_inb[3], feedthrough_mem_inb[2], feedthrough_mem_inb[1], feedthrough_mem_inb[0]}),
		.fle_out({logical_tile_clb_mode_default__fle_0_fle_out[1], logical_tile_clb_mode_default__fle_0_fle_out[0]}));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_1 (
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_7_out, mux_2level_size20_6_out, mux_2level_size20_5_out, mux_2level_size20_4_out}),
		.fle_clk(direct_interc_9_out),
		.feedthrough_mem_in({feedthrough_mem_in[51], feedthrough_mem_in[50], feedthrough_mem_in[49], feedthrough_mem_in[48], feedthrough_mem_in[47], feedthrough_mem_in[46], feedthrough_mem_in[45], feedthrough_mem_in[44], feedthrough_mem_in[43], feedthrough_mem_in[42], feedthrough_mem_in[41], feedthrough_mem_in[40], feedthrough_mem_in[39], feedthrough_mem_in[38], feedthrough_mem_in[37], feedthrough_mem_in[36], feedthrough_mem_in[35], feedthrough_mem_in[34], feedthrough_mem_in[33], feedthrough_mem_in[32], feedthrough_mem_in[31], feedthrough_mem_in[30], feedthrough_mem_in[29], feedthrough_mem_in[28], feedthrough_mem_in[27], feedthrough_mem_in[26]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[51], feedthrough_mem_inb[50], feedthrough_mem_inb[49], feedthrough_mem_inb[48], feedthrough_mem_inb[47], feedthrough_mem_inb[46], feedthrough_mem_inb[45], feedthrough_mem_inb[44], feedthrough_mem_inb[43], feedthrough_mem_inb[42], feedthrough_mem_inb[41], feedthrough_mem_inb[40], feedthrough_mem_inb[39], feedthrough_mem_inb[38], feedthrough_mem_inb[37], feedthrough_mem_inb[36], feedthrough_mem_inb[35], feedthrough_mem_inb[34], feedthrough_mem_inb[33], feedthrough_mem_inb[32], feedthrough_mem_inb[31], feedthrough_mem_inb[30], feedthrough_mem_inb[29], feedthrough_mem_inb[28], feedthrough_mem_inb[27], feedthrough_mem_inb[26]}),
		.fle_out({logical_tile_clb_mode_default__fle_1_fle_out[1], logical_tile_clb_mode_default__fle_1_fle_out[0]}));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_2 (
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_11_out, mux_2level_size20_10_out, mux_2level_size20_9_out, mux_2level_size20_8_out}),
		.fle_clk(direct_interc_10_out),
		.feedthrough_mem_in({feedthrough_mem_in[77], feedthrough_mem_in[76], feedthrough_mem_in[75], feedthrough_mem_in[74], feedthrough_mem_in[73], feedthrough_mem_in[72], feedthrough_mem_in[71], feedthrough_mem_in[70], feedthrough_mem_in[69], feedthrough_mem_in[68], feedthrough_mem_in[67], feedthrough_mem_in[66], feedthrough_mem_in[65], feedthrough_mem_in[64], feedthrough_mem_in[63], feedthrough_mem_in[62], feedthrough_mem_in[61], feedthrough_mem_in[60], feedthrough_mem_in[59], feedthrough_mem_in[58], feedthrough_mem_in[57], feedthrough_mem_in[56], feedthrough_mem_in[55], feedthrough_mem_in[54], feedthrough_mem_in[53], feedthrough_mem_in[52]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[77], feedthrough_mem_inb[76], feedthrough_mem_inb[75], feedthrough_mem_inb[74], feedthrough_mem_inb[73], feedthrough_mem_inb[72], feedthrough_mem_inb[71], feedthrough_mem_inb[70], feedthrough_mem_inb[69], feedthrough_mem_inb[68], feedthrough_mem_inb[67], feedthrough_mem_inb[66], feedthrough_mem_inb[65], feedthrough_mem_inb[64], feedthrough_mem_inb[63], feedthrough_mem_inb[62], feedthrough_mem_inb[61], feedthrough_mem_inb[60], feedthrough_mem_inb[59], feedthrough_mem_inb[58], feedthrough_mem_inb[57], feedthrough_mem_inb[56], feedthrough_mem_inb[55], feedthrough_mem_inb[54], feedthrough_mem_inb[53], feedthrough_mem_inb[52]}),
		.fle_out({logical_tile_clb_mode_default__fle_2_fle_out[1], logical_tile_clb_mode_default__fle_2_fle_out[0]}));

	logical_tile_clb_mode_default__fle logical_tile_clb_mode_default__fle_3 (
		.set(set),
		.reset(reset),
		.clk(clk),
		.fle_in({mux_2level_size20_15_out, mux_2level_size20_14_out, mux_2level_size20_13_out, mux_2level_size20_12_out}),
		.fle_clk(direct_interc_11_out),
		.feedthrough_mem_in({feedthrough_mem_in[103], feedthrough_mem_in[102], feedthrough_mem_in[101], feedthrough_mem_in[100], feedthrough_mem_in[99], feedthrough_mem_in[98], feedthrough_mem_in[97], feedthrough_mem_in[96], feedthrough_mem_in[95], feedthrough_mem_in[94], feedthrough_mem_in[93], feedthrough_mem_in[92], feedthrough_mem_in[91], feedthrough_mem_in[90], feedthrough_mem_in[89], feedthrough_mem_in[88], feedthrough_mem_in[87], feedthrough_mem_in[86], feedthrough_mem_in[85], feedthrough_mem_in[84], feedthrough_mem_in[83], feedthrough_mem_in[82], feedthrough_mem_in[81], feedthrough_mem_in[80], feedthrough_mem_in[79], feedthrough_mem_in[78]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[103], feedthrough_mem_inb[102], feedthrough_mem_inb[101], feedthrough_mem_inb[100], feedthrough_mem_inb[99], feedthrough_mem_inb[98], feedthrough_mem_inb[97], feedthrough_mem_inb[96], feedthrough_mem_inb[95], feedthrough_mem_inb[94], feedthrough_mem_inb[93], feedthrough_mem_inb[92], feedthrough_mem_inb[91], feedthrough_mem_inb[90], feedthrough_mem_inb[89], feedthrough_mem_inb[88], feedthrough_mem_inb[87], feedthrough_mem_inb[86], feedthrough_mem_inb[85], feedthrough_mem_inb[84], feedthrough_mem_inb[83], feedthrough_mem_inb[82], feedthrough_mem_inb[81], feedthrough_mem_inb[80], feedthrough_mem_inb[79], feedthrough_mem_inb[78]}),
		.fle_out({logical_tile_clb_mode_default__fle_3_fle_out[1], logical_tile_clb_mode_default__fle_3_fle_out[0]}));

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

	mux_2level_size20_feedthrough_mem virtual_mem_fle_0_in_0 (
		.feedthrough_mem_in({feedthrough_mem_in[113], feedthrough_mem_in[112], feedthrough_mem_in[111], feedthrough_mem_in[110], feedthrough_mem_in[109], feedthrough_mem_in[108], feedthrough_mem_in[107], feedthrough_mem_in[106], feedthrough_mem_in[105], feedthrough_mem_in[104]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[113], feedthrough_mem_inb[112], feedthrough_mem_inb[111], feedthrough_mem_inb[110], feedthrough_mem_inb[109], feedthrough_mem_inb[108], feedthrough_mem_inb[107], feedthrough_mem_inb[106], feedthrough_mem_inb[105], feedthrough_mem_inb[104]}),
		.mem_out({mux_2level_size20_0_sram[9], mux_2level_size20_0_sram[8], mux_2level_size20_0_sram[7], mux_2level_size20_0_sram[6], mux_2level_size20_0_sram[5], mux_2level_size20_0_sram[4], mux_2level_size20_0_sram[3], mux_2level_size20_0_sram[2], mux_2level_size20_0_sram[1], mux_2level_size20_0_sram[0]}),
		.mem_outb({mux_2level_size20_0_sram_inv[9], mux_2level_size20_0_sram_inv[8], mux_2level_size20_0_sram_inv[7], mux_2level_size20_0_sram_inv[6], mux_2level_size20_0_sram_inv[5], mux_2level_size20_0_sram_inv[4], mux_2level_size20_0_sram_inv[3], mux_2level_size20_0_sram_inv[2], mux_2level_size20_0_sram_inv[1], mux_2level_size20_0_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_0_in_1 (
		.feedthrough_mem_in({feedthrough_mem_in[123], feedthrough_mem_in[122], feedthrough_mem_in[121], feedthrough_mem_in[120], feedthrough_mem_in[119], feedthrough_mem_in[118], feedthrough_mem_in[117], feedthrough_mem_in[116], feedthrough_mem_in[115], feedthrough_mem_in[114]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[123], feedthrough_mem_inb[122], feedthrough_mem_inb[121], feedthrough_mem_inb[120], feedthrough_mem_inb[119], feedthrough_mem_inb[118], feedthrough_mem_inb[117], feedthrough_mem_inb[116], feedthrough_mem_inb[115], feedthrough_mem_inb[114]}),
		.mem_out({mux_2level_size20_1_sram[9], mux_2level_size20_1_sram[8], mux_2level_size20_1_sram[7], mux_2level_size20_1_sram[6], mux_2level_size20_1_sram[5], mux_2level_size20_1_sram[4], mux_2level_size20_1_sram[3], mux_2level_size20_1_sram[2], mux_2level_size20_1_sram[1], mux_2level_size20_1_sram[0]}),
		.mem_outb({mux_2level_size20_1_sram_inv[9], mux_2level_size20_1_sram_inv[8], mux_2level_size20_1_sram_inv[7], mux_2level_size20_1_sram_inv[6], mux_2level_size20_1_sram_inv[5], mux_2level_size20_1_sram_inv[4], mux_2level_size20_1_sram_inv[3], mux_2level_size20_1_sram_inv[2], mux_2level_size20_1_sram_inv[1], mux_2level_size20_1_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_0_in_2 (
		.feedthrough_mem_in({feedthrough_mem_in[133], feedthrough_mem_in[132], feedthrough_mem_in[131], feedthrough_mem_in[130], feedthrough_mem_in[129], feedthrough_mem_in[128], feedthrough_mem_in[127], feedthrough_mem_in[126], feedthrough_mem_in[125], feedthrough_mem_in[124]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[133], feedthrough_mem_inb[132], feedthrough_mem_inb[131], feedthrough_mem_inb[130], feedthrough_mem_inb[129], feedthrough_mem_inb[128], feedthrough_mem_inb[127], feedthrough_mem_inb[126], feedthrough_mem_inb[125], feedthrough_mem_inb[124]}),
		.mem_out({mux_2level_size20_2_sram[9], mux_2level_size20_2_sram[8], mux_2level_size20_2_sram[7], mux_2level_size20_2_sram[6], mux_2level_size20_2_sram[5], mux_2level_size20_2_sram[4], mux_2level_size20_2_sram[3], mux_2level_size20_2_sram[2], mux_2level_size20_2_sram[1], mux_2level_size20_2_sram[0]}),
		.mem_outb({mux_2level_size20_2_sram_inv[9], mux_2level_size20_2_sram_inv[8], mux_2level_size20_2_sram_inv[7], mux_2level_size20_2_sram_inv[6], mux_2level_size20_2_sram_inv[5], mux_2level_size20_2_sram_inv[4], mux_2level_size20_2_sram_inv[3], mux_2level_size20_2_sram_inv[2], mux_2level_size20_2_sram_inv[1], mux_2level_size20_2_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_0_in_3 (
		.feedthrough_mem_in({feedthrough_mem_in[143], feedthrough_mem_in[142], feedthrough_mem_in[141], feedthrough_mem_in[140], feedthrough_mem_in[139], feedthrough_mem_in[138], feedthrough_mem_in[137], feedthrough_mem_in[136], feedthrough_mem_in[135], feedthrough_mem_in[134]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[143], feedthrough_mem_inb[142], feedthrough_mem_inb[141], feedthrough_mem_inb[140], feedthrough_mem_inb[139], feedthrough_mem_inb[138], feedthrough_mem_inb[137], feedthrough_mem_inb[136], feedthrough_mem_inb[135], feedthrough_mem_inb[134]}),
		.mem_out({mux_2level_size20_3_sram[9], mux_2level_size20_3_sram[8], mux_2level_size20_3_sram[7], mux_2level_size20_3_sram[6], mux_2level_size20_3_sram[5], mux_2level_size20_3_sram[4], mux_2level_size20_3_sram[3], mux_2level_size20_3_sram[2], mux_2level_size20_3_sram[1], mux_2level_size20_3_sram[0]}),
		.mem_outb({mux_2level_size20_3_sram_inv[9], mux_2level_size20_3_sram_inv[8], mux_2level_size20_3_sram_inv[7], mux_2level_size20_3_sram_inv[6], mux_2level_size20_3_sram_inv[5], mux_2level_size20_3_sram_inv[4], mux_2level_size20_3_sram_inv[3], mux_2level_size20_3_sram_inv[2], mux_2level_size20_3_sram_inv[1], mux_2level_size20_3_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_1_in_0 (
		.feedthrough_mem_in({feedthrough_mem_in[153], feedthrough_mem_in[152], feedthrough_mem_in[151], feedthrough_mem_in[150], feedthrough_mem_in[149], feedthrough_mem_in[148], feedthrough_mem_in[147], feedthrough_mem_in[146], feedthrough_mem_in[145], feedthrough_mem_in[144]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[153], feedthrough_mem_inb[152], feedthrough_mem_inb[151], feedthrough_mem_inb[150], feedthrough_mem_inb[149], feedthrough_mem_inb[148], feedthrough_mem_inb[147], feedthrough_mem_inb[146], feedthrough_mem_inb[145], feedthrough_mem_inb[144]}),
		.mem_out({mux_2level_size20_4_sram[9], mux_2level_size20_4_sram[8], mux_2level_size20_4_sram[7], mux_2level_size20_4_sram[6], mux_2level_size20_4_sram[5], mux_2level_size20_4_sram[4], mux_2level_size20_4_sram[3], mux_2level_size20_4_sram[2], mux_2level_size20_4_sram[1], mux_2level_size20_4_sram[0]}),
		.mem_outb({mux_2level_size20_4_sram_inv[9], mux_2level_size20_4_sram_inv[8], mux_2level_size20_4_sram_inv[7], mux_2level_size20_4_sram_inv[6], mux_2level_size20_4_sram_inv[5], mux_2level_size20_4_sram_inv[4], mux_2level_size20_4_sram_inv[3], mux_2level_size20_4_sram_inv[2], mux_2level_size20_4_sram_inv[1], mux_2level_size20_4_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_1_in_1 (
		.feedthrough_mem_in({feedthrough_mem_in[163], feedthrough_mem_in[162], feedthrough_mem_in[161], feedthrough_mem_in[160], feedthrough_mem_in[159], feedthrough_mem_in[158], feedthrough_mem_in[157], feedthrough_mem_in[156], feedthrough_mem_in[155], feedthrough_mem_in[154]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[163], feedthrough_mem_inb[162], feedthrough_mem_inb[161], feedthrough_mem_inb[160], feedthrough_mem_inb[159], feedthrough_mem_inb[158], feedthrough_mem_inb[157], feedthrough_mem_inb[156], feedthrough_mem_inb[155], feedthrough_mem_inb[154]}),
		.mem_out({mux_2level_size20_5_sram[9], mux_2level_size20_5_sram[8], mux_2level_size20_5_sram[7], mux_2level_size20_5_sram[6], mux_2level_size20_5_sram[5], mux_2level_size20_5_sram[4], mux_2level_size20_5_sram[3], mux_2level_size20_5_sram[2], mux_2level_size20_5_sram[1], mux_2level_size20_5_sram[0]}),
		.mem_outb({mux_2level_size20_5_sram_inv[9], mux_2level_size20_5_sram_inv[8], mux_2level_size20_5_sram_inv[7], mux_2level_size20_5_sram_inv[6], mux_2level_size20_5_sram_inv[5], mux_2level_size20_5_sram_inv[4], mux_2level_size20_5_sram_inv[3], mux_2level_size20_5_sram_inv[2], mux_2level_size20_5_sram_inv[1], mux_2level_size20_5_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_1_in_2 (
		.feedthrough_mem_in({feedthrough_mem_in[173], feedthrough_mem_in[172], feedthrough_mem_in[171], feedthrough_mem_in[170], feedthrough_mem_in[169], feedthrough_mem_in[168], feedthrough_mem_in[167], feedthrough_mem_in[166], feedthrough_mem_in[165], feedthrough_mem_in[164]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[173], feedthrough_mem_inb[172], feedthrough_mem_inb[171], feedthrough_mem_inb[170], feedthrough_mem_inb[169], feedthrough_mem_inb[168], feedthrough_mem_inb[167], feedthrough_mem_inb[166], feedthrough_mem_inb[165], feedthrough_mem_inb[164]}),
		.mem_out({mux_2level_size20_6_sram[9], mux_2level_size20_6_sram[8], mux_2level_size20_6_sram[7], mux_2level_size20_6_sram[6], mux_2level_size20_6_sram[5], mux_2level_size20_6_sram[4], mux_2level_size20_6_sram[3], mux_2level_size20_6_sram[2], mux_2level_size20_6_sram[1], mux_2level_size20_6_sram[0]}),
		.mem_outb({mux_2level_size20_6_sram_inv[9], mux_2level_size20_6_sram_inv[8], mux_2level_size20_6_sram_inv[7], mux_2level_size20_6_sram_inv[6], mux_2level_size20_6_sram_inv[5], mux_2level_size20_6_sram_inv[4], mux_2level_size20_6_sram_inv[3], mux_2level_size20_6_sram_inv[2], mux_2level_size20_6_sram_inv[1], mux_2level_size20_6_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_1_in_3 (
		.feedthrough_mem_in({feedthrough_mem_in[183], feedthrough_mem_in[182], feedthrough_mem_in[181], feedthrough_mem_in[180], feedthrough_mem_in[179], feedthrough_mem_in[178], feedthrough_mem_in[177], feedthrough_mem_in[176], feedthrough_mem_in[175], feedthrough_mem_in[174]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[183], feedthrough_mem_inb[182], feedthrough_mem_inb[181], feedthrough_mem_inb[180], feedthrough_mem_inb[179], feedthrough_mem_inb[178], feedthrough_mem_inb[177], feedthrough_mem_inb[176], feedthrough_mem_inb[175], feedthrough_mem_inb[174]}),
		.mem_out({mux_2level_size20_7_sram[9], mux_2level_size20_7_sram[8], mux_2level_size20_7_sram[7], mux_2level_size20_7_sram[6], mux_2level_size20_7_sram[5], mux_2level_size20_7_sram[4], mux_2level_size20_7_sram[3], mux_2level_size20_7_sram[2], mux_2level_size20_7_sram[1], mux_2level_size20_7_sram[0]}),
		.mem_outb({mux_2level_size20_7_sram_inv[9], mux_2level_size20_7_sram_inv[8], mux_2level_size20_7_sram_inv[7], mux_2level_size20_7_sram_inv[6], mux_2level_size20_7_sram_inv[5], mux_2level_size20_7_sram_inv[4], mux_2level_size20_7_sram_inv[3], mux_2level_size20_7_sram_inv[2], mux_2level_size20_7_sram_inv[1], mux_2level_size20_7_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_2_in_0 (
		.feedthrough_mem_in({feedthrough_mem_in[193], feedthrough_mem_in[192], feedthrough_mem_in[191], feedthrough_mem_in[190], feedthrough_mem_in[189], feedthrough_mem_in[188], feedthrough_mem_in[187], feedthrough_mem_in[186], feedthrough_mem_in[185], feedthrough_mem_in[184]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[193], feedthrough_mem_inb[192], feedthrough_mem_inb[191], feedthrough_mem_inb[190], feedthrough_mem_inb[189], feedthrough_mem_inb[188], feedthrough_mem_inb[187], feedthrough_mem_inb[186], feedthrough_mem_inb[185], feedthrough_mem_inb[184]}),
		.mem_out({mux_2level_size20_8_sram[9], mux_2level_size20_8_sram[8], mux_2level_size20_8_sram[7], mux_2level_size20_8_sram[6], mux_2level_size20_8_sram[5], mux_2level_size20_8_sram[4], mux_2level_size20_8_sram[3], mux_2level_size20_8_sram[2], mux_2level_size20_8_sram[1], mux_2level_size20_8_sram[0]}),
		.mem_outb({mux_2level_size20_8_sram_inv[9], mux_2level_size20_8_sram_inv[8], mux_2level_size20_8_sram_inv[7], mux_2level_size20_8_sram_inv[6], mux_2level_size20_8_sram_inv[5], mux_2level_size20_8_sram_inv[4], mux_2level_size20_8_sram_inv[3], mux_2level_size20_8_sram_inv[2], mux_2level_size20_8_sram_inv[1], mux_2level_size20_8_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_2_in_1 (
		.feedthrough_mem_in({feedthrough_mem_in[203], feedthrough_mem_in[202], feedthrough_mem_in[201], feedthrough_mem_in[200], feedthrough_mem_in[199], feedthrough_mem_in[198], feedthrough_mem_in[197], feedthrough_mem_in[196], feedthrough_mem_in[195], feedthrough_mem_in[194]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[203], feedthrough_mem_inb[202], feedthrough_mem_inb[201], feedthrough_mem_inb[200], feedthrough_mem_inb[199], feedthrough_mem_inb[198], feedthrough_mem_inb[197], feedthrough_mem_inb[196], feedthrough_mem_inb[195], feedthrough_mem_inb[194]}),
		.mem_out({mux_2level_size20_9_sram[9], mux_2level_size20_9_sram[8], mux_2level_size20_9_sram[7], mux_2level_size20_9_sram[6], mux_2level_size20_9_sram[5], mux_2level_size20_9_sram[4], mux_2level_size20_9_sram[3], mux_2level_size20_9_sram[2], mux_2level_size20_9_sram[1], mux_2level_size20_9_sram[0]}),
		.mem_outb({mux_2level_size20_9_sram_inv[9], mux_2level_size20_9_sram_inv[8], mux_2level_size20_9_sram_inv[7], mux_2level_size20_9_sram_inv[6], mux_2level_size20_9_sram_inv[5], mux_2level_size20_9_sram_inv[4], mux_2level_size20_9_sram_inv[3], mux_2level_size20_9_sram_inv[2], mux_2level_size20_9_sram_inv[1], mux_2level_size20_9_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_2_in_2 (
		.feedthrough_mem_in({feedthrough_mem_in[213], feedthrough_mem_in[212], feedthrough_mem_in[211], feedthrough_mem_in[210], feedthrough_mem_in[209], feedthrough_mem_in[208], feedthrough_mem_in[207], feedthrough_mem_in[206], feedthrough_mem_in[205], feedthrough_mem_in[204]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[213], feedthrough_mem_inb[212], feedthrough_mem_inb[211], feedthrough_mem_inb[210], feedthrough_mem_inb[209], feedthrough_mem_inb[208], feedthrough_mem_inb[207], feedthrough_mem_inb[206], feedthrough_mem_inb[205], feedthrough_mem_inb[204]}),
		.mem_out({mux_2level_size20_10_sram[9], mux_2level_size20_10_sram[8], mux_2level_size20_10_sram[7], mux_2level_size20_10_sram[6], mux_2level_size20_10_sram[5], mux_2level_size20_10_sram[4], mux_2level_size20_10_sram[3], mux_2level_size20_10_sram[2], mux_2level_size20_10_sram[1], mux_2level_size20_10_sram[0]}),
		.mem_outb({mux_2level_size20_10_sram_inv[9], mux_2level_size20_10_sram_inv[8], mux_2level_size20_10_sram_inv[7], mux_2level_size20_10_sram_inv[6], mux_2level_size20_10_sram_inv[5], mux_2level_size20_10_sram_inv[4], mux_2level_size20_10_sram_inv[3], mux_2level_size20_10_sram_inv[2], mux_2level_size20_10_sram_inv[1], mux_2level_size20_10_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_2_in_3 (
		.feedthrough_mem_in({feedthrough_mem_in[223], feedthrough_mem_in[222], feedthrough_mem_in[221], feedthrough_mem_in[220], feedthrough_mem_in[219], feedthrough_mem_in[218], feedthrough_mem_in[217], feedthrough_mem_in[216], feedthrough_mem_in[215], feedthrough_mem_in[214]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[223], feedthrough_mem_inb[222], feedthrough_mem_inb[221], feedthrough_mem_inb[220], feedthrough_mem_inb[219], feedthrough_mem_inb[218], feedthrough_mem_inb[217], feedthrough_mem_inb[216], feedthrough_mem_inb[215], feedthrough_mem_inb[214]}),
		.mem_out({mux_2level_size20_11_sram[9], mux_2level_size20_11_sram[8], mux_2level_size20_11_sram[7], mux_2level_size20_11_sram[6], mux_2level_size20_11_sram[5], mux_2level_size20_11_sram[4], mux_2level_size20_11_sram[3], mux_2level_size20_11_sram[2], mux_2level_size20_11_sram[1], mux_2level_size20_11_sram[0]}),
		.mem_outb({mux_2level_size20_11_sram_inv[9], mux_2level_size20_11_sram_inv[8], mux_2level_size20_11_sram_inv[7], mux_2level_size20_11_sram_inv[6], mux_2level_size20_11_sram_inv[5], mux_2level_size20_11_sram_inv[4], mux_2level_size20_11_sram_inv[3], mux_2level_size20_11_sram_inv[2], mux_2level_size20_11_sram_inv[1], mux_2level_size20_11_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_3_in_0 (
		.feedthrough_mem_in({feedthrough_mem_in[233], feedthrough_mem_in[232], feedthrough_mem_in[231], feedthrough_mem_in[230], feedthrough_mem_in[229], feedthrough_mem_in[228], feedthrough_mem_in[227], feedthrough_mem_in[226], feedthrough_mem_in[225], feedthrough_mem_in[224]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[233], feedthrough_mem_inb[232], feedthrough_mem_inb[231], feedthrough_mem_inb[230], feedthrough_mem_inb[229], feedthrough_mem_inb[228], feedthrough_mem_inb[227], feedthrough_mem_inb[226], feedthrough_mem_inb[225], feedthrough_mem_inb[224]}),
		.mem_out({mux_2level_size20_12_sram[9], mux_2level_size20_12_sram[8], mux_2level_size20_12_sram[7], mux_2level_size20_12_sram[6], mux_2level_size20_12_sram[5], mux_2level_size20_12_sram[4], mux_2level_size20_12_sram[3], mux_2level_size20_12_sram[2], mux_2level_size20_12_sram[1], mux_2level_size20_12_sram[0]}),
		.mem_outb({mux_2level_size20_12_sram_inv[9], mux_2level_size20_12_sram_inv[8], mux_2level_size20_12_sram_inv[7], mux_2level_size20_12_sram_inv[6], mux_2level_size20_12_sram_inv[5], mux_2level_size20_12_sram_inv[4], mux_2level_size20_12_sram_inv[3], mux_2level_size20_12_sram_inv[2], mux_2level_size20_12_sram_inv[1], mux_2level_size20_12_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_3_in_1 (
		.feedthrough_mem_in({feedthrough_mem_in[243], feedthrough_mem_in[242], feedthrough_mem_in[241], feedthrough_mem_in[240], feedthrough_mem_in[239], feedthrough_mem_in[238], feedthrough_mem_in[237], feedthrough_mem_in[236], feedthrough_mem_in[235], feedthrough_mem_in[234]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[243], feedthrough_mem_inb[242], feedthrough_mem_inb[241], feedthrough_mem_inb[240], feedthrough_mem_inb[239], feedthrough_mem_inb[238], feedthrough_mem_inb[237], feedthrough_mem_inb[236], feedthrough_mem_inb[235], feedthrough_mem_inb[234]}),
		.mem_out({mux_2level_size20_13_sram[9], mux_2level_size20_13_sram[8], mux_2level_size20_13_sram[7], mux_2level_size20_13_sram[6], mux_2level_size20_13_sram[5], mux_2level_size20_13_sram[4], mux_2level_size20_13_sram[3], mux_2level_size20_13_sram[2], mux_2level_size20_13_sram[1], mux_2level_size20_13_sram[0]}),
		.mem_outb({mux_2level_size20_13_sram_inv[9], mux_2level_size20_13_sram_inv[8], mux_2level_size20_13_sram_inv[7], mux_2level_size20_13_sram_inv[6], mux_2level_size20_13_sram_inv[5], mux_2level_size20_13_sram_inv[4], mux_2level_size20_13_sram_inv[3], mux_2level_size20_13_sram_inv[2], mux_2level_size20_13_sram_inv[1], mux_2level_size20_13_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_3_in_2 (
		.feedthrough_mem_in({feedthrough_mem_in[253], feedthrough_mem_in[252], feedthrough_mem_in[251], feedthrough_mem_in[250], feedthrough_mem_in[249], feedthrough_mem_in[248], feedthrough_mem_in[247], feedthrough_mem_in[246], feedthrough_mem_in[245], feedthrough_mem_in[244]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[253], feedthrough_mem_inb[252], feedthrough_mem_inb[251], feedthrough_mem_inb[250], feedthrough_mem_inb[249], feedthrough_mem_inb[248], feedthrough_mem_inb[247], feedthrough_mem_inb[246], feedthrough_mem_inb[245], feedthrough_mem_inb[244]}),
		.mem_out({mux_2level_size20_14_sram[9], mux_2level_size20_14_sram[8], mux_2level_size20_14_sram[7], mux_2level_size20_14_sram[6], mux_2level_size20_14_sram[5], mux_2level_size20_14_sram[4], mux_2level_size20_14_sram[3], mux_2level_size20_14_sram[2], mux_2level_size20_14_sram[1], mux_2level_size20_14_sram[0]}),
		.mem_outb({mux_2level_size20_14_sram_inv[9], mux_2level_size20_14_sram_inv[8], mux_2level_size20_14_sram_inv[7], mux_2level_size20_14_sram_inv[6], mux_2level_size20_14_sram_inv[5], mux_2level_size20_14_sram_inv[4], mux_2level_size20_14_sram_inv[3], mux_2level_size20_14_sram_inv[2], mux_2level_size20_14_sram_inv[1], mux_2level_size20_14_sram_inv[0]}));

	mux_2level_size20_feedthrough_mem virtual_mem_fle_3_in_3 (
		.feedthrough_mem_in({feedthrough_mem_in[263], feedthrough_mem_in[262], feedthrough_mem_in[261], feedthrough_mem_in[260], feedthrough_mem_in[259], feedthrough_mem_in[258], feedthrough_mem_in[257], feedthrough_mem_in[256], feedthrough_mem_in[255], feedthrough_mem_in[254]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[263], feedthrough_mem_inb[262], feedthrough_mem_inb[261], feedthrough_mem_inb[260], feedthrough_mem_inb[259], feedthrough_mem_inb[258], feedthrough_mem_inb[257], feedthrough_mem_inb[256], feedthrough_mem_inb[255], feedthrough_mem_inb[254]}),
		.mem_out({mux_2level_size20_15_sram[9], mux_2level_size20_15_sram[8], mux_2level_size20_15_sram[7], mux_2level_size20_15_sram[6], mux_2level_size20_15_sram[5], mux_2level_size20_15_sram[4], mux_2level_size20_15_sram[3], mux_2level_size20_15_sram[2], mux_2level_size20_15_sram[1], mux_2level_size20_15_sram[0]}),
		.mem_outb({mux_2level_size20_15_sram_inv[9], mux_2level_size20_15_sram_inv[8], mux_2level_size20_15_sram_inv[7], mux_2level_size20_15_sram_inv[6], mux_2level_size20_15_sram_inv[5], mux_2level_size20_15_sram_inv[4], mux_2level_size20_15_sram_inv[3], mux_2level_size20_15_sram_inv[2], mux_2level_size20_15_sram_inv[1], mux_2level_size20_15_sram_inv[0]}));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_clb_ -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: clb -----
