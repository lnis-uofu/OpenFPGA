//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "wb_slave.v"                                      ////
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
// $Log: pci_wb_slave.v,v $
// Revision 1.5  2004/01/24 11:54:18  mihad
// Update! SPOCI Implemented!
//
// Revision 1.4  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.3  2003/08/14 18:01:53  simons
// ifdefs moved to thier own lines, this confuses some of the tools.
//
// Revision 1.2  2003/08/03 18:05:06  mihad
// Added limited WISHBONE B3 support for WISHBONE Slave Unit.
// Doesn't support full speed bursts yet.
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.4  2002/08/19 16:54:25  mihad
// Got rid of undef directives
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

`include "bus_commands.v"
`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_wb_slave
               (    wb_clock_in,
                    reset_in,
                    wb_hit_in,
                    wb_conf_hit_in,
                    wb_map_in,
                    wb_pref_en_in,
                    wb_mrl_en_in,
                    wb_addr_in,
                    del_bc_in,
                    wb_del_req_pending_in,
                    wb_del_comp_pending_in,
                    pci_drcomp_pending_in,
                    del_bc_out,
                    del_req_out,
                    del_done_out,
                    del_burst_out,
                    del_write_out,
                    del_write_in,
                    del_error_in,
                    del_in_progress_out,
                    ccyc_addr_in,
                    wb_del_addr_in,
                    wb_del_be_in,
                    wb_conf_offset_out,
                    wb_conf_renable_out,
                    wb_conf_wenable_out,
                    wb_conf_be_out,
                    wb_conf_data_in,
                    wb_conf_data_out,
                    wb_data_out,
                    wb_cbe_out,
                    wbw_fifo_wenable_out,
                    wbw_fifo_control_out,
                    wbw_fifo_almost_full_in,
                    wbw_fifo_full_in,
                    wbr_fifo_renable_out,
                    wbr_fifo_be_in,
                    wbr_fifo_data_in,
                    wbr_fifo_control_in,
                    wbr_fifo_flush_out,
                    wbr_fifo_empty_in,
                    pciw_fifo_empty_in,
                    wbs_lock_in,
                    init_complete_in,
                    cache_line_size_not_zero,
                    sample_address_out,
                    CYC_I,
                    STB_I,
                    WE_I,
                    SEL_I,
                    SDATA_I,
                    SDATA_O,
                    ACK_O,
                    RTY_O,
                    ERR_O,
                    CAB_I
                );

/*----------------------------------------------------------------------------------------------------------------------
Various parameters needed for state machine and other stuff
----------------------------------------------------------------------------------------------------------------------*/
parameter WBR_SEL  = 1'b0 ;
parameter CONF_SEL = 1'b1 ;

`define FSM_BITS 3
parameter S_IDLE         = `FSM_BITS'h0 ;
parameter S_DEC1         = `FSM_BITS'h1 ;
parameter S_DEC2         = `FSM_BITS'h2 ;
parameter S_START        = `FSM_BITS'h3 ;
parameter S_W_ADDR_DATA  = `FSM_BITS'h4 ;
parameter S_READ         = `FSM_BITS'h5 ;
parameter S_CONF_WRITE   = `FSM_BITS'h6 ;
parameter S_CONF_READ    = `FSM_BITS'h7 ;

/*----------------------------------------------------------------------------------------------------------------------
System signals inputs
wb_clock_in - WISHBONE bus clock input
reset_in    - system reset input controlled by bridge's reset logic
----------------------------------------------------------------------------------------------------------------------*/
input wb_clock_in, reset_in ;

/*----------------------------------------------------------------------------------------------------------------------
Inputs from address decoding logic
wb_hit_in - Decoder logic indicates if address is in a range of one of images
wb_conf_hit_in - Decoder logic indicates that address is in configuration space range
wb_map_in   - Decoder logic provides information about image mapping - memory mapped image   - wb_map_in = 0
                                                                       IO space mapped image - wb_map_in = 1
wb_pref_en_in - Prefetch enable signal from currently selected image - used for PCI bus command usage
wb_addr_in - Address already transalted from WB bus to PCI bus input
wb_mrl_en_in - Memory read line enable input for each image
----------------------------------------------------------------------------------------------------------------------*/
input [4:0]     wb_hit_in ;         // hit indicators
input           wb_conf_hit_in ;    // configuration hit indicator
input [4:0]     wb_pref_en_in ;     // prefetch enable from all images
input [4:0]     wb_mrl_en_in ;      // Memory Read line command enable from images
input [4:0]     wb_map_in ;         // address space mapping indicators - 1 memory space mapping, 0-IO space mapping
input [31:0]    wb_addr_in ;        // Translated address input

/*----------------------------------------------------------------------------------------------------------------------
Delayed transaction control inputs and outputs:
Used for locking particular accesses when delayed transactions are in progress:
wb_del_addr_in - delayed transaction address input - when completion is ready it's used for transaction decoding
wb_del_be_in   - delayed transaction byte enable input - when completion is ready it's used for transaction decoding
----------------------------------------------------------------------------------------------------------------------*/
input  [31:0] wb_del_addr_in ;
input  [3:0]  wb_del_be_in ;

input [3:0] del_bc_in ;           // delayed request bus command used
input       wb_del_req_pending_in ;   // delayed request pending indicator
input       wb_del_comp_pending_in ;  // delayed completion pending indicator
input       pci_drcomp_pending_in ; // PCI initiated delayed read completion pending

output [3:0] del_bc_out ; // delayed transaction bus command output

output del_req_out ; // output for issuing delayed transaction requests

output del_done_out ; // output indicating current delayed completion finished on WISHBONE bus

output del_burst_out ; // delayed burst transaction indicator

output del_in_progress_out ; // delayed in progress indicator - since delayed transaction can be a burst transaction, progress indicator must be used for proper operation

output del_write_out ;   // write enable for delayed transaction - used for indicating that transaction is a write

input  del_write_in ;    // indicates that current delayed completion is from a write request
input  del_error_in ;    // indicate that delayed request terminated with an error - used for write requests

input  [31:0] ccyc_addr_in ; // configuration cycle address input - it's separate from other addresses, since it is stored separately and decoded for type 0 configuration access

/*----------------------------------------------------------------------------------------------------------------------
Configuration space access control and data signals
wb_conf_offset_out  - lower 12 bits of address input provided for register offset
wb_conf_renable     - read enable signal for configuration space accesses
wb_conf_wenable     - write enable signal for configuration space accesses
wb_conf_be_out      - byte enable signals for configuration space accesses
wb_conf_data_in     - data from configuration space
wb_conf_data_in     - data provided for configuration space
----------------------------------------------------------------------------------------------------------------------*/
output [11:0]   wb_conf_offset_out ;  // register offset output
output          wb_conf_renable_out,  // configuration read and write enable outputs
                wb_conf_wenable_out ;
output [3:0]    wb_conf_be_out ;      // byte enable outputs for configuration space
input  [31:0]   wb_conf_data_in ;     // configuration data input from configuration space
output [31:0]   wb_conf_data_out ;    // configuration data output for configuration space

/*----------------------------------------------------------------------------------------------------------------------
Data from WISHBONE bus output to interiror of the core:
Data output is used for normal and configuration accesses.
---------------------------------------------------------------------------------------------------------------------*/
output [31:0] wb_data_out ;

/*----------------------------------------------------------------------------------------------------------------------
Bus command - byte enable output - during address phase of image access this bus holds information about PCI
bus command that should be used, during dataphases ( configuration or image access ) this bus contains inverted
SEL_I signals
---------------------------------------------------------------------------------------------------------------------*/
output [3:0] wb_cbe_out ;

/*----------------------------------------------------------------------------------------------------------------------
WBW_FIFO control signals used for sinking data into WBW_FIFO and status monitoring
---------------------------------------------------------------------------------------------------------------------*/
output       wbw_fifo_wenable_out ;    // write enable for WBW_FIFO output
output [3:0] wbw_fifo_control_out ;    // control bus output for WBW_FIFO
input        wbw_fifo_almost_full_in ; // almost full status indicator from WBW_FIFO
input        wbw_fifo_full_in ;        // full status indicator from WBW_FIFO

/*----------------------------------------------------------------------------------------------------------------------
WBR_FIFO control signals used for fetching data from WBR_FIFO and status monitoring
---------------------------------------------------------------------------------------------------------------------*/
output          wbr_fifo_renable_out ;      // WBR_FIFO read enable output
input   [3:0]   wbr_fifo_be_in ;            // byte enable input from WBR_FIFO
input   [31:0]  wbr_fifo_data_in ;          // data input from WBR_FIFO
input   [3:0]   wbr_fifo_control_in ;       // control bus input from WBR_FIFO
output          wbr_fifo_flush_out ;        // flush signal for WBR_FIFO
input           wbr_fifo_empty_in ;         // empty status indicator from WBR_FIFO

// used for transaction ordering requirements - WISHBONE read cannot complete until writes from PCI are completed
input           pciw_fifo_empty_in ;        // empty status indicator from PCIW_FIFO

/*----------------------------------------------------------------------------------------------------------------------
wbs_lock_in: internal signal that locks out all accesses, except delayed completions or configuration accesses.
( when master operation is disabled via master enable bit in configuration spacei )
init_complete_in: while initialization sequence is in progress, the state machine
remains in the idle state - it does not respond to accesses.
---------------------------------------------------------------------------------------------------------------------*/
input           wbs_lock_in ;
input           init_complete_in ;

// cache line size register must hold appropriate value to enable read bursts and special commands on PCI bus!
input           cache_line_size_not_zero ;

// state machine signals to wb_addr_mux when to sample wb address input
output          sample_address_out ;
reg             sample_address_out ;

/*----------------------------------------------------------------------------------------------------------------------
WISHBONE bus interface signals - can be connected directly to WISHBONE bus
---------------------------------------------------------------------------------------------------------------------*/
input           CYC_I ;     // cycle indicator
input           STB_I ;     // strobe input - input data is valid when strobe and cycle indicator are high
input           WE_I  ;     // write enable input - 1 - write operation, 0 - read operation
input   [3:0]   SEL_I ;     // Byte select inputs
input   [31:0]  SDATA_I ;   // WISHBONE slave interface input data bus
output  [31:0]  SDATA_O ;   // WISHBONE slave interface output data bus
output          ACK_O ;     // Acknowledge output - qualifies valid data on data output bus or received data on data input bus
output          RTY_O ;     // retry output - signals to WISHBONE master that cycle should be terminated and retried later
output          ERR_O ;     // Signals to WISHBONE master that access resulted in an error
input           CAB_I ;     // consecutive address burst input - indicates that master will do a serial address transfer in current cycle

`ifdef REGISTER_WBS_OUTPUTS
reg [31:0]  SDATA_O ;
reg         ACK_O   ;
reg         RTY_O   ;
reg         ERR_O   ;

reg [3:0] del_bc_out ; // delayed transaction bus command output
reg       del_req_out ; // output for issuing delayed transaction requests
reg       del_done_out ; // output indicating current delayed completion finished on WISHBONE bus
reg       del_burst_out ; // delayed burst transaction indicator
reg       del_in_progress_out ; // delayed in progress indicator - since delayed transaction can be a burst transaction, progress indicator must be used for proper operation
reg       del_write_out ;   // write enable for delayed transaction - used for indicating that transaction is a write

`ifdef HOST
reg          wb_conf_wenable_out ;
reg [31:0]   wb_conf_data_out ;    // configuration data output for configuration space
`endif

reg [3:0]  wb_conf_be_out ;      // byte enable outputs for configuration space
reg [31:0] wb_data_out ;

reg [3:0] wb_cbe_out ;

reg       wbw_fifo_wenable_out ;    // write enable for WBW_FIFO output
reg [3:0] wbw_fifo_control_out ;    // control bus output for WBW_FIFO

reg       wbr_fifo_renable_out ;      // WBR_FIFO read enable output
`endif

reg [(`FSM_BITS - 1):0]  c_state ; //current state register

reg [(`FSM_BITS - 1):0]  n_state ; //next state input to current state register

// state machine register control
always@(posedge wb_clock_in or posedge reset_in)
begin
    if (reset_in)
        c_state <= #`FF_DELAY S_IDLE ;
    else
        c_state <= #`FF_DELAY n_state ;
end


// variable for bus command multiplexer logic output for delayed requests
reg [3:0] del_bc ;

//register for intermediate data and select storage
reg [35:0] d_incoming ;

// enable for incoming data register
reg d_incoming_ena ;

// incoming data register control logic
always@(posedge wb_clock_in or posedge reset_in)
begin
    if (reset_in)
        d_incoming <= #`FF_DELAY {35{1'b0}} ;
    else if (d_incoming_ena)
        d_incoming <= #`FF_DELAY {SEL_I, SDATA_I} ;
end

/*===================================================================================================================================================================================
Write allow for image accesses. Writes through images are allowed when all of following are true:
- WBW_FIFO musn't be almost full nor full for image writes to be allowed - Every transaction takes at least two locations in the FIFO
- delayed read from from WISHBONE to PCI request musn't be present
- delayed read from PCI to WISHBONE completion musn't be present
- lock input musn't be set - it can be set because of error reporting or because PCI master state machine is disabled
===================================================================================================================================================================================*/
wire wimg_wallow           = ~|{ wbw_fifo_almost_full_in , wbw_fifo_full_in, wb_del_req_pending_in, pci_drcomp_pending_in, wbs_lock_in } ;
reg img_wallow ;
/*===================================================================================================================================================================================
WISHBONE slave can request an image read accesses when all of following are true:
- delayed completion is not present
- delayed request is not present
- operation is not locked because of error reporting mechanism or because PCI master is disabled
===================================================================================================================================================================================*/
wire wdo_del_request     = ~|{ wb_del_req_pending_in, wb_del_comp_pending_in, wbs_lock_in } ;
reg do_del_request ;
/*===================================================================================================================================================================================
WISHBONE slave can complete an image read accesses when all of following are true:
- delayed read completion is present
- delayed read completion is the same as current read access ( dread_completion_hit is 1 )
- PCI Write FIFO is empty - no posted write is waiting to be finished in PCIW_FIFO
- WBR_FIFO empty status is not active
===================================================================================================================================================================================*/
wire wdel_addr_hit = ( wb_del_addr_in == wb_addr_in ) && ( SEL_I == wb_del_be_in ) ;
reg  del_addr_hit ;
wire wdel_completion_allow = wb_del_comp_pending_in && ((~del_write_in && ~WE_I && pciw_fifo_empty_in && ~wbr_fifo_empty_in) || (del_write_in && WE_I)) ;
reg del_completion_allow ;

/*----------------------------------------------------------------------------------------------------------------------
img_hit - state of wb_hit_in bus when when state machine signals decode is over
---------------------------------------------------------------------------------------------------------------------*/
reg [4:0] img_hit ;
wire wb_hit = |( img_hit ) ;

/*----------------------------------------------------------------------------------------------------------------------
Control logic for image control signals
pref_en - prefetch enable of currently selected image
mrl_en  - Memory read line enable of currently selected image
map     - Address space mapping for currently selected image
---------------------------------------------------------------------------------------------------------------------*/
reg pref_en, mrl_en, map ;
wire wpref_en   = |(wb_pref_en_in & wb_hit_in) ;
wire wmrl_en    = |(wb_mrl_en_in & wb_hit_in) ;
wire wmap       = |(wb_map_in & wb_hit_in) ;

// state machine controls when results from decoders, comparison etc. are sampled into registers to decode an access
reg decode_en ;

reg wb_conf_hit ;
always@(posedge reset_in or posedge wb_clock_in)
begin
    if (reset_in)
    begin
        img_wallow           <= #`FF_DELAY 1'b0 ;
        wb_conf_hit          <= #`FF_DELAY 1'b0 ;
        do_del_request       <= #`FF_DELAY 1'b0 ;
        del_addr_hit         <= #`FF_DELAY 1'b0 ;
        del_completion_allow <= #`FF_DELAY 1'b0 ;
        img_hit              <= #`FF_DELAY 5'h00 ;
        pref_en              <= #`FF_DELAY 1'b0 ;
        mrl_en               <= #`FF_DELAY 1'b0 ;
        map                  <= #`FF_DELAY 1'b0 ;
    end
    else
    if (decode_en)
    begin
        img_wallow           <= #`FF_DELAY wimg_wallow ;
        wb_conf_hit          <= #`FF_DELAY wb_conf_hit_in ;
        do_del_request       <= #`FF_DELAY wdo_del_request ;
        del_addr_hit         <= #`FF_DELAY wdel_addr_hit ;
        del_completion_allow <= #`FF_DELAY wdel_completion_allow ;
        img_hit              <= #`FF_DELAY wb_hit_in ;
        pref_en              <= #`FF_DELAY wpref_en && cache_line_size_not_zero ;
        mrl_en               <= #`FF_DELAY wmrl_en  && cache_line_size_not_zero ;
        map                  <= #`FF_DELAY wmap ;
    end
end

wire   del_burst = CAB_I && (pref_en || mrl_en) && ~WE_I && cache_line_size_not_zero ; // delayed burst indicator - only when WB master attempts CAB transfer and cache line size register is set appropriately and
                                                                                       // either prefetch enable or memory read line enable of corresponding image are set -
                                                                                       // applies for reads only - delayed write cannot be a burst
wire do_dread_completion = del_completion_allow && del_addr_hit ;

`ifdef GUEST

    // wires indicating allowance for configuration cycle generation requests
    wire do_ccyc_req  = 1'b0 ;
    wire do_ccyc_comp = 1'b0 ;

    // wires indicating allowance for interrupt acknowledge cycle generation requests
    wire do_iack_req  = 1'b0 ;
    wire do_iack_comp = 1'b0 ;

    // variables for configuration access control signals
    reg conf_wenable ;
    assign wb_conf_wenable_out = 1'b0 ;

    // configuration cycle data register hit
    wire ccyc_hit = 1'b0 ;
    wire iack_hit = 1'b0 ;

    wire wccyc_hit = 1'b0 ;
    wire wiack_hit = 1'b0 ;

`else
`ifdef HOST
    // only host implementation has access for generating interrupt acknowledge and configuration cycles
    // configuration cycle data register hit
    reg current_delayed_is_ccyc ;
    reg current_delayed_is_iack ;

    wire wccyc_hit = (wb_addr_in[8:2] == {1'b1, `CNF_DATA_ADDR}) ;

    wire wiack_hit = (wb_addr_in[8:2] == {1'b1, `INT_ACK_ADDR})  ;

    reg iack_hit ;
    reg ccyc_hit ;
    always@(posedge reset_in or posedge wb_clock_in)
    begin
        if (reset_in)
        begin
            ccyc_hit <= #`FF_DELAY 1'b0 ;
            iack_hit <= #`FF_DELAY 1'b0 ;
        end
        else
        if (decode_en)
        begin
            ccyc_hit <= #`FF_DELAY wccyc_hit ;
            iack_hit <= #`FF_DELAY wiack_hit ;
        end
    end

    // wires indicating allowance for configuration cycle generation requests
    wire do_ccyc_req  = do_del_request && ccyc_hit;
    wire do_ccyc_comp = del_completion_allow && ccyc_hit && current_delayed_is_ccyc ; // && del_bc_hit

    // wires indicating allowance for interrupt acknowledge cycle generation requests
    wire do_iack_req  = do_del_request && iack_hit ;
    wire do_iack_comp = del_completion_allow && iack_hit && current_delayed_is_iack ; // && del_bc_hit

    // variables for configuration access control signals
    reg conf_wenable ;

    // following flip-flops remember whether current delayed transaction is interrupt acknowledge or configuration cycle transaction
    always@(posedge wb_clock_in or posedge reset_in)
    begin
        if ( reset_in )
        begin
            current_delayed_is_ccyc <= #`FF_DELAY 1'b0 ;
            current_delayed_is_iack <= #`FF_DELAY 1'b0 ;
        end
        else
        if ( del_done_out )
        begin
            current_delayed_is_ccyc <= #`FF_DELAY 1'b0 ;
            current_delayed_is_iack <= #`FF_DELAY 1'b0 ;
        end
        else
        if ( del_req_out && wb_conf_hit )
        begin
            current_delayed_is_ccyc <= #`FF_DELAY do_ccyc_req ;
            current_delayed_is_iack <= #`FF_DELAY do_iack_req ;
        end
    end

`endif
`endif

// configuration read enable - supplied for host and guest bridges
reg conf_renable ;
assign wb_conf_renable_out = conf_renable ;

// burst access indicator
wire burst_transfer = CYC_I && CAB_I ;

// WBW_FIFO control output
reg [3:0] wbw_fifo_control ;

// WBW_FIFO wenable output assignment
reg wbw_fifo_wenable ;

// WBR_FIFO control outputs
reg wbr_fifo_flush, wbr_fifo_renable ; // flush and read enable outputs

// flush signal for WBR_FIFO must be registered, since it asinchronously resets some status registers
wire		wbr_fifo_flush_reg ;
pci_async_reset_flop async_reset_as_wbr_flush
(
    .data_in        	  (wbr_fifo_flush),
    .clk_in         	  (wb_clock_in),
    .async_reset_data_out (wbr_fifo_flush_reg),
    .reset_in    		  (reset_in)
) ;
assign  wbr_fifo_flush_out = wbr_fifo_flush_reg ;

// delayed transaction request control signals
reg del_req, del_done ;

// WISHBONE handshaking control outputs
reg ack, rty, err ;

`ifdef REGISTER_WBS_OUTPUTS
// wire for write attempt - 1 when external WB master is attempting a write
// wire for read attempt  - 1 when external master is attempting a read
wire wattempt = ( CYC_I && STB_I && WE_I ) && (!ACK_O && !ERR_O && !RTY_O) ;
wire rattempt = ( CYC_I && STB_I && ~WE_I ) && (!ACK_O && !ERR_O && !RTY_O) ;

`else
// wire for write attempt - 1 when external WB master is attempting a write
// wire for read attempt  - 1 when external master is attempting a read
wire wattempt = ( CYC_I && STB_I && WE_I ) ; // write is qualified when cycle, strobe and write enable inputs are all high
wire rattempt = ( CYC_I && STB_I && ~WE_I ) ; // read is qualified when cycle and strobe are high and write enable is low

`endif
/*----------------------------------------------------------------------------------------------------------------------
Delayed transaction bus command generation
Bus command for delayed reads depends on image's address space mapping and control bits and
whether or not these are interrupt acknowledge requests or configuration cycle requests
---------------------------------------------------------------------------------------------------------------------*/

always@(map or mrl_en or ccyc_hit or WE_I or wb_conf_hit or CAB_I or pref_en)
begin
`ifdef HOST
// only host implementation supports configuration and interrupt acknowledge commands
    if (wb_conf_hit)
    begin
        case( {ccyc_hit, WE_I} )
            2'b11:  del_bc = `BC_CONF_WRITE ;
            2'b10:  del_bc = `BC_CONF_READ ;
            2'b01:  del_bc = `BC_RESERVED0 ; // invalid combination - interrupt acknowledge cycle must be a read
            2'b00:  del_bc = `BC_IACK ;
        endcase
    end
    else
`endif
    begin
        if ( map )
        begin
            del_bc = `BC_IO_READ ;
        end
        else
        begin
            case ({(CAB_I && mrl_en), pref_en})
                2'b00: del_bc = `BC_MEM_READ ;     // if this is not burst transfer or memory read line command is disabled - use memory read
                2'b01: del_bc = `BC_MEM_READ ;     // same as previous case
                2'b10: del_bc = `BC_MEM_READ_LN ;  // burst transfer, memory read line command enabled, prefetch disabled - use memory read line command
                2'b11: del_bc = `BC_MEM_READ_MUL ; // same as previous case, except prefetch is enabled - use memory read multiple command
            endcase
        end
    end
end

reg del_in_progress ; // state machine indicates whether current read completion is in progress on WISHBONE bus

wire image_access_error = (map && burst_transfer) ; // IO write is a burst

`ifdef HOST
    reg [1:0]   wbw_data_out_sel ;
    parameter SEL_ADDR_IN = 2'b10 ;
    parameter SEL_CCYC_ADDR = 2'b11 ;
    parameter SEL_DATA_IN   = 2'b00 ;
`else
`ifdef GUEST
    reg wbw_data_out_sel ;
    parameter SEL_ADDR_IN = 1'b1 ;
    parameter SEL_DATA_IN = 1'b0 ;
`endif
`endif

`ifdef WB_DECODE_FAST
    `ifdef REGISTER_WBS_OUTPUTS
        `define PCI_WB_SLAVE_S_DEC1
    `endif
`endif

`ifdef WB_DECODE_MEDIUM
    `define PCI_WB_SLAVE_S_DEC1
`endif

`ifdef WB_DECODE_SLOW
    `define PCI_WB_SLAVE_S_DEC1
    `define PCI_WB_SLAVE_S_DEC2
`endif
// state machine logic
always@(
        c_state                     or
        wattempt                    or
        img_wallow                  or
        burst_transfer              or
        wb_hit                      or
        map                         or
        rattempt                    or
        do_dread_completion         or
        wbr_fifo_control_in         or
        wb_conf_hit                 or
        do_ccyc_req                 or
        do_ccyc_comp                or
        ccyc_hit                    or
        del_error_in                or
        do_iack_req                 or
        do_iack_comp                or
        iack_hit                    or
        image_access_error          or
        wbw_fifo_almost_full_in     or
        wbw_fifo_full_in            or
        do_del_request              or
        wbr_fifo_empty_in           or
        init_complete_in
       )
begin
    // default signal values
    // response signals inactive
    ack         = 1'b0 ;
    rty         = 1'b0 ;
    err         = 1'b0 ;

    //write signals inactive
    wbw_fifo_control[`ADDR_CTRL_BIT] = 1'b1 ;
    wbw_fifo_control[`DATA_ERROR_CTRL_BIT] = 1'b0 ;
    wbw_fifo_control[`LAST_CTRL_BIT] = 1'b0 ;
    wbw_fifo_control[`UNUSED_CTRL_BIT] = 1'b0 ;

    wbw_fifo_wenable = 1'b0 ;
    d_incoming_ena   = 1'b0 ;

    // read signals inactive
    wbr_fifo_flush   = 1'b0 ;
    wbr_fifo_renable = 1'b0 ;
    del_req          = 1'b0 ;
    del_done         = 1'b0 ;

    // configuration space control signals inactive
    conf_wenable = 1'b0 ;
    conf_renable = 1'b0 ;

    // read is not in progress
    del_in_progress = 1'b0 ;

    decode_en = 1'b0 ;

    wbw_data_out_sel         = SEL_ADDR_IN ;

    sample_address_out = 1'b0 ;

    case (c_state)
    S_IDLE: begin
                if ( (wattempt || rattempt) & init_complete_in )
                begin
                
                `ifdef PCI_WB_SLAVE_S_DEC1
                    n_state = S_DEC1 ;
                `else
                    decode_en = 1'b1 ;
                    n_state = S_START ;
                `endif
                    
                    sample_address_out = 1'b1 ;
                end
                else
                    n_state = S_IDLE ;
            end
`ifdef PCI_WB_SLAVE_S_DEC1
    S_DEC1: begin
                if ( wattempt || rattempt )
                begin

                `ifdef PCI_WB_SLAVE_S_DEC2
                    n_state = S_DEC2 ;
                `else
                    decode_en = 1'b1 ;
                    n_state = S_START ;
                `endif

                end
                else
                    n_state = S_IDLE ;
            end
`endif
`ifdef PCI_WB_SLAVE_S_DEC2
    S_DEC2: begin

                if ( wattempt || rattempt )
                begin
                    decode_en = 1'b1 ;
                    n_state = S_START ;
                end
                else
                    n_state = S_IDLE ;
            end
`endif
    S_START:begin
                if (wb_conf_hit) // configuration space hit
                begin
                    `ifdef HOST
                        wbw_data_out_sel = SEL_CCYC_ADDR ;
                    `endif

                    if ( wattempt )
                        n_state = S_CONF_WRITE ; // go to conf. write state
                    else
                    if ( rattempt )
                    begin
                        n_state     = S_CONF_READ ; // go to conf. read state
                    end
                    else
                        n_state = S_IDLE ; // master terminated - go back to idle state

                end // wb_conf_hit
                else
                if( wb_hit && (wattempt || rattempt) )
                begin
                    wbw_data_out_sel = SEL_DATA_IN ;

                    // check error conditions for image writes or reads
                    if ( image_access_error )
                    begin
                        n_state = S_IDLE ; // go back to idle state because of an error condition
                        err     = 1'b1 ;
                    end // error conditions
                    else
                    // check for retry conditions for image writes or reads
                    if ( (wattempt && ~img_wallow) ||
                         (rattempt && ~do_dread_completion) // write to image not allowed, no read ready yet - retry
                       )
                    begin
                        n_state = S_IDLE ; // go back to IDLE

                        rty     = 1'b1 ;

                        del_req = do_del_request && rattempt ;

                    end //retry
                    else // everything OK - proceed
                    if ( wattempt )
                    begin
                        n_state = S_W_ADDR_DATA ; // goto write transfer state

                        // respond with acknowledge
                        ack              = 1'b1 ;

                        wbw_fifo_wenable = 1'b1 ;

                        // data is latched to data incoming intermidiate stage - it will be put in FIFO later
                        d_incoming_ena   = 1'b1 ;
                    end
                    else
                    begin
                        err = wbr_fifo_control_in[`DATA_ERROR_CTRL_BIT] ;
                        ack = ~wbr_fifo_control_in[`DATA_ERROR_CTRL_BIT] ;
                        wbr_fifo_renable    = 1'b1 ;
                        del_in_progress = 1'b1 ;

                        if ( wbr_fifo_control_in[`DATA_ERROR_CTRL_BIT] || wbr_fifo_control_in[`LAST_CTRL_BIT] )
                        begin

                            n_state = S_IDLE ; // go back to idle state
                            // respond that read is finished
                            del_done     = 1'b1 ;

                        end // end read
                        else
                            n_state = S_READ ; // go to read state
                    end
                end
                else
                    n_state = S_IDLE ;

            end

    S_W_ADDR_DATA: begin
                        wbw_data_out_sel = SEL_DATA_IN ;
                        err = 1'b0 ;
                        rty = burst_transfer && wattempt && (wbw_fifo_almost_full_in || wbw_fifo_full_in) ;

                        if ( ~burst_transfer || wattempt && ( wbw_fifo_almost_full_in || wbw_fifo_full_in ) )
                        begin
                            n_state = S_IDLE ;

                            // write last data to FIFO and don't latch new data
                            wbw_fifo_control[`ADDR_CTRL_BIT] = 1'b0 ;
                            wbw_fifo_control[`LAST_CTRL_BIT] = 1'b1 ;
                            wbw_fifo_wenable = 1'b1 ;
                        end
                        else
                        begin
                            n_state = S_W_ADDR_DATA ;
                            wbw_fifo_control[`ADDR_CTRL_BIT] = 1'b0 ;
                            wbw_fifo_control[`LAST_CTRL_BIT] = 1'b0 ;
                            ack              = wattempt ;
                            wbw_fifo_wenable = wattempt ;
                            d_incoming_ena   = wattempt ;
                        end
                    end // S_W_ADDR_DATA

    S_READ:begin
                // this state is for reads only - in this state read is in progress all the time
                del_in_progress = 1'b1 ;

                ack = burst_transfer && rattempt && ~wbr_fifo_control_in[`DATA_ERROR_CTRL_BIT] && ~wbr_fifo_empty_in ;
                err = burst_transfer && rattempt && wbr_fifo_control_in[`DATA_ERROR_CTRL_BIT]  && ~wbr_fifo_empty_in  ;

                // if acknowledge is beeing signalled then enable read from wbr fifo
                wbr_fifo_renable = burst_transfer && rattempt && ~wbr_fifo_empty_in ;

                if ( ~burst_transfer || rattempt && (wbr_fifo_empty_in || wbr_fifo_control_in[`DATA_ERROR_CTRL_BIT] || wbr_fifo_control_in[`LAST_CTRL_BIT]) )
                begin
                    n_state = S_IDLE ;
                    del_done = 1'b1 ;
                    wbr_fifo_flush = ~wbr_fifo_empty_in ;
                end
                else
                begin
                    n_state          = S_READ ;
                end
            end // S_READ

    S_CONF_WRITE:  begin
                        `ifdef HOST
                            wbw_data_out_sel = SEL_CCYC_ADDR ;
                            del_req          = do_ccyc_req && ~burst_transfer  ;
                            del_done         = do_ccyc_comp && ~burst_transfer ;
                            del_in_progress  = do_ccyc_comp && ~burst_transfer ;
                        `endif

                        n_state         = S_IDLE ; // next state after configuration access is always idle

                        if ( burst_transfer )
                        begin
                            err = 1'b1 ;
                        end
                        else
                        begin
                            `ifdef HOST
                            if ( do_ccyc_req || (ccyc_hit && ~do_ccyc_comp))
                            begin
                                rty = 1'b1 ;
                            end
                            else
                            if ( do_ccyc_comp )
                            begin
                                err = del_error_in ;
                                ack = ~del_error_in ;
                            end
                            else
                            begin
                                ack = ~ccyc_hit ;
                                conf_wenable = ~ccyc_hit ;
                            end
                            `else
                            ack = 1'b1 ;
                            conf_wenable = 1'b1 ;
                            `endif
                        end
                    end // S_CONF_WRITE

    S_CONF_READ:   begin
                        `ifdef HOST
                            wbw_data_out_sel = SEL_CCYC_ADDR ;
                            del_req          = ~burst_transfer && ( do_ccyc_req  || do_iack_req  ) ;
                            del_done         = ~burst_transfer && ( do_ccyc_comp || do_iack_comp ) ;
                            del_in_progress  = ~burst_transfer && ( do_ccyc_comp || do_iack_comp ) ;
                            wbr_fifo_renable = ~burst_transfer && ( do_ccyc_comp || do_iack_comp ) ;
                        `endif

                        n_state = S_IDLE ; // next state after configuration access is always idle

                        if ( burst_transfer )
                        begin
                            err = 1'b1 ;
                        end
                        else
                        begin
                            `ifdef HOST
                            if ( do_ccyc_req || ( ccyc_hit && ~do_ccyc_comp ))
                            begin
                                rty = 1'b1 ;
                            end
                            else
                            if ( do_iack_req || ( iack_hit && ~do_iack_comp ))
                            begin
                                rty = 1'b1 ;
                            end
                            else
                            if ( do_iack_comp || do_ccyc_comp )
                            begin
                                err = del_error_in ;
                                ack = ~del_error_in ;
                            end
                            else
                            begin
                                ack = ~( ccyc_hit || iack_hit ) ;
                                conf_renable = ~( ccyc_hit || iack_hit ) ;
                            end
                            `else
                            ack = 1'b1 ;
                            conf_renable = 1'b1 ;
                            `endif
                        end
                    end //S_CONF_READ
    default:begin
                n_state = S_IDLE ; // return to idle state
            end //default
    endcase
end

// configuration space offset output assignment
assign wb_conf_offset_out = {wb_addr_in[11:2], 2'b00} ; // upper 10 bits of address input and two zeros

// data output assignment - for image writes, first data is address, subsequent data comes from intermediate register
reg [31:0] wb_data ;
`ifdef HOST
reg [1:0] wbw_data_out_sel_reg ;
always@(posedge wb_clock_in or posedge reset_in)
begin
    if ( reset_in )
        wbw_data_out_sel_reg <= #`FF_DELAY SEL_ADDR_IN ;
    else
        wbw_data_out_sel_reg <= #`FF_DELAY wbw_data_out_sel ;
end

always@(wbw_data_out_sel_reg or wb_addr_in or ccyc_addr_in or d_incoming)
begin
    case ( wbw_data_out_sel_reg )
        SEL_CCYC_ADDR:  wb_data = ccyc_addr_in ;
        SEL_DATA_IN:    wb_data = d_incoming ;
        default: wb_data        = wb_addr_in ;
    endcase
end
`else
`ifdef GUEST
reg wbw_data_out_sel_reg ;
always@(posedge wb_clock_in or posedge reset_in)
begin
    if ( reset_in )
        wbw_data_out_sel_reg <= #`FF_DELAY SEL_ADDR_IN ;
    else
        wbw_data_out_sel_reg <= #`FF_DELAY wbw_data_out_sel ;
end

always@(wbw_data_out_sel_reg or wb_addr_in or d_incoming)
begin
    if ( wbw_data_out_sel_reg )
        wb_data = wb_addr_in ;
    else
        wb_data = d_incoming ;
end
`endif
`endif

// command / byte enable assignment - with address, bus command is provided, with data - byte enables are provided
reg [3:0] wb_cbe ;

always@(wbw_data_out_sel_reg or d_incoming or map)
begin
    if (wbw_data_out_sel_reg && map)
        wb_cbe = `BC_IO_WRITE ;
    else
    if (wbw_data_out_sel_reg)
        wb_cbe = `BC_MEM_WRITE ;
    else
        wb_cbe = ~(d_incoming[35:32]) ;
end

// for configuration writes, data output is always data from WISHBONE - in guest implementation data is all 0.
`ifdef GUEST
    assign wb_conf_data_out = 32'h00000000 ;
`endif

`ifdef GUEST
    `ifdef NO_CNF_IMAGE
    `else
        `define PCI_WB_SLAVE_DO_OUT_MUX
    `endif
`else
`ifdef HOST
    `define PCI_WB_SLAVE_DO_OUT_MUX ;
`endif
`endif

`ifdef PCI_WB_SLAVE_DO_OUT_MUX
    reg [31:0] sdata_source ;

    // WISHBONE data output select lines for output multiplexor
    wire sdata_o_sel_new = ( wb_conf_hit_in && ~wiack_hit && ~wccyc_hit ) ? CONF_SEL : WBR_SEL ;
    reg sdata_o_sel ;

    always@(posedge wb_clock_in or posedge reset_in)
    begin
        if ( reset_in )
            sdata_o_sel <= #`FF_DELAY WBR_SEL ;
        else
        if ( decode_en )
            sdata_o_sel <= #`FF_DELAY sdata_o_sel_new ;
    end

    always@(sdata_o_sel or wbr_fifo_data_in or wb_conf_data_in)
    begin
        case (sdata_o_sel)
            WBR_SEL :sdata_source = wbr_fifo_data_in ;
            CONF_SEL:sdata_source = wb_conf_data_in ;
        endcase
    end
`else
    wire [31:0] sdata_source = wbr_fifo_data_in ;
`endif

`ifdef REGISTER_WBS_OUTPUTS

always@(posedge wb_clock_in or posedge reset_in)
begin
    if ( reset_in )
    begin
        ACK_O         <= #`FF_DELAY 1'b0 ;
        RTY_O         <= #`FF_DELAY 1'b0 ;
        ERR_O         <= #`FF_DELAY 1'b0 ;
        SDATA_O       <= #`FF_DELAY 0 ;
        del_write_out <= #`FF_DELAY 1'b0 ;

        `ifdef HOST
        wb_conf_wenable_out <= #`FF_DELAY 1'b0 ;
        wb_conf_data_out    <= #`FF_DELAY 0 ;
        `endif

        del_bc_out    <= #`FF_DELAY `BC_RESERVED0 ;
        del_req_out <= #`FF_DELAY 1'b0 ;
        del_done_out <= #`FF_DELAY 1'b0 ;
        del_burst_out <= #`FF_DELAY 1'b0 ;
        del_in_progress_out <= #`FF_DELAY 1'b0 ;
        wb_conf_be_out <= #`FF_DELAY 0 ;
        wb_data_out    <= #`FF_DELAY 0 ;
        wb_cbe_out <= #`FF_DELAY 0 ;
        wbw_fifo_wenable_out <= #`FF_DELAY 0 ;
        wbw_fifo_control_out <= #`FF_DELAY 0 ;
        wbr_fifo_renable_out <= #`FF_DELAY 0 ;
    end
    else
    begin
        ACK_O   <= #`FF_DELAY ack && !ACK_O ;
        RTY_O   <= #`FF_DELAY rty && !RTY_O ;
        ERR_O    <= #`FF_DELAY err && !ERR_O ;
        SDATA_O       <= #`FF_DELAY sdata_source ;
        del_write_out <= #`FF_DELAY WE_I ;

        `ifdef HOST
        wb_conf_wenable_out <= #`FF_DELAY conf_wenable ;
        wb_conf_data_out    <= #`FF_DELAY SDATA_I ;
        `endif

        del_bc_out <= #`FF_DELAY del_bc ;
        del_req_out <= #`FF_DELAY del_req ;
        del_done_out <= #`FF_DELAY del_done ;
        del_burst_out <= #`FF_DELAY del_burst ;
        del_in_progress_out <= #`FF_DELAY del_in_progress ;
        wb_conf_be_out <= #`FF_DELAY SEL_I ;
        wb_data_out <= #`FF_DELAY wb_data ;
        wb_cbe_out <= #`FF_DELAY wb_cbe ;
        wbw_fifo_wenable_out <= #`FF_DELAY wbw_fifo_wenable ;
        wbw_fifo_control_out <= #`FF_DELAY wbw_fifo_control ;
        wbr_fifo_renable_out <= #`FF_DELAY wbr_fifo_renable ;
    end
end

`else

assign SDATA_O = sdata_source ;

assign ACK_O = ack ;
assign RTY_O = rty ;
assign ERR_O = err ;

// write operation indicator for delayed transaction requests
assign del_write_out = WE_I ;
assign del_bc_out = del_bc ;
assign del_req_out  = del_req ; // read request
assign del_done_out = del_done ; // read done
assign del_burst_out = del_burst ;
assign del_in_progress_out = del_in_progress ;
`ifdef HOST
assign wb_conf_data_out = SDATA_I ;
assign wb_conf_wenable_out = conf_wenable ;
`endif
// Configuration space byte enables output
assign wb_conf_be_out = SEL_I ; // just route select lines from WISHBONE to conf space
assign wb_data_out    = wb_data ;
assign wb_cbe_out = wb_cbe ;
assign wbw_fifo_wenable_out = wbw_fifo_wenable ; //write enable for WBW_FIFO
assign wbw_fifo_control_out = wbw_fifo_control ; //control bus output for WBW_FIFO
assign wbr_fifo_renable_out = wbr_fifo_renable ; //read enable for wbr_fifo
`endif

endmodule //WB_SLAVE
