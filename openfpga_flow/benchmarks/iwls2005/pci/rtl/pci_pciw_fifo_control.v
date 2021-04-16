//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pciw_fifo_control.v"                             ////
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
// $Log: pci_pciw_fifo_control.v,v $
// Revision 1.5  2003/08/14 13:06:03  simons
// synchronizer_flop replaced with pci_synchronizer_flop, artisan ram instance updated.
//
// Revision 1.4  2003/08/08 16:36:33  tadejm
// Added 'three_left_out' to pci_pciw_fifo signaling three locations before full. Added comparison between current registered cbe and next unregistered cbe to signal wb_master whether it is allowed to performe burst or not. Due to this, I needed 'three_left_out' so that writing to pci_pciw_fifo can be registered, otherwise timing problems would occure.
//
// Revision 1.3  2003/07/29 08:20:11  mihad
// Found and simulated the problem in the synchronization logic.
// Repaired the synchronization logic in the FIFOs.
//
//

/* FIFO_CONTROL module provides read/write address and status generation for
   FIFOs implemented with standard dual port SRAM cells in ASIC or FPGA designs */
`include "pci_constants.v"
// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_pciw_fifo_control
(
    rclock_in,
    wclock_in,
    renable_in,
    wenable_in,
    reset_in,
    almost_full_out,
    full_out,
    almost_empty_out,
    empty_out,
    waddr_out,
    raddr_out,
    rallow_out,
    wallow_out,
    three_left_out,
    two_left_out
);

parameter ADDR_LENGTH = 7 ;

// independent clock inputs - rclock_in = read clock, wclock_in = write clock
input  rclock_in, wclock_in;

// enable inputs - read address changes on rising edge of rclock_in when reads are allowed
//                 write address changes on rising edge of wclock_in when writes are allowed
input  renable_in, wenable_in;

// reset input
input  reset_in;

// almost full and empy status outputs
output almost_full_out, almost_empty_out;

// full and empty status outputs
output full_out, empty_out;

// read and write addresses outputs
output [(ADDR_LENGTH - 1):0] waddr_out, raddr_out;

// read and write allow outputs
output rallow_out, wallow_out ;

// three and two locations left output indicator
output three_left_out ;
output two_left_out ;

// read address register
reg [(ADDR_LENGTH - 1):0] raddr ;

// write address register
reg [(ADDR_LENGTH - 1):0] waddr;
reg [(ADDR_LENGTH - 1):0] waddr_plus1;
assign waddr_out = waddr ;

// grey code registers
// grey code pipeline for write address
reg [(ADDR_LENGTH - 1):0] wgrey_minus1 ; // previous
reg [(ADDR_LENGTH - 1):0] wgrey_addr   ; // current
reg [(ADDR_LENGTH - 1):0] wgrey_next   ; // next

reg [(ADDR_LENGTH - 1):0] wgrey_next_plus1   ; // next plus 1


// next write gray address calculation - bitwise xor between address and shifted address
wire [(ADDR_LENGTH - 2):0] calc_wgrey_next  = waddr[(ADDR_LENGTH - 1):1] ^ waddr[(ADDR_LENGTH - 2):0] ;
wire [(ADDR_LENGTH - 2):0] calc_wgrey_next_plus1  = waddr_plus1[(ADDR_LENGTH - 1):1] ^ waddr_plus1[(ADDR_LENGTH - 2):0] ;

// grey code pipeline for read address
reg [(ADDR_LENGTH - 1):0] rgrey_minus2 ; // two before current
reg [(ADDR_LENGTH - 1):0] rgrey_minus1 ; // one before current
reg [(ADDR_LENGTH - 1):0] rgrey_addr ; // current
reg [(ADDR_LENGTH - 1):0] rgrey_next ; // next

// next read gray address calculation - bitwise xor between address and shifted address
wire [(ADDR_LENGTH - 2):0] calc_rgrey_next  = raddr[(ADDR_LENGTH - 1):1] ^ raddr[(ADDR_LENGTH - 2):0] ;

// write allow - writes are allowed when fifo is not full
assign wallow_out = wenable_in & ~full_out ;

// clear generation for FFs and registers
wire clear = reset_in ;

//rallow generation
assign rallow_out = renable_in & ~empty_out ; // reads allowed if read enable is high and FIFO is not empty

// at any clock edge that rallow is high, this register provides next read address, so wait cycles are not necessary
// when FIFO is empty, this register provides actual read address, so first location can be read
reg [(ADDR_LENGTH - 1):0] raddr_plus_one ;


// read address mux - when read is performed, next address is driven, so next data is available immediately after read
// this is convenient for zero wait stait bursts
assign raddr_out = rallow_out ? raddr_plus_one : raddr ;

always@(posedge rclock_in or posedge clear)
begin
    if (clear)
    begin
        // initial values seem a bit odd - they are this way to allow easier grey pipeline implementation and to allow min fifo size of 8
        raddr_plus_one <= #`FF_DELAY 5 ;
        raddr          <= #`FF_DELAY 4 ;
//        raddr_plus_one <= #`FF_DELAY 6 ;
//        raddr          <= #`FF_DELAY 5 ;
    end
    else if (rallow_out)
    begin
        raddr_plus_one <= #`FF_DELAY raddr_plus_one + 1'b1 ;
        raddr          <= #`FF_DELAY raddr_plus_one ;
    end
end

/*-----------------------------------------------------------------------------------------------
Read address control consists of Read address counter and Grey Address pipeline
There are 4 Grey addresses:
    - rgrey_minus2 is Grey Code of address two before current address
    - rgrey_minus1 is Grey Code of address one before current address
    - rgrey_addr is Grey Code of current read address
    - rgrey_next is Grey Code of next read address
--------------------------------------------------------------------------------------------------*/
// grey coded address pipeline for status generation in read clock domain
always@(posedge rclock_in or posedge clear)
begin
    if (clear)
    begin
        rgrey_minus2 <= #1 0 ;
        rgrey_minus1 <= #`FF_DELAY 1 ;  
        rgrey_addr   <= #1 3 ;
        rgrey_next   <= #`FF_DELAY 2 ;
    end
    else
    if (rallow_out)
    begin
        rgrey_minus2 <= #1 rgrey_minus1 ;
        rgrey_minus1 <= #`FF_DELAY rgrey_addr ;
        rgrey_addr   <= #1 rgrey_next ;
        rgrey_next   <= #`FF_DELAY {raddr[ADDR_LENGTH - 1], calc_rgrey_next} ;
    end
end

/*--------------------------------------------------------------------------------------------
Write address control consists of write address counter and 3 Grey Code Registers:
    - wgrey_minus1 represents previous Grey coded write address
    - wgrey_addr   represents current Grey Coded write address
    - wgrey_next   represents next Grey Coded write address

    - wgrey_next_plus1 represents second next Grey Coded write address

----------------------------------------------------------------------------------------------*/
// grey coded address pipeline for status generation in write clock domain
always@(posedge wclock_in or posedge clear)
begin
    if (clear)
    begin
        wgrey_minus1 <= #`FF_DELAY 1 ;
        wgrey_addr   <= #`FF_DELAY 3 ;
        wgrey_next   <= #`FF_DELAY 2 ;

        wgrey_next_plus1 <= #`FF_DELAY 6;

    end
    else
    if (wallow_out)
    begin
        wgrey_minus1 <= #`FF_DELAY wgrey_addr ;
        wgrey_addr   <= #`FF_DELAY wgrey_next ;

        wgrey_next   <= #`FF_DELAY {waddr[(ADDR_LENGTH - 1)], calc_wgrey_next} ;
//        wgrey_next   <= #`FF_DELAY wgrey_next_plus1 ;
        wgrey_next_plus1 <= #`FF_DELAY {waddr_plus1[(ADDR_LENGTH - 1)], calc_wgrey_next_plus1} ;

    end
end

// write address counter - nothing special except initial value
always@(posedge wclock_in or posedge clear)
begin
    if (clear)
    begin
        // initial value 5

        waddr <= #`FF_DELAY 4 ;
        waddr_plus1 <= #`FF_DELAY 5 ;
    end
    else
    if (wallow_out)
    begin
        waddr <= #`FF_DELAY waddr + 1'b1 ;
        waddr_plus1 <= #`FF_DELAY waddr_plus1 + 1'b1 ;
    end
end

/*------------------------------------------------------------------------------------------------------------------------------
Gray coded address of read address decremented by two is synchronized to write clock domain and compared to:
- previous grey coded write address - if they are equal, the fifo is full

- gray coded write address. If they are equal, fifo is almost full.

- grey coded next write address. If they are equal, the fifo has two free locations left.
--------------------------------------------------------------------------------------------------------------------------------*/
wire [(ADDR_LENGTH - 1):0] wclk_sync_rgrey_minus2 ;
reg  [(ADDR_LENGTH - 1):0] wclk_rgrey_minus2 ;

pci_synchronizer_flop #(ADDR_LENGTH, 0) i_synchronizer_reg_rgrey_minus2
(
    .data_in        (rgrey_minus2),
    .clk_out        (wclock_in),
    .sync_data_out  (wclk_sync_rgrey_minus2),
    .async_reset    (clear)
) ;

always@(posedge wclock_in or posedge clear)
begin
    if (clear)
    begin
        wclk_rgrey_minus2 <= #`FF_DELAY 0 ;
    end
    else
    begin
        wclk_rgrey_minus2 <= #`FF_DELAY wclk_sync_rgrey_minus2 ;
    end
end

assign full_out        = (wgrey_minus1 == wclk_rgrey_minus2) ;
assign almost_full_out = (wgrey_addr   == wclk_rgrey_minus2) ;
assign two_left_out    = (wgrey_next   == wclk_rgrey_minus2) ;

assign three_left_out  = (wgrey_next_plus1 == wclk_rgrey_minus2) ;


/*------------------------------------------------------------------------------------------------------------------------------
Empty control:
Gray coded write address pointer is synchronized to read clock domain and compared to Gray coded read address pointer.
If they are equal, fifo is empty.

Almost empty control:
Synchronized write pointer is also compared to Gray coded next read address. If these two are
equal, fifo is almost empty.
--------------------------------------------------------------------------------------------------------------------------------*/
wire [(ADDR_LENGTH - 1):0] rclk_sync_wgrey_addr ;
reg  [(ADDR_LENGTH - 1):0] rclk_wgrey_addr ;
pci_synchronizer_flop #(ADDR_LENGTH, 3) i_synchronizer_reg_wgrey_addr
(
    .data_in        (wgrey_addr),
    .clk_out        (rclock_in),
    .sync_data_out  (rclk_sync_wgrey_addr),
    .async_reset    (clear)
) ;

always@(posedge rclock_in or posedge clear)
begin
    if (clear)
        rclk_wgrey_addr <= #`FF_DELAY 3 ;
    else
        rclk_wgrey_addr <= #`FF_DELAY rclk_sync_wgrey_addr ;
end

assign almost_empty_out = (rgrey_next == rclk_wgrey_addr) ;
assign empty_out        = (rgrey_addr == rclk_wgrey_addr) ;
endmodule
