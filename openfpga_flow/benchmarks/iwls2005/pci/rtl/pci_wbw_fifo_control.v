//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "wbw_fifo_control.v"                              ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////                                                              ////
////  All additional information is avaliable in the README       ////
////  file.                                                       ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2001 Miha Dolenc, mihad@opencores.org          ////
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
// $Log: pci_wbw_fifo_control.v,v $
// Revision 1.4  2003/08/14 13:06:03  simons
// synchronizer_flop replaced with pci_synchronizer_flop, artisan ram instance updated.
//
// Revision 1.3  2003/07/29 08:20:11  mihad
// Found and simulated the problem in the synchronization logic.
// Repaired the synchronization logic in the FIFOs.
//
// Revision 1.2  2003/03/26 13:16:18  mihad
// Added the reset value parameter to the synchronizer flop module.
// Added resets to all synchronizer flop instances.
// Repaired initial sync value in fifos.
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.6  2002/11/27 20:36:13  mihad
// Changed the code a bit to make it more readable.
// Functionality not changed in any way.
// More robust synchronization in fifos is still pending.
//
// Revision 1.5  2002/09/30 16:03:04  mihad
// Added meta flop module for easier meta stable FF identification during synthesis
//
// Revision 1.4  2002/09/25 15:53:52  mihad
// Removed all logic from asynchronous reset network
//
// Revision 1.3  2002/02/01 15:25:14  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:30  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:47  mihad
// New project directory structure
//
//

/* FIFO_CONTROL module provides read/write address and status generation for
   FIFOs implemented with standard dual port SRAM cells in ASIC or FPGA designs */
`include "pci_constants.v"
// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_wbw_fifo_control
(
    rclock_in,
    wclock_in,
    renable_in,
    wenable_in,
    reset_in,
    almost_full_out,
    full_out,
    empty_out,
    waddr_out,
    raddr_out,
    rallow_out,
    wallow_out
);

parameter ADDR_LENGTH = 7 ;

// independent clock inputs - rclock_in = read clock, wclock_in = write clock
input  rclock_in, wclock_in;

// enable inputs - read address changes on rising edge of rclock_in when reads are allowed
//                 write address changes on rising edge of wclock_in when writes are allowed
input  renable_in, wenable_in ;

// reset input
input  reset_in;

// flush input
// input flush_in ; // not used

// almost full and empy status outputs
output almost_full_out ;

// full and empty status outputs
output full_out, empty_out;

// read and write addresses outputs
output [(ADDR_LENGTH - 1):0] waddr_out, raddr_out;

// read and write allow outputs
output rallow_out, wallow_out ;

// read address register
reg [(ADDR_LENGTH - 1):0] raddr ;

// write address register
reg [(ADDR_LENGTH - 1):0] waddr;
assign waddr_out = waddr ;

// grey code registers
reg [(ADDR_LENGTH - 1):0] wgrey_addr ; // current
// grey code register for next write address
reg [(ADDR_LENGTH - 1):0] wgrey_next ; // next

// next write gray address calculation - bitwise xor between address and shifted address
wire [(ADDR_LENGTH - 2):0] calc_wgrey_next  = waddr[(ADDR_LENGTH - 1):1] ^ waddr[(ADDR_LENGTH - 2):0] ;

// grey code pipeline for read address
reg [(ADDR_LENGTH - 1):0] rgrey_minus1 ; // one before current
reg [(ADDR_LENGTH - 1):0] rgrey_addr ; // current
reg [(ADDR_LENGTH - 1):0] rgrey_next ; // next

// next read gray address calculation - bitwise xor between address and shifted address
wire [(ADDR_LENGTH - 2):0] calc_rgrey_next  = raddr[(ADDR_LENGTH - 1):1] ^ raddr[(ADDR_LENGTH - 2):0] ;

// write allow wire - writes are allowed when fifo is not full
assign wallow_out = wenable_in & ~full_out ;

// clear generation for FFs and registers
wire clear = reset_in ;

//rallow generation
assign rallow_out = renable_in & ~empty_out ; // reads allowed if read enable is high and FIFO is not empty

// at any clock edge that rallow is high, this register provides next read address, so wait cycles are not necessary
// when FIFO is empty, this register provides actual read address, so first location can be read
reg [(ADDR_LENGTH - 1):0] raddr_plus_one ;

// address output mux - when FIFO is empty, current actual address is driven out, when it is non - empty next address is driven out
// done for zero wait state burst
assign raddr_out = rallow_out ? raddr_plus_one : raddr ;

always@(posedge rclock_in or posedge clear)
begin
    if (clear)
    begin
        raddr_plus_one <= #`FF_DELAY 4 ;
        raddr          <= #`FF_DELAY 3 ;
    end
    else if (rallow_out)
    begin
        raddr_plus_one <= #`FF_DELAY raddr_plus_one + 1'b1 ;
        raddr          <= #`FF_DELAY raddr_plus_one ;
    end
end

/*-----------------------------------------------------------------------------------------------
Read address control consists of Read address counter and Grey Address pipeline
There are 3 Grey addresses:
    - rgrey_minus1 is Grey Code of address one before current address
    - rgrey_addr is Grey Code of current read address
    - rgrey_next is Grey Code of next read address
--------------------------------------------------------------------------------------------------*/
// grey coded address pipeline for status generation in read clock domain
always@(posedge rclock_in or posedge clear)
begin
	if (clear)
    begin
        // initial value is 0
        rgrey_minus1 <= #1 0 ;
        rgrey_addr   <= #1 1 ;
        rgrey_next   <= #`FF_DELAY 3 ;
    end
    else
    if (rallow_out)
    begin
        rgrey_minus1 <= #1 rgrey_addr ;
        rgrey_addr   <= #1 rgrey_next ;
        rgrey_next   <= #`FF_DELAY {raddr[ADDR_LENGTH - 1], calc_rgrey_next} ;
    end
end

/*--------------------------------------------------------------------------------------------
Write address control consists of write address counter and Grey Code Register
----------------------------------------------------------------------------------------------*/
// grey coded address pipeline for status generation in write clock domain
always@(posedge wclock_in or posedge clear)
begin
    if (clear)
    begin
        wgrey_addr <= #`FF_DELAY 1 ;
        wgrey_next <= #1 3         ;
    end
    else
    if (wallow_out)
    begin
        wgrey_addr <= #`FF_DELAY wgrey_next ;
        wgrey_next <= #1 {waddr[(ADDR_LENGTH - 1)], calc_wgrey_next} ;
    end
end

// write address counter - nothing special - initial value is important though
always@(posedge wclock_in or posedge clear)
begin
    if (clear)
        // initial value 4
	    waddr <= #`FF_DELAY 3 ;
    else
    if (wallow_out)
        waddr <= #`FF_DELAY waddr + 1'b1 ;
end

/*------------------------------------------------------------------------------------------------------------------------------
Gray coded address of read address decremented by 1 is synchronized to write clock domain and compared to:

- Gray coded write address. If they are equal, fifo is full.

- Gray coded next write address. If they are equal, fifo is almost full.
--------------------------------------------------------------------------------------------------------------------------------*/
wire [(ADDR_LENGTH - 1):0] wclk_sync_rgrey_minus1 ;
reg  [(ADDR_LENGTH - 1):0] wclk_rgrey_minus1 ;

pci_synchronizer_flop #(ADDR_LENGTH, 0) i_synchronizer_reg_rgrey_minus1
(
    .data_in        (rgrey_minus1),
    .clk_out        (wclock_in),
    .sync_data_out  (wclk_sync_rgrey_minus1),
    .async_reset    (clear)
) ;

always@(posedge wclock_in or posedge clear)
begin
    if (clear)
    begin
        wclk_rgrey_minus1 <= #`FF_DELAY 0 ;
    end
    else
    begin
        wclk_rgrey_minus1 <= #`FF_DELAY wclk_sync_rgrey_minus1 ;
    end
end

assign full_out        = (wgrey_addr == wclk_rgrey_minus1) ;
assign almost_full_out = (wgrey_next == wclk_rgrey_minus1) ;

/*------------------------------------------------------------------------------------------------------------------------------
Empty control:
Gray coded address of next write address is synchronized to read clock domain and compared to Gray coded next read address.
If they are equal, fifo is empty.
--------------------------------------------------------------------------------------------------------------------------------*/
wire [(ADDR_LENGTH - 1):0] rclk_sync_wgrey_next ;
reg  [(ADDR_LENGTH - 1):0] rclk_wgrey_next ;
pci_synchronizer_flop #(ADDR_LENGTH, 3) i_synchronizer_reg_wgrey_next
(
    .data_in        (wgrey_next),
    .clk_out        (rclock_in),
    .sync_data_out  (rclk_sync_wgrey_next),
    .async_reset    (clear)
) ;

always@(posedge rclock_in or posedge clear)
begin
    if (clear)
        rclk_wgrey_next <= #`FF_DELAY 3 ;
    else
        rclk_wgrey_next <= #`FF_DELAY rclk_sync_wgrey_next ;
end

assign empty_out = (rgrey_next == rclk_wgrey_next) ;

endmodule
