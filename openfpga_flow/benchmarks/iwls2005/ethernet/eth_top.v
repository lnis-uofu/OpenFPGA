//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_top.v                                                   ////
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
// $Log: eth_top.v,v $
// Revision 1.52  2005/03/21 20:07:18  igorm
// Some small fixes + some troubles fixed.
//
// Revision 1.51  2005/02/21 11:13:17  igorm
// Defer indication fixed.
//
// Revision 1.50  2004/04/26 15:26:23  igorm
// - Bug connected to the TX_BD_NUM_Wr signal fixed (bug came in with the
//   previous update of the core.
// - TxBDAddress is set to 0 after the TX is enabled in the MODER register.
// - RxBDAddress is set to r_TxBDNum<<1 after the RX is enabled in the MODER
//   register. (thanks to Mathias and Torbjorn)
// - Multicast reception was fixed. Thanks to Ulrich Gries
//
// Revision 1.49  2003/11/12 18:24:59  tadejm
// WISHBONE slave changed and tested from only 32-bit accesss to byte access.
//
// Revision 1.48  2003/10/17 07:46:16  markom
// mbist signals updated according to newest convention
//
// Revision 1.47  2003/10/06 15:43:45  knguyen
// Update RxEnSync only when mrxdv_pad_i is inactive (LOW).
//
// Revision 1.46  2003/01/30 13:30:22  tadejm
// Defer indication changed.
//
// Revision 1.45  2003/01/22 13:49:26  tadejm
// When control packets were received, they were ignored in some cases.
//
// Revision 1.44  2003/01/21 12:09:40  mohor
// When receiving normal data frame and RxFlow control was switched on, RXB
// interrupt was not set.
//
// Revision 1.43  2002/11/22 01:57:06  mohor
// Rx Flow control fixed. CF flag added to the RX buffer descriptor. RxAbort
// synchronized.
//
// Revision 1.42  2002/11/21 00:09:19  mohor
// TPauseRq synchronized to tx_clk.
//
// Revision 1.41  2002/11/19 18:13:49  mohor
// r_MiiMRst is not used for resetting the MIIM module. wb_rst used instead.
//
// Revision 1.40  2002/11/19 17:34:25  mohor
// AddressMiss status is connecting to the Rx BD. AddressMiss is identifying
// that a frame was received because of the promiscous mode.
//
// Revision 1.39  2002/11/18 17:31:55  mohor
// wb_rst_i is used for MIIM reset.
//
// Revision 1.38  2002/11/14 18:37:20  mohor
// r_Rst signal does not reset any module any more and is removed from the design.
//
// Revision 1.37  2002/11/13 22:25:36  tadejm
// All modules are reset with wb_rst instead of the r_Rst. Exception is MII module.
//
// Revision 1.36  2002/10/18 17:04:20  tadejm
// Changed BIST scan signals.
//
// Revision 1.35  2002/10/11 13:36:58  mohor
// Typo error fixed. (When using Bist)
//
// Revision 1.34  2002/10/10 16:49:50  mohor
// Signals for WISHBONE B3 compliant interface added.
//
// Revision 1.33  2002/10/10 16:29:30  mohor
// BIST added.
//
// Revision 1.32  2002/09/20 17:12:58  mohor
// CsMiss added. When address between 0x800 and 0xfff is accessed within
// Ethernet Core, error acknowledge is generated.
//
// Revision 1.31  2002/09/12 14:50:17  mohor
// CarrierSenseLost bug fixed when operating in full duplex mode.
//
// Revision 1.30  2002/09/10 10:35:23  mohor
// Ethernet debug registers removed.
//
// Revision 1.29  2002/09/09 13:03:13  mohor
// Error acknowledge is generated when accessing BDs and RST bit in the
// MODER register (r_Rst) is set.
//
// Revision 1.28  2002/09/04 18:44:10  mohor
// Signals related to the control frames connected. Debug registers reg1, 2, 3, 4
// connected.
//
// Revision 1.27  2002/07/25 18:15:37  mohor
// RxAbort changed. Packets received with MRxErr (from PHY) are also
// aborted.
//
// Revision 1.26  2002/07/17 18:51:50  mohor
// EXTERNAL_DMA removed. External DMA not supported.
//
// Revision 1.25  2002/05/03 10:15:50  mohor
// Outputs registered. Reset changed for eth_wishbone module.
//
// Revision 1.24  2002/04/22 14:15:42  mohor
// Wishbone signals are registered when ETH_REGISTERED_OUTPUTS is
// selected in eth_defines.v
//
// Revision 1.23  2002/03/25 13:33:53  mohor
// md_padoen_o changed to md_padoe_o. Signal was always active high, just
// name was incorrect.
//
// Revision 1.22  2002/02/26 16:59:54  mohor
// Small fixes for external/internal DMA missmatches.
//
// Revision 1.21  2002/02/26 16:21:00  mohor
// Interrupts changed in the top file
//
// Revision 1.20  2002/02/18 10:40:17  mohor
// Small fixes.
//
// Revision 1.19  2002/02/16 14:03:44  mohor
// Registered trimmed. Unused registers removed.
//
// Revision 1.18  2002/02/16 13:06:33  mohor
// EXTERNAL_DMA used instead of WISHBONE_DMA.
//
// Revision 1.17  2002/02/16 07:15:27  mohor
// Testbench fixed, code simplified, unused signals removed.
//
// Revision 1.16  2002/02/15 13:49:39  mohor
// RxAbort is connected differently.
//
// Revision 1.15  2002/02/15 11:38:26  mohor
// Changes that were lost when updating from 1.11 to 1.14 fixed.
//
// Revision 1.14  2002/02/14 20:19:11  billditt
// Modified for Address Checking,
// addition of eth_addrcheck.v
//
// Revision 1.13  2002/02/12 17:03:03  mohor
// HASH0 and HASH1 registers added. Registers address width was
// changed to 8 bits.
//
// Revision 1.12  2002/02/11 09:18:22  mohor
// Tx status is written back to the BD.
//
// Revision 1.11  2002/02/08 16:21:54  mohor
// Rx status is written back to the BD.
//
// Revision 1.10  2002/02/06 14:10:21  mohor
// non-DMA host interface added. Select the right configutation in eth_defines.
//
// Revision 1.9  2002/01/23 10:28:16  mohor
// Link in the header changed.
//
// Revision 1.8  2001/12/05 15:00:16  mohor
// RX_BD_NUM changed to TX_BD_NUM (holds number of TX descriptors
// instead of the number of RX descriptors).
//
// Revision 1.7  2001/12/05 10:45:59  mohor
// ETH_RX_BD_ADR register deleted. ETH_RX_BD_NUM is used instead.
//
// Revision 1.6  2001/10/19 11:24:29  mohor
// Number of addresses (wb_adr_i) minimized.
//
// Revision 1.5  2001/10/19 08:43:51  mohor
// eth_timescale.v changed to timescale.v This is done because of the
// simulation of the few cores in a one joined project.
//
// Revision 1.4  2001/10/18 12:07:11  mohor
// Status signals changed, Adress decoding changed, interrupt controller
// added.
//
// Revision 1.3  2001/09/24 15:02:56  mohor
// Defines changed (All precede with ETH_). Small changes because some
// tools generate warnings when two operands are together. Synchronization
// between two clocks domains in eth_wishbonedma.v is changed (due to ASIC
// demands).
//
// Revision 1.2  2001/08/15 14:03:59  mohor
// Signal names changed on the top level for easier pad insertion (ASIC).
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


`include "eth_defines.v"
`include "timescale.v"


module eth_top
(
  // WISHBONE common
  wb_clk_i, wb_rst_i, wb_dat_i, wb_dat_o, 

  // WISHBONE slave
  wb_adr_i, wb_sel_i, wb_we_i, wb_cyc_i, wb_stb_i, wb_ack_o, wb_err_o, 

  // WISHBONE master
  m_wb_adr_o, m_wb_sel_o, m_wb_we_o, 
  m_wb_dat_o, m_wb_dat_i, m_wb_cyc_o, 
  m_wb_stb_o, m_wb_ack_i, m_wb_err_i, 

`ifdef ETH_WISHBONE_B3
  m_wb_cti_o, m_wb_bte_o, 
`endif

  //TX
  mtx_clk_pad_i, mtxd_pad_o, mtxen_pad_o, mtxerr_pad_o,

  //RX
  mrx_clk_pad_i, mrxd_pad_i, mrxdv_pad_i, mrxerr_pad_i, mcoll_pad_i, mcrs_pad_i, 
  
  // MIIM
  mdc_pad_o, md_pad_i, md_pad_o, md_padoe_o,

  int_o

  // Bist
`ifdef ETH_BIST
  ,
  // debug chain signals
  mbist_si_i,       // bist scan serial in
  mbist_so_o,       // bist scan serial out
  mbist_ctrl_i        // bist chain shift control
`endif

);


parameter Tp = 1;


// WISHBONE common
input           wb_clk_i;     // WISHBONE clock
input           wb_rst_i;     // WISHBONE reset
input   [31:0]  wb_dat_i;     // WISHBONE data input
output  [31:0]  wb_dat_o;     // WISHBONE data output
output          wb_err_o;     // WISHBONE error output

// WISHBONE slave
input   [11:2]  wb_adr_i;     // WISHBONE address input
input    [3:0]  wb_sel_i;     // WISHBONE byte select input
input           wb_we_i;      // WISHBONE write enable input
input           wb_cyc_i;     // WISHBONE cycle input
input           wb_stb_i;     // WISHBONE strobe input
output          wb_ack_o;     // WISHBONE acknowledge output

// WISHBONE master
output  [31:0]  m_wb_adr_o;
output   [3:0]  m_wb_sel_o;
output          m_wb_we_o;
input   [31:0]  m_wb_dat_i;
output  [31:0]  m_wb_dat_o;
output          m_wb_cyc_o;
output          m_wb_stb_o;
input           m_wb_ack_i;
input           m_wb_err_i;

wire    [29:0]  m_wb_adr_tmp;

`ifdef ETH_WISHBONE_B3
output   [2:0]  m_wb_cti_o;   // Cycle Type Identifier
output   [1:0]  m_wb_bte_o;   // Burst Type Extension
`endif

// Tx
input           mtx_clk_pad_i; // Transmit clock (from PHY)
output   [3:0]  mtxd_pad_o;    // Transmit nibble (to PHY)
output          mtxen_pad_o;   // Transmit enable (to PHY)
output          mtxerr_pad_o;  // Transmit error (to PHY)

// Rx
input           mrx_clk_pad_i; // Receive clock (from PHY)
input    [3:0]  mrxd_pad_i;    // Receive nibble (from PHY)
input           mrxdv_pad_i;   // Receive data valid (from PHY)
input           mrxerr_pad_i;  // Receive data error (from PHY)

// Common Tx and Rx
input           mcoll_pad_i;   // Collision (from PHY)
input           mcrs_pad_i;    // Carrier sense (from PHY)

// MII Management interface
input           md_pad_i;      // MII data input (from I/O cell)
output          mdc_pad_o;     // MII Management data clock (to PHY)
output          md_pad_o;      // MII data output (to I/O cell)
output          md_padoe_o;    // MII data output enable (to I/O cell)

output          int_o;         // Interrupt output

// Bist
`ifdef ETH_BIST
input   mbist_si_i;       // bist scan serial in
output  mbist_so_o;       // bist scan serial out
input [`ETH_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i;       // bist chain shift control
`endif

wire     [7:0]  r_ClkDiv;
wire            r_MiiNoPre;
wire    [15:0]  r_CtrlData;
wire     [4:0]  r_FIAD;
wire     [4:0]  r_RGAD;
wire            r_WCtrlData;
wire            r_RStat;
wire            r_ScanStat;
wire            NValid_stat;
wire            Busy_stat;
wire            LinkFail;
wire    [15:0]  Prsd;             // Read Status Data (data read from the PHY)
wire            WCtrlDataStart;
wire            RStatStart;
wire            UpdateMIIRX_DATAReg;

wire            TxStartFrm;
wire            TxEndFrm;
wire            TxUsedData;
wire     [7:0]  TxData;
wire            TxRetry;
wire            TxAbort;
wire            TxUnderRun;
wire            TxDone;


reg             WillSendControlFrame_sync1;
reg             WillSendControlFrame_sync2;
reg             WillSendControlFrame_sync3;
reg             RstTxPauseRq;

reg             TxPauseRq_sync1;
reg             TxPauseRq_sync2;
reg             TxPauseRq_sync3;
reg             TPauseRq;


// Connecting Miim module
eth_miim miim1
(
  .Clk(wb_clk_i),                         .Reset(wb_rst_i),                   .Divider(r_ClkDiv), 
  .NoPre(r_MiiNoPre),                     .CtrlData(r_CtrlData),              .Rgad(r_RGAD), 
  .Fiad(r_FIAD),                          .WCtrlData(r_WCtrlData),            .RStat(r_RStat), 
  .ScanStat(r_ScanStat),                  .Mdi(md_pad_i),                     .Mdo(md_pad_o), 
  .MdoEn(md_padoe_o),                     .Mdc(mdc_pad_o),                    .Busy(Busy_stat), 
  .Prsd(Prsd),                            .LinkFail(LinkFail),                .Nvalid(NValid_stat), 
  .WCtrlDataStart(WCtrlDataStart),        .RStatStart(RStatStart),            .UpdateMIIRX_DATAReg(UpdateMIIRX_DATAReg)
);




wire  [3:0] RegCs;          // Connected to registers
wire [31:0] RegDataOut;     // Multiplexed to wb_dat_o
wire        r_RecSmall;     // Receive small frames
wire        r_LoopBck;      // Loopback
wire        r_TxEn;         // Tx Enable
wire        r_RxEn;         // Rx Enable

wire        MRxDV_Lb;       // Muxed MII receive data valid
wire        MRxErr_Lb;      // Muxed MII Receive Error
wire  [3:0] MRxD_Lb;        // Muxed MII Receive Data
wire        Transmitting;   // Indication that TxEthMAC is transmitting
wire        r_HugEn;        // Huge packet enable
wire        r_DlyCrcEn;     // Delayed CRC enabled
wire [15:0] r_MaxFL;        // Maximum frame length

wire [15:0] r_MinFL;        // Minimum frame length
wire        ShortFrame;
wire        DribbleNibble;  // Extra nibble received
wire        ReceivedPacketTooBig; // Received packet is too big
wire [47:0] r_MAC;          // MAC address
wire        LoadRxStatus;   // Rx status was loaded
wire [31:0] r_HASH0;        // HASH table, lower 4 bytes
wire [31:0] r_HASH1;        // HASH table, upper 4 bytes
wire  [7:0] r_TxBDNum;      // Receive buffer descriptor number
wire  [6:0] r_IPGT;         // 
wire  [6:0] r_IPGR1;        // 
wire  [6:0] r_IPGR2;        // 
wire  [5:0] r_CollValid;    // 
wire [15:0] r_TxPauseTV;    // Transmit PAUSE value
wire        r_TxPauseRq;    // Transmit PAUSE request

wire  [3:0] r_MaxRet;       //
wire        r_NoBckof;      // 
wire        r_ExDfrEn;      // 
wire        r_TxFlow;       // Tx flow control enable
wire        r_IFG;          // Minimum interframe gap for incoming packets

wire        TxB_IRQ;        // Interrupt Tx Buffer
wire        TxE_IRQ;        // Interrupt Tx Error
wire        RxB_IRQ;        // Interrupt Rx Buffer
wire        RxE_IRQ;        // Interrupt Rx Error
wire        Busy_IRQ;       // Interrupt Busy (lack of buffers)

//wire        DWord;
wire        ByteSelected;
wire        BDAck;
wire [31:0] BD_WB_DAT_O;    // wb_dat_o that comes from the Wishbone module (for buffer descriptors read/write)
wire  [3:0] BDCs;           // Buffer descriptor CS
wire        CsMiss;         // When access to the address between 0x800 and 0xfff occurs, acknowledge is set
                            // but data is not valid.
wire        r_Pad;
wire        r_CrcEn;
wire        r_FullD;
wire        r_Pro;
wire        r_Bro;
wire        r_NoPre;
wire        r_RxFlow;
wire        r_PassAll;
wire        TxCtrlEndFrm;
wire        StartTxDone;
wire        SetPauseTimer;
wire        TxUsedDataIn;
wire        TxDoneIn;
wire        TxAbortIn;
wire        PerPacketPad;
wire        PadOut;
wire        PerPacketCrcEn;
wire        CrcEnOut;
wire        TxStartFrmOut;
wire        TxEndFrmOut;
wire        ReceivedPauseFrm;
wire        ControlFrmAddressOK;
wire        RxStatusWriteLatched_sync2;
wire        LateCollision;
wire        DeferIndication;
wire        LateCollLatched;
wire        DeferLatched;
wire        RstDeferLatched;
wire        CarrierSenseLost;

wire        temp_wb_ack_o;
wire [31:0] temp_wb_dat_o;
wire        temp_wb_err_o;

`ifdef ETH_REGISTERED_OUTPUTS
  reg         temp_wb_ack_o_reg;
  reg [31:0]  temp_wb_dat_o_reg;
  reg         temp_wb_err_o_reg;
`endif

//assign DWord = &wb_sel_i;
assign ByteSelected = |wb_sel_i;
assign RegCs[3] = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] & ~wb_adr_i[10] & wb_sel_i[3];   // 0x0   - 0x3FF
assign RegCs[2] = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] & ~wb_adr_i[10] & wb_sel_i[2];   // 0x0   - 0x3FF
assign RegCs[1] = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] & ~wb_adr_i[10] & wb_sel_i[1];   // 0x0   - 0x3FF
assign RegCs[0] = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] & ~wb_adr_i[10] & wb_sel_i[0];   // 0x0   - 0x3FF
assign BDCs[3]  = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] &  wb_adr_i[10] & wb_sel_i[3];   // 0x400 - 0x7FF
assign BDCs[2]  = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] &  wb_adr_i[10] & wb_sel_i[2];   // 0x400 - 0x7FF
assign BDCs[1]  = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] &  wb_adr_i[10] & wb_sel_i[1];   // 0x400 - 0x7FF
assign BDCs[0]  = wb_stb_i & wb_cyc_i & ByteSelected & ~wb_adr_i[11] &  wb_adr_i[10] & wb_sel_i[0];   // 0x400 - 0x7FF
assign CsMiss = wb_stb_i & wb_cyc_i & ByteSelected & wb_adr_i[11];                   // 0x800 - 0xfFF
assign temp_wb_dat_o = ((|RegCs) & ~wb_we_i)? RegDataOut : BD_WB_DAT_O;
assign temp_wb_err_o = wb_stb_i & wb_cyc_i & (~ByteSelected | CsMiss);

`ifdef ETH_REGISTERED_OUTPUTS
  assign wb_ack_o = temp_wb_ack_o_reg;
  assign wb_dat_o[31:0] = temp_wb_dat_o_reg;
  assign wb_err_o = temp_wb_err_o_reg;
`else
  assign wb_ack_o = temp_wb_ack_o;
  assign wb_dat_o[31:0] = temp_wb_dat_o;
  assign wb_err_o = temp_wb_err_o;
`endif

`ifdef ETH_AVALON_BUS
  // As Avalon has no corresponding "error" signal, I (erroneously) will
  // send an ack to Avalon, even when accessing undefined memory. This
  // is a grey area in Avalon vs. Wishbone specs: My understanding
  // is that Avalon expects all memory addressable by the addr bus feeding
  // a slave to be, at the very minimum, readable.
  assign temp_wb_ack_o = (|RegCs) | BDAck | CsMiss;
`else // WISHBONE
  assign temp_wb_ack_o = (|RegCs) | BDAck;
`endif

`ifdef ETH_REGISTERED_OUTPUTS
  always @ (posedge wb_clk_i or posedge wb_rst_i)
  begin
    if(wb_rst_i)
      begin
        temp_wb_ack_o_reg <=#Tp 1'b0;
        temp_wb_dat_o_reg <=#Tp 32'h0;
        temp_wb_err_o_reg <=#Tp 1'b0;
      end
    else
      begin
        temp_wb_ack_o_reg <=#Tp temp_wb_ack_o & ~temp_wb_ack_o_reg;
        temp_wb_dat_o_reg <=#Tp temp_wb_dat_o;
        temp_wb_err_o_reg <=#Tp temp_wb_err_o & ~temp_wb_err_o_reg;
      end
  end
`endif


// Connecting Ethernet registers
eth_registers ethreg1
(
  .DataIn(wb_dat_i),                      .Address(wb_adr_i[9:2]),                    .Rw(wb_we_i), 
  .Cs(RegCs),                             .Clk(wb_clk_i),                             .Reset(wb_rst_i), 
  .DataOut(RegDataOut),                   .r_RecSmall(r_RecSmall), 
  .r_Pad(r_Pad),                          .r_HugEn(r_HugEn),                          .r_CrcEn(r_CrcEn), 
  .r_DlyCrcEn(r_DlyCrcEn),                .r_FullD(r_FullD), 
  .r_ExDfrEn(r_ExDfrEn),                  .r_NoBckof(r_NoBckof),                      .r_LoopBck(r_LoopBck), 
  .r_IFG(r_IFG),                          .r_Pro(r_Pro),                              .r_Iam(), 
  .r_Bro(r_Bro),                          .r_NoPre(r_NoPre),                          .r_TxEn(r_TxEn), 
  .r_RxEn(r_RxEn),                        .Busy_IRQ(Busy_IRQ),                        .RxE_IRQ(RxE_IRQ), 
  .RxB_IRQ(RxB_IRQ),                      .TxE_IRQ(TxE_IRQ),                          .TxB_IRQ(TxB_IRQ), 
  .r_IPGT(r_IPGT), 
  .r_IPGR1(r_IPGR1),                      .r_IPGR2(r_IPGR2),                          .r_MinFL(r_MinFL), 
  .r_MaxFL(r_MaxFL),                      .r_MaxRet(r_MaxRet),                        .r_CollValid(r_CollValid), 
  .r_TxFlow(r_TxFlow),                    .r_RxFlow(r_RxFlow),                        .r_PassAll(r_PassAll), 
  .r_MiiNoPre(r_MiiNoPre),                .r_ClkDiv(r_ClkDiv), 
  .r_WCtrlData(r_WCtrlData),              .r_RStat(r_RStat),                          .r_ScanStat(r_ScanStat), 
  .r_RGAD(r_RGAD),                        .r_FIAD(r_FIAD),                            .r_CtrlData(r_CtrlData), 
  .NValid_stat(NValid_stat),              .Busy_stat(Busy_stat),                   
  .LinkFail(LinkFail),                    .r_MAC(r_MAC),                              .WCtrlDataStart(WCtrlDataStart),
  .RStatStart(RStatStart),                .UpdateMIIRX_DATAReg(UpdateMIIRX_DATAReg),  .Prsd(Prsd), 
  .r_TxBDNum(r_TxBDNum),                  .int_o(int_o),
  .r_HASH0(r_HASH0),                      .r_HASH1(r_HASH1),                          .r_TxPauseRq(r_TxPauseRq), 
  .r_TxPauseTV(r_TxPauseTV),              .RstTxPauseRq(RstTxPauseRq),                .TxCtrlEndFrm(TxCtrlEndFrm), 
  .StartTxDone(StartTxDone),              .TxClk(mtx_clk_pad_i),                      .RxClk(mrx_clk_pad_i), 
  .SetPauseTimer(SetPauseTimer)
  
);



wire  [7:0] RxData;
wire        RxValid;
wire        RxStartFrm;
wire        RxEndFrm;
wire        RxAbort;

wire        WillTransmit;            // Will transmit (to RxEthMAC)
wire        ResetCollision;          // Reset Collision (for synchronizing collision)
wire  [7:0] TxDataOut;               // Transmit Packet Data (to TxEthMAC)
wire        WillSendControlFrame;
wire        ReceiveEnd;
wire        ReceivedPacketGood;
wire        ReceivedLengthOK;
wire        InvalidSymbol;
wire        LatchedCrcError;
wire        RxLateCollision;
wire  [3:0] RetryCntLatched;   
wire  [3:0] RetryCnt;   
wire        StartTxAbort;   
wire        MaxCollisionOccured;   
wire        RetryLimit;   
wire        StatePreamble;   
wire  [1:0] StateData; 

// Connecting MACControl
eth_maccontrol maccontrol1
(
  .MTxClk(mtx_clk_pad_i),                       .TPauseRq(TPauseRq), 
  .TxPauseTV(r_TxPauseTV),                      .TxDataIn(TxData), 
  .TxStartFrmIn(TxStartFrm),                    .TxEndFrmIn(TxEndFrm), 
  .TxUsedDataIn(TxUsedDataIn),                  .TxDoneIn(TxDoneIn), 
  .TxAbortIn(TxAbortIn),                        .MRxClk(mrx_clk_pad_i), 
  .RxData(RxData),                              .RxValid(RxValid), 
  .RxStartFrm(RxStartFrm),                      .RxEndFrm(RxEndFrm),
  .ReceiveEnd(ReceiveEnd),                      .ReceivedPacketGood(ReceivedPacketGood),
  .TxFlow(r_TxFlow), 
  .RxFlow(r_RxFlow),                            .DlyCrcEn(r_DlyCrcEn),
  .MAC(r_MAC),                                  .PadIn(r_Pad | PerPacketPad), 
  .PadOut(PadOut),                              .CrcEnIn(r_CrcEn | PerPacketCrcEn), 
  .CrcEnOut(CrcEnOut),                          .TxReset(wb_rst_i), 
  .RxReset(wb_rst_i),                           .ReceivedLengthOK(ReceivedLengthOK),
  .TxDataOut(TxDataOut),                        .TxStartFrmOut(TxStartFrmOut), 
  .TxEndFrmOut(TxEndFrmOut),                    .TxUsedDataOut(TxUsedData), 
  .TxDoneOut(TxDone),                           .TxAbortOut(TxAbort), 
  .WillSendControlFrame(WillSendControlFrame),  .TxCtrlEndFrm(TxCtrlEndFrm), 
  .ReceivedPauseFrm(ReceivedPauseFrm),          .ControlFrmAddressOK(ControlFrmAddressOK),
  .SetPauseTimer(SetPauseTimer),
  .RxStatusWriteLatched_sync2(RxStatusWriteLatched_sync2),                .r_PassAll(r_PassAll)
);



wire TxCarrierSense;          // Synchronized CarrierSense (to Tx clock)
wire Collision;               // Synchronized Collision

reg CarrierSense_Tx1;
reg CarrierSense_Tx2;
reg Collision_Tx1;
reg Collision_Tx2;

reg RxEnSync;                 // Synchronized Receive Enable
reg WillTransmit_q;
reg WillTransmit_q2;



// Muxed MII receive data valid
assign MRxDV_Lb = r_LoopBck? mtxen_pad_o : mrxdv_pad_i & RxEnSync;

// Muxed MII Receive Error
assign MRxErr_Lb = r_LoopBck? mtxerr_pad_o : mrxerr_pad_i & RxEnSync;

// Muxed MII Receive Data
assign MRxD_Lb[3:0] = r_LoopBck? mtxd_pad_o[3:0] : mrxd_pad_i[3:0];



// Connecting TxEthMAC
eth_txethmac txethmac1
(
  .MTxClk(mtx_clk_pad_i),             .Reset(wb_rst_i),                   .CarrierSense(TxCarrierSense), 
  .Collision(Collision),              .TxData(TxDataOut),                 .TxStartFrm(TxStartFrmOut), 
  .TxUnderRun(TxUnderRun),            .TxEndFrm(TxEndFrmOut),             .Pad(PadOut),  
  .MinFL(r_MinFL),                    .CrcEn(CrcEnOut),                   .FullD(r_FullD), 
  .HugEn(r_HugEn),                    .DlyCrcEn(r_DlyCrcEn),              .IPGT(r_IPGT), 
  .IPGR1(r_IPGR1),                    .IPGR2(r_IPGR2),                    .CollValid(r_CollValid), 
  .MaxRet(r_MaxRet),                  .NoBckof(r_NoBckof),                .ExDfrEn(r_ExDfrEn), 
  .MaxFL(r_MaxFL),                    .MTxEn(mtxen_pad_o),                .MTxD(mtxd_pad_o), 
  .MTxErr(mtxerr_pad_o),              .TxUsedData(TxUsedDataIn),          .TxDone(TxDoneIn), 
  .TxRetry(TxRetry),                  .TxAbort(TxAbortIn),                .WillTransmit(WillTransmit), 
  .ResetCollision(ResetCollision),    .RetryCnt(RetryCnt),                .StartTxDone(StartTxDone), 
  .StartTxAbort(StartTxAbort),        .MaxCollisionOccured(MaxCollisionOccured), .LateCollision(LateCollision),   
  .DeferIndication(DeferIndication),  .StatePreamble(StatePreamble),      .StateData(StateData)   
);




wire  [15:0]  RxByteCnt;
wire          RxByteCntEq0;
wire          RxByteCntGreat2;
wire          RxByteCntMaxFrame;
wire          RxCrcError;
wire          RxStateIdle;
wire          RxStatePreamble;
wire          RxStateSFD;
wire   [1:0]  RxStateData;
wire          AddressMiss;



// Connecting RxEthMAC
eth_rxethmac rxethmac1
(
  .MRxClk(mrx_clk_pad_i),               .MRxDV(MRxDV_Lb),                     .MRxD(MRxD_Lb),
  .Transmitting(Transmitting),          .HugEn(r_HugEn),                      .DlyCrcEn(r_DlyCrcEn), 
  .MaxFL(r_MaxFL),                      .r_IFG(r_IFG),                        .Reset(wb_rst_i),
  .RxData(RxData),                      .RxValid(RxValid),                    .RxStartFrm(RxStartFrm), 
  .RxEndFrm(RxEndFrm),                  .ByteCnt(RxByteCnt), 
  .ByteCntEq0(RxByteCntEq0),            .ByteCntGreat2(RxByteCntGreat2),      .ByteCntMaxFrame(RxByteCntMaxFrame), 
  .CrcError(RxCrcError),                .StateIdle(RxStateIdle),              .StatePreamble(RxStatePreamble), 
  .StateSFD(RxStateSFD),                .StateData(RxStateData),
  .MAC(r_MAC),                          .r_Pro(r_Pro),                        .r_Bro(r_Bro),
  .r_HASH0(r_HASH0),                    .r_HASH1(r_HASH1),                    .RxAbort(RxAbort), 
  .AddressMiss(AddressMiss),            .PassAll(r_PassAll),                  .ControlFrmAddressOK(ControlFrmAddressOK)
);


// MII Carrier Sense Synchronization
always @ (posedge mtx_clk_pad_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    begin
      CarrierSense_Tx1 <= #Tp 1'b0;
      CarrierSense_Tx2 <= #Tp 1'b0;
    end
  else
    begin
      CarrierSense_Tx1 <= #Tp mcrs_pad_i;
      CarrierSense_Tx2 <= #Tp CarrierSense_Tx1;
    end
end

assign TxCarrierSense = ~r_FullD & CarrierSense_Tx2;


// MII Collision Synchronization
always @ (posedge mtx_clk_pad_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    begin
      Collision_Tx1 <= #Tp 1'b0;
      Collision_Tx2 <= #Tp 1'b0;
    end
  else
    begin
      Collision_Tx1 <= #Tp mcoll_pad_i;
      if(ResetCollision)
        Collision_Tx2 <= #Tp 1'b0;
      else
      if(Collision_Tx1)
        Collision_Tx2 <= #Tp 1'b1;
    end
end


// Synchronized Collision
assign Collision = ~r_FullD & Collision_Tx2;



// Delayed WillTransmit
always @ (posedge mrx_clk_pad_i)
begin
  WillTransmit_q <= #Tp WillTransmit;
  WillTransmit_q2 <= #Tp WillTransmit_q;
end 


assign Transmitting = ~r_FullD & WillTransmit_q2;



// Synchronized Receive Enable
always @ (posedge mrx_clk_pad_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    RxEnSync <= #Tp 1'b0;
  else
  if(~mrxdv_pad_i)
    RxEnSync <= #Tp r_RxEn;
end 



// Synchronizing WillSendControlFrame to WB_CLK;
always @ (posedge wb_clk_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    WillSendControlFrame_sync1 <= 1'b0;
  else
    WillSendControlFrame_sync1 <=#Tp WillSendControlFrame;
end

always @ (posedge wb_clk_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    WillSendControlFrame_sync2 <= 1'b0;
  else
    WillSendControlFrame_sync2 <=#Tp WillSendControlFrame_sync1;
end

always @ (posedge wb_clk_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    WillSendControlFrame_sync3 <= 1'b0;
  else
    WillSendControlFrame_sync3 <=#Tp WillSendControlFrame_sync2;
end

always @ (posedge wb_clk_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    RstTxPauseRq <= 1'b0;
  else
    RstTxPauseRq <=#Tp WillSendControlFrame_sync2 & ~WillSendControlFrame_sync3;
end




// TX Pause request Synchronization
always @ (posedge mtx_clk_pad_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    begin
      TxPauseRq_sync1 <= #Tp 1'b0;
      TxPauseRq_sync2 <= #Tp 1'b0;
      TxPauseRq_sync3 <= #Tp 1'b0;
    end
  else
    begin
      TxPauseRq_sync1 <= #Tp (r_TxPauseRq & r_TxFlow);
      TxPauseRq_sync2 <= #Tp TxPauseRq_sync1;
      TxPauseRq_sync3 <= #Tp TxPauseRq_sync2;
    end
end


always @ (posedge mtx_clk_pad_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    TPauseRq <= #Tp 1'b0;
  else
    TPauseRq <= #Tp TxPauseRq_sync2 & (~TxPauseRq_sync3);
end


wire LatchedMRxErr;
reg RxAbort_latch;
reg RxAbort_sync1;
reg RxAbort_wb;
reg RxAbortRst_sync1;
reg RxAbortRst;

// Synchronizing RxAbort to the WISHBONE clock
always @ (posedge mrx_clk_pad_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    RxAbort_latch <= #Tp 1'b0;
  else if(RxAbort | (ShortFrame & ~r_RecSmall) | LatchedMRxErr & ~InvalidSymbol | (ReceivedPauseFrm & (~r_PassAll)))
    RxAbort_latch <= #Tp 1'b1;
  else if(RxAbortRst)
    RxAbort_latch <= #Tp 1'b0;
end

always @ (posedge wb_clk_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    begin
      RxAbort_sync1 <= #Tp 1'b0;
      RxAbort_wb    <= #Tp 1'b0;
      RxAbort_wb    <= #Tp 1'b0;
    end
  else
    begin
      RxAbort_sync1 <= #Tp RxAbort_latch;
      RxAbort_wb    <= #Tp RxAbort_sync1;
    end
end

always @ (posedge mrx_clk_pad_i or posedge wb_rst_i)
begin
  if(wb_rst_i)
    begin
      RxAbortRst_sync1 <= #Tp 1'b0;
      RxAbortRst       <= #Tp 1'b0;
    end
  else
    begin
      RxAbortRst_sync1 <= #Tp RxAbort_wb;
      RxAbortRst       <= #Tp RxAbortRst_sync1;
    end
end



// Connecting Wishbone module
eth_wishbone wishbone
(
  .WB_CLK_I(wb_clk_i),                .WB_DAT_I(wb_dat_i), 
  .WB_DAT_O(BD_WB_DAT_O), 

  // WISHBONE slave
  .WB_ADR_I(wb_adr_i[9:2]),           .WB_WE_I(wb_we_i), 
  .BDCs(BDCs),                        .WB_ACK_O(BDAck), 

  .Reset(wb_rst_i), 

  // WISHBONE master
  .m_wb_adr_o(m_wb_adr_tmp),          .m_wb_sel_o(m_wb_sel_o),                  .m_wb_we_o(m_wb_we_o), 
  .m_wb_dat_i(m_wb_dat_i),            .m_wb_dat_o(m_wb_dat_o),                  .m_wb_cyc_o(m_wb_cyc_o), 
  .m_wb_stb_o(m_wb_stb_o),            .m_wb_ack_i(m_wb_ack_i),                  .m_wb_err_i(m_wb_err_i), 
  
`ifdef ETH_WISHBONE_B3
  .m_wb_cti_o(m_wb_cti_o),            .m_wb_bte_o(m_wb_bte_o), 
`endif
  

    //TX
  .MTxClk(mtx_clk_pad_i),             .TxStartFrm(TxStartFrm),                  .TxEndFrm(TxEndFrm), 
  .TxUsedData(TxUsedData),            .TxData(TxData), 
  .TxRetry(TxRetry),                  .TxAbort(TxAbort),                        .TxUnderRun(TxUnderRun), 
  .TxDone(TxDone), 
  .PerPacketCrcEn(PerPacketCrcEn),    .PerPacketPad(PerPacketPad), 

  // Register
  .r_TxEn(r_TxEn),                    .r_RxEn(r_RxEn),                          .r_TxBDNum(r_TxBDNum), 
  .r_RxFlow(r_RxFlow),                      .r_PassAll(r_PassAll), 

  //RX
  .MRxClk(mrx_clk_pad_i),             .RxData(RxData),                          .RxValid(RxValid), 
  .RxStartFrm(RxStartFrm),            .RxEndFrm(RxEndFrm),                      
  .Busy_IRQ(Busy_IRQ),                .RxE_IRQ(RxE_IRQ),                        .RxB_IRQ(RxB_IRQ), 
  .TxE_IRQ(TxE_IRQ),                  .TxB_IRQ(TxB_IRQ), 

  .RxAbort(RxAbort_wb),               .RxStatusWriteLatched_sync2(RxStatusWriteLatched_sync2), 

  .InvalidSymbol(InvalidSymbol),      .LatchedCrcError(LatchedCrcError),        .RxLength(RxByteCnt),
  .RxLateCollision(RxLateCollision),  .ShortFrame(ShortFrame),                  .DribbleNibble(DribbleNibble),
  .ReceivedPacketTooBig(ReceivedPacketTooBig), .LoadRxStatus(LoadRxStatus),     .RetryCntLatched(RetryCntLatched),
  .RetryLimit(RetryLimit),            .LateCollLatched(LateCollLatched),        .DeferLatched(DeferLatched),   
  .RstDeferLatched(RstDeferLatched), 
  .CarrierSenseLost(CarrierSenseLost),.ReceivedPacketGood(ReceivedPacketGood),  .AddressMiss(AddressMiss),
  .ReceivedPauseFrm(ReceivedPauseFrm)
  
`ifdef ETH_BIST
  ,
  .mbist_si_i       (mbist_si_i),
  .mbist_so_o       (mbist_so_o),
  .mbist_ctrl_i       (mbist_ctrl_i)
`endif
);

assign m_wb_adr_o = {m_wb_adr_tmp, 2'h0};

// Connecting MacStatus module
eth_macstatus macstatus1 
(
  .MRxClk(mrx_clk_pad_i),             .Reset(wb_rst_i),
  .ReceiveEnd(ReceiveEnd),            .ReceivedPacketGood(ReceivedPacketGood),     .ReceivedLengthOK(ReceivedLengthOK), 
  .RxCrcError(RxCrcError),            .MRxErr(MRxErr_Lb),                          .MRxDV(MRxDV_Lb), 
  .RxStateSFD(RxStateSFD),            .RxStateData(RxStateData),                   .RxStatePreamble(RxStatePreamble), 
  .RxStateIdle(RxStateIdle),          .Transmitting(Transmitting),                 .RxByteCnt(RxByteCnt), 
  .RxByteCntEq0(RxByteCntEq0),        .RxByteCntGreat2(RxByteCntGreat2),           .RxByteCntMaxFrame(RxByteCntMaxFrame), 
  .InvalidSymbol(InvalidSymbol),
  .MRxD(MRxD_Lb),                     .LatchedCrcError(LatchedCrcError),           .Collision(mcoll_pad_i),
  .CollValid(r_CollValid),            .RxLateCollision(RxLateCollision),           .r_RecSmall(r_RecSmall),
  .r_MinFL(r_MinFL),                  .r_MaxFL(r_MaxFL),                           .ShortFrame(ShortFrame),
  .DribbleNibble(DribbleNibble),      .ReceivedPacketTooBig(ReceivedPacketTooBig), .r_HugEn(r_HugEn),
  .LoadRxStatus(LoadRxStatus),        .RetryCnt(RetryCnt),                         .StartTxDone(StartTxDone),
  .StartTxAbort(StartTxAbort),        .RetryCntLatched(RetryCntLatched),           .MTxClk(mtx_clk_pad_i),
  .MaxCollisionOccured(MaxCollisionOccured), .RetryLimit(RetryLimit),              .LateCollision(LateCollision),
  .LateCollLatched(LateCollLatched),  .DeferIndication(DeferIndication),           .DeferLatched(DeferLatched),
  .RstDeferLatched(RstDeferLatched), 
  .TxStartFrm(TxStartFrmOut),         .StatePreamble(StatePreamble),               .StateData(StateData),
  .CarrierSense(CarrierSense_Tx2),    .CarrierSenseLost(CarrierSenseLost),         .TxUsedData(TxUsedDataIn),
  .LatchedMRxErr(LatchedMRxErr),      .Loopback(r_LoopBck),                        .r_FullD(r_FullD)
);


endmodule
