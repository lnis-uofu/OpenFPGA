//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_registers.v                                             ////
////                                                              ////
////  This file is part of the Ethernet IP core project           ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Igor Mohor (igorM@opencores.org)                      ////
////                                                              ////
////  All additional information is avaliable in the Readme.txt   ////
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
// $Log: eth_registers.v,v $
// Revision 1.29  2005/03/21 20:07:18  igorm
// Some small fixes + some troubles fixed.
//
// Revision 1.28  2004/04/26 15:26:23  igorm
// - Bug connected to the TX_BD_NUM_Wr signal fixed (bug came in with the
//   previous update of the core.
// - TxBDAddress is set to 0 after the TX is enabled in the MODER register.
// - RxBDAddress is set to r_TxBDNum<<1 after the RX is enabled in the MODER
//   register. (thanks to Mathias and Torbjorn)
// - Multicast reception was fixed. Thanks to Ulrich Gries
//
// Revision 1.27  2004/04/26 11:42:17  igorm
// TX_BD_NUM_Wr error fixed. Error was entered with the last check-in.
//
// Revision 1.26  2003/11/12 18:24:59  tadejm
// WISHBONE slave changed and tested from only 32-bit accesss to byte access.
//
// Revision 1.25  2003/04/18 16:26:25  mohor
// RxBDAddress was updated also when value to r_TxBDNum was written with
// greater value than allowed.
//
// Revision 1.24  2002/11/22 01:57:06  mohor
// Rx Flow control fixed. CF flag added to the RX buffer descriptor. RxAbort
// synchronized.
//
// Revision 1.23  2002/11/19 18:13:49  mohor
// r_MiiMRst is not used for resetting the MIIM module. wb_rst used instead.
//
// Revision 1.22  2002/11/14 18:37:20  mohor
// r_Rst signal does not reset any module any more and is removed from the design.
//
// Revision 1.21  2002/09/10 10:35:23  mohor
// Ethernet debug registers removed.
//
// Revision 1.20  2002/09/04 18:40:25  mohor
// ETH_TXCTRL and ETH_RXCTRL registers added. Interrupts related to
// the control frames connected.
//
// Revision 1.19  2002/08/19 16:01:40  mohor
// Only values smaller or equal to 0x80 can be written to TX_BD_NUM register.
// r_TxEn and r_RxEn depend on the limit values of the TX_BD_NUMOut.
//
// Revision 1.18  2002/08/16 22:28:23  mohor
// Syntax error fixed.
//
// Revision 1.17  2002/08/16 22:23:03  mohor
// Syntax error fixed.
//
// Revision 1.16  2002/08/16 22:14:22  mohor
// Synchronous reset added to all registers. Defines used for width. r_MiiMRst
// changed from bit position 10 to 9.
//
// Revision 1.15  2002/08/14 18:26:37  mohor
// LinkFailRegister is reflecting the status of the PHY's link fail status bit.
//
// Revision 1.14  2002/04/22 14:03:44  mohor
// Interrupts are visible in the ETH_INT_SOURCE regardless if they are enabled
// or not.
//
// Revision 1.13  2002/02/26 16:18:09  mohor
// Reset values are passed to registers through parameters
//
// Revision 1.12  2002/02/17 13:23:42  mohor
// Define missmatch fixed.
//
// Revision 1.11  2002/02/16 14:03:44  mohor
// Registered trimmed. Unused registers removed.
//
// Revision 1.10  2002/02/15 11:08:25  mohor
// File format fixed a bit.
//
// Revision 1.9  2002/02/14 20:19:41  billditt
// Modified for Address Checking,
// addition of eth_addrcheck.v
//
// Revision 1.8  2002/02/12 17:01:19  mohor
// HASH0 and HASH1 registers added. 

// Revision 1.7  2002/01/23 10:28:16  mohor
// Link in the header changed.
//
// Revision 1.6  2001/12/05 15:00:16  mohor
// RX_BD_NUM changed to TX_BD_NUM (holds number of TX descriptors
// instead of the number of RX descriptors).
//
// Revision 1.5  2001/12/05 10:22:19  mohor
// ETH_RX_BD_ADR register deleted. ETH_RX_BD_NUM is used instead.
//
// Revision 1.4  2001/10/19 08:43:51  mohor
// eth_timescale.v changed to timescale.v This is done because of the
// simulation of the few cores in a one joined project.
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
// Revision 1.2  2001/08/02 09:25:31  mohor
// Unconnected signals are now connected.
//
// Revision 1.1  2001/07/30 21:23:42  mohor
// Directory structure changed. Files checked and joind together.
//
//
//
//
//
//

`include "eth_defines.v"
`include "timescale.v"


module eth_registers( DataIn, Address, Rw, Cs, Clk, Reset, DataOut, 
                      r_RecSmall, r_Pad, r_HugEn, r_CrcEn, r_DlyCrcEn, 
                      r_FullD, r_ExDfrEn, r_NoBckof, r_LoopBck, r_IFG, 
                      r_Pro, r_Iam, r_Bro, r_NoPre, r_TxEn, r_RxEn, 
                      TxB_IRQ, TxE_IRQ, RxB_IRQ, RxE_IRQ, Busy_IRQ, 
                      r_IPGT, r_IPGR1, r_IPGR2, r_MinFL, r_MaxFL, r_MaxRet, 
                      r_CollValid, r_TxFlow, r_RxFlow, r_PassAll, 
                      r_MiiNoPre, r_ClkDiv, r_WCtrlData, r_RStat, r_ScanStat, 
                      r_RGAD, r_FIAD, r_CtrlData, NValid_stat, Busy_stat, 
                      LinkFail, r_MAC, WCtrlDataStart, RStatStart,
                      UpdateMIIRX_DATAReg, Prsd, r_TxBDNum, int_o,
                      r_HASH0, r_HASH1, r_TxPauseTV, r_TxPauseRq, RstTxPauseRq, TxCtrlEndFrm, 
                      StartTxDone, TxClk, RxClk, SetPauseTimer
                    );

parameter Tp = 1;

input [31:0] DataIn;
input [7:0] Address;

input Rw;
input [3:0] Cs;
input Clk;
input Reset;

input WCtrlDataStart;
input RStatStart;

input UpdateMIIRX_DATAReg;
input [15:0] Prsd;

output [31:0] DataOut;
reg    [31:0] DataOut;

output r_RecSmall;
output r_Pad;
output r_HugEn;
output r_CrcEn;
output r_DlyCrcEn;
output r_FullD;
output r_ExDfrEn;
output r_NoBckof;
output r_LoopBck;
output r_IFG;
output r_Pro;
output r_Iam;
output r_Bro;
output r_NoPre;
output r_TxEn;
output r_RxEn;
output [31:0] r_HASH0;
output [31:0] r_HASH1;

input TxB_IRQ;
input TxE_IRQ;
input RxB_IRQ;
input RxE_IRQ;
input Busy_IRQ;

output [6:0] r_IPGT;

output [6:0] r_IPGR1;

output [6:0] r_IPGR2;

output [15:0] r_MinFL;
output [15:0] r_MaxFL;

output [3:0] r_MaxRet;
output [5:0] r_CollValid;

output r_TxFlow;
output r_RxFlow;
output r_PassAll;

output r_MiiNoPre;
output [7:0] r_ClkDiv;

output r_WCtrlData;
output r_RStat;
output r_ScanStat;

output [4:0] r_RGAD;
output [4:0] r_FIAD;

output [15:0]r_CtrlData;


input NValid_stat;
input Busy_stat;
input LinkFail;

output [47:0]r_MAC;
output [7:0] r_TxBDNum;
output       int_o;
output [15:0]r_TxPauseTV;
output       r_TxPauseRq;
input        RstTxPauseRq;
input        TxCtrlEndFrm;
input        StartTxDone;
input        TxClk;
input        RxClk;
input        SetPauseTimer;

reg          irq_txb;
reg          irq_txe;
reg          irq_rxb;
reg          irq_rxe;
reg          irq_busy;
reg          irq_txc;
reg          irq_rxc;

reg SetTxCIrq_txclk;
reg SetTxCIrq_sync1, SetTxCIrq_sync2, SetTxCIrq_sync3;
reg SetTxCIrq;
reg ResetTxCIrq_sync1, ResetTxCIrq_sync2;

reg SetRxCIrq_rxclk;
reg SetRxCIrq_sync1, SetRxCIrq_sync2, SetRxCIrq_sync3;
reg SetRxCIrq;
reg ResetRxCIrq_sync1;
reg ResetRxCIrq_sync2;
reg ResetRxCIrq_sync3;

wire [3:0] Write =   Cs  & {4{Rw}};
wire       Read  = (|Cs) &   ~Rw;

wire MODER_Sel      = (Address == `ETH_MODER_ADR       );
wire INT_SOURCE_Sel = (Address == `ETH_INT_SOURCE_ADR  );
wire INT_MASK_Sel   = (Address == `ETH_INT_MASK_ADR    );
wire IPGT_Sel       = (Address == `ETH_IPGT_ADR        );
wire IPGR1_Sel      = (Address == `ETH_IPGR1_ADR       );
wire IPGR2_Sel      = (Address == `ETH_IPGR2_ADR       );
wire PACKETLEN_Sel  = (Address == `ETH_PACKETLEN_ADR   );
wire COLLCONF_Sel   = (Address == `ETH_COLLCONF_ADR    );
     
wire CTRLMODER_Sel  = (Address == `ETH_CTRLMODER_ADR   );
wire MIIMODER_Sel   = (Address == `ETH_MIIMODER_ADR    );
wire MIICOMMAND_Sel = (Address == `ETH_MIICOMMAND_ADR  );
wire MIIADDRESS_Sel = (Address == `ETH_MIIADDRESS_ADR  );
wire MIITX_DATA_Sel = (Address == `ETH_MIITX_DATA_ADR  );
wire MAC_ADDR0_Sel  = (Address == `ETH_MAC_ADDR0_ADR   );
wire MAC_ADDR1_Sel  = (Address == `ETH_MAC_ADDR1_ADR   );
wire HASH0_Sel      = (Address == `ETH_HASH0_ADR       );
wire HASH1_Sel      = (Address == `ETH_HASH1_ADR       );
wire TXCTRL_Sel     = (Address == `ETH_TX_CTRL_ADR     );
wire RXCTRL_Sel     = (Address == `ETH_RX_CTRL_ADR     );
wire TX_BD_NUM_Sel  = (Address == `ETH_TX_BD_NUM_ADR   );


wire [2:0] MODER_Wr;
wire [0:0] INT_SOURCE_Wr;
wire [0:0] INT_MASK_Wr;
wire [0:0] IPGT_Wr;
wire [0:0] IPGR1_Wr;
wire [0:0] IPGR2_Wr;
wire [3:0] PACKETLEN_Wr;
wire [2:0] COLLCONF_Wr;
wire [0:0] CTRLMODER_Wr;
wire [1:0] MIIMODER_Wr;
wire [0:0] MIICOMMAND_Wr;
wire [1:0] MIIADDRESS_Wr;
wire [1:0] MIITX_DATA_Wr;
wire       MIIRX_DATA_Wr;
wire [3:0] MAC_ADDR0_Wr;
wire [1:0] MAC_ADDR1_Wr;
wire [3:0] HASH0_Wr;
wire [3:0] HASH1_Wr;
wire [2:0] TXCTRL_Wr;
wire [0:0] TX_BD_NUM_Wr;

assign MODER_Wr[0]       = Write[0]  & MODER_Sel; 
assign MODER_Wr[1]       = Write[1]  & MODER_Sel; 
assign MODER_Wr[2]       = Write[2]  & MODER_Sel; 
assign INT_SOURCE_Wr[0]  = Write[0]  & INT_SOURCE_Sel; 
assign INT_MASK_Wr[0]    = Write[0]  & INT_MASK_Sel; 
assign IPGT_Wr[0]        = Write[0]  & IPGT_Sel; 
assign IPGR1_Wr[0]       = Write[0]  & IPGR1_Sel; 
assign IPGR2_Wr[0]       = Write[0]  & IPGR2_Sel; 
assign PACKETLEN_Wr[0]   = Write[0]  & PACKETLEN_Sel; 
assign PACKETLEN_Wr[1]   = Write[1]  & PACKETLEN_Sel; 
assign PACKETLEN_Wr[2]   = Write[2]  & PACKETLEN_Sel; 
assign PACKETLEN_Wr[3]   = Write[3]  & PACKETLEN_Sel; 
assign COLLCONF_Wr[0]    = Write[0]  & COLLCONF_Sel; 
assign COLLCONF_Wr[1]    = 1'b0;  // Not used
assign COLLCONF_Wr[2]    = Write[2]  & COLLCONF_Sel; 
     
assign CTRLMODER_Wr[0]   = Write[0]  & CTRLMODER_Sel; 
assign MIIMODER_Wr[0]    = Write[0]  & MIIMODER_Sel; 
assign MIIMODER_Wr[1]    = Write[1]  & MIIMODER_Sel; 
assign MIICOMMAND_Wr[0]  = Write[0]  & MIICOMMAND_Sel; 
assign MIIADDRESS_Wr[0]  = Write[0]  & MIIADDRESS_Sel; 
assign MIIADDRESS_Wr[1]  = Write[1]  & MIIADDRESS_Sel; 
assign MIITX_DATA_Wr[0]  = Write[0]  & MIITX_DATA_Sel; 
assign MIITX_DATA_Wr[1]  = Write[1]  & MIITX_DATA_Sel; 
assign MIIRX_DATA_Wr     = UpdateMIIRX_DATAReg;     
assign MAC_ADDR0_Wr[0]   = Write[0]  & MAC_ADDR0_Sel; 
assign MAC_ADDR0_Wr[1]   = Write[1]  & MAC_ADDR0_Sel; 
assign MAC_ADDR0_Wr[2]   = Write[2]  & MAC_ADDR0_Sel; 
assign MAC_ADDR0_Wr[3]   = Write[3]  & MAC_ADDR0_Sel; 
assign MAC_ADDR1_Wr[0]   = Write[0]  & MAC_ADDR1_Sel; 
assign MAC_ADDR1_Wr[1]   = Write[1]  & MAC_ADDR1_Sel; 
assign HASH0_Wr[0]       = Write[0]  & HASH0_Sel; 
assign HASH0_Wr[1]       = Write[1]  & HASH0_Sel; 
assign HASH0_Wr[2]       = Write[2]  & HASH0_Sel; 
assign HASH0_Wr[3]       = Write[3]  & HASH0_Sel; 
assign HASH1_Wr[0]       = Write[0]  & HASH1_Sel; 
assign HASH1_Wr[1]       = Write[1]  & HASH1_Sel; 
assign HASH1_Wr[2]       = Write[2]  & HASH1_Sel; 
assign HASH1_Wr[3]       = Write[3]  & HASH1_Sel; 
assign TXCTRL_Wr[0]      = Write[0]  & TXCTRL_Sel; 
assign TXCTRL_Wr[1]      = Write[1]  & TXCTRL_Sel; 
assign TXCTRL_Wr[2]      = Write[2]  & TXCTRL_Sel; 
assign TX_BD_NUM_Wr[0]   = Write[0]  & TX_BD_NUM_Sel & (DataIn<='h80); 



wire [31:0] MODEROut;
wire [31:0] INT_SOURCEOut;
wire [31:0] INT_MASKOut;
wire [31:0] IPGTOut;
wire [31:0] IPGR1Out;
wire [31:0] IPGR2Out;
wire [31:0] PACKETLENOut;
wire [31:0] COLLCONFOut;
wire [31:0] CTRLMODEROut;
wire [31:0] MIIMODEROut;
wire [31:0] MIICOMMANDOut;
wire [31:0] MIIADDRESSOut;
wire [31:0] MIITX_DATAOut;
wire [31:0] MIIRX_DATAOut;
wire [31:0] MIISTATUSOut;
wire [31:0] MAC_ADDR0Out;
wire [31:0] MAC_ADDR1Out;
wire [31:0] TX_BD_NUMOut;
wire [31:0] HASH0Out;
wire [31:0] HASH1Out;
wire [31:0] TXCTRLOut;

// MODER Register
eth_register #(`ETH_MODER_WIDTH_0, `ETH_MODER_DEF_0)        MODER_0
  (
   .DataIn    (DataIn[`ETH_MODER_WIDTH_0 - 1:0]),
   .DataOut   (MODEROut[`ETH_MODER_WIDTH_0 - 1:0]),
   .Write     (MODER_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MODER_WIDTH_1, `ETH_MODER_DEF_1)        MODER_1
  (
   .DataIn    (DataIn[`ETH_MODER_WIDTH_1 + 7:8]),
   .DataOut   (MODEROut[`ETH_MODER_WIDTH_1 + 7:8]),
   .Write     (MODER_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MODER_WIDTH_2, `ETH_MODER_DEF_2)        MODER_2
  (
   .DataIn    (DataIn[`ETH_MODER_WIDTH_2 + 15:16]),
   .DataOut   (MODEROut[`ETH_MODER_WIDTH_2 + 15:16]),
   .Write     (MODER_Wr[2]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign MODEROut[31:`ETH_MODER_WIDTH_2 + 16] = 0;

// INT_MASK Register
eth_register #(`ETH_INT_MASK_WIDTH_0, `ETH_INT_MASK_DEF_0)  INT_MASK_0
  (
   .DataIn    (DataIn[`ETH_INT_MASK_WIDTH_0 - 1:0]),  
   .DataOut   (INT_MASKOut[`ETH_INT_MASK_WIDTH_0 - 1:0]),
   .Write     (INT_MASK_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign INT_MASKOut[31:`ETH_INT_MASK_WIDTH_0] = 0;

// IPGT Register
eth_register #(`ETH_IPGT_WIDTH_0, `ETH_IPGT_DEF_0)          IPGT_0
  (
   .DataIn    (DataIn[`ETH_IPGT_WIDTH_0 - 1:0]),
   .DataOut   (IPGTOut[`ETH_IPGT_WIDTH_0 - 1:0]),
   .Write     (IPGT_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign IPGTOut[31:`ETH_IPGT_WIDTH_0] = 0;

// IPGR1 Register
eth_register #(`ETH_IPGR1_WIDTH_0, `ETH_IPGR1_DEF_0)        IPGR1_0
  (
   .DataIn    (DataIn[`ETH_IPGR1_WIDTH_0 - 1:0]),
   .DataOut   (IPGR1Out[`ETH_IPGR1_WIDTH_0 - 1:0]),
   .Write     (IPGR1_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign IPGR1Out[31:`ETH_IPGR1_WIDTH_0] = 0;

// IPGR2 Register
eth_register #(`ETH_IPGR2_WIDTH_0, `ETH_IPGR2_DEF_0)        IPGR2_0
  (
   .DataIn    (DataIn[`ETH_IPGR2_WIDTH_0 - 1:0]),
   .DataOut   (IPGR2Out[`ETH_IPGR2_WIDTH_0 - 1:0]),
   .Write     (IPGR2_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign IPGR2Out[31:`ETH_IPGR2_WIDTH_0] = 0;

// PACKETLEN Register
eth_register #(`ETH_PACKETLEN_WIDTH_0, `ETH_PACKETLEN_DEF_0) PACKETLEN_0
  (
   .DataIn    (DataIn[`ETH_PACKETLEN_WIDTH_0 - 1:0]),
   .DataOut   (PACKETLENOut[`ETH_PACKETLEN_WIDTH_0 - 1:0]),
   .Write     (PACKETLEN_Wr[0]),
   .Clk       (Clk), 
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_PACKETLEN_WIDTH_1, `ETH_PACKETLEN_DEF_1) PACKETLEN_1
  (
   .DataIn    (DataIn[`ETH_PACKETLEN_WIDTH_1 + 7:8]),
   .DataOut   (PACKETLENOut[`ETH_PACKETLEN_WIDTH_1 + 7:8]),
   .Write     (PACKETLEN_Wr[1]),
   .Clk       (Clk), 
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_PACKETLEN_WIDTH_2, `ETH_PACKETLEN_DEF_2) PACKETLEN_2
  (
   .DataIn    (DataIn[`ETH_PACKETLEN_WIDTH_2 + 15:16]),
   .DataOut   (PACKETLENOut[`ETH_PACKETLEN_WIDTH_2 + 15:16]),
   .Write     (PACKETLEN_Wr[2]),
   .Clk       (Clk), 
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_PACKETLEN_WIDTH_3, `ETH_PACKETLEN_DEF_3) PACKETLEN_3
  (
   .DataIn    (DataIn[`ETH_PACKETLEN_WIDTH_3 + 23:24]),
   .DataOut   (PACKETLENOut[`ETH_PACKETLEN_WIDTH_3 + 23:24]),
   .Write     (PACKETLEN_Wr[3]),
   .Clk       (Clk), 
   .Reset     (Reset),
   .SyncReset (1'b0)
  );

// COLLCONF Register
eth_register #(`ETH_COLLCONF_WIDTH_0, `ETH_COLLCONF_DEF_0)   COLLCONF_0
  (
   .DataIn    (DataIn[`ETH_COLLCONF_WIDTH_0 - 1:0]),
   .DataOut   (COLLCONFOut[`ETH_COLLCONF_WIDTH_0 - 1:0]),
   .Write     (COLLCONF_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_COLLCONF_WIDTH_2, `ETH_COLLCONF_DEF_2)   COLLCONF_2
  (
   .DataIn    (DataIn[`ETH_COLLCONF_WIDTH_2 + 15:16]),
   .DataOut   (COLLCONFOut[`ETH_COLLCONF_WIDTH_2 + 15:16]),
   .Write     (COLLCONF_Wr[2]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign COLLCONFOut[15:`ETH_COLLCONF_WIDTH_0] = 0;
assign COLLCONFOut[31:`ETH_COLLCONF_WIDTH_2 + 16] = 0;

// TX_BD_NUM Register
eth_register #(`ETH_TX_BD_NUM_WIDTH_0, `ETH_TX_BD_NUM_DEF_0) TX_BD_NUM_0
  (
   .DataIn    (DataIn[`ETH_TX_BD_NUM_WIDTH_0 - 1:0]),
   .DataOut   (TX_BD_NUMOut[`ETH_TX_BD_NUM_WIDTH_0 - 1:0]),
   .Write     (TX_BD_NUM_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign TX_BD_NUMOut[31:`ETH_TX_BD_NUM_WIDTH_0] = 0;

// CTRLMODER Register
eth_register #(`ETH_CTRLMODER_WIDTH_0, `ETH_CTRLMODER_DEF_0)  CTRLMODER_0
  (
   .DataIn    (DataIn[`ETH_CTRLMODER_WIDTH_0 - 1:0]),
   .DataOut   (CTRLMODEROut[`ETH_CTRLMODER_WIDTH_0 - 1:0]),
   .Write     (CTRLMODER_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign CTRLMODEROut[31:`ETH_CTRLMODER_WIDTH_0] = 0;

// MIIMODER Register
eth_register #(`ETH_MIIMODER_WIDTH_0, `ETH_MIIMODER_DEF_0)    MIIMODER_0
  (
   .DataIn    (DataIn[`ETH_MIIMODER_WIDTH_0 - 1:0]),
   .DataOut   (MIIMODEROut[`ETH_MIIMODER_WIDTH_0 - 1:0]),
   .Write     (MIIMODER_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MIIMODER_WIDTH_1, `ETH_MIIMODER_DEF_1)    MIIMODER_1
  (
   .DataIn    (DataIn[`ETH_MIIMODER_WIDTH_1 + 7:8]),
   .DataOut   (MIIMODEROut[`ETH_MIIMODER_WIDTH_1 + 7:8]),
   .Write     (MIIMODER_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign MIIMODEROut[31:`ETH_MIIMODER_WIDTH_1 + 8] = 0;

// MIICOMMAND Register
eth_register #(1, 0)                                      MIICOMMAND0
  (
   .DataIn    (DataIn[0]),
   .DataOut   (MIICOMMANDOut[0]),
   .Write     (MIICOMMAND_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(1, 0)                                      MIICOMMAND1
  (
   .DataIn    (DataIn[1]),
   .DataOut   (MIICOMMANDOut[1]),
   .Write     (MIICOMMAND_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (RStatStart)
  );
eth_register #(1, 0)                                      MIICOMMAND2
  (
   .DataIn    (DataIn[2]),
   .DataOut   (MIICOMMANDOut[2]),
   .Write     (MIICOMMAND_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (WCtrlDataStart)
  );
assign MIICOMMANDOut[31:`ETH_MIICOMMAND_WIDTH_0] = 29'h0;

// MIIADDRESSRegister
eth_register #(`ETH_MIIADDRESS_WIDTH_0, `ETH_MIIADDRESS_DEF_0) MIIADDRESS_0
  (
   .DataIn    (DataIn[`ETH_MIIADDRESS_WIDTH_0 - 1:0]),
   .DataOut   (MIIADDRESSOut[`ETH_MIIADDRESS_WIDTH_0 - 1:0]),
   .Write     (MIIADDRESS_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MIIADDRESS_WIDTH_1, `ETH_MIIADDRESS_DEF_1) MIIADDRESS_1
  (
   .DataIn    (DataIn[`ETH_MIIADDRESS_WIDTH_1 + 7:8]),
   .DataOut   (MIIADDRESSOut[`ETH_MIIADDRESS_WIDTH_1 + 7:8]),
   .Write     (MIIADDRESS_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign MIIADDRESSOut[7:`ETH_MIIADDRESS_WIDTH_0] = 0;
assign MIIADDRESSOut[31:`ETH_MIIADDRESS_WIDTH_1 + 8] = 0;

// MIITX_DATA Register
eth_register #(`ETH_MIITX_DATA_WIDTH_0, `ETH_MIITX_DATA_DEF_0) MIITX_DATA_0
  (
   .DataIn    (DataIn[`ETH_MIITX_DATA_WIDTH_0 - 1:0]),
   .DataOut   (MIITX_DATAOut[`ETH_MIITX_DATA_WIDTH_0 - 1:0]), 
   .Write     (MIITX_DATA_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MIITX_DATA_WIDTH_1, `ETH_MIITX_DATA_DEF_1) MIITX_DATA_1
  (
   .DataIn    (DataIn[`ETH_MIITX_DATA_WIDTH_1 + 7:8]),
   .DataOut   (MIITX_DATAOut[`ETH_MIITX_DATA_WIDTH_1 + 7:8]), 
   .Write     (MIITX_DATA_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign MIITX_DATAOut[31:`ETH_MIITX_DATA_WIDTH_1 + 8] = 0;

// MIIRX_DATA Register
eth_register #(`ETH_MIIRX_DATA_WIDTH, `ETH_MIIRX_DATA_DEF) MIIRX_DATA
  (
   .DataIn    (Prsd[`ETH_MIIRX_DATA_WIDTH-1:0]),
   .DataOut   (MIIRX_DATAOut[`ETH_MIIRX_DATA_WIDTH-1:0]),
   .Write     (MIIRX_DATA_Wr), // not written from WB
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign MIIRX_DATAOut[31:`ETH_MIIRX_DATA_WIDTH] = 0;

// MAC_ADDR0 Register
eth_register #(`ETH_MAC_ADDR0_WIDTH_0, `ETH_MAC_ADDR0_DEF_0)  MAC_ADDR0_0
  (
   .DataIn    (DataIn[`ETH_MAC_ADDR0_WIDTH_0 - 1:0]),
   .DataOut   (MAC_ADDR0Out[`ETH_MAC_ADDR0_WIDTH_0 - 1:0]),
   .Write     (MAC_ADDR0_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MAC_ADDR0_WIDTH_1, `ETH_MAC_ADDR0_DEF_1)  MAC_ADDR0_1
  (
   .DataIn    (DataIn[`ETH_MAC_ADDR0_WIDTH_1 + 7:8]),
   .DataOut   (MAC_ADDR0Out[`ETH_MAC_ADDR0_WIDTH_1 + 7:8]),
   .Write     (MAC_ADDR0_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MAC_ADDR0_WIDTH_2, `ETH_MAC_ADDR0_DEF_2)  MAC_ADDR0_2
  (
   .DataIn    (DataIn[`ETH_MAC_ADDR0_WIDTH_2 + 15:16]),
   .DataOut   (MAC_ADDR0Out[`ETH_MAC_ADDR0_WIDTH_2 + 15:16]),
   .Write     (MAC_ADDR0_Wr[2]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MAC_ADDR0_WIDTH_3, `ETH_MAC_ADDR0_DEF_3)  MAC_ADDR0_3
  (
   .DataIn    (DataIn[`ETH_MAC_ADDR0_WIDTH_3 + 23:24]),
   .DataOut   (MAC_ADDR0Out[`ETH_MAC_ADDR0_WIDTH_3 + 23:24]),
   .Write     (MAC_ADDR0_Wr[3]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );

// MAC_ADDR1 Register
eth_register #(`ETH_MAC_ADDR1_WIDTH_0, `ETH_MAC_ADDR1_DEF_0)  MAC_ADDR1_0
  (
   .DataIn    (DataIn[`ETH_MAC_ADDR1_WIDTH_0 - 1:0]),
   .DataOut   (MAC_ADDR1Out[`ETH_MAC_ADDR1_WIDTH_0 - 1:0]),
   .Write     (MAC_ADDR1_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_MAC_ADDR1_WIDTH_1, `ETH_MAC_ADDR1_DEF_1)  MAC_ADDR1_1
  (
   .DataIn    (DataIn[`ETH_MAC_ADDR1_WIDTH_1 + 7:8]),
   .DataOut   (MAC_ADDR1Out[`ETH_MAC_ADDR1_WIDTH_1 + 7:8]),
   .Write     (MAC_ADDR1_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
assign MAC_ADDR1Out[31:`ETH_MAC_ADDR1_WIDTH_1 + 8] = 0;

// RXHASH0 Register
eth_register #(`ETH_HASH0_WIDTH_0, `ETH_HASH0_DEF_0)          RXHASH0_0
  (
   .DataIn    (DataIn[`ETH_HASH0_WIDTH_0 - 1:0]),
   .DataOut   (HASH0Out[`ETH_HASH0_WIDTH_0 - 1:0]),
   .Write     (HASH0_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_HASH0_WIDTH_1, `ETH_HASH0_DEF_1)          RXHASH0_1
  (
   .DataIn    (DataIn[`ETH_HASH0_WIDTH_1 + 7:8]),
   .DataOut   (HASH0Out[`ETH_HASH0_WIDTH_1 + 7:8]),
   .Write     (HASH0_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_HASH0_WIDTH_2, `ETH_HASH0_DEF_2)          RXHASH0_2
  (
   .DataIn    (DataIn[`ETH_HASH0_WIDTH_2 + 15:16]),
   .DataOut   (HASH0Out[`ETH_HASH0_WIDTH_2 + 15:16]),
   .Write     (HASH0_Wr[2]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_HASH0_WIDTH_3, `ETH_HASH0_DEF_3)          RXHASH0_3
  (
   .DataIn    (DataIn[`ETH_HASH0_WIDTH_3 + 23:24]),
   .DataOut   (HASH0Out[`ETH_HASH0_WIDTH_3 + 23:24]),
   .Write     (HASH0_Wr[3]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );

// RXHASH1 Register
eth_register #(`ETH_HASH1_WIDTH_0, `ETH_HASH1_DEF_0)          RXHASH1_0
  (
   .DataIn    (DataIn[`ETH_HASH1_WIDTH_0 - 1:0]),
   .DataOut   (HASH1Out[`ETH_HASH1_WIDTH_0 - 1:0]),
   .Write     (HASH1_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_HASH1_WIDTH_1, `ETH_HASH1_DEF_1)          RXHASH1_1
  (
   .DataIn    (DataIn[`ETH_HASH1_WIDTH_1 + 7:8]),
   .DataOut   (HASH1Out[`ETH_HASH1_WIDTH_1 + 7:8]),
   .Write     (HASH1_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_HASH1_WIDTH_2, `ETH_HASH1_DEF_2)          RXHASH1_2
  (
   .DataIn    (DataIn[`ETH_HASH1_WIDTH_2 + 15:16]),
   .DataOut   (HASH1Out[`ETH_HASH1_WIDTH_2 + 15:16]),
   .Write     (HASH1_Wr[2]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_HASH1_WIDTH_3, `ETH_HASH1_DEF_3)          RXHASH1_3
  (
   .DataIn    (DataIn[`ETH_HASH1_WIDTH_3 + 23:24]),
   .DataOut   (HASH1Out[`ETH_HASH1_WIDTH_3 + 23:24]),
   .Write     (HASH1_Wr[3]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );

// TXCTRL Register
eth_register #(`ETH_TX_CTRL_WIDTH_0, `ETH_TX_CTRL_DEF_0)  TXCTRL_0
  (
   .DataIn    (DataIn[`ETH_TX_CTRL_WIDTH_0 - 1:0]),
   .DataOut   (TXCTRLOut[`ETH_TX_CTRL_WIDTH_0 - 1:0]),
   .Write     (TXCTRL_Wr[0]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_TX_CTRL_WIDTH_1, `ETH_TX_CTRL_DEF_1)  TXCTRL_1
  (
   .DataIn    (DataIn[`ETH_TX_CTRL_WIDTH_1 + 7:8]),
   .DataOut   (TXCTRLOut[`ETH_TX_CTRL_WIDTH_1 + 7:8]),
   .Write     (TXCTRL_Wr[1]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (1'b0)
  );
eth_register #(`ETH_TX_CTRL_WIDTH_2, `ETH_TX_CTRL_DEF_2)  TXCTRL_2 // Request bit is synchronously reset
  (
   .DataIn    (DataIn[`ETH_TX_CTRL_WIDTH_2 + 15:16]),
   .DataOut   (TXCTRLOut[`ETH_TX_CTRL_WIDTH_2 + 15:16]),
   .Write     (TXCTRL_Wr[2]),
   .Clk       (Clk),
   .Reset     (Reset),
   .SyncReset (RstTxPauseRq)
  );
assign TXCTRLOut[31:`ETH_TX_CTRL_WIDTH_2 + 16] = 0;



// Reading data from registers
always @ (Address       or Read           or MODEROut       or INT_SOURCEOut  or
          INT_MASKOut   or IPGTOut        or IPGR1Out       or IPGR2Out       or
          PACKETLENOut  or COLLCONFOut    or CTRLMODEROut   or MIIMODEROut    or
          MIICOMMANDOut or MIIADDRESSOut  or MIITX_DATAOut  or MIIRX_DATAOut  or 
          MIISTATUSOut  or MAC_ADDR0Out   or MAC_ADDR1Out   or TX_BD_NUMOut   or
          HASH0Out      or HASH1Out       or TXCTRLOut       
         )
begin
  if(Read)  // read
    begin
      case(Address)
        `ETH_MODER_ADR        :  DataOut<=MODEROut;
        `ETH_INT_SOURCE_ADR   :  DataOut<=INT_SOURCEOut;
        `ETH_INT_MASK_ADR     :  DataOut<=INT_MASKOut;
        `ETH_IPGT_ADR         :  DataOut<=IPGTOut;
        `ETH_IPGR1_ADR        :  DataOut<=IPGR1Out;
        `ETH_IPGR2_ADR        :  DataOut<=IPGR2Out;
        `ETH_PACKETLEN_ADR    :  DataOut<=PACKETLENOut;
        `ETH_COLLCONF_ADR     :  DataOut<=COLLCONFOut;
        `ETH_CTRLMODER_ADR    :  DataOut<=CTRLMODEROut;
        `ETH_MIIMODER_ADR     :  DataOut<=MIIMODEROut;
        `ETH_MIICOMMAND_ADR   :  DataOut<=MIICOMMANDOut;
        `ETH_MIIADDRESS_ADR   :  DataOut<=MIIADDRESSOut;
        `ETH_MIITX_DATA_ADR   :  DataOut<=MIITX_DATAOut;
        `ETH_MIIRX_DATA_ADR   :  DataOut<=MIIRX_DATAOut;
        `ETH_MIISTATUS_ADR    :  DataOut<=MIISTATUSOut;
        `ETH_MAC_ADDR0_ADR    :  DataOut<=MAC_ADDR0Out;
        `ETH_MAC_ADDR1_ADR    :  DataOut<=MAC_ADDR1Out;
        `ETH_TX_BD_NUM_ADR    :  DataOut<=TX_BD_NUMOut;
        `ETH_HASH0_ADR        :  DataOut<=HASH0Out;
        `ETH_HASH1_ADR        :  DataOut<=HASH1Out;
        `ETH_TX_CTRL_ADR      :  DataOut<=TXCTRLOut;

        default:             DataOut<=32'h0;
      endcase
    end
  else
    DataOut<=32'h0;
end


assign r_RecSmall         = MODEROut[16];
assign r_Pad              = MODEROut[15];
assign r_HugEn            = MODEROut[14];
assign r_CrcEn            = MODEROut[13];
assign r_DlyCrcEn         = MODEROut[12];
// assign r_Rst           = MODEROut[11];   This signal is not used any more
assign r_FullD            = MODEROut[10];
assign r_ExDfrEn          = MODEROut[9];
assign r_NoBckof          = MODEROut[8];
assign r_LoopBck          = MODEROut[7];
assign r_IFG              = MODEROut[6];
assign r_Pro              = MODEROut[5];
assign r_Iam              = MODEROut[4];
assign r_Bro              = MODEROut[3];
assign r_NoPre            = MODEROut[2];
assign r_TxEn             = MODEROut[1] & (TX_BD_NUMOut>0);     // Transmission is enabled when there is at least one TxBD.
assign r_RxEn             = MODEROut[0] & (TX_BD_NUMOut<'h80);  // Reception is enabled when there is  at least one RxBD.

assign r_IPGT[6:0]        = IPGTOut[6:0];

assign r_IPGR1[6:0]       = IPGR1Out[6:0];

assign r_IPGR2[6:0]       = IPGR2Out[6:0];

assign r_MinFL[15:0]      = PACKETLENOut[31:16];
assign r_MaxFL[15:0]      = PACKETLENOut[15:0];

assign r_MaxRet[3:0]      = COLLCONFOut[19:16];
assign r_CollValid[5:0]   = COLLCONFOut[5:0];

assign r_TxFlow           = CTRLMODEROut[2];
assign r_RxFlow           = CTRLMODEROut[1];
assign r_PassAll          = CTRLMODEROut[0];

assign r_MiiNoPre         = MIIMODEROut[8];
assign r_ClkDiv[7:0]      = MIIMODEROut[7:0];

assign r_WCtrlData        = MIICOMMANDOut[2];
assign r_RStat            = MIICOMMANDOut[1];
assign r_ScanStat         = MIICOMMANDOut[0];

assign r_RGAD[4:0]        = MIIADDRESSOut[12:8];
assign r_FIAD[4:0]        = MIIADDRESSOut[4:0];

assign r_CtrlData[15:0]   = MIITX_DATAOut[15:0];

assign MIISTATUSOut[31:`ETH_MIISTATUS_WIDTH] = 0; 
assign MIISTATUSOut[2]    = NValid_stat         ; 
assign MIISTATUSOut[1]    = Busy_stat           ; 
assign MIISTATUSOut[0]    = LinkFail            ; 

assign r_MAC[31:0]        = MAC_ADDR0Out[31:0];
assign r_MAC[47:32]       = MAC_ADDR1Out[15:0];
assign r_HASH1[31:0]      = HASH1Out;
assign r_HASH0[31:0]      = HASH0Out;

assign r_TxBDNum[7:0]     = TX_BD_NUMOut[7:0];

assign r_TxPauseTV[15:0]  = TXCTRLOut[15:0];
assign r_TxPauseRq        = TXCTRLOut[16];


// Synchronizing TxC Interrupt
always @ (posedge TxClk or posedge Reset)
begin
  if(Reset)
    SetTxCIrq_txclk <=#Tp 1'b0;
  else
  if(TxCtrlEndFrm & StartTxDone & r_TxFlow)
    SetTxCIrq_txclk <=#Tp 1'b1;
  else
  if(ResetTxCIrq_sync2)
    SetTxCIrq_txclk <=#Tp 1'b0;
end


always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetTxCIrq_sync1 <=#Tp 1'b0;
  else
    SetTxCIrq_sync1 <=#Tp SetTxCIrq_txclk;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetTxCIrq_sync2 <=#Tp 1'b0;
  else
    SetTxCIrq_sync2 <=#Tp SetTxCIrq_sync1;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetTxCIrq_sync3 <=#Tp 1'b0;
  else
    SetTxCIrq_sync3 <=#Tp SetTxCIrq_sync2;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetTxCIrq <=#Tp 1'b0;
  else
    SetTxCIrq <=#Tp SetTxCIrq_sync2 & ~SetTxCIrq_sync3;
end

always @ (posedge TxClk or posedge Reset)
begin
  if(Reset)
    ResetTxCIrq_sync1 <=#Tp 1'b0;
  else
    ResetTxCIrq_sync1 <=#Tp SetTxCIrq_sync2;
end

always @ (posedge TxClk or posedge Reset)
begin
  if(Reset)
    ResetTxCIrq_sync2 <=#Tp 1'b0;
  else
    ResetTxCIrq_sync2 <=#Tp SetTxCIrq_sync1;
end


// Synchronizing RxC Interrupt
always @ (posedge RxClk or posedge Reset)
begin
  if(Reset)
    SetRxCIrq_rxclk <=#Tp 1'b0;
  else
  if(SetPauseTimer & r_RxFlow)
    SetRxCIrq_rxclk <=#Tp 1'b1;
  else
  if(ResetRxCIrq_sync2 & (~ResetRxCIrq_sync3))
    SetRxCIrq_rxclk <=#Tp 1'b0;
end


always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetRxCIrq_sync1 <=#Tp 1'b0;
  else
    SetRxCIrq_sync1 <=#Tp SetRxCIrq_rxclk;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetRxCIrq_sync2 <=#Tp 1'b0;
  else
    SetRxCIrq_sync2 <=#Tp SetRxCIrq_sync1;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetRxCIrq_sync3 <=#Tp 1'b0;
  else
    SetRxCIrq_sync3 <=#Tp SetRxCIrq_sync2;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    SetRxCIrq <=#Tp 1'b0;
  else
    SetRxCIrq <=#Tp SetRxCIrq_sync2 & ~SetRxCIrq_sync3;
end

always @ (posedge RxClk or posedge Reset)
begin
  if(Reset)
    ResetRxCIrq_sync1 <=#Tp 1'b0;
  else
    ResetRxCIrq_sync1 <=#Tp SetRxCIrq_sync2;
end

always @ (posedge RxClk or posedge Reset)
begin
  if(Reset)
    ResetRxCIrq_sync2 <=#Tp 1'b0;
  else
    ResetRxCIrq_sync2 <=#Tp ResetRxCIrq_sync1;
end

always @ (posedge RxClk or posedge Reset)
begin
  if(Reset)
    ResetRxCIrq_sync3 <=#Tp 1'b0;
  else
    ResetRxCIrq_sync3 <=#Tp ResetRxCIrq_sync2;
end



// Interrupt generation
always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    irq_txb <= 1'b0;
  else
  if(TxB_IRQ)
    irq_txb <= #Tp 1'b1;
  else
  if(INT_SOURCE_Wr[0] & DataIn[0])
    irq_txb <= #Tp 1'b0;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    irq_txe <= 1'b0;
  else
  if(TxE_IRQ)
    irq_txe <= #Tp 1'b1;
  else
  if(INT_SOURCE_Wr[0] & DataIn[1])
    irq_txe <= #Tp 1'b0;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    irq_rxb <= 1'b0;
  else
  if(RxB_IRQ)
    irq_rxb <= #Tp 1'b1;
  else
  if(INT_SOURCE_Wr[0] & DataIn[2])
    irq_rxb <= #Tp 1'b0;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    irq_rxe <= 1'b0;
  else
  if(RxE_IRQ)
    irq_rxe <= #Tp 1'b1;
  else
  if(INT_SOURCE_Wr[0] & DataIn[3])
    irq_rxe <= #Tp 1'b0;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    irq_busy <= 1'b0;
  else
  if(Busy_IRQ)
    irq_busy <= #Tp 1'b1;
  else
  if(INT_SOURCE_Wr[0] & DataIn[4])
    irq_busy <= #Tp 1'b0;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    irq_txc <= 1'b0;
  else
  if(SetTxCIrq)
    irq_txc <= #Tp 1'b1;
  else
  if(INT_SOURCE_Wr[0] & DataIn[5])
    irq_txc <= #Tp 1'b0;
end

always @ (posedge Clk or posedge Reset)
begin
  if(Reset)
    irq_rxc <= 1'b0;
  else
  if(SetRxCIrq)
    irq_rxc <= #Tp 1'b1;
  else
  if(INT_SOURCE_Wr[0] & DataIn[6])
    irq_rxc <= #Tp 1'b0;
end

// Generating interrupt signal
assign int_o = irq_txb  & INT_MASKOut[0] | 
               irq_txe  & INT_MASKOut[1] | 
               irq_rxb  & INT_MASKOut[2] | 
               irq_rxe  & INT_MASKOut[3] | 
               irq_busy & INT_MASKOut[4] | 
               irq_txc  & INT_MASKOut[5] | 
               irq_rxc  & INT_MASKOut[6] ;

// For reading interrupt status
assign INT_SOURCEOut = {{(32-`ETH_INT_SOURCE_WIDTH_0){1'b0}}, irq_rxc, irq_txc, irq_busy, irq_rxe, irq_rxb, irq_txe, irq_txb};



endmodule
