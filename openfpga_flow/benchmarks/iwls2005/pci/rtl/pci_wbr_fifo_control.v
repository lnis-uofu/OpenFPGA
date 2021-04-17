//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "wbr_fifo_control.v"                              ////
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
// $Log: pci_wbr_fifo_control.v,v $
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
// Revision 1.6  2002/11/27 20:36:12  mihad
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
// Revision 1.3  2002/02/01 15:25:13  mihad
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

module pci_wbr_fifo_control
(
    rclock_in,
    wclock_in,
    renable_in,
    wenable_in,
    reset_in,
    flush_in,
    empty_out,
    waddr_out,
    raddr_out,
    rallow_out,
    wallow_out
) ;

parameter ADDR_LENGTH = 7 ;

// independent clock inputs - rclock_in = read clock, wclock_in = write clock
input  rclock_in, wclock_in;

// enable inputs - read address changes on rising edge of rclock_in when reads are allowed
//                 write address changes on rising edge of wclock_in when writes are allowed
input  renable_in, wenable_in;

// reset input
input  reset_in;

// flush input
input flush_in ;

// empty status output
output empty_out;

// read and write addresses outputs
output [(ADDR_LENGTH - 1):0] waddr_out, raddr_out;

// read and write allow outputs
output rallow_out, wallow_out ;

// read address register
reg [(ADDR_LENGTH - 1):0] raddr ;

// write address register
reg [(ADDR_LENGTH - 1):0] waddr;
assign waddr_out = waddr ;

// grey code register
reg [(ADDR_LENGTH - 1):0] wgrey_addr ;

// next write gray address calculation - bitwise xor between address and shifted address
wire [(ADDR_LENGTH - 2):0] calc_wgrey_next  = waddr[(ADDR_LENGTH - 1):1] ^ waddr[(ADDR_LENGTH - 2):0] ;

// grey code register
reg [(ADDR_LENGTH - 1):0] rgrey_addr ;

// next read gray address calculation - bitwise xor between address and shifted address
wire [(ADDR_LENGTH - 2):0] calc_rgrey_next  = raddr[(ADDR_LENGTH - 1):1] ^ raddr[(ADDR_LENGTH - 2):0] ;

// FF for registered empty flag
wire empty ;

// write allow wire
wire wallow = wenable_in ;

// write allow output assignment
assign wallow_out = wallow ;

// read allow wire
wire rallow ;

// clear generation for FFs and registers
wire clear = reset_in /*|| flush_in*/ ; // flush changed to synchronous operation

assign empty_out = empty ;

//rallow generation
assign rallow = renable_in && !empty ; // reads allowed if read enable is high and FIFO is not empty

// rallow output assignment
assign rallow_out = renable_in ;

// at any clock edge that rallow is high, this register provides next read address, so wait cycles are not necessary
// when FIFO is empty, this register provides actual read address, so first location can be read
reg [(ADDR_LENGTH - 1):0] raddr_plus_one ;

// address output mux - when FIFO is empty, current actual address is driven out, when it is non - empty next address is driven out
// done for zero wait state burst
assign raddr_out = rallow ? raddr_plus_one : raddr ;

always@(posedge rclock_in or posedge clear)
begin
    if (clear)
    begin
        raddr_plus_one <= #`FF_DELAY 2 ;
        raddr          <= #`FF_DELAY 1 ;
    end
    else if (flush_in)
    begin
        raddr_plus_one <= #`FF_DELAY waddr + 1'b1 ; 
        raddr          <= #`FF_DELAY waddr ;
    end
    else if (rallow)
    begin
        raddr_plus_one <= #`FF_DELAY raddr_plus_one + 1'b1 ;
        raddr          <= #`FF_DELAY raddr_plus_one ;
    end
end

/*-----------------------------------------------------------------------------------------------
Read address control consists of Read address counter and Grey Address register
--------------------------------------------------------------------------------------------------*/
// grey coded address
always@(posedge rclock_in or posedge clear)
begin
    if (clear)
    begin
        rgrey_addr <= #`FF_DELAY 0 ;
    end
    else if (flush_in)
    begin
        rgrey_addr <= #`FF_DELAY wgrey_addr ;   // when flushed, copy value from write side
    end
    else if (rallow)
    begin
        rgrey_addr <= #`FF_DELAY {raddr[ADDR_LENGTH - 1], calc_rgrey_next} ;
    end
end

/*--------------------------------------------------------------------------------------------
Write address control consists of write address counter and Grey Code Register
----------------------------------------------------------------------------------------------*/
// grey coded address for status generation in write clock domain
always@(posedge wclock_in or posedge clear)
begin
    if (clear)
    begin
        wgrey_addr <= #1 0 ;
    end
    else
    if (wallow)
    begin
        wgrey_addr <= #1 {waddr[(ADDR_LENGTH - 1)], calc_wgrey_next} ;
    end
end

// write address counter - nothing special except initial value
always@(posedge wclock_in or posedge clear)
begin
    if (clear)
        // initial value is 1
        waddr <= #`FF_DELAY 1 ;
    else
    if (wallow)
        waddr <= #`FF_DELAY waddr + 1'b1 ;
end


/*------------------------------------------------------------------------------------------------------------------------------
Empty control:
Gray coded write address pointer is synchronized to read clock domain and compared to Gray coded read address pointer.
If they are equal, fifo is empty.
--------------------------------------------------------------------------------------------------------------------------------*/
wire [(ADDR_LENGTH - 1):0] rclk_sync_wgrey_addr ;
reg  [(ADDR_LENGTH - 1):0] rclk_wgrey_addr ;
pci_synchronizer_flop #(ADDR_LENGTH, 0) i_synchronizer_reg_wgrey_addr
(
    .data_in        (wgrey_addr),
    .clk_out        (rclock_in),
    .sync_data_out  (rclk_sync_wgrey_addr),
    .async_reset    (clear)
) ;

always@(posedge rclock_in or posedge clear)
begin
    if (clear)
        rclk_wgrey_addr <= #`FF_DELAY 0 ;
    else
        rclk_wgrey_addr <= #`FF_DELAY rclk_sync_wgrey_addr ;
end

assign empty = (rgrey_addr == rclk_wgrey_addr) ;
endmodule
