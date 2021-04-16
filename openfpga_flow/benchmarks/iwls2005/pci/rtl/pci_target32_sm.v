//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: pci_target32_sm.v                                ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Tadej Markovic, tadej@opencores.org                   ////
////                                                              ////
////  All additional information is avaliable in the README.txt   ////
////  file.                                                       ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Tadej Markovic, tadej@opencores.org       ////
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
// $Log: pci_target32_sm.v,v $
// Revision 1.11  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.10  2003/08/08 16:36:33  tadejm
// Added 'three_left_out' to pci_pciw_fifo signaling three locations before full. Added comparison between current registered cbe and next unregistered cbe to signal wb_master whether it is allowed to performe burst or not. Due to this, I needed 'three_left_out' so that writing to pci_pciw_fifo can be registered, otherwise timing problems would occure.
//
// Revision 1.9  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.8  2003/01/21 16:06:56  mihad
// Bug fixes, testcases added.
//
// Revision 1.7  2002/09/24 19:09:17  mihad
// Number of state bits define was removed
//
// Revision 1.6  2002/09/24 18:30:00  mihad
// Changed state machine encoding to true one-hot
//
// Revision 1.5  2002/08/22 09:07:06  mihad
// Fixed a bug and provided testcase for it. Target was responding to configuration cycle type 1 transactions.
//
// Revision 1.4  2002/02/19 16:32:37  mihad
// Modified testbench and fixed some bugs
//
// Revision 1.3  2002/02/01 15:25:12  mihad
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

module pci_target32_sm
(
    // system inputs
    clk_in,
    reset_in,
    // master inputs
    pci_frame_in,
    pci_irdy_in,
    pci_idsel_in,
    pci_frame_reg_in,
    pci_irdy_reg_in,
    pci_idsel_reg_in,
    // target response outputs
    pci_trdy_out,
    pci_stop_out,
    pci_devsel_out,
    pci_trdy_en_out,
    pci_stop_en_out,
    pci_devsel_en_out,
    ad_load_out,
    ad_load_on_transfer_out,
    // address, data, bus command, byte enable in/outs
    pci_ad_reg_in,
    pci_ad_out,
    pci_ad_en_out,
    pci_cbe_reg_in,
    pci_cbe_in,
    bckp_trdy_en_in,
    bckp_devsel_in,
    bckp_trdy_in,
    bckp_stop_in,
    pci_trdy_reg_in,
    pci_stop_reg_in,

    // backend side of state machine with control signals to pci_io_mux ...
    address_out,
    addr_claim_in,
    bc_out,
    bc0_out,
    data_out,
    data_in,
    be_out,
    next_be_out,
    req_out,
    rdy_out,
    addr_phase_out,
    bckp_devsel_out,
    bckp_trdy_out,
    bckp_stop_out,
    last_reg_out,
    frame_reg_out,
    fetch_pcir_fifo_out,
    load_medium_reg_out,
    sel_fifo_mreg_out,
    sel_conf_fifo_out,
    load_to_pciw_fifo_out,
    load_to_conf_out,
    same_read_in,
    norm_access_to_config_in,
    read_completed_in,
    read_processing_in,
    target_abort_in,
    disconect_wo_data_in,
    disconect_w_data_in,
    target_abort_set_out,
    pciw_fifo_full_in,
    pcir_fifo_data_err_in,
    wbw_fifo_empty_in,
    wbu_del_read_comp_pending_in,
    wbu_frame_en_in

) ;

/*----------------------------------------------------------------------------------------------------------------------
Various parameters needed for state machine and other stuff
----------------------------------------------------------------------------------------------------------------------*/
parameter       S_IDLE          = 3'b001 ;
parameter       S_WAIT          = 3'b010 ;
parameter       S_TRANSFERE     = 3'b100 ;


/*==================================================================================================================
System inputs.
==================================================================================================================*/
// PCI side clock and reset
input   clk_in,
        reset_in ;


/*==================================================================================================================
PCI interface signals - bidirectional signals are divided to inputs and outputs in I/O cells instantiation
module. Enables are separate signals.
==================================================================================================================*/
// master inputs
input   pci_frame_in,
        pci_irdy_in,
        pci_idsel_in ;
input   pci_frame_reg_in,
        pci_irdy_reg_in,
        pci_idsel_reg_in ;

// target response outputs
output  pci_trdy_out,
        pci_stop_out,
        pci_devsel_out ;
output  pci_trdy_en_out,
        pci_stop_en_out,
        pci_devsel_en_out ;
output  ad_load_out ;
output  ad_load_on_transfer_out ;
// address, data, bus command, byte enable in/outs
input   [31:0]  pci_ad_reg_in ;
output  [31:0]  pci_ad_out ;
output          pci_ad_en_out ;
input   [3:0]   pci_cbe_reg_in ;
input   [3:0]   pci_cbe_in ;
input           bckp_trdy_en_in ;
input           bckp_devsel_in ;
input           bckp_trdy_in ;
input           bckp_stop_in ;
input           pci_trdy_reg_in ;
input           pci_stop_reg_in ;


/*==================================================================================================================
Other side of PCI Target state machine
==================================================================================================================*/
// Data, byte enables, bus commands and address ports
output  [31:0]  address_out ;       // current request address output - registered
input           addr_claim_in ;     // current request address claim input
output  [3:0]   bc_out ;            // current request bus command output - registered
output          bc0_out ;           // current cycle RW signal output
input   [31:0]  data_in ;           // for read operations - current dataphase data input
output  [31:0]  data_out ;          // for write operations - current request data output - registered
output   [3:0]  be_out ;            // current dataphase byte enable outputs - registered
output   [3:0]  next_be_out ;       // next dataphase byte enable outputs - NOT registered
// Port connection control signals from PCI FSM
output          req_out ;           // Read is requested to WB master
output          rdy_out ;           // DATA / ADDRESS selection when read or write - registered
output          addr_phase_out ;    // Indicates address phase and also fast-back-to-back address phase - registered
output			bckp_devsel_out ;	// DEVSEL output (which is registered) equivalent
output          bckp_trdy_out ;     // TRDY output (which is registered) equivalent
output			bckp_stop_out ;		// STOP output (which is registered) equivalent
output          last_reg_out ;      // Indicates last data phase - registered
output          frame_reg_out ;     // FRAME output signal - registered
output          fetch_pcir_fifo_out ;// Read enable for PCIR_FIFO when when read is finishen on WB side
output          load_medium_reg_out ;// Load data from PCIR_FIFO to medium register (first data must be prepared on time)
output          sel_fifo_mreg_out ; // Read data selection between PCIR_FIFO and medium register
output          sel_conf_fifo_out ; // Read data selection between Configuration registers and "FIFO"
output          load_to_pciw_fifo_out ;// Write enable to PCIW_FIFO
output          load_to_conf_out ;  // Write enable to Configuration space registers


/*==================================================================================================================
Status
==================================================================================================================*/
input           same_read_in ;              // Indicates the same read request (important when read is finished on WB side)
input           norm_access_to_config_in ;  // Indicates the access to Configuration space with MEMORY commands
input           read_completed_in ;         // Indicates that read request is completed on WB side
input           read_processing_in ;        // Indicates that read request is processing on WB side
input           target_abort_in ;           // Indicates target abort termination
input           disconect_wo_data_in ;      // Indicates disconnect without data termination
input			disconect_w_data_in ;		// Indicates disconnect with data termination
input           pciw_fifo_full_in ;         // Indicates that write PCIW_FIFO is full
input           pcir_fifo_data_err_in ;     // Indicates data error on current data read from PCIR_FIFO
input           wbw_fifo_empty_in ;         // Indicates that WB SLAVE UNIT has no data to be written to PCI bus
input			wbu_del_read_comp_pending_in ; // Indicates that WB SÈAVE UNIT has a delayed read pending
input           wbu_frame_en_in ;           // Indicates that WB SLAVE UNIT is accessing the PCI bus (important if
                                            //   address on PCI bus is also claimed by decoder in this PCI TARGET UNIT
output          target_abort_set_out ;      // Signal used to be set in configuration space registers

/*==================================================================================================================
END of input / output PORT DEFINITONS !!!
==================================================================================================================*/

// Delayed frame signal for determining the address phase
reg             previous_frame ;
// Delayed read completed signal for preparing the data from pcir fifo
reg             read_completed_reg ;
// Delayed disconnect with/without data for stop loading data to PCIW_FIFO
//reg             disconect_wo_data_reg ;

wire config_disconnect ;
wire disconect_wo_data = disconect_wo_data_in || config_disconnect ;
wire disconect_w_data = disconect_w_data_in ;
// Delayed frame signal for determining the address phase!
always@(posedge clk_in or posedge reset_in)
begin
    if (reset_in)
    begin
        previous_frame <= #`FF_DELAY 1'b0 ;
        read_completed_reg <= #`FF_DELAY 1'b0 ;
    end
    else
    begin
        previous_frame <= #`FF_DELAY pci_frame_reg_in ;
        read_completed_reg <= #`FF_DELAY read_completed_in ;
    end
end

// Address phase is when previous frame was 1 and this frame is 0 and frame isn't generated from pci master (in WBU)
wire    addr_phase = (previous_frame && ~pci_frame_reg_in && ~wbu_frame_en_in) ;

`ifdef      HOST
    `ifdef  NO_CNF_IMAGE
            // Wire tells when there is configuration (read or write) command with IDSEL signal active
            wire    config_access = 1'b0 ;
            // Write and read progresses are used for determining next state
            wire    write_progress  =   ( (read_completed_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in) ||
                                          (~read_processing_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in) ) ;
            wire    read_progress   =   ( (read_completed_in && wbw_fifo_empty_in) ) ;
    `else
            // Wire tells when there is configuration (read or write) command with IDSEL signal active
            wire    config_access = (pci_idsel_reg_in && pci_cbe_reg_in[3]) && (~pci_cbe_reg_in[2] && pci_cbe_reg_in[1]) &&     // idsel asserted with correct bus command(101x)
                                    (pci_ad_reg_in[1:0] == 2'b00) ;        // has to be type 0 configuration cycle
    
            // Write and read progresses are used for determining next state
            wire    write_progress  =   ( (norm_access_to_config_in) || 
            							  (read_completed_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in) ||
                                          (~read_processing_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in) ) ;
            wire    read_progress   =   ( (~read_completed_in && norm_access_to_config_in) ||
                                          (read_completed_in && wbw_fifo_empty_in) ) ;
    `endif
`else
            // Wire tells when there is configuration (read or write) command with IDSEL signal active
            wire    config_access = (pci_idsel_reg_in && pci_cbe_reg_in[3]) && (~pci_cbe_reg_in[2] && pci_cbe_reg_in[1]) &&     // idsel asserted with correct bus command(101x)
                                    (pci_ad_reg_in[1:0] == 2'b00) ;        // has to be type 0 configuration cycle

            // Write and read progresses are used for determining next state
            wire    write_progress  =   ( (norm_access_to_config_in) || 
            							  (read_completed_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in) ||
                                          (~read_processing_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in) ) ;
            wire    read_progress   =   ( (~read_completed_in && norm_access_to_config_in) ||
                                          (read_completed_in && wbw_fifo_empty_in) ) ;
`endif

// Signal for loading data to medium register from pcir fifo when read completed from WB side!
wire    prepare_rd_fifo_data = (read_completed_in && ~read_completed_reg) ;

// Write allowed to PCIW_FIFO
wire    write_to_fifo   =   ((read_completed_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in) || 
							 (~read_processing_in && ~pciw_fifo_full_in && ~wbu_del_read_comp_pending_in)) ;
// Read allowed from PCIR_FIFO
wire    read_from_fifo  =   (read_completed_in && wbw_fifo_empty_in) ;
`ifdef      HOST
    `ifdef  NO_CNF_IMAGE
            // Read request is allowed to be proceed regarding the WB side
            wire    read_request    =   (~read_completed_in && ~read_processing_in) ;
    `else
            // Read request is allowed to be proceed regarding the WB side
            wire    read_request    =   (~read_completed_in && ~read_processing_in && ~norm_access_to_config_in) ;
    `endif
`else
            // Read request is allowed to be proceed regarding the WB side
            wire    read_request    =   (~read_completed_in && ~read_processing_in && ~norm_access_to_config_in) ;
`endif

// Critically calculated signals are latched in this clock period (address phase) to be used in the next clock period
reg             rw_cbe0 ;
reg             wr_progress ;
reg             rd_progress ;
reg             rd_from_fifo ;
reg             rd_request ;
reg             wr_to_fifo ;
reg             same_read_reg ;

always@(posedge clk_in or posedge reset_in)
begin
    if (reset_in)
    begin
        rw_cbe0                         <= #`FF_DELAY 1'b0 ;
        wr_progress                     <= #`FF_DELAY 1'b0 ;
        rd_progress                     <= #`FF_DELAY 1'b0 ;
        rd_from_fifo                    <= #`FF_DELAY 1'b0 ;
        rd_request                      <= #`FF_DELAY 1'b0 ;
        wr_to_fifo                      <= #`FF_DELAY 1'b0 ;
        same_read_reg                   <= #`FF_DELAY 1'b0 ;
    end
    else
    begin
        if (addr_phase)
        begin
            rw_cbe0                     <= #`FF_DELAY pci_cbe_reg_in[0] ;
            wr_progress                 <= #`FF_DELAY write_progress ;
            rd_progress                 <= #`FF_DELAY read_progress ;
            rd_from_fifo                <= #`FF_DELAY read_from_fifo ;
            rd_request                  <= #`FF_DELAY read_request ;
            wr_to_fifo                  <= #`FF_DELAY write_to_fifo ;
            same_read_reg               <= #`FF_DELAY same_read_in ;
        end
    end
end

`ifdef      HOST
    `ifdef  NO_CNF_IMAGE
            wire    norm_access_to_conf_reg     = 1'b0 ;
            wire    cnf_progress                = 1'b0 ;
    `else
            reg     norm_access_to_conf_reg ;
            reg     cnf_progress ;
            always@(posedge clk_in or posedge reset_in)
            begin
                if (reset_in)
                begin
                    norm_access_to_conf_reg     <= #`FF_DELAY 1'b0 ;
                    cnf_progress                <= #`FF_DELAY 1'b0 ;
                end
                else
                begin
                    if (addr_phase)
                    begin
                        norm_access_to_conf_reg <= #`FF_DELAY norm_access_to_config_in ;
                        cnf_progress            <= #`FF_DELAY config_access ;
                    end
                end
            end
    `endif
`else
            reg     norm_access_to_conf_reg ;
            reg     cnf_progress ;
            always@(posedge clk_in or posedge reset_in)
            begin
                if (reset_in)
                begin
                    norm_access_to_conf_reg     <= #`FF_DELAY 1'b0 ;
                    cnf_progress                <= #`FF_DELAY 1'b0 ;
                end
                else
                begin
                    if (addr_phase)
                    begin
                        norm_access_to_conf_reg <= #`FF_DELAY norm_access_to_config_in ;
                        cnf_progress            <= #`FF_DELAY config_access ;
                    end
                end
            end
`endif

// Signal used in S_WAIT state to determin next state
wire s_wait_progress =  (
                        (~cnf_progress && rw_cbe0 && wr_progress && ~target_abort_in) ||
                        (~cnf_progress && ~rw_cbe0 && same_read_reg && rd_progress && ~target_abort_in && ~pcir_fifo_data_err_in) ||
                        (~cnf_progress && ~rw_cbe0 && ~same_read_reg && norm_access_to_conf_reg && ~target_abort_in) ||
                        (cnf_progress && ~target_abort_in)
                        ) ;

// Signal used in S_TRANSFERE state to determin next state
wire s_tran_progress =  (
                        (rw_cbe0 && !disconect_wo_data) ||
                        (~rw_cbe0 && !disconect_wo_data && !target_abort_in && !pcir_fifo_data_err_in)
                        ) ;

// Clock enable for PCI state machine driven directly from critical inputs - FRAME and IRDY
wire            pcit_sm_clk_en ;
// FSM states signals indicating the current state
reg             state_idle ;
reg             state_wait ;
reg             sm_transfere ;
reg             backoff ;
reg             state_default ;
wire            state_backoff   = sm_transfere && backoff ;
wire            state_transfere = sm_transfere && !backoff ;

always@(posedge clk_in or posedge reset_in)
begin
    if ( reset_in )
        backoff <= #`FF_DELAY 1'b0 ;
    else if ( state_idle )
        backoff <= #`FF_DELAY 1'b0 ;
    else
        backoff <= #`FF_DELAY (state_wait && !s_wait_progress) ||
                              (sm_transfere && !s_tran_progress && !pci_frame_in && !pci_irdy_in) ||
                              backoff ;
end
assign config_disconnect = sm_transfere && (norm_access_to_conf_reg || cnf_progress) ;

// Clock enable module used for preserving the architecture because of minimum delay for critical inputs
pci_target32_clk_en pci_target_clock_en
(
    .addr_phase             (addr_phase),
    .config_access          (config_access),
    .addr_claim_in          (addr_claim_in),
    .pci_frame_in           (pci_frame_in),
    .state_wait             (state_wait),
    .state_transfere        (sm_transfere),
    .state_default          (state_default),
    .clk_enable             (pcit_sm_clk_en)
);

reg [2:0]  c_state ; //current state register
reg [2:0]  n_state ; //next state input to current state register

// state machine register control
always@(posedge clk_in or posedge reset_in)
begin
    if (reset_in) // reset state machine to S_IDLE state
        c_state <= #`FF_DELAY S_IDLE ;
    else
        if (pcit_sm_clk_en) // if conditions are true, then FSM goes to next state!
            c_state <= #`FF_DELAY n_state ;
end

// state machine logic
always@(c_state)
begin
    case (c_state)
    S_IDLE :
    begin
        state_idle      <= 1'b1 ;
        state_wait      <= 1'b0 ;
        sm_transfere <= 1'b0 ;
        state_default   <= 1'b0 ;
        n_state <= S_WAIT ;
    end
    S_WAIT :
    begin
        state_idle      <= 1'b0 ;
        state_wait      <= 1'b1 ;
        sm_transfere <= 1'b0 ;
        state_default   <= 1'b0 ;
        n_state <= S_TRANSFERE ;
    end
    S_TRANSFERE :
    begin
        state_idle      <= 1'b0 ;
        state_wait      <= 1'b0 ;
        sm_transfere <= 1'b1 ;
        state_default   <= 1'b0 ;
        n_state <= S_IDLE ;
    end
    default :
    begin
        state_idle      <= 1'b0 ;
        state_wait      <= 1'b0 ;
        sm_transfere <= 1'b0 ;
        state_default   <= 1'b1 ;
        n_state <= S_IDLE ;
    end
    endcase
end

        // if not retry and not target abort
        // NO CRITICAL SIGNALS
wire    trdy_w          =   (
        (state_wait && ~cnf_progress && rw_cbe0 && wr_progress && ~target_abort_in) ||
        (state_wait && ~cnf_progress && ~rw_cbe0 && same_read_reg && rd_progress && ~target_abort_in && !pcir_fifo_data_err_in) ||
        (state_wait && ~cnf_progress && ~rw_cbe0 && ~same_read_reg && norm_access_to_conf_reg && ~target_abort_in) ||
        (state_wait && cnf_progress && ~target_abort_in)
                            ) ;
        // if not disconnect without data and not target abort (only during reads)
        // MUST BE ANDED WITH CRITICAL ~FRAME
wire    trdy_w_frm      =   (
        (state_transfere && !cnf_progress && !norm_access_to_conf_reg && rw_cbe0 && !disconect_wo_data) ||
        (state_transfere && !cnf_progress && !norm_access_to_conf_reg && ~rw_cbe0 && !disconect_wo_data && ~pcir_fifo_data_err_in) ||
        (state_transfere && !cnf_progress && !norm_access_to_conf_reg && disconect_w_data && pci_irdy_reg_in && 
                                                                         ((~rw_cbe0 && ~pcir_fifo_data_err_in) || rw_cbe0))
                            ) ;
        // if not disconnect without data and not target abort (only during reads)
        // MUST BE ANDED WITH CRITICAL ~FRAME AND IRDY
wire    trdy_w_frm_irdy =   ( ~bckp_trdy_in ) ;
// TRDY critical module used for preserving the architecture because of minimum delay for critical inputs
pci_target32_trdy_crit pci_target_trdy_critical
(
    .trdy_w                 (trdy_w),
    .trdy_w_frm             (trdy_w_frm),
    .trdy_w_frm_irdy        (trdy_w_frm_irdy),
    .pci_frame_in           (pci_frame_in),
    .pci_irdy_in            (pci_irdy_in),
    .pci_trdy_out           (pci_trdy_out)
);

        // if target abort or retry
        // NO CRITICAL SIGNALS
wire    stop_w          =   (
        (state_wait && target_abort_in) ||
        (state_wait && ~cnf_progress && rw_cbe0 && ~wr_progress) ||
        (state_wait && ~cnf_progress && ~rw_cbe0 && same_read_reg && ~rd_progress) ||
        (state_wait && ~cnf_progress && ~rw_cbe0 && same_read_reg && rd_progress && pcir_fifo_data_err_in) ||
        (state_wait && ~cnf_progress && ~rw_cbe0 && ~same_read_reg && ~norm_access_to_conf_reg)
                            ) ;
        // if asserted, wait for deactivating the frame
        // MUST BE ANDED WITH CRITICAL ~FRAME
wire    stop_w_frm      =   (
        (state_backoff && ~bckp_stop_in)
                            ) ;
        // if target abort or if disconnect without data (after data transfere)
        // MUST BE ANDED WITH CRITICAL ~FRAME AND ~IRDY
wire    stop_w_frm_irdy =   (
        (state_transfere && (disconect_wo_data)) ||
        (state_transfere && ~rw_cbe0 && pcir_fifo_data_err_in)
                            ) ;
// STOP critical module used for preserving the architecture because of minimum delay for critical inputs
pci_target32_stop_crit pci_target_stop_critical
(
    .stop_w                 (stop_w),
    .stop_w_frm             (stop_w_frm),
    .stop_w_frm_irdy        (stop_w_frm_irdy),
    .pci_frame_in           (pci_frame_in),
    .pci_irdy_in            (pci_irdy_in),
    .pci_stop_out           (pci_stop_out)
);

        // if OK to respond and not target abort
        // NO CRITICAL SIGNALS
wire    devs_w          =   (
        (addr_phase && config_access) ||
        (addr_phase && ~config_access && addr_claim_in) ||
        (state_wait && ~target_abort_in && !(~cnf_progress && ~rw_cbe0 && same_read_reg && rd_progress && pcir_fifo_data_err_in) )
                            ) ;

        // if not target abort (only during reads) or if asserted, wait for deactivating the frame
        // MUST BE ANDED WITH CRITICAL ~FRAME
wire    devs_w_frm      =   (
        (state_transfere && rw_cbe0) ||
        (state_transfere && ~rw_cbe0 && ~pcir_fifo_data_err_in) ||
        (state_backoff && ~bckp_devsel_in)
                            ) ;
        // if not target abort (only during reads)
        // MUST BE ANDED WITH CRITICAL ~FRAME AND IRDY
wire    devs_w_frm_irdy =   (
        (state_transfere && ~rw_cbe0 && pcir_fifo_data_err_in)
                            ) ;
// DEVSEL critical module used for preserving the architecture because of minimum delay for critical inputs
pci_target32_devs_crit pci_target_devsel_critical
(
    .devs_w                 (devs_w),
    .devs_w_frm             (devs_w_frm),
    .devs_w_frm_irdy        (devs_w_frm_irdy),
    .pci_frame_in           (pci_frame_in),
    .pci_irdy_in            (pci_irdy_in),
    .pci_devsel_out         (pci_devsel_out)
);

// signal used in AD enable module with preserving the hierarchy because of minimum delay for critical inputs
assign	pci_ad_en_out =    (
        (addr_phase && config_access && ~pci_cbe_reg_in[0]) ||
        (addr_phase && ~config_access && addr_claim_in && ~pci_cbe_reg_in[0]) ||
        (state_wait && ~rw_cbe0) || 
        (state_transfere && ~rw_cbe0) ||
        (state_backoff && ~rw_cbe0 && ~pci_frame_reg_in)
                            ) ;

wire fast_back_to_back  =   (addr_phase && ~pci_irdy_reg_in) ;

        // if cycle will progress or will not be stopped
        // NO CRITICAL SIGNALS
wire    ctrl_en       =
        /*(~wbu_frame_en_in && fast_back_to_back) ||*/
        (addr_phase && config_access) ||
        (addr_phase && ~config_access && addr_claim_in) ||
        (state_wait) ||
        (state_transfere && ~(pci_frame_reg_in && ~pci_irdy_reg_in && (~pci_stop_reg_in || ~pci_trdy_reg_in))) ||
        (state_backoff && ~(pci_frame_reg_in && ~pci_irdy_reg_in && (~pci_stop_reg_in || ~pci_trdy_reg_in))) ;

assign pci_trdy_en_out   = ctrl_en ;
assign pci_stop_en_out   = ctrl_en ;
assign pci_devsel_en_out = ctrl_en ;

// target ready output signal delayed for one clock used in conjunction with irdy_reg to select which
//   data are registered in io mux module - from fifo or medoum register
reg             bckp_trdy_reg ;
// delayed indicators for states transfere and backoff
reg             state_transfere_reg ;
reg             state_backoff_reg ;
always@(posedge clk_in or posedge reset_in)
begin
    if (reset_in)
    begin
        bckp_trdy_reg <= #`FF_DELAY 1'b1 ;
        state_transfere_reg <= #`FF_DELAY 1'b0 ;
        state_backoff_reg <= #`FF_DELAY 1'b0 ;
    end
    else
    begin
        bckp_trdy_reg <= #`FF_DELAY bckp_trdy_in ;
        state_transfere_reg <= #`FF_DELAY state_transfere ;
        state_backoff_reg <= #`FF_DELAY state_backoff ;
    end
end

// Read control signals assignments
assign
    fetch_pcir_fifo_out =   (
        (prepare_rd_fifo_data) ||
        (state_wait && ~cnf_progress && ~rw_cbe0 && same_read_reg && rd_from_fifo && ~target_abort_in) ||
        (bckp_trdy_en_in && ~pci_trdy_reg_in && ~cnf_progress && ~rw_cbe0 && same_read_reg && rd_from_fifo && ~pci_irdy_reg_in)
                            ) ;

assign  ad_load_out         =   (state_wait) ;

assign  ad_load_on_transfer_out = (bckp_trdy_en_in && ~rw_cbe0) ;

assign 	load_medium_reg_out =   (
        (prepare_rd_fifo_data) ||
        (state_wait && ~rw_cbe0 && ~cnf_progress && same_read_reg && rd_from_fifo && ~target_abort_in) || 
        (~pci_irdy_reg_in && ~rw_cbe0 && ~cnf_progress && same_read_reg && rd_from_fifo && ~pci_trdy_reg_in && bckp_trdy_en_in)
                                ) ;

assign  sel_fifo_mreg_out = (~pci_irdy_reg_in && ~bckp_trdy_reg) ;

`ifdef      HOST
    `ifdef  NO_CNF_IMAGE
            assign  sel_conf_fifo_out = 1'b0 ;
    `else
            assign  sel_conf_fifo_out = (cnf_progress || norm_access_to_conf_reg) ;
    `endif
`else
            assign  sel_conf_fifo_out = (cnf_progress || norm_access_to_conf_reg) ;
`endif

// Write control signals assignments
assign
    load_to_pciw_fifo_out = (
        (state_wait && (~cnf_progress && ~norm_access_to_conf_reg) && rw_cbe0 && wr_to_fifo && ~target_abort_in) ||
        (state_transfere_reg && ~state_backoff && rw_cbe0 && wr_to_fifo /*&& ~disconect_wo_data_reg*/ && ~pci_irdy_reg_in && ~bckp_trdy_reg && (~cnf_progress && ~norm_access_to_conf_reg)) ||
        ((state_backoff || state_backoff_reg) && rw_cbe0 && wr_to_fifo && ~pci_irdy_reg_in && ~bckp_trdy_reg && (~cnf_progress && ~norm_access_to_conf_reg))
                            ) ;

`ifdef      HOST
    `ifdef  NO_CNF_IMAGE
            assign  load_to_conf_out =  1'b0 ;
    `else
            assign  load_to_conf_out =  (
            (state_transfere_reg && cnf_progress && rw_cbe0 && ~pci_irdy_reg_in && ~bckp_trdy_reg) ||
            (state_transfere_reg && norm_access_to_conf_reg && rw_cbe0 && ~pci_irdy_reg_in && ~bckp_trdy_reg)
                                        ) ;
    `endif
`else
            assign  load_to_conf_out =  (
            (state_transfere_reg && cnf_progress && rw_cbe0 && ~pci_irdy_reg_in && ~bckp_trdy_reg) ||
            (state_transfere_reg && norm_access_to_conf_reg && rw_cbe0 && ~pci_irdy_reg_in && ~bckp_trdy_reg)
                                        ) ;
`endif

// General control sigal assignments
assign  addr_phase_out = addr_phase ;
assign  last_reg_out = (pci_frame_reg_in && ~pci_irdy_reg_in) ;
assign  frame_reg_out = pci_frame_reg_in ;
assign	bckp_devsel_out = bckp_devsel_in ;
assign  bckp_trdy_out   = bckp_trdy_in ;
assign	bckp_stop_out	= bckp_stop_in ;
assign  target_abort_set_out = (bckp_devsel_in && bckp_trdy_in && ~bckp_stop_in && bckp_trdy_en_in) ;
// request signal for delayed sinc. module
reg master_will_request_read ;
always@(posedge clk_in or posedge reset_in)
begin
    if ( reset_in )
        master_will_request_read <= #`FF_DELAY 1'b0 ;
    else
        master_will_request_read <= #`FF_DELAY ((state_wait && ~target_abort_in) || (state_backoff && ~target_abort_set_out)) && ~cnf_progress && ~norm_access_to_conf_reg && ~rw_cbe0 && rd_request ;
end
// MORE OPTIMIZED READS, but not easy to control in a testbench!
//assign  req_out = master_will_request_read ; 
assign req_out = master_will_request_read && !pci_irdy_reg_in && !read_processing_in ;

// ready tells when address or data are written into fifo - RDY ? DATA : ADDRESS
assign  rdy_out = ~bckp_trdy_reg ;

// data and address outputs assignments!
assign  pci_ad_out = data_in ;

assign  data_out = pci_ad_reg_in ;
assign  be_out = pci_cbe_reg_in ;
assign  next_be_out = pci_cbe_in ;
assign  address_out = pci_ad_reg_in ;
assign  bc_out = pci_cbe_reg_in ;
assign  bc0_out = rw_cbe0 ;


endmodule
