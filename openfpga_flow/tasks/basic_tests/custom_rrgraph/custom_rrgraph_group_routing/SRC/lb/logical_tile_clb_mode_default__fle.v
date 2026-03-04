//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: fle
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: fle -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_default__fle -----
module logical_tile_clb_mode_default__fle(set,
                                          reset,
                                          clk,
                                          fle_in,
                                          fle_clk,
                                          feedthrough_mem_in,
                                          feedthrough_mem_inb,
                                          fle_out);
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
input [25:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [25:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [1:0] fle_out;

//----- BEGIN wire-connection ports -----
wire [3:0] fle_in;
wire [0:0] fle_clk;
wire [1:0] fle_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_2_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [1:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle_mode_physical__fabric logical_tile_clb_mode_default__fle_mode_physical__fabric_0 (
		.set(set),
		.reset(reset),
		.clk(clk),
		.fabric_in({direct_interc_5_out, direct_interc_4_out, direct_interc_3_out, direct_interc_2_out}),
		.fabric_clk(direct_interc_6_out),
		.feedthrough_mem_in({feedthrough_mem_in[25], feedthrough_mem_in[24], feedthrough_mem_in[23], feedthrough_mem_in[22], feedthrough_mem_in[21], feedthrough_mem_in[20], feedthrough_mem_in[19], feedthrough_mem_in[18], feedthrough_mem_in[17], feedthrough_mem_in[16], feedthrough_mem_in[15], feedthrough_mem_in[14], feedthrough_mem_in[13], feedthrough_mem_in[12], feedthrough_mem_in[11], feedthrough_mem_in[10], feedthrough_mem_in[9], feedthrough_mem_in[8], feedthrough_mem_in[7], feedthrough_mem_in[6], feedthrough_mem_in[5], feedthrough_mem_in[4], feedthrough_mem_in[3], feedthrough_mem_in[2], feedthrough_mem_in[1], feedthrough_mem_in[0]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[25], feedthrough_mem_inb[24], feedthrough_mem_inb[23], feedthrough_mem_inb[22], feedthrough_mem_inb[21], feedthrough_mem_inb[20], feedthrough_mem_inb[19], feedthrough_mem_inb[18], feedthrough_mem_inb[17], feedthrough_mem_inb[16], feedthrough_mem_inb[15], feedthrough_mem_inb[14], feedthrough_mem_inb[13], feedthrough_mem_inb[12], feedthrough_mem_inb[11], feedthrough_mem_inb[10], feedthrough_mem_inb[9], feedthrough_mem_inb[8], feedthrough_mem_inb[7], feedthrough_mem_inb[6], feedthrough_mem_inb[5], feedthrough_mem_inb[4], feedthrough_mem_inb[3], feedthrough_mem_inb[2], feedthrough_mem_inb[1], feedthrough_mem_inb[0]}),
		.fabric_out({logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out[1], logical_tile_clb_mode_default__fle_mode_physical__fabric_0_fabric_out[0]}));

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

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: fle -----
