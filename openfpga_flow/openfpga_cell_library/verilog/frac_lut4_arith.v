//-----------------------------------------------------
// Design Name : frac_lut4_arith
// File Name   : frac_lut4_arith.v
// Function    : 4-input Look Up Table with integrated carry logic
//               - mode_bit[0] switch between arithmetic mode and LUT mode
//               - mode_bit[1] switch between regular LUT mode and fracturable
//               mode
// Note        : The HDL is a technology mapped netlist based on the Skywater
//               130nm High-Density cell library. 
//               TODO: Create a behavioral HDL version so that we are portable
//               between PDKs
// Coder       : Xifan TANG
//-----------------------------------------------------
module frac_lut4_arith (
input [0:3] in,
input [0:0] cin,
output [0:1] lut3_out,
output [0:0] lut4_out,
output [0:0] cout,
input [0:15] sram,
input [0:1] mode);

//----- BEGIN wire-connection ports -----
wire [0:3] in;
wire [0:0] cin;
wire [0:1] lut2_out;
wire [0:1] lut3_out;
wire [0:0] lut4_out;
wire [0:0] cout;
wire [0:0] arith_in2;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] sky130_fd_sc_hd__buf_2_0_X;
wire [0:0] sky130_fd_sc_hd__buf_2_1_X;
wire [0:0] sky130_fd_sc_hd__buf_2_2_X;
wire [0:0] sky130_fd_sc_hd__buf_2_3_X;
wire [0:0] sky130_fd_sc_hd__inv_1_0_Y;
wire [0:0] sky130_fd_sc_hd__inv_1_1_Y;
wire [0:0] sky130_fd_sc_hd__inv_1_2_Y;
wire [0:0] sky130_fd_sc_hd__inv_1_3_Y;
wire [0:0] sky130_fd_sc_hd__or2_1_0_X;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	sky130_fd_sc_hd__or2_1 sky130_fd_sc_hd__or2_1_0_ (
		.A(mode[1]),
		.B(in[3]),
		.X(sky130_fd_sc_hd__or2_1_0_X[0]));

	sky130_fd_sc_hd__inv_1 sky130_fd_sc_hd__inv_1_0_ (
		.A(in[0]),
		.Y(sky130_fd_sc_hd__inv_1_0_Y[0]));

	sky130_fd_sc_hd__inv_1 sky130_fd_sc_hd__inv_1_1_ (
		.A(in[1]),
		.Y(sky130_fd_sc_hd__inv_1_1_Y[0]));

  assign arith_in2 = mode[0] ? in[2] : cin;

	sky130_fd_sc_hd__inv_1 sky130_fd_sc_hd__inv_1_2_ (
		.A(arith_in2),
		.Y(sky130_fd_sc_hd__inv_1_2_Y[0]));

	sky130_fd_sc_hd__inv_1 sky130_fd_sc_hd__inv_1_3_ (
		.A(sky130_fd_sc_hd__or2_1_0_X[0]),
		.Y(sky130_fd_sc_hd__inv_1_3_Y[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_0_ (
		.A(in[0]),
		.X(sky130_fd_sc_hd__buf_2_0_X[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_1_ (
		.A(in[1]),
		.X(sky130_fd_sc_hd__buf_2_1_X[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_2_ (
		.A(in[2]),
		.X(sky130_fd_sc_hd__buf_2_2_X[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_3_ (
		.A(sky130_fd_sc_hd__or2_1_0_X[0]),
		.X(sky130_fd_sc_hd__buf_2_3_X[0]));

	frac_lut4_mux frac_lut4_mux_0_ (
		.in(sram[0:15]),
		.sram({sky130_fd_sc_hd__buf_2_0_X[0], sky130_fd_sc_hd__buf_2_1_X[0], sky130_fd_sc_hd__buf_2_2_X[0], sky130_fd_sc_hd__buf_2_3_X[0]}),
		.sram_inv({sky130_fd_sc_hd__inv_1_0_Y[0], sky130_fd_sc_hd__inv_1_1_Y[0], sky130_fd_sc_hd__inv_1_2_Y[0], sky130_fd_sc_hd__inv_1_3_Y[0]}),
		.lut2_out(lut2_out[0:1]),
		.lut3_out(lut3_out[0:1]),
		.lut4_out(lut4_out[0]));
 
    assign cout = lut2_out[0] ? cin : lut2_out[1];

endmodule

// ----- Verilog module for frac_lut4_mux -----
module frac_lut4_mux(in,
                     sram,
                     sram_inv,
                     lut2_out,
                     lut3_out,
                     lut4_out);
//----- INPUT PORTS -----
input [0:15] in;
//----- INPUT PORTS -----
input [0:3] sram;
//----- INPUT PORTS -----
input [0:3] sram_inv;
//----- OUTPUT PORTS -----
output [0:1] lut2_out;
//----- OUTPUT PORTS -----
output [0:1] lut3_out;
//----- OUTPUT PORTS -----
output [0:0] lut4_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] sky130_fd_sc_hd__buf_2_5_X;
wire [0:0] sky130_fd_sc_hd__buf_2_6_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_0_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_10_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_11_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_12_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_13_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_14_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_1_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_2_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_3_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_4_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_5_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_6_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_7_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_8_X;
wire [0:0] sky130_fd_sc_hd__mux2_1_9_X;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_0_ (
		.A(sky130_fd_sc_hd__mux2_1_10_X[0]),
		.X(lut2_out[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_1_ (
		.A(sky130_fd_sc_hd__mux2_1_11_X[0]),
		.X(lut2_out[1]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_2_ (
		.A(sky130_fd_sc_hd__mux2_1_12_X[0]),
		.X(lut3_out[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_3_ (
		.A(sky130_fd_sc_hd__mux2_1_13_X[0]),
		.X(lut3_out[1]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_4_ (
		.A(sky130_fd_sc_hd__mux2_1_14_X[0]),
		.X(lut4_out[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_5_ (
		.A(sky130_fd_sc_hd__mux2_1_8_X[0]),
		.X(sky130_fd_sc_hd__buf_2_5_X[0]));

	sky130_fd_sc_hd__buf_2 sky130_fd_sc_hd__buf_2_6_ (
		.A(sky130_fd_sc_hd__mux2_1_9_X[0]),
		.X(sky130_fd_sc_hd__buf_2_6_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_0_ (
		.A1(in[0]),
		.A0(in[1]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_0_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_1_ (
		.A1(in[2]),
		.A0(in[3]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_1_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_2_ (
		.A1(in[4]),
		.A0(in[5]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_2_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_3_ (
		.A1(in[6]),
		.A0(in[7]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_3_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_4_ (
		.A1(in[8]),
		.A0(in[9]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_4_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_5_ (
		.A1(in[10]),
		.A0(in[11]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_5_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_6_ (
		.A1(in[12]),
		.A0(in[13]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_6_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l1_in_7_ (
		.A1(in[14]),
		.A0(in[15]),
		.S(sram[0]),
		.X(sky130_fd_sc_hd__mux2_1_7_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l2_in_0_ (
		.A1(sky130_fd_sc_hd__mux2_1_0_X[0]),
		.A0(sky130_fd_sc_hd__mux2_1_1_X[0]),
		.S(sram[1]),
		.X(sky130_fd_sc_hd__mux2_1_8_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l2_in_1_ (
		.A1(sky130_fd_sc_hd__mux2_1_2_X[0]),
		.A0(sky130_fd_sc_hd__mux2_1_3_X[0]),
		.S(sram[1]),
		.X(sky130_fd_sc_hd__mux2_1_9_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l2_in_2_ (
		.A1(sky130_fd_sc_hd__mux2_1_4_X[0]),
		.A0(sky130_fd_sc_hd__mux2_1_5_X[0]),
		.S(sram[1]),
		.X(sky130_fd_sc_hd__mux2_1_10_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l2_in_3_ (
		.A1(sky130_fd_sc_hd__mux2_1_6_X[0]),
		.A0(sky130_fd_sc_hd__mux2_1_7_X[0]),
		.S(sram[1]),
		.X(sky130_fd_sc_hd__mux2_1_11_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l3_in_0_ (
		.A1(sky130_fd_sc_hd__buf_2_5_X[0]),
		.A0(sky130_fd_sc_hd__buf_2_6_X[0]),
		.S(sram[2]),
		.X(sky130_fd_sc_hd__mux2_1_12_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l3_in_1_ (
		.A1(sky130_fd_sc_hd__mux2_1_10_X[0]),
		.A0(sky130_fd_sc_hd__mux2_1_11_X[0]),
		.S(sram[2]),
		.X(sky130_fd_sc_hd__mux2_1_13_X[0]));

	sky130_fd_sc_hd__mux2_1 mux_l4_in_0_ (
		.A1(sky130_fd_sc_hd__mux2_1_12_X[0]),
		.A0(sky130_fd_sc_hd__mux2_1_13_X[0]),
		.S(sram[3]),
		.X(sky130_fd_sc_hd__mux2_1_14_X[0]));

endmodule
// ----- END Verilog module for frac_lut4_mux -----
