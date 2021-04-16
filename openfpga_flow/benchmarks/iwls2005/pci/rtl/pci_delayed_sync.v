//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "delayed_sync.v"                                  ////
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
// $Log: pci_delayed_sync.v,v $
// Revision 1.3  2003/08/14 13:06:02  simons
// synchronizer_flop replaced with pci_synchronizer_flop, artisan ram instance updated.
//
// Revision 1.2  2003/03/26 13:16:18  mihad
// Added the reset value parameter to the synchronizer flop module.
// Added resets to all synchronizer flop instances.
// Repaired initial sync value in fifos.
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.5  2002/09/25 09:54:50  mihad
// Added completion expiration test for WB Slave unit. Changed expiration signalling
//
// Revision 1.4  2002/03/05 11:53:47  mihad
// Added some testcases, removed un-needed fifo signals
//
// Revision 1.3  2002/02/01 15:25:12  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:28  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:46  mihad
// New project directory structure
//
//

// module provides synchronization mechanism between requesting and completing side of the bridge
`include "pci_constants.v"
`include "bus_commands.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_delayed_sync
(
    reset_in,
    req_clk_in,
    comp_clk_in,
    req_in,
    comp_in,
    done_in,
    in_progress_in,
    comp_req_pending_out,
    req_req_pending_out,
    req_comp_pending_out,
    comp_comp_pending_out,
    addr_in,
    be_in,
    addr_out,
    be_out,
    we_in,
    we_out,
    bc_in,
    bc_out,
    status_in,
    status_out,
    comp_flush_out,
    burst_in,
    burst_out,
    retry_expired_in
);

// system inputs
input reset_in,         // reset input
      req_clk_in,       // requesting clock input
      comp_clk_in ;     // completing clock input

// request, completion, done and in progress indication inputs
input req_in,           // request qualifier - when 1 it indicates that valid request data is provided on inputs
      comp_in,          // completion qualifier - when 1, completing side indicates that request has completed
      done_in,          // done input - when 1 indicates that requesting side of the bridge has completed a transaction on requesting bus
      in_progress_in ;  // in progress indicator - indicates that current completion is in progress on requesting side of the bridge

// pending indication outputs
output  comp_req_pending_out,   // completion side request output - resynchronized from requesting clock to completing clock
        req_req_pending_out,    // request pending output for requesting side
        req_comp_pending_out,   // completion pending output for requesting side of the bridge - it indicates when completion is ready for completing on requesting bus
        comp_comp_pending_out ; // completion pending output for completing side of the bridge

// additional signals and wires for clock domain passage of signals
reg     comp_req_pending,
        req_req_pending,
        req_comp_pending,
        req_comp_pending_sample,
        comp_comp_pending,
        req_done_reg,
        comp_done_reg_main,
        comp_done_reg_clr,
        req_rty_exp_reg,
        req_rty_exp_clr,
        comp_rty_exp_reg,
        comp_rty_exp_clr ;

wire    sync_comp_req_pending,
        sync_req_comp_pending,
        sync_comp_done,
        sync_req_rty_exp,
        sync_comp_rty_exp_clr ;

// inputs from requesting side - only this side can set address, bus command, byte enables, write enable and burst - outputs are common for both sides
// all signals that identify requests are stored in this module

input [31:0]    addr_in ;   // address bus input
input [3:0]     be_in ;     // byte enable input
input           we_in ;     // write enable input - read/write request indication 1 = write request / 0 = read request
input [3:0]     bc_in ;     // bus command input
input           burst_in ;  // burst indicator    - qualifies operation as burst/single transfer 1 = burst / 0 = single transfer

// common request outputs used both by completing and requesting sides
// this outputs are not resynchronized, since flags determine the request status
output [31:0]   addr_out ;
output [3:0]    be_out ;
output          we_out ;
output [3:0]    bc_out ;
output          burst_out ;

// completion side signals encoded termination status - 0 = normal completion / 1 = error terminated completion
input          status_in ;
output         status_out ;

// input signals that delayed transaction has been retried for max number of times
// on this signal request is ditched, otherwise it would cause a deadlock
// requestor can issue another request and procedure will be repeated
input   retry_expired_in ;

// completion flush output - if in 2^^16 clock cycles transaction is not repeated by requesting agent - flush completion data
output  comp_flush_out ;

// output registers for common signals
reg [31:0]   addr_out ;
reg [3:0]    be_out ;
reg          we_out ;
reg [3:0]    bc_out ;
reg          burst_out ;

// delayed transaction information is stored only when request is issued and request nor completion are pending
wire new_request = req_in && ~req_comp_pending_out && ~req_req_pending_out ;
always@(posedge req_clk_in or posedge reset_in)
begin
    if (reset_in)
    begin
        addr_out  <= #`FF_DELAY 32'h0000_0000 ;
        be_out    <= #`FF_DELAY 4'h0 ;
        we_out    <= #`FF_DELAY 1'b0 ;
        bc_out    <= #`FF_DELAY `BC_RESERVED0 ;
        burst_out <= #`FF_DELAY 1'b0 ;
    end
    else
        if (new_request)
        begin
            addr_out  <= #`FF_DELAY addr_in ;
            be_out    <= #`FF_DELAY be_in ;
            we_out    <= #`FF_DELAY we_in ;
            bc_out    <= #`FF_DELAY bc_in ;
            burst_out <= #`FF_DELAY burst_in ;
        end
end

// completion pending cycle counter
reg [16:0] comp_cycle_count ;

/*=================================================================================================================================
Passing of requests between clock domains:
request originates on requesting side. It's then synchronized with two flip-flops to cross to completing clock domain
=================================================================================================================================*/
// main request flip-flop triggered on requesting side's clock
// request is cleared whenever completion or retry expired is signalled from opposite side of the bridge
wire req_req_clear = req_comp_pending || (req_rty_exp_reg && ~req_rty_exp_clr) ;
always@(posedge req_clk_in or posedge reset_in)
begin
    if ( reset_in )
        req_req_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( req_req_clear )
        req_req_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( req_in )
        req_req_pending <= #`FF_DELAY 1'b1 ;
end

// interemediate stage request synchronization flip - flop - this one is prone to metastability
// and should have setup and hold times disabled during simulation
pci_synchronizer_flop #(1, 0) req_sync
(
    .data_in        (req_req_pending),
    .clk_out        (comp_clk_in),
    .sync_data_out  (sync_comp_req_pending),
    .async_reset    (reset_in)
) ;

// wire for clearing completion side request flag - whenever completion or retry expired are signalled
wire comp_req_pending_clear = comp_req_pending && ( comp_in || retry_expired_in) ;

// wire for enabling request flip - flop - it is enabled when completion is not active and done is not active
wire comp_req_pending_ena   = ~comp_comp_pending && ~comp_done_reg_main && ~comp_rty_exp_reg ;

// completion side request flip flop - gets a value from intermediate stage sync flip flop
always@(posedge comp_clk_in or posedge reset_in)
begin
    if ( reset_in )
        comp_req_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( comp_req_pending_clear )
        comp_req_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( comp_req_pending_ena )
        comp_req_pending <= #`FF_DELAY sync_comp_req_pending ;
end

// completion side request output assignment - when request ff is set and completion ff is not set
assign comp_req_pending_out = comp_req_pending ;

// requesting side request pending output
assign req_req_pending_out  = req_req_pending ;
/*=================================================================================================================================
Passing of completions between clock domains:
completion originates on completing side. It's then synchronized with two flip-flops to cross to requesting clock domain
=================================================================================================================================*/
// main completion Flip - Flop - triggered by completing side's clock
// completion side completion pending flag is cleared when done flag propagates through clock domains
wire comp_comp_clear = comp_done_reg_main && ~comp_done_reg_clr ;
always@(posedge comp_clk_in or posedge reset_in)
begin
    if ( reset_in )
        comp_comp_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( comp_comp_clear )
        comp_comp_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( comp_in && comp_req_pending )
        comp_comp_pending <= #`FF_DELAY 1'b1 ;
end

assign comp_comp_pending_out = comp_comp_pending ;

// interemediate stage completion synchronization flip - flop - this one is prone to metastability
pci_synchronizer_flop #(1, 0) comp_sync
(
    .data_in        (comp_comp_pending),
    .clk_out        (req_clk_in),
    .sync_data_out  (sync_req_comp_pending),
    .async_reset    (reset_in)
) ;

// request side completion pending flip flop is cleared whenever done is signalled or completion counter expires - 2^^16 clock cycles
wire req_comp_pending_clear = done_in || comp_cycle_count[16];

// request side completion pending flip flop is disabled while done flag is set
wire req_comp_pending_ena   = ~req_done_reg ;

// request side completion flip flop - gets a value from intermediate stage sync flip flop
always@(posedge req_clk_in or posedge reset_in)
begin
    if ( reset_in )
        req_comp_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( req_comp_pending_clear )
        req_comp_pending <= #`FF_DELAY 1'b0 ;
    else
    if ( req_comp_pending_ena )
        req_comp_pending <= #`FF_DELAY sync_req_comp_pending ;
end

// sampling FF - used for sampling incoming completion flag from completing side
always@(posedge req_clk_in or posedge reset_in)
begin
    if ( reset_in )
        req_comp_pending_sample <= #`FF_DELAY 1'b0 ;
    else
        req_comp_pending_sample <= #`FF_DELAY sync_req_comp_pending ;
end

// requesting side completion pending output assignment
assign req_comp_pending_out = req_comp_pending && ~req_req_pending ;

/*==================================================================================================================================
Passing of delayed transaction done signal between clock domains.
Done is signalled by requesting side of the bridge and is passed to completing side of the bridge
==================================================================================================================================*/
// main done flip-flop triggered on requesting side's clock
// when completing side removes completion flag, done flag is also removed, so requests can proceede
wire req_done_clear = ~req_comp_pending_sample ;
always@(posedge req_clk_in or posedge reset_in)
begin
    if ( reset_in )
        req_done_reg <= #`FF_DELAY 1'b0 ;
    else
    if ( req_done_clear )
        req_done_reg <= #`FF_DELAY 1'b0 ;
    else
    if ( done_in || comp_cycle_count[16] )
        req_done_reg <= #`FF_DELAY 1'b1 ;
end

pci_synchronizer_flop  #(1, 0) done_sync
(
    .data_in        (req_done_reg),
    .clk_out        (comp_clk_in),
    .sync_data_out  (sync_comp_done),
    .async_reset    (reset_in)
) ;

always@(posedge comp_clk_in or posedge reset_in)
begin
    if ( reset_in )
        comp_done_reg_main <= #`FF_DELAY 1'b0 ;
    else
        comp_done_reg_main <= #`FF_DELAY sync_comp_done ;
end

always@(posedge comp_clk_in or posedge reset_in)
begin
    if ( reset_in )
        comp_done_reg_clr <= #`FF_DELAY 1'b0 ;
    else
        comp_done_reg_clr <= #`FF_DELAY comp_done_reg_main ;
end

/*=================================================================================================================================
Passing of retry expired signal between clock domains
Retry expiration originates on completing side. It's then synchronized with two flip-flops to cross to requesting clock domain
=================================================================================================================================*/
// main retry expired Flip - Flop - triggered by completing side's clock
wire comp_rty_exp_clear = comp_rty_exp_clr && comp_rty_exp_reg ;

// retry expired is a special case of transaction removal - retry expired propagates from completing
// clock domain to requesting clock domain to remove all pending requests and than propagates back
// to completing side to qualify valid new requests

always@(posedge comp_clk_in or posedge reset_in)
begin
    if ( reset_in )
        comp_rty_exp_reg <= #`FF_DELAY 1'b0 ;
    else
    if ( comp_rty_exp_clear )
        comp_rty_exp_reg <= #`FF_DELAY 1'b0 ;
    else
    if ( retry_expired_in && comp_req_pending)
        comp_rty_exp_reg <= #`FF_DELAY 1'b1 ;
end

// interemediate stage retry expired synchronization flip - flop - this one is prone to metastability
pci_synchronizer_flop #(1, 0) rty_exp_sync
(
    .data_in        (comp_rty_exp_reg),
    .clk_out        (req_clk_in),
    .sync_data_out  (sync_req_rty_exp),
    .async_reset    (reset_in)
) ;

// request retry expired flip flop - gets a value from intermediate stage sync flip flop
always@(posedge req_clk_in or posedge reset_in)
begin
    if ( reset_in )
        req_rty_exp_reg <= #`FF_DELAY 1'b0 ;
    else
        req_rty_exp_reg <= #`FF_DELAY sync_req_rty_exp ;
end

always@(posedge req_clk_in or posedge reset_in)
begin
    if ( reset_in )
        req_rty_exp_clr <= #`FF_DELAY 1'b0 ;
    else
        req_rty_exp_clr <= #`FF_DELAY req_rty_exp_reg ;
end

pci_synchronizer_flop #(1, 0) rty_exp_back_prop_sync
(
    .data_in        (req_rty_exp_reg && req_rty_exp_clr),
    .clk_out        (comp_clk_in),
    .sync_data_out  (sync_comp_rty_exp_clr),
    .async_reset    (reset_in)
) ;

always@(posedge comp_clk_in or posedge reset_in)
begin
    if ( reset_in )
        comp_rty_exp_clr <= #`FF_DELAY 1'b0 ;
    else
        comp_rty_exp_clr <= #`FF_DELAY sync_comp_rty_exp_clr ;
end

// completion status flip flop - if 0 when completion is signalled it's finished OK otherwise it means error
reg status_out ;
always@(posedge comp_clk_in or posedge reset_in)
begin
    if (reset_in)
        status_out <= #`FF_DELAY 1'b0 ;
    else
    if (comp_in && comp_req_pending)
        status_out <= #`FF_DELAY status_in ;
end

// clocks counter - it counts how many clock cycles completion is present without beeing repeated
// if it counts to 2^^16 cycles the completion must be ditched

// wire for clearing this counter
wire clear_count = in_progress_in || ~req_comp_pending_out || comp_cycle_count[16] ;
always@(posedge req_clk_in or posedge reset_in)
begin
    if (reset_in)
        comp_cycle_count <= #`FF_DELAY 17'h0_0000 ;
    else
    if (clear_count)
        comp_cycle_count <= #`FF_DELAY 17'h0_0000 ;
    else
        comp_cycle_count <= #`FF_DELAY comp_cycle_count + 1'b1 ;
end

// completion flush output - used for flushing fifos when counter expires
// if counter doesn't expire, fifo flush is up to WISHBONE slave or PCI target state machines
reg comp_flush_out ;
always@(posedge req_clk_in or posedge reset_in)
begin
    if (reset_in)
        comp_flush_out <= #`FF_DELAY 1'b0 ;
    else
        comp_flush_out <= #`FF_DELAY comp_cycle_count[16] ;
end

endmodule //delayed_sync
