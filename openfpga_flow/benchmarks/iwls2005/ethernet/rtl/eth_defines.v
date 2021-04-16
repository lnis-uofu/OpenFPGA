//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_defines.v                                               ////
////                                                              ////
////  This file is part of the Ethernet IP core project           ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Igor Mohor (igorM@opencores.org)                      ////
////                                                              ////
////  All additional information is available in the Readme.txt   ////
////  file.                                                       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2001, 2002 Authors                             ////
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
// $Log: eth_defines.v,v $
// Revision 1.34  2005/02/21 12:48:06  igorm
// Warning fixes.
//
// Revision 1.33  2003/11/12 18:24:58  tadejm
// WISHBONE slave changed and tested from only 32-bit accesss to byte access.
//
// Revision 1.32  2003/10/17 07:46:13  markom
// mbist signals updated according to newest convention
//
// Revision 1.31  2003/08/14 16:42:58  simons
// Artisan ram instance added.
//
// Revision 1.30  2003/06/13 11:55:37  mohor
// Define file in eth_cop.v is changed to eth_defines.v. Some defines were
// moved from tb_eth_defines.v to eth_defines.v.
//
// Revision 1.29  2002/11/19 18:13:49  mohor
// r_MiiMRst is not used for resetting the MIIM module. wb_rst used instead.
//
// Revision 1.28  2002/11/15 14:27:15  mohor
// Since r_Rst bit is not used any more, default value is changed to 0xa000.
//
// Revision 1.27  2002/11/01 18:19:34  mohor
// Defines fixed to use generic RAM by default.
//
// Revision 1.26  2002/10/24 18:53:03  mohor
// fpga define added.
//
// Revision 1.3  2002/10/11 16:57:54  igorm
// eth_defines.v tagged with rel_5 used.
//
// Revision 1.25  2002/10/10 16:47:44  mohor
// Defines changed to have ETH_ prolog.
// ETH_WISHBONE_B# define added.
//
// Revision 1.24  2002/10/10 16:33:11  mohor
// Bist added.
//
// Revision 1.23  2002/09/23 18:22:48  mohor
// Virtual Silicon RAM might be used in the ASIC implementation of the ethernet
// core.
//
// Revision 1.22  2002/09/04 18:36:49  mohor
// Defines for control registers added (ETH_TXCTRL and ETH_RXCTRL).
//
// Revision 1.21  2002/08/16 22:09:47  mohor
// Defines for register width added. mii_rst signal in MIIMODER register
// changed.
//
// Revision 1.20  2002/08/14 19:31:48  mohor
// Register TX_BD_NUM is changed so it contains value of the Tx buffer descriptors. No
// need to multiply or devide any more.
//
// Revision 1.19  2002/07/23 15:28:31  mohor
// Ram , used for BDs changed from generic_spram to eth_spram_256x32.
//
// Revision 1.18  2002/05/03 10:15:50  mohor
// Outputs registered. Reset changed for eth_wishbone module.
//
// Revision 1.17  2002/04/24 08:52:19  mohor
// Compiler directives added. Tx and Rx fifo size incremented. A "late collision"
// bug fixed.
//
// Revision 1.16  2002/03/19 12:53:29  mohor
// Some defines that are used in testbench only were moved to tb_eth_defines.v
// file.
//
// Revision 1.15  2002/02/26 16:11:32  mohor
// Number of interrupts changed
//
// Revision 1.14  2002/02/16 14:03:44  mohor
// Registered trimmed. Unused registers removed.
//
// Revision 1.13  2002/02/16 13:06:33  mohor
// EXTERNAL_DMA used instead of WISHBONE_DMA.
//
// Revision 1.12  2002/02/15 10:58:31  mohor
// Changed that were lost with last update put back to the file.
//
// Revision 1.11  2002/02/14 20:19:41  billditt
// Modified for Address Checking,
// addition of eth_addrcheck.v
//
// Revision 1.10  2002/02/12 17:01:19  mohor
// HASH0 and HASH1 registers added. 

// Revision 1.9  2002/02/08 16:21:54  mohor
// Rx status is written back to the BD.
//
// Revision 1.8  2002/02/05 16:44:38  mohor
// Both rx and tx part are finished. Tested with wb_clk_i between 10 and 200
// MHz. Statuses, overrun, control frame transmission and reception still  need
// to be fixed.
//
// Revision 1.7  2002/01/23 10:28:16  mohor
// Link in the header changed.
//
// Revision 1.6  2001/12/05 15:00:16  mohor
// RX_BD_NUM changed to TX_BD_NUM (holds number of TX descriptors
// instead of the number of RX descriptors).
//
// Revision 1.5  2001/12/05 10:21:37  mohor
// ETH_RX_BD_ADR register deleted. ETH_RX_BD_NUM is used instead.
//
// Revision 1.4  2001/11/13 14:23:56  mohor
// Generic memory model is used. Defines are changed for the same reason.
//
// Revision 1.3  2001/10/18 12:07:11  mohor
// Status signals changed, Adress decoding changed, interrupt controller
// added.
//
// Revision 1.2  2001/09/24 15:02:56  mohor
// Defines changed (All precede with ETH_). Small changes because some
// tools generate warnings when two operands are together. Synchronization
// between two clocks domains in eth_wishbonedma.v is changed (due to ASIC
// demands).
//
// Revision 1.1  2001/08/06 14:44:29  mohor
// A define FPGA added to select between Artisan RAM (for ASIC) and Block Ram (For Virtex).
// Include files fixed to contain no path.
// File names and module names changed ta have a eth_ prologue in the name.
// File eth_timescale.v is used to define timescale
// All pin names on the top module are changed to contain _I, _O or _OE at the end.
// Bidirectional signal MDIO is changed to three signals (Mdc_O, Mdi_I, Mdo_O
// and Mdo_OE. The bidirectional signal must be created on the top level. This
// is done due to the ASIC tools.
//
// Revision 1.1  2001/07/30 21:23:42  mohor
// Directory structure changed. Files checked and joind together.
//
//
//
//
//



//`define ETH_BIST                    // Bist for usage with Virtual Silicon RAMS

`define ETH_MBIST_CTRL_WIDTH 3        // width of MBIST control bus

// Ethernet implemented in Xilinx Chips (uncomment following lines)
// `define ETH_FIFO_XILINX             // Use Xilinx distributed ram for tx and rx fifo
// `define ETH_XILINX_RAMB4            // Selection of the used memory for Buffer descriptors
                                      // Core is going to be implemented in Virtex FPGA and contains Virtex 
                                      // specific elements. 

// Ethernet implemented in Altera Chips (uncomment following lines)
//`define ETH_ALTERA_ALTSYNCRAM

// Ethernet implemented in ASIC with Virtual Silicon RAMs
// `define ETH_VIRTUAL_SILICON_RAM     // Virtual Silicon RAMS used storing buffer decriptors (ASIC implementation)

// Ethernet implemented in ASIC with Artisan RAMs
// `define ETH_ARTISAN_RAM             // Artisan RAMS used storing buffer decriptors (ASIC implementation)

// Uncomment when Avalon bus is used
//`define ETH_AVALON_BUS

`define ETH_MODER_ADR         8'h0    // 0x0 
`define ETH_INT_SOURCE_ADR    8'h1    // 0x4 
`define ETH_INT_MASK_ADR      8'h2    // 0x8 
`define ETH_IPGT_ADR          8'h3    // 0xC 
`define ETH_IPGR1_ADR         8'h4    // 0x10
`define ETH_IPGR2_ADR         8'h5    // 0x14
`define ETH_PACKETLEN_ADR     8'h6    // 0x18
`define ETH_COLLCONF_ADR      8'h7    // 0x1C
`define ETH_TX_BD_NUM_ADR     8'h8    // 0x20
`define ETH_CTRLMODER_ADR     8'h9    // 0x24
`define ETH_MIIMODER_ADR      8'hA    // 0x28
`define ETH_MIICOMMAND_ADR    8'hB    // 0x2C
`define ETH_MIIADDRESS_ADR    8'hC    // 0x30
`define ETH_MIITX_DATA_ADR    8'hD    // 0x34
`define ETH_MIIRX_DATA_ADR    8'hE    // 0x38
`define ETH_MIISTATUS_ADR     8'hF    // 0x3C
`define ETH_MAC_ADDR0_ADR     8'h10   // 0x40
`define ETH_MAC_ADDR1_ADR     8'h11   // 0x44
`define ETH_HASH0_ADR         8'h12   // 0x48
`define ETH_HASH1_ADR         8'h13   // 0x4C
`define ETH_TX_CTRL_ADR       8'h14   // 0x50
`define ETH_RX_CTRL_ADR       8'h15   // 0x54


`define ETH_MODER_DEF_0         8'h00
`define ETH_MODER_DEF_1         8'hA0
`define ETH_MODER_DEF_2         1'h0
`define ETH_INT_MASK_DEF_0      7'h0
`define ETH_IPGT_DEF_0          7'h12
`define ETH_IPGR1_DEF_0         7'h0C
`define ETH_IPGR2_DEF_0         7'h12
`define ETH_PACKETLEN_DEF_0     8'h00
`define ETH_PACKETLEN_DEF_1     8'h06
`define ETH_PACKETLEN_DEF_2     8'h40
`define ETH_PACKETLEN_DEF_3     8'h00
`define ETH_COLLCONF_DEF_0      6'h3f
`define ETH_COLLCONF_DEF_2      4'hF
`define ETH_TX_BD_NUM_DEF_0     8'h40
`define ETH_CTRLMODER_DEF_0     3'h0
`define ETH_MIIMODER_DEF_0      8'h64
`define ETH_MIIMODER_DEF_1      1'h0
`define ETH_MIIADDRESS_DEF_0    5'h00
`define ETH_MIIADDRESS_DEF_1    5'h00
`define ETH_MIITX_DATA_DEF_0    8'h00
`define ETH_MIITX_DATA_DEF_1    8'h00
`define ETH_MIIRX_DATA_DEF     16'h0000 // not written from WB
`define ETH_MAC_ADDR0_DEF_0     8'h00
`define ETH_MAC_ADDR0_DEF_1     8'h00
`define ETH_MAC_ADDR0_DEF_2     8'h00
`define ETH_MAC_ADDR0_DEF_3     8'h00
`define ETH_MAC_ADDR1_DEF_0     8'h00
`define ETH_MAC_ADDR1_DEF_1     8'h00
`define ETH_HASH0_DEF_0         8'h00
`define ETH_HASH0_DEF_1         8'h00
`define ETH_HASH0_DEF_2         8'h00
`define ETH_HASH0_DEF_3         8'h00
`define ETH_HASH1_DEF_0         8'h00
`define ETH_HASH1_DEF_1         8'h00
`define ETH_HASH1_DEF_2         8'h00
`define ETH_HASH1_DEF_3         8'h00
`define ETH_TX_CTRL_DEF_0       8'h00 //
`define ETH_TX_CTRL_DEF_1       8'h00 //
`define ETH_TX_CTRL_DEF_2       1'h0  //
`define ETH_RX_CTRL_DEF_0       8'h00
`define ETH_RX_CTRL_DEF_1       8'h00


`define ETH_MODER_WIDTH_0       8
`define ETH_MODER_WIDTH_1       8
`define ETH_MODER_WIDTH_2       1
`define ETH_INT_SOURCE_WIDTH_0  7
`define ETH_INT_MASK_WIDTH_0    7
`define ETH_IPGT_WIDTH_0        7
`define ETH_IPGR1_WIDTH_0       7
`define ETH_IPGR2_WIDTH_0       7
`define ETH_PACKETLEN_WIDTH_0   8
`define ETH_PACKETLEN_WIDTH_1   8
`define ETH_PACKETLEN_WIDTH_2   8
`define ETH_PACKETLEN_WIDTH_3   8
`define ETH_COLLCONF_WIDTH_0    6
`define ETH_COLLCONF_WIDTH_2    4
`define ETH_TX_BD_NUM_WIDTH_0   8
`define ETH_CTRLMODER_WIDTH_0   3
`define ETH_MIIMODER_WIDTH_0    8
`define ETH_MIIMODER_WIDTH_1    1
`define ETH_MIICOMMAND_WIDTH_0  3
`define ETH_MIIADDRESS_WIDTH_0  5
`define ETH_MIIADDRESS_WIDTH_1  5
`define ETH_MIITX_DATA_WIDTH_0  8
`define ETH_MIITX_DATA_WIDTH_1  8
`define ETH_MIIRX_DATA_WIDTH    16 // not written from WB
`define ETH_MIISTATUS_WIDTH     3 // not written from WB
`define ETH_MAC_ADDR0_WIDTH_0   8
`define ETH_MAC_ADDR0_WIDTH_1   8
`define ETH_MAC_ADDR0_WIDTH_2   8
`define ETH_MAC_ADDR0_WIDTH_3   8
`define ETH_MAC_ADDR1_WIDTH_0   8
`define ETH_MAC_ADDR1_WIDTH_1   8
`define ETH_HASH0_WIDTH_0       8
`define ETH_HASH0_WIDTH_1       8
`define ETH_HASH0_WIDTH_2       8
`define ETH_HASH0_WIDTH_3       8
`define ETH_HASH1_WIDTH_0       8
`define ETH_HASH1_WIDTH_1       8
`define ETH_HASH1_WIDTH_2       8
`define ETH_HASH1_WIDTH_3       8
`define ETH_TX_CTRL_WIDTH_0     8
`define ETH_TX_CTRL_WIDTH_1     8
`define ETH_TX_CTRL_WIDTH_2     1
`define ETH_RX_CTRL_WIDTH_0     8
`define ETH_RX_CTRL_WIDTH_1     8


// Outputs are registered (uncomment when needed)
`define ETH_REGISTERED_OUTPUTS

// Settings for TX FIFO
`define ETH_TX_FIFO_CNT_WIDTH  5
`define ETH_TX_FIFO_DEPTH      16
`define ETH_TX_FIFO_DATA_WIDTH 32

// Settings for RX FIFO
`define ETH_RX_FIFO_CNT_WIDTH  5
`define ETH_RX_FIFO_DEPTH      16
`define ETH_RX_FIFO_DATA_WIDTH 32

// Burst length
`define ETH_BURST_LENGTH       4    // Change also ETH_BURST_CNT_WIDTH
`define ETH_BURST_CNT_WIDTH    3    // The counter must be width enough to count to ETH_BURST_LENGTH

// WISHBONE interface is Revision B3 compliant (uncomment when needed)
//`define ETH_WISHBONE_B3


// Following defines are needed when eth_cop.v is used. Otherwise they may be deleted.
`define ETH_BASE              32'hd0000000
`define ETH_WIDTH             32'h800
`define MEMORY_BASE           32'h2000
`define MEMORY_WIDTH          32'h10000

`define M1_ADDRESSED_S1 ( (m1_wb_adr_i >= `ETH_BASE)    & (m1_wb_adr_i < (`ETH_BASE    + `ETH_WIDTH   )) )
`define M1_ADDRESSED_S2 ( (m1_wb_adr_i >= `MEMORY_BASE) & (m1_wb_adr_i < (`MEMORY_BASE + `MEMORY_WIDTH)) )
`define M2_ADDRESSED_S1 ( (m2_wb_adr_i >= `ETH_BASE)    & (m2_wb_adr_i < (`ETH_BASE    + `ETH_WIDTH   )) )
`define M2_ADDRESSED_S2 ( (m2_wb_adr_i >= `MEMORY_BASE) & (m2_wb_adr_i < (`MEMORY_BASE + `MEMORY_WIDTH)) )
// Previous defines are only needed for eth_cop.v

