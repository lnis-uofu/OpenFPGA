//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: pci_rst_int.v                                    ////
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
// $Log: pci_rst_int.v,v $
// Revision 1.3  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.2  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.1  2002/02/01 14:43:31  mihad
// *** empty log message ***
//
//
//

`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

// Module is used to switch appropriate reset and interrupt signals with few logic
module pci_rst_int
(
	clk_in,
	// reset signals
	rst_i,
	pci_rstn_in,
	conf_soft_res_in,
	reset,
	pci_rstn_out,
	pci_rstn_en_out,
	rst_o,
	// interrupt signals
	pci_intan_in,
	conf_int_in,
	int_i,
	pci_intan_out,
	pci_intan_en_out,
	int_o,
	conf_isr_int_prop_out,
    init_complete_in
);

input	clk_in;
// RESET inputs and outputs
input	rst_i;
input	pci_rstn_in;
input	conf_soft_res_in;
output	reset;
output	pci_rstn_out;
output	pci_rstn_en_out;
output	rst_o;

// INTERRUPT inputs and outputs
input	pci_intan_in;
input	conf_int_in;
input	int_i;
output	pci_intan_out;
output	pci_intan_en_out;
output	int_o;
output	conf_isr_int_prop_out;

input   init_complete_in ;

/*--------------------------------------------------------------------------------------------------------
RESET logic
--------------------------------------------------------------------------------------------------------*/
assign pci_rstn_out			= 1'b0 ;
// host implementation of the bridge gets its reset from WISHBONE bus - RST_I and propagates it to PCI bus
`ifdef HOST
  assign reset				= rst_i ;
  `ifdef ACTIVE_LOW_OE
  assign pci_rstn_en_out	= ~(rst_i || conf_soft_res_in) ;
  `else
  assign pci_rstn_en_out        = rst_i || conf_soft_res_in ;
  `endif
  assign rst_o				= 1'b0 ;
`else
// guest implementation of the bridge gets its reset from PCI bus - RST# and propagates it to WISHBONE bus
`ifdef GUEST
  assign reset 				= ~pci_rstn_in ;
  assign rst_o 				= (~pci_rstn_in) || conf_soft_res_in ;
  `ifdef ACTIVE_LOW_OE
  assign pci_rstn_en_out	        = 1'b1 ; // disabled
  `else
  assign pci_rstn_en_out                = 1'b0 ; // disabled
  `endif
`endif
`endif

/*--------------------------------------------------------------------------------------------------------
INTERRUPT logic
--------------------------------------------------------------------------------------------------------*/
assign pci_intan_out = 1'b0 ;
// host implementation of the bridge gets its interrupt from PCI bus - INTA# and propagates it to WISHBONE bus
`ifdef HOST
  assign conf_isr_int_prop_out  = ~pci_intan_in ;
  assign int_o                  = conf_int_in ;
  `ifdef ACTIVE_LOW_OE
  assign pci_intan_en_out       = 1'b1 ; // disabled
  `else
  assign pci_intan_en_out       = 1'b0 ; // disabled
  `endif
`else
// guest implementation of the bridge gets its interrupt from WISHBONE bus - INT_I and propagates it to PCI bus
`ifdef GUEST
    wire interrupt_a_en;
    pci_out_reg inta
    (
        .reset_in     ( reset ),
        .clk_in       ( clk_in) ,
        .dat_en_in    ( 1'b1 ),
        .en_en_in     ( init_complete_in ),
        .dat_in       ( 1'b0 ) , // active low
        .en_in        ( conf_int_in ) ,
        .en_out       ( interrupt_a_en ),
        .dat_out      ( )
    );
  assign conf_isr_int_prop_out = int_i ;
  assign int_o                 = 1'b0 ;
  assign pci_intan_en_out      = interrupt_a_en ;
`endif
`endif


endmodule
