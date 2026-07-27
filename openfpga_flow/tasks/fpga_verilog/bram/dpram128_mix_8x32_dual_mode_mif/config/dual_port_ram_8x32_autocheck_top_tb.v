//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: FPGA Verilog full testbench for top-level netlist of design: dual_port_ram_8x32
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Mon Jul 27 12:41:48 2026
//-------------------------------------------
//----- Default net type -----
`default_nettype none

module dual_port_ram_8x32_autocheck_top_tb;
// ----- Local wires for global ports of FPGA fabric -----
wire [0:0] clk;
wire [0:0] pReset;
wire [0:0] prog_clk;
wire [0:0] set;
wire [0:0] reset;
wire [0:0] mem_init_rst_n;
wire [0:0] mem_init_clk;
wire [0:0] mem_init_start;
wire [0:2] mem128_init_addr;
wire [0:3] mem256_init_addr;

// ----- Local wires for I/Os of FPGA fabric -----
wire [0:159] gfpga_pad_GPIO_PAD;

wire [0:15] gfpga_pad_frac_mem_256_preload_mem_init_data;
wire [0:15] gfpga_pad_dpram_8x16_preload_mem_init_data;

wire [0:0] gfpga_pad_frac_mem_256_preload_mem_init_done;
wire [0:0] gfpga_pad_dpram_8x16_preload_mem_init_done;

reg [0:0] __config_done__;
wire [0:0] __config_all_done__;
wire [0:0] __prog_clock__;
reg [0:0] __prog_clock___reg__;
wire [0:0] __op_clock__;
reg [0:0] __op_clock___reg__;
reg [0:0] __prog_reset__;
reg [0:0] __prog_set_;
reg [0:0] __greset__;
reg [0:0] __gset__;
// ---- Configuration-chain head -----
reg [0:15] ccff_head;
// ---- Configuration-chain tail -----
wire [0:15] ccff_tail;
// ----- Shared inputs -------
	reg [0:0] wen_shared_input;
	reg [0:0] ren_shared_input;
	reg [0:0] waddr_0__shared_input;
	reg [0:0] waddr_1__shared_input;
	reg [0:0] raddr_0__shared_input;
	reg [0:0] raddr_1__shared_input;
	reg [0:0] din_0__shared_input;
	reg [0:0] din_1__shared_input;
	reg [0:0] din_2__shared_input;
	reg [0:0] din_3__shared_input;
	reg [0:0] din_4__shared_input;
	reg [0:0] din_5__shared_input;
	reg [0:0] din_6__shared_input;
	reg [0:0] din_7__shared_input;
	reg [0:0] din_8__shared_input;
	reg [0:0] din_9__shared_input;
	reg [0:0] din_10__shared_input;
	reg [0:0] din_11__shared_input;
	reg [0:0] din_12__shared_input;
	reg [0:0] din_13__shared_input;
	reg [0:0] din_14__shared_input;
	reg [0:0] din_15__shared_input;
	reg [0:0] din_16__shared_input;
	reg [0:0] din_17__shared_input;
	reg [0:0] din_18__shared_input;
	reg [0:0] din_19__shared_input;
	reg [0:0] din_20__shared_input;
	reg [0:0] din_21__shared_input;
	reg [0:0] din_22__shared_input;
	reg [0:0] din_23__shared_input;
	reg [0:0] din_24__shared_input;
	reg [0:0] din_25__shared_input;
	reg [0:0] din_26__shared_input;
	reg [0:0] din_27__shared_input;
	reg [0:0] din_28__shared_input;
	reg [0:0] din_29__shared_input;
	reg [0:0] din_30__shared_input;
	reg [0:0] din_31__shared_input;

// ----- FPGA fabric outputs -------
	wire [0:0] dout_0__fpga;
	wire [0:0] dout_1__fpga;
	wire [0:0] dout_2__fpga;
	wire [0:0] dout_3__fpga;
	wire [0:0] dout_4__fpga;
	wire [0:0] dout_5__fpga;
	wire [0:0] dout_6__fpga;
	wire [0:0] dout_7__fpga;
	wire [0:0] dout_8__fpga;
	wire [0:0] dout_9__fpga;
	wire [0:0] dout_10__fpga;
	wire [0:0] dout_11__fpga;
	wire [0:0] dout_12__fpga;
	wire [0:0] dout_13__fpga;
	wire [0:0] dout_14__fpga;
	wire [0:0] dout_15__fpga;
	wire [0:0] dout_16__fpga;
	wire [0:0] dout_17__fpga;
	wire [0:0] dout_18__fpga;
	wire [0:0] dout_19__fpga;
	wire [0:0] dout_20__fpga;
	wire [0:0] dout_21__fpga;
	wire [0:0] dout_22__fpga;
	wire [0:0] dout_23__fpga;
	wire [0:0] dout_24__fpga;
	wire [0:0] dout_25__fpga;
	wire [0:0] dout_26__fpga;
	wire [0:0] dout_27__fpga;
	wire [0:0] dout_28__fpga;
	wire [0:0] dout_29__fpga;
	wire [0:0] dout_30__fpga;
	wire [0:0] dout_31__fpga;

// ----- Benchmark outputs -------
	wire [0:0] dout_0__benchmark;
	wire [0:0] dout_1__benchmark;
	wire [0:0] dout_2__benchmark;
	wire [0:0] dout_3__benchmark;
	wire [0:0] dout_4__benchmark;
	wire [0:0] dout_5__benchmark;
	wire [0:0] dout_6__benchmark;
	wire [0:0] dout_7__benchmark;
	wire [0:0] dout_8__benchmark;
	wire [0:0] dout_9__benchmark;
	wire [0:0] dout_10__benchmark;
	wire [0:0] dout_11__benchmark;
	wire [0:0] dout_12__benchmark;
	wire [0:0] dout_13__benchmark;
	wire [0:0] dout_14__benchmark;
	wire [0:0] dout_15__benchmark;
	wire [0:0] dout_16__benchmark;
	wire [0:0] dout_17__benchmark;
	wire [0:0] dout_18__benchmark;
	wire [0:0] dout_19__benchmark;
	wire [0:0] dout_20__benchmark;
	wire [0:0] dout_21__benchmark;
	wire [0:0] dout_22__benchmark;
	wire [0:0] dout_23__benchmark;
	wire [0:0] dout_24__benchmark;
	wire [0:0] dout_25__benchmark;
	wire [0:0] dout_26__benchmark;
	wire [0:0] dout_27__benchmark;
	wire [0:0] dout_28__benchmark;
	wire [0:0] dout_29__benchmark;
	wire [0:0] dout_30__benchmark;
	wire [0:0] dout_31__benchmark;

// ----- Output vectors checking flags -------
	reg [0:0] dout_0__flag;
	reg [0:0] dout_1__flag;
	reg [0:0] dout_2__flag;
	reg [0:0] dout_3__flag;
	reg [0:0] dout_4__flag;
	reg [0:0] dout_5__flag;
	reg [0:0] dout_6__flag;
	reg [0:0] dout_7__flag;
	reg [0:0] dout_8__flag;
	reg [0:0] dout_9__flag;
	reg [0:0] dout_10__flag;
	reg [0:0] dout_11__flag;
	reg [0:0] dout_12__flag;
	reg [0:0] dout_13__flag;
	reg [0:0] dout_14__flag;
	reg [0:0] dout_15__flag;
	reg [0:0] dout_16__flag;
	reg [0:0] dout_17__flag;
	reg [0:0] dout_18__flag;
	reg [0:0] dout_19__flag;
	reg [0:0] dout_20__flag;
	reg [0:0] dout_21__flag;
	reg [0:0] dout_22__flag;
	reg [0:0] dout_23__flag;
	reg [0:0] dout_24__flag;
	reg [0:0] dout_25__flag;
	reg [0:0] dout_26__flag;
	reg [0:0] dout_27__flag;
	reg [0:0] dout_28__flag;
	reg [0:0] dout_29__flag;
	reg [0:0] dout_30__flag;
	reg [0:0] dout_31__flag;

// ----- Error counter: Deposit an error for config_done signal is not raised at the beginning -----
	integer nb_error= 1;
// ----- Number of clock cycles in configuration phase: 7949 -----
// ----- Begin configuration done signal generation -----
initial
	begin
		__config_done__[0] = 1'b0;
	end

// ----- End configuration done signal generation -----

// ----- Begin raw programming clock signal generation -----
initial
	begin
		__prog_clock___reg__[0] = 1'b0;
	end
always
	begin
		#1.666666746	__prog_clock___reg__[0] = ~__prog_clock___reg__[0];
	end

// ----- End raw programming clock signal generation -----

// ----- Actual programming clock is triggered only when __config_done__ and __prog_reset__ are disabled -----
	assign __prog_clock__[0] = __prog_clock___reg__[0] & (~__config_done__[0]) & (~__prog_reset__[0]);

// ----- __config_all_done__ requires BOTH embedded dpram_8x16_preload memories to report mem_init_done, in addition to the bitstream configuration finishing -----
reg [0:1] mem_init_done_sticky;
always @(posedge mem_init_clk[0] or negedge mem_init_rst_n[0]) begin
    if (1'b0 == mem_init_rst_n[0]) begin
        mem_init_done_sticky <= 2'b00;
    end else begin
        if (gfpga_pad_frac_mem_256_preload_mem_init_done[0]) mem_init_done_sticky[0] <= 1'b1;
        if (gfpga_pad_dpram_8x16_preload_mem_init_done[0]) mem_init_done_sticky[1] <= 1'b1;
    end
end

assign __config_all_done__[0] = __config_done__[0] & mem_init_done_sticky[0] & mem_init_done_sticky[1];

// ----- Begin raw operating clock signal generation -----
initial
	begin
		__op_clock___reg__[0] = 1'b0;
	end
always wait(~__greset__)
	begin
		#1	__op_clock___reg__[0] = ~__op_clock___reg__[0];
	end

// ----- End raw operating clock signal generation -----
// ----- Actual operating clock is triggered only when __config_all_done__ is enabled -----
	assign __op_clock__[0] = __op_clock___reg__[0] & __config_all_done__[0];

// ----- Begin programming reset signal generation -----
initial
	begin
		__prog_reset__[0] = 1'b1;
	#3.333333492	__prog_reset__[0] = 1'b0;
	end

// ----- End programming reset signal generation -----

// ----- Begin programming set signal generation -----
initial
	begin
		__prog_set_[0] = 1'b1;
	#3.333333492	__prog_set_[0] = 1'b0;
	end

// ----- End programming set signal generation -----

// ----- Begin operating reset signal generation -----
// ----- Reset signal is enabled until the first clock cycle in operation phase -----
initial
	begin
		__greset__[0] = 1'b1;
	wait(__config_all_done__)
	#2	__greset__[0] = 1'b1;
	#4	__greset__[0] = 1'b0;
	end

// ----- End operating reset signal generation -----
// ----- Begin operating set signal generation: always disabled -----
initial
	begin
		__gset__[0] = 1'b0;
	end

// ----- End operating set signal generation: always disabled -----

// ----- Begin connecting global ports of FPGA fabric to stimuli -----
	assign prog_clk[0] = __prog_clock__[0];
	assign clk[0] = __op_clock__[0];
	assign reset[0] = __greset__[0];
	assign pReset[0] = __prog_reset__[0];
	assign set[0] = __gset__[0];
// ----- mem_init_rst_n is driven by __prog_reset__ (active-low reset, released once programming reset drops) -----
	assign mem_init_rst_n[0] = ~__prog_reset__[0];
// ----- mem_init_clk is driven directly by __prog_clock__ -- memory init now runs concurrently with bitstream configuration, using the same clock -----
	assign mem_init_clk[0] = __prog_clock__[0];
// ----- mem_init_start mirrors the "init_start" stimulus of dpram_8x16_preload_tb.v -----
	assign mem_init_start[0] = mem_init_start_reg[0];
// ----- mem_init_addr is a GLOBAL, TB-driven port (grouped with clk/reset, not with the per-instance gfpga_pad_... data/done pins) -----
// ----- so the testbench must actively sweep it -- an internal counter, incremented once per mem_init_clk cycle while mem_init_start is asserted -----
	assign mem128_init_addr[0:2] = mem128_init_addr_reg;
// ----- End connecting global ports of FPGA fabric to stimuli -----

// ----- Begin mem_init_addr counter: sweeps 0 to 7 (3-bit, matching the 8-deep RAM), one step per mem_init_clk cycle, while mem_init_start is held -----
	reg [0:2] mem128_init_addr_reg;
always @(posedge mem_init_clk[0] or negedge mem_init_rst_n[0]) begin
	if (1'b0 == mem_init_rst_n[0]) begin
		mem128_init_addr_reg <= 3'b000;
	end else if ((1'b1 == mem_init_start[0]) && (mem128_init_addr_reg != 3'b111)) begin
		mem128_init_addr_reg <= mem128_init_addr_reg + 3'b001;
	end
end
	assign mem256_init_addr[0:3] = mem256_init_addr_reg;
// ----- Begin mem_init_addr counter: sweeps 0 to 16 (4-bit, matching the 16-deep RAM), one step per mem_init_clk cycle, while mem_init_start is held -----
	reg [0:3] mem256_init_addr_reg;
always @(posedge mem_init_clk[0] or negedge mem_init_rst_n[0]) begin
	if (1'b0 == mem_init_rst_n[0]) begin
		mem256_init_addr_reg <= 4'b0000;
	end else if ((1'b1 == mem_init_start[0]) && (mem256_init_addr_reg != 4'b1111)) begin
		mem256_init_addr_reg <= mem256_init_addr_reg + 4'b001;
	end
end

// ----- End connecting global ports of FPGA fabric to stimuli -----
// ----- FPGA top-level module to be capsulated -----
	fpga_top FPGA_DUT (
		.clk(clk[0]),
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.set(set[0]),
		.reset(reset[0]),
		.mem_init_rst_n(mem_init_rst_n[0]),
		.mem_init_clk(mem_init_clk[0]),
		.mem_init_start(mem_init_start[0]),
		.mem128_init_addr(mem128_init_addr[0:2]),
		.mem256_init_addr(mem256_init_addr[0:3]),
		.gfpga_pad_frac_mem_256_preload_mem_init_data(gfpga_pad_frac_mem_256_preload_mem_init_data[0:15]),
		.gfpga_pad_dpram_8x16_preload_mem_init_data(gfpga_pad_dpram_8x16_preload_mem_init_data[0:15]),
		.gfpga_pad_frac_mem_256_preload_mem_init_done(gfpga_pad_frac_mem_256_preload_mem_init_done[0]),
		.gfpga_pad_dpram_8x16_preload_mem_init_done(gfpga_pad_dpram_8x16_preload_mem_init_done[0]),
		.gfpga_pad_GPIO_PAD(gfpga_pad_GPIO_PAD[0:159]),
		.ccff_head(ccff_head[0:15]),
		.ccff_tail(ccff_tail[0:15]));
// ----- Initialization memory source, loaded from a single external 32-bit-wide MIF file (maps to init_src_data/init_src_addr in dpram_8x16_preload_tb.v) -----
// ----- fpga_top embeds TWO physical dpram_8x16_preload memories that share ONE 32-bit init data bus, looked up by the TB-driven 3-bit mem_init_addr counter above -----
// ----- File format is standard $readmemh sparse-address syntax: "@ADDR" on its own token followed by a hex DATA word, e.g. "@0 FFFFFFFF" -- matching dual_port_ram_128_mem_init.mif -----
// ----- NOTE: update the file name/path below if your generated MIF file is named/located differently. -----
`define MEM_INIT_MIF "dual_port_ram_8x32_mem_init.mif"
	reg [0:31] mem_init_rom [0:15];
initial begin
	$readmemh(`MEM_INIT_MIF, mem_init_rom);
end
	assign gfpga_pad_dpram_8x16_preload_mem_init_data[0:15] = mem_init_rom[mem128_init_addr[0:2]][0:15];
	assign gfpga_pad_frac_mem_256_preload_mem_init_data[0:15] = mem_init_rom[mem256_init_addr[0:3]][16:31];

// ----- Link BLIF Benchmark I/Os to FPGA I/Os -----
// ----- Blif Benchmark input clk is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[142] -----
	assign gfpga_pad_GPIO_PAD[142] = clk[0];

// ----- Blif Benchmark input wen is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[107] -----
	assign gfpga_pad_GPIO_PAD[107] = wen_shared_input[0];

// ----- Blif Benchmark input ren is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[109] -----
	assign gfpga_pad_GPIO_PAD[109] = ren_shared_input[0];

// ----- Blif Benchmark input waddr_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[22] -----
	assign gfpga_pad_GPIO_PAD[22] = waddr_0__shared_input[0];

// ----- Blif Benchmark input waddr_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[108] -----
	assign gfpga_pad_GPIO_PAD[108] = waddr_1__shared_input[0];

// ----- Blif Benchmark input raddr_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[56] -----
	assign gfpga_pad_GPIO_PAD[56] = raddr_0__shared_input[0];

// ----- Blif Benchmark input raddr_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[63] -----
	assign gfpga_pad_GPIO_PAD[63] = raddr_1__shared_input[0];

// ----- Blif Benchmark input din_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[106] -----
	assign gfpga_pad_GPIO_PAD[106] = din_0__shared_input[0];

// ----- Blif Benchmark input din_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[20] -----
	assign gfpga_pad_GPIO_PAD[20] = din_1__shared_input[0];

// ----- Blif Benchmark input din_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[97] -----
	assign gfpga_pad_GPIO_PAD[97] = din_2__shared_input[0];

// ----- Blif Benchmark input din_3_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[28] -----
	assign gfpga_pad_GPIO_PAD[28] = din_3__shared_input[0];

// ----- Blif Benchmark input din_4_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[144] -----
	assign gfpga_pad_GPIO_PAD[144] = din_4__shared_input[0];

// ----- Blif Benchmark input din_5_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[103] -----
	assign gfpga_pad_GPIO_PAD[103] = din_5__shared_input[0];

// ----- Blif Benchmark input din_6_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[101] -----
	assign gfpga_pad_GPIO_PAD[101] = din_6__shared_input[0];

// ----- Blif Benchmark input din_7_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[105] -----
	assign gfpga_pad_GPIO_PAD[105] = din_7__shared_input[0];

// ----- Blif Benchmark input din_8_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[155] -----
	assign gfpga_pad_GPIO_PAD[155] = din_8__shared_input[0];

// ----- Blif Benchmark input din_9_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[111] -----
	assign gfpga_pad_GPIO_PAD[111] = din_9__shared_input[0];

// ----- Blif Benchmark input din_10_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[54] -----
	assign gfpga_pad_GPIO_PAD[54] = din_10__shared_input[0];

// ----- Blif Benchmark input din_11_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[96] -----
	assign gfpga_pad_GPIO_PAD[96] = din_11__shared_input[0];

// ----- Blif Benchmark input din_12_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[24] -----
	assign gfpga_pad_GPIO_PAD[24] = din_12__shared_input[0];

// ----- Blif Benchmark input din_13_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[110] -----
	assign gfpga_pad_GPIO_PAD[110] = din_13__shared_input[0];

// ----- Blif Benchmark input din_14_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[93] -----
	assign gfpga_pad_GPIO_PAD[93] = din_14__shared_input[0];

// ----- Blif Benchmark input din_15_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[53] -----
	assign gfpga_pad_GPIO_PAD[53] = din_15__shared_input[0];

// ----- Blif Benchmark input din_16_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[31] -----
	assign gfpga_pad_GPIO_PAD[31] = din_16__shared_input[0];

// ----- Blif Benchmark input din_17_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[102] -----
	assign gfpga_pad_GPIO_PAD[102] = din_17__shared_input[0];

// ----- Blif Benchmark input din_18_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[147] -----
	assign gfpga_pad_GPIO_PAD[147] = din_18__shared_input[0];

// ----- Blif Benchmark input din_19_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[30] -----
	assign gfpga_pad_GPIO_PAD[30] = din_19__shared_input[0];

// ----- Blif Benchmark input din_20_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[100] -----
	assign gfpga_pad_GPIO_PAD[100] = din_20__shared_input[0];

// ----- Blif Benchmark input din_21_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[51] -----
	assign gfpga_pad_GPIO_PAD[51] = din_21__shared_input[0];

// ----- Blif Benchmark input din_22_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[159] -----
	assign gfpga_pad_GPIO_PAD[159] = din_22__shared_input[0];

// ----- Blif Benchmark input din_23_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[157] -----
	assign gfpga_pad_GPIO_PAD[157] = din_23__shared_input[0];

// ----- Blif Benchmark input din_24_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[17] -----
	assign gfpga_pad_GPIO_PAD[17] = din_24__shared_input[0];

// ----- Blif Benchmark input din_25_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[61] -----
	assign gfpga_pad_GPIO_PAD[61] = din_25__shared_input[0];

// ----- Blif Benchmark input din_26_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[154] -----
	assign gfpga_pad_GPIO_PAD[154] = din_26__shared_input[0];

// ----- Blif Benchmark input din_27_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[158] -----
	assign gfpga_pad_GPIO_PAD[158] = din_27__shared_input[0];

// ----- Blif Benchmark input din_28_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[19] -----
	assign gfpga_pad_GPIO_PAD[19] = din_28__shared_input[0];

// ----- Blif Benchmark input din_29_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[99] -----
	assign gfpga_pad_GPIO_PAD[99] = din_29__shared_input[0];

// ----- Blif Benchmark input din_30_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[145] -----
	assign gfpga_pad_GPIO_PAD[145] = din_30__shared_input[0];

// ----- Blif Benchmark input din_31_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[104] -----
	assign gfpga_pad_GPIO_PAD[104] = din_31__shared_input[0];

// ----- Blif Benchmark output dout_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[27] -----
	assign dout_0__fpga[0] = gfpga_pad_GPIO_PAD[27];

// ----- Blif Benchmark output dout_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[26] -----
	assign dout_1__fpga[0] = gfpga_pad_GPIO_PAD[26];

// ----- Blif Benchmark output dout_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[123] -----
	assign dout_2__fpga[0] = gfpga_pad_GPIO_PAD[123];

// ----- Blif Benchmark output dout_3_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[121] -----
	assign dout_3__fpga[0] = gfpga_pad_GPIO_PAD[121];

// ----- Blif Benchmark output dout_4_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[49] -----
	assign dout_4__fpga[0] = gfpga_pad_GPIO_PAD[49];

// ----- Blif Benchmark output dout_5_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[18] -----
	assign dout_5__fpga[0] = gfpga_pad_GPIO_PAD[18];

// ----- Blif Benchmark output dout_6_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[98] -----
	assign dout_6__fpga[0] = gfpga_pad_GPIO_PAD[98];

// ----- Blif Benchmark output dout_7_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[152] -----
	assign dout_7__fpga[0] = gfpga_pad_GPIO_PAD[152];

// ----- Blif Benchmark output dout_8_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[126] -----
	assign dout_8__fpga[0] = gfpga_pad_GPIO_PAD[126];

// ----- Blif Benchmark output dout_9_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[146] -----
	assign dout_9__fpga[0] = gfpga_pad_GPIO_PAD[146];

// ----- Blif Benchmark output dout_10_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[29] -----
	assign dout_10__fpga[0] = gfpga_pad_GPIO_PAD[29];

// ----- Blif Benchmark output dout_11_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[148] -----
	assign dout_11__fpga[0] = gfpga_pad_GPIO_PAD[148];

// ----- Blif Benchmark output dout_12_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[118] -----
	assign dout_12__fpga[0] = gfpga_pad_GPIO_PAD[118];

// ----- Blif Benchmark output dout_13_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[16] -----
	assign dout_13__fpga[0] = gfpga_pad_GPIO_PAD[16];

// ----- Blif Benchmark output dout_14_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[25] -----
	assign dout_14__fpga[0] = gfpga_pad_GPIO_PAD[25];

// ----- Blif Benchmark output dout_15_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[153] -----
	assign dout_15__fpga[0] = gfpga_pad_GPIO_PAD[153];

// ----- Blif Benchmark output dout_16_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[23] -----
	assign dout_16__fpga[0] = gfpga_pad_GPIO_PAD[23];

// ----- Blif Benchmark output dout_17_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[50] -----
	assign dout_17__fpga[0] = gfpga_pad_GPIO_PAD[50];

// ----- Blif Benchmark output dout_18_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[58] -----
	assign dout_18__fpga[0] = gfpga_pad_GPIO_PAD[58];

// ----- Blif Benchmark output dout_19_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[119] -----
	assign dout_19__fpga[0] = gfpga_pad_GPIO_PAD[119];

// ----- Blif Benchmark output dout_20_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[127] -----
	assign dout_20__fpga[0] = gfpga_pad_GPIO_PAD[127];

// ----- Blif Benchmark output dout_21_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[156] -----
	assign dout_21__fpga[0] = gfpga_pad_GPIO_PAD[156];

// ----- Blif Benchmark output dout_22_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[115] -----
	assign dout_22__fpga[0] = gfpga_pad_GPIO_PAD[115];

// ----- Blif Benchmark output dout_23_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[91] -----
	assign dout_23__fpga[0] = gfpga_pad_GPIO_PAD[91];

// ----- Blif Benchmark output dout_24_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[151] -----
	assign dout_24__fpga[0] = gfpga_pad_GPIO_PAD[151];

// ----- Blif Benchmark output dout_25_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[21] -----
	assign dout_25__fpga[0] = gfpga_pad_GPIO_PAD[21];

// ----- Blif Benchmark output dout_26_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[149] -----
	assign dout_26__fpga[0] = gfpga_pad_GPIO_PAD[149];

// ----- Blif Benchmark output dout_27_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[55] -----
	assign dout_27__fpga[0] = gfpga_pad_GPIO_PAD[55];

// ----- Blif Benchmark output dout_28_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[85] -----
	assign dout_28__fpga[0] = gfpga_pad_GPIO_PAD[85];

// ----- Blif Benchmark output dout_29_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[60] -----
	assign dout_29__fpga[0] = gfpga_pad_GPIO_PAD[60];

// ----- Blif Benchmark output dout_30_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[95] -----
	assign dout_30__fpga[0] = gfpga_pad_GPIO_PAD[95];

// ----- Blif Benchmark output dout_31_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[150] -----
	assign dout_31__fpga[0] = gfpga_pad_GPIO_PAD[150];

// ----- Wire unused FPGA I/Os to constants -----
	assign gfpga_pad_GPIO_PAD[0] = 1'b0;
	assign gfpga_pad_GPIO_PAD[1] = 1'b0;
	assign gfpga_pad_GPIO_PAD[2] = 1'b0;
	assign gfpga_pad_GPIO_PAD[3] = 1'b0;
	assign gfpga_pad_GPIO_PAD[4] = 1'b0;
	assign gfpga_pad_GPIO_PAD[5] = 1'b0;
	assign gfpga_pad_GPIO_PAD[6] = 1'b0;
	assign gfpga_pad_GPIO_PAD[7] = 1'b0;
	assign gfpga_pad_GPIO_PAD[8] = 1'b0;
	assign gfpga_pad_GPIO_PAD[9] = 1'b0;
	assign gfpga_pad_GPIO_PAD[10] = 1'b0;
	assign gfpga_pad_GPIO_PAD[11] = 1'b0;
	assign gfpga_pad_GPIO_PAD[12] = 1'b0;
	assign gfpga_pad_GPIO_PAD[13] = 1'b0;
	assign gfpga_pad_GPIO_PAD[14] = 1'b0;
	assign gfpga_pad_GPIO_PAD[15] = 1'b0;
	assign gfpga_pad_GPIO_PAD[32] = 1'b0;
	assign gfpga_pad_GPIO_PAD[33] = 1'b0;
	assign gfpga_pad_GPIO_PAD[34] = 1'b0;
	assign gfpga_pad_GPIO_PAD[35] = 1'b0;
	assign gfpga_pad_GPIO_PAD[36] = 1'b0;
	assign gfpga_pad_GPIO_PAD[37] = 1'b0;
	assign gfpga_pad_GPIO_PAD[38] = 1'b0;
	assign gfpga_pad_GPIO_PAD[39] = 1'b0;
	assign gfpga_pad_GPIO_PAD[40] = 1'b0;
	assign gfpga_pad_GPIO_PAD[41] = 1'b0;
	assign gfpga_pad_GPIO_PAD[42] = 1'b0;
	assign gfpga_pad_GPIO_PAD[43] = 1'b0;
	assign gfpga_pad_GPIO_PAD[44] = 1'b0;
	assign gfpga_pad_GPIO_PAD[45] = 1'b0;
	assign gfpga_pad_GPIO_PAD[46] = 1'b0;
	assign gfpga_pad_GPIO_PAD[47] = 1'b0;
	assign gfpga_pad_GPIO_PAD[48] = 1'b0;
	assign gfpga_pad_GPIO_PAD[52] = 1'b0;
	assign gfpga_pad_GPIO_PAD[57] = 1'b0;
	assign gfpga_pad_GPIO_PAD[59] = 1'b0;
	assign gfpga_pad_GPIO_PAD[62] = 1'b0;
	assign gfpga_pad_GPIO_PAD[64] = 1'b0;
	assign gfpga_pad_GPIO_PAD[65] = 1'b0;
	assign gfpga_pad_GPIO_PAD[66] = 1'b0;
	assign gfpga_pad_GPIO_PAD[67] = 1'b0;
	assign gfpga_pad_GPIO_PAD[68] = 1'b0;
	assign gfpga_pad_GPIO_PAD[69] = 1'b0;
	assign gfpga_pad_GPIO_PAD[70] = 1'b0;
	assign gfpga_pad_GPIO_PAD[71] = 1'b0;
	assign gfpga_pad_GPIO_PAD[72] = 1'b0;
	assign gfpga_pad_GPIO_PAD[73] = 1'b0;
	assign gfpga_pad_GPIO_PAD[74] = 1'b0;
	assign gfpga_pad_GPIO_PAD[75] = 1'b0;
	assign gfpga_pad_GPIO_PAD[76] = 1'b0;
	assign gfpga_pad_GPIO_PAD[77] = 1'b0;
	assign gfpga_pad_GPIO_PAD[78] = 1'b0;
	assign gfpga_pad_GPIO_PAD[79] = 1'b0;
	assign gfpga_pad_GPIO_PAD[80] = 1'b0;
	assign gfpga_pad_GPIO_PAD[81] = 1'b0;
	assign gfpga_pad_GPIO_PAD[82] = 1'b0;
	assign gfpga_pad_GPIO_PAD[83] = 1'b0;
	assign gfpga_pad_GPIO_PAD[84] = 1'b0;
	assign gfpga_pad_GPIO_PAD[86] = 1'b0;
	assign gfpga_pad_GPIO_PAD[87] = 1'b0;
	assign gfpga_pad_GPIO_PAD[88] = 1'b0;
	assign gfpga_pad_GPIO_PAD[89] = 1'b0;
	assign gfpga_pad_GPIO_PAD[90] = 1'b0;
	assign gfpga_pad_GPIO_PAD[92] = 1'b0;
	assign gfpga_pad_GPIO_PAD[94] = 1'b0;
	assign gfpga_pad_GPIO_PAD[112] = 1'b0;
	assign gfpga_pad_GPIO_PAD[113] = 1'b0;
	assign gfpga_pad_GPIO_PAD[114] = 1'b0;
	assign gfpga_pad_GPIO_PAD[116] = 1'b0;
	assign gfpga_pad_GPIO_PAD[117] = 1'b0;
	assign gfpga_pad_GPIO_PAD[120] = 1'b0;
	assign gfpga_pad_GPIO_PAD[122] = 1'b0;
	assign gfpga_pad_GPIO_PAD[124] = 1'b0;
	assign gfpga_pad_GPIO_PAD[125] = 1'b0;
	assign gfpga_pad_GPIO_PAD[128] = 1'b0;
	assign gfpga_pad_GPIO_PAD[129] = 1'b0;
	assign gfpga_pad_GPIO_PAD[130] = 1'b0;
	assign gfpga_pad_GPIO_PAD[131] = 1'b0;
	assign gfpga_pad_GPIO_PAD[132] = 1'b0;
	assign gfpga_pad_GPIO_PAD[133] = 1'b0;
	assign gfpga_pad_GPIO_PAD[134] = 1'b0;
	assign gfpga_pad_GPIO_PAD[135] = 1'b0;
	assign gfpga_pad_GPIO_PAD[136] = 1'b0;
	assign gfpga_pad_GPIO_PAD[137] = 1'b0;
	assign gfpga_pad_GPIO_PAD[138] = 1'b0;
	assign gfpga_pad_GPIO_PAD[139] = 1'b0;
	assign gfpga_pad_GPIO_PAD[140] = 1'b0;
	assign gfpga_pad_GPIO_PAD[141] = 1'b0;
	assign gfpga_pad_GPIO_PAD[143] = 1'b0;

// ----- Reference Benchmark Instanication -------
	dual_port_ram_8x32 REF_DUT(
		.clk(clk),
		.wen(wen_shared_input),
		.ren(ren_shared_input),
		.waddr({waddr_2__shared_input, waddr_1__shared_input, waddr_0__shared_input}),
		.raddr({raddr_2__shared_input, raddr_1__shared_input, raddr_0__shared_input}),
		.din({din_31__shared_input, din_30__shared_input, din_29__shared_input, din_28__shared_input, din_27__shared_input, din_26__shared_input, din_25__shared_input, din_24__shared_input, din_23__shared_input, din_22__shared_input, din_21__shared_input, din_20__shared_input, din_19__shared_input, din_18__shared_input, din_17__shared_input, din_16__shared_input, din_15__shared_input, din_14__shared_input, din_13__shared_input, din_12__shared_input, din_11__shared_input, din_10__shared_input, din_9__shared_input, din_8__shared_input, din_7__shared_input, din_6__shared_input, din_5__shared_input, din_4__shared_input, din_3__shared_input, din_2__shared_input, din_1__shared_input, din_0__shared_input}),
		.dout({dout_31__benchmark, dout_30__benchmark, dout_29__benchmark, dout_28__benchmark, dout_27__benchmark, dout_26__benchmark, dout_25__benchmark, dout_24__benchmark, dout_23__benchmark, dout_22__benchmark, dout_21__benchmark, dout_20__benchmark, dout_19__benchmark, dout_18__benchmark, dout_17__benchmark, dout_16__benchmark, dout_15__benchmark, dout_14__benchmark, dout_13__benchmark, dout_12__benchmark, dout_11__benchmark, dout_10__benchmark, dout_9__benchmark, dout_8__benchmark, dout_7__benchmark, dout_6__benchmark, dout_5__benchmark, dout_4__benchmark, dout_3__benchmark, dout_2__benchmark, dout_1__benchmark, dout_0__benchmark})
	);
// ----- End reference Benchmark Instanication -------

// ----- Begin bitstream loading during configuration phase -----
`define BITSTREAM_LENGTH 7948
`define BITSTREAM_WIDTH 16
// ----- Virtual memory to store the bitstream from external file -----
reg [0:`BITSTREAM_WIDTH - 1] bit_mem[0:`BITSTREAM_LENGTH - 1];
reg [$clog2(`BITSTREAM_LENGTH):0] bit_index;
// ----- Registers used for fast configuration logic -----
reg [$clog2(`BITSTREAM_LENGTH):0] ibit;
reg [0:0] skip_bits;
// ----- Preload bitstream file to a virtual memory -----
initial begin
	$readmemb("fabric_bitstream.bit", bit_mem);
// ----- Configuration chain default input -----
	ccff_head[0:15] <= {16{1'b0}};
	bit_index <= 0;
	skip_bits[0] <= 1'b0;
	for (ibit = 0; ibit < `BITSTREAM_LENGTH + 1; ibit = ibit + 1) begin
		if ({16{1'b0}} == bit_mem[ibit]) begin
			if (1'b1 == skip_bits[0]) begin
				bit_index <= bit_index + 1;
			end
		end else begin
			skip_bits[0] <= 1'b0;
		end
	end
end
// ----- 'else if' condition is required by Modelsim to synthesis the Verilog correctly -----
always @(negedge __prog_clock___reg__[0]) begin
	if (bit_index >= `BITSTREAM_LENGTH) begin
		__config_done__[0] <= 1'b1;
	end else if (bit_index >= 0 && bit_index < `BITSTREAM_LENGTH) begin
		ccff_head[0:15] <= bit_mem[bit_index];
		bit_index <= bit_index + 1;
	end
end
// ----- End bitstream loading during configuration phase -----
// ----- Begin reset signal generation -----
// ----- Input Initialization -------
	initial begin
		wen_shared_input <= 1'b0;
		ren_shared_input <= 1'b0;
		waddr_0__shared_input <= 1'b0;
		waddr_1__shared_input <= 1'b0;
		raddr_0__shared_input <= 1'b0;
		raddr_1__shared_input <= 1'b0;
		din_0__shared_input <= 1'b0;
		din_1__shared_input <= 1'b0;
		din_2__shared_input <= 1'b0;
		din_3__shared_input <= 1'b0;
		din_4__shared_input <= 1'b0;
		din_5__shared_input <= 1'b0;
		din_6__shared_input <= 1'b0;
		din_7__shared_input <= 1'b0;
		din_8__shared_input <= 1'b0;
		din_9__shared_input <= 1'b0;
		din_10__shared_input <= 1'b0;
		din_11__shared_input <= 1'b0;
		din_12__shared_input <= 1'b0;
		din_13__shared_input <= 1'b0;
		din_14__shared_input <= 1'b0;
		din_15__shared_input <= 1'b0;
		din_16__shared_input <= 1'b0;
		din_17__shared_input <= 1'b0;
		din_18__shared_input <= 1'b0;
		din_19__shared_input <= 1'b0;
		din_20__shared_input <= 1'b0;
		din_21__shared_input <= 1'b0;
		din_22__shared_input <= 1'b0;
		din_23__shared_input <= 1'b0;
		din_24__shared_input <= 1'b0;
		din_25__shared_input <= 1'b0;
		din_26__shared_input <= 1'b0;
		din_27__shared_input <= 1'b0;
		din_28__shared_input <= 1'b0;
		din_29__shared_input <= 1'b0;
		din_30__shared_input <= 1'b0;
		din_31__shared_input <= 1'b0;

		dout_0__flag[0] <= 1'b0;
		dout_1__flag[0] <= 1'b0;
		dout_2__flag[0] <= 1'b0;
		dout_3__flag[0] <= 1'b0;
		dout_4__flag[0] <= 1'b0;
		dout_5__flag[0] <= 1'b0;
		dout_6__flag[0] <= 1'b0;
		dout_7__flag[0] <= 1'b0;
		dout_8__flag[0] <= 1'b0;
		dout_9__flag[0] <= 1'b0;
		dout_10__flag[0] <= 1'b0;
		dout_11__flag[0] <= 1'b0;
		dout_12__flag[0] <= 1'b0;
		dout_13__flag[0] <= 1'b0;
		dout_14__flag[0] <= 1'b0;
		dout_15__flag[0] <= 1'b0;
		dout_16__flag[0] <= 1'b0;
		dout_17__flag[0] <= 1'b0;
		dout_18__flag[0] <= 1'b0;
		dout_19__flag[0] <= 1'b0;
		dout_20__flag[0] <= 1'b0;
		dout_21__flag[0] <= 1'b0;
		dout_22__flag[0] <= 1'b0;
		dout_23__flag[0] <= 1'b0;
		dout_24__flag[0] <= 1'b0;
		dout_25__flag[0] <= 1'b0;
		dout_26__flag[0] <= 1'b0;
		dout_27__flag[0] <= 1'b0;
		dout_28__flag[0] <= 1'b0;
		dout_29__flag[0] <= 1'b0;
		dout_30__flag[0] <= 1'b0;
		dout_31__flag[0] <= 1'b0;
	end

// ----- Begin memory-initialization trigger sequence -----
// ----- Maps to the "Step 1: Trigger Preload Phase" stimulus of dpram_8x16_preload_tb.v: -----
// ----- init_start is asserted, then held until init_done rises, then de-asserted one clock later. -----
// ----- Adapted here to wait for BOTH embedded dpram_8x16_preload memories (mem_init_done[0] and mem_init_done[1]) to finish. -----
	reg [0:0] mem_init_start_reg;
initial begin
	mem_init_start_reg[0] = 1'b0;
	wait (__prog_reset__[0] === 1'b0);
	$display("[TB] Triggering preload memory initialization for both embedded RAMs (mem_init_clk running, mem_init_addr sweeping)...");
	mem_init_start_reg[0] = 1'b1;
	wait (gfpga_pad_dpram_8x16_preload_mem_init_done[0] === 1'b1 && gfpga_pad_frac_mem_256_preload_mem_init_done[0] === 1'b1);
	@(posedge mem_init_clk[0]); #1;
	mem_init_start_reg[0] = 1'b0;
	$display("[TB] Preload memory initialization complete for both embedded RAMs!");
end
// ----- End memory-initialization trigger sequence -----

// ----- Input Stimulus -------
// ----- Adapted from the Step 2 / Step 3 stimulus of dpram_8x16_preload_tb.v (LINE100-178): -----
// -----   waddr_*__shared_input  <-> sys_waddr -----
// -----   raddr_*__shared_input  <-> sys_raddr -----
// -----   din_*__shared_input    <-> sys_d_in -----
// ----- Runs once __config_all_done__ is asserted (bitstream configured AND both embedded RAMs preloaded). -----
// ----- KEY TIMING: the checker skips the FIRST negedge clk after __config_all_done__ (sim_start handshake) and compares from the SECOND negedge onwards. -----
// ----- Therefore ren/raddr must be asserted IMMEDIATELY when __config_all_done__ goes high (before any negedge), so they are stable a full cycle before the first real compare. -----
// ----- The checker skips negedge #1 (sim_start handshake) and compares from negedge #2. -----
	wire [0:31] dout_fpga_bus = {dout_31__fpga, dout_30__fpga, dout_29__fpga, dout_28__fpga,
	                             dout_27__fpga, dout_26__fpga, dout_25__fpga, dout_24__fpga,
	                             dout_23__fpga, dout_22__fpga, dout_21__fpga, dout_20__fpga,
	                             dout_19__fpga, dout_18__fpga, dout_17__fpga, dout_16__fpga,
	                             dout_15__fpga, dout_14__fpga, dout_13__fpga, dout_12__fpga,
	                             dout_11__fpga, dout_10__fpga, dout_9__fpga,  dout_8__fpga,
	                             dout_7__fpga,  dout_6__fpga,  dout_5__fpga,  dout_4__fpga,
	                             dout_3__fpga,  dout_2__fpga,  dout_1__fpga,  dout_0__fpga};


initial begin
	wait (__config_all_done__[0] === 1'b1);

	// ---- Assert ren + first raddr IMMEDIATELY (no clock edge wait) so they are stable before the checker's sim_start negedge ----
	$display("[TB] Reading back initialized values via system fabric...");
	ren_shared_input = 1'b1;
	{raddr_1__shared_input, raddr_0__shared_input} = 3'b000;

	// ---- negedge #1: checker consumes sim_start (no compare yet); advance raddr for next cycle ----
	@(negedge clk[0]);
	$display("Addr 0 | Got: %h", dout_fpga_bus);
	{raddr_1__shared_input, raddr_0__shared_input} = 2'b00;

	// ---- Step 2: Read back and verify all preloaded values ----
	@(negedge clk[0]);
	$display("Addr 1 | Got: %h", dout_fpga_bus);
	{raddr_1__shared_input, raddr_0__shared_input} = 2'b01;

	@(negedge clk[0]);
	$display("Addr 2 | Got: %h", dout_fpga_bus);
	{raddr_1__shared_input, raddr_0__shared_input} = 2'b10;

	@(negedge clk[0]);
	$display("Addr 3 | Got: %h", dout_fpga_bus);
	{raddr_1__shared_input, raddr_0__shared_input} = 2'b11;

	@(negedge clk[0]);

	// ---- Step 3: Normal system write then readback ----
	$display("[TB] Testing standard system runtime write/read...");
	wen_shared_input = 1'b1;
	{waddr_1__shared_input, waddr_0__shared_input} = 2'b10;
	{din_31__shared_input, din_30__shared_input, din_29__shared_input, din_28__shared_input,
	 din_27__shared_input, din_26__shared_input, din_25__shared_input, din_24__shared_input,
	 din_23__shared_input, din_22__shared_input, din_21__shared_input, din_20__shared_input,
	 din_19__shared_input, din_18__shared_input, din_17__shared_input, din_16__shared_input,
	 din_15__shared_input, din_14__shared_input, din_13__shared_input, din_12__shared_input,
	 din_11__shared_input, din_10__shared_input, din_9__shared_input,  din_8__shared_input,
	 din_7__shared_input,  din_6__shared_input,  din_5__shared_input,  din_4__shared_input,
	 din_3__shared_input,  din_2__shared_input,  din_1__shared_input,  din_0__shared_input} = 32'hDEADBEEF;

	@(negedge clk[0]);
	wen_shared_input = 1'b0;
	ren_shared_input = 1'b1;
	{raddr_1__shared_input, raddr_0__shared_input} = 2'b10;

	@(negedge clk[0]);
	$display("Addr 2 (post-write) | Expected: DEADBEEF | Got: %h", dout_fpga_bus);
	ren_shared_input = 1'b0;

	repeat (3) @(negedge clk[0]);
	$display("[TB] All tests complete.");
end

// ----- Begin checking output vectors -------
// ----- Skip the first falling edge of clock, it is for initialization -------
	reg [0:0] sim_start;

	always@(negedge clk[0]) begin
		if (1'b1 == sim_start[0]) begin
			sim_start[0] <= ~sim_start[0];
		end else 
			if (1'b1 == __config_all_done__) begin
			if(!(dout_0__fpga === dout_0__benchmark) && !(dout_0__benchmark === 1'bx)) begin
				dout_0__flag <= 1'b1;
			end else begin
				dout_0__flag<= 1'b0;
			end
			if(!(dout_1__fpga === dout_1__benchmark) && !(dout_1__benchmark === 1'bx)) begin
				dout_1__flag <= 1'b1;
			end else begin
				dout_1__flag<= 1'b0;
			end
			if(!(dout_2__fpga === dout_2__benchmark) && !(dout_2__benchmark === 1'bx)) begin
				dout_2__flag <= 1'b1;
			end else begin
				dout_2__flag<= 1'b0;
			end
			if(!(dout_3__fpga === dout_3__benchmark) && !(dout_3__benchmark === 1'bx)) begin
				dout_3__flag <= 1'b1;
			end else begin
				dout_3__flag<= 1'b0;
			end
			if(!(dout_4__fpga === dout_4__benchmark) && !(dout_4__benchmark === 1'bx)) begin
				dout_4__flag <= 1'b1;
			end else begin
				dout_4__flag<= 1'b0;
			end
			if(!(dout_5__fpga === dout_5__benchmark) && !(dout_5__benchmark === 1'bx)) begin
				dout_5__flag <= 1'b1;
			end else begin
				dout_5__flag<= 1'b0;
			end
			if(!(dout_6__fpga === dout_6__benchmark) && !(dout_6__benchmark === 1'bx)) begin
				dout_6__flag <= 1'b1;
			end else begin
				dout_6__flag<= 1'b0;
			end
			if(!(dout_7__fpga === dout_7__benchmark) && !(dout_7__benchmark === 1'bx)) begin
				dout_7__flag <= 1'b1;
			end else begin
				dout_7__flag<= 1'b0;
			end
			if(!(dout_8__fpga === dout_8__benchmark) && !(dout_8__benchmark === 1'bx)) begin
				dout_8__flag <= 1'b1;
			end else begin
				dout_8__flag<= 1'b0;
			end
			if(!(dout_9__fpga === dout_9__benchmark) && !(dout_9__benchmark === 1'bx)) begin
				dout_9__flag <= 1'b1;
			end else begin
				dout_9__flag<= 1'b0;
			end
			if(!(dout_10__fpga === dout_10__benchmark) && !(dout_10__benchmark === 1'bx)) begin
				dout_10__flag <= 1'b1;
			end else begin
				dout_10__flag<= 1'b0;
			end
			if(!(dout_11__fpga === dout_11__benchmark) && !(dout_11__benchmark === 1'bx)) begin
				dout_11__flag <= 1'b1;
			end else begin
				dout_11__flag<= 1'b0;
			end
			if(!(dout_12__fpga === dout_12__benchmark) && !(dout_12__benchmark === 1'bx)) begin
				dout_12__flag <= 1'b1;
			end else begin
				dout_12__flag<= 1'b0;
			end
			if(!(dout_13__fpga === dout_13__benchmark) && !(dout_13__benchmark === 1'bx)) begin
				dout_13__flag <= 1'b1;
			end else begin
				dout_13__flag<= 1'b0;
			end
			if(!(dout_14__fpga === dout_14__benchmark) && !(dout_14__benchmark === 1'bx)) begin
				dout_14__flag <= 1'b1;
			end else begin
				dout_14__flag<= 1'b0;
			end
			if(!(dout_15__fpga === dout_15__benchmark) && !(dout_15__benchmark === 1'bx)) begin
				dout_15__flag <= 1'b1;
			end else begin
				dout_15__flag<= 1'b0;
			end
			if(!(dout_16__fpga === dout_16__benchmark) && !(dout_16__benchmark === 1'bx)) begin
				dout_16__flag <= 1'b1;
			end else begin
				dout_16__flag<= 1'b0;
			end
			if(!(dout_17__fpga === dout_17__benchmark) && !(dout_17__benchmark === 1'bx)) begin
				dout_17__flag <= 1'b1;
			end else begin
				dout_17__flag<= 1'b0;
			end
			if(!(dout_18__fpga === dout_18__benchmark) && !(dout_18__benchmark === 1'bx)) begin
				dout_18__flag <= 1'b1;
			end else begin
				dout_18__flag<= 1'b0;
			end
			if(!(dout_19__fpga === dout_19__benchmark) && !(dout_19__benchmark === 1'bx)) begin
				dout_19__flag <= 1'b1;
			end else begin
				dout_19__flag<= 1'b0;
			end
			if(!(dout_20__fpga === dout_20__benchmark) && !(dout_20__benchmark === 1'bx)) begin
				dout_20__flag <= 1'b1;
			end else begin
				dout_20__flag<= 1'b0;
			end
			if(!(dout_21__fpga === dout_21__benchmark) && !(dout_21__benchmark === 1'bx)) begin
				dout_21__flag <= 1'b1;
			end else begin
				dout_21__flag<= 1'b0;
			end
			if(!(dout_22__fpga === dout_22__benchmark) && !(dout_22__benchmark === 1'bx)) begin
				dout_22__flag <= 1'b1;
			end else begin
				dout_22__flag<= 1'b0;
			end
			if(!(dout_23__fpga === dout_23__benchmark) && !(dout_23__benchmark === 1'bx)) begin
				dout_23__flag <= 1'b1;
			end else begin
				dout_23__flag<= 1'b0;
			end
			if(!(dout_24__fpga === dout_24__benchmark) && !(dout_24__benchmark === 1'bx)) begin
				dout_24__flag <= 1'b1;
			end else begin
				dout_24__flag<= 1'b0;
			end
			if(!(dout_25__fpga === dout_25__benchmark) && !(dout_25__benchmark === 1'bx)) begin
				dout_25__flag <= 1'b1;
			end else begin
				dout_25__flag<= 1'b0;
			end
			if(!(dout_26__fpga === dout_26__benchmark) && !(dout_26__benchmark === 1'bx)) begin
				dout_26__flag <= 1'b1;
			end else begin
				dout_26__flag<= 1'b0;
			end
			if(!(dout_27__fpga === dout_27__benchmark) && !(dout_27__benchmark === 1'bx)) begin
				dout_27__flag <= 1'b1;
			end else begin
				dout_27__flag<= 1'b0;
			end
			if(!(dout_28__fpga === dout_28__benchmark) && !(dout_28__benchmark === 1'bx)) begin
				dout_28__flag <= 1'b1;
			end else begin
				dout_28__flag<= 1'b0;
			end
			if(!(dout_29__fpga === dout_29__benchmark) && !(dout_29__benchmark === 1'bx)) begin
				dout_29__flag <= 1'b1;
			end else begin
				dout_29__flag<= 1'b0;
			end
			if(!(dout_30__fpga === dout_30__benchmark) && !(dout_30__benchmark === 1'bx)) begin
				dout_30__flag <= 1'b1;
			end else begin
				dout_30__flag<= 1'b0;
			end
			if(!(dout_31__fpga === dout_31__benchmark) && !(dout_31__benchmark === 1'bx)) begin
				dout_31__flag <= 1'b1;
			end else begin
				dout_31__flag<= 1'b0;
			end
		end
	end

	always@(posedge dout_0__flag) begin
		if(dout_0__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_0__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_1__flag) begin
		if(dout_1__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_1__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_2__flag) begin
		if(dout_2__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_2__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_3__flag) begin
		if(dout_3__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_3__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_4__flag) begin
		if(dout_4__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_4__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_5__flag) begin
		if(dout_5__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_5__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_6__flag) begin
		if(dout_6__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_6__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_7__flag) begin
		if(dout_7__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_7__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_8__flag) begin
		if(dout_8__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_8__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_9__flag) begin
		if(dout_9__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_9__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_10__flag) begin
		if(dout_10__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_10__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_11__flag) begin
		if(dout_11__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_11__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_12__flag) begin
		if(dout_12__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_12__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_13__flag) begin
		if(dout_13__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_13__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_14__flag) begin
		if(dout_14__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_14__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_15__flag) begin
		if(dout_15__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_15__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_16__flag) begin
		if(dout_16__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_16__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_17__flag) begin
		if(dout_17__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_17__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_18__flag) begin
		if(dout_18__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_18__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_19__flag) begin
		if(dout_19__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_19__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_20__flag) begin
		if(dout_20__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_20__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_21__flag) begin
		if(dout_21__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_21__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_22__flag) begin
		if(dout_22__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_22__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_23__flag) begin
		if(dout_23__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_23__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_24__flag) begin
		if(dout_24__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_24__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_25__flag) begin
		if(dout_25__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_25__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_26__flag) begin
		if(dout_26__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_26__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_27__flag) begin
		if(dout_27__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_27__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_28__flag) begin
		if(dout_28__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_28__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_29__flag) begin
		if(dout_29__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_29__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_30__flag) begin
		if(dout_30__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_30__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout_31__flag) begin
		if(dout_31__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout_31__fpga at time = %t", $realtime);
		end
	end


// ----- Configuration done must be raised in the end -------
	always@(posedge __config_all_done__[0]) begin
		nb_error = nb_error - 1;
	end

// ----- Begin output waveform to VCD file-------
	initial begin
		$dumpfile("dual_port_ram_8x32_formal.vcd");
		$dumpvars(1, dual_port_ram_8x32_autocheck_top_tb);
	end
// ----- END output waveform to VCD file -------

initial begin
	sim_start[0] <= 1'b1;
	$timeformat(-9, 2, "ns", 20);
	$display("Simulation start");
// ----- Can be changed by the user for his/her need -------
	#26528
	if(nb_error == 0) begin
		$display("Simulation Succeed");
	end else begin
		$display("Simulation Failed with %d error(s)", nb_error);
	end
	$finish;
end

endmodule
// ----- END Verilog module for dual_port_ram_8x32_autocheck_top_tb -----

//----- Default net type -----
`default_nettype wire

