//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: fabric
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: fabric -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric -----
module logical_tile_clb_mode_default__fle_mode_physical__fabric(set,
                                                                reset,
                                                                fabric_in,
                                                                fabric_clk,
                                                                feedthrough_mem_in,
                                                                feedthrough_mem_inb,
                                                                fabric_out);
//----- GLOBAL PORTS -----
input [0:0] set;
//----- GLOBAL PORTS -----
input [0:0] reset;
//----- INPUT PORTS -----
input [3:0] fabric_in;
//----- INPUT PORTS -----
input [0:0] fabric_clk;
//----- INPUT PORTS -----
input [25:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [25:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [1:0] fabric_out;

//----- BEGIN wire-connection ports -----
wire [3:0] fabric_in;
wire [0:0] fabric_clk;
wire [1:0] fabric_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_0_out;
wire [0:0] direct_interc_1_out;
wire [0:0] direct_interc_2_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [0:0] direct_interc_7_out;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0_ff_Q;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1_ff_Q;
wire [1:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out;
wire [2:0] mux_1level_tapbuf_size2_0_sram;
wire [2:0] mux_1level_tapbuf_size2_0_sram_inv;
wire [2:0] mux_1level_tapbuf_size2_1_sram;
wire [2:0] mux_1level_tapbuf_size2_1_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0 (
		.frac_logic_in({direct_interc_3_out, direct_interc_2_out, direct_interc_1_out, direct_interc_0_out}),
		.feedthrough_mem_in({feedthrough_mem_in[19], feedthrough_mem_in[18], feedthrough_mem_in[17], feedthrough_mem_in[16], feedthrough_mem_in[15], feedthrough_mem_in[14], feedthrough_mem_in[13], feedthrough_mem_in[12], feedthrough_mem_in[11], feedthrough_mem_in[10], feedthrough_mem_in[9], feedthrough_mem_in[8], feedthrough_mem_in[7], feedthrough_mem_in[6], feedthrough_mem_in[5], feedthrough_mem_in[4], feedthrough_mem_in[3], feedthrough_mem_in[2], feedthrough_mem_in[1], feedthrough_mem_in[0]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[19], feedthrough_mem_inb[18], feedthrough_mem_inb[17], feedthrough_mem_inb[16], feedthrough_mem_inb[15], feedthrough_mem_inb[14], feedthrough_mem_inb[13], feedthrough_mem_inb[12], feedthrough_mem_inb[11], feedthrough_mem_inb[10], feedthrough_mem_inb[9], feedthrough_mem_inb[8], feedthrough_mem_inb[7], feedthrough_mem_inb[6], feedthrough_mem_inb[5], feedthrough_mem_inb[4], feedthrough_mem_inb[3], feedthrough_mem_inb[2], feedthrough_mem_inb[1], feedthrough_mem_inb[0]}),
		.frac_logic_out({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[1], logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[0]}));

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0 (
		.set(set),
		.reset(reset),
		.ff_D(direct_interc_4_out),
		.ff_Q(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0_ff_Q),
		.ff_clk(direct_interc_5_out));

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1 (
		.set(set),
		.reset(reset),
		.ff_D(direct_interc_6_out),
		.ff_Q(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1_ff_Q),
		.ff_clk(direct_interc_7_out));

	mux_1level_tapbuf_size2 mux_fabric_out_0 (
		.in({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[0], logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0_ff_Q}),
		.sram({mux_1level_tapbuf_size2_0_sram[2], mux_1level_tapbuf_size2_0_sram[1], mux_1level_tapbuf_size2_0_sram[0]}),
		.sram_inv({mux_1level_tapbuf_size2_0_sram_inv[2], mux_1level_tapbuf_size2_0_sram_inv[1], mux_1level_tapbuf_size2_0_sram_inv[0]}),
		.out(fabric_out[0]));

	mux_1level_tapbuf_size2 mux_fabric_out_1 (
		.in({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[1], logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1_ff_Q}),
		.sram({mux_1level_tapbuf_size2_1_sram[2], mux_1level_tapbuf_size2_1_sram[1], mux_1level_tapbuf_size2_1_sram[0]}),
		.sram_inv({mux_1level_tapbuf_size2_1_sram_inv[2], mux_1level_tapbuf_size2_1_sram_inv[1], mux_1level_tapbuf_size2_1_sram_inv[0]}),
		.out(fabric_out[1]));

	mux_1level_tapbuf_size2_feedthrough_mem virtual_mem_fabric_out_0 (
		.feedthrough_mem_in({feedthrough_mem_in[22], feedthrough_mem_in[21], feedthrough_mem_in[20]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[22], feedthrough_mem_inb[21], feedthrough_mem_inb[20]}),
		.mem_out({mux_1level_tapbuf_size2_0_sram[2], mux_1level_tapbuf_size2_0_sram[1], mux_1level_tapbuf_size2_0_sram[0]}),
		.mem_outb({mux_1level_tapbuf_size2_0_sram_inv[2], mux_1level_tapbuf_size2_0_sram_inv[1], mux_1level_tapbuf_size2_0_sram_inv[0]}));

	mux_1level_tapbuf_size2_feedthrough_mem virtual_mem_fabric_out_1 (
		.feedthrough_mem_in({feedthrough_mem_in[25], feedthrough_mem_in[24], feedthrough_mem_in[23]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[25], feedthrough_mem_inb[24], feedthrough_mem_inb[23]}),
		.mem_out({mux_1level_tapbuf_size2_1_sram[2], mux_1level_tapbuf_size2_1_sram[1], mux_1level_tapbuf_size2_1_sram[0]}),
		.mem_outb({mux_1level_tapbuf_size2_1_sram_inv[2], mux_1level_tapbuf_size2_1_sram_inv[1], mux_1level_tapbuf_size2_1_sram_inv[0]}));

	direct_interc direct_interc_0_ (
		.in(fabric_in[0]),
		.out(direct_interc_0_out));

	direct_interc direct_interc_1_ (
		.in(fabric_in[1]),
		.out(direct_interc_1_out));

	direct_interc direct_interc_2_ (
		.in(fabric_in[2]),
		.out(direct_interc_2_out));

	direct_interc direct_interc_3_ (
		.in(fabric_in[3]),
		.out(direct_interc_3_out));

	direct_interc direct_interc_4_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[0]),
		.out(direct_interc_4_out));

	direct_interc direct_interc_5_ (
		.in(fabric_clk),
		.out(direct_interc_5_out));

	direct_interc direct_interc_6_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[1]),
		.out(direct_interc_6_out));

	direct_interc direct_interc_7_ (
		.in(fabric_clk),
		.out(direct_interc_7_out));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: fabric -----
