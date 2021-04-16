//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pci_master32_sm_if.v"                            ////
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
// $Log: pci_master32_sm_if.v,v $
// Revision 1.7  2004/03/19 16:36:55  mihad
// Single PCI Master write fix.
//
// Revision 1.6  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.5  2003/06/12 10:12:22  mihad
// Changed one critical PCI bus signal logic.
//
// Revision 1.4  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.3  2002/02/01 15:25:12  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:29  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:46  mihad
// New project directory structure
//
//

`include "pci_constants.v"
`include "bus_commands.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

/*====================================================================
Module provides interface between PCI bridge internals and PCI master
state machine
====================================================================*/
module pci_master32_sm_if
(
    clk_in,
    reset_in,

    // interconnect to pci master state machine
    address_out,
    bc_out,
    data_out,
    data_in,
    be_out,
    req_out,
    rdy_out,
    last_out,

    next_data_out,
    next_be_out,
    next_last_out,

    // status inputs from master SM
    wait_in,
    wtransfer_in,
    rtransfer_in,
    retry_in,
    rerror_in,
    first_in ,
    mabort_in,


    // WISHBONE WRITE fifo inputs and outputs
    wbw_renable_out,
    wbw_fifo_addr_data_in,
    wbw_fifo_cbe_in,
    wbw_fifo_control_in,
    wbw_fifo_empty_in,
    wbw_fifo_transaction_ready_in,

    // WISHBONE READ fifo inputs and outputs
    wbr_fifo_wenable_out,
    wbr_fifo_data_out,
    wbr_fifo_be_out,
    wbr_fifo_control_out,

    // delayed transaction control logic inputs and outputs
    del_wdata_in,
    del_complete_out,
    del_req_in,
    del_addr_in,
    del_bc_in,
    del_be_in,
    del_burst_in,
    del_error_out,
    del_rty_exp_out,
    del_we_in,

    // configuration space interconnect
    // error reporting
    err_addr_out,
    err_bc_out,
    err_signal_out,
    err_source_out,
    err_rty_exp_out,

    cache_line_size_in,

    // two signals for pci control and status
    mabort_received_out,
    tabort_received_out,

    posted_write_not_present_out
);

// system inputs
input clk_in ;
input reset_in ;

// PCI master state machine interconnect
output  [31:0]  address_out ;   // address output

output  [3:0]   bc_out ;        // bus command output
reg     [3:0]   bc_out ;

output  [31:0]  data_out ;      // data output for writes
reg     [31:0]  data_out ;

input   [31:0]  data_in ;       // data input for reads
output  [3:0]   be_out  ;       // byte enable output
reg     [3:0]   be_out  ;

output          req_out ;       // request output

output          rdy_out ;       // ready output
reg             rdy_out ;

output          last_out ;      // last data indicator output

output  [31:0]  next_data_out ; // next data output
output  [3:0]   next_be_out ;   // next byte enable output
output          next_last_out ; // next transfer last indicator

input           wait_in,
                wtransfer_in,
                rtransfer_in,
                retry_in,
                rerror_in,
                first_in ,
                mabort_in ;

// WISHBONE write fifo interconnect
output          wbw_renable_out ;          // WBW_FIFO read enable signal

input   [31:0]  wbw_fifo_addr_data_in ;         // WBW_FIFO address/data bus
input   [3:0]   wbw_fifo_cbe_in ;               // WBW_FIFO command/byte enable bus
input   [3:0]   wbw_fifo_control_in ;           // WBW_FIFO control bus
input           wbw_fifo_empty_in ;             // WBW_FIFO's empty status indicator
input           wbw_fifo_transaction_ready_in ; // WBW_FIFO transaction ready indicator

// WISHBONE read FIFO interconnect
output          wbr_fifo_wenable_out ;          // write enable for WBR_FIFO

output  [31:0]  wbr_fifo_data_out ;             // data output to WBR_FIFO

output  [3:0]   wbr_fifo_be_out ;               // byte enable output for WBR_FIFO

output  [3:0]   wbr_fifo_control_out ;          // WBR_FIFO control output

// delayed transaction control logic inputs and outputs
input   [31:0]  del_wdata_in ;                  // delayed write data input
output          del_complete_out ;              // delayed transaction completed output

input           del_req_in ;                    // delayed transaction request
input   [31:0]  del_addr_in ;                   // delayed transaction address
input   [3:0]   del_bc_in ;                     // delayed transaction bus command input
input   [3:0]   del_be_in ;                     // delayed transaction byte enables input
input           del_burst_in ;                  // delayed transaction burst req. indicator
output          del_error_out ;                 // delayed transation error termination signal

output          del_rty_exp_out ;               // retry expired output for delayed transactions

input           del_we_in ;                     // delayed write request indicator

output  [31:0]  err_addr_out ;                  // erroneous address output
output  [3:0]   err_bc_out ;                    // erroneous bus command output

output          err_signal_out ;                // error signalization

output          err_source_out ;                // error source indicator

input   [7:0]   cache_line_size_in ;            // cache line size value input

output          err_rty_exp_out ;               // retry expired error output

output          mabort_received_out ;           // master abort signaled to status register
output          tabort_received_out ;           // target abort signaled to status register

output          posted_write_not_present_out ;  // used in target state machine - must deny read completions when this signal is 0


assign err_bc_out   = bc_out ;

// assign read outputs
/*==================================================================================================================
WISHBONE read FIFO data outputs - just link them to SM data outputs and delayed BE input
==================================================================================================================*/
assign wbr_fifo_data_out = data_in ;
assign wbr_fifo_be_out   = del_be_in ;

// decode if current bus command is configuration command
wire conf_cyc_bc = ( bc_out[3:1] == `BC_CONF_RW ) ;

// register for indicating that current data is also last in transfer
reg current_last ;

// register indicating that last data was transfered OK
reg last_transfered ;
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        last_transfered <= #`FF_DELAY 1'b0 ;
    else
        last_transfered <= #`FF_DELAY ~wait_in && last_out && wtransfer_in ;
end

// status signals output assignement
assign mabort_received_out = mabort_in ;

wire tabort_ff_in = ~wait_in && rerror_in ;

reg    tabort_received_out ;
always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
        tabort_received_out <= #`FF_DELAY 1'b0 ;
    else
        tabort_received_out <= #`FF_DELAY tabort_ff_in ;
end

// error recovery indicator
reg err_recovery ;

// operation is locked until error recovery is in progress or error bit is not cleared in configuration space
wire err_lock = err_recovery ;

// three requests are possible - posted write, delayed write and delayed read
reg del_write_req ;
reg posted_write_req ;
reg del_read_req ;

// assign request output
assign req_out = del_write_req || posted_write_req || del_read_req ;

// posted write is not present, when WB Write Fifo is empty and posted write transaction is not beeing requested at present time
assign posted_write_not_present_out = !posted_write_req && wbw_fifo_empty_in ;

// write requests are staged, so data is read from source into current data register and next data register
reg write_req_int ;
always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
        write_req_int <= #`FF_DELAY 1'b0 ;
    else
        write_req_int <= #`FF_DELAY posted_write_req || del_write_req ;

end

// ready output is generated one clock after request for reads and two after for writes
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        rdy_out <= #`FF_DELAY 1'b0 ;
    else
        rdy_out <= #`FF_DELAY del_read_req || ( (posted_write_req || del_write_req) && write_req_int) ;
end

// wires with logic used as inputs to request FFs
wire do_posted_write = ( wbw_fifo_transaction_ready_in && ~wbw_fifo_empty_in && ~err_lock ) ;
wire do_del          = ( del_req_in && ~err_lock && wbw_fifo_empty_in ) ;
wire do_del_write    = do_del &&  del_we_in ;
wire do_del_read     = do_del && ~del_we_in ;

// register for indicating current operation's data source
parameter DELAYED_WRITE = 1'b1 ;
parameter POSTED_WRITE  = 1'b0 ;

// new data source - depending on which transaction will be processed next - delayed read is here because source of byte enables must
// be specified for delayed reads also - data source is not relevant for delayed reads, so value is don't care anyway
wire new_data_source = (do_del_write || do_del_read) ? DELAYED_WRITE : POSTED_WRITE ; // input to data source register
wire data_source_change = ~req_out ;    // change (enable) for data source register - when no requests are in progress

reg data_source ;           // data source value
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        // default value is posted write source - wbw_fifo
        data_source <= #`FF_DELAY POSTED_WRITE ;
    else
    if (data_source_change)
        // change data source on rising clock edge
        data_source <= #`FF_DELAY new_data_source ;
end

// multiplexer for data output to PCI MASTER state machine
reg [31:0] source_data ;
reg [3:0]  source_be ;
always@(data_source or wbw_fifo_addr_data_in or wbw_fifo_cbe_in or del_wdata_in or del_be_in or del_burst_in)
begin
    case (data_source)
        POSTED_WRITE:   begin
                            source_data = wbw_fifo_addr_data_in ;
                            source_be   = wbw_fifo_cbe_in ;
                        end
        DELAYED_WRITE:  begin
                            source_data = del_wdata_in ;
                            // read all bytes during delayed burst read!
                            source_be   = ~( del_be_in | {4{del_burst_in}} ) ;
                        end
    endcase
end

wire            waddr =  wbw_fifo_control_in[`ADDR_CTRL_BIT] ;

// address change indicator - address is allowed to be loaded only when no transaction is in progress!
wire            address_change = ~req_out ; // address change - whenever there is no request in progress

// new address - input to register storing address of current request - if posted write request will be next,
// load address and bus command from wbw_fifo, else load data from delayed transaction logic
wire     [31:0] new_address = ( ~req_out && do_posted_write ) ? wbw_fifo_addr_data_in[31:0] : del_addr_in[31:0] ;
wire     [3:0]  new_bc      = ( ~req_out && do_posted_write ) ? wbw_fifo_cbe_in : del_bc_in ;

// address counter enable - only for posted writes when data is actually transfered
wire addr_count_en = !wait_in && posted_write_req && rtransfer_in ;

always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        bc_out <= #`FF_DELAY `BC_RESERVED0 ;
    else
    if (address_change)
        bc_out <= #`FF_DELAY new_bc ;
end

reg [29:0] current_dword_address ;

// DWORD address counter with load
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        current_dword_address <= #`FF_DELAY 30'h0000_0000 ;
    else
    if (address_change)
        current_dword_address <= #`FF_DELAY new_address[31:2] ;
    else
    if (addr_count_en)
        current_dword_address <= #`FF_DELAY current_dword_address + 1'b1 ;
end

reg [1:0] current_byte_address ;
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        current_byte_address <= #`FF_DELAY 2'b00 ;
    else
    if (address_change)
        current_byte_address <= #`FF_DELAY new_address[1:0] ;
end

// byte address generation logic
reg [ 1: 0] generated_byte_adr ;
reg [ 1: 0] pci_byte_adr       ;

always@(be_out)
begin
    casex(be_out)
    4'bxxx0:generated_byte_adr = 2'b00 ;
    4'bxx01:generated_byte_adr = 2'b01 ;
    4'bx011:generated_byte_adr = 2'b10 ;
    4'b0111:generated_byte_adr = 2'b11 ;
    4'b1111:generated_byte_adr = 2'b00 ;
    endcase
end

always@(generated_byte_adr or bc_out or current_byte_address)
begin
    // for memory access commands, set lower 2 address bits to 0
    if ((bc_out == `BC_MEM_READ) | (bc_out == `BC_MEM_WRITE) | 
        (bc_out == `BC_MEM_READ_MUL) | (bc_out == `BC_MEM_READ_LN) | 
        (bc_out == `BC_MEM_WRITE_INVAL))
    begin
        pci_byte_adr = 2'b00 ;
    end
    else if ((bc_out == `BC_IO_WRITE) | (bc_out == `BC_IO_READ))
    begin
        pci_byte_adr = generated_byte_adr ;
    end
    else
    begin
        pci_byte_adr = current_byte_address ;
    end
end

// address output to PCI master state machine assignment
assign address_out  = { current_dword_address, pci_byte_adr } ;

// the same for erroneous address assignement
assign err_addr_out = { current_dword_address, pci_byte_adr } ;

// cacheline size counter - for read transaction length control
// cache line count is enabled during burst reads when data is actually transfered
wire read_count_enable = ~wait_in && del_read_req && del_burst_in && wtransfer_in  ;

// cache line counter is loaded when del read request is not in progress
wire read_count_load   = ~del_read_req ;

reg [(`WBR_ADDR_LENGTH - 1):0] max_read_count ;
always@(cache_line_size_in or del_bc_in)
begin
    if ( (cache_line_size_in >= `WBR_DEPTH) || (~del_bc_in[1] && ~del_bc_in[0]) )
        max_read_count = `WBR_DEPTH - 1'b1;
    else
        max_read_count = cache_line_size_in ;
end

reg [(`WBR_ADDR_LENGTH - 1):0] read_count ;

// cache line bound indicator - it signals when data for one complete cacheline was read
wire read_bound_comb = ~|(read_count[(`WBR_ADDR_LENGTH - 1):2]) ;
reg  read_bound ;
always@(posedge clk_in or posedge reset_in)
begin
    if ( reset_in )
        read_bound <= #`FF_DELAY 1'b0 ;
    else if (read_count_load)
        read_bound <= #`FF_DELAY 1'b0 ;
    else if ( read_count_enable )
        read_bound <= #`FF_DELAY read_bound_comb ;
end

wire read_count_change_val = read_count_load | read_count_enable ;

wire [(`WBR_ADDR_LENGTH - 1):0] read_count_next = read_count_load ? max_read_count : (read_count - 1'b1) ;

// down counter with load
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        read_count <= #`FF_DELAY 0 ;
    else
/*    if (read_count_load)
        read_count <= #`FF_DELAY max_read_count ;
    else
    if (read_count_enable)
        read_count <= #`FF_DELAY read_count - 1'b1 ;
*/  if (read_count_change_val)
        read_count <= #`FF_DELAY read_count_next ;
end

// flip flop indicating error recovery is in progress
reg err_recovery_in ;
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        err_recovery <= #`FF_DELAY 1'b0 ;
    else
        err_recovery <= #`FF_DELAY err_recovery_in ;
end

/*// retry counter implementation
reg [7:0] retry_count ;

wire retry_expired = ~|(retry_count[7:1]) ;

// loading of retry counter - whenever no request is present or other termination than retry or wait is signalled
wire retry_load = ~req_out || (~wait_in && rtransfer_in) ;

// retry DOWN counter with load
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        retry_count <= #`FF_DELAY 8'hFF ;
    else
    if ( retry_load )
        retry_count <= #`FF_DELAY `PCI_RTY_CNT_MAX ;
    else
    if (retry_in)
        retry_count <= #`FF_DELAY retry_count - 1'b1 ;
end*/

/*==================================================================================================================
Delayed write requests are always single transfers!
Delayed write request starts, when no request is currently beeing processed and it is signaled from other side
of the bridge.
==================================================================================================================*/
// delayed write request FF input control
reg del_write_req_input ;

always@(
    do_del_write or
    del_write_req or
    posted_write_req or
    del_read_req or
    wait_in or
    //retry_in or
    //retry_expired or
    rtransfer_in or
    rerror_in or
    mabort_in
)
begin
    if (~del_write_req)
    begin
        // delayed write is not in progress and is requested
        // delayed write can be requested when no other request is in progress
        del_write_req_input = ~posted_write_req && ~del_read_req && do_del_write ;
    end
    else
    begin
        // delayed write request is in progress - assign input
        del_write_req_input = wait_in ||
                              ( /*~( retry_in && retry_expired) &&*/
                                ~rtransfer_in && ~rerror_in && ~mabort_in
                              );
    end
end

// delayed write request FLIP-FLOP
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        del_write_req <= #`FF_DELAY 1'b0 ;
    else
        del_write_req <= #`FF_DELAY del_write_req_input ;
end

/*================================================================================================
Posted write request indicator.
Posted write starts whenever no request is in progress and one whole posted write is
stored in WBW_FIFO. It ends on error terminations ( master, target abort, retry expired) or
data transfer terminations if last data is on top of FIFO.
Continues on wait, retry, and disconnect without data.
================================================================================================*/
// posted write request FF input control
reg posted_write_req_input ;
always@(
    do_posted_write or
    del_write_req or
    posted_write_req or
    del_read_req or
    wait_in or
    //retry_in or
    rerror_in or
    mabort_in or
    //retry_expired or
    rtransfer_in or
    last_transfered
)
begin
    if (~posted_write_req)
    begin
        // posted write is not in progress
        posted_write_req_input = ~del_write_req && ~del_read_req && do_posted_write ;
    end
    else
    begin
        posted_write_req_input = wait_in ||
                                 (/*~(retry_in && retry_expired && ~rtransfer_in) &&*/
                                  ~rerror_in && ~mabort_in &&
                                  ~(last_transfered)
                                 ) ;

    end
end

// posted write request flip flop
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        posted_write_req <= #`FF_DELAY 1'b0 ;
    else
        posted_write_req <= #`FF_DELAY posted_write_req_input ;

end

/*================================================================================================
Delayed read request indicator.
Delayed read starts whenever no request is in progress and delayed read request is signaled from
other side of bridge. It ends on error terminations ( master, target abort, retry expired) or
data transfer terminations if it is not burst transfer or on cache line bounds on burst transfer.
It also ends on disconnects.
Continues on wait and retry.
================================================================================================*/
// delayed read FF input control
reg del_read_req_input ;
always@(
    do_del_read or
    del_write_req or
    posted_write_req or
    del_read_req or
    last_transfered or
    wait_in or
    retry_in or
    //retry_expired or
    mabort_in or
    rtransfer_in or
    rerror_in or
    first_in or
    del_complete_out
)
begin
    if (~del_read_req)
    begin
        del_read_req_input = ~del_write_req && ~posted_write_req && ~del_complete_out && do_del_read ;
    end
    else
    begin
        del_read_req_input = wait_in ||
                             ( ~(retry_in && (~first_in /*|| retry_expired */)) &&
                               ~mabort_in && ~rerror_in &&
                               ~(last_transfered)
                             ) ;
    end
end

// delayed read request FF
always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        del_read_req <= #`FF_DELAY 1'b0 ;
    else
        del_read_req <= #`FF_DELAY del_read_req_input ;
end

// wire indicating last entry of transaction on top of fifo
wire wlast = wbw_fifo_control_in[`LAST_CTRL_BIT] ;

wire last_int = posted_write_req && wlast || del_write_req ;

// intermidiate data, byte enable and last registers
reg [31:0] intermediate_data ;
reg  [3:0] intermediate_be ;
reg        intermediate_last ;

wire intermediate_enable = ( posted_write_req || del_write_req ) && ( ~write_req_int || (( ~rdy_out || ~wait_in && rtransfer_in ) && ~intermediate_last)) ;

always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
    begin
        intermediate_data <= #`FF_DELAY 32'h0000_0000 ;
        intermediate_be   <= #`FF_DELAY 4'h0 ;
        intermediate_last <= #`FF_DELAY 1'b0 ;
    end
    else
    if ( intermediate_enable )
    begin
        intermediate_data <= #`FF_DELAY source_data ;
        intermediate_be   <= #`FF_DELAY source_be ;
        intermediate_last <= #`FF_DELAY last_int ;
    end
end

// multiplexer for next data
reg [31:0] next_data_out ;
reg [3:0] next_be_out   ;
reg write_next_last ;
reg [3:0] write_next_be ;

always@
(
    rtransfer_in            or 
    intermediate_data       or 
    intermediate_be         or 
    intermediate_last       or 
    wbw_fifo_addr_data_in   or 
    wbw_fifo_cbe_in         or 
    wlast                   or
    wait_in
)
begin
    if( rtransfer_in & ~wait_in )
    begin
        next_data_out   = wbw_fifo_addr_data_in ;
        write_next_last = wlast ;
        write_next_be   = wbw_fifo_cbe_in ;
    end
    else
    begin
        next_data_out   = intermediate_data ;
        write_next_last = intermediate_last ;
        write_next_be   = intermediate_be ;
    end
end

always@(del_read_req or source_be or write_next_be)
begin
    if (del_read_req)
        next_be_out = source_be ;
    else
        next_be_out = write_next_be ;
end
/*================================================================================================
WBW_FIFO read enable - read from WBW_FIFO is performed on posted writes, when data transfer
termination is received - transfer or disconnect with data. Reads are enabled during error
recovery also, since erroneous transaction must be pulled out of FIFO!
================================================================================================*/
// wbw_fifo read enable input control

assign wbw_renable_out = ~req_out && (do_posted_write || err_recovery) ||
                              posted_write_req && ( ~write_req_int || (~rdy_out && ~intermediate_last) || (~wait_in && rtransfer_in && ~intermediate_last)) ;

/*================================================================================================
WBR_FIFO write enable control -
writes to FIFO are possible only when delayed read request is in progress and data transfer
or error termination is signalled. It is not enabled on retry or disconnect without data.
================================================================================================*/
// wbr_fifo write enable control - enabled when transfer is in progress and data is transfered or error is signalled
assign wbr_fifo_wenable_out = del_read_req && ~wait_in && ( rtransfer_in || mabort_in || rerror_in ) ;

/*================================================================================================
WBR_FIFO control output for identifying data entries.
This is necesary because of prefetched reads, which partially succeed. On error, error entry
gets in to signal it on WISHBONE bus if WISHBONE master reads up to this entry.
================================================================================================*/
assign wbr_fifo_control_out[`ADDR_CTRL_BIT]       = 1'b0 ;
assign wbr_fifo_control_out[`LAST_CTRL_BIT]       = last_transfered ;
assign wbr_fifo_control_out[`DATA_ERROR_CTRL_BIT] = rerror_in || (mabort_in && ~conf_cyc_bc) ;
assign wbr_fifo_control_out[`UNUSED_CTRL_BIT]     = 1'b0 ;

// retry expired error for posted writes control
//assign err_rty_exp_out = posted_write_req && ~wait_in && retry_in && retry_expired && ~rtransfer_in;
assign err_rty_exp_out = 1'b0 ;

// error source and error signal output control logic - only for posted writes
assign err_source_out = mabort_in /*|| err_rty_exp_out*/ ;

assign err_signal_out = /*err_rty_exp_out || */ posted_write_req && ~wait_in && (mabort_in || rerror_in) ;

//assign del_rty_exp_out = (~wait_in && (del_read_req || del_write_req)) && (retry_in && retry_expired && ~rtransfer_in) ;
assign del_rty_exp_out = 1'b0 ;

assign del_error_out   = ~wait_in && (del_write_req || del_read_req) && ( (mabort_in && ~conf_cyc_bc) || rerror_in ) ;

wire   del_write_complete = del_write_req && ~wait_in && ( rtransfer_in || rerror_in || mabort_in ) ;
wire   del_read_complete  = del_read_req  && ~wait_in && ( rerror_in || mabort_in || last_transfered || ( retry_in && ~first_in ) ) ;

assign del_complete_out = ~wait_in && ( del_write_complete || del_read_complete ) ;

// next last output generation
assign next_last_out = del_write_req || del_read_req && ( ~del_burst_in || read_bound ) || posted_write_req && ( write_next_last ) ;
/*==================================================================================================================
Error recovery FF gets a value of one, when during posted write error occurs. It is cleared when all the data provided
for erroneous transaction is pulled out of WBW_FIFO
==================================================================================================================*/

// error recovery flip flop input - used when posted write is terminated with an error
always@(
    err_recovery or
    last_out or
    wlast or
    err_signal_out or
    intermediate_last
)
begin
    // when error recovery is not set - drive its input so it gets set
    if ( ~err_recovery )
        err_recovery_in = ~last_out && ~intermediate_last && err_signal_out ;
    else
        // when error recovery is set, wbw_fifo is enabled - clear err_recovery when last data entry of erroneous transaction is pulled out of fifo
        err_recovery_in = ~wlast ;
end

wire data_out_load = (posted_write_req || del_write_req) && ( !rdy_out || ( !wait_in && rtransfer_in ) ) ;

wire be_out_load  = (req_out && !rdy_out) || ( posted_write_req && !wait_in && rtransfer_in ) ;

wire last_load  = req_out && ( ~rdy_out || ~wait_in && wtransfer_in ) ;

always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        data_out <= #`FF_DELAY 32'h0000_0000 ;
    else
    if ( data_out_load )
        data_out <= #`FF_DELAY intermediate_data ;
end

always@(posedge clk_in or posedge reset_in)
begin
    if ( reset_in )
        be_out <= #`FF_DELAY 4'hF ;
    else
    if ( be_out_load )
        be_out <= #`FF_DELAY posted_write_req ? intermediate_be : source_be ;
end

always@(posedge reset_in or posedge clk_in)
begin
    if (reset_in)
        current_last <= #`FF_DELAY 1'b0 ;
    else
    if ( last_load )
        current_last <= #`FF_DELAY next_last_out ;
end

assign last_out = current_last ;
endmodule
