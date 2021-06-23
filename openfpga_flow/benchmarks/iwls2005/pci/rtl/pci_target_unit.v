//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: pci_target_unit.v                                ////
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
// $Log: pci_target_unit.v,v $
// Revision 1.16  2004/08/19 15:27:34  mihad
// Changed minimum pci image size to 256 bytes because
// of some PC system problems with size of IO images.
//
// Revision 1.15  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.14  2003/10/17 09:11:52  markom
// mbist signals updated according to newest convention
//
// Revision 1.13  2003/08/21 20:55:14  tadejm
// Corrected bug when writing to FIFO (now it is registered).
//
// Revision 1.12  2003/08/08 16:36:33  tadejm
// Added 'three_left_out' to pci_pciw_fifo signaling three locations before full. Added comparison between current registered cbe and next unregistered cbe to signal wb_master whether it is allowed to performe burst or not. Due to this, I needed 'three_left_out' so that writing to pci_pciw_fifo can be registered, otherwise timing problems would occure.
//
// Revision 1.11  2003/01/27 16:49:31  mihad
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
// Revision 1.7  2002/10/08 17:17:05  mihad
// Added BIST signals for RAMs.
//
// Revision 1.6  2002/09/25 15:53:52  mihad
// Removed all logic from asynchronous reset network
//
// Revision 1.5  2002/03/05 11:53:47  mihad
// Added some testcases, removed un-needed fifo signals
//
// Revision 1.4  2002/02/19 16:32:37  mihad
// Modified testbench and fixed some bugs
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

// Module instantiates and connects other modules lower in hierarcy
// PCI target unit consists of modules that together form datapath
// between external WISHBONE slaves and external PCI initiators
`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_target_unit
(
    reset_in,
    wb_clock_in,
    pci_clock_in,

    pciu_wbm_adr_o,
    pciu_wbm_dat_o,
    pciu_wbm_dat_i,
    pciu_wbm_cyc_o,
    pciu_wbm_stb_o,
    pciu_wbm_we_o,
    pciu_wbm_cti_o,
    pciu_wbm_bte_o,
    pciu_wbm_sel_o,
    pciu_wbm_ack_i,
    pciu_wbm_rty_i,
    pciu_wbm_err_i,
    pciu_mem_enable_in,
    pciu_io_enable_in,
    pciu_map_in,
    pciu_pref_en_in,
    pciu_conf_data_in,
    pciu_wbw_fifo_empty_in,
    pciu_wbu_del_read_comp_pending_in,
    pciu_wbu_frame_en_in,
    pciu_bar0_in,
    pciu_bar1_in,
    pciu_bar2_in,
    pciu_bar3_in,
    pciu_bar4_in,
    pciu_bar5_in,
    pciu_am0_in,
    pciu_am1_in,
    pciu_am2_in,
    pciu_am3_in,
    pciu_am4_in,
    pciu_am5_in,
    pciu_ta0_in,
    pciu_ta1_in,
    pciu_ta2_in,
    pciu_ta3_in,
    pciu_ta4_in,
    pciu_ta5_in,
    pciu_at_en_in,
    pciu_cache_line_size_in,
    pciu_cache_lsize_not_zero_in,
    pciu_pciif_frame_in,
    pciu_pciif_irdy_in,
    pciu_pciif_idsel_in,
    pciu_pciif_frame_reg_in,
    pciu_pciif_irdy_reg_in,
    pciu_pciif_idsel_reg_in,
    pciu_pciif_ad_reg_in,
    pciu_pciif_cbe_reg_in,
    pciu_pciif_cbe_in,
    pciu_pciif_bckp_trdy_en_in,
    pciu_pciif_bckp_devsel_in,
    pciu_pciif_bckp_trdy_in,
    pciu_pciif_bckp_stop_in,
    pciu_pciif_trdy_reg_in,
    pciu_pciif_stop_reg_in,
    pciu_pciif_trdy_out,
    pciu_pciif_stop_out,
    pciu_pciif_devsel_out,
    pciu_pciif_trdy_en_out,
    pciu_pciif_stop_en_out,
    pciu_pciif_devsel_en_out,
    pciu_ad_load_out,
    pciu_ad_load_on_transfer_out,
    pciu_pciif_ad_out,
    pciu_pciif_ad_en_out,
    pciu_pciif_tabort_set_out,
    pciu_err_addr_out,
    pciu_err_bc_out,
    pciu_err_data_out,
    pciu_err_be_out,
    pciu_err_signal_out,
    pciu_err_source_out,
    pciu_err_rty_exp_out,
    pciu_conf_offset_out,
    pciu_conf_renable_out,
    pciu_conf_wenable_out,
    pciu_conf_be_out,
    pciu_conf_data_out,
    pciu_pci_drcomp_pending_out,
    pciu_pciw_fifo_empty_out

`ifdef PCI_BIST
    ,
    // debug chain signals
    mbist_si_i,       // bist scan serial in
    mbist_so_o,       // bist scan serial out
    mbist_ctrl_i        // bist chain shift control
`endif
);

`ifdef HOST
    `ifdef NO_CNF_IMAGE
        parameter pci_ba0_width = `PCI_NUM_OF_DEC_ADDR_LINES ;
    `else
        parameter pci_ba0_width = 20    ;
    `endif
`endif

`ifdef GUEST
    parameter pci_ba0_width = 20 ;
`endif

parameter pci_ba1_5_width = `PCI_NUM_OF_DEC_ADDR_LINES ;

input reset_in,
      wb_clock_in,
      pci_clock_in ;

output  [31:0]  pciu_wbm_adr_o   ;
output  [31:0]  pciu_wbm_dat_o ;
input   [31:0]  pciu_wbm_dat_i ;
output          pciu_wbm_cyc_o   ;
output          pciu_wbm_stb_o   ;
output          pciu_wbm_we_o    ;
output  [2:0]   pciu_wbm_cti_o   ;
output  [1:0]   pciu_wbm_bte_o   ;
output  [3:0]   pciu_wbm_sel_o   ;
input           pciu_wbm_ack_i   ;
input           pciu_wbm_rty_i   ;
input           pciu_wbm_err_i   ;

input           pciu_wbw_fifo_empty_in ;
input			pciu_wbu_del_read_comp_pending_in ;
input           pciu_wbu_frame_en_in ;

input           pciu_mem_enable_in ;
input           pciu_io_enable_in ;
input   [5:0]   pciu_map_in ;
input   [5:0]   pciu_pref_en_in ;
input   [31:0]  pciu_conf_data_in ;

input   [pci_ba0_width   - 1:0] pciu_bar0_in ;
input   [pci_ba1_5_width - 1:0] pciu_bar1_in ;
input   [pci_ba1_5_width - 1:0] pciu_bar2_in ;
input   [pci_ba1_5_width - 1:0] pciu_bar3_in ;
input   [pci_ba1_5_width - 1:0] pciu_bar4_in ;
input   [pci_ba1_5_width - 1:0] pciu_bar5_in ;
input   [pci_ba1_5_width - 1:0] pciu_am0_in ;
input   [pci_ba1_5_width - 1:0] pciu_am1_in ;
input   [pci_ba1_5_width - 1:0] pciu_am2_in ;
input   [pci_ba1_5_width - 1:0] pciu_am3_in ;
input   [pci_ba1_5_width - 1:0] pciu_am4_in ;
input   [pci_ba1_5_width - 1:0] pciu_am5_in ;
input   [pci_ba1_5_width - 1:0] pciu_ta0_in ;
input   [pci_ba1_5_width - 1:0] pciu_ta1_in ;
input   [pci_ba1_5_width - 1:0] pciu_ta2_in ;
input   [pci_ba1_5_width - 1:0] pciu_ta3_in ;
input   [pci_ba1_5_width - 1:0] pciu_ta4_in ;
input   [pci_ba1_5_width - 1:0] pciu_ta5_in ;
input   [5:0]                   pciu_at_en_in ;

input   [7:0]   pciu_cache_line_size_in ;
input           pciu_cache_lsize_not_zero_in ;

input           pciu_pciif_frame_in ;
input           pciu_pciif_irdy_in ;
input           pciu_pciif_idsel_in ;
input           pciu_pciif_frame_reg_in ;
input           pciu_pciif_irdy_reg_in ;
input           pciu_pciif_idsel_reg_in ;
input  [31:0]   pciu_pciif_ad_reg_in ;
input   [3:0]   pciu_pciif_cbe_reg_in ;
input   [3:0]   pciu_pciif_cbe_in;
input           pciu_pciif_bckp_trdy_en_in ;
input           pciu_pciif_bckp_devsel_in ;
input           pciu_pciif_bckp_trdy_in ;
input           pciu_pciif_bckp_stop_in ;
input           pciu_pciif_trdy_reg_in ;
input           pciu_pciif_stop_reg_in ;


output          pciu_pciif_trdy_out ;
output          pciu_pciif_stop_out ;
output          pciu_pciif_devsel_out ;
output          pciu_pciif_trdy_en_out ;
output          pciu_pciif_stop_en_out ;
output          pciu_pciif_devsel_en_out ;
output          pciu_ad_load_out ;
output          pciu_ad_load_on_transfer_out ;
output [31:0]   pciu_pciif_ad_out ;
output          pciu_pciif_ad_en_out ;
output          pciu_pciif_tabort_set_out ;

output  [31:0]  pciu_err_addr_out ;
output  [3:0]   pciu_err_bc_out ;
output  [31:0]  pciu_err_data_out ;
output  [3:0]   pciu_err_be_out ;
output          pciu_err_signal_out ;
output          pciu_err_source_out ;
output          pciu_err_rty_exp_out ;

output  [11:0]  pciu_conf_offset_out ;
output          pciu_conf_renable_out ;
output          pciu_conf_wenable_out ;
output  [3:0]   pciu_conf_be_out ;
output  [31:0]  pciu_conf_data_out ;

output          pciu_pci_drcomp_pending_out ;
output          pciu_pciw_fifo_empty_out ;

`ifdef PCI_BIST
/*-----------------------------------------------------
BIST debug chain port signals
-----------------------------------------------------*/
input   mbist_si_i;       // bist scan serial in
output  mbist_so_o;       // bist scan serial out
input [`PCI_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i;       // bist chain shift control
`endif


// pci target state machine and interface outputs
wire        pcit_sm_trdy_out ;
wire        pcit_sm_stop_out ;
wire        pcit_sm_devsel_out ;
wire        pcit_sm_trdy_en_out ;
wire        pcit_sm_stop_en_out ;
wire        pcit_sm_devsel_en_out ;
wire        pcit_sm_ad_load_out ;
wire        pcit_sm_ad_load_on_transfer_out ;
wire [31:0] pcit_sm_ad_out ;
wire        pcit_sm_ad_en_out ;
wire [31:0] pcit_sm_address_out ;
wire  [3:0] pcit_sm_bc_out ;
wire        pcit_sm_bc0_out ;
wire [31:0] pcit_sm_data_out ;
wire  [3:0] pcit_sm_be_out ;
wire  [3:0] pcit_sm_next_be_out ;
wire        pcit_sm_req_out ;
wire        pcit_sm_rdy_out ;
wire        pcit_sm_addr_phase_out ;
wire		pcit_sm_bckp_devsel_out ;
wire        pcit_sm_bckp_trdy_out ;
wire		pcit_sm_bckp_stop_out ;
wire        pcit_sm_last_reg_out ;
wire        pcit_sm_frame_reg_out ;
wire        pcit_sm_fetch_pcir_fifo_out ;
wire        pcit_sm_load_medium_reg_out ;
wire        pcit_sm_sel_fifo_mreg_out ;
wire        pcit_sm_sel_conf_fifo_out ;
wire        pcit_sm_load_to_pciw_fifo_out ;
wire        pcit_sm_load_to_conf_out ;

wire        pcit_sm_target_abort_set_out ; // to conf space

assign  pciu_pciif_trdy_out             =   pcit_sm_trdy_out ;
assign  pciu_pciif_stop_out             =   pcit_sm_stop_out ;
assign  pciu_pciif_devsel_out           =   pcit_sm_devsel_out ;
assign  pciu_pciif_trdy_en_out          =   pcit_sm_trdy_en_out ;
assign  pciu_pciif_stop_en_out          =   pcit_sm_stop_en_out ;
assign  pciu_pciif_devsel_en_out        =   pcit_sm_devsel_en_out ;
assign  pciu_ad_load_out                =   pcit_sm_ad_load_out ;
assign  pciu_ad_load_on_transfer_out    =   pcit_sm_ad_load_on_transfer_out ;
assign  pciu_pciif_ad_out               =   pcit_sm_ad_out ;
assign  pciu_pciif_ad_en_out            =   pcit_sm_ad_en_out ;
assign  pciu_pciif_tabort_set_out       =   pcit_sm_target_abort_set_out ;

wire        pcit_if_addr_claim_out ;
wire [31:0] pcit_if_data_out ;
wire        pcit_if_same_read_out ;
wire        pcit_if_norm_access_to_config_out ;
wire        pcit_if_read_completed_out ;
wire        pcit_if_read_processing_out ;
wire        pcit_if_target_abort_out ;
wire        pcit_if_disconect_wo_data_out ;
wire		pcit_if_disconect_w_data_out ;
wire        pcit_if_pciw_fifo_full_out ;
wire        pcit_if_pcir_fifo_data_err_out ;
wire        pcit_if_wbw_fifo_empty_out ;
wire		pcit_if_wbu_del_read_comp_pending_out ;
wire        pcit_if_req_out ;
wire        pcit_if_done_out ;
wire        pcit_if_in_progress_out ;
wire [31:0] pcit_if_addr_out ;
wire  [3:0] pcit_if_be_out ;
wire        pcit_if_we_out ;
wire  [3:0] pcit_if_bc_out ;
wire        pcit_if_burst_ok_out ;
wire        pcit_if_pcir_fifo_renable_out ;
wire        pcit_if_pcir_fifo_flush_out ;
wire        pcit_if_pciw_fifo_wenable_out ;
wire [31:0] pcit_if_pciw_fifo_addr_data_out ;
wire  [3:0] pcit_if_pciw_fifo_cbe_out ;
wire  [3:0] pcit_if_pciw_fifo_control_out ;
wire [11:0] pcit_if_conf_addr_out ;
wire [31:0] pcit_if_conf_data_out ;
wire  [3:0] pcit_if_conf_be_out ;
wire        pcit_if_conf_we_out ;
wire        pcit_if_conf_re_out ;

// pci target state machine outputs
// pci interface signals
assign  pciu_conf_offset_out    =   pcit_if_conf_addr_out ;
assign  pciu_conf_renable_out   =   pcit_if_conf_re_out ;
assign  pciu_conf_wenable_out   =   pcit_if_conf_we_out ;
assign  pciu_conf_be_out        =   pcit_if_conf_be_out ;
assign  pciu_conf_data_out      =   pcit_if_conf_data_out ;

// wishbone master state machine outputs
wire        wbm_sm_wb_read_done ;
wire		wbm_sm_write_attempt ;
wire        wbm_sm_pcir_fifo_wenable_out ;
wire [31:0] wbm_sm_pcir_fifo_data_out ;
wire  [3:0] wbm_sm_pcir_fifo_be_out ;
wire  [3:0] wbm_sm_pcir_fifo_control_out ;
wire        wbm_sm_pciw_fifo_renable_out ;
wire        wbm_sm_pci_error_sig_out ;
wire  [3:0] wbm_sm_pci_error_bc ;
wire        wbm_sm_write_rty_cnt_exp_out ;
wire        wbm_sm_error_source_out ;
wire        wbm_sm_read_rty_cnt_exp_out ;
wire        wbm_sm_cyc_out ;
wire        wbm_sm_stb_out ;
wire        wbm_sm_we_out ;
wire  [2:0] wbm_sm_cti_out ;
wire  [1:0] wbm_sm_bte_out ;
wire  [3:0] wbm_sm_sel_out ;
wire [31:0] wbm_sm_adr_out ;
wire [31:0] wbm_sm_mdata_out ;

assign  pciu_err_addr_out       =   wbm_sm_adr_out ;
assign  pciu_err_bc_out         =   wbm_sm_pci_error_bc ;
assign  pciu_err_data_out       =   wbm_sm_mdata_out ;
assign  pciu_err_be_out         =   ~wbm_sm_sel_out ;
assign  pciu_err_signal_out     =   wbm_sm_pci_error_sig_out ;
assign  pciu_err_source_out     =   wbm_sm_error_source_out ;
assign  pciu_err_rty_exp_out    =   wbm_sm_write_rty_cnt_exp_out ;

assign  pciu_wbm_adr_o       =   wbm_sm_adr_out ;
assign  pciu_wbm_dat_o       =   wbm_sm_mdata_out ;
assign  pciu_wbm_cyc_o       =   wbm_sm_cyc_out ;
assign  pciu_wbm_stb_o       =   wbm_sm_stb_out ;
assign  pciu_wbm_we_o        =   wbm_sm_we_out ;
assign  pciu_wbm_cti_o       =   wbm_sm_cti_out ;
assign  pciu_wbm_bte_o       =   wbm_sm_bte_out ;
assign  pciu_wbm_sel_o       =   wbm_sm_sel_out ;

// pciw_pcir fifo outputs

// pciw_fifo_outputs:
wire [31:0] fifos_pciw_addr_data_out ;
wire [3:0]  fifos_pciw_cbe_out ;
wire [3:0]  fifos_pciw_control_out ;
wire        fifos_pciw_three_left_out ;
wire        fifos_pciw_two_left_out ;
wire        fifos_pciw_almost_full_out ;
wire        fifos_pciw_full_out ;
wire        fifos_pciw_almost_empty_out ;
wire        fifos_pciw_empty_out ;
wire        fifos_pciw_transaction_ready_out ;

assign  pciu_pciw_fifo_empty_out = !wbm_sm_write_attempt;

// pcir_fifo_outputs
wire [31:0] fifos_pcir_data_out ;
wire [3:0]  fifos_pcir_be_out ;
wire [3:0]  fifos_pcir_control_out ;
wire        fifos_pcir_almost_empty_out ;
wire        fifos_pcir_empty_out ;

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

assign  pciu_pci_drcomp_pending_out = del_sync_comp_comp_pending_out ;

// WISHBONE master interface inputs
wire        wbm_sm_pci_tar_read_request             =   del_sync_comp_req_pending_out ;
wire [31:0] wbm_sm_pci_tar_address                  =   del_sync_addr_out ;
wire  [3:0] wbm_sm_pci_tar_cmd                      =   del_sync_bc_out ;
wire  [3:0] wbm_sm_pci_tar_be                       =   del_sync_be_out ;
wire        wbm_sm_pci_tar_burst_ok                 =   del_sync_burst_out ;
wire  [7:0] wbm_sm_pci_cache_line_size              =   pciu_cache_line_size_in ;
wire        wbm_sm_cache_lsize_not_zero_in          =   pciu_cache_lsize_not_zero_in ;
wire [31:0] wbm_sm_pciw_fifo_addr_data_in           =   fifos_pciw_addr_data_out ;
wire  [3:0] wbm_sm_pciw_fifo_cbe_in                 =   fifos_pciw_cbe_out ;
wire  [3:0] wbm_sm_pciw_fifo_control_in             =   fifos_pciw_control_out ;
wire        wbm_sm_pciw_fifo_almost_empty_in        =   fifos_pciw_almost_empty_out ;
wire        wbm_sm_pciw_fifo_empty_in               =   fifos_pciw_empty_out ;
wire        wbm_sm_pciw_fifo_transaction_ready_in   =   fifos_pciw_transaction_ready_out ;
wire [31:0] wbm_sm_mdata_in                         =   pciu_wbm_dat_i ;
wire        wbm_sm_ack_in                           =   pciu_wbm_ack_i ;
wire        wbm_sm_rty_in                           =   pciu_wbm_rty_i ;
wire        wbm_sm_err_in                           =   pciu_wbm_err_i ;

// WISHBONE master interface instantiation
pci_wb_master wishbone_master
(
    .wb_clock_in                    (wb_clock_in),
    .reset_in                       (reset_in),
    .pci_tar_read_request           (wbm_sm_pci_tar_read_request),  //in
    .pci_tar_address                (wbm_sm_pci_tar_address),       //in
    .pci_tar_cmd                    (wbm_sm_pci_tar_cmd),           //in
    .pci_tar_be                     (wbm_sm_pci_tar_be),            //in
    .pci_tar_burst_ok				(wbm_sm_pci_tar_burst_ok),		//in
    .pci_cache_line_size            (wbm_sm_pci_cache_line_size),   //in
    .cache_lsize_not_zero           (wbm_sm_cache_lsize_not_zero_in),
    .wb_read_done_out               (wbm_sm_wb_read_done),          //out
    .w_attempt						(wbm_sm_write_attempt),			//out
    .pcir_fifo_wenable_out          (wbm_sm_pcir_fifo_wenable_out),
    .pcir_fifo_data_out             (wbm_sm_pcir_fifo_data_out),
    .pcir_fifo_be_out               (wbm_sm_pcir_fifo_be_out),
    .pcir_fifo_control_out          (wbm_sm_pcir_fifo_control_out),
    .pciw_fifo_renable_out          (wbm_sm_pciw_fifo_renable_out),
    .pciw_fifo_addr_data_in         (wbm_sm_pciw_fifo_addr_data_in),
    .pciw_fifo_cbe_in               (wbm_sm_pciw_fifo_cbe_in),
    .pciw_fifo_control_in           (wbm_sm_pciw_fifo_control_in),
    .pciw_fifo_almost_empty_in      (wbm_sm_pciw_fifo_almost_empty_in),
    .pciw_fifo_empty_in             (wbm_sm_pciw_fifo_empty_in),
    .pciw_fifo_transaction_ready_in (wbm_sm_pciw_fifo_transaction_ready_in),
    .pci_error_sig_out              (wbm_sm_pci_error_sig_out),
    .pci_error_bc                   (wbm_sm_pci_error_bc),
    .write_rty_cnt_exp_out          (wbm_sm_write_rty_cnt_exp_out),
    .error_source_out               (wbm_sm_error_source_out),
    .read_rty_cnt_exp_out           (wbm_sm_read_rty_cnt_exp_out),
    .wb_cyc_o                      (wbm_sm_cyc_out),
    .wb_stb_o                      (wbm_sm_stb_out),
    .wb_we_o                       (wbm_sm_we_out),
    .wb_cti_o                      (wbm_sm_cti_out),
    .wb_bte_o                      (wbm_sm_bte_out),
    .wb_sel_o                      (wbm_sm_sel_out),
    .wb_adr_o                      (wbm_sm_adr_out),
    .wb_dat_i                      (wbm_sm_mdata_in),
    .wb_dat_o                      (wbm_sm_mdata_out),
    .wb_ack_i                      (wbm_sm_ack_in),
    .wb_rty_i                      (wbm_sm_rty_in),
    .wb_err_i                      (wbm_sm_err_in)
);

// pciw_pcir_fifos inputs
// PCIW_FIFO inputs
wire        fifos_pciw_wenable_in       =   pcit_if_pciw_fifo_wenable_out ;
wire [31:0] fifos_pciw_addr_data_in     =   pcit_if_pciw_fifo_addr_data_out ;
wire [3:0]  fifos_pciw_cbe_in           =   pcit_if_pciw_fifo_cbe_out ;
wire [3:0]  fifos_pciw_control_in       =   pcit_if_pciw_fifo_control_out ;
wire        fifos_pciw_renable_in       =   wbm_sm_pciw_fifo_renable_out ;
//wire        fifos_pciw_flush_in         =   1'b0 ;    // flush not used for write fifo

// PCIR_FIFO inputs
wire        fifos_pcir_wenable_in       =   wbm_sm_pcir_fifo_wenable_out ;
wire [31:0] fifos_pcir_data_in          =   wbm_sm_pcir_fifo_data_out ;
wire [3:0]  fifos_pcir_be_in            =   wbm_sm_pcir_fifo_be_out ;
wire [3:0]  fifos_pcir_control_in       =   wbm_sm_pcir_fifo_control_out ;
wire        fifos_pcir_renable_in       =   pcit_if_pcir_fifo_renable_out ;
wire        fifos_pcir_flush_in         =   pcit_if_pcir_fifo_flush_out ;

// PCIW_FIFO and PCIR_FIFO instantiation
pci_pciw_pcir_fifos fifos
(
    .wb_clock_in                (wb_clock_in),
    .pci_clock_in               (pci_clock_in),
    .reset_in                   (reset_in),
    .pciw_wenable_in            (fifos_pciw_wenable_in),      //for PCI Target !!!
    .pciw_addr_data_in          (fifos_pciw_addr_data_in),    //for PCI Target !!!
    .pciw_cbe_in                (fifos_pciw_cbe_in),          //for PCI Target !!!
    .pciw_control_in            (fifos_pciw_control_in),      //for PCI Target !!!
    .pciw_renable_in            (fifos_pciw_renable_in),
    .pciw_addr_data_out         (fifos_pciw_addr_data_out),
    .pciw_cbe_out               (fifos_pciw_cbe_out),
    .pciw_control_out           (fifos_pciw_control_out),
//    .pciw_flush_in              (fifos_pciw_flush_in),      // flush not used for write fifo
    .pciw_three_left_out        (fifos_pciw_three_left_out),  //for PCI Target !!!
    .pciw_two_left_out          (fifos_pciw_two_left_out),    //for PCI Target !!!
    .pciw_almost_full_out       (fifos_pciw_almost_full_out), //for PCI Target !!!
    .pciw_full_out              (fifos_pciw_full_out),        //for PCI Target !!!
    .pciw_almost_empty_out      (fifos_pciw_almost_empty_out),
    .pciw_empty_out             (fifos_pciw_empty_out),
    .pciw_transaction_ready_out (fifos_pciw_transaction_ready_out),
    .pcir_wenable_in            (fifos_pcir_wenable_in),
    .pcir_data_in               (fifos_pcir_data_in),
    .pcir_be_in                 (fifos_pcir_be_in),
    .pcir_control_in            (fifos_pcir_control_in),
    .pcir_renable_in            (fifos_pcir_renable_in),      //for PCI Target !!!
    .pcir_data_out              (fifos_pcir_data_out),        //for PCI Target !!!
    .pcir_be_out                (fifos_pcir_be_out),          //for PCI Target !!!
    .pcir_control_out           (fifos_pcir_control_out),     //for PCI Target !!!
    .pcir_flush_in              (fifos_pcir_flush_in),        //for PCI Target !!!
    .pcir_full_out              (),
    .pcir_almost_empty_out      (fifos_pcir_almost_empty_out), //for PCI Target !!!
    .pcir_empty_out             (fifos_pcir_empty_out),        //for PCI Target !!!
    .pcir_transaction_ready_out ()

`ifdef PCI_BIST
    ,
    .mbist_si_i       (mbist_si_i),
    .mbist_so_o       (mbist_so_o),
    .mbist_ctrl_i       (mbist_ctrl_i)
`endif
) ;

// delayed transaction logic inputs
wire        del_sync_req_in             =   pcit_if_req_out ;
wire        del_sync_comp_in            =   wbm_sm_wb_read_done ;
wire        del_sync_done_in            =   pcit_if_done_out ;
wire        del_sync_in_progress_in     =   pcit_if_in_progress_out ;
wire [31:0] del_sync_addr_in            =   pcit_if_addr_out ;
wire  [3:0] del_sync_be_in              =   pcit_if_be_out ;
wire        del_sync_we_in              =   pcit_if_we_out ;
wire  [3:0] del_sync_bc_in              =   pcit_if_bc_out ;
wire        del_sync_status_in          =   1'b0 ;
wire        del_sync_burst_in           =   pcit_if_burst_ok_out ;
wire        del_sync_retry_expired_in   =   wbm_sm_read_rty_cnt_exp_out ;

// delayed transaction logic instantiation
pci_delayed_sync del_sync
(
    .reset_in               (reset_in),
    .req_clk_in             (pci_clock_in),
    .comp_clk_in            (wb_clock_in),
    .req_in                 (del_sync_req_in),
    .comp_in                (del_sync_comp_in),
    .done_in                (del_sync_done_in),
    .in_progress_in         (del_sync_in_progress_in),
    .comp_req_pending_out   (del_sync_comp_req_pending_out),
    .comp_comp_pending_out  (del_sync_comp_comp_pending_out),
    .req_req_pending_out    (del_sync_req_req_pending_out),
    .req_comp_pending_out   (del_sync_req_comp_pending_out),
    .addr_in                (del_sync_addr_in),
    .be_in                  (del_sync_be_in),
    .addr_out               (del_sync_addr_out),
    .be_out                 (del_sync_be_out),
    .we_in                  (del_sync_we_in),
    .we_out                 (del_sync_we_out),
    .bc_in                  (del_sync_bc_in),
    .bc_out                 (del_sync_bc_out),
    .status_in              (del_sync_status_in),
    .status_out             (del_sync_status_out),
    .comp_flush_out         (del_sync_comp_flush_out),
    .burst_in               (del_sync_burst_in),
    .burst_out              (del_sync_burst_out),
    .retry_expired_in       (del_sync_retry_expired_in)
);

// pci target interface inputs
wire [31:0] pcit_if_address_in                      =   pcit_sm_address_out ;
wire  [3:0] pcit_if_bc_in                           =   pcit_sm_bc_out ;
wire        pcit_if_bc0_in                          =   pcit_sm_bc0_out ;
wire [31:0] pcit_if_data_in                         =   pcit_sm_data_out ;
wire  [3:0] pcit_if_be_in                           =   pcit_sm_be_out ;
wire  [3:0] pcit_if_next_be_in                      =   pcit_sm_next_be_out ;
wire        pcit_if_req_in                          =   pcit_sm_req_out ;
wire        pcit_if_rdy_in                          =   pcit_sm_rdy_out ;
wire        pcit_if_addr_phase_in                   =   pcit_sm_addr_phase_out ;
wire		pcit_if_bckp_devsel_in					=	pcit_sm_bckp_devsel_out ;
wire        pcit_if_bckp_trdy_in                    =   pcit_sm_bckp_trdy_out ;
wire		pcit_if_bckp_stop_in					=	pcit_sm_bckp_stop_out ;
wire        pcit_if_last_reg_in                     =   pcit_sm_last_reg_out ;
wire        pcit_if_frame_reg_in                    =   pcit_sm_frame_reg_out ;
wire        pcit_if_fetch_pcir_fifo_in              =   pcit_sm_fetch_pcir_fifo_out ;
wire        pcit_if_load_medium_reg_in              =   pcit_sm_load_medium_reg_out ;
wire        pcit_if_sel_fifo_mreg_in                =   pcit_sm_sel_fifo_mreg_out ;
wire        pcit_if_sel_conf_fifo_in                =   pcit_sm_sel_conf_fifo_out ;
wire        pcit_if_load_to_pciw_fifo_in            =   pcit_sm_load_to_pciw_fifo_out ;
wire        pcit_if_load_to_conf_in                 =   pcit_sm_load_to_conf_out ;
wire        pcit_if_req_req_pending_in              =   del_sync_req_req_pending_out ;
wire        pcit_if_req_comp_pending_in             =   del_sync_req_comp_pending_out ;
wire        pcit_if_status_in                       =   del_sync_status_out ;
wire [31:0] pcit_if_strd_addr_in                    =   del_sync_addr_out ;
wire  [3:0] pcit_if_strd_bc_in                      =   del_sync_bc_out ;
wire        pcit_if_comp_flush_in                   =   del_sync_comp_flush_out ;
wire [31:0] pcit_if_pcir_fifo_data_in               =   fifos_pcir_data_out ;
wire  [3:0] pcit_if_pcir_fifo_be_in                 =   fifos_pcir_be_out ;
wire  [3:0] pcit_if_pcir_fifo_control_in            =   fifos_pcir_control_out ;
wire        pcit_if_pcir_fifo_almost_empty_in       =   fifos_pcir_almost_empty_out ;
wire        pcit_if_pcir_fifo_empty_in              =   fifos_pcir_empty_out ;
wire        pcit_if_pciw_fifo_three_left_in         =   fifos_pciw_three_left_out ;
wire        pcit_if_pciw_fifo_two_left_in           =   fifos_pciw_two_left_out ;
wire        pcit_if_pciw_fifo_almost_full_in        =   fifos_pciw_almost_full_out ;
wire        pcit_if_pciw_fifo_full_in               =   fifos_pciw_full_out ;
wire        pcit_if_wbw_fifo_empty_in               =   pciu_wbw_fifo_empty_in ;
wire		pcit_if_wbu_del_read_comp_pending_in	=	pciu_wbu_del_read_comp_pending_in ;
wire [31:0] pcit_if_conf_data_in                    =   pciu_conf_data_in ;
wire        pcit_if_mem_enable_in                   =   pciu_mem_enable_in ;
wire        pcit_if_io_enable_in                    =   pciu_io_enable_in ;
wire        pcit_if_mem_io_addr_space0_in           =   pciu_map_in[0] ;
wire        pcit_if_mem_io_addr_space1_in           =   pciu_map_in[1] ;
wire        pcit_if_mem_io_addr_space2_in           =   pciu_map_in[2] ;
wire        pcit_if_mem_io_addr_space3_in           =   pciu_map_in[3] ;
wire        pcit_if_mem_io_addr_space4_in           =   pciu_map_in[4] ;
wire        pcit_if_mem_io_addr_space5_in           =   pciu_map_in[5] ;
wire        pcit_if_pre_fetch_en0_in                =   pciu_pref_en_in[0] ;
wire        pcit_if_pre_fetch_en1_in                =   pciu_pref_en_in[1] ;
wire        pcit_if_pre_fetch_en2_in                =   pciu_pref_en_in[2] ;
wire        pcit_if_pre_fetch_en3_in                =   pciu_pref_en_in[3] ;
wire        pcit_if_pre_fetch_en4_in                =   pciu_pref_en_in[4] ;
wire        pcit_if_pre_fetch_en5_in                =   pciu_pref_en_in[5] ;
wire [(pci_ba0_width   - 1):0] pcit_if_pci_base_addr0_in =   pciu_bar0_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_base_addr1_in =   pciu_bar1_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_base_addr2_in =   pciu_bar2_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_base_addr3_in =   pciu_bar3_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_base_addr4_in =   pciu_bar4_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_base_addr5_in =   pciu_bar5_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_addr_mask0_in =   pciu_am0_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_addr_mask1_in =   pciu_am1_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_addr_mask2_in =   pciu_am2_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_addr_mask3_in =   pciu_am3_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_addr_mask4_in =   pciu_am4_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_addr_mask5_in =   pciu_am5_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_tran_addr0_in =   pciu_ta0_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_tran_addr1_in =   pciu_ta1_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_tran_addr2_in =   pciu_ta2_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_tran_addr3_in =   pciu_ta3_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_tran_addr4_in =   pciu_ta4_in ;
wire [(pci_ba1_5_width - 1):0] pcit_if_pci_tran_addr5_in =   pciu_ta5_in ;
wire        pcit_if_addr_tran_en0_in                =   pciu_at_en_in[0] ;
wire        pcit_if_addr_tran_en1_in                =   pciu_at_en_in[1] ;
wire        pcit_if_addr_tran_en2_in                =   pciu_at_en_in[2] ;
wire        pcit_if_addr_tran_en3_in                =   pciu_at_en_in[3] ;
wire        pcit_if_addr_tran_en4_in                =   pciu_at_en_in[4] ;
wire        pcit_if_addr_tran_en5_in                =   pciu_at_en_in[5] ;

pci_target32_interface pci_target_if
(
    .clk_in                         (pci_clock_in),
    .reset_in                       (reset_in),
    .address_in                     (pcit_if_address_in),
    .addr_claim_out                 (pcit_if_addr_claim_out),
    .bc_in                          (pcit_if_bc_in),
    .bc0_in                         (pcit_if_bc0_in),
    .data_in                        (pcit_if_data_in),
    .data_out                       (pcit_if_data_out),
    .be_in                          (pcit_if_be_in),
    .next_be_in                     (pcit_if_next_be_in),
    .req_in                         (pcit_if_req_in),
    .rdy_in                         (pcit_if_rdy_in),
    .addr_phase_in                  (pcit_if_addr_phase_in),
    .bckp_devsel_in                 (pcit_if_bckp_devsel_in),
    .bckp_trdy_in                   (pcit_if_bckp_trdy_in),
    .bckp_stop_in                   (pcit_if_bckp_stop_in),
    .last_reg_in                    (pcit_if_last_reg_in),
    .frame_reg_in                   (pcit_if_frame_reg_in),
    .fetch_pcir_fifo_in             (pcit_if_fetch_pcir_fifo_in),
    .load_medium_reg_in             (pcit_if_load_medium_reg_in),
    .sel_fifo_mreg_in               (pcit_if_sel_fifo_mreg_in),
    .sel_conf_fifo_in               (pcit_if_sel_conf_fifo_in),
    .load_to_pciw_fifo_in           (pcit_if_load_to_pciw_fifo_in),
    .load_to_conf_in                (pcit_if_load_to_conf_in),
    .same_read_out                  (pcit_if_same_read_out),
    .norm_access_to_config_out      (pcit_if_norm_access_to_config_out),
    .read_completed_out             (pcit_if_read_completed_out),
    .read_processing_out            (pcit_if_read_processing_out),
    .target_abort_out               (pcit_if_target_abort_out),
    .disconect_wo_data_out          (pcit_if_disconect_wo_data_out),
    .disconect_w_data_out			(pcit_if_disconect_w_data_out),
    .pciw_fifo_full_out             (pcit_if_pciw_fifo_full_out),
    .pcir_fifo_data_err_out         (pcit_if_pcir_fifo_data_err_out),
    .wbw_fifo_empty_out             (pcit_if_wbw_fifo_empty_out),
    .wbu_del_read_comp_pending_out	(pcit_if_wbu_del_read_comp_pending_out),
    .req_out                        (pcit_if_req_out),
    .done_out                       (pcit_if_done_out),
    .in_progress_out                (pcit_if_in_progress_out),
    .req_req_pending_in             (pcit_if_req_req_pending_in),
    .req_comp_pending_in            (pcit_if_req_comp_pending_in),
    .addr_out                       (pcit_if_addr_out),
    .be_out                         (pcit_if_be_out),
    .we_out                         (pcit_if_we_out),
    .bc_out                         (pcit_if_bc_out),
    .burst_ok_out                   (pcit_if_burst_ok_out),
    .strd_addr_in                   (pcit_if_strd_addr_in),
    .strd_bc_in                     (pcit_if_strd_bc_in),
    .status_in                      (pcit_if_status_in),
    .comp_flush_in                  (pcit_if_comp_flush_in),
    .pcir_fifo_renable_out          (pcit_if_pcir_fifo_renable_out),
    .pcir_fifo_data_in              (pcit_if_pcir_fifo_data_in),
    .pcir_fifo_be_in                (pcit_if_pcir_fifo_be_in),
    .pcir_fifo_control_in           (pcit_if_pcir_fifo_control_in),
    .pcir_fifo_flush_out            (pcit_if_pcir_fifo_flush_out),
    .pcir_fifo_almost_empty_in      (pcit_if_pcir_fifo_almost_empty_in),
    .pcir_fifo_empty_in             (pcit_if_pcir_fifo_empty_in),
    .pciw_fifo_wenable_out          (pcit_if_pciw_fifo_wenable_out),
    .pciw_fifo_addr_data_out        (pcit_if_pciw_fifo_addr_data_out),
    .pciw_fifo_cbe_out              (pcit_if_pciw_fifo_cbe_out),
    .pciw_fifo_control_out          (pcit_if_pciw_fifo_control_out),
    .pciw_fifo_three_left_in        (pcit_if_pciw_fifo_three_left_in),
    .pciw_fifo_two_left_in          (pcit_if_pciw_fifo_two_left_in),
    .pciw_fifo_almost_full_in       (pcit_if_pciw_fifo_almost_full_in),
    .pciw_fifo_full_in              (pcit_if_pciw_fifo_full_in),
    .wbw_fifo_empty_in              (pcit_if_wbw_fifo_empty_in),
    .wbu_del_read_comp_pending_in	(pcit_if_wbu_del_read_comp_pending_in),
    .conf_addr_out                  (pcit_if_conf_addr_out),
    .conf_data_out                  (pcit_if_conf_data_out),
    .conf_data_in                   (pcit_if_conf_data_in),
    .conf_be_out                    (pcit_if_conf_be_out),
    .conf_we_out                    (pcit_if_conf_we_out),
    .conf_re_out                    (pcit_if_conf_re_out),
    .mem_enable_in                  (pcit_if_mem_enable_in),
    .io_enable_in                   (pcit_if_io_enable_in),
    .mem_io_addr_space0_in          (pcit_if_mem_io_addr_space0_in),
    .mem_io_addr_space1_in          (pcit_if_mem_io_addr_space1_in),
    .mem_io_addr_space2_in          (pcit_if_mem_io_addr_space2_in),
    .mem_io_addr_space3_in          (pcit_if_mem_io_addr_space3_in),
    .mem_io_addr_space4_in          (pcit_if_mem_io_addr_space4_in),
    .mem_io_addr_space5_in          (pcit_if_mem_io_addr_space5_in),
    .pre_fetch_en0_in               (pcit_if_pre_fetch_en0_in),
    .pre_fetch_en1_in               (pcit_if_pre_fetch_en1_in),
    .pre_fetch_en2_in               (pcit_if_pre_fetch_en2_in),
    .pre_fetch_en3_in               (pcit_if_pre_fetch_en3_in),
    .pre_fetch_en4_in               (pcit_if_pre_fetch_en4_in),
    .pre_fetch_en5_in               (pcit_if_pre_fetch_en5_in),
    .pci_base_addr0_in              (pcit_if_pci_base_addr0_in),
    .pci_base_addr1_in              (pcit_if_pci_base_addr1_in),
    .pci_base_addr2_in              (pcit_if_pci_base_addr2_in),
    .pci_base_addr3_in              (pcit_if_pci_base_addr3_in),
    .pci_base_addr4_in              (pcit_if_pci_base_addr4_in),
    .pci_base_addr5_in              (pcit_if_pci_base_addr5_in),
    .pci_addr_mask0_in              (pcit_if_pci_addr_mask0_in),
    .pci_addr_mask1_in              (pcit_if_pci_addr_mask1_in),
    .pci_addr_mask2_in              (pcit_if_pci_addr_mask2_in),
    .pci_addr_mask3_in              (pcit_if_pci_addr_mask3_in),
    .pci_addr_mask4_in              (pcit_if_pci_addr_mask4_in),
    .pci_addr_mask5_in              (pcit_if_pci_addr_mask5_in),
    .pci_tran_addr0_in              (pcit_if_pci_tran_addr0_in),
    .pci_tran_addr1_in              (pcit_if_pci_tran_addr1_in),
    .pci_tran_addr2_in              (pcit_if_pci_tran_addr2_in),
    .pci_tran_addr3_in              (pcit_if_pci_tran_addr3_in),
    .pci_tran_addr4_in              (pcit_if_pci_tran_addr4_in),
    .pci_tran_addr5_in              (pcit_if_pci_tran_addr5_in),
    .addr_tran_en0_in               (pcit_if_addr_tran_en0_in),
    .addr_tran_en1_in               (pcit_if_addr_tran_en1_in),
    .addr_tran_en2_in               (pcit_if_addr_tran_en2_in),
    .addr_tran_en3_in               (pcit_if_addr_tran_en3_in),
    .addr_tran_en4_in               (pcit_if_addr_tran_en4_in),
    .addr_tran_en5_in               (pcit_if_addr_tran_en5_in)
) ;

// pci target state machine inputs
wire        pcit_sm_frame_in                    =   pciu_pciif_frame_in ;
wire        pcit_sm_irdy_in                     =   pciu_pciif_irdy_in ;
wire        pcit_sm_idsel_in                    =   pciu_pciif_idsel_in ;
wire        pcit_sm_frame_reg_in                =   pciu_pciif_frame_reg_in ;
wire        pcit_sm_irdy_reg_in                 =   pciu_pciif_irdy_reg_in ;
wire        pcit_sm_idsel_reg_in                =   pciu_pciif_idsel_reg_in ;
wire [31:0] pcit_sm_ad_reg_in                   =   pciu_pciif_ad_reg_in ;
wire  [3:0] pcit_sm_cbe_reg_in                  =   pciu_pciif_cbe_reg_in ;
wire  [3:0] pcit_sm_cbe_in                      =   pciu_pciif_cbe_in ;
wire        pcit_sm_bckp_trdy_en_in             =   pciu_pciif_bckp_trdy_en_in ;
wire        pcit_sm_bckp_devsel_in              =   pciu_pciif_bckp_devsel_in ;
wire        pcit_sm_bckp_trdy_in                =   pciu_pciif_bckp_trdy_in ;
wire        pcit_sm_bckp_stop_in                =   pciu_pciif_bckp_stop_in ;
wire        pcit_sm_addr_claim_in               =   pcit_if_addr_claim_out ;
wire [31:0] pcit_sm_data_in                     =   pcit_if_data_out ;
wire        pcit_sm_same_read_in                =   pcit_if_same_read_out ;
wire        pcit_sm_norm_access_to_config_in    =   pcit_if_norm_access_to_config_out ;
wire        pcit_sm_read_completed_in           =   pcit_if_read_completed_out ;
wire        pcit_sm_read_processing_in          =   pcit_if_read_processing_out ;
wire        pcit_sm_target_abort_in             =   pcit_if_target_abort_out ;
wire        pcit_sm_disconect_wo_data_in        =   pcit_if_disconect_wo_data_out ;
wire		pcit_sm_disconect_w_data_in			=	pcit_if_disconect_w_data_out ;
wire        pcit_sm_pciw_fifo_full_in           =   pcit_if_pciw_fifo_full_out ;
wire        pcit_sm_pcir_fifo_data_err_in       =   pcit_if_pcir_fifo_data_err_out ;
wire        pcit_sm_wbw_fifo_empty_in           =   pcit_if_wbw_fifo_empty_out ;
wire		pcit_sm_wbu_del_read_comp_pending_in	=	pcit_if_wbu_del_read_comp_pending_out ;
wire        pcit_sm_wbu_frame_en_in             =   pciu_wbu_frame_en_in ;
wire        pcit_sm_trdy_reg_in                 =   pciu_pciif_trdy_reg_in ;
wire        pcit_sm_stop_reg_in                 =   pciu_pciif_stop_reg_in ;


pci_target32_sm pci_target_sm
(
    .clk_in                             (pci_clock_in),
    .reset_in                           (reset_in),
    .pci_frame_in                       (pcit_sm_frame_in),
    .pci_irdy_in                        (pcit_sm_irdy_in),
    .pci_idsel_in                       (pcit_sm_idsel_in),
    .pci_frame_reg_in                   (pcit_sm_frame_reg_in),
    .pci_irdy_reg_in                    (pcit_sm_irdy_reg_in),
    .pci_idsel_reg_in                   (pcit_sm_idsel_reg_in),
    .pci_trdy_out                       (pcit_sm_trdy_out),
    .pci_stop_out                       (pcit_sm_stop_out),
    .pci_devsel_out                     (pcit_sm_devsel_out),
    .pci_trdy_en_out                    (pcit_sm_trdy_en_out),
    .pci_stop_en_out                    (pcit_sm_stop_en_out),
    .pci_devsel_en_out                  (pcit_sm_devsel_en_out),
    .ad_load_out                        (pcit_sm_ad_load_out),
    .ad_load_on_transfer_out            (pcit_sm_ad_load_on_transfer_out),
    .pci_ad_reg_in                      (pcit_sm_ad_reg_in),
    .pci_ad_out                         (pcit_sm_ad_out),
    .pci_ad_en_out                      (pcit_sm_ad_en_out),
    .pci_cbe_reg_in                     (pcit_sm_cbe_reg_in),
    .pci_cbe_in                         (pcit_sm_cbe_in),
    .bckp_trdy_en_in                    (pcit_sm_bckp_trdy_en_in),
    .bckp_devsel_in                     (pcit_sm_bckp_devsel_in),
    .bckp_trdy_in                       (pcit_sm_bckp_trdy_in),
    .bckp_stop_in                       (pcit_sm_bckp_stop_in),
    .pci_trdy_reg_in                    (pcit_sm_trdy_reg_in),
    .pci_stop_reg_in                    (pcit_sm_stop_reg_in),
    .address_out                        (pcit_sm_address_out),
    .addr_claim_in                      (pcit_sm_addr_claim_in),
    .bc_out                             (pcit_sm_bc_out),
    .bc0_out                            (pcit_sm_bc0_out),
    .data_out                           (pcit_sm_data_out),
    .data_in                            (pcit_sm_data_in),
    .be_out                             (pcit_sm_be_out),
    .next_be_out                        (pcit_sm_next_be_out),
    .req_out                            (pcit_sm_req_out),
    .rdy_out                            (pcit_sm_rdy_out),
    .addr_phase_out                     (pcit_sm_addr_phase_out),
    .bckp_devsel_out					(pcit_sm_bckp_devsel_out),
    .bckp_trdy_out                      (pcit_sm_bckp_trdy_out),
    .bckp_stop_out						(pcit_sm_bckp_stop_out),
    .last_reg_out                       (pcit_sm_last_reg_out),
    .frame_reg_out                      (pcit_sm_frame_reg_out),
    .fetch_pcir_fifo_out                (pcit_sm_fetch_pcir_fifo_out),
    .load_medium_reg_out                (pcit_sm_load_medium_reg_out),
    .sel_fifo_mreg_out                  (pcit_sm_sel_fifo_mreg_out),
    .sel_conf_fifo_out                  (pcit_sm_sel_conf_fifo_out),
    .load_to_pciw_fifo_out              (pcit_sm_load_to_pciw_fifo_out),
    .load_to_conf_out                   (pcit_sm_load_to_conf_out),
    .same_read_in                       (pcit_sm_same_read_in),
    .norm_access_to_config_in           (pcit_sm_norm_access_to_config_in),
    .read_completed_in                  (pcit_sm_read_completed_in),
    .read_processing_in                 (pcit_sm_read_processing_in),
    .target_abort_in                    (pcit_sm_target_abort_in),
    .disconect_wo_data_in               (pcit_sm_disconect_wo_data_in),
    .disconect_w_data_in				(pcit_sm_disconect_w_data_in),
    .target_abort_set_out               (pcit_sm_target_abort_set_out),
    .pciw_fifo_full_in                  (pcit_sm_pciw_fifo_full_in),
    .pcir_fifo_data_err_in              (pcit_sm_pcir_fifo_data_err_in),
    .wbw_fifo_empty_in                  (pcit_sm_wbw_fifo_empty_in),
    .wbu_del_read_comp_pending_in		(pcit_sm_wbu_del_read_comp_pending_in),
    .wbu_frame_en_in                    (pcit_sm_wbu_frame_en_in)
) ;

endmodule
