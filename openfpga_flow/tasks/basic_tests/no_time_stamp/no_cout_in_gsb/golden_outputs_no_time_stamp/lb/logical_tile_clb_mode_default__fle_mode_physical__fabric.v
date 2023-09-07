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
module logical_tile_clb_mode_default__fle_mode_physical__fabric(pReset,
                                                                prog_clk,
                                                                set,
                                                                reset,
                                                                clk,
                                                                fabric_in,
                                                                fabric_cin,
                                                                fabric_clk,
                                                                ccff_head,
                                                                fabric_out,
                                                                fabric_cout,
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
input [0:3] fabric_in;
//----- INPUT PORTS -----
input [0:0] fabric_cin;
//----- INPUT PORTS -----
input [0:0] fabric_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:1] fabric_out;
//----- OUTPUT PORTS -----
output [0:0] fabric_cout;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
wire [0:3] fabric_in;
wire [0:0] fabric_cin;
wire [0:0] fabric_clk;
wire [0:1] fabric_out;
wire [0:0] fabric_cout;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_1_out;
wire [0:0] direct_interc_2_out;
wire [0:0] direct_interc_3_out;
wire [0:0] direct_interc_4_out;
wire [0:0] direct_interc_5_out;
wire [0:0] direct_interc_6_out;
wire [0:0] direct_interc_7_out;
wire [0:0] direct_interc_8_out;
wire [0:0] direct_interc_9_out;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_cout;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_sumout;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0_ff_Q;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1_ff_Q;
wire [0:0] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_ccff_tail;
wire [0:1] logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out;
wire [0:0] mux_1level_tapbuf_size2_0_out;
wire [0:2] mux_1level_tapbuf_size2_0_sram;
wire [0:2] mux_1level_tapbuf_size2_0_sram_inv;
wire [0:0] mux_1level_tapbuf_size2_1_out;
wire [0:2] mux_1level_tapbuf_size2_1_sram;
wire [0:2] mux_1level_tapbuf_size2_1_sram_inv;
wire [0:0] mux_1level_tapbuf_size2_mem_0_ccff_tail;
wire [0:3] mux_1level_tapbuf_size3_0_sram;
wire [0:3] mux_1level_tapbuf_size3_0_sram_inv;
wire [0:3] mux_1level_tapbuf_size3_1_sram;
wire [0:3] mux_1level_tapbuf_size3_1_sram_inv;
wire [0:0] mux_1level_tapbuf_size3_mem_0_ccff_tail;
wire [0:0] mux_1level_tapbuf_size3_mem_1_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.frac_logic_in({direct_interc_1_out, direct_interc_2_out, direct_interc_3_out, direct_interc_4_out}),
		.ccff_head(ccff_head),
		.frac_logic_out(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[0:1]),
		.ccff_tail(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_ccff_tail));

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0 (
		.set(set),
		.reset(reset),
		.clk(clk),
		.ff_D(mux_1level_tapbuf_size2_0_out),
		.ff_Q(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0_ff_Q),
		.ff_clk(direct_interc_5_out));

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1 (
		.set(set),
		.reset(reset),
		.clk(clk),
		.ff_D(mux_1level_tapbuf_size2_1_out),
		.ff_Q(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1_ff_Q),
		.ff_clk(direct_interc_6_out));

	logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0 (
		.adder_a(direct_interc_7_out),
		.adder_b(direct_interc_8_out),
		.adder_cin(direct_interc_9_out),
		.adder_cout(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_cout),
		.adder_sumout(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_sumout));

	mux_1level_tapbuf_size3 mux_fabric_out_0 (
		.in({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_cout, logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_0_ff_Q, logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[0]}),
		.sram(mux_1level_tapbuf_size3_0_sram[0:3]),
		.sram_inv(mux_1level_tapbuf_size3_0_sram_inv[0:3]),
		.out(fabric_out[0]));

	mux_1level_tapbuf_size3 mux_fabric_out_1 (
		.in({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_sumout, logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff_1_ff_Q, logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[1]}),
		.sram(mux_1level_tapbuf_size3_1_sram[0:3]),
		.sram_inv(mux_1level_tapbuf_size3_1_sram_inv[0:3]),
		.out(fabric_out[1]));

	mux_1level_tapbuf_size3_mem mem_fabric_out_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size3_mem_0_ccff_tail),
		.mem_out(mux_1level_tapbuf_size3_0_sram[0:3]),
		.mem_outb(mux_1level_tapbuf_size3_0_sram_inv[0:3]));

	mux_1level_tapbuf_size3_mem mem_fabric_out_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size3_mem_0_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size3_mem_1_ccff_tail),
		.mem_out(mux_1level_tapbuf_size3_1_sram[0:3]),
		.mem_outb(mux_1level_tapbuf_size3_1_sram_inv[0:3]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_cout),
		.out(fabric_cout));

	direct_interc direct_interc_1_ (
		.in(fabric_in[0]),
		.out(direct_interc_1_out));

	direct_interc direct_interc_2_ (
		.in(fabric_in[1]),
		.out(direct_interc_2_out));

	direct_interc direct_interc_3_ (
		.in(fabric_in[2]),
		.out(direct_interc_3_out));

	direct_interc direct_interc_4_ (
		.in(fabric_in[3]),
		.out(direct_interc_4_out));

	direct_interc direct_interc_5_ (
		.in(fabric_clk),
		.out(direct_interc_5_out));

	direct_interc direct_interc_6_ (
		.in(fabric_clk),
		.out(direct_interc_6_out));

	direct_interc direct_interc_7_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[0]),
		.out(direct_interc_7_out));

	direct_interc direct_interc_8_ (
		.in(logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[1]),
		.out(direct_interc_8_out));

	direct_interc direct_interc_9_ (
		.in(fabric_cin),
		.out(direct_interc_9_out));

	mux_1level_tapbuf_size2 mux_ff_0_D_0 (
		.in({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[0], logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_cout}),
		.sram(mux_1level_tapbuf_size2_0_sram[0:2]),
		.sram_inv(mux_1level_tapbuf_size2_0_sram_inv[0:2]),
		.out(mux_1level_tapbuf_size2_0_out));

	mux_1level_tapbuf_size2 mux_ff_1_D_0 (
		.in({logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_0_frac_logic_out[1], logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder_0_adder_sumout}),
		.sram(mux_1level_tapbuf_size2_1_sram[0:2]),
		.sram_inv(mux_1level_tapbuf_size2_1_sram_inv[0:2]),
		.out(mux_1level_tapbuf_size2_1_out));

	mux_1level_tapbuf_size2_mem mem_ff_0_D_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size3_mem_1_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_0_ccff_tail),
		.mem_out(mux_1level_tapbuf_size2_0_sram[0:2]),
		.mem_outb(mux_1level_tapbuf_size2_0_sram_inv[0:2]));

	mux_1level_tapbuf_size2_mem mem_ff_1_D_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_0_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mux_1level_tapbuf_size2_1_sram[0:2]),
		.mem_outb(mux_1level_tapbuf_size2_1_sram_inv[0:2]));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: fabric -----
