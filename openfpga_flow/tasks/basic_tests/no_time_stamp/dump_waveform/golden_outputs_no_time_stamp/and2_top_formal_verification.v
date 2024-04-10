//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog netlist for pre-configured FPGA fabric by design: and2
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

module and2_top_formal_verification (
input [0:0] a,
input [0:0] b,
output [0:0] c);

// ----- Local wires for FPGA fabric -----
wire [0:31] gfpga_pad_GPIO_PAD_fm;
wire [0:0] ccff_head_fm;
wire [0:0] ccff_tail_fm;
wire [0:0] prog_clk_fm;
wire [0:0] set_fm;
wire [0:0] reset_fm;
wire [0:0] clk_fm;

// ----- FPGA top-level module to be capsulated -----
	fpga_top U0_formal_verification (
		.prog_clk(prog_clk_fm[0]),
		.set(set_fm[0]),
		.reset(reset_fm[0]),
		.clk(clk_fm[0]),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD_fm[0:31]),
		.ccff_head(ccff_head_fm[0]),
		.ccff_tail(ccff_tail_fm[0]));

// ----- Begin Connect Global ports of FPGA top module -----
	assign set_fm[0] = 1'b0;
	assign reset_fm[0] = 1'b0;
	assign clk_fm[0] = 1'b0;
	assign prog_clk_fm[0] = 1'b0;
// ----- End Connect Global ports of FPGA top module -----

// ----- Link BLIF Benchmark I/Os to FPGA I/Os -----
// ----- Blif Benchmark input a is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD_fm[11] -----
	assign gfpga_pad_GPIO_PAD_fm[11] = a[0];

// ----- Blif Benchmark input b is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD_fm[14] -----
	assign gfpga_pad_GPIO_PAD_fm[14] = b[0];

// ----- Blif Benchmark output c is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD_fm[1] -----
	assign c[0] = gfpga_pad_GPIO_PAD_fm[1];

// ----- Wire unused FPGA I/Os to constants -----
	assign gfpga_pad_GPIO_PAD_fm[0] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[2] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[3] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[4] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[5] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[6] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[7] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[8] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[9] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[10] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[12] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[13] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[15] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[16] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[17] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[18] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[19] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[20] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[21] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[22] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[23] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[24] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[25] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[26] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[27] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[28] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[29] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[30] = 1'b0;
	assign gfpga_pad_GPIO_PAD_fm[31] = 1'b0;

// ----- Begin load bitstream to configuration memories -----
// ----- Begin assign bitstream to configuration memories -----
initial begin
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_out[0:15] = {16{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_outb[0:15] = {16{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_1.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_out[0:15] = {16{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_1.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_outb[0:15] = {16{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_1.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_1.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_2.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_out[0:15] = {16{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_2.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_outb[0:15] = {16{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_2.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_2.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_3.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_out[0:15] = 16'b1010101000000000;
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_3.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0.lut4_DFF_mem.mem_outb[0:15] = 16'b0101010111111111;
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_3.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_out[0:1] = 2'b01;
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.logical_tile_clb_mode_default__fle_3.logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0.mem_ble4_out_0.mem_outb[0:1] = 2'b10;
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_0.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_0.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_1.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_1.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_2.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_2.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_3.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_0_in_3.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_0.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_0.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_1.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_1.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_2.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_2.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_3.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_1_in_3.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_0.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_0.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_1.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_1.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_2.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_2.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_3.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_2_in_3.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_0.mem_out[0:3] = 4'b0110;
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_0.mem_outb[0:3] = 4'b1001;
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_1.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_1.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_2.mem_out[0:3] = {4{1'b0}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_2.mem_outb[0:3] = {4{1'b1}};
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_3.mem_out[0:3] = 4'b0111;
	force U0_formal_verification.grid_clb_1__1_.logical_tile_clb_mode_clb__0.mem_fle_3_in_3.mem_outb[0:3] = 4'b1000;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b0;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_top_1__2_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_right_2__1_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_bottom_1__0_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__0.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__1.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__2.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__3.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__4.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__5.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__6.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_out[0] = 1'b1;
	force U0_formal_verification.grid_io_left_0__1_.logical_tile_io_mode_io__7.logical_tile_io_mode_physical__iopad_0.GPIO_DFF_mem.mem_outb[0] = 1'b0;
	force U0_formal_verification.sb_0__0_.mem_top_track_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_2.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_2.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_4.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_4.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_6.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_6.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_8.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_8.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_10.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_10.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_12.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_12.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_14.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_14.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_16.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_16.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_18.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_18.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_20.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_20.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_22.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_22.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_top_track_24.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_top_track_24.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_4.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_4.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_6.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_6.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_8.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_8.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_10.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_10.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_12.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_12.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_14.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_14.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_16.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_16.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_18.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_18.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_20.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_20.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_22.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_22.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__0_.mem_right_track_24.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__0_.mem_right_track_24.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_2.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_2.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_4.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_4.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_6.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_6.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_8.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_8.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_10.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_10.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_12.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_12.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_14.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_14.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_16.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_16.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_18.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_18.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_20.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_20.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_22.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_22.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_right_track_24.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_right_track_24.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_3.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_3.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_5.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_5.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_7.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_7.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_9.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_9.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_11.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_11.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_13.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_13.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_15.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_15.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_17.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_17.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_19.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_19.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_21.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_21.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_23.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_23.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_25.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_0__1_.mem_bottom_track_25.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_0.mem_out[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_0.mem_outb[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_4.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_4.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_6.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_6.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_8.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_8.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_10.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_10.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_12.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_12.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_14.mem_out[0:2] = 3'b011;
	force U0_formal_verification.sb_1__0_.mem_top_track_14.mem_outb[0:2] = 3'b100;
	force U0_formal_verification.sb_1__0_.mem_top_track_16.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_16.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_18.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_18.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_20.mem_out[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_20.mem_outb[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_22.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_22.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_top_track_24.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_top_track_24.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_3.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_3.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_5.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_5.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_7.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_7.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_9.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_9.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_11.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_11.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_13.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_13.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_15.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_15.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_17.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_17.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_19.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_19.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_21.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_21.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_23.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_23.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__0_.mem_left_track_25.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__0_.mem_left_track_25.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_3.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_3.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_5.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_5.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_7.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_7.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_9.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_9.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_11.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_11.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_13.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_13.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_15.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_15.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_17.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_17.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_19.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_19.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_21.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_21.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_23.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_23.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_25.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_bottom_track_25.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_3.mem_out[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_3.mem_outb[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_5.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_5.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_7.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_7.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_9.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_9.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_11.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_11.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_13.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_13.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_15.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_15.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_17.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_17.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_19.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_19.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_21.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_21.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_23.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_23.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.sb_1__1_.mem_left_track_25.mem_out[0:1] = {2{1'b0}};
	force U0_formal_verification.sb_1__1_.mem_left_track_25.mem_outb[0:1] = {2{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_bottom_ipin_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_bottom_ipin_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_bottom_ipin_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_bottom_ipin_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_bottom_ipin_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_bottom_ipin_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_3.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_3.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_4.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_4.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_5.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_5.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_6.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_6.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_7.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__0_.mem_top_ipin_7.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_1.mem_out[0:2] = 3'b001;
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_1.mem_outb[0:2] = 3'b110;
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_3.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_3.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_4.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_4.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_5.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_5.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_6.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_6.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_7.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_bottom_ipin_7.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_top_ipin_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_top_ipin_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_top_ipin_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_top_ipin_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cbx_1__1_.mem_top_ipin_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cbx_1__1_.mem_top_ipin_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_left_ipin_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_left_ipin_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_left_ipin_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_left_ipin_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_3.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_3.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_4.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_4.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_5.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_5.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_6.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_6.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_7.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_0__1_.mem_right_ipin_7.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_0.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_0.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_2.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_2.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_3.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_3.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_4.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_4.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_5.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_5.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_6.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_6.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_7.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_left_ipin_7.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_right_ipin_0.mem_out[0:2] = 3'b101;
	force U0_formal_verification.cby_1__1_.mem_right_ipin_0.mem_outb[0:2] = 3'b010;
	force U0_formal_verification.cby_1__1_.mem_right_ipin_1.mem_out[0:2] = {3{1'b0}};
	force U0_formal_verification.cby_1__1_.mem_right_ipin_1.mem_outb[0:2] = {3{1'b1}};
	force U0_formal_verification.cby_1__1_.mem_right_ipin_2.mem_out[0:2] = 3'b110;
	force U0_formal_verification.cby_1__1_.mem_right_ipin_2.mem_outb[0:2] = 3'b001;
end
// ----- End assign bitstream to configuration memories -----
// ----- End load bitstream to configuration memories -----
// ------ Use DUMP_FSDB to enable FSDB waveform output -----
`ifdef DUMP_FSDB
initial begin
	$fsdbDumpfile("and2.fsdb");
	$fsdbDumpvars(0, "U0_formal_verification");
end
`endif
// ------ Use DUMP_VCD to enable VCD waveform output -----
`ifdef DUMP_VCD
initial begin
	$dumpfile("and2.vcd");
	$dumpvars(0, "U0_formal_verification");
end
`endif
endmodule
// ----- END Verilog module for and2_top_formal_verification -----

//----- Default net type -----
`default_nettype wire

