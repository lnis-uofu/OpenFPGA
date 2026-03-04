//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: frac_logic
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: frac_logic -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic -----
module logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic(frac_logic_in,
                                                                                         feedthrough_mem_in,
                                                                                         feedthrough_mem_inb,
                                                                                         frac_logic_out);
//----- INPUT PORTS -----
input [3:0] frac_logic_in;
//----- INPUT PORTS -----
input [19:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [19:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [1:0] frac_logic_out;

//----- BEGIN wire-connection ports -----
wire [3:0] frac_logic_in;
wire [1:0] frac_logic_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_1_out;
wire [0:0] direct_interc_2_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [1:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut3_out;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut4_out;
wire [2:0] mux_1level_tapbuf_size2_0_sram;
wire [2:0] mux_1level_tapbuf_size2_0_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4 logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0 (
		.frac_lut4_in({direct_interc_4_out, direct_interc_3_out, direct_interc_2_out, direct_interc_1_out}),
		.feedthrough_mem_in({feedthrough_mem_in[16], feedthrough_mem_in[15], feedthrough_mem_in[14], feedthrough_mem_in[13], feedthrough_mem_in[12], feedthrough_mem_in[11], feedthrough_mem_in[10], feedthrough_mem_in[9], feedthrough_mem_in[8], feedthrough_mem_in[7], feedthrough_mem_in[6], feedthrough_mem_in[5], feedthrough_mem_in[4], feedthrough_mem_in[3], feedthrough_mem_in[2], feedthrough_mem_in[1], feedthrough_mem_in[0]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[16], feedthrough_mem_inb[15], feedthrough_mem_inb[14], feedthrough_mem_inb[13], feedthrough_mem_inb[12], feedthrough_mem_inb[11], feedthrough_mem_inb[10], feedthrough_mem_inb[9], feedthrough_mem_inb[8], feedthrough_mem_inb[7], feedthrough_mem_inb[6], feedthrough_mem_inb[5], feedthrough_mem_inb[4], feedthrough_mem_inb[3], feedthrough_mem_inb[2], feedthrough_mem_inb[1], feedthrough_mem_inb[0]}),
		.frac_lut4_lut3_out({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut3_out[1], logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut3_out[0]}),
		.frac_lut4_lut4_out(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut4_out));

	mux_1level_tapbuf_size2 mux_frac_logic_out_0 (
		.in({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut3_out[0], logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut4_out}),
		.sram({mux_1level_tapbuf_size2_0_sram[2], mux_1level_tapbuf_size2_0_sram[1], mux_1level_tapbuf_size2_0_sram[0]}),
		.sram_inv({mux_1level_tapbuf_size2_0_sram_inv[2], mux_1level_tapbuf_size2_0_sram_inv[1], mux_1level_tapbuf_size2_0_sram_inv[0]}),
		.out(frac_logic_out[0]));

	mux_1level_tapbuf_size2_feedthrough_mem virtual_mem_frac_logic_out_0 (
		.feedthrough_mem_in({feedthrough_mem_in[19], feedthrough_mem_in[18], feedthrough_mem_in[17]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[19], feedthrough_mem_inb[18], feedthrough_mem_inb[17]}),
		.mem_out({mux_1level_tapbuf_size2_0_sram[2], mux_1level_tapbuf_size2_0_sram[1], mux_1level_tapbuf_size2_0_sram[0]}),
		.mem_outb({mux_1level_tapbuf_size2_0_sram_inv[2], mux_1level_tapbuf_size2_0_sram_inv[1], mux_1level_tapbuf_size2_0_sram_inv[0]}));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4_0_frac_lut4_lut3_out[1]),
		.out(frac_logic_out[1]));

	direct_interc direct_interc_1_ (
		.in(frac_logic_in[0]),
		.out(direct_interc_1_out));

	direct_interc direct_interc_2_ (
		.in(frac_logic_in[1]),
		.out(direct_interc_2_out));

	direct_interc direct_interc_3_ (
		.in(frac_logic_in[2]),
		.out(direct_interc_3_out));

	direct_interc direct_interc_4_ (
		.in(frac_logic_in[3]),
		.out(direct_interc_4_out));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: frac_logic -----
