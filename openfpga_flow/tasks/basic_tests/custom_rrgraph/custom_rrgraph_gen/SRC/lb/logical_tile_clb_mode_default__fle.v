//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: fle
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: fle -----
// ----- Verilog module for logical_tile_clb_mode_default__fle -----
module logical_tile_clb_mode_default__fle(pReset,
                                          prog_clk,
                                          set,
                                          reset,
                                          clk,
                                          fle_in,
                                          fle_clk,
                                          ccff_head,
                                          fle_out,
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
input [3:0] fle_in;
//----- INPUT PORTS -----
input [0:0] fle_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [1:0] fle_out;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [1:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle_mode_physical__fabric logical_tile_clb_mode_default__fle_mode_physical__fabric_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.set(set),
		.reset(reset),
		.clk(clk),
		.fabric_in({direct_interc_5_out, direct_interc_4_out, direct_interc_3_out, direct_interc_2_out}),
		.fabric_clk(direct_interc_6_out),
		.ccff_head(ccff_head),
		.fabric_out({logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out[1], logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out[0]}),
		.ccff_tail(ccff_tail));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out[0]),
		.out(fle_out[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out[1]),
		.out(fle_out[1]));

	direct_interc direct_interc_2_ (
		.in(fle_in[0]),
		.out(direct_interc_2_out));

	direct_interc direct_interc_3_ (
		.in(fle_in[1]),
		.out(direct_interc_3_out));

	direct_interc direct_interc_4_ (
		.in(fle_in[2]),
		.out(direct_interc_4_out));

	direct_interc direct_interc_5_ (
		.in(fle_in[3]),
		.out(direct_interc_5_out));

	direct_interc direct_interc_6_ (
		.in(fle_clk),
		.out(direct_interc_6_out));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle -----



// ----- END Physical programmable logic block Verilog module: fle -----
