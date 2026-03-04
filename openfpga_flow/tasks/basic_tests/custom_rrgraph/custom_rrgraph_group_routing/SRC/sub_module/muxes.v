//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexers
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size16 -----
module mux_2level_tapbuf_size16(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [15:0] in;
//----- INPUT PORTS -----
input [9:0] sram;
//----- INPUT PORTS -----
input [9:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input5_mem5_0_out;
wire [0:0] mux_2level_tapbuf_basis_input5_mem5_1_out;
wire [0:0] mux_2level_tapbuf_basis_input5_mem5_2_out;
wire [0:0] mux_2level_tapbuf_basis_input5_mem5_3_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input5_mem5_3_out),
		.out(out));

	mux_2level_tapbuf_basis_input5_mem5 mux_l1_in_0_ (
		.in({INVTX1_4_out, INVTX1_3_out, INVTX1_2_out, INVTX1_1_out, INVTX1_0_out}),
		.mem({sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input5_mem5_0_out));

	mux_2level_tapbuf_basis_input5_mem5 mux_l1_in_1_ (
		.in({INVTX1_9_out, INVTX1_8_out, INVTX1_7_out, INVTX1_6_out, INVTX1_5_out}),
		.mem({sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input5_mem5_1_out));

	mux_2level_tapbuf_basis_input5_mem5 mux_l1_in_2_ (
		.in({INVTX1_14_out, INVTX1_13_out, INVTX1_12_out, INVTX1_11_out, INVTX1_10_out}),
		.mem({sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input5_mem5_2_out));

	mux_2level_tapbuf_basis_input5_mem5 mux_l2_in_0_ (
		.in({const1_0_const1, INVTX1_15_out, mux_2level_tapbuf_basis_input5_mem5_2_out, mux_2level_tapbuf_basis_input5_mem5_1_out, mux_2level_tapbuf_basis_input5_mem5_0_out}),
		.mem({sram[9], sram[8], sram[7], sram[6], sram[5]}),
		.mem_inv({sram_inv[9], sram_inv[8], sram_inv[7], sram_inv[6], sram_inv[5]}),
		.out(mux_2level_tapbuf_basis_input5_mem5_3_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size16 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size39 -----
module mux_2level_tapbuf_size39(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [38:0] in;
//----- INPUT PORTS -----
input [13:0] sram;
//----- INPUT PORTS -----
input [13:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_16_out;
wire [0:0] INVTX1_17_out;
wire [0:0] INVTX1_18_out;
wire [0:0] INVTX1_19_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_20_out;
wire [0:0] INVTX1_21_out;
wire [0:0] INVTX1_22_out;
wire [0:0] INVTX1_23_out;
wire [0:0] INVTX1_24_out;
wire [0:0] INVTX1_25_out;
wire [0:0] INVTX1_26_out;
wire [0:0] INVTX1_27_out;
wire [0:0] INVTX1_28_out;
wire [0:0] INVTX1_29_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_30_out;
wire [0:0] INVTX1_31_out;
wire [0:0] INVTX1_32_out;
wire [0:0] INVTX1_33_out;
wire [0:0] INVTX1_34_out;
wire [0:0] INVTX1_35_out;
wire [0:0] INVTX1_36_out;
wire [0:0] INVTX1_37_out;
wire [0:0] INVTX1_38_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input4_mem4_0_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_0_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_1_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_2_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_3_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_4_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_5_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	INVTX1 INVTX1_16_ (
		.in(in[16]),
		.out(INVTX1_16_out));

	INVTX1 INVTX1_17_ (
		.in(in[17]),
		.out(INVTX1_17_out));

	INVTX1 INVTX1_18_ (
		.in(in[18]),
		.out(INVTX1_18_out));

	INVTX1 INVTX1_19_ (
		.in(in[19]),
		.out(INVTX1_19_out));

	INVTX1 INVTX1_20_ (
		.in(in[20]),
		.out(INVTX1_20_out));

	INVTX1 INVTX1_21_ (
		.in(in[21]),
		.out(INVTX1_21_out));

	INVTX1 INVTX1_22_ (
		.in(in[22]),
		.out(INVTX1_22_out));

	INVTX1 INVTX1_23_ (
		.in(in[23]),
		.out(INVTX1_23_out));

	INVTX1 INVTX1_24_ (
		.in(in[24]),
		.out(INVTX1_24_out));

	INVTX1 INVTX1_25_ (
		.in(in[25]),
		.out(INVTX1_25_out));

	INVTX1 INVTX1_26_ (
		.in(in[26]),
		.out(INVTX1_26_out));

	INVTX1 INVTX1_27_ (
		.in(in[27]),
		.out(INVTX1_27_out));

	INVTX1 INVTX1_28_ (
		.in(in[28]),
		.out(INVTX1_28_out));

	INVTX1 INVTX1_29_ (
		.in(in[29]),
		.out(INVTX1_29_out));

	INVTX1 INVTX1_30_ (
		.in(in[30]),
		.out(INVTX1_30_out));

	INVTX1 INVTX1_31_ (
		.in(in[31]),
		.out(INVTX1_31_out));

	INVTX1 INVTX1_32_ (
		.in(in[32]),
		.out(INVTX1_32_out));

	INVTX1 INVTX1_33_ (
		.in(in[33]),
		.out(INVTX1_33_out));

	INVTX1 INVTX1_34_ (
		.in(in[34]),
		.out(INVTX1_34_out));

	INVTX1 INVTX1_35_ (
		.in(in[35]),
		.out(INVTX1_35_out));

	INVTX1 INVTX1_36_ (
		.in(in[36]),
		.out(INVTX1_36_out));

	INVTX1 INVTX1_37_ (
		.in(in[37]),
		.out(INVTX1_37_out));

	INVTX1 INVTX1_38_ (
		.in(in[38]),
		.out(INVTX1_38_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input7_mem7_5_out),
		.out(out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_0_ (
		.in({INVTX1_6_out, INVTX1_5_out, INVTX1_4_out, INVTX1_3_out, INVTX1_2_out, INVTX1_1_out, INVTX1_0_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_0_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_1_ (
		.in({INVTX1_13_out, INVTX1_12_out, INVTX1_11_out, INVTX1_10_out, INVTX1_9_out, INVTX1_8_out, INVTX1_7_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_1_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_2_ (
		.in({INVTX1_20_out, INVTX1_19_out, INVTX1_18_out, INVTX1_17_out, INVTX1_16_out, INVTX1_15_out, INVTX1_14_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_2_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_3_ (
		.in({INVTX1_27_out, INVTX1_26_out, INVTX1_25_out, INVTX1_24_out, INVTX1_23_out, INVTX1_22_out, INVTX1_21_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_3_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_4_ (
		.in({INVTX1_34_out, INVTX1_33_out, INVTX1_32_out, INVTX1_31_out, INVTX1_30_out, INVTX1_29_out, INVTX1_28_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_4_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l2_in_0_ (
		.in({const1_0_const1, mux_2level_tapbuf_basis_input4_mem4_0_out, mux_2level_tapbuf_basis_input7_mem7_4_out, mux_2level_tapbuf_basis_input7_mem7_3_out, mux_2level_tapbuf_basis_input7_mem7_2_out, mux_2level_tapbuf_basis_input7_mem7_1_out, mux_2level_tapbuf_basis_input7_mem7_0_out}),
		.mem({sram[13], sram[12], sram[11], sram[10], sram[9], sram[8], sram[7]}),
		.mem_inv({sram_inv[13], sram_inv[12], sram_inv[11], sram_inv[10], sram_inv[9], sram_inv[8], sram_inv[7]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_5_out));

	mux_2level_tapbuf_basis_input4_mem4 mux_l1_in_5_ (
		.in({INVTX1_38_out, INVTX1_37_out, INVTX1_36_out, INVTX1_35_out}),
		.mem({sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input4_mem4_0_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size39 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size32 -----
module mux_2level_tapbuf_size32(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [31:0] in;
//----- INPUT PORTS -----
input [11:0] sram;
//----- INPUT PORTS -----
input [11:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_16_out;
wire [0:0] INVTX1_17_out;
wire [0:0] INVTX1_18_out;
wire [0:0] INVTX1_19_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_20_out;
wire [0:0] INVTX1_21_out;
wire [0:0] INVTX1_22_out;
wire [0:0] INVTX1_23_out;
wire [0:0] INVTX1_24_out;
wire [0:0] INVTX1_25_out;
wire [0:0] INVTX1_26_out;
wire [0:0] INVTX1_27_out;
wire [0:0] INVTX1_28_out;
wire [0:0] INVTX1_29_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_30_out;
wire [0:0] INVTX1_31_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input3_mem3_0_out;
wire [0:0] mux_2level_tapbuf_basis_input6_mem6_0_out;
wire [0:0] mux_2level_tapbuf_basis_input6_mem6_1_out;
wire [0:0] mux_2level_tapbuf_basis_input6_mem6_2_out;
wire [0:0] mux_2level_tapbuf_basis_input6_mem6_3_out;
wire [0:0] mux_2level_tapbuf_basis_input6_mem6_4_out;
wire [0:0] mux_2level_tapbuf_basis_input6_mem6_5_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	INVTX1 INVTX1_16_ (
		.in(in[16]),
		.out(INVTX1_16_out));

	INVTX1 INVTX1_17_ (
		.in(in[17]),
		.out(INVTX1_17_out));

	INVTX1 INVTX1_18_ (
		.in(in[18]),
		.out(INVTX1_18_out));

	INVTX1 INVTX1_19_ (
		.in(in[19]),
		.out(INVTX1_19_out));

	INVTX1 INVTX1_20_ (
		.in(in[20]),
		.out(INVTX1_20_out));

	INVTX1 INVTX1_21_ (
		.in(in[21]),
		.out(INVTX1_21_out));

	INVTX1 INVTX1_22_ (
		.in(in[22]),
		.out(INVTX1_22_out));

	INVTX1 INVTX1_23_ (
		.in(in[23]),
		.out(INVTX1_23_out));

	INVTX1 INVTX1_24_ (
		.in(in[24]),
		.out(INVTX1_24_out));

	INVTX1 INVTX1_25_ (
		.in(in[25]),
		.out(INVTX1_25_out));

	INVTX1 INVTX1_26_ (
		.in(in[26]),
		.out(INVTX1_26_out));

	INVTX1 INVTX1_27_ (
		.in(in[27]),
		.out(INVTX1_27_out));

	INVTX1 INVTX1_28_ (
		.in(in[28]),
		.out(INVTX1_28_out));

	INVTX1 INVTX1_29_ (
		.in(in[29]),
		.out(INVTX1_29_out));

	INVTX1 INVTX1_30_ (
		.in(in[30]),
		.out(INVTX1_30_out));

	INVTX1 INVTX1_31_ (
		.in(in[31]),
		.out(INVTX1_31_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input6_mem6_5_out),
		.out(out));

	mux_2level_tapbuf_basis_input6_mem6 mux_l1_in_0_ (
		.in({INVTX1_5_out, INVTX1_4_out, INVTX1_3_out, INVTX1_2_out, INVTX1_1_out, INVTX1_0_out}),
		.mem({sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input6_mem6_0_out));

	mux_2level_tapbuf_basis_input6_mem6 mux_l1_in_1_ (
		.in({INVTX1_11_out, INVTX1_10_out, INVTX1_9_out, INVTX1_8_out, INVTX1_7_out, INVTX1_6_out}),
		.mem({sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input6_mem6_1_out));

	mux_2level_tapbuf_basis_input6_mem6 mux_l1_in_2_ (
		.in({INVTX1_17_out, INVTX1_16_out, INVTX1_15_out, INVTX1_14_out, INVTX1_13_out, INVTX1_12_out}),
		.mem({sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input6_mem6_2_out));

	mux_2level_tapbuf_basis_input6_mem6 mux_l1_in_3_ (
		.in({INVTX1_23_out, INVTX1_22_out, INVTX1_21_out, INVTX1_20_out, INVTX1_19_out, INVTX1_18_out}),
		.mem({sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input6_mem6_3_out));

	mux_2level_tapbuf_basis_input6_mem6 mux_l1_in_4_ (
		.in({INVTX1_29_out, INVTX1_28_out, INVTX1_27_out, INVTX1_26_out, INVTX1_25_out, INVTX1_24_out}),
		.mem({sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input6_mem6_4_out));

	mux_2level_tapbuf_basis_input6_mem6 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input3_mem3_0_out, mux_2level_tapbuf_basis_input6_mem6_4_out, mux_2level_tapbuf_basis_input6_mem6_3_out, mux_2level_tapbuf_basis_input6_mem6_2_out, mux_2level_tapbuf_basis_input6_mem6_1_out, mux_2level_tapbuf_basis_input6_mem6_0_out}),
		.mem({sram[11], sram[10], sram[9], sram[8], sram[7], sram[6]}),
		.mem_inv({sram_inv[11], sram_inv[10], sram_inv[9], sram_inv[8], sram_inv[7], sram_inv[6]}),
		.out(mux_2level_tapbuf_basis_input6_mem6_5_out));

	mux_2level_tapbuf_basis_input3_mem3 mux_l1_in_5_ (
		.in({const1_0_const1, INVTX1_31_out, INVTX1_30_out}),
		.mem({sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input3_mem3_0_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size32 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size48 -----
module mux_2level_tapbuf_size48(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [47:0] in;
//----- INPUT PORTS -----
input [13:0] sram;
//----- INPUT PORTS -----
input [13:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_16_out;
wire [0:0] INVTX1_17_out;
wire [0:0] INVTX1_18_out;
wire [0:0] INVTX1_19_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_20_out;
wire [0:0] INVTX1_21_out;
wire [0:0] INVTX1_22_out;
wire [0:0] INVTX1_23_out;
wire [0:0] INVTX1_24_out;
wire [0:0] INVTX1_25_out;
wire [0:0] INVTX1_26_out;
wire [0:0] INVTX1_27_out;
wire [0:0] INVTX1_28_out;
wire [0:0] INVTX1_29_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_30_out;
wire [0:0] INVTX1_31_out;
wire [0:0] INVTX1_32_out;
wire [0:0] INVTX1_33_out;
wire [0:0] INVTX1_34_out;
wire [0:0] INVTX1_35_out;
wire [0:0] INVTX1_36_out;
wire [0:0] INVTX1_37_out;
wire [0:0] INVTX1_38_out;
wire [0:0] INVTX1_39_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_40_out;
wire [0:0] INVTX1_41_out;
wire [0:0] INVTX1_42_out;
wire [0:0] INVTX1_43_out;
wire [0:0] INVTX1_44_out;
wire [0:0] INVTX1_45_out;
wire [0:0] INVTX1_46_out;
wire [0:0] INVTX1_47_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_0_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_1_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_2_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_3_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_4_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_5_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_6_out;
wire [0:0] mux_2level_tapbuf_basis_input7_mem7_7_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	INVTX1 INVTX1_16_ (
		.in(in[16]),
		.out(INVTX1_16_out));

	INVTX1 INVTX1_17_ (
		.in(in[17]),
		.out(INVTX1_17_out));

	INVTX1 INVTX1_18_ (
		.in(in[18]),
		.out(INVTX1_18_out));

	INVTX1 INVTX1_19_ (
		.in(in[19]),
		.out(INVTX1_19_out));

	INVTX1 INVTX1_20_ (
		.in(in[20]),
		.out(INVTX1_20_out));

	INVTX1 INVTX1_21_ (
		.in(in[21]),
		.out(INVTX1_21_out));

	INVTX1 INVTX1_22_ (
		.in(in[22]),
		.out(INVTX1_22_out));

	INVTX1 INVTX1_23_ (
		.in(in[23]),
		.out(INVTX1_23_out));

	INVTX1 INVTX1_24_ (
		.in(in[24]),
		.out(INVTX1_24_out));

	INVTX1 INVTX1_25_ (
		.in(in[25]),
		.out(INVTX1_25_out));

	INVTX1 INVTX1_26_ (
		.in(in[26]),
		.out(INVTX1_26_out));

	INVTX1 INVTX1_27_ (
		.in(in[27]),
		.out(INVTX1_27_out));

	INVTX1 INVTX1_28_ (
		.in(in[28]),
		.out(INVTX1_28_out));

	INVTX1 INVTX1_29_ (
		.in(in[29]),
		.out(INVTX1_29_out));

	INVTX1 INVTX1_30_ (
		.in(in[30]),
		.out(INVTX1_30_out));

	INVTX1 INVTX1_31_ (
		.in(in[31]),
		.out(INVTX1_31_out));

	INVTX1 INVTX1_32_ (
		.in(in[32]),
		.out(INVTX1_32_out));

	INVTX1 INVTX1_33_ (
		.in(in[33]),
		.out(INVTX1_33_out));

	INVTX1 INVTX1_34_ (
		.in(in[34]),
		.out(INVTX1_34_out));

	INVTX1 INVTX1_35_ (
		.in(in[35]),
		.out(INVTX1_35_out));

	INVTX1 INVTX1_36_ (
		.in(in[36]),
		.out(INVTX1_36_out));

	INVTX1 INVTX1_37_ (
		.in(in[37]),
		.out(INVTX1_37_out));

	INVTX1 INVTX1_38_ (
		.in(in[38]),
		.out(INVTX1_38_out));

	INVTX1 INVTX1_39_ (
		.in(in[39]),
		.out(INVTX1_39_out));

	INVTX1 INVTX1_40_ (
		.in(in[40]),
		.out(INVTX1_40_out));

	INVTX1 INVTX1_41_ (
		.in(in[41]),
		.out(INVTX1_41_out));

	INVTX1 INVTX1_42_ (
		.in(in[42]),
		.out(INVTX1_42_out));

	INVTX1 INVTX1_43_ (
		.in(in[43]),
		.out(INVTX1_43_out));

	INVTX1 INVTX1_44_ (
		.in(in[44]),
		.out(INVTX1_44_out));

	INVTX1 INVTX1_45_ (
		.in(in[45]),
		.out(INVTX1_45_out));

	INVTX1 INVTX1_46_ (
		.in(in[46]),
		.out(INVTX1_46_out));

	INVTX1 INVTX1_47_ (
		.in(in[47]),
		.out(INVTX1_47_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input7_mem7_7_out),
		.out(out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_0_ (
		.in({INVTX1_6_out, INVTX1_5_out, INVTX1_4_out, INVTX1_3_out, INVTX1_2_out, INVTX1_1_out, INVTX1_0_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_0_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_1_ (
		.in({INVTX1_13_out, INVTX1_12_out, INVTX1_11_out, INVTX1_10_out, INVTX1_9_out, INVTX1_8_out, INVTX1_7_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_1_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_2_ (
		.in({INVTX1_20_out, INVTX1_19_out, INVTX1_18_out, INVTX1_17_out, INVTX1_16_out, INVTX1_15_out, INVTX1_14_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_2_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_3_ (
		.in({INVTX1_27_out, INVTX1_26_out, INVTX1_25_out, INVTX1_24_out, INVTX1_23_out, INVTX1_22_out, INVTX1_21_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_3_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_4_ (
		.in({INVTX1_34_out, INVTX1_33_out, INVTX1_32_out, INVTX1_31_out, INVTX1_30_out, INVTX1_29_out, INVTX1_28_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_4_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_5_ (
		.in({INVTX1_41_out, INVTX1_40_out, INVTX1_39_out, INVTX1_38_out, INVTX1_37_out, INVTX1_36_out, INVTX1_35_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_5_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l1_in_6_ (
		.in({const1_0_const1, INVTX1_47_out, INVTX1_46_out, INVTX1_45_out, INVTX1_44_out, INVTX1_43_out, INVTX1_42_out}),
		.mem({sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_6_out));

	mux_2level_tapbuf_basis_input7_mem7 mux_l2_in_0_ (
		.in({mux_2level_tapbuf_basis_input7_mem7_6_out, mux_2level_tapbuf_basis_input7_mem7_5_out, mux_2level_tapbuf_basis_input7_mem7_4_out, mux_2level_tapbuf_basis_input7_mem7_3_out, mux_2level_tapbuf_basis_input7_mem7_2_out, mux_2level_tapbuf_basis_input7_mem7_1_out, mux_2level_tapbuf_basis_input7_mem7_0_out}),
		.mem({sram[13], sram[12], sram[11], sram[10], sram[9], sram[8], sram[7]}),
		.mem_inv({sram_inv[13], sram_inv[12], sram_inv[11], sram_inv[10], sram_inv[9], sram_inv[8], sram_inv[7]}),
		.out(mux_2level_tapbuf_basis_input7_mem7_7_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size48 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size56 -----
module mux_2level_tapbuf_size56(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [55:0] in;
//----- INPUT PORTS -----
input [15:0] sram;
//----- INPUT PORTS -----
input [15:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_16_out;
wire [0:0] INVTX1_17_out;
wire [0:0] INVTX1_18_out;
wire [0:0] INVTX1_19_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_20_out;
wire [0:0] INVTX1_21_out;
wire [0:0] INVTX1_22_out;
wire [0:0] INVTX1_23_out;
wire [0:0] INVTX1_24_out;
wire [0:0] INVTX1_25_out;
wire [0:0] INVTX1_26_out;
wire [0:0] INVTX1_27_out;
wire [0:0] INVTX1_28_out;
wire [0:0] INVTX1_29_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_30_out;
wire [0:0] INVTX1_31_out;
wire [0:0] INVTX1_32_out;
wire [0:0] INVTX1_33_out;
wire [0:0] INVTX1_34_out;
wire [0:0] INVTX1_35_out;
wire [0:0] INVTX1_36_out;
wire [0:0] INVTX1_37_out;
wire [0:0] INVTX1_38_out;
wire [0:0] INVTX1_39_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_40_out;
wire [0:0] INVTX1_41_out;
wire [0:0] INVTX1_42_out;
wire [0:0] INVTX1_43_out;
wire [0:0] INVTX1_44_out;
wire [0:0] INVTX1_45_out;
wire [0:0] INVTX1_46_out;
wire [0:0] INVTX1_47_out;
wire [0:0] INVTX1_48_out;
wire [0:0] INVTX1_49_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_50_out;
wire [0:0] INVTX1_51_out;
wire [0:0] INVTX1_52_out;
wire [0:0] INVTX1_53_out;
wire [0:0] INVTX1_54_out;
wire [0:0] INVTX1_55_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_0_out;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_1_out;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_2_out;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_3_out;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_4_out;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_5_out;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_6_out;
wire [0:0] mux_2level_tapbuf_basis_input8_mem8_7_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	INVTX1 INVTX1_16_ (
		.in(in[16]),
		.out(INVTX1_16_out));

	INVTX1 INVTX1_17_ (
		.in(in[17]),
		.out(INVTX1_17_out));

	INVTX1 INVTX1_18_ (
		.in(in[18]),
		.out(INVTX1_18_out));

	INVTX1 INVTX1_19_ (
		.in(in[19]),
		.out(INVTX1_19_out));

	INVTX1 INVTX1_20_ (
		.in(in[20]),
		.out(INVTX1_20_out));

	INVTX1 INVTX1_21_ (
		.in(in[21]),
		.out(INVTX1_21_out));

	INVTX1 INVTX1_22_ (
		.in(in[22]),
		.out(INVTX1_22_out));

	INVTX1 INVTX1_23_ (
		.in(in[23]),
		.out(INVTX1_23_out));

	INVTX1 INVTX1_24_ (
		.in(in[24]),
		.out(INVTX1_24_out));

	INVTX1 INVTX1_25_ (
		.in(in[25]),
		.out(INVTX1_25_out));

	INVTX1 INVTX1_26_ (
		.in(in[26]),
		.out(INVTX1_26_out));

	INVTX1 INVTX1_27_ (
		.in(in[27]),
		.out(INVTX1_27_out));

	INVTX1 INVTX1_28_ (
		.in(in[28]),
		.out(INVTX1_28_out));

	INVTX1 INVTX1_29_ (
		.in(in[29]),
		.out(INVTX1_29_out));

	INVTX1 INVTX1_30_ (
		.in(in[30]),
		.out(INVTX1_30_out));

	INVTX1 INVTX1_31_ (
		.in(in[31]),
		.out(INVTX1_31_out));

	INVTX1 INVTX1_32_ (
		.in(in[32]),
		.out(INVTX1_32_out));

	INVTX1 INVTX1_33_ (
		.in(in[33]),
		.out(INVTX1_33_out));

	INVTX1 INVTX1_34_ (
		.in(in[34]),
		.out(INVTX1_34_out));

	INVTX1 INVTX1_35_ (
		.in(in[35]),
		.out(INVTX1_35_out));

	INVTX1 INVTX1_36_ (
		.in(in[36]),
		.out(INVTX1_36_out));

	INVTX1 INVTX1_37_ (
		.in(in[37]),
		.out(INVTX1_37_out));

	INVTX1 INVTX1_38_ (
		.in(in[38]),
		.out(INVTX1_38_out));

	INVTX1 INVTX1_39_ (
		.in(in[39]),
		.out(INVTX1_39_out));

	INVTX1 INVTX1_40_ (
		.in(in[40]),
		.out(INVTX1_40_out));

	INVTX1 INVTX1_41_ (
		.in(in[41]),
		.out(INVTX1_41_out));

	INVTX1 INVTX1_42_ (
		.in(in[42]),
		.out(INVTX1_42_out));

	INVTX1 INVTX1_43_ (
		.in(in[43]),
		.out(INVTX1_43_out));

	INVTX1 INVTX1_44_ (
		.in(in[44]),
		.out(INVTX1_44_out));

	INVTX1 INVTX1_45_ (
		.in(in[45]),
		.out(INVTX1_45_out));

	INVTX1 INVTX1_46_ (
		.in(in[46]),
		.out(INVTX1_46_out));

	INVTX1 INVTX1_47_ (
		.in(in[47]),
		.out(INVTX1_47_out));

	INVTX1 INVTX1_48_ (
		.in(in[48]),
		.out(INVTX1_48_out));

	INVTX1 INVTX1_49_ (
		.in(in[49]),
		.out(INVTX1_49_out));

	INVTX1 INVTX1_50_ (
		.in(in[50]),
		.out(INVTX1_50_out));

	INVTX1 INVTX1_51_ (
		.in(in[51]),
		.out(INVTX1_51_out));

	INVTX1 INVTX1_52_ (
		.in(in[52]),
		.out(INVTX1_52_out));

	INVTX1 INVTX1_53_ (
		.in(in[53]),
		.out(INVTX1_53_out));

	INVTX1 INVTX1_54_ (
		.in(in[54]),
		.out(INVTX1_54_out));

	INVTX1 INVTX1_55_ (
		.in(in[55]),
		.out(INVTX1_55_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_2level_tapbuf_basis_input8_mem8_7_out),
		.out(out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l1_in_0_ (
		.in({INVTX1_7_out, INVTX1_6_out, INVTX1_5_out, INVTX1_4_out, INVTX1_3_out, INVTX1_2_out, INVTX1_1_out, INVTX1_0_out}),
		.mem({sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[7], sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_0_out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l1_in_1_ (
		.in({INVTX1_15_out, INVTX1_14_out, INVTX1_13_out, INVTX1_12_out, INVTX1_11_out, INVTX1_10_out, INVTX1_9_out, INVTX1_8_out}),
		.mem({sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[7], sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_1_out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l1_in_2_ (
		.in({INVTX1_23_out, INVTX1_22_out, INVTX1_21_out, INVTX1_20_out, INVTX1_19_out, INVTX1_18_out, INVTX1_17_out, INVTX1_16_out}),
		.mem({sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[7], sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_2_out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l1_in_3_ (
		.in({INVTX1_31_out, INVTX1_30_out, INVTX1_29_out, INVTX1_28_out, INVTX1_27_out, INVTX1_26_out, INVTX1_25_out, INVTX1_24_out}),
		.mem({sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[7], sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_3_out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l1_in_4_ (
		.in({INVTX1_39_out, INVTX1_38_out, INVTX1_37_out, INVTX1_36_out, INVTX1_35_out, INVTX1_34_out, INVTX1_33_out, INVTX1_32_out}),
		.mem({sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[7], sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_4_out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l1_in_5_ (
		.in({INVTX1_47_out, INVTX1_46_out, INVTX1_45_out, INVTX1_44_out, INVTX1_43_out, INVTX1_42_out, INVTX1_41_out, INVTX1_40_out}),
		.mem({sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[7], sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_5_out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l1_in_6_ (
		.in({INVTX1_55_out, INVTX1_54_out, INVTX1_53_out, INVTX1_52_out, INVTX1_51_out, INVTX1_50_out, INVTX1_49_out, INVTX1_48_out}),
		.mem({sram[7], sram[6], sram[5], sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[7], sram_inv[6], sram_inv[5], sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_6_out));

	mux_2level_tapbuf_basis_input8_mem8 mux_l2_in_0_ (
		.in({const1_0_const1, mux_2level_tapbuf_basis_input8_mem8_6_out, mux_2level_tapbuf_basis_input8_mem8_5_out, mux_2level_tapbuf_basis_input8_mem8_4_out, mux_2level_tapbuf_basis_input8_mem8_3_out, mux_2level_tapbuf_basis_input8_mem8_2_out, mux_2level_tapbuf_basis_input8_mem8_1_out, mux_2level_tapbuf_basis_input8_mem8_0_out}),
		.mem({sram[15], sram[14], sram[13], sram[12], sram[11], sram[10], sram[9], sram[8]}),
		.mem_inv({sram_inv[15], sram_inv[14], sram_inv[13], sram_inv[12], sram_inv[11], sram_inv[10], sram_inv[9], sram_inv[8]}),
		.out(mux_2level_tapbuf_basis_input8_mem8_7_out));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size56 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_size20 -----
module mux_2level_size20(in,
                         sram,
                         sram_inv,
                         out);
//----- INPUT PORTS -----
input [19:0] in;
//----- INPUT PORTS -----
input [9:0] sram;
//----- INPUT PORTS -----
input [9:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_16_out;
wire [0:0] INVTX1_17_out;
wire [0:0] INVTX1_18_out;
wire [0:0] INVTX1_19_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_2level_basis_input5_mem5_0_out;
wire [0:0] mux_2level_basis_input5_mem5_1_out;
wire [0:0] mux_2level_basis_input5_mem5_2_out;
wire [0:0] mux_2level_basis_input5_mem5_3_out;
wire [0:0] mux_2level_basis_input5_mem5_4_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	INVTX1 INVTX1_16_ (
		.in(in[16]),
		.out(INVTX1_16_out));

	INVTX1 INVTX1_17_ (
		.in(in[17]),
		.out(INVTX1_17_out));

	INVTX1 INVTX1_18_ (
		.in(in[18]),
		.out(INVTX1_18_out));

	INVTX1 INVTX1_19_ (
		.in(in[19]),
		.out(INVTX1_19_out));

	INVTX1 INVTX1_20_ (
		.in(mux_2level_basis_input5_mem5_4_out),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	mux_2level_basis_input5_mem5 mux_l1_in_0_ (
		.in({INVTX1_4_out, INVTX1_3_out, INVTX1_2_out, INVTX1_1_out, INVTX1_0_out}),
		.mem({sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_basis_input5_mem5_0_out));

	mux_2level_basis_input5_mem5 mux_l1_in_1_ (
		.in({INVTX1_9_out, INVTX1_8_out, INVTX1_7_out, INVTX1_6_out, INVTX1_5_out}),
		.mem({sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_basis_input5_mem5_1_out));

	mux_2level_basis_input5_mem5 mux_l1_in_2_ (
		.in({INVTX1_14_out, INVTX1_13_out, INVTX1_12_out, INVTX1_11_out, INVTX1_10_out}),
		.mem({sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_basis_input5_mem5_2_out));

	mux_2level_basis_input5_mem5 mux_l1_in_3_ (
		.in({INVTX1_19_out, INVTX1_18_out, INVTX1_17_out, INVTX1_16_out, INVTX1_15_out}),
		.mem({sram[4], sram[3], sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[4], sram_inv[3], sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_2level_basis_input5_mem5_3_out));

	mux_2level_basis_input5_mem5 mux_l2_in_0_ (
		.in({const1_0_const1, mux_2level_basis_input5_mem5_3_out, mux_2level_basis_input5_mem5_2_out, mux_2level_basis_input5_mem5_1_out, mux_2level_basis_input5_mem5_0_out}),
		.mem({sram[9], sram[8], sram[7], sram[6], sram[5]}),
		.mem_inv({sram_inv[9], sram_inv[8], sram_inv[7], sram_inv[6], sram_inv[5]}),
		.out(mux_2level_basis_input5_mem5_4_out));

endmodule
// ----- END Verilog module for mux_2level_size20 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_size2 -----
module mux_1level_tapbuf_size2(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [1:0] in;
//----- INPUT PORTS -----
input [2:0] sram;
//----- INPUT PORTS -----
input [2:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_1_out;
wire [0:0] const1_0_const1;
wire [0:0] mux_1level_tapbuf_basis_input3_mem3_0_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	tap_buf4 tap_buf4_0_ (
		.in(mux_1level_tapbuf_basis_input3_mem3_0_out),
		.out(out));

	mux_1level_tapbuf_basis_input3_mem3 mux_l1_in_0_ (
		.in({const1_0_const1, INVTX1_1_out, INVTX1_0_out}),
		.mem({sram[2], sram[1], sram[0]}),
		.mem_inv({sram_inv[2], sram_inv[1], sram_inv[0]}),
		.out(mux_1level_tapbuf_basis_input3_mem3_0_out));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for frac_lut4_mux -----
module frac_lut4_mux(in,
                     sram,
                     sram_inv,
                     lut3_out,
                     lut4_out);
//----- INPUT PORTS -----
input [15:0] in;
//----- INPUT PORTS -----
input [3:0] sram;
//----- INPUT PORTS -----
input [3:0] sram_inv;
//----- OUTPUT PORTS -----
output [1:0] lut3_out;
//----- OUTPUT PORTS -----
output [0:0] lut4_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] INVTX1_0_out;
wire [0:0] INVTX1_10_out;
wire [0:0] INVTX1_11_out;
wire [0:0] INVTX1_12_out;
wire [0:0] INVTX1_13_out;
wire [0:0] INVTX1_14_out;
wire [0:0] INVTX1_15_out;
wire [0:0] INVTX1_1_out;
wire [0:0] INVTX1_2_out;
wire [0:0] INVTX1_3_out;
wire [0:0] INVTX1_4_out;
wire [0:0] INVTX1_5_out;
wire [0:0] INVTX1_6_out;
wire [0:0] INVTX1_7_out;
wire [0:0] INVTX1_8_out;
wire [0:0] INVTX1_9_out;
wire [0:0] buf4_0_out;
wire [0:0] buf4_1_out;
wire [0:0] buf4_2_out;
wire [0:0] buf4_3_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_0_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_10_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_11_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_12_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_13_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_14_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_1_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_2_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_3_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_4_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_5_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_6_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_7_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_8_out;
wire [0:0] frac_lut4_mux_basis_input2_mem1_9_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	INVTX1 INVTX1_0_ (
		.in(in[0]),
		.out(INVTX1_0_out));

	INVTX1 INVTX1_1_ (
		.in(in[1]),
		.out(INVTX1_1_out));

	INVTX1 INVTX1_2_ (
		.in(in[2]),
		.out(INVTX1_2_out));

	INVTX1 INVTX1_3_ (
		.in(in[3]),
		.out(INVTX1_3_out));

	INVTX1 INVTX1_4_ (
		.in(in[4]),
		.out(INVTX1_4_out));

	INVTX1 INVTX1_5_ (
		.in(in[5]),
		.out(INVTX1_5_out));

	INVTX1 INVTX1_6_ (
		.in(in[6]),
		.out(INVTX1_6_out));

	INVTX1 INVTX1_7_ (
		.in(in[7]),
		.out(INVTX1_7_out));

	INVTX1 INVTX1_8_ (
		.in(in[8]),
		.out(INVTX1_8_out));

	INVTX1 INVTX1_9_ (
		.in(in[9]),
		.out(INVTX1_9_out));

	INVTX1 INVTX1_10_ (
		.in(in[10]),
		.out(INVTX1_10_out));

	INVTX1 INVTX1_11_ (
		.in(in[11]),
		.out(INVTX1_11_out));

	INVTX1 INVTX1_12_ (
		.in(in[12]),
		.out(INVTX1_12_out));

	INVTX1 INVTX1_13_ (
		.in(in[13]),
		.out(INVTX1_13_out));

	INVTX1 INVTX1_14_ (
		.in(in[14]),
		.out(INVTX1_14_out));

	INVTX1 INVTX1_15_ (
		.in(in[15]),
		.out(INVTX1_15_out));

	INVTX1 INVTX1_16_ (
		.in(frac_lut4_mux_basis_input2_mem1_12_out),
		.out(lut3_out[0]));

	INVTX1 INVTX1_17_ (
		.in(frac_lut4_mux_basis_input2_mem1_13_out),
		.out(lut3_out[1]));

	INVTX1 INVTX1_18_ (
		.in(frac_lut4_mux_basis_input2_mem1_14_out),
		.out(lut4_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_0_ (
		.in({INVTX1_1_out, INVTX1_0_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_0_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_1_ (
		.in({INVTX1_3_out, INVTX1_2_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_1_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_2_ (
		.in({INVTX1_5_out, INVTX1_4_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_2_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_3_ (
		.in({INVTX1_7_out, INVTX1_6_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_3_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_4_ (
		.in({INVTX1_9_out, INVTX1_8_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_4_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_5_ (
		.in({INVTX1_11_out, INVTX1_10_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_5_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_6_ (
		.in({INVTX1_13_out, INVTX1_12_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_6_out));

	frac_lut4_mux_basis_input2_mem1 mux_l1_in_7_ (
		.in({INVTX1_15_out, INVTX1_14_out}),
		.mem(sram[0]),
		.mem_inv(sram_inv[0]),
		.out(frac_lut4_mux_basis_input2_mem1_7_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_0_ (
		.in({frac_lut4_mux_basis_input2_mem1_1_out, frac_lut4_mux_basis_input2_mem1_0_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_8_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_1_ (
		.in({frac_lut4_mux_basis_input2_mem1_3_out, frac_lut4_mux_basis_input2_mem1_2_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_9_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_2_ (
		.in({frac_lut4_mux_basis_input2_mem1_5_out, frac_lut4_mux_basis_input2_mem1_4_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_10_out));

	frac_lut4_mux_basis_input2_mem1 mux_l2_in_3_ (
		.in({frac_lut4_mux_basis_input2_mem1_7_out, frac_lut4_mux_basis_input2_mem1_6_out}),
		.mem(sram[1]),
		.mem_inv(sram_inv[1]),
		.out(frac_lut4_mux_basis_input2_mem1_11_out));

	frac_lut4_mux_basis_input2_mem1 mux_l3_in_0_ (
		.in({buf4_1_out, buf4_0_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(frac_lut4_mux_basis_input2_mem1_12_out));

	frac_lut4_mux_basis_input2_mem1 mux_l3_in_1_ (
		.in({buf4_3_out, buf4_2_out}),
		.mem(sram[2]),
		.mem_inv(sram_inv[2]),
		.out(frac_lut4_mux_basis_input2_mem1_13_out));

	frac_lut4_mux_basis_input2_mem1 mux_l4_in_0_ (
		.in({frac_lut4_mux_basis_input2_mem1_13_out, frac_lut4_mux_basis_input2_mem1_12_out}),
		.mem(sram[3]),
		.mem_inv(sram_inv[3]),
		.out(frac_lut4_mux_basis_input2_mem1_14_out));

	buf4 buf4_0_ (
		.in(frac_lut4_mux_basis_input2_mem1_8_out),
		.out(buf4_0_out));

	buf4 buf4_1_ (
		.in(frac_lut4_mux_basis_input2_mem1_9_out),
		.out(buf4_1_out));

	buf4 buf4_2_ (
		.in(frac_lut4_mux_basis_input2_mem1_10_out),
		.out(buf4_2_out));

	buf4 buf4_3_ (
		.in(frac_lut4_mux_basis_input2_mem1_11_out),
		.out(buf4_3_out));

endmodule
// ----- END Verilog module for frac_lut4_mux -----

//----- Default net type -----
`default_nettype wire




