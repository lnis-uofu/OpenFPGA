//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pci_constants.v"                                 ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////      - Tadej Markovic (tadej@opencores.org)                  ////
////                                                              ////
////  All additional information is avaliable in the README.txt   ////
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
// $Log: pci_constants.v,v $
// Revision 1.2  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.1  2002/02/01 14:43:31  mihad
// *** empty log message ***
//
// Revision 1.2  2001/10/05 08:14:28  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:46  mihad
// New project directory structure
//

// first include user definable parameters
`ifdef REGRESSION // Used only for regression testing purposes!!!
	`include "pci_regression_constants.v"
`else
	`include "pci_user_constants.v"
`endif

////////////////////////////////////////////////////////////////////////
////                                                                ////
//// FIFO parameters define behaviour of FIFO control logic and     ////
//// FIFO depths.                                                   ////
////                                                                ////
////////////////////////////////////////////////////////////////////////
`define WBW_DEPTH (1 << `WBW_ADDR_LENGTH)
`define WBR_DEPTH (1 << `WBR_ADDR_LENGTH)
`define PCIW_DEPTH (1 << `PCIW_ADDR_LENGTH)
`define PCIR_DEPTH (1 << `PCIR_ADDR_LENGTH)

// defines on which bit in control bus means what
`define ADDR_CTRL_BIT 3
`define LAST_CTRL_BIT 0
`define DATA_ERROR_CTRL_BIT 1
`define UNUSED_CTRL_BIT 2
`define	BURST_BIT 2

// MAX Retry counter value for PCI Master state-machine
// 	This value is 8-bit because of 8-bit retry counter !!!
//`define PCI_RTY_CNT_MAX			8'h08

// Value of address mask for WB configuration image. This has to be defined always, since it is a value, that is not changable in runtime.
// !!!!!!!!!!!!!!!!!!!!!!!If this is not defined, WB configuration access will not be possible!!!!!!!!!!!!!!!!!!!!!1
`define WB_AM0 20'hffff_f

// PCI target & WB slave ADDRESS names for configuration space !!!
// This does not include address offsets of PCI Header registers - they starts at offset 0 (see PCI spec.)
//   ALL VALUES are without 2 LSBits AND there is required that address bit [8] is set while
//   accessing this registers, otherwise the configuration header will be accessed !!!
`define PCI_CAP_PTR_VAL         8'h80
`define P_IMG_CTRL0_ADDR		6'h00	//	Address offset = h 100
`define P_BA0_ADDR				6'h01	//	Address offset = h 104
`define P_AM0_ADDR				6'h02   //	Address offset = h 108
`define P_TA0_ADDR				6'h03   //	Address offset = h 10c
`define P_IMG_CTRL1_ADDR        6'h04   //	Address offset = h 110
`define	P_BA1_ADDR				6'h05   //	Address offset = h 114
`define	P_AM1_ADDR				6'h06   //	Address offset = h 118
`define	P_TA1_ADDR				6'h07   //	Address offset = h 11c
`define	P_IMG_CTRL2_ADDR		6'h08   //	Address offset = h 120
`define	P_BA2_ADDR				6'h09   //	Address offset = h 124
`define	P_AM2_ADDR				6'h0a   //	Address offset = h 128
`define	P_TA2_ADDR				6'h0b   //	Address offset = h 12c
`define	P_IMG_CTRL3_ADDR		6'h0c   //	Address offset = h 130
`define	P_BA3_ADDR				6'h0d   //	Address offset = h 134
`define	P_AM3_ADDR				6'h0e   //	Address offset = h 138
`define	P_TA3_ADDR				6'h0f   //	Address offset = h 13c
`define	P_IMG_CTRL4_ADDR		6'h10   //	Address offset = h 140
`define	P_BA4_ADDR				6'h11   //	Address offset = h 144
`define	P_AM4_ADDR				6'h12   //	Address offset = h 148
`define	P_TA4_ADDR				6'h13   //	Address offset = h 14c
`define	P_IMG_CTRL5_ADDR		6'h14   //	Address offset = h 150
`define	P_BA5_ADDR				6'h15   //	Address offset = h 154
`define	P_AM5_ADDR				6'h16   //	Address offset = h 158
`define	P_TA5_ADDR				6'h17   //	Address offset = h 15c
`define	P_ERR_CS_ADDR			6'h18   //	Address offset = h 160
`define	P_ERR_ADDR_ADDR			6'h19   //	Address offset = h 164
`define	P_ERR_DATA_ADDR			6'h1a   //	Address offset = h 168

`define	WB_CONF_SPC_BAR_ADDR	6'h20	//	Address offset = h 180
`define	W_IMG_CTRL1_ADDR		6'h21   //	Address offset = h 184
`define	W_BA1_ADDR				6'h22   //	Address offset = h 188
`define	W_AM1_ADDR				6'h23   //	Address offset = h 18c
`define	W_TA1_ADDR				6'h24   //	Address offset = h 190
`define	W_IMG_CTRL2_ADDR		6'h25   //	Address offset = h 194
`define	W_BA2_ADDR				6'h26   //	Address offset = h 198
`define	W_AM2_ADDR				6'h27   //	Address offset = h 19c
`define	W_TA2_ADDR				6'h28   //	Address offset = h 1a0
`define	W_IMG_CTRL3_ADDR		6'h29   //	Address offset = h 1a4
`define	W_BA3_ADDR				6'h2a   //	Address offset = h 1a8
`define	W_AM3_ADDR				6'h2b   //	Address offset = h 1ac
`define	W_TA3_ADDR				6'h2c   //	Address offset = h 1b0
`define	W_IMG_CTRL4_ADDR		6'h2d   //	Address offset = h 1b4
`define	W_BA4_ADDR				6'h2e   //	Address offset = h 1b8
`define	W_AM4_ADDR				6'h2f   //	Address offset = h 1bc
`define	W_TA4_ADDR				6'h30   //	Address offset = h 1c0
`define	W_IMG_CTRL5_ADDR		6'h31   //	Address offset = h 1c4
`define	W_BA5_ADDR				6'h32   //	Address offset = h 1c8
`define	W_AM5_ADDR				6'h33   //	Address offset = h 1cc
`define	W_TA5_ADDR				6'h34   //	Address offset = h 1d0
`define	W_ERR_CS_ADDR			6'h35   //	Address offset = h 1d4
`define	W_ERR_ADDR_ADDR			6'h36   //	Address offset = h 1d8
`define	W_ERR_DATA_ADDR			6'h37   //	Address offset = h 1dc
`define	CNF_ADDR_ADDR			6'h38   //	Address offset = h 1e0
// Following two registers are not implemented in a configuration space but in a WishBone unit!
`define	CNF_DATA_ADDR			6'h39	//	Address offset = h 1e4
`define	INT_ACK_ADDR			6'h3a   //	Address offset = h 1e8
// -------------------------------------
`define	ICR_ADDR				6'h3b   //	Address offset = h 1ec
`define	ISR_ADDR		        6'h3c   //	Address offset = h 1f0

`ifdef PCI33
    `define HEADER_66MHz        1'b0
`else
`ifdef PCI66
    `define HEADER_66MHz        1'b1
`endif
`endif

// all flip-flops in the design have this inter-assignment delay
`define FF_DELAY 1

