//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "wb_slave_unit.v"                                 ////
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
// $Log: pci_wb_slave_unit.v,v $
// Revision 1.3  2004/01/24 11:54:18  mihad
// Update! SPOCI Implemented!
//
// Revision 1.2  2003/10/17 09:11:52  markom
// mbist signals updated according to newest convention
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.8  2002/10/18 03:36:37  tadejm
// Changed wrong signal name mbist_sen into mbist_ctrl_i.
//
// Revision 1.7  2002/10/17 22:49:22  tadejm
// Changed BIST signals for RAMs.
//
// Revision 1.6  2002/10/11 10:09:01  mihad
// Added additional testcase and changed rst name in BIST to trst
//
// Revision 1.5  2002/10/08 17:17:06  mihad
// Added BIST signals for RAMs.
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
// Revision 1.1.1.1  2001/10/02 15:33:46  mihad
// New project directory structure
//
//

// Module instantiates and connects other modules lower in hierarcy
// Wishbone slave unit consists of modules that together form datapath
// between external WISHBONE masters and external PCI targets
`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_wb_slave_unit
(
    reset_in,
    wb_clock_in,
    pci_clock_in,
    ADDR_I,
    SDATA_I,
    SDATA_O,
    CYC_I,
    STB_I,
    WE_I,
    SEL_I,
    ACK_O,
    RTY_O,
    ERR_O,
    CAB_I,
    wbu_map_in,
    wbu_pref_en_in,
    wbu_mrl_en_in,
    wbu_pci_drcomp_pending_in,
    wbu_conf_data_in,
    wbu_pciw_empty_in,
    wbu_bar0_in,
    wbu_bar1_in,
    wbu_bar2_in,
    wbu_bar3_in,
    wbu_bar4_in,
    wbu_bar5_in,
    wbu_am0_in,
    wbu_am1_in,
    wbu_am2_in,
    wbu_am3_in,
    wbu_am4_in,
    wbu_am5_in,
    wbu_ta0_in,
    wbu_ta1_in,
    wbu_ta2_in,
    wbu_ta3_in,
    wbu_ta4_in,
    wbu_ta5_in,
    wbu_at_en_in,
    wbu_ccyc_addr_in ,
    wbu_master_enable_in,
    wb_init_complete_in,
    wbu_cache_line_size_not_zero,
    wbu_cache_line_size_in,
    wbu_pciif_gnt_in,
    wbu_pciif_frame_in,
    wbu_pciif_irdy_in,
    wbu_pciif_trdy_in,
    wbu_pciif_trdy_reg_in,
    wbu_pciif_stop_in,
    wbu_pciif_stop_reg_in,
    wbu_pciif_devsel_in,
    wbu_pciif_devsel_reg_in,
    wbu_pciif_ad_reg_in,
    wbu_pciif_req_out,
    wbu_pciif_frame_out,
    wbu_pciif_frame_en_out,
    wbu_pciif_frame_en_in,
    wbu_pciif_frame_out_in,
    wbu_pciif_frame_load_out,
    wbu_pciif_irdy_out,
    wbu_pciif_irdy_en_out,
    wbu_pciif_ad_out,
    wbu_pciif_ad_en_out,
    wbu_pciif_cbe_out,
    wbu_pciif_cbe_en_out,
    wbu_err_addr_out,
    wbu_err_bc_out,
    wbu_err_signal_out,
    wbu_err_source_out,
    wbu_err_rty_exp_out,
    wbu_tabort_rec_out,
    wbu_mabort_rec_out,
    wbu_conf_offset_out,
    wbu_conf_renable_out,
    wbu_conf_wenable_out,
    wbu_conf_be_out,
    wbu_conf_data_out,
    wbu_del_read_comp_pending_out,
    wbu_wbw_fifo_empty_out,
    wbu_latency_tim_val_in,
    wbu_ad_load_out,
    wbu_ad_load_on_transfer_out

`ifdef PCI_BIST
    ,
    // debug chain signals
    mbist_si_i,       // bist scan serial in
    mbist_so_o,       // bist scan serial out
    mbist_ctrl_i        // bist chain shift control
`endif
);

input reset_in,
      wb_clock_in,
      pci_clock_in ;

input   [31:0]  ADDR_I   ;
input   [31:0]  SDATA_I  ;
output  [31:0]  SDATA_O  ;
input           CYC_I    ;
input           STB_I    ;
input           WE_I     ;
input   [3:0]   SEL_I    ;
output          ACK_O    ;
output          RTY_O    ;
output          ERR_O    ;
input           CAB_I    ;

input   [5:0]   wbu_map_in ;
input   [5:0]   wbu_pref_en_in ;
input   [5:0]   wbu_mrl_en_in ;

input           wbu_pci_drcomp_pending_in ;

input   [31:0]  wbu_conf_data_in ;

input           wbu_pciw_empty_in ;

input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar0_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar1_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar2_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar3_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar4_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar5_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am0_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am1_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am2_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am3_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am4_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am5_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta0_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta1_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta2_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta3_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta4_in ;
input   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta5_in ;
input   [5:0]                               wbu_at_en_in ;

input   [23:0]  wbu_ccyc_addr_in ;

input           wbu_master_enable_in    ;
input           wb_init_complete_in     ;

input			wbu_cache_line_size_not_zero ;
input   [7:0]   wbu_cache_line_size_in ;

input           wbu_pciif_gnt_in ;
input           wbu_pciif_frame_in ;
input           wbu_pciif_frame_en_in ;
input           wbu_pciif_irdy_in ;
input           wbu_pciif_trdy_in;
input           wbu_pciif_trdy_reg_in;
input           wbu_pciif_stop_in ;
input           wbu_pciif_stop_reg_in ;
input           wbu_pciif_devsel_in ;
input           wbu_pciif_devsel_reg_in ;
input [31:0]    wbu_pciif_ad_reg_in ;

output          wbu_pciif_req_out ;
output          wbu_pciif_frame_out ;
output          wbu_pciif_frame_en_out ;
input           wbu_pciif_frame_out_in ;
output          wbu_pciif_frame_load_out ;
output          wbu_pciif_irdy_out ;
output          wbu_pciif_irdy_en_out ;
output  [31:0]  wbu_pciif_ad_out ;
output          wbu_pciif_ad_en_out ;
output  [3:0]   wbu_pciif_cbe_out ;
output          wbu_pciif_cbe_en_out ;

output  [31:0]  wbu_err_addr_out ;
output  [3:0]   wbu_err_bc_out ;
output          wbu_err_signal_out ;
output          wbu_err_source_out ;
output          wbu_err_rty_exp_out ;
output          wbu_tabort_rec_out ;
output          wbu_mabort_rec_out ;

output  [11:0]  wbu_conf_offset_out ;
output          wbu_conf_renable_out ;
output          wbu_conf_wenable_out ;
output  [3:0]   wbu_conf_be_out ;
output  [31:0]  wbu_conf_data_out ;

output          wbu_del_read_comp_pending_out ;
output          wbu_wbw_fifo_empty_out ;

input   [7:0]   wbu_latency_tim_val_in ;

output          wbu_ad_load_out ;
output          wbu_ad_load_on_transfer_out ;

`ifdef PCI_BIST
/*-----------------------------------------------------
BIST debug chain port signals
-----------------------------------------------------*/
input   mbist_si_i;       // bist scan serial in
output  mbist_so_o;       // bist scan serial out
input [`PCI_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i;       // bist chain shift control
`endif

// pci master interface outputs
wire [31:0] pcim_if_address_out ;
wire [3:0]  pcim_if_bc_out ;
wire [31:0] pcim_if_data_out ;
wire [3:0]  pcim_if_be_out ;
wire        pcim_if_req_out ;
wire        pcim_if_rdy_out ;
wire        pcim_if_last_out ;
wire        pcim_if_wbw_renable_out ;
wire        pcim_if_wbr_wenable_out ;
wire [31:0] pcim_if_wbr_data_out ;
wire [3:0]  pcim_if_wbr_be_out ;
wire [3:0]  pcim_if_wbr_control_out ;
wire        pcim_if_del_complete_out ;
wire        pcim_if_del_error_out ;
wire        pcim_if_del_rty_exp_out ;
wire [31:0] pcim_if_err_addr_out ;
wire [3:0]  pcim_if_err_bc_out ;
wire        pcim_if_err_signal_out ;
wire        pcim_if_err_source_out ;
wire        pcim_if_err_rty_exp_out ;
wire        pcim_if_tabort_out ;
wire        pcim_if_mabort_out ;
wire [31:0] pcim_if_next_data_out ;
wire [3:0]  pcim_if_next_be_out ;
wire        pcim_if_next_last_out ;
wire        pcim_if_posted_write_not_present_out ;



wire        pcim_sm_req_out ;
wire        pcim_sm_frame_out ;
wire        pcim_sm_frame_en_out ;
wire        pcim_sm_irdy_out ;
wire        pcim_sm_irdy_en_out ;
wire [31:0] pcim_sm_ad_out ;
wire        pcim_sm_ad_en_out ;
wire [3:0]  pcim_sm_cbe_out ;
wire        pcim_sm_cbe_en_out ;
wire        pcim_sm_ad_load_out ;
wire        pcim_sm_ad_load_on_transfer_out ;

wire        pcim_sm_wait_out ;
wire        pcim_sm_wtransfer_out ;
wire        pcim_sm_rtransfer_out ;
wire        pcim_sm_retry_out ;
wire        pcim_sm_rerror_out ;
wire        pcim_sm_first_out ;
wire        pcim_sm_mabort_out ;
wire        pcim_sm_frame_load_out ;

assign wbu_pciif_frame_load_out = pcim_sm_frame_load_out ;

assign wbu_err_addr_out     =   pcim_if_err_addr_out ;
assign wbu_err_bc_out       =   pcim_if_err_bc_out ;
assign wbu_err_signal_out   =   pcim_if_err_signal_out ;
assign wbu_err_source_out   =   pcim_if_err_source_out ;
assign wbu_err_rty_exp_out  =   pcim_if_err_rty_exp_out ;
assign wbu_tabort_rec_out   =   pcim_if_tabort_out ;
assign wbu_mabort_rec_out   =   pcim_if_mabort_out ;

assign wbu_wbw_fifo_empty_out = pcim_if_posted_write_not_present_out ;

// pci master state machine outputs
// pci interface signals
assign  wbu_pciif_req_out           =           pcim_sm_req_out ;
assign  wbu_pciif_frame_out         =           pcim_sm_frame_out ;
assign  wbu_pciif_frame_en_out      =           pcim_sm_frame_en_out ;
assign  wbu_pciif_irdy_out          =           pcim_sm_irdy_out ;
assign  wbu_pciif_irdy_en_out       =           pcim_sm_irdy_en_out ;
assign  wbu_pciif_ad_out            =           pcim_sm_ad_out ;
assign  wbu_pciif_ad_en_out         =           pcim_sm_ad_en_out ;
assign  wbu_pciif_cbe_out           =           pcim_sm_cbe_out ;
assign  wbu_pciif_cbe_en_out        =           pcim_sm_cbe_en_out ;
assign  wbu_ad_load_out             =           pcim_sm_ad_load_out ;
assign  wbu_ad_load_on_transfer_out =           pcim_sm_ad_load_on_transfer_out ;

// signals to internal of the core
wire [31:0] pcim_sm_data_out ;

// wishbone slave state machine outputs
wire [3:0]  wbs_sm_del_bc_out ;
wire        wbs_sm_del_req_out ;
wire        wbs_sm_del_done_out ;
wire        wbs_sm_del_burst_out ;
wire        wbs_sm_del_write_out ;
wire [11:0] wbs_sm_conf_offset_out ;
wire        wbs_sm_conf_renable_out ;
wire        wbs_sm_conf_wenable_out ;
wire [3:0]  wbs_sm_conf_be_out ;
wire [31:0] wbs_sm_conf_data_out ;
wire [31:0] wbs_sm_data_out ;
wire [3:0]  wbs_sm_cbe_out ;
wire        wbs_sm_wbw_wenable_out ;
wire [3:0]  wbs_sm_wbw_control_out ;
wire        wbs_sm_wbr_renable_out ;
wire        wbs_sm_wbr_flush_out ;
wire        wbs_sm_del_in_progress_out ;
wire [31:0] wbs_sm_sdata_out ;
wire        wbs_sm_ack_out ;
wire        wbs_sm_rty_out ;
wire        wbs_sm_err_out ;
wire        wbs_sm_sample_address_out ;

assign wbu_conf_offset_out  = wbs_sm_conf_offset_out ;
assign wbu_conf_renable_out = wbs_sm_conf_renable_out ;
assign wbu_conf_wenable_out = wbs_sm_conf_wenable_out ;
assign wbu_conf_be_out      = ~wbs_sm_conf_be_out ;
assign wbu_conf_data_out    = wbs_sm_conf_data_out ;

assign SDATA_O = wbs_sm_sdata_out ;
assign ACK_O   = wbs_sm_ack_out ;
assign RTY_O   = wbs_sm_rty_out ;
assign ERR_O   = wbs_sm_err_out ;


// wbw_wbr fifo outputs

// wbw_fifo_outputs:
wire [31:0] fifos_wbw_addr_data_out ;
wire [3:0]  fifos_wbw_cbe_out ;
wire [3:0]  fifos_wbw_control_out ;
wire        fifos_wbw_almost_full_out ;
wire        fifos_wbw_full_out ;
wire        fifos_wbw_empty_out ;
wire        fifos_wbw_transaction_ready_out ;

// wbr_fifo_outputs
wire [31:0] fifos_wbr_data_out ;
wire [3:0]  fifos_wbr_be_out ;
wire [3:0]  fifos_wbr_control_out ;
wire        fifos_wbr_empty_out ;

// address multiplexer outputs
wire [5:0]  amux_hit_out ;
wire [31:0] amux_address_out ;

// delayed transaction logic outputs
wire [31:0] del_sync_addr_out ;
wire [3:0]  del_sync_be_out ;
wire        del_sync_we_out ;
wire        del_sync_comp_req_pending_out ;
wire        del_sync_comp_comp_pending_out ;
wire        del_sync_req_req_pending_out ;
wire        del_sync_req_comp_pending_out ;
wire [3:0]  del_sync_bc_out ;
wire        del_sync_status_out ;
wire        del_sync_comp_flush_out ;
wire        del_sync_burst_out ;

assign wbu_del_read_comp_pending_out = del_sync_comp_comp_pending_out ;

// delayed write storage output
wire [31:0] del_write_data_out ;

// config. cycle address decoder output
wire [31:0] ccyc_addr_out ;


// WISHBONE slave interface inputs
wire [4:0]  wbs_sm_hit_in                   =       amux_hit_out[5:1] ;
wire        wbs_sm_conf_hit_in              =       amux_hit_out[0]   ;
wire [4:0]  wbs_sm_map_in                   =       wbu_map_in[5:1]        ;
wire [4:0]  wbs_sm_pref_en_in               =       wbu_pref_en_in[5:1]    ;
wire [4:0]  wbs_sm_mrl_en_in                =       wbu_mrl_en_in[5:1]     ;
wire [31:0] wbs_sm_addr_in                  =       amux_address_out ;
wire [3:0]  wbs_sm_del_bc_in                =       del_sync_bc_out  ;
wire        wbs_sm_del_req_pending_in       =       del_sync_req_req_pending_out ;
wire        wbs_sm_wb_del_comp_pending_in   =       del_sync_req_comp_pending_out ;
wire        wbs_sm_pci_drcomp_pending_in    =       wbu_pci_drcomp_pending_in ;
wire        wbs_sm_del_write_in             =       del_sync_we_out ;
wire        wbs_sm_del_error_in             =       del_sync_status_out ;
wire [31:0] wbs_sm_del_addr_in              =       del_sync_addr_out ;
wire [3:0]  wbs_sm_del_be_in                =       del_sync_be_out ;
wire [31:0] wbs_sm_conf_data_in             =       wbu_conf_data_in ;
wire        wbs_sm_wbw_almost_full_in       =       fifos_wbw_almost_full_out ;
wire        wbs_sm_wbw_full_in              =       fifos_wbw_full_out ;
wire [3:0]  wbs_sm_wbr_be_in                =       fifos_wbr_be_out ;
wire [31:0] wbs_sm_wbr_data_in              =       fifos_wbr_data_out ;
wire [3:0]  wbs_sm_wbr_control_in           =       fifos_wbr_control_out ;
wire        wbs_sm_wbr_empty_in             =       fifos_wbr_empty_out ;
wire        wbs_sm_pciw_empty_in            =       wbu_pciw_empty_in ;
wire        wbs_sm_lock_in                  =       ~wbu_master_enable_in ;
wire		wbs_sm_cache_line_size_not_zero	=		wbu_cache_line_size_not_zero ;
wire        wbs_sm_cyc_in                   =       CYC_I ;
wire        wbs_sm_stb_in                   =       STB_I ;
wire        wbs_sm_we_in                    =       WE_I  ;
wire [3:0]  wbs_sm_sel_in                   =       SEL_I ;
wire [31:0] wbs_sm_sdata_in                 =       SDATA_I ;
wire        wbs_sm_cab_in                   =       CAB_I ;
wire [31:0] wbs_sm_ccyc_addr_in             =       ccyc_addr_out ;
wire        wbs_sm_init_complete_in         =       wb_init_complete_in ;

// WISHBONE slave interface instantiation
pci_wb_slave wishbone_slave(
                        .wb_clock_in              (wb_clock_in) ,
                        .reset_in                 (reset_in) ,
                        .wb_hit_in                (wbs_sm_hit_in) ,
                        .wb_conf_hit_in           (wbs_sm_conf_hit_in) ,
                        .wb_map_in                (wbs_sm_map_in) ,
                        .wb_pref_en_in            (wbs_sm_pref_en_in) ,
                        .wb_mrl_en_in             (wbs_sm_mrl_en_in) ,
                        .wb_addr_in               (wbs_sm_addr_in),
                        .del_bc_in                (wbs_sm_del_bc_in),
                        .wb_del_req_pending_in    (wbs_sm_del_req_pending_in),
                        .wb_del_comp_pending_in   (wbs_sm_wb_del_comp_pending_in),
                        .pci_drcomp_pending_in    (wbs_sm_pci_drcomp_pending_in),
                        .del_bc_out               (wbs_sm_del_bc_out),
                        .del_req_out              (wbs_sm_del_req_out),
                        .del_done_out             (wbs_sm_del_done_out),
                       	.del_burst_out            (wbs_sm_del_burst_out),
                        .del_write_out            (wbs_sm_del_write_out),
                        .del_write_in             (wbs_sm_del_write_in),
                        .del_error_in             (wbs_sm_del_error_in),
                        .wb_del_addr_in           (wbs_sm_del_addr_in),
                        .wb_del_be_in             (wbs_sm_del_be_in),
                        .wb_conf_offset_out       (wbs_sm_conf_offset_out),
                        .wb_conf_renable_out      (wbs_sm_conf_renable_out),
                        .wb_conf_wenable_out      (wbs_sm_conf_wenable_out),
                        .wb_conf_be_out           (wbs_sm_conf_be_out),
                        .wb_conf_data_in          (wbs_sm_conf_data_in),
                        .wb_conf_data_out         (wbs_sm_conf_data_out),
                        .wb_data_out              (wbs_sm_data_out),
                        .wb_cbe_out               (wbs_sm_cbe_out),
                        .wbw_fifo_wenable_out     (wbs_sm_wbw_wenable_out),
                        .wbw_fifo_control_out     (wbs_sm_wbw_control_out),
                        .wbw_fifo_almost_full_in  (wbs_sm_wbw_almost_full_in),
                        .wbw_fifo_full_in         (wbs_sm_wbw_full_in),
                        .wbr_fifo_renable_out     (wbs_sm_wbr_renable_out),
                        .wbr_fifo_be_in           (wbs_sm_wbr_be_in),
                        .wbr_fifo_data_in         (wbs_sm_wbr_data_in),
                        .wbr_fifo_control_in      (wbs_sm_wbr_control_in),
                        .wbr_fifo_flush_out       (wbs_sm_wbr_flush_out),
                        .wbr_fifo_empty_in        (wbs_sm_wbr_empty_in),
                        .pciw_fifo_empty_in       (wbs_sm_pciw_empty_in),
                        .wbs_lock_in              (wbs_sm_lock_in),
                        .init_complete_in         (wbs_sm_init_complete_in),
                        .cache_line_size_not_zero (wbs_sm_cache_line_size_not_zero),
                        .del_in_progress_out      (wbs_sm_del_in_progress_out),
                        .ccyc_addr_in             (wbs_sm_ccyc_addr_in),
                        .sample_address_out       (wbs_sm_sample_address_out),
                        .CYC_I                    (wbs_sm_cyc_in),
                        .STB_I                    (wbs_sm_stb_in),
                        .WE_I                     (wbs_sm_we_in),
                        .SEL_I                    (wbs_sm_sel_in),
                        .SDATA_I                  (wbs_sm_sdata_in),
                        .SDATA_O                  (wbs_sm_sdata_out),
                        .ACK_O                    (wbs_sm_ack_out),
                        .RTY_O                    (wbs_sm_rty_out),
                        .ERR_O                    (wbs_sm_err_out),
                        .CAB_I                    (wbs_sm_cab_in)
                       );

// wbw_wbr_fifos inputs
// WBW_FIFO inputs
wire        fifos_wbw_wenable_in        =       wbs_sm_wbw_wenable_out;
wire [31:0] fifos_wbw_addr_data_in      =       wbs_sm_data_out ;
wire [3:0]  fifos_wbw_cbe_in            =       wbs_sm_cbe_out ;
wire [3:0]  fifos_wbw_control_in        =       wbs_sm_wbw_control_out ;
wire        fifos_wbw_renable_in        =       pcim_if_wbw_renable_out ;

//wire        fifos_wbw_flush_in          =       1'b0 ; flush for write fifo not used

// WBR_FIFO inputs
wire        fifos_wbr_wenable_in        =       pcim_if_wbr_wenable_out ;
wire [31:0] fifos_wbr_data_in           =       pcim_if_wbr_data_out ;
wire [3:0]  fifos_wbr_be_in             =       pcim_if_wbr_be_out ;
wire [3:0]  fifos_wbr_control_in        =       pcim_if_wbr_control_out ;
wire        fifos_wbr_renable_in        =       wbs_sm_wbr_renable_out ;
wire        fifos_wbr_flush_in          =       wbs_sm_wbr_flush_out || del_sync_comp_flush_out ;

// WBW_FIFO and WBR_FIFO instantiation
pci_wbw_wbr_fifos fifos
(
    .wb_clock_in               (wb_clock_in),
    .pci_clock_in              (pci_clock_in),
    .reset_in                  (reset_in),
    .wbw_wenable_in            (fifos_wbw_wenable_in),
    .wbw_addr_data_in          (fifos_wbw_addr_data_in),
    .wbw_cbe_in                (fifos_wbw_cbe_in),
    .wbw_control_in            (fifos_wbw_control_in),
    .wbw_renable_in            (fifos_wbw_renable_in),
    .wbw_addr_data_out         (fifos_wbw_addr_data_out),
    .wbw_cbe_out               (fifos_wbw_cbe_out),
    .wbw_control_out           (fifos_wbw_control_out),
//    .wbw_flush_in              (fifos_wbw_flush_in),        // flush for write fifo not used
    .wbw_almost_full_out       (fifos_wbw_almost_full_out),
    .wbw_full_out              (fifos_wbw_full_out),
    .wbw_empty_out             (fifos_wbw_empty_out),
    .wbw_transaction_ready_out (fifos_wbw_transaction_ready_out),
    .wbr_wenable_in            (fifos_wbr_wenable_in),
    .wbr_data_in               (fifos_wbr_data_in),
    .wbr_be_in                 (fifos_wbr_be_in),
    .wbr_control_in            (fifos_wbr_control_in),
    .wbr_renable_in            (fifos_wbr_renable_in),
    .wbr_data_out              (fifos_wbr_data_out),
    .wbr_be_out                (fifos_wbr_be_out),
    .wbr_control_out           (fifos_wbr_control_out),
    .wbr_flush_in              (fifos_wbr_flush_in),
    .wbr_empty_out             (fifos_wbr_empty_out)

`ifdef PCI_BIST
    ,
    .mbist_si_i       (mbist_si_i),
    .mbist_so_o       (mbist_so_o),
    .mbist_ctrl_i       (mbist_ctrl_i)
`endif
) ;

wire [31:0] amux_addr_in  = ADDR_I ;
wire        amux_sample_address_in = wbs_sm_sample_address_out ;

wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_bar0_in   =   wbu_bar0_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_bar1_in   =   wbu_bar1_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_bar2_in   =   wbu_bar2_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_bar3_in   =   wbu_bar3_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_bar4_in   =   wbu_bar4_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_bar5_in   =   wbu_bar5_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_am0_in    =   wbu_am0_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_am1_in    =   wbu_am1_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_am2_in    =   wbu_am2_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_am3_in    =   wbu_am3_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_am4_in    =   wbu_am4_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_am5_in    =   wbu_am5_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_ta0_in    =   wbu_ta0_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_ta1_in    =   wbu_ta1_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_ta2_in    =   wbu_ta2_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_ta3_in    =   wbu_ta3_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_ta4_in    =   wbu_ta4_in ;
wire [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] amux_ta5_in    =   wbu_ta5_in ;
wire [5:0]  amux_at_en_in = wbu_at_en_in ;

pci_wb_addr_mux wb_addr_dec
(
    `ifdef REGISTER_WBS_OUTPUTS
    .clk_in      (wb_clock_in),
    .reset_in    (reset_in),
    .sample_address_in (amux_sample_address_in),
    `endif
    .address_in  (amux_addr_in),
    .bar0_in     (amux_bar0_in),
    .bar1_in     (amux_bar1_in),
    .bar2_in     (amux_bar2_in),
    .bar3_in     (amux_bar3_in),
    .bar4_in     (amux_bar4_in),
    .bar5_in     (amux_bar5_in),
    .am0_in      (amux_am0_in),
    .am1_in      (amux_am1_in),
    .am2_in      (amux_am2_in),
    .am3_in      (amux_am3_in),
    .am4_in      (amux_am4_in),
    .am5_in      (amux_am5_in),
    .ta0_in      (amux_ta0_in),
    .ta1_in      (amux_ta1_in),
    .ta2_in      (amux_ta2_in),
    .ta3_in      (amux_ta3_in),
    .ta4_in      (amux_ta4_in),
    .ta5_in      (amux_ta5_in),
    .at_en_in    (amux_at_en_in),
    .hit_out     (amux_hit_out),
    .address_out (amux_address_out)
);

// delayed transaction logic inputs
wire        del_sync_req_in             =       wbs_sm_del_req_out ;
wire        del_sync_comp_in            =       pcim_if_del_complete_out ;
wire        del_sync_done_in            =       wbs_sm_del_done_out ;
wire        del_sync_in_progress_in     =       wbs_sm_del_in_progress_out ;
wire [31:0] del_sync_addr_in            =       wbs_sm_data_out ;
wire [3:0]  del_sync_be_in              =       wbs_sm_conf_be_out ;
wire        del_sync_we_in              =       wbs_sm_del_write_out ;
wire [3:0]  del_sync_bc_in              =       wbs_sm_del_bc_out ;
wire        del_sync_status_in          =       pcim_if_del_error_out ;
wire        del_sync_burst_in           =       wbs_sm_del_burst_out ;
wire        del_sync_retry_expired_in   =       pcim_if_del_rty_exp_out ;

// delayed transaction logic instantiation
pci_delayed_sync del_sync  (
                            .reset_in             (reset_in),
                            .req_clk_in           (wb_clock_in),
                            .comp_clk_in          (pci_clock_in),
                            .req_in               (del_sync_req_in),
                            .comp_in              (del_sync_comp_in),
                            .done_in              (del_sync_done_in),
                            .in_progress_in       (del_sync_in_progress_in),
                            .comp_req_pending_out (del_sync_comp_req_pending_out),
                            .comp_comp_pending_out(del_sync_comp_comp_pending_out),
                            .req_req_pending_out  (del_sync_req_req_pending_out),
                            .req_comp_pending_out (del_sync_req_comp_pending_out),
                            .addr_in              (del_sync_addr_in),
                            .be_in                (del_sync_be_in),
                            .addr_out             (del_sync_addr_out),
                            .be_out               (del_sync_be_out),
                            .we_in                (del_sync_we_in),
                            .we_out               (del_sync_we_out),
                            .bc_in                (del_sync_bc_in),
                            .bc_out               (del_sync_bc_out),
                            .status_in            (del_sync_status_in),
                            .status_out           (del_sync_status_out),
                            .comp_flush_out       (del_sync_comp_flush_out),
                            .burst_in             (del_sync_burst_in),
                            .burst_out            (del_sync_burst_out),
                            .retry_expired_in     (del_sync_retry_expired_in)
                        );

// delayed write storage inputs
wire        del_write_we_in         =       wbs_sm_del_req_out && wbs_sm_del_write_out ;
wire [31:0] del_write_data_in       =       wbs_sm_conf_data_out ;

pci_delayed_write_reg delayed_write_data
(
	.reset_in       (reset_in),
	.req_clk_in     (wb_clock_in),
	.comp_wdata_out (del_write_data_out),
	.req_we_in      (del_write_we_in),
	.req_wdata_in   (del_write_data_in)
);

`ifdef HOST
    // configuration cycle address decoder input
    wire    [31:0]      ccyc_addr_in = {8'h00, wbu_ccyc_addr_in} ;

    pci_conf_cyc_addr_dec ccyc_addr_dec
    (
        .ccyc_addr_in   (ccyc_addr_in),
        .ccyc_addr_out  (ccyc_addr_out)
    ) ;
`else
`ifdef GUEST
    assign ccyc_addr_out = 32'h0000_0000 ;
`endif
`endif

// pci master interface inputs
wire [31:0] pcim_if_wbw_addr_data_in            =           fifos_wbw_addr_data_out ;
wire [3:0]  pcim_if_wbw_cbe_in                  =           fifos_wbw_cbe_out ;
wire [3:0]  pcim_if_wbw_control_in              =           fifos_wbw_control_out ;
wire        pcim_if_wbw_empty_in                =           fifos_wbw_empty_out ;
wire        pcim_if_wbw_transaction_ready_in    =           fifos_wbw_transaction_ready_out ;
wire [31:0] pcim_if_data_in                     =           pcim_sm_data_out ;
wire [31:0] pcim_if_del_wdata_in                =           del_write_data_out ;
wire        pcim_if_del_req_in                  =           del_sync_comp_req_pending_out ;
wire [31:0] pcim_if_del_addr_in                 =           del_sync_addr_out ;
wire [3:0]  pcim_if_del_bc_in                   =           del_sync_bc_out ;
wire [3:0]  pcim_if_del_be_in                   =           del_sync_be_out ;
wire        pcim_if_del_burst_in                =           del_sync_burst_out ;
wire        pcim_if_del_we_in                   =           del_sync_we_out ;
wire [7:0]  pcim_if_cache_line_size_in          =           wbu_cache_line_size_in ;
wire        pcim_if_wait_in                     =           pcim_sm_wait_out ;
wire        pcim_if_wtransfer_in                =           pcim_sm_wtransfer_out ;
wire        pcim_if_rtransfer_in                =           pcim_sm_rtransfer_out ;
wire        pcim_if_retry_in                    =           pcim_sm_retry_out ;
wire        pcim_if_rerror_in                   =           pcim_sm_rerror_out ;
wire        pcim_if_first_in                    =           pcim_sm_first_out ;
wire        pcim_if_mabort_in                   =           pcim_sm_mabort_out ;

pci_master32_sm_if pci_initiator_if
(
    .clk_in                        (pci_clock_in),
    .reset_in                      (reset_in),
    .address_out                   (pcim_if_address_out),
    .bc_out                        (pcim_if_bc_out),
    .data_out                      (pcim_if_data_out),
    .data_in                       (pcim_if_data_in),
    .be_out                        (pcim_if_be_out),
    .req_out                       (pcim_if_req_out),
    .rdy_out                       (pcim_if_rdy_out),
    .last_out                      (pcim_if_last_out),
    .wbw_renable_out               (pcim_if_wbw_renable_out),
    .wbw_fifo_addr_data_in         (pcim_if_wbw_addr_data_in),
    .wbw_fifo_cbe_in               (pcim_if_wbw_cbe_in),
    .wbw_fifo_control_in           (pcim_if_wbw_control_in),
    .wbw_fifo_empty_in             (pcim_if_wbw_empty_in),
    .wbw_fifo_transaction_ready_in (pcim_if_wbw_transaction_ready_in),
    .wbr_fifo_wenable_out          (pcim_if_wbr_wenable_out),
    .wbr_fifo_data_out             (pcim_if_wbr_data_out),
    .wbr_fifo_be_out               (pcim_if_wbr_be_out),
    .wbr_fifo_control_out          (pcim_if_wbr_control_out),
    .del_wdata_in                  (pcim_if_del_wdata_in),
    .del_complete_out              (pcim_if_del_complete_out),
    .del_req_in                    (pcim_if_del_req_in),
    .del_addr_in                   (pcim_if_del_addr_in),
    .del_bc_in                     (pcim_if_del_bc_in),
    .del_be_in                     (pcim_if_del_be_in),
    .del_burst_in                  (pcim_if_del_burst_in),
    .del_error_out                 (pcim_if_del_error_out),
    .del_rty_exp_out               (pcim_if_del_rty_exp_out),
    .del_we_in                     (pcim_if_del_we_in),
    .err_addr_out                  (pcim_if_err_addr_out),
    .err_bc_out                    (pcim_if_err_bc_out),
    .err_signal_out                (pcim_if_err_signal_out),
    .err_source_out                (pcim_if_err_source_out),
    .err_rty_exp_out               (pcim_if_err_rty_exp_out),
    .cache_line_size_in            (pcim_if_cache_line_size_in),
    .mabort_received_out           (pcim_if_mabort_out),
    .tabort_received_out           (pcim_if_tabort_out),
    .next_data_out                 (pcim_if_next_data_out),
    .next_be_out                   (pcim_if_next_be_out),
    .next_last_out                 (pcim_if_next_last_out),
    .wait_in                       (pcim_if_wait_in),
    .wtransfer_in                  (pcim_if_wtransfer_in),
    .rtransfer_in                  (pcim_if_rtransfer_in),
    .retry_in                      (pcim_if_retry_in),
    .rerror_in                     (pcim_if_rerror_in),
    .first_in                      (pcim_if_first_in),
    .mabort_in                     (pcim_if_mabort_in),
    .posted_write_not_present_out  (pcim_if_posted_write_not_present_out)
);

// pci master state machine inputs
wire        pcim_sm_gnt_in                  =       wbu_pciif_gnt_in ;
wire        pcim_sm_frame_in                =       wbu_pciif_frame_in ;
wire        pcim_sm_irdy_in                 =       wbu_pciif_irdy_in ;
wire        pcim_sm_trdy_in                 =       wbu_pciif_trdy_in;
wire        pcim_sm_stop_in                 =       wbu_pciif_stop_in ;
wire        pcim_sm_devsel_in               =       wbu_pciif_devsel_in ;
wire [31:0] pcim_sm_ad_reg_in               =       wbu_pciif_ad_reg_in ;
wire [31:0] pcim_sm_address_in              =       pcim_if_address_out ;
wire [3:0]  pcim_sm_bc_in                   =       pcim_if_bc_out ;
wire [31:0] pcim_sm_data_in                 =       pcim_if_data_out ;
wire [3:0]  pcim_sm_be_in                   =       pcim_if_be_out ;
wire        pcim_sm_req_in                  =       pcim_if_req_out ;
wire        pcim_sm_rdy_in                  =       pcim_if_rdy_out ;
wire        pcim_sm_last_in                 =       pcim_if_last_out ;
wire [7:0]  pcim_sm_latency_tim_val_in      =       wbu_latency_tim_val_in ;
wire [31:0] pcim_sm_next_data_in            =       pcim_if_next_data_out ;
wire [3:0]  pcim_sm_next_be_in              =       pcim_if_next_be_out ;
wire        pcim_sm_next_last_in            =       pcim_if_next_last_out ;
wire        pcim_sm_trdy_reg_in             =       wbu_pciif_trdy_reg_in ;
wire        pcim_sm_stop_reg_in             =       wbu_pciif_stop_reg_in ;
wire        pcim_sm_devsel_reg_in           =       wbu_pciif_devsel_reg_in ;
wire        pcim_sm_frame_en_in             =       wbu_pciif_frame_en_in ;
wire        pcim_sm_frame_out_in            =       wbu_pciif_frame_out_in ;

pci_master32_sm pci_initiator_sm
(
    .clk_in                     (pci_clock_in),
    .reset_in                   (reset_in),
    .pci_req_out                (pcim_sm_req_out),
    .pci_gnt_in                 (pcim_sm_gnt_in),
    .pci_frame_in               (pcim_sm_frame_in),
    .pci_frame_out              (pcim_sm_frame_out),
    .pci_frame_en_out           (pcim_sm_frame_en_out),
    .pci_frame_out_in           (pcim_sm_frame_out_in),
    .pci_frame_load_out         (pcim_sm_frame_load_out),
    .pci_frame_en_in            (pcim_sm_frame_en_in),
    .pci_irdy_in                (pcim_sm_irdy_in),
    .pci_irdy_out               (pcim_sm_irdy_out),
    .pci_irdy_en_out            (pcim_sm_irdy_en_out),
    .pci_trdy_in                (pcim_sm_trdy_in),
    .pci_trdy_reg_in            (pcim_sm_trdy_reg_in),
    .pci_stop_in                (pcim_sm_stop_in),
    .pci_stop_reg_in            (pcim_sm_stop_reg_in),
    .pci_devsel_in              (pcim_sm_devsel_in),
    .pci_devsel_reg_in          (pcim_sm_devsel_reg_in),
    .pci_ad_reg_in              (pcim_sm_ad_reg_in),
    .pci_ad_out                 (pcim_sm_ad_out),
    .pci_ad_en_out              (pcim_sm_ad_en_out),
    .pci_cbe_out                (pcim_sm_cbe_out),
    .pci_cbe_en_out             (pcim_sm_cbe_en_out),
    .address_in                 (pcim_sm_address_in),
    .bc_in                      (pcim_sm_bc_in),
    .data_in                    (pcim_sm_data_in),
    .data_out                   (pcim_sm_data_out),
    .be_in                      (pcim_sm_be_in),
    .req_in                     (pcim_sm_req_in),
    .rdy_in                     (pcim_sm_rdy_in),
    .last_in                    (pcim_sm_last_in),
    .latency_tim_val_in         (pcim_sm_latency_tim_val_in),
    .next_data_in               (pcim_sm_next_data_in),
    .next_be_in                 (pcim_sm_next_be_in),
    .next_last_in               (pcim_sm_next_last_in),
    .ad_load_out                (pcim_sm_ad_load_out),
    .ad_load_on_transfer_out    (pcim_sm_ad_load_on_transfer_out),
    .wait_out                   (pcim_sm_wait_out),
    .wtransfer_out              (pcim_sm_wtransfer_out),
    .rtransfer_out              (pcim_sm_rtransfer_out),
    .retry_out                  (pcim_sm_retry_out),
    .rerror_out                 (pcim_sm_rerror_out),
    .first_out                  (pcim_sm_first_out),
    .mabort_out                 (pcim_sm_mabort_out)
) ;

endmodule
