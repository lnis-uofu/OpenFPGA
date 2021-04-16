//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pciw_pcir_fifos.v"                               ////
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
//// Copyright (C) 2000 Miha Dolenc, mihad@opencores.org          ////
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
// $Log: pci_pciw_pcir_fifos.v,v $
// Revision 1.6  2003/10/17 09:11:52  markom
// mbist signals updated according to newest convention
//
// Revision 1.5  2003/08/14 13:06:03  simons
// synchronizer_flop replaced with pci_synchronizer_flop, artisan ram instance updated.
//
// Revision 1.4  2003/08/08 16:36:33  tadejm
// Added 'three_left_out' to pci_pciw_fifo signaling three locations before full. Added comparison between current registered cbe and next unregistered cbe to signal wb_master whether it is allowed to performe burst or not. Due to this, I needed 'three_left_out' so that writing to pci_pciw_fifo can be registered, otherwise timing problems would occure.
//
// Revision 1.3  2003/03/26 13:16:18  mihad
// Added the reset value parameter to the synchronizer flop module.
// Added resets to all synchronizer flop instances.
// Repaired initial sync value in fifos.
//
// Revision 1.2  2003/01/30 22:01:08  mihad
// Updated synchronization in top level fifo modules.
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.10  2002/10/18 03:36:37  tadejm
// Changed wrong signal name mbist_sen into mbist_ctrl_i.
//
// Revision 1.9  2002/10/17 22:51:08  tadejm
// Changed BIST signals for RAMs.
//
// Revision 1.8  2002/10/11 10:09:01  mihad
// Added additional testcase and changed rst name in BIST to trst
//
// Revision 1.7  2002/10/08 17:17:06  mihad
// Added BIST signals for RAMs.
//
// Revision 1.6  2002/09/30 16:03:04  mihad
// Added meta flop module for easier meta stable FF identification during synthesis
//
// Revision 1.5  2002/09/25 15:53:52  mihad
// Removed all logic from asynchronous reset network
//
// Revision 1.4  2002/03/05 11:53:47  mihad
// Added some testcases, removed un-needed fifo signals
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

`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_pciw_pcir_fifos
(
    wb_clock_in,
    pci_clock_in,
    reset_in,
    pciw_wenable_in,
    pciw_addr_data_in,
    pciw_cbe_in,
    pciw_control_in,
    pciw_renable_in,
    pciw_addr_data_out,
    pciw_cbe_out,
    pciw_control_out,
//    pciw_flush_in,    // not used
    pciw_three_left_out,
    pciw_two_left_out,
    pciw_almost_full_out,
    pciw_full_out,
    pciw_almost_empty_out,
    pciw_empty_out,
    pciw_transaction_ready_out,
    pcir_wenable_in,
    pcir_data_in,
    pcir_be_in,
    pcir_control_in,
    pcir_renable_in,
    pcir_data_out,
    pcir_be_out,
    pcir_control_out,
    pcir_flush_in,
    pcir_full_out,
    pcir_almost_empty_out,
    pcir_empty_out,
    pcir_transaction_ready_out

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
PCI WRITE FIFO interface signals prefixed with pciw_ - FIFO is used for posted writes initiated by external
PCI master through PCI target interface, traveling through FIFO and are completed on WISHBONE by
WISHBONE master interface

write enable signal:
pciw_wenable_in = write enable input for PCIW_FIFO - driven by PCI TARGET interface

data input signals:
pciw_addr_data_in = data input - data from PCI bus - first entry of transaction is address others are data entries
pciw_cbe_in       = bus command/byte enable(~#BE[3:0]) input - first entry of transaction is bus command, other are byte enables
pciw_control_in   = control input - encoded control bus input

read enable signal:
pciw_renable_in = read enable input driven by WISHBONE master interface

data output signals:
pciw_addr_data_out = data output - data from PCI bus - first entry of transaction is address, others are data entries
pciw_cbe_out      = bus command/byte enable output - first entry of transaction is bus command, others are byte enables
pciw_control_out = control input - encoded control bus input

status signals - monitored by various resources in the core
pciw_flush_in = flush signal input for PCIW_FIFO - when asserted, fifo is flushed(emptied)
pciw_almost_full_out = almost full output from PCIW_FIFO
pciw_full_out = full output from PCIW_FIFO
pciw_almost_empty_out = almost empty output from PCIW_FIFO
pciw_empty_out = empty output from PCIW_FIFO
pciw_transaction_ready_out = output indicating that one complete transaction is waiting in PCIW_FIFO
-----------------------------------------------------------------------------------------------------------*/
// input control and data
input        pciw_wenable_in ;
input [31:0] pciw_addr_data_in ;
input [3:0]  pciw_cbe_in ;
input [3:0]  pciw_control_in ;

// output control and data
input         pciw_renable_in ;
output [31:0] pciw_addr_data_out ;
output [3:0]  pciw_cbe_out ;
output [3:0]  pciw_control_out ;

// flush input
//input pciw_flush_in ;     // not used

// status outputs
output pciw_three_left_out ;
output pciw_two_left_out ;
output pciw_almost_full_out ;
output pciw_full_out ;
output pciw_almost_empty_out ;
output pciw_empty_out ;
output pciw_transaction_ready_out ;

/*-----------------------------------------------------------------------------------------------------------
PCI READ FIFO interface signals prefixed with pcir_ - FIFO is used for holding delayed read completions
initiated by master on PCI bus and completed on WISHBONE bus,

write enable signal:
pcir_wenable_in = write enable input for PCIR_FIFO - driven by WISHBONE master interface

data input signals:
pcir_data_in      = data input - data from WISHBONE bus - there is no address entry here, since address is stored in separate register
pcir_be_in        = byte enable(~SEL[3:0]) input - byte enables - same through one transaction
pcir_control_in   = control input - encoded control bus input

read enable signal:
pcir_renable_in = read enable input driven by PCI target interface

data output signals:
pcir_data_out = data output - data from WISHBONE bus
pcir_be_out      = byte enable output(~SEL)
pcir_control_out = control output - encoded control bus output

status signals - monitored by various resources in the core
pcir_flush_in = flush signal input for PCIR_FIFO - when asserted, fifo is flushed(emptied)
pcir full_out = full output from PCIR_FIFO
pcir_almost_empty_out = almost empty output from PCIR_FIFO
pcir_empty_out = empty output from PCIR_FIFO
pcir_transaction_ready_out = output indicating that one complete transaction is waiting in PCIR_FIFO
-----------------------------------------------------------------------------------------------------------*/
// input control and data
input        pcir_wenable_in ;
input [31:0] pcir_data_in ;
input [3:0]  pcir_be_in ;
input [3:0]  pcir_control_in ;

// output control and data
input         pcir_renable_in ;
output [31:0] pcir_data_out ;
output [3:0]  pcir_be_out ;
output [3:0]  pcir_control_out ;

// flush input
input pcir_flush_in ;

// status outputs
output pcir_full_out ;
output pcir_almost_empty_out ;
output pcir_empty_out ;
output pcir_transaction_ready_out ;

`ifdef PCI_BIST
/*-----------------------------------------------------
BIST debug chain port signals
-----------------------------------------------------*/
input   mbist_si_i;       // bist scan serial in
output  mbist_so_o;       // bist scan serial out
input [`PCI_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i;       // bist chain shift control
`endif

/*-----------------------------------------------------------------------------------------------------------
Address length parameters:
PCIW_DEPTH = defines PCIW_FIFO depth
PCIR_DEPTH = defines PCIR_FIFO depth
PCIW_ADDR_LENGTH = defines PCIW_FIFO's location address length - log2(PCIW_DEPTH)
PCIR_ADDR_LENGTH = defines PCIR_FIFO's location address length - log2(PCIR_DEPTH)
-----------------------------------------------------------------------------------------------------------*/
parameter PCIW_DEPTH = `PCIW_DEPTH ;
parameter PCIW_ADDR_LENGTH = `PCIW_ADDR_LENGTH ;
parameter PCIR_DEPTH = `PCIR_DEPTH ;
parameter PCIR_ADDR_LENGTH = `PCIR_ADDR_LENGTH ;

/*-----------------------------------------------------------------------------------------------------------
pciw_wallow = PCIW_FIFO write allow wire - writes to FIFO are allowed when FIFO isn't full and write enable is 1
pciw_rallow = PCIW_FIFO read allow wire - reads from FIFO are allowed when FIFO isn't empty and read enable is 1
-----------------------------------------------------------------------------------------------------------*/
wire pciw_wallow ;
wire pciw_rallow ;

/*-----------------------------------------------------------------------------------------------------------
pcir_wallow = PCIR_FIFO write allow wire - writes to FIFO are allowed when FIFO isn't full and write enable is 1
pcir_rallow = PCIR_FIFO read allow wire - reads from FIFO are allowed when FIFO isn't empty and read enable is 1
-----------------------------------------------------------------------------------------------------------*/
wire pcir_wallow ;
wire pcir_rallow ;

/*-----------------------------------------------------------------------------------------------------------
wires for address port conections from PCIW_FIFO control logic to RAM blocks used for PCIW_FIFO
-----------------------------------------------------------------------------------------------------------*/
wire [(PCIW_ADDR_LENGTH - 1):0] pciw_raddr ;
wire [(PCIW_ADDR_LENGTH - 1):0] pciw_waddr ;

/*-----------------------------------------------------------------------------------------------------------
wires for address port conections from PCIR_FIFO control logic to RAM blocks used for PCIR_FIFO
-----------------------------------------------------------------------------------------------------------*/
wire [(PCIR_ADDR_LENGTH - 1):0] pcir_raddr ;
wire [(PCIR_ADDR_LENGTH - 1):0] pcir_waddr ;

/*-----------------------------------------------------------------------------------------------------------
PCIW_FIFO transaction counters: used to count incoming transactions and outgoing transactions. When number of
input transactions is equal to number of output transactions, it means that there isn't any complete transaction
currently present in the FIFO.
-----------------------------------------------------------------------------------------------------------*/
reg [(PCIW_ADDR_LENGTH - 1):0] pciw_inTransactionCount ;
reg [(PCIW_ADDR_LENGTH - 1):0] pciw_outTransactionCount ;

/*-----------------------------------------------------------------------------------------------------------
FlipFlops for indicating if complete delayed read completion is present in the FIFO
-----------------------------------------------------------------------------------------------------------*/
/*reg pcir_inTransactionCount ;
reg pcir_outTransactionCount ;*/
/*-----------------------------------------------------------------------------------------------------------
wires monitoring control bus. When control bus on a write transaction has a value of `LAST, it means that
complete transaction is in the FIFO. When control bus on a read transaction has a value of `LAST,
it means that there was one complete transaction taken out of FIFO.
-----------------------------------------------------------------------------------------------------------*/
wire pciw_last_in  = pciw_control_in[`LAST_CTRL_BIT] ;
wire pciw_last_out = pciw_control_out[`LAST_CTRL_BIT] ;

/*wire pcir_last_in  = pcir_wallow && (pcir_control_in == `LAST) ;
wire pcir_last_out = pcir_rallow && (pcir_control_out == `LAST) ;*/

wire pciw_empty ;
wire pcir_empty ;

assign pciw_empty_out = pciw_empty ;
assign pcir_empty_out = pcir_empty ;

// clear wires for clearing FFs and registers
wire pciw_clear = reset_in /*|| pciw_flush_in*/ ; // PCIW_FIFO's clear signal - flush not used
wire pcir_clear = reset_in /*|| pcir_flush_in*/ ; // PCIR_FIFO's clear signal - flush changed to synchronous op.

/*-----------------------------------------------------------------------------------------------------------
Definitions of wires for connecting RAM instances
-----------------------------------------------------------------------------------------------------------*/
wire [39:0] dpram_portA_output ;
wire [39:0] dpram_portB_output ;

wire [39:0] dpram_portA_input = {pciw_control_in, pciw_cbe_in, pciw_addr_data_in} ;
wire [39:0] dpram_portB_input = {pcir_control_in, pcir_be_in, pcir_data_in} ;

/*-----------------------------------------------------------------------------------------------------------
Fifo output assignments - each ram port provides data for different fifo
-----------------------------------------------------------------------------------------------------------*/
assign pciw_control_out = dpram_portB_output[39:36] ;
assign pcir_control_out = dpram_portA_output[39:36] ;

assign pciw_cbe_out     = dpram_portB_output[35:32] ;
assign pcir_be_out      = dpram_portA_output[35:32] ;

assign pciw_addr_data_out = dpram_portB_output[31:0] ;
assign pcir_data_out      = dpram_portA_output[31:0] ;

`ifdef PCI_RAM_DONT_SHARE

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
    wire [(`PCIW_FIFO_RAM_ADDR_LENGTH - PCIW_ADDR_LENGTH - 1):0] pciw_addr_prefix = {( `PCIW_FIFO_RAM_ADDR_LENGTH - PCIW_ADDR_LENGTH){1'b0}} ;
    wire [(`PCIR_FIFO_RAM_ADDR_LENGTH - PCIR_ADDR_LENGTH - 1):0] pcir_addr_prefix = {( `PCIR_FIFO_RAM_ADDR_LENGTH - PCIR_ADDR_LENGTH){1'b0}} ;
    */

    // compose complete port addresses
    wire [(`PCI_FIFO_RAM_ADDR_LENGTH-1):0] pciw_whole_waddr = pciw_waddr ;
    wire [(`PCI_FIFO_RAM_ADDR_LENGTH-1):0] pciw_whole_raddr = pciw_raddr ;

    wire [(`PCI_FIFO_RAM_ADDR_LENGTH-1):0] pcir_whole_waddr = pcir_waddr ;
    wire [(`PCI_FIFO_RAM_ADDR_LENGTH-1):0] pcir_whole_raddr = pcir_raddr ;

    wire pciw_read_enable = 1'b1 ;
    wire pcir_read_enable = 1'b1 ;

    `ifdef PCI_BIST
    wire mbist_so_o_internal ; // wires for connection of debug ports on two rams
    wire mbist_si_i_internal = mbist_so_o_internal ;
    `endif

    // instantiate and connect two generic rams - one for pci write fifo and one for pci read fifo
    pci_pci_tpram #(`PCI_FIFO_RAM_ADDR_LENGTH, 40) pciw_fifo_storage
    (
        // Generic synchronous two-port RAM interface
        .clk_a(pci_clock_in),
        .rst_a(reset_in),
        .ce_a(1'b1),
        .we_a(pciw_wallow),
        .oe_a(1'b1),
        .addr_a(pciw_whole_waddr),
        .di_a(dpram_portA_input),
        .do_a(),

        .clk_b(wb_clock_in),
        .rst_b(reset_in),
        .ce_b(pciw_read_enable),
        .we_b(1'b0),
        .oe_b(1'b1),
        .addr_b(pciw_whole_raddr),
        .di_b(40'h00_0000_0000),
        .do_b(dpram_portB_output)

    `ifdef PCI_BIST
        ,
        .mbist_si_i       (mbist_si_i),
        .mbist_so_o       (mbist_so_o_internal),
        .mbist_ctrl_i       (mbist_ctrl_i)
    `endif
    );

    pci_pci_tpram #(`PCI_FIFO_RAM_ADDR_LENGTH, 40) pcir_fifo_storage
    (
        // Generic synchronous two-port RAM interface
        .clk_a(wb_clock_in),
        .rst_a(reset_in),
        .ce_a(1'b1),
        .we_a(pcir_wallow),
        .oe_a(1'b1),
        .addr_a(pcir_whole_waddr),
        .di_a(dpram_portB_input),
        .do_a(),

        .clk_b(pci_clock_in),
        .rst_b(reset_in),
        .ce_b(pcir_read_enable),
        .we_b(1'b0),
        .oe_b(1'b1),
        .addr_b(pcir_whole_raddr),
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
    case one RAM instance is used for both - pci read and pci write fifo.
    -----------------------------------------------------------------------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------------
    Address prefix definition - since both FIFOs reside in same RAM instance, storage is separated by MSB
    addresses. pci write fifo addresses are padded with zeros on the MSB side ( at least one address line
    must be used for this ), pci read fifo addresses are padded with ones on the right ( at least one ).
    -----------------------------------------------------------------------------------------------------------*/
    wire [(`PCI_FIFO_RAM_ADDR_LENGTH - PCIW_ADDR_LENGTH - 1):0] pciw_addr_prefix = {( `PCI_FIFO_RAM_ADDR_LENGTH - PCIW_ADDR_LENGTH){1'b0}} ;
    wire [(`PCI_FIFO_RAM_ADDR_LENGTH - PCIR_ADDR_LENGTH - 1):0] pcir_addr_prefix = {( `PCI_FIFO_RAM_ADDR_LENGTH - PCIR_ADDR_LENGTH){1'b1}} ;

    /*-----------------------------------------------------------------------------------------------------------
    Port A address generation for RAM instance. RAM instance must be full two port RAM - read and write capability
    on both sides.
    Port A is clocked by PCI clock, DIA is input for pciw_fifo, DOA is output for pcir_fifo.
    Address is multiplexed so operation can be switched between fifos. Default is a read on port.
    -----------------------------------------------------------------------------------------------------------*/
    wire [(`PCI_FIFO_RAM_ADDR_LENGTH-1):0] portA_addr = pciw_wallow ? {pciw_addr_prefix, pciw_waddr} : {pcir_addr_prefix, pcir_raddr} ;

    /*-----------------------------------------------------------------------------------------------------------
    Port B is clocked by WISHBONE clock, DIB is input for pcir_fifo, DOB is output for pciw_fifo.
    Address is multiplexed so operation can be switched between fifos. Default is a read on port.
    -----------------------------------------------------------------------------------------------------------*/
    wire [(`PCI_FIFO_RAM_ADDR_LENGTH-1):0] portB_addr  = pcir_wallow ? {pcir_addr_prefix, pcir_waddr} : {pciw_addr_prefix, pciw_raddr} ;

    wire portA_enable      = 1'b1 ;

    wire portB_enable      = 1'b1 ;

    // instantiate RAM for these two fifos
    pci_pci_tpram #(`PCI_FIFO_RAM_ADDR_LENGTH, 40) pciu_fifo_storage
    (
        // Generic synchronous two-port RAM interface
        .clk_a(pci_clock_in),
        .rst_a(reset_in),
        .ce_a(portA_enable),
        .we_a(pciw_wallow),
        .oe_a(1'b1),
        .addr_a(portA_addr),
        .di_a(dpram_portA_input),
        .do_a(dpram_portA_output),
        .clk_b(wb_clock_in),
        .rst_b(reset_in),
        .ce_b(portB_enable),
        .we_b(pcir_wallow),
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
Instantiation of two control logic modules - one for PCIW_FIFO and one for PCIR_FIFO
-----------------------------------------------------------------------------------------------------------*/
pci_pciw_fifo_control #(PCIW_ADDR_LENGTH) pciw_fifo_ctrl
(
    .rclock_in(wb_clock_in),
    .wclock_in(pci_clock_in),
    .renable_in(pciw_renable_in),
    .wenable_in(pciw_wenable_in),
    .reset_in(reset_in),
//    .flush_in(pciw_flush_in),                     // flush not used
    .three_left_out(pciw_three_left_out),
    .two_left_out(pciw_two_left_out),
    .almost_full_out(pciw_almost_full_out),
    .full_out(pciw_full_out),
    .almost_empty_out(pciw_almost_empty_out),
    .empty_out(pciw_empty),
    .waddr_out(pciw_waddr),
    .raddr_out(pciw_raddr),
    .rallow_out(pciw_rallow),
    .wallow_out(pciw_wallow)
);

pci_pcir_fifo_control #(PCIR_ADDR_LENGTH) pcir_fifo_ctrl
(
    .rclock_in(pci_clock_in),
    .wclock_in(wb_clock_in),
    .renable_in(pcir_renable_in),
    .wenable_in(pcir_wenable_in),
    .reset_in(reset_in),
    .flush_in(pcir_flush_in),
    .full_out(pcir_full_out),
    .almost_empty_out(pcir_almost_empty_out),
    .empty_out(pcir_empty),
    .waddr_out(pcir_waddr),
    .raddr_out(pcir_raddr),
    .rallow_out(pcir_rallow),
    .wallow_out(pcir_wallow)
);


// in and out transaction counters and grey codes
reg  [(PCIW_ADDR_LENGTH-2):0] inGreyCount ;
reg  [(PCIW_ADDR_LENGTH-2):0] outGreyCount ;
wire [(PCIW_ADDR_LENGTH-2):0] inNextGreyCount  = {pciw_inTransactionCount[(PCIW_ADDR_LENGTH-2)], pciw_inTransactionCount[(PCIW_ADDR_LENGTH-2):1] ^ pciw_inTransactionCount[(PCIW_ADDR_LENGTH-3):0]} ;
wire [(PCIW_ADDR_LENGTH-2):0] outNextGreyCount = {pciw_outTransactionCount[(PCIW_ADDR_LENGTH-2)], pciw_outTransactionCount[(PCIW_ADDR_LENGTH-2):1] ^ pciw_outTransactionCount[(PCIW_ADDR_LENGTH-3):0]} ;

// input transaction counter is incremented when whole transaction is written to fifo. This is indicated by last control bit written to last transaction location
wire in_count_en  = pciw_wallow && pciw_last_in ;

// output transaction counter is incremented when whole transaction is pulled out of fifo. This is indicated when location with last control bit set is read
wire out_count_en = pciw_rallow && pciw_last_out ;

always@(posedge pci_clock_in or posedge pciw_clear)
begin
    if (pciw_clear)
    begin
        inGreyCount <= 0 ;
    end
    else
    if (in_count_en)
        inGreyCount <= #`FF_DELAY inNextGreyCount ;
end

wire [(PCIW_ADDR_LENGTH-2):0] wb_clk_sync_inGreyCount ;
reg  [(PCIW_ADDR_LENGTH-2):0] wb_clk_inGreyCount ;
pci_synchronizer_flop #((PCIW_ADDR_LENGTH - 1), 0) i_synchronizer_reg_inGreyCount
(
    .data_in        (inGreyCount),
    .clk_out        (wb_clock_in),
    .sync_data_out  (wb_clk_sync_inGreyCount),
    .async_reset    (pciw_clear)
) ;

always@(posedge wb_clock_in or posedge pciw_clear)
begin
    if (pciw_clear)
        wb_clk_inGreyCount <= #`FF_DELAY 0 ;
    else
        wb_clk_inGreyCount <= # `FF_DELAY wb_clk_sync_inGreyCount ;
end

always@(posedge wb_clock_in or posedge pciw_clear)
begin
    if (pciw_clear)
    begin
        outGreyCount <= #`FF_DELAY 0 ;
    end
    else
    if (out_count_en)
        outGreyCount <= #`FF_DELAY outNextGreyCount ;
end

always@(posedge pci_clock_in or posedge pciw_clear)
begin
    if (pciw_clear)
        pciw_inTransactionCount <= #`FF_DELAY 1 ;
    else
    if (in_count_en)
        pciw_inTransactionCount <= #`FF_DELAY pciw_inTransactionCount + 1'b1 ;
end

always@(posedge wb_clock_in or posedge pciw_clear)
begin
    if (pciw_clear)
        pciw_outTransactionCount <= #`FF_DELAY 1 ;
    else
    if (out_count_en)
        pciw_outTransactionCount <= #`FF_DELAY pciw_outTransactionCount + 1'b1 ;
end

assign pciw_transaction_ready_out = wb_clk_inGreyCount != outGreyCount ;

assign pcir_transaction_ready_out  = 1'b0 ;

endmodule

