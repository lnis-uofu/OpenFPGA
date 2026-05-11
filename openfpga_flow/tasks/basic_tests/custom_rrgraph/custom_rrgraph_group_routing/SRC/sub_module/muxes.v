//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Multiplexers
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- Verilog module for mux_2level_tapbuf_size16 -----
module mux_2level_tapbuf_size16(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [15:0] in;
//----- INPUT PORTS -----
input [4:0] sram;
//----- INPUT PORTS -----
input [4:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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
		.in(MUX2_15_Y),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(INVTX1_2_out),
		.S0(sram[1]),
		.Y(MUX2_1_Y));

	MUX2 mux_l2_in_1_ (
		.B(INVTX1_3_out),
		.A(INVTX1_4_out),
		.S0(sram[1]),
		.Y(MUX2_2_Y));

	MUX2 mux_l2_in_2_ (
		.B(INVTX1_5_out),
		.A(INVTX1_6_out),
		.S0(sram[1]),
		.Y(MUX2_3_Y));

	MUX2 mux_l2_in_3_ (
		.B(INVTX1_7_out),
		.A(INVTX1_8_out),
		.S0(sram[1]),
		.Y(MUX2_4_Y));

	MUX2 mux_l2_in_4_ (
		.B(INVTX1_9_out),
		.A(INVTX1_10_out),
		.S0(sram[1]),
		.Y(MUX2_5_Y));

	MUX2 mux_l2_in_5_ (
		.B(INVTX1_11_out),
		.A(INVTX1_12_out),
		.S0(sram[1]),
		.Y(MUX2_6_Y));

	MUX2 mux_l2_in_6_ (
		.B(INVTX1_13_out),
		.A(INVTX1_14_out),
		.S0(sram[1]),
		.Y(MUX2_7_Y));

	MUX2 mux_l2_in_7_ (
		.B(INVTX1_15_out),
		.A(const1_0_const1),
		.S0(sram[1]),
		.Y(MUX2_8_Y));

	MUX2 mux_l3_in_0_ (
		.B(MUX2_1_Y),
		.A(MUX2_2_Y),
		.S0(sram[2]),
		.Y(MUX2_9_Y));

	MUX2 mux_l3_in_1_ (
		.B(MUX2_3_Y),
		.A(MUX2_4_Y),
		.S0(sram[2]),
		.Y(MUX2_10_Y));

	MUX2 mux_l3_in_2_ (
		.B(MUX2_5_Y),
		.A(MUX2_6_Y),
		.S0(sram[2]),
		.Y(MUX2_11_Y));

	MUX2 mux_l3_in_3_ (
		.B(MUX2_7_Y),
		.A(MUX2_8_Y),
		.S0(sram[2]),
		.Y(MUX2_12_Y));

	MUX2 mux_l4_in_0_ (
		.B(MUX2_9_Y),
		.A(MUX2_10_Y),
		.S0(sram[3]),
		.Y(MUX2_13_Y));

	MUX2 mux_l4_in_1_ (
		.B(MUX2_11_Y),
		.A(MUX2_12_Y),
		.S0(sram[3]),
		.Y(MUX2_14_Y));

	MUX2 mux_l5_in_0_ (
		.B(MUX2_13_Y),
		.A(MUX2_14_Y),
		.S0(sram[4]),
		.Y(MUX2_15_Y));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size16 -----




// ----- Verilog module for mux_2level_tapbuf_size40 -----
module mux_2level_tapbuf_size40(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [39:0] in;
//----- INPUT PORTS -----
input [5:0] sram;
//----- INPUT PORTS -----
input [5:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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
		.in(MUX2_39_Y),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l1_in_1_ (
		.B(INVTX1_2_out),
		.A(INVTX1_3_out),
		.S0(sram[0]),
		.Y(MUX2_1_Y));

	MUX2 mux_l1_in_2_ (
		.B(INVTX1_4_out),
		.A(INVTX1_5_out),
		.S0(sram[0]),
		.Y(MUX2_2_Y));

	MUX2 mux_l1_in_3_ (
		.B(INVTX1_6_out),
		.A(INVTX1_7_out),
		.S0(sram[0]),
		.Y(MUX2_3_Y));

	MUX2 mux_l1_in_4_ (
		.B(INVTX1_8_out),
		.A(INVTX1_9_out),
		.S0(sram[0]),
		.Y(MUX2_4_Y));

	MUX2 mux_l1_in_5_ (
		.B(INVTX1_10_out),
		.A(INVTX1_11_out),
		.S0(sram[0]),
		.Y(MUX2_5_Y));

	MUX2 mux_l1_in_6_ (
		.B(INVTX1_12_out),
		.A(INVTX1_13_out),
		.S0(sram[0]),
		.Y(MUX2_6_Y));

	MUX2 mux_l1_in_7_ (
		.B(INVTX1_14_out),
		.A(INVTX1_15_out),
		.S0(sram[0]),
		.Y(MUX2_7_Y));

	MUX2 mux_l1_in_8_ (
		.B(INVTX1_16_out),
		.A(INVTX1_17_out),
		.S0(sram[0]),
		.Y(MUX2_8_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(MUX2_1_Y),
		.S0(sram[1]),
		.Y(MUX2_9_Y));

	MUX2 mux_l2_in_1_ (
		.B(MUX2_2_Y),
		.A(MUX2_3_Y),
		.S0(sram[1]),
		.Y(MUX2_10_Y));

	MUX2 mux_l2_in_2_ (
		.B(MUX2_4_Y),
		.A(MUX2_5_Y),
		.S0(sram[1]),
		.Y(MUX2_11_Y));

	MUX2 mux_l2_in_3_ (
		.B(MUX2_6_Y),
		.A(MUX2_7_Y),
		.S0(sram[1]),
		.Y(MUX2_12_Y));

	MUX2 mux_l2_in_4_ (
		.B(MUX2_8_Y),
		.A(INVTX1_18_out),
		.S0(sram[1]),
		.Y(MUX2_13_Y));

	MUX2 mux_l2_in_5_ (
		.B(INVTX1_19_out),
		.A(INVTX1_20_out),
		.S0(sram[1]),
		.Y(MUX2_14_Y));

	MUX2 mux_l2_in_6_ (
		.B(INVTX1_21_out),
		.A(INVTX1_22_out),
		.S0(sram[1]),
		.Y(MUX2_15_Y));

	MUX2 mux_l2_in_7_ (
		.B(INVTX1_23_out),
		.A(INVTX1_24_out),
		.S0(sram[1]),
		.Y(MUX2_16_Y));

	MUX2 mux_l2_in_8_ (
		.B(INVTX1_25_out),
		.A(INVTX1_26_out),
		.S0(sram[1]),
		.Y(MUX2_17_Y));

	MUX2 mux_l2_in_9_ (
		.B(INVTX1_27_out),
		.A(INVTX1_28_out),
		.S0(sram[1]),
		.Y(MUX2_18_Y));

	MUX2 mux_l2_in_10_ (
		.B(INVTX1_29_out),
		.A(INVTX1_30_out),
		.S0(sram[1]),
		.Y(MUX2_19_Y));

	MUX2 mux_l2_in_11_ (
		.B(INVTX1_31_out),
		.A(INVTX1_32_out),
		.S0(sram[1]),
		.Y(MUX2_20_Y));

	MUX2 mux_l2_in_12_ (
		.B(INVTX1_33_out),
		.A(INVTX1_34_out),
		.S0(sram[1]),
		.Y(MUX2_21_Y));

	MUX2 mux_l2_in_13_ (
		.B(INVTX1_35_out),
		.A(INVTX1_36_out),
		.S0(sram[1]),
		.Y(MUX2_22_Y));

	MUX2 mux_l2_in_14_ (
		.B(INVTX1_37_out),
		.A(INVTX1_38_out),
		.S0(sram[1]),
		.Y(MUX2_23_Y));

	MUX2 mux_l2_in_15_ (
		.B(INVTX1_39_out),
		.A(const1_0_const1),
		.S0(sram[1]),
		.Y(MUX2_24_Y));

	MUX2 mux_l3_in_0_ (
		.B(MUX2_9_Y),
		.A(MUX2_10_Y),
		.S0(sram[2]),
		.Y(MUX2_25_Y));

	MUX2 mux_l3_in_1_ (
		.B(MUX2_11_Y),
		.A(MUX2_12_Y),
		.S0(sram[2]),
		.Y(MUX2_26_Y));

	MUX2 mux_l3_in_2_ (
		.B(MUX2_13_Y),
		.A(MUX2_14_Y),
		.S0(sram[2]),
		.Y(MUX2_27_Y));

	MUX2 mux_l3_in_3_ (
		.B(MUX2_15_Y),
		.A(MUX2_16_Y),
		.S0(sram[2]),
		.Y(MUX2_28_Y));

	MUX2 mux_l3_in_4_ (
		.B(MUX2_17_Y),
		.A(MUX2_18_Y),
		.S0(sram[2]),
		.Y(MUX2_29_Y));

	MUX2 mux_l3_in_5_ (
		.B(MUX2_19_Y),
		.A(MUX2_20_Y),
		.S0(sram[2]),
		.Y(MUX2_30_Y));

	MUX2 mux_l3_in_6_ (
		.B(MUX2_21_Y),
		.A(MUX2_22_Y),
		.S0(sram[2]),
		.Y(MUX2_31_Y));

	MUX2 mux_l3_in_7_ (
		.B(MUX2_23_Y),
		.A(MUX2_24_Y),
		.S0(sram[2]),
		.Y(MUX2_32_Y));

	MUX2 mux_l4_in_0_ (
		.B(MUX2_25_Y),
		.A(MUX2_26_Y),
		.S0(sram[3]),
		.Y(MUX2_33_Y));

	MUX2 mux_l4_in_1_ (
		.B(MUX2_27_Y),
		.A(MUX2_28_Y),
		.S0(sram[3]),
		.Y(MUX2_34_Y));

	MUX2 mux_l4_in_2_ (
		.B(MUX2_29_Y),
		.A(MUX2_30_Y),
		.S0(sram[3]),
		.Y(MUX2_35_Y));

	MUX2 mux_l4_in_3_ (
		.B(MUX2_31_Y),
		.A(MUX2_32_Y),
		.S0(sram[3]),
		.Y(MUX2_36_Y));

	MUX2 mux_l5_in_0_ (
		.B(MUX2_33_Y),
		.A(MUX2_34_Y),
		.S0(sram[4]),
		.Y(MUX2_37_Y));

	MUX2 mux_l5_in_1_ (
		.B(MUX2_35_Y),
		.A(MUX2_36_Y),
		.S0(sram[4]),
		.Y(MUX2_38_Y));

	MUX2 mux_l6_in_0_ (
		.B(MUX2_37_Y),
		.A(MUX2_38_Y),
		.S0(sram[5]),
		.Y(MUX2_39_Y));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size40 -----




// ----- Verilog module for mux_2level_tapbuf_size32 -----
module mux_2level_tapbuf_size32(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [31:0] in;
//----- INPUT PORTS -----
input [5:0] sram;
//----- INPUT PORTS -----
input [5:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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
		.in(MUX2_31_Y),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(INVTX1_2_out),
		.S0(sram[1]),
		.Y(MUX2_1_Y));

	MUX2 mux_l2_in_1_ (
		.B(INVTX1_3_out),
		.A(INVTX1_4_out),
		.S0(sram[1]),
		.Y(MUX2_2_Y));

	MUX2 mux_l2_in_2_ (
		.B(INVTX1_5_out),
		.A(INVTX1_6_out),
		.S0(sram[1]),
		.Y(MUX2_3_Y));

	MUX2 mux_l2_in_3_ (
		.B(INVTX1_7_out),
		.A(INVTX1_8_out),
		.S0(sram[1]),
		.Y(MUX2_4_Y));

	MUX2 mux_l2_in_4_ (
		.B(INVTX1_9_out),
		.A(INVTX1_10_out),
		.S0(sram[1]),
		.Y(MUX2_5_Y));

	MUX2 mux_l2_in_5_ (
		.B(INVTX1_11_out),
		.A(INVTX1_12_out),
		.S0(sram[1]),
		.Y(MUX2_6_Y));

	MUX2 mux_l2_in_6_ (
		.B(INVTX1_13_out),
		.A(INVTX1_14_out),
		.S0(sram[1]),
		.Y(MUX2_7_Y));

	MUX2 mux_l2_in_7_ (
		.B(INVTX1_15_out),
		.A(INVTX1_16_out),
		.S0(sram[1]),
		.Y(MUX2_8_Y));

	MUX2 mux_l2_in_8_ (
		.B(INVTX1_17_out),
		.A(INVTX1_18_out),
		.S0(sram[1]),
		.Y(MUX2_9_Y));

	MUX2 mux_l2_in_9_ (
		.B(INVTX1_19_out),
		.A(INVTX1_20_out),
		.S0(sram[1]),
		.Y(MUX2_10_Y));

	MUX2 mux_l2_in_10_ (
		.B(INVTX1_21_out),
		.A(INVTX1_22_out),
		.S0(sram[1]),
		.Y(MUX2_11_Y));

	MUX2 mux_l2_in_11_ (
		.B(INVTX1_23_out),
		.A(INVTX1_24_out),
		.S0(sram[1]),
		.Y(MUX2_12_Y));

	MUX2 mux_l2_in_12_ (
		.B(INVTX1_25_out),
		.A(INVTX1_26_out),
		.S0(sram[1]),
		.Y(MUX2_13_Y));

	MUX2 mux_l2_in_13_ (
		.B(INVTX1_27_out),
		.A(INVTX1_28_out),
		.S0(sram[1]),
		.Y(MUX2_14_Y));

	MUX2 mux_l2_in_14_ (
		.B(INVTX1_29_out),
		.A(INVTX1_30_out),
		.S0(sram[1]),
		.Y(MUX2_15_Y));

	MUX2 mux_l2_in_15_ (
		.B(INVTX1_31_out),
		.A(const1_0_const1),
		.S0(sram[1]),
		.Y(MUX2_16_Y));

	MUX2 mux_l3_in_0_ (
		.B(MUX2_1_Y),
		.A(MUX2_2_Y),
		.S0(sram[2]),
		.Y(MUX2_17_Y));

	MUX2 mux_l3_in_1_ (
		.B(MUX2_3_Y),
		.A(MUX2_4_Y),
		.S0(sram[2]),
		.Y(MUX2_18_Y));

	MUX2 mux_l3_in_2_ (
		.B(MUX2_5_Y),
		.A(MUX2_6_Y),
		.S0(sram[2]),
		.Y(MUX2_19_Y));

	MUX2 mux_l3_in_3_ (
		.B(MUX2_7_Y),
		.A(MUX2_8_Y),
		.S0(sram[2]),
		.Y(MUX2_20_Y));

	MUX2 mux_l3_in_4_ (
		.B(MUX2_9_Y),
		.A(MUX2_10_Y),
		.S0(sram[2]),
		.Y(MUX2_21_Y));

	MUX2 mux_l3_in_5_ (
		.B(MUX2_11_Y),
		.A(MUX2_12_Y),
		.S0(sram[2]),
		.Y(MUX2_22_Y));

	MUX2 mux_l3_in_6_ (
		.B(MUX2_13_Y),
		.A(MUX2_14_Y),
		.S0(sram[2]),
		.Y(MUX2_23_Y));

	MUX2 mux_l3_in_7_ (
		.B(MUX2_15_Y),
		.A(MUX2_16_Y),
		.S0(sram[2]),
		.Y(MUX2_24_Y));

	MUX2 mux_l4_in_0_ (
		.B(MUX2_17_Y),
		.A(MUX2_18_Y),
		.S0(sram[3]),
		.Y(MUX2_25_Y));

	MUX2 mux_l4_in_1_ (
		.B(MUX2_19_Y),
		.A(MUX2_20_Y),
		.S0(sram[3]),
		.Y(MUX2_26_Y));

	MUX2 mux_l4_in_2_ (
		.B(MUX2_21_Y),
		.A(MUX2_22_Y),
		.S0(sram[3]),
		.Y(MUX2_27_Y));

	MUX2 mux_l4_in_3_ (
		.B(MUX2_23_Y),
		.A(MUX2_24_Y),
		.S0(sram[3]),
		.Y(MUX2_28_Y));

	MUX2 mux_l5_in_0_ (
		.B(MUX2_25_Y),
		.A(MUX2_26_Y),
		.S0(sram[4]),
		.Y(MUX2_29_Y));

	MUX2 mux_l5_in_1_ (
		.B(MUX2_27_Y),
		.A(MUX2_28_Y),
		.S0(sram[4]),
		.Y(MUX2_30_Y));

	MUX2 mux_l6_in_0_ (
		.B(MUX2_29_Y),
		.A(MUX2_30_Y),
		.S0(sram[5]),
		.Y(MUX2_31_Y));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size32 -----




// ----- Verilog module for mux_2level_tapbuf_size48 -----
module mux_2level_tapbuf_size48(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [47:0] in;
//----- INPUT PORTS -----
input [5:0] sram;
//----- INPUT PORTS -----
input [5:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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
		.in(MUX2_47_Y),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l1_in_1_ (
		.B(INVTX1_2_out),
		.A(INVTX1_3_out),
		.S0(sram[0]),
		.Y(MUX2_1_Y));

	MUX2 mux_l1_in_2_ (
		.B(INVTX1_4_out),
		.A(INVTX1_5_out),
		.S0(sram[0]),
		.Y(MUX2_2_Y));

	MUX2 mux_l1_in_3_ (
		.B(INVTX1_6_out),
		.A(INVTX1_7_out),
		.S0(sram[0]),
		.Y(MUX2_3_Y));

	MUX2 mux_l1_in_4_ (
		.B(INVTX1_8_out),
		.A(INVTX1_9_out),
		.S0(sram[0]),
		.Y(MUX2_4_Y));

	MUX2 mux_l1_in_5_ (
		.B(INVTX1_10_out),
		.A(INVTX1_11_out),
		.S0(sram[0]),
		.Y(MUX2_5_Y));

	MUX2 mux_l1_in_6_ (
		.B(INVTX1_12_out),
		.A(INVTX1_13_out),
		.S0(sram[0]),
		.Y(MUX2_6_Y));

	MUX2 mux_l1_in_7_ (
		.B(INVTX1_14_out),
		.A(INVTX1_15_out),
		.S0(sram[0]),
		.Y(MUX2_7_Y));

	MUX2 mux_l1_in_8_ (
		.B(INVTX1_16_out),
		.A(INVTX1_17_out),
		.S0(sram[0]),
		.Y(MUX2_8_Y));

	MUX2 mux_l1_in_9_ (
		.B(INVTX1_18_out),
		.A(INVTX1_19_out),
		.S0(sram[0]),
		.Y(MUX2_9_Y));

	MUX2 mux_l1_in_10_ (
		.B(INVTX1_20_out),
		.A(INVTX1_21_out),
		.S0(sram[0]),
		.Y(MUX2_10_Y));

	MUX2 mux_l1_in_11_ (
		.B(INVTX1_22_out),
		.A(INVTX1_23_out),
		.S0(sram[0]),
		.Y(MUX2_11_Y));

	MUX2 mux_l1_in_12_ (
		.B(INVTX1_24_out),
		.A(INVTX1_25_out),
		.S0(sram[0]),
		.Y(MUX2_12_Y));

	MUX2 mux_l1_in_13_ (
		.B(INVTX1_26_out),
		.A(INVTX1_27_out),
		.S0(sram[0]),
		.Y(MUX2_13_Y));

	MUX2 mux_l1_in_14_ (
		.B(INVTX1_28_out),
		.A(INVTX1_29_out),
		.S0(sram[0]),
		.Y(MUX2_14_Y));

	MUX2 mux_l1_in_15_ (
		.B(INVTX1_30_out),
		.A(INVTX1_31_out),
		.S0(sram[0]),
		.Y(MUX2_15_Y));

	MUX2 mux_l1_in_16_ (
		.B(INVTX1_32_out),
		.A(INVTX1_33_out),
		.S0(sram[0]),
		.Y(MUX2_16_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(MUX2_1_Y),
		.S0(sram[1]),
		.Y(MUX2_17_Y));

	MUX2 mux_l2_in_1_ (
		.B(MUX2_2_Y),
		.A(MUX2_3_Y),
		.S0(sram[1]),
		.Y(MUX2_18_Y));

	MUX2 mux_l2_in_2_ (
		.B(MUX2_4_Y),
		.A(MUX2_5_Y),
		.S0(sram[1]),
		.Y(MUX2_19_Y));

	MUX2 mux_l2_in_3_ (
		.B(MUX2_6_Y),
		.A(MUX2_7_Y),
		.S0(sram[1]),
		.Y(MUX2_20_Y));

	MUX2 mux_l2_in_4_ (
		.B(MUX2_8_Y),
		.A(MUX2_9_Y),
		.S0(sram[1]),
		.Y(MUX2_21_Y));

	MUX2 mux_l2_in_5_ (
		.B(MUX2_10_Y),
		.A(MUX2_11_Y),
		.S0(sram[1]),
		.Y(MUX2_22_Y));

	MUX2 mux_l2_in_6_ (
		.B(MUX2_12_Y),
		.A(MUX2_13_Y),
		.S0(sram[1]),
		.Y(MUX2_23_Y));

	MUX2 mux_l2_in_7_ (
		.B(MUX2_14_Y),
		.A(MUX2_15_Y),
		.S0(sram[1]),
		.Y(MUX2_24_Y));

	MUX2 mux_l2_in_8_ (
		.B(MUX2_16_Y),
		.A(INVTX1_34_out),
		.S0(sram[1]),
		.Y(MUX2_25_Y));

	MUX2 mux_l2_in_9_ (
		.B(INVTX1_35_out),
		.A(INVTX1_36_out),
		.S0(sram[1]),
		.Y(MUX2_26_Y));

	MUX2 mux_l2_in_10_ (
		.B(INVTX1_37_out),
		.A(INVTX1_38_out),
		.S0(sram[1]),
		.Y(MUX2_27_Y));

	MUX2 mux_l2_in_11_ (
		.B(INVTX1_39_out),
		.A(INVTX1_40_out),
		.S0(sram[1]),
		.Y(MUX2_28_Y));

	MUX2 mux_l2_in_12_ (
		.B(INVTX1_41_out),
		.A(INVTX1_42_out),
		.S0(sram[1]),
		.Y(MUX2_29_Y));

	MUX2 mux_l2_in_13_ (
		.B(INVTX1_43_out),
		.A(INVTX1_44_out),
		.S0(sram[1]),
		.Y(MUX2_30_Y));

	MUX2 mux_l2_in_14_ (
		.B(INVTX1_45_out),
		.A(INVTX1_46_out),
		.S0(sram[1]),
		.Y(MUX2_31_Y));

	MUX2 mux_l2_in_15_ (
		.B(INVTX1_47_out),
		.A(const1_0_const1),
		.S0(sram[1]),
		.Y(MUX2_32_Y));

	MUX2 mux_l3_in_0_ (
		.B(MUX2_17_Y),
		.A(MUX2_18_Y),
		.S0(sram[2]),
		.Y(MUX2_33_Y));

	MUX2 mux_l3_in_1_ (
		.B(MUX2_19_Y),
		.A(MUX2_20_Y),
		.S0(sram[2]),
		.Y(MUX2_34_Y));

	MUX2 mux_l3_in_2_ (
		.B(MUX2_21_Y),
		.A(MUX2_22_Y),
		.S0(sram[2]),
		.Y(MUX2_35_Y));

	MUX2 mux_l3_in_3_ (
		.B(MUX2_23_Y),
		.A(MUX2_24_Y),
		.S0(sram[2]),
		.Y(MUX2_36_Y));

	MUX2 mux_l3_in_4_ (
		.B(MUX2_25_Y),
		.A(MUX2_26_Y),
		.S0(sram[2]),
		.Y(MUX2_37_Y));

	MUX2 mux_l3_in_5_ (
		.B(MUX2_27_Y),
		.A(MUX2_28_Y),
		.S0(sram[2]),
		.Y(MUX2_38_Y));

	MUX2 mux_l3_in_6_ (
		.B(MUX2_29_Y),
		.A(MUX2_30_Y),
		.S0(sram[2]),
		.Y(MUX2_39_Y));

	MUX2 mux_l3_in_7_ (
		.B(MUX2_31_Y),
		.A(MUX2_32_Y),
		.S0(sram[2]),
		.Y(MUX2_40_Y));

	MUX2 mux_l4_in_0_ (
		.B(MUX2_33_Y),
		.A(MUX2_34_Y),
		.S0(sram[3]),
		.Y(MUX2_41_Y));

	MUX2 mux_l4_in_1_ (
		.B(MUX2_35_Y),
		.A(MUX2_36_Y),
		.S0(sram[3]),
		.Y(MUX2_42_Y));

	MUX2 mux_l4_in_2_ (
		.B(MUX2_37_Y),
		.A(MUX2_38_Y),
		.S0(sram[3]),
		.Y(MUX2_43_Y));

	MUX2 mux_l4_in_3_ (
		.B(MUX2_39_Y),
		.A(MUX2_40_Y),
		.S0(sram[3]),
		.Y(MUX2_44_Y));

	MUX2 mux_l5_in_0_ (
		.B(MUX2_41_Y),
		.A(MUX2_42_Y),
		.S0(sram[4]),
		.Y(MUX2_45_Y));

	MUX2 mux_l5_in_1_ (
		.B(MUX2_43_Y),
		.A(MUX2_44_Y),
		.S0(sram[4]),
		.Y(MUX2_46_Y));

	MUX2 mux_l6_in_0_ (
		.B(MUX2_45_Y),
		.A(MUX2_46_Y),
		.S0(sram[5]),
		.Y(MUX2_47_Y));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size48 -----




// ----- Verilog module for mux_2level_tapbuf_size56 -----
module mux_2level_tapbuf_size56(in,
                                sram,
                                sram_inv,
                                out);
//----- INPUT PORTS -----
input [55:0] in;
//----- INPUT PORTS -----
input [5:0] sram;
//----- INPUT PORTS -----
input [5:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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

	INVTX1 INVTX1_56_ (
		.in(MUX2_55_Y),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l1_in_1_ (
		.B(INVTX1_2_out),
		.A(INVTX1_3_out),
		.S0(sram[0]),
		.Y(MUX2_1_Y));

	MUX2 mux_l1_in_2_ (
		.B(INVTX1_4_out),
		.A(INVTX1_5_out),
		.S0(sram[0]),
		.Y(MUX2_2_Y));

	MUX2 mux_l1_in_3_ (
		.B(INVTX1_6_out),
		.A(INVTX1_7_out),
		.S0(sram[0]),
		.Y(MUX2_3_Y));

	MUX2 mux_l1_in_4_ (
		.B(INVTX1_8_out),
		.A(INVTX1_9_out),
		.S0(sram[0]),
		.Y(MUX2_4_Y));

	MUX2 mux_l1_in_5_ (
		.B(INVTX1_10_out),
		.A(INVTX1_11_out),
		.S0(sram[0]),
		.Y(MUX2_5_Y));

	MUX2 mux_l1_in_6_ (
		.B(INVTX1_12_out),
		.A(INVTX1_13_out),
		.S0(sram[0]),
		.Y(MUX2_6_Y));

	MUX2 mux_l1_in_7_ (
		.B(INVTX1_14_out),
		.A(INVTX1_15_out),
		.S0(sram[0]),
		.Y(MUX2_7_Y));

	MUX2 mux_l1_in_8_ (
		.B(INVTX1_16_out),
		.A(INVTX1_17_out),
		.S0(sram[0]),
		.Y(MUX2_8_Y));

	MUX2 mux_l1_in_9_ (
		.B(INVTX1_18_out),
		.A(INVTX1_19_out),
		.S0(sram[0]),
		.Y(MUX2_9_Y));

	MUX2 mux_l1_in_10_ (
		.B(INVTX1_20_out),
		.A(INVTX1_21_out),
		.S0(sram[0]),
		.Y(MUX2_10_Y));

	MUX2 mux_l1_in_11_ (
		.B(INVTX1_22_out),
		.A(INVTX1_23_out),
		.S0(sram[0]),
		.Y(MUX2_11_Y));

	MUX2 mux_l1_in_12_ (
		.B(INVTX1_24_out),
		.A(INVTX1_25_out),
		.S0(sram[0]),
		.Y(MUX2_12_Y));

	MUX2 mux_l1_in_13_ (
		.B(INVTX1_26_out),
		.A(INVTX1_27_out),
		.S0(sram[0]),
		.Y(MUX2_13_Y));

	MUX2 mux_l1_in_14_ (
		.B(INVTX1_28_out),
		.A(INVTX1_29_out),
		.S0(sram[0]),
		.Y(MUX2_14_Y));

	MUX2 mux_l1_in_15_ (
		.B(INVTX1_30_out),
		.A(INVTX1_31_out),
		.S0(sram[0]),
		.Y(MUX2_15_Y));

	MUX2 mux_l1_in_16_ (
		.B(INVTX1_32_out),
		.A(INVTX1_33_out),
		.S0(sram[0]),
		.Y(MUX2_16_Y));

	MUX2 mux_l1_in_17_ (
		.B(INVTX1_34_out),
		.A(INVTX1_35_out),
		.S0(sram[0]),
		.Y(MUX2_17_Y));

	MUX2 mux_l1_in_18_ (
		.B(INVTX1_36_out),
		.A(INVTX1_37_out),
		.S0(sram[0]),
		.Y(MUX2_18_Y));

	MUX2 mux_l1_in_19_ (
		.B(INVTX1_38_out),
		.A(INVTX1_39_out),
		.S0(sram[0]),
		.Y(MUX2_19_Y));

	MUX2 mux_l1_in_20_ (
		.B(INVTX1_40_out),
		.A(INVTX1_41_out),
		.S0(sram[0]),
		.Y(MUX2_20_Y));

	MUX2 mux_l1_in_21_ (
		.B(INVTX1_42_out),
		.A(INVTX1_43_out),
		.S0(sram[0]),
		.Y(MUX2_21_Y));

	MUX2 mux_l1_in_22_ (
		.B(INVTX1_44_out),
		.A(INVTX1_45_out),
		.S0(sram[0]),
		.Y(MUX2_22_Y));

	MUX2 mux_l1_in_23_ (
		.B(INVTX1_46_out),
		.A(INVTX1_47_out),
		.S0(sram[0]),
		.Y(MUX2_23_Y));

	MUX2 mux_l1_in_24_ (
		.B(INVTX1_48_out),
		.A(INVTX1_49_out),
		.S0(sram[0]),
		.Y(MUX2_24_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(MUX2_1_Y),
		.S0(sram[1]),
		.Y(MUX2_25_Y));

	MUX2 mux_l2_in_1_ (
		.B(MUX2_2_Y),
		.A(MUX2_3_Y),
		.S0(sram[1]),
		.Y(MUX2_26_Y));

	MUX2 mux_l2_in_2_ (
		.B(MUX2_4_Y),
		.A(MUX2_5_Y),
		.S0(sram[1]),
		.Y(MUX2_27_Y));

	MUX2 mux_l2_in_3_ (
		.B(MUX2_6_Y),
		.A(MUX2_7_Y),
		.S0(sram[1]),
		.Y(MUX2_28_Y));

	MUX2 mux_l2_in_4_ (
		.B(MUX2_8_Y),
		.A(MUX2_9_Y),
		.S0(sram[1]),
		.Y(MUX2_29_Y));

	MUX2 mux_l2_in_5_ (
		.B(MUX2_10_Y),
		.A(MUX2_11_Y),
		.S0(sram[1]),
		.Y(MUX2_30_Y));

	MUX2 mux_l2_in_6_ (
		.B(MUX2_12_Y),
		.A(MUX2_13_Y),
		.S0(sram[1]),
		.Y(MUX2_31_Y));

	MUX2 mux_l2_in_7_ (
		.B(MUX2_14_Y),
		.A(MUX2_15_Y),
		.S0(sram[1]),
		.Y(MUX2_32_Y));

	MUX2 mux_l2_in_8_ (
		.B(MUX2_16_Y),
		.A(MUX2_17_Y),
		.S0(sram[1]),
		.Y(MUX2_33_Y));

	MUX2 mux_l2_in_9_ (
		.B(MUX2_18_Y),
		.A(MUX2_19_Y),
		.S0(sram[1]),
		.Y(MUX2_34_Y));

	MUX2 mux_l2_in_10_ (
		.B(MUX2_20_Y),
		.A(MUX2_21_Y),
		.S0(sram[1]),
		.Y(MUX2_35_Y));

	MUX2 mux_l2_in_11_ (
		.B(MUX2_22_Y),
		.A(MUX2_23_Y),
		.S0(sram[1]),
		.Y(MUX2_36_Y));

	MUX2 mux_l2_in_12_ (
		.B(MUX2_24_Y),
		.A(INVTX1_50_out),
		.S0(sram[1]),
		.Y(MUX2_37_Y));

	MUX2 mux_l2_in_13_ (
		.B(INVTX1_51_out),
		.A(INVTX1_52_out),
		.S0(sram[1]),
		.Y(MUX2_38_Y));

	MUX2 mux_l2_in_14_ (
		.B(INVTX1_53_out),
		.A(INVTX1_54_out),
		.S0(sram[1]),
		.Y(MUX2_39_Y));

	MUX2 mux_l2_in_15_ (
		.B(INVTX1_55_out),
		.A(const1_0_const1),
		.S0(sram[1]),
		.Y(MUX2_40_Y));

	MUX2 mux_l3_in_0_ (
		.B(MUX2_25_Y),
		.A(MUX2_26_Y),
		.S0(sram[2]),
		.Y(MUX2_41_Y));

	MUX2 mux_l3_in_1_ (
		.B(MUX2_27_Y),
		.A(MUX2_28_Y),
		.S0(sram[2]),
		.Y(MUX2_42_Y));

	MUX2 mux_l3_in_2_ (
		.B(MUX2_29_Y),
		.A(MUX2_30_Y),
		.S0(sram[2]),
		.Y(MUX2_43_Y));

	MUX2 mux_l3_in_3_ (
		.B(MUX2_31_Y),
		.A(MUX2_32_Y),
		.S0(sram[2]),
		.Y(MUX2_44_Y));

	MUX2 mux_l3_in_4_ (
		.B(MUX2_33_Y),
		.A(MUX2_34_Y),
		.S0(sram[2]),
		.Y(MUX2_45_Y));

	MUX2 mux_l3_in_5_ (
		.B(MUX2_35_Y),
		.A(MUX2_36_Y),
		.S0(sram[2]),
		.Y(MUX2_46_Y));

	MUX2 mux_l3_in_6_ (
		.B(MUX2_37_Y),
		.A(MUX2_38_Y),
		.S0(sram[2]),
		.Y(MUX2_47_Y));

	MUX2 mux_l3_in_7_ (
		.B(MUX2_39_Y),
		.A(MUX2_40_Y),
		.S0(sram[2]),
		.Y(MUX2_48_Y));

	MUX2 mux_l4_in_0_ (
		.B(MUX2_41_Y),
		.A(MUX2_42_Y),
		.S0(sram[3]),
		.Y(MUX2_49_Y));

	MUX2 mux_l4_in_1_ (
		.B(MUX2_43_Y),
		.A(MUX2_44_Y),
		.S0(sram[3]),
		.Y(MUX2_50_Y));

	MUX2 mux_l4_in_2_ (
		.B(MUX2_45_Y),
		.A(MUX2_46_Y),
		.S0(sram[3]),
		.Y(MUX2_51_Y));

	MUX2 mux_l4_in_3_ (
		.B(MUX2_47_Y),
		.A(MUX2_48_Y),
		.S0(sram[3]),
		.Y(MUX2_52_Y));

	MUX2 mux_l5_in_0_ (
		.B(MUX2_49_Y),
		.A(MUX2_50_Y),
		.S0(sram[4]),
		.Y(MUX2_53_Y));

	MUX2 mux_l5_in_1_ (
		.B(MUX2_51_Y),
		.A(MUX2_52_Y),
		.S0(sram[4]),
		.Y(MUX2_54_Y));

	MUX2 mux_l6_in_0_ (
		.B(MUX2_53_Y),
		.A(MUX2_54_Y),
		.S0(sram[5]),
		.Y(MUX2_55_Y));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size56 -----




// ----- Verilog module for mux_2level_size20 -----
module mux_2level_size20(in,
                         sram,
                         sram_inv,
                         out);
//----- INPUT PORTS -----
input [19:0] in;
//----- INPUT PORTS -----
input [4:0] sram;
//----- INPUT PORTS -----
input [4:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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
		.in(MUX2_19_Y),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l1_in_1_ (
		.B(INVTX1_2_out),
		.A(INVTX1_3_out),
		.S0(sram[0]),
		.Y(MUX2_1_Y));

	MUX2 mux_l1_in_2_ (
		.B(INVTX1_4_out),
		.A(INVTX1_5_out),
		.S0(sram[0]),
		.Y(MUX2_2_Y));

	MUX2 mux_l1_in_3_ (
		.B(INVTX1_6_out),
		.A(INVTX1_7_out),
		.S0(sram[0]),
		.Y(MUX2_3_Y));

	MUX2 mux_l1_in_4_ (
		.B(INVTX1_8_out),
		.A(INVTX1_9_out),
		.S0(sram[0]),
		.Y(MUX2_4_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(MUX2_1_Y),
		.S0(sram[1]),
		.Y(MUX2_5_Y));

	MUX2 mux_l2_in_1_ (
		.B(MUX2_2_Y),
		.A(MUX2_3_Y),
		.S0(sram[1]),
		.Y(MUX2_6_Y));

	MUX2 mux_l2_in_2_ (
		.B(MUX2_4_Y),
		.A(INVTX1_10_out),
		.S0(sram[1]),
		.Y(MUX2_7_Y));

	MUX2 mux_l2_in_3_ (
		.B(INVTX1_11_out),
		.A(INVTX1_12_out),
		.S0(sram[1]),
		.Y(MUX2_8_Y));

	MUX2 mux_l2_in_4_ (
		.B(INVTX1_13_out),
		.A(INVTX1_14_out),
		.S0(sram[1]),
		.Y(MUX2_9_Y));

	MUX2 mux_l2_in_5_ (
		.B(INVTX1_15_out),
		.A(INVTX1_16_out),
		.S0(sram[1]),
		.Y(MUX2_10_Y));

	MUX2 mux_l2_in_6_ (
		.B(INVTX1_17_out),
		.A(INVTX1_18_out),
		.S0(sram[1]),
		.Y(MUX2_11_Y));

	MUX2 mux_l2_in_7_ (
		.B(INVTX1_19_out),
		.A(const1_0_const1),
		.S0(sram[1]),
		.Y(MUX2_12_Y));

	MUX2 mux_l3_in_0_ (
		.B(MUX2_5_Y),
		.A(MUX2_6_Y),
		.S0(sram[2]),
		.Y(MUX2_13_Y));

	MUX2 mux_l3_in_1_ (
		.B(MUX2_7_Y),
		.A(MUX2_8_Y),
		.S0(sram[2]),
		.Y(MUX2_14_Y));

	MUX2 mux_l3_in_2_ (
		.B(MUX2_9_Y),
		.A(MUX2_10_Y),
		.S0(sram[2]),
		.Y(MUX2_15_Y));

	MUX2 mux_l3_in_3_ (
		.B(MUX2_11_Y),
		.A(MUX2_12_Y),
		.S0(sram[2]),
		.Y(MUX2_16_Y));

	MUX2 mux_l4_in_0_ (
		.B(MUX2_13_Y),
		.A(MUX2_14_Y),
		.S0(sram[3]),
		.Y(MUX2_17_Y));

	MUX2 mux_l4_in_1_ (
		.B(MUX2_15_Y),
		.A(MUX2_16_Y),
		.S0(sram[3]),
		.Y(MUX2_18_Y));

	MUX2 mux_l5_in_0_ (
		.B(MUX2_17_Y),
		.A(MUX2_18_Y),
		.S0(sram[4]),
		.Y(MUX2_19_Y));

endmodule
// ----- END Verilog module for mux_2level_size20 -----




// ----- Verilog module for mux_1level_tapbuf_size2 -----
module mux_1level_tapbuf_size2(in,
                               sram,
                               sram_inv,
                               out);
//----- INPUT PORTS -----
input [1:0] in;
//----- INPUT PORTS -----
input [1:0] sram;
//----- INPUT PORTS -----
input [1:0] sram_inv;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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
		.in(MUX2_1_Y),
		.out(out));

	const1 const1_0_ (
		.const1(const1_0_const1));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(const1_0_const1),
		.S0(sram[1]),
		.Y(MUX2_1_Y));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2 -----




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

//----- BEGIN Registered ports -----
//----- END Registered ports -----



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
		.in(MUX2_12_Y),
		.out(lut3_out[0]));

	INVTX1 INVTX1_17_ (
		.in(MUX2_13_Y),
		.out(lut3_out[1]));

	INVTX1 INVTX1_18_ (
		.in(MUX2_14_Y),
		.out(lut4_out));

	MUX2 mux_l1_in_0_ (
		.B(INVTX1_0_out),
		.A(INVTX1_1_out),
		.S0(sram[0]),
		.Y(MUX2_0_Y));

	MUX2 mux_l1_in_1_ (
		.B(INVTX1_2_out),
		.A(INVTX1_3_out),
		.S0(sram[0]),
		.Y(MUX2_1_Y));

	MUX2 mux_l1_in_2_ (
		.B(INVTX1_4_out),
		.A(INVTX1_5_out),
		.S0(sram[0]),
		.Y(MUX2_2_Y));

	MUX2 mux_l1_in_3_ (
		.B(INVTX1_6_out),
		.A(INVTX1_7_out),
		.S0(sram[0]),
		.Y(MUX2_3_Y));

	MUX2 mux_l1_in_4_ (
		.B(INVTX1_8_out),
		.A(INVTX1_9_out),
		.S0(sram[0]),
		.Y(MUX2_4_Y));

	MUX2 mux_l1_in_5_ (
		.B(INVTX1_10_out),
		.A(INVTX1_11_out),
		.S0(sram[0]),
		.Y(MUX2_5_Y));

	MUX2 mux_l1_in_6_ (
		.B(INVTX1_12_out),
		.A(INVTX1_13_out),
		.S0(sram[0]),
		.Y(MUX2_6_Y));

	MUX2 mux_l1_in_7_ (
		.B(INVTX1_14_out),
		.A(INVTX1_15_out),
		.S0(sram[0]),
		.Y(MUX2_7_Y));

	MUX2 mux_l2_in_0_ (
		.B(MUX2_0_Y),
		.A(MUX2_1_Y),
		.S0(sram[1]),
		.Y(MUX2_8_Y));

	MUX2 mux_l2_in_1_ (
		.B(MUX2_2_Y),
		.A(MUX2_3_Y),
		.S0(sram[1]),
		.Y(MUX2_9_Y));

	MUX2 mux_l2_in_2_ (
		.B(MUX2_4_Y),
		.A(MUX2_5_Y),
		.S0(sram[1]),
		.Y(MUX2_10_Y));

	MUX2 mux_l2_in_3_ (
		.B(MUX2_6_Y),
		.A(MUX2_7_Y),
		.S0(sram[1]),
		.Y(MUX2_11_Y));

	MUX2 mux_l3_in_0_ (
		.B(buf4_0_out),
		.A(buf4_1_out),
		.S0(sram[2]),
		.Y(MUX2_12_Y));

	MUX2 mux_l3_in_1_ (
		.B(buf4_2_out),
		.A(buf4_3_out),
		.S0(sram[2]),
		.Y(MUX2_13_Y));

	MUX2 mux_l4_in_0_ (
		.B(MUX2_12_Y),
		.A(MUX2_13_Y),
		.S0(sram[3]),
		.Y(MUX2_14_Y));

	buf4 buf4_0_ (
		.in(MUX2_8_Y),
		.out(buf4_0_out));

	buf4 buf4_1_ (
		.in(MUX2_9_Y),
		.out(buf4_1_out));

	buf4 buf4_2_ (
		.in(MUX2_10_Y),
		.out(buf4_2_out));

	buf4 buf4_3_ (
		.in(MUX2_11_Y),
		.out(buf4_3_out));

endmodule
// ----- END Verilog module for frac_lut4_mux -----




