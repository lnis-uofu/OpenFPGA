//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "wbw_wbr_fifos.v"                                 ////
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
// $Log: pci_wbw_wbr_fifos.v,v $
// Revision 1.6  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.5  2003/10/17 09:11:52  markom
// mbist signals updated according to newest convention
//
// Revision 1.4  2003/08/14 13:06:03  simons
// synchronizer_flop replaced with pci_synchronizer_flop, artisan ram instance updated.
//
// Revision 1.3  2003/03/26 13:16:18  mihad
// Added the reset value parameter to the synchronizer flop module.
// Added resets to all synchronizer flop instances.
// Repaired initial sync value in fifos.
//
// Revision 1.2  2003/01/30 22:01:09  mihad
// Updated synchronization in top level fifo modules.
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.9  2002/10/18 03:36:37  tadejm
// Changed wrong signal name mbist_sen into mbist_ctrl_i.
//
// Revision 1.8  2002/10/17 22:49:22  tadejm
// Changed BIST signals for RAMs.
//
// Revision 1.7  2002/10/11 10:09:01  mihad
// Added additional testcase and changed rst name in BIST to trst
//
// Revision 1.6  2002/10/08 17:17:06  mihad
// Added BIST signals for RAMs.
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
// Revision 1.2  2001/10/05 08:20:12  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:47  mihad
// New project directory structure
//
//

`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_wbw_wbr_fifos
(
    wb_clock_in,
    pci_clock_in,
    reset_in,
    wbw_wenable_in,
    wbw_addr_data_in,
    wbw_cbe_in,
    wbw_control_in,
    wbw_renable_in,
    wbw_addr_data_out,
    wbw_cbe_out,
    wbw_control_out,
//    wbw_flush_in,         write fifo flush not used
    wbw_almost_full_out,
    wbw_full_out,
    wbw_empty_out,
    wbw_transaction_ready_out,
    wbr_wenable_in,
    wbr_data_in,
    wbr_be_in,
    wbr_control_in,
    wbr_renable_in,
    wbr_data_out,
    wbr_be_out,
    wbr_control_out,
    wbr_flush_in,
    wbr_empty_out

`ifdef PCI_BIST
    ,
    // debug chain signals
    mbist_si_i,       // bist scan serial in
    mbist_so_o,       // bist scan serial out
    mbist_ctrl_i        // bist chain shift control
`endif                        
) ;

/*-----------------------------------------------------------------------------------------------------------
System inputs:
wb_clock_in - WISHBONE bus clock
pci_clock_in - PCI bus clock
reset_in - reset from control logic
-------------------------------------------------------------------------------------------------------------*/
input wb_clock_in, pci_clock_in, reset_in ;

/*-----------------------------------------------------------------------------------------------------------
WISHBONE WRITE FIFO interface signals prefixed with wbw_ - FIFO is used for posted writes initiated by
WISHBONE master, traveling through FIFO and are completed on PCI by PCI master interface

write enable signal:
wbw_wenable_in = write enable input for WBW_FIFO - driven by WISHBONE slave interface

data input signals:
wbw_addr_data_in = data input - data from WISHBONE bus - first entry of transaction is address others are data entries
wbw_cbe_in       = bus command/byte enable(~SEL[3:0]) input - first entry of transaction is bus command, other are byte enables
wbw_control_in   = control input - encoded control bus input

read enable signal:
wbw_renable_in = read enable input driven by PCI master interface

data output signals:
wbw_addr_data_out = data output - data from WISHBONE bus - first entry of transaction is address, others are data entries
wbw_cbe_out      = bus command/byte enable output - first entry of transaction is bus command, others are byte enables
wbw_control_out = control input - encoded control bus input

status signals - monitored by various resources in the core
wbw_flush_in = flush signal input for WBW_FIFO - when asserted, fifo is flushed(emptied)
wbw_almost_full_out = almost full output from WBW_FIFO
wbw_full_out = full output from WBW_FIFO
wbw_empty_out = empty output from WBW_FIFO
wbw_transaction_ready_out = output indicating that one complete transaction is waiting in WBW_FIFO
-----------------------------------------------------------------------------------------------------------*/
// input control and data
input        wbw_wenable_in ;
input [31:0] wbw_addr_data_in ;
input [3:0]  wbw_cbe_in ;
input [3:0]  wbw_control_in ;

// output control and data
input         wbw_renable_in ;
output [31:0] wbw_addr_data_out ;
output [3:0]  wbw_cbe_out ;
output [3:0]  wbw_control_out ;

// flush input
// input wbw_flush_in ; // not used

// status outputs
output wbw_almost_full_out ;
output wbw_full_out ;
output wbw_empty_out ;
output wbw_transaction_ready_out ;

/*-----------------------------------------------------------------------------------------------------------
WISHBONE READ FIFO interface signals prefixed with wbr_ - FIFO is used for holding delayed read completions
initiated by master on WISHBONE bus and completed on PCI bus,

write enable signal:
wbr_wenable_in = write enable input for WBR_FIFO - driven by PCI master interface

data input signals:
wbr_data_in      = data input - data from PCI bus - there is no address entry here, since address is stored in separate register
wbr_be_in        = byte enable(~BE#[3:0]) input - byte enables - same through one transaction
wbr_control_in   = control input - encoded control bus input

read enable signal:
wbr_renable_in = read enable input driven by WISHBONE slave interface

data output signals:
wbr_data_out = data output - data from PCI bus
wbr_be_out      = byte enable output(~#BE)
wbr_control_out = control output - encoded control bus output

status signals - monitored by various resources in the core
wbr_flush_in = flush signal input for WBR_FIFO - when asserted, fifo is flushed(emptied)
wbr full_out = full output from WBR_FIFO
wbr_empty_out = empty output from WBR_FIFO
-----------------------------------------------------------------------------------------------------------*/
// input control and data
input        wbr_wenable_in ;
input [31:0] wbr_data_in ;
input [3:0]  wbr_be_in ;
input [3:0]  wbr_control_in ;

// output control and data
input         wbr_renable_in ;
output [31:0] wbr_data_out ;
output [3:0]  wbr_be_out ;
output [3:0]  wbr_control_out ;

// flush input
input wbr_flush_in ;

output wbr_empty_out ;

`ifdef PCI_BIST
/*-----------------------------------------------------
BIST debug chain port signals
-----------------------------------------------------*/
input   mbist_si_i;       // bist scan serial in
output  mbist_so_o;       // bist scan serial out
input [`PCI_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i;       // bist chain shift control
`endif

/*-----------------------------------------------------------------------------------------------------------
FIFO depth parameters:
WBW_DEPTH = defines WBW_FIFO depth
WBR_DEPTH = defines WBR_FIFO depth
WBW_ADDR_LENGTH = defines WBW_FIFO's location address length = log2(WBW_DEPTH)
WBR_ADDR_LENGTH = defines WBR_FIFO's location address length = log2(WBR_DEPTH)
-----------------------------------------------------------------------------------------------------------*/
parameter WBW_DEPTH = `WBW_DEPTH ;
parameter WBW_ADDR_LENGTH = `WBW_ADDR_LENGTH ;
parameter WBR_DEPTH = `WBR_DEPTH ;
parameter WBR_ADDR_LENGTH = `WBR_ADDR_LENGTH ;

/*-----------------------------------------------------------------------------------------------------------
wbw_wallow = WBW_FIFO write allow wire - writes to FIFO are allowed when FIFO isn't full and write enable is 1
wbw_rallow = WBW_FIFO read allow wire - reads from FIFO are allowed when FIFO isn't empty and read enable is 1
-----------------------------------------------------------------------------------------------------------*/
wire wbw_wallow ;
wire wbw_rallow ;

/*-----------------------------------------------------------------------------------------------------------
wbr_wallow = WBR_FIFO write allow wire - writes to FIFO are allowed when FIFO isn't full and write enable is 1
wbr_rallow = WBR_FIFO read allow wire - reads from FIFO are allowed when FIFO isn't empty and read enable is 1
-----------------------------------------------------------------------------------------------------------*/
wire wbr_wallow ;
wire wbr_rallow ;

/*-----------------------------------------------------------------------------------------------------------
wires for address port conections from WBW_FIFO control logic to RAM blocks used for WBW_FIFO
-----------------------------------------------------------------------------------------------------------*/
wire [(WBW_ADDR_LENGTH - 1):0] wbw_raddr ;
wire [(WBW_ADDR_LENGTH - 1):0] wbw_waddr ;

/*-----------------------------------------------------------------------------------------------------------
wires for address port conections from WBR_FIFO control logic to RAM blocks used for WBR_FIFO
-----------------------------------------------------------------------------------------------------------*/
wire [(WBR_ADDR_LENGTH - 1):0] wbr_raddr ;
wire [(WBR_ADDR_LENGTH - 1):0] wbr_waddr ;

/*-----------------------------------------------------------------------------------------------------------
WBW_FIFO transaction counters: used to count incoming transactions and outgoing transactions. When number of
input transactions is equal to number of output transactions, it means that there isn't any complete transaction
currently present in the FIFO.
-----------------------------------------------------------------------------------------------------------*/
reg [(WBW_ADDR_LENGTH - 2):0] wbw_inTransactionCount ;
reg [(WBW_ADDR_LENGTH - 2):0] wbw_outTransactionCount ;

/*-----------------------------------------------------------------------------------------------------------
wires monitoring control bus. When control bus on a write transaction has a value of `LAST, it means that
complete transaction is in the FIFO. When control bus on a read transaction has a value of `LAST,
it means that there was one complete transaction taken out of FIFO.
-----------------------------------------------------------------------------------------------------------*/
wire wbw_last_in  = wbw_control_in[`LAST_CTRL_BIT]  ;
wire wbw_last_out = wbw_control_out[`LAST_CTRL_BIT] ;

wire wbw_empty ;
wire wbr_empty ;

assign wbw_empty_out = wbw_empty ;
assign wbr_empty_out = wbr_empty ;

// clear wires for fifos
wire wbw_clear = reset_in /*|| wbw_flush_in*/ ; // WBW_FIFO clear flush not used
wire wbr_clear = reset_in /*|| wbr_flush_in*/ ; // WBR_FIFO clear - flush changed from asynchronous to synchronous

/*-----------------------------------------------------------------------------------------------------------
Definitions of wires for connecting RAM instances
-----------------------------------------------------------------------------------------------------------*/
wire [39:0] dpram_portA_output ;
wire [39:0] dpram_portB_output ;

wire [39:0] dpram_portA_input = {wbw_control_in, wbw_cbe_in, wbw_addr_data_in} ;
wire [39:0] dpram_portB_input = {wbr_control_in, wbr_be_in, wbr_data_in} ;

/*-----------------------------------------------------------------------------------------------------------
Fifo output assignments - each ram port provides data for different fifo
-----------------------------------------------------------------------------------------------------------*/
assign wbw_control_out = dpram_portB_output[39:36] ;
assign wbr_control_out = dpram_portA_output[39:36] ;

assign wbw_cbe_out     = dpram_portB_output[35:32] ;
assign wbr_be_out      = dpram_portA_output[35:32] ;

assign wbw_addr_data_out = dpram_portB_output[31:0] ;
assign wbr_data_out      = dpram_portA_output[31:0] ;

`ifdef WB_RAM_DONT_SHARE

    /*-----------------------------------------------------------------------------------------------------------
    Piece of code in this ifdef section is used in applications which can provide enough RAM instances to
    accomodate four fifos - each occupying its own instance of ram. Ports are connected in such a way,
    that instances of RAMs can be changed from two port to dual port ( async read/write port ). In that case,
    write port is always port a and read port is port b.
    -----------------------------------------------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------------------------------------------
    Pad redundant address lines with zeros. This may seem stupid, but it comes in perfect for FPGA impl.
    -----------------------------------------------------------------------------------------------------------*/
    /*
    wire [(`WBW_FIFO_RAM_ADDR_LENGTH - WBW_ADDR_LENGTH - 1):0] wbw_addr_prefix = {( `WBW_FIFO_RAM_ADDR_LENGTH - WBW_ADDR_LENGTH){1'b0}} ;
    wire [(`WBR_FIFO_RAM_ADDR_LENGTH - WBR_ADDR_LENGTH - 1):0] wbr_addr_prefix = {( `WBR_FIFO_RAM_ADDR_LENGTH - WBR_ADDR_LENGTH){1'b0}} ;
    */

    // compose complete port addresses
    wire [(`WB_FIFO_RAM_ADDR_LENGTH-1):0] wbw_whole_waddr = wbw_waddr ;
    wire [(`WB_FIFO_RAM_ADDR_LENGTH-1):0] wbw_whole_raddr = wbw_raddr ;

    wire [(`WB_FIFO_RAM_ADDR_LENGTH-1):0] wbr_whole_waddr = wbr_waddr ;
    wire [(`WB_FIFO_RAM_ADDR_LENGTH-1):0] wbr_whole_raddr = wbr_raddr ;

    wire wbw_read_enable = 1'b1 ;
    wire wbr_read_enable = 1'b1 ;

    `ifdef PCI_BIST
    wire mbist_so_o_internal ; // wires for connection of debug ports on two rams
    wire mbist_si_i_internal = mbist_so_o_internal ;
    `endif

    // instantiate and connect two generic rams - one for wishbone write fifo and one for wishbone read fifo
    pci_wb_tpram #(`WB_FIFO_RAM_ADDR_LENGTH, 40) wbw_fifo_storage
    (
        // Generic synchronous two-port RAM interface
        .clk_a(wb_clock_in),
        .rst_a(reset_in),
        .ce_a(1'b1),
        .we_a(wbw_wallow),
        .oe_a(1'b1),
        .addr_a(wbw_whole_waddr),
        .di_a(dpram_portA_input),
        .do_a(),

        .clk_b(pci_clock_in),
        .rst_b(reset_in),
        .ce_b(wbw_read_enable),
        .we_b(1'b0),
        .oe_b(1'b1),
        .addr_b(wbw_whole_raddr),
        .di_b(40'h00_0000_0000),
        .do_b(dpram_portB_output)

    `ifdef PCI_BIST
        ,
        .mbist_si_i       (mbist_si_i),
        .mbist_so_o       (mbist_so_o_internal),
        .mbist_ctrl_i       (mbist_ctrl_i)
    `endif
    );

    pci_wb_tpram #(`WB_FIFO_RAM_ADDR_LENGTH, 40) wbr_fifo_storage
    (
        // Generic synchronous two-port RAM interface
        .clk_a(pci_clock_in),
        .rst_a(reset_in),
        .ce_a(1'b1),
        .we_a(wbr_wallow),
        .oe_a(1'b1),
        .addr_a(wbr_whole_waddr),
        .di_a(dpram_portB_input),
        .do_a(),

        .clk_b(wb_clock_in),
        .rst_b(reset_in),
        .ce_b(wbr_read_enable),
        .we_b(1'b0),
        .oe_b(1'b1),
        .addr_b(wbr_whole_raddr),
        .di_b(40'h00_0000_0000),
        .do_b(dpram_portA_output)

    `ifdef PCI_BIST
        ,
        .mbist_si_i       (mbist_si_i_internal),
        .mbist_so_o       (mbist_so_o),
        .mbist_ctrl_i       (mbist_ctrl_i)
    `endif
    );

`else // RAM blocks sharing between two fifos

    /*-----------------------------------------------------------------------------------------------------------
    Code section under this ifdef is used for implementation where RAM instances are too expensive. In this
    case one RAM instance is used for both - WISHBONE read and WISHBONE write fifo.
    -----------------------------------------------------------------------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------------
    Address prefix definition - since both FIFOs reside in same RAM instance, storage is separated by MSB
    addresses. WISHBONE write fifo addresses are padded with zeros on the MSB side ( at least one address line
    must be used for this ), WISHBONE read fifo addresses are padded with ones on the right ( at least one ).
    -----------------------------------------------------------------------------------------------------------*/
    wire [(`WB_FIFO_RAM_ADDR_LENGTH - WBW_ADDR_LENGTH - 1):0] wbw_addr_prefix = {( `WB_FIFO_RAM_ADDR_LENGTH - WBW_ADDR_LENGTH){1'b0}} ;
    wire [(`WB_FIFO_RAM_ADDR_LENGTH - WBR_ADDR_LENGTH - 1):0] wbr_addr_prefix = {( `WB_FIFO_RAM_ADDR_LENGTH - WBR_ADDR_LENGTH){1'b1}} ;

    /*-----------------------------------------------------------------------------------------------------------
    Port A address generation for RAM instance. RAM instance must be full two port RAM - read and write capability
    on both sides.
    Port A is clocked by WISHBONE clock, DIA is input for wbw_fifo, DOA is output for wbr_fifo.
    Address is multiplexed so operation can be switched between fifos. Default is a read on port.
    -----------------------------------------------------------------------------------------------------------*/
    wire [(`WB_FIFO_RAM_ADDR_LENGTH-1):0] portA_addr = wbw_wallow ? {wbw_addr_prefix, wbw_waddr} : {wbr_addr_prefix, wbr_raddr} ;

    /*-----------------------------------------------------------------------------------------------------------
    Port B is clocked by PCI clock, DIB is input for wbr_fifo, DOB is output for wbw_fifo.
    Address is multiplexed so operation can be switched between fifos. Default is a read on port.
    -----------------------------------------------------------------------------------------------------------*/
    wire [(`WB_FIFO_RAM_ADDR_LENGTH-1):0] portB_addr  = wbr_wallow ? {wbr_addr_prefix, wbr_waddr} : {wbw_addr_prefix, wbw_raddr} ;

    wire portA_enable      = 1'b1 ;

    wire portB_enable      = 1'b1 ;

    // instantiate RAM for these two fifos
    pci_wb_tpram #(`WB_FIFO_RAM_ADDR_LENGTH, 40) wbu_fifo_storage
    (
        // Generic synchronous two-port RAM interface
        .clk_a(wb_clock_in),
        .rst_a(reset_in),
        .ce_a(portA_enable),
        .we_a(wbw_wallow),
        .oe_a(1'b1),
        .addr_a(portA_addr),
        .di_a(dpram_portA_input),
        .do_a(dpram_portA_output),
        .clk_b(pci_clock_in),
        .rst_b(reset_in),
        .ce_b(portB_enable),
        .we_b(wbr_wallow),
        .oe_b(1'b1),
        .addr_b(portB_addr),
        .di_b(dpram_portB_input),
        .do_b(dpram_portB_output)

    `ifdef PCI_BIST
        ,
        .mbist_si_i       (mbist_si_i),
        .mbist_so_o       (mbist_so_o),
        .mbist_ctrl_i       (mbist_ctrl_i)
    `endif
    );

`endif

/*-----------------------------------------------------------------------------------------------------------
Instantiation of two control logic modules - one for WBW_FIFO and one for WBR_FIFO
-----------------------------------------------------------------------------------------------------------*/
pci_wbw_fifo_control #(WBW_ADDR_LENGTH) wbw_fifo_ctrl
(
    .rclock_in(pci_clock_in),
    .wclock_in(wb_clock_in),
    .renable_in(wbw_renable_in),
    .wenable_in(wbw_wenable_in),
    .reset_in(reset_in),
//    .flush_in(wbw_flush_in),
    .almost_full_out(wbw_almost_full_out),
    .full_out(wbw_full_out),
    .empty_out(wbw_empty),
    .waddr_out(wbw_waddr),
    .raddr_out(wbw_raddr),
    .rallow_out(wbw_rallow),
    .wallow_out(wbw_wallow)
);

pci_wbr_fifo_control #(WBR_ADDR_LENGTH) wbr_fifo_ctrl
(   .rclock_in(wb_clock_in),
    .wclock_in(pci_clock_in),
    .renable_in(wbr_renable_in),
    .wenable_in(wbr_wenable_in),
    .reset_in(reset_in),
    .flush_in(wbr_flush_in),
    .empty_out(wbr_empty),
    .waddr_out(wbr_waddr),
    .raddr_out(wbr_raddr),
    .rallow_out(wbr_rallow),
    .wallow_out(wbr_wallow)
);


// in and out transaction counters and grey codes
reg  [(WBW_ADDR_LENGTH-2):0] inGreyCount ;
reg  [(WBW_ADDR_LENGTH-2):0] outGreyCount ;
wire [(WBW_ADDR_LENGTH-2):0] inNextGreyCount = {wbw_inTransactionCount[(WBW_ADDR_LENGTH-2)], wbw_inTransactionCount[(WBW_ADDR_LENGTH-2):1] ^ wbw_inTransactionCount[(WBW_ADDR_LENGTH-3):0]} ;
wire [(WBW_ADDR_LENGTH-2):0] outNextGreyCount = {wbw_outTransactionCount[(WBW_ADDR_LENGTH-2)], wbw_outTransactionCount[(WBW_ADDR_LENGTH-2):1] ^ wbw_outTransactionCount[(WBW_ADDR_LENGTH-3):0]} ;

// input transaction counter increment - when last data of transaction is written to fifo
wire in_count_en  = wbw_wallow && wbw_last_in ;

// output transaction counter increment - when last data is on top of fifo and read from it
wire out_count_en = wbw_renable_in && wbw_last_out ;

// register holding grey coded count of incoming transactions
always@(posedge wb_clock_in or posedge wbw_clear)
begin
    if (wbw_clear)
    begin
        inGreyCount <= #3 0 ;
    end
    else
    if (in_count_en)
        inGreyCount <= #3 inNextGreyCount ;
end

wire [(WBW_ADDR_LENGTH-2):0] pci_clk_sync_inGreyCount ;
reg  [(WBW_ADDR_LENGTH-2):0] pci_clk_inGreyCount ;
pci_synchronizer_flop #((WBW_ADDR_LENGTH - 1), 0) i_synchronizer_reg_inGreyCount
(
    .data_in        (inGreyCount),
    .clk_out        (pci_clock_in),
    .sync_data_out  (pci_clk_sync_inGreyCount),
    .async_reset    (wbw_clear)
) ;

always@(posedge pci_clock_in or posedge wbw_clear)
begin
    if (wbw_clear)
        pci_clk_inGreyCount <= #`FF_DELAY 0 ;
    else
        pci_clk_inGreyCount <= # `FF_DELAY pci_clk_sync_inGreyCount ;
end

// register holding grey coded count of outgoing transactions
always@(posedge pci_clock_in or posedge wbw_clear)
begin
    if (wbw_clear)
    begin
        outGreyCount <= #`FF_DELAY 0 ;
    end
    else
    if (out_count_en)
        outGreyCount <= #`FF_DELAY outNextGreyCount ;
end

// incoming transactions counter
always@(posedge wb_clock_in or posedge wbw_clear)
begin
    if (wbw_clear)
        wbw_inTransactionCount <= #`FF_DELAY 1 ;
    else
    if (in_count_en)
        wbw_inTransactionCount <= #`FF_DELAY wbw_inTransactionCount + 1'b1 ;
end

// outgoing transactions counter
always@(posedge pci_clock_in or posedge wbw_clear)
begin
    if (wbw_clear)
        wbw_outTransactionCount <= 1 ;
    else
    if (out_count_en)
        wbw_outTransactionCount <= #`FF_DELAY wbw_outTransactionCount + 1'b1 ;
end

assign wbw_transaction_ready_out = pci_clk_inGreyCount != outGreyCount ;

endmodule

