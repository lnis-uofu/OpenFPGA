//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: fle
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- BEGIN Physical programmable logic block Verilog module: fle -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_default__fle -----
module logical_tile_clb_mode_default__fle(prog_clk,
                                          set,
                                          reset,
                                          clk,
                                          fle_in,
                                          fle_clk,
                                          ccff_head,
                                          fle_out,
                                          ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GLOBAL PORTS -----
input [0:0] set;
//----- GLOBAL PORTS -----
input [0:0] reset;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:3] fle_in;
//----- INPUT PORTS -----
input [0:0] fle_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] fle_out;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:3] fle_in;
wire [0:0] fle_clk;
wire [0:0] fle_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_1_out;
wire [0:0] direct_interc_2_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0_ble4_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4 logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0 (
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.ble4_in({direct_interc_1_out, direct_interc_2_out, direct_interc_3_out, direct_interc_4_out}),
		.ble4_clk(direct_interc_5_out),
		.ccff_head(ccff_head),
		.ble4_out(logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0_ble4_out),
		.ccff_tail(ccff_tail));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0_ble4_out),
		.out(fle_out));

	direct_interc direct_interc_1_ (
		.in(fle_in[0]),
		.out(direct_interc_1_out));

	direct_interc direct_interc_2_ (
		.in(fle_in[1]),
		.out(direct_interc_2_out));

	direct_interc direct_interc_3_ (
		.in(fle_in[2]),
		.out(direct_interc_3_out));

	direct_interc direct_interc_4_ (
		.in(fle_in[3]),
		.out(direct_interc_4_out));

	direct_interc direct_interc_5_ (
		.in(fle_clk),
		.out(direct_interc_5_out));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle -----

//----- Default net type -----
`default_nettype none



// ----- END Physical programmable logic block Verilog module: fle -----
