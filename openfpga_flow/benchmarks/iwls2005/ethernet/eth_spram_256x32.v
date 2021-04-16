//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_spram_256x32.v                                          ////
////                                                              ////
////  This file is part of the Ethernet IP core project           ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Igor Mohor (igorM@opencores.org)                      ////
////                                                              ////
////  All additional information is available in the Readme.txt   ////
////  file.                                                       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2001, 2002 Authors                             ////
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
// $Log: eth_spram_256x32.v,v $
// Revision 1.10  2005/02/21 12:48:07  igorm
// Warning fixes.
//
// Revision 1.9  2003/12/05 12:43:06  tadejm
// Corrected address mismatch for xilinx RAMB4_S8 model which has wider address than RAMB4_S16.
//
// Revision 1.8  2003/12/04 14:59:13  simons
// Lapsus fixed (!we -> ~we).
//
// Revision 1.7  2003/11/12 18:24:59  tadejm
// WISHBONE slave changed and tested from only 32-bit accesss to byte access.
//
// Revision 1.6  2003/10/17 07:46:15  markom
// mbist signals updated according to newest convention
//
// Revision 1.5  2003/08/14 16:42:58  simons
// Artisan ram instance added.
//
// Revision 1.4  2002/10/18 17:04:20  tadejm
// Changed BIST scan signals.
//
// Revision 1.3  2002/10/10 16:29:30  mohor
// BIST added.
//
// Revision 1.2  2002/09/23 18:24:31  mohor
// ETH_VIRTUAL_SILICON_RAM supported (for ASIC implementation).
//
// Revision 1.1  2002/07/23 16:36:09  mohor
// ethernet spram added. So far a generic ram and xilinx RAMB4 are used.
//
//
//

`include "eth_defines.v"
`include "timescale.v"

module eth_spram_256x32(
	// Generic synchronous single-port RAM interface
	clk, rst, ce, we, oe, addr, di, do

`ifdef ETH_BIST
  ,
  // debug chain signals
  mbist_si_i,       // bist scan serial in
  mbist_so_o,       // bist scan serial out
  mbist_ctrl_i        // bist chain shift control
`endif



);

	//
	// Generic synchronous single-port RAM interface
	//
	input           clk;  // Clock, rising edge
	input           rst;  // Reset, active high
	input           ce;   // Chip enable input, active high
	input  [3:0]    we;   // Write enable input, active high
	input           oe;   // Output enable input, active high
	input  [7:0]    addr; // address bus inputs
	input  [31:0]   di;   // input data bus
	output [31:0]   do;   // output data bus


`ifdef ETH_BIST
  input   mbist_si_i;       // bist scan serial in
  output  mbist_so_o;       // bist scan serial out
  input [`ETH_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i;       // bist chain shift control
`endif

`ifdef ETH_XILINX_RAMB4

    /*RAMB4_S16 ram0
    (
        .DO      (do[15:0]),
        .ADDR    (addr),
        .DI      (di[15:0]),
        .EN      (ce),
        .CLK     (clk),
        .WE      (we),
        .RST     (rst)
    );

    RAMB4_S16 ram1
    (
        .DO      (do[31:16]),
        .ADDR    (addr),
        .DI      (di[31:16]),
        .EN      (ce),
        .CLK     (clk),
        .WE      (we),
        .RST     (rst)
    );*/

    RAMB4_S8 ram0
    (
        .DO      (do[7:0]),
        .ADDR    ({1'b0, addr}),
        .DI      (di[7:0]),
        .EN      (ce),
        .CLK     (clk),
        .WE      (we[0]),
        .RST     (rst)
    );

    RAMB4_S8 ram1
    (
        .DO      (do[15:8]),
        .ADDR    ({1'b0, addr}),
        .DI      (di[15:8]),
        .EN      (ce),
        .CLK     (clk),
        .WE      (we[1]),
        .RST     (rst)
    );

    RAMB4_S8 ram2
    (
        .DO      (do[23:16]),
        .ADDR    ({1'b0, addr}),
        .DI      (di[23:16]),
        .EN      (ce),
        .CLK     (clk),
        .WE      (we[2]),
        .RST     (rst)
    );

    RAMB4_S8 ram3
    (
        .DO      (do[31:24]),
        .ADDR    ({1'b0, addr}),
        .DI      (di[31:24]),
        .EN      (ce),
        .CLK     (clk),
        .WE      (we[3]),
        .RST     (rst)
    );

`else   // !ETH_XILINX_RAMB4
`ifdef  ETH_VIRTUAL_SILICON_RAM
  `ifdef ETH_BIST
      //vs_hdsp_256x32_bist ram0_bist
      vs_hdsp_256x32_bw_bist ram0_bist
  `else
      //vs_hdsp_256x32 ram0
      vs_hdsp_256x32_bw ram0
  `endif
      (
        .CK         (clk),
        .CEN        (!ce),
        .WEN        (~we),
        .OEN        (!oe),
        .ADR        (addr),
        .DI         (di),
        .DOUT       (do)

      `ifdef ETH_BIST
        ,
        // debug chain signals
        .mbist_si_i       (mbist_si_i),
        .mbist_so_o       (mbist_so_o),
        .mbist_ctrl_i       (mbist_ctrl_i)
      `endif
      );

`else   // !ETH_VIRTUAL_SILICON_RAM

`ifdef  ETH_ARTISAN_RAM
  `ifdef ETH_BIST
      //art_hssp_256x32_bist ram0_bist
      art_hssp_256x32_bw_bist ram0_bist
  `else
      //art_hssp_256x32 ram0
      art_hssp_256x32_bw ram0
  `endif
      (
        .CLK        (clk),
        .CEN        (!ce),
        .WEN        (~we),
        .OEN        (!oe),
        .A          (addr),
        .D          (di),
        .Q          (do)

      `ifdef ETH_BIST
        ,
        // debug chain signals
        .mbist_si_i       (mbist_si_i),
        .mbist_so_o       (mbist_so_o),
        .mbist_ctrl_i     (mbist_ctrl_i)
      `endif
      );

`else   // !ETH_ARTISAN_RAM
`ifdef ETH_ALTERA_ALTSYNCRAM

    altera_spram_256x32	altera_spram_256x32_inst
    (
  	  .address        (addr),
  	  .wren           (ce & we),
  	  .clock          (clk),
  	  .data           (di),
  	  .q              (do)
  	);  //exemplar attribute altera_spram_256x32_inst NOOPT TRUE

`else   // !ETH_ALTERA_ALTSYNCRAM


	//
	// Generic single-port synchronous RAM model
	//

	//
	// Generic RAM's registers and wires
	//
	reg  [ 7: 0] mem0 [255:0]; // RAM content
	reg  [15: 8] mem1 [255:0]; // RAM content
	reg  [23:16] mem2 [255:0]; // RAM content
	reg  [31:24] mem3 [255:0]; // RAM content
	wire [31:0]  q;            // RAM output
	reg  [7:0]   raddr;        // RAM read address
	//
	// Data output drivers
	//
	assign do = (oe & ce) ? q : {32{1'bz}};

	//
	// RAM read and write
	//

	// read operation
	always@(posedge clk)
	  if (ce) // && !we)
		raddr <= #1 addr;    // read address needs to be registered to read clock

	assign #1 q = rst ? {32{1'b0}} : {mem3[raddr], mem2[raddr], mem1[raddr], mem0[raddr]};

	// write operation
	always@(posedge clk)
        begin
		if (ce && we[3])
			mem3[addr] <= #1 di[31:24];
		if (ce && we[2])
			mem2[addr] <= #1 di[23:16];
		if (ce && we[1])
			mem1[addr] <= #1 di[15: 8];
		if (ce && we[0])
			mem0[addr] <= #1 di[ 7: 0];
        end

	// Task prints range of memory
	// *** Remember that tasks are non reentrant, don't call this task in parallel for multiple instantiations. 
	task print_ram;
	input [7:0] start;
	input [7:0] finish;
	integer rnum;
  	begin
    		for (rnum=start;rnum<=finish;rnum=rnum+1)
      			$display("Addr %h = %0h %0h %0h %0h",rnum,mem3[rnum],mem2[rnum],mem1[rnum],mem0[rnum]);
  	end
	endtask

`endif  // !ETH_ALTERA_ALTSYNCRAM
`endif  // !ETH_ARTISAN_RAM
`endif  // !ETH_VIRTUAL_SILICON_RAM
`endif  // !ETH_XILINX_RAMB4

endmodule
