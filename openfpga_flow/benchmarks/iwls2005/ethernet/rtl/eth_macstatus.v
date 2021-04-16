//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_macstatus.v                                             ////
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
// $Log: eth_macstatus.v,v $
// Revision 1.17  2005/03/21 20:07:18  igorm
// Some small fixes + some troubles fixed.
//
// Revision 1.16  2005/02/21 10:42:11  igorm
// Defer indication fixed.
//
// Revision 1.15  2003/01/30 13:28:19  tadejm
// Defer indication changed.
//
// Revision 1.14  2002/11/22 01:57:06  mohor
// Rx Flow control fixed. CF flag added to the RX buffer descriptor. RxAbort
// synchronized.
//
// Revision 1.13  2002/11/13 22:30:58  tadejm
// Late collision is reported only when not in the full duplex.
// Sample is taken (for status) as soon as MRxDV is not valid (regardless
// of the received byte cnt).
//
// Revision 1.12  2002/09/12 14:50:16  mohor
// CarrierSenseLost bug fixed when operating in full duplex mode.
//
// Revision 1.11  2002/09/04 18:38:03  mohor
// CarrierSenseLost status is not set when working in loopback mode.
//
// Revision 1.10  2002/07/25 18:17:46  mohor
// InvalidSymbol generation changed.
//
// Revision 1.9  2002/04/22 13:51:44  mohor
// Short frame and ReceivedLengthOK were not detected correctly.
//
// Revision 1.8  2002/02/18 10:40:17  mohor
// Small fixes.
//
// Revision 1.7  2002/02/15 17:07:39  mohor
// Status was not written correctly when frames were discarted because of
// address mismatch.
//
// Revision 1.6  2002/02/11 09:18:21  mohor
// Tx status is written back to the BD.
//
// Revision 1.5  2002/02/08 16:21:54  mohor
// Rx status is written back to the BD.
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
//
//
//
//

`include "timescale.v"


module eth_macstatus(
                      MRxClk, Reset, ReceivedLengthOK, ReceiveEnd, ReceivedPacketGood, RxCrcError, 
                      MRxErr, MRxDV, RxStateSFD, RxStateData, RxStatePreamble, RxStateIdle, Transmitting, 
                      RxByteCnt, RxByteCntEq0, RxByteCntGreat2, RxByteCntMaxFrame, 
                      InvalidSymbol, MRxD, LatchedCrcError, Collision, CollValid, RxLateCollision,
                      r_RecSmall, r_MinFL, r_MaxFL, ShortFrame, DribbleNibble, ReceivedPacketTooBig, r_HugEn,
                      LoadRxStatus, StartTxDone, StartTxAbort, RetryCnt, RetryCntLatched, MTxClk, MaxCollisionOccured, 
                      RetryLimit, LateCollision, LateCollLatched, DeferIndication, DeferLatched, RstDeferLatched, TxStartFrm,
                      StatePreamble, StateData, CarrierSense, CarrierSenseLost, TxUsedData, LatchedMRxErr, Loopback, 
                      r_FullD
                    );



parameter Tp = 1;


input         MRxClk;
input         Reset;
input         RxCrcError;
input         MRxErr;
input         MRxDV;

input         RxStateSFD;
input   [1:0] RxStateData;
input         RxStatePreamble;
input         RxStateIdle;
input         Transmitting;
input  [15:0] RxByteCnt;
input         RxByteCntEq0;
input         RxByteCntGreat2;
input         RxByteCntMaxFrame;
input   [3:0] MRxD;
input         Collision;
input   [5:0] CollValid;
input         r_RecSmall;
input  [15:0] r_MinFL;
input  [15:0] r_MaxFL;
input         r_HugEn;
input         StartTxDone;
input         StartTxAbort;
input   [3:0] RetryCnt;
input         MTxClk;
input         MaxCollisionOccured;
input         LateCollision;
input         DeferIndication;
input         TxStartFrm;
input         StatePreamble;
input   [1:0] StateData;
input         CarrierSense;
input         TxUsedData;
input         Loopback;
input         r_FullD;


output        ReceivedLengthOK;
output        ReceiveEnd;
output        ReceivedPacketGood;
output        InvalidSymbol;
output        LatchedCrcError;
output        RxLateCollision;
output        ShortFrame;
output        DribbleNibble;
output        ReceivedPacketTooBig;
output        LoadRxStatus;
output  [3:0] RetryCntLatched;
output        RetryLimit;
output        LateCollLatched;
output        DeferLatched;
input         RstDeferLatched;
output        CarrierSenseLost;
output        LatchedMRxErr;


reg           ReceiveEnd;

reg           LatchedCrcError;
reg           LatchedMRxErr;
reg           LoadRxStatus;
reg           InvalidSymbol;
reg     [3:0] RetryCntLatched;
reg           RetryLimit;
reg           LateCollLatched;
reg           DeferLatched;
reg           CarrierSenseLost;

wire          TakeSample;
wire          SetInvalidSymbol; // Invalid symbol was received during reception in 100Mbps 

// Crc error
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    LatchedCrcError <=#Tp 1'b0;
  else
  if(RxStateSFD)
    LatchedCrcError <=#Tp 1'b0;
  else
  if(RxStateData[0])
    LatchedCrcError <=#Tp RxCrcError & ~RxByteCntEq0;
end


// LatchedMRxErr
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    LatchedMRxErr <=#Tp 1'b0;
  else
  if(MRxErr & MRxDV & (RxStatePreamble | RxStateSFD | (|RxStateData) | RxStateIdle & ~Transmitting))
    LatchedMRxErr <=#Tp 1'b1;
  else
    LatchedMRxErr <=#Tp 1'b0;
end


// ReceivedPacketGood
assign ReceivedPacketGood = ~LatchedCrcError;


// ReceivedLengthOK
assign ReceivedLengthOK = RxByteCnt[15:0] >= r_MinFL[15:0] & RxByteCnt[15:0] <= r_MaxFL[15:0];





// Time to take a sample
//assign TakeSample = |RxStateData     & ~MRxDV & RxByteCntGreat2  |
assign TakeSample = (|RxStateData)   & (~MRxDV)                    |
                      RxStateData[0] &   MRxDV & RxByteCntMaxFrame;


// LoadRxStatus
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    LoadRxStatus <=#Tp 1'b0;
  else
    LoadRxStatus <=#Tp TakeSample;
end



// ReceiveEnd
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    ReceiveEnd  <=#Tp 1'b0;
  else
    ReceiveEnd  <=#Tp LoadRxStatus;                     
end


// Invalid Symbol received during 100Mbps mode
assign SetInvalidSymbol = MRxDV & MRxErr & MRxD[3:0] == 4'he;


// InvalidSymbol
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    InvalidSymbol <=#Tp 1'b0;
  else
  if(LoadRxStatus & ~SetInvalidSymbol)
    InvalidSymbol <=#Tp 1'b0;
  else
  if(SetInvalidSymbol)
    InvalidSymbol <=#Tp 1'b1;
end


// Late Collision

reg RxLateCollision;
reg RxColWindow;
// Collision Window
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    RxLateCollision <=#Tp 1'b0;
  else
  if(LoadRxStatus)
    RxLateCollision <=#Tp 1'b0;
  else
  if(Collision & (~r_FullD) & (~RxColWindow | r_RecSmall))
    RxLateCollision <=#Tp 1'b1;
end

// Collision Window
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    RxColWindow <=#Tp 1'b1;
  else
  if(~Collision & RxByteCnt[5:0] == CollValid[5:0] & RxStateData[1])
    RxColWindow <=#Tp 1'b0;
  else
  if(RxStateIdle)
    RxColWindow <=#Tp 1'b1;
end


// ShortFrame
reg ShortFrame;
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    ShortFrame <=#Tp 1'b0;
  else
  if(LoadRxStatus)
    ShortFrame <=#Tp 1'b0;
  else
  if(TakeSample)
    ShortFrame <=#Tp RxByteCnt[15:0] < r_MinFL[15:0];
end


// DribbleNibble
reg DribbleNibble;
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    DribbleNibble <=#Tp 1'b0;
  else
  if(RxStateSFD)
    DribbleNibble <=#Tp 1'b0;
  else
  if(~MRxDV & RxStateData[1])
    DribbleNibble <=#Tp 1'b1;
end


reg ReceivedPacketTooBig;
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    ReceivedPacketTooBig <=#Tp 1'b0;
  else
  if(LoadRxStatus)
    ReceivedPacketTooBig <=#Tp 1'b0;
  else
  if(TakeSample)
    ReceivedPacketTooBig <=#Tp ~r_HugEn & RxByteCnt[15:0] > r_MaxFL[15:0];
end



// Latched Retry counter for tx status
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    RetryCntLatched <=#Tp 4'h0;
  else
  if(StartTxDone | StartTxAbort)
    RetryCntLatched <=#Tp RetryCnt;
end


// Latched Retransmission limit
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    RetryLimit <=#Tp 1'h0;
  else
  if(StartTxDone | StartTxAbort)
    RetryLimit <=#Tp MaxCollisionOccured;
end


// Latched Late Collision
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    LateCollLatched <=#Tp 1'b0;
  else
  if(StartTxDone | StartTxAbort)
    LateCollLatched <=#Tp LateCollision;
end



// Latched Defer state
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    DeferLatched <=#Tp 1'b0;
  else
  if(DeferIndication)
    DeferLatched <=#Tp 1'b1;
  else
  if(RstDeferLatched)
    DeferLatched <=#Tp 1'b0;
end


// CarrierSenseLost
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    CarrierSenseLost <=#Tp 1'b0;
  else
  if((StatePreamble | (|StateData)) & ~CarrierSense & ~Loopback & ~Collision & ~r_FullD)
    CarrierSenseLost <=#Tp 1'b1;
  else
  if(TxStartFrm)
    CarrierSenseLost <=#Tp 1'b0;
end


endmodule
