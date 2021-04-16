//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pci_io_mux.v"                                    ////
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
// $Log: pci_io_mux.v,v $
// Revision 1.5  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
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

// this module instantiates output flip flops for PCI interface and
// some fanout downsizing logic because of heavily constrained PCI signals

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_io_mux
(
    reset_in,
    clk_in,
    frame_in,
    frame_en_in,
    frame_load_in,
    irdy_in,
    irdy_en_in,
    devsel_in,
    devsel_en_in,
    trdy_in,
    trdy_en_in,
    stop_in,
    stop_en_in,
    master_load_in,
    master_load_on_transfer_in,
    target_load_in,
    target_load_on_transfer_in,
    cbe_in,
    cbe_en_in,
    mas_ad_in,
    tar_ad_in,

    par_in,
    par_en_in,
    perr_in,
    perr_en_in,
    serr_in,
    serr_en_in,

    req_in,

    mas_ad_en_in,
    tar_ad_en_in,
    tar_ad_en_reg_in,

    ad_en_out,
    frame_en_out,
    irdy_en_out,
    devsel_en_out,
    trdy_en_out,
    stop_en_out,
    cbe_en_out,

    frame_out,
    irdy_out,
    devsel_out,
    trdy_out,
    stop_out,
    cbe_out,
    ad_out,
    ad_load_out,
    ad_en_unregistered_out,

    par_out,
    par_en_out,
    perr_out,
    perr_en_out,
    serr_out,
    serr_en_out,

    req_out,
    req_en_out,
    pci_trdy_in,
    pci_irdy_in,
    pci_frame_in,
    pci_stop_in,

    init_complete_in
);

input reset_in, clk_in ;

input           frame_in ;
input           frame_en_in ;
input           frame_load_in ;
input           irdy_in ;
input           irdy_en_in ;
input           devsel_in ;
input           devsel_en_in ;
input           trdy_in ;
input           trdy_en_in ;
input           stop_in ;
input           stop_en_in ;
input           master_load_in ;
input           target_load_in ;

input [3:0]     cbe_in ;
input           cbe_en_in ;
input [31:0]    mas_ad_in ;
input [31:0]    tar_ad_in ;

input           mas_ad_en_in ;
input           tar_ad_en_in ;
input           tar_ad_en_reg_in ;

input par_in ;
input par_en_in ;
input perr_in ;
input perr_en_in ;
input serr_in ;
input serr_en_in ;

output          frame_en_out ;
output          irdy_en_out ;
output          devsel_en_out ;
output          trdy_en_out ;
output          stop_en_out ;
output [31:0]   ad_en_out ;
output [3:0]    cbe_en_out ;

output          frame_out ;
output          irdy_out ;
output          devsel_out ;
output          trdy_out ;
output          stop_out ;
output [3:0]    cbe_out ;
output [31:0]   ad_out ;
output          ad_load_out ;
output          ad_en_unregistered_out ;

output          par_out ;
output          par_en_out ;
output          perr_out ;
output          perr_en_out ;
output          serr_out ;
output          serr_en_out ;

input           req_in ;

output          req_out ;
output          req_en_out ;

input           pci_trdy_in,
                pci_irdy_in,
                pci_frame_in,
                pci_stop_in ;

input           master_load_on_transfer_in ;
input           target_load_on_transfer_in ;

input           init_complete_in    ;

wire   [31:0]   temp_ad = tar_ad_en_reg_in ? tar_ad_in : mas_ad_in ;

wire ad_en_ctrl_low ;

wire ad_en_ctrl_mlow ;

wire ad_en_ctrl_mhigh ;

wire ad_en_ctrl_high ;

wire ad_enable_internal = mas_ad_en_in || tar_ad_en_in ;

pci_io_mux_ad_en_crit ad_en_low_gen
(
    .ad_en_in       (ad_enable_internal),
    .pci_frame_in   (pci_frame_in),
    .pci_trdy_in    (pci_trdy_in),
    .pci_stop_in    (pci_stop_in),
    .ad_en_out      (ad_en_ctrl_low)
);

pci_io_mux_ad_en_crit ad_en_mlow_gen
(
    .ad_en_in       (ad_enable_internal),
    .pci_frame_in   (pci_frame_in),
    .pci_trdy_in    (pci_trdy_in),
    .pci_stop_in    (pci_stop_in),
    .ad_en_out      (ad_en_ctrl_mlow)
);

pci_io_mux_ad_en_crit ad_en_mhigh_gen
(
    .ad_en_in       (ad_enable_internal),
    .pci_frame_in   (pci_frame_in),
    .pci_trdy_in    (pci_trdy_in),
    .pci_stop_in    (pci_stop_in),
    .ad_en_out      (ad_en_ctrl_mhigh)
);

pci_io_mux_ad_en_crit ad_en_high_gen
(
    .ad_en_in       (ad_enable_internal),
    .pci_frame_in   (pci_frame_in),
    .pci_trdy_in    (pci_trdy_in),
    .pci_stop_in    (pci_stop_in),
    .ad_en_out      (ad_en_ctrl_high)
);

assign ad_en_unregistered_out = ad_en_ctrl_high ;

wire load = master_load_in || target_load_in ;
wire load_on_transfer = master_load_on_transfer_in || target_load_on_transfer_in ;

wire   ad_load_ctrl_low ;
wire   ad_load_ctrl_mlow ;
wire   ad_load_ctrl_mhigh ;
wire   ad_load_ctrl_high ;

assign ad_load_out = ad_load_ctrl_high ;

pci_io_mux_ad_load_crit ad_load_low_gen
(
    .load_in(load),
    .load_on_transfer_in(load_on_transfer),
    .pci_irdy_in(pci_irdy_in),
    .pci_trdy_in(pci_trdy_in),
    .load_out(ad_load_ctrl_low)
);

pci_io_mux_ad_load_crit ad_load_mlow_gen
(
    .load_in(load),
    .load_on_transfer_in(load_on_transfer),
    .pci_irdy_in(pci_irdy_in),
    .pci_trdy_in(pci_trdy_in),
    .load_out(ad_load_ctrl_mlow)
);

pci_io_mux_ad_load_crit ad_load_mhigh_gen
(
    .load_in(load),
    .load_on_transfer_in(load_on_transfer),
    .pci_irdy_in(pci_irdy_in),
    .pci_trdy_in(pci_trdy_in),
    .load_out(ad_load_ctrl_mhigh)
);

pci_io_mux_ad_load_crit ad_load_high_gen
(
    .load_in(load),
    .load_on_transfer_in(load_on_transfer),
    .pci_irdy_in(pci_irdy_in),
    .pci_trdy_in(pci_trdy_in),
    .load_out(ad_load_ctrl_high)
);

pci_out_reg ad_iob0
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[0] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[0] ),
    .dat_out      ( ad_out[0] )
);

pci_out_reg ad_iob1
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[1] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[1] ),
    .dat_out      ( ad_out[1] )
);

pci_out_reg ad_iob2
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[2] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[2] ),
    .dat_out      ( ad_out[2] )
);

pci_out_reg ad_iob3
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[3] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[3] ),
    .dat_out      ( ad_out[3] )
);

pci_out_reg ad_iob4
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[4] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[4] ),
    .dat_out      ( ad_out[4] )
);

pci_out_reg ad_iob5
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[5] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[5] ),
    .dat_out      ( ad_out[5] )
);

pci_out_reg ad_iob6
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[6] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[6] ),
    .dat_out      ( ad_out[6] )
);

pci_out_reg ad_iob7
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_low ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[7] ) ,
    .en_in        ( ad_en_ctrl_low ) ,
    .en_out       ( ad_en_out[7] ),
    .dat_out      ( ad_out[7] )
);

pci_out_reg ad_iob8
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[8] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[8] ),
    .dat_out      ( ad_out[8] )
);

pci_out_reg ad_iob9
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[9] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[9] ),
    .dat_out      ( ad_out[9] )
);

pci_out_reg ad_iob10
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[10] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[10] ),
    .dat_out      ( ad_out[10] )
);

pci_out_reg ad_iob11
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[11] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[11] ),
    .dat_out      ( ad_out[11] )
);

pci_out_reg ad_iob12
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[12] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[12] ),
    .dat_out      ( ad_out[12] )
);

pci_out_reg ad_iob13
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[13] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[13] ),
    .dat_out      ( ad_out[13] )
);

pci_out_reg ad_iob14
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[14] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[14] ),
    .dat_out      ( ad_out[14] )
);

pci_out_reg ad_iob15
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mlow ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[15] ) ,
    .en_in        ( ad_en_ctrl_mlow ) ,
    .en_out       ( ad_en_out[15] ),
    .dat_out      ( ad_out[15] )
);

pci_out_reg ad_iob16
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[16] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[16] ),
    .dat_out      ( ad_out[16] )
);

pci_out_reg ad_iob17
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[17] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[17] ),
    .dat_out      ( ad_out[17] )
);

pci_out_reg ad_iob18
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[18] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[18] ),
    .dat_out      ( ad_out[18] )
);

pci_out_reg ad_iob19
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[19] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[19] ),
    .dat_out      ( ad_out[19] )
);

pci_out_reg ad_iob20
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[20] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[20] ),
    .dat_out      ( ad_out[20] )
);

pci_out_reg ad_iob21
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[21] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[21] ),
    .dat_out      ( ad_out[21] )
);

pci_out_reg ad_iob22
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[22] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[22] ),
    .dat_out      ( ad_out[22] )
);

pci_out_reg ad_iob23
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_mhigh ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[23] ) ,
    .en_in        ( ad_en_ctrl_mhigh ) ,
    .en_out       ( ad_en_out[23] ),
    .dat_out      ( ad_out[23] )
);

pci_out_reg ad_iob24
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[24] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[24] ),
    .dat_out      ( ad_out[24] )
);

pci_out_reg ad_iob25
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[25] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[25] ),
    .dat_out      ( ad_out[25] )
);

pci_out_reg ad_iob26
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[26] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[26] ),
    .dat_out      ( ad_out[26] )
);

pci_out_reg ad_iob27
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[27] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[27] ),
    .dat_out      ( ad_out[27] )
);

pci_out_reg ad_iob28
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[28] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[28] ),
    .dat_out      ( ad_out[28] )
);

pci_out_reg ad_iob29
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[29] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[29] ),
    .dat_out      ( ad_out[29] )
);

pci_out_reg ad_iob30
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[30] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[30] ),
    .dat_out      ( ad_out[30] )
);

pci_out_reg ad_iob31
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( ad_load_ctrl_high ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( temp_ad[31] ) ,
    .en_in        ( ad_en_ctrl_high ) ,
    .en_out       ( ad_en_out[31] ),
    .dat_out      ( ad_out[31] )
);

wire [3:0] cbe_load_ctrl = {4{ master_load_in }} ;
wire [3:0] cbe_en_ctrl   = {4{ cbe_en_in }} ;

pci_out_reg cbe_iob0
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( cbe_load_ctrl[0] ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( cbe_in[0] ) ,
    .en_in        ( cbe_en_ctrl[0] ) ,
    .en_out       ( cbe_en_out[0] ),
    .dat_out      ( cbe_out[0] )
);

pci_out_reg cbe_iob1
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( cbe_load_ctrl[1] ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( cbe_in[1] ) ,
    .en_in        ( cbe_en_ctrl[1] ) ,
    .en_out       ( cbe_en_out[1] ),
    .dat_out      ( cbe_out[1] )
);

pci_out_reg cbe_iob2
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( cbe_load_ctrl[2] ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( cbe_in[2] ) ,
    .en_in        ( cbe_en_ctrl[2] ) ,
    .en_out       ( cbe_en_out[2] ),
    .dat_out      ( cbe_out[2] )
);

pci_out_reg cbe_iob3
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( cbe_load_ctrl[3] ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( cbe_in[3] ) ,
    .en_in        ( cbe_en_ctrl[3] ) ,
    .en_out       ( cbe_en_out[3] ),
    .dat_out      ( cbe_out[3] )
);

pci_out_reg frame_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( frame_load_in ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( frame_in ) ,
    .en_in        ( frame_en_in ) ,
    .en_out       ( frame_en_out ),
    .dat_out      ( frame_out )
);

pci_out_reg irdy_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( irdy_in ) ,
    .en_in        ( irdy_en_in ) ,
    .en_out       ( irdy_en_out ),
    .dat_out      ( irdy_out )
);

pci_out_reg trdy_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( trdy_in ) ,
    .en_in        ( trdy_en_in ) ,
    .en_out       ( trdy_en_out ),
    .dat_out      ( trdy_out )
);

pci_out_reg stop_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( stop_in ) ,
    .en_in        ( stop_en_in ) ,
    .en_out       ( stop_en_out ),
    .dat_out      ( stop_out )
);

pci_out_reg devsel_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( devsel_in ) ,
    .en_in        ( devsel_en_in ) ,
    .en_out       ( devsel_en_out ),
    .dat_out      ( devsel_out )
);

pci_out_reg par_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( par_in ) ,
    .en_in        ( par_en_in ) ,
    .en_out       ( par_en_out ),
    .dat_out      ( par_out )
);

pci_out_reg perr_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( perr_in ) ,
    .en_in        ( perr_en_in ) ,
    .en_out       ( perr_en_out ),
    .dat_out      ( perr_out )
);

pci_out_reg serr_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( serr_in ) ,
    .en_in        ( serr_en_in ) ,
    .en_out       ( serr_en_out ),
    .dat_out      ( serr_out )
);

pci_out_reg req_iob
(
    .reset_in     ( reset_in ),
    .clk_in       ( clk_in) ,
    .dat_en_in    ( 1'b1 ),
    .en_en_in     ( 1'b1 ),
    .dat_in       ( req_in ) ,
    .en_in        ( init_complete_in ) ,
    .en_out       ( req_en_out ),
    .dat_out      ( req_out )
);

endmodule
