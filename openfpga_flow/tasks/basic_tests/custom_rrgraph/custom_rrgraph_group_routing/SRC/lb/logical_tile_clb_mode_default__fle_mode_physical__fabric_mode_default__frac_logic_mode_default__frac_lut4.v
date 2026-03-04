//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: frac_lut4
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4 -----
module logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4(frac_lut4_in,
                                                                                                                 feedthrough_mem_in,
                                                                                                                 feedthrough_mem_inb,
                                                                                                                 frac_lut4_lut3_out,
                                                                                                                 frac_lut4_lut4_out);
//----- INPUT PORTS -----
input [3:0] frac_lut4_in;
//----- INPUT PORTS -----
input [16:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [16:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [1:0] frac_lut4_lut3_out;
//----- OUTPUT PORTS -----
output [0:0] frac_lut4_lut4_out;

//----- BEGIN wire-connection ports -----
wire [3:0] frac_lut4_in;
wire [1:0] frac_lut4_lut3_out;
wire [0:0] frac_lut4_lut4_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] frac_lut4_0_mode;
wire [0:0] frac_lut4_0_mode_inv;
wire [15:0] frac_lut4_0_sram;
wire [15:0] frac_lut4_0_sram_inv;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	frac_lut4 frac_lut4_0_ (
		.in({frac_lut4_in[3], frac_lut4_in[2], frac_lut4_in[1], frac_lut4_in[0]}),
		.sram({frac_lut4_0_sram[15], frac_lut4_0_sram[14], frac_lut4_0_sram[13], frac_lut4_0_sram[12], frac_lut4_0_sram[11], frac_lut4_0_sram[10], frac_lut4_0_sram[9], frac_lut4_0_sram[8], frac_lut4_0_sram[7], frac_lut4_0_sram[6], frac_lut4_0_sram[5], frac_lut4_0_sram[4], frac_lut4_0_sram[3], frac_lut4_0_sram[2], frac_lut4_0_sram[1], frac_lut4_0_sram[0]}),
		.sram_inv({frac_lut4_0_sram_inv[15], frac_lut4_0_sram_inv[14], frac_lut4_0_sram_inv[13], frac_lut4_0_sram_inv[12], frac_lut4_0_sram_inv[11], frac_lut4_0_sram_inv[10], frac_lut4_0_sram_inv[9], frac_lut4_0_sram_inv[8], frac_lut4_0_sram_inv[7], frac_lut4_0_sram_inv[6], frac_lut4_0_sram_inv[5], frac_lut4_0_sram_inv[4], frac_lut4_0_sram_inv[3], frac_lut4_0_sram_inv[2], frac_lut4_0_sram_inv[1], frac_lut4_0_sram_inv[0]}),
		.mode(frac_lut4_0_mode),
		.mode_inv(frac_lut4_0_mode_inv),
		.lut3_out({frac_lut4_lut3_out[1], frac_lut4_lut3_out[0]}),
		.lut4_out(frac_lut4_lut4_out));

	frac_lut4_feedthrough_DFFR_mem frac_lut4_feedthrough_DFFR_mem (
		.feedthrough_mem_in({feedthrough_mem_in[16], feedthrough_mem_in[15], feedthrough_mem_in[14], feedthrough_mem_in[13], feedthrough_mem_in[12], feedthrough_mem_in[11], feedthrough_mem_in[10], feedthrough_mem_in[9], feedthrough_mem_in[8], feedthrough_mem_in[7], feedthrough_mem_in[6], feedthrough_mem_in[5], feedthrough_mem_in[4], feedthrough_mem_in[3], feedthrough_mem_in[2], feedthrough_mem_in[1], feedthrough_mem_in[0]}),
		.feedthrough_mem_inb({feedthrough_mem_inb[16], feedthrough_mem_inb[15], feedthrough_mem_inb[14], feedthrough_mem_inb[13], feedthrough_mem_inb[12], feedthrough_mem_inb[11], feedthrough_mem_inb[10], feedthrough_mem_inb[9], feedthrough_mem_inb[8], feedthrough_mem_inb[7], feedthrough_mem_inb[6], feedthrough_mem_inb[5], feedthrough_mem_inb[4], feedthrough_mem_inb[3], feedthrough_mem_inb[2], feedthrough_mem_inb[1], feedthrough_mem_inb[0]}),
		.mem_out({frac_lut4_0_mode, frac_lut4_0_sram[15], frac_lut4_0_sram[14], frac_lut4_0_sram[13], frac_lut4_0_sram[12], frac_lut4_0_sram[11], frac_lut4_0_sram[10], frac_lut4_0_sram[9], frac_lut4_0_sram[8], frac_lut4_0_sram[7], frac_lut4_0_sram[6], frac_lut4_0_sram[5], frac_lut4_0_sram[4], frac_lut4_0_sram[3], frac_lut4_0_sram[2], frac_lut4_0_sram[1], frac_lut4_0_sram[0]}),
		.mem_outb({frac_lut4_0_mode_inv, frac_lut4_0_sram_inv[15], frac_lut4_0_sram_inv[14], frac_lut4_0_sram_inv[13], frac_lut4_0_sram_inv[12], frac_lut4_0_sram_inv[11], frac_lut4_0_sram_inv[10], frac_lut4_0_sram_inv[9], frac_lut4_0_sram_inv[8], frac_lut4_0_sram_inv[7], frac_lut4_0_sram_inv[6], frac_lut4_0_sram_inv[5], frac_lut4_0_sram_inv[4], frac_lut4_0_sram_inv[3], frac_lut4_0_sram_inv[2], frac_lut4_0_sram_inv[1], frac_lut4_0_sram_inv[0]}));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4 -----

//----- Default net type -----
`default_nettype wire



