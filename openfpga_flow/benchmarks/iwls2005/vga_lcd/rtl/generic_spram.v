//////////////////////////////////////////////////////////////////////
////                                                              ////
////  Generic Single-Port Synchronous RAM                         ////
////                                                              ////
////  This file is part of memory library available from          ////
////  http://www.opencores.org/cvsweb.shtml/generic_memories/     ////
////                                                              ////
////  Description                                                 ////
////  This block is a wrapper with common single-port             ////
////  synchronous memory interface for different                  ////
////  types of ASIC and FPGA RAMs. Beside universal memory        ////
////  interface it also provides a behavioral model of generic    ////
////  single-port synchronous RAM.                                ////
////  It also contains a synthesizeable model for FPGAs.          ////
////  It should be used in all OPENCORES designs that want to be  ////
////  portable accross different target technologies and          ////
////  independent of target memory.                               ////
////                                                              ////
////  Supported ASIC RAMs are:                                    ////
////  - Artisan Single-Port Sync RAM                              ////
////  - Avant! Two-Port Sync RAM (*)                              ////
////  - Virage Single-Port Sync RAM                               ////
////  - Virtual Silicon Single-Port Sync RAM                      ////
////                                                              ////
////  Supported FPGA RAMs are:                                    ////
////  - Generic FPGA (VENDOR_FPGA)                                ////
////    Tested RAMs: Altera, Xilinx                               ////
////    Synthesis tools: LeonardoSpectrum, Synplicity             ////
////  - Xilinx (VENDOR_XILINX)                                    ////
////  - Altera (VENDOR_ALTERA)                                    ////
////                                                              ////
////  To Do:                                                      ////
////   - fix avant! two-port ram                                  ////
////   - add additional RAMs                                      ////
////                                                              ////
////  Author(s):                                                  ////
////      - Richard Herveille, richard@asics.ws                   ////
////      - Damjan Lampret, lampret@opencores.org                 ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: generic_spram.v,v $
// Revision 1.3  2003/03/18 21:45:48  rherveille
// Added WISHBONE revB.3 Registered Feedback Cycles support
//
//
//

`include "timescale.v"

//`define VENDOR_XILINX
//`define VENDOR_ALTERA
`define VENDOR_FPGA

module generic_spram(
	// Generic synchronous single-port RAM interface
	clk, rst, ce, we, oe, addr, di, do
);

	//
	// Default address and data buses width
	//
	parameter aw = 6; //number of address-bits
	parameter dw = 8; //number of data-bits

	//
	// Generic synchronous single-port RAM interface
	//
	input           clk;  // Clock, rising edge
	input           rst;  // Reset, active high
	input           ce;   // Chip enable input, active high
	input           we;   // Write enable input, active high
	input           oe;   // Output enable input, active high
	input  [aw-1:0] addr; // address bus inputs
	input  [dw-1:0] di;   // input data bus
	output [dw-1:0] do;   // output data bus

	//
	// Module body
	//

`ifdef VENDOR_FPGA
	//
	// Instantiation synthesizeable FPGA memory
	//
	// This code has been tested using LeonardoSpectrum and Synplicity.
	// The code correctly instantiates Altera EABs and Xilinx BlockRAMs.
	//

	// NOTE:
	// 'synthesis syn_ramstyle="block_ram"' is a Synplify attribute.
	// It instructs Synplify to map to BlockRAMs instead of the default SelectRAMs

	reg [dw-1:0] mem [(1<<aw) -1:0] /* synthesis syn_ramstyle="block_ram" */;
	reg [aw-1:0] ra;

	// read operation
	always @(posedge clk)
	  if (ce)
	    ra <= #1 addr;     // read address needs to be registered to read clock

	assign #1 do = mem[ra];

	// write operation
	always @(posedge clk)
	  if (we && ce)
	    mem[addr] <= #1 di;
`else

`ifdef VENDOR_XILINX

	wire [dw-1:0] q;  // output from xilinx ram
	//
	// Instantiation of FPGA memory:
	//
	// Virtex/Spartan2 BlockRAMs
	//
	xilinx_ram_sp xilinx_ram(
		.clk(clk),
		.rst(rst),
		.addr(addr),
		.di(di),
		.en(ce),
		.we(we),
		.do(do)
	);

	defparam
		xilinx_ram.dwidth = dw,
		xilinx_ram.awidth = aw;

`else

`ifdef VENDOR_ALTERA

	//
	// Instantiation of FPGA memory:
	//
	// Altera FLEX EABs
	//

	altera_ram_sp altera_ram(
		.inclock(clk),
		.address(addr),
		.data(di),
		.we(we && ce),
		.q(do)
	);

	defparam
		altera_ram.dwidth = dw,
		altera_ram.awidth = aw;

`else

`ifdef VENDOR_ARTISAN

	//
	// Instantiation of ASIC memory:
	//
	// Artisan Synchronous Single-Port RAM (ra1sh)
	//
	artisan_ssp #(dw, 1<<aw, aw) artisan_ssp(
		.CLK(clk),
		.CEN(~ce),
		.WEN(~we),
		.A(addr),
		.D(di),
		.OEN(~oe),
		.Q(do)
	);

`else

`ifdef VENDOR_AVANT

	//
	// Instantiation of ASIC memory:
	//
	// Avant! Asynchronous Two-Port RAM
	//
	avant_atp avant_atp(
		.web(~we),
		.reb(),
		.oeb(~oe),
		.rcsb(),
		.wcsb(),
		.ra(addr),
		.wa(addr),
		.di(di),
		.do(do)
	);

`else

`ifdef VENDOR_VIRAGE

	//
	// Instantiation of ASIC memory:
	//
	// Virage Synchronous 1-port R/W RAM
	//
	virage_ssp virage_ssp(
		.clk(clk),
		.adr(addr),
		.d(di),
		.we(we),
		.oe(oe),
		.me(ce),
		.q(do)
	);

`else

`ifdef VENDOR_VIRTUALSILICON

	//
	// Instantiation of ASIC memory:
	//
	// Virtual Silicon Single-Port Synchronous SRAM
	//
	virtualsilicon_spram #(1<<aw, aw-1, dw-1) virtualsilicon_ssp(
		.CK(clk),
		.ADR(addr),
		.DI(di),
		.WEN(~we),
		.CEN(~ce),
		.OEN(~oe),
		.DOUT(do)
	);

`else

	//
	// Generic single-port synchronous RAM model
	//

	//
	// Generic RAM's registers and wires
	//
	reg  [dw-1:0] mem [(1<<aw)-1:0];	// RAM content
	wire [dw-1:0] q;                 // RAM output
	reg  [aw-1:0] raddr;             // RAM read address
	//
	// Data output drivers
	//
	assign do = (oe) ? q : {dw{1'bz}};

	//
	// RAM read and write
	//

	// read operation
	always@(posedge clk)
	if (ce) // && !we)
		raddr <= #1 addr;    // read address needs to be registered to read clock

	assign #1 q = rst ? {dw{1'b0}} : mem[raddr];

	// write operation
	always@(posedge clk)
		if (ce && we)
			mem[addr] <= #1 di;


`endif // !VIRTUALSILICON_SSP
`endif // !VIRAGE_SSP
`endif // !AVANT_ATP
`endif // !ARTISAN_SSP
`endif // !VENDOR_ALTERA
`endif // !VENDOR_XILINX
`endif // !VENDOR_FPGA

endmodule


//
// Black-box modules
//

`ifdef VENDOR_ALTERA
	module altera_ram_sp (
		address,
		inclock,
		we,
		data,
		q) /* synthesis black_box */;

		parameter awidth = 7;
		parameter dwidth = 8;

		input  [awidth -1:0] address;
		input                inclock;
		input                we;
		input  [dwidth -1:0] data;
		output [dwidth -1:0] q;

		// synopsis translate_off
		// exemplar translate_off

		syn_ram_irou #(
			"UNUSED",
			dwidth,
			awidth,
			1 << awidth
		)
		altera_spram_model (
			.Inclock(inclock),
			.Address(address),
			.Data(data),
			.WE(we),
			.Q(q)
		);

		// exemplar translate_on
		// synopsis translate_on

	endmodule
`endif // VENDOR_ALTERA

`ifdef VENDOR_XILINX
	module xilinx_ram_sp (
			clk,
			rst,
			addr,
			di,
			en,
			we,
			do) /* synthesis black_box */ ;

		parameter awidth = 7;
		parameter dwidth = 8;

		input                clk;
		input                rst;
		input  [awidth -1:0] addr;
		input  [dwidth -1:0] di;
		input                en;
		input                we;
		output [dwidth -1:0] do;

		// insert simulation model


		// synopsys translate_off
		// exemplar translate_off

		C_MEM_SP_BLOCK_V1_0 #(
			awidth,
			1,
			"0",
			1 << awidth,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			"",
			16,
			0,
			0,
			1,
			1,
			dwidth
		)
		xilinx_spram_model (
			.CLK(clk),
			.RST(rst),
			.ADDR(addr),
			.DI(di),
			.EN(en),
			.WE(we),
			.DO(do)
		);

		// exemplar translate_on
		// synopsys translate_on

	endmodule
`endif // VENDOR_XILINX

