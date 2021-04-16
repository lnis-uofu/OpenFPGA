//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pci_bridge32.v"                                  ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////      - Tadej Markovic (tadej@opencores.org)                  ////
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
// $Log: pci_bridge32.v,v $
// Revision 1.19  2004/09/23 13:48:53  mihad
// The control inputs from PCI are now muxed with control outputs
// using output enable state for given signal.
//
// Revision 1.18  2004/08/19 15:27:34  mihad
// Changed minimum pci image size to 256 bytes because
// of some PC system problems with size of IO images.
//
// Revision 1.17  2004/01/24 11:54:18  mihad
// Update! SPOCI Implemented!
//
// Revision 1.16  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.15  2003/12/10 12:02:54  mihad
// The wbs B3 to B2 translation logic had wrong reset wire connected!
//
// Revision 1.14  2003/12/09 09:33:57  simons
// Some warning cleanup.
//
// Revision 1.13  2003/10/17 09:11:52  markom
// mbist signals updated according to newest convention
//
// Revision 1.12  2003/08/21 20:49:03  tadejm
// Added signals for WB Master B3.
//
// Revision 1.11  2003/08/08 16:36:33  tadejm
// Added 'three_left_out' to pci_pciw_fifo signaling three locations before full. Added comparison between current registered cbe and next unregistered cbe to signal wb_master whether it is allowed to performe burst or not. Due to this, I needed 'three_left_out' so that writing to pci_pciw_fifo can be registered, otherwise timing problems would occure.
//
// Revision 1.10  2003/08/03 18:05:06  mihad
// Added limited WISHBONE B3 support for WISHBONE Slave Unit.
// Doesn't support full speed bursts yet.
//
// Revision 1.9  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.8  2002/10/21 13:04:33  mihad
// Changed BIST signal names etc..
//
// Revision 1.7  2002/10/18 03:36:37  tadejm
// Changed wrong signal name mbist_sen into mbist_ctrl_i.
//
// Revision 1.6  2002/10/17 22:51:50  tadejm
// Changed BIST signals for RAMs.
//
// Revision 1.5  2002/10/11 10:09:01  mihad
// Added additional testcase and changed rst name in BIST to trst
//
// Revision 1.4  2002/10/08 17:17:05  mihad
// Added BIST signals for RAMs.
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

`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

// this is top level module of pci bridge core
// it instantiates and connects other lower level modules
// check polarity of PCI output enables in file out_reg.v and change it according to IO interface specification

module pci_bridge32
(
    // WISHBONE system signals
    wb_clk_i,
    wb_rst_i,
    wb_rst_o,
    wb_int_i,
    wb_int_o,

    // WISHBONE slave interface
    wbs_adr_i,
    wbs_dat_i,
    wbs_dat_o,
    wbs_sel_i,
    wbs_cyc_i,
    wbs_stb_i,
    wbs_we_i,

`ifdef PCI_WB_REV_B3

    wbs_cti_i,
    wbs_bte_i,

`else

    wbs_cab_i,

`endif

    wbs_ack_o,
    wbs_rty_o,
    wbs_err_o,

    // WISHBONE master interface
    wbm_adr_o,
    wbm_dat_i,
    wbm_dat_o,
    wbm_sel_o,
    wbm_cyc_o,
    wbm_stb_o,
    wbm_we_o,
    wbm_cti_o,
    wbm_bte_o,
    wbm_ack_i,
    wbm_rty_i,
    wbm_err_i,

    // pci interface - system pins
    pci_clk_i,
    pci_rst_i,
    pci_rst_o,
    pci_inta_i,
    pci_inta_o,
    pci_rst_oe_o,
    pci_inta_oe_o,

    // arbitration pins
    pci_req_o,
    pci_req_oe_o,

    pci_gnt_i,

    // protocol pins
    pci_frame_i,
    pci_frame_o,

    pci_frame_oe_o,
    pci_irdy_oe_o,
    pci_devsel_oe_o,
    pci_trdy_oe_o,
    pci_stop_oe_o,
    pci_ad_oe_o,
    pci_cbe_oe_o,

    pci_irdy_i,
    pci_irdy_o,

    pci_idsel_i,

    pci_devsel_i,
    pci_devsel_o,

    pci_trdy_i,
    pci_trdy_o,

    pci_stop_i,
    pci_stop_o          ,

    // data transfer pins
    pci_ad_i,
    pci_ad_o,

    pci_cbe_i,
    pci_cbe_o,

    // parity generation and checking pins
    pci_par_i,
    pci_par_o,
    pci_par_oe_o,

    pci_perr_i,
    pci_perr_o,
    pci_perr_oe_o,

    // system error pin
    pci_serr_o,
    pci_serr_oe_o

`ifdef PCI_BIST
    ,
    // debug chain signals
    mbist_si_i,       // bist scan serial in
    mbist_so_o,       // bist scan serial out
    mbist_ctrl_i        // bist chain shift control
`endif

`ifdef PCI_CPCI_HS_IMPLEMENT
    ,
    // Compact PCI Hot Swap signals
    pci_cpci_hs_enum_o      ,   //  ENUM# output with output enable (open drain)
    pci_cpci_hs_enum_oe_o   ,   //  ENUM# enum output enable
    pci_cpci_hs_led_o       ,   //  LED output with output enable (open drain)
    pci_cpci_hs_led_oe_o    ,   //  LED output enable
    pci_cpci_hs_es_i            //  ejector switch state indicator input
`endif

`ifdef PCI_SPOCI
    ,
    // Serial power on configuration interface
    spoci_scl_o     ,
    spoci_scl_oe_o  ,
    spoci_sda_i     ,
    spoci_sda_o     ,
    spoci_sda_oe_o
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

// WISHBONE system signals
input   wb_clk_i ;
input   wb_rst_i ;
output  wb_rst_o ;
input   wb_int_i ;
output  wb_int_o ;

// WISHBONE slave interface
input   [31:0]  wbs_adr_i ;
input   [31:0]  wbs_dat_i ;
output  [31:0]  wbs_dat_o ;
input   [3:0]   wbs_sel_i ;
input           wbs_cyc_i ;
input           wbs_stb_i ;
input           wbs_we_i ;

`ifdef PCI_WB_REV_B3

input [2:0] wbs_cti_i ;
input [1:0] wbs_bte_i ;

`else

input wbs_cab_i ;

`endif

output          wbs_ack_o ;
output          wbs_rty_o ;
output          wbs_err_o ;

// WISHBONE master interface
output  [31:0]  wbm_adr_o ;
input   [31:0]  wbm_dat_i ;
output  [31:0]  wbm_dat_o ;
output  [3:0]   wbm_sel_o ;
output          wbm_cyc_o ;
output          wbm_stb_o ;
output          wbm_we_o ;
output  [2:0]   wbm_cti_o ;
output  [1:0]   wbm_bte_o ;
input           wbm_ack_i ;
input           wbm_rty_i ;
input           wbm_err_i ;

// pci interface - system pins
input   pci_clk_i ;
input   pci_rst_i ;
output  pci_rst_o ;
output  pci_rst_oe_o ;

input   pci_inta_i ;
output  pci_inta_o ;
output  pci_inta_oe_o ;

// arbitration pins
output  pci_req_o ;
output  pci_req_oe_o ;

input   pci_gnt_i ;

// protocol pins
input   pci_frame_i ;
output  pci_frame_o ;
output  pci_frame_oe_o ;
output  pci_irdy_oe_o ;
output  pci_devsel_oe_o ;
output  pci_trdy_oe_o ;
output  pci_stop_oe_o ;
output  [31:0] pci_ad_oe_o ;
output  [3:0]  pci_cbe_oe_o ;

input   pci_irdy_i ;
output  pci_irdy_o ;

input   pci_idsel_i ;

input   pci_devsel_i ;
output  pci_devsel_o ;

input   pci_trdy_i ;
output  pci_trdy_o ;

input   pci_stop_i ;
output  pci_stop_o ;

// data transfer pins
input   [31:0]  pci_ad_i ;
output  [31:0]  pci_ad_o ;

input   [3:0]   pci_cbe_i ;
output  [3:0]   pci_cbe_o ;

// parity generation and checking pins
input   pci_par_i ;
output  pci_par_o ;
output  pci_par_oe_o ;

input   pci_perr_i ;
output  pci_perr_o ;
output  pci_perr_oe_o ;

// system error pin
output  pci_serr_o ;
output  pci_serr_oe_o ;

`ifdef PCI_BIST
/*-----------------------------------------------------
BIST debug chain port signals
-----------------------------------------------------*/
input   mbist_si_i;       // bist scan serial in
output  mbist_so_o;       // bist scan serial out
input [`PCI_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i;       // bist chain shift control
`endif

`ifdef PCI_CPCI_HS_IMPLEMENT
    // Compact PCI Hot Swap signals
output  pci_cpci_hs_enum_o      ;   //  ENUM# output with output enable (open drain)
output  pci_cpci_hs_enum_oe_o   ;   //  ENUM# enum output enable
output  pci_cpci_hs_led_o       ;   //  LED output with output enable (open drain)
output  pci_cpci_hs_led_oe_o    ;   //  LED output enable
input   pci_cpci_hs_es_i        ;   //  ejector switch state indicator input

assign  pci_cpci_hs_enum_o = 1'b0   ;
assign  pci_cpci_hs_led_o  = 1'b0   ;
`endif

`ifdef PCI_SPOCI
output  spoci_scl_o     ;
output  spoci_scl_oe_o  ;
input   spoci_sda_i     ;
output  spoci_sda_o     ;
output  spoci_sda_oe_o  ;

assign  spoci_scl_o = 1'b0  ;
assign  spoci_sda_o = 1'b0  ;
`endif

// declare clock and reset wires
wire pci_clk = pci_clk_i ;
wire wb_clk  = wb_clk_i ;
wire reset ; // assigned at pci bridge reset and interrupt logic

/*=========================================================================================================
First comes definition of all modules' outputs, so they can be assigned to any other module's input later
  in the file, when module is instantiated
=========================================================================================================*/
// PCI BRIDGE RESET AND INTERRUPT LOGIC OUTPUTS
wire    pci_reso_reset ;
wire    pci_reso_pci_rstn_out ;
wire    pci_reso_pci_rstn_en_out ;
wire    pci_reso_rst_o ;
wire    pci_into_pci_intan_out ;
wire    pci_into_pci_intan_en_out ;
wire    pci_into_int_o ;
wire    pci_into_conf_isr_int_prop_out ;

// assign pci bridge reset interrupt logic outputs to top outputs where possible
assign reset            = pci_reso_reset ;
assign pci_rst_o     = pci_reso_pci_rstn_out ;
assign pci_rst_oe_o  = pci_reso_pci_rstn_en_out ;
assign wb_rst_o         = pci_reso_rst_o ;
assign pci_inta_o    = pci_into_pci_intan_out ;
assign pci_inta_oe_o = pci_into_pci_intan_en_out ;
assign wb_int_o         = pci_into_int_o ;

// WISHBONE SLAVE UNIT OUTPUTS
wire    [31:0]  wbu_sdata_out ;
wire            wbu_ack_out ;
wire            wbu_rty_out ;
wire            wbu_err_out ;
wire            wbu_pciif_req_out ;
wire            wbu_pciif_frame_out ;
wire            wbu_pciif_frame_en_out ;
wire            wbu_pciif_irdy_out ;
wire            wbu_pciif_irdy_en_out ;
wire    [31:0]  wbu_pciif_ad_out ;
wire            wbu_pciif_ad_en_out ;
wire    [3:0]   wbu_pciif_cbe_out ;
wire            wbu_pciif_cbe_en_out ;
wire    [31:0]  wbu_err_addr_out ;
wire    [3:0]   wbu_err_bc_out ;
wire            wbu_err_signal_out ;
wire            wbu_err_source_out ;
wire            wbu_err_rty_exp_out ;
wire            wbu_tabort_rec_out ;
wire            wbu_mabort_rec_out ;
wire    [11:0]  wbu_conf_offset_out ;
wire            wbu_conf_renable_out ;
wire            wbu_conf_wenable_out ;
wire    [3:0]   wbu_conf_be_out ;
wire    [31:0]  wbu_conf_data_out ;
wire            wbu_del_read_comp_pending_out ;
wire            wbu_wbw_fifo_empty_out ;
wire            wbu_ad_load_out ;
wire            wbu_ad_load_on_transfer_out ;
wire            wbu_pciif_frame_load_out ;

// PCI TARGET UNIT OUTPUTS
wire    [31:0]  pciu_adr_out ;
wire    [31:0]  pciu_mdata_out ;
wire            pciu_cyc_out ;
wire            pciu_stb_out ;
wire            pciu_we_out ;
wire    [2:0]   pciu_cti_out ;
wire    [1:0]   pciu_bte_out ;
wire    [3:0]   pciu_sel_out ;
wire            pciu_pciif_trdy_out ;
wire            pciu_pciif_stop_out ;
wire            pciu_pciif_devsel_out ;
wire            pciu_pciif_trdy_en_out ;
wire            pciu_pciif_stop_en_out ;
wire            pciu_pciif_devsel_en_out ;
wire            pciu_ad_load_out ;
wire            pciu_ad_load_on_transfer_out ;
wire   [31:0]   pciu_pciif_ad_out ;
wire            pciu_pciif_ad_en_out ;
wire            pciu_pciif_tabort_set_out ;
wire    [31:0]  pciu_err_addr_out ;
wire    [3:0]   pciu_err_bc_out ;
wire    [31:0]  pciu_err_data_out ;
wire    [3:0]   pciu_err_be_out ;
wire            pciu_err_signal_out ;
wire            pciu_err_source_out ;
wire            pciu_err_rty_exp_out ;
wire    [11:0]  pciu_conf_offset_out ;
wire            pciu_conf_renable_out ;
wire            pciu_conf_wenable_out ;
wire    [3:0]   pciu_conf_be_out ;
wire    [31:0]  pciu_conf_data_out ;
wire            pciu_pci_drcomp_pending_out ;
wire            pciu_pciw_fifo_empty_out ;

// assign pci target unit's outputs to top outputs where possible
assign wbm_adr_o    =   pciu_adr_out ;
assign wbm_dat_o    =   pciu_mdata_out ;
assign wbm_cyc_o    =   pciu_cyc_out ;
assign wbm_stb_o    =   pciu_stb_out ;
assign wbm_we_o     =   pciu_we_out ;
assign wbm_cti_o    =   pciu_cti_out ;
assign wbm_bte_o    =   pciu_bte_out ;
assign wbm_sel_o    =   pciu_sel_out ;

// CONFIGURATION SPACE OUTPUTS
wire    [31:0]  conf_w_data_out ;
wire    [31:0]  conf_r_data_out ;
wire            conf_serr_enable_out ;
wire            conf_perr_response_out ;
wire            conf_pci_master_enable_out ;
wire            conf_mem_space_enable_out ;
wire            conf_io_space_enable_out ;
wire    [7:0]   conf_cache_line_size_to_pci_out ;
wire    [7:0]   conf_cache_line_size_to_wb_out ;
wire            conf_cache_lsize_not_zero_to_wb_out ;
wire    [7:0]   conf_latency_tim_out ;

wire    [pci_ba0_width   - 1:0]   conf_pci_ba0_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ba1_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ba2_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ba3_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ba4_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ba5_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ta0_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ta1_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ta2_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ta3_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ta4_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_ta5_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_am0_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_am1_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_am2_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_am3_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_am4_out ;
wire    [pci_ba1_5_width - 1:0]   conf_pci_am5_out ;

wire            conf_pci_mem_io0_out ;
wire            conf_pci_mem_io1_out ;
wire            conf_pci_mem_io2_out ;
wire            conf_pci_mem_io3_out ;
wire            conf_pci_mem_io4_out ;
wire            conf_pci_mem_io5_out ;

wire    [1:0]   conf_pci_img_ctrl0_out ;
wire    [1:0]   conf_pci_img_ctrl1_out ;
wire    [1:0]   conf_pci_img_ctrl2_out ;
wire    [1:0]   conf_pci_img_ctrl3_out ;
wire    [1:0]   conf_pci_img_ctrl4_out ;
wire    [1:0]   conf_pci_img_ctrl5_out ;

wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ba0_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ba1_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ba2_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ba3_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ba4_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ba5_out ;

wire            conf_wb_mem_io0_out ;
wire            conf_wb_mem_io1_out ;
wire            conf_wb_mem_io2_out ;
wire            conf_wb_mem_io3_out ;
wire            conf_wb_mem_io4_out ;
wire            conf_wb_mem_io5_out ;

wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_am0_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_am1_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_am2_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_am3_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_am4_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_am5_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ta0_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ta1_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ta2_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ta3_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ta4_out ;
wire    [19:(20 - `WB_NUM_OF_DEC_ADDR_LINES)]  conf_wb_ta5_out ;
wire    [2:0]   conf_wb_img_ctrl0_out ;
wire    [2:0]   conf_wb_img_ctrl1_out ;
wire    [2:0]   conf_wb_img_ctrl2_out ;
wire    [2:0]   conf_wb_img_ctrl3_out ;
wire    [2:0]   conf_wb_img_ctrl4_out ;
wire    [2:0]   conf_wb_img_ctrl5_out ;
wire    [23:0]  conf_ccyc_addr_out ;
wire            conf_soft_res_out ;
wire            conf_int_out ;
wire            conf_wb_init_complete_out  ;
wire            conf_pci_init_complete_out ;

// PCI IO MUX OUTPUTS
wire        pci_mux_frame_out ;
wire        pci_mux_irdy_out ;
wire        pci_mux_devsel_out ;
wire        pci_mux_trdy_out ;
wire        pci_mux_stop_out ;
wire [3:0]  pci_mux_cbe_out ;
wire [31:0] pci_mux_ad_out ;
wire        pci_mux_ad_load_out ;

wire [31:0] pci_mux_ad_en_out ;
wire        pci_mux_ad_en_unregistered_out ;
wire        pci_mux_frame_en_out ;
wire        pci_mux_irdy_en_out ;
wire        pci_mux_devsel_en_out ;
wire        pci_mux_trdy_en_out ;
wire        pci_mux_stop_en_out ;
wire [3:0]  pci_mux_cbe_en_out ;

wire        pci_mux_par_out ;
wire        pci_mux_par_en_out ;
wire        pci_mux_perr_out ;
wire        pci_mux_perr_en_out ;
wire        pci_mux_serr_out ;
wire        pci_mux_serr_en_out ;

wire        pci_mux_req_out ;
wire        pci_mux_req_en_out ;

// assign outputs to top level outputs

assign pci_ad_oe_o       = pci_mux_ad_en_out ;
assign pci_frame_oe_o   = pci_mux_frame_en_out ;
assign pci_irdy_oe_o    = pci_mux_irdy_en_out ;
assign pci_cbe_oe_o     = pci_mux_cbe_en_out ;

assign pci_par_o         =   pci_mux_par_out ;
assign pci_par_oe_o      =   pci_mux_par_en_out ;
assign pci_perr_o       =   pci_mux_perr_out ;
assign pci_perr_oe_o    =   pci_mux_perr_en_out ;
assign pci_serr_o       =   pci_mux_serr_out ;
assign pci_serr_oe_o    =   pci_mux_serr_en_out ;

assign pci_req_o        =   pci_mux_req_out ;
assign pci_req_oe_o     =   pci_mux_req_en_out ;

assign pci_trdy_oe_o    = pci_mux_trdy_en_out ;
assign pci_devsel_oe_o  = pci_mux_devsel_en_out ;
assign pci_stop_oe_o    = pci_mux_stop_en_out ;
assign pci_trdy_o       =  pci_mux_trdy_out ;
assign pci_devsel_o     = pci_mux_devsel_out ;
assign pci_stop_o       = pci_mux_stop_out ;

assign pci_ad_o          = pci_mux_ad_out ;
assign pci_frame_o      = pci_mux_frame_out ;
assign pci_irdy_o       = pci_mux_irdy_out ;
assign pci_cbe_o        = pci_mux_cbe_out ;

// duplicate output register's outputs
wire            out_bckp_frame_out ;
wire            out_bckp_irdy_out ;
wire            out_bckp_devsel_out ;
wire            out_bckp_trdy_out ;
wire            out_bckp_stop_out ;
wire    [3:0]   out_bckp_cbe_out ;
wire            out_bckp_cbe_en_out ;
wire    [31:0]  out_bckp_ad_out ;
wire            out_bckp_ad_en_out ;
wire            out_bckp_irdy_en_out ;
wire            out_bckp_frame_en_out ;
wire            out_bckp_tar_ad_en_out ;
wire            out_bckp_mas_ad_en_out ;
wire            out_bckp_trdy_en_out ;

wire            out_bckp_par_out ;
wire            out_bckp_par_en_out ;
wire            out_bckp_perr_out ;
wire            out_bckp_perr_en_out ;
wire            out_bckp_serr_out ;
wire            out_bckp_serr_en_out ;

wire            int_pci_frame   = out_bckp_frame_en_out ? out_bckp_frame_out  : pci_frame_i     ;
wire            int_pci_irdy    = out_bckp_irdy_en_out  ? out_bckp_irdy_out   : pci_irdy_i      ;
wire            int_pci_devsel  = out_bckp_trdy_en_out  ? out_bckp_devsel_out : pci_devsel_i    ;
wire            int_pci_trdy    = out_bckp_trdy_en_out  ? out_bckp_trdy_out   : pci_trdy_i      ;
wire            int_pci_stop    = out_bckp_trdy_en_out  ? out_bckp_stop_out   : pci_stop_i      ;
wire    [ 3: 0] int_pci_cbe     = out_bckp_cbe_en_out   ? out_bckp_cbe_out    : pci_cbe_i       ;
wire            int_pci_par     = out_bckp_par_en_out   ? out_bckp_par_out    : pci_par_i       ;
wire            int_pci_perr    = out_bckp_perr_en_out  ? out_bckp_perr_out   : pci_perr_i      ;
// PARITY CHECKER OUTPUTS
wire    parchk_pci_par_out ;
wire    parchk_pci_par_en_out ;
wire    parchk_pci_perr_out ;
wire    parchk_pci_perr_en_out ;
wire    parchk_pci_serr_out ;
wire    parchk_pci_serr_en_out ;
wire    parchk_par_err_detect_out ;
wire    parchk_perr_mas_detect_out ;
wire    parchk_sig_serr_out ;

// input register outputs
wire            in_reg_gnt_out ;
wire            in_reg_frame_out ;
wire            in_reg_irdy_out ;
wire            in_reg_trdy_out ;
wire            in_reg_stop_out ;
wire            in_reg_devsel_out ;
wire            in_reg_idsel_out ;
wire    [31:0]  in_reg_ad_out ;
wire    [3:0]   in_reg_cbe_out ;

/*=========================================================================================================
Now comes definition of all modules' and their appropriate inputs
=========================================================================================================*/
// PCI BRIDGE RESET AND INTERRUPT LOGIC INPUTS
wire    pci_resi_rst_i                  = wb_rst_i ;
wire    pci_resi_pci_rstn_in            = pci_rst_i ;
wire    pci_resi_conf_soft_res_in       = conf_soft_res_out ;
wire    pci_inti_pci_intan_in           = pci_inta_i ;
wire    pci_inti_conf_int_in            = conf_int_out ;
wire    pci_inti_int_i                  = wb_int_i ;
wire    pci_into_init_complete_in       = conf_pci_init_complete_out ;

pci_rst_int pci_resets_and_interrupts
(
    .clk_in                 (pci_clk),
    .rst_i                  (pci_resi_rst_i),
    .pci_rstn_in            (pci_resi_pci_rstn_in),
    .conf_soft_res_in       (pci_resi_conf_soft_res_in),
    .reset                  (pci_reso_reset),
    .pci_rstn_out           (pci_reso_pci_rstn_out),
    .pci_rstn_en_out        (pci_reso_pci_rstn_en_out),
    .rst_o                  (pci_reso_rst_o),
    .pci_intan_in           (pci_inti_pci_intan_in),
    .conf_int_in            (pci_inti_conf_int_in),
    .int_i                  (pci_inti_int_i),
    .pci_intan_out          (pci_into_pci_intan_out),
    .pci_intan_en_out       (pci_into_pci_intan_en_out),
    .int_o                  (pci_into_int_o),
    .conf_isr_int_prop_out  (pci_into_conf_isr_int_prop_out),
    .init_complete_in       (pci_into_init_complete_in)
);


`ifdef PCI_WB_REV_B3

wire            wbs_wbb3_2_wbb2_cyc_o   ;
wire            wbs_wbb3_2_wbb2_stb_o   ;
wire    [31:0]  wbs_wbb3_2_wbb2_adr_o   ;
wire    [31:0]  wbs_wbb3_2_wbb2_dat_i_o ;
wire    [31:0]  wbs_wbb3_2_wbb2_dat_o_o ;
wire            wbs_wbb3_2_wbb2_we_o    ;
wire    [ 3:0]  wbs_wbb3_2_wbb2_sel_o   ;
wire            wbs_wbb3_2_wbb2_ack_o   ;
wire            wbs_wbb3_2_wbb2_err_o   ;
wire            wbs_wbb3_2_wbb2_rty_o   ;
wire            wbs_wbb3_2_wbb2_cab_o   ;

// assign wishbone slave unit's outputs to top outputs where possible
assign wbs_dat_o    =   wbs_wbb3_2_wbb2_dat_o_o ;
assign wbs_ack_o    =   wbs_wbb3_2_wbb2_ack_o   ;
assign wbs_rty_o    =   wbs_wbb3_2_wbb2_rty_o   ;
assign wbs_err_o    =   wbs_wbb3_2_wbb2_err_o       ;

wire            wbs_wbb3_2_wbb2_cyc_i   =   wbs_cyc_i       ;
wire            wbs_wbb3_2_wbb2_stb_i   =   wbs_stb_i       ;
wire            wbs_wbb3_2_wbb2_we_i    =   wbs_we_i        ;
wire            wbs_wbb3_2_wbb2_ack_i   =   wbu_ack_out     ;
wire            wbs_wbb3_2_wbb2_err_i   =   wbu_err_out     ;
wire            wbs_wbb3_2_wbb2_rty_i   =   wbu_rty_out     ;
wire    [31:0]  wbs_wbb3_2_wbb2_adr_i   =   wbs_adr_i       ;
wire    [ 3:0]  wbs_wbb3_2_wbb2_sel_i   =   wbs_sel_i       ;
wire    [31:0]  wbs_wbb3_2_wbb2_dat_i_i =   wbs_dat_i       ;
wire    [31:0]  wbs_wbb3_2_wbb2_dat_o_i =   wbu_sdata_out   ;
wire    [ 2:0]  wbs_wbb3_2_wbb2_cti_i   =   wbs_cti_i       ;
wire    [ 1:0]  wbs_wbb3_2_wbb2_bte_i   =   wbs_bte_i       ;

pci_wbs_wbb3_2_wbb2 i_pci_wbs_wbb3_2_wbb2
(
    .wb_clk_i           (   wb_clk_i    )   ,
    .wb_rst_i           (   reset       )   ,
                        
    .wbs_cyc_i          (   wbs_wbb3_2_wbb2_cyc_i       )   ,
    .wbs_cyc_o          (   wbs_wbb3_2_wbb2_cyc_o       )   ,
    .wbs_stb_i          (   wbs_wbb3_2_wbb2_stb_i       )   ,
    .wbs_stb_o          (   wbs_wbb3_2_wbb2_stb_o       )   ,
    .wbs_adr_i          (   wbs_wbb3_2_wbb2_adr_i       )   ,
    .wbs_adr_o          (   wbs_wbb3_2_wbb2_adr_o       )   ,
    .wbs_dat_i_i        (   wbs_wbb3_2_wbb2_dat_i_i     )   ,
    .wbs_dat_i_o        (   wbs_wbb3_2_wbb2_dat_i_o     )   ,
    .wbs_dat_o_i        (   wbs_wbb3_2_wbb2_dat_o_i     )   ,
    .wbs_dat_o_o        (   wbs_wbb3_2_wbb2_dat_o_o     )   ,
    .wbs_we_i           (   wbs_wbb3_2_wbb2_we_i        )   ,
    .wbs_we_o           (   wbs_wbb3_2_wbb2_we_o        )   ,
    .wbs_sel_i          (   wbs_wbb3_2_wbb2_sel_i       )   ,
    .wbs_sel_o          (   wbs_wbb3_2_wbb2_sel_o       )   ,
    .wbs_ack_i          (   wbs_wbb3_2_wbb2_ack_i       )   ,
    .wbs_ack_o          (   wbs_wbb3_2_wbb2_ack_o       )   ,
    .wbs_err_i          (   wbs_wbb3_2_wbb2_err_i       )   ,
    .wbs_err_o          (   wbs_wbb3_2_wbb2_err_o       )   ,
    .wbs_rty_i          (   wbs_wbb3_2_wbb2_rty_i       )   ,
    .wbs_rty_o          (   wbs_wbb3_2_wbb2_rty_o       )   ,
    .wbs_cti_i          (   wbs_wbb3_2_wbb2_cti_i       )   ,
    .wbs_bte_i          (   wbs_wbb3_2_wbb2_bte_i       )   ,
    .wbs_cab_o          (   wbs_wbb3_2_wbb2_cab_o       )   ,
    .wb_init_complete_i (   conf_wb_init_complete_out   )
) ;

// WISHBONE SLAVE UNIT INPUTS
wire    [31:0]  wbu_addr_in     =   wbs_wbb3_2_wbb2_adr_o   ;
wire    [31:0]  wbu_sdata_in    =   wbs_wbb3_2_wbb2_dat_i_o ;
wire            wbu_cyc_in      =   wbs_wbb3_2_wbb2_cyc_o   ;
wire            wbu_stb_in      =   wbs_wbb3_2_wbb2_stb_o   ;
wire            wbu_we_in       =   wbs_wbb3_2_wbb2_we_o    ;
wire    [3:0]   wbu_sel_in      =   wbs_wbb3_2_wbb2_sel_o   ;
wire            wbu_cab_in      =   wbs_wbb3_2_wbb2_cab_o   ;

`else

// WISHBONE SLAVE UNIT INPUTS
wire    [31:0]  wbu_addr_in                     =   wbs_adr_i ;
wire    [31:0]  wbu_sdata_in                    =   wbs_dat_i ;
wire            wbu_cyc_in                      =   wbs_cyc_i ;
wire            wbu_stb_in                      =   wbs_stb_i ;
wire            wbu_we_in                       =   wbs_we_i ;
wire    [3:0]   wbu_sel_in                      =   wbs_sel_i ;
wire            wbu_cab_in                      =   wbs_cab_i ;

// assign wishbone slave unit's outputs to top outputs where possible
assign wbs_dat_o    =   wbu_sdata_out   ;
assign wbs_ack_o    =   wbu_ack_out     ;
assign wbs_rty_o    =   wbu_rty_out     ;
assign wbs_err_o    =   wbu_err_out     ;

`endif

wire    [5:0]   wbu_map_in                      =   {
                                                     conf_wb_mem_io5_out,
                                                     conf_wb_mem_io4_out,
                                                     conf_wb_mem_io3_out,
                                                     conf_wb_mem_io2_out,
                                                     conf_wb_mem_io1_out,
                                                     conf_wb_mem_io0_out
                                                    } ;

wire    [5:0]   wbu_pref_en_in                  =   {
                                                     conf_wb_img_ctrl5_out[1],
                                                     conf_wb_img_ctrl4_out[1],
                                                     conf_wb_img_ctrl3_out[1],
                                                     conf_wb_img_ctrl2_out[1],
                                                     conf_wb_img_ctrl1_out[1],
                                                     conf_wb_img_ctrl0_out[1]
                                                    };
wire    [5:0]   wbu_mrl_en_in                   =   {
                                                     conf_wb_img_ctrl5_out[0],
                                                     conf_wb_img_ctrl4_out[0],
                                                     conf_wb_img_ctrl3_out[0],
                                                     conf_wb_img_ctrl2_out[0],
                                                     conf_wb_img_ctrl1_out[0],
                                                     conf_wb_img_ctrl0_out[0]
                                                    };

wire    [5:0]   wbu_at_en_in                    =   {
                                                     conf_wb_img_ctrl5_out[2],
                                                     conf_wb_img_ctrl4_out[2],
                                                     conf_wb_img_ctrl3_out[2],
                                                     conf_wb_img_ctrl2_out[2],
                                                     conf_wb_img_ctrl1_out[2],
                                                     conf_wb_img_ctrl0_out[2]
                                                    } ;

wire            wbu_pci_drcomp_pending_in       =   pciu_pci_drcomp_pending_out ;
wire            wbu_pciw_empty_in               =   pciu_pciw_fifo_empty_out ;

`ifdef HOST
    wire    [31:0]  wbu_conf_data_in            =   conf_w_data_out ;
`else
`ifdef GUEST
    wire    [31:0]  wbu_conf_data_in            =   conf_r_data_out ;
`endif
`endif

wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar0_in  =   conf_wb_ba0_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar1_in  =   conf_wb_ba1_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar2_in  =   conf_wb_ba2_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar3_in  =   conf_wb_ba3_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar4_in  =   conf_wb_ba4_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_bar5_in  =   conf_wb_ba5_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am0_in   =   conf_wb_am0_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am1_in   =   conf_wb_am1_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am2_in   =   conf_wb_am2_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am3_in   =   conf_wb_am3_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am4_in   =   conf_wb_am4_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_am5_in   =   conf_wb_am5_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta0_in   =   conf_wb_ta0_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta1_in   =   conf_wb_ta1_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta2_in   =   conf_wb_ta2_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta3_in   =   conf_wb_ta3_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta4_in   =   conf_wb_ta4_out ;
wire   [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] wbu_ta5_in   =   conf_wb_ta5_out ;

wire    [23:0]  wbu_ccyc_addr_in                        =   conf_ccyc_addr_out ;
wire            wbu_master_enable_in                    =   conf_pci_master_enable_out ;
wire            wbu_cache_line_size_not_zero            =   conf_cache_lsize_not_zero_to_wb_out ;
wire    [7:0]   wbu_cache_line_size_in                  =   conf_cache_line_size_to_pci_out ;

wire            wbu_pciif_gnt_in                        = pci_gnt_i ;
wire            wbu_pciif_frame_in                      = in_reg_frame_out ;
wire            wbu_pciif_irdy_in                       = in_reg_irdy_out ;
wire            wbu_pciif_trdy_in                       = int_pci_trdy  ;
wire            wbu_pciif_stop_in                       = int_pci_stop  ;
wire            wbu_pciif_devsel_in                     = int_pci_devsel ;
wire    [31:0]  wbu_pciif_ad_reg_in                     = in_reg_ad_out ;
wire            wbu_pciif_trdy_reg_in                   = in_reg_trdy_out ;
wire            wbu_pciif_stop_reg_in                   = in_reg_stop_out ;
wire            wbu_pciif_devsel_reg_in                 = in_reg_devsel_out ;


wire    [7:0]   wbu_latency_tim_val_in                  = conf_latency_tim_out ;

wire            wbu_pciif_frame_en_in                   = out_bckp_frame_en_out ;
wire            wbu_pciif_frame_out_in                  = out_bckp_frame_out ;
wire            wbu_wb_init_complete_in                 = conf_wb_init_complete_out ;

pci_wb_slave_unit wishbone_slave_unit
(
    .reset_in                      (reset),
    .wb_clock_in                   (wb_clk),
    .pci_clock_in                  (pci_clk),
    .ADDR_I                        (wbu_addr_in),
    .SDATA_I                       (wbu_sdata_in),
    .SDATA_O                       (wbu_sdata_out),
    .CYC_I                         (wbu_cyc_in),
    .STB_I                         (wbu_stb_in),
    .WE_I                          (wbu_we_in),
    .SEL_I                         (wbu_sel_in),
    .ACK_O                         (wbu_ack_out),
    .RTY_O                         (wbu_rty_out),
    .ERR_O                         (wbu_err_out),
    .CAB_I                         (wbu_cab_in),
    .wbu_map_in                    (wbu_map_in),
    .wbu_pref_en_in                (wbu_pref_en_in),
    .wbu_mrl_en_in                 (wbu_mrl_en_in),
    .wbu_pci_drcomp_pending_in     (wbu_pci_drcomp_pending_in),
    .wbu_conf_data_in              (wbu_conf_data_in),
    .wbu_pciw_empty_in             (wbu_pciw_empty_in),
    .wbu_bar0_in                   (wbu_bar0_in),
    .wbu_bar1_in                   (wbu_bar1_in),
    .wbu_bar2_in                   (wbu_bar2_in),
    .wbu_bar3_in                   (wbu_bar3_in),
    .wbu_bar4_in                   (wbu_bar4_in),
    .wbu_bar5_in                   (wbu_bar5_in),
    .wbu_am0_in                    (wbu_am0_in),
    .wbu_am1_in                    (wbu_am1_in),
    .wbu_am2_in                    (wbu_am2_in),
    .wbu_am3_in                    (wbu_am3_in),
    .wbu_am4_in                    (wbu_am4_in),
    .wbu_am5_in                    (wbu_am5_in),
    .wbu_ta0_in                    (wbu_ta0_in),
    .wbu_ta1_in                    (wbu_ta1_in),
    .wbu_ta2_in                    (wbu_ta2_in),
    .wbu_ta3_in                    (wbu_ta3_in),
    .wbu_ta4_in                    (wbu_ta4_in),
    .wbu_ta5_in                    (wbu_ta5_in),
    .wbu_at_en_in                  (wbu_at_en_in),
    .wbu_ccyc_addr_in              (wbu_ccyc_addr_in),
    .wbu_master_enable_in          (wbu_master_enable_in),
    .wb_init_complete_in           (wbu_wb_init_complete_in),
    .wbu_cache_line_size_not_zero  (wbu_cache_line_size_not_zero),
    .wbu_cache_line_size_in        (wbu_cache_line_size_in),
    .wbu_pciif_gnt_in              (wbu_pciif_gnt_in),
    .wbu_pciif_frame_in            (wbu_pciif_frame_in),
    .wbu_pciif_frame_en_in         (wbu_pciif_frame_en_in),
    .wbu_pciif_frame_out_in        (wbu_pciif_frame_out_in),
    .wbu_pciif_irdy_in             (wbu_pciif_irdy_in),
    .wbu_pciif_trdy_in             (wbu_pciif_trdy_in),
    .wbu_pciif_stop_in             (wbu_pciif_stop_in),
    .wbu_pciif_devsel_in           (wbu_pciif_devsel_in),
    .wbu_pciif_ad_reg_in           (wbu_pciif_ad_reg_in),
    .wbu_pciif_req_out             (wbu_pciif_req_out),
    .wbu_pciif_frame_out           (wbu_pciif_frame_out),
    .wbu_pciif_frame_en_out        (wbu_pciif_frame_en_out),
    .wbu_pciif_frame_load_out      (wbu_pciif_frame_load_out),
    .wbu_pciif_irdy_out            (wbu_pciif_irdy_out),
    .wbu_pciif_irdy_en_out         (wbu_pciif_irdy_en_out),
    .wbu_pciif_ad_out              (wbu_pciif_ad_out),
    .wbu_pciif_ad_en_out           (wbu_pciif_ad_en_out),
    .wbu_pciif_cbe_out             (wbu_pciif_cbe_out),
    .wbu_pciif_cbe_en_out          (wbu_pciif_cbe_en_out),
    .wbu_err_addr_out              (wbu_err_addr_out),
    .wbu_err_bc_out                (wbu_err_bc_out),
    .wbu_err_signal_out            (wbu_err_signal_out),
    .wbu_err_source_out            (wbu_err_source_out),
    .wbu_err_rty_exp_out           (wbu_err_rty_exp_out),
    .wbu_tabort_rec_out            (wbu_tabort_rec_out),
    .wbu_mabort_rec_out            (wbu_mabort_rec_out),
    .wbu_conf_offset_out           (wbu_conf_offset_out),
    .wbu_conf_renable_out          (wbu_conf_renable_out),
    .wbu_conf_wenable_out          (wbu_conf_wenable_out),
    .wbu_conf_be_out               (wbu_conf_be_out),
    .wbu_conf_data_out             (wbu_conf_data_out),
    .wbu_del_read_comp_pending_out (wbu_del_read_comp_pending_out),
    .wbu_wbw_fifo_empty_out        (wbu_wbw_fifo_empty_out),
    .wbu_latency_tim_val_in        (wbu_latency_tim_val_in),
    .wbu_ad_load_out               (wbu_ad_load_out),
    .wbu_ad_load_on_transfer_out   (wbu_ad_load_on_transfer_out),
    .wbu_pciif_trdy_reg_in         (wbu_pciif_trdy_reg_in),
    .wbu_pciif_stop_reg_in         (wbu_pciif_stop_reg_in),
    .wbu_pciif_devsel_reg_in       (wbu_pciif_devsel_reg_in)

`ifdef PCI_BIST
    ,
    .mbist_si_i       (mbist_si_i),
    .mbist_so_o       (mbist_so_o_internal),
    .mbist_ctrl_i       (mbist_ctrl_i)
`endif
);

// PCI TARGET UNIT INPUTS
wire    [31:0]  pciu_mdata_in                   =   wbm_dat_i ;
wire            pciu_ack_in                     =   wbm_ack_i ;
wire            pciu_rty_in                     =   wbm_rty_i ;
wire            pciu_err_in                     =   wbm_err_i ;

wire    [5:0]   pciu_map_in                     =   {
                                                     conf_pci_mem_io5_out,
                                                     conf_pci_mem_io4_out,
                                                     conf_pci_mem_io3_out,
                                                     conf_pci_mem_io2_out,
                                                     conf_pci_mem_io1_out,
                                                     conf_pci_mem_io0_out
                                                    } ;

wire    [5:0]   pciu_pref_en_in                 =   {
                                                     conf_pci_img_ctrl5_out[0],
                                                     conf_pci_img_ctrl4_out[0],
                                                     conf_pci_img_ctrl3_out[0],
                                                     conf_pci_img_ctrl2_out[0],
                                                     conf_pci_img_ctrl1_out[0],
                                                     conf_pci_img_ctrl0_out[0]
                                                    };

wire    [5:0]   pciu_at_en_in                   =   {
                                                     conf_pci_img_ctrl5_out[1],
                                                     conf_pci_img_ctrl4_out[1],
                                                     conf_pci_img_ctrl3_out[1],
                                                     conf_pci_img_ctrl2_out[1],
                                                     conf_pci_img_ctrl1_out[1],
                                                     conf_pci_img_ctrl0_out[1]
                                                    } ;

wire            pciu_mem_enable_in              =   conf_mem_space_enable_out ;
wire            pciu_io_enable_in               =   conf_io_space_enable_out ;

wire            pciu_wbw_fifo_empty_in          =   wbu_wbw_fifo_empty_out ;
wire			pciu_wbu_del_read_comp_pending_in	=	wbu_del_read_comp_pending_out ;
wire            pciu_wbu_frame_en_in            =   out_bckp_frame_en_out ;

`ifdef HOST
    wire    [31:0]  pciu_conf_data_in           =   conf_r_data_out ;
`else
`ifdef GUEST
    wire    [31:0]  pciu_conf_data_in           =   conf_w_data_out ;
`endif
`endif

wire    [pci_ba0_width   - 1:0] pciu_bar0_in =   conf_pci_ba0_out    ;
wire    [pci_ba1_5_width - 1:0] pciu_bar1_in =   conf_pci_ba1_out ;
wire    [pci_ba1_5_width - 1:0] pciu_bar2_in =   conf_pci_ba2_out ;
wire    [pci_ba1_5_width - 1:0] pciu_bar3_in =   conf_pci_ba3_out ;
wire    [pci_ba1_5_width - 1:0] pciu_bar4_in =   conf_pci_ba4_out ;
wire    [pci_ba1_5_width - 1:0] pciu_bar5_in =   conf_pci_ba5_out ;
wire    [pci_ba1_5_width - 1:0] pciu_am0_in  =   conf_pci_am0_out ;
wire    [pci_ba1_5_width - 1:0] pciu_am1_in  =   conf_pci_am1_out ;
wire    [pci_ba1_5_width - 1:0] pciu_am2_in  =   conf_pci_am2_out ;
wire    [pci_ba1_5_width - 1:0] pciu_am3_in  =   conf_pci_am3_out ;
wire    [pci_ba1_5_width - 1:0] pciu_am4_in  =   conf_pci_am4_out ;
wire    [pci_ba1_5_width - 1:0] pciu_am5_in  =   conf_pci_am5_out ;
wire    [pci_ba1_5_width - 1:0] pciu_ta0_in  =   conf_pci_ta0_out ;
wire    [pci_ba1_5_width - 1:0] pciu_ta1_in  =   conf_pci_ta1_out ;
wire    [pci_ba1_5_width - 1:0] pciu_ta2_in  =   conf_pci_ta2_out ;
wire    [pci_ba1_5_width - 1:0] pciu_ta3_in  =   conf_pci_ta3_out ;
wire    [pci_ba1_5_width - 1:0] pciu_ta4_in  =   conf_pci_ta4_out ;
wire    [pci_ba1_5_width - 1:0] pciu_ta5_in  =   conf_pci_ta5_out ;

wire    [7:0]   pciu_cache_line_size_in                 =   conf_cache_line_size_to_wb_out ;
wire            pciu_cache_lsize_not_zero_in            =   conf_cache_lsize_not_zero_to_wb_out ;

wire            pciu_pciif_frame_in                     =   int_pci_frame   ;
wire            pciu_pciif_irdy_in                      =   int_pci_irdy    ;
wire            pciu_pciif_idsel_in                     =   pci_idsel_i ;
wire            pciu_pciif_frame_reg_in                 =   in_reg_frame_out ;
wire            pciu_pciif_irdy_reg_in                  =   in_reg_irdy_out ;
wire            pciu_pciif_idsel_reg_in                 =   in_reg_idsel_out ;
wire    [31:0]  pciu_pciif_ad_reg_in                    =   in_reg_ad_out ;
wire    [3:0]   pciu_pciif_cbe_reg_in                   =   in_reg_cbe_out ;
wire    [3:0]   pciu_pciif_cbe_in                       =   int_pci_cbe ;

wire            pciu_pciif_bckp_trdy_en_in              =   out_bckp_trdy_en_out ;
wire            pciu_pciif_bckp_devsel_in               =   out_bckp_devsel_out ;
wire            pciu_pciif_bckp_trdy_in                 =   out_bckp_trdy_out ;
wire            pciu_pciif_bckp_stop_in                 =   out_bckp_stop_out ;
wire            pciu_pciif_trdy_reg_in                  =   in_reg_trdy_out ;
wire            pciu_pciif_stop_reg_in                  =   in_reg_stop_out ;

pci_target_unit pci_target_unit
(
    .reset_in                       (reset),
    .wb_clock_in                    (wb_clk),
    .pci_clock_in                   (pci_clk),
    .pciu_wbm_adr_o                 (pciu_adr_out),
    .pciu_wbm_dat_o                 (pciu_mdata_out),
    .pciu_wbm_dat_i                 (pciu_mdata_in),
    .pciu_wbm_cyc_o                 (pciu_cyc_out),
    .pciu_wbm_stb_o                 (pciu_stb_out),
    .pciu_wbm_we_o                  (pciu_we_out),
    .pciu_wbm_cti_o                 (pciu_cti_out),
    .pciu_wbm_bte_o                 (pciu_bte_out),
    .pciu_wbm_sel_o                 (pciu_sel_out),
    .pciu_wbm_ack_i                 (pciu_ack_in),
    .pciu_wbm_rty_i                 (pciu_rty_in),
    .pciu_wbm_err_i                 (pciu_err_in),
    .pciu_mem_enable_in             (pciu_mem_enable_in),
    .pciu_io_enable_in              (pciu_io_enable_in),
    .pciu_map_in                    (pciu_map_in),
    .pciu_pref_en_in                (pciu_pref_en_in),
    .pciu_conf_data_in              (pciu_conf_data_in),
    .pciu_wbw_fifo_empty_in         (pciu_wbw_fifo_empty_in),
    .pciu_wbu_del_read_comp_pending_in	(pciu_wbu_del_read_comp_pending_in),
    .pciu_wbu_frame_en_in           (pciu_wbu_frame_en_in),
    .pciu_bar0_in                   (pciu_bar0_in),
    .pciu_bar1_in                   (pciu_bar1_in),
    .pciu_bar2_in                   (pciu_bar2_in),
    .pciu_bar3_in                   (pciu_bar3_in),
    .pciu_bar4_in                   (pciu_bar4_in),
    .pciu_bar5_in                   (pciu_bar5_in),
    .pciu_am0_in                    (pciu_am0_in),
    .pciu_am1_in                    (pciu_am1_in),
    .pciu_am2_in                    (pciu_am2_in),
    .pciu_am3_in                    (pciu_am3_in),
    .pciu_am4_in                    (pciu_am4_in),
    .pciu_am5_in                    (pciu_am5_in),
    .pciu_ta0_in                    (pciu_ta0_in),
    .pciu_ta1_in                    (pciu_ta1_in),
    .pciu_ta2_in                    (pciu_ta2_in),
    .pciu_ta3_in                    (pciu_ta3_in),
    .pciu_ta4_in                    (pciu_ta4_in),
    .pciu_ta5_in                    (pciu_ta5_in),
    .pciu_at_en_in                  (pciu_at_en_in),
    .pciu_cache_line_size_in        (pciu_cache_line_size_in),
    .pciu_cache_lsize_not_zero_in   (pciu_cache_lsize_not_zero_in),
    .pciu_pciif_frame_in            (pciu_pciif_frame_in),
    .pciu_pciif_irdy_in             (pciu_pciif_irdy_in),
    .pciu_pciif_idsel_in            (pciu_pciif_idsel_in),
    .pciu_pciif_frame_reg_in        (pciu_pciif_frame_reg_in),
    .pciu_pciif_irdy_reg_in         (pciu_pciif_irdy_reg_in),
    .pciu_pciif_idsel_reg_in        (pciu_pciif_idsel_reg_in),
    .pciu_pciif_ad_reg_in           (pciu_pciif_ad_reg_in),
    .pciu_pciif_cbe_reg_in          (pciu_pciif_cbe_reg_in),
    .pciu_pciif_cbe_in              (pciu_pciif_cbe_in),
    .pciu_pciif_bckp_trdy_en_in     (pciu_pciif_bckp_trdy_en_in),
    .pciu_pciif_bckp_devsel_in      (pciu_pciif_bckp_devsel_in),
    .pciu_pciif_bckp_trdy_in        (pciu_pciif_bckp_trdy_in),
    .pciu_pciif_bckp_stop_in        (pciu_pciif_bckp_stop_in),
    .pciu_pciif_trdy_reg_in         (pciu_pciif_trdy_reg_in),
    .pciu_pciif_stop_reg_in         (pciu_pciif_stop_reg_in),
    .pciu_pciif_trdy_out            (pciu_pciif_trdy_out),
    .pciu_pciif_stop_out            (pciu_pciif_stop_out),
    .pciu_pciif_devsel_out          (pciu_pciif_devsel_out),
    .pciu_pciif_trdy_en_out         (pciu_pciif_trdy_en_out),
    .pciu_pciif_stop_en_out         (pciu_pciif_stop_en_out),
    .pciu_pciif_devsel_en_out       (pciu_pciif_devsel_en_out),
    .pciu_ad_load_out               (pciu_ad_load_out),
    .pciu_ad_load_on_transfer_out   (pciu_ad_load_on_transfer_out),
    .pciu_pciif_ad_out              (pciu_pciif_ad_out),
    .pciu_pciif_ad_en_out           (pciu_pciif_ad_en_out),
    .pciu_pciif_tabort_set_out      (pciu_pciif_tabort_set_out),
    .pciu_err_addr_out              (pciu_err_addr_out),
    .pciu_err_bc_out                (pciu_err_bc_out),
    .pciu_err_data_out              (pciu_err_data_out),
    .pciu_err_be_out                (pciu_err_be_out),
    .pciu_err_signal_out            (pciu_err_signal_out),
    .pciu_err_source_out            (pciu_err_source_out),
    .pciu_err_rty_exp_out           (pciu_err_rty_exp_out),
    .pciu_conf_offset_out           (pciu_conf_offset_out),
    .pciu_conf_renable_out          (pciu_conf_renable_out),
    .pciu_conf_wenable_out          (pciu_conf_wenable_out),
    .pciu_conf_be_out               (pciu_conf_be_out),
    .pciu_conf_data_out             (pciu_conf_data_out),
    .pciu_pci_drcomp_pending_out    (pciu_pci_drcomp_pending_out),
    .pciu_pciw_fifo_empty_out       (pciu_pciw_fifo_empty_out)

`ifdef PCI_BIST
    ,
    .mbist_si_i       (mbist_so_o_internal),
    .mbist_so_o       (mbist_so_o),
    .mbist_ctrl_i       (mbist_ctrl_i)
`endif
);


// CONFIGURATION SPACE INPUTS
`ifdef HOST

    wire    [11:0]  conf_w_addr_in          =       wbu_conf_offset_out ;
    wire    [31:0]  conf_w_data_in          =       wbu_conf_data_out ;
    wire            conf_w_we_in            =       wbu_conf_wenable_out ;
    wire            conf_w_re_in            =       wbu_conf_renable_out ;
    wire    [3:0]   conf_w_be_in            =       wbu_conf_be_out     ;
    wire            conf_w_clock            =       wb_clk ;
    wire    [11:0]  conf_r_addr_in          =       pciu_conf_offset_out ;
    wire            conf_r_re_in            =       pciu_conf_renable_out ;

`else
`ifdef GUEST

    wire    [11:0]  conf_r_addr_in          =       wbu_conf_offset_out ;
    wire            conf_r_re_in            =       wbu_conf_renable_out ;
    wire            conf_w_clock            =       pci_clk ;
    wire    [11:0]  conf_w_addr_in          =       pciu_conf_offset_out ;
    wire    [31:0]  conf_w_data_in          =       pciu_conf_data_out ;
    wire            conf_w_we_in            =       pciu_conf_wenable_out ;
    wire            conf_w_re_in            =       pciu_conf_renable_out ;
    wire    [3:0]   conf_w_be_in            =       pciu_conf_be_out ;

`endif
`endif


wire            conf_perr_in                            =   parchk_par_err_detect_out ;
wire            conf_serr_in                            =   parchk_sig_serr_out ;
wire            conf_master_abort_recv_in               =   wbu_mabort_rec_out ;
wire            conf_target_abort_recv_in               =   wbu_tabort_rec_out ;
wire            conf_target_abort_set_in                =   pciu_pciif_tabort_set_out ;

wire            conf_master_data_par_err_in             =   parchk_perr_mas_detect_out ;

wire    [3:0]   conf_pci_err_be_in      = pciu_err_be_out ;
wire    [3:0]   conf_pci_err_bc_in      = pciu_err_bc_out;
wire            conf_pci_err_es_in      = pciu_err_source_out ;
wire            conf_pci_err_rty_exp_in = pciu_err_rty_exp_out ;
wire            conf_pci_err_sig_in     = pciu_err_signal_out ;
wire    [31:0]  conf_pci_err_addr_in    = pciu_err_addr_out ;
wire    [31:0]  conf_pci_err_data_in    = pciu_err_data_out ;

wire    [3:0]   conf_wb_err_be_in       =   out_bckp_cbe_out ;
wire    [3:0]   conf_wb_err_bc_in       =   wbu_err_bc_out ;
wire            conf_wb_err_rty_exp_in  =   wbu_err_rty_exp_out ;
wire            conf_wb_err_es_in       =   wbu_err_source_out ;
wire            conf_wb_err_sig_in      =   wbu_err_signal_out ;
wire    [31:0]  conf_wb_err_addr_in     =   wbu_err_addr_out ;
wire    [31:0]  conf_wb_err_data_in     =   out_bckp_ad_out ;

wire            conf_isr_int_prop_in    =   pci_into_conf_isr_int_prop_out ;
wire            conf_par_err_int_in     =   parchk_perr_mas_detect_out ;
wire            conf_sys_err_int_in     =   parchk_sig_serr_out ;

pci_conf_space configuration(
                                .reset                      (reset),
                                .pci_clk                    (pci_clk),
                                .wb_clk                     (wb_clk),
                                .w_conf_address_in          (conf_w_addr_in),
                                .w_conf_data_in             (conf_w_data_in),
                                .w_conf_data_out            (conf_w_data_out),
                                .r_conf_address_in          (conf_r_addr_in),
                                .r_conf_data_out            (conf_r_data_out),
                                .w_we_i                     (conf_w_we_in),
                                .w_re                       (conf_w_re_in),
                                .r_re                       (conf_r_re_in),
                                .w_byte_en_in               (conf_w_be_in),
                                .w_clock                    (conf_w_clock),
                                .serr_enable                (conf_serr_enable_out),
                                .perr_response              (conf_perr_response_out),
                                .pci_master_enable          (conf_pci_master_enable_out),
                                .memory_space_enable        (conf_mem_space_enable_out),
                                .io_space_enable            (conf_io_space_enable_out),
                                .perr_in                    (conf_perr_in),
                                .serr_in                    (conf_serr_in),
                                .master_abort_recv          (conf_master_abort_recv_in),
                                .target_abort_recv          (conf_target_abort_recv_in),
                                .target_abort_set           (conf_target_abort_set_in),
                                .master_data_par_err        (conf_master_data_par_err_in),
                                .cache_line_size_to_pci     (conf_cache_line_size_to_pci_out),
                                .cache_line_size_to_wb      (conf_cache_line_size_to_wb_out),
                                .cache_lsize_not_zero_to_wb (conf_cache_lsize_not_zero_to_wb_out),
                                .latency_tim                (conf_latency_tim_out),
                                .pci_base_addr0             (conf_pci_ba0_out),
                                .pci_base_addr1             (conf_pci_ba1_out),
                                .pci_base_addr2             (conf_pci_ba2_out),
                                .pci_base_addr3             (conf_pci_ba3_out),
                                .pci_base_addr4             (conf_pci_ba4_out),
                                .pci_base_addr5             (conf_pci_ba5_out),
                                .pci_memory_io0             (conf_pci_mem_io0_out),
                                .pci_memory_io1             (conf_pci_mem_io1_out),
                                .pci_memory_io2             (conf_pci_mem_io2_out),
                                .pci_memory_io3             (conf_pci_mem_io3_out),
                                .pci_memory_io4             (conf_pci_mem_io4_out),
                                .pci_memory_io5             (conf_pci_mem_io5_out),
                                .pci_addr_mask0             (conf_pci_am0_out),
                                .pci_addr_mask1             (conf_pci_am1_out),
                                .pci_addr_mask2             (conf_pci_am2_out),
                                .pci_addr_mask3             (conf_pci_am3_out),
                                .pci_addr_mask4             (conf_pci_am4_out),
                                .pci_addr_mask5             (conf_pci_am5_out),
                                .pci_tran_addr0             (conf_pci_ta0_out),
                                .pci_tran_addr1             (conf_pci_ta1_out),
                                .pci_tran_addr2             (conf_pci_ta2_out),
                                .pci_tran_addr3             (conf_pci_ta3_out),
                                .pci_tran_addr4             (conf_pci_ta4_out),
                                .pci_tran_addr5             (conf_pci_ta5_out),
                                .pci_img_ctrl0              (conf_pci_img_ctrl0_out),
                                .pci_img_ctrl1              (conf_pci_img_ctrl1_out),
                                .pci_img_ctrl2              (conf_pci_img_ctrl2_out),
                                .pci_img_ctrl3              (conf_pci_img_ctrl3_out),
                                .pci_img_ctrl4              (conf_pci_img_ctrl4_out),
                                .pci_img_ctrl5              (conf_pci_img_ctrl5_out),
                                .pci_error_be               (conf_pci_err_be_in),
                                .pci_error_bc               (conf_pci_err_bc_in),
                                .pci_error_rty_exp          (conf_pci_err_rty_exp_in),
                                .pci_error_es               (conf_pci_err_es_in),
                                .pci_error_sig              (conf_pci_err_sig_in),
                                .pci_error_addr             (conf_pci_err_addr_in),
                                .pci_error_data             (conf_pci_err_data_in),
                                .wb_base_addr0              (conf_wb_ba0_out),
                                .wb_base_addr1              (conf_wb_ba1_out),
                                .wb_base_addr2              (conf_wb_ba2_out),
                                .wb_base_addr3              (conf_wb_ba3_out),
                                .wb_base_addr4              (conf_wb_ba4_out),
                                .wb_base_addr5              (conf_wb_ba5_out),
                                .wb_memory_io0              (conf_wb_mem_io0_out),
                                .wb_memory_io1              (conf_wb_mem_io1_out),
                                .wb_memory_io2              (conf_wb_mem_io2_out),
                                .wb_memory_io3              (conf_wb_mem_io3_out),
                                .wb_memory_io4              (conf_wb_mem_io4_out),
                                .wb_memory_io5              (conf_wb_mem_io5_out),
                                .wb_addr_mask0              (conf_wb_am0_out),
                                .wb_addr_mask1              (conf_wb_am1_out),
                                .wb_addr_mask2              (conf_wb_am2_out),
                                .wb_addr_mask3              (conf_wb_am3_out),
                                .wb_addr_mask4              (conf_wb_am4_out),
                                .wb_addr_mask5              (conf_wb_am5_out),
                                .wb_tran_addr0              (conf_wb_ta0_out),
                                .wb_tran_addr1              (conf_wb_ta1_out),
                                .wb_tran_addr2              (conf_wb_ta2_out),
                                .wb_tran_addr3              (conf_wb_ta3_out),
                                .wb_tran_addr4              (conf_wb_ta4_out),
                                .wb_tran_addr5              (conf_wb_ta5_out),
                                .wb_img_ctrl0               (conf_wb_img_ctrl0_out),
                                .wb_img_ctrl1               (conf_wb_img_ctrl1_out),
                                .wb_img_ctrl2               (conf_wb_img_ctrl2_out),
                                .wb_img_ctrl3               (conf_wb_img_ctrl3_out),
                                .wb_img_ctrl4               (conf_wb_img_ctrl4_out),
                                .wb_img_ctrl5               (conf_wb_img_ctrl5_out),
                                .wb_error_be                (conf_wb_err_be_in),
                                .wb_error_bc                (conf_wb_err_bc_in),
                                .wb_error_rty_exp           (conf_wb_err_rty_exp_in),
                                .wb_error_es                (conf_wb_err_es_in),
                                .wb_error_sig               (conf_wb_err_sig_in),
                                .wb_error_addr              (conf_wb_err_addr_in),
                                .wb_error_data              (conf_wb_err_data_in),
                                .config_addr                (conf_ccyc_addr_out),
                                .icr_soft_res               (conf_soft_res_out),
                                .int_out                    (conf_int_out),
                                .isr_int_prop               (conf_isr_int_prop_in),
                                .isr_par_err_int            (conf_par_err_int_in),
                                .isr_sys_err_int            (conf_sys_err_int_in),

                                .pci_init_complete_out      (conf_pci_init_complete_out),
                                .wb_init_complete_out       (conf_wb_init_complete_out)

                            `ifdef PCI_CPCI_HS_IMPLEMENT
                                ,
                                .pci_cpci_hs_enum_oe_o      (pci_cpci_hs_enum_oe_o) ,
                                .pci_cpci_hs_led_oe_o       (pci_cpci_hs_led_oe_o ) ,
                                .pci_cpci_hs_es_i           (pci_cpci_hs_es_i)
                            `endif
        
                            `ifdef PCI_SPOCI
                                ,
                                // Serial power on configuration interface
                                .spoci_scl_oe_o (spoci_scl_oe_o )  ,
                                .spoci_sda_i    (spoci_sda_i    )  ,
                                .spoci_sda_oe_o (spoci_sda_oe_o )
                            `endif
                            ) ;

// pci data io multiplexer inputs
wire            pci_mux_tar_ad_en_in            = pciu_pciif_ad_en_out ;
wire            pci_mux_tar_ad_en_reg_in        = out_bckp_tar_ad_en_out ;
wire    [31:0]  pci_mux_tar_ad_in               = pciu_pciif_ad_out ;
wire            pci_mux_devsel_in               = pciu_pciif_devsel_out ;
wire            pci_mux_devsel_en_in            = pciu_pciif_devsel_en_out ;
wire            pci_mux_trdy_in                 = pciu_pciif_trdy_out ;
wire            pci_mux_trdy_en_in              = pciu_pciif_trdy_en_out ;
wire            pci_mux_stop_in                 = pciu_pciif_stop_out ;
wire            pci_mux_stop_en_in              = pciu_pciif_stop_en_out ;
wire            pci_mux_tar_load_in             = pciu_ad_load_out ;
wire            pci_mux_tar_load_on_transfer_in = pciu_ad_load_on_transfer_out ;

wire            pci_mux_mas_ad_en_in    = wbu_pciif_ad_en_out ;
wire    [31:0]  pci_mux_mas_ad_in       = wbu_pciif_ad_out ;

wire            pci_mux_frame_in                = wbu_pciif_frame_out ;
wire            pci_mux_frame_en_in             = wbu_pciif_frame_en_out ;
wire            pci_mux_irdy_in                 = wbu_pciif_irdy_out;
wire            pci_mux_irdy_en_in              = wbu_pciif_irdy_en_out;
wire            pci_mux_mas_load_in             = wbu_ad_load_out ;
wire            pci_mux_mas_load_on_transfer_in = wbu_ad_load_on_transfer_out ;
wire [3:0]      pci_mux_cbe_in                  = wbu_pciif_cbe_out ;
wire            pci_mux_cbe_en_in               = wbu_pciif_cbe_en_out ;

wire            pci_mux_par_in              = parchk_pci_par_out ;
wire            pci_mux_par_en_in           = parchk_pci_par_en_out ;
wire            pci_mux_perr_in             = parchk_pci_perr_out ;
wire            pci_mux_perr_en_in          = parchk_pci_perr_en_out ;
wire            pci_mux_serr_in             = parchk_pci_serr_out ;
wire            pci_mux_serr_en_in          = parchk_pci_serr_en_out;

wire            pci_mux_req_in              =   wbu_pciif_req_out ;
wire            pci_mux_frame_load_in       =   wbu_pciif_frame_load_out ;

wire            pci_mux_pci_irdy_in         =   int_pci_irdy    ;
wire            pci_mux_pci_trdy_in         =   int_pci_trdy    ;
wire            pci_mux_pci_frame_in        =   int_pci_frame   ;
wire            pci_mux_pci_stop_in         =   int_pci_stop    ;

wire            pci_mux_init_complete_in    =   conf_pci_init_complete_out ;

pci_io_mux pci_io_mux
(
    .reset_in                   (reset),
    .clk_in                     (pci_clk),
    .frame_in                   (pci_mux_frame_in),
    .frame_en_in                (pci_mux_frame_en_in),
    .frame_load_in              (pci_mux_frame_load_in),
    .irdy_in                    (pci_mux_irdy_in),
    .irdy_en_in                 (pci_mux_irdy_en_in),
    .devsel_in                  (pci_mux_devsel_in),
    .devsel_en_in               (pci_mux_devsel_en_in),
    .trdy_in                    (pci_mux_trdy_in),
    .trdy_en_in                 (pci_mux_trdy_en_in),
    .stop_in                    (pci_mux_stop_in),
    .stop_en_in                 (pci_mux_stop_en_in),
    .master_load_in             (pci_mux_mas_load_in),
    .master_load_on_transfer_in (pci_mux_mas_load_on_transfer_in),
    .target_load_in             (pci_mux_tar_load_in),
    .target_load_on_transfer_in (pci_mux_tar_load_on_transfer_in),
    .cbe_in                     (pci_mux_cbe_in),
    .cbe_en_in                  (pci_mux_cbe_en_in),
    .mas_ad_in                  (pci_mux_mas_ad_in),
    .tar_ad_in                  (pci_mux_tar_ad_in),

    .mas_ad_en_in               (pci_mux_mas_ad_en_in),
    .tar_ad_en_in               (pci_mux_tar_ad_en_in),
    .tar_ad_en_reg_in           (pci_mux_tar_ad_en_reg_in),

    .par_in                     (pci_mux_par_in),
    .par_en_in                  (pci_mux_par_en_in),
    .perr_in                    (pci_mux_perr_in),
    .perr_en_in                 (pci_mux_perr_en_in),
    .serr_in                    (pci_mux_serr_in),
    .serr_en_in                 (pci_mux_serr_en_in),

    .frame_en_out               (pci_mux_frame_en_out),
    .irdy_en_out                (pci_mux_irdy_en_out),
    .devsel_en_out              (pci_mux_devsel_en_out),
    .trdy_en_out                (pci_mux_trdy_en_out),
    .stop_en_out                (pci_mux_stop_en_out),
    .cbe_en_out                 (pci_mux_cbe_en_out),
    .ad_en_out                  (pci_mux_ad_en_out),

    .frame_out                  (pci_mux_frame_out),
    .irdy_out                   (pci_mux_irdy_out),
    .devsel_out                 (pci_mux_devsel_out),
    .trdy_out                   (pci_mux_trdy_out),
    .stop_out                   (pci_mux_stop_out),
    .cbe_out                    (pci_mux_cbe_out),
    .ad_out                     (pci_mux_ad_out),
    .ad_load_out                (pci_mux_ad_load_out),

    .par_out                    (pci_mux_par_out),
    .par_en_out                 (pci_mux_par_en_out),
    .perr_out                   (pci_mux_perr_out),
    .perr_en_out                (pci_mux_perr_en_out),
    .serr_out                   (pci_mux_serr_out),
    .serr_en_out                (pci_mux_serr_en_out),
    .req_in                     (pci_mux_req_in),
    .req_out                    (pci_mux_req_out),
    .req_en_out                 (pci_mux_req_en_out),
    .pci_irdy_in                (pci_mux_pci_irdy_in),
    .pci_trdy_in                (pci_mux_pci_trdy_in),
    .pci_frame_in               (pci_mux_pci_frame_in),
    .pci_stop_in                (pci_mux_pci_stop_in),
    .ad_en_unregistered_out     (pci_mux_ad_en_unregistered_out),

    .init_complete_in           (pci_mux_init_complete_in)
);

pci_cur_out_reg output_backup
(
    .reset_in               (reset),
    .clk_in                 (pci_clk),
    .frame_in               (pci_mux_frame_in),
    .frame_en_in            (pci_mux_frame_en_in),
    .frame_load_in          (pci_mux_frame_load_in),
    .irdy_in                (pci_mux_irdy_in),
    .irdy_en_in             (pci_mux_irdy_en_in),
    .devsel_in              (pci_mux_devsel_in),
    .trdy_in                (pci_mux_trdy_in),
    .trdy_en_in             (pci_mux_trdy_en_in),
    .stop_in                (pci_mux_stop_in),
    .ad_load_in             (pci_mux_ad_load_out),
    .cbe_in                 (pci_mux_cbe_in),
    .cbe_en_in              (pci_mux_cbe_en_in),
    .mas_ad_in              (pci_mux_mas_ad_in),
    .tar_ad_in              (pci_mux_tar_ad_in),

    .mas_ad_en_in           (pci_mux_mas_ad_en_in),
    .tar_ad_en_in           (pci_mux_tar_ad_en_in),
    .ad_en_unregistered_in  (pci_mux_ad_en_unregistered_out),

    .par_in                 (pci_mux_par_in),
    .par_en_in              (pci_mux_par_en_in),
    .perr_in                (pci_mux_perr_in),
    .perr_en_in             (pci_mux_perr_en_in),
    .serr_in                (pci_mux_serr_in),
    .serr_en_in             (pci_mux_serr_en_in),

    .frame_out              (out_bckp_frame_out),
    .frame_en_out           (out_bckp_frame_en_out),
    .irdy_out               (out_bckp_irdy_out),
    .irdy_en_out            (out_bckp_irdy_en_out),
    .devsel_out             (out_bckp_devsel_out),
    .trdy_out               (out_bckp_trdy_out),
    .trdy_en_out            (out_bckp_trdy_en_out),
    .stop_out               (out_bckp_stop_out),
    .cbe_out                (out_bckp_cbe_out),
    .ad_out                 (out_bckp_ad_out),
    .ad_en_out              (out_bckp_ad_en_out),
    .cbe_en_out             (out_bckp_cbe_en_out),
    .tar_ad_en_out          (out_bckp_tar_ad_en_out),
    .mas_ad_en_out          (out_bckp_mas_ad_en_out),

    .par_out                (out_bckp_par_out),
    .par_en_out             (out_bckp_par_en_out),
    .perr_out               (out_bckp_perr_out),
    .perr_en_out            (out_bckp_perr_en_out),
    .serr_out               (out_bckp_serr_out),
    .serr_en_out            (out_bckp_serr_en_out)
) ;

// PARITY CHECKER INPUTS
wire            parchk_pci_par_in               =   int_pci_par ;
wire            parchk_pci_perr_in              =   int_pci_perr ;
wire            parchk_pci_frame_reg_in         =   in_reg_frame_out ;
wire            parchk_pci_frame_en_in          =   out_bckp_frame_en_out ;
wire            parchk_pci_irdy_en_in           =   out_bckp_irdy_en_out ;
wire            parchk_pci_irdy_reg_in          =   in_reg_irdy_out ;
wire            parchk_pci_trdy_reg_in          =   in_reg_trdy_out ;


wire            parchk_pci_trdy_en_in           =   out_bckp_trdy_en_out ;


wire    [31:0]  parchk_pci_ad_out_in            =   out_bckp_ad_out ;
wire    [31:0]  parchk_pci_ad_reg_in            =   in_reg_ad_out ;
wire    [3:0]   parchk_pci_cbe_in_in            =   int_pci_cbe   ;
wire    [3:0]   parchk_pci_cbe_reg_in           =   in_reg_cbe_out ;
wire    [3:0]   parchk_pci_cbe_out_in           =   out_bckp_cbe_out ;
wire            parchk_pci_ad_en_in             =   out_bckp_ad_en_out ;
wire            parchk_par_err_response_in      =   conf_perr_response_out ;
wire            parchk_serr_enable_in           =   conf_serr_enable_out ;

wire            parchk_pci_perr_out_in          =   out_bckp_perr_out ;
wire            parchk_pci_serr_en_in           =   out_bckp_serr_en_out ;
wire            parchk_pci_serr_out_in          =   out_bckp_serr_out ;
wire            parchk_pci_cbe_en_in            =   out_bckp_cbe_en_out ;

wire            parchk_pci_par_en_in            =   out_bckp_par_en_out ;

pci_parity_check parity_checker
(
    .reset_in               (reset),
    .clk_in                 (pci_clk),
    .pci_par_in             (parchk_pci_par_in),
    .pci_par_out            (parchk_pci_par_out),
    .pci_par_en_out         (parchk_pci_par_en_out),
    .pci_par_en_in          (parchk_pci_par_en_in),
    .pci_perr_in            (parchk_pci_perr_in),
    .pci_perr_out           (parchk_pci_perr_out),
    .pci_perr_en_out        (parchk_pci_perr_en_out),
    .pci_perr_out_in        (parchk_pci_perr_out_in),
    .pci_serr_out           (parchk_pci_serr_out),
    .pci_serr_out_in        (parchk_pci_serr_out_in),
    .pci_serr_en_out        (parchk_pci_serr_en_out),
    .pci_serr_en_in         (parchk_pci_serr_en_in),
    .pci_frame_reg_in       (parchk_pci_frame_reg_in),
    .pci_frame_en_in        (parchk_pci_frame_en_in),
    .pci_irdy_en_in         (parchk_pci_irdy_en_in),
    .pci_irdy_reg_in        (parchk_pci_irdy_reg_in),
    .pci_trdy_reg_in        (parchk_pci_trdy_reg_in),
    .pci_trdy_en_in         (parchk_pci_trdy_en_in),
    .pci_ad_out_in          (parchk_pci_ad_out_in),
    .pci_ad_reg_in          (parchk_pci_ad_reg_in),
    .pci_cbe_in_in          (parchk_pci_cbe_in_in),
    .pci_cbe_reg_in         (parchk_pci_cbe_reg_in),
    .pci_cbe_en_in          (parchk_pci_cbe_en_in),
    .pci_cbe_out_in         (parchk_pci_cbe_out_in),
    .pci_ad_en_in           (parchk_pci_ad_en_in),
    .par_err_response_in    (parchk_par_err_response_in),
    .par_err_detect_out     (parchk_par_err_detect_out),
    .perr_mas_detect_out    (parchk_perr_mas_detect_out),
    .serr_enable_in         (parchk_serr_enable_in),
    .sig_serr_out           (parchk_sig_serr_out)
);

wire            in_reg_gnt_in    = pci_gnt_i ;
wire            in_reg_frame_in  = int_pci_frame ;
wire            in_reg_irdy_in   = int_pci_irdy  ;
wire            in_reg_trdy_in   = int_pci_trdy  ;
wire            in_reg_stop_in   = int_pci_stop  ;
wire            in_reg_devsel_in = int_pci_devsel ;
wire            in_reg_idsel_in  = pci_idsel_i ;
wire    [31:0]  in_reg_ad_in     = pci_ad_i ;
wire    [3:0]   in_reg_cbe_in    = int_pci_cbe ;

pci_in_reg input_register
(
    .reset_in           (reset),
    .clk_in             (pci_clk),
    .init_complete_in   (conf_pci_init_complete_out),

    .pci_gnt_in     (in_reg_gnt_in),
    .pci_frame_in   (in_reg_frame_in),
    .pci_irdy_in    (in_reg_irdy_in),
    .pci_trdy_in    (in_reg_trdy_in),
    .pci_stop_in    (in_reg_stop_in),
    .pci_devsel_in  (in_reg_devsel_in),
    .pci_idsel_in   (in_reg_idsel_in),
    .pci_ad_in      (in_reg_ad_in),
    .pci_cbe_in     (in_reg_cbe_in),

    .pci_gnt_reg_out    (in_reg_gnt_out),
    .pci_frame_reg_out  (in_reg_frame_out),
    .pci_irdy_reg_out   (in_reg_irdy_out),
    .pci_trdy_reg_out   (in_reg_trdy_out),
    .pci_stop_reg_out   (in_reg_stop_out),
    .pci_devsel_reg_out (in_reg_devsel_out),
    .pci_idsel_reg_out  (in_reg_idsel_out),
    .pci_ad_reg_out     (in_reg_ad_out),
    .pci_cbe_reg_out    (in_reg_cbe_out)
);

endmodule
