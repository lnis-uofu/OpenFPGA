//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: FPGA Verilog full testbench for top-level netlist of design: dual_port_ram_8x16_8x32
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Mon Jul 27 12:41:51 2026
//-------------------------------------------
//----- Default net type -----
`default_nettype none

module dual_port_ram_8x16_8x32_autocheck_top_tb;
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
	reg [0:0] waddr8x16_0__shared_input;
	reg [0:0] waddr8x16_1__shared_input;
	reg [0:0] waddr8x16_2__shared_input;
	reg [0:0] raddr8x16_0__shared_input;
	reg [0:0] raddr8x16_1__shared_input;
	reg [0:0] raddr8x16_2__shared_input;
	reg [0:0] din8x16_0__shared_input;
	reg [0:0] din8x16_1__shared_input;
	reg [0:0] din8x16_2__shared_input;
	reg [0:0] din8x16_3__shared_input;
	reg [0:0] din8x16_4__shared_input;
	reg [0:0] din8x16_5__shared_input;
	reg [0:0] din8x16_6__shared_input;
	reg [0:0] din8x16_7__shared_input;
	reg [0:0] din8x16_8__shared_input;
	reg [0:0] din8x16_9__shared_input;
	reg [0:0] din8x16_10__shared_input;
	reg [0:0] din8x16_11__shared_input;
	reg [0:0] din8x16_12__shared_input;
	reg [0:0] din8x16_13__shared_input;
	reg [0:0] din8x16_14__shared_input;
	reg [0:0] din8x16_15__shared_input;
	reg [0:0] waddr8x32_0__shared_input;
	reg [0:0] waddr8x32_1__shared_input;
	reg [0:0] raddr8x32_0__shared_input;
	reg [0:0] raddr8x32_1__shared_input;
	reg [0:0] din8x32_0__shared_input;
	reg [0:0] din8x32_1__shared_input;
	reg [0:0] din8x32_2__shared_input;
	reg [0:0] din8x32_3__shared_input;
	reg [0:0] din8x32_4__shared_input;
	reg [0:0] din8x32_5__shared_input;
	reg [0:0] din8x32_6__shared_input;
	reg [0:0] din8x32_7__shared_input;
	reg [0:0] din8x32_8__shared_input;
	reg [0:0] din8x32_9__shared_input;
	reg [0:0] din8x32_10__shared_input;
	reg [0:0] din8x32_11__shared_input;
	reg [0:0] din8x32_12__shared_input;
	reg [0:0] din8x32_13__shared_input;
	reg [0:0] din8x32_14__shared_input;
	reg [0:0] din8x32_15__shared_input;
	reg [0:0] din8x32_16__shared_input;
	reg [0:0] din8x32_17__shared_input;
	reg [0:0] din8x32_18__shared_input;
	reg [0:0] din8x32_19__shared_input;
	reg [0:0] din8x32_20__shared_input;
	reg [0:0] din8x32_21__shared_input;
	reg [0:0] din8x32_22__shared_input;
	reg [0:0] din8x32_23__shared_input;
	reg [0:0] din8x32_24__shared_input;
	reg [0:0] din8x32_25__shared_input;
	reg [0:0] din8x32_26__shared_input;
	reg [0:0] din8x32_27__shared_input;
	reg [0:0] din8x32_28__shared_input;
	reg [0:0] din8x32_29__shared_input;
	reg [0:0] din8x32_30__shared_input;
	reg [0:0] din8x32_31__shared_input;

// ----- FPGA fabric outputs -------
	wire [0:0] dout8x16_0__fpga;
	wire [0:0] dout8x16_1__fpga;
	wire [0:0] dout8x16_2__fpga;
	wire [0:0] dout8x16_3__fpga;
	wire [0:0] dout8x16_4__fpga;
	wire [0:0] dout8x16_5__fpga;
	wire [0:0] dout8x16_6__fpga;
	wire [0:0] dout8x16_7__fpga;
	wire [0:0] dout8x16_8__fpga;
	wire [0:0] dout8x16_9__fpga;
	wire [0:0] dout8x16_10__fpga;
	wire [0:0] dout8x16_11__fpga;
	wire [0:0] dout8x16_12__fpga;
	wire [0:0] dout8x16_13__fpga;
	wire [0:0] dout8x16_14__fpga;
	wire [0:0] dout8x16_15__fpga;
	wire [0:0] dout8x32_0__fpga;
	wire [0:0] dout8x32_1__fpga;
	wire [0:0] dout8x32_2__fpga;
	wire [0:0] dout8x32_3__fpga;
	wire [0:0] dout8x32_4__fpga;
	wire [0:0] dout8x32_5__fpga;
	wire [0:0] dout8x32_6__fpga;
	wire [0:0] dout8x32_7__fpga;
	wire [0:0] dout8x32_8__fpga;
	wire [0:0] dout8x32_9__fpga;
	wire [0:0] dout8x32_10__fpga;
	wire [0:0] dout8x32_11__fpga;
	wire [0:0] dout8x32_12__fpga;
	wire [0:0] dout8x32_13__fpga;
	wire [0:0] dout8x32_14__fpga;
	wire [0:0] dout8x32_15__fpga;
	wire [0:0] dout8x32_16__fpga;
	wire [0:0] dout8x32_17__fpga;
	wire [0:0] dout8x32_18__fpga;
	wire [0:0] dout8x32_19__fpga;
	wire [0:0] dout8x32_20__fpga;
	wire [0:0] dout8x32_21__fpga;
	wire [0:0] dout8x32_22__fpga;
	wire [0:0] dout8x32_23__fpga;
	wire [0:0] dout8x32_24__fpga;
	wire [0:0] dout8x32_25__fpga;
	wire [0:0] dout8x32_26__fpga;
	wire [0:0] dout8x32_27__fpga;
	wire [0:0] dout8x32_28__fpga;
	wire [0:0] dout8x32_29__fpga;
	wire [0:0] dout8x32_30__fpga;
	wire [0:0] dout8x32_31__fpga;

// ----- Benchmark outputs -------
	wire [0:0] dout8x16_0__benchmark;
	wire [0:0] dout8x16_1__benchmark;
	wire [0:0] dout8x16_2__benchmark;
	wire [0:0] dout8x16_3__benchmark;
	wire [0:0] dout8x16_4__benchmark;
	wire [0:0] dout8x16_5__benchmark;
	wire [0:0] dout8x16_6__benchmark;
	wire [0:0] dout8x16_7__benchmark;
	wire [0:0] dout8x16_8__benchmark;
	wire [0:0] dout8x16_9__benchmark;
	wire [0:0] dout8x16_10__benchmark;
	wire [0:0] dout8x16_11__benchmark;
	wire [0:0] dout8x16_12__benchmark;
	wire [0:0] dout8x16_13__benchmark;
	wire [0:0] dout8x16_14__benchmark;
	wire [0:0] dout8x16_15__benchmark;
	wire [0:0] dout8x32_0__benchmark;
	wire [0:0] dout8x32_1__benchmark;
	wire [0:0] dout8x32_2__benchmark;
	wire [0:0] dout8x32_3__benchmark;
	wire [0:0] dout8x32_4__benchmark;
	wire [0:0] dout8x32_5__benchmark;
	wire [0:0] dout8x32_6__benchmark;
	wire [0:0] dout8x32_7__benchmark;
	wire [0:0] dout8x32_8__benchmark;
	wire [0:0] dout8x32_9__benchmark;
	wire [0:0] dout8x32_10__benchmark;
	wire [0:0] dout8x32_11__benchmark;
	wire [0:0] dout8x32_12__benchmark;
	wire [0:0] dout8x32_13__benchmark;
	wire [0:0] dout8x32_14__benchmark;
	wire [0:0] dout8x32_15__benchmark;
	wire [0:0] dout8x32_16__benchmark;
	wire [0:0] dout8x32_17__benchmark;
	wire [0:0] dout8x32_18__benchmark;
	wire [0:0] dout8x32_19__benchmark;
	wire [0:0] dout8x32_20__benchmark;
	wire [0:0] dout8x32_21__benchmark;
	wire [0:0] dout8x32_22__benchmark;
	wire [0:0] dout8x32_23__benchmark;
	wire [0:0] dout8x32_24__benchmark;
	wire [0:0] dout8x32_25__benchmark;
	wire [0:0] dout8x32_26__benchmark;
	wire [0:0] dout8x32_27__benchmark;
	wire [0:0] dout8x32_28__benchmark;
	wire [0:0] dout8x32_29__benchmark;
	wire [0:0] dout8x32_30__benchmark;
	wire [0:0] dout8x32_31__benchmark;

// ----- Output vectors checking flags -------
	reg [0:0] dout8x16_0__flag;
	reg [0:0] dout8x16_1__flag;
	reg [0:0] dout8x16_2__flag;
	reg [0:0] dout8x16_3__flag;
	reg [0:0] dout8x16_4__flag;
	reg [0:0] dout8x16_5__flag;
	reg [0:0] dout8x16_6__flag;
	reg [0:0] dout8x16_7__flag;
	reg [0:0] dout8x16_8__flag;
	reg [0:0] dout8x16_9__flag;
	reg [0:0] dout8x16_10__flag;
	reg [0:0] dout8x16_11__flag;
	reg [0:0] dout8x16_12__flag;
	reg [0:0] dout8x16_13__flag;
	reg [0:0] dout8x16_14__flag;
	reg [0:0] dout8x16_15__flag;
	reg [0:0] dout8x32_0__flag;
	reg [0:0] dout8x32_1__flag;
	reg [0:0] dout8x32_2__flag;
	reg [0:0] dout8x32_3__flag;
	reg [0:0] dout8x32_4__flag;
	reg [0:0] dout8x32_5__flag;
	reg [0:0] dout8x32_6__flag;
	reg [0:0] dout8x32_7__flag;
	reg [0:0] dout8x32_8__flag;
	reg [0:0] dout8x32_9__flag;
	reg [0:0] dout8x32_10__flag;
	reg [0:0] dout8x32_11__flag;
	reg [0:0] dout8x32_12__flag;
	reg [0:0] dout8x32_13__flag;
	reg [0:0] dout8x32_14__flag;
	reg [0:0] dout8x32_15__flag;
	reg [0:0] dout8x32_16__flag;
	reg [0:0] dout8x32_17__flag;
	reg [0:0] dout8x32_18__flag;
	reg [0:0] dout8x32_19__flag;
	reg [0:0] dout8x32_20__flag;
	reg [0:0] dout8x32_21__flag;
	reg [0:0] dout8x32_22__flag;
	reg [0:0] dout8x32_23__flag;
	reg [0:0] dout8x32_24__flag;
	reg [0:0] dout8x32_25__flag;
	reg [0:0] dout8x32_26__flag;
	reg [0:0] dout8x32_27__flag;
	reg [0:0] dout8x32_28__flag;
	reg [0:0] dout8x32_29__flag;
	reg [0:0] dout8x32_30__flag;
	reg [0:0] dout8x32_31__flag;

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

// ----- mem_init_done signals are single-cycle pulses. Register as sticky latches -----
// ----- so __config_all_done__ can safely sample them regardless of when config_done arrives -----
	reg [0:0] mem_init_done_dpram_sticky;
	reg [0:0] mem_init_done_frac_sticky;
always @(posedge mem_init_clk[0] or negedge mem_init_rst_n[0]) begin
	if (1'b0 == mem_init_rst_n[0]) begin
		mem_init_done_dpram_sticky[0] <= 1'b0;
		mem_init_done_frac_sticky[0]  <= 1'b0;
	end else begin
		if (gfpga_pad_dpram_8x16_preload_mem_init_done[0]) mem_init_done_dpram_sticky[0] <= 1'b1;
		if (gfpga_pad_frac_mem_256_preload_mem_init_done[0]) mem_init_done_frac_sticky[0]  <= 1'b1;
	end
end
// ----- __config_all_done__ requires bitstream config AND both memory inits to be done -----
	assign __config_all_done__[0] = __config_done__[0] & mem_init_done_dpram_sticky[0] & mem_init_done_frac_sticky[0];
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
	wait(__config_done__)
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
// ----- mem_init_rst_n driven by __prog_reset__ (active-low, released when programming reset drops) -----
	assign mem_init_rst_n[0] = ~__prog_reset__[0];
	assign set[0] = __gset__[0];
// ----- mem_init_clk driven directly by __prog_clock__ (runs concurrently with bitstream configuration) -----
	assign mem_init_clk[0] = __prog_clock__[0];
// ----- mem_init_start driven by registered signal -----
	assign mem_init_start[0] = mem_init_start_reg[0];
// ----- mem128_init_addr driven by 3-bit TB address counter for dpram_8x16_preload -----
	assign mem128_init_addr[0:2] = mem128_init_addr_reg[0:2];
// ----- mem256_init_addr driven by 4-bit TB address counter for frac_mem_256_preload -----
	assign mem256_init_addr[0:3] = mem256_init_addr_reg[0:3];
// ----- End connecting global ports of FPGA fabric to stimuli -----

// ----- Begin mem128_init_addr counter (3-bit, for dpram_8x16_preload, 8 addresses) -----
// ----- FSM takes 2 preload_clk edges after init_start before first write (IDLE->PREPARE->WRITE_DATA) -----
// ----- Initialise to 3'h6 (-2 mod 8) so counter reaches 0 exactly when FSM addr_counter writes row 0 -----
	reg [0:2] mem128_init_addr_reg;
always @(posedge mem_init_clk[0] or negedge mem_init_rst_n[0]) begin
	if (1'b0 == mem_init_rst_n[0]) begin
		mem128_init_addr_reg <= 3'h6;
	end else if (1'b0 == mem_init_start[0]) begin
		mem128_init_addr_reg <= 3'h6;
	end else begin
		mem128_init_addr_reg <= mem128_init_addr_reg + 3'h1;
	end
end
// ----- End mem128_init_addr counter -----

// ----- Begin mem256_init_addr counter (4-bit, for frac_mem_256_preload, 16 addresses) -----
// ----- Same -2 offset: initialise to 4'hE (-2 mod 16) -----
	reg [0:3] mem256_init_addr_reg;
always @(posedge mem_init_clk[0] or negedge mem_init_rst_n[0]) begin
	if (1'b0 == mem_init_rst_n[0]) begin
		mem256_init_addr_reg <= 4'hE;
	end else if (1'b0 == mem_init_start[0]) begin
		mem256_init_addr_reg <= 4'hE;
	end else begin
		mem256_init_addr_reg <= mem256_init_addr_reg + 4'h1;
	end
end
// ----- End mem256_init_addr counter -----

// ----- Begin MIF ROM -----
// ----- Loaded from external file using $readmemh (native @ADDR DATA sparse format) -----
// ----- Each 32-bit word: bits [0:15] -> dpram_8x16_preload, bits [16:31] -> frac_mem_256_preload -----
// ----- mem128 uses rows 0-7 (3-bit addr); mem256 uses all 16 rows (4-bit addr) -----
// ----- NOTE: update filename below if your MIF is named differently -----
`define MEM_INIT_MIF "ram_mif.mem"
	reg [0:31] mem_init_rom [0:15];
initial begin
	$readmemh(`MEM_INIT_MIF, mem_init_rom);
end
	assign gfpga_pad_dpram_8x16_preload_mem_init_data[0:15]   = mem_init_rom[mem128_init_addr[0:2]][0:15];
	assign gfpga_pad_frac_mem_256_preload_mem_init_data[0:15] = mem_init_rom[mem256_init_addr[0:3]][16:31];
// ----- End MIF ROM -----

// ----- Begin mem_init_start trigger sequence -----
// ----- Assert once __prog_reset__ releases; wait for BOTH sticky done latches; then deassert -----
	reg [0:0] mem_init_start_reg;
initial begin
	mem_init_start_reg[0] = 1'b0;
	wait (__prog_reset__[0] === 1'b0);
	$display("[TB] Triggering memory initialization for dpram_8x16_preload and frac_mem_256_preload...");
	mem_init_start_reg[0] = 1'b1;
	wait (mem_init_done_dpram_sticky[0] === 1'b1 && mem_init_done_frac_sticky[0] === 1'b1);
	@(posedge mem_init_clk[0]); #1;
	mem_init_start_reg[0] = 1'b0;
	$display("[TB] Memory initialization complete for both memories!");
end
// ----- End mem_init_start trigger sequence -----
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

// ----- mem_init_data buses are driven by the MIF ROM above -----
// ----- Link BLIF Benchmark I/Os to FPGA I/Os -----
// ----- Blif Benchmark input clk is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[11] -----
	assign gfpga_pad_GPIO_PAD[11] = clk[0];

// ----- Blif Benchmark input wen is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[125] -----
	assign gfpga_pad_GPIO_PAD[125] = wen_shared_input[0];

// ----- Blif Benchmark input ren is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[87] -----
	assign gfpga_pad_GPIO_PAD[87] = ren_shared_input[0];

// ----- Blif Benchmark input waddr8x16_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[130] -----
	assign gfpga_pad_GPIO_PAD[130] = waddr8x16_0__shared_input[0];

// ----- Blif Benchmark input waddr8x16_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[107] -----
	assign gfpga_pad_GPIO_PAD[107] = waddr8x16_1__shared_input[0];

// ----- Blif Benchmark input waddr8x16_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[97] -----
	assign gfpga_pad_GPIO_PAD[97] = waddr8x16_2__shared_input[0];

// ----- Blif Benchmark input raddr8x16_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[84] -----
	assign gfpga_pad_GPIO_PAD[84] = raddr8x16_0__shared_input[0];

// ----- Blif Benchmark input raddr8x16_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[98] -----
	assign gfpga_pad_GPIO_PAD[98] = raddr8x16_1__shared_input[0];

// ----- Blif Benchmark input raddr8x16_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[96] -----
	assign gfpga_pad_GPIO_PAD[96] = raddr8x16_2__shared_input[0];

// ----- Blif Benchmark input din8x16_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[114] -----
	assign gfpga_pad_GPIO_PAD[114] = din8x16_0__shared_input[0];

// ----- Blif Benchmark input din8x16_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[80] -----
	assign gfpga_pad_GPIO_PAD[80] = din8x16_1__shared_input[0];

// ----- Blif Benchmark input din8x16_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[100] -----
	assign gfpga_pad_GPIO_PAD[100] = din8x16_2__shared_input[0];

// ----- Blif Benchmark input din8x16_3_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[103] -----
	assign gfpga_pad_GPIO_PAD[103] = din8x16_3__shared_input[0];

// ----- Blif Benchmark input din8x16_4_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[122] -----
	assign gfpga_pad_GPIO_PAD[122] = din8x16_4__shared_input[0];

// ----- Blif Benchmark input din8x16_5_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[110] -----
	assign gfpga_pad_GPIO_PAD[110] = din8x16_5__shared_input[0];

// ----- Blif Benchmark input din8x16_6_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[102] -----
	assign gfpga_pad_GPIO_PAD[102] = din8x16_6__shared_input[0];

// ----- Blif Benchmark input din8x16_7_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[101] -----
	assign gfpga_pad_GPIO_PAD[101] = din8x16_7__shared_input[0];

// ----- Blif Benchmark input din8x16_8_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[109] -----
	assign gfpga_pad_GPIO_PAD[109] = din8x16_8__shared_input[0];

// ----- Blif Benchmark input din8x16_9_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[99] -----
	assign gfpga_pad_GPIO_PAD[99] = din8x16_9__shared_input[0];

// ----- Blif Benchmark input din8x16_10_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[105] -----
	assign gfpga_pad_GPIO_PAD[105] = din8x16_10__shared_input[0];

// ----- Blif Benchmark input din8x16_11_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[88] -----
	assign gfpga_pad_GPIO_PAD[88] = din8x16_11__shared_input[0];

// ----- Blif Benchmark input din8x16_12_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[81] -----
	assign gfpga_pad_GPIO_PAD[81] = din8x16_12__shared_input[0];

// ----- Blif Benchmark input din8x16_13_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[111] -----
	assign gfpga_pad_GPIO_PAD[111] = din8x16_13__shared_input[0];

// ----- Blif Benchmark input din8x16_14_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[106] -----
	assign gfpga_pad_GPIO_PAD[106] = din8x16_14__shared_input[0];

// ----- Blif Benchmark input din8x16_15_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[135] -----
	assign gfpga_pad_GPIO_PAD[135] = din8x16_15__shared_input[0];

// ----- Blif Benchmark input waddr8x32_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[16] -----
	assign gfpga_pad_GPIO_PAD[16] = waddr8x32_0__shared_input[0];

// ----- Blif Benchmark input waddr8x32_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[25] -----
	assign gfpga_pad_GPIO_PAD[25] = waddr8x32_1__shared_input[0];

// ----- Blif Benchmark input raddr8x32_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[57] -----
	assign gfpga_pad_GPIO_PAD[57] = raddr8x32_0__shared_input[0];

// ----- Blif Benchmark input raddr8x32_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[22] -----
	assign gfpga_pad_GPIO_PAD[22] = raddr8x32_1__shared_input[0];

// ----- Blif Benchmark input din8x32_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[146] -----
	assign gfpga_pad_GPIO_PAD[146] = din8x32_0__shared_input[0];

// ----- Blif Benchmark input din8x32_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[104] -----
	assign gfpga_pad_GPIO_PAD[104] = din8x32_1__shared_input[0];

// ----- Blif Benchmark input din8x32_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[148] -----
	assign gfpga_pad_GPIO_PAD[148] = din8x32_2__shared_input[0];

// ----- Blif Benchmark input din8x32_3_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[31] -----
	assign gfpga_pad_GPIO_PAD[31] = din8x32_3__shared_input[0];

// ----- Blif Benchmark input din8x32_4_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[62] -----
	assign gfpga_pad_GPIO_PAD[62] = din8x32_4__shared_input[0];

// ----- Blif Benchmark input din8x32_5_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[150] -----
	assign gfpga_pad_GPIO_PAD[150] = din8x32_5__shared_input[0];

// ----- Blif Benchmark input din8x32_6_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[48] -----
	assign gfpga_pad_GPIO_PAD[48] = din8x32_6__shared_input[0];

// ----- Blif Benchmark input din8x32_7_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[49] -----
	assign gfpga_pad_GPIO_PAD[49] = din8x32_7__shared_input[0];

// ----- Blif Benchmark input din8x32_8_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[17] -----
	assign gfpga_pad_GPIO_PAD[17] = din8x32_8__shared_input[0];

// ----- Blif Benchmark input din8x32_9_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[26] -----
	assign gfpga_pad_GPIO_PAD[26] = din8x32_9__shared_input[0];

// ----- Blif Benchmark input din8x32_10_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[19] -----
	assign gfpga_pad_GPIO_PAD[19] = din8x32_10__shared_input[0];

// ----- Blif Benchmark input din8x32_11_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[157] -----
	assign gfpga_pad_GPIO_PAD[157] = din8x32_11__shared_input[0];

// ----- Blif Benchmark input din8x32_12_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[23] -----
	assign gfpga_pad_GPIO_PAD[23] = din8x32_12__shared_input[0];

// ----- Blif Benchmark input din8x32_13_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[60] -----
	assign gfpga_pad_GPIO_PAD[60] = din8x32_13__shared_input[0];

// ----- Blif Benchmark input din8x32_14_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[149] -----
	assign gfpga_pad_GPIO_PAD[149] = din8x32_14__shared_input[0];

// ----- Blif Benchmark input din8x32_15_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[63] -----
	assign gfpga_pad_GPIO_PAD[63] = din8x32_15__shared_input[0];

// ----- Blif Benchmark input din8x32_16_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[56] -----
	assign gfpga_pad_GPIO_PAD[56] = din8x32_16__shared_input[0];

// ----- Blif Benchmark input din8x32_17_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[20] -----
	assign gfpga_pad_GPIO_PAD[20] = din8x32_17__shared_input[0];

// ----- Blif Benchmark input din8x32_18_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[147] -----
	assign gfpga_pad_GPIO_PAD[147] = din8x32_18__shared_input[0];

// ----- Blif Benchmark input din8x32_19_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[108] -----
	assign gfpga_pad_GPIO_PAD[108] = din8x32_19__shared_input[0];

// ----- Blif Benchmark input din8x32_20_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[153] -----
	assign gfpga_pad_GPIO_PAD[153] = din8x32_20__shared_input[0];

// ----- Blif Benchmark input din8x32_21_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[156] -----
	assign gfpga_pad_GPIO_PAD[156] = din8x32_21__shared_input[0];

// ----- Blif Benchmark input din8x32_22_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[152] -----
	assign gfpga_pad_GPIO_PAD[152] = din8x32_22__shared_input[0];

// ----- Blif Benchmark input din8x32_23_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[151] -----
	assign gfpga_pad_GPIO_PAD[151] = din8x32_23__shared_input[0];

// ----- Blif Benchmark input din8x32_24_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[28] -----
	assign gfpga_pad_GPIO_PAD[28] = din8x32_24__shared_input[0];

// ----- Blif Benchmark input din8x32_25_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[154] -----
	assign gfpga_pad_GPIO_PAD[154] = din8x32_25__shared_input[0];

// ----- Blif Benchmark input din8x32_26_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[21] -----
	assign gfpga_pad_GPIO_PAD[21] = din8x32_26__shared_input[0];

// ----- Blif Benchmark input din8x32_27_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[53] -----
	assign gfpga_pad_GPIO_PAD[53] = din8x32_27__shared_input[0];

// ----- Blif Benchmark input din8x32_28_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[155] -----
	assign gfpga_pad_GPIO_PAD[155] = din8x32_28__shared_input[0];

// ----- Blif Benchmark input din8x32_29_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[50] -----
	assign gfpga_pad_GPIO_PAD[50] = din8x32_29__shared_input[0];

// ----- Blif Benchmark input din8x32_30_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[59] -----
	assign gfpga_pad_GPIO_PAD[59] = din8x32_30__shared_input[0];

// ----- Blif Benchmark input din8x32_31_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[145] -----
	assign gfpga_pad_GPIO_PAD[145] = din8x32_31__shared_input[0];

// ----- Blif Benchmark output dout8x16_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[118] -----
	assign dout8x16_0__fpga[0] = gfpga_pad_GPIO_PAD[118];

// ----- Blif Benchmark output dout8x16_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[90] -----
	assign dout8x16_1__fpga[0] = gfpga_pad_GPIO_PAD[90];

// ----- Blif Benchmark output dout8x16_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[93] -----
	assign dout8x16_2__fpga[0] = gfpga_pad_GPIO_PAD[93];

// ----- Blif Benchmark output dout8x16_3_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[82] -----
	assign dout8x16_3__fpga[0] = gfpga_pad_GPIO_PAD[82];

// ----- Blif Benchmark output dout8x16_4_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[112] -----
	assign dout8x16_4__fpga[0] = gfpga_pad_GPIO_PAD[112];

// ----- Blif Benchmark output dout8x16_5_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[117] -----
	assign dout8x16_5__fpga[0] = gfpga_pad_GPIO_PAD[117];

// ----- Blif Benchmark output dout8x16_6_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[113] -----
	assign dout8x16_6__fpga[0] = gfpga_pad_GPIO_PAD[113];

// ----- Blif Benchmark output dout8x16_7_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[85] -----
	assign dout8x16_7__fpga[0] = gfpga_pad_GPIO_PAD[85];

// ----- Blif Benchmark output dout8x16_8_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[127] -----
	assign dout8x16_8__fpga[0] = gfpga_pad_GPIO_PAD[127];

// ----- Blif Benchmark output dout8x16_9_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[124] -----
	assign dout8x16_9__fpga[0] = gfpga_pad_GPIO_PAD[124];

// ----- Blif Benchmark output dout8x16_10_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[115] -----
	assign dout8x16_10__fpga[0] = gfpga_pad_GPIO_PAD[115];

// ----- Blif Benchmark output dout8x16_11_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[120] -----
	assign dout8x16_11__fpga[0] = gfpga_pad_GPIO_PAD[120];

// ----- Blif Benchmark output dout8x16_12_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[123] -----
	assign dout8x16_12__fpga[0] = gfpga_pad_GPIO_PAD[123];

// ----- Blif Benchmark output dout8x16_13_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[119] -----
	assign dout8x16_13__fpga[0] = gfpga_pad_GPIO_PAD[119];

// ----- Blif Benchmark output dout8x16_14_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[126] -----
	assign dout8x16_14__fpga[0] = gfpga_pad_GPIO_PAD[126];

// ----- Blif Benchmark output dout8x16_15_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[86] -----
	assign dout8x16_15__fpga[0] = gfpga_pad_GPIO_PAD[86];

// ----- Blif Benchmark output dout8x32_0_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[18] -----
	assign dout8x32_0__fpga[0] = gfpga_pad_GPIO_PAD[18];

// ----- Blif Benchmark output dout8x32_1_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[35] -----
	assign dout8x32_1__fpga[0] = gfpga_pad_GPIO_PAD[35];

// ----- Blif Benchmark output dout8x32_2_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[61] -----
	assign dout8x32_2__fpga[0] = gfpga_pad_GPIO_PAD[61];

// ----- Blif Benchmark output dout8x32_3_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[54] -----
	assign dout8x32_3__fpga[0] = gfpga_pad_GPIO_PAD[54];

// ----- Blif Benchmark output dout8x32_4_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[89] -----
	assign dout8x32_4__fpga[0] = gfpga_pad_GPIO_PAD[89];

// ----- Blif Benchmark output dout8x32_5_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[158] -----
	assign dout8x32_5__fpga[0] = gfpga_pad_GPIO_PAD[158];

// ----- Blif Benchmark output dout8x32_6_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[51] -----
	assign dout8x32_6__fpga[0] = gfpga_pad_GPIO_PAD[51];

// ----- Blif Benchmark output dout8x32_7_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[36] -----
	assign dout8x32_7__fpga[0] = gfpga_pad_GPIO_PAD[36];

// ----- Blif Benchmark output dout8x32_8_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[83] -----
	assign dout8x32_8__fpga[0] = gfpga_pad_GPIO_PAD[83];

// ----- Blif Benchmark output dout8x32_9_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[116] -----
	assign dout8x32_9__fpga[0] = gfpga_pad_GPIO_PAD[116];

// ----- Blif Benchmark output dout8x32_10_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[55] -----
	assign dout8x32_10__fpga[0] = gfpga_pad_GPIO_PAD[55];

// ----- Blif Benchmark output dout8x32_11_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[52] -----
	assign dout8x32_11__fpga[0] = gfpga_pad_GPIO_PAD[52];

// ----- Blif Benchmark output dout8x32_12_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[30] -----
	assign dout8x32_12__fpga[0] = gfpga_pad_GPIO_PAD[30];

// ----- Blif Benchmark output dout8x32_13_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[91] -----
	assign dout8x32_13__fpga[0] = gfpga_pad_GPIO_PAD[91];

// ----- Blif Benchmark output dout8x32_14_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[24] -----
	assign dout8x32_14__fpga[0] = gfpga_pad_GPIO_PAD[24];

// ----- Blif Benchmark output dout8x32_15_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[144] -----
	assign dout8x32_15__fpga[0] = gfpga_pad_GPIO_PAD[144];

// ----- Blif Benchmark output dout8x32_16_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[27] -----
	assign dout8x32_16__fpga[0] = gfpga_pad_GPIO_PAD[27];

// ----- Blif Benchmark output dout8x32_17_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[46] -----
	assign dout8x32_17__fpga[0] = gfpga_pad_GPIO_PAD[46];

// ----- Blif Benchmark output dout8x32_18_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[38] -----
	assign dout8x32_18__fpga[0] = gfpga_pad_GPIO_PAD[38];

// ----- Blif Benchmark output dout8x32_19_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[58] -----
	assign dout8x32_19__fpga[0] = gfpga_pad_GPIO_PAD[58];

// ----- Blif Benchmark output dout8x32_20_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[47] -----
	assign dout8x32_20__fpga[0] = gfpga_pad_GPIO_PAD[47];

// ----- Blif Benchmark output dout8x32_21_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[14] -----
	assign dout8x32_21__fpga[0] = gfpga_pad_GPIO_PAD[14];

// ----- Blif Benchmark output dout8x32_22_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[8] -----
	assign dout8x32_22__fpga[0] = gfpga_pad_GPIO_PAD[8];

// ----- Blif Benchmark output dout8x32_23_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[95] -----
	assign dout8x32_23__fpga[0] = gfpga_pad_GPIO_PAD[95];

// ----- Blif Benchmark output dout8x32_24_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[5] -----
	assign dout8x32_24__fpga[0] = gfpga_pad_GPIO_PAD[5];

// ----- Blif Benchmark output dout8x32_25_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[29] -----
	assign dout8x32_25__fpga[0] = gfpga_pad_GPIO_PAD[29];

// ----- Blif Benchmark output dout8x32_26_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[3] -----
	assign dout8x32_26__fpga[0] = gfpga_pad_GPIO_PAD[3];

// ----- Blif Benchmark output dout8x32_27_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[121] -----
	assign dout8x32_27__fpga[0] = gfpga_pad_GPIO_PAD[121];

// ----- Blif Benchmark output dout8x32_28_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[15] -----
	assign dout8x32_28__fpga[0] = gfpga_pad_GPIO_PAD[15];

// ----- Blif Benchmark output dout8x32_29_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[92] -----
	assign dout8x32_29__fpga[0] = gfpga_pad_GPIO_PAD[92];

// ----- Blif Benchmark output dout8x32_30_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[34] -----
	assign dout8x32_30__fpga[0] = gfpga_pad_GPIO_PAD[34];

// ----- Blif Benchmark output dout8x32_31_ is mapped to FPGA IOPAD gfpga_pad_GPIO_PAD[94] -----
	assign dout8x32_31__fpga[0] = gfpga_pad_GPIO_PAD[94];

// ----- Wire unused FPGA I/Os to constants -----
	assign gfpga_pad_GPIO_PAD[0] = 1'b0;
	assign gfpga_pad_GPIO_PAD[1] = 1'b0;
	assign gfpga_pad_GPIO_PAD[2] = 1'b0;
	assign gfpga_pad_GPIO_PAD[4] = 1'b0;
	assign gfpga_pad_GPIO_PAD[6] = 1'b0;
	assign gfpga_pad_GPIO_PAD[7] = 1'b0;
	assign gfpga_pad_GPIO_PAD[9] = 1'b0;
	assign gfpga_pad_GPIO_PAD[10] = 1'b0;
	assign gfpga_pad_GPIO_PAD[12] = 1'b0;
	assign gfpga_pad_GPIO_PAD[13] = 1'b0;
	assign gfpga_pad_GPIO_PAD[32] = 1'b0;
	assign gfpga_pad_GPIO_PAD[33] = 1'b0;
	assign gfpga_pad_GPIO_PAD[37] = 1'b0;
	assign gfpga_pad_GPIO_PAD[39] = 1'b0;
	assign gfpga_pad_GPIO_PAD[40] = 1'b0;
	assign gfpga_pad_GPIO_PAD[41] = 1'b0;
	assign gfpga_pad_GPIO_PAD[42] = 1'b0;
	assign gfpga_pad_GPIO_PAD[43] = 1'b0;
	assign gfpga_pad_GPIO_PAD[44] = 1'b0;
	assign gfpga_pad_GPIO_PAD[45] = 1'b0;
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
	assign gfpga_pad_GPIO_PAD[128] = 1'b0;
	assign gfpga_pad_GPIO_PAD[129] = 1'b0;
	assign gfpga_pad_GPIO_PAD[131] = 1'b0;
	assign gfpga_pad_GPIO_PAD[132] = 1'b0;
	assign gfpga_pad_GPIO_PAD[133] = 1'b0;
	assign gfpga_pad_GPIO_PAD[134] = 1'b0;
	assign gfpga_pad_GPIO_PAD[136] = 1'b0;
	assign gfpga_pad_GPIO_PAD[137] = 1'b0;
	assign gfpga_pad_GPIO_PAD[138] = 1'b0;
	assign gfpga_pad_GPIO_PAD[139] = 1'b0;
	assign gfpga_pad_GPIO_PAD[140] = 1'b0;
	assign gfpga_pad_GPIO_PAD[141] = 1'b0;
	assign gfpga_pad_GPIO_PAD[142] = 1'b0;
	assign gfpga_pad_GPIO_PAD[143] = 1'b0;
	assign gfpga_pad_GPIO_PAD[159] = 1'b0;

// ----- Reference Benchmark Instanication -------
	dual_port_ram_8x16_8x32 REF_DUT(
		.clk(clk),
		.wen(wen_shared_input),
		.ren(ren_shared_input),
		.waddr8x16({waddr8x16_2__shared_input, waddr8x16_1__shared_input, waddr8x16_0__shared_input}),
		.raddr8x16({raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input}),
		.din8x16({din8x16_15__shared_input, din8x16_14__shared_input, din8x16_13__shared_input, din8x16_12__shared_input, din8x16_11__shared_input, din8x16_10__shared_input, din8x16_9__shared_input, din8x16_8__shared_input, din8x16_7__shared_input, din8x16_6__shared_input, din8x16_5__shared_input, din8x16_4__shared_input, din8x16_3__shared_input, din8x16_2__shared_input, din8x16_1__shared_input, din8x16_0__shared_input}),
		.waddr8x32({waddr8x32_1__shared_input, waddr8x32_0__shared_input}),
		.raddr8x32({raddr8x32_1__shared_input, raddr8x32_0__shared_input}),
		.din8x32({din8x32_31__shared_input, din8x32_30__shared_input, din8x32_29__shared_input, din8x32_28__shared_input, din8x32_27__shared_input, din8x32_26__shared_input, din8x32_25__shared_input, din8x32_24__shared_input, din8x32_23__shared_input, din8x32_22__shared_input, din8x32_21__shared_input, din8x32_20__shared_input, din8x32_19__shared_input, din8x32_18__shared_input, din8x32_17__shared_input, din8x32_16__shared_input, din8x32_15__shared_input, din8x32_14__shared_input, din8x32_13__shared_input, din8x32_12__shared_input, din8x32_11__shared_input, din8x32_10__shared_input, din8x32_9__shared_input, din8x32_8__shared_input, din8x32_7__shared_input, din8x32_6__shared_input, din8x32_5__shared_input, din8x32_4__shared_input, din8x32_3__shared_input, din8x32_2__shared_input, din8x32_1__shared_input, din8x32_0__shared_input}),
		.dout8x16({dout8x16_15__benchmark, dout8x16_14__benchmark, dout8x16_13__benchmark, dout8x16_12__benchmark, dout8x16_11__benchmark, dout8x16_10__benchmark, dout8x16_9__benchmark, dout8x16_8__benchmark, dout8x16_7__benchmark, dout8x16_6__benchmark, dout8x16_5__benchmark, dout8x16_4__benchmark, dout8x16_3__benchmark, dout8x16_2__benchmark, dout8x16_1__benchmark, dout8x16_0__benchmark}),
		.dout8x32({dout8x32_31__benchmark, dout8x32_30__benchmark, dout8x32_29__benchmark, dout8x32_28__benchmark, dout8x32_27__benchmark, dout8x32_26__benchmark, dout8x32_25__benchmark, dout8x32_24__benchmark, dout8x32_23__benchmark, dout8x32_22__benchmark, dout8x32_21__benchmark, dout8x32_20__benchmark, dout8x32_19__benchmark, dout8x32_18__benchmark, dout8x32_17__benchmark, dout8x32_16__benchmark, dout8x32_15__benchmark, dout8x32_14__benchmark, dout8x32_13__benchmark, dout8x32_12__benchmark, dout8x32_11__benchmark, dout8x32_10__benchmark, dout8x32_9__benchmark, dout8x32_8__benchmark, dout8x32_7__benchmark, dout8x32_6__benchmark, dout8x32_5__benchmark, dout8x32_4__benchmark, dout8x32_3__benchmark, dout8x32_2__benchmark, dout8x32_1__benchmark, dout8x32_0__benchmark})
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
		waddr8x16_0__shared_input <= 1'b0;
		waddr8x16_1__shared_input <= 1'b0;
		waddr8x16_2__shared_input <= 1'b0;
		raddr8x16_0__shared_input <= 1'b0;
		raddr8x16_1__shared_input <= 1'b0;
		raddr8x16_2__shared_input <= 1'b0;
		din8x16_0__shared_input <= 1'b0;
		din8x16_1__shared_input <= 1'b0;
		din8x16_2__shared_input <= 1'b0;
		din8x16_3__shared_input <= 1'b0;
		din8x16_4__shared_input <= 1'b0;
		din8x16_5__shared_input <= 1'b0;
		din8x16_6__shared_input <= 1'b0;
		din8x16_7__shared_input <= 1'b0;
		din8x16_8__shared_input <= 1'b0;
		din8x16_9__shared_input <= 1'b0;
		din8x16_10__shared_input <= 1'b0;
		din8x16_11__shared_input <= 1'b0;
		din8x16_12__shared_input <= 1'b0;
		din8x16_13__shared_input <= 1'b0;
		din8x16_14__shared_input <= 1'b0;
		din8x16_15__shared_input <= 1'b0;
		waddr8x32_0__shared_input <= 1'b0;
		waddr8x32_1__shared_input <= 1'b0;
		raddr8x32_0__shared_input <= 1'b0;
		raddr8x32_1__shared_input <= 1'b0;
		din8x32_0__shared_input <= 1'b0;
		din8x32_1__shared_input <= 1'b0;
		din8x32_2__shared_input <= 1'b0;
		din8x32_3__shared_input <= 1'b0;
		din8x32_4__shared_input <= 1'b0;
		din8x32_5__shared_input <= 1'b0;
		din8x32_6__shared_input <= 1'b0;
		din8x32_7__shared_input <= 1'b0;
		din8x32_8__shared_input <= 1'b0;
		din8x32_9__shared_input <= 1'b0;
		din8x32_10__shared_input <= 1'b0;
		din8x32_11__shared_input <= 1'b0;
		din8x32_12__shared_input <= 1'b0;
		din8x32_13__shared_input <= 1'b0;
		din8x32_14__shared_input <= 1'b0;
		din8x32_15__shared_input <= 1'b0;
		din8x32_16__shared_input <= 1'b0;
		din8x32_17__shared_input <= 1'b0;
		din8x32_18__shared_input <= 1'b0;
		din8x32_19__shared_input <= 1'b0;
		din8x32_20__shared_input <= 1'b0;
		din8x32_21__shared_input <= 1'b0;
		din8x32_22__shared_input <= 1'b0;
		din8x32_23__shared_input <= 1'b0;
		din8x32_24__shared_input <= 1'b0;
		din8x32_25__shared_input <= 1'b0;
		din8x32_26__shared_input <= 1'b0;
		din8x32_27__shared_input <= 1'b0;
		din8x32_28__shared_input <= 1'b0;
		din8x32_29__shared_input <= 1'b0;
		din8x32_30__shared_input <= 1'b0;
		din8x32_31__shared_input <= 1'b0;

		dout8x16_0__flag[0] <= 1'b0;
		dout8x16_1__flag[0] <= 1'b0;
		dout8x16_2__flag[0] <= 1'b0;
		dout8x16_3__flag[0] <= 1'b0;
		dout8x16_4__flag[0] <= 1'b0;
		dout8x16_5__flag[0] <= 1'b0;
		dout8x16_6__flag[0] <= 1'b0;
		dout8x16_7__flag[0] <= 1'b0;
		dout8x16_8__flag[0] <= 1'b0;
		dout8x16_9__flag[0] <= 1'b0;
		dout8x16_10__flag[0] <= 1'b0;
		dout8x16_11__flag[0] <= 1'b0;
		dout8x16_12__flag[0] <= 1'b0;
		dout8x16_13__flag[0] <= 1'b0;
		dout8x16_14__flag[0] <= 1'b0;
		dout8x16_15__flag[0] <= 1'b0;
		dout8x32_0__flag[0] <= 1'b0;
		dout8x32_1__flag[0] <= 1'b0;
		dout8x32_2__flag[0] <= 1'b0;
		dout8x32_3__flag[0] <= 1'b0;
		dout8x32_4__flag[0] <= 1'b0;
		dout8x32_5__flag[0] <= 1'b0;
		dout8x32_6__flag[0] <= 1'b0;
		dout8x32_7__flag[0] <= 1'b0;
		dout8x32_8__flag[0] <= 1'b0;
		dout8x32_9__flag[0] <= 1'b0;
		dout8x32_10__flag[0] <= 1'b0;
		dout8x32_11__flag[0] <= 1'b0;
		dout8x32_12__flag[0] <= 1'b0;
		dout8x32_13__flag[0] <= 1'b0;
		dout8x32_14__flag[0] <= 1'b0;
		dout8x32_15__flag[0] <= 1'b0;
		dout8x32_16__flag[0] <= 1'b0;
		dout8x32_17__flag[0] <= 1'b0;
		dout8x32_18__flag[0] <= 1'b0;
		dout8x32_19__flag[0] <= 1'b0;
		dout8x32_20__flag[0] <= 1'b0;
		dout8x32_21__flag[0] <= 1'b0;
		dout8x32_22__flag[0] <= 1'b0;
		dout8x32_23__flag[0] <= 1'b0;
		dout8x32_24__flag[0] <= 1'b0;
		dout8x32_25__flag[0] <= 1'b0;
		dout8x32_26__flag[0] <= 1'b0;
		dout8x32_27__flag[0] <= 1'b0;
		dout8x32_28__flag[0] <= 1'b0;
		dout8x32_29__flag[0] <= 1'b0;
		dout8x32_30__flag[0] <= 1'b0;
		dout8x32_31__flag[0] <= 1'b0;
	end

// ----- Input Stimulus -------
// ----- Deterministic readback/write sequence after __config_all_done__ -----
// ----- KEY TIMING: assert ren/raddr IMMEDIATELY on __config_all_done__ (before any negedge) -----
// ----- so signals are stable before checker's first real compare (negedge #1 consumes sim_start) -----
	wire [0:15] dout8x16_fpga_bus = {dout8x16_15__fpga, dout8x16_14__fpga, dout8x16_13__fpga, dout8x16_12__fpga,
	                                  dout8x16_11__fpga, dout8x16_10__fpga, dout8x16_9__fpga,  dout8x16_8__fpga,
	                                  dout8x16_7__fpga,  dout8x16_6__fpga,  dout8x16_5__fpga,  dout8x16_4__fpga,
	                                  dout8x16_3__fpga,  dout8x16_2__fpga,  dout8x16_1__fpga,  dout8x16_0__fpga};
	wire [0:31] dout8x32_fpga_bus = {dout8x32_31__fpga, dout8x32_30__fpga, dout8x32_29__fpga, dout8x32_28__fpga,
	                                  dout8x32_27__fpga, dout8x32_26__fpga, dout8x32_25__fpga, dout8x32_24__fpga,
	                                  dout8x32_23__fpga, dout8x32_22__fpga, dout8x32_21__fpga, dout8x32_20__fpga,
	                                  dout8x32_19__fpga, dout8x32_18__fpga, dout8x32_17__fpga, dout8x32_16__fpga,
	                                  dout8x32_15__fpga, dout8x32_14__fpga, dout8x32_13__fpga, dout8x32_12__fpga,
	                                  dout8x32_11__fpga, dout8x32_10__fpga, dout8x32_9__fpga,  dout8x32_8__fpga,
	                                  dout8x32_7__fpga,  dout8x32_6__fpga,  dout8x32_5__fpga,  dout8x32_4__fpga,
	                                  dout8x32_3__fpga,  dout8x32_2__fpga,  dout8x32_1__fpga,  dout8x32_0__fpga};

initial begin
	wait (__config_all_done__[0] === 1'b1);

	// ---- Assert ren + first raddr for both RAMs IMMEDIATELY (no clock edge wait) ----
	$display("[TB] Reading back initialized values via system fabric...");
	ren_shared_input = 1'b1;
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b000;
	{raddr8x32_1__shared_input, raddr8x32_0__shared_input} = 2'b00;

	// ---- negedge #1: checker consumes sim_start (no compare yet); display addr 0 and advance ----
	@(negedge clk[0]);
	$display("8x16 Addr 0 | Got: %h", dout8x16_fpga_bus);
	$display("8x32 Addr 0 | Got: %h", dout8x32_fpga_bus);
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b001;
	{raddr8x32_1__shared_input, raddr8x32_0__shared_input} = 2'b01;

	// ---- Step 2: Read back preloaded values ----
	@(negedge clk[0]);
	$display("8x16 Addr 1 | Got: %h", dout8x16_fpga_bus);
	$display("8x32 Addr 1 | Got: %h", dout8x32_fpga_bus);
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b010;
	{raddr8x32_1__shared_input, raddr8x32_0__shared_input} = 2'b10;

	@(negedge clk[0]);
	$display("8x16 Addr 2 | Got: %h", dout8x16_fpga_bus);
	$display("8x32 Addr 2 | Got: %h", dout8x32_fpga_bus);
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b011;
	{raddr8x32_1__shared_input, raddr8x32_0__shared_input} = 2'b11;

	@(negedge clk[0]);
	$display("8x16 Addr 3 | Got: %h", dout8x16_fpga_bus);
	$display("8x32 Addr 3 | Got: %h", dout8x32_fpga_bus);
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b100;

	@(negedge clk[0]);
	$display("8x16 Addr 4 | Got: %h", dout8x16_fpga_bus);
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b101;

	@(negedge clk[0]);
	$display("8x16 Addr 5 | Got: %h", dout8x16_fpga_bus);
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b110;

	@(negedge clk[0]);
	$display("8x16 Addr 6 | Got: %h", dout8x16_fpga_bus);
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b111;

	@(negedge clk[0]);
	$display("8x16 Addr 7 | Got: %h", dout8x16_fpga_bus);
	ren_shared_input = 1'b0;

	@(negedge clk[0]);

	// ---- Step 3: Runtime write then readback for 8x16 ----
	$display("[TB] Testing runtime write/read for 8x16...");
	wen_shared_input = 1'b1;
	{waddr8x16_2__shared_input, waddr8x16_1__shared_input, waddr8x16_0__shared_input} = 3'b010;
	{din8x16_15__shared_input, din8x16_14__shared_input, din8x16_13__shared_input, din8x16_12__shared_input,
	 din8x16_11__shared_input, din8x16_10__shared_input, din8x16_9__shared_input,  din8x16_8__shared_input,
	 din8x16_7__shared_input,  din8x16_6__shared_input,  din8x16_5__shared_input,  din8x16_4__shared_input,
	 din8x16_3__shared_input,  din8x16_2__shared_input,  din8x16_1__shared_input,  din8x16_0__shared_input} = 16'hBEEF;
	@(negedge clk[0]);
	wen_shared_input = 1'b0;
	ren_shared_input = 1'b1;
	{raddr8x16_2__shared_input, raddr8x16_1__shared_input, raddr8x16_0__shared_input} = 3'b010;
	@(negedge clk[0]);
	$display("8x16 Addr 2 (post-write) | Expected: BEEF | Got: %h", dout8x16_fpga_bus);
	ren_shared_input = 1'b0;

	@(negedge clk[0]);

	// ---- Step 4: Runtime write then readback for 8x32 ----
	$display("[TB] Testing runtime write/read for 8x32...");
	wen_shared_input = 1'b1;
	{waddr8x32_1__shared_input, waddr8x32_0__shared_input} = 2'b10;
	{din8x32_31__shared_input, din8x32_30__shared_input, din8x32_29__shared_input, din8x32_28__shared_input,
	 din8x32_27__shared_input, din8x32_26__shared_input, din8x32_25__shared_input, din8x32_24__shared_input,
	 din8x32_23__shared_input, din8x32_22__shared_input, din8x32_21__shared_input, din8x32_20__shared_input,
	 din8x32_19__shared_input, din8x32_18__shared_input, din8x32_17__shared_input, din8x32_16__shared_input,
	 din8x32_15__shared_input, din8x32_14__shared_input, din8x32_13__shared_input, din8x32_12__shared_input,
	 din8x32_11__shared_input, din8x32_10__shared_input, din8x32_9__shared_input,  din8x32_8__shared_input,
	 din8x32_7__shared_input,  din8x32_6__shared_input,  din8x32_5__shared_input,  din8x32_4__shared_input,
	 din8x32_3__shared_input,  din8x32_2__shared_input,  din8x32_1__shared_input,  din8x32_0__shared_input} = 32'hDEADBEEF;
	@(negedge clk[0]);
	wen_shared_input = 1'b0;
	ren_shared_input = 1'b1;
	{raddr8x32_1__shared_input, raddr8x32_0__shared_input} = 2'b10;
	@(negedge clk[0]);
	$display("8x32 Addr 2 (post-write) | Expected: DEADBEEF | Got: %h", dout8x32_fpga_bus);
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
			if(!(dout8x16_0__fpga === dout8x16_0__benchmark) && !(dout8x16_0__benchmark === 1'bx)) begin
				dout8x16_0__flag <= 1'b1;
			end else begin
				dout8x16_0__flag<= 1'b0;
			end
			if(!(dout8x16_1__fpga === dout8x16_1__benchmark) && !(dout8x16_1__benchmark === 1'bx)) begin
				dout8x16_1__flag <= 1'b1;
			end else begin
				dout8x16_1__flag<= 1'b0;
			end
			if(!(dout8x16_2__fpga === dout8x16_2__benchmark) && !(dout8x16_2__benchmark === 1'bx)) begin
				dout8x16_2__flag <= 1'b1;
			end else begin
				dout8x16_2__flag<= 1'b0;
			end
			if(!(dout8x16_3__fpga === dout8x16_3__benchmark) && !(dout8x16_3__benchmark === 1'bx)) begin
				dout8x16_3__flag <= 1'b1;
			end else begin
				dout8x16_3__flag<= 1'b0;
			end
			if(!(dout8x16_4__fpga === dout8x16_4__benchmark) && !(dout8x16_4__benchmark === 1'bx)) begin
				dout8x16_4__flag <= 1'b1;
			end else begin
				dout8x16_4__flag<= 1'b0;
			end
			if(!(dout8x16_5__fpga === dout8x16_5__benchmark) && !(dout8x16_5__benchmark === 1'bx)) begin
				dout8x16_5__flag <= 1'b1;
			end else begin
				dout8x16_5__flag<= 1'b0;
			end
			if(!(dout8x16_6__fpga === dout8x16_6__benchmark) && !(dout8x16_6__benchmark === 1'bx)) begin
				dout8x16_6__flag <= 1'b1;
			end else begin
				dout8x16_6__flag<= 1'b0;
			end
			if(!(dout8x16_7__fpga === dout8x16_7__benchmark) && !(dout8x16_7__benchmark === 1'bx)) begin
				dout8x16_7__flag <= 1'b1;
			end else begin
				dout8x16_7__flag<= 1'b0;
			end
			if(!(dout8x16_8__fpga === dout8x16_8__benchmark) && !(dout8x16_8__benchmark === 1'bx)) begin
				dout8x16_8__flag <= 1'b1;
			end else begin
				dout8x16_8__flag<= 1'b0;
			end
			if(!(dout8x16_9__fpga === dout8x16_9__benchmark) && !(dout8x16_9__benchmark === 1'bx)) begin
				dout8x16_9__flag <= 1'b1;
			end else begin
				dout8x16_9__flag<= 1'b0;
			end
			if(!(dout8x16_10__fpga === dout8x16_10__benchmark) && !(dout8x16_10__benchmark === 1'bx)) begin
				dout8x16_10__flag <= 1'b1;
			end else begin
				dout8x16_10__flag<= 1'b0;
			end
			if(!(dout8x16_11__fpga === dout8x16_11__benchmark) && !(dout8x16_11__benchmark === 1'bx)) begin
				dout8x16_11__flag <= 1'b1;
			end else begin
				dout8x16_11__flag<= 1'b0;
			end
			if(!(dout8x16_12__fpga === dout8x16_12__benchmark) && !(dout8x16_12__benchmark === 1'bx)) begin
				dout8x16_12__flag <= 1'b1;
			end else begin
				dout8x16_12__flag<= 1'b0;
			end
			if(!(dout8x16_13__fpga === dout8x16_13__benchmark) && !(dout8x16_13__benchmark === 1'bx)) begin
				dout8x16_13__flag <= 1'b1;
			end else begin
				dout8x16_13__flag<= 1'b0;
			end
			if(!(dout8x16_14__fpga === dout8x16_14__benchmark) && !(dout8x16_14__benchmark === 1'bx)) begin
				dout8x16_14__flag <= 1'b1;
			end else begin
				dout8x16_14__flag<= 1'b0;
			end
			if(!(dout8x16_15__fpga === dout8x16_15__benchmark) && !(dout8x16_15__benchmark === 1'bx)) begin
				dout8x16_15__flag <= 1'b1;
			end else begin
				dout8x16_15__flag<= 1'b0;
			end
			if(!(dout8x32_0__fpga === dout8x32_0__benchmark) && !(dout8x32_0__benchmark === 1'bx)) begin
				dout8x32_0__flag <= 1'b1;
			end else begin
				dout8x32_0__flag<= 1'b0;
			end
			if(!(dout8x32_1__fpga === dout8x32_1__benchmark) && !(dout8x32_1__benchmark === 1'bx)) begin
				dout8x32_1__flag <= 1'b1;
			end else begin
				dout8x32_1__flag<= 1'b0;
			end
			if(!(dout8x32_2__fpga === dout8x32_2__benchmark) && !(dout8x32_2__benchmark === 1'bx)) begin
				dout8x32_2__flag <= 1'b1;
			end else begin
				dout8x32_2__flag<= 1'b0;
			end
			if(!(dout8x32_3__fpga === dout8x32_3__benchmark) && !(dout8x32_3__benchmark === 1'bx)) begin
				dout8x32_3__flag <= 1'b1;
			end else begin
				dout8x32_3__flag<= 1'b0;
			end
			if(!(dout8x32_4__fpga === dout8x32_4__benchmark) && !(dout8x32_4__benchmark === 1'bx)) begin
				dout8x32_4__flag <= 1'b1;
			end else begin
				dout8x32_4__flag<= 1'b0;
			end
			if(!(dout8x32_5__fpga === dout8x32_5__benchmark) && !(dout8x32_5__benchmark === 1'bx)) begin
				dout8x32_5__flag <= 1'b1;
			end else begin
				dout8x32_5__flag<= 1'b0;
			end
			if(!(dout8x32_6__fpga === dout8x32_6__benchmark) && !(dout8x32_6__benchmark === 1'bx)) begin
				dout8x32_6__flag <= 1'b1;
			end else begin
				dout8x32_6__flag<= 1'b0;
			end
			if(!(dout8x32_7__fpga === dout8x32_7__benchmark) && !(dout8x32_7__benchmark === 1'bx)) begin
				dout8x32_7__flag <= 1'b1;
			end else begin
				dout8x32_7__flag<= 1'b0;
			end
			if(!(dout8x32_8__fpga === dout8x32_8__benchmark) && !(dout8x32_8__benchmark === 1'bx)) begin
				dout8x32_8__flag <= 1'b1;
			end else begin
				dout8x32_8__flag<= 1'b0;
			end
			if(!(dout8x32_9__fpga === dout8x32_9__benchmark) && !(dout8x32_9__benchmark === 1'bx)) begin
				dout8x32_9__flag <= 1'b1;
			end else begin
				dout8x32_9__flag<= 1'b0;
			end
			if(!(dout8x32_10__fpga === dout8x32_10__benchmark) && !(dout8x32_10__benchmark === 1'bx)) begin
				dout8x32_10__flag <= 1'b1;
			end else begin
				dout8x32_10__flag<= 1'b0;
			end
			if(!(dout8x32_11__fpga === dout8x32_11__benchmark) && !(dout8x32_11__benchmark === 1'bx)) begin
				dout8x32_11__flag <= 1'b1;
			end else begin
				dout8x32_11__flag<= 1'b0;
			end
			if(!(dout8x32_12__fpga === dout8x32_12__benchmark) && !(dout8x32_12__benchmark === 1'bx)) begin
				dout8x32_12__flag <= 1'b1;
			end else begin
				dout8x32_12__flag<= 1'b0;
			end
			if(!(dout8x32_13__fpga === dout8x32_13__benchmark) && !(dout8x32_13__benchmark === 1'bx)) begin
				dout8x32_13__flag <= 1'b1;
			end else begin
				dout8x32_13__flag<= 1'b0;
			end
			if(!(dout8x32_14__fpga === dout8x32_14__benchmark) && !(dout8x32_14__benchmark === 1'bx)) begin
				dout8x32_14__flag <= 1'b1;
			end else begin
				dout8x32_14__flag<= 1'b0;
			end
			if(!(dout8x32_15__fpga === dout8x32_15__benchmark) && !(dout8x32_15__benchmark === 1'bx)) begin
				dout8x32_15__flag <= 1'b1;
			end else begin
				dout8x32_15__flag<= 1'b0;
			end
			if(!(dout8x32_16__fpga === dout8x32_16__benchmark) && !(dout8x32_16__benchmark === 1'bx)) begin
				dout8x32_16__flag <= 1'b1;
			end else begin
				dout8x32_16__flag<= 1'b0;
			end
			if(!(dout8x32_17__fpga === dout8x32_17__benchmark) && !(dout8x32_17__benchmark === 1'bx)) begin
				dout8x32_17__flag <= 1'b1;
			end else begin
				dout8x32_17__flag<= 1'b0;
			end
			if(!(dout8x32_18__fpga === dout8x32_18__benchmark) && !(dout8x32_18__benchmark === 1'bx)) begin
				dout8x32_18__flag <= 1'b1;
			end else begin
				dout8x32_18__flag<= 1'b0;
			end
			if(!(dout8x32_19__fpga === dout8x32_19__benchmark) && !(dout8x32_19__benchmark === 1'bx)) begin
				dout8x32_19__flag <= 1'b1;
			end else begin
				dout8x32_19__flag<= 1'b0;
			end
			if(!(dout8x32_20__fpga === dout8x32_20__benchmark) && !(dout8x32_20__benchmark === 1'bx)) begin
				dout8x32_20__flag <= 1'b1;
			end else begin
				dout8x32_20__flag<= 1'b0;
			end
			if(!(dout8x32_21__fpga === dout8x32_21__benchmark) && !(dout8x32_21__benchmark === 1'bx)) begin
				dout8x32_21__flag <= 1'b1;
			end else begin
				dout8x32_21__flag<= 1'b0;
			end
			if(!(dout8x32_22__fpga === dout8x32_22__benchmark) && !(dout8x32_22__benchmark === 1'bx)) begin
				dout8x32_22__flag <= 1'b1;
			end else begin
				dout8x32_22__flag<= 1'b0;
			end
			if(!(dout8x32_23__fpga === dout8x32_23__benchmark) && !(dout8x32_23__benchmark === 1'bx)) begin
				dout8x32_23__flag <= 1'b1;
			end else begin
				dout8x32_23__flag<= 1'b0;
			end
			if(!(dout8x32_24__fpga === dout8x32_24__benchmark) && !(dout8x32_24__benchmark === 1'bx)) begin
				dout8x32_24__flag <= 1'b1;
			end else begin
				dout8x32_24__flag<= 1'b0;
			end
			if(!(dout8x32_25__fpga === dout8x32_25__benchmark) && !(dout8x32_25__benchmark === 1'bx)) begin
				dout8x32_25__flag <= 1'b1;
			end else begin
				dout8x32_25__flag<= 1'b0;
			end
			if(!(dout8x32_26__fpga === dout8x32_26__benchmark) && !(dout8x32_26__benchmark === 1'bx)) begin
				dout8x32_26__flag <= 1'b1;
			end else begin
				dout8x32_26__flag<= 1'b0;
			end
			if(!(dout8x32_27__fpga === dout8x32_27__benchmark) && !(dout8x32_27__benchmark === 1'bx)) begin
				dout8x32_27__flag <= 1'b1;
			end else begin
				dout8x32_27__flag<= 1'b0;
			end
			if(!(dout8x32_28__fpga === dout8x32_28__benchmark) && !(dout8x32_28__benchmark === 1'bx)) begin
				dout8x32_28__flag <= 1'b1;
			end else begin
				dout8x32_28__flag<= 1'b0;
			end
			if(!(dout8x32_29__fpga === dout8x32_29__benchmark) && !(dout8x32_29__benchmark === 1'bx)) begin
				dout8x32_29__flag <= 1'b1;
			end else begin
				dout8x32_29__flag<= 1'b0;
			end
			if(!(dout8x32_30__fpga === dout8x32_30__benchmark) && !(dout8x32_30__benchmark === 1'bx)) begin
				dout8x32_30__flag <= 1'b1;
			end else begin
				dout8x32_30__flag<= 1'b0;
			end
			if(!(dout8x32_31__fpga === dout8x32_31__benchmark) && !(dout8x32_31__benchmark === 1'bx)) begin
				dout8x32_31__flag <= 1'b1;
			end else begin
				dout8x32_31__flag<= 1'b0;
			end
		end
	end

	always@(posedge dout8x16_0__flag) begin
		if(dout8x16_0__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_0__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_1__flag) begin
		if(dout8x16_1__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_1__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_2__flag) begin
		if(dout8x16_2__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_2__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_3__flag) begin
		if(dout8x16_3__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_3__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_4__flag) begin
		if(dout8x16_4__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_4__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_5__flag) begin
		if(dout8x16_5__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_5__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_6__flag) begin
		if(dout8x16_6__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_6__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_7__flag) begin
		if(dout8x16_7__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_7__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_8__flag) begin
		if(dout8x16_8__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_8__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_9__flag) begin
		if(dout8x16_9__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_9__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_10__flag) begin
		if(dout8x16_10__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_10__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_11__flag) begin
		if(dout8x16_11__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_11__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_12__flag) begin
		if(dout8x16_12__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_12__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_13__flag) begin
		if(dout8x16_13__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_13__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_14__flag) begin
		if(dout8x16_14__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_14__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x16_15__flag) begin
		if(dout8x16_15__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x16_15__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_0__flag) begin
		if(dout8x32_0__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_0__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_1__flag) begin
		if(dout8x32_1__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_1__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_2__flag) begin
		if(dout8x32_2__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_2__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_3__flag) begin
		if(dout8x32_3__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_3__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_4__flag) begin
		if(dout8x32_4__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_4__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_5__flag) begin
		if(dout8x32_5__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_5__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_6__flag) begin
		if(dout8x32_6__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_6__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_7__flag) begin
		if(dout8x32_7__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_7__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_8__flag) begin
		if(dout8x32_8__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_8__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_9__flag) begin
		if(dout8x32_9__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_9__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_10__flag) begin
		if(dout8x32_10__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_10__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_11__flag) begin
		if(dout8x32_11__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_11__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_12__flag) begin
		if(dout8x32_12__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_12__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_13__flag) begin
		if(dout8x32_13__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_13__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_14__flag) begin
		if(dout8x32_14__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_14__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_15__flag) begin
		if(dout8x32_15__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_15__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_16__flag) begin
		if(dout8x32_16__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_16__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_17__flag) begin
		if(dout8x32_17__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_17__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_18__flag) begin
		if(dout8x32_18__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_18__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_19__flag) begin
		if(dout8x32_19__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_19__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_20__flag) begin
		if(dout8x32_20__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_20__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_21__flag) begin
		if(dout8x32_21__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_21__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_22__flag) begin
		if(dout8x32_22__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_22__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_23__flag) begin
		if(dout8x32_23__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_23__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_24__flag) begin
		if(dout8x32_24__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_24__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_25__flag) begin
		if(dout8x32_25__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_25__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_26__flag) begin
		if(dout8x32_26__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_26__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_27__flag) begin
		if(dout8x32_27__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_27__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_28__flag) begin
		if(dout8x32_28__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_28__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_29__flag) begin
		if(dout8x32_29__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_29__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_30__flag) begin
		if(dout8x32_30__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_30__fpga at time = %t", $realtime);
		end
	end

	always@(posedge dout8x32_31__flag) begin
		if(dout8x32_31__flag) begin
			nb_error = nb_error + 1;
			$display("Mismatch on dout8x32_31__fpga at time = %t", $realtime);
		end
	end


// ----- Configuration done must be raised in the end -------
	always@(posedge __config_all_done__[0]) begin
		nb_error = nb_error - 1;
	end

// ----- Begin output waveform to VCD file-------
	initial begin
		$dumpfile("dual_port_ram_8x16_8x32_formal.vcd");
		$dumpvars(1, dual_port_ram_8x16_8x32_autocheck_top_tb);
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
// ----- END Verilog module for dual_port_ram_8x16_8x32_autocheck_top_tb -----

//----- Default net type -----
`default_nettype wire
