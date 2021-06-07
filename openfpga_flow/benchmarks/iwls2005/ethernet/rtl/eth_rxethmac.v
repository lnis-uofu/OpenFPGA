//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_rxethmac.v                                              ////
////                                                              ////
////  This file is part of the Ethernet IP core project           ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Igor Mohor (igorM@opencores.org)                      ////
////      - Novan Hartadi (novan@vlsi.itb.ac.id)                  ////
////      - Mahmud Galela (mgalela@vlsi.itb.ac.id)                ////
////                                                              ////
////  All additional information is avaliable in the Readme.txt   ////
////  file.                                                       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2001 Authors                                   ////
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
// $Log: eth_rxethmac.v,v $
// Revision 1.13  2005/02/21 12:48:07  igorm
// Warning fixes.
//
// Revision 1.12  2004/04/26 15:26:23  igorm
// - Bug connected to the TX_BD_NUM_Wr signal fixed (bug came in with the
//   previous update of the core.
// - TxBDAddress is set to 0 after the TX is enabled in the MODER register.
// - RxBDAddress is set to r_TxBDNum<<1 after the RX is enabled in the MODER
//   register. (thanks to Mathias and Torbjorn)
// - Multicast reception was fixed. Thanks to Ulrich Gries
//
// Revision 1.11  2004/03/17 09:32:15  igorm
// Multicast detection fixed. Only the LSB of the first byte is checked.
//
// Revision 1.10  2002/11/22 01:57:06  mohor
// Rx Flow control fixed. CF flag added to the RX buffer descriptor. RxAbort
// synchronized.
//
// Revision 1.9  2002/11/19 17:35:35  mohor
// AddressMiss status is connecting to the Rx BD. AddressMiss is identifying
// that a frame was received because of the promiscous mode.
//
// Revision 1.8  2002/02/16 07:15:27  mohor
// Testbench fixed, code simplified, unused signals removed.
//
// Revision 1.7  2002/02/15 13:44:28  mohor
// RxAbort is an output. No need to have is declared as wire.
//
// Revision 1.6  2002/02/15 11:17:48  mohor
// File format changed.
//
// Revision 1.5  2002/02/14 20:48:43  billditt
// Addition  of new module eth_addrcheck.v
//
// Revision 1.4  2002/01/23 10:28:16  mohor
// Link in the header changed.
//
// Revision 1.3  2001/10/19 08:43:51  mohor
// eth_timescale.v changed to timescale.v This is done because of the
// simulation of the few cores in a one joined project.
//
// Revision 1.2  2001/09/11 14:17:00  mohor
// Few little NCSIM warnings fixed.
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
// Revision 1.1  2001/06/27 21:26:19  mohor
// Initial release of the RxEthMAC module.
//
//
//
//
//

`include "timescale.v"


module eth_rxethmac (MRxClk, MRxDV, MRxD, Reset, Transmitting, MaxFL, r_IFG, HugEn, DlyCrcEn, 
                     RxData, RxValid, RxStartFrm, RxEndFrm, ByteCnt, ByteCntEq0, ByteCntGreat2, 
                     ByteCntMaxFrame, CrcError, StateIdle, StatePreamble, StateSFD, StateData,
                     MAC, r_Pro, r_Bro,r_HASH0, r_HASH1, RxAbort, AddressMiss, PassAll, ControlFrmAddressOK
                    );

parameter Tp = 1;



input         MRxClk;
input         MRxDV;
input   [3:0] MRxD;
input         Transmitting;
input         HugEn;
input         DlyCrcEn;
input  [15:0] MaxFL;
input         r_IFG;
input         Reset;
input  [47:0] MAC;     //  Station Address  
input         r_Bro;   //  broadcast disable
input         r_Pro;   //  promiscuous enable 
input [31:0]  r_HASH0; //  lower 4 bytes Hash Table
input [31:0]  r_HASH1; //  upper 4 bytes Hash Table
input         PassAll;
input         ControlFrmAddressOK;

output  [7:0] RxData;
output        RxValid;
output        RxStartFrm;
output        RxEndFrm;
output [15:0] ByteCnt;
output        ByteCntEq0;
output        ByteCntGreat2;
output        ByteCntMaxFrame;
output        CrcError;
output        StateIdle;
output        StatePreamble;
output        StateSFD;
output  [1:0] StateData;
output        RxAbort;
output        AddressMiss;

reg     [7:0] RxData;
reg           RxValid;
reg           RxStartFrm;
reg           RxEndFrm;
reg           Broadcast;
reg           Multicast;
reg     [5:0] CrcHash;
reg           CrcHashGood;
reg           DelayData;
reg     [7:0] LatchedByte;
reg     [7:0] RxData_d;
reg           RxValid_d;
reg           RxStartFrm_d;
reg           RxEndFrm_d;

wire          MRxDEqD;
wire          MRxDEq5;
wire          StateDrop;
wire          ByteCntEq1;
wire          ByteCntEq2;
wire          ByteCntEq3;
wire          ByteCntEq4;
wire          ByteCntEq5;
wire          ByteCntEq6;
wire          ByteCntEq7;
wire          ByteCntSmall7;
wire   [31:0] Crc;
wire          Enable_Crc;
wire          Initialize_Crc;
wire    [3:0] Data_Crc;
wire          GenerateRxValid;
wire          GenerateRxStartFrm;
wire          GenerateRxEndFrm;
wire          DribbleRxEndFrm;
wire    [3:0] DlyCrcCnt;
wire          IFGCounterEq24;

assign MRxDEqD = MRxD == 4'hd;
assign MRxDEq5 = MRxD == 4'h5;


// Rx State Machine module
eth_rxstatem rxstatem1 (.MRxClk(MRxClk), .Reset(Reset), .MRxDV(MRxDV), .ByteCntEq0(ByteCntEq0), 
                        .ByteCntGreat2(ByteCntGreat2), .Transmitting(Transmitting), .MRxDEq5(MRxDEq5), 
                        .MRxDEqD(MRxDEqD), .IFGCounterEq24(IFGCounterEq24), .ByteCntMaxFrame(ByteCntMaxFrame), 
                        .StateData(StateData), .StateIdle(StateIdle), .StatePreamble(StatePreamble), 
                        .StateSFD(StateSFD), .StateDrop(StateDrop)
                       );


// Rx Counters module
eth_rxcounters rxcounters1 (.MRxClk(MRxClk), .Reset(Reset), .MRxDV(MRxDV), .StateIdle(StateIdle), 
                            .StateSFD(StateSFD), .StateData(StateData), .StateDrop(StateDrop), 
                            .StatePreamble(StatePreamble), .MRxDEqD(MRxDEqD), .DlyCrcEn(DlyCrcEn), 
                            .DlyCrcCnt(DlyCrcCnt), .Transmitting(Transmitting), .MaxFL(MaxFL), .r_IFG(r_IFG), 
                            .HugEn(HugEn), .IFGCounterEq24(IFGCounterEq24), .ByteCntEq0(ByteCntEq0), 
                            .ByteCntEq1(ByteCntEq1), .ByteCntEq2(ByteCntEq2), .ByteCntEq3(ByteCntEq3), 
                            .ByteCntEq4(ByteCntEq4), .ByteCntEq5(ByteCntEq5), .ByteCntEq6(ByteCntEq6), 
                            .ByteCntEq7(ByteCntEq7), .ByteCntGreat2(ByteCntGreat2), 
                            .ByteCntSmall7(ByteCntSmall7), .ByteCntMaxFrame(ByteCntMaxFrame), 
                            .ByteCntOut(ByteCnt)
                           );

// Rx Address Check

eth_rxaddrcheck rxaddrcheck1
              (.MRxClk(MRxClk),         .Reset( Reset),             .RxData(RxData), 
               .Broadcast (Broadcast),  .r_Bro (r_Bro),             .r_Pro(r_Pro),
               .ByteCntEq6(ByteCntEq6), .ByteCntEq7(ByteCntEq7),    .ByteCntEq2(ByteCntEq2), 
               .ByteCntEq3(ByteCntEq3), .ByteCntEq4(ByteCntEq4),    .ByteCntEq5(ByteCntEq5), 
               .HASH0(r_HASH0),         .HASH1(r_HASH1),           
               .CrcHash(CrcHash),       .CrcHashGood(CrcHashGood),  .StateData(StateData),
               .Multicast(Multicast),   .MAC(MAC),                  .RxAbort(RxAbort),
               .RxEndFrm(RxEndFrm),     .AddressMiss(AddressMiss),  .PassAll(PassAll),
               .ControlFrmAddressOK(ControlFrmAddressOK)
              );


assign Enable_Crc = MRxDV & (|StateData & ~ByteCntMaxFrame);
assign Initialize_Crc = StateSFD | DlyCrcEn & (|DlyCrcCnt[3:0]) & DlyCrcCnt[3:0] < 4'h9;

assign Data_Crc[0] = MRxD[3];
assign Data_Crc[1] = MRxD[2];
assign Data_Crc[2] = MRxD[1];
assign Data_Crc[3] = MRxD[0];


// Connecting module Crc
eth_crc crcrx (.Clk(MRxClk), .Reset(Reset), .Data(Data_Crc), .Enable(Enable_Crc), .Initialize(Initialize_Crc), 
               .Crc(Crc), .CrcError(CrcError)
              );



// Latching CRC for use in the hash table

always @ (posedge MRxClk)
begin
  CrcHashGood <= #Tp StateData[0] & ByteCntEq6;
end

always @ (posedge MRxClk)
begin
  if(Reset | StateIdle)
    CrcHash[5:0] <= #Tp 6'h0;
  else
  if(StateData[0] & ByteCntEq6)
    CrcHash[5:0] <= #Tp Crc[31:26];
end


// Output byte stream
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    begin
      RxData_d[7:0]      <= #Tp 8'h0;
      DelayData          <= #Tp 1'b0;
      LatchedByte[7:0]   <= #Tp 8'h0;
      RxData[7:0]        <= #Tp 8'h0;
    end
  else
    begin
      LatchedByte[7:0]   <= #Tp {MRxD[3:0], LatchedByte[7:4]};  // Latched byte
      DelayData          <= #Tp StateData[0];

      if(GenerateRxValid)
        RxData_d[7:0] <= #Tp LatchedByte[7:0] & {8{|StateData}};  // Data goes through only in data state 
      else
      if(~DelayData)
        RxData_d[7:0] <= #Tp 8'h0;                                // Delaying data to be valid for two cycles. Zero when not active.

      RxData[7:0] <= #Tp RxData_d[7:0];                           // Output data byte
    end
end

    

always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    Broadcast <= #Tp 1'b0;
  else
    begin      
      if(StateData[0] & ~(&LatchedByte[7:0]) & ByteCntSmall7)
        Broadcast <= #Tp 1'b0;
      else
      if(StateData[0] & (&LatchedByte[7:0]) & ByteCntEq1)
        Broadcast <= #Tp 1'b1;
      else
      if(RxAbort | RxEndFrm)
        Broadcast <= #Tp 1'b0;
    end
end


always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    Multicast <= #Tp 1'b0;
  else
    begin      
      if(StateData[0] & ByteCntEq1 & LatchedByte[0])
        Multicast <= #Tp 1'b1;
      else if(RxAbort | RxEndFrm)
      Multicast <= #Tp 1'b0;
    end
end


assign GenerateRxValid = StateData[0] & (~ByteCntEq0 | DlyCrcCnt >= 4'h3);

always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    begin
      RxValid_d <= #Tp 1'b0;
      RxValid   <= #Tp 1'b0;
    end
  else
    begin
      RxValid_d <= #Tp GenerateRxValid;
      RxValid   <= #Tp RxValid_d;
    end
end


assign GenerateRxStartFrm = StateData[0] & (ByteCntEq1 & ~DlyCrcEn | DlyCrcCnt == 4'h3 & DlyCrcEn);

always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    begin
      RxStartFrm_d <= #Tp 1'b0;
      RxStartFrm   <= #Tp 1'b0;
    end
  else
    begin
      RxStartFrm_d <= #Tp GenerateRxStartFrm;
      RxStartFrm   <= #Tp RxStartFrm_d;
    end
end


assign GenerateRxEndFrm = StateData[0] & (~MRxDV & ByteCntGreat2 | ByteCntMaxFrame);
assign DribbleRxEndFrm  = StateData[1] &  ~MRxDV & ByteCntGreat2;


always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    begin
      RxEndFrm_d <= #Tp 1'b0;
      RxEndFrm   <= #Tp 1'b0;
    end
  else
    begin
      RxEndFrm_d <= #Tp GenerateRxEndFrm;
      RxEndFrm   <= #Tp RxEndFrm_d | DribbleRxEndFrm;
    end
end


endmodule
